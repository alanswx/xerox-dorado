/*
 * dorado_machine — runnable Dorado orchestration (see machine.h).
 *
 * This is the Stage-2 Alto-style Ethernet software-boot path extracted
 * from test_cpu.c's probe_full_boot_with_bootstrap, with the trace /
 * probe / assertion scaffolding removed. The boot sequence is:
 *
 *   1. BaseBoard cold-boots from its EPROMs; three boot-button presses
 *      drive CoolBoot -> RebootDorado -> LoadDoradoCode, which jams
 *      Boot0 into IM and releases the Dorado.
 *   2. When the Dorado spins at Boot0Go (0o7740) after the BB has begun
 *      streaming Boot1, we swap in Bootstrap.MB, then (at 0o7717) the
 *      canonical Initial+kernel+memMisc+IfuComplex microcode.
 *   3. Initial falls through DiskHardMicrocodeBoot to EtherMicrocodeBoot;
 *      the in-process fake Pup server serves the boot-0110 world; Initial
 *      checksums it, LoadRam's it, and the Alto/Mesa emulator world runs.
 *   4. With alto_ether_boot set, the machine plays boot server: it
 *      broadcasts breath-of-life / time packets and serves NETEXEC.BOOT
 *      over EFTP, and injects the Ethernet + display task wakeups the
 *      loaded world needs.
 */

#include "machine.h"

#include "baseboard.h"
#include "cpu.h"
#include "disk.h"
#include "ethernet.h"
#include "fastio.h"
#include "io.h"
#include "mb.h"
#include "memory.h"
#include "microcode.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Default firmware/microcode locations, relative to the dorado/ dir. */
#define DEF_BB_ROM    "../chm/dorado/doradobaserom.mb!13"
#define DEF_BOOTSTRAP "../chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb"
#define DEF_INITIAL   "../chm/dorado/expanded/bootstrap.dm!20_/Initial.mb"
#define DEF_KERNEL    "../chm/dorado/expanded/kernel.dm!38_/kernel.mb"
#define DEF_MEMMISC   "../chm/dorado/expanded/memMisc.dm!11_/memMisc.mb"
#define DEF_IFU       "../chm/dorado/expanded/Ifu.dm!51_/IfuComplex.mb"
#define DEF_ETH_110   "../chm/microcode/AltoMesaDorado.eb!1"
#define DEF_EFTP      "../chm/bootfiles/NETEXEC.BOOT!8"

/* Boot-button press schedule (BaseBoard cycles). */
#define T_PRESS1_DOWN 1000000u
#define T_PRESS1_UP   1400000u
#define T_PRESS2_DOWN 2000000u
#define T_PRESS2_UP   2400000u
#define T_PRESS3_DOWN 3000000u
#define T_PRESS3_UP   3400000u

struct dorado_machine {
    /* mb_file sources must outlive the microcode that references their
     * symbol tables. */
    mb_file bs_mb;
    mb_file initial_mb, kernel_mb, memmisc_mb, ifu_mb;

    dorado_microcode mc;       /* the live control store           */
    dorado_microcode bs_mc;    /* Bootstrap.MB (BB-streamed loader) */
    dorado_microcode init_mc;  /* canonical Initial+kernel+...      */

    dorado_baseboard bb;
    dorado_cpu cpu;
    dorado_memory mem;
    dorado_io io;
    dorado_display display;
    dorado_disk_controller disk;
    dorado_ethernet ethernet;
    dorado_disk_pack disk_pack;
    dorado_fastio_router fastio;

    int disk_attached;
    int alto_ether_boot;
    int alto_ether_quote;

    /* Boot state machine. */
    int      pressed;
    int      swapped;
    int      bb_strobes_pre_swap;
    uint16_t pre_swap_cpreg;
    int      initial_canonical_loaded;
    int      initial_substituted;
    int      checksum_and_load_seen;
    int      identity_map_shimmed;
    uint64_t ether_loaded_world_cycle;
    uint64_t next_bol_cycle;
    uint64_t next_display_scanline_cycle;
};

static const uint8_t standard_alufm[ALUFM_SIZE] = {
    025, 000, 014, 054, 062, 022, 035, 027,
    023, 031, 040, 036, 013, 033, 001, 006,
};

static void restore_standard_alufm(dorado_microcode *mc)
{
    for (int i = 0; i < ALUFM_SIZE; i++) {
        mc->alufm[i] = standard_alufm[i];
        mc->alufm_present[i] = 1;
    }
}

/* Coherently write one word at a VA: through the map to physical
 * storage and into any matching cache line. Used to seed the Alto
 * keyboard words the loaded world polls. */
static void machine_store_va(dorado_memory *mem, uint32_t va, uint16_t value)
{
    if (!mem || !mem->storage) return;
    if ((size_t)va < mem->storage_words) mem->storage[va] = value;

    uint32_t idx = dorado_map_index(va);
    const dorado_map_entry *e = dorado_map_get(mem, idx);
    size_t phys = (size_t)e->rp * DM_PAGE_SIZE + (va & (DM_PAGE_SIZE - 1));
    if (phys < mem->storage_words) mem->storage[phys] = value;

    uint32_t row = (va >> 4) & DM_CACHE_ROW_MASK;
    uint32_t tag = va >> 10;
    uint32_t off = va & DM_CACHE_LINE_MASK;
    for (int way = 0; way < DM_CACHE_WAYS; way++) {
        dorado_cache_line *line = &mem->cache[row].ways[way];
        if (line->valid && line->tag == tag) line->data[off] = value;
    }
}

/* Seed the four Alto keyboard words (base+0177034..7) at each plausible
 * base (absolute, IOBR, MDS) so the loaded world's boot-key poll sees
 * them regardless of which space it reads. */
static void machine_seed_keyboard(dorado_memory *mem, const uint16_t w[4])
{
    if (!mem || !mem->storage) return;
    uint32_t bases[] = { 0, dorado_br_get(mem, 031), dorado_br_get(mem, 036) };
    for (size_t b = 0; b < sizeof bases / sizeof bases[0]; b++) {
        for (uint32_t i = 0; i < 4; i++) {
            uint32_t va = (bases[b] + 0177034u + i) & 0x0FFFFFFFu;
            machine_store_va(mem, va, w[i]);
        }
    }
}

/* Identity-map the installed storage pages. Bring-up shim: Initial's
 * PresetMap should leave installed storage identity-mapped before
 * BootEmulator clears the first 64K and builds the Ethernet request at
 * VM 177400, but the Config'/PresetMap path still reaches the vacant-map
 * case after the NoStorage bypass. Install the map shape directly until
 * that gap is closed. */
static void machine_identity_map_storage(dorado_memory *mem)
{
    uint32_t pages = (uint32_t)(mem->storage_words / DM_PAGE_SIZE);
    if (pages > DM_MAP_ENTRIES) pages = DM_MAP_ENTRIES;
    for (uint32_t pg = 0; pg < pages; pg++) {
        dorado_map_set(mem, pg, (uint16_t)pg, /*wp=*/0, /*dirty=*/0);
    }
}

void dorado_machine_config_default(dorado_machine_config *cfg)
{
    if (!cfg) return;
    memset(cfg, 0, sizeof *cfg);
    cfg->bb_rom       = DEF_BB_ROM;
    cfg->bootstrap_mb = DEF_BOOTSTRAP;
    cfg->initial_mb   = DEF_INITIAL;
    cfg->kernel_mb    = DEF_KERNEL;
    cfg->memmisc_mb   = DEF_MEMMISC;
    cfg->ifu_mb       = DEF_IFU;
    cfg->eth_boot_110 = DEF_ETH_110;
    cfg->eftp_boot    = DEF_EFTP;
    cfg->alto_ether_boot = 1;
    cfg->alto_ether_quote = 0;
    cfg->no_disk = 1;
    cfg->storage_modules = 1;
}

static const char *pick(const char *v, const char *def)
{
    return (v && *v) ? v : def;
}

dorado_machine *dorado_machine_create(const dorado_machine_config *user_cfg)
{
    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    if (user_cfg) {
        cfg.bb_rom       = pick(user_cfg->bb_rom,       cfg.bb_rom);
        cfg.bootstrap_mb = pick(user_cfg->bootstrap_mb, cfg.bootstrap_mb);
        cfg.initial_mb   = pick(user_cfg->initial_mb,   cfg.initial_mb);
        cfg.kernel_mb    = pick(user_cfg->kernel_mb,    cfg.kernel_mb);
        cfg.memmisc_mb   = pick(user_cfg->memmisc_mb,   cfg.memmisc_mb);
        cfg.ifu_mb       = pick(user_cfg->ifu_mb,       cfg.ifu_mb);
        cfg.eth_boot_110 = pick(user_cfg->eth_boot_110, cfg.eth_boot_110);
        cfg.eftp_boot    = pick(user_cfg->eftp_boot,    cfg.eftp_boot);
        cfg.alto_ether_boot  = user_cfg->alto_ether_boot;
        cfg.alto_ether_quote = user_cfg->alto_ether_quote;
        cfg.no_disk          = user_cfg->no_disk;
        if (user_cfg->storage_modules)
            cfg.storage_modules = user_cfg->storage_modules;
    }
    if (cfg.storage_modules < 1 || cfg.storage_modules > 4)
        cfg.storage_modules = 1;

    dorado_machine *m = calloc(1, sizeof *m);
    if (!m) return NULL;
    m->alto_ether_boot  = cfg.alto_ether_boot;
    m->alto_ether_quote = cfg.alto_ether_quote;
    m->pre_swap_cpreg   = 0;

    /* Bootstrap.MB -> bs_mc (the loader the BB streams in). */
    mb_init(&m->bs_mb);
    if (mb_load(&m->bs_mb, cfg.bootstrap_mb) != MB_OK ||
        dorado_microcode_load(&m->bs_mb, &m->bs_mc) != DM_OK) {
        fprintf(stderr, "dorado: cannot load Bootstrap.MB (%s)\n",
                cfg.bootstrap_mb);
        free(m);
        return NULL;
    }

    /* Live control store starts with the canonical ALUFM convention so
     * Boot0 has working ALU semantics from the first instruction. */
    memset(&m->mc, 0, sizeof m->mc);
    restore_standard_alufm(&m->mc);

    /* Canonical Initial = kernel + memMisc + IfuComplex + Initial,
     * layered (kernel first so Initial overrides conflicts). */
    mb_init(&m->kernel_mb);
    mb_init(&m->memmisc_mb);
    mb_init(&m->ifu_mb);
    mb_init(&m->initial_mb);
    if (mb_load(&m->kernel_mb, cfg.kernel_mb) == MB_OK)
        dorado_microcode_load(&m->kernel_mb, &m->init_mc);
    if (mb_load(&m->memmisc_mb, cfg.memmisc_mb) == MB_OK)
        dorado_microcode_layer_load(&m->memmisc_mb, &m->init_mc);
    if (mb_load(&m->ifu_mb, cfg.ifu_mb) == MB_OK)
        dorado_microcode_layer_load(&m->ifu_mb, &m->init_mc);
    if (mb_load(&m->initial_mb, cfg.initial_mb) == MB_OK &&
        dorado_microcode_layer_load(&m->initial_mb, &m->init_mc) == DM_OK) {
        m->initial_canonical_loaded = 1;
    }
    if (!m->initial_canonical_loaded) {
        fprintf(stderr, "dorado: cannot load canonical Initial microcode\n");
        free(m);
        return NULL;
    }

    /* BaseBoard. */
    baseboard_init(&m->bb);
    if (baseboard_load_rom(&m->bb, cfg.bb_rom) != 0) {
        fprintf(stderr, "dorado: cannot load BaseBoard ROM (%s)\n", cfg.bb_rom);
        free(m);
        return NULL;
    }
    baseboard_reset(&m->bb);

    /* Microengine. */
    dorado_cpu_init(&m->cpu, &m->mc, 0);
    m->cpu.baseboard = &m->bb;
    m->cpu.baseboard_cycles_per_uop = 1;

    /* Memory + I/O + devices. Resize storage to the requested module
     * count, independent of the DORADO_STORAGE_MODULES env var that
     * dorado_memory_init() consults — a library shouldn't depend on
     * process environment for correctness. */
    if (dorado_memory_init(&m->mem) != 0) {
        fprintf(stderr, "dorado: memory init failed\n");
        free(m);
        return NULL;
    }
    {
        size_t want = (size_t)DM_STORAGE_MODULE_WORDS *
                      (size_t)cfg.storage_modules;
        if (m->mem.storage_words != want) {
            uint16_t *ns = calloc(want, sizeof(uint16_t));
            if (!ns) {
                fprintf(stderr, "dorado: storage alloc failed\n");
                free(m->mem.storage);
                free(m);
                return NULL;
            }
            free(m->mem.storage);
            m->mem.storage = ns;
            m->mem.storage_words = want;
        }
    }
    dorado_io_init(&m->io);
    dorado_display_init(&m->display);
    dorado_disk_controller_init(&m->disk);
    dorado_ethernet_init(&m->ethernet);
    dorado_ethernet_set_boot_file(&m->ethernet, 0110, cfg.eth_boot_110);
    dorado_ethernet_set_eftp_boot_file(&m->ethernet, cfg.eftp_boot);

    dorado_display_attach_to_io(&m->display, &m->io);
    dorado_disk_controller_attach_to_io(&m->disk, &m->io);
    dorado_ethernet_attach_to_io(&m->ethernet, &m->io);
    dorado_fastio_router_init(&m->fastio, &m->display, &m->disk);
    m->mem.fast_io_cb = dorado_fastio_dispatch;
    m->mem.fast_io_ctx = &m->fastio;
    m->cpu.mem = &m->mem;
    m->cpu.io = &m->io;

    return m;
}

void dorado_machine_destroy(dorado_machine *m)
{
    if (!m) return;
    dorado_ethernet_free(&m->ethernet);
    mb_free(&m->bs_mb);
    mb_free(&m->kernel_mb);
    mb_free(&m->memmisc_mb);
    mb_free(&m->ifu_mb);
    mb_free(&m->initial_mb);
    free(m);
}

uint64_t dorado_machine_run_until(dorado_machine *m, uint64_t until_cycle)
{
    if (!m) return 0;
    dorado_baseboard *bb = &m->bb;
    dorado_cpu *cpu = &m->cpu;
    dorado_display *disp = &m->display;
    dorado_ethernet *eth = &m->ethernet;

    while (bb->cycles < until_cycle && !cpu->halted) {
        /* Boot-button schedule (three presses). */
        if (!m->pressed && bb->cycles >= T_PRESS1_DOWN &&
            bb->cycles < T_PRESS1_UP) {
            baseboard_boot_button(bb, 1); m->pressed = 1;
        } else if (m->pressed && bb->cycles >= T_PRESS1_UP &&
                   bb->cycles < T_PRESS2_DOWN) {
            baseboard_boot_button(bb, 0); m->pressed = 0;
        } else if (!m->pressed && bb->cycles >= T_PRESS2_DOWN &&
                   bb->cycles < T_PRESS2_UP) {
            baseboard_boot_button(bb, 1); m->pressed = 1;
        } else if (m->pressed && bb->cycles >= T_PRESS2_UP &&
                   bb->cycles < T_PRESS3_DOWN) {
            baseboard_boot_button(bb, 0); m->pressed = 0;
        } else if (!m->pressed && bb->cycles >= T_PRESS3_DOWN &&
                   bb->cycles < T_PRESS3_UP) {
            baseboard_boot_button(bb, 1); m->pressed = 1;
        } else if (m->pressed && bb->cycles >= T_PRESS3_UP) {
            baseboard_boot_button(bb, 0); m->pressed = 0;
        }

        int will_inject = bb->dorado_ss_pending && bb->dorado_mir_loaded;
        int will_hold   = !will_inject && !bb->dorado_running;
        int is_imfetch  = !will_inject && !will_hold;

        /* Count BB CPReg strobes before the Bootstrap swap so we wait
         * until the Boot1 stream is in flight. */
        if (!m->swapped && bb->cpreg_to_dorado != m->pre_swap_cpreg) {
            m->bb_strobes_pre_swap++;
            m->pre_swap_cpreg = bb->cpreg_to_dorado;
        }

        /* Swap Bootstrap.MB in once the Dorado is spinning at Boot0Go
         * (0o7740) and the BB has begun streaming Boot1. */
        if (!m->swapped && is_imfetch && cpu->real_PC == 07740 &&
            m->bb_strobes_pre_swap >= 16) {
            for (int a = 0; a < 4096; a++) {
                if (m->bs_mc.im_present[a]) {
                    m->mc.im[a]         = m->bs_mc.im[a];
                    m->mc.im_present[a] = 1;
                }
            }
            for (int a = 0; a < 16; a++) {
                if (m->bs_mc.alufm_present[a]) {
                    m->mc.alufm[a]         = m->bs_mc.alufm[a];
                    m->mc.alufm_present[a] = 1;
                }
            }
            m->swapped = 1;
            if (getenv("DORADO_MACHINE_TRACE"))
                fprintf(stderr, "[machine] Bootstrap swap @cyc=%llu\n",
                        (unsigned long long)bb->cycles);
        }

        /* Replace the (different-build / stream-corrupted) IM with the
         * canonical Initial microcode once Bootstrap reaches 0o7717,
         * keeping Bootstrap's own 0o7700-0o7777 page. */
        if (m->initial_canonical_loaded && !m->initial_substituted &&
            m->swapped && is_imfetch && cpu->real_PC == 07717) {
            for (int a = 0; a < 4096; a++) {
                if (a >= 07700 && a < 010000) continue;
                if (m->init_mc.im_present[a]) {
                    m->mc.im[a]         = m->init_mc.im[a];
                    m->mc.im_present[a] = 1;
                }
            }
            for (int a = 0; a < 1024; a++) {
                if (m->init_mc.ifum_present[a]) {
                    m->mc.ifum_lo[a]      = m->init_mc.ifum_lo[a];
                    m->mc.ifum_hi[a]      = m->init_mc.ifum_hi[a];
                    m->mc.ifum_present[a] = 1;
                }
            }
            for (int a = 0; a < 16; a++) {
                if (m->init_mc.alufm_present[a]) {
                    m->mc.alufm[a]         = m->init_mc.alufm[a];
                    m->mc.alufm_present[a] = 1;
                }
            }
            m->initial_substituted = 1;
            if (getenv("DORADO_MACHINE_TRACE"))
                fprintf(stderr, "[machine] Initial substituted @cyc=%llu\n",
                        (unsigned long long)bb->cycles);
            /* Drive the Stage-2 boot decision toward NetExec via the
             * DDC keyboard (BS down selects Ethernet software boot). */
            if (m->alto_ether_boot) {
                dorado_display_keyboard_set_key(disp, DORADO_KEY_BS, 1);
                dorado_display_keyboard_set_key(disp, DORADO_KEY_QUOTE,
                                                m->alto_ether_quote);
            }
        }

        uint16_t pre_pc = cpu->real_PC;

        /* Keep Initial's boot keys "up" until CheckChecksumAndLoad so it
         * falls through DiskHardMicrocodeBoot to EtherMicrocodeBoot.
         * (The 7-wire terminal back-channel is not modeled.) */
        if (m->initial_substituted && is_imfetch &&
            !m->checksum_and_load_seen &&
            (pre_pc == 06417 || (pre_pc >= 06407 && pre_pc <= 06431))) {
            cpu->RM[0x1A] = 0xFFFFu;
            cpu->RM[0x1B] = 0xFFFFu;
            cpu->RM[0x1C] = 0xFFFFu;
            cpu->RM[0x1D] = 0xFFFFu;
        }

        /* Identity-map installed storage before BootEmulator builds the
         * Ethernet request (bring-up shim for the PresetMap gap). */
        if (m->initial_substituted && is_imfetch &&
            !m->identity_map_shimmed && pre_pc == 06222) {
            machine_identity_map_storage(&m->mem);
            m->identity_map_shimmed = 1;
            if (getenv("DORADO_MACHINE_TRACE"))
                fprintf(stderr, "[machine] identity-map @cyc=%llu\n",
                        (unsigned long long)bb->cycles);
        }

        /* CheckChecksumAndLoad has started. */
        if (m->initial_substituted && is_imfetch &&
            !m->checksum_and_load_seen && pre_pc == 06460) {
            m->checksum_and_load_seen = 1;
            if (getenv("DORADO_MACHINE_TRACE"))
                fprintf(stderr, "[machine] checksum&load @cyc=%llu\n",
                        (unsigned long long)bb->cycles);
        }

        /* Detect the LoadRam handoff: Initial has replaced the control
         * store and branched into the loaded Alto/Mesa world (running
         * code outside Initial's own region that differs from Initial). */
        if (m->checksum_and_load_seen && m->ether_loaded_world_cycle == 0 &&
            is_imfetch && pre_pc < IM_SIZE && m->mc.im_present[pre_pc] &&
            !(pre_pc >= 06000 && pre_pc < 07700)) {
            restore_standard_alufm(&m->mc);
            m->ether_loaded_world_cycle = bb->cycles;
            if (getenv("DORADO_MACHINE_TRACE"))
                fprintf(stderr, "[machine] world loaded @cyc=%llu pc=0o%o\n",
                        (unsigned long long)bb->cycles, pre_pc);
        }

        /* Seed BS-down into the Alto keyboard words so the loaded world
         * selects the Ethernet software boot (AEm0.mc branches to EBoot
         * -> Mayday -> NetExec). The 7-wire DDC keyboard back-channel is
         * not modeled, so write the polled words directly. */
        if (m->alto_ether_boot && m->ether_loaded_world_cycle &&
            !cpu->ifu_active) {
            uint16_t w[4] = {
                0xFFFEu,                                  /* BS down      */
                m->alto_ether_quote ? 0xFFF7u : 0xFFFFu,  /* quote        */
                0xFFFFu, 0xFFFFu,
            };
            dorado_display_keyboard_set_key(disp, DORADO_KEY_BS, 1);
            dorado_display_keyboard_set_key(disp, DORADO_KEY_QUOTE,
                                            m->alto_ether_quote);
            machine_seed_keyboard(&m->mem, w);
        }

        if (dorado_cpu_step(cpu)) break;

        if (m->initial_substituted) {
            /* Bring-up guard: clear the EOT load-overflow retry mask
             * Initial's `ELoad_ A0` should have zeroed, until the
             * remaining RM/RBase init issue is fixed. */
            if (eth->tx_on && eth->requests_seen == 0 &&
                eth->tx_count == 0 && eth->data_writes == 0 &&
                cpu->RM[0x19] != 0) {
                cpu->RM[0x19] = 0;
            }

            /* Stage-2 boot server: broadcast breath-of-life / time
             * packets so AEmu's EBoot receives one and the loaded Pup
             * package's unbounded raw-receive completes. */
            if (m->alto_ether_boot && m->ether_loaded_world_cycle &&
                bb->cycles >= m->next_bol_cycle) {
                int sent = (eth->eftp_max_seq > 0)
                    ? dorado_ethernet_time_broadcast(eth)
                    : dorado_ethernet_breath_of_life(eth);
                uint64_t interval = sent ? 2000000u : 100000u;
                if (sent && eth->time_bcasts > 3) interval = 50000000u;
                m->next_bol_cycle = bb->cycles + interval;
            }

            uint16_t eth_mask = dorado_ethernet_wakeup_mask(eth);
            for (int task = 0; task < 16; task++) {
                if (eth_mask & (uint16_t)(1u << task)) {
                    dorado_cpu_wakeup(cpu, task);
                }
            }

            /* Display scan-line wakeups (DHT/AHT) and DWT word-task
             * wakeups, every ~1000 cycles. */
            if (bb->cycles >= m->next_display_scanline_cycle) {
                uint16_t mask = dorado_display_scanline_wakeup_mask(disp);
                for (int task = 0; task < 16; task++) {
                    if (mask & (uint16_t)(1u << task)) {
                        if (cpu->task_tpc[task] != 0177777)
                            dorado_cpu_wakeup(cpu, task);
                    }
                }
                m->next_display_scanline_cycle = bb->cycles + 1000;
            }
            int dwt_subtask = 0;
            if (dorado_display_dwt_wakeup(disp, &dwt_subtask)) {
                int word_task = disp->terminal_task == DORADO_DISPLAY_TASK_AHT
                                    ? DORADO_DISPLAY_TASK_AWT
                                    : DORADO_DISPLAY_TASK_DWT;
                if (cpu->task_tpc[word_task] != 0177777) {
                    dorado_cpu_set_subtask(cpu, word_task,
                                           (uint8_t)dwt_subtask);
                    dorado_cpu_wakeup(cpu, word_task);
                }
            }
        }
    }
    return bb->cycles;
}

uint64_t dorado_machine_cycles(const dorado_machine *m)
{
    return m ? m->bb.cycles : 0;
}

int dorado_machine_booted(const dorado_machine *m)
{
    return m && m->ether_loaded_world_cycle != 0;
}

dorado_display *dorado_machine_display(dorado_machine *m)
{
    return m ? &m->display : NULL;
}

void dorado_machine_debug(dorado_machine *m)
{
    if (!m) return;
    dorado_ethernet *e = &m->ethernet;
    uint32_t mds = dorado_br_get(&m->mem, 036);
    uint16_t dastart = dorado_visible_word_at_va(&m->mem, mds + 0420u);
    fprintf(stderr,
            "[machine] booted=%d cyc=%llu | eth: requests=%llu replies=%llu "
            "eftp_req=%llu eftp_repl=%llu max_seq=%u bol=%llu time=%llu | "
            "DASTART=%06o\n",
            dorado_machine_booted(m),
            (unsigned long long)m->bb.cycles,
            (unsigned long long)e->requests_seen,
            (unsigned long long)e->replies_queued,
            (unsigned long long)e->eftp_requests_seen,
            (unsigned long long)e->eftp_replies_queued,
            e->eftp_max_seq,
            (unsigned long long)e->bol_queued,
            (unsigned long long)e->time_bcasts,
            dastart);
}

int dorado_machine_render_display_list(dorado_machine *m)
{
    if (!m) return 0;
    dorado_memory *mem = &m->mem;
    dorado_display *disp = &m->display;
    uint32_t dmds = dorado_br_get(mem, 036);
    uint32_t dl = dorado_visible_word_at_va(mem, dmds + 0420u);
    int pixels = 0, y = 0;
    for (int g = 0; g < 64 && dl > 1u && y < DORADO_DISPLAY_H; g++) {
        uint16_t c   = dorado_visible_word_at_va(mem, dmds + dl + 1u);
        uint16_t sa  = dorado_visible_word_at_va(mem, dmds + dl + 2u);
        uint16_t slc = dorado_visible_word_at_va(mem, dmds + dl + 3u);
        int htab  = (c >> 8) & 077;
        int nwrds = c & 0377;
        int inv   = (c >> 14) & 1;
        for (int s = 0; s < (int)slc * 2 && y < DORADO_DISPLAY_H; s++, y++) {
            int row = s / 2;
            for (int wi = 0; wi < nwrds; wi++) {
                uint16_t bits = dorado_visible_word_at_va(
                    mem, dmds + sa + (uint32_t)(row * nwrds + wi));
                for (int b = 0; b < 16; b++) {
                    int pix = (bits >> (15 - b)) & 1;
                    if (inv) pix ^= 1;
                    int x = (htab + wi) * 16 + b;
                    if (x < DORADO_DISPLAY_W) {
                        dorado_display_set_pixel(disp, x, y, pix);
                        if (pix) pixels++;
                    }
                }
            }
        }
        dl = dorado_visible_word_at_va(mem, dmds + dl);
    }
    return pixels;
}
