#ifndef DORADO_MB_H
#define DORADO_MB_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/*
 * .MB ("micro-binary") file loader.
 *
 * A .MB file is the output of MicroD — Xerox PARC's microcode
 * placer/linker for the Dorado and D0 (see chm/microd/, in particular
 * mddump.bcpl which writes this format and printmb.bcpl which reads it).
 *
 * A .MB file is a flat sequence of typed blocks. Each block starts with
 * a 16-bit big-endian type code and is followed by type-specific words.
 *
 *   type = 0  MB_END           end of file
 *   type = 1  MB_DATA          (sourceline, data...)  — data words for current (mem, addr); addr post-increments
 *   type = 2  MB_ADDRESS       (memory, addr)         — set "current" memory and address
 *   type = 3  MB_FIXUP         (memory, addr, bits, value)
 *                                bits packs (firstbit << 8 | lastbit)
 *   type = 4  MB_MEMORY        (memory, width, name) — declare memory: id, width in bits, BCPL string
 *   type = 5  MB_SYMBOL        (memory, addr, name)  — define symbol
 *   type = 6  MB_EXT_FIXUP     (memory, addr, bits, name) — fixup that resolves to an external symbol
 *
 * Strings are BCPL-style: first byte = length, then "length" bytes
 * packed two-per-16-bit-word. We unpack to a NUL-terminated C string.
 *
 * Memory IDs are NOT fixed across files. They are assigned by MicroD
 * when it emits the file, and declared via MB_MEMORY blocks. We recover
 * each memory's role (IM / RM / IFUM / ALUFM / STK / BR / ...) by name.
 */

typedef enum {
    MB_END        = 0,
    MB_DATA       = 1,
    MB_ADDRESS    = 2,
    MB_FIXUP      = 3,
    MB_MEMORY     = 4,
    MB_SYMBOL     = 5,
    MB_EXT_FIXUP  = 6,
} mb_block_type;

#define MB_MAX_MEMS    16
#define MB_MAX_NAME    32          /* longest mem/symbol name we keep */
#define MB_MAX_WIDTH   8           /* words per entry; 4 used by Dorado IM */

typedef struct {
    int      defined;              /* 1 = MB_MEMORY block seen for this id */
    int      width_bits;           /* as declared by MB_MEMORY */
    int      width_words;          /* ceil(width_bits / 16); per-entry word count */
    char     name[MB_MAX_NAME];
    /* Storage: one entry per address. cap is the highest addr + 1 seen. */
    uint16_t *data;                /* size = cap * width_words, big-endian-decoded host-order words */
    uint8_t  *present;             /* size = cap; 1 if MB_DATA wrote this addr */
    int      cap;
    int      max_addr;             /* highest addr written + 1 */
} mb_memory;

typedef struct mb_symbol {
    int   mem_id;
    int   addr;
    char  name[MB_MAX_NAME];
    struct mb_symbol *next;
} mb_symbol;

typedef struct mb_ext_fixup {
    int   mem_id;
    int   addr;
    int   first_bit;
    int   last_bit;
    char  name[MB_MAX_NAME];
    struct mb_ext_fixup *next;
} mb_ext_fixup;

typedef struct {
    mb_memory     mems[MB_MAX_MEMS];
    mb_symbol    *symbols;         /* head of singly-linked list */
    mb_ext_fixup *ext_fixups;
    /* Cached lookups by name; -1 if absent. */
    int  im_id, rm_id, ifum_id, alufm_id, stk_id, br_id;
    /* DMachine: 0 = D0, 1 = Dorado model 0, 2 = Dorado model 1. */
    int  dmachine;
} mb_file;

typedef enum {
    MB_OK = 0,
    MB_ERR_OPEN,
    MB_ERR_TRUNCATED,
    MB_ERR_BAD_BLOCK,
    MB_ERR_MEM_UNDEF,
    MB_ERR_MEM_OVERFLOW,
    MB_ERR_NAME_TOO_LONG,
    MB_ERR_NOMEM,
} mb_status;

/* Initialize an empty mb_file. Free with mb_free. */
void mb_init(mb_file *mb);
void mb_free(mb_file *mb);

/* Parse a .MB file from path or FILE*. Status codes above. */
mb_status mb_load(mb_file *mb, const char *path);
mb_status mb_load_stream(mb_file *mb, FILE *fp);

const char *mb_status_str(mb_status s);

/* Look up a memory by name (case-insensitive). Returns id (0..15) or -1. */
int mb_find_mem(const mb_file *mb, const char *name);

/* Look up a symbol at (mem, addr). NULL if none. */
const char *mb_lookup_symbol(const mb_file *mb, int mem_id, int addr);

/* Reverse lookup — find a symbol's image address by name in the
 * given memory. Returns -1 if not found. Case-sensitive. */
int mb_find_symbol_addr(const mb_file *mb, int mem_id, const char *name);

#endif
