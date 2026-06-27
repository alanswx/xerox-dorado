/* dsk2trident — convert a ContrAlto Diablo-Model-31 .dsk image into a
 * Diablo-on-Trident pack that the Alto-on-Dorado emulator (AEmu) can boot.
 *
 * Background. The AEmu world emulates an Alto Diablo disk on a single head
 * ("partition") of a Trident drive, which it low-level formats as
 * 815 cyl x 5 heads x 29 short sectors, each 2 header + 8 label + 256 data
 * words (DORADO_DISK_DIABLO). AltoDiabloDisk.mc maps a Diablo disk address
 * (cyl, head, sector) to the physical Trident CHS:
 *
 *   DoradoCyl    = 406 * diabloDrive + diabloCyl + offsetCylinderDiablo(=3)
 *   DoradoHead   = partition - 1            [default partition 5 (T-80) -> 4]
 *   effHead      = diabloHead XOR (diabloCyl & 1)              [staggerSectors]
 *   DoradoSector = nSectorsDiablo(=16B=14) * effHead + diabloSector
 *
 * (Constants verified octal in the Micro source: nSectorsDiablo 16B = 14,
 * offsetCylinderDiablo 3, MaxPartition 5C for a T-80; the "626B = 406"
 * comment in ASeek pins the octal convention. See
 * chm/doradosource/AEmuSources-cedar6.0.dm!1_/AltoDiabloDisk.mc and
 * docs/alto-disk-boot-plan.md.)
 *
 * Each Diablo sector's header/label/data records are copied into the mapped
 * Trident sector. The header block carries the Alto disk address that the
 * microcode's header-check compares; for the second emulated Diablo drive we
 * set the AEmu drive bit in that header word. Conveniently a Diablo .dsk
 * sector and a Diablo-on-Trident sector have the identical 534-byte on-disk
 * layout (dummy 1w + header 2w + label 8w + data 256w, little-endian); this
 * tool just places each one at its mapped position in the larger Trident
 * geometry. The default input geometry is Diablo-31; Lisp-sized AEmu images can
 * use --diablo-cylinders 406 --diablo-sectors 14 --drive1 second.dsk. Use
 * --base existing.pack to overlay another partition without clearing the pack.
 *
 * Do not use --remap-vda for production pack generation. It is a debugging aid
 * for experiments with 12-sector source images whose filesystem metadata claims
 * 14-sector addressing; real Alto utilities can still carry absolute disk
 * addresses that make host-side remapping ambiguous.
 */

#include "disk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ContrAlto Diablo Model 31 geometry (DiskGeometry.Diablo31). */
#define DIABLO31_CYL   203
#define DIABLO31_HEAD  2
#define DIABLO31_SEC   12
/* AltoDiabloDisk.mc: "626B = 406 = number of cylinders". */
#define DIABLO_DRIVE_CYL_STRIDE 406
#define DIABLO_DRIVE_HEADER_BIT 000002u
/* dummy 1w + header 2w + label 8w + data 256w = 267 words = 534 bytes. */
#define DIABLO_DUMMY_W  1
#define DIABLO_HDR_W    2
#define DIABLO_LBL_W    8
#define DIABLO_DATA_W   256
#define DIABLO_SEC_W    (DIABLO_DUMMY_W + DIABLO_HDR_W + DIABLO_LBL_W + DIABLO_DATA_W)
#define DIABLO_SEC_B    (DIABLO_SEC_W * 2)

static void usage(const char *p)
{
    fprintf(stderr,
        "Usage: %s [options] in.dsk out.trident\n"
        "Convert a ContrAlto Diablo-31 .dsk into a Diablo-on-Trident pack.\n\n"
        "  --partition N     default disk partition the Alto boots from\n"
        "                    (head = N-1; default 5 => head 4, T-80 default)\n"
        "  --base PACK       load an existing Diablo-on-Trident pack first,\n"
        "                    then overlay the converted image onto --partition\n"
        "  --all-heads       replicate the image onto every Trident head\n"
        "                    (boots regardless of the partition the AEmu picks)\n"
        "  --single-drive    populate only emulated Diablo drive 0\n"
        "                    (default mirrors the image onto drives 0 and 1)\n"
        "  --drive1 PATH     populate emulated Diablo drive 1 from PATH instead\n"
        "                    of mirroring the first input image\n"
        "  --diablo-cylinders N  input cylinders per emulated Diablo drive\n"
        "                    (default 203 = Diablo-31; AEmu max is 406)\n"
        "  --diablo-sectors N  input sectors per emulated Diablo head\n"
        "                    (default 12 = Diablo-31; AEmu max is 14)\n"
        "  --sectors-diablo N  Trident sectors reserved per Diablo head\n"
        "                    (default 14 = 16B, AltoDiabloDisk nSectorsDiablo)\n"
        "  --offset-cyl N    cylinders reserved at the start (default 3)\n"
        "  --sector-offset N physical-sector bias (default 1): the controller\n"
        "                    executes a command in the sector AFTER it is issued\n"
        "                    (UpdateSector: logical sector S is recorded at S-1),\n"
        "                    so a command for sector S reads physical sector S+1\n"
        "  --remap-vda       EXPERIMENTAL: when input/output sector counts differ,\n"
        "                    preserve VDA order and rewrite header/label RDAs\n"
        "  --no-stagger      do not flip the head bit on odd cylinders\n",
        p);
}

/* Read one little-endian 16-bit word from a byte buffer. */
static uint16_t rd16(const uint8_t *b) { return (uint16_t)(b[0] | (b[1] << 8)); }

static uint16_t alto_rda_from_vda(int vda, int cylinders, int sectors,
                                  int edrive)
{
    int sector = vda % sectors;
    vda /= sectors;
    int head = vda % DIABLO31_HEAD;
    vda /= DIABLO31_HEAD;
    int cylinder = vda % cylinders;

    return (uint16_t)((cylinder << 3) | (head << 2) |
                      (sector << 12) |
                      (edrive ? DIABLO_DRIVE_HEADER_BIT : 0));
}

static int alto_vda_from_rda(uint16_t rda, int cylinders, int sectors,
                             int *vda)
{
    int cylinder = (rda >> 3) & 0777;
    int head = (rda >> 2) & 1;
    int sector = (rda >> 12) & 017;
    int edrive = (rda >> 1) & 1;

    if ((rda & 1) || cylinder >= cylinders || sector >= sectors)
        return 0;

    *vda = (((edrive * cylinders + cylinder) * DIABLO31_HEAD + head) *
            sectors) + sector;
    return 1;
}

static uint16_t translate_rda(uint16_t rda, int src_cylinders,
                              int src_sectors, int dst_cylinders,
                              int dst_sectors, int edrive)
{
    int vda = 0;
    if (rda == 0 || !alto_vda_from_rda(rda, src_cylinders, src_sectors, &vda))
        return rda;
    return alto_rda_from_vda(vda, dst_cylinders, dst_sectors, edrive);
}

static int place_diablo_image(dorado_disk_pack *pack, const char *path,
                              int edrive, int diablo_cyls, int diablo_secs,
                              int all_heads, int boot_head,
                              int sectors_diablo, int offset_cyl,
                              int sector_offset, int stagger, int remap_vda,
                              int *placed, int *missed)
{
    const dorado_disk_geometry *geom = &pack->geometry;
    FILE *in = fopen(path, "rb");
    if (!in) { perror(path); return 1; }
    if (fseek(in, 0, SEEK_END) != 0) {
        perror("fseek");
        fclose(in);
        return 1;
    }
    long sz = ftell(in);
    rewind(in);

    long expect = (long)diablo_cyls * DIABLO31_HEAD * diablo_secs * DIABLO_SEC_B;
    if (sz != expect) {
        fprintf(stderr,
                "dsk2trident: %s is %ld bytes, expected %ld "
                "(Diablo-like: %dx%dx%d x %d)\n",
                path, sz, expect, diablo_cyls, DIABLO31_HEAD,
                diablo_secs, DIABLO_SEC_B);
        fclose(in);
        return 1;
    }

    uint8_t buf[DIABLO_SEC_B];
    for (int dcyl = 0; dcyl < diablo_cyls; dcyl++) {
        for (int dhead = 0; dhead < DIABLO31_HEAD; dhead++) {
            for (int dsec = 0; dsec < diablo_secs; dsec++) {
                if (fread(buf, 1, sizeof buf, in) != sizeof buf) {
                    fprintf(stderr, "dsk2trident: short read at drive%d c%d/h%d/s%d\n",
                            edrive, dcyl, dhead, dsec);
                    fclose(in);
                    return 1;
                }

                int src_vda = (dcyl * DIABLO31_HEAD + dhead) * diablo_secs + dsec;
                int dst_cyl = dcyl;
                int dst_head = dhead;
                int dst_sec = dsec;
                if (remap_vda && diablo_secs != sectors_diablo) {
                    int v = src_vda;
                    dst_sec = v % sectors_diablo;
                    v /= sectors_diablo;
                    dst_head = v % DIABLO31_HEAD;
                    v /= DIABLO31_HEAD;
                    dst_cyl = v;
                }

                if (dst_cyl >= diablo_cyls) {
                    (*missed)++;
                    continue;
                }

                uint16_t hdr[DIABLO_HDR_W];
                uint16_t lbl[DIABLO_LBL_W];
                uint16_t dat[DIABLO_DATA_W];
                const uint8_t *p = buf + DIABLO_DUMMY_W * 2;
                for (int w = 0; w < DIABLO_HDR_W; w++) {
                    hdr[w] = rd16(p);
                    p += 2;
                }
                for (int w = 0; w < DIABLO_LBL_W; w++) {
                    lbl[w] = rd16(p);
                    p += 2;
                }
                for (int w = 0; w < DIABLO_DATA_W; w++) {
                    dat[w] = rd16(p);
                    p += 2;
                }

                /* AEmu addresses Diablo sectors with nSectorsDiablo (14).
                 * Stock Diablo-31 images are 12-sector containers but their
                 * Alto filesystem code may compute RDAs with a 14-sector
                 * DiskDescriptor. Preserve file VDA order while rewriting
                 * header and label links into the destination RDA space. */
                if (remap_vda && diablo_secs != sectors_diablo) {
                    hdr[1] = alto_rda_from_vda(src_vda, diablo_cyls,
                                               sectors_diablo, edrive);
                    lbl[0] = translate_rda(lbl[0], diablo_cyls, diablo_secs,
                                           diablo_cyls, sectors_diablo, edrive);
                    lbl[1] = translate_rda(lbl[1], diablo_cyls, diablo_secs,
                                           diablo_cyls, sectors_diablo, edrive);
                } else if (edrive == 1) {
                    hdr[1] |= DIABLO_DRIVE_HEADER_BIT;
                    lbl[0] = translate_rda(lbl[0], diablo_cyls, diablo_secs,
                                           diablo_cyls, diablo_secs, edrive);
                    lbl[1] = translate_rda(lbl[1], diablo_cyls, diablo_secs,
                                           diablo_cyls, diablo_secs, edrive);
                }

                int eff_head = dst_head;
                if (stagger && (dst_cyl & 1)) eff_head ^= 1;
                int tsec = (sectors_diablo * eff_head + dst_sec + sector_offset)
                           % geom->sectors;

                int h0 = all_heads ? 0 : boot_head;
                int h1 = all_heads ? geom->heads : boot_head + 1;
                int tcyl = dst_cyl + offset_cyl +
                           edrive * DIABLO_DRIVE_CYL_STRIDE;
                for (int thead = h0; thead < h1; thead++) {
                    dorado_disk_sector *s =
                        dorado_disk_pack_sector(pack, tcyl, thead, tsec);
                    if (!s) { (*missed)++; continue; }
                    /* Skip the dummy word; copy header/label/data with each block's
                     * word order REVERSED. AltoDiabloDisk.mc reads each block out of
                     * the FIFO into descending memory addresses (DskMAddr counts
                     * down), so the on-Trident block must be stored reversed for the
                     * read to land word 0 at the low (entry) address -- matching a
                     * real Alto disk read. */
                    for (int w = DIABLO_HDR_W  - 1; w >= 0; w--) s->header[w] = hdr[DIABLO_HDR_W - 1 - w];
                    for (int w = DIABLO_LBL_W  - 1; w >= 0; w--) s->label[w]  = lbl[DIABLO_LBL_W - 1 - w];
                    for (int w = DIABLO_DATA_W - 1; w >= 0; w--) s->data[w]   = dat[DIABLO_DATA_W - 1 - w];
                    s->modified = 1;
                    (*placed)++;
                }
            }
        }
    }

    fclose(in);
    return 0;
}

int main(int argc, char **argv)
{
    int partition = 5;          /* MaxPartition for a T-80 (default boot) */
    int all_heads = 0;
    int both_drives = 1;
    int diablo_cyls = DIABLO31_CYL;
    int diablo_secs = DIABLO31_SEC;
    int sectors_diablo = 14;    /* nSectorsDiablo = 16B */
    int offset_cyl = 3;         /* offsetCylinderDiablo */
    int sector_offset = 1;      /* command-vs-physical sector bias (see usage) */
    int stagger = 1;            /* staggerSectors */
    int remap_vda = 0;          /* experimental 12-sector->14-sector VDA remap */
    const char *in_path = NULL, *out_path = NULL;
    const char *base_path = NULL;
    const char *drive1_path = NULL;

    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];
        if (!strcmp(a, "--partition") && i + 1 < argc)        partition = atoi(argv[++i]);
        else if (!strcmp(a, "--base") && i + 1 < argc)        base_path = argv[++i];
        else if (!strcmp(a, "--all-heads"))                   all_heads = 1;
        else if (!strcmp(a, "--single-drive"))                both_drives = 0;
        else if (!strcmp(a, "--drive1") && i + 1 < argc)      { drive1_path = argv[++i]; both_drives = 0; }
        else if (!strcmp(a, "--disk2") && i + 1 < argc)       { drive1_path = argv[++i]; both_drives = 0; }
        else if (!strcmp(a, "--diablo-cylinders") && i + 1 < argc) diablo_cyls = atoi(argv[++i]);
        else if (!strcmp(a, "--diablo-sectors") && i + 1 < argc) diablo_secs = atoi(argv[++i]);
        else if (!strcmp(a, "--sectors-diablo") && i + 1 < argc) sectors_diablo = atoi(argv[++i]);
        else if (!strcmp(a, "--offset-cyl") && i + 1 < argc)  offset_cyl = atoi(argv[++i]);
        else if (!strcmp(a, "--sector-offset") && i + 1 < argc) sector_offset = atoi(argv[++i]);
        else if (!strcmp(a, "--remap-vda"))                   remap_vda = 1;
        else if (!strcmp(a, "--no-stagger"))                  stagger = 0;
        else if (!strcmp(a, "-h") || !strcmp(a, "--help"))    { usage(argv[0]); return 0; }
        else if (a[0] == '-')                                 { usage(argv[0]); return 2; }
        else if (!in_path)                                    in_path = a;
        else if (!out_path)                                   out_path = a;
        else                                                  { usage(argv[0]); return 2; }
    }
    if (!in_path || !out_path) { usage(argv[0]); return 2; }

    /* The output pack uses the Diablo-on-Trident geometry (2/8/256 framing). */
    dorado_disk_geometry geom = DORADO_DISK_DIABLO;
    int drive_ranges = (both_drives || drive1_path) ? 2 : 1;
    int required_cyls = offset_cyl + diablo_cyls +
                        (drive_ranges - 1) * DIABLO_DRIVE_CYL_STRIDE;
    if (required_cyls > geom.cylinders) {
        fprintf(stderr,
                "dsk2trident: DORADO_DISK_DIABLO has %d cylinders; need >= %d "
                "(offset %d + %d Diablo cyls + %d drive stride)\n",
                geom.cylinders, required_cyls, offset_cyl, diablo_cyls,
                (drive_ranges - 1) * DIABLO_DRIVE_CYL_STRIDE);
        return 1;
    }
    if (diablo_cyls < 1 || diablo_cyls > DIABLO_DRIVE_CYL_STRIDE ||
        diablo_secs < 1 || diablo_secs > sectors_diablo) {
        fprintf(stderr,
                "dsk2trident: invalid Diablo-like input geometry %dx2x%d "
                "(limits: cylinders 1..%d, sectors 1..%d)\n",
                diablo_cyls, diablo_secs, DIABLO_DRIVE_CYL_STRIDE,
                sectors_diablo);
        return 1;
    }

    int boot_head = partition - 1;
    if (!all_heads && (boot_head < 0 || boot_head >= geom.heads)) {
        fprintf(stderr, "dsk2trident: partition %d -> head %d out of range [0,%d)\n",
                partition, boot_head, geom.heads);
        return 1;
    }

    dorado_disk_pack pack;
    if (base_path) {
        if (dorado_disk_pack_load(&pack, &geom, base_path) != 0) {
            fprintf(stderr, "dsk2trident: failed to load base pack %s\n",
                    base_path);
            return 1;
        }
    } else {
        if (dorado_disk_pack_create(&pack, &geom) != 0) {
            fprintf(stderr, "dsk2trident: failed to allocate Trident pack\n");
            return 1;
        }
    }

    int placed = 0, missed = 0;
    if (place_diablo_image(&pack, in_path, 0, diablo_cyls, diablo_secs,
                           all_heads, boot_head, sectors_diablo, offset_cyl,
                           sector_offset, stagger, remap_vda,
                           &placed, &missed) != 0) {
        dorado_disk_pack_free(&pack);
        return 1;
    }
    if (drive1_path || both_drives) {
        const char *path1 = drive1_path ? drive1_path : in_path;
        if (place_diablo_image(&pack, path1, 1, diablo_cyls, diablo_secs,
                               all_heads, boot_head, sectors_diablo, offset_cyl,
                               sector_offset, stagger, remap_vda,
                               &placed, &missed) != 0) {
            dorado_disk_pack_free(&pack);
            return 1;
        }
    }

    if (missed) {
        fprintf(stderr, "dsk2trident: warning: %d sectors fell outside the "
                "Trident geometry (check --offset-cyl/--sectors-diablo)\n", missed);
    }

    snprintf(pack.path, sizeof pack.path, "%s", out_path);
    pack.read_only = 0;
    if (dorado_disk_pack_save(&pack) != 0) {
        fprintf(stderr, "dsk2trident: failed to write %s\n", out_path);
        dorado_disk_pack_free(&pack);
        return 1;
    }
    dorado_disk_pack_free(&pack);

    printf("dsk2trident: %s -> %s\n", in_path, out_path);
    if (base_path) printf("  base      %s\n", base_path);
    printf("  geometry  %d cyl x %d head x %d sec (2/8/256 words/sector)\n",
           geom.cylinders, geom.heads, geom.sectors);
    printf("  mapping   cyl=%d*drive+cyl+%d, sector=%d*head+sec+%d, stagger=%s, ",
           DIABLO_DRIVE_CYL_STRIDE, offset_cyl, sectors_diablo,
           sector_offset, stagger ? "on" : "off");
    if (all_heads) printf("all heads\n");
    else           printf("partition %d -> head %d\n", partition, boot_head);
    printf("  input     %d cyl x 2 head x %d sec\n", diablo_cyls, diablo_secs);
    if (remap_vda) printf("  remap     experimental VDA/RDA rewrite enabled\n");
    if (drive1_path) printf("  drives    0=%s, 1=%s\n", in_path, drive1_path);
    else             printf("  drives    %s\n", both_drives ? "0 and 1 (mirrored)" : "0 only");
    printf("  placed    %d sectors%s\n", placed,
           missed ? " (some missed, see warning)" : "");
    return 0;
}
