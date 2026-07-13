/*
 * pdidump.c — offline inspector for a Pilot/Cedar PARC Disk Image (PDI).
 *
 *   pdidump <image.pdi> [--files] [--scan] [--verify] [--locate FILEID]
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

static int extract_file(const dorado_pdi *p, uint32_t fid, const char *out)
{
    /* size the file, then read it in file-page order */
    long pages = dorado_pdi_read_file(p, fid, NULL, 0);
    if (pages <= 0) { fprintf(stderr, "FileID %u: no content pages\n", fid); return 1; }
    size_t words = (size_t)pages * p->data_words;
    uint16_t *buf = calloc(words, sizeof(uint16_t));
    dorado_pdi_read_file(p, fid, buf, words);
    FILE *f = fopen(out, "wb");
    if (!f) { free(buf); fprintf(stderr, "cannot write %s\n", out); return 1; }
    /* big-endian on disk; emit big-endian bytes */
    for (size_t i = 0; i < words; i++) { fputc(buf[i] >> 8, f); fputc(buf[i] & 0xFF, f); }
    fclose(f);
    printf("extracted FileID=%u: %ld pages, %zu words -> %s (data word0=0%o, word1=0%o)\n",
           fid, pages, words, out, buf[0], buf[1]);
    free(buf);
    return 0;
}

static void locate_file(const dorado_pdi *p, uint32_t fid)
{
    uint32_t count = 0;
    printf("== FileID %u locations ==\n", fid);
    for (uint32_t pg = 0; pg < p->page_count; pg++) {
        const uint16_t *l = dorado_pdi_page_label(p, pg);
        if (dorado_pdi_label_fileid(l) != fid) continue;
        printf("  pvPage=%u filePage=%u attr=%u (%s)\n", pg,
               dorado_pdi_label_filepage(l), dorado_pdi_label_attr(l),
               attr_name(dorado_pdi_label_attr(l)));
        count++;
    }
    if (!count) printf("  (none)\n");
}

static int verify_cedar_volume(const dorado_pdi *p)
{
    const uint16_t *pv = dorado_pdi_page_data(p, 0);
    enum { ROOTFILE_VAM_WORD = 85 + 7 * 6,
           ROOTFILE_CLIENT_WORD = 85 + 8 * 6 };
    if (!pv || pv[64] == 0) {
        printf("== Cedar volume verification ==\n  FAIL: no subvolume\n");
        return 1;
    }
    uint32_t lv_page = long_lo_first(pv + 75, 7);
    uint32_t pv_page = long_lo_first(pv + 75, 9);
    uint32_t n_pages = long_lo_first(pv + 75, 11);
    const uint16_t *lv = dorado_pdi_page_data(p, pv_page);
    printf("== Cedar volume verification ==\n");
    if (!lv || pv_page + n_pages > p->page_count) {
        printf("  FAIL: subvolume range is outside the image\n");
        return 1;
    }
    printf("  LV root       seal=0%o version=%u type=%u checksum=%s\n",
           lv[0], lv[1], lv[28],
           dorado_pilot_checksum(lv, 255) == lv[255] ? "OK" : "MISMATCH");

    uint32_t vam_fid = long_lo_first(lv, ROOTFILE_VAM_WORD);
    uint32_t vam_lp = long_lo_first(lv, ROOTFILE_VAM_WORD + 2);
    uint32_t vam_hint = long_lo_first(lv, ROOTFILE_VAM_WORD + 4);
    uint32_t client_fid = long_lo_first(lv, ROOTFILE_CLIENT_WORD);
    uint32_t client_lp = long_lo_first(lv, ROOTFILE_CLIENT_WORD + 2);
    uint32_t client_hint = long_lo_first(lv, ROOTFILE_CLIENT_WORD + 4);
    printf("  rootFile[VAM] id=%u fp.da=%u page=%u\n",
           vam_fid, vam_lp, vam_hint);
    printf("  rootFile[client] id=%u fp.da=%u page=%u%s\n",
           client_fid, client_lp, client_hint,
           client_fid && (!client_lp || client_lp != client_hint)
               ? " (BAD HINT)" : "");

    if (vam_lp < lv_page || vam_lp - lv_page >= n_pages) {
        printf("  FAIL: VAM leader logical page is outside subvolume\n");
        return 1;
    }
    uint32_t vam_pv = pv_page + vam_lp - lv_page;
    const uint16_t *vh = dorado_pdi_page_data(p, vam_pv);
    const uint16_t *vl = dorado_pdi_page_label(p, vam_pv);
    if (!vh || dorado_pdi_label_fileid(vl) != vam_fid ||
        dorado_pdi_label_attr(vl) != 9729) {
        printf("  FAIL: VAM hint does not identify its header label\n");
        return 1;
    }

    /* VolumeFormat.LogicalRunObject: headerPages/maxRuns precede the run
     * sequence at word 5; each run is {first LONG, size CARDINAL}. */
    uint32_t header_pages = vh[0];
    uint32_t run_first = long_lo_first(vh, 5);
    uint32_t run_pages = vh[7];
    uint32_t data_lp = run_first + header_pages;
    uint32_t data_pages = run_pages >= header_pages
        ? run_pages - header_pages : 0;
    uint32_t last = long_lo_first(vh, 8);
    printf("  VAM run       leader=%u headers=%u first=%u count=%u "
           "data=%u+%u terminator=0x%08X\n",
           vam_lp, header_pages, run_first, run_pages, data_lp, data_pages,
           last);
    if (!data_pages || data_lp < lv_page ||
        data_lp - lv_page + data_pages > n_pages || last != 0x7FFFFFFFu) {
        printf("  FAIL: malformed VAM run table\n");
        return 1;
    }

    uint32_t data_pv = pv_page + data_lp - lv_page;
    const uint16_t *v0 = dorado_pdi_page_data(p, data_pv);
    uint32_t bitmap_size = v0 ? long_lo_first(v0, 2) : 0;
    printf("  VAM object    size=%u (subvolume=%u)\n", bitmap_size, n_pages);
    if (!v0 || bitmap_size != n_pages) {
        printf("  FAIL: VAM size disagrees with subvolume size\n");
        return 1;
    }

    uint32_t label_free = 0, bitmap_free = 0, mismatches = 0;
    for (uint32_t lp = 0; lp < n_pages; lp++) {
        const uint16_t *label = dorado_pdi_page_label(p, pv_page + lp);
        int label_used = dorado_pdi_label_attr(label) != 9728;
        uint32_t wi = 4u + lp / 16u;
        uint32_t dp = wi / p->data_words;
        uint32_t off = wi % p->data_words;
        const uint16_t *vd = dp < data_pages
            ? dorado_pdi_page_data(p, data_pv + dp) : NULL;
        int bitmap_used = vd && (vd[off] & (uint16_t)(1u << (lp % 16u)));
        if (!label_used) label_free++;
        if (!bitmap_used) bitmap_free++;
        if (label_used != bitmap_used) {
            if (mismatches < 8)
                printf("    mismatch logicalPage=%u label=%s bitmap=%s\n",
                       lp, label_used ? "used" : "free",
                       bitmap_used ? "used" : "free");
            mismatches++;
        }
    }
    printf("  allocation    labels=%u free bitmap=%u free mismatches=%u %s\n",
           label_free, bitmap_free, mismatches,
           mismatches ? "(FAIL)" : "(OK)");
    return mismatches ? 1 : 0;
}

/* Kept only to reject the obsolete command explicitly.  The historical
 * writer's defect affects every file header and the VAM size, so patching the
 * client-root hint alone produces a volume that looks plausible to an offline
 * reader but cannot be mounted by Cedar.  Regenerate with corrected Rusty
 * Backup instead. */
static int repair_client_hint(const char *input, dorado_pdi *p,
                              const char *output)
{
    (void)input;
    (void)p;
    (void)output;
    fprintf(stderr, "--repair-cedar is unsafe for old Rusty Backup images; "
                    "regenerate or repack the volume instead\n");
    return 1;
#if 0
    uint16_t *pv = p->data;
    if (pv[64] == 0) {
        fprintf(stderr, "repair-client: no subvolume\n");
        return 1;
    }
    uint32_t lv_page = long_lo_first(pv + 75, 7);
    uint32_t pv_page = long_lo_first(pv + 75, 9);
    if (pv_page >= p->page_count) {
        fprintf(stderr, "repair-client: LV root is outside image\n");
        return 1;
    }
    uint16_t *lv = p->data + (size_t)pv_page * p->data_words;
    enum { ROOTFILE_VAM_WORD = 85 + 7 * 6,
           ROOTFILE_CLIENT_WORD = 85 + 8 * 6 };
    uint32_t fid = long_lo_first(lv, ROOTFILE_CLIENT_WORD);
    if (fid == 0) {
        fprintf(stderr, "repair-client: no client root FileID\n");
        return 1;
    }
    uint32_t header_pv = UINT32_MAX;
    for (uint32_t pg = 0; pg < p->page_count; pg++) {
        const uint16_t *l = dorado_pdi_page_label(p, pg);
        if (dorado_pdi_label_fileid(l) == fid &&
            dorado_pdi_label_attr(l) == 9729 &&
            dorado_pdi_label_filepage(l) == 0) {
            header_pv = pg;
            break;
        }
    }
    if (header_pv == UINT32_MAX || header_pv < pv_page) {
        fprintf(stderr, "repair-client: FileID %u has no leader in subvolume\n",
                fid);
        return 1;
    }
    uint32_t header_lv = lv_page + header_pv - pv_page;
    lv[ROOTFILE_CLIENT_WORD + 2] = (uint16_t)header_lv;
    lv[ROOTFILE_CLIENT_WORD + 3] = (uint16_t)(header_lv >> 16);
    lv[ROOTFILE_CLIENT_WORD + 4] = (uint16_t)header_lv;
    lv[ROOTFILE_CLIENT_WORD + 5] = (uint16_t)(header_lv >> 16);
    lv[255] = dorado_pilot_checksum(lv, 255);

    uint32_t volume_size = long_lo_first(lv, 29);
    uint32_t vam_header_lv = long_lo_first(lv, ROOTFILE_VAM_WORD + 2);
    uint32_t vam_header_pv = pv_page + vam_header_lv - lv_page;
    if (vam_header_pv >= p->page_count) {
        fprintf(stderr, "repair-client: VAM leader is outside image\n");
        return 1;
    }
    uint16_t *vam = p->data + (size_t)vam_header_pv * p->data_words;
    uint32_t vam_words = 4u + (volume_size + 15u) / 16u;
    uint16_t vam_pages = (uint16_t)((vam_words + 255u) / 256u);
    /* Rusty Backup's older writer placed the run at word 4 and transposed
     * first/size, yielding {first=nPages,size=1}.  VolumeFormat's
     * LogicalRunObject begins its run sequence at word 5. */
    vam[4] = 0;
    vam[5] = (uint16_t)(vam_header_lv + 1u);
    vam[6] = (uint16_t)((vam_header_lv + 1u) >> 16);
    vam[7] = vam_pages;
    vam[8] = 0xFFFFu;
    vam[9] = 0x7FFFu;

    FILE *in = fopen(input, "rb");
    if (!in) { fprintf(stderr, "repair-client: cannot reopen input\n"); return 1; }
    if (fseek(in, 0, SEEK_END) != 0) { fclose(in); return 1; }
    long size = ftell(in);
    rewind(in);
    uint8_t *raw = malloc((size_t)size);
    if (!raw || fread(raw, 1, (size_t)size, in) != (size_t)size) {
        free(raw); fclose(in); return 1;
    }
    fclose(in);
    size_t sector_bytes = 2u * (p->label_words + p->data_words);
    size_t base = DORADO_PDI_HEADER_BYTES + (size_t)pv_page * sector_bytes +
                  2u * p->label_words;
    static const int patch_words[] = {
        ROOTFILE_CLIENT_WORD + 2, ROOTFILE_CLIENT_WORD + 3,
        ROOTFILE_CLIENT_WORD + 4, ROOTFILE_CLIENT_WORD + 5, 255
    };
    for (size_t i = 0; i < sizeof patch_words / sizeof patch_words[0]; i++) {
        size_t off = base + 2u * (size_t)patch_words[i];
        raw[off] = (uint8_t)(lv[patch_words[i]] >> 8);
        raw[off + 1] = (uint8_t)lv[patch_words[i]];
    }
    size_t vam_base = DORADO_PDI_HEADER_BYTES +
                      (size_t)vam_header_pv * sector_bytes +
                      2u * p->label_words;
    for (int w = 4; w <= 9; w++) {
        size_t off = vam_base + 2u * (size_t)w;
        raw[off] = (uint8_t)(vam[w] >> 8);
        raw[off + 1] = (uint8_t)vam[w];
    }
    FILE *out = fopen(output, "wb");
    int ok = out && fwrite(raw, 1, (size_t)size, out) == (size_t)size;
    if (out) fclose(out);
    free(raw);
    if (!ok) { fprintf(stderr, "repair-client: cannot write output\n"); return 1; }
    printf("repaired VAM run: logicalPage=%u dataPage=%u count=%u\n",
           vam_header_lv, vam_header_lv + 1u, vam_pages);
    printf("repaired client FileID=%u hint: logicalPage=%u (pvPage=%u) -> %s\n",
           fid, header_lv, header_pv, output);
    return 0;
#endif
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s <image.pdi> [--files] [--scan] "
                        "[--verify] [--locate FILEID] [--extract FILEID OUT] "
                        "[--repair-cedar OUT]\n", argv[0]);
        return 2;
    }
    int files = 0, scan = 0, verify = 0;
    uint32_t locate_fid = 0, ext_fid = 0;
    int locate = 0;
    const char *ext_out = NULL, *repair_out = NULL;
    for (int i = 2; i < argc; i++) {
        if (!strcmp(argv[i], "--files")) files = 1;
        else if (!strcmp(argv[i], "--scan")) scan = 1;
        else if (!strcmp(argv[i], "--verify")) verify = 1;
        else if (!strcmp(argv[i], "--locate") && i + 1 < argc) {
            locate_fid = (uint32_t)strtoul(argv[++i], NULL, 0);
            locate = 1;
        }
        else if (!strcmp(argv[i], "--extract") && i + 2 < argc) {
            ext_fid = (uint32_t)strtoul(argv[i + 1], NULL, 0);
            ext_out = argv[i + 2];
            i += 2;
        }
        else if ((!strcmp(argv[i], "--repair-cedar") ||
                  !strcmp(argv[i], "--repair-client")) && i + 1 < argc)
            repair_out = argv[++i];
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
    if (verify && verify_cedar_volume(&pdi) != 0) {
        dorado_pdi_free(&pdi);
        return 1;
    }
    if (locate) locate_file(&pdi, locate_fid);
    if (ext_out) extract_file(&pdi, ext_fid, ext_out);
    if (repair_out && repair_client_hint(argv[1], &pdi, repair_out) != 0) {
        dorado_pdi_free(&pdi);
        return 1;
    }

    dorado_pdi_free(&pdi);
    return 0;
}
