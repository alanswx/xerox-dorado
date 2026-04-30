/*
 * bbdis — disassemble the Dorado BaseBoard 6502 ROM.
 *
 * Loads a .MB file (typically chm/dorado/doradobaserom.mb!13) into a
 * 64K byte image and walks a chosen range emitting annotated 6502
 * mnemonics. Designed to feed gap A1/A2/B8/D2 research per
 * docs/research-plan.md.
 *
 * Usage:
 *   bbdis [--start HEX] [--end HEX] [--labels FILE] [--vectors] PATH
 *
 *   --start HEX     start address (default C000)
 *   --end HEX       end address inclusive (default FFFF)
 *   --labels FILE   tab-separated `HEX<tab>name`, one per line
 *   --vectors       print reset/irq/nmi vectors, then exit
 *
 * Output one line per instruction:
 *   ADDR: bb bb bb   MNEM operand          ; label_or_target
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include "mb.h"
#include "disasm.h"

#define ROM_SIZE 0x10000

/* Addressing modes (encoded in a single byte per opcode). */
enum {
    IMP, ACC, IMM, ZP, ZPX, ZPY, ABS, ABX, ABY, IND, IZX, IZY, REL, UNK
};

static const uint8_t ins_size[] = {
    /* IMP ACC IMM ZP ZPX ZPY ABS ABX ABY IND IZX IZY REL UNK */
       1,  1,  2,  2, 2,  2,  3,  3,  3,  3,  2,  2,  2,  1
};

typedef struct {
    const char *m;
    uint8_t mode;
} op_t;

/* Standard NMOS 6502 opcode table. Illegal opcodes -> "???"/UNK. */
static op_t ops[256];

static void init_ops(void)
{
    for (int i = 0; i < 256; i++) { ops[i].m = "???"; ops[i].mode = UNK; }
#define O(x, m, mode) ops[x] = (op_t){m, mode}
    /* 0x00 */ O(0x00,"BRK",IMP); O(0x01,"ORA",IZX); O(0x05,"ORA",ZP);
    O(0x06,"ASL",ZP);  O(0x08,"PHP",IMP); O(0x09,"ORA",IMM); O(0x0A,"ASL",ACC);
    O(0x0D,"ORA",ABS); O(0x0E,"ASL",ABS);
    /* 0x10 */ O(0x10,"BPL",REL); O(0x11,"ORA",IZY); O(0x15,"ORA",ZPX);
    O(0x16,"ASL",ZPX); O(0x18,"CLC",IMP); O(0x19,"ORA",ABY);
    O(0x1D,"ORA",ABX); O(0x1E,"ASL",ABX);
    /* 0x20 */ O(0x20,"JSR",ABS); O(0x21,"AND",IZX); O(0x24,"BIT",ZP);
    O(0x25,"AND",ZP);  O(0x26,"ROL",ZP);  O(0x28,"PLP",IMP); O(0x29,"AND",IMM);
    O(0x2A,"ROL",ACC); O(0x2C,"BIT",ABS); O(0x2D,"AND",ABS); O(0x2E,"ROL",ABS);
    /* 0x30 */ O(0x30,"BMI",REL); O(0x31,"AND",IZY); O(0x35,"AND",ZPX);
    O(0x36,"ROL",ZPX); O(0x38,"SEC",IMP); O(0x39,"AND",ABY);
    O(0x3D,"AND",ABX); O(0x3E,"ROL",ABX);
    /* 0x40 */ O(0x40,"RTI",IMP); O(0x41,"EOR",IZX); O(0x45,"EOR",ZP);
    O(0x46,"LSR",ZP);  O(0x48,"PHA",IMP); O(0x49,"EOR",IMM); O(0x4A,"LSR",ACC);
    O(0x4C,"JMP",ABS); O(0x4D,"EOR",ABS); O(0x4E,"LSR",ABS);
    /* 0x50 */ O(0x50,"BVC",REL); O(0x51,"EOR",IZY); O(0x55,"EOR",ZPX);
    O(0x56,"LSR",ZPX); O(0x58,"CLI",IMP); O(0x59,"EOR",ABY);
    O(0x5D,"EOR",ABX); O(0x5E,"LSR",ABX);
    /* 0x60 */ O(0x60,"RTS",IMP); O(0x61,"ADC",IZX); O(0x65,"ADC",ZP);
    O(0x66,"ROR",ZP);  O(0x68,"PLA",IMP); O(0x69,"ADC",IMM); O(0x6A,"ROR",ACC);
    O(0x6C,"JMP",IND); O(0x6D,"ADC",ABS); O(0x6E,"ROR",ABS);
    /* 0x70 */ O(0x70,"BVS",REL); O(0x71,"ADC",IZY); O(0x75,"ADC",ZPX);
    O(0x76,"ROR",ZPX); O(0x78,"SEI",IMP); O(0x79,"ADC",ABY);
    O(0x7D,"ADC",ABX); O(0x7E,"ROR",ABX);
    /* 0x80 */ O(0x81,"STA",IZX); O(0x84,"STY",ZP);  O(0x85,"STA",ZP);
    O(0x86,"STX",ZP);  O(0x88,"DEY",IMP); O(0x8A,"TXA",IMP);
    O(0x8C,"STY",ABS); O(0x8D,"STA",ABS); O(0x8E,"STX",ABS);
    /* 0x90 */ O(0x90,"BCC",REL); O(0x91,"STA",IZY); O(0x94,"STY",ZPX);
    O(0x95,"STA",ZPX); O(0x96,"STX",ZPY); O(0x98,"TYA",IMP); O(0x99,"STA",ABY);
    O(0x9A,"TXS",IMP); O(0x9D,"STA",ABX);
    /* 0xA0 */ O(0xA0,"LDY",IMM); O(0xA1,"LDA",IZX); O(0xA2,"LDX",IMM);
    O(0xA4,"LDY",ZP);  O(0xA5,"LDA",ZP);  O(0xA6,"LDX",ZP);  O(0xA8,"TAY",IMP);
    O(0xA9,"LDA",IMM); O(0xAA,"TAX",IMP); O(0xAC,"LDY",ABS); O(0xAD,"LDA",ABS);
    O(0xAE,"LDX",ABS);
    /* 0xB0 */ O(0xB0,"BCS",REL); O(0xB1,"LDA",IZY); O(0xB4,"LDY",ZPX);
    O(0xB5,"LDA",ZPX); O(0xB6,"LDX",ZPY); O(0xB8,"CLV",IMP); O(0xB9,"LDA",ABY);
    O(0xBA,"TSX",IMP); O(0xBC,"LDY",ABX); O(0xBD,"LDA",ABX); O(0xBE,"LDX",ABY);
    /* 0xC0 */ O(0xC0,"CPY",IMM); O(0xC1,"CMP",IZX); O(0xC4,"CPY",ZP);
    O(0xC5,"CMP",ZP);  O(0xC6,"DEC",ZP);  O(0xC8,"INY",IMP); O(0xC9,"CMP",IMM);
    O(0xCA,"DEX",IMP); O(0xCC,"CPY",ABS); O(0xCD,"CMP",ABS); O(0xCE,"DEC",ABS);
    /* 0xD0 */ O(0xD0,"BNE",REL); O(0xD1,"CMP",IZY); O(0xD5,"CMP",ZPX);
    O(0xD6,"DEC",ZPX); O(0xD8,"CLD",IMP); O(0xD9,"CMP",ABY);
    O(0xDD,"CMP",ABX); O(0xDE,"DEC",ABX);
    /* 0xE0 */ O(0xE0,"CPX",IMM); O(0xE1,"SBC",IZX); O(0xE4,"CPX",ZP);
    O(0xE5,"SBC",ZP);  O(0xE6,"INC",ZP);  O(0xE8,"INX",IMP); O(0xE9,"SBC",IMM);
    O(0xEA,"NOP",IMP); O(0xEC,"CPX",ABS); O(0xED,"SBC",ABS); O(0xEE,"INC",ABS);
    /* 0xF0 */ O(0xF0,"BEQ",REL); O(0xF1,"SBC",IZY); O(0xF5,"SBC",ZPX);
    O(0xF6,"INC",ZPX); O(0xF8,"SED",IMP); O(0xF9,"SBC",ABY);
    O(0xFD,"SBC",ABX); O(0xFE,"INC",ABX);
#undef O
}

/* Symbol table: simple linear array. */
typedef struct { uint16_t addr; const char *name; } sym_t;
#define MAX_SYMS 4096
static sym_t syms[MAX_SYMS];
static int n_syms;

static const char *sym_lookup(uint16_t a)
{
    for (int i = 0; i < n_syms; i++) if (syms[i].addr == a) return syms[i].name;
    return NULL;
}

static int load_labels(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp) { perror(path); return 1; }
    char line[256];
    while (fgets(line, sizeof line, fp)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        char *tab = strchr(line, '\t');
        if (!tab) continue;
        *tab = 0;
        unsigned a = (unsigned)strtoul(line, NULL, 16);
        char *name = tab + 1;
        size_t len = strlen(name);
        while (len && (name[len-1] == '\n' || name[len-1] == '\r' ||
                       name[len-1] == ' '  || name[len-1] == '\t')) {
            name[--len] = 0;
        }
        if (!len) continue;
        if (n_syms >= MAX_SYMS) break;
        syms[n_syms].addr = (uint16_t)a;
        syms[n_syms].name = strdup(name);
        n_syms++;
    }
    fclose(fp);
    return 0;
}

/* Print one instruction. Returns its byte length. */
static int dis_one(uint16_t pc, const uint8_t *rom, char *out, size_t out_sz)
{
    uint8_t op = rom[pc];
    op_t  *o  = &ops[op];
    int    sz = ins_size[o->mode];
    char   bytes[16];
    int    bp = 0;
    for (int i = 0; i < sz; i++) {
        bp += snprintf(bytes + bp, sizeof bytes - bp, "%02X ", rom[(pc+i) & 0xFFFF]);
    }
    while (bp < 9) bytes[bp++] = ' ';
    bytes[9] = 0;

    char operand[40] = "";
    char target[40]  = "";
    uint16_t tgt_addr = 0;
    int has_target = 0;

    switch (o->mode) {
    case IMP: break;
    case ACC: snprintf(operand, sizeof operand, "A"); break;
    case IMM: snprintf(operand, sizeof operand, "#$%02X", rom[pc+1]); break;
    case ZP:  snprintf(operand, sizeof operand, "$%02X", rom[pc+1]); break;
    case ZPX: snprintf(operand, sizeof operand, "$%02X,X", rom[pc+1]); break;
    case ZPY: snprintf(operand, sizeof operand, "$%02X,Y", rom[pc+1]); break;
    case ABS:
        tgt_addr = (uint16_t)(rom[pc+1] | (rom[pc+2] << 8));
        has_target = 1;
        snprintf(operand, sizeof operand, "$%04X", tgt_addr);
        break;
    case ABX:
        tgt_addr = (uint16_t)(rom[pc+1] | (rom[pc+2] << 8));
        snprintf(operand, sizeof operand, "$%04X,X", tgt_addr);
        break;
    case ABY:
        tgt_addr = (uint16_t)(rom[pc+1] | (rom[pc+2] << 8));
        snprintf(operand, sizeof operand, "$%04X,Y", tgt_addr);
        break;
    case IND:
        tgt_addr = (uint16_t)(rom[pc+1] | (rom[pc+2] << 8));
        snprintf(operand, sizeof operand, "($%04X)", tgt_addr);
        break;
    case IZX: snprintf(operand, sizeof operand, "($%02X,X)", rom[pc+1]); break;
    case IZY: snprintf(operand, sizeof operand, "($%02X),Y", rom[pc+1]); break;
    case REL: {
        int8_t off = (int8_t)rom[pc+1];
        tgt_addr = (uint16_t)(pc + 2 + off);
        has_target = 1;
        snprintf(operand, sizeof operand, "$%04X", tgt_addr);
        break;
    }
    case UNK:
        snprintf(operand, sizeof operand, " ; illegal $%02X", op);
        break;
    }

    if (has_target) {
        const char *s = sym_lookup(tgt_addr);
        if (s) snprintf(target, sizeof target, "  ; %s", s);
    }

    snprintf(out, out_sz, "%04X: %s %s %-15s%s",
             pc, bytes, o->m, operand, target);
    return sz ? sz : 1;
}

static int load_rom(const char *path, uint8_t *rom)
{
    mb_file mb;
    mb_init(&mb);
    if (mb_load(&mb, path) != MB_OK) {
        fprintf(stderr, "could not load %s\n", path);
        return 1;
    }
    int memid = -1;
    for (int i = 0; i < MB_MAX_MEMS; i++) {
        if (mb.mems[i].defined &&
            (strcmp(mb.mems[i].name, "$ABSOLUTE") == 0 ||
             strcmp(mb.mems[i].name, "ABSOLUTE")  == 0)) {
            memid = i;
            break;
        }
    }
    if (memid < 0) {
        fprintf(stderr, "no $ABSOLUTE memory in %s\n", path);
        mb_free(&mb);
        return 1;
    }
    const mb_memory *m = &mb.mems[memid];
    int n = 0;
    for (int a = 0; a < m->max_addr && a < ROM_SIZE; a++) {
        if (m->present[a]) {
            rom[a] = (uint8_t)((m->data[a] >> 8) & 0xFF);
            n++;
        }
    }
    fprintf(stderr, "; loaded %d bytes from %s\n", n, path);
    mb_free(&mb);
    return 0;
}

static void dump_vectors(const uint8_t *rom)
{
    uint16_t nmi   = rom[0xFFFA] | (rom[0xFFFB] << 8);
    uint16_t reset = rom[0xFFFC] | (rom[0xFFFD] << 8);
    uint16_t irq   = rom[0xFFFE] | (rom[0xFFFF] << 8);
    printf("; NMI vector   ($FFFA): $%04X\n", nmi);
    printf("; RESET vector ($FFFC): $%04X\n", reset);
    printf("; IRQ vector   ($FFFE): $%04X\n", irq);
}

/*
 * Decode one 17-byte hunk into 4 dorado_uinstr per the format
 * documented in doradoboot.masm SendAHunk:
 *
 *   Byte 0: 0RSTK.0 0BLOCK 1RSTK.0 1BLOCK 2RSTK.0 2BLOCK 3RSTK.0 3BLOCK
 *   Bytes 1..4   = instr 0 (RSTK[1..3]/ALUF/BSEL/LC/ASEL/FF/JCN)
 *   Bytes 5..8   = instr 1
 *   Bytes 9..12  = instr 2
 *   Bytes 13..16 = instr 3
 *
 * Per-instruction packing (MSB-first):
 *   B1: RSTK[1..3] ALUF[0..3] BSEL[0]
 *   B2: BSEL[1..2] LC[0..2]   ASEL[0..2]
 *   B3: FF[0..7]
 *   B4: JCN[0..7]
 */
static void decode_hunk(const uint8_t hunk[17], dorado_uinstr out[4])
{
    for (int i = 0; i < 4; i++) {
        uint8_t rstk0 = (hunk[0] >> (7 - 2*i)) & 1;
        uint8_t block = (hunk[0] >> (6 - 2*i)) & 1;
        const uint8_t *p = hunk + 1 + 4*i;
        uint8_t rstk13 = (p[0] >> 5) & 7;
        uint8_t aluf   = (p[0] >> 1) & 0xF;
        uint8_t bsel0  = p[0] & 1;
        uint8_t bsel12 = (p[1] >> 6) & 3;
        uint8_t lc     = (p[1] >> 3) & 7;
        uint8_t asel   = p[1] & 7;
        uint8_t ff     = p[2];
        uint8_t jcn    = p[3];

        memset(&out[i], 0, sizeof out[i]);
        out[i].rstk  = (uint8_t)((rstk0 << 3) | rstk13);
        out[i].aluf  = aluf;
        out[i].bsel  = (uint8_t)((bsel0 << 2) | bsel12);
        out[i].lc    = lc;
        out[i].asel  = asel;
        out[i].block = block;
        out[i].ff    = ff;
        out[i].jcn   = jcn;
    }
}

static void dump_hunks(const uint8_t *rom, uint16_t base, int count,
                       uint16_t im_base)
{
    printf("; Hunk decode: base=$%04X count=%d (=%d microinstr) "
           "→ IM[%04o..%04o]\n",
           base, count, count * 4, im_base, im_base + count * 4 - 1);
    for (int h = 0; h < count; h++) {
        uint16_t addr = (uint16_t)(base + h * 17);
        if (addr + 17 > 0x10000) break;
        dorado_uinstr u[4];
        decode_hunk(rom + addr, u);
        printf("\n; ── hunk %d  $%04X..$%04X  IM[%04o..%04o] ──\n",
               h, addr, addr + 16, im_base + h*4, im_base + h*4 + 3);
        printf("; raw:");
        for (int j = 0; j < 17; j++) printf(" %02X", rom[addr + j]);
        printf("\n");
        for (int i = 0; i < 4; i++) {
            char buf[256];
            dorado_format(&u[i], buf, sizeof buf);
            printf("IM[%04o]:  %s\n", im_base + h*4 + i, buf);
        }
    }
}

int main(int argc, char **argv)
{
    init_ops();

    uint16_t start = 0xC000, end = 0xFFFF;
    const char *labels = NULL;
    const char *path   = NULL;
    int do_vectors = 0;
    int do_hunks   = 0;
    uint16_t hunk_base = 0;
    int hunk_count = 0;
    uint16_t hunk_im_base = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--start") == 0 && i + 1 < argc) {
            start = (uint16_t)strtoul(argv[++i], NULL, 16);
        } else if (strcmp(argv[i], "--end") == 0 && i + 1 < argc) {
            end = (uint16_t)strtoul(argv[++i], NULL, 16);
        } else if (strcmp(argv[i], "--labels") == 0 && i + 1 < argc) {
            labels = argv[++i];
        } else if (strcmp(argv[i], "--vectors") == 0) {
            do_vectors = 1;
        } else if (strcmp(argv[i], "--hunks") == 0 && i + 3 < argc) {
            do_hunks = 1;
            hunk_base    = (uint16_t)strtoul(argv[++i], NULL, 16);
            hunk_count   = (int)strtoul(argv[++i], NULL, 0);
            hunk_im_base = (uint16_t)strtoul(argv[++i], NULL, 8);
        } else if (argv[i][0] != '-') {
            path = argv[i];
        }
    }

    if (!path) {
        fprintf(stderr,
            "usage: bbdis [--start HEX] [--end HEX] [--labels FILE]\n"
            "             [--vectors] [--hunks BASE COUNT IM_BASE_OCT] PATH\n");
        return 2;
    }

    static uint8_t rom[ROM_SIZE];
    if (load_rom(path, rom)) return 1;

    if (labels && load_labels(labels)) return 1;

    if (do_vectors) {
        dump_vectors(rom);
        return 0;
    }

    if (do_hunks) {
        dump_hunks(rom, hunk_base, hunk_count, hunk_im_base);
        return 0;
    }

    uint32_t pc = start;
    while (pc <= end) {
        const char *lbl = sym_lookup((uint16_t)pc);
        if (lbl) printf("\n%s:\n", lbl);
        char line[160];
        int sz = dis_one((uint16_t)pc, rom, line, sizeof line);
        printf("%s\n", line);
        pc += sz;
    }
    return 0;
}
