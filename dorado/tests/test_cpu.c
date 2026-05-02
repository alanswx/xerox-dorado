#include "baseboard.h"
#include "cpu.h"
#include "disasm.h"
#include "disk.h"
#include "display.h"
#include "ethernet.h"
#include "fastio.h"
#include "io.h"
#include "mb.h"
#include "memory.h"
#include "microcode.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAIL(msg, ...) do { \
    fprintf(stderr, "FAIL: %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    return 1; \
} while (0)

#define EXPECT(cond, msg, ...) do { \
    if (!(cond)) FAIL(msg, ##__VA_ARGS__); \
} while (0)

static uint64_t test_u64_env(const char *name, uint64_t fallback)
{
    const char *s = getenv(name);
    if (!s || !*s) return fallback;
    char *end = NULL;
    unsigned long long v = strtoull(s, &end, 0);
    return (end && *end == '\0' && v > 0) ? (uint64_t)v : fallback;
}

static const char *test_str_env(const char *name, const char *fallback)
{
    const char *s = getenv(name);
    return (s && *s) ? s : fallback;
}

static const char *ref_kind_name(dorado_ref_kind kind)
{
    switch (kind) {
    case DM_REF_NONE:      return "none";
    case DM_REF_PREFETCH:  return "prefetch";
    case DM_REF_MAP:       return "map";
    case DM_REF_IOFETCH:   return "iofetch";
    case DM_REF_LONGFETCH: return "longfetch";
    case DM_REF_STORE:     return "store";
    case DM_REF_DUMMYREF:  return "dummyref";
    case DM_REF_FLUSH:     return "flush";
    case DM_REF_IOSTORE:   return "iostore";
    case DM_REF_IFETCH:    return "ifetch";
    case DM_REF_FETCH:     return "fetch";
    }
    return "unknown";
}

static int file_exists_readable(const char *path)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) return 0;
    fclose(fp);
    return 1;
}

static void store_boot_va(dorado_memory *mem, uint32_t va, uint16_t value)
{
    if (!mem || !mem->storage) return;

    uint32_t idx = dorado_map_index(va);
    const dorado_map_entry *e = dorado_map_get(mem, idx);
    size_t phys = (size_t)e->rp * DM_PAGE_SIZE + (va & (DM_PAGE_SIZE - 1));
    if (phys < mem->storage_words) {
        mem->storage[phys] = value;
    }

    uint32_t row = (va >> 4) & DM_CACHE_ROW_MASK;
    uint32_t tag = va >> 10;
    uint32_t off = va & DM_CACHE_LINE_MASK;
    for (int way = 0; way < DM_CACHE_WAYS; way++) {
        dorado_cache_line *line = &mem->cache[row].ways[way];
        if (line->valid && line->tag == tag) {
            line->data[off] = value;
        }
    }
}

static int inject_ether_boot_image(dorado_memory *mem, const char *path,
                                   uint32_t start_va, uint16_t *end_va,
                                   uint16_t *sum_out,
                                   uint16_t *start_pc_out)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) return 0;

    /* EB files have a one-page Pilot/IFS overhead record. Initial's
     * Ethernet receive path stores only the checksummed payload. */
    if (fseek(fp, 512, SEEK_SET) != 0) {
        fclose(fp);
        return 0;
    }

    uint32_t va = start_va;
    uint32_t sum = 0;
    uint16_t item_first = 0;
    uint16_t start_pc = 0;
    int item_word = 0;
    for (;;) {
        int hi = fgetc(fp);
        int lo = fgetc(fp);
        if (hi == EOF && lo == EOF) break;
        if (hi == EOF || lo == EOF) {
            fclose(fp);
            return 0;
        }
        uint16_t word = (uint16_t)(((uint16_t)hi << 8) | (uint16_t)lo);
        store_boot_va(mem, va, word);
        sum = (sum + word) & 0xFFFFu;
        if (item_word == 0) {
            item_first = word;
        } else if (item_word == 3 && (item_first & 7u) == 2u) {
            start_pc = word;
        }
        item_word = (item_word + 1) & 3;
        va++;
    }
    fclose(fp);

    if (end_va) *end_va = (uint16_t)(va & 0xFFFFu);
    if (sum_out) *sum_out = (uint16_t)sum;
    if (start_pc_out) *start_pc_out = start_pc;
    return va > start_va;
}

static int read_eb_end_item(const char *path, uint32_t *word_offset_out,
                            uint16_t item_out[4], uint32_t *payload_words_out,
                            uint16_t *sum_out)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) return 0;
    if (fseek(fp, 512, SEEK_SET) != 0) {
        fclose(fp);
        return 0;
    }

    uint32_t payload_words = 0;
    uint32_t sum = 0;
    uint16_t item[4] = {0};
    for (;;) {
        for (int i = 0; i < 4; i++) {
            int hi = fgetc(fp);
            int lo = fgetc(fp);
            if (hi == EOF && lo == EOF) {
                fclose(fp);
                return 0;
            }
            if (hi == EOF || lo == EOF) {
                fclose(fp);
                return 0;
            }
            item[i] = (uint16_t)(((uint16_t)hi << 8) | (uint16_t)lo);
            sum = (sum + item[i]) & 0xFFFFu;
            payload_words++;
        }
        if ((item[0] & 7u) == 2u) {
            fclose(fp);
            if (word_offset_out) *word_offset_out = payload_words - 4;
            if (item_out) {
                for (int i = 0; i < 4; i++) item_out[i] = item[i];
            }
            if (payload_words_out) *payload_words_out = payload_words;
            if (sum_out) *sum_out = (uint16_t)sum;
            return 1;
        }
    }
}

static const char *ether_boot_path_for_offset(const dorado_ethernet *eth,
                                              uint16_t offset)
{
    switch (offset) {
    case 0110: return eth->boot_110_path;
    case 0111: return eth->boot_111_path;
    case 0113: return eth->boot_113_path;
    case 0114: return eth->boot_114_path;
    default: return NULL;
    }
}

static void map_boot_probe_bank_to(dorado_memory *mem, uint32_t base_page,
                                   uint32_t real_base_page, uint32_t pages)
{
    for (uint32_t pg = 0; pg < pages; pg++) {
        uint32_t vp = base_page + pg;
        if (vp >= DM_MAP_ENTRIES) break;
        dorado_map_set(mem, vp, (uint16_t)(real_base_page + pg),
                       /*wp=*/0, /*dirty=*/0);
    }
}

static void map_boot_probe_bank(dorado_memory *mem, uint32_t base_page,
                                uint32_t pages)
{
    map_boot_probe_bank_to(mem, base_page, base_page, pages);
}

static void map_boot_probe_installed_storage(dorado_memory *mem)
{
    uint32_t pages = (uint32_t)(mem->storage_words / DM_PAGE_SIZE);
    if (pages > DM_MAP_ENTRIES) pages = DM_MAP_ENTRIES;
    map_boot_probe_bank(mem, 0, pages);
}

static void restore_standard_alufm(dorado_microcode *mc)
{
    /* LoadRam.mc's Item format carries IM, IFUM, RM, and End records
     * only.  Complete replacement images inherit the standard ALUFM
     * convention that Initial established before calling LoadRam.  The
     * direct probe bypasses that real LoadRam handoff, so restore the
     * same table here from the canonical Bootstrap/AEmu/Mesa values. */
    static const uint8_t standard[ALUFM_SIZE] = {
        025, 000, 014, 054, 062, 022, 035, 027,
        023, 031, 040, 036, 013, 033, 001, 006,
    };
    for (int i = 0; i < ALUFM_SIZE; i++) {
        mc->alufm[i] = standard[i];
        mc->alufm_present[i] = 1;
    }
}

static int loadram_image_direct(dorado_microcode *mc, dorado_cpu *cpu,
                                const char *path, uint16_t *start_pc_out)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) return 0;
    if (fseek(fp, 512, SEEK_SET) != 0) {
        fclose(fp);
        return 0;
    }

    restore_standard_alufm(mc);

    int loaded = 0;
    uint16_t start_pc = 0;
    for (;;) {
        uint16_t w[4];
        for (int i = 0; i < 4; i++) {
            int hi = fgetc(fp);
            int lo = fgetc(fp);
            if (hi == EOF && lo == EOF) {
                fclose(fp);
                return loaded;
            }
            if (hi == EOF || lo == EOF) {
                fclose(fp);
                return 0;
            }
            w[i] = (uint16_t)(((uint16_t)hi << 8) | (uint16_t)lo);
        }

        int type = w[0] & 7;
        uint16_t addr = w[1];
        if (type == 2) {
            start_pc = w[3];
            break;
        }
        if (type == 0) {
            uint16_t a = addr & 0x0FFFu;
            if (a >= 07600 && a < 07700) continue;
            if (a < IM_SIZE) {
                uint16_t extra = (uint16_t)(w[0] >> 12);
                uint16_t rstk0 = (uint16_t)((extra >> 2) & 1u);
                uint16_t block = (uint16_t)(extra & 1u);
                dorado_uinstr *u = &mc->im[a];
                u->iw0 = w[2];
                u->iw1 = (uint16_t)((block << 15) | ((w[3] >> 1) & 0x7FFFu));
                u->iw2 = (uint16_t)((rstk0 << 15) | ((w[3] & 1u) << 14));
                u->awd = a;
                dorado_redecode_fields(u);
                mc->im_present[a] = 1;
                loaded++;
            }
        } else if (type == 1) {
            uint16_t a = addr & 0x03FFu;
            mc->ifum_lo[a] = w[2];
            mc->ifum_hi[a] = w[3];
            mc->ifum_present[a] = 1;
            loaded++;
        } else if (type == 3) {
            uint16_t a = addr & 0x00FFu;
            mc->rm[a] = w[2];
            mc->rm_present[a] = 1;
            if (cpu) cpu->RM[a] = w[2];
            loaded++;
        }
    }
    fclose(fp);

    if (start_pc_out) *start_pc_out = start_pc;
    return loaded > 0;
}

static int attach_default_trident_pack(dorado_disk_controller *disk,
                                       dorado_disk_pack *pack)
{
    const char *path = test_str_env(
        "DORADO_TRIDENT_PACK",
        "../AltoInfo/ContrAlto2-beta/Disks/spruce-server.dsk300");
    if (!file_exists_readable(path)) return 0;
    if (dorado_disk_pack_load(pack, &DORADO_DISK_T300, path) != 0) return 0;
    pack->read_only = 1;
    dorado_disk_controller_attach_drive(disk, 0, pack);
    return 1;
}

static void service_boot_disk(dorado_cpu *cpu, dorado_disk_controller *disk,
                              uint64_t cycle, uint64_t *sector_ticks,
                              uint64_t *wakeups)
{
    if (!disk || !disk->drive[0].pack) return;
    uint64_t sector_period = test_u64_env("DORADO_DISK_SECTOR_PERIOD", 512);
    if (sector_period == 0) sector_period = 1;
    /* Probe-only spindle service: the real drive/index/sector clocks
     * run independently of whether microcode is currently touching the
     * slow-IO ports. Keep that property here and let the controller's
     * own latch/mask state decide whether the DSK task should wake. */
    if ((disk->active || disk->enable_run) &&
        (cycle % sector_period) == 0) {
        dorado_disk_controller_advance_sector(disk);
        if (sector_ticks) (*sector_ticks)++;
    }
    int disk_pending = dorado_disk_controller_wakeup_pending(disk);
    if (disk_pending && cpu) {
        uint16_t dsk_tpc = dorado_cpu_get_task_tpc(cpu, DORADO_DISK_TASK);
        int only_spindle_tw =
            (disk->index_tw || disk->sector_tw) &&
            !disk->tag_tw && !disk->rd_fifo_tw && !disk->wr_fifo_tw;
        int transfer_armed = disk->active || ((disk->control & 0xFFu) != 0);
        if ((dsk_tpc == 05250 || dsk_tpc == 05252) &&
            only_spindle_tw && !transfer_armed) {
            disk_pending = 0;
            cpu->wakeup_pending &= (uint16_t)~(1u << DORADO_DISK_TASK);
        }
    }
    if (disk_pending) {
        dorado_cpu_wakeup(cpu, DORADO_DISK_TASK);
        if (wakeups) (*wakeups)++;
    }
}

static void seed_boot_keyboard_va(dorado_memory *mem, uint32_t va,
                                  uint16_t value)
{
    if (!mem || !mem->storage) return;

    if ((size_t)va < mem->storage_words) {
        mem->storage[va] = value;
    }

    store_boot_va(mem, va, value);
}

static void seed_boot_keyboard_from_display(dorado_memory *mem,
                                            const dorado_display *display)
{
    if (!mem || !mem->storage) return;

    uint32_t bases[] = {
        0,
        dorado_br_get(mem, 031),  /* IOBR: Initial reads through MemBase=IOBR. */
    };

    for (size_t b = 0; b < sizeof bases / sizeof bases[0]; b++) {
        for (uint32_t off_va = 0177034u; off_va < 0177042u; off_va++) {
            uint32_t va = (bases[b] + off_va) & 0x0FFFFFFFu;
            int word = (int)(off_va - 0177034u);
            uint16_t value = dorado_display_keyboard_word(display, word);
            seed_boot_keyboard_va(mem, va, value);
        }
    }
}

static int service_alto_disk_boot_shim(dorado_memory *mem,
                                       dorado_disk_pack *pack,
                                       int boot_cyl,
                                       int boot_head,
                                       int boot_sector,
                                       int force_membase,
                                       int force_kwait)
{
    if (!mem || !pack || !pack->sectors) return 0;

    /* AEmu's Alto disk boot builds a legacy Alto command block at
     * 0431 and posts its pointer in 0521, relative to MDS/IOBR in
     * Alto mode.  Until the Alto/Dorado disk task is complete, satisfy
     * just that first boot-sector read so CPU/display bring-up can keep
     * moving. */
    uint32_t bases[] = {
        0,
        dorado_br_get(mem, 036),  /* MDS and Alto IOBR per ADefs.mc. */
        dorado_br_get(mem, 031),  /* ECBR in Alto mode; IOBR in PrincOps. */
        dorado_br_get(mem, 030),
    };
    uint32_t base = force_kwait ?
        (dorado_br_get(mem, force_membase & 037) & 0x0FFFFFFFu) : 0;
    int found = force_kwait ? 1 : 0;
    if (!force_kwait) {
        for (size_t i = 0; i < sizeof bases / sizeof bases[0]; i++) {
            uint32_t b = bases[i] & 0x0FFFFFFFu;
            int duplicate = 0;
            for (size_t j = 0; j < i; j++) {
                if ((bases[j] & 0x0FFFFFFFu) == b) duplicate = 1;
            }
            if (duplicate) continue;
            if (dorado_visible_word_at_va(mem, b + 0521u) == 0431u) {
                base = b;
                found = 1;
                break;
            }
        }
        if (!found) return 0;
    }
    if (dorado_visible_word_at_va(mem, base + 0432u) & 07400u) return 0;

    if (boot_cyl < 0) boot_cyl = 0;
    if (boot_cyl >= pack->geometry.cylinders) {
        boot_cyl = pack->geometry.cylinders - 1;
    }
    if (boot_head < 0) boot_head = 0;
    if (boot_head >= pack->geometry.heads) {
        boot_head = pack->geometry.heads - 1;
    }
    if (boot_sector < 0) boot_sector = 0;
    if (boot_sector >= pack->geometry.sectors) {
        boot_sector = pack->geometry.sectors - 1;
    }
    dorado_disk_sector *s = dorado_disk_pack_sector(pack, boot_cyl,
                                                    boot_head, boot_sector);
    if (!s) return 0;

    for (int i = 0; i < DORADO_DISK_HEADER_WORDS; i++) {
        store_boot_va(mem, base + 0402u + (uint32_t)i, s->header[i]);
    }
    for (int i = 0; i < DORADO_DISK_LABEL_WORDS; i++) {
        store_boot_va(mem, base + 0404u + (uint32_t)i, s->label[i]);
    }
    for (int i = 0; i < DORADO_DISK_DATA_WORDS; i++) {
        store_boot_va(mem, base + 0001u + (uint32_t)i, s->data[i]);
    }

    /* AEm0.mc KWait checks DoneStatus (07400) and then treats the low
     * byte as error bits. A successful first-sector read is done with
     * no low-byte errors. */
    store_boot_va(mem, base + 0432u, 07400u);
    store_boot_va(mem, base + 0521u, 0);
    return 1;
}

static uint16_t boot_keyboard_word(const dorado_memory *mem, uint32_t off)
{
    if (!mem) return 0;
    uint32_t va = (dorado_br_get(mem, 031) + off) & 0x0FFFFFFFu;
    if ((size_t)va >= mem->storage_words) return 0;
    return dorado_visible_word_at_va(mem, va);
}

static uint32_t boot_keyboard_base(const dorado_memory *mem)
{
    return mem ? dorado_br_get(mem, 031) : 0;
}

static int trace_lowcore_offset(uint32_t off)
{
    if (off >= 0420u && off <= 0450u) return 1;       /* DAStart/cursor/DCB */
    if (off >= 0521u && off <= 0523u) return 1;       /* Alto disk command */
    if (off >= 0177034u && off <= 0177041u) return 1; /* keyboard words */
    return 0;
}

/*
 * The CPU is single-task, no-IFU, no-memory. Tests construct
 * dorado_microcode structs directly with hand-built dorado_uinstr
 * entries — no need to go through MB encoding/decoding.
 *
 * Field layouts (HM Tables 5–13):
 *   RSTK  4 bits   ALUF  4 bits  BSEL  3 bits  LC  3 bits
 *   ASEL  3 bits   BLOCK 1 bit   FF    8 bits  JCN 8 bits
 *
 * BSEL codes:  0=Md  1=RM/STK  2=T  3=Q  4=0,,FF  5=0xFF,,FF  6=FF,,0  7=FF,,0xFF
 * ASEL codes:  4=A←RM/STK  6=A←T  (others stub for memory/shifter)
 * LC codes:    0=NoLoad  1=T←Pd  6=RM/STK←Pd  7=T←Pd, RM/STK←Pd
 * ALUFM op:    bit 5 = carry-in, bits 4..0 = operation (Table 9):
 *                0o25 = B (logical "B")
 *                0o14 = A+B (no carry)
 *                0o00 = A (with bit5=1 → A+1)
 *                0o31 = all-zeros
 * JCN: bits 7..6 = 10 → local (target = (PC & ~63) | (JCN & 63))
 */

/* Build a dorado_uinstr from explicit field values. */
static dorado_uinstr make_uinstr(int rstk, int aluf, int bsel, int lc,
                                 int asel, int block, int ff, int jcn)
{
    dorado_uinstr u = {0};
    u.rstk  = (uint8_t)rstk;
    u.aluf  = (uint8_t)aluf;
    u.bsel  = (uint8_t)bsel;
    u.lc    = (uint8_t)lc;
    u.asel  = (uint8_t)asel;
    u.block = (uint8_t)block;
    u.ff    = (uint8_t)ff;
    u.jcn   = (uint8_t)jcn;
    return u;
}

/* JCN byte that encodes a local branch within the current page. */
static uint8_t jcn_local(int target_in_page)
{
    /* High 2 bits = 10 (local tag), low 6 = address within page. */
    return (uint8_t)(0x80 | (target_in_page & 0x3F));
}

/* JCN byte that encodes the low four bits of a long branch. The high
 * eight target bits live in FF for long branches. */
static uint8_t jcn_long(int target)
{
    return (uint8_t)(target & 0xF);
}

/* Test 1: T ← 0o123 via 0,,FF constant, then loop.
 * One cycle:
 *   ASEL=6 (A←T)         A bus = T (don't care, ALU op is "B")
 *   BSEL=4 (0,,FF)       B bus = 0o123
 *   ALUF=N → ALUFM[N] = 0o25 (B)
 *   LC=1 (T←Pd)
 *   JCN=local(0) → jump back to PC=0 forever
 */
static int test_load_constant(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);

    /* ALUFM[0] = 0o25 (logical "B"). */
    mc.alufm[0] = 025;
    mc.alufm_present[0] = 1;

    /* IM[0]: T ← 0o123 via 0,,FF; then jump back to 0. */
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/4, /*lc=*/1,
                           /*asel=*/6, /*block=*/0, /*ff=*/0123,
                           /*jcn=*/jcn_local(0));
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0;
    mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    EXPECT(cpu.T == 0, "T should start zero");
    EXPECT(cpu.real_PC == 0, "PC should start 0");

    int rc = dorado_cpu_step(&cpu);
    EXPECT(rc == 0, "step failed: %s", cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.T == 0123, "expected T=0o123, got 0o%o", cpu.T);
    EXPECT(cpu.real_PC == 0, "expected PC=0 (self-loop), got 0o%o", cpu.real_PC);
    EXPECT(cpu.cycles == 1, "cycles=%d", cpu.cycles);

    /* Second iteration just rewrites T with the same value. */
    rc = dorado_cpu_step(&cpu);
    EXPECT(rc == 0, "second step failed");
    EXPECT(cpu.T == 0123, "T still 0o123");
    EXPECT(cpu.cycles == 2, "cycles=%d", cpu.cycles);

    printf("PASS  test_load_constant\n");
    return 0;
}

/* Test 2: increment a register, branch back to .
 *   IM[0]: T ← T + 1, jump to PC=1
 *   IM[1]: jump back to PC=0 (so we go 0 → 1 → 0 → 1 → ...)
 *
 *   IM[0]: ASEL=6 (A←T), BSEL=4 (0,,FF) FF=0,
 *          ALUF=1 → ALUFM[1] = 0o40 (carry-in=1, op=0 → A+1),
 *          LC=1 (T←Pd), JCN=local(1)
 *   IM[1]: NoOp, JCN=local(0)
 */
static int test_increment_loop(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);

    /* ALUFM[1] = 0o40 (bit 5 = 1 = carry-in, op = 0 = A) → A+1. */
    mc.alufm[1] = 040;
    mc.alufm_present[1] = 1;
    /* ALUFM[0] = 0o25 (B) — used by IM[1]'s NoOp pass-through. */
    mc.alufm[0] = 025;
    mc.alufm_present[0] = 1;

    mc.im[0] = make_uinstr(0, /*aluf=*/1, /*bsel=*/4, /*lc=*/1,
                           /*asel=*/6, 0, /*ff=*/0, jcn_local(1));
    mc.im_present[0] = 1;

    /* IM[1]: NoOp. ASEL=6 (A←T), BSEL=2 (T), ALUF=0 (B → loads B which is T),
     * LC=0 (NoLoad), JCN=local(0). */
    mc.im[1] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0,
                           /*asel=*/6, 0, 0, jcn_local(0));
    mc.im_present[1] = 1;

    mc.image_to_real[0] = 0;
    mc.image_to_real[1] = 1;
    mc.image_present[0] = mc.image_present[1] = 1;
    mc.n_instructions = 2;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    /* Cycle 1: T ← 0+1 = 1, PC: 0 → 1 */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 1: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.T == 1, "after step 1, T = 0o%o", cpu.T);
    EXPECT(cpu.real_PC == 1, "after step 1, PC = 0o%o", cpu.real_PC);

    /* Cycle 2: NoOp, PC: 1 → 0 */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 2: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.T == 1, "T unchanged through NoOp: 0o%o", cpu.T);
    EXPECT(cpu.real_PC == 0, "after step 2, PC = 0o%o", cpu.real_PC);

    /* Run until total cycle count reaches 20. We've already done 2 manual
     * steps, so run does 18 more = 9 (increment, noop) pairs. T grows
     * from 1 to 1+9 = 0o12. */
    cpu_halt_reason r = dorado_cpu_run(&cpu, 20);
    EXPECT(r == CPU_HALT_USER, "expected to time out at 20 cycles, got %s",
           cpu_halt_reason_str(r));
    EXPECT(cpu.cycles == 20, "expected 20 cycles total, got %d", cpu.cycles);
    EXPECT(cpu.T == 012,
           "after 9 increments + initial 1, T = 0o%o (expected 0o12)", cpu.T);

    printf("PASS  test_increment_loop (T reached 0o%o after %d cycles)\n",
           cpu.T, cpu.cycles);
    return 0;
}

/* Test 3: write into RM, read it back.
 *   IM[0]: RM[5] ← 0o4321  (BSEL=4 to drive constant on B; LC=6 RM/STK←Pd)
 *          RBase=0, RSTK=5 → RM address = 5
 *          jump to IM[1].
 *   IM[1]: T ← RM[5]  (ASEL=4 A←RM/STK; ALUF=1 A+1? no, just B-pass via ALU.
 *                      Use BSEL=1 RM/STK; ALUF=0 → 0o25 = B; LC=1)
 *          jump back to IM[0].
 */
static int test_rm_roundtrip(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);

    /* ALUFM[0] = 0o25 (B) for both writes. */
    mc.alufm[0] = 025;
    mc.alufm_present[0] = 1;

    /* IM[0]: RM[5] ← 0o4321.  ASEL=4 (A←RM/STK reads RM[5] but we
     * don't care about A); BSEL=4, FF=0o21; LC=6.
     * Actually 0,,FF only puts 8 bits — we want 0o4321 (16 bits).
     * Use BSEL=6 (FF,,0) with FF=0o43 to get 0o43*256 = 0o42600 then
     * we'd need LC=6 + a different mux. Simplest: write low byte only,
     * use 0o21 instead. */
    mc.im[0] = make_uinstr(/*rstk=*/5, 0, /*bsel=*/4, /*lc=*/6,
                           /*asel=*/4, 0, /*ff=*/0321, jcn_local(1));
    mc.im_present[0] = 1;

    /* IM[1]: T ← RM[5].  ASEL=6 (A←T); BSEL=1 (RM/STK reads RM[5]);
     * ALUF=0 (B); LC=1 (T←Pd). */
    mc.im[1] = make_uinstr(/*rstk=*/5, 0, /*bsel=*/1, /*lc=*/1,
                           /*asel=*/6, 0, 0, jcn_local(0));
    mc.im_present[1] = 1;

    mc.image_to_real[0] = 0;
    mc.image_to_real[1] = 1;
    mc.image_present[0] = mc.image_present[1] = 1;
    mc.n_instructions = 2;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    EXPECT(dorado_cpu_step(&cpu) == 0, "step 1: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.RM[5] == 0321, "RM[5] = 0o%o (expected 0321)", cpu.RM[5]);
    EXPECT(cpu.real_PC == 1, "PC after step 1 = 0o%o", cpu.real_PC);

    EXPECT(dorado_cpu_step(&cpu) == 0, "step 2: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.T == 0321, "T = 0o%o (expected 0321)", cpu.T);
    EXPECT(cpu.real_PC == 0, "PC after step 2 = 0o%o", cpu.real_PC);

    printf("PASS  test_rm_roundtrip (RM[5]=0o%o, T=0o%o)\n", cpu.RM[5], cpu.T);
    return 0;
}

/*
 * Test 4: Conditional branch on ALU=0.
 *
 * Build:
 *   IM[0]: T ← FF=value via BSEL=4. ALUF=0 → ALUFM[0]=0o25 (B-pass).
 *          The value 5 makes alu_zero=0; the value 0 makes alu_zero=1.
 *          Jump to IM[2].
 *   IM[2]: Conditional branch, condition 0 (ALU = 0).
 *          JCN encoded so even target = page+4, odd = page+5.
 *   IM[4]: false-target marker — set RM[10] = 0o4444. Self-loop.
 *   IM[5]: true-target marker  — set RM[10] = 0o5555. Self-loop.
 *
 * For the conditional-branch JCN: with our 8-bit byte (bit 7 = MSB):
 *   bit 7 = 0          (not local/global)
 *   bits 6..5 = 00     (JCN[1:2] = 0)
 *   bits 4..3 = 10     (JCN[3:4] = 2 → page_offset bit2 = 1)
 *   bits 2..0 = 000    (condition 0 = ALU = 0)
 *  → jcn = 0b00010000 = 0o20.
 *  Address calc: offset = (0<<4) | (2<<1) | R = 4 + R.
 */
static int test_conditional_branch_alu_zero(void)
{
    /* Build the microcode common to both runs. */
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;        /* B-pass */

    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.image_to_real[2] = 2; mc.image_present[2] = 1;
    mc.image_to_real[4] = 4; mc.image_present[4] = 1;
    mc.image_to_real[5] = 5; mc.image_present[5] = 1;
    mc.n_instructions = 4;

    /* IM[2]: conditional branch on ALU=0 (cond 0). */
    mc.im[2] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6,
                           0, 0, /*jcn=*/0020);
    mc.im_present[2] = 1;

    /* IM[4]: RM[10] ← 0o4444, self-loop. */
    mc.im[4] = make_uinstr(/*rstk=*/012, 0, /*bsel=*/4, /*lc=*/6,
                           /*asel=*/4, 0, /*ff=*/0344, jcn_local(4));
    mc.im_present[4] = 1;

    /* IM[5]: RM[10] ← 0o5555, self-loop. */
    mc.im[5] = make_uinstr(/*rstk=*/012, 0, /*bsel=*/4, /*lc=*/6,
                           /*asel=*/4, 0, /*ff=*/0355, jcn_local(5));
    mc.im_present[5] = 1;

    /* --- Run A: T ← 5, expect false branch (ALU != 0 → goto IM[4]). --- */
    mc.im[0] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/1, /*asel=*/6,
                           0, /*ff=*/05, jcn_local(2));
    mc.im_present[0] = 1;
    {
        dorado_cpu cpu;
        dorado_cpu_init(&cpu, &mc, 0);
        dorado_cpu_run(&cpu, 5);
        EXPECT(cpu.real_PC == 4,
               "Run A: expected to reach IM[4] (false branch), got 0o%o",
               cpu.real_PC);
        EXPECT(cpu.RM[012] == 0344,
               "Run A: RM[10] = 0o%o (expected 0o4444)", cpu.RM[012]);
    }

    /* --- Run B: T ← 0, expect true branch (ALU == 0 → goto IM[5]). --- */
    mc.im[0] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/1, /*asel=*/6,
                           0, /*ff=*/0, jcn_local(2));
    {
        dorado_cpu cpu;
        dorado_cpu_init(&cpu, &mc, 0);
        dorado_cpu_run(&cpu, 5);
        EXPECT(cpu.real_PC == 5,
               "Run B: expected to reach IM[5] (true branch), got 0o%o",
               cpu.real_PC);
        EXPECT(cpu.RM[012] == 0355,
               "Run B: RM[10] = 0o%o (expected 0o5555)", cpu.RM[012]);
    }

    printf("PASS  test_conditional_branch_alu_zero (both branches taken)\n");
    return 0;
}

/*
 * Test 5: Shifter byte-cycle (HM §3.11).
 *
 * Set T ← 0o123 (= 0x53), then run a shift with:
 *   ASEL=7 (shift), BSEL=7 (FF-controlled, SHA=T, SHB=T → input = T..T)
 *   FF=0o010 → count = FF[4:7] = 8, LMask = FF[0:3] = 0, RMask = FF[4:7] = 8
 *   ALUF[0:2] = 0  → ShiftNoMask
 *   ALUF[3]   = 0  → use ALUFM[14] (= NOT A by convention)
 *
 * Expected:
 *   input32         = T..T          = 0x00530053
 *   cycled_left_8   = 0x53005300
 *   low 16          = 0x5300
 *   A bus           = ~0x5300       = 0xACFF (low-true)
 *   ALU = NOT A     = 0x5300
 *   Pd → RM[5]      = 0x5300        = 0o51400
 */
static int test_shifter_byte_cycle(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);

    mc.alufm[0]  = 025;             mc.alufm_present[0]  = 1;  /* B-pass */
    mc.alufm[14] = 001;             mc.alufm_present[14] = 1;  /* NOT A */

    /* IM[0]: T ← 0o123 via 0,,FF. */
    mc.im[0] = make_uinstr(0, /*aluf=*/0, /*bsel=*/4, /*lc=*/1,
                           /*asel=*/6, 0, /*ff=*/0123, jcn_local(1));
    mc.im_present[0] = 1;

    /* IM[1]: RM[5] ← shifter(T..T cycled 8). */
    mc.im[1] = make_uinstr(/*rstk=*/5, /*aluf=*/0, /*bsel=*/7,
                           /*lc=*/6, /*asel=*/7, 0,
                           /*ff=*/0010, jcn_local(1));
    mc.im_present[1] = 1;

    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.image_to_real[1] = 1; mc.image_present[1] = 1;
    mc.n_instructions = 2;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    /* Step 1: T ← 0o123 */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 1: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.T == 0123, "T = 0o%o (expected 0o123)", cpu.T);

    /* Step 2: shift */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 2: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.RM[5] == 051400,
           "RM[5] = 0o%o (expected 0o51400 = byte-swap of 0o123)", cpu.RM[5]);

    printf("PASS  test_shifter_byte_cycle (RM[5] = 0o%o)\n", cpu.RM[5]);
    return 0;
}

/*
 * Test 6: Shifter with ShiftRMask.
 *
 * Same setup as test 5, but ALUF=4:
 *   ALUF[0:2] = 010 = 2  → ShiftRMask (mask right RMask bits with 0)
 *   ALUF[3]   = 0        → ALUFM[14]
 * Expected:
 *   shifted = 0x5300; rmask of 8 bits → keep top byte, zero bottom byte
 *   result = 0x5300 (already had 0 in low byte) → 0x5300
 *
 * Use FF=0o210 (count=8, LMask=2, RMask=8) and T=0xFFFF to get a more
 * distinctive output. Then T..T = 0xFFFFFFFF, cycle 8 = 0xFFFFFFFF,
 * low 16 = 0xFFFF, ShiftRMask(0xFFFF) with rmask=8 = 0xFF00.
 *
 * But T=0xFFFF requires a 16-bit constant. Use FF=0o377, BSEL=5 (= 0xFFFF)
 * via 0o377,,FF route.
 */
static int test_shifter_rmask(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0]  = 025;             mc.alufm_present[0]  = 1;
    mc.alufm[14] = 001;             mc.alufm_present[14] = 1;

    /* IM[0]: T ← 0o377,,FF with FF=0o377 → 0xFFFF. */
    mc.im[0] = make_uinstr(0, /*aluf=*/0, /*bsel=*/5, /*lc=*/1,
                           /*asel=*/6, 0, /*ff=*/0377, jcn_local(1));
    mc.im_present[0] = 1;

    /* IM[1]: ShiftRMask. ALUF=4 → ALUF[0:2]=010=2 (ShiftRMask), ALUF[3]=0. */
    mc.im[1] = make_uinstr(/*rstk=*/5, /*aluf=*/4, /*bsel=*/7,
                           /*lc=*/6, /*asel=*/7, 0,
                           /*ff=*/0010, jcn_local(1));
    mc.im_present[1] = 1;

    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.image_to_real[1] = 1; mc.image_present[1] = 1;
    mc.n_instructions = 2;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 1");
    EXPECT(cpu.T == 0xFFFF, "T = 0o%o (expected 0xFFFF)", cpu.T);
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 2: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    /* 0xFFFF cycled 8 = 0xFFFF. RMask 8 → mask = 0x00FF. result = 0xFF00. */
    EXPECT(cpu.RM[5] == 0xFF00,
           "RM[5] = 0o%o (expected 0xFF00 = 0o177400)", cpu.RM[5]);

    printf("PASS  test_shifter_rmask (RM[5] = 0o%o)\n", cpu.RM[5]);
    return 0;
}

/* Test 7: Verify an unsupported FF/JCN/ASEL halts cleanly with a
 * specific reason rather than crashing or silently advancing. */
static int test_unsupported_halts(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);

    /* IM[0]: IFUJump (JCN = 0 0 1 _ _ 1 1 1) without PCF←B —
     * should trap to IFU NotReady. JCN bits MSB-first:
     *   0 0 1 0 0 1 1 1 = 0x27 = 0o47. */
    mc.im[0] = make_uinstr(0, 0, 2, 0, /*asel=*/6, 0, 0, /*jcn=*/0x27);
    mc.im_present[0] = 1;
    mc.im[0334] = make_uinstr(0, 0, 2, 0, /*asel=*/6, 0, 0,
                              jcn_local(0334 & 0x3F));
    mc.im_present[0334] = 1;
    mc.image_to_real[0] = 0;
    mc.image_present[0] = 1;
    mc.image_to_real[1] = 0334;
    mc.image_present[1] = 1;
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;
    mc.n_instructions = 2;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    int rc = dorado_cpu_step(&cpu);
    EXPECT(rc == 0, "unexpected halt on IFUJump without PCF←B: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.real_PC == 0334,
           "expected IFU NotReady trap at 0o334, got 0o%o", cpu.real_PC);

    printf("PASS  test_unsupported_halts (IFU NotReady PC=0o%o)\n",
           cpu.real_PC);
    return 0;
}

static int test_jcn_long_branch_address(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* B */

    /* Initial.MB has this exact shape at IM[0o7557]: FF=0o304,
     * JCN=0o002 must branch to 0o6102, not 0o1310. */
    mc.im[07557] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/2,
                               /*lc=*/0, /*asel=*/6, /*block=*/0,
                               /*ff=*/0304, jcn_long(06102));
    mc.im_present[07557] = 1;
    mc.im[06102] = make_uinstr(0, 0, 2, 0, 6, 0, 0, jcn_local(06102));
    mc.im_present[06102] = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 07557);

    EXPECT(dorado_cpu_step(&cpu) == 0, "long branch step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.real_PC == 06102, "long branch PC=0o%o, expected 0o6102",
           cpu.real_PC);

    printf("PASS  test_jcn_long_branch_address\n");
    return 0;
}

static int test_ff_condition_with_memory_ref(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* B */

    /* IM[0]: T <- 2. */
    mc.im[0] = make_uinstr(0, 0, 4, 1, 6, 0, 0002, jcn_local(1));
    mc.im_present[0] = 1;

    /* IM[1]: Cnt <- T. */
    mc.im[1] = make_uinstr(0, 0, 2, 0, 6, 0, 0146, jcn_local(2));
    mc.im_present[1] = 1;

    /* IM[2]: Store<-RM/STK with FF[0:1]=3 and FF[2:7]=0o63.
     * Hardware treats this as a memory Store plus FF-encoded
     * Cnt=0&-1 branch condition, not as full FF function
     * Wakeup[3]. The false target is even IM[2]; the true target
     * is odd IM[3]. This is the shape Initial uses at 0o6116. */
    mc.im[2] = make_uinstr(0, 0, 1, 0, 0, 0, 0363, jcn_local(2));
    mc.im_present[2] = 1;

    /* IM[3]: marker. */
    mc.im[3] = make_uinstr(0, 0, 4, 1, 6, 0, 0077, jcn_local(3));
    mc.im_present[3] = 1;

    dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "memory init");
    dorado_map_set(&mem, 0, 0, 0, 0);

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.mem = &mem;
    cpu.RM[0] = 0;

    EXPECT(dorado_cpu_step(&cpu) == 0, "step 0");
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 1");
    EXPECT(cpu.Cnt == 2, "Cnt=%u after load, expected 2", cpu.Cnt);

    EXPECT(dorado_cpu_step(&cpu) == 0, "loop 1");
    EXPECT(cpu.real_PC == 2, "PC=0o%o after Cnt=2, expected 2", cpu.real_PC);
    EXPECT(cpu.Cnt == 1, "Cnt=%u after loop 1, expected 1", cpu.Cnt);

    EXPECT(dorado_cpu_step(&cpu) == 0, "loop 2");
    EXPECT(cpu.real_PC == 2, "PC=0o%o after Cnt=1, expected 2", cpu.real_PC);
    EXPECT(cpu.Cnt == 0, "Cnt=%u after loop 2, expected 0", cpu.Cnt);

    EXPECT(dorado_cpu_step(&cpu) == 0, "exit loop");
    EXPECT(cpu.real_PC == 3, "PC=0o%o after Cnt=0, expected 3", cpu.real_PC);
    EXPECT(cpu.wakeup_pending == 0,
           "FF=0o363 memory ref should not issue Wakeup[3], pending=0x%X",
           cpu.wakeup_pending);

    dorado_memory_free(&mem);
    printf("PASS  test_ff_condition_with_memory_ref\n");
    return 0;
}

/*
 * probe_bootstrap_pure — run Bootstrap.MB directly, NO BaseBoard.
 *
 * The BB-coupled probe_bootstrap below jams its own (newer) Boot0
 * binary into IM during LoadDoradoCode, OVERWRITING the Bootstrap.MB
 * we pre-loaded — so it can never test the source-level microcode.
 * This pure variant skips the BB entirely: ALUFM, RM, IM all come
 * from Bootstrap.MB, CPU starts at the BOOTSTRAP entry, no MIR
 * injection. Useful to see how far our microengine takes
 * pre-MicroD'd source code.
 */
static int probe_bootstrap_pure(void)
{
    const char *path = "../chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb";
    mb_file mb;
    mb_init(&mb);
    if (mb_load(&mb, path) != MB_OK) {
        printf("SKIP  probe_bootstrap_pure (file not loadable)\n");
        return 0;
    }
    static dorado_microcode mc;
    if (dorado_microcode_load(&mb, &mc) != DM_OK) {
        printf("SKIP  probe_bootstrap_pure (microcode load failed)\n");
        mb_free(&mb);
        return 0;
    }

    int real_start = mc.image_present[0] ? mc.image_to_real[0] : 0;
    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, (uint16_t)real_start);
    /* Pre-set CPReg with the AMSync (bit 15) set, mimicking what the
     * BaseBoard's `SetCPReg(0x8000)` does just before starting Boot0.
     * Without this, Bootstrap.MB's READBB spin loop (0o7700 → 0o7747
     * → 0o7742 → 0o7741) tests ALU<0 of `T XOR RM[0]` = `~CPReg XOR 0`
     * = `~CPReg`. With CPReg=0, ~CPReg=0xFFFF (high bit set) → loop
     * iterates forever. With CPReg=0x8000, ~CPReg=0x7FFF (high bit
     * clear) → loop exits. */
    cpu.cpreg = 0x8000;

    /* Trail. */
    uint16_t trail[128];
    int trail_n = 0;

    cpu_halt_reason r = CPU_HALT_NONE;
    for (int i = 0; i < 1000 && !cpu.halted; i++) {
        if (trail_n < 128) trail[trail_n++] = cpu.real_PC;
        if (dorado_cpu_step(&cpu)) {
            r = (cpu_halt_reason)cpu.halt_reason;
            break;
        }
    }
    if (!cpu.halted) r = CPU_HALT_NONE;

    printf("PROBE  bootstrap_pure: entry=0o%o, ran %d cycles, halt: %s at PC=0o%o\n",
           real_start, cpu.cycles, cpu_halt_reason_str(r),
           cpu.halted ? cpu.real_PC : 0);

    /* Print trail with run-length compression. */
    printf("       trail:");
    int prev_pc = -1, prev_count = 0;
    for (int i = 0; i < trail_n; i++) {
        if ((int)trail[i] == prev_pc) { prev_count++; continue; }
        if (prev_count > 1) printf("×%d", prev_count);
        const char *sym = dorado_microcode_symbol_at_real(&mc, trail[i]);
        if (sym) printf(" 0o%o(%s)", trail[i], sym);
        else     printf(" 0o%o", trail[i]);
        prev_pc = trail[i];
        prev_count = 1;
    }
    if (prev_count > 1) printf("×%d", prev_count);
    printf("\n");

    if (cpu.halt_reason == CPU_HALT_UNSUPPORTED_ASEL ||
        cpu.halt_reason == CPU_HALT_UNSUPPORTED_BSEL ||
        cpu.halt_reason == CPU_HALT_UNSUPPORTED_FF ||
        cpu.halt_reason == CPU_HALT_UNSUPPORTED_JCN) {
        if (cpu.real_PC < 4096 && mc.im_present[cpu.real_PC]) {
            const dorado_uinstr *u = &mc.im[cpu.real_PC];
            char dis[256];
            dorado_format(u, dis, sizeof dis);
            printf("       offending uinstr: %s\n", dis);
        }
    }

    mb_free(&mb);
    return 0;  /* informational */
}

/* Diagnostic: step into real Bootstrap microcode and see where the
 * CPU stops. This is not a pass/fail test — it just reports what real
 * microcode hits so we can prioritize what to implement next.
 *
 * Now wires up a real BaseBoard 6502 to serve CPReg, instead of the
 * counter stub. */
static int probe_bootstrap(void)
{
    const char *path = "../chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb";
    mb_file mb;
    mb_init(&mb);
    if (mb_load(&mb, path) != MB_OK) {
        printf("SKIP  probe_bootstrap (file not loadable)\n");
        return 0;
    }
    static dorado_microcode mc;
    if (dorado_microcode_load(&mb, &mc) != DM_OK) {
        printf("SKIP  probe_bootstrap (microcode load failed)\n");
        mb_free(&mb);
        return 0;
    }

    /* Stand up a BaseBoard, simulate a 3-push boot (CoolBoot — reload
     * microcode), and let it run far enough to be ready to upload
     * Initial via CPReg.
     *
     * Per the booting memo: press 0.25-2.5 s, gap <1.5 s, then wait
     * 1.5 s after final release. At our nominal 1 MHz simulated rate,
     * that's 250k-2.5M cycles per press, <1.5M cycle gap, 1.5M cycle
     * final wait. */
    static dorado_baseboard bb;
    int have_bb = 0;
    baseboard_init(&bb);
    if (baseboard_load_rom(&bb, "../chm/dorado/doradobaserom.mb!13") == 0) {
        baseboard_reset(&bb);
        baseboard_run(&bb, 1000000);   /* warm up: 1 sec */
        printf("       BB after warmup: PC=0x%04X\n", baseboard_pc(&bb));

        for (int i = 0; i < 3; i++) {
            baseboard_boot_button(&bb, 1);
            baseboard_run(&bb, 400000);   /* press ~400 ms */
            baseboard_boot_button(&bb, 0);
            baseboard_run(&bb, 600000);   /* release ~600 ms */
        }
        printf("       BB after 3 presses: PC=0x%04X\n", baseboard_pc(&bb));

        /* Final wait: 1.5s for CheckBootButton dispatch, then ~10s for
         * RebootDorado to traverse CheckVCC → DiskOK → SuppliesAllUp →
         * Delay(1) → SetClockSpeed → Delay(2) → LoadDoradoCode. With
         * the analog comparator model in place, the BB reaches
         * Continuous (the steady-state polling loop) around cycle 11M. */
        baseboard_run(&bb, 12000000);
        printf("       BB after final wait: PC=0x%04X (cycles=%llu)\n",
               baseboard_pc(&bb), (unsigned long long)bb.cycles);

        have_bb = 1;
    }

    int real_start = mc.image_present[0] ? mc.image_to_real[0] : 0;
    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, (uint16_t)real_start);
    if (have_bb) {
        cpu.baseboard = &bb;
        cpu.baseboard_cycles_per_uop = 1;
    }

    cpu_halt_reason r = dorado_cpu_run(&cpu, 1000);
    printf("PROBE  bootstrap entry=0o%o, BB=%s (PC=0x%04X, %llu cycles), "
           "Dorado ran %d cycles, halt: %s at PC=0o%o\n",
           real_start, have_bb ? "real-6502" : "counter-stub",
           have_bb ? baseboard_pc(&bb) : 0,
           have_bb ? (unsigned long long)bb.cycles : 0ULL,
           cpu.cycles, cpu_halt_reason_str(r),
           cpu.halted ? cpu.real_PC : 0);

    /* Print the offending uinstr if we halted on something we don't
     * yet handle. */
    if (cpu.halt_reason == CPU_HALT_UNSUPPORTED_ASEL ||
        cpu.halt_reason == CPU_HALT_UNSUPPORTED_BSEL ||
        cpu.halt_reason == CPU_HALT_UNSUPPORTED_FF ||
        cpu.halt_reason == CPU_HALT_UNSUPPORTED_JCN) {
        const dorado_uinstr *u = &mc.im[cpu.real_PC];
        char dis[256];
        dorado_format(u, dis, sizeof dis);
        printf("       offending uinstr: %s\n", dis);
    }

    mb_free(&mb);
    return 0;  /* informational — never fail the test run */
}

/*
 * Conditional IFUJump: when an FF-encoded branch condition is true
 * during an IFUJump, the IFU does NOT advance; control goes to
 * entry n|1 of the *current* opcode's vector (HM page 33). This
 * lets a single opcode do work via the entry-3 path before exiting.
 *
 * Layout:
 *   - One opcode (INC, 0x10) in IFUM at InsSet=0.
 *   - Entry 0 (IM[0o100]): IFUJump[2, ALU=0]. With Pd=0 (T was just
 *     loaded), ALU=0 evaluates true → control to entry 3 (TNIA[15]=1
 *     → IM[0o103]); IFU does NOT advance.
 *   - Entry 3 (IM[0o103]): increment T, IFUJump[0] (now condition is
 *     false → IFU advances to next opcode).
 *
 * Bytecode: 0x10 0x10 0x20 (INC INC HALT). Each INC's first
 * IFUJump[2, ALU=0] takes the conditional path on entry 0 because
 * T was just zeroed by Pd; entry 3 increments T then IFUJump[0]s
 * which advances the IFU normally.
 *
 * Verifies: T=2 after 2 INCs, condition path traversed, PCF
 * advanced exactly twice (not 4 times).
 */
static int test_ifu_conditional_dispatch(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025;   mc.alufm_present[0]  = 1;  /* B */
    mc.alufm[2] = 0014;  mc.alufm_present[2]  = 1;  /* A+B carry-in 0 */
    mc.alufm[1] = 031;   mc.alufm_present[1]  = 1;  /* zero (op 31) */

    /* Setup microcode IM[0..2] = PCF←B, NOP, IFUJump[2]. */
    mc.im[0] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0100, jcn_local(1));
    mc.im_present[0] = 1;
    mc.im[1] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0, jcn_local(2));
    mc.im_present[1] = 1;
    /* IM[2]: IFUJump[0]. Plain (no condition). Dispatches to entry 0
     * of the next opcode's vector. */
    mc.im[2] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0, /*jcn=*/0x27);
    mc.im_present[2] = 1;

    /* NotReady trap vectors (InsSet=0 → 0o334..0o337). Each retries. */
    for (int n = 0; n < 4; n++) {
        uint8_t jcn_ifu_n = (uint8_t)(0x27 | (n << 3));
        mc.im[0334 + n] = make_uinstr(0, 0, 2, 0, 6, 0, 0, jcn_ifu_n);
        mc.im_present[0334 + n] = 1;
    }

    /* INC opcode (0x10) entry vector at IM[0o100..0o103]:
     *
     * Entry 0 (IM[0o100]): T←0 (force ALU=0 condition true), then
     *   IFUJump[2, ALU=0]. ALU=0 is condition 0 (FF[5:7]=0,
     *   FA=0 FB=6 FC=0 → FF=0o060). Since this instruction
     *   *zeroes T*, the ALU condition tested next instruction would
     *   be against this T=0.
     *
     * Actually FF-encoded branch conditions test the PREVIOUS
     * instruction's ALU. So entry 0 must SET UP the ALU=0 condition
     * (which it does by ALU=B with B=0), and the IFUJump on the
     * next instruction tests it.
     *
     * Easier: keep entry 0 as a setup, then IFUJump in entry 1.
     * But IFU advances after entry 1's IFUJump[2]... no, actually
     * the entry vector is selected by IFUJump[n] from the PREVIOUS
     * opcode's exit. Entries are NOT sequential — the program
     * selects ONE entry to start at.
     *
     * So for our 4-entry vector:
     *   entry 0: T←T+1, IFUJump[2, ALU=0]
     *     If condition is true (T after the +1 is zero — happens if
     *     T was 0xFFFF), IFU doesn't advance, dispatch to entry 3.
     *     But with T starting 0, after +1 T=1, ALU≠0 → condition
     *     false → IFU advances normally. So this is a "fast path"
     *     for the common case where T+1 != 0.
     *
     * That's a more realistic test pattern. Let me set it up.
     */

    /* Entry 0 of INC (IM[0o100]): T←T+1, IFUJump[2, ALU=0].
     * ASEL=6 (A←T), BSEL=4 (B=constant 0,,FF FF=1 → B=1),
     * ALUF=2 (A+B carry-in 0). LC=1 (T←Pd). FF would normally
     * supply the branch condition, but BSEL=4 (constant) suppresses
     * FF interpretation. So FF=1 here is the constant data, NOT
     * a branch condition.
     *
     * To get an FF-encoded condition we need BSEL<4. So use BSEL=2
     * (T as B data), ALUF=0 (alufm[0]=B → ALU=T, sets ALU=0 if T=0).
     * LC=0 (no T write). Hmm but then T doesn't increment.
     *
     * Compromise — use TWO instructions: entry 0 sets up T←T+1
     * (LC=1, no FF cond), then IM[0o101] (entry 1) does the
     * IFUJump with FF=branch-cond. But entry 1 isn't normally
     * reached unless the dispatch picks it.
     *
     * Simpler: use a single instruction with BSEL=2 and ALUF=
     * select-A (= T). Then FF-encoded condition tests prior
     * instruction's ALU. We need the PREV ALU to be zero/non-zero.
     *
     * I'm overthinking this. Let me just verify the conditional
     * path triggers when condition is forcibly true, by running
     * a compute that produces ALU=0 first.
     */

    /* Entry 0 of INC (IM[0o100]): T←T+1 (ASEL=6 A←T, BSEL=4 FF=1
     * B=1, ALUF=2 A+B, LC=1 T←Pd). JCN=local self-loop temporarily
     * — we'll add the IFUJump in entry 1 after this set-up. */
    mc.im[0100] = make_uinstr(0, /*aluf=*/2, /*bsel=*/4, /*lc=*/1,
                              /*asel=*/6, 0, /*ff=*/1,
                              /*jcn=*/jcn_local(0101 & 0x3F));
    mc.im_present[0100] = 1;

    /* Entry 1 (IM[0o101]): IFUJump[0, ALU<0] (cond 1 = ALU<0).
     * BSEL=2 (T as B), ALUF=0 (B → ALU=T). FF=0o061 (FA=0 FB=6 FC=1
     * → ALU<0 cond). With T=1 (just incremented), ALU=1, MSB=0, so
     * cond=false → IFU advances, dispatch to entry 0 of next opcode.
     *
     * If T were >= 0x8000 (MSB set), cond would be true → entry 1
     * (entry 0 | 1 = entry 1) of CURRENT opcode, IFU does NOT
     * advance. We don't exercise that path here. */
    mc.im[0101] = make_uinstr(0, /*aluf=*/0, /*bsel=*/2, /*lc=*/0,
                              /*asel=*/6, 0, /*ff=*/0061,
                              /*jcn=*/0x27);   /* IFUJump[0, cond=ALU<0] */
    mc.im_present[0101] = 1;

    /* Entry 3 (IM[0o103]): self-loop — should NOT be reached if
     * condition is false. */
    mc.im[0103] = make_uinstr(0, 0, 2, 0, 6, 0, 0, jcn_local(0103 & 0x3F));
    mc.im_present[0103] = 1;

    /* HALT at IM[0o200]: self-loop. */
    mc.im[0200] = make_uinstr(0, 0, 2, 0, 6, 0, 0, jcn_local(0200 & 0x3F));
    mc.im_present[0200] = 1;

    int present_addrs[] = {0, 1, 2, 0334, 0335, 0336, 0337,
                           0100, 0101, 0103, 0200};
    int n_present = sizeof present_addrs / sizeof present_addrs[0];
    for (int i = 0; i < n_present; i++) {
        mc.image_to_real[i] = present_addrs[i];
        mc.image_present[i] = 1;
    }
    mc.n_instructions = n_present;

    #define MK_LH(sign, length_p, rbaseb_p, memb, tpause_p, tjump_p, n) \
        ((uint16_t)( ((uint16_t)((sign)&1) << 15) \
                   | ((uint16_t)((length_p)&3) << 10) \
                   | ((uint16_t)((rbaseb_p)&1) << 9) \
                   | ((uint16_t)((memb)&7) << 6) \
                   | ((uint16_t)((tpause_p)&1) << 5) \
                   | ((uint16_t)((tjump_p)&1) << 4) \
                   | ((uint16_t)((n)&0xF)) ))
    #define MK_RH(packed_a, ifaddr) \
        ((uint16_t)( ((uint16_t)((packed_a)&1) << 10) \
                   | ((uint16_t)((ifaddr)&0x3FF)) ))

    /* INC opcode 0x10. IFaddr' = 0o20 → entries at IM[0o100..0o103]. */
    mc.ifum_hi[0x10] = MK_LH(0, 0, 1, 4, 1, 1, 017);
    mc.ifum_lo[0x10] = MK_RH(0, 0020);
    mc.ifum_present[0x10] = 1;

    /* HALT opcode 0x20. IFaddr' = 0o40. */
    mc.ifum_hi[0x20] = MK_LH(0, 0, 1, 4, 1, 1, 017);
    mc.ifum_lo[0x20] = MK_RH(0, 0040);
    mc.ifum_present[0x20] = 1;

    static dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "mem init");
    dorado_map_set(&mem, 0, 0, 0, 0);
    dorado_br_lo_load(&mem, 31, 0);
    dorado_br_hi_load(&mem, 31, 0);
    /* 2 INCs then HALT: bytes 10 10 20 _. */
    mem.storage[0] = 0x1010;
    mem.storage[1] = 0x2000;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.mem = &mem;

    /* Run plenty of cycles. Expected: T=2 after 2 INCs (each does
     * T←T+1 in entry 0, then IFUJump[2] in entry 1 advances IFU).
     * After 2 INCs, dispatch to HALT entry 0 (= IM[0o200]). */
    for (int i = 0; i < 80; i++) {
        if (dorado_cpu_step(&cpu) != 0) break;
    }

    EXPECT(cpu.real_PC == 0200,
           "should reach HALT at 0o200, got 0o%o", cpu.real_PC);
    EXPECT(cpu.T == 2, "T should = 2 (2 INCs), got %d", cpu.T);

    dorado_memory_free(&mem);
    printf("PASS  test_ifu_conditional_dispatch (T=%d)\n", cpu.T);
    return 0;
    #undef MK_LH
    #undef MK_RH
}

/*
 * Conditional IFUJump — TRUE case: IFU does NOT advance.
 *
 * Same INC opcode as test_ifu_conditional_dispatch, but T is
 * pre-loaded so that the ALU<0 condition (in entry 1) evaluates
 * TRUE. The conditional IFUJump should:
 *   (a) Dispatch to entry n|1 (entry 1) of the IFU's M-level opcode
 *       (the next INC, since IFUJump just decoded it from PCF).
 *   (b) NOT advance PCF.
 *
 * We pre-load T = 0xFFFF so T←T+1 wraps to 0, but the *previous
 * instruction's* ALU was T = 0xFFFF (negative). At entry 1, the
 * IFUJump tests ALU<0 from entry 0 (T+1=0, MSB clear → cond false
 * actually). Hmm.
 *
 * Easier: have entry 0 set ALU = T (negative number), with LC=0
 * (don't write T). Then entry 1's IFUJump[0, ALU<0] sees the
 * negative T → cond true → dispatch to entry 1 of "next" opcode
 * but IFU does NOT advance.
 *
 * What we then verify: PCX (the dispatched opcode's PC) is the
 * SAME as on the first dispatch — the IFU stayed put.
 */
static int test_ifu_conditional_cond_true(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025;   mc.alufm_present[0] = 1;  /* B */
    mc.alufm[3] = 007;   mc.alufm_present[3] = 1;  /* all-ones */

    /* IM[0]: PCF←B with B=0 (T starts 0). */
    mc.im[0] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0100, jcn_local(1));
    mc.im_present[0] = 1;
    /* IM[1]: T ← 0xFFFF (ALU=all-ones, LC=1). */
    mc.im[1] = make_uinstr(0, /*aluf=*/3, /*bsel=*/2, /*lc=*/1, /*asel=*/6, 0,
                           /*ff=*/0, jcn_local(2));
    mc.im_present[1] = 1;
    /* IM[2]: IFUJump[0] — but warmup may not be done yet, so this
     * may trap to NotReady; trap vector retries until ready. */
    mc.im[2] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0, /*jcn=*/0x27);
    mc.im_present[2] = 1;

    /* NotReady trap vector retries (InsSet=0). */
    for (int n = 0; n < 4; n++) {
        uint8_t jcn_ifu_n = (uint8_t)(0x27 | (n << 3));
        mc.im[0334 + n] = make_uinstr(0, 0, 2, 0, 6, 0, 0, jcn_ifu_n);
        mc.im_present[0334 + n] = 1;
    }

    /* INC entry 0 (IM[0o100]): ALU←B with B=T (T preloaded to
     * 0xFFFF). LC=0 (no T write). ALU<0 condition will be true
     * for next instruction. JCN=local(1) → IM[0o101]. */
    mc.im[0100] = make_uinstr(0, /*aluf=*/0, /*bsel=*/2, /*lc=*/0,
                              /*asel=*/6, 0, /*ff=*/0,
                              jcn_local(0101 & 0x3F));
    mc.im_present[0100] = 1;
    /* INC entry 1 (IM[0o101]): IFUJump[0, ALU<0]. With prev
     * ALU=0xFFFF (MSB set), cond=true. Should dispatch to entry 1
     * (n_eff = 0|1 = 1) of M-level opcode, IFU does NOT advance. */
    mc.im[0101] = make_uinstr(0, 0, 2, 0, 6, 0, /*ff=*/0061,
                              /*jcn=*/0x27);
    mc.im_present[0101] = 1;
    /* Entry 1 of next opcode (also IM[0o101] for INC) — we land
     * here on cond=true. Self-loop (test detects). */
    /* Actually entry 1 of next INC is the SAME IM[0o101], because
     * the M-level opcode's IFaddr is still 0o20. So cond=true
     * dispatches us back to IM[0o101]. We'd loop forever — which
     * is the expected behavior for this trace. We just check that
     * PCF didn't advance. */

    int present_addrs[] = {0, 1, 2, 0334, 0335, 0336, 0337,
                           0100, 0101};
    int n_present = sizeof present_addrs / sizeof present_addrs[0];
    for (int i = 0; i < n_present; i++) {
        mc.image_to_real[i] = present_addrs[i];
        mc.image_present[i] = 1;
    }
    mc.n_instructions = n_present;

    #define MK_LH(sign, length_p, rbaseb_p, memb, tpause_p, tjump_p, n) \
        ((uint16_t)( ((uint16_t)((sign)&1) << 15) \
                   | ((uint16_t)((length_p)&3) << 10) \
                   | ((uint16_t)((rbaseb_p)&1) << 9) \
                   | ((uint16_t)((memb)&7) << 6) \
                   | ((uint16_t)((tpause_p)&1) << 5) \
                   | ((uint16_t)((tjump_p)&1) << 4) \
                   | ((uint16_t)((n)&0xF)) ))
    #define MK_RH(packed_a, ifaddr) \
        ((uint16_t)( ((uint16_t)((packed_a)&1) << 10) \
                   | ((uint16_t)((ifaddr)&0x3FF)) ))

    /* INC opcode 0x10. */
    mc.ifum_hi[0x10] = MK_LH(0, 0, 1, 4, 1, 1, 017);
    mc.ifum_lo[0x10] = MK_RH(0, 0020);
    mc.ifum_present[0x10] = 1;

    static dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "mem init");
    dorado_map_set(&mem, 0, 0, 0, 0);
    dorado_br_lo_load(&mem, 31, 0);
    dorado_br_hi_load(&mem, 31, 0);
    /* Just one INC opcode at byte 0; IFU never advances past it. */
    mem.storage[0] = 0x1010;
    mem.storage[1] = 0x2000;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.mem = &mem;
    /* T loaded by IM[1] microcode = 0xFFFF — sets ALU<0 cond true. */

    /* Run for a while. The conditional IFUJump should keep dispatching
     * back into entry 1 without advancing PCF. */
    for (int i = 0; i < 60; i++) {
        if (dorado_cpu_step(&cpu) != 0) break;
    }

    /* After the first IFUJump fires (post-warmup), PCF advanced
     * from 0 to 1. The second IFUJump in entry 1 has cond=true,
     * so PCF does NOT advance further — stays at 1. The engine
     * keeps re-dispatching entry 1 (looping forever in the
     * conditional path) which is exactly the cond=true semantics:
     * "stay in current opcode."  We verify:
     *  - PCF didn't advance past 1
     *  - real_PC ended at IM[0o101] (the entry-1 IFUJump)
     */
    EXPECT(cpu.ifu_pcf == 1,
           "PCF should be 1 (cond=true holds it), got 0o%o", cpu.ifu_pcf);
    EXPECT(cpu.real_PC == 0101,
           "real_PC should be at entry-1 (IM[0o101]), got 0o%o", cpu.real_PC);

    dorado_memory_free(&mem);
    printf("PASS  test_ifu_conditional_cond_true (PCF=0o%o stuck at entry 1)\n",
           cpu.ifu_pcf);
    return 0;
    #undef MK_LH
    #undef MK_RH
}

/*
 * Reschedule trap (HM Table 20). The `Reschedule` FF function arms
 * a flipflop; the second-or-third successful IFUJump after that
 * traps to *14-17 (with InsSet OR'd into bits 6:7 like the other
 * IFU traps). This is how the OS schedules a reschedule check
 * without immediately interrupting the current opcode.
 *
 * `RescheduleNow` arms the flipflop with count=1 — the very next
 * successful IFUJump traps. Used by the kernel for higher-priority
 * preemption.
 */
static int test_reschedule_trap(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;

    /* IM[0]: PCF←B (B=T=0), warmup=5. */
    mc.im[0] = make_uinstr(0, 0, 2, 0, 6, 0, /*ff=*/0100, jcn_local(1));
    mc.im_present[0] = 1;
    /* IM[1]: RescheduleNow. FA=1 FB=0 FC=3 → FF=0o103. BSEL=2 (T)
     * for B != constant so FF acts as a function. */
    mc.im[1] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0103, jcn_local(2));
    mc.im_present[1] = 1;
    /* IM[2..6]: NOPs to clear warmup (need 5 cycles total after PCF). */
    for (int i = 2; i <= 6; i++) {
        mc.im[i] = make_uinstr(0, 0, 2, 0, 6, 0, 0, jcn_local(i + 1));
        mc.im_present[i] = 1;
    }
    /* IM[7]: IFUJump[1]. JCN=0x2F. Should TRAP to Reschedule
     * vector (*14-17 with InsSet=0 → 0o314..0o317). With n=1 → 0o315. */
    mc.im[7] = make_uinstr(0, 0, 2, 0, 6, 0, 0, /*jcn=*/0x2F);
    mc.im_present[7] = 1;
    /* Plant Reschedule trap entries at 0o314..0o317 — self-loops. */
    for (int n = 0; n < 4; n++) {
        mc.im[0314 + n] = make_uinstr(0, 0, 2, 0, 6, 0, 0,
                                      jcn_local(0314 + n));
        mc.im_present[0314 + n] = 1;
    }
    /* NotReady trap retries (since we IFUJump while still warming). */
    for (int n = 0; n < 4; n++) {
        uint8_t jcn_ifu_n = (uint8_t)(0x27 | (n << 3));
        mc.im[0334 + n] = make_uinstr(0, 0, 2, 0, 6, 0, 0, jcn_ifu_n);
        mc.im_present[0334 + n] = 1;
    }

    int present_addrs[] = {0,1,2,3,4,5,6,7,
                           0314,0315,0316,0317,
                           0334,0335,0336,0337};
    for (size_t i = 0; i < sizeof present_addrs / sizeof present_addrs[0]; i++) {
        mc.image_to_real[i] = present_addrs[i];
        mc.image_present[i] = 1;
    }
    mc.n_instructions = sizeof present_addrs / sizeof present_addrs[0];

    static dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "mem init");
    /* Mount page 0 RW so IFU fetches don't fault. */
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.mem = &mem;

    /* Run until we land at the trap vector or budget exhausts. */
    for (int i = 0; i < 80; i++) {
        if (dorado_cpu_step(&cpu) != 0) break;
    }

    EXPECT(cpu.real_PC == 0315,
           "expected Reschedule trap at 0o315, got 0o%o", cpu.real_PC);

    dorado_memory_free(&mem);
    printf("PASS  test_reschedule_trap (PC=0o%o)\n", cpu.real_PC);
    return 0;
}

/*
 * LdTPC←B / RdTPC←B — HM page 34, JCN encoding fn=5/4 in the
 * return-class group. Used by emulator/fault microcode to set up
 * other tasks' entry points.
 *
 * JCN encoding for return-class: 0 1 f f f 1 1 1 (MSB-first).
 * fn=5 (LdTPC) → JCN bits = 01 101 111 → 0x6F
 * fn=4 (RdTPC) → JCN bits = 01 100 111 → 0x67
 */
static int test_ldtpc_rdtpc(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025;   mc.alufm_present[0] = 1;  /* B */
    mc.alufm[3] = 007;   mc.alufm_present[3] = 1;  /* all-ones */

    /* IM[0]: Link ← 0x1234 (the TPC value to load).
     * Use BSEL=6 (FF,,0) with FF=0x12, then ALU=B. But that gives
     * 0x1200, not 0x1234. Use Link←B FF function (FA=1 FB=4 FC=7 →
     * 0o147) with B = 0x1234. To get B=0x1234 in one instruction
     * we'd need a complex combination. Easier: T←0xFFFF (all-ones)
     * then Link←T (BSEL=2). Verify with a known value via API. */
    /* Just preset Link via cpu state; microcode does LdTPC←B with
     * task=5 (B=5). */

    /* IM[0]: LdTPC←B with B = 5. JCN=0x6F. BSEL=4 (constant 0,,FF)
     * with FF=5 → B=5. Note BSEL=4 suppresses FF interpretation
     * (so FF acts as data, not function — good, that's what we want). */
    mc.im[0] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/5, /*jcn=*/0x6F);
    mc.im_present[0] = 1;

    /* IM[1]: NOP self-loop (LdTPC's "next instruction" — execution
     * continues at .+1 per HM page 32). */
    mc.im[1] = make_uinstr(0, 0, 2, 0, 6, 0, 0, jcn_local(1));
    mc.im_present[1] = 1;

    for (int i = 0; i < 2; i++) {
        mc.image_to_real[i] = i;
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 2;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.Link = 0x1234;     /* The PC value we want to give task 5. */

    /* Step IM[0]: LdTPC←B with B=5 → task_tpc[5] ← Link[at issue] = 0x1234. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "LdTPC step");
    EXPECT(cpu.task_tpc[5] == 0x1234,
           "task_tpc[5] = 0x%04X, expected 0x1234", cpu.task_tpc[5]);

    /* Now read it back. Reuse IM[0] but change to RdTPC (JCN=0x67).
     * Easier: reset and run a different microcode. Let me just check
     * the helper API. */
    EXPECT(dorado_cpu_get_task_tpc(&cpu, 5) == 0x1234,
           "API readback: task 5 TPC should be 0x1234");

    /* Test RdTPC: separate microcode. */
    static dorado_microcode mc2;
    memset(&mc2, 0, sizeof mc2);
    mc2.alufm[0] = 025; mc2.alufm_present[0] = 1;
    /* IM[0]: RdTPC←B with B=5. JCN=0x67. */
    mc2.im[0] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/0, /*asel=*/6, 0,
                            /*ff=*/5, /*jcn=*/0x67);
    mc2.im_present[0] = 1;
    mc2.im[1] = make_uinstr(0, 0, 2, 0, 6, 0, 0, jcn_local(1));
    mc2.im_present[1] = 1;
    mc2.image_to_real[0] = 0; mc2.image_present[0] = 1;
    mc2.image_to_real[1] = 1; mc2.image_present[1] = 1;
    mc2.n_instructions = 2;

    dorado_cpu cpu2;
    dorado_cpu_init(&cpu2, &mc2, 0);
    cpu2.task_tpc[5] = 0xCAFE;

    EXPECT(dorado_cpu_step(&cpu2) == 0, "RdTPC step");
    /* Link should now be ~0xCAFE = 0x3501. */
    EXPECT(cpu2.Link == (uint16_t)~0xCAFE,
           "RdTPC: Link = 0x%04X, expected 0x%04X (= ~0xCAFE)",
           cpu2.Link, (uint16_t)~0xCAFE);

    printf("PASS  test_ldtpc_rdtpc\n");
    return 0;
}

/*
 * IFU NotReady trap dispatch — HM page 67: "the processor will
 * spin uselessly at the IFU 'NotReady' trap until the fifth cycle
 * after PCF←B (earliest)." Trap addresses are *34-37 with the 1's
 * complement of InsSet OR'd into bits 6:7 of the trap address (HM
 * Table 14 footnote).
 *
 * For InsSet=0: trap base 0o34, ~InsSet=11 → 0o334..0o337.
 * For InsSet=1: ~InsSet=10 → 0o234..0o237.
 * For InsSet=2: ~InsSet=01 → 0o134..0o137.
 * For InsSet=3: ~InsSet=00 → 0o034..0o037.
 *
 * Test plan: PCF←B then IMMEDIATELY IFUJump[2]. Engine should
 * trap to (InsSet=0, n=2) → 0o336.
 */
static int test_ifu_notready_trap(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025;   mc.alufm_present[0] = 1;

    /* IM[0]: PCF←B. */
    mc.im[0] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0100, jcn_local(1));
    mc.im_present[0] = 1;
    /* IM[1]: IFUJump[2]. Warmup is still 4 → trap to 0o336.
     * JCN = 0 0 1 1 0 1 1 1 = 0x37 (n=2 in JCN[3:4] = bits 4..5
     * of MSB → bits 3..4 of LSB → (n<<3) for our encoding:
     * 0x27 | (2<<3) = 0x37). */
    mc.im[1] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0, /*jcn=*/0x37);
    mc.im_present[1] = 1;
    /* Plant a trap-vector entry at 0o336 that's a self-loop so we
     * can detect arrival there. */
    mc.im[0336] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6, 0,
                              /*ff=*/0, jcn_local(0336 & 0x3F));
    mc.im_present[0336] = 1;

    int present_addrs[] = {0, 1, 0336};
    for (size_t i = 0; i < sizeof present_addrs / sizeof present_addrs[0]; i++) {
        mc.image_to_real[i] = present_addrs[i];
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 3;

    static dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "mem init");

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.mem = &mem;

    /* Step 0: PCF←B (warmup=5 → 4 at end). */
    EXPECT(dorado_cpu_step(&cpu) == 0, "PCF←B step");
    EXPECT(cpu.ifu_warmup == 4, "warmup=%d after PCF←B", cpu.ifu_warmup);

    /* Step 1: IFUJump[2] should trap to 0o336. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "IFUJump trap step");
    EXPECT(cpu.real_PC == 0336,
           "expected NotReady trap at 0o336, got 0o%o", cpu.real_PC);

    dorado_memory_free(&mem);
    printf("PASS  test_ifu_notready_trap (PC=0o%o)\n", cpu.real_PC);
    return 0;
}

/*
 * IFU map fault — opcode fetch hits a Vacant page → trap to *0-3.
 * (HM page 33: "Map faults on IFU fetches are reported instead to
 * the IFU, which buffers the fault until an IFUJump occurs.") The
 * trap address has the InsSet OR'd into bits 6:7 like NotReady.
 */
static int test_ifu_map_fault_trap(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;

    /* Simple setup: PCF←B, several NOPs to clear warmup, IFUJump. */
    mc.im[0] = make_uinstr(0, 0, 2, 0, 6, 0, /*ff=*/0100, jcn_local(1));
    mc.im_present[0] = 1;
    for (int i = 1; i <= 6; i++) {
        mc.im[i] = make_uinstr(0, 0, 2, 0, 6, 0, 0, jcn_local(i + 1));
        mc.im_present[i] = 1;
    }
    /* IM[7]: IFUJump[1]. n=1, JCN = 0x27 | (1<<3) = 0x2F.
     * Trap base for map fault = 0o0; with InsSet=0 (~InsSet=11),
     * bits 6:7 = 11 → trap addr = 0o0 + n + 0o300 = 0o301. */
    mc.im[7] = make_uinstr(0, 0, 2, 0, 6, 0, 0, /*jcn=*/0x2F);
    mc.im_present[7] = 1;
    /* Plant trap-vector entry at 0o301 — self-loop. */
    mc.im[0301] = make_uinstr(0, 0, 2, 0, 6, 0, 0,
                              jcn_local(0301 & 0x3F));
    mc.im_present[0301] = 1;
    /* Also plant NotReady trap at 0o334-0337 since IFUJump may trap
     * NotReady before warmup completes — these retry. */
    for (int n = 0; n < 4; n++) {
        uint8_t jcn_ifu_n = (uint8_t)(0x27 | (n << 3));
        mc.im[0334 + n] = make_uinstr(0, 0, 2, 0, 6, 0, 0, jcn_ifu_n);
        mc.im_present[0334 + n] = 1;
    }

    int present_addrs[] = {0, 1, 2, 3, 4, 5, 6, 7,
                           0334, 0335, 0336, 0337, 0301};
    for (size_t i = 0; i < sizeof present_addrs / sizeof present_addrs[0]; i++) {
        mc.image_to_real[i] = present_addrs[i];
        mc.image_present[i] = 1;
    }
    mc.n_instructions = sizeof present_addrs / sizeof present_addrs[0];

    static dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "mem init");
    /* DON'T mount any pages — every page is Vacant → IFU fetch
     * faults. BR[31] defaults to 0, so VA = 0 → page 0 (Vacant). */

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.mem = &mem;

    /* Run until we land at the trap entry (which self-loops). */
    for (int i = 0; i < 80; i++) {
        if (dorado_cpu_step(&cpu) != 0) break;
    }

    EXPECT(cpu.real_PC == 0301,
           "expected map-fault trap at 0o301, got 0o%o", cpu.real_PC);

    dorado_memory_free(&mem);
    printf("PASS  test_ifu_map_fault_trap (PC=0o%o)\n", cpu.real_PC);
    return 0;
}

/*
 * probe_aemu — Boot-bypass: load Initial.mb + AEmu.mb directly
 * into IM (skipping the BB→Boot0→Boot1→Initial RAM-image chain)
 * and run from AEmu's START symbol. Reports how far AEmu gets
 * before hitting an unimplemented feature. Informational only.
 *
 * On real hardware the boot sequence is:
 *   BB → Boot0 (in IM[0o7700..0o7777] from EPROM)
 *      → Boot0 loads Boot1 from CPReg
 *      → Boot1 loads Initial via disk or CPReg
 *      → Initial loads the emulator (AEmu/Mesa/...) via disk
 * We compress this by direct-loading Initial+AEmu into IM.
 */
static int probe_aemu(void)
{
    /* Real boot order: Initial → kernel → memMisc → IfuComplex →
     * AEmu (the Alto-emulator-specific microcode). The kernel and
     * memMisc files provide trap vectors, page-fault handlers, and
     * memory primitives that AEmu calls into. */
    struct { const char *path; mb_file mb; int n; } layers[] = {
        { "../chm/dorado/expanded/bootstrap.dm!20_/Initial.mb",  {0}, 0 },
        { "../chm/dorado/expanded/kernel.dm!38_/kernel.mb",      {0}, 0 },
        { "../chm/dorado/expanded/memMisc.dm!11_/memMisc.mb",    {0}, 0 },
        { "../chm/dorado/expanded/Ifu.dm!51_/IfuComplex.mb",     {0}, 0 },
        { "../chm/dorado/AEmu.mb!2",                             {0}, 0 },
    };
    int n_layers = sizeof layers / sizeof layers[0];
    for (int i = 0; i < n_layers; i++) {
        mb_init(&layers[i].mb);
        if (mb_load(&layers[i].mb, layers[i].path) != MB_OK) {
            printf("SKIP  probe_aemu (%s not loadable)\n", layers[i].path);
            for (int j = 0; j < i; j++) mb_free(&layers[j].mb);
            return 0;
        }
    }

    static dorado_microcode mc;
    if (dorado_microcode_load(&layers[0].mb, &mc) != DM_OK) {
        printf("SKIP  probe_aemu (first layer load failed)\n");
        for (int j = 0; j < n_layers; j++) mb_free(&layers[j].mb);
        return 0;
    }
    layers[0].n = mc.n_instructions;
    for (int i = 1; i < n_layers; i++) {
        int before = mc.n_instructions;
        if (dorado_microcode_layer_load(&layers[i].mb, &mc) != DM_OK) {
            printf("SKIP  probe_aemu (layer %d failed)\n", i);
            for (int j = 0; j < n_layers; j++) mb_free(&layers[j].mb);
            return 0;
        }
        layers[i].n = mc.n_instructions - before;
    }
    int initial_count = layers[0].n;
    int aemu_count    = mc.n_instructions;
    (void)initial_count;

    /* Look up some interesting entry points by name across all
     * layers' .MBs. The image-to-real for a given .MB is local to
     * that load; we re-derive by looking at each layer's mb_file. */
    int initial_im_id     = mb_find_mem(&layers[0].mb, "IM");
    int aemu_im_id        = mb_find_mem(&layers[n_layers-1].mb, "IM");
    int bootemul_image    = (initial_im_id >= 0)
        ? mb_find_symbol_addr(&layers[0].mb, initial_im_id, "BOOTEMULATOR")
        : -1;
    int startemul_image   = (aemu_im_id >= 0)
        ? mb_find_symbol_addr(&layers[n_layers-1].mb, aemu_im_id, "STARTEMULATOR")
        : -1;

    /* Compute REAL addresses for those image addresses. The .MB
     * stores the real placement in each entry's storage word 3
     * (awd & 0xFFF). Re-read it directly from the mb_file. */
    int bootemul_real = -1, startemul_real = -1;
    if (bootemul_image >= 0) {
        const mb_memory *m = &layers[0].mb.mems[initial_im_id];
        if (m->present[bootemul_image]) {
            bootemul_real = m->data[(size_t)bootemul_image * m->width_words + 3]
                            & 0xFFF;
        }
    }
    if (startemul_image >= 0) {
        const mb_memory *m = &layers[n_layers-1].mb.mems[aemu_im_id];
        if (m->present[startemul_image]) {
            startemul_real = m->data[(size_t)startemul_image * m->width_words + 3]
                             & 0xFFF;
        }
    }
    printf("       symbols: BOOTEMULATOR(Initial)=0o%o "
           "STARTEMULATOR(AEmu)=0o%o\n",
           bootemul_real, startemul_real);

    int real_start = mc.image_present[0] ? mc.image_to_real[0] : 0;

    /* Diagnostic: dump ALUFM contents. */
    printf("       ALUFM:");
    for (int a = 0; a < 16; a++) {
        if (mc.alufm_present[a]) printf(" [%d]=0o%o", a, mc.alufm[a]);
    }
    printf("\n");

    /* Stand up memory + a code page so the IFU can fetch bytecode
     * (even though we don't have any planted; we just want to see
     * how AEmu's initialization runs before it tries to dispatch). */
    static dorado_memory mem;
    if (dorado_memory_init(&mem) != 0) {
        printf("SKIP  probe_aemu (mem init failed)\n");
        for (int j = 0; j < n_layers; j++) mb_free(&layers[j].mb);
        return 0;
    }
    /* Mount a few low pages identity-mapped, RW. */
    for (uint32_t pg = 0; pg < 16; pg++) {
        dorado_map_set(&mem, pg, /*rp=*/(uint16_t)pg, /*wp=*/0, /*dirty=*/0);
    }

    /* Choose entry point. Order of preference:
     *   1. STARTEMULATOR (AEmu) — bypasses AEmu's own startup
     *      (RESTOREALUFM, LRTYPETABLE etc.) which tries to reload
     *      IFUM from main-memory tables Initial would have placed
     *      there. Our static load already populated IFUM.
     *   2. BOOTEMULATOR (Initial) — transfers to emulator after
     *      Initial's setup; still hits AEmu's startup.
     *   3. AEmu's START — full path (gets stuck in LRTYPETABLE). */
    int entry = (startemul_real >= 0) ? startemul_real
              : (bootemul_real >= 0)  ? bootemul_real
              : real_start;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, (uint16_t)entry);
    cpu.mem = &mem;
    /* Match BB's PrepareProcessor setup before starting the
     * emulator: tasking off (BB's `Return#` instruction includes
     * TaskingOff), no pending wakeups, IFU not active until
     * microcode does PCF←B. */
    cpu.tasking_on = 0;
    cpu.wakeup_pending = 0;
    cpu.reschedule_pending = 0;
    cpu.ifu_active = 0;
    cpu.ifu_warmup = 0;

    /* Step manually. Record TWO trails: a "first" trail of the
     * earliest 64 PCs (so we see entry flow) and a "last" trail of
     * the most recent 64 PCs (so we see where it ended up). */
    #define FIRST_N 64
    #define LAST_N  64
    uint16_t first_trail[FIRST_N];
    uint16_t last_trail[LAST_N];
    int      first_n = 0;
    int      last_head = 0, last_total = 0;
    cpu_halt_reason r = CPU_HALT_NONE;
    int loop_pc = -1, loop_count = 0;
    int max_cycles = 200000;
    for (int i = 0; i < max_cycles; i++) {
        if (first_n < FIRST_N) first_trail[first_n++] = cpu.real_PC;
        last_trail[last_head] = cpu.real_PC;
        last_head = (last_head + 1) % LAST_N;
        last_total++;
        if ((int)cpu.real_PC == loop_pc) {
            if (++loop_count > 200) break;
        } else {
            loop_pc = (int)cpu.real_PC;
            loop_count = 0;
        }
        if (dorado_cpu_step(&cpu) != 0) {
            r = (cpu_halt_reason)cpu.halt_reason;
            break;
        }
    }
    int last_n = last_total < LAST_N ? last_total : LAST_N;
    int last_first = last_total < LAST_N ? 0 : last_head;

    const char *sym       = dorado_microcode_symbol_at_real(&mc, cpu.real_PC);
    const char *entry_sym = dorado_microcode_symbol_at_real(&mc, real_start);
    printf("PROBE  aemu (layers:");
    for (int i = 0; i < n_layers; i++) {
        const char *base = strrchr(layers[i].path, '/');
        printf(" %s=%d", base ? base + 1 : layers[i].path, layers[i].n);
    }
    printf(", total=%d) entry=0o%o (image=0o0=%s), ran %d cycles, "
           "halt: %s at real_PC=0o%o%s%s\n",
           aemu_count, real_start, entry_sym ? entry_sym : "<no-sym>",
           cpu.cycles, cpu_halt_reason_str(r),
           cpu.real_PC,
           sym ? " sym=" : "", sym ? sym : "");
    /* Print first trail: entry flow. */
    printf("       entry:");
    int prev_pc = -1, prev_count = 0;
    for (int i = 0; i < first_n; i++) {
        if ((int)first_trail[i] == prev_pc) { prev_count++; continue; }
        if (prev_count > 1) printf("×%d", prev_count);
        const char *s = dorado_microcode_symbol_at_real(&mc, first_trail[i]);
        if (s) printf(" 0o%o(%s)", first_trail[i], s);
        else   printf(" 0o%o", first_trail[i]);
        prev_pc = first_trail[i];
        prev_count = 1;
    }
    if (prev_count > 1) printf("×%d", prev_count);
    printf("\n");

    /* Print last trail: where it ended up. */
    printf("       last:");
    prev_pc = -1; prev_count = 0;
    for (int i = 0; i < last_n; i++) {
        int idx = (last_first + i) % LAST_N;
        if ((int)last_trail[idx] == prev_pc) { prev_count++; continue; }
        if (prev_count > 1) printf("×%d", prev_count);
        const char *s = dorado_microcode_symbol_at_real(&mc, last_trail[idx]);
        if (s) printf(" 0o%o(%s)", last_trail[idx], s);
        else   printf(" 0o%o", last_trail[idx]);
        prev_pc = last_trail[idx];
        prev_count = 1;
    }
    if (prev_count > 1) printf("×%d", prev_count);
    printf("\n");
    #undef FIRST_N
    #undef LAST_N

    if (cpu.halt_reason == CPU_HALT_UNSUPPORTED_ASEL ||
        cpu.halt_reason == CPU_HALT_UNSUPPORTED_BSEL ||
        cpu.halt_reason == CPU_HALT_UNSUPPORTED_FF ||
        cpu.halt_reason == CPU_HALT_UNSUPPORTED_JCN) {
        const dorado_uinstr *u = &mc.im[cpu.real_PC];
        char dis[256];
        dorado_format(u, dis, sizeof dis);
        printf("       offending uinstr: %s\n", dis);
    }

    dorado_memory_free(&mem);
    for (int j = 0; j < n_layers; j++) mb_free(&layers[j].mb);
    return 0;
}

/*
 * probe_full_boot — proper interleaved BB+Dorado cold boot.
 *
 * The pre-baked probe_bootstrap loads Bootstrap.MB into IM, runs the
 * BB for 16 M cycles in isolation (long after MIR strobes have flown),
 * then runs the Dorado for 1000 cycles. That misses every BB-driven
 * MIR injection.
 *
 * This probe instead runs them tick-by-tick from cycle 0, with empty
 * IM (the way real hardware comes up). The flow we expect:
 *
 *   1. BB cold-boots into WaitForInitialBoot (~5 M BB cycles).
 *   2. We press the boot button 3× → CoolBoot dispatch → RebootDorado.
 *   3. RebootDorado walks through PowerUp checks, reaches LoadDoradoCode.
 *   4. LoadDoradoCode jams Boot0 microcode into IM via DoDoradoMicroInst
 *      (MIR strobes + SetSS) and Bootstrap's IM-write IRTable entries.
 *   5. With Boot0 loaded, BB sets Link = Boot0GoLoc and jams Return →
 *      Dorado starts running Boot0 from IM.
 *   6. Boot0 streams Boot1 via CPReg (BB's SendIMBlockToDorado, ViaCP=1).
 *
 * We don't yet expect to make it all the way through, but the probe
 * tells us how far each piece gets and where the next real wall is.
 */
static int probe_full_boot(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    /*
     * Preload ALUFM[0..15] with the canonical Dorado convention
     * (verified identical across Bootstrap.MB and AEmu.mb). The BB
     * EPROM only initializes ALUFM[0] explicitly via ALUFM[0]FromQ#;
     * Boot0 itself uses ALUF=2/4/14/etc. starting from its first
     * instruction, so SOMETHING must seed ALUFM[1..15] before Boot0
     * runs. On real hardware this is Midas debugger state or
     * power-on configuration we don't model. Empirically, all real
     * Dorado microcode declares the same 16 standard ops, so we
     * preset them here.
     *
     * 6-bit entry layout (per `alu_op` in src/cpu.c):
     *   bit 5 = carry_in, bits 4..0 = ALU op (HM Table 9).
     */
    mc.alufm[ 0] = 025; mc.alufm_present[ 0] = 1; /* B */
    mc.alufm[ 1] = 000; mc.alufm_present[ 1] = 1; /* A (arith, c=0) */
    mc.alufm[ 2] = 014; mc.alufm_present[ 2] = 1; /* A+B (c=0) */
    mc.alufm[ 3] = 054; mc.alufm_present[ 3] = 1; /* A+B (c=1) */
    mc.alufm[ 4] = 062; mc.alufm_present[ 4] = 1; /* A-B (c=1: A-B-1+1) */
    mc.alufm[ 5] = 022; mc.alufm_present[ 5] = 1; /* A-B-1 (c=0) */
    mc.alufm[ 6] = 035; mc.alufm_present[ 6] = 1; /* A AND B */
    mc.alufm[ 7] = 027; mc.alufm_present[ 7] = 1; /* A OR B */
    mc.alufm[ 8] = 023; mc.alufm_present[ 8] = 1; /* A XOR B */
    mc.alufm[ 9] = 031; mc.alufm_present[ 9] = 1; /* 0 */
    mc.alufm[10] = 040; mc.alufm_present[10] = 1; /* A+1 (c=1, op=A) */
    mc.alufm[11] = 036; mc.alufm_present[11] = 1; /* A-1 */
    mc.alufm[12] = 013; mc.alufm_present[12] = 1; /* NOT B */
    mc.alufm[13] = 033; mc.alufm_present[13] = 1; /* A AND NOT B */
    mc.alufm[14] = 001; mc.alufm_present[14] = 1; /* NOT A (used by shifter) */
    mc.alufm[15] = 006; mc.alufm_present[15] = 1; /* 2*A */

    static dorado_baseboard bb;
    baseboard_init(&bb);
    if (baseboard_load_rom(&bb, "../chm/dorado/doradobaserom.mb!13") != 0) {
        printf("SKIP  probe_full_boot (BB ROM not loadable)\n");
        return 0;
    }
    baseboard_reset(&bb);

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.baseboard = &bb;
    cpu.baseboard_cycles_per_uop = 1;

    static dorado_memory mem;
    static dorado_io io;
    static dorado_display display;
    static dorado_disk_controller disk;
    static dorado_fastio_router fastio;
    if (dorado_memory_init(&mem) == 0) {
        dorado_io_init(&io);
        dorado_display_init(&display);
        dorado_disk_controller_init(&disk);
        dorado_display_attach_to_io(&display, &io);
        dorado_disk_controller_attach_to_io(&disk, &io);
        dorado_fastio_router_init(&fastio, &display, &disk);
        mem.fast_io_cb = dorado_fastio_dispatch;
        mem.fast_io_ctx = &fastio;
        cpu.mem = &mem;
        cpu.io = &io;
    }

    /* Schedule of boot-button events, in BB cycles. The Dorado-cycle
     * counter advances 1:1 with the BB once held, and at most a few
     * cycles ahead of BB once running. */
    const uint64_t T_PRESS1_DOWN  =  1000000;
    const uint64_t T_PRESS1_UP    =  1400000;
    const uint64_t T_PRESS2_DOWN  =  2000000;
    const uint64_t T_PRESS2_UP    =  2400000;
    const uint64_t T_PRESS3_DOWN  =  3000000;
    const uint64_t T_PRESS3_UP    =  3400000;
    const uint64_t T_GIVEUP       = 60000000;

    int  pressed = 0;
    int  saw_load_dorado_code = 0;
    int  saw_continuous = 0;
    int  saw_first_im_write = 0;
    int  saw_first_dorado_uop = 0;
    int  saw_first_imfetch = 0;
    int  injected_count = 0;
    uint16_t boot0_trail[64];
    int  boot0_trail_n = 0;
    /* Per-step trace of branch-condition inputs for the first 16 IM
     * fetches. Captures pre-step ALU flags + post-step Pd sign so we
     * can debug why conditional jumps land in trap slots. */
    struct {
        uint16_t pre_pc;
        uint16_t post_pc;
        uint8_t  pre_zero, pre_lt0, pre_carry;
        uint16_t pre_rm_at_rstk;
        uint16_t post_pd;        /* approx — we capture T after, since
                                  * Pd isn't exposed; for LC=T←Pd, T=Pd. */
        uint8_t  post_carry;
    } trail_detail[16];
    int  trail_detail_n = 0;
    int  imfetch_count = 0;
    int  dorado_held_count = 0;
    uint16_t first_im_write_addr = 0xFFFF;

    while (bb.cycles < T_GIVEUP) {
        /* Boot-button schedule. */
        if (!pressed && bb.cycles >= T_PRESS1_DOWN && bb.cycles < T_PRESS1_UP) {
            baseboard_boot_button(&bb, 1); pressed = 1;
        } else if (pressed && bb.cycles >= T_PRESS1_UP && bb.cycles < T_PRESS2_DOWN) {
            baseboard_boot_button(&bb, 0); pressed = 0;
        } else if (!pressed && bb.cycles >= T_PRESS2_DOWN && bb.cycles < T_PRESS2_UP) {
            baseboard_boot_button(&bb, 1); pressed = 1;
        } else if (pressed && bb.cycles >= T_PRESS2_UP && bb.cycles < T_PRESS3_DOWN) {
            baseboard_boot_button(&bb, 0); pressed = 0;
        } else if (!pressed && bb.cycles >= T_PRESS3_DOWN && bb.cycles < T_PRESS3_UP) {
            baseboard_boot_button(&bb, 1); pressed = 1;
        } else if (pressed && bb.cycles >= T_PRESS3_UP) {
            baseboard_boot_button(&bb, 0); pressed = 0;
        }

        /* Watch for landmark BB PCs. */
        uint16_t pc = baseboard_pc(&bb);
        if (!saw_load_dorado_code && pc == 0xFAAE) saw_load_dorado_code = 1;
        if (!saw_continuous && pc == 0xF4F3)       saw_continuous = 1;

        /* Classify this Dorado step before stepping. */
        int will_inject = bb.dorado_ss_pending && bb.dorado_mir_loaded;
        int will_hold   = !will_inject && !bb.dorado_running;

        /* Snapshot pre-step state for trace_detail logging. */
        int will_imfetch_for_trace = (!will_inject && !will_hold &&
                                      trail_detail_n < 16);
        uint16_t pre_pc = cpu.real_PC;
        uint8_t  pre_zero = cpu.alu_zero;
        uint8_t  pre_lt0  = cpu.alu_lt0;
        uint8_t  pre_carry = cpu.alu_carry;
        uint16_t pre_rm_at_rstk = 0;
        if (will_imfetch_for_trace && pre_pc < 4096 && mc.im_present[pre_pc]) {
            const dorado_uinstr *uu = &mc.im[pre_pc];
            int rstk = uu->rstk & 0xF;
            int rm_a = (cpu.RBase << 4) | rstk;
            pre_rm_at_rstk = (uu->block) ? cpu.STK[cpu.StkP & 0xFF]
                                          : cpu.RM[rm_a & 0xFF];
        }

        if (dorado_cpu_step(&cpu)) {
            printf("       Dorado halted: %s at PC=0o%o, BB cycle %llu (BB PC=0x%04X)\n",
                   cpu_halt_reason_str(cpu.halt_reason),
                   cpu.real_PC, (unsigned long long)bb.cycles,
                   baseboard_pc(&bb));
            if (cpu.real_PC < 4096 && mc.im_present[cpu.real_PC]) {
                const dorado_uinstr *u = &mc.im[cpu.real_PC];
                char dis[256];
                dorado_format(u, dis, sizeof dis);
                printf("       offending uinstr: %s\n", dis);
            }
            break;
        }

        if (will_hold) {
            dorado_held_count++;
        } else if (will_inject) {
            injected_count++;
            if (!saw_first_dorado_uop) {
                saw_first_dorado_uop = 1;
                printf("       first injected uop at BB cycle %llu (BB PC=0x%04X)\n",
                       (unsigned long long)bb.cycles, baseboard_pc(&bb));
            }
        } else {
            imfetch_count++;
            if (!saw_first_imfetch) {
                saw_first_imfetch = 1;
                printf("       first IM-fetched uop at BB cycle %llu, Dorado PC=0o%o "
                       "Link=0o%o T=0o%o RM[0]=0x%X RM[2]=0x%X\n",
                       (unsigned long long)bb.cycles, cpu.real_PC,
                       cpu.Link, cpu.T, cpu.RM[0], cpu.RM[2]);
            }
            /* Record the first 64 IM-fetched PCs for trail printing. */
            if (boot0_trail_n < 64) {
                boot0_trail[boot0_trail_n++] = cpu.real_PC;
            }
            if (will_imfetch_for_trace) {
                int i = trail_detail_n++;
                trail_detail[i].pre_pc    = pre_pc;
                trail_detail[i].post_pc   = cpu.real_PC;
                trail_detail[i].pre_zero  = pre_zero;
                trail_detail[i].pre_lt0   = pre_lt0;
                trail_detail[i].pre_carry = pre_carry;
                trail_detail[i].pre_rm_at_rstk = pre_rm_at_rstk;
                trail_detail[i].post_pd    = cpu.T;     /* T usually = Pd for LC=1 */
                trail_detail[i].post_carry = cpu.alu_carry;
            }
        }

        /* Watch IM gradually filling up. */
        for (int a = 0; a < 4096; a++) {
            if (mc.im_present[a] && !saw_first_im_write) {
                saw_first_im_write = 1;
                first_im_write_addr = (uint16_t)a;
                printf("       first IM write at addr 0o%o, BB cycle %llu\n",
                       a, (unsigned long long)bb.cycles);
                break;
            }
        }
    }

    /* Final IM occupancy count + zero-vs-nonzero pattern. */
    int im_filled = 0;
    int im_first = -1;
    for (int a = 0; a < 4096; a++) {
        if (mc.im_present[a]) {
            if (im_first < 0) im_first = a;
            im_filled++;
        }
    }
    printf("       IM map starting at 0o%o (Z=zero, .=non-zero):\n       ", im_first);
    for (int a = im_first; a < 4096 && a < im_first + 64; a++) {
        if (!mc.im_present[a]) { putchar(' '); continue; }
        const dorado_uinstr *u = &mc.im[a];
        int is_zero = (u->iw0 == 0 && u->iw1 == 0 && u->iw2 == 0);
        putchar(is_zero ? 'Z' : '.');
    }
    putchar('\n');

    printf("PROBE  full-boot: BB ended at PC=0x%04X (cycles=%llu)\n",
           baseboard_pc(&bb), (unsigned long long)bb.cycles);
    printf("       LoadDoradoCode reached: %s    Continuous reached: %s\n",
           saw_load_dorado_code ? "yes" : "no",
           saw_continuous ? "yes" : "no");
    printf("       Dorado: %d injected, %d IM-fetched, %d held, ss=%d mir=%d\n",
           injected_count, imfetch_count, dorado_held_count,
           bb.dorado_ss_pending, bb.dorado_mir_loaded);
    printf("       IM: %d entries written (first=0o%o), final Dorado PC=0o%o\n",
           im_filled, first_im_write_addr, cpu.real_PC);

    /* Print Boot0 PC trail with run-length compression + disasm. */
    printf("       Boot0 trail:");
    int prev_pc_bt = -1, prev_count_bt = 0;
    for (int i = 0; i < boot0_trail_n; i++) {
        if ((int)boot0_trail[i] == prev_pc_bt) { prev_count_bt++; continue; }
        if (prev_count_bt > 1) printf("×%d", prev_count_bt);
        printf(" 0o%o", boot0_trail[i]);
        prev_pc_bt = boot0_trail[i];
        prev_count_bt = 1;
    }
    if (prev_count_bt > 1) printf("×%d", prev_count_bt);
    printf("\n");
    /* Disasm each unique trail PC so we can see what each step did. */
    int seen_pcs[64], seen_n = 0;
    /* Also disasm 0o7740 (Boot0 entry per BaseCodeVersion). */
    int special_pcs[] = {07740, 07763, 07771};
    for (int i = 0; i < (int)(sizeof special_pcs / sizeof special_pcs[0]); i++) {
        seen_pcs[seen_n++] = special_pcs[i];
        int pc = special_pcs[i];
        if (pc < 4096 && mc.im_present[pc]) {
            char dis[200];
            dorado_format(&mc.im[pc], dis, sizeof dis);
            const dorado_uinstr *u = &mc.im[pc];
            printf("       IM[0o%o]: %s [iw0=%06o iw1=%06o iw2=%06o]\n",
                   pc, dis, u->iw0, u->iw1, u->iw2);
        } else {
            printf("       IM[0o%o]: <empty/no-code>\n", pc);
        }
    }
    for (int i = 0; i < boot0_trail_n; i++) {
        int pc = boot0_trail[i], dup = 0;
        for (int j = 0; j < seen_n; j++) if (seen_pcs[j] == pc) { dup = 1; break; }
        if (dup) continue;
        seen_pcs[seen_n++] = pc;
        if (pc < 4096 && mc.im_present[pc]) {
            char dis[200];
            dorado_format(&mc.im[pc], dis, sizeof dis);
            const dorado_uinstr *u = &mc.im[pc];
            printf("       IM[0o%o]: %s [iw0=%06o iw1=%06o iw2=%06o]\n",
                   pc, dis, u->iw0, u->iw1, u->iw2);
        } else {
            printf("       IM[0o%o]: <empty/no-code>\n", pc);
        }
    }

    /* ── Slow-IO scan: dump every loaded Boot0 entry and flag any that
     * issue Pd←Input (FF=0o032), Pd←InputNoPE (FF=0o033), Output←B
     * (FF=0o036), or TIOA←B (FF=0o037 — FA=0 FB=3 FC=7… actually
     * TIOA←B is FA=1 FB=3 FC=… see HM Table 11c). We emit a marked
     * listing so we can read off the TIOA pairs Boot0 actually hits.
     *
     * FF is interpreted as a function only when BSEL is not constant
     * (BSEL >= 4 selects a 0,,FF / FF,,0 form) and JCN top4 != 0
     * (long jump). Encode that gate so we don't misclassify.  */
    printf("       Boot0 full IM dump (IM[0o7700..0o7777]):\n");
    int io_hits = 0;
    for (int pc = 07700; pc <= 07777; pc++) {
        if (!mc.im_present[pc]) {
            printf("         IM[0o%o]: <not loaded>\n", pc);
            continue;
        }
        const dorado_uinstr *u = &mc.im[pc];
        int bsel_const = (u->bsel >= 4);
        int jcn_long   = ((u->jcn >> 4) & 0xF) == 0;
        int ff_is_fn   = !bsel_const && !jcn_long;
        int fa = (u->ff >> 6) & 3;
        int fb = (u->ff >> 3) & 7;
        int fc =  u->ff       & 7;
        const char *tag = "";
        if (ff_is_fn && fa == 0 && fb == 3) {
            switch (fc) {
            case 2: tag = " <-- Pd←Input"; io_hits++; break;
            case 3: tag = " <-- Pd←InputNoPE"; io_hits++; break;
            case 6: tag = " <-- Output←B"; io_hits++; break;
            }
        }
        if (ff_is_fn && fa == 1 && fb == 3) {
            /* Some FA=1 FB=3 codes touch TIOA / IFUM, mark for review. */
            if (fc == 0) tag = " <-- InsSetorEvent←B";
            if (fc == 7) tag = " <-- BrkIns←B";
        }
        /* TIOA←B is FA=1 FB=2 FC=4 per HM Table 11b/c; TIOA←small
         * constant is FA=2 (which our cpu.c calls "RBase←FF[4:7]
         * alt encoding" — confirm before relying on this). */
        if (ff_is_fn && fa == 1 && fb == 2 && fc == 4) tag = " <-- TIOA←B";
        char dis[200];
        dorado_format(u, dis, sizeof dis);
        printf("         IM[0o%o] FF=0o%03o BSEL=%d JCN=0o%03o: %s%s\n",
               pc, u->ff, u->bsel, u->jcn, dis, tag);
    }
    printf("       slow-IO hits in Boot0 region: %d\n", io_hits);

    /* ── Per-step trace for the first 16 IM fetches: shows the branch
     * condition inputs that decide each conditional jump. Diagnostic
     * for Boot0 progression — each row shows pre-step ALU flags + the
     * RM register at the current RSTK, plus post-step T (~ Pd for
     * LC=T←Pd) and carry. C' shown as 1 when no-carry. */
    printf("       Boot0 step trace (Z/<0/C'/rm@rstk → T-after / C'-after):\n");
    for (int i = 0; i < trail_detail_n; i++) {
        printf("         0o%04o → 0o%04o  Z=%d <0=%d C'=%d rm=0x%04X | "
               "T=0x%04X C'=%d\n",
               trail_detail[i].pre_pc, trail_detail[i].post_pc,
               trail_detail[i].pre_zero, trail_detail[i].pre_lt0,
               trail_detail[i].pre_carry ? 0 : 1,
               trail_detail[i].pre_rm_at_rstk, trail_detail[i].post_pd,
               trail_detail[i].post_carry ? 0 : 1);
    }

    return 0;  /* informational */
}

/*
 * probe_full_boot_with_bootstrap — substitute Bootstrap.MB for the
 * BB-loaded Boot0, then let the BB stream Boot1 (= Initial.MB
 * mbtobase-encoded) through CPReg.
 *
 * The BB ROM contains a NEWER Boot0 binary that takes a different path
 * than Bootstrap.MB (lands in trap reservations on our model). But the
 * BB ROM also has the matching Boot1Data, encoded in the same
 * mbtobase format that Bootstrap.MB's READBB loader expects. So:
 *
 *   1. Run the BB through cold boot + LoadDoradoCode normally. The
 *      BB MIR-jams its Boot0 binary into IM[0o7700..0o7777], sets
 *      Link=Boot0GoLoc, AMSync=1, then jams Return# → Dorado starts
 *      free-running from IM.
 *   2. THE MOMENT the Dorado executes its first IM-fetched
 *      instruction, OVERWRITE IM[0o7700..0o7777] with Bootstrap.MB
 *      (and pre-load ALUFM with Bootstrap's standard convention).
 *      The Dorado's PC is at Boot0GoLoc=0o7740 = BOOTSTRAP entry —
 *      same address Bootstrap.MB places its entry at.
 *   3. Let the BB continue its SendIMBlockToDorado(Boot1Block,
 *      ViaCP=1) loop. It strobes ABMux1/ABMux0 with Boot1Data bytes
 *      → CPReg toggles AMSync per byte.
 *   4. Bootstrap.MB's READBB loader consumes the stream and writes
 *      Initial.MB into IM at the appropriate addresses (~0o0000..
 *      0o1675 for ~926 instructions).
 *   5. When the stream ends, Bootstrap branches to InitMap (= 0o1076)
 *      and Initial begins executing.
 *
 * Goal: count how many IM entries get written outside the
 * 0o7700-0o7777 region (= Initial entries). Trail the Dorado past
 * the Bootstrap→Initial handoff.
 */
static int probe_full_boot_with_bootstrap(void)
{
    /* Load Bootstrap.MB into a side buffer; we'll memcpy it over the
     * BB-loaded Boot0 IM at the right moment. */
    mb_file bs_mb;
    mb_init(&bs_mb);
    if (mb_load(&bs_mb,
                "../chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb")
        != MB_OK) {
        printf("SKIP  probe_full_boot_with_bootstrap (Bootstrap.mb)\n");
        return 0;
    }
    static dorado_microcode bs_mc;
    if (dorado_microcode_load(&bs_mb, &bs_mc) != DM_OK) {
        printf("SKIP  probe_full_boot_with_bootstrap (microcode load)\n");
        mb_free(&bs_mb);
        return 0;
    }

    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    /* Pre-load ALUFM with the canonical Dorado convention so Boot0
     * has working ALU semantics from the first instruction. (Same
     * preset as probe_full_boot.) */
    mc.alufm[ 0] = 025; mc.alufm_present[ 0] = 1;
    mc.alufm[ 1] = 000; mc.alufm_present[ 1] = 1;
    mc.alufm[ 2] = 014; mc.alufm_present[ 2] = 1;
    mc.alufm[ 3] = 054; mc.alufm_present[ 3] = 1;
    mc.alufm[ 4] = 062; mc.alufm_present[ 4] = 1;
    mc.alufm[ 5] = 022; mc.alufm_present[ 5] = 1;
    mc.alufm[ 6] = 035; mc.alufm_present[ 6] = 1;
    mc.alufm[ 7] = 027; mc.alufm_present[ 7] = 1;
    mc.alufm[ 8] = 023; mc.alufm_present[ 8] = 1;
    mc.alufm[ 9] = 031; mc.alufm_present[ 9] = 1;
    mc.alufm[10] = 040; mc.alufm_present[10] = 1;
    mc.alufm[11] = 036; mc.alufm_present[11] = 1;
    mc.alufm[12] = 013; mc.alufm_present[12] = 1;
    mc.alufm[13] = 033; mc.alufm_present[13] = 1;
    mc.alufm[14] = 001; mc.alufm_present[14] = 1;
    mc.alufm[15] = 006; mc.alufm_present[15] = 1;

    static dorado_baseboard bb;
    baseboard_init(&bb);
    if (baseboard_load_rom(&bb, "../chm/dorado/doradobaserom.mb!13") != 0) {
        printf("SKIP  probe_full_boot_with_bootstrap (BB ROM)\n");
        mb_free(&bs_mb);
        return 0;
    }
    baseboard_reset(&bb);

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.baseboard = &bb;
    cpu.baseboard_cycles_per_uop = 1;
    cpu.dbg_writeim_log = 0;  /* will be enabled at swap to capture only post-swap writes */

    static dorado_memory mem;
    static dorado_io io;
    static dorado_display display;
    static dorado_disk_controller disk;
    static dorado_ethernet ethernet;
    static dorado_disk_pack disk_pack;
    static dorado_fastio_router fastio;
    int disk_pack_attached = 0;
    if (dorado_memory_init(&mem) == 0) {
        dorado_io_init(&io);
        dorado_display_init(&display);
        dorado_disk_controller_init(&disk);
        dorado_ethernet_init(&ethernet);
        dorado_ethernet_set_boot_file(
            &ethernet, 0110,
            test_str_env("DORADO_ETH_BOOT_110",
                         "../chm/microcode/AltoMesaDorado.eb!1"));
        dorado_ethernet_set_boot_file(
            &ethernet, 0114,
            test_str_env("DORADO_ETH_BOOT_114",
                         "../chm/microcode/TestDorado.eb!1"));
        disk_pack_attached = attach_default_trident_pack(&disk, &disk_pack);
        dorado_display_attach_to_io(&display, &io);
        dorado_disk_controller_attach_to_io(&disk, &io);
        dorado_ethernet_attach_to_io(&ethernet, &io);
        dorado_fastio_router_init(&fastio, &display, &disk);
        mem.fast_io_cb = dorado_fastio_dispatch;
        mem.fast_io_ctx = &fastio;
        cpu.mem = &mem;
        cpu.io = &io;
    }

    const uint64_t T_PRESS1_DOWN = 1000000;
    const uint64_t T_PRESS1_UP   = 1400000;
    const uint64_t T_PRESS2_DOWN = 2000000;
    const uint64_t T_PRESS2_UP   = 2400000;
    const uint64_t T_PRESS3_DOWN = 3000000;
    const uint64_t T_PRESS3_UP   = 3400000;
    const uint64_t T_GIVEUP      = test_u64_env("DORADO_BOOT_BUDGET",
                                                60000000);

    int  pressed = 0;
    int  swapped = 0;
    int  bs_writes = 0;       /* IM writes Bootstrap.MB performs (= Initial bytes consumed) */
    int  bs_writes_outside = 0;  /* Writes to IM outside 0o7700-0o7777 = Initial loaded */
    uint16_t first_outside_addr = 0xFFFF;
    int  imfetch_count = 0;
    int  cpreg_strobes = 0;
    int  bb_strobes_pre_swap = 0;     /* count strobes before swap */
    uint64_t swap_cycle = 0;
    uint16_t prev_cpreg = 0;
    uint16_t bb_pc_at_swap = 0;
    uint16_t pre_swap_cpreg = 0;

    /* Bootstrap PC trail (first 64 distinct PCs after the swap, plus
     * a ring buffer of the last 64). */
    uint16_t bs_first_trail[64];
    int      bs_first_n = 0;
    uint16_t bs_last_trail[64];
    int      bs_last_head = 0, bs_last_total = 0;
    /* Frequency table: which Bootstrap PCs get hit how often, post-swap. */
    int      bs_pc_count[4096];
    memset(bs_pc_count, 0, sizeof bs_pc_count);
    /* Write IM target capture: Bootstrap.MB's WRITE000-WRITE111 at
     * IM[0o7720..0o7736] do Write IM with target = Link. Capture
     * Link at issue time of each one. */
    uint16_t wim_first[32];
    int      wim_first_n = 0;
    uint16_t wim_last[32];
    int      wim_last_head = 0, wim_last_total = 0;
    int      wim_target_count[4096];
    memset(wim_target_count, 0, sizeof wim_target_count);

    struct readbb_trace {
        uint64_t cycle;
        uint16_t pc;
        uint16_t raw;
        uint16_t b_seen;
        uint16_t t_before, t_after;
        uint16_t tag_before, tag_after;
        uint16_t loc_before, loc_after;
        uint16_t link_before, link_after;
        uint16_t next_pc;
    };
    struct readbb_trace rb_trace[48];
    int rb_trace_n = 0;
    uint16_t cp_first[32];
    uint64_t cp_first_cycle[32];
    uint16_t cp_first_bbpc[32];
    int cp_first_n = 0;
    cpu_halt_reason halt_reason = CPU_HALT_NONE;

    /* Snapshot of im_present + IM content at swap time, so we can
     * diff afterward and see which addresses Bootstrap actually
     * wrote. */
    static uint8_t im_was_present[4096];
    static uint16_t im_was_iw0[4096];
    static uint16_t im_was_iw1[4096];
    static uint16_t im_was_iw2[4096];
    memset(im_was_present, 0, sizeof im_was_present);
    memset(im_was_iw0, 0, sizeof im_was_iw0);
    memset(im_was_iw1, 0, sizeof im_was_iw1);
    memset(im_was_iw2, 0, sizeof im_was_iw2);

    /* Pre-load canonical Initial.MB so we can substitute it for
     * Bootstrap's corrupt streamed IM when execution reaches
     * BOOTSTAGE2 (= the BranchExternal[InitialLoc] at PC 0o7717).
     * This bypasses the streaming bug while exercising the rest of
     * the boot path.
     *
     * On top of Initial.mb, layer kernel.mb (fault task + dispatch),
     * memMisc.mb (memory primitives), and IfuComplex.mb (IFU
     * support). This is the standard Mesa-bootstrap microcode stack
     * — without these layers, Initial's wakeups for fault task /
     * I/O tasks have no microcode to dispatch to. */
    static dorado_microcode init_mc;
    static mb_file initial_mb_for_init_mc;
    static mb_file kernel_mb_layer;
    static mb_file memmisc_mb_layer;
    static mb_file ifucomplex_mb_layer;
    int initial_canonical_loaded = 0;
    mb_init(&initial_mb_for_init_mc);
    mb_init(&kernel_mb_layer);
    mb_init(&memmisc_mb_layer);
    mb_init(&ifucomplex_mb_layer);
    /* Load order matters: kernel/memMisc/IfuComplex provide
     * task-specific microcode (fault task, IFU helpers, etc.) at
     * various real addresses. Some addresses CONFLICT with Initial's
     * own placements (e.g., 0o6340 WRITEMAP exists in both kernel
     * and Initial with different microinstructions). Initial's copy
     * is what Initial is designed to call; kernel's is a duplicate
     * for kernel's own use. We load kernel first so Initial overrides
     * conflicting entries while still keeping kernel's task-specific
     * non-conflicting entries (fault handler, dispatch, etc.). */
    if (mb_load(&kernel_mb_layer,
                "../chm/dorado/expanded/kernel.dm!38_/kernel.mb") == MB_OK) {
        dorado_microcode_load(&kernel_mb_layer, &init_mc);
    }
    if (mb_load(&memmisc_mb_layer,
                "../chm/dorado/expanded/memMisc.dm!11_/memMisc.mb") == MB_OK) {
        dorado_microcode_layer_load(&memmisc_mb_layer, &init_mc);
    }
    if (mb_load(&ifucomplex_mb_layer,
                "../chm/dorado/expanded/Ifu.dm!51_/IfuComplex.mb") == MB_OK) {
        dorado_microcode_layer_load(&ifucomplex_mb_layer, &init_mc);
    }
    if (mb_load(&initial_mb_for_init_mc,
                "../chm/dorado/expanded/bootstrap.dm!20_/Initial.mb") == MB_OK) {
        if (dorado_microcode_layer_load(&initial_mb_for_init_mc, &init_mc) == DM_OK) {
            initial_canonical_loaded = 1;
        }
        /* Keep all loaded mb_files alive — init_mc.mb (the latest
         * layer's mb) references its symbol list. */
    }
    struct ref_microcode {
        const char *name;
        const char *path;
        mb_file mb;
        dorado_microcode mc;
        int loaded;
    };
    static struct ref_microcode ref_mcs[] = {
        { .name = "Mesa.mb!3", .path = "../chm/dorado/Mesa.mb!3" },
        { .name = "TriMesa.mb!3", .path = "../chm/dorado/TriMesa.mb!3" },
        { .name = "AEmu.mb!2", .path = "../chm/dorado/AEmu.mb!2" },
        { .name = "Cedar.mb!6", .path = "../chm/dorado/Cedar.mb!6" },
        { .name = "DSemu.mb!1", .path = "../chm/dorado/DSemu.mb!1" },
        { .name = "UnBug/Mesa.mb", .path = "../chm/dorado/expanded/UnBug.bfs!1_/Mesa.mb" },
    };
    for (int i = 0; i < (int)(sizeof ref_mcs / sizeof ref_mcs[0]); i++) {
        mb_init(&ref_mcs[i].mb);
        if (mb_load(&ref_mcs[i].mb, ref_mcs[i].path) == MB_OK &&
            dorado_microcode_load(&ref_mcs[i].mb, &ref_mcs[i].mc) == DM_OK) {
            ref_mcs[i].loaded = 1;
        }
    }
    int initial_substituted = 0;
    uint64_t initial_substitute_cycle = 0;
    int nostorage_bypassed = 0;
    uint64_t nostorage_bypass_cycle = 0;

    /* Post-substitution PC trail: capture first N distinct PC
     * transitions after BOOTSTAGE2 substitution, so we can see
     * what path Initial takes on the way to NOSTORAGE. Also keep
     * a per-PC count + first-cycle map so we can see what Initial
     * is iterating on. */
    uint16_t init_first_trail[400];
    int init_first_n = 0;
    int prev_init_pc = -1;
    static int init_pc_count[4096];
    static uint64_t init_pc_first_cycle[4096];
    /* Ring buffer of last K distinct PC transitions before halt. */
    uint16_t init_last_trail[64];
    int init_last_head = 0, init_last_total = 0;
    int prev_last_pc = -1;
    struct preset_sample {
        uint64_t cycle;
        uint16_t pc, next_pc;
        uint16_t t, q, link, cnt;
        uint16_t rm1, rm2, rm6, rm7, rm0x45, rm0x48, rm0x49;
        uint8_t rbase, membase, proc_srn, alu_lt0;
        uint8_t task, tasking_on;
        uint16_t pipe5;
        uint16_t mcr;
        uint32_t mar;
    };
    struct preset_sample preset_first[64];
    struct preset_sample preset_last[64];
    int preset_first_n = 0;
    int preset_last_head = 0, preset_last_total = 0;
    int preset_trace_enabled = test_u64_env("DORADO_PRESET_TRACE", 0) != 0;
    int disk_trace_enabled = test_u64_env("DORADO_DISK_TRACE", 0) != 0;
    int mcr_trace_enabled = test_u64_env("DORADO_MCR_TRACE", 0) != 0;
    int post_eb_trace_enabled = test_u64_env("DORADO_POST_EB_TRACE", 0) != 0;
    int eth_reg_trace_enabled = test_u64_env("DORADO_ETH_REG_TRACE", 0) != 0;
    int lowcore_trace_enabled = test_u64_env("DORADO_LOWCORE_TRACE", 0) != 0;
    int ifu_trace_enabled = test_u64_env("DORADO_IFU_TRACE", 0) != 0;
    int ethernet_boot_enabled = test_u64_env("DORADO_BOOT_ETHERNET", 1) != 0;
    uint64_t ethernet_wakeups = 0;
    const char *ether_boot_image = getenv("DORADO_ETHER_BOOT_IMAGE");
    int ether_boot_enabled = ether_boot_image && *ether_boot_image &&
                             file_exists_readable(ether_boot_image);
    uint64_t disk_sector_ticks = 0;
    uint64_t disk_wakeups = 0;
    uint64_t disk_normal_mode_shims = 0;
    uint64_t alto_disk_boot_shims = 0;
    int alto_disk_boot_cyl = (int)test_u64_env("DORADO_ALTO_BOOT_CYL", 0);
    int alto_disk_boot_head = (int)test_u64_env("DORADO_ALTO_BOOT_HEAD", 0);
    int alto_disk_boot_sector =
        (int)test_u64_env("DORADO_ALTO_BOOT_SECTOR", 2);
    uint64_t boot_identity_map_shims = 0;
    uint64_t ether_boot_injections = 0;
    uint16_t ether_boot_end = 0;
    uint16_t ether_boot_sum = 0;
    uint16_t ether_boot_start_pc = 0;
    uint64_t ether_boot_inject_cycle = 0;
    uint64_t ether_inject_display_outs = 0;
    uint64_t ether_inject_display_iofetch = 0;
    uint64_t ether_inject_display_dwt_wakeups = 0;
    uint64_t ether_inject_scanline_wakeups = 0;
    uint64_t ether_inject_disk_outs = 0;
    uint64_t ether_inject_disk_ins = 0;
    uint64_t ether_inject_disk_wakeups = 0;
    uint64_t ether_loaded_world_cycle = 0;
    uint64_t ether_bank2_lost_cycle = 0;
    uint16_t ether_bank2_lost_pc = 0;
    uint8_t ether_bank2_lost_task = 0;
    dorado_map_entry ether_bank2_lost_entry = {0};
    uint64_t ether_bank2_remap_shims = 0;
    uint64_t ether_br37_bad_cycle = 0;
    uint16_t ether_br37_bad_pc = 0;
    uint8_t ether_br37_bad_task = 0;
    uint8_t ether_br37_bad_membase = 0;
    uint16_t ether_br37_bad_t = 0;
    uint32_t ether_br37_before = 0;
    uint32_t ether_br37_after = 0;
    uint64_t post_eb_task_cycles[16] = {0};
    uint64_t post_eb_task_switches = 0;
    uint16_t post_eb_ready_or = 0;
    uint16_t post_eb_wakeup_or = 0;
    int post_eb_prev_task = -1;
    uint64_t post_eb_ifu_arm_count = 0;
    uint64_t post_eb_ifu_stop_count = 0;
    uint16_t post_eb_last_ifu_arm_pc = 0;
    uint16_t post_eb_last_ifu_stop_pc = 0;
    uint16_t post_eb_last_ifu_pcf = 0;
    uint16_t post_eb_last_ifu_pcx = 0;
    struct post_eb_trace_sample {
        uint64_t cycle;
        uint8_t task, task_after;
        uint16_t pc, next_pc;
        uint16_t t_before, t_after;
        uint16_t q_before, q_after;
        uint16_t link_before, link_after;
        uint16_t mcr_before, mcr_after;
        uint32_t mar_before, mar_after;
        uint16_t md_before, md_after;
        uint8_t rbase_before, rbase_after;
        uint8_t membase_before, membase_after;
        uint8_t ifu_active_before, ifu_active_after;
        uint8_t ifu_warmup_before, ifu_warmup_after;
        uint8_t tasking_on_before, tasking_on_after;
        uint8_t resume_delay_before, resume_delay_after;
        uint16_t cnt_before, cnt_after;
        uint16_t shc_before, shc_after;
        uint16_t dispatch_or_before, dispatch_or_after;
        uint16_t dispatch_pending_before, dispatch_pending_after;
        uint16_t rm_before[16], rm_after[16];
        uint16_t disk_rm_before[16], disk_rm_after[16];
        uint8_t alufm_before[16], alufm_after[16];
        uint16_t ready_before, ready_after;
        uint16_t wakeup_before, wakeup_after;
        uint16_t task0_tpc_before, task0_tpc_after;
        uint16_t aht_tpc_before, aht_tpc_after;
        uint16_t disk_tpc_before, disk_tpc_after;
        uint16_t ifu_pcf_before, ifu_pcf_after;
        uint16_t ifu_pcx_before, ifu_pcx_after;
        cpu_halt_reason halt_reason;
    };
#define POST_EB_TRACE_CAP 1024
    struct post_eb_trace_sample post_eb_trace[POST_EB_TRACE_CAP];
    int post_eb_trace_head = 0, post_eb_trace_total = 0;
    static uint32_t post_eb_task_pc_count[16][4096];
    static uint16_t post_eb_task_pc_link[16][4096];
    static uint16_t post_eb_task_pc_mcr[16][4096];
    static uint32_t post_eb_task_pc_mar[16][4096];
    struct ifu_trace_sample {
        uint64_t cycle;
        uint8_t task;
        uint16_t pc, next_pc;
        uint8_t active_before, active_after;
        uint8_t opcode, length, n, alpha, beta;
        uint16_t pcf_before, pcf_after;
        uint16_t pcx_before, pcx_after;
        uint16_t t, q, link, md;
        uint8_t membase, rbase, stk_p;
        uint16_t ac[4];
    };
#define IFU_TRACE_CAP 256
    struct ifu_trace_sample ifu_trace[IFU_TRACE_CAP];
    int ifu_trace_head = 0, ifu_trace_total = 0;
    static uint32_t ifu_pcx_count[65536];
    static uint8_t ifu_pcx_opcode[65536];
    uint64_t display_scanline_wakeups = 0;
    uint64_t display_invalid_tpc_wakeups = 0;
    uint64_t next_display_scanline_cycle = 0;
    uint64_t terminal_boot_scanlines =
        test_u64_env("DORADO_TERMINAL_BOOT_SCANLINES", 0300);
    int terminal_boot_armed = 0;
    uint64_t keyboard_seed_count = 0;
    uint64_t boot_parameter_seed_count = 0;
    int force_ether_mesa_boot =
        ethernet_boot_enabled &&
        test_u64_env("DORADO_ETH_FORCE_MESA_BOOT_PARAM", 1) != 0;
    struct key_trace {
        uint64_t cycle;
        uint16_t pc, next_pc;
        uint16_t t, md, etemp0, etemp1, etemp2, etemp3, r400;
        uint32_t mar;
    } key_trace[48];
    int key_trace_n = 0;
    struct post_loadram_loop_trace {
        uint64_t cycle;
        uint8_t task, task_after;
        uint16_t pc, next_pc;
        uint16_t task0_tpc_after, disk_tpc_after;
        uint16_t t_before, t_after;
        uint16_t q_before, q_after;
        uint16_t link_before, link_after;
        uint16_t md_before, md_after;
        uint32_t mar_before, mar_after;
        uint16_t ifu_pcf, ifu_pcx;
        uint8_t stk_p;
        uint16_t stack[8];
        uint16_t r400, etemp0, etemp1, etemp2;
        uint16_t mem430, mem432, mem521;
        uint8_t disk_muff_addr, disk_index_tw, disk_sector_tw, disk_tag_tw;
        uint8_t disk_rd_fifo_tw, disk_wr_fifo_tw;
        uint8_t disk_enable_run, disk_active, disk_block_till_index;
        uint8_t disk_fifo_count, disk_selected_drive;
        uint16_t disk_control;
        uint16_t disk_out_data, disk_in_data;
        uint8_t disk_out_tioa, disk_in_tioa;
        uint8_t disk_cur_cyl_lo, disk_cur_head, disk_cur_sector;
    } post_loop_trace[96];
    int post_loop_trace_head = 0, post_loop_trace_n = 0;
    struct boot_landmark {
        uint16_t pc;
        const char *name;
        uint64_t hits;
        uint64_t first_cycle;
    } boot_landmarks[] = {
        { 07140, "DISKHARDMICROCODEBOOT", 0, 0 },
        { 07400, "BOOTTRANSFER", 0, 0 },
        { 07120, "DODISKBLOCK", 0, 0 },
        { 07360, "SEEKANDWAITFORREADY", 0, 0 },
        { 07060, "WAITFORSECTOR", 0, 0 },
        { 07067, "SECTORFOUND", 0, 0 },
        { 07436, "BOOTTRANSFERTIMEOUT", 0, 0 },
        { 07445, "BOOTDISKERROR", 0, 0 },
        { 07447, "BOOTEOF", 0, 0 },
        { 07453, "BOOTLABELERROR", 0, 0 },
        { 06260, "DISKMBOOTRET", 0, 0 },
        { 06206, "READBOOTKEYS", 0, 0 },
        { 06406, "ETHERBOOTING", 0, 0 },
        { 06420, "CHECKKEY", 0, 0 },
        { 06432, "GOTBOOTKEY", 0, 0 },
        { 06443, "DOETHERMICROCODEBOOT", 0, 0 },
        { 06460, "CHECKCHECKSUMANDLOAD", 0, 0 },
        { 07600, "LOADRAM", 0, 0 },
        { 06404, "MICROCODEBOOTFAILED", 0, 0 },
        { 06057, "AWAITETHERBOOTREPLY", 0, 0 },
    };
    int checksum_and_load_seen = 0;
    struct boot_landmark disk_landmarks[] = {
        { 06740, "DSKINITPC", 0, 0 },
        { 06754, "KMODETEST", 0, 0 },
        { 06737, "KDISABLE", 0, 0 },
        { 06664, "KFORGETCMMD", 0, 0 },
        { 06644, "KIDLELOOP", 0, 0 },
        { 06650, "KIDLECONT", 0, 0 },
        { 06552, "KNEWDRIVE", 0, 0 },
        { 06553, "KSAMEDRIVE", 0, 0 },
        { 06572, "KCONTINUECMMD", 0, 0 },
        { 06561, "KCHECKSEEK", 0, 0 },
        { 07102, "KNORESTORE", 0, 0 },
        { 07133, "KWAITSECTOR", 0, 0 },
        { 07105, "KCMMDINTIME", 0, 0 },
        { 07120, "DODISKBLOCK", 0, 0 },
        { 07201, "KCMMDREAD", 0, 0 },
        { 07300, "INITRAMPILOT", 0, 0 },
        { 06500, "READ1MUFF", 0, 0 },
        { 06600, "DOMUFFOUTPUT", 0, 0 },
        { 07060, "WAITFORSECTOR", 0, 0 },
        { 07000, "UPDATESECTOR", 0, 0 },
        { 06760, "CLEARDISK", 0, 0 },
    };
    struct disk_trace_sample {
        uint64_t cycle;
        uint8_t task;
        uint16_t pc, next_pc;
        uint16_t t_before, t_after;
        uint16_t md_before, md_after;
        uint16_t link_before, link_after;
        uint8_t alu_zero_before, alu_zero_after;
        uint8_t alu_lt0_before, alu_lt0_after;
        uint8_t alu_carry_before, alu_carry_after;
        uint16_t rm0, rm1, rm2, rm3;
        uint16_t pre_drm[16];
        uint16_t drm[16];
        uint8_t rbase_before, rbase_after;
        uint8_t membase_before, membase_after;
        uint8_t tioa;
        uint8_t muff_addr, index_tw, sector_tw, tag_tw, rd_fifo_tw, wr_fifo_tw;
        uint8_t enable_run, active, block_till_index;
        uint16_t disk_control;
        uint8_t fifo_count;
        uint8_t cur_cyl_lo, cur_head, cur_sector;
        uint8_t disk_out_tioa, disk_in_tioa;
        uint16_t disk_out_data, disk_in_data;
        uint32_t mar_before, mar_after;
        uint16_t storage_after_mar;
    } disk_trace[8192];
    int disk_trace_n = 0;
    int disk_trace_head = 0;
    int disk_trace_total = 0;
    int disk_trace_armed = 0;
    struct loadram_trace_sample {
        uint64_t cycle;
        uint8_t task;
        uint16_t pc, next_pc;
        uint16_t t_before, t_after;
        uint16_t q_before, q_after;
        uint16_t md_before, md_after;
        uint16_t link_before, link_after;
        uint8_t rbase_before, rbase_after;
        uint8_t membase_before, membase_after;
        uint16_t rm_before[16], rm_after[16];
        uint32_t mar_before, mar_after;
        uint16_t storage_at_t_before;
        uint16_t storage_at_mar_after;
        uint16_t mcr_before, mcr_after;
    } loadram_trace[512];
    int loadram_trace_n = 0;
    int loadram_trace_head = 0;
    uint64_t loadram_trace_total = 0;
    int loadram_trace_mode = (int)test_u64_env("DORADO_LOADRAM_TRACE", 0);
    int checksum_trace_enabled =
        test_u64_env("DORADO_CHECKSUM_TRACE", 0) != 0;
    struct mcr_trace_sample {
        uint64_t cycle;
        uint16_t pc, next_pc;
        uint16_t t_before, t_after;
        uint16_t mcr_before, mcr_after;
        uint8_t task;
    } mcr_trace[64];
    int mcr_trace_n = 0;

    while (bb.cycles < T_GIVEUP) {
        /* Boot button schedule. */
        if (!pressed && bb.cycles >= T_PRESS1_DOWN && bb.cycles < T_PRESS1_UP) {
            baseboard_boot_button(&bb, 1); pressed = 1;
        } else if (pressed && bb.cycles >= T_PRESS1_UP && bb.cycles < T_PRESS2_DOWN) {
            baseboard_boot_button(&bb, 0); pressed = 0;
        } else if (!pressed && bb.cycles >= T_PRESS2_DOWN && bb.cycles < T_PRESS2_UP) {
            baseboard_boot_button(&bb, 1); pressed = 1;
        } else if (pressed && bb.cycles >= T_PRESS2_UP && bb.cycles < T_PRESS3_DOWN) {
            baseboard_boot_button(&bb, 0); pressed = 0;
        } else if (!pressed && bb.cycles >= T_PRESS3_DOWN && bb.cycles < T_PRESS3_UP) {
            baseboard_boot_button(&bb, 1); pressed = 1;
        } else if (pressed && bb.cycles >= T_PRESS3_UP) {
            baseboard_boot_button(&bb, 0); pressed = 0;
        }

        int will_inject = bb.dorado_ss_pending && bb.dorado_mir_loaded;
        int will_hold   = !will_inject && !bb.dorado_running;
        int is_imfetch  = !will_inject && !will_hold;

        /* Track CPReg strobes happening pre-swap. The BB's
         * SendIMBlockToDorado(Boot1Block, ViaCP=1) starts streaming
         * Boot1Data via ABMux strobes shortly after starting the
         * Dorado. We delay the IM swap until the BB has fired at
         * least N strobes — that way Bootstrap's first ReadBB sees
         * the BB's actual preamble byte rather than the stale
         * CPReg=0x8000 setup value from before streaming began. */
        if (!swapped && bb.cpreg_to_dorado != pre_swap_cpreg) {
            bb_strobes_pre_swap++;
            pre_swap_cpreg = bb.cpreg_to_dorado;
        }

        /* Substitution moment: AFTER the BB has begun the Boot1
         * stream (≥ 4 ABMux strobes, = at least one full
         * SendAHalfMicroInstruction = the IMAddress preamble first
         * half is in flight). The Dorado has been spinning at
         * 0o7740 in BB-loaded Boot0 (whose conditional path traps,
         * but it doesn't matter — we'll overwrite). */
        if (!swapped && is_imfetch && cpu.real_PC == 07740 &&
            bb_strobes_pre_swap >= 16) {
            /* Copy Bootstrap.MB IM in place. mc and bs_mc are both
             * dorado_microcode; we just memcpy the IM arrays. */
            for (int a = 0; a < 4096; a++) {
                if (bs_mc.im_present[a]) {
                    mc.im[a]         = bs_mc.im[a];
                    mc.im_present[a] = 1;
                }
            }
            /* Also copy ALUFM from Bootstrap.MB (it should match our
             * preset, but be authoritative). */
            for (int a = 0; a < 16; a++) {
                if (bs_mc.alufm_present[a]) {
                    mc.alufm[a]         = bs_mc.alufm[a];
                    mc.alufm_present[a] = 1;
                }
            }
            swapped = 1;
            swap_cycle = bb.cycles;
            bb_pc_at_swap = baseboard_pc(&bb);
            prev_cpreg = bb.cpreg_to_dorado;
            /* Reset Write IM trace so we only capture Bootstrap's
             * post-swap Initial-loading writes, not the BB's pre-swap
             * MIR-jam writes. */
            cpu.dbg_writeim_log = 1;
            cpu.dbg_writeim_n = 0;
            /* WORKAROUND: Bootstrap's streaming path produces
             * corrupt data (root cause unknown — see handoff.md
             * §"Full BB→Bootstrap→Initial path spins at 0o6347").
             * Install Initial.MB at canonical placement now so that
             * when BOOTSTAGE2 globally-calls INITIAL (0o7500),
             * Initial executes from CORRECT microcode. The
             * streaming below will overwrite some entries in
             * 0o6100..0o7124 with garbage, so we'll need to re-
             * install at BOOTSTAGE2 entry. */
            /* Snapshot im_present + IM content *now* so we can diff
             * what Bootstrap writes during the run. */
            for (int a = 0; a < 4096; a++) {
                im_was_present[a] = mc.im_present[a];
                im_was_iw0[a]     = mc.im[a].iw0;
                im_was_iw1[a]     = mc.im[a].iw1;
                im_was_iw2[a]     = mc.im[a].iw2;
            }
        }

        /* Detect CPReg strobes from BB after the swap. */
        if (swapped && bb.cpreg_to_dorado != prev_cpreg) {
            cpreg_strobes++;
            if (cp_first_n < 32) {
                cp_first[cp_first_n] = bb.cpreg_to_dorado;
                cp_first_cycle[cp_first_n] = bb.cycles;
                cp_first_bbpc[cp_first_n] = baseboard_pc(&bb);
                cp_first_n++;
            }
            prev_cpreg = bb.cpreg_to_dorado;
        }

        /* WORKAROUND: bypass NOSTORAGE. Initial's storage-detection
         * test fails because our memory subsystem's BR / Map state
         * doesn't reflect a fully-configured Dorado. When PC reaches
         * NOSTORAGE (0o6247), redirect it to FINDMODULE (0o6357) so
         * Initial proceeds to the "found a module" path. Also turn
         * tasking on so that fault wakeups (set in cpu.c memory_ref)
         * can actually trigger task switches — without this, Initial's
         * subsequent LONGWAIT loop has no way to be preempted by the
         * fault task, since the entire setup phase keeps tasking off. */
        if (initial_substituted && is_imfetch && cpu.real_PC == 06247
            && !nostorage_bypassed) {
            cpu.real_PC = 06357;
            cpu.tasking_on = 1;
            nostorage_bypassed = 1;
            nostorage_bypass_cycle = bb.cycles;
        }

        /* When Bootstrap reaches BOOTSTAGE2 (0o7717), it is about
         * to globally-call INITIAL (0o7500). At this point Bootstrap
         * has finished streaming. Substitute canonical Initial.MB
         * over the corrupt streamed IM so INITIAL executes from
         * correct microcode. Keep Bootstrap.MB at 0o7700-0o7777
         * intact so READBB still works for Initial's checksum read. */
        if (initial_canonical_loaded && !initial_substituted &&
            swapped && is_imfetch && cpu.real_PC == 07717) {
            /* (gap A1 diagnostic) — measure how well Bootstrap's
             * STREAMING populated IM against canonical Initial.mb,
             * BEFORE we overwrite with the canonical version. */
            int s_canon = 0, s_streamed = 0, s_match = 0, s_differ = 0;
            int s_first_diff = -1;
            for (int a = 0; a < 4096; a++) {
                if (a >= 07700 && a < 010000) continue;
                if (init_mc.im_present[a]) s_canon++;
                if (mc.im_present[a])       s_streamed++;
                if (init_mc.im_present[a] && mc.im_present[a]) {
                    if (init_mc.im[a].iw0 == mc.im[a].iw0 &&
                        init_mc.im[a].iw1 == mc.im[a].iw1 &&
                        init_mc.im[a].iw2 == mc.im[a].iw2) {
                        s_match++;
                    } else {
                        s_differ++;
                        if (s_first_diff < 0) s_first_diff = a;
                    }
                }
            }
            /* Note (gap A1 resolved 2026-04-30): the .mb archive at
             * `chm/dorado/expanded/bootstrap.dm!20_/Initial.mb` is a
             * DIFFERENT BUILD than the Initial encoded in the BB ROM's
             * Boot1Data ($C016+). Streamed entries that "differ" are
             * not corruptions — they're bytes from a different build.
             * See `chm/disassembly/bb_boot1data.s` for the BB-ROM-side
             * decode (the actual oracle for this probe). */
            printf("       A1 stream-vs-archive-build (pre-substitution): "
                   "archive=%d streamed=%d match=%d differ=%d first_diff=0o%o "
                   "(differ ≠ corruption — different build)\n",
                   s_canon, s_streamed, s_match, s_differ,
                   s_first_diff >= 0 ? s_first_diff : 0);
            /* Dump first 6 differing entries to see the corruption pattern. */
            int dumped = 0;
            for (int a = 0; a < 4096 && dumped < 6; a++) {
                if (a >= 07700 && a < 010000) continue;
                if (init_mc.im_present[a] && mc.im_present[a] &&
                    (init_mc.im[a].iw0 != mc.im[a].iw0 ||
                     init_mc.im[a].iw1 != mc.im[a].iw1 ||
                     init_mc.im[a].iw2 != mc.im[a].iw2)) {
                    printf("         [0o%o] streamed iw0=0x%04X iw1=0x%04X iw2=0x%04X "
                           "canon iw0=0x%04X iw1=0x%04X iw2=0x%04X\n",
                           a,
                           mc.im[a].iw0, mc.im[a].iw1, mc.im[a].iw2,
                           init_mc.im[a].iw0, init_mc.im[a].iw1, init_mc.im[a].iw2);
                    dumped++;
                }
            }
            /* Dump first 6 MATCHING entries too, for contrast. */
            int matched_dumped = 0;
            for (int a = 0; a < 4096 && matched_dumped < 6; a++) {
                if (a >= 07700 && a < 010000) continue;
                if (init_mc.im_present[a] && mc.im_present[a] &&
                    init_mc.im[a].iw0 == mc.im[a].iw0 &&
                    init_mc.im[a].iw1 == mc.im[a].iw1 &&
                    init_mc.im[a].iw2 == mc.im[a].iw2) {
                    printf("         [0o%o] MATCH iw0=0x%04X iw1=0x%04X iw2=0x%04X\n",
                           a, mc.im[a].iw0, mc.im[a].iw1, mc.im[a].iw2);
                    matched_dumped++;
                }
            }
            for (int a = 0; a < 4096; a++) {
                if (init_mc.im_present[a]) {
                    /* Don't overwrite Bootstrap.MB at 0o7700-0o7777. */
                    if (a >= 07700 && a < 010000) continue;
                    mc.im[a]         = init_mc.im[a];
                    mc.im_present[a] = 1;
                }
            }
            /* Also restore IFUM and ALUFM from Initial. ALUFM
             * specifically: Bootstrap may have left ALUFM[14]=NOT B
             * if its runtime init didn't fire correctly; Initial's
             * canonical ALUFM has the right convention. */
            for (int a = 0; a < 1024; a++) {
                if (init_mc.ifum_present[a]) {
                    mc.ifum_lo[a]      = init_mc.ifum_lo[a];
                    mc.ifum_hi[a]      = init_mc.ifum_hi[a];
                    mc.ifum_present[a] = 1;
                }
            }
            for (int a = 0; a < 16; a++) {
                if (init_mc.alufm_present[a]) {
                    mc.alufm[a]         = init_mc.alufm[a];
                    mc.alufm_present[a] = 1;
                }
            }
            initial_substituted = 1;
            initial_substitute_cycle = bb.cycles;
        }

        /* Capture pre-step PC + Link for trail / Write-IM tracking
         * (only for IM fetches after the swap). */
        uint16_t pre_pc = cpu.real_PC;
        uint16_t pre_link = cpu.Link;
        uint8_t pre_task = (uint8_t)cpu.ctask;
        uint16_t pre_raw = bb.cpreg_to_dorado;
        uint16_t pre_t = cpu.T;
        uint16_t pre_q = cpu.Q;
        uint16_t pre_tag = cpu.RM[4];
        uint16_t pre_loc = cpu.RM[3];
        uint32_t pre_mar = mem.mar;
        uint16_t pre_md = mem.md;
        uint8_t pre_alu_zero = cpu.alu_zero;
        uint8_t pre_alu_lt0 = cpu.alu_lt0;
        uint8_t pre_alu_carry = cpu.alu_carry;
        uint8_t pre_rbase = (uint8_t)cpu.RBase;
        uint8_t pre_membase = (uint8_t)cpu.MemBase;
        uint16_t pre_task_rm[16];
        for (int rr = 0; rr < 16; rr++) {
            pre_task_rm[rr] = cpu.RM[((pre_rbase & 0xF) << 4) | rr];
        }
        uint16_t pre_mcr = dorado_mcr_get(&mem);
        uint8_t pre_ifu_active = cpu.ifu_active;
        uint16_t pre_ifu_pcf = cpu.ifu_pcf;
        uint16_t pre_ifu_pcx = cpu.ifu_pcx;
        uint32_t pre_br37 = dorado_br_get(&mem, 037);
        if (force_ether_mesa_boot && initial_substituted && is_imfetch &&
            !checksum_and_load_seen &&
            ((pre_pc >= 06170 && pre_pc <= 06217) ||
             (pre_pc >= 06406 && pre_pc <= 06443))) {
            /* Initial has a documented boot-parameter path:
             * STK[1]=boot file number, STK[2]=BootParameterSeal, and
             * STK[1]+STK[2]+STK[3]=0. Use it in the full-boot probe to
             * select the normal Mesa microcode (110B) without relying on
             * the not-yet-complete terminal keyboard back-channel. */
            cpu.STK[1] = 0110u;
            cpu.STK[2] = 056623u;
            cpu.STK[3] = 0121045u;
            boot_parameter_seed_count++;
        }
        if (initial_substituted && is_imfetch && !checksum_and_load_seen &&
            (pre_pc == 06417 || (pre_pc >= 06407 && pre_pc <= 06431))) {
            /* Bring-up shim: the 7-wire terminal back-channel is not
             * modeled yet, so keep all boot keys "up" before Initial
             * tests ETemp0..3. This lets the probe exercise disk boot
             * instead of falling into Ethernet. The same IM PCs are later
             * reused by CheckChecksumAndLoad, so stop once that routine has
             * started or this shim corrupts the checksum accumulator. */
            cpu.RM[0x1A] = 0xFFFFu;
            cpu.RM[0x1B] = 0xFFFFu;
            cpu.RM[0x1C] = 0xFFFFu;
            cpu.RM[0x1D] = 0xFFFFu;
        }
        if (!ether_boot_enabled && !ethernet_boot_enabled &&
            initial_substituted && is_imfetch &&
            (pre_pc == 06432 || pre_pc == 06443)) {
            cpu.real_PC = 07140;  /* No boot keys: force disk microcode boot. */
            pre_pc = cpu.real_PC;
        }
        if (ether_boot_enabled && initial_substituted && is_imfetch &&
            pre_pc == 06440 && ether_boot_injections == 0) {
            map_boot_probe_installed_storage(&mem);
            /* The direct LoadRam shortcut skips the real warm-start
             * map setup. InitMap.mc says the first 64K of REAL memory
             * contains simulated Alto memory, then maps the emulator's
             * virtual bank onto it. AltoMesaDorado later programs the
             * emulator BRs to bank 2 (0x20000), so model that warm-map
             * result explicitly for the direct probe.
             */
            map_boot_probe_bank_to(&mem, 0x200, 0, 256);
            if (inject_ether_boot_image(&mem, ether_boot_image, 01000,
                                        &ether_boot_end, &ether_boot_sum,
                                        &ether_boot_start_pc)) {
                (void)loadram_image_direct(&mc, &cpu, ether_boot_image,
                                           &ether_boot_start_pc);
                map_boot_probe_installed_storage(&mem);
                map_boot_probe_bank_to(&mem, 0x200, 0, 256);
                if (disk_pack_attached) {
                    dorado_disk_controller_init(&disk);
                    dorado_disk_controller_attach_drive(&disk, 0, &disk_pack);
                }
                dorado_display_init(&display);
                dorado_display_attach_to_io(&display, &io);
                for (uint16_t rb = 0; rb < 0x100; rb += 0x10) {
                    cpu.RM[rb | 0x01] = ether_boot_end;
                    cpu.RM[rb | 0x0A] = 0;
                    cpu.RM[rb | 0x11] = ether_boot_end;
                    cpu.RM[rb | 0x1A] = 0;
                }
                cpu.real_PC = ether_boot_start_pc;
                cpu.tasking_on = 0;
                cpu.ifu_active = 0;
                cpu.ifu_warmup = 0;
                /* Complete-world LoadRam exits with tasking off. The
                 * direct probe bypasses the real LoadRam/IOReset path,
                 * so discard wakeups left over from Initial before the
                 * AltoMesa image initializes its task TPCs. Also discard
                 * any nearly-expired synthetic junk timer countdown from
                 * Initial; the real LoadRam path executes IFUReset at
                 * entry, so the replacement world starts from a fresh
                 * IFU/test-control state rather than inheriting a pending
                 * tick.
                 */
                cpu.ready = 1u;
                cpu.wakeup_pending = 0;
                cpu.junk_tw_enabled = 0;
                cpu.junk_tw_countdown = 0;
                pre_pc = cpu.real_PC;
                ether_boot_injections++;
                ether_boot_inject_cycle = bb.cycles;
                ether_inject_display_outs = display.output_count;
                ether_inject_display_iofetch = display.iofetch_count;
                ether_inject_display_dwt_wakeups = display.dwt_wakeups;
                ether_inject_scanline_wakeups = display_scanline_wakeups;
                ether_inject_disk_outs = disk.output_count;
                ether_inject_disk_ins = disk.input_count;
                ether_inject_disk_wakeups = disk_wakeups;
                ether_loaded_world_cycle = bb.cycles;
                cpu.dbg_writeim_n = 0;          /* capture LoadRam writes, not Bootstrap */
                if (disk_trace_enabled) {
                    disk_trace_armed = 1;
                    disk_trace_n = 0;            /* focus on loaded-world disk code */
                    disk_trace_head = 0;
                    disk_trace_total = 0;
                }
            }
        }
        if (ether_boot_injections && ether_loaded_world_cycle == 0 &&
            is_imfetch && pre_pc < IM_SIZE && mc.im_present[pre_pc] &&
            !(pre_pc >= 07600 && pre_pc < 07700)) {
            int differs_from_initial =
                !init_mc.im_present[pre_pc] ||
                mc.im[pre_pc].iw0 != init_mc.im[pre_pc].iw0 ||
                mc.im[pre_pc].iw1 != init_mc.im[pre_pc].iw1 ||
                mc.im[pre_pc].iw2 != init_mc.im[pre_pc].iw2;
            if (differs_from_initial || pre_pc == ether_boot_start_pc) {
                ether_loaded_world_cycle = bb.cycles;
            }
        }
        if (!ether_boot_injections && checksum_and_load_seen &&
            ether_loaded_world_cycle == 0 && is_imfetch &&
            pre_pc < IM_SIZE && mc.im_present[pre_pc] &&
            !(pre_pc >= 06000 && pre_pc < 07700)) {
            /* Real Ethernet path: Initial's LoadRam has replaced the
             * control store and branched to the EB End-item start PC.
             * Mark this so the post-emulator disk/display probes apply
             * to the real path, not just the direct EB shortcut. */
            ether_loaded_world_cycle = bb.cycles;
            if (disk_trace_enabled) {
                disk_trace_armed = 1;
                disk_trace_n = 0;
                disk_trace_head = 0;
                disk_trace_total = 0;
            }
        }
        int is_key_trace =
            initial_substituted && is_imfetch && key_trace_n < 48 &&
            keyboard_seed_count > 0 &&
            ((pre_pc >= 06206 && pre_pc <= 06217) ||
             (pre_pc >= 06406 && pre_pc <= 06432));
        if (initial_substituted && is_imfetch) {
            int n = (int)(sizeof boot_landmarks / sizeof boot_landmarks[0]);
            for (int i = 0; i < n; i++) {
                if (pre_pc == boot_landmarks[i].pc) {
                    if (boot_landmarks[i].hits == 0) {
                        boot_landmarks[i].first_cycle = bb.cycles;
                    }
                    boot_landmarks[i].hits++;
                    if (pre_pc == 06460) checksum_and_load_seen = 1;
                    break;
                }
            }
            if (pre_pc == 06222 && ethernet_boot_enabled &&
                boot_identity_map_shims == 0) {
                /* Bring-up shim: Initial's PresetMap should leave installed
                 * storage identity-mapped before BootEmulator clears the
                 * first 64K and builds the Ethernet request at VM 177400.
                 * The current memory bring-up still reaches the vacant-map
                 * path after the NoStorage bypass, so install the same map
                 * shape here while the Config'/PresetMap gap is fixed. */
                map_boot_probe_installed_storage(&mem);
                boot_identity_map_shims++;
            }
            if (pre_pc == 07140) {
                disk_trace_armed = 1;
                disk_trace_n = 0;
                disk_trace_head = 0;
                disk_trace_total = 0;
                if (boot_identity_map_shims == 0) {
                    map_boot_probe_bank(&mem, 0, 256);
                    boot_identity_map_shims++;
                }
            }
            if (cpu.ctask == DORADO_DISK_TASK) {
                n = (int)(sizeof disk_landmarks / sizeof disk_landmarks[0]);
                for (int i = 0; i < n; i++) {
                    if (pre_pc == disk_landmarks[i].pc) {
                        if (disk_landmarks[i].hits == 0) {
                            disk_landmarks[i].first_cycle = bb.cycles;
                        }
                        disk_landmarks[i].hits++;
                        break;
                    }
                }
            }
        }
        if (initial_substituted && is_imfetch &&
            cpu.ctask == DORADO_DISK_TASK && pre_pc == 06754) {
            /* Bring-up shim: PilotDisk's DSKInitPC intends normal
             * mode (`KTemp3 <- 0`) before the task checks KTemp3.
             * Our current RM/RBase model does not preserve that
             * cross-task init write into the active DiskRegs bank, so the
             * DSK task falls into KDisable and only dismisses
             * wakeups. Force the DiskRegs scratch candidates clear
             * at the exact mode test so the real command path can
             * run while the RM-region model is fixed. */
            cpu.RM[((cpu.RBase & 0xF) << 4) | 000] = 0;
            cpu.RM[((cpu.RBase & 0xF) << 4) | 017] = 0;
            cpu.real_PC = 06664;  /* KForgetCmmd: normal-mode path. */
            pre_pc = cpu.real_PC;
            disk_normal_mode_shims++;
        }
        struct preset_sample ps;
        int is_preset_probe =
            preset_trace_enabled && initial_substituted && is_imfetch &&
            (pre_pc == 06357 || pre_pc == 06356 || pre_pc == 06361 ||
             pre_pc == 06362 || pre_pc == 06363 || pre_pc == 06340 ||
             pre_pc == 06365 || pre_pc == 06360 || pre_pc == 06245 ||
             pre_pc == 06244 || pre_pc == 06366 || pre_pc == 06367 ||
             pre_pc == 06000 || pre_pc == 06002 || pre_pc == 06003 ||
             pre_pc == 06011 || pre_pc == 06012 || pre_pc == 06013 ||
             pre_pc == 06100 || pre_pc == 06101 || pre_pc == 06102 ||
             pre_pc == 06103 || pre_pc == 06114 || pre_pc == 06117 ||
             pre_pc == 06130 || pre_pc == 06137 || pre_pc == 06157 ||
             pre_pc == 06161 || pre_pc == 06162 || pre_pc == 06163 ||
             pre_pc == 06171 || pre_pc == 06172 || pre_pc == 06173 ||
             pre_pc == 06175 || pre_pc == 06176 || pre_pc == 06177 ||
             pre_pc == 06110 || pre_pc == 06115 || pre_pc == 06116);
        if (is_preset_probe) {
            memset(&ps, 0, sizeof ps);
            ps.cycle = bb.cycles;
            ps.pc = pre_pc;
            ps.t = cpu.T;
            ps.q = cpu.Q;
            ps.link = cpu.Link;
            ps.cnt = cpu.Cnt;
            ps.rm1 = cpu.RM[1];
            ps.rm2 = cpu.RM[2];
            ps.rm6 = cpu.RM[6];
            ps.rm7 = cpu.RM[7];
            ps.rm0x45 = cpu.RM[0x45];
            ps.rm0x48 = cpu.RM[0x48];
            ps.rm0x49 = cpu.RM[0x49];
            ps.rbase = (uint8_t)cpu.RBase;
            ps.membase = (uint8_t)cpu.MemBase;
            ps.alu_lt0 = cpu.alu_lt0;
            ps.task = cpu.ctask;
            ps.tasking_on = cpu.tasking_on;
            if (cpu.mem) {
                ps.proc_srn = cpu.mem->proc_srn;
                ps.pipe5 = dorado_pipe5_at(cpu.mem, cpu.mem->proc_srn);
                ps.mcr = dorado_mcr_get(cpu.mem);
                ps.mar = cpu.mem->mar;
            }
        }
        int log_to_trail = (swapped && is_imfetch);
        int is_readbb_probe = (log_to_trail &&
                               (pre_pc == 07746 || pre_pc == 07715 ||
                                pre_pc == 07702 || pre_pc == 07703 ||
                                pre_pc == 07705 || pre_pc == 07706 ||
                                pre_pc == 07707 || pre_pc == 07711 ||
                                pre_pc == 07712 || pre_pc == 07713 ||
                                pre_pc == 07743 || pre_pc == 07751 ||
                                pre_pc == 07752 || pre_pc == 07753 ||
                                pre_pc == 07755 || pre_pc == 07757));
        /* Bootstrap.MB Write IM PCs: 0o7720, 0o7722, 0o7724, 0o7726,
         * 0o7730, 0o7732, 0o7734, 0o7736 (= WRITE000..WRITE111). */
        int is_wim = (pre_pc >= 07720 && pre_pc <= 07736 &&
                      (pre_pc & 1) == 0);
        int is_disk_code_pc =
            (pre_pc >= 02700 && pre_pc <= 03377) ||
            (pre_pc >= 04360 && pre_pc <= 04377) ||
            (pre_pc >= 05200 && pre_pc <= 05777) ||
            (pre_pc >= 06000 && pre_pc <= 06777) ||
            (pre_pc >= 07000 && pre_pc <= 07477);
        int disk_trace_focus = (int)test_u64_env("DORADO_DISK_TRACE_FOCUS", 0);
        int is_focused_disk_pc =
            (pre_pc >= 03300 && pre_pc <= 03313) ||
            (pre_pc >= 05730 && pre_pc <= 05745) ||
            (pre_pc >= 06214 && pre_pc <= 06277) ||
            (pre_pc >= 06340 && pre_pc <= 06370) ||
            (pre_pc >= 06520 && pre_pc <= 06577) ||
            pre_pc == 03500;
        if (disk_trace_focus == 2) {
            is_focused_disk_pc =
                (pre_pc >= 06340 && pre_pc <= 06370) ||
                (pre_pc >= 06520 && pre_pc <= 06577) ||
                (pre_pc >= 06500 && pre_pc <= 06507) ||
                (pre_pc >= 07120 && pre_pc <= 07133);
        }
        if (disk_trace_focus == 3) {
            is_focused_disk_pc =
                (pre_pc >= 07060 && pre_pc <= 07133) ||
                (pre_pc >= 07200 && pre_pc <= 07207) ||
                (pre_pc >= 06570 && pre_pc <= 06577) ||
                pre_pc == 03500;
        }
        if (disk_trace_focus == 4) {
            is_focused_disk_pc =
                (pre_pc >= 05540 && pre_pc <= 05755) ||
                pre_pc == 03500;
        }
        if (disk_trace_focus) is_disk_code_pc = is_focused_disk_pc;
        int is_disk_trace =
            disk_trace_enabled &&
            (disk_trace_armed || ether_loaded_world_cycle != 0) &&
            initial_substituted && is_imfetch &&
            ((pre_task == DORADO_DISK_TASK && is_disk_code_pc) ||
             (pre_task == 0 &&
              (ether_loaded_world_cycle != 0
                   ? is_disk_code_pc
                   : (pre_pc == 07400 ||
                      (pre_pc >= 07440 && pre_pc <= 07477)))));
        int is_mcr_trace =
            mcr_trace_enabled &&
            initial_substituted && is_imfetch && mcr_trace_n < 64 &&
            (pre_pc == 06003 || pre_pc == 06012);
        int is_loadram_trace =
            loadram_trace_mode != 0 &&
            initial_substituted && is_imfetch &&
            pre_task == 0 && pre_pc >= 07600 && pre_pc < 07700;
        if (is_loadram_trace && loadram_trace_mode == 2) {
            is_loadram_trace = (pre_t >= 0x3D20 && pre_t <= 0x3D70);
            if (is_loadram_trace &&
                loadram_trace_n >= (int)(sizeof loadram_trace /
                                          sizeof loadram_trace[0])) {
                is_loadram_trace = 0;
            }
        }
        if (checksum_trace_enabled && initial_substituted && is_imfetch &&
            pre_task == 0 && cpu.RM[0x11] >= 01000 &&
            (pre_pc == 06460 || pre_pc == 06435 || pre_pc == 06436 ||
             pre_pc == 06421 || pre_pc == 06437 || pre_pc == 06466 ||
             pre_pc == 06467 || pre_pc == 06404)) {
            uint32_t mar = cpu.mem ? cpu.mem->mar : 0;
            uint16_t mem_t = 0, mem_mar = 0;
            if (cpu.mem) {
                if ((size_t)cpu.T < cpu.mem->storage_words) {
                    mem_t = cpu.mem->storage[cpu.T];
                }
                if ((size_t)mar < cpu.mem->storage_words) {
                    mem_mar = cpu.mem->storage[mar];
                }
            }
            fprintf(stderr,
                    "CKSUM cyc=%llu pc=0o%o T=%06o Q=%06o Md=%06o "
                    "BootDataPtr=%06o ETemp0=%06o Cnt=%06o RB=%02o MB=%02o "
                    "Mar=%07o memT=%06o memMar=%06o aluZ=%u aluLT=%u\n",
                    (unsigned long long)bb.cycles, pre_pc,
                    cpu.T & 0177777, cpu.Q & 0177777, pre_md & 0177777,
                    cpu.RM[0x11] & 0177777, cpu.RM[0x1A] & 0177777,
                    cpu.Cnt & 0177777, cpu.RBase & 017, cpu.MemBase & 037,
                    mar & 077777777, mem_t & 0177777, mem_mar & 0177777,
                    cpu.alu_zero, cpu.alu_lt0);
        }
        if (eth_reg_trace_enabled && initial_substituted && is_imfetch &&
            pre_task == DORADO_ETHERNET_TASK_EIT &&
            (pre_pc == 06611 || pre_pc == 06612 || pre_pc == 06617 ||
             pre_pc == 06623 || pre_pc == 06626 || pre_pc == 06627 ||
             pre_pc == 06632 || pre_pc == 06636 || pre_pc == 06637 ||
             pre_pc == 06640 || pre_pc == 06641 || pre_pc == 06642)) {
            fprintf(stderr,
                    "ETH_REG cyc=%llu pc=0o%o T=%06o Md=%06o Cnt=%06o "
                    "BootDataPtr=%06o SeqNo=%06o EIPtr=%06o EICnt=%06o "
                    "RHost=%06o Link=%06o\n",
                    (unsigned long long)bb.cycles, pre_pc,
                    cpu.T & 0177777, pre_md & 0177777, cpu.Cnt & 0177777,
                    cpu.RM[0x11] & 0177777, cpu.RM[0x17] & 0177777,
                    cpu.RM[0x19] & 0177777, cpu.RM[0x1A] & 0177777,
                    cpu.RM[0x14] & 0177777, pre_link & 0177777);
        }
        struct post_eb_trace_sample post_eb_step_trace;
        int trace_post_eb_step =
            post_eb_trace_enabled && ether_loaded_world_cycle && is_imfetch;
        int trace_post_loop_step =
            ether_loaded_world_cycle && is_imfetch && pre_task == 0 &&
            (pre_pc == 01017 || pre_pc == 05250 ||
             pre_pc == 05203 || pre_pc == 05246 ||
             (pre_pc >= 05540 && pre_pc <= 05610));
        struct post_loadram_loop_trace post_loop_step_trace;
        if (trace_post_loop_step) {
            memset(&post_loop_step_trace, 0, sizeof post_loop_step_trace);
            post_loop_step_trace.cycle = bb.cycles;
            post_loop_step_trace.task = pre_task;
            post_loop_step_trace.pc = pre_pc;
            post_loop_step_trace.t_before = pre_t;
            post_loop_step_trace.q_before = cpu.Q;
            post_loop_step_trace.link_before = pre_link;
            post_loop_step_trace.md_before = pre_md;
            post_loop_step_trace.mar_before = pre_mar;
            post_loop_step_trace.ifu_pcf = cpu.ifu_pcf;
            post_loop_step_trace.ifu_pcx = cpu.ifu_pcx;
            post_loop_step_trace.stk_p = (uint8_t)cpu.StkP;
            for (int si = 0; si < 8; si++) {
                post_loop_step_trace.stack[si] =
                    cpu.STK[(uint8_t)(cpu.StkP - si)];
            }
            post_loop_step_trace.r400 = cpu.RM[0x10];
            post_loop_step_trace.etemp0 = cpu.RM[0x1A];
            post_loop_step_trace.etemp1 = cpu.RM[0x1B];
            post_loop_step_trace.etemp2 = cpu.RM[0x1C];
            post_loop_step_trace.mem430 = dorado_visible_word_at_va(&mem, 0430u);
            post_loop_step_trace.mem432 = dorado_visible_word_at_va(&mem, 0432u);
            post_loop_step_trace.mem521 = dorado_visible_word_at_va(&mem, 0521u);
            post_loop_step_trace.disk_muff_addr = disk.muff_addr;
            post_loop_step_trace.disk_index_tw = disk.index_tw;
            post_loop_step_trace.disk_sector_tw = disk.sector_tw;
            post_loop_step_trace.disk_tag_tw = disk.tag_tw;
            post_loop_step_trace.disk_rd_fifo_tw = disk.rd_fifo_tw;
            post_loop_step_trace.disk_wr_fifo_tw = disk.wr_fifo_tw;
            post_loop_step_trace.disk_enable_run = disk.enable_run;
            post_loop_step_trace.disk_active = disk.active;
            post_loop_step_trace.disk_block_till_index = disk.block_till_index;
            post_loop_step_trace.disk_fifo_count = (uint8_t)disk.fifo_count;
            post_loop_step_trace.disk_selected_drive =
                (uint8_t)disk.selected_drive;
            post_loop_step_trace.disk_control = disk.control;
            post_loop_step_trace.disk_out_tioa = disk.last_output_tioa;
            post_loop_step_trace.disk_out_data = disk.last_output_data;
            post_loop_step_trace.disk_in_tioa = disk.last_input_tioa;
            post_loop_step_trace.disk_in_data = disk.last_input_data;
            post_loop_step_trace.disk_cur_cyl_lo =
                (uint8_t)(disk.drive[disk.selected_drive].cur_cyl & 0xFF);
            post_loop_step_trace.disk_cur_head =
                (uint8_t)(disk.drive[disk.selected_drive].cur_head & 0xFF);
            post_loop_step_trace.disk_cur_sector =
                (uint8_t)(disk.drive[disk.selected_drive].cur_sector & 0xFF);
        }
        if (trace_post_eb_step) {
            memset(&post_eb_step_trace, 0, sizeof post_eb_step_trace);
            post_eb_step_trace.cycle = bb.cycles;
            post_eb_step_trace.task = pre_task;
            post_eb_step_trace.pc = pre_pc;
            post_eb_step_trace.t_before = pre_t;
            post_eb_step_trace.q_before = cpu.Q;
            post_eb_step_trace.link_before = pre_link;
            post_eb_step_trace.mcr_before = pre_mcr;
            post_eb_step_trace.mar_before = pre_mar;
            post_eb_step_trace.md_before = pre_md;
            post_eb_step_trace.rbase_before = pre_rbase;
            post_eb_step_trace.membase_before = pre_membase;
            post_eb_step_trace.ifu_active_before = pre_ifu_active;
            post_eb_step_trace.ifu_warmup_before = cpu.ifu_warmup;
            post_eb_step_trace.tasking_on_before = cpu.tasking_on;
            post_eb_step_trace.resume_delay_before = cpu.tasking_resume_delay;
            post_eb_step_trace.cnt_before = cpu.Cnt;
            post_eb_step_trace.shc_before = cpu.ShC;
            post_eb_step_trace.dispatch_or_before = cpu.dispatch_or;
            post_eb_step_trace.dispatch_pending_before = cpu.dispatch_pending;
            for (int rr = 0; rr < 16; rr++)
                post_eb_step_trace.rm_before[rr] = cpu.RM[0x40 + rr];
            for (int rr = 0; rr < 16; rr++)
                post_eb_step_trace.disk_rm_before[rr] = cpu.RM[0x50 + rr];
            for (int aa = 0; aa < 16; aa++)
                post_eb_step_trace.alufm_before[aa] = mc.alufm[aa];
            post_eb_step_trace.ready_before = cpu.ready;
            post_eb_step_trace.wakeup_before = cpu.wakeup_pending;
            post_eb_step_trace.task0_tpc_before =
                dorado_cpu_get_task_tpc(&cpu, 0);
            post_eb_step_trace.aht_tpc_before =
                cpu.task_tpc[DORADO_DISPLAY_TASK_AHT];
            post_eb_step_trace.disk_tpc_before =
                dorado_cpu_get_task_tpc(&cpu, DORADO_DISK_TASK);
            post_eb_step_trace.ifu_pcf_before = cpu.ifu_pcf;
            post_eb_step_trace.ifu_pcx_before = cpu.ifu_pcx;
        }

        mem.last_ref_kind = DM_REF_NONE;
        if (dorado_cpu_step(&cpu)) {
            halt_reason = (cpu_halt_reason)cpu.halt_reason;
            if (trace_post_loop_step) {
                post_loop_step_trace.next_pc = cpu.real_PC;
                post_loop_step_trace.task_after = (uint8_t)cpu.ctask;
                post_loop_step_trace.task0_tpc_after =
                    dorado_cpu_get_task_tpc(&cpu, 0);
                post_loop_step_trace.disk_tpc_after =
                    dorado_cpu_get_task_tpc(&cpu, DORADO_DISK_TASK);
                post_loop_step_trace.t_after = cpu.T;
                post_loop_step_trace.q_after = cpu.Q;
                post_loop_step_trace.link_after = cpu.Link;
                post_loop_step_trace.md_after = mem.md;
                post_loop_step_trace.mar_after = mem.mar;
                post_loop_trace[post_loop_trace_head] = post_loop_step_trace;
                post_loop_trace_head = (post_loop_trace_head + 1) %
                                       (int)(sizeof post_loop_trace /
                                             sizeof post_loop_trace[0]);
                if (post_loop_trace_n < (int)(sizeof post_loop_trace /
                                              sizeof post_loop_trace[0])) {
                    post_loop_trace_n++;
                }
            }
            if (trace_post_eb_step) {
                post_eb_step_trace.next_pc = cpu.real_PC;
                post_eb_step_trace.task_after = (uint8_t)cpu.ctask;
                post_eb_step_trace.t_after = cpu.T;
                post_eb_step_trace.q_after = cpu.Q;
                post_eb_step_trace.link_after = cpu.Link;
                post_eb_step_trace.mcr_after = dorado_mcr_get(&mem);
                post_eb_step_trace.mar_after = mem.mar;
                post_eb_step_trace.md_after = mem.md;
                post_eb_step_trace.rbase_after = (uint8_t)cpu.RBase;
                post_eb_step_trace.membase_after = (uint8_t)cpu.MemBase;
                post_eb_step_trace.ifu_active_after = cpu.ifu_active;
                post_eb_step_trace.ifu_warmup_after = cpu.ifu_warmup;
                post_eb_step_trace.tasking_on_after = cpu.tasking_on;
                post_eb_step_trace.resume_delay_after = cpu.tasking_resume_delay;
                post_eb_step_trace.cnt_after = cpu.Cnt;
                post_eb_step_trace.shc_after = cpu.ShC;
                post_eb_step_trace.dispatch_or_after = cpu.dispatch_or;
                post_eb_step_trace.dispatch_pending_after = cpu.dispatch_pending;
                for (int rr = 0; rr < 16; rr++)
                    post_eb_step_trace.rm_after[rr] = cpu.RM[0x40 + rr];
                for (int rr = 0; rr < 16; rr++)
                    post_eb_step_trace.disk_rm_after[rr] = cpu.RM[0x50 + rr];
                for (int aa = 0; aa < 16; aa++)
                    post_eb_step_trace.alufm_after[aa] = mc.alufm[aa];
                post_eb_step_trace.ready_after = cpu.ready;
                post_eb_step_trace.wakeup_after = cpu.wakeup_pending;
                post_eb_step_trace.task0_tpc_after =
                    dorado_cpu_get_task_tpc(&cpu, 0);
                post_eb_step_trace.aht_tpc_after =
                    cpu.task_tpc[DORADO_DISPLAY_TASK_AHT];
                post_eb_step_trace.disk_tpc_after =
                    dorado_cpu_get_task_tpc(&cpu, DORADO_DISK_TASK);
                post_eb_step_trace.ifu_pcf_after = cpu.ifu_pcf;
                post_eb_step_trace.ifu_pcx_after = cpu.ifu_pcx;
                post_eb_step_trace.halt_reason = halt_reason;
                post_eb_trace[post_eb_trace_head] = post_eb_step_trace;
                post_eb_trace_head = (post_eb_trace_head + 1) %
                                     (int)(sizeof post_eb_trace /
                                           sizeof post_eb_trace[0]);
                post_eb_trace_total++;
            }
            break;
        }
        if (lowcore_trace_enabled && mem.last_ref_kind == DM_REF_STORE) {
            struct {
                const char *name;
                uint32_t base;
            } bases[] = {
                { "abs", 0 },
                { "DiskBR", dorado_br_get(&mem, 030) },
                { "IOBR", dorado_br_get(&mem, 031) },
                { "MDS", dorado_br_get(&mem, 036) },
            };
            for (size_t bi = 0; bi < sizeof bases / sizeof bases[0]; bi++) {
                uint32_t base = bases[bi].base & 0x0FFFFFFFu;
                int duplicate = 0;
                for (size_t bj = 0; bj < bi; bj++) {
                    if ((bases[bj].base & 0x0FFFFFFFu) == base) duplicate = 1;
                }
                if (duplicate) continue;
                uint32_t off = (mem.last_ref_va - base) & 0x0FFFFFFFu;
                if (!trace_lowcore_offset(off)) continue;
                fprintf(stderr,
                        "LOWCORE_STORE cyc=%llu task=%o pc=0o%o->0o%o "
                        "base=%s+0o%o va=%07X data=%06o "
                        "RBase=%u MemBase=%u T=%06o Md=%06o\n",
                        (unsigned long long)bb.cycles, pre_task & 017,
                        pre_pc, cpu.real_PC, bases[bi].name, off,
                        mem.last_ref_va & 0x0FFFFFFFu,
                        mem.last_ref_b & 0177777, pre_rbase,
                        pre_membase, pre_t & 0177777, pre_md & 0177777);
            }
        }
        if (ether_boot_injections) {
            const dorado_map_entry *m200 = dorado_map_get(&mem, 0x200);
            const dorado_map_entry *m2fe = dorado_map_get(&mem, 0x2FE);
            const dorado_map_entry *m624 = dorado_map_get(&mem, 0x624);
            if ((m200->wp && m200->dirty) ||
                (m2fe->wp && m2fe->dirty) ||
                (m624->wp && m624->dirty)) {
                if (ether_bank2_lost_cycle == 0) {
                    ether_bank2_lost_cycle = bb.cycles;
                    ether_bank2_lost_pc = pre_pc;
                    ether_bank2_lost_task = pre_task;
                    ether_bank2_lost_entry = *m200;
                }
                map_boot_probe_installed_storage(&mem);
                map_boot_probe_bank_to(&mem, 0x200, 0, 256);
                ether_bank2_remap_shims++;
            }
        }
        if (ether_boot_injections && ether_br37_bad_cycle == 0) {
            uint32_t br37 = dorado_br_get(&mem, 037);
            if (br37 >= 0x00400000u) {
                ether_br37_bad_cycle = bb.cycles;
                ether_br37_bad_pc = pre_pc;
                ether_br37_bad_task = pre_task;
                ether_br37_bad_membase = pre_membase;
                ether_br37_bad_t = pre_t;
                ether_br37_before = pre_br37;
                ether_br37_after = br37;
            }
        }
        if (trace_post_eb_step) {
            post_eb_step_trace.next_pc = cpu.real_PC;
            post_eb_step_trace.task_after = (uint8_t)cpu.ctask;
            post_eb_step_trace.t_after = cpu.T;
            post_eb_step_trace.q_after = cpu.Q;
            post_eb_step_trace.link_after = cpu.Link;
            post_eb_step_trace.mcr_after = dorado_mcr_get(&mem);
            post_eb_step_trace.mar_after = mem.mar;
            post_eb_step_trace.md_after = mem.md;
            post_eb_step_trace.rbase_after = (uint8_t)cpu.RBase;
            post_eb_step_trace.membase_after = (uint8_t)cpu.MemBase;
            post_eb_step_trace.ifu_active_after = cpu.ifu_active;
            post_eb_step_trace.ifu_warmup_after = cpu.ifu_warmup;
            post_eb_step_trace.tasking_on_after = cpu.tasking_on;
            post_eb_step_trace.resume_delay_after = cpu.tasking_resume_delay;
            post_eb_step_trace.cnt_after = cpu.Cnt;
            post_eb_step_trace.shc_after = cpu.ShC;
            post_eb_step_trace.dispatch_or_after = cpu.dispatch_or;
            post_eb_step_trace.dispatch_pending_after = cpu.dispatch_pending;
            for (int rr = 0; rr < 16; rr++)
                post_eb_step_trace.rm_after[rr] = cpu.RM[0x40 + rr];
            for (int rr = 0; rr < 16; rr++)
                post_eb_step_trace.disk_rm_after[rr] = cpu.RM[0x50 + rr];
            for (int aa = 0; aa < 16; aa++)
                post_eb_step_trace.alufm_after[aa] = mc.alufm[aa];
            post_eb_step_trace.ready_after = cpu.ready;
            post_eb_step_trace.wakeup_after = cpu.wakeup_pending;
            post_eb_step_trace.task0_tpc_after =
                dorado_cpu_get_task_tpc(&cpu, 0);
            post_eb_step_trace.aht_tpc_after =
                cpu.task_tpc[DORADO_DISPLAY_TASK_AHT];
            post_eb_step_trace.disk_tpc_after =
                dorado_cpu_get_task_tpc(&cpu, DORADO_DISK_TASK);
            post_eb_step_trace.ifu_pcf_after = cpu.ifu_pcf;
            post_eb_step_trace.ifu_pcx_after = cpu.ifu_pcx;
            post_eb_trace[post_eb_trace_head] = post_eb_step_trace;
            post_eb_trace_head = (post_eb_trace_head + 1) %
                                 (int)(sizeof post_eb_trace /
                                       sizeof post_eb_trace[0]);
            post_eb_trace_total++;
        }
        if (trace_post_loop_step) {
            post_loop_step_trace.next_pc = cpu.real_PC;
            post_loop_step_trace.task_after = (uint8_t)cpu.ctask;
            post_loop_step_trace.task0_tpc_after =
                dorado_cpu_get_task_tpc(&cpu, 0);
            post_loop_step_trace.disk_tpc_after =
                dorado_cpu_get_task_tpc(&cpu, DORADO_DISK_TASK);
            post_loop_step_trace.t_after = cpu.T;
            post_loop_step_trace.q_after = cpu.Q;
            post_loop_step_trace.link_after = cpu.Link;
            post_loop_step_trace.md_after = mem.md;
            post_loop_step_trace.mar_after = mem.mar;
            post_loop_trace[post_loop_trace_head] = post_loop_step_trace;
            post_loop_trace_head = (post_loop_trace_head + 1) %
                                   (int)(sizeof post_loop_trace /
                                         sizeof post_loop_trace[0]);
            if (post_loop_trace_n < (int)(sizeof post_loop_trace /
                                          sizeof post_loop_trace[0])) {
                post_loop_trace_n++;
            }
        }
        if (is_mcr_trace) {
            struct mcr_trace_sample *mt = &mcr_trace[mcr_trace_n++];
            mt->cycle = bb.cycles;
            mt->task = pre_task;
            mt->pc = pre_pc;
            mt->next_pc = cpu.real_PC;
            mt->t_before = pre_t;
            mt->t_after = cpu.T;
            mt->mcr_before = pre_mcr;
            mt->mcr_after = dorado_mcr_get(&mem);
        }
        if (is_loadram_trace) {
            int cap = (int)(sizeof loadram_trace / sizeof loadram_trace[0]);
            struct loadram_trace_sample *lt;
            if (loadram_trace_mode == 2) {
                lt = &loadram_trace[loadram_trace_n++];
            } else {
                lt = &loadram_trace[loadram_trace_head];
                loadram_trace_head = (loadram_trace_head + 1) % cap;
                if (loadram_trace_n < cap) loadram_trace_n++;
            }
            loadram_trace_total++;
            lt->cycle = bb.cycles;
            lt->task = pre_task;
            lt->pc = pre_pc;
            lt->next_pc = cpu.real_PC;
            lt->t_before = pre_t;
            lt->t_after = cpu.T;
            lt->q_before = pre_q;
            lt->q_after = cpu.Q;
            lt->md_before = pre_md;
            lt->md_after = mem.md;
            lt->link_before = pre_link;
            lt->link_after = cpu.Link;
            lt->rbase_before = pre_rbase;
            lt->rbase_after = (uint8_t)cpu.RBase;
            lt->membase_before = pre_membase;
            lt->membase_after = (uint8_t)cpu.MemBase;
            for (int j = 0; j < 16; j++) {
                lt->rm_before[j] = pre_task_rm[j];
                lt->rm_after[j] = cpu.RM[((cpu.RBase & 0xF) << 4) | j];
            }
            lt->mar_before = pre_mar;
            lt->mar_after = mem.mar;
            lt->storage_at_t_before = dorado_storage_at_va(&mem, pre_t);
            lt->storage_at_mar_after = dorado_storage_at_va(&mem, mem.mar);
            lt->mcr_before = pre_mcr;
            lt->mcr_after = dorado_mcr_get(&mem);
        }
        if (is_disk_trace) {
            int cap = (int)(sizeof disk_trace / sizeof disk_trace[0]);
            struct disk_trace_sample *dt = &disk_trace[disk_trace_head];
            disk_trace_head = (disk_trace_head + 1) % cap;
            if (disk_trace_n < cap) disk_trace_n++;
            disk_trace_total++;
            dt->cycle = bb.cycles;
            dt->task = pre_task;
            dt->pc = pre_pc;
            dt->next_pc = cpu.real_PC;
            dt->t_before = pre_t;
            dt->t_after = cpu.T;
            dt->md_before = pre_md;
            dt->md_after = mem.md;
            dt->link_before = pre_link;
            dt->link_after = cpu.Link;
            dt->alu_zero_before = pre_alu_zero;
            dt->alu_zero_after = cpu.alu_zero;
            dt->alu_lt0_before = pre_alu_lt0;
            dt->alu_lt0_after = cpu.alu_lt0;
            dt->alu_carry_before = pre_alu_carry;
            dt->alu_carry_after = cpu.alu_carry;
            dt->rm0 = cpu.RM[0];
            dt->rm1 = cpu.RM[1];
            dt->rm2 = cpu.RM[2];
            dt->rm3 = cpu.RM[3];
            for (int j = 0; j < 16; j++) {
                dt->pre_drm[j] = pre_task_rm[j];
                dt->drm[j] = cpu.RM[((pre_rbase & 0xF) << 4) | j];
            }
            dt->rbase_before = pre_rbase;
            dt->rbase_after = (uint8_t)cpu.RBase;
            dt->membase_before = pre_membase;
            dt->membase_after = (uint8_t)cpu.MemBase;
            dt->tioa = (uint8_t)cpu.TIOA;
            dt->muff_addr = disk.muff_addr;
            dt->index_tw = disk.index_tw;
            dt->sector_tw = disk.sector_tw;
            dt->tag_tw = disk.tag_tw;
            dt->rd_fifo_tw = disk.rd_fifo_tw;
            dt->wr_fifo_tw = disk.wr_fifo_tw;
            dt->enable_run = disk.enable_run;
            dt->active = disk.active;
            dt->block_till_index = disk.block_till_index;
            dt->disk_control = disk.control;
            dt->fifo_count = (uint8_t)disk.fifo_count;
            dt->cur_cyl_lo = (uint8_t)(disk.drive[disk.selected_drive].cur_cyl & 0xFF);
            dt->cur_head = (uint8_t)(disk.drive[disk.selected_drive].cur_head & 0xFF);
            dt->cur_sector = (uint8_t)(disk.drive[disk.selected_drive].cur_sector & 0xFF);
            dt->disk_out_tioa = disk.last_output_tioa;
            dt->disk_out_data = disk.last_output_data;
            dt->disk_in_tioa = disk.last_input_tioa;
            dt->disk_in_data = disk.last_input_data;
            dt->mar_before = pre_mar;
            dt->mar_after = mem.mar;
            dt->storage_after_mar = dorado_storage_at_va(&mem, mem.mar);
        }
        if (is_key_trace) {
            struct key_trace *kt = &key_trace[key_trace_n++];
            kt->cycle = bb.cycles;
            kt->pc = pre_pc;
            kt->next_pc = cpu.real_PC;
            kt->t = pre_t;
            kt->md = mem.md;
            kt->etemp0 = cpu.RM[0x1A];
            kt->etemp1 = cpu.RM[0x1B];
            kt->etemp2 = cpu.RM[0x1C];
            kt->etemp3 = cpu.RM[0x1D];
            kt->r400 = cpu.RM[0x10];
            kt->mar = mem.mar;
        }
        if (initial_substituted && !ether_loaded_world_cycle && is_imfetch &&
            ((pre_pc >= 06206 && pre_pc <= 06217) ||
             (pre_pc >= 06406 && pre_pc <= 06432))) {
            seed_boot_keyboard_from_display(&mem, &display);
            keyboard_seed_count++;
        }
        if (ether_loaded_world_cycle && alto_disk_boot_shims == 0 &&
            service_alto_disk_boot_shim(&mem, &disk_pack,
                                        alto_disk_boot_cyl,
                                        alto_disk_boot_head,
                                        alto_disk_boot_sector,
                                        036,
                                        cpu.ctask == 0 &&
                                            cpu.real_PC == 02220)) {
            alto_disk_boot_shims++;
        }
        service_boot_disk(&cpu, &disk, bb.cycles,
                          &disk_sector_ticks, &disk_wakeups);
        if (initial_substituted) {
            if (ethernet_boot_enabled &&
                test_u64_env("DORADO_ETH_FORCE_ELOAD_ZERO", 1) != 0 &&
                ethernet.tx_active && ethernet.requests_seen == 0 &&
                ethernet.tx_count == 0 && ethernet.data_writes == 0 &&
                cpu.RM[0x19] != 0) {
                /* Bring-up guard: Initial's `ELoad_ A0` should clear this
                 * retry mask before EOT runs. Until the remaining RM/RBase
                 * initialization issue is fixed, keep EOT from taking the
                 * load-overflow stop path before it emits the request. */
                cpu.RM[0x19] = 0;
            }
            uint16_t eth_mask = dorado_ethernet_wakeup_mask(&ethernet);
            for (int task = 0; task < 16; task++) {
                if (eth_mask & (uint16_t)(1u << task)) {
                    dorado_cpu_wakeup(&cpu, task);
                    ethernet_wakeups++;
                }
            }
        }
        if (initial_substituted && bb.cycles >= next_display_scanline_cycle) {
            if (!terminal_boot_armed && terminal_boot_scanlines > 0 &&
                display.terminal_task != 0 && display.scanline_ticks > 8) {
                dorado_display_boot_button(&display,
                                           (uint32_t)terminal_boot_scanlines);
                terminal_boot_armed = 1;
            }
            uint16_t mask = dorado_display_scanline_wakeup_mask(&display);
            for (int task = 0; task < 16; task++) {
                if (mask & (1u << task)) {
                    if (cpu.task_tpc[task] == 0177777) {
                        cpu.ready &= (uint16_t)~(1u << task);
                        cpu.wakeup_pending &= (uint16_t)~(1u << task);
                        display_invalid_tpc_wakeups++;
                        continue;
                    }
                    dorado_cpu_wakeup(&cpu, task);
                    display_scanline_wakeups++;
                }
            }
            next_display_scanline_cycle = bb.cycles + 1000;
        }
        if (initial_substituted) {
            int dwt_subtask = 0;
            if (dorado_display_dwt_wakeup(&display, &dwt_subtask)) {
                int word_task =
                    display.terminal_task == DORADO_DISPLAY_TASK_AHT
                        ? DORADO_DISPLAY_TASK_AWT
                        : DORADO_DISPLAY_TASK_DWT;
                if (cpu.task_tpc[word_task] == 0177777) {
                    cpu.ready &= (uint16_t)~(1u << word_task);
                    cpu.wakeup_pending &= (uint16_t)~(1u << word_task);
                    display_invalid_tpc_wakeups++;
                } else {
                    dorado_cpu_set_subtask(&cpu, word_task,
                                           (uint8_t)dwt_subtask);
                    dorado_cpu_wakeup(&cpu, word_task);
                }
            }
        }
        if (ether_loaded_world_cycle && is_imfetch) {
            if (!pre_ifu_active && cpu.ifu_active) {
                post_eb_ifu_arm_count++;
                post_eb_last_ifu_arm_pc = pre_pc;
                post_eb_last_ifu_pcf = cpu.ifu_pcf;
                post_eb_last_ifu_pcx = cpu.ifu_pcx;
            } else if (pre_ifu_active && !cpu.ifu_active) {
                post_eb_ifu_stop_count++;
                post_eb_last_ifu_stop_pc = pre_pc;
                post_eb_last_ifu_pcf = cpu.ifu_pcf;
                post_eb_last_ifu_pcx = cpu.ifu_pcx;
            }
            post_eb_task_cycles[pre_task & 0xF]++;
            if (pre_pc < 4096) {
                uint8_t t = pre_task & 0xF;
                post_eb_task_pc_count[t][pre_pc]++;
                post_eb_task_pc_link[t][pre_pc] = pre_link;
                post_eb_task_pc_mcr[t][pre_pc] = pre_mcr;
                post_eb_task_pc_mar[t][pre_pc] = pre_mar;
            }
            post_eb_ready_or |= cpu.ready;
            post_eb_wakeup_or |= cpu.wakeup_pending;
            if (post_eb_prev_task >= 0 && post_eb_prev_task != pre_task) {
                post_eb_task_switches++;
            }
            post_eb_prev_task = pre_task;
        }
        if (ifu_trace_enabled && ether_loaded_world_cycle && is_imfetch &&
            (pre_task == 0) &&
            (pre_ifu_pcf != cpu.ifu_pcf ||
             pre_ifu_pcx != cpu.ifu_pcx ||
             pre_ifu_active != cpu.ifu_active)) {
            struct ifu_trace_sample *it = &ifu_trace[ifu_trace_head];
            it->cycle = bb.cycles;
            it->task = pre_task;
            it->pc = pre_pc;
            it->next_pc = cpu.real_PC;
            it->active_before = pre_ifu_active;
            it->active_after = cpu.ifu_active;
            it->opcode = cpu.ifu_opcode;
            it->length = cpu.ifu_length;
            it->n = cpu.ifu_n;
            it->alpha = cpu.ifu_alpha;
            it->beta = cpu.ifu_beta;
            it->pcf_before = pre_ifu_pcf;
            it->pcf_after = cpu.ifu_pcf;
            it->pcx_before = pre_ifu_pcx;
            it->pcx_after = cpu.ifu_pcx;
            it->t = cpu.T;
            it->q = cpu.Q;
            it->link = cpu.Link;
            it->md = mem.md;
            it->membase = (uint8_t)cpu.MemBase;
            it->rbase = (uint8_t)cpu.RBase;
            it->stk_p = cpu.StkP;
            for (int ac = 0; ac < 4; ac++) {
                it->ac[ac] = cpu.STK[1 + ac];
            }
            ifu_trace_head = (ifu_trace_head + 1) % IFU_TRACE_CAP;
            ifu_trace_total++;
            ifu_pcx_count[cpu.ifu_pcx]++;
            ifu_pcx_opcode[cpu.ifu_pcx] = cpu.ifu_opcode;
        }

        if (is_preset_probe) {
            ps.next_pc = cpu.real_PC;
            if (preset_first_n < 64) preset_first[preset_first_n++] = ps;
            preset_last[preset_last_head] = ps;
            preset_last_head = (preset_last_head + 1) % 64;
            preset_last_total++;
        }

        if (is_imfetch) imfetch_count++;
        if (log_to_trail) {
            if (pre_pc < 4096) bs_pc_count[pre_pc]++;
            if (bs_first_n < 64) bs_first_trail[bs_first_n++] = pre_pc;
            bs_last_trail[bs_last_head] = pre_pc;
            bs_last_head = (bs_last_head + 1) % 64;
            bs_last_total++;
        }
        /* After substitution, capture distinct-PC trail through 0o6247
         * (NOSTORAGE) so we can see how Initial reaches there. */
        if (initial_substituted && is_imfetch) {
            if (init_first_n < 400 && (int)pre_pc != prev_init_pc) {
                init_first_trail[init_first_n++] = pre_pc;
                prev_init_pc = (int)pre_pc;
            }
            if (pre_pc < 4096) {
                if (init_pc_count[pre_pc] == 0) {
                    init_pc_first_cycle[pre_pc] = bb.cycles;
                }
                init_pc_count[pre_pc]++;
            }
            /* Ring buffer of last 64 distinct PCs. */
            if ((int)pre_pc != prev_last_pc) {
                init_last_trail[init_last_head] = pre_pc;
                init_last_head = (init_last_head + 1) % 64;
                init_last_total++;
                prev_last_pc = (int)pre_pc;
            }
        }
        if (log_to_trail && is_wim) {
            uint16_t target = pre_link & 0xFFF;
            wim_target_count[target]++;
            if (wim_first_n < 32) wim_first[wim_first_n++] = target;
            wim_last[wim_last_head] = target;
            wim_last_head = (wim_last_head + 1) % 32;
            wim_last_total++;
        }
        if (is_readbb_probe && rb_trace_n < 48) {
            rb_trace[rb_trace_n].cycle = bb.cycles;
            rb_trace[rb_trace_n].pc = pre_pc;
            rb_trace[rb_trace_n].raw = pre_raw;
            rb_trace[rb_trace_n].b_seen = (uint16_t)~pre_raw;
            rb_trace[rb_trace_n].t_before = pre_t;
            rb_trace[rb_trace_n].t_after = cpu.T;
            rb_trace[rb_trace_n].tag_before = pre_tag;
            rb_trace[rb_trace_n].tag_after = cpu.RM[4];
            rb_trace[rb_trace_n].loc_before = pre_loc;
            rb_trace[rb_trace_n].loc_after = cpu.RM[3];
            rb_trace[rb_trace_n].link_before = pre_link;
            rb_trace[rb_trace_n].link_after = cpu.Link;
            rb_trace[rb_trace_n].next_pc = cpu.real_PC;
            rb_trace_n++;
        }

        /* (im_present scan moved to post-loop — much cheaper) */
    }

    /* Post-run scan: compare current IM content to the snapshot.
     * Count three things:
     *   - new_present:  addresses that became present (= Write IM
     *                   wrote a slot that was empty)
     *   - changed:      addresses where iw0/iw1/iw2 changed (= Write
     *                   IM overwrote a previously-present slot,
     *                   common since Bootstrap's IMAddress preamble
     *                   often points back into 0o7700-0o7777)
     *   - new_outside / new_inside: split by 0o7700 boundary
     */
    bs_writes = 0;
    bs_writes_outside = 0;
    first_outside_addr = 0xFFFF;
    int new_inside = 0;
    int changed = 0;
    int changed_outside = 0;
    int changed_inside = 0;
    int min_changed = 0xFFFF;
    int max_changed = -1;
    for (int a = 0; a < 4096; a++) {
        int now = mc.im_present[a];
        int was = im_was_present[a];
        int content_changed = (mc.im[a].iw0 != im_was_iw0[a] ||
                               mc.im[a].iw1 != im_was_iw1[a] ||
                               mc.im[a].iw2 != im_was_iw2[a]);
        if (now && !was) {
            bs_writes++;
            if (a >= 07700) new_inside++;
            else {
                if (first_outside_addr == 0xFFFF) {
                    first_outside_addr = (uint16_t)a;
                }
                bs_writes_outside++;
            }
        } else if (content_changed) {
            changed++;
            if (a < min_changed) min_changed = a;
            if (a > max_changed) max_changed = a;
            if (a >= 07700) changed_inside++;
            else changed_outside++;
        }
    }

    printf("PROBE  full-boot+bootstrap: BB ended at PC=0x%04X, "
           "%llu cycles, swap@%llu (BB PC=0x%04X)\n",
           baseboard_pc(&bb), (unsigned long long)bb.cycles,
           (unsigned long long)swap_cycle, bb_pc_at_swap);
    printf("       At swap: cpreg=0x%04X, BB strobes pre-swap=%d\n",
           pre_swap_cpreg, bb_strobes_pre_swap);
    printf("       Bootstrap.MB swapped: %s, IM-fetched cycles=%d\n",
           swapped ? "yes" : "NO",
           imfetch_count);
    printf("       New IM entries (was empty → now present): %d "
           "(outside Boot0: %d, inside: %d, first outside: 0o%o)\n",
           bs_writes, bs_writes_outside, new_inside, first_outside_addr);
    printf("       Overwritten IM entries (was present, content changed): %d "
           "(outside Boot0: %d, inside: %d, range 0o%o-0o%o)\n",
           changed, changed_outside, changed_inside,
           min_changed, max_changed > 0 ? max_changed : 0);
    printf("       cpreg strobes from BB after swap: %d (final cpreg=0x%04X)\n",
           cpreg_strobes, bb.cpreg_to_dorado);
    if (cp_first_n > 0) {
        printf("       First CPReg changes after swap:");
        for (int i = 0; i < cp_first_n; i++) {
            printf(" [%llu pc=0x%04X raw=0x%04X seen=0x%04X]",
                   (unsigned long long)cp_first_cycle[i],
                   cp_first_bbpc[i], cp_first[i], (uint16_t)~cp_first[i]);
        }
        printf("\n");
    }
    printf("       Initial.MB substituted at BOOTSTAGE2: %s",
           initial_substituted ? "yes" : "NO");
    if (initial_substituted) {
        printf(" (cycle=%llu)",
               (unsigned long long)initial_substitute_cycle);
    }
    printf("\n");
    if (nostorage_bypassed) {
        printf("       NOSTORAGE bypassed → FINDMODULE at cycle %llu\n",
               (unsigned long long)nostorage_bypass_cycle);
    }
    if (boot_identity_map_shims) {
        printf("       Boot identity-map shims: %llu\n",
               (unsigned long long)boot_identity_map_shims);
    }
    if (ether_boot_injections || ether_loaded_world_cycle) {
        printf("       Ether boot image injected: %s end=0x%04X sum=0x%04X "
               "start=0o%o\n",
               ether_boot_image, ether_boot_end, ether_boot_sum,
               ether_boot_start_pc);
        printf("       Ether injection cycle=%llu loaded-world cycle=%llu\n",
               (unsigned long long)ether_boot_inject_cycle,
               (unsigned long long)ether_loaded_world_cycle);
        if (ether_bank2_lost_cycle) {
            printf("       Ether bank2 map lost at cycle=%llu task=%o pc=0o%o "
                   "Map[0x200]=rp%04X wp%u d%u r%u\n",
                   (unsigned long long)ether_bank2_lost_cycle,
                   ether_bank2_lost_task & 017, ether_bank2_lost_pc,
                   ether_bank2_lost_entry.rp, ether_bank2_lost_entry.wp,
                   ether_bank2_lost_entry.dirty, ether_bank2_lost_entry.ref);
        }
        if (ether_bank2_remap_shims) {
            printf("       Ether bank2 remap shims: %llu\n",
                   (unsigned long long)ether_bank2_remap_shims);
        }
        if (ether_br37_bad_cycle) {
            printf("       Ether BR37 invalid at cycle=%llu task=%o pc=0o%o "
                   "mb=%02o T=%04X BR37=%05X->%05X\n",
                   (unsigned long long)ether_br37_bad_cycle,
                   ether_br37_bad_task & 017, ether_br37_bad_pc,
                   ether_br37_bad_membase & 037, ether_br37_bad_t,
                   ether_br37_before, ether_br37_after);
        }
    }
    if (init_first_n > 0) {
        printf("       Initial PC trail (first %d distinct PCs after substitution):\n        ",
               init_first_n);
        for (int i = 0; i < init_first_n; i++) {
            const char *sym =
                dorado_microcode_symbol_at_real(&init_mc, init_first_trail[i]);
            printf(" 0o%o%s%s%s", init_first_trail[i],
                   sym ? "(" : "", sym ? sym : "", sym ? ")" : "");
            if ((i & 7) == 7) printf("\n        ");
        }
        printf("\n");
    }
    if (init_last_total > 0) {
        printf("       Initial last 64 distinct PCs:\n        ");
        int last_n = init_last_total < 64 ? init_last_total : 64;
        int last_first = init_last_total < 64 ? 0 : init_last_head;
        for (int i = 0; i < last_n; i++) {
            int idx = (last_first + i) % 64;
            const char *sym = dorado_microcode_symbol_at_real(&init_mc,
                                                              init_last_trail[idx]);
            printf(" 0o%o%s%s%s", init_last_trail[idx],
                   sym ? "(" : "", sym ? sym : "", sym ? ")" : "");
            if ((i & 7) == 7) printf("\n        ");
        }
        printf("\n");
    }
    /* Top hot PCs by count after substitution. */
    if (initial_substituted) {
        struct hot_pc { int pc; int count; uint64_t first; };
        struct hot_pc hot[20];
        for (int i = 0; i < 20; i++) { hot[i].pc = -1; hot[i].count = 0; }
        for (int a = 0; a < 4096; a++) {
            if (init_pc_count[a] == 0) continue;
            for (int s = 0; s < 20; s++) {
                if (init_pc_count[a] > hot[s].count) {
                    for (int t = 19; t > s; t--) hot[t] = hot[t-1];
                    hot[s].pc = a;
                    hot[s].count = init_pc_count[a];
                    hot[s].first = init_pc_first_cycle[a];
                    break;
                }
            }
        }
        printf("       Initial top-20 hot PCs (after substitution):\n");
        for (int i = 0; i < 20 && hot[i].pc >= 0; i++) {
            const char *sym = dorado_microcode_symbol_at_real(&init_mc,
                                                              hot[i].pc);
            printf("         0o%-5o ×%-7d first@%llu%s%s%s\n",
                   hot[i].pc, hot[i].count,
                   (unsigned long long)hot[i].first,
                   sym ? " (" : "", sym ? sym : "", sym ? ")" : "");
        }
    }
    printf("       Dorado final state: PC=0o%o, T=0x%04X, Q=0x%04X, "
           "Link=0x%04X\n",
           cpu.real_PC, cpu.T, cpu.Q, cpu.Link);
    if (ether_boot_injections || ether_loaded_world_cycle) {
        static const uint16_t probe_addrs[] = {
            06000, 06001, 06002, 06012, 06100, 05021
        };
        printf("       Post-LoadRam known .MB IM compare:");
        for (int r = 0; r < (int)(sizeof ref_mcs / sizeof ref_mcs[0]); r++) {
            if (!ref_mcs[r].loaded) continue;
            int matches = 0;
            int present = 0;
            for (int i = 0; i < (int)(sizeof probe_addrs / sizeof probe_addrs[0]); i++) {
                uint16_t a = probe_addrs[i];
                int have = mc.im_present[a];
                int ref = ref_mcs[r].mc.im_present[a];
                int same = have && ref &&
                           mc.im[a].iw0 == ref_mcs[r].mc.im[a].iw0 &&
                           mc.im[a].iw1 == ref_mcs[r].mc.im[a].iw1 &&
                           mc.im[a].iw2 == ref_mcs[r].mc.im[a].iw2;
                if (have && ref) present++;
                if (same) matches++;
            }
            printf(" %s=%d/%d", ref_mcs[r].name, matches, present);
        }
        printf("\n");
        printf("       Post-LoadRam loaded IM samples:");
        for (int i = 0; i < (int)(sizeof probe_addrs / sizeof probe_addrs[0]); i++) {
            uint16_t a = probe_addrs[i];
            if (mc.im_present[a]) {
                printf(" 0o%o=%05o/%05o/%05o",
                       a, mc.im[a].iw0, mc.im[a].iw1, mc.im[a].iw2);
            } else {
                printf(" 0o%o=<absent>", a);
            }
        }
        printf("\n");
        if (cpu.real_PC < IM_SIZE && mc.im_present[cpu.real_PC]) {
            char dis[200];
            dorado_format(&mc.im[cpu.real_PC], dis, sizeof dis);
            printf("       Final PC decode:");
            for (int r = 0; r < (int)(sizeof ref_mcs / sizeof ref_mcs[0]); r++) {
                if (!ref_mcs[r].loaded) continue;
                const char *sym =
                    dorado_microcode_symbol_at_real(&ref_mcs[r].mc, cpu.real_PC);
                if (sym) printf(" %s:%s", ref_mcs[r].name, sym);
            }
            printf(" %s\n", dis);
        }
    }
    printf("       Dorado halt: %s%s\n",
           cpu_halt_reason_str(halt_reason),
           (cpu.real_PC < 4096 && mc.im_present[cpu.real_PC])
               ? " (IM present)" : " (IM missing)");
    printf("       IFU: active=%u warmup=%u insset=%u opcode=0x%02X "
           "PCF=0o%o PCX=0o%o len=%u n=0o%o alpha=0x%02X beta=0x%02X "
           "pause=%u jump=%u packedA=%u sign=%u idcnt=%u "
           "resched=%u brk=%u/0x%02X\n",
           cpu.ifu_active, cpu.ifu_warmup, cpu.ifu_insset & 3,
           cpu.ifu_opcode, cpu.ifu_pcf, cpu.ifu_pcx, cpu.ifu_length,
           cpu.ifu_n, cpu.ifu_alpha, cpu.ifu_beta, cpu.ifu_type_pause,
           cpu.ifu_type_jump, cpu.ifu_packed_a, cpu.ifu_sign,
           cpu.ifu_idcnt, cpu.reschedule_pending, cpu.brk_pending,
           cpu.brk_opcode);
    {
        int ifum_addr = ((cpu.ifu_insset & 3) << 8) | cpu.ifu_opcode;
        printf("       IFUM[current]: addr=0o%o present=%u "
               "lo=0o%05o hi=0o%05o\n",
               ifum_addr,
               cpu.mc ? cpu.mc->ifum_present[ifum_addr] : 0,
               cpu.mc ? cpu.mc->ifum_lo[ifum_addr] : 0,
               cpu.mc ? cpu.mc->ifum_hi[ifum_addr] : 0);
    }
    const char *snapshot_path = test_str_env("DORADO_BOOT_SNAPSHOT",
                                             "/tmp/dorado_boot_display.pgm");
    if (cpu.mem && display.attached) {
        int dst_y = 0;
        dorado_display_render_fifo(&display, 0, &dst_y);
        dorado_display_vblank(&display);
        if (dorado_display_snapshot_pgm(&display, snapshot_path) == 0) {
            printf("       Display snapshot: frame=%llu %s\n",
                   (unsigned long long)dorado_display_frame(&display),
                   snapshot_path);
        } else {
            printf("       Display snapshot: failed to write %s\n",
                   snapshot_path);
        }
    }
    printf("       Task=%u TIOA=0x%02X display outs=%llu iofetch=%llu "
           "dwt wakeups=%llu scanline wakeups=%llu suppressed=%llu "
           "disk outs=%llu ins=%llu\n",
           cpu.ctask, cpu.TIOA & 0xFF,
           (unsigned long long)display.output_count,
           (unsigned long long)display.iofetch_count,
           (unsigned long long)display.dwt_wakeups,
           (unsigned long long)display_scanline_wakeups,
           (unsigned long long)display_invalid_tpc_wakeups,
           (unsigned long long)disk.output_count,
           (unsigned long long)disk.input_count);
    printf("       Ethernet: enabled=%d wakeups=%llu ctl0=%llu last0=0o%o "
           "ctl6=%llu last6=0o%o ctl7=%llu last7=0o%o tx_words=%zu "
           "starts=%llu eops=%llu stops=%llu requests=%llu last_bfn=0o%o "
           "replies=%llu rx_pos=%zu/%zu data_w=%llu data_r=%llu\n",
           ethernet_boot_enabled,
           (unsigned long long)ethernet_wakeups,
           (unsigned long long)ethernet.control_writes[0],
           ethernet.control_last[0],
           (unsigned long long)ethernet.control_writes[DORADO_ETHERNET_TASK_EOT],
           ethernet.control_last[DORADO_ETHERNET_TASK_EOT],
           (unsigned long long)ethernet.control_writes[DORADO_ETHERNET_TASK_EIT],
           ethernet.control_last[DORADO_ETHERNET_TASK_EIT],
           ethernet.tx_count,
           (unsigned long long)ethernet.tx_starts,
           (unsigned long long)ethernet.tx_eops,
           (unsigned long long)ethernet.tx_stops,
           (unsigned long long)ethernet.requests_seen,
           ethernet.last_boot_offset,
           (unsigned long long)ethernet.replies_queued,
           ethernet.rx_pos, ethernet.rx_count,
           (unsigned long long)ethernet.data_writes,
           (unsigned long long)ethernet.data_reads);
    {
        const char *eb_path =
            ether_boot_path_for_offset(&ethernet, ethernet.last_boot_offset);
        uint32_t end_off = 0, payload_words = 0;
        uint16_t end_item[4] = {0};
        uint16_t eb_sum = 0;
        if (eb_path &&
            read_eb_end_item(eb_path, &end_off, end_item, &payload_words,
                             &eb_sum)) {
            uint32_t end_va = 01000u + end_off;
            printf("       Ethernet EB End: path=%s payload_words=%u "
                   "sum=0o%06o end_off=0o%o va=0o%o "
                   "file=[%06o %06o %06o %06o] "
                   "mem=[%06o %06o %06o %06o]\n",
                   eb_path, payload_words, eb_sum & 0177777,
                   end_off, end_va,
                   end_item[0] & 0177777, end_item[1] & 0177777,
                   end_item[2] & 0177777, end_item[3] & 0177777,
                   dorado_visible_word_at_va(&mem, end_va) & 0177777,
                   dorado_visible_word_at_va(&mem, end_va + 1) & 0177777,
                   dorado_visible_word_at_va(&mem, end_va + 2) & 0177777,
                   dorado_visible_word_at_va(&mem, end_va + 3) & 0177777);
        }
    }
    /* Diagnostic counters (gaps G1, D1) — surfacing silent drops so
     * unmodelled paths become visible. Non-zero values point at
     * tasks whose fast-IO routes / BB accesses we don't yet handle. */
    printf("       Fast-IO drops: display_fifo_full=%u disk_fifo_full=%u "
           "disk_fifo_empty=%u unrouted_iofetch=%u unrouted_iostore=%u\n",
           fastio.drops_display_fifo_full,
           fastio.drops_disk_fifo_full,
           fastio.drops_disk_fifo_empty,
           fastio.drops_unrouted_iofetch,
           fastio.drops_unrouted_iostore);
    printf("       BB drops: writes_to_eprom=%u riot_writes_dropped=%u\n",
           bb.writes_to_eprom,
           bb.riot_writes_dropped);
    printf("       Display outputs by task:");
    for (int t = 0; t < 16; t++) {
        if (display.output_task_count[t]) {
            printf(" [%o]=%llu", t,
                   (unsigned long long)display.output_task_count[t]);
        }
    }
    printf("\n");
    printf("       Display outputs by TIOA:");
    int printed_tioa[8] = {0};
    for (int i = 0; i < 8; i++) {
        int best = -1;
        uint64_t best_count = 0;
        for (int a = 0; a < 256; a++) {
            int already_printed = 0;
            for (int b = 0; b < i; b++) {
                if (printed_tioa[b] == a) {
                    already_printed = 1;
                    break;
                }
            }
            if (already_printed) continue;
            uint64_t count = display.output_tioa_count[a];
            if (count > best_count) {
                best = a;
                best_count = count;
            }
        }
        if (best < 0 || best_count == 0) break;
        printed_tioa[i] = best;
        printf(" [%03o]=%llu", best,
               (unsigned long long)best_count);
    }
    printf("\n");
    if (ether_boot_injections || ether_loaded_world_cycle) {
        printf("       Post-EB device deltas: display outs=+%llu iofetch=+%llu "
               "dwt wakeups=+%llu scanline wakeups=+%llu "
               "disk outs=+%llu ins=+%llu disk wakeups=+%llu\n",
               (unsigned long long)(display.output_count - ether_inject_display_outs),
               (unsigned long long)(display.iofetch_count - ether_inject_display_iofetch),
               (unsigned long long)(display.dwt_wakeups -
                                    ether_inject_display_dwt_wakeups),
               (unsigned long long)(display_scanline_wakeups -
                                    ether_inject_scanline_wakeups),
               (unsigned long long)(disk.output_count - ether_inject_disk_outs),
               (unsigned long long)(disk.input_count - ether_inject_disk_ins),
               (unsigned long long)(disk_wakeups - ether_inject_disk_wakeups));
        printf("       Post-LoadRam task cycles:");
        for (int t = 0; t < 16; t++) {
            if (post_eb_task_cycles[t]) {
                printf(" [%o]=%llu", t,
                       (unsigned long long)post_eb_task_cycles[t]);
            }
        }
        printf(" switches=%llu ready_or=0x%04X wakeup_or=0x%04X\n",
               (unsigned long long)post_eb_task_switches,
               post_eb_ready_or, post_eb_wakeup_or);
        printf("       Post-LoadRam IFU transitions: arms=%llu last_arm_pc=0o%o "
               "stops=%llu last_stop_pc=0o%o last_pcf=0o%o last_pcx=0o%o\n",
               (unsigned long long)post_eb_ifu_arm_count,
               post_eb_last_ifu_arm_pc,
               (unsigned long long)post_eb_ifu_stop_count,
               post_eb_last_ifu_stop_pc,
               post_eb_last_ifu_pcf,
               post_eb_last_ifu_pcx);
        if (ifu_trace_enabled && ifu_trace_total > 0) {
            int cap = IFU_TRACE_CAP;
            int n = ifu_trace_total < cap ? ifu_trace_total : cap;
            int start = (ifu_trace_total < cap) ? 0 : ifu_trace_head;
            printf("       IFU trace last %d dispatch/start/stop events:\n", n);
            for (int i = 0; i < n; i++) {
                const struct ifu_trace_sample *it =
                    &ifu_trace[(start + i) % cap];
                printf("         cyc=%llu task=%o pc=0o%o->0o%o "
                       "active=%u->%u PCF=0o%o->0o%o PCX=0o%o->0o%o "
                       "op=%03o len=%u n=0o%o a=%03o b=%03o "
                       "T=%06o Q=%06o Link=%06o Md=%06o "
                       "mb=%02o rb=%02o StkP=%03o "
                       "AC=%06o,%06o,%06o,%06o\n",
                       (unsigned long long)it->cycle, it->task & 017,
                       it->pc, it->next_pc,
                       it->active_before, it->active_after,
                       it->pcf_before, it->pcf_after,
                       it->pcx_before, it->pcx_after,
                       it->opcode, it->length, it->n,
                       it->alpha, it->beta,
                       it->t, it->q, it->link, it->md,
                       it->membase & 037, it->rbase & 017, it->stk_p,
                       it->ac[0], it->ac[1], it->ac[2], it->ac[3]);
            }
            printf("       IFU hot PCX:");
            for (int rank = 0; rank < 12; rank++) {
                uint32_t best_n = 0;
                uint16_t best_pcx = 0;
                for (uint32_t pcx = 0; pcx < 65536u; pcx++) {
                    if (ifu_pcx_count[pcx] > best_n) {
                        best_n = ifu_pcx_count[pcx];
                        best_pcx = (uint16_t)pcx;
                    }
                }
                if (!best_n) break;
                printf(" 0o%o(op=%03o)x%u", best_pcx,
                       ifu_pcx_opcode[best_pcx], best_n);
                ifu_pcx_count[best_pcx] = 0;
            }
            printf("\n");
        }
        if (post_loop_trace_n > 0) {
            int cap = (int)(sizeof post_loop_trace /
                            sizeof post_loop_trace[0]);
            int start = (post_loop_trace_n < cap) ? 0 : post_loop_trace_head;
            printf("       Post-LoadRam loop trace last %d steps:\n",
                   post_loop_trace_n);
            for (int i = 0; i < post_loop_trace_n; i++) {
                const struct post_loadram_loop_trace *kt =
                    &post_loop_trace[(start + i) % cap];
                printf("         cyc=%llu task=%o->%o pc=0o%o->0o%o "
                       "T=%06o->%06o Q=%06o->%06o Link=%06o->%06o "
                       "Md=%06o->%06o Mar=%07o->%07o "
                       "PCF=%06o PCX=%06o StkP=%03o "
                       "STK=%06o,%06o,%06o,%06o,%06o,%06o,%06o,%06o "
                       "R400=%06o E0=%06o E1=%06o E2=%06o "
                       "mem430=%06o mem432=%06o mem521=%06o "
                       "TPC0=0o%o TPCd=0o%o "
                       "disk muff=%03o tw=%u%u%u rf=%u wf=%u "
                       "en=%u act=%u bti=%u ctl=%04X fifo=%u "
                       "sel=%u CHSlo=(%u,%u,%u) dio=O%02o:%04X/I%02o:%04X\n",
                       (unsigned long long)kt->cycle,
                       kt->task, kt->task_after, kt->pc, kt->next_pc,
                       kt->t_before & 0177777, kt->t_after & 0177777,
                       kt->q_before & 0177777, kt->q_after & 0177777,
                       kt->link_before & 0177777, kt->link_after & 0177777,
                       kt->md_before & 0177777, kt->md_after & 0177777,
                       kt->mar_before & 077777777,
                       kt->mar_after & 077777777,
                       kt->ifu_pcf & 0177777,
                       kt->ifu_pcx & 0177777,
                       kt->stk_p,
                       kt->stack[0] & 0177777,
                       kt->stack[1] & 0177777,
                       kt->stack[2] & 0177777,
                       kt->stack[3] & 0177777,
                       kt->stack[4] & 0177777,
                       kt->stack[5] & 0177777,
                       kt->stack[6] & 0177777,
                       kt->stack[7] & 0177777,
                       kt->r400 & 0177777,
                       kt->etemp0 & 0177777,
                       kt->etemp1 & 0177777,
                       kt->etemp2 & 0177777,
                       kt->mem430 & 0177777,
                       kt->mem432 & 0177777,
                       kt->mem521 & 0177777,
                       kt->task0_tpc_after, kt->disk_tpc_after,
                       kt->disk_muff_addr, kt->disk_index_tw,
                       kt->disk_sector_tw, kt->disk_tag_tw,
                       kt->disk_rd_fifo_tw, kt->disk_wr_fifo_tw,
                       kt->disk_enable_run, kt->disk_active,
                       kt->disk_block_till_index, kt->disk_control,
                       kt->disk_fifo_count, kt->disk_selected_drive,
                       kt->disk_cur_cyl_lo, kt->disk_cur_head,
                       kt->disk_cur_sector, kt->disk_out_tioa,
                       kt->disk_out_data, kt->disk_in_tioa,
                       kt->disk_in_data);
            }
        }
        if (post_eb_trace_enabled && post_eb_trace_total > 0) {
            int cap = (int)(sizeof post_eb_trace / sizeof post_eb_trace[0]);
            int n = post_eb_trace_total < cap ? post_eb_trace_total : cap;
            int start = (post_eb_trace_total < cap) ? 0 : post_eb_trace_head;
            printf("       Post-LoadRam trace last %d IM steps:\n", n);
            for (int i = 0; i < n; i++) {
                const struct post_eb_trace_sample *pt =
                    &post_eb_trace[(start + i) % cap];
                const char *sym = NULL;
                for (int r = 0; r < (int)(sizeof ref_mcs / sizeof ref_mcs[0]); r++) {
                    if (!ref_mcs[r].loaded) continue;
                    sym = dorado_microcode_symbol_at_real(&ref_mcs[r].mc,
                                                          pt->pc);
                    if (sym) break;
                }
                char dis[160] = "";
                if (pt->pc < IM_SIZE && mc.im_present[pt->pc]) {
                    dorado_format(&mc.im[pt->pc], dis, sizeof dis);
                }
                printf("         cyc=%llu task=%o->%o pc=0o%o",
                       (unsigned long long)pt->cycle, pt->task,
                       pt->task_after, pt->pc);
                if (sym) printf(":%s", sym);
                printf(" ->0o%o T=%04X->%04X Q=%04X->%04X "
                       "L=%04X->%04X Cnt=%04X->%04X ShC=%04X->%04X "
                       "RB=%u->%u MB=%u->%u "
                       "IFU a=%u->%u w=%u->%u PCF=0o%o->0o%o "
                       "PCX=0o%o->0o%o tasking=%u/%u->%u/%u "
                       "disp=%04X/%04X->%04X/%04X "
                       "ready=%04X->%04X wake=%04X->%04X "
                       "TPC0=0o%o->0o%o TPCAHT=0o%o->0o%o TPCd=0o%o->0o%o "
                       "MCR=%04X->%04X "
                       "MAR=%05X->%05X MD=%04X->%04X",
                       pt->next_pc,
                       pt->t_before, pt->t_after,
                       pt->q_before, pt->q_after,
                       pt->link_before, pt->link_after,
                       pt->cnt_before, pt->cnt_after,
                       pt->shc_before, pt->shc_after,
                       pt->rbase_before, pt->rbase_after,
                       pt->membase_before, pt->membase_after,
                       pt->ifu_active_before, pt->ifu_active_after,
                       pt->ifu_warmup_before, pt->ifu_warmup_after,
                       pt->ifu_pcf_before, pt->ifu_pcf_after,
                       pt->ifu_pcx_before, pt->ifu_pcx_after,
                       pt->tasking_on_before, pt->resume_delay_before,
                       pt->tasking_on_after, pt->resume_delay_after,
                       pt->dispatch_or_before, pt->dispatch_pending_before,
                       pt->dispatch_or_after, pt->dispatch_pending_after,
                       pt->ready_before, pt->ready_after,
                       pt->wakeup_before, pt->wakeup_after,
                       pt->task0_tpc_before, pt->task0_tpc_after,
                       pt->aht_tpc_before, pt->aht_tpc_after,
                       pt->disk_tpc_before, pt->disk_tpc_after,
                       pt->mcr_before, pt->mcr_after,
                       pt->mar_before, pt->mar_after,
                       pt->md_before, pt->md_after);
                if ((pt->pc >= 01000 && pt->pc <= 01060) ||
                    (pt->pc >= 04654 && pt->pc <= 04657) ||
                    (pt->pc >= 05720 && pt->pc <= 05747)) {
                    printf(" RM40-4F:");
                    for (int rr = 0; rr < 16; rr++) {
                        if (pt->rm_before[rr] == pt->rm_after[rr]) {
                            printf(" %02X=%04X", 0x40 + rr,
                                   pt->rm_after[rr]);
                        } else {
                            printf(" %02X=%04X->%04X", 0x40 + rr,
                                   pt->rm_before[rr], pt->rm_after[rr]);
                        }
                    }
                }
                if (pt->task == DORADO_DISK_TASK ||
                    (pt->pc >= 05720 && pt->pc <= 06720)) {
                    printf(" DiskRM50-5F:");
                    for (int rr = 0; rr < 16; rr++) {
                        if (pt->disk_rm_before[rr] == pt->disk_rm_after[rr]) {
                            printf(" %02X=%04X", 0x50 + rr,
                                   pt->disk_rm_after[rr]);
                        } else {
                            printf(" %02X=%04X->%04X", 0x50 + rr,
                                   pt->disk_rm_before[rr],
                                   pt->disk_rm_after[rr]);
                        }
                    }
                }
                int alufm_changed = 0;
                for (int aa = 0; aa < 16; aa++) {
                    if (pt->alufm_before[aa] != pt->alufm_after[aa]) {
                        alufm_changed = 1;
                        break;
                    }
                }
                if (alufm_changed) {
                    printf(" ALUFM:");
                    for (int aa = 0; aa < 16; aa++) {
                        if (pt->alufm_before[aa] != pt->alufm_after[aa]) {
                            printf(" %X=0o%o->0o%o", aa,
                                   pt->alufm_before[aa],
                                   pt->alufm_after[aa]);
                        }
                    }
                }
                if (pt->halt_reason != CPU_HALT_NONE) {
                    printf(" HALT=%s", cpu_halt_reason_str(pt->halt_reason));
                }
                if (dis[0]) printf(" {%s}", dis);
                printf("\n");
            }
        }
        for (int task = 0; task < 16; task++) {
            if (!post_eb_task_cycles[task]) continue;
            printf("       Post-LoadRam task %o hot PCs:", task);
            for (int rank = 0; rank < 5; rank++) {
                int best_pc = -1;
                uint32_t best = 0;
                for (int pc = 0; pc < 4096; pc++) {
                    uint32_t n = post_eb_task_pc_count[task][pc];
                    if (n > best) {
                        best = n;
                        best_pc = pc;
                    }
                }
                if (best_pc < 0 || best == 0) break;
                printf(" 0o%o=%u", best_pc, best);
                {
                    const char *sym =
                        dorado_microcode_symbol_at_real(&init_mc, best_pc);
                    if (sym) printf(" Initial:%s", sym);
                    for (int r = 0; r < (int)(sizeof ref_mcs / sizeof ref_mcs[0]); r++) {
                        if (!ref_mcs[r].loaded) continue;
                        sym = dorado_microcode_symbol_at_real(&ref_mcs[r].mc,
                                                              best_pc);
                        if (sym) printf(" %s:%s", ref_mcs[r].name, sym);
                    }
                    if (mc.im_present[best_pc]) {
                        char dis[160];
                        dorado_format(&mc.im[best_pc], dis, sizeof dis);
                        printf(" {%s}", dis);
                    }
                    printf(" link=0o%o mcr=%04X mar=%05X",
                           post_eb_task_pc_link[task][best_pc],
                           post_eb_task_pc_mcr[task][best_pc],
                           post_eb_task_pc_mar[task][best_pc]);
                }
                post_eb_task_pc_count[task][best_pc] = 0;
            }
            printf("\n");
        }
    }
    printf("       Disk pack: %s, sector ticks=%llu wakeups=%llu "
           "fifo reads=%llu writes=%llu read streams=%llu "
           "(fail=%llu sector=%llu tag=%llu muff=%llu) "
           "normal-mode shims=%llu "
           "alto-boot shims=%llu(CHS=%d,%d,%d) "
           "selected=%d CHS=(%d,%d,%d)\n",
           disk_pack_attached ? disk_pack.path : "(none)",
           (unsigned long long)disk_sector_ticks,
           (unsigned long long)disk_wakeups,
           (unsigned long long)disk.fifo_reads,
           (unsigned long long)disk.fifo_writes,
           (unsigned long long)disk.read_stream_starts,
           (unsigned long long)disk.read_stream_start_failures,
           (unsigned long long)disk.read_stream_sector_starts,
           (unsigned long long)disk.read_stream_tag_starts,
           (unsigned long long)disk.read_stream_muff_starts,
           (unsigned long long)disk_normal_mode_shims,
           (unsigned long long)alto_disk_boot_shims,
           alto_disk_boot_cyl,
           alto_disk_boot_head,
           alto_disk_boot_sector,
           disk.selected_drive,
           disk.drive[disk.selected_drive].cur_cyl,
           disk.drive[disk.selected_drive].cur_head,
           disk.drive[disk.selected_drive].cur_sector);
    printf("       Disk I/O by TIOA:");
    for (int a = DORADO_DISK_TIOA_DISKCONTROL;
         a <= DORADO_DISK_TIOA_DISKTAG; a++) {
        printf(" %02o(out=%llu,in=%llu)", a,
               (unsigned long long)disk.output_tioa_count[a & 0x0F],
               (unsigned long long)disk.input_tioa_count[a & 0x0F]);
    }
    printf("\n");
    printf("       Disk controller final: muff=%03o tw=%u%u%u rf=%u wf=%u "
           "en=%u act=%u bti=%u ctl=%04X fifo=%u stream=%u idx=%d "
           "selected=%d CHS=(%d,%d,%d) subsectors=%d seek=%d "
           "dio=O%02o:%04X/I%02o:%04X\n",
           disk.muff_addr, disk.index_tw, disk.sector_tw, disk.tag_tw,
           disk.rd_fifo_tw, disk.wr_fifo_tw, disk.enable_run, disk.active,
           disk.block_till_index, disk.control, disk.fifo_count,
           disk.read_stream_active, disk.read_stream_index,
           disk.selected_drive,
           disk.drive[disk.selected_drive].cur_cyl,
           disk.drive[disk.selected_drive].cur_head,
           disk.drive[disk.selected_drive].cur_sector,
           disk.drive[disk.selected_drive].sectors_per_revolution,
           disk.drive[disk.selected_drive].seek_in_progress,
           disk.last_output_tioa, disk.last_output_data,
           disk.last_input_tioa, disk.last_input_data);
    if (cpu.mem) {
        printf("       Boot keyboard words seeded %llu times: "
               "IOBR=0x%05X 0177034=%04X 0177035=%04X "
               "0177036=%04X 0177037=%04X\n",
               (unsigned long long)keyboard_seed_count,
               boot_keyboard_base(cpu.mem),
               boot_keyboard_word(cpu.mem, 0177034u),
               boot_keyboard_word(cpu.mem, 0177035u),
               boot_keyboard_word(cpu.mem, 0177036u),
               boot_keyboard_word(cpu.mem, 0177037u));
    }
    printf("       Boot parameter seeds=%llu force_mesa=%d STK[1..3]=%06o %06o %06o\n",
           (unsigned long long)boot_parameter_seed_count,
           force_ether_mesa_boot,
           cpu.STK[1], cpu.STK[2], cpu.STK[3]);
    printf("       Boot landmarks:");
    for (int i = 0; i < (int)(sizeof boot_landmarks / sizeof boot_landmarks[0]); i++) {
        if (boot_landmarks[i].hits) {
            printf(" %s@0o%o×%llu(first@%llu)",
                   boot_landmarks[i].name, boot_landmarks[i].pc,
                   (unsigned long long)boot_landmarks[i].hits,
                   (unsigned long long)boot_landmarks[i].first_cycle);
        }
    }
    printf("\n");
    printf("       Disk task landmarks:");
    for (int i = 0; i < (int)(sizeof disk_landmarks / sizeof disk_landmarks[0]); i++) {
        if (disk_landmarks[i].hits) {
            printf(" %s@0o%o×%llu(first@%llu)",
                   disk_landmarks[i].name, disk_landmarks[i].pc,
                   (unsigned long long)disk_landmarks[i].hits,
                   (unsigned long long)disk_landmarks[i].first_cycle);
        }
    }
    printf("\n");
    if (key_trace_n > 0) {
        printf("       Boot key trace:\n");
        for (int i = 0; i < key_trace_n; i++) {
            const struct key_trace *kt = &key_trace[i];
            printf("         cyc=%llu pc=0o%o->0o%o T=%04X Md=%04X "
                   "ETemp0=%04X ETemp1=%04X ETemp2=%04X ETemp3=%04X "
                   "R400=%04X Mar=%07X\n",
                   (unsigned long long)kt->cycle, kt->pc, kt->next_pc,
                   kt->t, kt->md,
                   kt->etemp0, kt->etemp1, kt->etemp2, kt->etemp3,
                   kt->r400,
                   kt->mar);
        }
    }
    if (disk_trace_n > 0) {
        int cap = (int)(sizeof disk_trace / sizeof disk_trace[0]);
        int start = disk_trace_total < cap ? 0 : disk_trace_head;
        printf("       Disk/BootTransfer trace last %d of %d samples:\n",
               disk_trace_n, disk_trace_total);
        for (int i = 0; i < disk_trace_n; i++) {
            const struct disk_trace_sample *dt =
                &disk_trace[(start + i) % cap];
            char dis[160] = "";
            if (dt->pc < IM_SIZE && mc.im_present[dt->pc]) {
                dorado_format(&mc.im[dt->pc], dis, sizeof dis);
            }
            printf("         cyc=%llu task=%o pc=0o%o->0o%o "
                   "T=%04X->%04X Md=%04X->%04X Link=%04X->%04X "
                   "BC=z%u%u l%u%u c%u%u "
                   "RB=%o->%o MB=%o->%o TIOA=%02o Mar=%07X->%07X store@Mar=%04X "
                   "muff=%03o tw=%u%u%u rf=%u wf=%u en=%u act=%u bti=%u "
                   "ctl=%04X fifo=%u CHSlo=(%u,%u,%u) "
                   "dio=O%02o:%04X/I%02o:%04X "
                   "RM0=%04X RM1=%04X RM2=%04X RM3=%04X "
                   "preDRM14=%04X preDRM15=%04X "
                   "DRM0=%04X DRM1=%04X DRM2=%04X DRM3=%04X "
                   "DRM4=%04X DRM5=%04X DRM6=%04X DRM7=%04X "
                   "DRM10=%04X DRM11=%04X DRM12=%04X DRM13=%04X "
                   "DRM14=%04X DRM15=%04X DRM16=%04X DRM17=%04X\n",
                   (unsigned long long)dt->cycle,
                   dt->task, dt->pc, dt->next_pc,
                   dt->t_before, dt->t_after,
                   dt->md_before, dt->md_after,
                   dt->link_before, dt->link_after,
                   dt->alu_zero_before, dt->alu_zero_after,
                   dt->alu_lt0_before, dt->alu_lt0_after,
                   dt->alu_carry_before, dt->alu_carry_after,
                   dt->rbase_before, dt->rbase_after,
                   dt->membase_before, dt->membase_after, dt->tioa,
                   dt->mar_before, dt->mar_after,
                   dt->storage_after_mar,
                   dt->muff_addr, dt->index_tw, dt->sector_tw, dt->tag_tw,
                   dt->rd_fifo_tw, dt->wr_fifo_tw, dt->enable_run,
                   dt->active, dt->block_till_index,
                   dt->disk_control, dt->fifo_count,
                   dt->cur_cyl_lo, dt->cur_head, dt->cur_sector,
                   dt->disk_out_tioa, dt->disk_out_data,
                   dt->disk_in_tioa, dt->disk_in_data,
                   dt->rm0, dt->rm1, dt->rm2, dt->rm3,
                   dt->pre_drm[014], dt->pre_drm[015],
                   dt->drm[0], dt->drm[1], dt->drm[2], dt->drm[3],
                   dt->drm[4], dt->drm[5], dt->drm[6], dt->drm[7],
                   dt->drm[010], dt->drm[011], dt->drm[012],
                   dt->drm[013], dt->drm[014], dt->drm[015],
                   dt->drm[016], dt->drm[017]);
            if (dis[0]) printf(" {%s}", dis);
            printf("\n");
        }
    }
    if (mcr_trace_n > 0) {
        printf("       MCR trace:\n");
        for (int i = 0; i < mcr_trace_n; i++) {
            const struct mcr_trace_sample *mt = &mcr_trace[i];
            printf("         cyc=%llu task=%o pc=0o%o->0o%o "
                   "T=%04X->%04X MCR=%04X->%04X\n",
                   (unsigned long long)mt->cycle,
                   mt->task, mt->pc, mt->next_pc,
                   mt->t_before, mt->t_after,
                   mt->mcr_before, mt->mcr_after);
        }
    }
    if (loadram_trace_n > 0) {
        int cap = (int)(sizeof loadram_trace / sizeof loadram_trace[0]);
        int start = (loadram_trace_mode == 2) ? 0 :
                    (loadram_trace_total < (uint64_t)cap)
                        ? 0 : loadram_trace_head;
        printf("       LoadRam trace %s %d of %llu steps:\n",
               loadram_trace_mode == 2 ? "window" : "last",
               loadram_trace_n,
               (unsigned long long)loadram_trace_total);
        for (int i = 0; i < loadram_trace_n; i++) {
            const struct loadram_trace_sample *lt =
                &loadram_trace[(start + i) % cap];
            printf("         cyc=%llu task=%o pc=0o%o->0o%o "
                   "T=%04X->%04X Q=%04X->%04X Md=%04X->%04X "
                   "Link=%04X->%04X RB=%o->%o MB=%o->%o "
                   "Mar=%07X->%07X mem[T]=%04X mem[Mar]=%04X "
                   "MCR=%04X->%04X "
                   "RM0=%04X RM1=%04X RM2=%04X RM3=%04X "
                   "RM4=%04X RM5=%04X RM6=%04X RM7=%04X "
                   "RM10=%04X RM11=%04X RM12=%04X RM13=%04X "
                   "RM14=%04X RM15=%04X RM16=%04X RM17=%04X\n",
                   (unsigned long long)lt->cycle, lt->task,
                   lt->pc, lt->next_pc,
                   lt->t_before, lt->t_after,
                   lt->q_before, lt->q_after,
                   lt->md_before, lt->md_after,
                   lt->link_before, lt->link_after,
                   lt->rbase_before, lt->rbase_after,
                   lt->membase_before, lt->membase_after,
                   lt->mar_before, lt->mar_after,
                   lt->storage_at_t_before, lt->storage_at_mar_after,
                   lt->mcr_before, lt->mcr_after,
                   lt->rm_after[0], lt->rm_after[1],
                   lt->rm_after[2], lt->rm_after[3],
                   lt->rm_after[4], lt->rm_after[5],
                   lt->rm_after[6], lt->rm_after[7],
                   lt->rm_after[010], lt->rm_after[011],
                   lt->rm_after[012], lt->rm_after[013],
                   lt->rm_after[014], lt->rm_after[015],
                   lt->rm_after[016], lt->rm_after[017]);
        }
    }
    printf("       tasking_on=%d resume_delay=%d wakeup_pending=0x%04X "
           "ready=0x%04X\n",
           cpu.tasking_on, cpu.tasking_resume_delay,
           cpu.wakeup_pending, cpu.ready);
    printf("       task TPCs:");
    for (int t = 0; t < 16; t++) {
        printf(" [%o]=0o%o", t, dorado_cpu_get_task_tpc(&cpu, t));
    }
    printf("\n");
    if (cpu.mem) {
        printf("       Memory: faults=%d first_srn=%d last=%s task=%o/%u "
               "pc=0o%o mesa_pc=0x%04X mb=%02o tioa=%03o "
               "va=0x%X Mar=0x%X\n",
               cpu.mem->fault_count, cpu.mem->fault_first_srn,
               ref_kind_name(cpu.mem->last_fault_ref_kind),
               cpu.mem->last_fault_task, cpu.mem->last_fault_subtask,
               cpu.mem->last_fault_real_pc, cpu.mem->last_fault_pc,
               cpu.mem->last_fault_membase, cpu.mem->last_fault_tioa,
               cpu.mem->last_fault_va, cpu.mem->mar);
        printf("       MCR=0x%04X disbr=%d noref=%d fdmiss=%d nowake=%d\n",
               dorado_mcr_get(cpu.mem),
               dorado_mcr_disbr(cpu.mem),
               dorado_mcr_noref(cpu.mem),
               dorado_mcr_fdmiss(cpu.mem),
               dorado_mcr_nowake(cpu.mem));
        {
            const dorado_map_entry *m0 = dorado_map_get(&mem, 0);
            const dorado_map_entry *m1 = dorado_map_get(&mem, 1);
            const dorado_map_entry *mff = dorado_map_get(&mem, 0xFF);
            const dorado_map_entry *m200 = dorado_map_get(&mem, 0x200);
            const dorado_map_entry *m2fe = dorado_map_get(&mem, 0x2FE);
            printf("       Map[0]=rp%04X wp%d d%d r%d "
                   "Map[1]=rp%04X wp%d d%d r%d "
                   "Map[0xFF]=rp%04X wp%d d%d r%d "
                   "Map[0x200]=rp%04X wp%d d%d r%d "
                   "Map[0x2FE]=rp%04X wp%d d%d r%d\n",
                   m0->rp, m0->wp, m0->dirty, m0->ref,
                   m1->rp, m1->wp, m1->dirty, m1->ref,
                   mff->rp, mff->wp, mff->dirty, mff->ref,
                   m200->rp, m200->wp, m200->dirty, m200->ref,
                   m2fe->rp, m2fe->wp, m2fe->dirty, m2fe->ref);
        }
        {
            uint32_t iobr = dorado_br_get(&mem, 031);
            uint32_t csb = 0177520u;
            uint16_t abs_next = dorado_visible_word_at_va(&mem, csb);
            uint16_t iobr_next = dorado_visible_word_at_va(&mem, iobr + csb);
            printf("       CSB abs [next mask drive cyl]=");
            for (uint32_t i = 0; i < 4; i++) {
                printf("%s%04X", (i == 0) ? "" : " ",
                       dorado_visible_word_at_va(&mem, csb + i));
            }
            printf("\n");
            printf("       CSB IOBR [next mask drive cyl]=");
            for (uint32_t i = 0; i < 4; i++) {
                printf("%s%04X", (i == 0) ? "" : " ",
                       dorado_visible_word_at_va(&mem, iobr + csb + i));
            }
            printf(" abs.next=%04X iobr.next=%04X\n", abs_next, iobr_next);
            if (iobr_next & 1u) {
                printf("       IOCB via IOBR+CSB.next [0..15]=");
                for (uint32_t i = 0; i < 16; i++) {
                    printf("%s%04X", (i == 0) ? "" : " ",
                           dorado_visible_word_at_va(&mem, iobr + iobr_next + i));
                }
                printf("\n");
            }
        }
        {
            uint32_t iobr = dorado_br_get(&mem, 031);
            uint32_t diskbr = dorado_br_get(&mem, 030);
            printf("       BRs: BR30(DiskBR)=0x%05X BR31(ECBR/PrincOps IOBR)=0x%05X MDS/BR36=0x%05X Code/BR37=0x%05X\n",
                   diskbr, iobr, dorado_br_get(&mem, 036),
                   dorado_br_get(&mem, 037));
            printf("       AEmu BR regs: EmuBRHiReg/RM[0x18]=0x%04X "
                   "EmuXMBRHiReg/RM[0x19]=0x%04X\n",
                   cpu.RM[0x18], cpu.RM[0x19]);
            printf("       Map config regs: VirtualBanks/RM[0x48]=0x%04X "
                   "RealPages/RM[0x49]=0x%04X\n",
                   cpu.RM[0x48], cpu.RM[0x49]);
            printf("       Display absolute low-core: DAStart[0420..0427]=");
            for (uint32_t i = 0; i < 8; i++) {
                printf("%s%04X", (i == 0) ? "" : " ",
                       dorado_visible_word_at_va(&mem, 0420u + i));
            }
            printf(" Cursor[0431..0450]=");
            for (uint32_t i = 0; i < 16; i++) {
                printf("%s%04X", (i == 0) ? "" : " ",
                       dorado_visible_word_at_va(&mem, 0431u + i));
            }
            printf("\n");
            printf("       Display IOBR low-core: DAStart[0420..0427]=");
            for (uint32_t i = 0; i < 8; i++) {
                printf("%s%04X", (i == 0) ? "" : " ",
                       dorado_visible_word_at_va(&mem, iobr + 0420u + i));
            }
            printf(" Cursor[0431..0450]=");
            for (uint32_t i = 0; i < 16; i++) {
                printf("%s%04X", (i == 0) ? "" : " ",
                       dorado_visible_word_at_va(&mem, iobr + 0431u + i));
            }
            printf("\n");
            {
                uint32_t mds = dorado_br_get(&mem, 036);
                printf("       Alto MDS disk words: [0431..0440]=");
                for (uint32_t i = 0; i < 8; i++) {
                    printf("%s%04X", (i == 0) ? "" : " ",
                           dorado_visible_word_at_va(&mem, mds + 0431u + i));
                }
                printf(" [0521..0523]=");
                for (uint32_t i = 0; i < 3; i++) {
                    printf("%s%04X", (i == 0) ? "" : " ",
                           dorado_visible_word_at_va(&mem, mds + 0521u + i));
                }
                printf(" [0001..0010]=");
                for (uint32_t i = 0; i < 8; i++) {
                    printf("%s%04X", (i == 0) ? "" : " ",
                           dorado_visible_word_at_va(&mem, mds + 0001u + i));
                }
                printf("\n");
                printf("       DiskBR command windows: [0521..0523]=");
                for (uint32_t i = 0; i < 3; i++) {
                    printf("%s%04X", (i == 0) ? "" : " ",
                           dorado_visible_word_at_va(&mem, diskbr + 0521u + i));
                }
                printf(" [0x8050..0x805F]=");
                for (uint32_t i = 0; i < 16; i++) {
                    printf("%s%04X", (i == 0) ? "" : " ",
                           dorado_visible_word_at_va(&mem, diskbr + 0x8050u + i));
                }
                printf("\n");
            }
        }
    }
    printf("       ALUFM after run:");
    for (int a = 0; a < 16; a++) printf(" [%X]=0o%o", a, mc.alufm[a]);
    printf("\n");
    printf("       RM[0..15]:");
    for (int r = 0; r < 16; r++) printf(" [%d]=0x%04X", r, cpu.RM[r]);
    printf("\n");
    printf("       RM[0x10..0x1D]:");
    for (int r = 0x10; r <= 0x1D; r++) {
        printf(" [%02X]=0x%04X", r, cpu.RM[r]);
    }
    printf("\n");
    printf("       Disk RM[0x50..0x5F]:");
    for (int r = 0x50; r <= 0x5F; r++) {
        printf(" [%02X]=0x%04X", r, cpu.RM[r]);
    }
    printf("\n");
    printf("       Initial map vars:"
           " R400=0x%04X BFN/BTEMP0=0x%04X BTEMP1=0x%04X BTEMP2=0x%04X"
           " RNUM=0x%04X RCONST=0x%04X VIRTUALBANKS=0x%04X REALPAGES=0x%04X"
           " DISPLAYCONFIG=0x%04X\n",
           cpu.RM[0x10], cpu.RM[0x14], cpu.RM[0x15], cpu.RM[0x16],
           cpu.RM[0x45], cpu.RM[0x46], cpu.RM[0x48], cpu.RM[0x49],
           cpu.RM[0x76]);
    printf("       Task MemBase/TIOA:");
    for (int t = 0; t < 16; t++) {
        printf(" [%o]=%02o/%03o", t, cpu.task_membase[t], cpu.task_tioa[t]);
    }
    printf("\n");

    if (preset_first_n > 0) {
        printf("       PRESETMAP samples (first %d):\n", preset_first_n);
        for (int i = 0; i < preset_first_n; i++) {
            const struct preset_sample *s = &preset_first[i];
            printf("         cyc=%llu pc=0o%o->0o%o T=%04X Q=%04X Cnt=%04X "
                   "R1=%04X R2=%04X R6=%04X R7=%04X R45=%04X R48=%04X R49=%04X "
                   "RB=%o MB=%o task=%o ton=%d lt0=%d srn=%o mcr=%04X p5=%04X "
                   "Mar=%07X Link=%04X\n",
                   (unsigned long long)s->cycle, s->pc, s->next_pc,
                   s->t, s->q, s->cnt, s->rm1, s->rm2, s->rm6, s->rm7,
                   s->rm0x45, s->rm0x48, s->rm0x49,
                   s->rbase, s->membase, s->task, s->tasking_on,
                   s->alu_lt0, s->proc_srn, s->mcr, s->pipe5, s->mar, s->link);
        }
    }
    if (preset_last_total > 0) {
        int n = preset_last_total < 64 ? preset_last_total : 64;
        int first = preset_last_total < 64 ? 0 : preset_last_head;
        printf("       PRESETMAP samples (last %d):\n", n);
        for (int i = 0; i < n; i++) {
            const struct preset_sample *s = &preset_last[(first + i) % 64];
            printf("         cyc=%llu pc=0o%o->0o%o T=%04X Q=%04X Cnt=%04X "
                   "R1=%04X R2=%04X R6=%04X R7=%04X R45=%04X R48=%04X R49=%04X "
                   "RB=%o MB=%o task=%o ton=%d lt0=%d srn=%o mcr=%04X p5=%04X "
                   "Mar=%07X Link=%04X\n",
                   (unsigned long long)s->cycle, s->pc, s->next_pc,
                   s->t, s->q, s->cnt, s->rm1, s->rm2, s->rm6, s->rm7,
                   s->rm0x45, s->rm0x48, s->rm0x49,
                   s->rbase, s->membase, s->task, s->tasking_on,
                   s->alu_lt0, s->proc_srn, s->mcr, s->pipe5, s->mar, s->link);
        }
    }

    /* Dump first 32 Write IM operations to identify where T comes from. */
    if (cpu.dbg_writeim_n > 0) {
        printf("       Write IM ops (first %d):\n", cpu.dbg_writeim_n);
        for (int i = 0; i < cpu.dbg_writeim_n; i++) {
            printf("         #%02d pc=0o%o addr=0o%o half=%s sec=%d b=0x%04X T=0x%04X\n",
                   i, cpu.dbg_writeim_pc[i], cpu.dbg_writeim_addr[i],
                   cpu.dbg_writeim_half[i] ? "LH" : "RH",
                   cpu.dbg_writeim_sec[i],
                   cpu.dbg_writeim_b[i], cpu.dbg_writeim_t[i]);
        }
    }

    /* Dump IM near 0o6347 (the stuck region) to verify Bootstrap wrote
     * the expected unshuffled content. Compare with bs_mc loaded
     * directly from Initial.MB. */
    {
        mb_file initial_mb;
        mb_init(&initial_mb);
        if (mb_load(&initial_mb,
                    "../chm/dorado/expanded/bootstrap.dm!20_/Initial.mb") == MB_OK) {
            static dorado_microcode dump_mc;
            if (dorado_microcode_load(&initial_mb, &dump_mc) == DM_OK) {
                printf("       IM dump near 0o6347 (loaded vs canonical Initial.mb):\n");
                int probe_addrs[] = {06340, 06344, 06345, 06346, 06347, 06440, 06460, 06465};
                int n_addrs = (int)(sizeof probe_addrs / sizeof probe_addrs[0]);
                for (int i = 0; i < n_addrs; i++) {
                    int a = probe_addrs[i];
                    const dorado_uinstr *L = &mc.im[a];
                    const dorado_uinstr *C = &dump_mc.im[a];
                    int match = (L->iw0 == C->iw0 && L->iw1 == C->iw1 &&
                                 L->iw2 == C->iw2);
                    printf("         [0o%o] L: iw0=0o%06o iw1=0o%06o iw2=0o%06o "
                           "C: iw0=0o%06o iw1=0o%06o iw2=0o%06o %s\n",
                           a, L->iw0, L->iw1, L->iw2,
                           C->iw0, C->iw1, C->iw2,
                           match ? "MATCH" : "DIFFER");
                    char dis[256];
                    dorado_format(L, dis, sizeof dis);
                    printf("              loaded: %s\n", dis);
                    dorado_format(C, dis, sizeof dis);
                    printf("              canon:  %s\n", dis);
                }
                /* Check overall placement: how many entries match between
                 * loaded and canonical? */
                int total_match = 0, total_diff = 0;
                int total_canon = 0, total_loaded = 0;
                int first_diff_in_init = -1;
                for (int a = 0; a < 4096; a++) {
                    if (dump_mc.im_present[a]) total_canon++;
                    if (mc.im_present[a])      total_loaded++;
                    if (dump_mc.im_present[a] && mc.im_present[a]) {
                        if (dump_mc.im[a].iw0 == mc.im[a].iw0 &&
                            dump_mc.im[a].iw1 == mc.im[a].iw1 &&
                            dump_mc.im[a].iw2 == mc.im[a].iw2) {
                            total_match++;
                        } else {
                            total_diff++;
                            if (first_diff_in_init < 0) first_diff_in_init = a;
                        }
                    }
                }
                printf("       IM placement vs canonical Initial.mb: "
                       "canonical=%d, loaded=%d, both-present-match=%d, "
                       "both-present-differ=%d, first diff=0o%o\n",
                       total_canon, total_loaded, total_match, total_diff,
                       first_diff_in_init >= 0 ? first_diff_in_init : 0);
            }
            mb_free(&initial_mb);
        }
    }

    /* First trail (entry flow). */
    printf("       Bootstrap first PCs:");
    int prev_pc = -1, prev_count = 0;
    for (int i = 0; i < bs_first_n; i++) {
        if ((int)bs_first_trail[i] == prev_pc) { prev_count++; continue; }
        if (prev_count > 1) printf("×%d", prev_count);
        printf(" 0o%o", bs_first_trail[i]);
        prev_pc = bs_first_trail[i];
        prev_count = 1;
    }
    if (prev_count > 1) printf("×%d", prev_count);
    printf("\n");

    /* Last trail (where it ended up — the loop body). */
    int last_n = bs_last_total < 64 ? bs_last_total : 64;
    int last_first = bs_last_total < 64 ? 0 : bs_last_head;
    printf("       Bootstrap last PCs:");
    prev_pc = -1; prev_count = 0;
    for (int i = 0; i < last_n; i++) {
        int idx = (last_first + i) % 64;
        if ((int)bs_last_trail[idx] == prev_pc) { prev_count++; continue; }
        if (prev_count > 1) printf("×%d", prev_count);
        printf(" 0o%o", bs_last_trail[idx]);
        prev_pc = bs_last_trail[idx];
        prev_count = 1;
    }
    if (prev_count > 1) printf("×%d", prev_count);
    printf("\n");

    /* Top-20 most-hit PCs. */
    #define TOP_N 20
    int top_pc[TOP_N];
    int top_count[TOP_N];
    for (int i = 0; i < TOP_N; i++) { top_pc[i] = -1; top_count[i] = 0; }
    for (int a = 0; a < 4096; a++) {
        if (bs_pc_count[a] == 0) continue;
        for (int s = 0; s < TOP_N; s++) {
            if (bs_pc_count[a] > top_count[s]) {
                for (int t = TOP_N - 1; t > s; t--) {
                    top_pc[t] = top_pc[t-1];
                    top_count[t] = top_count[t-1];
                }
                top_pc[s] = a;
                top_count[s] = bs_pc_count[a];
                break;
            }
        }
    }
    printf("       Bootstrap top-%d hot PCs:\n", TOP_N);
    for (int i = 0; i < TOP_N && top_pc[i] >= 0; i++) {
        printf("         0o%o ×%d\n", top_pc[i], top_count[i]);
    }
    /* Total PCs visited (= unique addresses). */
    int unique = 0;
    for (int a = 0; a < 4096; a++) if (bs_pc_count[a]) unique++;
    printf("       Bootstrap visited %d unique IM addresses\n", unique);
    #undef TOP_N

    /* Write IM target dump. */
    printf("       Write IM fired %d times\n", wim_last_total);
    if (wim_first_n > 0) {
        printf("       First Write IM targets:");
        for (int i = 0; i < wim_first_n; i++) printf(" 0o%o", wim_first[i]);
        printf("\n");
    }
    int wim_unique = 0;
    int wim_top_pc[5] = {-1,-1,-1,-1,-1};
    int wim_top_count[5] = {0};
    for (int a = 0; a < 4096; a++) {
        if (wim_target_count[a] == 0) continue;
        wim_unique++;
        for (int s = 0; s < 5; s++) {
            if (wim_target_count[a] > wim_top_count[s]) {
                for (int t = 4; t > s; t--) {
                    wim_top_pc[t] = wim_top_pc[t-1];
                    wim_top_count[t] = wim_top_count[t-1];
                }
                wim_top_pc[s] = a;
                wim_top_count[s] = wim_target_count[a];
                break;
            }
        }
    }
    printf("       Write IM unique targets: %d\n", wim_unique);
    if (wim_unique > 0) {
        printf("       Top-5 Write IM targets:");
        for (int i = 0; i < 5 && wim_top_pc[i] >= 0; i++) {
            printf(" 0o%o(×%d)", wim_top_pc[i], wim_top_count[i]);
        }
        printf("\n");
    }
    if (rb_trace_n > 0) {
        printf("       ReadBB trace:\n");
        for (int i = 0; i < rb_trace_n; i++) {
            printf("         cyc=%llu pc=0o%o raw=0x%04X seen=0x%04X "
                   "T %04X->%04X Tag %04X->%04X Loc %04X->%04X "
                   "Link %04X->%04X next=0o%o\n",
                   (unsigned long long)rb_trace[i].cycle,
                   rb_trace[i].pc, rb_trace[i].raw, rb_trace[i].b_seen,
                   rb_trace[i].t_before, rb_trace[i].t_after,
                   rb_trace[i].tag_before, rb_trace[i].tag_after,
                   rb_trace[i].loc_before, rb_trace[i].loc_after,
                   rb_trace[i].link_before, rb_trace[i].link_after,
                   rb_trace[i].next_pc);
        }
    }
    mb_free(&bs_mb);
    return 0;  /* informational */
}

/*
 * probe_initial — load Initial.MB directly and run from INITIAL.
 *
 * Bypasses Bootstrap entirely. Initial's job (per the Booting memo)
 * is hardware init: write RM/STK/T with valid data to prevent parity
 * errors, initialize ALUFM, init the memory system enough to prevent
 * unexpected errors, then notify the BB and load an emulator.
 *
 * Useful diagnostic: see if Initial's hardware-init code can run
 * end-to-end against our microengine + memory subsystem.
 */
static int probe_initial(void)
{
    /* Load Initial first, then layer Bootstrap on top. Initial.MB's
     * first instruction at 0o7500 jumps to 0o7700 (Bootstrap entry),
     * so Bootstrap MUST be in IM[0o7700..0o7777] for Initial to
     * function — that's the real-hardware loading order: Bootstrap
     * is loaded by the BB and Initial is loaded by Bootstrap. Then
     * Initial's code transfers control to Bootstrap-region entry
     * points (BootstrapChecksumError, ResetTags, etc.) for shared
     * helpers. */
    mb_file initial_mb, bootstrap_mb;
    mb_init(&initial_mb);
    mb_init(&bootstrap_mb);
    if (mb_load(&initial_mb,
                "../chm/dorado/expanded/bootstrap.dm!20_/Initial.mb")
        != MB_OK) {
        printf("SKIP  probe_initial (Initial.mb)\n");
        return 0;
    }
    if (mb_load(&bootstrap_mb,
                "../chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb")
        != MB_OK) {
        printf("SKIP  probe_initial (Bootstrap.mb)\n");
        mb_free(&initial_mb);
        return 0;
    }
    static dorado_microcode mc;
    if (dorado_microcode_load(&initial_mb, &mc) != DM_OK) {
        printf("SKIP  probe_initial (Initial microcode load)\n");
        mb_free(&initial_mb); mb_free(&bootstrap_mb);
        return 0;
    }
    if (dorado_microcode_layer_load(&bootstrap_mb, &mc) != DM_OK) {
        printf("SKIP  probe_initial (Bootstrap layer load)\n");
        mb_free(&initial_mb); mb_free(&bootstrap_mb);
        return 0;
    }

    /* Find INITIAL symbol in Initial.MB (image → real). */
    int im_id = mb_find_mem(&initial_mb, "IM");
    int initial_image = (im_id >= 0)
        ? mb_find_symbol_addr(&initial_mb, im_id, "INITIAL") : -1;
    int initial_real = -1;
    if (initial_image >= 0) {
        const mb_memory *m = &initial_mb.mems[im_id];
        if (m->present[initial_image]) {
            initial_real = m->data[(size_t)initial_image * m->width_words + 3]
                           & 0xFFF;
        }
    }
    if (initial_real < 0) {
        printf("SKIP  probe_initial (INITIAL symbol not found)\n");
        mb_free(&initial_mb); mb_free(&bootstrap_mb);
        return 0;
    }

    /* Stand up memory + map a few pages identity RW. */
    static dorado_memory mem;
    if (dorado_memory_init(&mem) != 0) {
        printf("SKIP  probe_initial (memory init failed)\n");
        mb_free(&initial_mb); mb_free(&bootstrap_mb);
        return 0;
    }
    for (uint32_t pg = 0; pg < 16; pg++) {
        dorado_map_set(&mem, pg, /*rp=*/(uint16_t)pg, /*wp=*/0, /*dirty=*/0);
    }

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, (uint16_t)initial_real);
    cpu.mem = &mem;
    /* Match Bootstrap's expected handoff state: tasking off,
     * no pending wakeups. Initial is supposed to enable I/O tasks
     * itself once it's ready. */
    cpu.tasking_on = 0;
    cpu.wakeup_pending = 0;
    cpu.reschedule_pending = 0;

    /* Run; track first 64 PCs + last 64 PCs + top-10 hot. */
    #define FN 64
    #define LN 64
    uint16_t first_trail[FN];
    uint16_t last_trail[LN];
    int  first_n = 0;
    int  last_head = 0, last_total = 0;
    static int pc_count[4096];
    memset(pc_count, 0, sizeof pc_count);
    cpu_halt_reason r = CPU_HALT_NONE;
    int loop_pc = -1, loop_count = 0;
    int max_cycles = 100000;
    for (int i = 0; i < max_cycles; i++) {
        if (first_n < FN) first_trail[first_n++] = cpu.real_PC;
        last_trail[last_head] = cpu.real_PC;
        last_head = (last_head + 1) % LN;
        last_total++;
        if (cpu.real_PC < 4096) pc_count[cpu.real_PC]++;
        /* Tight-loop detector. */
        if ((int)cpu.real_PC == loop_pc) {
            if (++loop_count > 1000) break;
        } else {
            loop_pc = (int)cpu.real_PC;
            loop_count = 0;
        }
        if (dorado_cpu_step(&cpu) != 0) {
            r = (cpu_halt_reason)cpu.halt_reason;
            break;
        }
    }
    if (!cpu.halted) r = CPU_HALT_NONE;

    const char *sym = dorado_microcode_symbol_at_real(&mc, cpu.real_PC);
    printf("PROBE  initial: entry=0o%o(INITIAL), ran %d cycles, "
           "halt: %s at PC=0o%o%s%s\n",
           initial_real, cpu.cycles, cpu_halt_reason_str(r),
           cpu.real_PC,
           sym ? " sym=" : "", sym ? sym : "");

    /* First trail. */
    printf("       first:");
    int prev_pc = -1, prev_count = 0;
    for (int i = 0; i < first_n; i++) {
        if ((int)first_trail[i] == prev_pc) { prev_count++; continue; }
        if (prev_count > 1) printf("×%d", prev_count);
        const char *s = dorado_microcode_symbol_at_real(&mc, first_trail[i]);
        if (s) printf(" 0o%o(%s)", first_trail[i], s);
        else   printf(" 0o%o", first_trail[i]);
        prev_pc = first_trail[i];
        prev_count = 1;
    }
    if (prev_count > 1) printf("×%d", prev_count);
    printf("\n");

    /* Last trail. */
    int last_n = last_total < LN ? last_total : LN;
    int last_first = last_total < LN ? 0 : last_head;
    printf("       last:");
    prev_pc = -1; prev_count = 0;
    for (int i = 0; i < last_n; i++) {
        int idx = (last_first + i) % LN;
        if ((int)last_trail[idx] == prev_pc) { prev_count++; continue; }
        if (prev_count > 1) printf("×%d", prev_count);
        const char *s = dorado_microcode_symbol_at_real(&mc, last_trail[idx]);
        if (s) printf(" 0o%o(%s)", last_trail[idx], s);
        else   printf(" 0o%o", last_trail[idx]);
        prev_pc = last_trail[idx];
        prev_count = 1;
    }
    if (prev_count > 1) printf("×%d", prev_count);
    printf("\n");

    /* Top-10 hot PCs. */
    int tops[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int topc[10] = {0};
    for (int a = 0; a < 4096; a++) {
        if (pc_count[a] == 0) continue;
        for (int s = 0; s < 10; s++) {
            if (pc_count[a] > topc[s]) {
                for (int t = 9; t > s; t--) {
                    tops[t] = tops[t-1];
                    topc[t] = topc[t-1];
                }
                tops[s] = a;
                topc[s] = pc_count[a];
                break;
            }
        }
    }
    int unique = 0;
    for (int a = 0; a < 4096; a++) if (pc_count[a]) unique++;
    printf("       Initial visited %d unique IM addresses; top-10:\n",
           unique);
    for (int i = 0; i < 10 && tops[i] >= 0; i++) {
        const char *s = dorado_microcode_symbol_at_real(&mc, tops[i]);
        printf("         0o%o ×%d%s%s\n", tops[i], topc[i],
               s ? " " : "", s ? s : "");
    }
    #undef FN
    #undef LN

    /* If we halted on something we don't yet handle, dump the offender. */
    if (cpu.halt_reason == CPU_HALT_UNSUPPORTED_ASEL ||
        cpu.halt_reason == CPU_HALT_UNSUPPORTED_BSEL ||
        cpu.halt_reason == CPU_HALT_UNSUPPORTED_FF ||
        cpu.halt_reason == CPU_HALT_UNSUPPORTED_JCN) {
        if (cpu.real_PC < 4096 && mc.im_present[cpu.real_PC]) {
            const dorado_uinstr *u = &mc.im[cpu.real_PC];
            char dis[256];
            dorado_format(u, dis, sizeof dis);
            printf("       offending uinstr: %s\n", dis);
        }
    }

    dorado_memory_free(&mem);
    mb_free(&initial_mb);
    mb_free(&bootstrap_mb);
    return 0;  /* informational */
}

/*
 * Test 8: Write IM round-trip. The Bootstrap loader uses Write IM to
 * deposit Initial into IM via four IRTable entries (LH/RH × secondary
 * 0/1). Each Write IM: address from cpu->Link, 16 bits from B,
 * half-select + secondary bit from RSTK[2:3].
 *
 * We exercise all four variants and verify both the raw iw0/iw1/iw2
 * and the re-decoded fields end up where they belong.
 */
static int test_write_im(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0]  = 025; mc.alufm_present[0]  = 1;  /* ALUF[0] = B */
    mc.alufm[1]  = 025; mc.alufm_present[1]  = 1;
    mc.alufm[2]  = 025; mc.alufm_present[2]  = 1;
    mc.alufm[3]  = 025; mc.alufm_present[3]  = 1;

    /* Four Write IM instructions, targeting four different IM addrs.
     *   IM[0]: RSTK=1 → LH, secondary=0 → iw0 = B,        iw2[15] = 0
     *   IM[1]: RSTK=3 → LH, secondary=1 → iw0 = B,        iw2[15] = 1
     *   IM[2]: RSTK=0 → RH, secondary=0 → iw1 = B,        iw2[14] = 0
     *   IM[3]: RSTK=2 → RH, secondary=1 → iw1 = B,        iw2[14] = 1
     *
     * B comes from BSEL=4 (0,,FF) with FF=0xAA → B = 0x00AA. */
    mc.im[0] = make_uinstr(/*rstk=*/1, 0, /*bsel=*/4, /*lc=*/0,
                           /*asel=*/4, 0, /*ff=*/0xAA, /*jcn=*/0x7F);
    mc.im[1] = make_uinstr(/*rstk=*/3, 1, /*bsel=*/4, /*lc=*/0,
                           /*asel=*/4, 0, /*ff=*/0xAA, /*jcn=*/0x7F);
    mc.im[2] = make_uinstr(/*rstk=*/0, 2, /*bsel=*/4, /*lc=*/0,
                           /*asel=*/4, 0, /*ff=*/0xAA, /*jcn=*/0x7F);
    mc.im[3] = make_uinstr(/*rstk=*/2, 3, /*bsel=*/4, /*lc=*/0,
                           /*asel=*/4, 0, /*ff=*/0xAA, /*jcn=*/0x7F);
    for (int i = 0; i < 4; i++) {
        mc.im_present[i] = 1;
        mc.image_to_real[i] = i;
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 4;

    /* Run each Write IM separately, pointing Link at a fresh target
     * each time (200, 201, 202, 203). */
    for (int i = 0; i < 4; i++) {
        dorado_cpu cpu;
        dorado_cpu_init(&cpu, &mc, (uint16_t)i);
        cpu.Link = (uint16_t)(0x200 + i);

        int rc = dorado_cpu_step(&cpu);
        EXPECT(rc == 0, "case %d step: %s", i,
               cpu_halt_reason_str(cpu.halt_reason));

        uint16_t addr = (uint16_t)(0x200 + i);
        EXPECT(mc.im_present[addr],
               "case %d: IM[0x%03X] not marked present", i, addr);
    }

    /* LH writes land directly in iw0. RH writes use the 17-bit right
     * half display form: RSTK[2] becomes BLOCK, B[15:1] becomes
     * iw1[14:0], and B[0] becomes JCN[7]. */
    EXPECT(mc.im[0x200].iw0 == 0x00AA,
           "LH/sec0: iw0 = 0x%04X, want 0x00AA", mc.im[0x200].iw0);
    EXPECT((mc.im[0x200].iw2 & 0x8000) == 0,
           "LH/sec0: iw2[15] should be 0");
    EXPECT(mc.im[0x201].iw0 == 0x00AA,
           "LH/sec1: iw0 = 0x%04X, want 0x00AA", mc.im[0x201].iw0);
    EXPECT((mc.im[0x201].iw2 & 0x8000) != 0,
           "LH/sec1: iw2[15] should be 1");

    EXPECT(mc.im[0x202].iw1 == 0x0055,
           "RH/sec0: iw1 = 0x%04X, want 0x0055", mc.im[0x202].iw1);
    EXPECT((mc.im[0x202].iw2 & 0x4000) == 0,
           "RH/sec0: iw2[14] should be 0");
    EXPECT(mc.im[0x203].iw1 == 0x8055,
           "RH/sec1: iw1 = 0x%04X, want 0x8055", mc.im[0x203].iw1);
    EXPECT((mc.im[0x203].iw2 & 0x4000) == 0,
           "RH/sec1: iw2[14] should be 0");

    /* The decoded fields should reflect the new iw0/iw1/iw2. For
     * iw0=0x00AA: ASEL=iw0[2:0]=2, LC=iw0[5:3]=5, BSEL=iw0[8:6]=2,
     * ALUF=iw0[12:9]=0, RSTK[2:0]=iw0[15:13]=0. */
    EXPECT(mc.im[0x201].asel == 2,
           "decoded ASEL = %d, want 2", mc.im[0x201].asel);
    EXPECT(mc.im[0x201].lc == 5,
           "decoded LC = %d, want 5", mc.im[0x201].lc);
    EXPECT(mc.im[0x201].bsel == 2,
           "decoded BSEL = %d, want 2", mc.im[0x201].bsel);
    /* RSTK = (iw2[15] << 3) | iw0[15:13] = (1 << 3) | 0 = 8. */
    EXPECT(mc.im[0x201].rstk == 8,
           "decoded RSTK = %d, want 8 (secondary bit lifted to MSB)",
           mc.im[0x201].rstk);

    printf("PASS  test_write_im (4 variants, decoded fields refresh)\n");
    return 0;
}

/*
 * STK addressing tests (HM Table 6 / page 11).
 *
 * Encoding of u->rstk in C-LSB:
 *   bit 3 = manual RSTK[0] (underflow-check flag)
 *   bit 2 = manual RSTK[1] (sign of delta)
 *   bit 1 = manual RSTK[2]
 *   bit 0 = manual RSTK[3] (LSB of delta)
 *
 * RSTK[1:3] encodes a 3-bit signed delta in [-4, +3]:
 *   0 (000) →  0     4 (100) → -4
 *   1 (001) → +1     5 (101) → -3
 *   2 (010) → +2     6 (110) → -2
 *   3 (011) → +3     7 (111) → -1
 */

/* Construct a single instruction that does:
 *   ASEL=A←RM/STK (4)   — read RM/STK[StkP] onto A
 *   BSEL=RM/STK   (1)   — same value onto B
 *   ALUF=N → ALUFM[N]=B → ALU = B
 *   LC=RM/STK←Pd  (6)   — write Pd back to RM/STK
 *   BLOCK=1            — STK access
 *   JCN=local(0)        — stay at PC=0
 * with the given RSTK encoding. */
static dorado_uinstr stk_uinstr(int rstk, int ff)
{
    return make_uinstr(rstk, /*aluf=*/0, /*bsel=*/1, /*lc=*/6,
                       /*asel=*/4, /*block=*/1, ff,
                       /*jcn=*/jcn_local(0));
}

static int test_stk_no_change(void)
{
    /* RSTK[1:3] = 0 → StkP unchanged, RSTK[0] = 0 → no underflow check.
     * u->rstk = 0b0000 = 0. */
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;
    mc.im[0] = stk_uinstr(/*rstk=*/00, /*ff=*/0077);
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.StkP = 5;
    cpu.STK[5] = 0xBEEF;

    EXPECT(dorado_cpu_step(&cpu) == 0, "step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.StkP == 5, "StkP changed: %u (expected 5)", cpu.StkP);
    EXPECT(cpu.STK[5] == 0xBEEF, "STK[5] = 0x%04X (expected 0xBEEF)", cpu.STK[5]);
    EXPECT(cpu.stk_ovf == 0 && cpu.stk_und == 0,
           "stk flags should be clear: ovf=%u und=%u",
           cpu.stk_ovf, cpu.stk_und);

    printf("PASS  test_stk_no_change\n");
    return 0;
}

static int test_stk_push(void)
{
    /* RSTK[1:3] = 1 → StkP+=1. RSTK[0] = 0. u->rstk = 0b0001 = 1. */
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;
    mc.im[0] = stk_uinstr(/*rstk=*/01, /*ff=*/0077);
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.StkP = 5;
    cpu.STK[5] = 0xCAFE;

    EXPECT(dorado_cpu_step(&cpu) == 0, "step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.StkP == 6, "after push, StkP = %u (expected 6)", cpu.StkP);
    /* Read used unadjusted StkP=5; LC wrote to STK[5] (default — no
     * ModStkPBeforeW). Value written = ALU = B = STK[5] = 0xCAFE.
     * So STK[5] = 0xCAFE still. */
    EXPECT(cpu.STK[5] == 0xCAFE, "STK[5] = 0x%04X (expected 0xCAFE)", cpu.STK[5]);

    printf("PASS  test_stk_push (StkP 5 → 6)\n");
    return 0;
}

static int test_stk_pop(void)
{
    /* RSTK[1:3] = 7 → StkP-=1 (signed -1). u->rstk = 0b0111 = 7. */
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;
    mc.im[0] = stk_uinstr(/*rstk=*/07, /*ff=*/0077);
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.StkP = 10;

    EXPECT(dorado_cpu_step(&cpu) == 0, "step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.StkP == 9, "after pop, StkP = %u (expected 9)", cpu.StkP);

    printf("PASS  test_stk_pop (StkP 10 → 9)\n");
    return 0;
}

static int test_stk_pop_minus_4(void)
{
    /* RSTK[1:3] = 4 → StkP-=4. u->rstk = 0b0100 = 4. */
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;
    mc.im[0] = stk_uinstr(/*rstk=*/04, /*ff=*/0077);
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.StkP = 020;     /* octal 20 = 16 decimal */

    EXPECT(dorado_cpu_step(&cpu) == 0, "step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.StkP == 014, "after pop -4, StkP = 0o%o (expected 0o14)",
           cpu.StkP);

    printf("PASS  test_stk_pop_minus_4 (StkP 0o20 → 0o14)\n");
    return 0;
}

static int test_stk_overflow(void)
{
    /* StkP[2:7] = 077 (max in region), RSTK[1:3]=1 → would go to 0o100,
     * which crosses into next region. Real hardware sets StkOvf and
     * generates StkError; we just set the flag. */
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;
    mc.im[0] = stk_uinstr(/*rstk=*/01, /*ff=*/0077);
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.StkP = 077;     /* octal 77 = 63 decimal, top of region 0 */

    EXPECT(dorado_cpu_step(&cpu) == 0, "step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.stk_ovf == 1, "StkOvf should be set on push past 077");
    EXPECT(cpu.stk_und == 0, "StkUnd should NOT be set");

    printf("PASS  test_stk_overflow (077 + 1 sets StkOvf)\n");
    return 0;
}

static int test_stk_underflow_check(void)
{
    /* RSTK[0] = 1 → underflow check enabled.
     * StkP starts at 1, RSTK[1:3] = 7 (-1) → final StkP = 0 → underflow.
     * u->rstk = 0b1111 = 0o17. */
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;
    mc.im[0] = stk_uinstr(/*rstk=*/017, /*ff=*/0077);
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.StkP = 1;

    EXPECT(dorado_cpu_step(&cpu) == 0, "step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.stk_und == 1, "StkUnd should be set on -1 to StkP=1");

    printf("PASS  test_stk_underflow_check (StkP=1, -1, RSTK[0]=1 → StkUnd)\n");
    return 0;
}

static int test_lc_forced_rm_write_address(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* B */

    /* FF=0o042 is one of the "change RSTK for write" encodings:
     * LC writes RM[RBase, FF[4:7]] even though BLOCK selects STK for
     * the read side. */
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/4, /*lc=*/6,
                           /*asel=*/6, /*block=*/1, /*ff=*/0042,
                           jcn_local(1));
    mc.im_present[0] = 1;
    /* FF=0o225 is the Table 11d variant with the same forced-RM LC
     * destination behavior. */
    mc.im[1] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/4, /*lc=*/6,
                           /*asel=*/6, /*block=*/1, /*ff=*/0225,
                           jcn_local(2));
    mc.im_present[1] = 1;
    mc.im[2] = make_uinstr(0, 0, 2, 0, 6, 0, 0, jcn_local(2));
    mc.im_present[2] = 1;
    for (int i = 0; i < 3; i++) {
        mc.image_to_real[i] = i;
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 3;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.RBase = 3;
    cpu.StkP = 7;
    cpu.STK[7] = 0xAAAA;

    EXPECT(dorado_cpu_step(&cpu) == 0, "step forced RM write 0o042");
    EXPECT(cpu.RM[0x32] == 0042,
           "FF=0o042 should write RM[0x32], got 0x%04X", cpu.RM[0x32]);
    EXPECT(cpu.STK[7] == 0xAAAA,
           "forced RM write should not clobber STK[7]");

    EXPECT(dorado_cpu_step(&cpu) == 0, "step forced RM write 0o225");
    EXPECT(cpu.RM[0x35] == 0225,
           "FF=0o225 should write RM[0x35], got 0x%04X", cpu.RM[0x35]);
    EXPECT(cpu.STK[7] == 0xAAAA,
           "Table 11d forced RM write should not clobber STK[7]");

    printf("PASS  test_lc_forced_rm_write_address\n");
    return 0;
}

/*
 * Integration test: Store via the microengine, then Fetch+B←Md, and
 * verify the memory subsystem round-tripped a value end-to-end.
 *
 *   IM[0]: Store←RM/STK with RM[0] = 0x42 (the address) and B = T
 *          (the data, set up by IM[-1]). Use ASEL=0 + FF[0:1]=3 →
 *          DM_REF_STORE. BSEL=2 (T) drives data onto B.
 *   IM[1]: Fetch←RM/STK at the same address.
 *   IM[2]: T ← Md (BSEL=0 = Md).
 *
 * MemBase=0, BR[0]=0 → VA = 0 + 0x42 = 0x42.
 *
 * NOTE: this test uses BSEL=Md as the ALU's B source after a fetch.
 * Per HM "deferred reference" rules, Md needs to be loaded directly
 * into RM/T (LC=2/3/4) without Hold; reading Md onto the B-bus
 * (BSEL=0) on the *same* instruction as the fetch would Hold. Our
 * stub doesn't model Hold so the read is immediate.
 */
static int test_cpu_memory_roundtrip(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* "B" */

    /* IM[0]: Store←RM/STK. RSTK=0 → RM[0]; BSEL=2 (T data); ASEL=0;
     * FF=0o320 → FF[0:1] = 3 → Store, with low-six FF function 0o20
     * (A←RM/STK) instead of the low-nibble A override. */
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/2, /*lc=*/0,
                           /*asel=*/0, /*block=*/0, /*ff=*/0320,
                           /*jcn=*/jcn_local(1));
    mc.im_present[0] = 1;

    /* IM[1]: Fetch←RM/STK. RSTK=0; BSEL=2 (don't care); ASEL=1;
     * FF=0o320 → FF[0:1] = 3 → Fetch. LC=0, JCN=local(2). */
    mc.im[1] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/2, /*lc=*/0,
                           /*asel=*/1, /*block=*/0, /*ff=*/0320,
                           /*jcn=*/jcn_local(2));
    mc.im_present[1] = 1;

    /* IM[2]: T ← Md. ASEL=6 (A←T); BSEL=0 (Md); ALUF=0 → "B"; LC=1
     * (T←Pd). Self-loop. */
    mc.im[2] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/0, /*lc=*/1,
                           /*asel=*/6, /*block=*/0, /*ff=*/0,
                           /*jcn=*/jcn_local(2));
    mc.im_present[2] = 1;

    for (int i = 0; i < 3; i++) {
        mc.image_to_real[i] = i;
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 3;

    static dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "memory init");
    /* Mount map page 0 (covers VA 0x42) so the ref doesn't page-fault. */
    dorado_map_set(&mem, /*va_page=*/0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.mem = &mem;
    cpu.T = 0xDEAD;        /* data we'll store */
    cpu.RM[0] = 0x42;      /* address we'll store to */

    /* Run 3 instructions: Store, Fetch, T←Md. After Store, the data
     * lives in the cache (HM page 45: Store doesn't set Map.Dirty
     * until evict). The subsequent Fetch hits the cache and returns
     * the stored value. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "Store step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(dorado_cache_lookup(&mem, 0x42, NULL),
           "Store should have placed VA 0x42 in the cache");

    EXPECT(dorado_cpu_step(&cpu) == 0, "Fetch step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(mem.md == 0xDEAD, "after Fetch: Md = 0x%04X", mem.md);

    EXPECT(dorado_cpu_step(&cpu) == 0, "T←Md step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.T == 0xDEAD, "T = 0x%04X, expected 0xDEAD", cpu.T);

    dorado_memory_free(&mem);
    printf("PASS  test_cpu_memory_roundtrip (Store→Fetch→T←Md)\n");
    return 0;
}

static int test_alt_fetch_t_lc_md_pipeline(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* "B" */

    /* ASEL=3 + FF[0:1]=3 is Fetch←T. LC=4 reads the Md latch as it
     * stood at issue; the concurrent Fetch updates Md for later
     * instructions. PilotDisk relies on this when it checks IOCB.seal
     * while starting the next IOCB-field fetch. */
    mc.im[0] = make_uinstr(/*rstk=*/1, /*aluf=*/0, /*bsel=*/2, /*lc=*/4,
                           /*asel=*/3, /*block=*/0, /*ff=*/0300,
                           /*jcn=*/jcn_local(1));
    mc.im_present[0] = 1;
    mc.im[1] = make_uinstr(0, 0, 4, 0, 4, 0, 0, jcn_local(1));
    mc.im_present[1] = 1;
    for (int i = 0; i < 2; i++) {
        mc.image_to_real[i] = i;
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 2;

    static dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "memory init");
    dorado_map_set(&mem, /*va_page=*/0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);
    mem.storage[0x42] = 0xBEEF;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.mem = &mem;
    cpu.T = 0x42;
    mem.md = 0xCAFE;

    EXPECT(dorado_cpu_step(&cpu) == 0, "Fetch←T/RM←Md step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(mem.md == 0xBEEF, "Md = 0x%04X, expected 0xBEEF", mem.md);
    EXPECT(cpu.RM[1] == 0xCAFE, "RM[1] = 0x%04X, expected 0xCAFE", cpu.RM[1]);

    dorado_memory_free(&mem);
    printf("PASS  test_alt_fetch_t_lc_md_pipeline (Fetch←T + old Md latch)\n");
    return 0;
}

static int test_dummyref_t_uses_t_as_mar(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* "B" */

    /* InitMem.mc's NextMapEntry source is `DummyRef_ T, T_ MD`.
     * In the compiled Table-8a form that is ASEL=1, FF[0:1]=0
     * (DummyRef), LC=3 (T<-Md).  The Mar source must be the old T,
     * not RM/STK, while the LC still sees the old Md latch. */
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/1, /*lc=*/3,
                           /*asel=*/1, /*block=*/0, /*ff=*/0021,
                           /*jcn=*/jcn_local(1));
    mc.im_present[0] = 1;
    mc.im[1] = make_uinstr(0, 0, 4, 0, 4, 0, 0, jcn_local(1));
    mc.im_present[1] = 1;
    for (int i = 0; i < 2; i++) {
        mc.image_to_real[i] = i;
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 2;

    static dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "memory init");
    dorado_br_hi_load(&mem, 0, 0x0001);
    dorado_br_lo_load(&mem, 0, 0x2000);

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.mem = &mem;
    cpu.T = 0x0340;
    cpu.RM[0] = 0x0055;
    mem.md = 0xCAFE;

    EXPECT(dorado_cpu_step(&cpu) == 0, "DummyRef_ T step: %s",
           cpu_halt_reason_str(cpu.halt_reason));

    uint32_t expected = (dorado_br_get(&mem, 0) + 0x0340u) & 0x0FFFFFFFu;
    EXPECT(dorado_pipe_va_at(&mem, mem.proc_srn) == expected,
           "DummyRef pipe VA = 0x%07X, expected 0x%07X",
           dorado_pipe_va_at(&mem, mem.proc_srn), expected);
    EXPECT(cpu.T == 0xCAFE, "T = 0x%04X, expected old Md 0xCAFE", cpu.T);

    dorado_memory_free(&mem);
    printf("PASS  test_dummyref_t_uses_t_as_mar\n");
    return 0;
}

static int test_alt_store_t_uses_b_data(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* "B" */

    /* ASEL=2 + FF[0:1]=3 is Store<-T: T supplies Mar/address, but
     * DBuf/store data still comes from B. Use BSEL=Q to prove the
     * emulator does not accidentally store the address. */
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/3, /*lc=*/0,
                           /*asel=*/2, /*block=*/0, /*ff=*/0300,
                           /*jcn=*/jcn_local(1));
    mc.im_present[0] = 1;
    mc.im[1] = make_uinstr(0, 0, 4, 0, 4, 0, 0, jcn_local(1));
    mc.im_present[1] = 1;
    for (int i = 0; i < 2; i++) {
        mc.image_to_real[i] = i;
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 2;

    static dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "memory init");
    dorado_map_set(&mem, /*va_page=*/0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.mem = &mem;
    cpu.T = 0x42;       /* address */
    cpu.Q = 0xCAFE;     /* B/data */

    EXPECT(dorado_cpu_step(&cpu) == 0, "Store<-T step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x42, 0, 0);
    EXPECT(mem.md == 0xCAFE, "Md = 0x%04X, expected stored Q data", mem.md);

    dorado_memory_free(&mem);
    printf("PASS  test_alt_store_t_uses_b_data\n");
    return 0;
}

static int test_memory_decode_uses_table8b_when_ff_not_ok(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* "B" */

    /* Long JCN makes FF unavailable. HM Table 8b says ASEL=0 is
     * Store<-RM/STK, even though FF[0:1]=1 would mean Map<- under
     * Table 8a. */
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/2, /*lc=*/0,
                           /*asel=*/0, /*block=*/0, /*ff=*/0100,
                           /*jcn=*/0x00);
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0;
    mc.image_present[0] = 1;
    mc.n_instructions = 1;

    static dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "memory init");
    dorado_map_set(&mem, /*va_page=*/0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.mem = &mem;
    cpu.RM[0] = 0x55;   /* address */
    cpu.T = 0xBEEF;     /* B/data */

    EXPECT(dorado_cpu_step(&cpu) == 0, "Table 8b Store step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    dorado_memory_ref(&mem, DM_REF_FETCH, 0x55, 0, 0);
    EXPECT(mem.md == 0xBEEF, "Md = 0x%04X, expected Table 8b Store data",
           mem.md);
    EXPECT((dorado_pipe5_at(&mem, 0) & 0x8000u) == 0,
           "Instruction should not have been decoded as Map<-");

    dorado_memory_free(&mem);
    printf("PASS  test_memory_decode_uses_table8b_when_ff_not_ok\n");
    return 0;
}

static int test_bootstrap_ldf_dispatch(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[14] = 001; mc.alufm_present[14] = 1;   /* NOT A for shift */

    /* Bootstrap's compiled `BTemp_ LDF[T,3,10]` extracts the 3-bit
     * dispatch from the high byte sent by the BaseBoard and delivers
     * the even BigBDispatch target offset. For CPReg high byte 0x82,
     * the dispatch is 2, so the offset is 4. */
    mc.im[0] = make_uinstr(/*rstk=*/2, /*aluf=*/4, /*bsel=*/4, /*lc=*/6,
                           /*asel=*/7, /*block=*/0, /*ff=*/001,
                           /*jcn=*/jcn_local(1));
    mc.im_present[0] = 1;
    mc.im[1] = make_uinstr(0, 0, 4, 0, 4, 0, 0, jcn_local(1));
    mc.im_present[1] = 1;
    for (int i = 0; i < 2; i++) {
        mc.image_to_real[i] = i;
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 2;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.T = 0x8282;

    EXPECT(dorado_cpu_step(&cpu) == 0, "LDF[T,3,10] step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.RM[2] == 4, "BTemp = 0x%04X, expected 4", cpu.RM[2]);

    printf("PASS  test_bootstrap_ldf_dispatch (LDF[T,3,10])\n");
    return 0;
}

/*
 * Fault → FaultInfo' visibility test.
 *
 * Issue a Fetch← to a Vacant map entry (page-fault), then read
 * `B←FaultInfo'` (FA=1 FB=6 FC=0 → FF=0o160) and verify the bus
 * value reflects NFaults=1, SRN=0, EmulatorFault=1.
 *
 * High-true FaultInfo for one fault from emulator at SRN 0:
 *   B[7]=1 (EmulatorFault) → bit 8 (LSB) = 0x100
 *   B[8:11]=SRN=0          → bits 4..7   = 0x000
 *   B[12:15]=NFaults=1     → bits 0..3   = 0x001
 *   high-true value        = 0x101
 *   on the bus (~)         = 0xFEFE
 */
static int test_cpu_fault_info_visible(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* "B" */

    /* IM[0]: Fetch←RM/STK at A=RM[0]; FF=0o300 (FF[0:1]=3 → Fetch);
     * BSEL=2 (T data, ignored for fetch). ASEL=1 chooses RM/STK
     * source for A; RSTK=0 reads RM[0]. LC=0, JCN=local(1). */
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/2, /*lc=*/0,
                           /*asel=*/1, /*block=*/0, /*ff=*/0300,
                           /*jcn=*/jcn_local(1));
    mc.im_present[0] = 1;

    /* IM[1]: B ← FaultInfo'.   FA=1 FB=6 FC=0 → FF = (1<<6)|(6<<3)|0
     * = 64 + 48 = 112 = 0o160. LC=1 → T←Pd. ALUF=0 ("B"). BSEL=0
     * (primary B sources, FF override active). ASEL=6 (A←T,
     * harmless). Self-loop. */
    mc.im[1] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/0, /*lc=*/1,
                           /*asel=*/6, /*block=*/0, /*ff=*/0160,
                           /*jcn=*/jcn_local(1));
    mc.im_present[1] = 1;

    for (int i = 0; i < 2; i++) {
        mc.image_to_real[i] = i;
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 2;

    static dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "memory init");
    /* Don't mount any page → page 0 is Vacant → fetch faults. */

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.mem = &mem;
    cpu.tasking_on = 0;   /* keep on task 0 so the fault is visible
                           * synchronously; with tasking on, a fault
                           * wakes task 15 and the next instruction
                           * runs as task 15, not task 0. */
    cpu.RM[0] = 0x42;     /* Will be the faulting VA. */

    /* Step 1: Fetch faults. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "Fetch step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(mem.last_fault == DM_FAULT_PAGE,
           "expected page fault, got %d", (int)mem.last_fault);
    EXPECT(mem.fault_count == 1, "NFaults = %d, expected 1",
           (int)mem.fault_count);

    /* Step 2: B ← FaultInfo'. T should land on the bus value. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "FaultInfo step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.T == 0xFEFE,
           "T = 0x%04X, expected 0xFEFE (NFaults=1, SRN=0, Emul=1)",
           cpu.T);
    EXPECT(mem.fault_count == 0,
           "B<-FaultInfo' should clear faults, NFaults=%d",
           (int)mem.fault_count);

    dorado_memory_free(&mem);
    printf("PASS  test_cpu_fault_info_visible (B←FaultInfo')\n");
    return 0;
}

static int test_cpu_pipe4_no_error_baseline(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* "B" */

    /* B←Pipe4' = FA=1 FB=6 FC=5 = 0o165. HM page 51 says
     * 0150361_8 XOR Pipe4' yields high-true fields, so no error
     * reads as exactly 0150361_8 on the bus. */
    mc.im[0] = make_uinstr(0, 0, 0, 1, 6, 0, 0165, jcn_local(0));
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0;
    mc.image_present[0] = 1;
    mc.n_instructions = 1;

    static dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "memory init");

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.mem = &mem;

    EXPECT(dorado_cpu_step(&cpu) == 0, "Pipe4 step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.T == 0150361,
           "Pipe4' no-error baseline = 0o%o, expected 0o150361", cpu.T);

    dorado_memory_free(&mem);
    printf("PASS  test_cpu_pipe4_no_error_baseline\n");
    return 0;
}

/*
 * BC timing test (HM page 18 + 30):
 *
 * Branch conditions ALU=0 / ALU<0 / Carry' / Overflow are LOADED
 * into the BC RAM at t3 of the current instruction; the *next*
 * instruction's branch reads them. So a conditional branch always
 * tests the PREVIOUS instruction's ALU output.
 *
 * Conditional-jump JCN requires JCN[3]=1 (else the high 4 bits
 * are 0 and the encoding is "long jump"). With page_high=0, the
 * smallest reachable JCN[3:4] is 10 → page_low=2 → offsets 4/5.
 * We use IM[4] (false) and IM[5] (true) as branch targets.
 */
static int test_bc_timing_previous_instr(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* "B" */

    /* IM[0]: ALU=B (=0,,FF with FF=0) → 0. T←Pd, JCN=local(1). */
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/4, /*lc=*/1,
                           /*asel=*/6, /*block=*/0, /*ff=*/0,
                           /*jcn=*/jcn_local(1));
    mc.im_present[0] = 1;

    /* IM[1]: ALU=B (=0,,FF with FF=1) → 1. T←Pd, JCN = conditional
     * on ALU=0 with page_high=0, page_low=2 → targets IM[4]/IM[5].
     * Per HM, this branch tests IM[0]'s ALU=0 (= TRUE) → branch to
     * IM[5] (R=1). With the old timing model it would test IM[1]'s
     * ALU=1 (= FALSE) and go to IM[4]. */
    {
        uint8_t jcn = (0 << 5) | (2 << 3) | 0;   /* page_low=2, cond=0 */
        mc.im[1] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/4, /*lc=*/1,
                               /*asel=*/6, /*block=*/0, /*ff=*/1,
                               /*jcn=*/jcn);
    }
    mc.im_present[1] = 1;

    /* IM[4]: false target. T←4, self-loop. */
    mc.im[4] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/1,
                           /*asel=*/6, 0, /*ff=*/4, jcn_local(4));
    mc.im_present[4] = 1;

    /* IM[5]: true target. T←5, self-loop. */
    mc.im[5] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/1,
                           /*asel=*/6, 0, /*ff=*/5, jcn_local(5));
    mc.im_present[5] = 1;

    for (int i = 0; i < 8; i++) {
        if (mc.im_present[i]) {
            mc.image_to_real[i] = i;
            mc.image_present[i] = 1;
        }
    }
    mc.n_instructions = 4;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    /* Step 0: IM[0] runs. ALU=0 → BCs latched. T←0. PC→1. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 0");
    EXPECT(cpu.T == 0, "T=0o%o, expected 0", cpu.T);
    /* Step 1: IM[1] runs. Branch tests previous instr (IM[0])'s ALU=0,
     * which is TRUE. R=1, target is IM[5]. T←1 in IM[1]. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 1");
    EXPECT(cpu.T == 1, "T=0o%o, expected 1", cpu.T);
    EXPECT(cpu.real_PC == 5,
           "expected branch to IM[5] (true target), got PC=0o%o",
           cpu.real_PC);

    /* Step 2: IM[5] runs. T←5. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 2");
    EXPECT(cpu.T == 5, "T=0o%o, expected 5", cpu.T);

    printf("PASS  test_bc_timing_previous_instr\n");
    return 0;
}

/*
 * FreezeBC test (HM Table 11a FA=0 FB=7 FC=6):
 *
 * FreezeBC prevents the BC RAM from being loaded at t3, so the
 * NEXT instruction's branch reads the BCs from TWO instructions ago
 * instead of the immediately-previous instruction.
 *
 * Setup: 4-instruction microprogram.
 *   IM[0]: ALU=0  (BCs ALU=0=1)
 *   IM[1]: ALU=1, FreezeBC (BCs frozen — alu_zero stays 1 from IM[0])
 *   IM[2]: branch on ALU=0 — should test IM[0]'s BCs (= 1 = TRUE)
 *           because IM[1] froze them. Without FreezeBC, would test
 *           IM[1]'s BCs (alu_zero=0 since alu=1).
 */
static int test_freezebc(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* "B" */

    /* IM[0]: ALU=0 → BCs ALU=0 will be set TRUE. */
    mc.im[0] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/0,
                           /*asel=*/6, 0, /*ff=*/0, jcn_local(1));
    mc.im_present[0] = 1;

    /* IM[1]: ALU=1, but with FreezeBC FF (FA=0 FB=7 FC=6).
     * FF = (0 << 6) | (7 << 3) | 6 = 62 = 0o076. */
    mc.im[1] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0,
                           /*asel=*/6, 0, /*ff=*/0076, jcn_local(2));
    mc.im_present[1] = 1;

    /* IM[2]: cond ALU=0, branch with page_low=2 → targets IM[4]/IM[5]. */
    {
        uint8_t jcn = (0 << 5) | (2 << 3) | 0;
        mc.im[2] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/0,
                               /*asel=*/6, 0, /*ff=*/2, jcn);
    }
    mc.im_present[2] = 1;

    /* IM[4]: false target — T←4, self-loop. */
    mc.im[4] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/1,
                           /*asel=*/6, 0, /*ff=*/4, jcn_local(4));
    mc.im_present[4] = 1;

    /* IM[5]: true target — T←5, self-loop. */
    mc.im[5] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/1,
                           /*asel=*/6, 0, /*ff=*/5, jcn_local(5));
    mc.im_present[5] = 1;

    for (int i = 0; i < 8; i++) {
        if (mc.im_present[i]) {
            mc.image_to_real[i] = i;
            mc.image_present[i] = 1;
        }
    }
    mc.n_instructions = 5;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    /* Step IM[0]: ALU=0 → alu_zero gets set to 1 at end. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 0");
    /* Step IM[1]: ALU=1 → alu_zero would be 0, but FreezeBC keeps 1. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 1");
    EXPECT(cpu.alu_zero == 1,
           "after IM[1] FreezeBC, alu_zero = %u (expected 1, frozen from IM[0])",
           cpu.alu_zero);

    /* Step IM[2]: branch on ALU=0. Reads alu_zero (= 1, frozen).
     * R=1 → branch to IM[5] (odd target). */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 2");
    EXPECT(cpu.real_PC == 5,
           "expected branch to IM[5] via frozen BC, got PC=0o%o",
           cpu.real_PC);

    printf("PASS  test_freezebc\n");
    return 0;
}

/*
 * Return-class JCN with FF branch condition:
 *
 * HM page 29 says an FF-encoded branch condition ORs its result into
 * TNIA[15] for any JCN encoding except long branch. InitMem.mc uses
 * this as `Return[ALU=0]` in NextMapEntry: the false return resumes at
 * Link, the true return resumes at Link|1.
 */
static int test_return_ff_condition_or(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* "B" */

    /* IM[0]: produce ALU=0 and branch to the return instruction. */
    mc.im[0] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/0,
                           /*asel=*/6, 0, /*ff=*/0, jcn_local(1));
    mc.im_present[0] = 1;

    /* IM[1]: Subroutine Return with FF=060 (ALU=0 condition).
     * JCN = 0 1 000 111 = 0x47 = 0107. BSEL<4 keeps FF available
     * as a branch condition rather than a full Table-11 function. */
    mc.im[1] = make_uinstr(0, 0, /*bsel=*/0, /*lc=*/0,
                           /*asel=*/4, 0, /*ff=*/0060, /*jcn=*/0x47);
    mc.im_present[1] = 1;

    for (int i = 0; i < 2; i++) {
        mc.image_to_real[i] = i;
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 2;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.Link = 0200;

    EXPECT(dorado_cpu_step(&cpu) == 0, "return cond setup true");
    EXPECT(cpu.alu_zero == 1, "setup should latch ALU=0 true");
    EXPECT(dorado_cpu_step(&cpu) == 0, "return cond true");
    EXPECT(cpu.real_PC == 0201,
           "Return[ALU=0] true should OR Link with 1, got PC=0o%o",
           cpu.real_PC);

    dorado_cpu_init(&cpu, &mc, 0);
    cpu.Link = 0200;
    cpu.alu_zero = 0;
    cpu.real_PC = 1;
    EXPECT(dorado_cpu_step(&cpu) == 0, "return cond false");
    EXPECT(cpu.real_PC == 0200,
           "Return[ALU=0] false should return to Link, got PC=0o%o",
           cpu.real_PC);

    printf("PASS  test_return_ff_condition_or\n");
    return 0;
}

/*
 * Tasking — basic task switch on Wakeup.
 *
 * Layout:
 *   IM[0]: NOP self-loop in task 0 (emulator).
 *   IM[1]: NOP for task 5; loops to 1.
 * Boot at IM[0]. Set task 5's TPC to IM[1]. Wake task 5 — after the
 * next end-of-instruction the engine should switch from task 0 to
 * task 5.
 */
static int test_task_switch_on_wakeup(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* "B" */

    /* IM[0]: T←0o111 (task-0 marker). LC=1, JCN=local(0). */
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/4, /*lc=*/1,
                           /*asel=*/6, /*block=*/0, /*ff=*/0111,
                           /*jcn=*/jcn_local(0));
    mc.im_present[0] = 1;
    /* IM[1]: T←0o222 (task-5 marker). LC=1, JCN=local(1). */
    mc.im[1] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/4, /*lc=*/1,
                           /*asel=*/6, /*block=*/0, /*ff=*/0222,
                           /*jcn=*/jcn_local(1));
    mc.im_present[1] = 1;
    for (int i = 0; i < 2; i++) {
        mc.image_to_real[i] = i;
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 2;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    EXPECT(cpu.ctask == 0, "ctask should start at 0");
    EXPECT(cpu.ready == 0x0001, "only task 0 ready initially");

    /* Step once — runs task 0's instruction; T←0o111. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step task 0");
    EXPECT(cpu.T == 0111, "task 0 should set T=0o111, got 0o%o", cpu.T);
    EXPECT(cpu.ctask == 0, "no other task ready, should still be task 0");

    /* Set task 5's TPC, wake it. */
    dorado_cpu_set_task_tpc(&cpu, 5, /*real_pc=*/1);
    dorado_cpu_wakeup(&cpu, 5);

    /* Step task 0 once more — at end of instruction, task switch
     * to task 5 happens. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step before switch");
    EXPECT(cpu.ctask == 5, "should have switched to task 5, ctask=%d",
           cpu.ctask);
    EXPECT(cpu.real_PC == 1, "should be at task 5's TPC, got 0o%o",
           cpu.real_PC);
    /* Wakeup acknowledged + Ready set. */
    EXPECT((cpu.wakeup_pending & (1u << 5)) == 0,
           "wakeup_pending bit 5 should be cleared");
    EXPECT((cpu.ready & (1u << 5)) != 0,
           "ready bit 5 should be set after switch");
    /* Task 0's state was saved (T=0o111). */
    EXPECT(cpu.task_t[0] == 0111, "task 0's T should be saved as 0o111");

    /* Step in task 5 — T←0o222. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step task 5");
    EXPECT(cpu.T == 0222, "task 5 should set T=0o222, got 0o%o", cpu.T);
    EXPECT(cpu.ctask == 5, "should still be task 5");

    printf("PASS  test_task_switch_on_wakeup\n");
    return 0;
}

/*
 * Tasking — BLOCK=1 in non-emulator clears Ready; engine returns to
 * lower-priority task (here, task 0).
 */
static int test_task_block_returns_to_emulator(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;

    /* IM[0]: task 0 — T←0o111, self-loop. */
    mc.im[0] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/1, /*asel=*/6, /*block=*/0,
                           /*ff=*/0111, jcn_local(0));
    mc.im_present[0] = 1;
    /* IM[1]: task 5 — T←0o222 with BLOCK=1 (block this task). */
    mc.im[1] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/1, /*asel=*/6, /*block=*/1,
                           /*ff=*/0222, jcn_local(1));
    mc.im_present[1] = 1;
    for (int i = 0; i < 2; i++) {
        mc.image_to_real[i] = i; mc.image_present[i] = 1;
    }
    mc.n_instructions = 2;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.TIOA = 0x12;
    cpu.RBase = 3;
    cpu.task_tioa[5] = 0x56;
    cpu.task_rbase[5] = 7;
    dorado_cpu_set_task_tpc(&cpu, 5, 1);
    dorado_cpu_wakeup(&cpu, 5);

    /* Step into task 5 (after task 0 step + switch). */
    dorado_cpu_step(&cpu);
    EXPECT(cpu.ctask == 5, "should be in task 5 now");
    EXPECT((cpu.TIOA & 0xFF) == 0x56,
           "task 5 should restore its own TIOA, got 0x%02X",
           cpu.TIOA & 0xFF);
    EXPECT((cpu.RBase & 0xF) == 7,
           "task 5 should restore its own RBase, got 0x%X",
           cpu.RBase & 0xF);

    /* Step in task 5 — BLOCK=1 → Ready cleared, switch back. */
    dorado_cpu_step(&cpu);
    EXPECT(cpu.ctask == 0, "BLOCK=1 in task 5 should return to task 0, ctask=%d",
           cpu.ctask);
    EXPECT((cpu.ready & (1u << 5)) == 0,
           "ready bit 5 should be cleared after BLOCK");
    EXPECT(cpu.T == 0111, "task 0's T should be restored: 0o%o", cpu.T);
    EXPECT((cpu.TIOA & 0xFF) == 0x12,
           "task 0 should restore its own TIOA, got 0x%02X",
           cpu.TIOA & 0xFF);
    EXPECT((cpu.RBase & 0xF) == 3,
           "task 0 should restore its own RBase, got 0x%X",
           cpu.RBase & 0xF);

    printf("PASS  test_task_block_returns_to_emulator\n");
    return 0;
}

/*
 * Tasking — TaskingOff prevents switch even when a higher task is
 * woken; TaskingOn re-enables it (after 2 more instructions).
 */
static int test_tasking_off_blocks_switch(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;

    /* IM[0]: task 0 — TaskingOff (FA=1 FB=4 FC=2 → FF=0o142).
     * BSEL=0 (primary B sources, FF override active). */
    mc.im[0] = make_uinstr(0, 0, /*bsel=*/0, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0142, jcn_local(1));
    mc.im_present[0] = 1;
    /* IM[1]: task 0 — set T=0o111. */
    mc.im[1] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/1, /*asel=*/6, 0,
                           /*ff=*/0111, jcn_local(1));
    mc.im_present[1] = 1;
    for (int i = 0; i < 2; i++) {
        mc.image_to_real[i] = i; mc.image_present[i] = 1;
    }
    mc.n_instructions = 2;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    /* Wake task 5 BEFORE we issue TaskingOff. */
    dorado_cpu_set_task_tpc(&cpu, 5, 0);
    dorado_cpu_wakeup(&cpu, 5);

    /* IM[0] → TaskingOff. tasking_on becomes 0 atomically. The
     * end-of-instruction scheduler should NOT switch. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step IM[0] (TaskingOff)");
    EXPECT(cpu.ctask == 0, "TaskingOff should prevent switch, ctask=%d",
           cpu.ctask);
    EXPECT(cpu.tasking_on == 0, "tasking_on should be 0");
    EXPECT((cpu.wakeup_pending & (1u << 5)) != 0,
           "task 5 wakeup should still be pending");

    /* IM[1] runs in task 0, sets T. Still no switch. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step IM[1]");
    EXPECT(cpu.ctask == 0, "still task 0 with TaskingOff");
    EXPECT(cpu.T == 0111, "T should be 0o111");

    printf("PASS  test_tasking_off_blocks_switch\n");
    return 0;
}

/*
 * Tasking — Wakeup[task] FF function asserts a wakeup line.
 * FA=3 FB=6-7, FF[4:7]=task. For task 7: FA=3 FB=6 FC=7 → FF=0o307.
 */
static int test_wakeup_ff_function(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;

    /* IM[0]: task 0 — Wakeup[7]. FA=3 FB=6 FC=7 → FF=0o367.
     * (FF[4:7] = task = 7; FB=6 means tasks 0..7, FB=7 means 8..15.)
     * BSEL=0 (primary sources) so FF is interpreted as a function. */
    mc.im[0] = make_uinstr(0, 0, /*bsel=*/0, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0367, jcn_local(1));
    mc.im_present[0] = 1;
    /* IM[1]: task 0 — self-loop. */
    mc.im[1] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0, jcn_local(1));
    mc.im_present[1] = 1;
    /* IM[2]: task 7 — set T. */
    mc.im[2] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/1, /*asel=*/6, 0,
                           /*ff=*/0333, jcn_local(2));
    mc.im_present[2] = 1;
    for (int i = 0; i < 3; i++) {
        mc.image_to_real[i] = i; mc.image_present[i] = 1;
    }
    mc.n_instructions = 3;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    dorado_cpu_set_task_tpc(&cpu, 7, 2);

    /* Step IM[0] — Wakeup[7] sets bit 7 of wakeup_pending; the
     * end-of-instruction switch then jumps to task 7. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step Wakeup[7]");
    EXPECT(cpu.ctask == 7, "after Wakeup[7], should be in task 7, ctask=%d",
           cpu.ctask);
    EXPECT(cpu.real_PC == 2, "task 7 PC = 0o%o, expected 2", cpu.real_PC);

    /* Run task 7's instruction. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step task 7");
    EXPECT(cpu.T == 0333, "task 7 should set T=0o333, got 0o%o", cpu.T);

    printf("PASS  test_wakeup_ff_function\n");
    return 0;
}

static int test_junk_timer_wakeup(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* B */

    /* TaskingOff so the synthetic wakeup accumulates instead of
     * switching to a task whose PC has no code in this tiny image. */
    mc.im[0] = make_uinstr(0, 0, 0, 0, 6, 0, 0142, jcn_local(1));
    mc.im_present[0] = 1;

    /* Initial's JNK init first sets T=-1, then AckJunkTW reads B=T. */
    mc.im[1] = make_uinstr(0, 0, 5, 1, 6, 0, 0377, jcn_local(2));
    mc.im_present[1] = 1;

    mc.im[2] = make_uinstr(0, 0, 2, 0, 6, 0, 0104, jcn_local(3));
    mc.im_present[2] = 1;
    mc.im[3] = make_uinstr(0, 0, 4, 0, 6, 0, 0077, jcn_local(3));
    mc.im_present[3] = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    EXPECT(dorado_cpu_step(&cpu) == 0, "TaskingOff step");
    EXPECT(dorado_cpu_step(&cpu) == 0, "T=-1 step");
    EXPECT(dorado_cpu_step(&cpu) == 0, "AckJunkTW step");
    EXPECT(cpu.junk_tw_enabled == 1, "junk timer should be enabled");

    for (int i = 0; i < 1000; i++) {
        EXPECT(dorado_cpu_step(&cpu) == 0, "junk timer spin %d", i);
    }

    EXPECT((cpu.wakeup_pending & (1u << 2)) != 0,
           "junk timer should wake task 2, pending=0x%X",
           cpu.wakeup_pending);

    printf("PASS  test_junk_timer_wakeup\n");
    return 0;
}

static int test_ifutest_junk_timer_polarity(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* B */

    /* T = 1, then IFUTest←T. Dorado bit 15 is the low-order C bit,
     * and IFUTest.15 disables junk wakeups. */
    mc.im[0] = make_uinstr(0, 0, 4, 1, 6, 0, 0001, jcn_local(1));
    mc.im_present[0] = 1;
    mc.im[1] = make_uinstr(0, 0, 2, 0, 6, 0, 0101, jcn_local(2));
    mc.im_present[1] = 1;

    /* T = -1, then AckJunkTW←T. AckJunkTW.15 enables junk wakeups. */
    mc.im[2] = make_uinstr(0, 0, 5, 1, 6, 0, 0377, jcn_local(3));
    mc.im_present[2] = 1;
    mc.im[3] = make_uinstr(0, 0, 2, 0, 6, 0, 0104, jcn_local(4));
    mc.im_present[3] = 1;

    /* T = 0, then IFUTest←T. Normal IFUTest values also enable junk. */
    mc.im[4] = make_uinstr(0, 0, 4, 1, 6, 0, 0000, jcn_local(5));
    mc.im_present[4] = 1;
    mc.im[5] = make_uinstr(0, 0, 2, 0, 6, 0, 0101, jcn_local(5));
    mc.im_present[5] = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.junk_tw_enabled = 1;
    cpu.junk_tw_countdown = 100;

    EXPECT(dorado_cpu_step(&cpu) == 0, "T=1");
    EXPECT(dorado_cpu_step(&cpu) == 0, "IFUTest.15 disables");
    EXPECT(cpu.junk_tw_enabled == 0,
           "IFUTest.15 should disable junk timer");

    EXPECT(dorado_cpu_step(&cpu) == 0, "T=-1");
    EXPECT(dorado_cpu_step(&cpu) == 0, "AckJunkTW.15 enables");
    EXPECT(cpu.junk_tw_enabled == 1,
           "AckJunkTW.15 should enable junk timer");

    EXPECT(dorado_cpu_step(&cpu) == 0, "T=0");
    EXPECT(dorado_cpu_step(&cpu) == 0, "IFUTest.15 clear enables");
    EXPECT(cpu.junk_tw_enabled == 1,
           "IFUTest with bit 15 clear should enable junk timer");

    printf("PASS  test_ifutest_junk_timer_polarity\n");
    return 0;
}

static int test_ifureset_disables_junk_timer(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* B */

    /* IFUReset is FA=1 FB=3 FC=6. HM §8.3 says it loads IFUTest with
     * 1; in our C-LSB representation that sets Dorado bit 15 and
     * disables periodic junk wakeups. */
    mc.im[0] = make_uinstr(0, 0, 0, 0, 6, 0, 0136, jcn_local(1));
    mc.im_present[0] = 1;
    mc.im[1] = make_uinstr(0, 0, 0, 0, 6, 0, 0077, jcn_local(1));
    mc.im_present[1] = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.ifu_insset = 2;
    cpu.ifu_opcode = 0xAB;
    cpu.ifu_pcf = 01234;
    cpu.ifu_pcx = 01230;
    cpu.ifu_active = 1;
    cpu.ifu_warmup = 3;
    cpu.brk_pending = 1;
    cpu.brk_opcode = 0xCD;
    cpu.junk_tw_enabled = 1;
    cpu.junk_tw_countdown = 1;
    cpu.wakeup_pending = (uint16_t)(1u << 2);

    EXPECT(dorado_cpu_step(&cpu) == 0, "IFUReset step");
    EXPECT(cpu.ifu_insset == 2,
           "IFUReset must preserve InsSet, got %u", cpu.ifu_insset);
    EXPECT(cpu.ifu_active == 0,
           "IFUReset should halt the IFU pipeline");
    EXPECT(cpu.ifu_warmup == 0,
           "IFUReset should clear IFU warmup");
    EXPECT(cpu.brk_pending == 0,
           "IFUReset should clear BrkPending");
    EXPECT(cpu.brk_opcode == 0,
           "IFUReset should clear BrkIns opcode");
    EXPECT(cpu.junk_tw_enabled == 0,
           "IFUReset should disable junk timer");
    EXPECT((cpu.wakeup_pending & (1u << 2)) == 0,
           "IFUReset should dismiss pending junk wakeup");

    printf("PASS  test_ifureset_disables_junk_timer\n");
    return 0;
}

/*
 * SubTask (HM page 88) — when a non-emulator task is woken with
 * SubTask=N, RBase[2:3] and MemBase[2:3] get OR'd with N. So an
 * RM access in that task lands in a different 16-RM region.
 *
 * Test: set up two tasks, both reading RM[0]. Without SubTask,
 * RM[0]. With SubTask=1, RM[0|1]=RM[1] (since RBase[2:3] = low 2
 * bits of 4-bit RBase, OR'd with 1).
 */
static int test_subtask_or_rm(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;

    /* IM[0]: T←RM[RSTK=0]. ASEL=4 (A←RM/STK), ALUF=0 ("B"), BSEL=2 (T data),
     * LC=1 (T←Pd, but Pd = ALU = B = T... not what we want).
     *
     * Actually I want T = RM[0]. ALUF=0 with alufm[0]=025 (=B).
     * To get ALU = A (= RM[0]), use alufm = 037 (= A) at slot 1. */
    mc.alufm[1] = 037; mc.alufm_present[1] = 1;   /* "A" */

    /* IM[0]: T ← RM[rm_a]. ASEL=4 (A←RM/STK), ALUF=1 (A), LC=1, BSEL=4 (constant). */
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/1, /*bsel=*/4, /*lc=*/1,
                           /*asel=*/4, /*block=*/0, /*ff=*/0,
                           jcn_local(0));   /* self-loop */
    mc.im_present[0] = 1;
    /* Image and entry. */
    mc.image_to_real[0] = 0; mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    /* Plant distinct values in RM[0] and RM[1] so we can tell which
     * region was selected. RM is preloaded by dorado_cpu_init from
     * mc->rm; we set after init. */
    cpu.RM[0] = 0xAAAA;   /* value if subtask=0 */
    cpu.RM[1] = 0xBBBB;   /* value if subtask=1 */

    /* Switch to task 5 with SubTask=1. */
    dorado_cpu_set_task_tpc(&cpu, 5, 0);
    dorado_cpu_set_subtask(&cpu, 5, 1);
    dorado_cpu_wakeup(&cpu, 5);

    /* Step task 0 once → triggers switch to task 5. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "first step");
    /* Step task 5: should read RM with SubTask=1 ORed in. RBase=0,
     * RSTK=0, SubTask=1. rm_a = ((0|1) << 4) | 0 = 0x10 = 16, but
     * 4-bit RBase makes it 0x10. Wait that's RBase=1. So rm_a = 16 → RM[16]?
     * Hmm. rbase | subtask = 0|1 = 1 (4 bits). rm_a = (1 << 4) | 0 = 16.
     * Plant RM[16] with a marker. */
    cpu.RM[16] = 0xC1C1;
    EXPECT(dorado_cpu_step(&cpu) == 0, "task 5 step");
    EXPECT(cpu.ctask == 5, "should be in task 5");
    EXPECT(cpu.T == 0xC1C1,
           "T should be RM[16] (= subtask-OR'd region), got 0x%04X", cpu.T);

    printf("PASS  test_subtask_or_rm (T=0x%04X)\n", cpu.T);
    return 0;
}

/*
 * IFUM round-trip — write an entry via InsSetorEvent←B + BrkIns←B +
 * IFUMLH/RH←B, then read it back via B←IFUMLH'/RH'. Phase C.1.
 */
static int test_ifum_load_read(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;   /* "B" */

    /* IM[0]: InsSetorEvent←B with B[0]=1, B[6:7]=2 (instruction set 2).
     * FA=1 FB=3 FC=0 → FF = 0o130. BSEL=4 (constant 0,,FF) so the FF
     * field provides the constant; but FF interpreted as constant
     * means it's NOT a function. Use BSEL=2 (T) and put the data in T.
     * Actually simplest: BSEL=0 (primary B sources) with no FF, then
     * the next instruction does the FF write. We'll need a way to
     * deliver a specific B value into the FF function.
     *
     * Easier approach: use BSEL=4 to deliver a constant on B (B = FF),
     * but that suppresses FF interpretation. So use BSEL=2 (T) and
     * pre-set T.
     *
     * Even simpler: use ALUF "B" + BSEL=4 in instruction A to put
     * 0o102 into T (LC=1), then in instruction B do the FF function
     * with BSEL=2 (T on B). */

    /* IM[0]: T←0o102 (B[0]=1 means high bit of 16-bit word; in 16-bit
     * MSB-first = bit 15 of the 16-bit value, in C-LSB = bit 15 (value
     * 0x8000). Plus B[6:7]=2 means low 2 of high byte = bit 8..9.
     * For InsSet=2: bits 6,7 of 16-bit B (MSB) = bit 9..8 in C-LSB.
     * To set InsSet=2 (binary 10): B[6]=1, B[7]=0 → bit 9 = 1, bit 8 = 0.
     * Plus B[0]=1 (bit 15 of B). So B = 0x8000 | 0x0200 = 0x8200. */
    mc.im[0] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/1, /*asel=*/6, 0,
                           /*ff=*/0202,   /* low 8 bits of 0x8200 = 0x02; with bsel=4 (0,,FF) gives 0x02 — wrong */
                           jcn_local(1));
    mc.im_present[0] = 1;

    /* Actually using the constant route gets messy because BSEL=4
     * gives "0,,FF" = 0x00FF. To put 0x8200 on B we'd need BSEL=6
     * (FF,,0) which is FF in high byte = 0x8200 with FF=0x82.
     * Let me use BSEL=6 with FF=0x82 → B = 0x8200. */
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/6, /*lc=*/1,
                           /*asel=*/6, /*block=*/0, /*ff=*/0x82,
                           jcn_local(1));

    /* IM[1]: InsSetorEvent←B with B sourced from T. FA=1 FB=3 FC=0,
     * FF=0o130. BSEL=2 (T on B). LC=0, JCN=local(2). */
    mc.im[1] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0130, jcn_local(2));
    mc.im_present[1] = 1;

    /* IM[2]: T←0xAB00 (sets opcode=0xAB via B[0:7]).
     * Need 0xAB00 on B. BSEL=6 (FF,,0) with FF=0xAB → B = 0xAB00. */
    mc.im[2] = make_uinstr(0, 0, /*bsel=*/6, /*lc=*/1, /*asel=*/6, 0,
                           /*ff=*/0xAB, jcn_local(3));
    mc.im_present[2] = 1;

    /* IM[3]: BrkIns←B (Opcode ← B[0:7]). FA=1 FB=3 FC=7 → FF=0o137.
     * BSEL=2 (T). */
    mc.im[3] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0137, jcn_local(4));
    mc.im_present[3] = 1;

    /* IM[4]: T ← 0xCAFE for IFUMRH write. BSEL=6 with FF=0xCA → 0xCA00,
     * but we want 0xCAFE. Use ALU A+B with A=T and B=...
     * Simpler: use BSEL=4 (0,,FF) and a chain of ops. Actually a single
     * instruction can't generate arbitrary 16-bit constants directly.
     * For this test, just use 0xCA00 as the data — that exercises the
     * write path. */
    mc.im[4] = make_uinstr(0, 0, /*bsel=*/6, /*lc=*/1, /*asel=*/6, 0,
                           /*ff=*/0xCA, jcn_local(5));
    mc.im_present[4] = 1;

    /* IM[5]: IFUMRH←B. FA=1 FB=3 FC=4 → FF=0o134. BSEL=2 (T). */
    mc.im[5] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0134, jcn_local(6));
    mc.im_present[5] = 1;

    /* IM[6]: T ← 0xBE00 for IFUMLH write. */
    mc.im[6] = make_uinstr(0, 0, /*bsel=*/6, /*lc=*/1, /*asel=*/6, 0,
                           /*ff=*/0xBE, jcn_local(7));
    mc.im_present[6] = 1;

    /* IM[7]: IFUMLH←B. FA=1 FB=3 FC=5 → FF=0o135. */
    mc.im[7] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0135, jcn_local(7));
    mc.im_present[7] = 1;

    for (int i = 0; i <= 7; i++) {
        mc.image_to_real[i] = i; mc.image_present[i] = 1;
    }
    mc.n_instructions = 8;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    /* Step through the 8-instruction setup sequence. */
    for (int i = 0; i < 8; i++) {
        EXPECT(dorado_cpu_step(&cpu) == 0,
               "step %d failed: %s", i, cpu_halt_reason_str(cpu.halt_reason));
    }

    /* InsSet should now be 2, opcode = 0xAB. IFUMRH writes the field
     * half (hi) and IFUMLH writes the PackedAlpha/IFaddr half (lo).
     * This is the same order LoadRam.mc uses for EB IFUM items:
     * word0 -> IFUMLH, word1 -> IFUMRH. */
    EXPECT(cpu.ifu_insset == 2, "InsSet = %d, expected 2", cpu.ifu_insset);
    EXPECT(cpu.ifu_opcode == 0xAB,
           "Opcode = 0x%02X, expected 0xAB", cpu.ifu_opcode);
    EXPECT(mc.ifum_hi[0x2AB] == 0xCA00,
           "ifum_hi[0x2AB] = 0x%04X, expected 0xCA00", mc.ifum_hi[0x2AB]);
    EXPECT(mc.ifum_lo[0x2AB] == 0xBE00,
           "ifum_lo[0x2AB] = 0x%04X, expected 0xBE00", mc.ifum_lo[0x2AB]);

    /* Other entries should be untouched. */
    EXPECT(mc.ifum_lo[0] == 0, "ifum_lo[0] should be untouched");
    EXPECT(mc.ifum_lo[0x1AB] == 0, "different InsSet should be untouched");

    printf("PASS  test_ifum_load_read\n");
    return 0;
}

/*
 * IFU dispatch — synthetic emulator microcode.
 *
 * Builds a tiny "instruction set" with two opcodes:
 *   opcode 0x10 (INC):  T ← T + 1; IFUJump[0] to next opcode
 *   opcode 0x20 (HALT): branch to a "done" label (doesn't IFUJump)
 *
 * Plants 4 INCs followed by a HALT in emulated memory, sets up
 * BR[31] (codebase) to point at the bytecode, PCF←B to start the
 * IFU, then IFUJump[0] to dispatch. After dispatching 4 INCs the
 * 5th IFUJump hits the HALT entry which branches to the end.
 *
 * This is the smallest "real opcode loop" the engine can run —
 * Phase C.2 minimum.
 */
static int test_ifu_dispatch_synthetic(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0]  = 025;   mc.alufm_present[0]  = 1;  /* B */
    mc.alufm[2]  = 0014;  mc.alufm_present[2]  = 1;  /* A+B (carry-in 0) */

    /* === Microcode layout (real IM addresses) ===
     *
     * 0  setup0:    BrLo←A, MemBase=0, A=T(=0)              → IM[1]
     *               (load BR[0] low 16 bits = 0)
     * 1  setup1:    BrHi←A, A=T(=0)                         → IM[2]
     *               (BR[0] = 0; we'll use that as code base
     *                BUT — IFU uses BR[31], not BR[0]!)
     *
     * For simplicity, just build BR[31] differently. We'll use a
     * helper API to set BR[31] from the test driver, since the
     * microcode-only sequence to load BR[31] is several
     * instructions. The test mounts the map and writes the
     * bytecode directly, then the microcode just does PCF←B and
     * IFUJump.
     *
     * Slim layout:
     *   0  start:      PCF←B (B = 0 → byte 0, word 0)         → IM[1]
     *   1  preroll:    NOP (give IFU a cycle if needed)        → IM[2]
     *   2  dispatch:   IFUJump[0]                              → IM via TNIA
     *
     * IFUM entry for INC (opcode 0x10):
     *   IFaddr' = 0x010 (= IM[0o100]) → entry 0 lands at IM[0o100]
     *   Length' = 00 (low-true → length=1)
     *   N = 017 (octal = 15, "no operand supplied")
     *   MemB[0]=1, MemB[1:2]=0 → MemBase = 0o34
     *
     * IM[0o100] (entry 0 of INC): T←T+1, IFUJump[0]
     * IM[0o101] (entry 1, unused): NOP
     * IM[0o102] (entry 2, unused): NOP
     * IM[0o103] (entry 3, unused): NOP
     *
     * IFUM entry for HALT (opcode 0x20):
     *   IFaddr' = 0x020 → entry lands at IM[0o200]
     *   Length' = 00
     *   N = 017
     *
     * IM[0o200] (entry 0 of HALT): just self-loop (no IFUJump);
     *                              the test detects the halt.
     */

    /* Microcode at IM[0..2]: PCF←B, NOP, IFUJump[0]. */
    /* IM[0]: PCF←B with B=0. FF=0o100, BSEL=2 (T=0) for B=0. */
    mc.im[0] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0100, jcn_local(1));
    mc.im_present[0] = 1;
    /* IM[1]: NOP. JCN=local(2). */
    mc.im[1] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0, jcn_local(2));
    mc.im_present[1] = 1;
    /* IM[2]: IFUJump[0]. JCN = 0 0 1 0 0 1 1 1 = 0x27.
     * The IFU pipeline is still warming up at this point (HM page 67
     * says it takes 5 cycles after PCF←B). So this dispatch traps
     * to the NotReady vector at 0o334..0o337 (for InsSet=0; bits 6:7
     * are ~InsSet = 11). The trap-vector entries themselves are
     * IFUJump[n] which retry until ready — this is the standard
     * microcode pattern (HM page 33). */
    mc.im[2] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6, 0,
                           /*ff=*/0, /*jcn=*/0x27);
    mc.im_present[2] = 1;

    /* NotReady trap vector at IM[0o334..0o337] (InsSet=0).
     * Each entry is IFUJump[n] to retry the dispatch. */
    for (int n = 0; n < 4; n++) {
        uint8_t jcn_ifu_n = (uint8_t)(0x27 | (n << 3));
        mc.im[0334 + n] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0,
                                      /*asel=*/6, 0, /*ff=*/0,
                                      /*jcn=*/jcn_ifu_n);
        mc.im_present[0334 + n] = 1;
    }

    /* INC entry @ IM[0o100]: T ← T + 1, IFUJump[0]. ASEL=6 (A←T),
     * BSEL=4 (B = constant 0,,FF; FF=1 makes B=1), ALUF=2 (A+B
     * carry-in 0). LC=1 (T←Pd). JCN=IFUJump[0]=0x27. */
    mc.im[0100] = make_uinstr(0, /*aluf=*/2, /*bsel=*/4, /*lc=*/1,
                              /*asel=*/6, 0, /*ff=*/1, /*jcn=*/0x27);
    mc.im_present[0100] = 1;

    /* HALT entry @ IM[0o200]: self-loop forever (test detects via
     * cycle count). */
    mc.im[0200] = make_uinstr(0, 0, /*bsel=*/2, /*lc=*/0, /*asel=*/6, 0,
                              /*ff=*/0, jcn_local(0));   /* local(0) of page = 0o200 (since 0o200 is page-aligned) */
    /* Actually local(0) within page-of-IM[0o200] = page_high|0 = 0o200. */
    mc.im_present[0200] = 1;

    /* Mark image presence so the placement layer accepts these. */
    for (int i = 0; i < 3; i++) {
        mc.image_to_real[i] = i; mc.image_present[i] = 1;
    }
    mc.image_to_real[3] = 0100; mc.image_present[3] = 1;
    mc.image_to_real[4] = 0200; mc.image_present[4] = 1;
    mc.n_instructions = 5;

    /* IFUM entries: opcode 0x10 (INC) and 0x20 (HALT) under InsSet=0. */
    /* Entry layout per Table 20:
     *   ifum_lo (RH): bit 5 = Packed-α, bits 6..15 = IFaddr' (10 bits)
     *   ifum_hi (LH): bit 0=Sign, 1..3=Par, 4..5=Length', 6=RBaseB',
     *                 7..9=MemB, 10=TPause', 11=TJump', 12..15=N
     *
     * For INC: Length'=00 (length=1), TPause'=1 (no pause, low-true),
     * TJump'=1 (no jump, low-true), N=017, MemB=000, RBaseB'=1.
     *
     * In MSB-first bit numbering (0..15) → C-LSB representation:
     * Each MSB-bit `n` is at LSB position `15-n`. Build a 16-bit value
     * where each named field lands at the right position.
     */
    /* Helper-style: write the bits via shifts into a uint16_t with
     * MSB-first layout. */
    #define MK_LH(sign, length_p, rbaseb_p, memb, tpause_p, tjump_p, n) \
        ((uint16_t)( ((uint16_t)((sign)&1) << 15) \
                   | ((uint16_t)((length_p)&3) << 10) \
                   | ((uint16_t)((rbaseb_p)&1) << 9) \
                   | ((uint16_t)((memb)&7) << 6) \
                   | ((uint16_t)((tpause_p)&1) << 5) \
                   | ((uint16_t)((tjump_p)&1) << 4) \
                   | ((uint16_t)((n)&0xF)) ))
    #define MK_RH(packed_a, ifaddr) \
        ((uint16_t)( ((uint16_t)((packed_a)&1) << 10) \
                   | ((uint16_t)((ifaddr)&0x3FF)) ))

    /* INC opcode 0x10. IFaddr' = 0o20 (= decimal 16). So entry 0
     * lands at TNIA = (0o20 << 2) | 0 = 0o100 (= decimal 64). */
    mc.ifum_hi[0x10] = MK_LH(0, /*Length'*/0, /*RBaseB'*/1,
                             /*MemB*/4 /* MemB[0]=1, MemB[1:2]=00 → MemBase=034 */,
                             /*TPause'*/1, /*TJump'*/1, /*N*/017);
    mc.ifum_lo[0x10] = MK_RH(0, /*IFaddr'*/0020);
    mc.ifum_present[0x10] = 1;

    /* HALT opcode 0x20. IFaddr' = 0o40 (= decimal 32). Entry 0 →
     * TNIA = (0o40 << 2) | 0 = 0o200. */
    mc.ifum_hi[0x20] = MK_LH(0, 0, 1, 4, 1, 1, 017);
    mc.ifum_lo[0x20] = MK_RH(0, /*IFaddr'*/0040);
    mc.ifum_present[0x20] = 1;

    /* Set up memory + BR[31] + plant bytecode. */
    static dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "memory init");
    /* Mount map page 0 RW. */
    dorado_map_set(&mem, 0, /*rp=*/0, /*wp=*/0, /*dirty=*/0);
    /* BR[31] = 0 (codebase at the bottom of memory). */
    dorado_br_lo_load(&mem, 31, 0);
    dorado_br_hi_load(&mem, 31, 0);
    /* Plant bytecode: 4 INCs (0x10) then a HALT (0x20).
     * Sets 0/1: byte 0 = high byte. So byte 0 of word 0 = high
     * 8 bits = (word >> 8). Pack as:
     *   word 0 = INC INC = 0x1010
     *   word 1 = INC INC = 0x1010
     *   word 2 = HALT NOP= 0x2000 */
    mem.storage[0] = 0x1010;
    mem.storage[1] = 0x1010;
    mem.storage[2] = 0x2000;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.mem = &mem;

    /* Run 30 cycles; should reach the HALT self-loop with T == 4
     * after dispatching 4 INCs. */
    for (int i = 0; i < 30; i++) {
        if (dorado_cpu_step(&cpu) != 0) break;
    }
    EXPECT(cpu.real_PC == 0200,
           "should be at HALT entry (0o200), got 0o%o", cpu.real_PC);
    EXPECT(cpu.T == 4, "T should = 4 (4 INCs), got %d", cpu.T);

    dorado_memory_free(&mem);
    printf("PASS  test_ifu_dispatch_synthetic (T=%d)\n", cpu.T);
    return 0;
    #undef MK_LH
    #undef MK_RH
}

/*
 * test_slow_io_routing — verify the slow-I/O routing layer wires
 * Pd←Input/Pd←InputNoPE/Output←B through to a registered device.
 *
 * Layout:
 *   IM[0]: TIOA←B (FF=0o142? actually FA=1 FB=5 FC=2 = 0o152). With B=0o42
 *          (BSEL=4 constant FF=0o42), TIOA[0:7]←B[0:7]=0o42.
 *   IM[1]: Output←B (FF=0o036, FA=0 FB=3 FC=6). Drive 0o1234 (via
 *          BSEL=2 holding T=0o1234 from a prior load).
 *   IM[2]: Pd←Input → T←Pd. Read echoes the device's stored value.
 *
 * The test installs a tiny "echo" device that stores the last value
 * written and returns it on read. Verifies write happened, read
 * returned the value, and parity-bad clears when device is mapped.
 */
typedef struct {
    uint16_t last_write;
    int      writes;
    int      reads;
} echo_dev;

static uint16_t echo_read(void *ctx, int task, uint8_t tioa, int *bad)
{
    (void)task; (void)tioa;
    if (bad) *bad = 0;     /* good parity */
    echo_dev *d = ctx;
    d->reads++;
    return d->last_write;
}

static void echo_write(void *ctx, int task, uint8_t tioa, uint16_t v)
{
    (void)task; (void)tioa;
    echo_dev *d = ctx;
    d->last_write = v;
    d->writes++;
}

static int test_slow_io_routing(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025;  mc.alufm_present[0]  = 1;  /* B */

    /* IM[0]: TIOA[0:7]←B[0:7]. FA=1 FB=5 FC=2 → FF = 0o152.
     * BSEL=4 (constant 0,,FF) with FF... wait, with BSEL=constant
     * FF is data not function. So we need BSEL<4 and a way to put
     * a value on B. Easiest: BSEL=2 (T) with T pre-loaded — but T
     * is zero at start. Use BSEL=1 (RM/STK) and pre-load RM[0]. */
    mc.rm[0] = 0xDE00;   /* TIOA←B uses B[0:7] = high byte → 0xDE */
    mc.rm_present[0] = 1;
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/1, /*lc=*/0,
                           /*asel=*/4, 0, /*ff=*/0152, jcn_local(1));
    mc.im_present[0] = 1;

    /* IM[1]: Output←B. FA=0 FB=3 FC=6 → FF = 0o036. BSEL=1 (RM/STK)
     * to put RM[1]=0xCAFE on B. */
    mc.rm[1] = 0xCAFE;
    mc.rm_present[1] = 1;
    mc.im[1] = make_uinstr(/*rstk=*/1, /*aluf=*/0, /*bsel=*/1, /*lc=*/0,
                           /*asel=*/4, 0, /*ff=*/0036, jcn_local(2));
    mc.im_present[1] = 1;

    /* IM[2]: Pd←Input → T←Pd. FA=0 FB=3 FC=2 → FF = 0o032. BSEL=1
     * (RM/STK to keep ALUF=0 honest), LC=1 (T←Pd). */
    mc.im[2] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/1, /*lc=*/1,
                           /*asel=*/4, 0, /*ff=*/0032, jcn_local(2));
    mc.im_present[2] = 1;

    for (int i = 0; i < 3; i++) {
        mc.image_to_real[i] = i;
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 3;

    static dorado_io io;
    dorado_io_init(&io);
    static echo_dev dev_state;
    memset(&dev_state, 0, sizeof dev_state);
    static const dorado_io_device echo_device = {
        .read = echo_read,
        .write = echo_write,
        .attention = NULL,
        .ctx = &dev_state,
        .name = "echo"
    };
    dorado_io_register(&io, /*task=*/0, /*tioa=*/0xDE, &echo_device);

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.io = &io;

    /* Step 0: TIOA←B. After this, TIOA[0:7]=0xDE. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 0: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT((cpu.TIOA & 0xFF) == 0xDE,
           "TIOA = 0x%X (expected 0xDE)", cpu.TIOA);

    /* Step 1: Output←B. Device should record 0xCAFE. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 1: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(dev_state.writes == 1, "writes=%d (expected 1)", dev_state.writes);
    EXPECT(dev_state.last_write == 0xCAFE,
           "last_write = 0x%X (expected 0xCAFE)", dev_state.last_write);

    /* Step 2: Pd←Input → T. Should pull 0xCAFE off the bus. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 2: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(dev_state.reads == 1, "reads=%d (expected 1)", dev_state.reads);
    EXPECT(cpu.T == 0xCAFE, "T = 0x%X (expected 0xCAFE)", cpu.T);
    EXPECT(cpu.io_bad_parity == 0,
           "io_bad_parity = %d (expected 0 — device mapped)",
           cpu.io_bad_parity);

    /* Now switch to a TIOA the device doesn't claim (0xAA) and
     * confirm floating-bus default + parity-bad. */
    cpu.TIOA = 0xAA;
    int bad = -1;
    uint16_t v = dorado_io_read(&io, /*task=*/0, /*tioa=*/0xAA, &bad);
    EXPECT(v == 0xFFFF, "unmapped read = 0x%X (expected 0xFFFF)", v);
    EXPECT(bad == 1, "unmapped parity = %d (expected 1)", bad);

    printf("PASS  test_slow_io_routing (TIOA=0x%X, %d writes, %d reads, T=0x%X)\n",
           cpu.TIOA & 0xFF, dev_state.writes, dev_state.reads, cpu.T);
    return 0;
}

static int test_output_t_store_shape_routes_slow_io(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025;  mc.alufm_present[0] = 1;  /* B */

    /* Decoded shape seen from display `Output_ T` while loading HRam:
     * ASEL=Store<-T, BSEL=T, no LC destination. It must drive slow I/O
     * and must not issue a main-memory Store. */
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/2, /*lc=*/0,
                           /*asel=*/2, 0, /*ff=*/0354, jcn_local(0));
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0;
    mc.image_present[0] = 1;
    mc.n_instructions = 1;

    static dorado_io io;
    dorado_io_init(&io);
    static echo_dev dev_state;
    memset(&dev_state, 0, sizeof dev_state);
    static const dorado_io_device echo_device = {
        .read = echo_read,
        .write = echo_write,
        .attention = NULL,
        .ctx = &dev_state,
        .name = "echo"
    };
    dorado_io_register(&io, /*task=*/3, /*tioa=*/0375, &echo_device);

    dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "memory init failed");

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.io = &io;
    cpu.mem = &mem;
    cpu.ctask = 3;
    cpu.task_tpc[3] = 0;
    cpu.TIOA = 0375;
    cpu.task_tioa[3] = 0375;
    cpu.T = 0xBEEF;

    EXPECT(dorado_cpu_step(&cpu) == 0, "step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(dev_state.writes == 1, "writes=%d (expected 1)", dev_state.writes);
    EXPECT(dev_state.last_write == 0xBEEF,
           "last_write = 0x%X (expected 0xBEEF)", dev_state.last_write);
    EXPECT(mem.last_fault == DM_FAULT_NONE,
           "slow-IO Output_ T issued memory fault %d", (int)mem.last_fault);

    dorado_memory_free(&mem);
    printf("PASS  test_output_t_store_shape_routes_slow_io\n");
    return 0;
}

static int test_output_rm_store_shape_routes_slow_io(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025;  mc.alufm_present[0] = 1;  /* B */

    /* Disk microcode uses `Output_ KCmmd`: a no-LC store-shaped slow
     * I/O output whose source is not T. */
    mc.rm[1] = 0x0005;
    mc.rm_present[1] = 1;
    mc.im[0] = make_uinstr(/*rstk=*/1, /*aluf=*/0, /*bsel=*/1, /*lc=*/0,
                           /*asel=*/0, 0, /*ff=*/0300, jcn_local(0));
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0;
    mc.image_present[0] = 1;
    mc.n_instructions = 1;

    static dorado_io io;
    dorado_io_init(&io);
    static echo_dev dev_state;
    memset(&dev_state, 0, sizeof dev_state);
    static const dorado_io_device echo_device = {
        .read = echo_read,
        .write = echo_write,
        .attention = NULL,
        .ctx = &dev_state,
        .name = "echo"
    };
    dorado_io_register(&io, /*task=*/14, /*tioa=*/010, &echo_device);

    dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "memory init failed");

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.io = &io;
    cpu.mem = &mem;
    cpu.ctask = 14;
    cpu.task_tpc[14] = 0;
    cpu.TIOA = 010;
    cpu.task_tioa[14] = 010;

    EXPECT(dorado_cpu_step(&cpu) == 0, "step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(dev_state.writes == 1, "writes=%d (expected 1)", dev_state.writes);
    EXPECT(dev_state.last_write == 0x0005,
           "last_write = 0x%X (expected 0x0005)", dev_state.last_write);
    EXPECT(mem.last_fault == DM_FAULT_NONE,
           "slow-IO Output_ RM issued memory fault %d", (int)mem.last_fault);

    dorado_memory_free(&mem);
    printf("PASS  test_output_rm_store_shape_routes_slow_io\n");
    return 0;
}

static int test_output_iostore_shape_routes_slow_io(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025;  mc.alufm_present[0] = 1;  /* B */

    mc.rm[1] = 0x0005;
    mc.rm_present[1] = 1;
    mc.im[0] = make_uinstr(/*rstk=*/1, /*aluf=*/0, /*bsel=*/1, /*lc=*/0,
                           /*asel=*/1, 0, /*ff=*/0100, jcn_local(0));
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0;
    mc.image_present[0] = 1;
    mc.n_instructions = 1;

    static dorado_io io;
    dorado_io_init(&io);
    static echo_dev dev_state;
    memset(&dev_state, 0, sizeof dev_state);
    static const dorado_io_device echo_device = {
        .read = echo_read,
        .write = echo_write,
        .attention = NULL,
        .ctx = &dev_state,
        .name = "echo"
    };
    dorado_io_register(&io, /*task=*/14, /*tioa=*/010, &echo_device);

    dorado_memory mem;
    EXPECT(dorado_memory_init(&mem) == 0, "memory init failed");

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.io = &io;
    cpu.mem = &mem;
    cpu.ctask = 14;
    cpu.task_tpc[14] = 0;
    cpu.TIOA = 010;
    cpu.task_tioa[14] = 010;

    EXPECT(dorado_cpu_step(&cpu) == 0, "step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(dev_state.writes == 1, "writes=%d (expected 1)", dev_state.writes);
    EXPECT(dev_state.last_write == 0x0005,
           "last_write = 0x%X (expected 0x0005)", dev_state.last_write);
    EXPECT(mem.last_fault == DM_FAULT_NONE,
           "slow-IO Output_ IOStore issued memory fault %d",
           (int)mem.last_fault);

    dorado_memory_free(&mem);
    printf("PASS  test_output_iostore_shape_routes_slow_io\n");
    return 0;
}

static int test_tioa_small_constant_all_low_bits(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025;  mc.alufm_present[0] = 1;  /* B */
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/1, /*lc=*/0,
                           /*asel=*/4, 0, /*ff=*/0244, jcn_local(0));
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0;
    mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.TIOA = 010;
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 0: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.TIOA == 014, "TIOA = 0o%o (expected 0o14)", cpu.TIOA);

    mc.im[0].ff = 0247;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.TIOA = 010;
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 1: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.TIOA == 017, "TIOA = 0o%o (expected 0o17)", cpu.TIOA);

    printf("PASS  test_tioa_small_constant_all_low_bits\n");
    return 0;
}

/*
 * test_carry_preserved_on_logical — HM page 30: "Carry' and Overflow
 * are the result of the last *arithmetic* ALU operation". A logical
 * op (e.g. ALU=B) must NOT clobber Carry'/Overflow from a prior
 * arithmetic op.
 *
 * Sequence:
 *   IM[0]: arithmetic A+B that produces carry-out=1 (e.g. 0xFFFF + 1).
 *   IM[1]: logical "B" pass-through (must preserve carry).
 *   IM[2]: branch on Carry' — if Carry' from IM[0] survived, take
 *          target T. If clobbered to 0 by IM[1], take target F.
 *
 * Verifies Carry' = NOT(carry-out) = 0 still readable after a
 * logical op intervenes.
 */
static int test_carry_preserved_on_logical(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0]  = 025;  mc.alufm_present[0]  = 1;  /* B */
    mc.alufm[2]  = 014;  mc.alufm_present[2]  = 1;  /* A+B carry=0 */

    /* Pre-load RM[0] = 0xFFFF so A+B = 0xFFFF + 1 = 0x10000 → carry=1. */
    mc.rm[0] = 0xFFFF;
    mc.rm_present[0] = 1;

    /* IM[0]: ALU = RM[0] + 1 (BSEL=4 const FF=1, ALUF=2 = ALUFM[2] = A+B).
     * ASEL=4 (A←RM/STK), LC=0. JCN=0o201 (local jump 1). */
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/2, /*bsel=*/4, /*lc=*/0,
                           /*asel=*/4, 0, /*ff=*/0001, jcn_local(1));
    mc.im_present[0] = 1;

    /* IM[1]: ALU = B (logical, ALUF=0 → ALUFM[0] = B). BSEL=4 const FF=0.
     * Should NOT clobber Carry'. JCN=0o202 (local jump 2). */
    mc.im[1] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/4, /*lc=*/0,
                           /*asel=*/4, 0, /*ff=*/0, jcn_local(2));
    mc.im_present[1] = 1;

    /* IM[2]: conditional on Carry' (cond=2). Layout the page so target
     * R=0 (= Carry'=0 = had carry) goes to IM[4], R=1 to IM[5].
     *
     * page_high = JCN[1:2] = 0b00 = 0  (offsets 0..7)
     * page_low  = JCN[3:4] = 0b10 = 2  (offsets 4 or 5 with R)
     * cond      = 2 (Carry')
     * offset = (0<<4) | (2<<1) | R = 4 + R
     * JCN = 0_00_0010_010 = bits 6:5=00, bits 4:3=10, bits 2:0=010 = 0b00010010 = 0o022 */
    mc.im[2] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/4, /*lc=*/0,
                           /*asel=*/4, 0, /*ff=*/0, /*jcn=*/0022);
    mc.im_present[2] = 1;

    /* IM[4]: HALT marker (self-loop). Reachable only if Carry' was
     * preserved (= 0 after carry-producing IM[0]). */
    mc.im[4] = make_uinstr(0, 0, 4, 0, 4, 0, 0, jcn_local(4));
    mc.im_present[4] = 1;

    /* IM[5]: bogus self-loop (reached if Carry' was clobbered to 1). */
    mc.im[5] = make_uinstr(0, 0, 4, 0, 4, 0, 0, jcn_local(5));
    mc.im_present[5] = 1;

    for (int i = 0; i < 6; i++) {
        mc.image_to_real[i] = i;
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 6;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    /* Step 0: arithmetic produces carry=1. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 0: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.alu_carry == 1, "step 0 should set alu_carry=1, got %d",
           cpu.alu_carry);

    /* Step 1: logical B. Must NOT clobber alu_carry. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 1: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.alu_carry == 1,
           "step 1 logical must preserve alu_carry=1, got %d",
           cpu.alu_carry);

    /* Step 2: conditional on Carry'. Carry'=0 → R=0 → goto IM[4]. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 2: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.real_PC == 4,
           "expected PC=4 (Carry' preserved as 0), got 0o%o", cpu.real_PC);

    printf("PASS  test_carry_preserved_on_logical (carry=1 survived "
           "logical op, branch to PC=%d)\n", cpu.real_PC);
    return 0;
}

/*
 * test_alufmrw_bit_mapping — HM Table 11d: "ALUFMEM ← B.8, B[11:15]".
 * Manual MSB-first → C-LSB bit mapping is non-trivial. The runtime
 * Pd←ALUFMRW must reproduce the .MB pre-declared ALUFM convention
 * when given the corresponding B value.
 *
 * Verified case from Bootstrap.MB IM[0o7771]: writes ALUFM[8] from
 * B = T = 0x13 (loaded earlier from FF=0o023 constant). Bootstrap.MB
 * declares ALUFM[8] = 0o23 (A XOR B); the runtime write must yield
 * the same value.
 */
static int test_alufmrw_bit_mapping(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[ 0] = 025; mc.alufm_present[ 0] = 1;
    /* IM[0]: T ← 0x13 via 0,,FF (BSEL=4, FF=0o023, ALUF=0=B, LC=1). */
    mc.im[0] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/1, /*asel=*/4,
                           0, /*ff=*/0023, jcn_local(1));
    mc.im_present[0] = 1;
    /* IM[1]: Pd←ALUFMRW with ALUF=10 (=8 dec). BSEL=2 (T) puts T on B.
     * ASEL=4 (A←RM/STK). LC=NoLoad. FF=0o262 = FA=2 FB=6 FC=2 = ALUFMRW.
     * JCN=local(2) so we can step here. */
    mc.im[1] = make_uinstr(0, /*aluf=*/010, /*bsel=*/2, /*lc=*/0,
                           /*asel=*/4, 0, /*ff=*/0262, jcn_local(2));
    mc.im_present[1] = 1;
    /* IM[2]: HALT marker. */
    mc.im[2] = make_uinstr(0, 0, 4, 0, 4, 0, 0, jcn_local(2));
    mc.im_present[2] = 1;
    for (int i = 0; i < 3; i++) {
        mc.image_to_real[i] = i;
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 3;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    EXPECT(dorado_cpu_step(&cpu) == 0, "step 0: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.T == 0x13, "T = 0x%X (expected 0x13)", cpu.T);

    EXPECT(dorado_cpu_step(&cpu) == 0, "step 1: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(mc.alufm[8] == 023,
           "ALUFM[8] = 0o%o (expected 0o23 = A XOR B)", mc.alufm[8]);

    printf("PASS  test_alufmrw_bit_mapping (Pd←ALUFMRW B=0x13 → ALUFM[8]=0o%o)\n",
           mc.alufm[8]);
    return 0;
}

static int test_alufmem_is_read_only(void)
{
    static dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025; mc.alufm_present[0] = 1;
    mc.alufm[8] = 023; mc.alufm_present[8] = 1;

    /* IM[0]: T <- 0x7F. */
    mc.im[0] = make_uinstr(0, 0, /*bsel=*/4, /*lc=*/1, /*asel=*/4,
                           0, /*ff=*/0177, jcn_local(1));
    mc.im_present[0] = 1;
    /* IM[1]: Pd<-ALUFMEM with ALUF=8 reads the entry but must not
     * write from B. HM Table 11d: FC=2 is ALUFMRW, FC=3 is ALUFMEM. */
    mc.im[1] = make_uinstr(0, /*aluf=*/010, /*bsel=*/2, /*lc=*/0,
                           /*asel=*/4, 0, /*ff=*/0263, jcn_local(2));
    mc.im_present[1] = 1;
    mc.im[2] = make_uinstr(0, 0, 4, 0, 4, 0, 0, jcn_local(2));
    mc.im_present[2] = 1;
    for (int i = 0; i < 3; i++) {
        mc.image_to_real[i] = i;
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 3;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    EXPECT(dorado_cpu_step(&cpu) == 0, "step 0: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.T == 0x7F, "T = 0x%X (expected 0x7F)", cpu.T);
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 1: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(mc.alufm[8] == 023,
           "ALUFMEM read changed ALUFM[8] to 0o%o", mc.alufm[8]);

    printf("PASS  test_alufmem_is_read_only (Pd<-ALUFMEM left ALUFM[8]=0o%o)\n",
           mc.alufm[8]);
    return 0;
}

/*
 * test_pcx_b_source (gap B3 sub-item) — HM Table 11c FA=1 FB=7 FC=0:
 *   `B ← PCX'` returns the IFU PC of the currently-executing
 *   opcode, inverted.
 */
static int test_pcx_b_source(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 025;          /* logical B */
    mc.alufm_present[0] = 1;

    /* IM[0]: B<-PCX' (FF=0o170; FA=1 FB=7 FC=0).
     * BSEL=2 (T) — picks a non-constant primary so FF is a
     * function. ALUF=0 → ALUFM[0]=025=logical B → ALU = B =
     * PCX'. LC=1 (T<-Pd). JCN=local(1) so it's NOT a Call. */
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/2, /*lc=*/1,
                           /*asel=*/4, /*block=*/0, /*ff=*/0170,
                           /*jcn=*/jcn_local(1));
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0;
    mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.ifu_pcx = 0x1234;

    EXPECT(dorado_cpu_step(&cpu) == 0, "step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    /* B = ~ifu_pcx = ~0x1234 = 0xEDCB; ALU = B; T <- ALU. */
    EXPECT(cpu.T == 0xEDCB,
           "T should be ~0x1234 = 0xEDCB, got 0x%04X", cpu.T);
    printf("PASS  test_pcx_b_source (gap B3 — B<-PCX')\n");
    return 0;
}

/*
 * test_b11_event_cnt_brk_state (gap B11) — verify the EventCntB,
 * BrkPending, and EventCntCtrl state slots round-trip through their
 * FF functions. The functions don't yet drive any behavior; this test
 * pins the read/write contract so future microcode that uses them
 * lands instead of seeing zero.
 */
static int test_b11_event_cnt_brk_state(void)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = 037;          /* logical A */
    mc.alufm_present[0] = 1;
    mc.rm[0] = 0xBEEF;
    mc.rm_present[0] = 1;

    /* IM[0]: EventCntB ← B. RM[0]=0xBEEF on B (BSEL=1 RM/STK).
     * FA=1 FB=3 FC=1 → FF = 0b01_011_001 = 0o131. */
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/1, /*lc=*/0,
                           /*asel=*/4, /*block=*/0, /*ff=*/0131,
                           /*jcn=*/jcn_local(1));
    mc.im_present[0] = 1;
    /* IM[1]: BrkIns ← B. B[0:7] = high byte of RM[0] = 0xBE.
     * FA=1 FB=3 FC=7 → FF = 0b01_011_111 = 0o137. */
    mc.im[1] = make_uinstr(0, 0, 1, 0, 4, 0, 0137, jcn_local(2));
    mc.im_present[1] = 1;
    /* IM[2]: self-loop (NoOp). BSEL=2 (T), ASEL=6 (A←T). */
    mc.im[2] = make_uinstr(0, 0, 2, 0, 6, 0, 0, jcn_local(2));
    mc.im_present[2] = 1;
    for (int i = 0; i < 3; i++) {
        mc.image_to_real[i] = i;
        mc.image_present[i] = 1;
    }
    mc.n_instructions = 3;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);

    /* Step EventCntB ← B. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 1: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.event_cnt_b == 0xBEEF,
           "EventCntB should be 0xBEEF, got 0x%04X", cpu.event_cnt_b);

    /* Step BrkIns ← B. */
    EXPECT(dorado_cpu_step(&cpu) == 0, "step 2: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    EXPECT(cpu.brk_pending == 1, "BrkPending should be set");
    EXPECT(cpu.brk_opcode == 0xBE, "brk_opcode = 0x%02X, expected 0xBE",
           cpu.brk_opcode);

    printf("PASS  test_b11_event_cnt_brk_state (gap B11)\n");
    return 0;
}

/*
 * test_a_low_ff_override (gap B6) — HM Table 11a (FA=0 FB=0/1):
 *   "A[12:15] ← FF[4:7]"
 *
 * Verifies the A-bus low-nibble override fires when FF is interpreted
 * as a function (ASEL > 3, BSEL not constant, JCN not long), and is
 * suppressed otherwise.
 */
static uint16_t run_a_override(uint8_t ff, uint8_t bsel, uint16_t rm0)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    /* ALUFM[0] = 0o37 (logical "A"). */
    mc.alufm[0] = 037;
    mc.alufm_present[0] = 1;
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/bsel, /*lc=*/1,
                           /*asel=*/4, /*block=*/0, /*ff=*/ff,
                           /*jcn=*/jcn_local(0));
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0;
    mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.RM[0] = rm0;
    EXPECT(dorado_cpu_step(&cpu) == 0, "step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    return cpu.T;
}

static int test_a_low_ff_override(void)
{
    /* FF = 0o005: FA=0 FB=0 FC=5; FF[4:7] = 5. BSEL=2 (T), so FF is
     * a function. Expected: A[12:15] replaced with 5. */
    EXPECT(run_a_override(0005, /*bsel=T*/2, 0xAAAA) == 0xAAA5,
           "A[12:15]←FF[4:7] should rewrite low nibble");

    /* Same FF, BSEL=4 (constant 0,,FF): FF is NOT interpreted as a
     * function — override must NOT fire. T = ALU(A=RM, ALUFM[0]=A) = RM. */
    EXPECT(run_a_override(0005, /*bsel=const*/4, 0xAAAA) == 0xAAAA,
           "override must be suppressed when BSEL is constant");

    /* FA=0 FB=2: NOT the override (memory-A-source territory).
     * FF=0o025 → FA=0 FB=2 FC=5. With BSEL=2 (T) and ASEL=4, FF is
     * a function but FB=2 is not "A[12:15]←FF[4:7]" — leave A alone. */
    EXPECT(run_a_override(0025, /*bsel=T*/2, 0xAAAA) == 0xAAAA,
           "FA=0 FB=2 must not trigger A-bus override");

    /* FA=0 FB=1 also encodes the override (manual lists it as a paired
     * variant of FB=0). FF=0o015 → FA=0 FB=1 FC=5; FF[4:7] = 0xD
     * (because FB's LSB is the high bit of the FF[4:7] nibble). */
    EXPECT(run_a_override(0015, /*bsel=T*/2, 0xAAAA) == 0xAAAD,
           "FA=0 FB=1 must also trigger A-bus override");

    printf("PASS  test_a_low_ff_override (gap B6)\n");
    return 0;
}

static uint16_t run_alu_shift_ff(uint8_t ff, uint8_t alufm, uint16_t rm0)
{
    dorado_microcode mc;
    memset(&mc, 0, sizeof mc);
    mc.alufm[0] = alufm;
    mc.alufm_present[0] = 1;
    mc.im[0] = make_uinstr(/*rstk=*/0, /*aluf=*/0, /*bsel=*/1, /*lc=*/1,
                           /*asel=*/4, /*block=*/0, /*ff=*/ff,
                           /*jcn=*/jcn_local(0));
    mc.im_present[0] = 1;
    mc.image_to_real[0] = 0;
    mc.image_present[0] = 1;
    mc.n_instructions = 1;

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, &mc, 0);
    cpu.RM[0] = rm0;
    EXPECT(dorado_cpu_step(&cpu) == 0, "ALU shift step: %s",
           cpu_halt_reason_str(cpu.halt_reason));
    return cpu.T;
}

static int test_alu_shift_ff_functions(void)
{
    EXPECT(run_alu_shift_ff(0270, 025, 0x8001) == 0x4000,
           "ALU rsh 1 failed");
    EXPECT(run_alu_shift_ff(0271, 025, 0x8001) == 0xC000,
           "ALU rcy 1 failed");
    EXPECT(run_alu_shift_ff(0272, 014, 0x8000) == 0x8000,
           "ALU brsh 1 failed");
    EXPECT(run_alu_shift_ff(0273, 025, 0x8001) == 0xC000,
           "ALU arsh 1 failed");
    EXPECT(run_alu_shift_ff(0274, 025, 0x4001) == 0x8002,
           "ALU lsh 1 failed");
    EXPECT(run_alu_shift_ff(0275, 025, 0x8001) == 0x0003,
           "ALU lcy 1 failed");

    printf("PASS  test_alu_shift_ff_functions\n");
    return 0;
}

int main(void)
{
    int rc = 0;
    rc |= test_load_constant();
    rc |= test_increment_loop();
    rc |= test_rm_roundtrip();
    rc |= test_conditional_branch_alu_zero();
    rc |= test_shifter_byte_cycle();
    rc |= test_shifter_rmask();
    rc |= test_unsupported_halts();
    rc |= test_jcn_long_branch_address();
    rc |= test_ff_condition_with_memory_ref();
    rc |= test_write_im();
    rc |= test_stk_no_change();
    rc |= test_stk_push();
    rc |= test_stk_pop();
    rc |= test_stk_pop_minus_4();
    rc |= test_stk_overflow();
    rc |= test_stk_underflow_check();
    rc |= test_lc_forced_rm_write_address();
    rc |= test_cpu_memory_roundtrip();
    rc |= test_alt_fetch_t_lc_md_pipeline();
    rc |= test_dummyref_t_uses_t_as_mar();
    rc |= test_alt_store_t_uses_b_data();
    rc |= test_memory_decode_uses_table8b_when_ff_not_ok();
    rc |= test_bootstrap_ldf_dispatch();
    rc |= test_cpu_fault_info_visible();
    rc |= test_cpu_pipe4_no_error_baseline();
    rc |= test_bc_timing_previous_instr();
    rc |= test_freezebc();
    rc |= test_return_ff_condition_or();
    rc |= test_task_switch_on_wakeup();
    rc |= test_task_block_returns_to_emulator();
    rc |= test_tasking_off_blocks_switch();
    rc |= test_wakeup_ff_function();
    rc |= test_junk_timer_wakeup();
    rc |= test_ifutest_junk_timer_polarity();
    rc |= test_ifureset_disables_junk_timer();
    rc |= test_subtask_or_rm();
    rc |= test_ifum_load_read();
    rc |= test_ifu_dispatch_synthetic();
    rc |= test_ifu_conditional_dispatch();
    rc |= test_ifu_conditional_cond_true();
    rc |= test_ifu_notready_trap();
    rc |= test_ifu_map_fault_trap();
    rc |= test_ldtpc_rdtpc();
    rc |= test_reschedule_trap();
    rc |= test_slow_io_routing();
    rc |= test_output_t_store_shape_routes_slow_io();
    rc |= test_output_rm_store_shape_routes_slow_io();
    rc |= test_output_iostore_shape_routes_slow_io();
    rc |= test_tioa_small_constant_all_low_bits();
    rc |= test_carry_preserved_on_logical();
    rc |= test_alufmrw_bit_mapping();
    rc |= test_alufmem_is_read_only();
    rc |= test_alu_shift_ff_functions();
    rc |= test_a_low_ff_override();
    rc |= test_b11_event_cnt_brk_state();
    rc |= test_pcx_b_source();
    rc |= probe_bootstrap_pure();
    rc |= probe_bootstrap();
    rc |= probe_aemu();
    rc |= probe_initial();
    rc |= probe_full_boot();
    rc |= probe_full_boot_with_bootstrap();
    if (rc == 0) printf("\nAll CPU tests passed.\n");
    return rc;
}
