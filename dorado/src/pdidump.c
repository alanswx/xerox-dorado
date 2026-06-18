/*
 * pdidump.c — offline inspector for a Pilot/Cedar PARC Disk Image (PDI).
 *
 *   pdidump <image.pdi> [--files] [--scan]
 *
 * Dumps the physical-volume root (page 0), the subvolume table, and a
 * label scan (attribute histogram + located boot files). Used to verify a
 * Cedar Dorado boot image is well-formed before wiring it into the emulator.
 * See docs/cedar-disk-boot-plan.md, CedarDisk/PARC_PILOT_FORMAT.md.
 */
#include "pdi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Pilot attribute values (PARC_PILOT_FORMAT.md §2.2). */
static const char *attr_name(uint16_t a)
{
    switch (a) {
    case 1:    return "physicalRoot";
    case 2:    return "badPageList";
    case 3:    return "badPage";
    case 4:    return "subVolumeMarker";
    case 5:    return "logicalRoot";
    case 9728: return "freePage";
    case 9729: return "header";
    case 9730: return "data";
    default:   return "?";
    }
}

/* 32-bit Mesa INT/LONG: low word first (PARC_PILOT_FORMAT.md header note). */
static uint32_t long_lo_first(const uint16_t *w, int i)
{
    return (uint32_t)w[i] | ((uint32_t)w[i + 1] << 16);
}

static void dump_pv_root(const dorado_pdi *p)
{
    const uint16_t *d = dorado_pdi_page_data(p, 0);
    const uint16_t *l = dorado_pdi_page_label(p, 0);
    if (!d) { printf("no page 0\n"); return; }

    printf("== Physical Volume root (page 0) ==\n");
    printf("  seal           = 0%o %s\n", d[0], d[0] == 0121212 ? "(PRSeal OK)" : "(BAD)");
    printf("  version        = %u %s\n", d[1], d[1] == 6 ? "(OK)" : "(expect 6)");
    printf("  labelLength    = %u\n", d[2]);

    /* label string at word 44, packed chars [0..40) */
    printf("  label          = \"");
    for (uint16_t i = 0; i < d[2] && i < 40; i++) {
        const uint8_t *b = (const uint8_t *)&d[44];
        /* big-endian word packing: char i = high byte of word i/2 if even */
        uint16_t wrd = d[44 + i / 2];
        char c = (i & 1) ? (char)(wrd & 0xFF) : (char)(wrd >> 8);
        putchar(c >= 32 && c < 127 ? c : '.');
        (void)b;
    }
    printf("\"\n");

    /* bootingInfo at word 8: [checkpoint, microcode, germ, bootFile] x DiskFileID(9w) */
    static const char *bn[] = { "checkpoint", "microcode", "germ", "bootFile" };
    printf("  bootingInfo:\n");
    for (int i = 0; i < 4; i++) {
        const uint16_t *e = d + 8 + i * 9;
        uint32_t fileID = long_lo_first(e, 0);      /* nucleus FileID (words 0-1) */
        uint32_t firstPage = long_lo_first(e, 5);
        printf("    %-11s fileID=%u firstPage=%u firstLink=%u,%u\n",
               bn[i], fileID, firstPage, e[7], e[8]);
    }

    printf("  subVolumeCount = %u\n", d[64]);

    /* checksum verify: computed over words [0..255), stored at word 255 */
    uint16_t cs = dorado_pilot_checksum(d, 255);
    printf("  checksum       = stored 0x%04X, computed 0x%04X %s\n",
           d[255], cs, cs == d[255] ? "(OK)" : "(MISMATCH)");

    printf("  page0 label    = fileID=%u filePage=%u attributes=%u (%s)\n",
           long_lo_first(l, 0), long_lo_first(l, 5), l[7], attr_name(l[7]));
}

static void dump_subvolumes(const dorado_pdi *p)
{
    const uint16_t *d = dorado_pdi_page_data(p, 0);
    if (!d) return;
    uint16_t n = d[64];
    if (n == 0 || n > 6) { printf("== Subvolumes: count=%u (none/invalid) ==\n", n); return; }
    printf("== Subvolume table (%u) ==\n", n);
    for (uint16_t i = 0; i < n; i++) {
        const uint16_t *s = d + 75 + i * 13;   /* SubVolumeDesc(13w) */
        printf("  sv[%u]: lvSize=%u lvPage=%u pvPage=%u nPages=%u\n",
               i, long_lo_first(s, 5), long_lo_first(s, 7),
               long_lo_first(s, 9), long_lo_first(s, 11));
    }
}

static void label_scan(const dorado_pdi *p, int list_files)
{
    printf("== Label scan (%u pages) ==\n", p->page_count);
    /* attribute histogram */
    uint32_t hist[10] = {0}; uint32_t other = 0;
    static const uint16_t known[] = {1,2,3,4,5,9728,9729,9730};
    for (uint32_t pg = 0; pg < p->page_count; pg++) {
        const uint16_t *l = dorado_pdi_page_label(p, pg);
        uint16_t a = l[7];
        int matched = 0;
        for (size_t k = 0; k < sizeof known / sizeof known[0]; k++)
            if (a == known[k]) { hist[k]++; matched = 1; break; }
        if (!matched) other++;
    }
    for (size_t k = 0; k < sizeof known / sizeof known[0]; k++)
        if (hist[k]) printf("  %6u  %-16s (attr %u)\n", hist[k], attr_name(known[k]), known[k]);
    if (other) printf("  %6u  (other attributes)\n", other);

    if (!list_files) return;

    /* locate the germ (FileID 2) and bootFile (FileID 3) page runs */
    for (uint32_t fid = 2; fid <= 3; fid++) {
        printf("  file FileID=%u pages:", fid);
        uint32_t count = 0, first = 0xFFFFFFFFu, last = 0;
        for (uint32_t pg = 0; pg < p->page_count; pg++) {
            const uint16_t *l = dorado_pdi_page_label(p, pg);
            if (long_lo_first(l, 0) == fid) {
                count++;
                if (pg < first) first = pg;
                if (pg > last) last = pg;
            }
        }
        if (count) printf(" %u pages, pvPage range [%u..%u]\n", count, first, last);
        else printf(" (none found)\n");
    }
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s <image.pdi> [--files] [--scan]\n", argv[0]);
        return 2;
    }
    int files = 0, scan = 0;
    for (int i = 2; i < argc; i++) {
        if (!strcmp(argv[i], "--files")) files = 1;
        else if (!strcmp(argv[i], "--scan")) scan = 1;
    }

    dorado_pdi pdi;
    char err[128];
    if (dorado_pdi_load(argv[1], &pdi, err, sizeof err) != 0) {
        fprintf(stderr, "load failed: %s\n", err);
        return 1;
    }

    printf("== PDI header ==\n");
    printf("  version=%u fsFamily=%u pages=%u labelWords=%u dataWords=%u\n",
           pdi.version, pdi.fs_family, pdi.page_count, pdi.label_words, pdi.data_words);
    dump_pv_root(&pdi);
    dump_subvolumes(&pdi);
    if (scan || files) label_scan(&pdi, files);

    dorado_pdi_free(&pdi);
    return 0;
}
