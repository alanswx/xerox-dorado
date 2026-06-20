/* dsk2trident — convert a ContrAlto Diablo-Model-31 .dsk image into a
 * Diablo-on-Trident pack that the Alto-on-Dorado emulator (AEmu) can boot.
 *
 * Background. The AEmu world emulates an Alto Diablo disk on a single head
 * ("partition") of a Trident drive, which it low-level formats as
 * 815 cyl x 5 heads x 29 short sectors, each 2 header + 8 label + 256 data
 * words (DORADO_DISK_DIABLO). AltoDiabloDisk.mc maps a Diablo disk address
 * (cyl, head, sector) to the physical Trident CHS:
 *
 *   DoradoCyl    = diabloCyl + offsetCylinderDiablo(=3)        [drive 0]
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
 * Each Diablo sector's header/label/data records are copied verbatim into the
 * mapped Trident sector. The header block carries the Alto disk address that
 * the microcode's header-check compares, so no translation is needed -- only
 * the CHS reordering. Conveniently a Diablo-31 .dsk sector and a
 * Diablo-on-Trident sector have the identical 534-byte on-disk layout
 * (dummy 1w + header 2w + label 8w + data 256w, little-endian); this tool
 * just places each one at its mapped position in the larger Trident geometry.
 */

#include "disk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ContrAlto Diablo Model 31 geometry (DiskGeometry.Diablo31). */
#define DIABLO31_CYL   203
#define DIABLO31_HEAD  2
#define DIABLO31_SEC   12
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
        "  --all-heads       replicate the image onto every Trident head\n"
        "                    (boots regardless of the partition the AEmu picks)\n"
        "  --sectors-diablo N  Trident sectors reserved per Diablo head\n"
        "                    (default 14 = 16B, AltoDiabloDisk nSectorsDiablo)\n"
        "  --offset-cyl N    cylinders reserved at the start (default 3)\n"
        "  --sector-offset N physical-sector bias (default 1): the controller\n"
        "                    executes a command in the sector AFTER it is issued\n"
        "                    (UpdateSector: logical sector S is recorded at S-1),\n"
        "                    so a command for sector S reads physical sector S+1\n"
        "  --no-stagger      do not flip the head bit on odd cylinders\n",
        p);
}

/* Read one little-endian 16-bit word from a byte buffer. */
static uint16_t rd16(const uint8_t *b) { return (uint16_t)(b[0] | (b[1] << 8)); }

int main(int argc, char **argv)
{
    int partition = 5;          /* MaxPartition for a T-80 (default boot) */
    int all_heads = 0;
    int sectors_diablo = 14;    /* nSectorsDiablo = 16B */
    int offset_cyl = 3;         /* offsetCylinderDiablo */
    int sector_offset = 1;      /* command-vs-physical sector bias (see usage) */
    int stagger = 1;            /* staggerSectors */
    const char *in_path = NULL, *out_path = NULL;

    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];
        if (!strcmp(a, "--partition") && i + 1 < argc)        partition = atoi(argv[++i]);
        else if (!strcmp(a, "--all-heads"))                   all_heads = 1;
        else if (!strcmp(a, "--sectors-diablo") && i + 1 < argc) sectors_diablo = atoi(argv[++i]);
        else if (!strcmp(a, "--offset-cyl") && i + 1 < argc)  offset_cyl = atoi(argv[++i]);
        else if (!strcmp(a, "--sector-offset") && i + 1 < argc) sector_offset = atoi(argv[++i]);
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
    if (offset_cyl + DIABLO31_CYL > geom.cylinders) {
        fprintf(stderr,
                "dsk2trident: DORADO_DISK_DIABLO has %d cylinders; need >= %d "
                "(offset %d + %d Diablo cyls)\n",
                geom.cylinders, offset_cyl + DIABLO31_CYL, offset_cyl, DIABLO31_CYL);
        return 1;
    }

    int boot_head = partition - 1;
    if (!all_heads && (boot_head < 0 || boot_head >= geom.heads)) {
        fprintf(stderr, "dsk2trident: partition %d -> head %d out of range [0,%d)\n",
                partition, boot_head, geom.heads);
        return 1;
    }

    FILE *in = fopen(in_path, "rb");
    if (!in) { perror(in_path); return 1; }
    if (fseek(in, 0, SEEK_END) != 0) { perror("fseek"); fclose(in); return 1; }
    long sz = ftell(in);
    rewind(in);
    long expect = (long)DIABLO31_CYL * DIABLO31_HEAD * DIABLO31_SEC * DIABLO_SEC_B;
    if (sz != expect) {
        fprintf(stderr,
                "dsk2trident: %s is %ld bytes, expected %ld "
                "(Diablo-31: %dx%dx%d x %d)\n",
                in_path, sz, expect, DIABLO31_CYL, DIABLO31_HEAD,
                DIABLO31_SEC, DIABLO_SEC_B);
        fclose(in);
        return 1;
    }

    dorado_disk_pack pack;
    if (dorado_disk_pack_create(&pack, &geom) != 0) {
        fprintf(stderr, "dsk2trident: failed to allocate Trident pack\n");
        fclose(in);
        return 1;
    }

    uint8_t buf[DIABLO_SEC_B];
    int placed = 0, missed = 0;
    /* The .dsk stores sectors in (cylinder, head, sector) order. */
    for (int dcyl = 0; dcyl < DIABLO31_CYL; dcyl++) {
        for (int dhead = 0; dhead < DIABLO31_HEAD; dhead++) {
            for (int dsec = 0; dsec < DIABLO31_SEC; dsec++) {
                if (fread(buf, 1, sizeof buf, in) != sizeof buf) {
                    fprintf(stderr, "dsk2trident: short read at c%d/h%d/s%d\n",
                            dcyl, dhead, dsec);
                    dorado_disk_pack_free(&pack);
                    fclose(in);
                    return 1;
                }
                /* Map to the physical Trident CHS. */
                int eff_head = dhead;
                if (stagger && (dcyl & 1)) eff_head ^= 1;
                int tcyl = dcyl + offset_cyl;
                int tsec = (sectors_diablo * eff_head + dsec + sector_offset)
                           % geom.sectors;

                int h0 = all_heads ? 0 : boot_head;
                int h1 = all_heads ? geom.heads : boot_head + 1;
                for (int thead = h0; thead < h1; thead++) {
                    dorado_disk_sector *s =
                        dorado_disk_pack_sector(&pack, tcyl, thead, tsec);
                    if (!s) { missed++; continue; }
                    /* Skip the dummy word, copy header/label/data verbatim. */
                    const uint8_t *p = buf + DIABLO_DUMMY_W * 2;
                    for (int w = 0; w < DIABLO_HDR_W; w++)  { s->header[w] = rd16(p); p += 2; }
                    for (int w = 0; w < DIABLO_LBL_W; w++)  { s->label[w]  = rd16(p); p += 2; }
                    for (int w = 0; w < DIABLO_DATA_W; w++) { s->data[w]   = rd16(p); p += 2; }
                    s->modified = 1;
                    placed++;
                }
            }
        }
    }
    fclose(in);

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
    printf("  geometry  %d cyl x %d head x %d sec (2/8/256 words/sector)\n",
           geom.cylinders, geom.heads, geom.sectors);
    printf("  mapping   cyl+%d, sector=%d*head+sec+%d, stagger=%s, ",
           offset_cyl, sectors_diablo, sector_offset, stagger ? "on" : "off");
    if (all_heads) printf("all heads\n");
    else           printf("partition %d -> head %d\n", partition, boot_head);
    printf("  placed    %d sectors%s\n", placed,
           missed ? " (some missed, see warning)" : "");
    return 0;
}
