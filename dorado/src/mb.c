#include "mb.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/*
 * .MB file reader. See include/mb.h and chm/microd/printmb.bcpl for the
 * format. All multi-byte values in a .MB file are 16-bit big-endian
 * words written by BCPL Puts() on the Alto.
 */

static int read_word(FILE *fp, uint16_t *out)
{
    int hi = fgetc(fp);
    int lo = fgetc(fp);
    if (hi == EOF || lo == EOF) return 0;
    *out = (uint16_t)((hi << 8) | lo);
    return 1;
}

/*
 * Read a BCPL packed string from a .MB stream.
 *
 * On disk the string is just a sequence of bytes packed two-per-16-bit
 * word, terminated by a NUL byte. Words are big-endian. There is NO
 * length prefix in the file — printmb.bcpl's readsymbol re-derives the
 * length while reading and stores it as the high byte of the
 * destination's first word per BCPL string convention.
 *
 * We follow the same algorithm: consume input words alternately
 * decoding their high then low byte; stop after we encounter a NUL.
 * Always consume entire words (so the caller's stream stays aligned
 * with the writer's word boundaries).
 */
static mb_status read_bcpl_string(FILE *fp, char *out, size_t cap)
{
    size_t n = 0;
    int done = 0;

    while (!done) {
        uint16_t wd;
        if (!read_word(fp, &wd)) return MB_ERR_TRUNCATED;
        int hi = (wd >> 8) & 0xff;
        int lo = wd & 0xff;

        if (hi == 0) { done = 1; break; }
        if (n >= cap - 1) return MB_ERR_NAME_TOO_LONG;
        out[n++] = (char)hi;

        if (lo == 0) { done = 1; break; }
        if (n >= cap - 1) return MB_ERR_NAME_TOO_LONG;
        out[n++] = (char)lo;
    }
    out[n] = '\0';
    return MB_OK;
}

static mb_memory *mem_at(mb_file *mb, int id)
{
    if (id < 0 || id >= MB_MAX_MEMS) return NULL;
    return &mb->mems[id];
}

static int ensure_capacity(mb_memory *m, int addr)
{
    if (addr < m->cap) return 1;
    int new_cap = m->cap ? m->cap : 256;
    while (new_cap <= addr) new_cap *= 2;
    uint16_t *nd = realloc(m->data, (size_t)new_cap * m->width_words * sizeof(uint16_t));
    uint8_t  *np = realloc(m->present, (size_t)new_cap * sizeof(uint8_t));
    if (!nd || !np) { free(nd); free(np); return 0; }
    /* zero the new tail */
    memset(nd + (size_t)m->cap * m->width_words, 0,
           (size_t)(new_cap - m->cap) * m->width_words * sizeof(uint16_t));
    memset(np + m->cap, 0, (size_t)(new_cap - m->cap) * sizeof(uint8_t));
    m->data = nd;
    m->present = np;
    m->cap = new_cap;
    return 1;
}

static int strieq(const char *a, const char *b)
{
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == 0 && *b == 0;
}

void mb_init(mb_file *mb)
{
    memset(mb, 0, sizeof(*mb));
    mb->im_id = mb->rm_id = mb->ifum_id = mb->alufm_id = mb->stk_id = mb->br_id = -1;
    mb->dmachine = -1;
}

void mb_free(mb_file *mb)
{
    for (int i = 0; i < MB_MAX_MEMS; i++) {
        free(mb->mems[i].data);
        free(mb->mems[i].present);
    }
    mb_symbol *s = mb->symbols;
    while (s) { mb_symbol *n = s->next; free(s); s = n; }
    mb_ext_fixup *f = mb->ext_fixups;
    while (f) { mb_ext_fixup *n = f->next; free(f); f = n; }
    memset(mb, 0, sizeof(*mb));
}

const char *mb_status_str(mb_status s)
{
    switch (s) {
    case MB_OK:                 return "ok";
    case MB_ERR_OPEN:           return "could not open file";
    case MB_ERR_TRUNCATED:      return "unexpected end of file";
    case MB_ERR_BAD_BLOCK:      return "unknown block type";
    case MB_ERR_MEM_UNDEF:      return "data block for undeclared memory";
    case MB_ERR_MEM_OVERFLOW:   return "address out of bounds";
    case MB_ERR_NAME_TOO_LONG:  return "memory or symbol name too long";
    case MB_ERR_NOMEM:          return "out of memory";
    }
    return "unknown error";
}

int mb_find_mem(const mb_file *mb, const char *name)
{
    for (int i = 0; i < MB_MAX_MEMS; i++) {
        if (mb->mems[i].defined && strieq(mb->mems[i].name, name)) return i;
    }
    return -1;
}

const char *mb_lookup_symbol(const mb_file *mb, int mem_id, int addr)
{
    for (mb_symbol *s = mb->symbols; s; s = s->next) {
        if (s->mem_id == mem_id && s->addr == addr) return s->name;
    }
    return NULL;
}

mb_status mb_load(mb_file *mb, const char *path)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) return MB_ERR_OPEN;
    mb_status s = mb_load_stream(mb, fp);
    fclose(fp);
    return s;
}

mb_status mb_load_stream(mb_file *mb, FILE *fp)
{
    int cur_mem = -1;
    int cur_addr = 0;

    for (;;) {
        uint16_t typ;
        if (!read_word(fp, &typ)) return MB_ERR_TRUNCATED;

        switch ((mb_block_type)typ) {
        case MB_END:
            goto done;

        case MB_MEMORY: {
            uint16_t mem, width;
            if (!read_word(fp, &mem) || !read_word(fp, &width))
                return MB_ERR_TRUNCATED;
            mb_memory *m = mem_at(mb, mem);
            if (!m) return MB_ERR_MEM_OVERFLOW;
            if (read_bcpl_string(fp, m->name, sizeof m->name) != MB_OK)
                return MB_ERR_NAME_TOO_LONG;
            m->defined = 1;
            m->width_bits = width;
            m->width_words = (width + 15) / 16;
            if (m->width_words > MB_MAX_WIDTH) return MB_ERR_MEM_OVERFLOW;
            /* Cache common memories by name. */
            if (mb->im_id    == -1 && strieq(m->name, "IM"))    mb->im_id = mem;
            if (mb->rm_id    == -1 && strieq(m->name, "RM"))    mb->rm_id = mem;
            if (mb->ifum_id  == -1 && strieq(m->name, "IFUM"))  mb->ifum_id = mem;
            if (mb->alufm_id == -1 && strieq(m->name, "ALUFM")) mb->alufm_id = mem;
            if (mb->stk_id   == -1 && strieq(m->name, "STK"))   mb->stk_id = mem;
            if (mb->br_id    == -1 && strieq(m->name, "BR"))    mb->br_id = mem;
            /* Infer DMachine from IM width: 36→1 (model 0); 64→2 (model 1); 48→0 (D0). */
            if (mb->im_id == (int)mem && mb->dmachine == -1) {
                if      (width >= 60) mb->dmachine = 2;
                else if (width >= 40) mb->dmachine = 0;
                else                  mb->dmachine = 1;
            }
            break;
        }

        case MB_ADDRESS: {
            uint16_t mem, addr;
            if (!read_word(fp, &mem) || !read_word(fp, &addr))
                return MB_ERR_TRUNCATED;
            cur_mem = mem;
            cur_addr = addr;
            break;
        }

        case MB_DATA: {
            uint16_t sourceline;
            if (!read_word(fp, &sourceline)) return MB_ERR_TRUNCATED;
            mb_memory *m = mem_at(mb, cur_mem);
            if (!m || !m->defined) return MB_ERR_MEM_UNDEF;
            if (!ensure_capacity(m, cur_addr)) return MB_ERR_NOMEM;
            uint16_t *slot = &m->data[(size_t)cur_addr * m->width_words];
            for (int i = 0; i < m->width_words; i++) {
                if (!read_word(fp, &slot[i])) return MB_ERR_TRUNCATED;
            }
            m->present[cur_addr] = 1;
            if (cur_addr + 1 > m->max_addr) m->max_addr = cur_addr + 1;
            cur_addr++;
            (void)sourceline;
            break;
        }

        case MB_FIXUP: {
            /* Same shape as MB_EXT_FIXUP but with an integer value
             * instead of a symbol. We capture nothing for now; future
             * work will re-emit fixups when patching IM. */
            uint16_t mem, addr, bits, val;
            if (!read_word(fp, &mem) || !read_word(fp, &addr) ||
                !read_word(fp, &bits) || !read_word(fp, &val))
                return MB_ERR_TRUNCATED;
            (void)mem; (void)addr; (void)bits; (void)val;
            break;
        }

        case MB_SYMBOL: {
            uint16_t mem, addr;
            if (!read_word(fp, &mem) || !read_word(fp, &addr))
                return MB_ERR_TRUNCATED;
            mb_symbol *s = calloc(1, sizeof *s);
            if (!s) return MB_ERR_NOMEM;
            s->mem_id = mem;
            s->addr = addr;
            mb_status rs = read_bcpl_string(fp, s->name, sizeof s->name);
            if (rs != MB_OK) { free(s); return rs; }
            s->next = mb->symbols;
            mb->symbols = s;
            break;
        }

        case MB_EXT_FIXUP: {
            uint16_t mem, addr, bits;
            if (!read_word(fp, &mem) || !read_word(fp, &addr) ||
                !read_word(fp, &bits))
                return MB_ERR_TRUNCATED;
            mb_ext_fixup *f = calloc(1, sizeof *f);
            if (!f) return MB_ERR_NOMEM;
            f->mem_id = mem;
            f->addr = addr;
            f->first_bit = (bits >> 8) & 0xff;
            f->last_bit  = bits & 0xff;
            mb_status rs = read_bcpl_string(fp, f->name, sizeof f->name);
            if (rs != MB_OK) { free(f); return rs; }
            f->next = mb->ext_fixups;
            mb->ext_fixups = f;
            break;
        }

        default:
            return MB_ERR_BAD_BLOCK;
        }
    }
done:
    return MB_OK;
}
