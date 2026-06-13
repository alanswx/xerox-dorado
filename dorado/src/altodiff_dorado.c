/*
 * altodiff-dorado — execute ONE Alto (Nova-family) opcode on the real
 * AEmu microcode and print the resulting architectural state. This is
 * the "device under test" half of a differential harness whose oracle
 * is a reference Alto emulator (salto). Diffing per-opcode state across
 * a sweep of opcodes/operands localizes Alto-opcode emulation bugs at
 * the instruction level, instead of by 90M-cycle boot forensics.
 *
 *   altodiff-dorado            run the built-in validation vectors
 *   altodiff-dorado OP AC0 AC1 AC2 AC3 CRY   run one vector (octal)
 *
 * Output line per vector:
 *   OP=<o> in: AC=<o,o,o,o> CRY=<0|1> -> AC=<o,o,o,o> CRY=<0|1> PC=<o> WR=<va:val,...> [status]
 *
 * --- AEmu register map (chm/doradosource/AEmuSources-cedar6.0.dm!1_) ---
 * AEmRegs = RM region 1 = RM[0x10..0x1F] (confirmed: EmuBRHiReg=RM[0x18]).
 *   Cry        = RM[0x11]  (Alto carry; all-0 or all-1)
 *   RCode      = RM[0x12]  (low 16 bits of IFU code base)
 *   spAC0..3   = RM[0x14..0x17] (stack-pointer values 1..4)
 *   EmuBRHiReg = RM[0x18]  (high half of MDS / the 64K Alto bank)
 * Accumulators AC0-AC3 live in STK[1..4] (Start.mc; ADefs nspAC0..3=1..4).
 * MemBase MDS = base register 0o36; Code = 0o37 (ADefs BR[MDS,36]/BR[Code,37]).
 *
 * --- IFU priming (AEmu Start.mc "Start:" cold-start) ---
 * Start: takes the Alto PC (word address) in T, sets RBase=AEmRegs, the
 * IFU instruction set (AEmuInsSet=0), RCode=T, loads IFU code base 0o37 =
 * EmuBRHiReg:RCode, PCF=0, RestoreALUFM (self-contained: ALUFM[15],[17]),
 * then IFUJump[0]. We pre-seed STK[1..4]/Cry/EmuBRHiReg, set the MDS data
 * base 0o36 = EmuBRHiReg<<16 (what SetupBRs would do), place the Nova word
 * in the MDS bank, enter at Start, and step until ifu_dispatch_count
 * reaches 2 (opcode-1's handler finished and is dispatching opcode-2),
 * so the captured state reflects exactly one executed opcode.
 */

#include "mb.h"
#include "microcode.h"
#include "cpu.h"
#include "memory.h"
#include "io.h"
#include "altoref.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* AEmRegs RM indices. */
enum {
    RM_CRY        = 0x11,
    RM_RCODE      = 0x12,
    RM_SPAC0      = 0x14,
    RM_EMUBRHI    = 0x18,
};
#define MEMBASE_MDS   036
#define MEMBASE_CODE  037
#define EMU_BANK      1               /* EmuBRHiReg: MDS at page 256 */
#define CODE_WORD     01000           /* Alto PC word addr for the program */

static const char *LAYERS[] = {
    "../chm/dorado/expanded/bootstrap.dm!20_/Initial.mb",
    "../chm/dorado/expanded/kernel.dm!38_/kernel.mb",
    "../chm/dorado/expanded/memMisc.dm!11_/memMisc.mb",
    "../chm/dorado/expanded/Ifu.dm!51_/IfuComplex.mb",
    "../chm/dorado/AEmu.mb!2",
};
#define N_LAYERS 5

/* Coherent store at a VA (storage + identity phys + matching cache line),
 * mirroring machine.c machine_store_va so the emulator reads what we wrote. */
static void store_va(dorado_memory *mem, uint32_t va, uint16_t value)
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

/* Read coherently: a matching valid cache line wins over storage, since
 * the emulator's stores land in the cache before writeback. */
static uint16_t load_va(dorado_memory *mem, uint32_t va)
{
    uint32_t row = (va >> 4) & DM_CACHE_ROW_MASK;
    uint32_t tag = va >> 10;
    uint32_t off = va & DM_CACHE_LINE_MASK;
    for (int way = 0; way < DM_CACHE_WAYS; way++) {
        const dorado_cache_line *line = &mem->cache[row].ways[way];
        if (line->valid && line->tag == tag) return line->data[off];
    }
    uint32_t idx = dorado_map_index(va);
    const dorado_map_entry *e = dorado_map_get(mem, idx);
    size_t phys = (size_t)e->rp * DM_PAGE_SIZE + (va & (DM_PAGE_SIZE - 1));
    return (phys < mem->storage_words) ? mem->storage[phys] : 0;
}

/* Real address of an image symbol in a given layer's .MB (awd = storage
 * word 3, low 12 bits), the way probe_aemu derives it. */
static int real_of_symbol(mb_file *mb, const char *name)
{
    int im_id = mb_find_mem(mb, "IM");
    if (im_id < 0) return -1;
    int img = mb_find_symbol_addr(mb, im_id, name);
    if (img < 0) return -1;
    const mb_memory *m = &mb->mems[im_id];
    if (!m->present[img]) return -1;
    return m->data[(size_t)img * m->width_words + 3] & 0xFFF;
}

struct result {
    uint16_t ac[4];
    int      cry;
    uint16_t pc;
    uint16_t pcx;          /* IFU PCX at the 2nd dispatch (for skip)   */
    int      ok;            /* 1 = executed one opcode cleanly */
    const char *status;     /* "" or a fault/halt note */
    /* memory writes captured against a snapshot window */
    uint32_t wr_va[8];
    uint16_t wr_val[8];
    int      n_wr;
};

/* Run one opcode. mc must be the loaded layer stack; start_real = Start.
 * pz (256 words) seeds page-zero memory before the opcode (NULL = all 0). */
static struct result run_one(dorado_microcode *mc, int start_real,
                             dorado_memory *mem, dorado_io *io,
                             uint16_t opcode, uint16_t next_op,
                             const uint16_t ac[4], int cry,
                             const uint16_t *pz)
{
    struct result r;
    memset(&r, 0, sizeof r);
    r.status = "";

    /* Reset memory state we touch (the MDS bank low window). */
    uint32_t bank = (uint32_t)EMU_BANK << 16;
    for (uint32_t w = 0; w < 0x400; w++) store_va(mem, bank | w, 0);
    if (pz)
        for (uint32_t w = 0; w < 0400; w++) store_va(mem, bank | w, pz[w]);

    /* Program: opcode under test at CODE_WORD, a benign follow-on so the
     * handler has a second opcode to dispatch (we stop before it runs). */
    store_va(mem, bank | CODE_WORD, opcode);
    store_va(mem, bank | (CODE_WORD + 1), next_op);

    /* Snapshot the data window to detect writes. */
    static uint16_t snap[0x400];
    for (uint32_t w = 0; w < 0x400; w++) snap[w] = load_va(mem, bank | w);

    dorado_cpu cpu;
    dorado_cpu_init(&cpu, mc, (uint16_t)start_real);
    cpu.mem = mem;
    cpu.io  = io;
    cpu.tasking_on = 0;
    cpu.wakeup_pending = 0;
    cpu.reschedule_pending = 0;
    cpu.ifu_active = 0;
    cpu.ifu_warmup = 0;

    /* Seed AEmu register state. */
    cpu.RM[RM_EMUBRHI] = EMU_BANK;
    cpu.RM[RM_CRY]     = cry ? 0xFFFF : 0x0000;
    cpu.RM[RM_RCODE]   = CODE_WORD;
    cpu.RM[RM_SPAC0 + 0] = 1;
    cpu.RM[RM_SPAC0 + 1] = 2;
    cpu.RM[RM_SPAC0 + 2] = 3;
    cpu.RM[RM_SPAC0 + 3] = 4;
    cpu.STK[1] = ac[0];
    cpu.STK[2] = ac[1];
    cpu.STK[3] = ac[2];
    cpu.STK[4] = ac[3];
    cpu.T = CODE_WORD;             /* Start: cold-start PC in T */

    /* MDS data base 0o36 = EmuBRHiReg<<16 (what SetupBRs establishes). */
    dorado_br_hi_load(mem, MEMBASE_MDS, EMU_BANK);
    dorado_br_lo_load(mem, MEMBASE_MDS, 0);

    cpu.ifu_dispatch_count = 0;
    int trace = getenv("ALTODIFF_TRACE") != NULL;
    int steps = 0, cap = 4000;
    cpu_halt_reason hr = CPU_HALT_NONE;
    for (; steps < cap; steps++) {
        if (cpu.ifu_dispatch_count >= 2) break;   /* one opcode done */
        if (trace)
            fprintf(stderr, "  step%-3d PC=0o%-5o tk=%d disp=%llu ifu(act=%d wu=%d op=0o%o pcf=0o%o)\n",
                    steps, cpu.real_PC, cpu.ctask,
                    (unsigned long long)cpu.ifu_dispatch_count,
                    cpu.ifu_active, cpu.ifu_warmup, cpu.ifu_opcode, cpu.ifu_pcf);
        hr = dorado_cpu_step(&cpu);
        if (cpu.halted) break;
    }

    if (cpu.ifu_dispatch_count < 2) {
        r.ok = 0;
        r.status = cpu.halted ? "HALT" : "NO-DISPATCH";
        (void)hr;
    } else {
        r.ok = 1;
    }

    r.ac[0] = cpu.STK[1];
    r.ac[1] = cpu.STK[2];
    r.ac[2] = cpu.STK[3];
    r.ac[3] = cpu.STK[4];
    r.cry   = (cpu.RM[RM_CRY] & 1);
    /* PC word = RCODE + (~PCX'>>1); approximate via RCODE for now. */
    r.pc    = cpu.RM[RM_RCODE];
    r.pcx   = cpu.ifu_pcx;

    for (uint32_t w = 0; w < 0x400 && r.n_wr < 8; w++) {
        uint16_t now = load_va(mem, bank | w);
        if (now != snap[w] && w != CODE_WORD && w != CODE_WORD + 1) {
            r.wr_va[r.n_wr]  = w;
            r.wr_val[r.n_wr] = now;
            r.n_wr++;
        }
    }
    return r;
}

static void print_result(uint16_t op, const uint16_t in_ac[4], int in_cry,
                         const struct result *r)
{
    printf("OP=%06o in:AC=%06o,%06o,%06o,%06o CRY=%d -> "
           "AC=%06o,%06o,%06o,%06o CRY=%d PC=%06o WR=",
           op, in_ac[0], in_ac[1], in_ac[2], in_ac[3], in_cry,
           r->ac[0], r->ac[1], r->ac[2], r->ac[3], r->cry, r->pc);
    for (int i = 0; i < r->n_wr; i++)
        printf("%s%04o:%06o", i ? "," : "", r->wr_va[i], r->wr_val[i]);
    if (!r->ok) printf(" [%s]", r->status);
    printf("\n");
}

/* Compare a Dorado result against the reference; report + count mismatches.
 * Compares AC0-3, carry, and the single memory write. */
static int diff_one(const char *label, uint16_t op, const uint16_t ac[4],
                    int cry, const struct result *d,
                    const alto_ref_result *ref)
{
    int bad = 0;
    int d_skip = ((int)(d->pcx >> 1)) - 1;   /* pcx=2 -> 0, pcx=4 -> 1 */
    for (int i = 0; i < 4; i++) if (d->ac[i] != ref->ac[i]) bad = 1;
    if ((d->cry & 1) != (ref->cry & 1)) bad = 1;
    if (d->ok && d_skip != ref->skip) bad = 1;
    if (ref->wrote) {
        int found = 0;
        for (int k = 0; k < d->n_wr; k++)
            if (d->wr_va[k] == ref->wr_addr && d->wr_val[k] == ref->wr_val)
                found = 1;
        if (!found) bad = 1;
    } else if (d->n_wr != 0) {
        bad = 1;
    }
    if (!d->ok) bad = 1;
    if (bad) {
        printf("MISMATCH %-10s OP=%06o in AC=%06o,%06o,%06o,%06o CRY=%d\n",
               label, op, ac[0], ac[1], ac[2], ac[3], cry);
        printf("   dorado AC=%06o,%06o,%06o,%06o CRY=%d WR=",
               d->ac[0], d->ac[1], d->ac[2], d->ac[3], d->cry);
        for (int k = 0; k < d->n_wr; k++)
            printf("%s%04o:%06o", k ? "," : "", d->wr_va[k], d->wr_val[k]);
        printf(" SKIP=%d", d_skip);
        if (!d->ok) printf(" [%s]", d->status);
        printf("\n   ref    AC=%06o,%06o,%06o,%06o CRY=%d WR=",
               ref->ac[0], ref->ac[1], ref->ac[2], ref->ac[3], ref->cry);
        if (ref->wrote) printf("%04o:%06o", ref->wr_addr, ref->wr_val);
        printf(" SKIP=%d", ref->skip);
        printf("\n");
    }
    return bad ? 1 : 0;
}

/* Sweep the ALC matrix + memory-reference opcodes on both the real AEmu
 * microcode and the spec reference; report divergences. */
static int run_sweep(dorado_microcode *mc, int start_real,
                     dorado_memory *mem, dorado_io *io)
{
    int total = 0, bad = 0;
    /* Skip is detected from the IFU PCX of the next opcode: pcx=2 (one
     * word, no skip) vs pcx=4 (two words, skipped) -- see diff_one. */
    static const uint16_t opsets[][4] = {
        { 0, 0, 0, 0 },
        { 1, 0177777, 0100000, 077777 },
        { 052525, 0125252, 1, 0177777 },
        { 0, 1, 0177777, 2 },
        { 7, 7, 0, 0 },
    };
    int nset = (int)(sizeof opsets / sizeof opsets[0]);

    /* ALC: acs=1, acd=2; sweep func/sh/cy/nl/cry_in; skip=0 (skip changes
     * PC only, which run_one does not track precisely). */
    for (int func = 0; func < 8; func++)
    for (int si = 0; si < nset; si++)
    for (int sh = 0; sh < 4; sh++)
    for (int cyf = 0; cyf < 4; cyf++)
    for (int nl = 0; nl < 2; nl++)
    for (int sk = 0; sk < 8; sk++)
    for (int cin = 0; cin < 2; cin++) {
        uint16_t op = (uint16_t)(0100000u | (1u << 13) | (2u << 11) |
                                 ((unsigned)func << 8) | ((unsigned)sh << 6) |
                                 ((unsigned)cyf << 4) | ((unsigned)nl << 3) |
                                 (unsigned)sk);
        const uint16_t *ac = opsets[si];
        struct result d = run_one(mc, start_real, mem, io, op, 0, ac, cin, NULL);
        uint16_t m[256]; memset(m, 0, sizeof m);
        alto_ref_result ref = alto_ref_exec(op, ac, cin, m, (uint16_t)CODE_WORD);
        total++;
        bad += diff_one("ALC", op, ac, cin, &d, &ref);
    }

    /* Memory-reference: STA/LDA/ISZ/DSZ at page-zero 0o100 across data
     * values; the data cell is preloaded (shared with the reference). */
    static const uint16_t datavals[] = { 0, 1, 0177777, 0100000, 052525, 0177776 };
    int ndv = (int)(sizeof datavals / sizeof datavals[0]);
    struct { const char *name; uint16_t op; int loads_ac1; } mri[] = {
        { "STA1",  (uint16_t)((2u << 13) | (1u << 11) | 0100), 0 },
        { "LDA1",  (uint16_t)((1u << 13) | (1u << 11) | 0100), 1 },
        { "ISZ",   (uint16_t)((2u << 11) | 0100), 0 },
        { "DSZ",   (uint16_t)((3u << 11) | 0100), 0 },
    };
    for (int mi = 0; mi < 4; mi++)
    for (int dv = 0; dv < ndv; dv++) {
        uint16_t ac[4] = { 0, 012345, 0, 0 };
        uint16_t pz[256]; memset(pz, 0, sizeof pz);
        pz[0100] = datavals[dv];     /* the data cell ISZ/DSZ/LDA read */
        struct result d = run_one(mc, start_real, mem, io, mri[mi].op, 0,
                                  ac, 0, pz);
        uint16_t m[256]; memset(m, 0, sizeof m);
        m[0100] = datavals[dv];
        alto_ref_result ref = alto_ref_exec(mri[mi].op, ac, 0, m,
                                            (uint16_t)CODE_WORD);
        total++;
        bad += diff_one(mri[mi].name, mri[mi].op, ac, 0, &d, &ref);
    }

    /* Indirect page-zero (I=1): disp points to a page-zero cell holding
     * the target address (one level; pointer bit 0 = 0 so no re-indirect).
     * The handoff's prime suspect: dsz/isz @indirect. */
    for (int mi = 0; mi < 4; mi++)
    for (int dv = 0; dv < ndv; dv++) {
        uint16_t op = (uint16_t)(mri[mi].op | (1u << 10));   /* set I bit */
        uint16_t ac[4] = { 0, 012345, 0, 0 };
        uint16_t pz[256]; memset(pz, 0, sizeof pz);
        pz[0100] = 0150;             /* pointer -> page-zero target 0o150 */
        pz[0150] = datavals[dv];     /* the actual data cell             */
        struct result d = run_one(mc, start_real, mem, io, op, 0, ac, 0, pz);
        uint16_t m[256]; memset(m, 0, sizeof m);
        m[0100] = 0150; m[0150] = datavals[dv];
        alto_ref_result ref = alto_ref_exec(op, ac, 0, m, (uint16_t)CODE_WORD);
        total++;
        char lbl[16]; snprintf(lbl, sizeof lbl, "%s@", mri[mi].name);
        bad += diff_one(lbl, op, ac, 0, &d, &ref);
    }

    /* Indexed addressing (X=2 AC2-relative, X=3 AC3-relative), direct and
     * indirect. The index AC is set so the effective address lands in
     * page zero. Covers the "jsr @disp,2" indirect-indexed suspect (here
     * exercised via LDA/STA/ISZ/DSZ so the data effect is checkable).
     * Cases: (index_ac_val, disp) -- includes a negative (sign-extended)
     * displacement. */
    struct { int x; uint16_t idxval; uint16_t disp; } idxcase[] = {
        { 2, 0100, 0 }, { 2, 0040, 0040 }, { 2, 0200, 0377 /* -1 -> 0177 */ },
        { 3, 0100, 0 }, { 3, 0060, 0020 },
    };
    int nidx = (int)(sizeof idxcase / sizeof idxcase[0]);
    for (int mi = 0; mi < 4; mi++)
    for (int ic = 0; ic < nidx; ic++)
    for (int ind = 0; ind < 2; ind++)
    for (int dv = 0; dv < ndv; dv++) {
        int x = idxcase[ic].x;
        uint16_t disp = idxcase[ic].disp;
        uint16_t mop = mri[mi].op & ~0xFFu;          /* strip page-zero disp */
        mop = (uint16_t)(mop | ((unsigned)x << 8) | (ind << 10) | disp);
        uint16_t ac[4] = { 0, 012345, 0, 0 };
        ac[x] = idxcase[ic].idxval;                  /* the index AC      */
        /* effective (pre-indirect) address, page zero */
        uint16_t sext = (disp > 0200) ? (uint16_t)(disp + 0177400) : disp;
        uint16_t ea = (uint16_t)((idxcase[ic].idxval + sext) & 0377);
        uint16_t pz[256]; memset(pz, 0, sizeof pz);
        uint16_t target = ea;
        if (ind) { pz[ea] = 0160; target = 0160; }   /* pointer -> 0o160 */
        pz[target] = datavals[dv];
        struct result d = run_one(mc, start_real, mem, io, mop, 0, ac, 0, pz);
        uint16_t m[256]; memset(m, 0, sizeof m);
        if (ind) m[ea] = 0160;
        m[target] = datavals[dv];
        alto_ref_result ref = alto_ref_exec(mop, ac, 0, m, (uint16_t)CODE_WORD);
        total++;
        char lbl[16];
        snprintf(lbl, sizeof lbl, "%s%s%d", mri[mi].name, ind ? "@" : ",", x);
        bad += diff_one(lbl, mop, ac, 0, &d, &ref);
    }

    printf("sweep: %d vectors, %d mismatches\n", total, bad);
    return bad ? 1 : 0;
}

int main(int argc, char **argv)
{
    static mb_file mbs[N_LAYERS];
    for (int i = 0; i < N_LAYERS; i++) {
        mb_init(&mbs[i]);
        if (mb_load(&mbs[i], LAYERS[i]) != MB_OK) {
            printf("SKIP altodiff-dorado (%s not loadable)\n", LAYERS[i]);
            return 0;
        }
    }
    static dorado_microcode mc;
    if (dorado_microcode_load(&mbs[0], &mc) != DM_OK) {
        printf("SKIP altodiff-dorado (layer 0 load failed)\n");
        return 0;
    }
    for (int i = 1; i < N_LAYERS; i++) {
        if (dorado_microcode_layer_load(&mbs[i], &mc) != DM_OK) {
            printf("SKIP altodiff-dorado (layer %d load failed)\n", i);
            return 0;
        }
    }

    int start_real = real_of_symbol(&mbs[N_LAYERS - 1], "START");
    if (start_real < 0) {
        printf("SKIP altodiff-dorado (START symbol not found)\n");
        return 0;
    }
    fprintf(stderr, "START real=0o%o\n", start_real);

    /* Diagnostic: dump IFUM entries for an opcode range (insset 0). */
    if (argc >= 2 && !strcmp(argv[1], "ifum")) {
        for (int op = 0; op < 256; op++) {
            if (mc.ifum_present[op])
                printf("IFUM[%03o] present lo=%06o hi=%06o\n",
                       op, mc.ifum_lo[op], mc.ifum_hi[op]);
        }
        return 0;
    }

    static dorado_memory mem;
    if (dorado_memory_init(&mem) != 0) {
        printf("SKIP altodiff-dorado (mem init failed)\n");
        return 0;
    }
    for (uint32_t pg = 0; pg < 512; pg++)
        dorado_map_set(&mem, pg, (uint16_t)pg, 0, 0);

    static dorado_io io;
    dorado_io_init(&io);

    /* Follow-on opcode: 0o1000 = JMP 0 page-zero? Use a harmless ALC NOP:
     * MOV 0,0 with no-load (= 0o101000 | nl) — actually any opcode works
     * since we stop before its handler runs. Use 0 (interpreted lazily). */
    uint16_t next_op = 0;

    if (argc >= 7) {
        uint16_t op = (uint16_t)strtol(argv[1], NULL, 8);
        uint16_t ac[4] = {
            (uint16_t)strtol(argv[2], NULL, 8),
            (uint16_t)strtol(argv[3], NULL, 8),
            (uint16_t)strtol(argv[4], NULL, 8),
            (uint16_t)strtol(argv[5], NULL, 8),
        };
        int cry = (int)strtol(argv[6], NULL, 8) & 1;
        struct result r = run_one(&mc, start_real, &mem, &io, op, next_op,
                                  ac, cry, NULL);
        print_result(op, ac, cry, &r);
        return 0;
    }

    if (argc >= 2 && !strcmp(argv[1], "sweep"))
        return run_sweep(&mc, start_real, &mem, &io);

    /* Built-in validation vectors. Nova ALC encoding (bit 0 = MSB):
     *   0o100000 | (ACS<<13) | (ACD<<11) | (func<<8) | (sh<<6) |
     *   (cy<<4) | (nl<<3) | skip.
     * func: COM=0 NEG=1 MOV=2 INC=3 ADC=4 SUB=5 ADD=6 AND=7. */
    #define ALC(acs,acd,func) (0100000u|((acs)<<13)|((acd)<<11)|((func)<<8))
    struct { const char *name; uint16_t op; uint16_t ac[4]; int cry; } V[] = {
        /* AC2 <- AC1 + AC2 */
        { "ADD 1,2 (5+3=10)", ALC(1,2,6), { 0, 5, 3, 0 }, 0 },
        { "ADD 1,2 (-1+1)",   ALC(1,2,6), { 0, 0177777, 1, 0 }, 0 },
        /* AC2 <- AC2 - AC1 */
        { "SUB 1,2 (8-3=5)",  ALC(1,2,5), { 0, 3, 010, 0 }, 0 },
        /* AC0 <- AC0 AND AC1 */
        { "AND 1,0",          ALC(1,0,7), { 0146, 0123, 0, 0 }, 0 },
        /* AC3 <- AC3, no-op MOV to check PC advance only */
        { "MOV 3,3",          ALC(3,3,2), { 0, 0, 0, 077 }, 0 },
        /* Memory-reference (the suspect class). Nova MRI (bit 0 = MSB,
         * salto include/emu.h): bit0=0; MFunc(bits1-2) 0=Jump 1=LDA
         * 2=STA; JFunc(bits3-4) within Jump 0=JMP 1=JSR 2=ISZ 3=DSZ;
         * DstAC bits3-4; I bit5; X(idx) bits6-7 (0=page zero); DISP
         * bits8-15. So LDA r,d=(1<<13)|(r<<11)|d; STA r,d=(2<<13)|
         * (r<<11)|d; ISZ d=(2<<11)|d; DSZ d=(3<<11)|d. */
        /* mem[0o100] <- AC1 (page zero) */
        { "STA 1,0100",       (uint16_t)((2u<<13)|(1u<<11)|0100), { 0, 012345, 0, 0 }, 0 },
        /* AC1 <- mem[0o100] : preload happens below (see note); checks load */
        { "LDA 1,0100",       (uint16_t)((1u<<13)|(1u<<11)|0100), { 0, 0, 0, 0 }, 0 },
        /* mem[0o100]++, skip if 0 : 0 -> 1, WR=0100:1 */
        { "ISZ 0100",         (uint16_t)((2u<<11)|0100), { 0, 0, 0, 0 }, 0 },
        /* mem[0o100]--, skip if 0 : 0 -> 177777, WR=0100:177777 */
        { "DSZ 0100",         (uint16_t)((3u<<11)|0100), { 0, 0, 0, 0 }, 0 },
    };
    #undef ALC
    for (size_t i = 0; i < sizeof V / sizeof V[0]; i++) {
        struct result r = run_one(&mc, start_real, &mem, &io, V[i].op,
                                  next_op, V[i].ac, V[i].cry, NULL);
        printf("%-18s ", V[i].name);
        print_result(V[i].op, V[i].ac, V[i].cry, &r);
    }
    return 0;
}
