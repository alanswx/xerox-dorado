/* altofs -- tiny Dorado-side wrapper around the local palo Alto FS library.
 *
 * This does not modify AltoInfo/palo. It uses palo's public fs API to create
 * the larger AEmu two-drive filesystem needed by Interlisp-D's LISP.VIRTUALMEM
 * file, then saves the two AAR-format Diablo halves for dsk2trident.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fs/fs.h"
#include "fs/fs_internal.h"

#define DEFAULT_CYLINDERS    406
#define DEFAULT_HEADS          2
#define DEFAULT_SECTORS       14
#define DEFAULT_SECTOR_WORDS 256
#define DEFAULT_DISKS          2
#define DEFAULT_VMEM_PAGES 15002

typedef struct {
    const char *host_path;
    const char *alto_name;
} insert_spec;

static int repair_file_metadata(struct fs *afs, const char *name);

static int list_directory(struct fs *afs, const char *name, int verbose)
{
    afs->checked = 1;
    if (!fs_print_directory(afs, name, verbose, stdout)) {
        fprintf(stderr, "altofs: could not list %s\n", name);
        return 1;
    }
    return 0;
}

static int find_serial(struct fs *afs, uint16_t sn_word2)
{
    int found = 0;

    afs->checked = 1;
    for (uint16_t vda = 0; vda < afs->length; vda++) {
        const struct page *pg = &afs->pages[vda];
        if (pg->label.s.version == VERSION_FREE ||
            pg->label.s.version == VERSION_BAD ||
            pg->label.s.sn.word2 != sn_word2)
            continue;

        uint16_t rda = 0;
        uint16_t next_vda = 0;
        uint16_t prev_vda = 0;
        (void)virtual_to_real(&afs->dg, pg->page_vda, &rda);
        if (pg->label.s.next_rda)
            (void)real_to_virtual(&afs->dg, pg->label.s.next_rda, &next_vda);
        if (pg->label.s.prev_rda)
            (void)real_to_virtual(&afs->dg, pg->label.s.prev_rda, &prev_vda);
        printf("serial %06o: vda=%u rda=%06o page=%u nbytes=%u "
               "prev=%u next=%u version=%u sn=%u,%u\n",
               sn_word2, vda, rda, pg->label.s.file_pgnum,
               pg->label.s.nbytes, prev_vda, next_vda,
               pg->label.s.version, pg->label.s.sn.word1,
               pg->label.s.sn.word2);
        found = 1;
    }

    if (!found) {
        fprintf(stderr, "altofs: no pages found for serial %06o\n", sn_word2);
        return 1;
    }
    return 0;
}

static int inspect_file_chain(struct fs *afs, const char *name)
{
    struct file_entry fe, dir_fe;
    int found = 0;

    if (!fs_resolve_name(afs, name, &found, &fe, &dir_fe, NULL) || !found) {
        fprintf(stderr, "altofs: inspect: could not resolve %s\n", name);
        return 1;
    }

    printf("inspect %s: leader_vda=%u sn=%u,%u version=%u\n",
           name, fe.leader_vda, fe.sn.word1, fe.sn.word2, fe.version);
    uint16_t vda = fe.leader_vda;
    uint16_t prev_vda = 0;
    unsigned pages = 0;
    while (vda != 0 && vda < afs->length) {
        struct page *pg = &afs->pages[vda];
        uint16_t rda = 0, next_vda = 0, prev_link_vda = 0;
        (void)virtual_to_real(&afs->dg, pg->page_vda, &rda);
        if (pg->label.s.next_rda != 0)
            (void)real_to_virtual(&afs->dg, pg->label.s.next_rda, &next_vda);
        if (pg->label.s.prev_rda != 0)
            (void)real_to_virtual(&afs->dg, pg->label.s.prev_rda, &prev_link_vda);
        if (pages < 20 || next_vda == 0) {
            printf("  page %u: slot_vda=%u page_vda=%u rda=%u "
                   "prev_rda=%u(prev_vda=%u) next_rda=%u(next_vda=%u) "
                   "file_pg=%u nbytes=%u\n",
                   pages, vda, pg->page_vda, rda,
                   pg->label.s.prev_rda, prev_link_vda,
                   pg->label.s.next_rda, next_vda,
                   pg->label.s.file_pgnum, pg->label.s.nbytes);
        }
        if (pages > 0 && prev_link_vda != prev_vda) {
            printf("  break: page %u prev link points to VDA %u, expected %u\n",
                   pages, prev_link_vda, prev_vda);
            return 1;
        }
        if (next_vda == 0) {
            printf("inspect %s: pages=%u last_vda=%u\n", name, pages + 1, vda);
            return 0;
        }
        prev_vda = vda;
        vda = next_vda;
        pages++;
        if (pages > afs->length) {
            fprintf(stderr, "altofs: inspect: cycle in %s\n", name);
            return 1;
        }
    }

    fprintf(stderr, "altofs: inspect: invalid VDA %u in %s\n", vda, name);
    return 1;
}

static int safe_allocate_next_page(struct fs *afs, uint16_t last_vda,
                                   uint16_t *out_vda)
{
    if ((uint32_t)last_vda + 1u >= afs->length)
        return allocate_page(afs, out_vda, NULL);
    return allocate_page(afs, out_vda, &last_vda);
}

static int append_empty_terminal_page(struct fs *afs, uint16_t *vda,
                                      uint16_t *pgnum)
{
    struct page *pg = &afs->pages[*vda];
    uint16_t next_vda = 0;

    if (!safe_allocate_next_page(afs, pg->page_vda, &next_vda))
        return 0;

    struct page *npg = &afs->pages[next_vda];
    virtual_to_real(&afs->dg, npg->page_vda, &pg->label.s.next_rda);
    virtual_to_real(&afs->dg, pg->page_vda, &npg->label.s.prev_rda);
    npg->label.s.next_rda = 0;
    npg->label.s.unused = pg->label.s.unused;
    npg->label.s.nbytes = 0;
    npg->label.s.file_pgnum = (uint16_t)(*pgnum + 1);
    npg->label.s.version = pg->label.s.version;
    npg->label.s.sn = pg->label.s.sn;

    *vda = next_vda;
    *pgnum = (uint16_t)(*pgnum + 1);
    return 1;
}

static int insert_file_verbose(struct fs *afs, const char *host_path,
                               const char *alto_name)
{
    struct open_file of;
    FILE *fp;

    if (!fs_open(afs, alto_name, "w", &of)) {
        fprintf(stderr, "altofs: could not open %s for insert: %s\n",
                alto_name, fs_error(of.error));
        return 1;
    }

    fp = fopen(host_path, "rb");
    if (!fp) {
        fprintf(stderr, "altofs: could not open %s\n", host_path);
        fs_close(afs, &of);
        return 1;
    }

    uint16_t vda = of.pos.vda;
    size_t total = 0;
    uint16_t pgnum = of.pos.pgnum;
    for (;;) {
        if (vda >= afs->length) {
            fprintf(stderr,
                    "altofs: insert write reached invalid VDA %u for %s\n",
                    vda, alto_name);
            fclose(fp);
            fs_close(afs, &of);
            return 1;
        }

        struct page *pg = &afs->pages[vda];
        size_t n = fread(pg->data, 1, afs->sector_bytes, fp);
        if (n < afs->sector_bytes)
            memset(pg->data + n, 0, afs->sector_bytes - n);
        pg->label.s.nbytes = (uint16_t)n;
        pg->label.s.file_pgnum = pgnum;
        total += n;

        int ch = fgetc(fp);
        if (ch == EOF) {
            if (n == afs->sector_bytes &&
                !append_empty_terminal_page(afs, &vda, &pgnum)) {
                fprintf(stderr,
                        "altofs: insert ran out of pages for terminal page "
                        "of %s after %zu bytes\n",
                        alto_name, total);
                fclose(fp);
                fs_close(afs, &of);
                return 1;
            }
            pg = &afs->pages[vda];
            pg->label.s.next_rda = 0;
            of.pos.vda = vda;
            of.pos.pgnum = pgnum;
            of.pos.pos = pg->label.s.nbytes;
            of.modified = 1;
            break;
        }
        ungetc(ch, fp);

        uint16_t next_vda = 0;
        if (!safe_allocate_next_page(afs, pg->page_vda, &next_vda)) {
            fprintf(stderr,
                    "altofs: insert ran out of pages for %s after %zu bytes\n",
                    alto_name, total);
            fclose(fp);
            fs_close(afs, &of);
            return 1;
        }
        struct page *npg = &afs->pages[next_vda];
        virtual_to_real(&afs->dg, npg->page_vda, &pg->label.s.next_rda);
        virtual_to_real(&afs->dg, pg->page_vda, &npg->label.s.prev_rda);
        npg->label.s.next_rda = 0;
        npg->label.s.unused = pg->label.s.unused;
        npg->label.s.nbytes = 0;
        npg->label.s.file_pgnum = (uint16_t)(pgnum + 1);
        npg->label.s.version = pg->label.s.version;
        npg->label.s.sn = pg->label.s.sn;
        vda = next_vda;
        pgnum++;
    }

    fclose(fp);
    if (!fs_close(afs, &of)) {
        fprintf(stderr, "altofs: could not close %s after insert: %s\n",
                alto_name, fs_error(of.error));
        return 1;
    }
    return repair_file_metadata(afs, alto_name);
}

static void wr16be(uint8_t *p, uint16_t v)
{
    p[0] = (uint8_t)(v >> 8);
    p[1] = (uint8_t)v;
}

static int install_sysdir_dshape(struct fs *afs)
{
    struct file_entry sysdir;
    struct file_info info;
    int old_checked = afs->checked;
    int error = 0;

    afs->checked = 1;
    if (!fs_get_sysdir(afs, &sysdir)) {
        fprintf(stderr, "altofs: could not locate SysDir. leader\n");
        afs->checked = old_checked;
        return 1;
    }
    if (!fs_get_file_info(afs, &sysdir, &info, &error)) {
        fprintf(stderr, "altofs: could not read SysDir. leader: %s\n",
                fs_error(error));
        afs->checked = old_checked;
        return 1;
    }

    /* Xerox BFSNewDisk.bcpl installs fpropTypeDShape in SysDir.'s leader
     * properties: FPROP type=1, length=lDSHAPE+1, then nDisks, nTracks,
     * nHeads, nSectors. Scavenger's FindDShape fast path depends on it. */
    memset(info.props, 0, sizeof info.props);
    info.props[0] = 1;
    info.props[1] = 5;
    wr16be(&info.props[2], afs->dg.num_disks);
    wr16be(&info.props[4], afs->dg.num_cylinders);
    wr16be(&info.props[6], afs->dg.num_heads);
    wr16be(&info.props[8], afs->dg.num_sectors);
    info.propbegin = 26;
    info.proplen = 210;
    info.has_dg = 1;
    info.dg = afs->dg;

    if (!fs_set_file_info(afs, &sysdir, &info, &error)) {
        fprintf(stderr, "altofs: could not write SysDir. DShape: %s\n",
                fs_error(error));
        afs->checked = old_checked;
        return 1;
    }
    afs->checked = old_checked;
    return 0;
}

static int repair_file_metadata(struct fs *afs, const char *name)
{
    struct file_entry fe, dir_fe;
    struct file_info info;
    int found = 0;
    int error = 0;

    if (!fs_resolve_name(afs, name, &found, &fe, &dir_fe, NULL) || !found) {
        fprintf(stderr, "altofs: could not resolve %s for metadata repair\n",
                name);
        return 1;
    }

    uint16_t vda = fe.leader_vda;
    uint16_t last_vda = vda;
    while (vda != 0) {
        struct page *pg = &afs->pages[vda];
        last_vda = vda;
        uint16_t next_vda = 0;
        if (pg->label.s.next_rda != 0 &&
            !real_to_virtual(&afs->dg, pg->label.s.next_rda, &next_vda)) {
            fprintf(stderr, "altofs: invalid next RDA in %s at VDA %u\n",
                    name, vda);
            return 1;
        }
        if (next_vda == 0) break;
        vda = next_vda;
    }

    struct page *last = &afs->pages[last_vda];
    if (!fs_get_file_info(afs, &fe, &info, &error)) {
        fprintf(stderr, "altofs: could not read %s leader: %s\n",
                name, fs_error(error));
        return 1;
    }
    info.fe = dir_fe;
    info.last_page.vda = last_vda;
    info.last_page.pgnum = last->label.s.file_pgnum;
    info.last_page.pos = last->label.s.nbytes;
    if (!fs_set_file_info(afs, &fe, &info, &error)) {
        fprintf(stderr, "altofs: could not repair %s leader: %s\n",
                name, fs_error(error));
        return 1;
    }
    update_disk_metadata(afs);
    return 0;
}

static void usage(const char *prog)
{
    fprintf(stderr,
        "Usage: %s --disk0 PATH --disk1 PATH [options]\n\n"
        "Create a two-drive Alto filesystem image pair for Dorado AEmu.\n\n"
        "Options:\n"
        "  --cylinders N      cylinders per emulated drive (default 406)\n"
        "  --heads N          heads per cylinder (default 2)\n"
        "  --sectors N        sectors per head (default 14)\n"
        "  --source-cylinders N\n"
        "  --source-sectors N load smaller existing AAR halves into output geometry\n"
        "  --source-single-drive\n"
        "                     load only disk0 from the smaller source geometry\n"
        "  --source-preserve-vda\n"
        "                     keep source VDA order and rewrite RDA links\n"
        "  --sector-words N   words per sector (default 256)\n"
        "  --vmem-pages N     create zero-filled LISP.VIRTUALMEM. pages (default 15002)\n"
        "  --vmem-name NAME   Alto filename for VMEM (default LISP.VIRTUALMEM.)\n"
        "  --vmem-after-inserts  create VMEM after the --insert files\n"
        "  --no-vmem          do not create a VMEM file\n"
        "  --existing         load and edit an existing AAR-format image pair\n"
        "  --force-existing   edit an existing pair even if host integrity check fails\n"
        "  --init-blank-free  mark blank existing sectors as valid free pages\n"
        "  --preserve-existing-metadata\n"
        "                     do not rewrite SysDir. DShape or DiskDescriptor.\n"
        "  --inspect NAME     print NAME's leader and first file-chain labels\n"
        "  --list NAME        list a directory without editing the image\n"
        "  --find-serial SN   print pages with Alto serial word2 SN\n"
        "  --extract NAME HOST extract Alto file NAME to host path HOST\n"
        "  --repair NAME      rewrite NAME's leader metadata from its chain\n"
        "  --insert HOST NAME insert a host file as Alto filename NAME\n"
        "  --boot-file NAME   install Alto boot sector from filesystem NAME\n"
        "  --help             show this help\n",
        prog);
}

static int parse_int(const char *s, const char *what)
{
    char *end = NULL;
    long v = strtol(s, &end, 0);
    if (!s[0] || (end && *end) || v < 0 || v > 65535) {
        fprintf(stderr, "altofs: invalid %s: %s\n", what, s);
        exit(2);
    }
    return (int)v;
}

static int make_zero_file(struct fs *afs, const char *name, size_t bytes)
{
    struct open_file of;
    if (!fs_open(afs, name, "w", &of)) {
        fprintf(stderr, "altofs: could not create %s: %s\n",
                name, fs_error(of.error));
        return 1;
    }

    size_t written = 0;
    uint16_t vda = of.pos.vda;
    uint16_t pgnum = of.pos.pgnum;
    for (;;) {
        if (vda >= afs->length) {
            fprintf(stderr,
                    "altofs: zero write reached invalid VDA %u for %s\n",
                    vda, name);
            fs_close(afs, &of);
            return 1;
        }

        struct page *pg = &afs->pages[vda];
        size_t n = bytes - written;
        if (n > afs->sector_bytes)
            n = afs->sector_bytes;
        memset(pg->data, 0, afs->sector_bytes);
        pg->label.s.nbytes = (uint16_t)n;
        pg->label.s.file_pgnum = pgnum;
        written += n;

        if (written >= bytes) {
            if (n == afs->sector_bytes &&
                !append_empty_terminal_page(afs, &vda, &pgnum)) {
                fprintf(stderr,
                        "altofs: zero write ran out of pages for terminal "
                        "page of %s at %zu/%zu bytes\n",
                        name, written, bytes);
                fs_close(afs, &of);
                return 1;
            }
            pg = &afs->pages[vda];
            pg->label.s.next_rda = 0;
            of.pos.vda = vda;
            of.pos.pgnum = pgnum;
            of.pos.pos = pg->label.s.nbytes;
            of.modified = 1;
            break;
        }

        uint16_t next_vda = 0;
        if (!safe_allocate_next_page(afs, pg->page_vda, &next_vda)) {
            fprintf(stderr,
                    "altofs: zero write ran out of pages for %s at %zu/%zu bytes\n",
                    name, written, bytes);
            fs_close(afs, &of);
            return 1;
        }
        struct page *npg = &afs->pages[next_vda];
        virtual_to_real(&afs->dg, npg->page_vda, &pg->label.s.next_rda);
        virtual_to_real(&afs->dg, pg->page_vda, &npg->label.s.prev_rda);
        npg->label.s.next_rda = 0;
        npg->label.s.unused = pg->label.s.unused;
        npg->label.s.nbytes = 0;
        npg->label.s.file_pgnum = (uint16_t)(pgnum + 1);
        npg->label.s.version = pg->label.s.version;
        npg->label.s.sn = pg->label.s.sn;
        vda = next_vda;
        pgnum++;
    }

    if (!fs_close(afs, &of)) {
        fprintf(stderr, "altofs: could not close %s: %s\n",
                name, fs_error(of.error));
        return 1;
    }
    return repair_file_metadata(afs, name);
}

static int page_is_blank(const struct fs *afs, const struct page *pg)
{
    if (pg->header[0] || pg->header[1])
        return 0;
    for (size_t i = 0; i < sizeof pg->label.r / sizeof pg->label.r[0]; i++) {
        if (pg->label.r[i])
            return 0;
    }
    for (size_t i = 0; i < afs->sector_bytes; i++) {
        if (pg->data[i])
            return 0;
    }
    return 1;
}

static void init_all_pages_free(struct fs *afs)
{
    for (uint16_t vda = 0; vda < afs->length; vda++) {
        struct page *pg = &afs->pages[vda];
        uint16_t rda = 0;
        pg->page_vda = vda;
        pg->header[0] = 0;
        if (!virtual_to_real(&afs->dg, vda, &rda))
            rda = 0;
        pg->header[1] = rda;
        memset(pg->label.r, 0, sizeof pg->label.r);
        pg->label.s.version = VERSION_FREE;
        pg->label.s.sn.word1 = VERSION_FREE;
        pg->label.s.sn.word2 = VERSION_FREE;
        memset(pg->data, 0, afs->sector_bytes);
    }
    update_disk_metadata(afs);
}

static uint16_t rd16le(FILE *fp, int *ok)
{
    int lo = fgetc(fp);
    int hi = fgetc(fp);
    if (lo == EOF || hi == EOF) {
        *ok = 0;
        return 0;
    }
    return (uint16_t)((lo & 0xFF) | ((hi & 0xFF) << 8));
}

static uint16_t translate_source_rda(const struct geometry *src,
                                     const struct geometry *dst,
                                     uint16_t rda)
{
    if (rda == 0)
        return 0;
    uint16_t vda = 0;
    uint16_t out = 0;
    if (!real_to_virtual(src, rda, &vda) ||
        !virtual_to_real(dst, vda, &out))
        return rda;
    return out;
}

static int load_short_aar(struct fs *afs, const char *path,
                          uint16_t disk_num, const struct geometry *src,
                          int preserve_vda_order)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        fprintf(stderr, "altofs: could not open existing image %s\n", path);
        return 0;
    }

    uint32_t src_pages = (uint32_t)src->num_cylinders *
                         (uint32_t)src->num_heads *
                         (uint32_t)src->num_sectors;
    size_t src_sector_bytes = (size_t)src->sector_words * 2u;
    if (src_sector_bytes != afs->sector_bytes) {
        fprintf(stderr, "altofs: source sector size does not match output\n");
        fclose(fp);
        return 0;
    }

    for (uint32_t i = 0; i < src_pages; i++) {
        int ok = 1;
        (void)rd16le(fp, &ok);        /* dummy word */
        uint16_t header[2];
        uint16_t label[8];
        for (size_t w = 0; w < 2; w++) header[w] = rd16le(fp, &ok);
        for (size_t w = 0; w < 8; w++) label[w] = rd16le(fp, &ok);
        if (!ok) {
            fprintf(stderr, "altofs: short read from %s\n", path);
            fclose(fp);
            return 0;
        }

        uint16_t vda = 0;
        if (preserve_vda_order) {
            vda = (uint16_t)(disk_num * afs->disk_length + i);
            if (vda >= afs->length) {
                fprintf(stderr, "altofs: source page outside output at %s:%u\n",
                        path, (unsigned)i);
                fclose(fp);
                return 0;
            }
            uint16_t rda = 0;
            if (virtual_to_real(&afs->dg, vda, &rda))
                header[1] = rda;
            label[0] = translate_source_rda(src, &afs->dg, label[0]);
            label[1] = translate_source_rda(src, &afs->dg, label[1]);
        } else if (!real_to_virtual(&afs->dg, header[1], &vda)) {
            vda = (uint16_t)(disk_num * afs->disk_length + i);
            if (vda >= afs->length) {
                fprintf(stderr, "altofs: source page outside output at %s:%u\n",
                        path, (unsigned)i);
                fclose(fp);
                return 0;
            }
        }

        struct page *pg = &afs->pages[vda];
        pg->page_vda = vda;
        memcpy(pg->header, header, sizeof header);
        memcpy(pg->label.r, label, sizeof label);
        for (size_t j = 0; j < afs->sector_bytes; j++) {
            int c = fgetc(fp);
            if (c == EOF) {
                fprintf(stderr, "altofs: short data read from %s\n", path);
                fclose(fp);
                return 0;
            }
            pg->data[j ^ 1u] = (uint8_t)c;
        }
    }

    int c = fgetc(fp);
    if (c != EOF) {
        fprintf(stderr, "altofs: source image %s has extra data\n", path);
        fclose(fp);
        return 0;
    }

    fclose(fp);
    return 1;
}

static int init_blank_free_pages(struct fs *afs)
{
    int count = 0;
    for (uint16_t vda = 1; vda < afs->length; vda++) {
        struct page *pg = &afs->pages[vda];
        if (!page_is_blank(afs, pg))
            continue;

        uint16_t rda = 0;
        if (!virtual_to_real(&afs->dg, vda, &rda)) {
            fprintf(stderr, "altofs: could not map VDA %u to RDA\n", vda);
            return -1;
        }
        pg->page_vda = vda;
        pg->header[0] = 0;
        pg->header[1] = rda;
        pg->label.s.version = VERSION_FREE;
        pg->label.s.sn.word1 = VERSION_FREE;
        pg->label.s.sn.word2 = VERSION_FREE;
        count++;
    }
    if (count > 0)
        update_disk_metadata(afs);
    return count;
}

int main(int argc, char **argv)
{
    const char *disk0 = NULL;
    const char *disk1 = NULL;
    const char *vmem_name = "LISP.VIRTUALMEM.";
    const char *boot_file = NULL;
    int vmem_pages = DEFAULT_VMEM_PAGES;
    int create_vmem = 1;
    int vmem_after_inserts = 0;
    int existing = 0;
    int force_existing = 0;
    int init_blank_free = 0;
    int preserve_existing_metadata = 0;
    int source_single_drive = 0;
    int source_preserve_vda = 0;
    const char *inspect_name = NULL;
    const char *list_name = NULL;
    uint16_t find_serial_word2 = 0;
    int do_find_serial = 0;
    const char *extract_name = NULL;
    const char *extract_path = NULL;
    const char *repair_name = NULL;
    struct geometry dg;
    struct geometry src_dg;
    int source_geometry = 0;
    insert_spec inserts[32];
    int insert_count = 0;

    dg.num_disks = DEFAULT_DISKS;
    dg.num_cylinders = DEFAULT_CYLINDERS;
    dg.num_heads = DEFAULT_HEADS;
    dg.num_sectors = DEFAULT_SECTORS;
    dg.sector_words = DEFAULT_SECTOR_WORDS;
    src_dg = dg;

    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];
        if (!strcmp(a, "--disk0") && i + 1 < argc) {
            disk0 = argv[++i];
        } else if (!strcmp(a, "--disk1") && i + 1 < argc) {
            disk1 = argv[++i];
        } else if (!strcmp(a, "--cylinders") && i + 1 < argc) {
            dg.num_cylinders = (uint16_t)parse_int(argv[++i], "cylinders");
        } else if (!strcmp(a, "--heads") && i + 1 < argc) {
            dg.num_heads = (uint16_t)parse_int(argv[++i], "heads");
        } else if (!strcmp(a, "--sectors") && i + 1 < argc) {
            dg.num_sectors = (uint16_t)parse_int(argv[++i], "sectors");
        } else if (!strcmp(a, "--source-cylinders") && i + 1 < argc) {
            src_dg.num_cylinders = (uint16_t)parse_int(argv[++i], "source cylinders");
            source_geometry = 1;
        } else if (!strcmp(a, "--source-sectors") && i + 1 < argc) {
            src_dg.num_sectors = (uint16_t)parse_int(argv[++i], "source sectors");
            source_geometry = 1;
        } else if (!strcmp(a, "--source-single-drive")) {
            source_single_drive = 1;
        } else if (!strcmp(a, "--source-preserve-vda")) {
            source_preserve_vda = 1;
        } else if (!strcmp(a, "--sector-words") && i + 1 < argc) {
            dg.sector_words = (uint16_t)parse_int(argv[++i], "sector words");
            src_dg.sector_words = dg.sector_words;
        } else if (!strcmp(a, "--vmem-pages") && i + 1 < argc) {
            vmem_pages = parse_int(argv[++i], "vmem pages");
        } else if (!strcmp(a, "--vmem-name") && i + 1 < argc) {
            vmem_name = argv[++i];
        } else if (!strcmp(a, "--vmem-after-inserts")) {
            vmem_after_inserts = 1;
        } else if (!strcmp(a, "--no-vmem")) {
            create_vmem = 0;
        } else if (!strcmp(a, "--existing")) {
            existing = 1;
        } else if (!strcmp(a, "--force-existing")) {
            force_existing = 1;
        } else if (!strcmp(a, "--init-blank-free")) {
            init_blank_free = 1;
        } else if (!strcmp(a, "--preserve-existing-metadata")) {
            preserve_existing_metadata = 1;
        } else if (!strcmp(a, "--inspect") && i + 1 < argc) {
            inspect_name = argv[++i];
        } else if (!strcmp(a, "--list") && i + 1 < argc) {
            list_name = argv[++i];
        } else if (!strcmp(a, "--find-serial") && i + 1 < argc) {
            find_serial_word2 = (uint16_t)parse_int(argv[++i], "serial");
            do_find_serial = 1;
        } else if (!strcmp(a, "--extract") && i + 2 < argc) {
            extract_name = argv[++i];
            extract_path = argv[++i];
        } else if (!strcmp(a, "--repair") && i + 1 < argc) {
            repair_name = argv[++i];
        } else if (!strcmp(a, "--insert") && i + 2 < argc) {
            if (insert_count >= (int)(sizeof inserts / sizeof inserts[0])) {
                fprintf(stderr, "altofs: too many --insert entries\n");
                return 2;
            }
            inserts[insert_count].host_path = argv[++i];
            inserts[insert_count].alto_name = argv[++i];
            insert_count++;
        } else if (!strcmp(a, "--boot-file") && i + 1 < argc) {
            boot_file = argv[++i];
        } else if (!strcmp(a, "--help") || !strcmp(a, "-h")) {
            usage(argv[0]);
            return 0;
        } else {
            usage(argv[0]);
            return 2;
        }
    }

    if (extract_name && (insert_count || boot_file || repair_name)) {
        fprintf(stderr, "altofs: --extract cannot be combined with image edits\n");
        return 2;
    }
    if ((list_name || do_find_serial) &&
        (insert_count || boot_file || repair_name || extract_name ||
         create_vmem || init_blank_free || !preserve_existing_metadata)) {
        fprintf(stderr,
                "altofs: --list/--find-serial require read-only options "
                "(use --existing --preserve-existing-metadata --no-vmem)\n");
        return 2;
    }

    if (init_blank_free && !existing) {
        fprintf(stderr, "altofs: --init-blank-free requires --existing\n");
        return 2;
    }

    if (!disk0 || !disk1) {
        usage(argv[0]);
        return 2;
    }

    if (dg.num_disks != 2) {
        fprintf(stderr, "altofs: this wrapper writes exactly two disk halves\n");
        return 2;
    }
    src_dg.num_disks = dg.num_disks;
    src_dg.num_heads = dg.num_heads;
    src_dg.sector_words = dg.sector_words;

    struct fs afs;
    fs_initvar(&afs);
    if (!fs_create(&afs, dg)) {
        fprintf(stderr, "altofs: fs_create failed\n");
        return 1;
    }

    int error = 0;
    if (existing) {
        int loaded = 0;
        if (source_geometry) {
            init_all_pages_free(&afs);
            loaded = load_short_aar(&afs, disk0, 0, &src_dg,
                                    source_preserve_vda) &&
                     (source_single_drive ||
                      load_short_aar(&afs, disk1, 1, &src_dg,
                                     source_preserve_vda));
            update_disk_metadata(&afs);
        } else {
            loaded = fs_load_image(&afs, disk0, 0, 0) &&
                     fs_load_image(&afs, disk1, 1, 0);
        }
        if (!loaded) {
            fprintf(stderr, "altofs: could not load existing disk images\n");
            fs_destroy(&afs);
            return 1;
        }
        if (!fs_check_integrity(&afs) && !force_existing) {
            fprintf(stderr, "altofs: existing filesystem failed integrity check\n");
            fs_destroy(&afs);
            return 1;
        } else if (!afs.checked) {
            fprintf(stderr,
                    "altofs: warning: forcing edits on filesystem that failed integrity check\n");
            update_disk_metadata(&afs);
            afs.checked = 1;
        }
    } else {
        if (!fs_format(&afs, &error)) {
            fprintf(stderr, "altofs: fs_format failed: %s\n", fs_error(error));
            fs_destroy(&afs);
            return 1;
        }
    }

    if (extract_name) {
        afs.checked = 1;
        int rc = fs_extract_file(&afs, extract_name, extract_path) ? 0 : 1;
        fs_destroy(&afs);
        return rc;
    }

    if (list_name) {
        int rc = list_directory(&afs, list_name, 0);
        fs_destroy(&afs);
        return rc;
    }

    if (do_find_serial) {
        int rc = find_serial(&afs, find_serial_word2);
        fs_destroy(&afs);
        return rc;
    }

    if (init_blank_free) {
        int n = init_blank_free_pages(&afs);
        if (n < 0) {
            fs_destroy(&afs);
            return 1;
        }
        printf("altofs: initialized %d blank sectors as free pages\n", n);
    }

    if (!preserve_existing_metadata && install_sysdir_dshape(&afs) != 0) {
        fs_destroy(&afs);
        return 1;
    }

    afs.checked = 1;

    if (inspect_name && !create_vmem && insert_count == 0 && !boot_file &&
        preserve_existing_metadata) {
        int rc = inspect_file_chain(&afs, inspect_name);
        fs_destroy(&afs);
        return rc;
    }

    if (create_vmem && !vmem_after_inserts) {
        size_t bytes = (size_t)vmem_pages * (size_t)dg.sector_words * 2u;
        if (make_zero_file(&afs, vmem_name, bytes) != 0) {
            fs_destroy(&afs);
            return 1;
        }
    }

    for (int i = 0; i < insert_count; i++) {
        if (insert_file_verbose(&afs, inserts[i].host_path,
                                inserts[i].alto_name) != 0) {
            fs_destroy(&afs);
            return 1;
        }
    }

    if (repair_name && repair_file_metadata(&afs, repair_name) != 0) {
        fs_destroy(&afs);
        return 1;
    }

    if (create_vmem && vmem_after_inserts) {
        size_t bytes = (size_t)vmem_pages * (size_t)dg.sector_words * 2u;
        if (make_zero_file(&afs, vmem_name, bytes) != 0) {
            fs_destroy(&afs);
            return 1;
        }
    }

    if (boot_file) {
        if (!fs_install_boot(&afs, boot_file, &error)) {
            fprintf(stderr, "altofs: could not install boot file %s: %s\n",
                    boot_file, fs_error(error));
            fs_destroy(&afs);
            return 1;
        }
    }

    if (inspect_name && inspect_file_chain(&afs, inspect_name) != 0) {
        fs_destroy(&afs);
        return 1;
    }

    if (!preserve_existing_metadata && !fs_update_disk_descriptor(&afs, &error)) {
        if (!force_existing) {
            fprintf(stderr, "altofs: DiskDescriptor update failed: %s\n",
                    fs_error(error));
            fs_destroy(&afs);
            return 1;
        }
        fprintf(stderr,
                "altofs: warning: DiskDescriptor update failed: %s\n",
                fs_error(error));
    }
    if (!preserve_existing_metadata &&
        repair_file_metadata(&afs, "DiskDescriptor.") != 0 && !force_existing) {
        fs_destroy(&afs);
        return 1;
    }

    if (!fs_check_integrity(&afs) && !force_existing) {
        fprintf(stderr, "altofs: final filesystem failed integrity check\n");
        fs_destroy(&afs);
        return 1;
    } else if (!afs.checked) {
        fprintf(stderr,
                "altofs: warning: saving filesystem that failed final integrity check\n");
        afs.checked = 1;
    }

    if (!fs_save_image(&afs, disk0, 0, 0) ||
        !fs_save_image(&afs, disk1, 1, 0)) {
        fprintf(stderr, "altofs: failed to save disk images\n");
        fs_destroy(&afs);
        return 1;
    }

    printf("altofs: wrote %s and %s\n", disk0, disk1);
    printf("  geometry  2 disks x %u cyl x %u head x %u sec x %u words\n",
           dg.num_cylinders, dg.num_heads, dg.num_sectors, dg.sector_words);
    if (create_vmem) {
        printf("  vmem      %s = %d pages\n", vmem_name, vmem_pages);
    }
    if (boot_file) {
        printf("  boot      %s\n", boot_file);
    }
    printf("  free      %u pages\n", afs.free_pages);

    fs_destroy(&afs);
    return 0;
}
