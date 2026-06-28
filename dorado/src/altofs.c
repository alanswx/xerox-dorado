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

static void usage(const char *prog)
{
    fprintf(stderr,
        "Usage: %s --disk0 PATH --disk1 PATH [options]\n\n"
        "Create a two-drive Alto filesystem image pair for Dorado AEmu.\n\n"
        "Options:\n"
        "  --cylinders N      cylinders per emulated drive (default 406)\n"
        "  --heads N          heads per cylinder (default 2)\n"
        "  --sectors N        sectors per head (default 14)\n"
        "  --sector-words N   words per sector (default 256)\n"
        "  --vmem-pages N     create zero-filled LISP.VIRTUALMEM. pages (default 15002)\n"
        "  --vmem-name NAME   Alto filename for VMEM (default LISP.VIRTUALMEM.)\n"
        "  --vmem-after-inserts  create VMEM after the --insert files\n"
        "  --no-vmem          do not create a VMEM file\n"
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
    while (written < bytes) {
        size_t chunk = bytes - written;
        if (chunk > 256 * 1024) chunk = 256 * 1024;
        size_t n = fs_write(afs, &of, NULL, chunk, 1);
        written += n;
        if (n != chunk || of.error < 0) {
            fprintf(stderr, "altofs: short zero write to %s at %zu/%zu: %s\n",
                    name, written, bytes, fs_error(of.error));
            fs_close(afs, &of);
            return 1;
        }
    }

    if (!fs_close(afs, &of)) {
        fprintf(stderr, "altofs: could not close %s: %s\n",
                name, fs_error(of.error));
        return 1;
    }
    return 0;
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
    struct geometry dg;
    insert_spec inserts[32];
    int insert_count = 0;

    dg.num_disks = DEFAULT_DISKS;
    dg.num_cylinders = DEFAULT_CYLINDERS;
    dg.num_heads = DEFAULT_HEADS;
    dg.num_sectors = DEFAULT_SECTORS;
    dg.sector_words = DEFAULT_SECTOR_WORDS;

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
        } else if (!strcmp(a, "--sector-words") && i + 1 < argc) {
            dg.sector_words = (uint16_t)parse_int(argv[++i], "sector words");
        } else if (!strcmp(a, "--vmem-pages") && i + 1 < argc) {
            vmem_pages = parse_int(argv[++i], "vmem pages");
        } else if (!strcmp(a, "--vmem-name") && i + 1 < argc) {
            vmem_name = argv[++i];
        } else if (!strcmp(a, "--vmem-after-inserts")) {
            vmem_after_inserts = 1;
        } else if (!strcmp(a, "--no-vmem")) {
            create_vmem = 0;
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

    if (!disk0 || !disk1) {
        usage(argv[0]);
        return 2;
    }

    if (dg.num_disks != 2) {
        fprintf(stderr, "altofs: this wrapper writes exactly two disk halves\n");
        return 2;
    }

    struct fs afs;
    fs_initvar(&afs);
    if (!fs_create(&afs, dg)) {
        fprintf(stderr, "altofs: fs_create failed\n");
        return 1;
    }

    int error = 0;
    if (!fs_format(&afs, &error)) {
        fprintf(stderr, "altofs: fs_format failed: %s\n", fs_error(error));
        fs_destroy(&afs);
        return 1;
    }

    if (install_sysdir_dshape(&afs) != 0) {
        fs_destroy(&afs);
        return 1;
    }

    afs.checked = 1;

    if (create_vmem && !vmem_after_inserts) {
        size_t bytes = (size_t)vmem_pages * (size_t)dg.sector_words * 2u;
        if (make_zero_file(&afs, vmem_name, bytes) != 0) {
            fs_destroy(&afs);
            return 1;
        }
    }

    for (int i = 0; i < insert_count; i++) {
        if (!fs_insert_file(&afs, inserts[i].host_path, inserts[i].alto_name)) {
            fprintf(stderr, "altofs: could not insert %s as %s\n",
                    inserts[i].host_path, inserts[i].alto_name);
            fs_destroy(&afs);
            return 1;
        }
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

    if (!fs_update_disk_descriptor(&afs, &error)) {
        fprintf(stderr, "altofs: DiskDescriptor update failed: %s\n",
                fs_error(error));
        fs_destroy(&afs);
        return 1;
    }

    if (!fs_check_integrity(&afs)) {
        fprintf(stderr, "altofs: final filesystem failed integrity check\n");
        fs_destroy(&afs);
        return 1;
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
