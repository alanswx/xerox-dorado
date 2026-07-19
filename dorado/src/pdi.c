/*
 * pdi.c — PARC Disk Image (PDI) loader for Pilot/Cedar volumes.
 * See include/pdi.h and CedarDisk/PARC_PILOT_FORMAT.md.
 */
#include "pdi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint16_t be16(const uint8_t *p)
{
    return (uint16_t)((p[0] << 8) | p[1]);
}

static int fail(char *err, size_t errlen, const char *msg)
{
    if (err && errlen) {
        strncpy(err, msg, errlen - 1);
        err[errlen - 1] = '\0';
    }
    return -1;
}

int dorado_pdi_load(const char *path, dorado_pdi *out, char *err, size_t errlen)
{
    memset(out, 0, sizeof *out);

    FILE *f = fopen(path, "rb");
    if (!f) return fail(err, errlen, "cannot open PDI file");

    if (fseek(f, 0, SEEK_END) != 0) { fclose(f); return fail(err, errlen, "seek"); }
    long fsize = ftell(f);
    if (fsize < DORADO_PDI_HEADER_BYTES) { fclose(f); return fail(err, errlen, "file too small"); }
    rewind(f);

    uint8_t *raw = malloc((size_t)fsize);
    if (!raw) { fclose(f); return fail(err, errlen, "out of memory"); }
    if (fread(raw, 1, (size_t)fsize, f) != (size_t)fsize) {
        free(raw); fclose(f); return fail(err, errlen, "short read");
    }
    fclose(f);

    if (memcmp(raw, DORADO_PDI_MAGIC, 8) != 0) {
        free(raw); return fail(err, errlen, "bad magic (not PARCDISK)");
    }

    out->version    = be16(raw + 8);
    out->fs_family  = be16(raw + 10);
    /* Words 8..10 are the drive's CHS geometry, and the page count is their
     * product -- NOT word 8 alone. Reading only the cylinder word capped an
     * image at 65535 pages (33 MB), so every image we build carries the
     * degenerate geometry 65535x1x1 to satisfy that misreading. A real
     * Trident T-300 is an order of magnitude larger, and a second logical
     * volume needs the room, so honor all three fields. Old images still
     * load unchanged: 65535 * 1 * 1 == 65535. */
    uint32_t cylinders = be16(raw + 16);      /* word 8  */
    uint32_t heads     = be16(raw + 18);      /* word 9  */
    uint32_t sectors   = be16(raw + 20);      /* word 10 */
    if (heads == 0) heads = 1;                /* tolerate under-filled */
    if (sectors == 0) sectors = 1;            /* headers from old writers */
    out->page_count = cylinders * heads * sectors;
    uint16_t label_bytes = be16(raw + 24);    /* word 12 */
    uint16_t data_bytes  = be16(raw + 26);    /* word 13 */

    if (out->fs_family != DORADO_PDI_FS_PILOT) {
        free(raw); return fail(err, errlen, "not a Pilot (fsFamily=2) image");
    }
    if ((label_bytes & 1) || (data_bytes & 1)) {
        free(raw); return fail(err, errlen, "odd label/data byte count");
    }
    out->label_words = (uint16_t)(label_bytes / 2);
    out->data_words  = (uint16_t)(data_bytes / 2);
    if (out->label_words == 0 || out->data_words == 0 || out->page_count == 0) {
        free(raw); return fail(err, errlen, "zero geometry");
    }

    size_t sector_bytes = (size_t)label_bytes + (size_t)data_bytes;
    size_t need = (size_t)DORADO_PDI_HEADER_BYTES + (size_t)out->page_count * sector_bytes;
    if ((size_t)fsize < need) {
        free(raw); return fail(err, errlen, "file shorter than header+pages");
    }

    out->labels = malloc((size_t)out->page_count * out->label_words * sizeof(uint16_t));
    out->data   = malloc((size_t)out->page_count * out->data_words  * sizeof(uint16_t));
    if (!out->labels || !out->data) {
        free(raw); dorado_pdi_free(out); return fail(err, errlen, "out of memory");
    }

    for (uint32_t p = 0; p < out->page_count; p++) {
        const uint8_t *sec = raw + DORADO_PDI_HEADER_BYTES + (size_t)p * sector_bytes;
        uint16_t *lab = out->labels + (size_t)p * out->label_words;
        uint16_t *dat = out->data   + (size_t)p * out->data_words;
        for (int w = 0; w < out->label_words; w++) lab[w] = be16(sec + 2 * w);
        const uint8_t *dsec = sec + label_bytes;
        for (int w = 0; w < out->data_words; w++)  dat[w] = be16(dsec + 2 * w);
    }

    free(raw);
    return 0;
}

int dorado_pdi_save(const char *path, const dorado_pdi *p,
                    char *err, size_t errlen)
{
    if (!path || !p || !p->labels || !p->data || !p->page_count ||
        !p->label_words || !p->data_words)
        return fail(err, errlen, "invalid PDI state");

    FILE *f = fopen(path, "r+b");
    if (!f) return fail(err, errlen, "cannot open PDI for update");
    if (fseek(f, DORADO_PDI_HEADER_BYTES, SEEK_SET) != 0) {
        fclose(f);
        return fail(err, errlen, "seek");
    }

    size_t words = (size_t)p->label_words + p->data_words;
    uint8_t *sector = malloc(words * 2u);
    if (!sector) {
        fclose(f);
        return fail(err, errlen, "out of memory");
    }
    int rc = 0;
    for (uint32_t page = 0; page < p->page_count; page++) {
        const uint16_t *label = p->labels + (size_t)page * p->label_words;
        const uint16_t *data = p->data + (size_t)page * p->data_words;
        for (uint16_t w = 0; w < p->label_words; w++) {
            sector[2u * w] = (uint8_t)(label[w] >> 8);
            sector[2u * w + 1u] = (uint8_t)label[w];
        }
        for (uint16_t w = 0; w < p->data_words; w++) {
            size_t off = 2u * ((size_t)p->label_words + w);
            sector[off] = (uint8_t)(data[w] >> 8);
            sector[off + 1u] = (uint8_t)data[w];
        }
        if (fwrite(sector, 1, words * 2u, f) != words * 2u) {
            rc = -1;
            break;
        }
    }
    free(sector);
    if (fclose(f) != 0) rc = -1;
    if (rc) return fail(err, errlen, "write error");
    return 0;
}

void dorado_pdi_free(dorado_pdi *p)
{
    if (!p) return;
    free(p->labels);
    free(p->data);
    p->labels = NULL;
    p->data = NULL;
}

const uint16_t *dorado_pdi_page_data(const dorado_pdi *p, uint32_t page)
{
    if (!p || page >= p->page_count) return NULL;
    return p->data + (size_t)page * p->data_words;
}

const uint16_t *dorado_pdi_page_label(const dorado_pdi *p, uint32_t page)
{
    if (!p || page >= p->page_count) return NULL;
    return p->labels + (size_t)page * p->label_words;
}

uint32_t dorado_pdi_label_fileid(const uint16_t *label)
{
    /* Cedar nucleus: words 0-1 = 32-bit FileID, low word first. */
    return (uint32_t)label[0] | ((uint32_t)label[1] << 16);
}

uint32_t dorado_pdi_label_filepage(const uint16_t *label)
{
    /* words 5 (low) / 6 (high) */
    return (uint32_t)label[5] | ((uint32_t)label[6] << 16);
}

uint16_t dorado_pdi_label_attr(const uint16_t *label)
{
    return label[7];
}

long dorado_pdi_read_file(const dorado_pdi *p, uint32_t file_id,
                          uint16_t *out_words, size_t max_words)
{
    if (!p) return -1;

    /* Find the highest file-page so we know the file length, then place each
     * page at its file-relative offset (handles non-contiguous / out-of-order
     * physical placement; labels are the truth, §2). */
    long npages = 0;
    uint32_t maxfp = 0;
    for (uint32_t pg = 0; pg < p->page_count; pg++) {
        const uint16_t *l = dorado_pdi_page_label(p, pg);
        if (dorado_pdi_label_fileid(l) != file_id) continue;
        uint16_t a = dorado_pdi_label_attr(l);
        if (a != 9729 /*header*/ && a != 9730 /*data*/) continue; /* skip non-content */
        uint32_t fp = dorado_pdi_label_filepage(l);
        if (fp > maxfp) maxfp = fp;
        npages++;
    }
    if (npages == 0) return 0;

    for (uint32_t pg = 0; pg < p->page_count; pg++) {
        const uint16_t *l = dorado_pdi_page_label(p, pg);
        if (dorado_pdi_label_fileid(l) != file_id) continue;
        uint16_t a = dorado_pdi_label_attr(l);
        if (a != 9729 && a != 9730) continue;
        uint32_t fp = dorado_pdi_label_filepage(l);
        size_t off = (size_t)fp * p->data_words;
        const uint16_t *src = dorado_pdi_page_data(p, pg);
        for (int w = 0; w < p->data_words; w++) {
            if (off + (size_t)w >= max_words) break;
            out_words[off + w] = src[w];
        }
    }
    return npages;
}

uint16_t dorado_pilot_checksum(const uint16_t *words, int n)
{
    uint16_t cs = 0;
    for (int i = 0; i < n; i++) {
        uint32_t t = (uint32_t)cs + words[i];
        if (t > 0xFFFF) t = (t & 0xFFFF) + 1;   /* ones-complement add */
        cs = (uint16_t)t;
        cs = (cs & 0x8000) ? (uint16_t)((cs << 1) | 1) : (uint16_t)(cs << 1);
    }
    return (cs == 0xFFFF) ? 0 : cs;
}
