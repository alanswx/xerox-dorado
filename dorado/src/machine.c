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
    uint16_t boot_file_number;

    /* Boot-key chord held down through Stage-2 boot selection (see
     * dorado_machine_config.boot_keys). Resolved at create time. */
    dorado_display_key boot_chord[8];
    int      boot_chord_count;

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
    int      keys_live;        /* 1 once boot selection is done and the
                                * frontend's live keys drive the world */
    int      divprot_set;      /* 1 once the divide-vector guard is armed */
    int      mouse_present;    /* 1 once the frontend has set the mouse */
    int      mouse_x, mouse_y; /* absolute mouse position (Alto coords) */
    int      mouse_buttons;    /* DORADO_MOUSE_* bitmask                */

    uint32_t pchist[4096];
    uint16_t initseq[600];     /* first task-0 PCs after world-load */
    int      initseq_n;

    /* Pilot germ plant (Route B). When germ_path is set, the germ file
     * image is loaded here at create and fed to the real PilotBoot disk
     * read passes the first time the Cedar germ-boot disk-transfer spin
     * is reached. */
    uint16_t germ_words[16384]; /* Dorado.germ!4 = 32 pages; !6/Dorado10 larger */
    int      germ_word_count;
    int      germ_passes;      /* # of DiskBootSoft IOCB passes completed */
    int      germ_descriptor_done; /* pass 1 (descriptor) completed       */
    int      germ_label_done;      /* pass 2 (label) completed            */
    int      germ_data_done;       /* pass 3 (germ file) completed        */
};

/* Pilot germ resident VM base (Dorado.loadmap GERM FILE MAP: file page
 * W -> VM word 0o17401000 + W, contiguous, no leader page). This is the
 * FINAL resident location, produced by PilotBoot.GERMREMAP. The microcode
 * first reads the germ into the low-64K buffer at BootDataPtr=baseGerm
 * (0o1000), validates the PV descriptor, then GERMREMAP relocates it. */
#define GERM_VM_BASE 017401000u

/* PilotBoot / DiskBootSoft / PilotDiskDefs constants (octal).
 * IOCB is built at (R400)+31 = 0o431 with MemBase=IOBR (base 0), so the
 * absolute VA of each field == 0o431 + field offset from PilotDiskDefs.mc. */
#define IOCB_BASE_VA      0431u
#define IOCB_SEAL_VA      0432u   /* IOCB.seal       (offset 1)  */
#define IOCB_PAGECOUNT_VA 0434u   /* IOCB.pageCount  (offset 3)  */
#define IOCB_COMMAND_VA   0435u   /* IOCB.command    (offset 4)  */
#define IOCB_LABELSTAT_VA 0453u   /* IOCB.labelStatus(offset 22) */
#define IOCB_DATAPTR_VA   0454u   /* IOCB.dataPtr lo (offset 23) */

#define IOCB_SEAL_VALUE   0125377u /* IOCBSealValue (PilotDiskDefs.mc)    */

/* DiskBootSoft.mc disk commands distinguishing the three read passes. */
#define DISK_CMD_DESCRIPTOR 0274u   /* [check,read,read], descriptor->page 0 */
#define DISK_CMD_LABEL      0260u   /* [check,read,none], first-page label    */
#define DISK_CMD_GERMDATA   0100254u/* incrementDataPtr|[check,check,read]    */

/* PilotBoot.mc: baseGerm = BootSwap.countSkip*wordsPerPage = 0o1000.
 * GermBoot sets BootDataPtr_ baseGerm, so pass 3 reads the germ into the
 * low-64K buffer starting here. */
#define GERM_LOW_BUFFER   01000u

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

/* Drive the boot-key chord onto the DDC keyboard: clear all keys, then
 * hold each chord key down. Returns the four resulting active-low Alto
 * keyboard words via w[] so the caller can also seed them into the cells
 * the loaded world polls (the DDC keyboard back-channel is not modeled;
 * gap E2). With the default chord {BS} (+ Quote) this reproduces the
 * historic forced seeding byte-for-byte. */
static void machine_apply_boot_chord(dorado_display *disp,
                                     const dorado_display_key *keys, int n,
                                     uint16_t w[4])
{
    dorado_display_keyboard_all_up(disp);
    for (int i = 0; i < n; i++)
        dorado_display_keyboard_set_key(disp, keys[i], 1);
    for (int i = 0; i < 4; i++)
        w[i] = dorado_display_keyboard_word(disp, i);
}

/* Seed the Alto mouse cells: MOUSEX 0o424 / MOUSEY 0o425 (absolute
 * position) and the UTILIN button word 0o177030..0o177033 (active-low
 * ~buttons, matching ContrAlto's ~(buttons|keyset)). Written at each
 * plausible base like the keyboard words. */
static void machine_seed_mouse(dorado_memory *mem, int x, int y, int buttons)
{
    if (!mem || !mem->storage) return;
    uint16_t bw = (uint16_t)~((unsigned)buttons & 07u);
    uint32_t bases[] = { 0, dorado_br_get(mem, 031), dorado_br_get(mem, 036) };
    for (size_t b = 0; b < sizeof bases / sizeof bases[0]; b++) {
        machine_store_va(mem, (bases[b] + 0424u) & 0x0FFFFFFFu, (uint16_t)x);
        machine_store_va(mem, (bases[b] + 0425u) & 0x0FFFFFFFu, (uint16_t)y);
        for (uint32_t a = 0177030u; a <= 0177033u; a++)
            machine_store_va(mem, (bases[b] + a) & 0x0FFFFFFFu, bw);
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

/* Find a real (physical) page not referenced by any resident Map entry.
 * Models a Pilot fault handler allocating a free real page to back a
 * faulting VM page. Returns the real-page number, or 0xFFFF if none. */
static uint16_t machine_find_free_rp(const dorado_memory *mem)
{
    uint32_t total = (uint32_t)(mem->storage_words / DM_PAGE_SIZE);
    if (total == 0) return 0xFFFF;
    if (total > 0x10000u) total = 0x10000u; /* rp is 16-bit */
    /* Mark every real page that a resident map entry uses. */
    static uint8_t used[0x10000];
    memset(used, 0, total);
    for (uint32_t i = 0; i < DM_MAP_ENTRIES; i++) {
        const dorado_map_entry *e = dorado_map_get(mem, i);
        if (e->wp && e->dirty) continue;          /* vacant -- ignore */
        if (e->rp < total) used[e->rp] = 1;
    }
    /* Allocate from the top so we avoid the low identity-mapped pages. */
    for (uint32_t rp = total; rp-- > 0; )
        if (!used[rp]) return (uint16_t)rp;
    return 0xFFFF;
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
    cfg->boot_file_number = 0110;
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
        cfg.germ_path    = pick(user_cfg->germ_path,    cfg.germ_path);
        cfg.alto_ether_boot  = user_cfg->alto_ether_boot;
        cfg.alto_ether_quote = user_cfg->alto_ether_quote;
        cfg.no_disk          = user_cfg->no_disk;
        if (user_cfg->storage_modules)
            cfg.storage_modules = user_cfg->storage_modules;
        if (user_cfg->boot_file_number)
            cfg.boot_file_number = user_cfg->boot_file_number;
        cfg.boot_dir_count = user_cfg->boot_dir_count;
        for (int i = 0; i < user_cfg->boot_dir_count &&
                        i < (int)(sizeof cfg.boot_dir / sizeof cfg.boot_dir[0]);
             i++)
            cfg.boot_dir[i] = user_cfg->boot_dir[i];
        cfg.boot_dir_all = user_cfg->boot_dir_all;
        cfg.boot_keys_count = user_cfg->boot_keys_count;
        for (int i = 0; i < user_cfg->boot_keys_count &&
                        i < (int)(sizeof cfg.boot_keys / sizeof cfg.boot_keys[0]);
             i++)
            cfg.boot_keys[i] = user_cfg->boot_keys[i];
    }
    if (cfg.storage_modules < 1 || cfg.storage_modules > 4)
        cfg.storage_modules = 1;

    dorado_machine *m = calloc(1, sizeof *m);
    if (!m) return NULL;
    m->alto_ether_boot  = cfg.alto_ether_boot;
    m->alto_ether_quote = cfg.alto_ether_quote;
    m->boot_file_number = cfg.boot_file_number;
    m->pre_swap_cpreg   = 0;

    /* Resolve the boot-key chord. An explicit chord (from the frontend's
     * --boot-keys / --boot-reason) is used verbatim; otherwise default to
     * BS, and fold in Quote iff alto_ether_quote -- byte-identical to the
     * historic forced BS-down (+Quote) seeding the regression gate locks
     * in. (Booting memo: BS selects the Ethernet software boot, Quote
     * NetExec.) */
    m->boot_chord_count = 0;
    if (cfg.boot_keys_count > 0) {
        for (int i = 0; i < cfg.boot_keys_count &&
                        m->boot_chord_count <
                            (int)(sizeof m->boot_chord / sizeof m->boot_chord[0]);
             i++)
            m->boot_chord[m->boot_chord_count++] = cfg.boot_keys[i];
    } else {
        m->boot_chord[m->boot_chord_count++] = DORADO_KEY_BS;
        if (cfg.alto_ether_quote)
            m->boot_chord[m->boot_chord_count++] = DORADO_KEY_QUOTE;
    }

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
    dorado_ethernet_set_boot_file(&m->ethernet, cfg.boot_file_number,
                                  cfg.eth_boot_110);
    dorado_ethernet_set_eftp_boot_file(&m->ethernet, cfg.eftp_boot);

    /* Register the boot-file directory entries the fake server advertises
     * to NetExec. Each is "NAME=BFN=PATH" (BFN octal). NAME must end in
     * ".boot"; PATH may itself contain no '=' (file paths here do not). */
    for (int i = 0; i < cfg.boot_dir_count; i++) {
        const char *spec = cfg.boot_dir[i];
        if (!spec) continue;
        const char *eq1 = strchr(spec, '=');
        if (!eq1) { fprintf(stderr, "dorado: bad --boot-dir '%s'\n", spec);
                    continue; }
        const char *eq2 = strchr(eq1 + 1, '=');
        if (!eq2) { fprintf(stderr, "dorado: bad --boot-dir '%s'\n", spec);
                    continue; }
        char name[48];
        size_t nlen = (size_t)(eq1 - spec);
        if (nlen >= sizeof name) nlen = sizeof name - 1;
        memcpy(name, spec, nlen);
        name[nlen] = '\0';
        uint16_t bfn = (uint16_t)strtoul(eq1 + 1, NULL, 8);
        const char *path = eq2 + 1;
        dorado_ethernet_add_boot_dir(&m->ethernet, bfn, name, path);
    }

    /* --boot-dir-all: auto-register every Alto B-format *.boot file in the
     * directory holding the EFTP boot file (chm/bootfiles/), so NetExec's
     * menu lists all the games. Explicit --boot-dir entries above take
     * precedence (auto-register skips names already present). */
    if (cfg.boot_dir_all && cfg.eftp_boot) {
        char dir[256];
        snprintf(dir, sizeof dir, "%s", cfg.eftp_boot);
        char *slash = strrchr(dir, '/');
        if (slash) *slash = '\0';
        else snprintf(dir, sizeof dir, ".");
        int n = dorado_ethernet_add_boot_dir_all(&m->ethernet, dir);
        if (getenv("DORADO_BOOTDIR_DEBUG"))
            fprintf(stderr, "[bootdir] --boot-dir-all: registered %d game(s) "
                    "from %s (%d entries total)\n", n, dir,
                    m->ethernet.bootdir_count);
    }

    /* Route B germ plant: load the Pilot germ image for later deposit into
     * VM. The germ file holds big-endian 16-bit words: the Dorado memory
     * word is (first byte << 8) | second byte. Proof: byte-swapping the
     * germ's GFT entries (MDS 0o1400+) yields EXACTLY the global-frame
     * addresses in Dorado.loadmap (gfi1->0o3400 ProcessorHead,
     * gfi4->0o4634 BootSwapGerm, ...), and BootSwapGerm's code-base-high
     * word byte-swaps to 0o76 = pilotMDSHi. Reading little-endian made
     * GFT[gfi] point past the loaded germ, so the boot Xfer through
     * SD[sBoot] took an sUnbound trap (microcode-grounded: DMesaXfer.mc
     * LoadGC/XferProc). Failure is non-fatal -- it just disables the plant. */
    if (cfg.germ_path) {
        FILE *gf = fopen(cfg.germ_path, "rb");
        if (!gf) {
            fprintf(stderr, "dorado: cannot open germ '%s'; plant disabled\n",
                    cfg.germ_path);
        } else {
            int n = 0;
            int b0, b1;
            while (n < (int)(sizeof m->germ_words / sizeof m->germ_words[0]) &&
                   (b0 = fgetc(gf)) != EOF && (b1 = fgetc(gf)) != EOF) {
                m->germ_words[n++] = (uint16_t)((b0 << 8) | b1);
            }
            fclose(gf);
            m->germ_word_count = n;
            if (getenv("DORADO_MACHINE_TRACE"))
                fprintf(stderr, "[machine] germ loaded: %d words from %s "
                        "(word0=0o%o)\n", n, cfg.germ_path,
                        n ? m->germ_words[0] : 0);
        }
    }

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
        /* Trace-gate cycle window (env DORADO_TRACE_GATE="lo,hi"), so the
         * standalone binary can drive the same gated IFUDISP/BR/store
         * traces as the test harness. Trace-only; no behavioral effect. */
        {
            static long tg_lo = -1, tg_hi = -1;
            extern int dorado_trace_gate;
            extern unsigned long long dorado_trace_cycle;
            dorado_trace_cycle = bb->cycles;
            if (tg_lo == -1) {
                const char *w = getenv("DORADO_TRACE_GATE");
                tg_lo = 0; tg_hi = 0;
                if (w) sscanf(w, "%ld,%ld", &tg_lo, &tg_hi);
            }
            dorado_trace_gate =
                (tg_hi && bb->cycles >= (uint64_t)tg_lo &&
                 bb->cycles <= (uint64_t)tg_hi);
        }
        /* One-shot VM memory dump (env DORADO_VMDUMP="lo,hi,cycle"),
         * trace-only. Dumps visible words [lo,hi) once at/after cycle. */
        {
            static long vd_lo = -2, vd_hi = 0, vd_cyc = 0;
            if (vd_lo == -2) {
                const char *w = getenv("DORADO_VMDUMP");
                vd_lo = -1;
                if (w) sscanf(w, "%li,%li,%li", &vd_lo, &vd_hi, &vd_cyc);
            }
            if (vd_lo >= 0 && bb->cycles >= (uint64_t)vd_cyc) {
                fprintf(stderr, "[VMDUMP] cyc=%llu lo=0o%lo hi=0o%lo\n",
                        (unsigned long long)bb->cycles,
                        (unsigned long)vd_lo, (unsigned long)vd_hi);
                for (long a = vd_lo; a < vd_hi; a++)
                    fprintf(stderr, "  M[0o%lo] = 0o%06o\n", (unsigned long)a,
                            dorado_visible_word_at_va(&m->mem, (uint32_t)a));
                vd_lo = -1; /* one-shot */
            }
        }
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
            /* Drive the Stage-2 boot decision via the DDC keyboard by
             * holding the configured boot-key chord (default BS, which
             * selects the Ethernet software boot). */
            if (m->alto_ether_boot) {
                uint16_t w[4];
                machine_apply_boot_chord(disp, m->boot_chord,
                                         m->boot_chord_count, w);
            }
        }

        uint16_t pre_pc = cpu->real_PC;

        /* Seed Initial's boot parameter (STK[1]=boot file number,
         * STK[2]=BootParameterSeal, STK[1]+STK[2]+STK[3]=0) so the
         * loaded world selects the normal Mesa boot instead of falling
         * into the cold/no-storage path. The 7-wire terminal back-channel
         * that would carry this is not modeled, so we plant it directly.
         * The boot file number is configurable (see boot_file_number);
         * STK[2] is the fixed BootParameterSeal and STK[3] is the
         * checksum word that makes the three sum to 0 mod 2^16. */
        if (m->initial_substituted && is_imfetch &&
            !m->checksum_and_load_seen &&
            ((pre_pc >= 06170 && pre_pc <= 06217) ||
             (pre_pc >= 06406 && pre_pc <= 06443))) {
            uint16_t seal = 056623u;
            cpu->STK[1] = m->boot_file_number;
            cpu->STK[2] = seal;
            cpu->STK[3] = (uint16_t)(0u - cpu->STK[1] - seal);
        }

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
            if (!getenv("DORADO_NO_ALUFM_RESTORE"))
                restore_standard_alufm(&m->mc);
            m->ether_loaded_world_cycle = bb->cycles;
            /* Quiet the junk timer and drop any stale pending wakeups
             * at the LoadRam handoff so the high-priority I/O tasks do
             * not run their pre-reload microcode and starve the
             * emulator task. Do NOT invalidate task TPCs: the fault
             * task (0o17) must remain runnable to service the map
             * faults InitMem raises while it enumerates storage. */
            cpu->wakeup_pending = 0;
            cpu->junk_tw_enabled = 0;
            cpu->junk_tw_countdown = 0;
            if (getenv("DORADO_MACHINE_TRACE"))
                fprintf(stderr, "[machine] world loaded @cyc=%llu pc=0o%o\n",
                        (unsigned long long)bb->cycles, pre_pc);
        }

        /* Route B germ disk-read interception. The Cedar microcode's disk
         * germ-boot is PilotBoot.GermBoot -> DiskBootSoft -> BootTransfer
         * (DiskBootTransfer.mc). DiskBootSoft issues THREE disk-read passes
         * against an IOCB at VM 0o431:
         *   1. command 0o274  ([check,read,read])  -- read the PV root-page
         *      Descriptor into memory page 0 (IOCB.dataPtr=0).
         *   2. command 0o260  ([check,read,none])  -- read the first page's
         *      label (no data) to learn the file type.
         *   3. command 0o100254 (incrementDataPtr|[check,check,read]) -- read
         *      the whole germ boot file into the low-64K buffer at
         *      IOCB.dataPtr = BootDataPtr = baseGerm (0o1000), advancing
         *      dataPtr by 0o400 per page.
         * Each pass posts the IOCB (seal=IOCBSealValue) then spins in
         * BootTransferLp at real PC 0o7012 (the seal-fetch) until the disk
         * microcode clears iocb.seal. We have no boot pack and the disk data
         * path is incomplete, so we FAKE each pass at the spin: deposit the
         * data the real microcode expects, then KCmmdDone-complete the IOCB
         * (seal=0, pageCount=0, labelStatus=0 -> BootTransfer returns +2,
         * proven mechanic). The REAL microcode then validates the Descriptor
         * (seal=0o121212, version=6 -- DiskBootSoft.mc), reads the germ,
         * runs PilotBoot.GERMREMAP to relocate the germ into MDS 76 and
         * XFERs into the germ wart -- we hand-roll none of that.
         *
         * PC 0o7012 is placed only in the Cedar/Mesa world (the Alto worlds
         * never reach it), so this stays gated off the regression gate. */
        if (m->germ_word_count && !m->germ_data_done &&
            m->ether_loaded_world_cycle && is_imfetch && cpu->ctask == 0 &&
            pre_pc == 07012) {
            uint16_t seal = dorado_visible_word_at_va(&m->mem, IOCB_SEAL_VA);
            if (seal == (uint16_t)IOCB_SEAL_VALUE) {
                uint16_t cmd = dorado_visible_word_at_va(&m->mem,
                                                         IOCB_COMMAND_VA);
                if (cmd == (uint16_t)DISK_CMD_DESCRIPTOR &&
                    !m->germ_descriptor_done) {
                    /* Pass 1: fabricate the PV root-page Descriptor in page 0.
                     * PhysicalVolumeFormat.mesa / PilotBootDefs.mc:
                     *   word 0  Desc.seal           = 121212B
                     *   word 1  Desc.currentVersion = 6
                     *   Desc.bi.germ (offset 0o32) = germ's DiskFileID
                     *     {fID[5]@0, firstPage@5, da[2]@7} -- the microcode
                     *     copies these into the IOCB but our fake completion
                     *     bypasses the real disk read, so plain zeroes (germ
                     *     file page 0, fID 0, da 0) suffice. */
                    /* The Mesa world leaves VM page 0 vacant (its null-trap
                     * page: wp=1,dirty=1). The real descriptor DMA write to
                     * page 0 would map-fault and XMFaultTask would make the
                     * page resident before the write lands; our fake bypasses
                     * the disk DMA, so emulate that side effect by making
                     * page 0 resident (identity rp 0, writable) before the
                     * deposit. The microcode comment confirms DiskBootSoft
                     * "clobbers memory page 0 by reading the root page into
                     * it." */
                    const dorado_map_entry *e0 = dorado_map_get(&m->mem, 0);
                    if (e0->wp && e0->dirty) {
                        /* Allocate a free real page (the existing e0->rp is
                         * aliased to another live VM page in the Mesa map, so
                         * reusing it lets that page's dirty writeback clobber
                         * our descriptor). */
                        uint16_t frp = machine_find_free_rp(&m->mem);
                        if (frp == 0xFFFF) frp = e0->rp;
                        dorado_map_set(&m->mem, 0, frp, /*wp=*/0, /*dirty=*/0);
                    }
                    for (uint32_t w = 0; w <= 0043u; w++)
                        dorado_storage_store_at_va(&m->mem, w, 0);
                    dorado_storage_store_at_va(&m->mem, 0, 0121212u);
                    dorado_storage_store_at_va(&m->mem, 1, 06u);
                    m->germ_descriptor_done = 1;
                    fprintf(stderr,
                        "[machine] germ pass1 (descriptor) @cyc=%llu: "
                        "seal=0o121212 version=6 at page 0 "
                        "(readback[0]=0o%o [1]=0o%o)\n",
                        (unsigned long long)bb->cycles,
                        dorado_visible_word_at_va(&m->mem, 0),
                        dorado_visible_word_at_va(&m->mem, 1));
                } else if (cmd == (uint16_t)DISK_CMD_LABEL &&
                           !m->germ_label_done) {
                    /* Pass 2: first-page label read (no data). DiskBootSoft
                     * does not inspect the label contents, so just complete. */
                    m->germ_label_done = 1;
                    fprintf(stderr,
                        "[machine] germ pass2 (label) @cyc=%llu: completed\n",
                        (unsigned long long)bb->cycles);
                } else if ((cmd == (uint16_t)DISK_CMD_GERMDATA) &&
                           !m->germ_data_done) {
                    /* Pass 3: read the germ file into the low buffer at
                     * IOCB.dataPtr (= BootDataPtr = baseGerm 0o1000). Deposit
                     * germ file word W -> VM dataPtr+W, then advance dataPtr
                     * by 0o400 per page (as the disk microcode would) so
                     * GermBoot reads BootDataPtr = dataPtr + 0o400*pages and
                     * GERMREMAP relocates exactly the loaded extent. */
                    uint16_t dptr = dorado_visible_word_at_va(&m->mem,
                                                              IOCB_DATAPTR_VA);
                    int landed = 0;
                    for (int w = 0; w < m->germ_word_count; w++) {
                        if (dorado_storage_store_at_va(&m->mem,
                                (uint32_t)dptr + (uint32_t)w,
                                m->germ_words[w]) == 0)
                            landed++;
                    }
                    uint32_t pages = (uint32_t)(m->germ_word_count + 0377) / 0400u;
                    uint16_t new_dptr = (uint16_t)(dptr + pages * 0400u);
                    dorado_storage_store_at_va(&m->mem, IOCB_DATAPTR_VA,
                                               new_dptr);
                    m->germ_data_done = 1;
                    fprintf(stderr,
                        "[machine] germ pass3 (data) @cyc=%llu: %d/%d words "
                        "at VM 0o%o+ (word0=0o%o), dataPtr 0o%o->0o%o\n",
                        (unsigned long long)bb->cycles, landed,
                        m->germ_word_count, dptr,
                        dorado_visible_word_at_va(&m->mem, dptr),
                        dptr, new_dptr);
                }
                /* KCmmdDone completion (proven): clear seal, zero pageCount
                 * and labelStatus -> BootTransfer's post-spin check sees
                 * pageCount==0 and returns +2 (success). */
                dorado_storage_store_at_va(&m->mem, IOCB_SEAL_VA, 0);
                dorado_storage_store_at_va(&m->mem, IOCB_PAGECOUNT_VA, 0);
                dorado_storage_store_at_va(&m->mem, IOCB_LABELSTAT_VA, 0);
                m->germ_passes++;
            }
        }

        /* Track the world's currently-posted Ethernet input-buffer size
         * (EICLOC at Alto VM 604B, Alto HW Manual Sec 7) so the receiver
         * can refuse to overflow it. The world re-posts this constantly and
         * frequently parks it at 0 between exchanges; handing the receiver a
         * packet bigger than the live buffer trips Input Buffer Overrun and
         * derails it (observed deriling CedarNetExec). Before the world is
         * loaded, 604B is not an Alto cell, so the gate stays disabled
         * (world_rx_words = 0xFFFF from init). */
        if (m->ether_loaded_world_cycle)
            eth->world_rx_words = dorado_visible_word_at_va(&m->mem, 0604u);

        /* Seed BS-down into the Alto keyboard words so the loaded world
         * selects the Ethernet software boot (AEm0.mc branches to EBoot
         * -> Mayday -> NetExec). The 7-wire DDC keyboard back-channel is
         * not modeled, so write the polled words directly. */
        if (m->alto_ether_boot && m->ether_loaded_world_cycle &&
            !cpu->ifu_active) {
            if (eth->eftp_max_seq == 0) {
                /* Boot-selection phase: hold the boot-key chord down so the
                 * world picks its boot path (default BS = Ethernet software
                 * boot). The chord is applied to the DDC keyboard and its
                 * Alto words are seeded into the polled cells (gap E2). */
                uint16_t w[4];
                machine_apply_boot_chord(disp, m->boot_chord,
                                         m->boot_chord_count, w);
                machine_seed_keyboard(&m->mem, w);
            } else {
                /* Interactive phase: the boot file is downloading, so
                 * release the held boot keys and deliver the frontend's
                 * live key state to the running world. */
                if (!m->keys_live) {
                    dorado_display_keyboard_all_up(disp);
                    m->keys_live = 1;
                }
                uint16_t w[4];
                for (int i = 0; i < 4; i++)
                    w[i] = dorado_display_keyboard_word(disp, i);
                machine_seed_keyboard(&m->mem, w);
                if (m->mouse_present)
                    machine_seed_mouse(&m->mem, m->mouse_x, m->mouse_y,
                                       m->mouse_buttons);
            }

            /* (The divide-vector guard was retired 2026-06-13: the
             * page-zero BitBlt spray that corrupted M[0o344] is fixed at
             * the source -- see the RM-write RBase-timing fix in cpu.c
             * and docs/CONTINUE-HERE.md. The world is now stable to 200M
             * with no guard.) */
        }

        /* Optional task-0 PC histogram of the loaded world (env-gated):
         * DORADO_MACHINE_PCHIST dumps the hottest emulator-task PCs at
         * the end of the run, to localize a post-LoadRam stall. */
        if (m->ether_loaded_world_cycle && is_imfetch && cpu->ctask == 0 &&
            pre_pc < 4096 && getenv("DORADO_MACHINE_PCHIST")) {
            m->pchist[pre_pc]++;
            if (m->initseq_n < (int)(sizeof m->initseq / sizeof m->initseq[0]))
                m->initseq[m->initseq_n++] = (uint16_t)pre_pc;
        }

        /* InitMem GotMapConfig/NoStorage register trace (env-gated):
         * dump the registers feeding the storage-detect branch so the
         * NoStorage divergence can be compared against the harness. */
        if (m->ether_loaded_world_cycle && is_imfetch && cpu->ctask == 0 &&
            pre_pc >= 01005 && pre_pc <= 01025 &&
            getenv("DORADO_MACHINE_INITMEM")) {
            fprintf(stderr,
                    "INITMEM pc=0o%o T=%06o Q=%06o ShC=%06o Cnt=%06o "
                    "aluZ=%u aluLT=%u link=0o%o\n",
                    pre_pc, cpu->T, cpu->Q, cpu->ShC, cpu->Cnt,
                    cpu->alu_zero, cpu->alu_lt0, cpu->Link);
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
                /* Once the world is loaded (post-EFTP), drop to the real
                 * gateway-info cadence -- ~one every 30s per the Pup spec
                 * (IFS GatewayInformationProtocol), not a flood. A flood
                 * lands an input completion in EPLOC during the loaded
                 * world's Tx-completion poll and derails CedarNetExec's
                 * Ethernet init; one early packet (delivered when its Rx is
                 * armed) is enough, and the 30s gap keeps subsequent
                 * broadcasts clear of its Tx poll. 500M cyc ~ 30s @60ns. */
                if (sent && eth->eftp_max_seq > 0) interval = 500000000u;
                m->next_bol_cycle = bb->cycles + interval;
            }

            uint16_t eth_mask = dorado_ethernet_wakeup_mask(eth);
            for (int task = 0; task < 16; task++) {
                if (eth_mask & (uint16_t)(1u << task)) {
                    dorado_cpu_wakeup(cpu, task);
                }
            }

            /* Display scan-line wakeups (DHT/AHT) and DWT word-task
             * wakeups, every ~1000 cycles -- but only once the loaded
             * world has installed a display list (DASTART at VM 0o420
             * nonzero). Waking the display tasks before then runs the
             * pre-LoadRam display microcode left in their stale TPCs,
             * which spins at high priority and starves the emulator
             * task before it can reach its boot decision. */
            uint32_t dl_mds = dorado_br_get(&m->mem, 036);
            uint16_t dl_head = dorado_visible_word_at_va(&m->mem,
                                                         dl_mds + 0420u);
            int display_active = (dl_head != 0 && dl_head != 0xFFFFu);
            if (display_active && bb->cycles >= m->next_display_scanline_cycle) {
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
            if (display_active && dorado_display_dwt_wakeup(disp, &dwt_subtask)) {
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

void dorado_machine_set_key(dorado_machine *m, dorado_display_key key,
                            int down)
{
    if (!m) return;
    dorado_display_keyboard_set_key(&m->display, key, down ? 1 : 0);
}

int dorado_machine_interactive(const dorado_machine *m)
{
    return m && m->keys_live;
}

void dorado_machine_set_mouse(dorado_machine *m, int x, int y, int buttons)
{
    if (!m) return;
    if (x < 0) x = 0; else if (x > 807) x = 807;
    if (y < 0) y = 0; else if (y > 605) y = 605;
    m->mouse_present = 1;
    m->mouse_x = x;
    m->mouse_y = y;
    m->mouse_buttons = buttons;
}

dorado_display *dorado_machine_display(dorado_machine *m)
{
    return m ? &m->display : NULL;
}

void dorado_machine_debug(dorado_machine *m)
{
    if (!m) return;
    if (getenv("DORADO_MACHINE_PCHIST")) {
        /* Top 12 hottest task-0 PCs in the loaded world. */
        int top[12]; for (int i = 0; i < 12; i++) top[i] = -1;
        for (int a = 0; a < 4096; a++) {
            if (!m->pchist[a]) continue;
            for (int s = 0; s < 12; s++) {
                if (top[s] < 0 || m->pchist[a] > m->pchist[top[s]]) {
                    for (int t = 11; t > s; t--) top[t] = top[t-1];
                    top[s] = a; break;
                }
            }
        }
        fprintf(stderr, "[machine] task-0 hot PCs:");
        for (int i = 0; i < 12 && top[i] >= 0; i++)
            fprintf(stderr, " 0o%o=%u", top[i], m->pchist[top[i]]);
        fprintf(stderr, "\n[machine] task-0 init sequence (first %d):\n",
                m->initseq_n);
        for (int i = 0; i < m->initseq_n; i++) {
            fprintf(stderr, " %o%s", m->initseq[i],
                    (i % 16 == 15) ? "\n" : "");
        }
        fprintf(stderr, "\n");
    }
    dorado_ethernet *e = &m->ethernet;
    uint32_t mds = dorado_br_get(&m->mem, 036);
    uint16_t dastart = dorado_visible_word_at_va(&m->mem, mds + 0420u);
    fprintf(stderr,
            "[machine] booted=%d cyc=%llu pc=0o%o tk=%u | eth: rx=%u tx=%u "
            "req=%llu repl=%llu eftp_r=%llu eftp_q=%llu seq=%u bol=%llu "
            "time=%llu | DASTART=%06o stwords=%zu\n",
            dorado_machine_booted(m),
            (unsigned long long)m->bb.cycles,
            m->cpu.real_PC, m->cpu.ctask,
            e->rx_on, e->tx_on,
            (unsigned long long)e->requests_seen,
            (unsigned long long)e->replies_queued,
            (unsigned long long)e->eftp_requests_seen,
            (unsigned long long)e->eftp_replies_queued,
            e->eftp_max_seq,
            (unsigned long long)e->bol_queued,
            (unsigned long long)e->time_bcasts,
            dastart, m->mem.storage_words);
    fprintf(stderr, "[machine] config_word=0o%o (B<-Config'=0o%o)\n",
            dorado_memory_config_word(&m->mem),
            (uint16_t)~dorado_memory_config_word(&m->mem));
    fprintf(stderr, "[machine] M[344]=0o%o (guard=%d) Swat-OutLdRet="
            "0o%o AC700=0o%o\n",
            dorado_visible_word_at_va(&m->mem, mds + 0344u),
            m->mem.protect_active,
            dorado_visible_word_at_va(&m->mem, mds + 03323u),
            dorado_visible_word_at_va(&m->mem, mds + 0700u));
    {
        uint32_t v344 = (mds + 0344u) & 0x0FFFFFFFu;
        uint32_t idx = dorado_map_index(v344);
        const dorado_map_entry *e344 = dorado_map_get(&m->mem, idx);
        fprintf(stderr, "[machine] MDS=0o%o v344=0o%o mapidx=0o%o "
                "rp=0o%o phys=0o%o\n",
                mds, v344, idx, e344->rp,
                (unsigned)((uint32_t)e344->rp * DM_PAGE_SIZE
                           + (v344 & (DM_PAGE_SIZE - 1))));
    }
}

/* Flip one framebuffer pixel (XOR), for the mouse pointer. */
static void fb_xor(dorado_display *d, int x, int y)
{
    if (x < 0 || x >= DORADO_DISPLAY_W || y < 0 || y >= DORADO_DISPLAY_H)
        return;
    d->fb[y * DORADO_DISPLAY_ROW_BYTES + (x >> 3)] ^=
        (uint8_t)(1u << (7 - (x & 7)));
}

int dorado_machine_render_display_list(dorado_machine *m)
{
    if (!m) return 0;
    dorado_memory *mem = &m->mem;
    dorado_display *disp = &m->display;

    /* The rasterizer owns the whole frame: clear to white (0 = white,
     * 1 = black), then paint everything. This avoids the smearing that
     * came from never erasing the previous frame. */
    memset(disp->fb, 0, sizeof disp->fb);

    uint32_t dmds = dorado_br_get(mem, 036);
    uint32_t dl = dorado_visible_word_at_va(mem, dmds + 0420u);
    int pixels = 0, y = 0;
    /* Alto DCB (Hardware Manual; salto helloworld.asm):
     *   w0 = next DCB (0 ends)
     *   w1 = (res<<15) | (inverse<<14) | (HTAB<<8) | NWRDS
     *        HTAB indents by 16*HTAB bits; NWRDS words/scanline (even)
     *   w2 = SA  bitmap start word address (even)
     *   w3 = SLC the block defines 2*SLC scanlines (SLC per field),
     *            i.e. 2*SLC consecutive bitmap rows of NWRDS words. */
    for (int g = 0; g < 64 && dl > 1u && y < DORADO_DISPLAY_H; g++) {
        uint16_t c   = dorado_visible_word_at_va(mem, dmds + dl + 1u);
        uint16_t sa  = dorado_visible_word_at_va(mem, dmds + dl + 2u);
        uint16_t slc = dorado_visible_word_at_va(mem, dmds + dl + 3u);
        int htab  = (c >> 8) & 077;
        int nwrds = c & 0377;
        int inv   = (c >> 14) & 1;
        int lines = (int)slc * 2;
        for (int row = 0; row < lines && y < DORADO_DISPLAY_H; row++, y++) {
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

    /* Mouse pointer: a fixed NW-arrow XOR'd in at the host mouse
     * position, so it is visible on any background and never smears
     * (the frame is fully redrawn each time). */
    if (m->mouse_present) {
        static const uint16_t arrow[16] = {
            0x8000, 0xC000, 0xE000, 0xF000, 0xF800, 0xFC00, 0xFE00, 0xFF00,
            0xFF80, 0xF800, 0xD800, 0x8C00, 0x0C00, 0x0600, 0x0600, 0x0300,
        };
        for (int r = 0; r < 16; r++)
            for (int b = 0; b < 16; b++)
                if ((arrow[r] >> (15 - b)) & 1)
                    fb_xor(disp, m->mouse_x + b, m->mouse_y + r);
    }
    return pixels;
}
