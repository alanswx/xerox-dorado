/*
 * test_pdi.c — unit tests for the PARC Disk Image (PDI) loader.
 *
 * Self-contained: builds a synthetic Pilot PDI in a temp file (no dependency
 * on the checked-in image, which is gitignored once decompressed), then loads
 * and verifies it. Also exercises dorado_pilot_checksum and the label/file
 * accessors. If ../CedarDisk/CedarDorado-boot.pdi is present, it additionally
 * probes the real volume (informational; not a hard assertion).
 */
#include "pdi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", msg); failures++; } } while (0)

static void put_be16(unsigned char *p, uint16_t v) { p[0] = v >> 8; p[1] = v & 0xFF; }

/* Build a 3-page synthetic Pilot PDI: page 0 = physical root (seal+checksum),
 * pages 1-2 = a 2-page file FileID 7 (header + data). */
static const char *write_synthetic_pdi(void)
{
    static char path[] = "/tmp/dorado_test_synth.pdi";
    const int LBL = 20, DAT = 512, SECT = 532, HDR = 512, PAGES = 3;
    unsigned char *img = calloc(1, HDR + PAGES * SECT);

    memcpy(img, "PARCDISK", 8);
    put_be16(img + 8, 1);            /* version */
    put_be16(img + 10, 2);           /* fsFamily = Pilot */
    put_be16(img + 16, PAGES);       /* pageCount */
    put_be16(img + 24, LBL);         /* labelBytes */
    put_be16(img + 26, DAT);         /* dataBytes */

    /* page 0: label (fileID 1, filePage 0, attr 1 physicalRoot) */
    unsigned char *l0 = img + HDR + 0 * SECT;
    put_be16(l0 + 0, 1);             /* fileID lo */
    put_be16(l0 + 14, 1);            /* attributes = physicalRoot (word 7) */
    /* page 0 data: seal + version + a checksum at word 255 */
    unsigned char *d0 = img + HDR + 0 * SECT + LBL;
    put_be16(d0 + 0, 0121212);       /* PRSeal */
    put_be16(d0 + 2, 6);             /* version */
    /* compute checksum over words [0..255) and store at 255 */
    {
        uint16_t words[256];
        for (int i = 0; i < 256; i++) words[i] = (d0[2*i] << 8) | d0[2*i+1];
        uint16_t cs = dorado_pilot_checksum(words, 255);
        put_be16(d0 + 2 * 255, cs);
    }

    /* pages 1-2: file FileID 7, attr data, filePage 0 and 1, data = page index marker */
    for (int pg = 1; pg <= 2; pg++) {
        unsigned char *l = img + HDR + pg * SECT;
        unsigned char *d = l + LBL;
        put_be16(l + 0, 7);          /* fileID lo */
        put_be16(l + 10, pg - 1);    /* filePage lo (word 5) */
        put_be16(l + 14, 9730);      /* attributes = data (word 7) */
        put_be16(d + 0, 0xBEEF);     /* marker word 0 */
        put_be16(d + 2, pg);         /* marker word 1 = physical page */
    }

    FILE *f = fopen(path, "wb");
    fwrite(img, 1, HDR + PAGES * SECT, f);
    fclose(f);
    free(img);
    return path;
}

int main(void)
{
    /* checksum: 0xFFFF normalizes to 0; empty input is 0 */
    CHECK(dorado_pilot_checksum(NULL, 0) == 0, "checksum of empty = 0");
    { uint16_t z[2] = {0,0}; CHECK(dorado_pilot_checksum(z, 2) == 0, "checksum of zeros = 0"); }

    const char *path = write_synthetic_pdi();
    dorado_pdi p; char err[128];
    CHECK(dorado_pdi_load(path, &p, err, sizeof err) == 0, "load synthetic PDI");

    CHECK(p.fs_family == 2, "fsFamily = 2");
    CHECK(p.page_count == 3, "page_count = 3");
    CHECK(p.label_words == 10 && p.data_words == 256, "label/data words");

    const uint16_t *d0 = dorado_pdi_page_data(&p, 0);
    CHECK(d0 && d0[0] == 0121212, "page 0 seal");
    CHECK(d0 && d0[1] == 6, "page 0 version");
    CHECK(d0 && dorado_pilot_checksum(d0, 255) == d0[255], "page 0 checksum verifies");

    const uint16_t *l0 = dorado_pdi_page_label(&p, 0);
    CHECK(l0 && dorado_pdi_label_attr(l0) == 1, "page 0 attr = physicalRoot");
    CHECK(l0 && dorado_pdi_label_fileid(l0) == 1, "page 0 fileID = 1");

    /* read file 7 (2 pages) */
    uint16_t buf[2 * 256];
    memset(buf, 0, sizeof buf);
    long n = dorado_pdi_read_file(&p, 7, buf, 2 * 256);
    CHECK(n == 2, "file 7 has 2 pages");
    CHECK(buf[0] == 0xBEEF && buf[1] == 1, "file 7 page 0 (filePage 0) marker");
    CHECK(buf[256] == 0xBEEF && buf[257] == 2, "file 7 page 1 (filePage 1) marker");

    CHECK(dorado_pdi_page_data(&p, 99) == NULL, "out-of-range page returns NULL");

    /* PDI persistence is explicit: update both label and data, save the
     * existing container, and ensure a fresh load sees the changes. */
    p.data[1 * p.data_words + 2] = 0xCAFE;
    p.labels[1 * p.label_words + 7] = 0x2601;
    CHECK(dorado_pdi_save(path, &p, err, sizeof err) == 0,
          "save synthetic PDI");
    dorado_pdi_free(&p);
    {
        dorado_pdi saved;
        CHECK(dorado_pdi_load(path, &saved, err, sizeof err) == 0,
              "reload saved synthetic PDI");
        CHECK(saved.data[1 * saved.data_words + 2] == 0xCAFE,
              "saved page data persists");
        CHECK(saved.labels[1 * saved.label_words + 7] == 0x2601,
              "saved page label persists");
        dorado_pdi_free(&saved);
    }

    /* informational: probe the real image if present */
    dorado_pdi real;
    if (dorado_pdi_load("../CedarDisk/CedarDorado-boot.pdi", &real, err, sizeof err) == 0) {
        const uint16_t *r0 = dorado_pdi_page_data(&real, 0);
        printf("  [real image] pages=%u seal=0%o version=%u checksum %s\n",
               real.page_count, r0[0], r0[1],
               dorado_pilot_checksum(r0, 255) == r0[255] ? "OK" : "MISMATCH");
        long germ = dorado_pdi_read_file(&real, 2, buf, 0);
        long boot = dorado_pdi_read_file(&real, 3, buf, 0);
        printf("  [real image] germ(FileID 2)=%ld pages, bootFile(FileID 3)=%ld pages\n",
               germ, boot);
        dorado_pdi_free(&real);
    }

    if (failures == 0) printf("PASS  all PDI tests passed.\n");
    else printf("FAIL  %d PDI test(s) failed.\n", failures);
    return failures ? 1 : 0;
}
