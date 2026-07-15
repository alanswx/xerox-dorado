#!/usr/bin/env python3
"""Install the logical-volume bootingInfo/rootFile entries in a PDI.

Rusty Backup's installer (`pilot::install_boot_file`) records the germ and
the Cedar boot file only in the PHYSICAL volume root's bootingInfo array --
that is all the microcode/germ cold-boot chain reads (VolumeFormat.mesa
`PhysicalRoot.bootingInfo(10B)`).  But Pilot's soft boot -- BootTool's
herald volume buttons, RollBack, Booting.Boot -- resolves the target
volume's boot files through the LOGICAL volume root instead:

  LogicalRoot.bootingInfo (37B): ARRAY VolumeFile[checkpoint..debuggee]
                                 OF BootFile.DiskFileID   (6 x 9 words)
  LogicalRoot.rootFile   (125B): ARRAY VolumeFile[0..16)
                                 OF RECORD[fp: File.FP, page: PageNumber]

On a volume Othello installed, File.SetRoot (FileImpl.mesa) populates both
arrays.  Our volumes never had that done, so clicking a herald boot button
raised an uncaught File.Error from FileImpl (observed 2026-07-15).

This tool copies the PV root's germ + bootFile DiskFileIDs verbatim into
the LV root's bootingInfo (same slot ordinals: germ=2, bootFile=3 --
File.mesa VolumeFile), writes matching rootFile entries (fp.id = the
label FileID, fp.da = the label DA-hint words, boot-chain firstPage 0),
and recomputes the LV root page checksum (Checksum.ComputeChecksum
software algorithm, verified byte-exact against rusty-backup's writer).
The DiskFileID words are copied without re-encoding on purpose: the
committed images carry the older flat-VDA firstLink convention that the
emulator's polled germ path expects; re-encoding to CHS would recreate
the 2e8018b cold-boot regression.

    python3 tools/pdi_install_lv_bootfiles.py image.pdi [--dry-run]

Idempotent.
"""
import sys

PR_SEAL = 0o121212
LR_SEAL = 0o131313
HDR = 512
PV_BOOTING_INFO = 8    # PhysicalRoot.bootingInfo (10B)
LV_BOOTING_INFO = 31   # LogicalRoot.bootingInfo (37B)
LV_ROOT_FILE = 85      # LogicalRoot.rootFile (125B)
DISK_FILE_ID_WORDS = 9  # fID(5) + firstPage(2) + firstLink(2)
ROOT_FILE_WORDS = 6     # fp(4: id 2 + da 2) + page(2)
SLOTS = {'germ': 2, 'bootFile': 3}  # File.mesa VolumeFile ordinals


def be16(raw, o):
    return (raw[o] << 8) | raw[o + 1]


def put16(raw, o, v):
    raw[o] = (v >> 8) & 0xFF
    raw[o + 1] = v & 0xFF


def pilot_checksum(words):
    """16-bit ones-complement add, left-rotate after each word, FFFF->0."""
    cs = 0
    for w in words:
        t = cs + w
        cs = (t + 1) & 0xFFFF if t > 0xFFFF else t
        cs = ((cs << 1) | 1) & 0xFFFF if cs & 0x8000 else (cs << 1) & 0xFFFF
    return 0 if cs == 0xFFFF else cs


def main():
    dry = '--dry-run' in sys.argv
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    if len(args) != 1:
        sys.exit(__doc__)
    path = args[0]
    raw = bytearray(open(path, 'rb').read())
    lb, db = be16(raw, 24), be16(raw, 26)
    sect = lb + db

    def data(p):
        return HDR + p * sect + lb

    pv = data(0)
    if be16(raw, pv) != PR_SEAL:
        sys.exit('page 0 is not a physical volume root')

    # Physical root subvolume table: count at word 64, 13-word descriptors
    # at word 75 (lvID 5, lvSize 2, lvPage 2, pvPage 2, nPages 2).
    sub_count = be16(raw, pv + 64 * 2)
    changed_total = 0
    for i in range(sub_count):
        base = pv + (75 + i * 13) * 2
        lv_page = be16(raw, base + 7 * 2) | (be16(raw, base + 8 * 2) << 16)
        pv_page = be16(raw, base + 9 * 2) | (be16(raw, base + 10 * 2) << 16)
        lr = data(pv_page + lv_page)
        if be16(raw, lr) != LR_SEAL:
            print(f'subvolume {i}: no LV root seal at pv {pv_page + lv_page}, skipped')
            continue
        name = bytes(raw[lr + 8 * 2:lr + 8 * 2 + 40]).split(b'\0')[0].decode('ascii', 'replace')
        changed = []
        for slot, ordinal in SLOTS.items():
            src = pv + (PV_BOOTING_INFO + ordinal * DISK_FILE_ID_WORDS) * 2
            fid = [be16(raw, src + w * 2) for w in range(5)]
            if not any(fid):
                print(f'subvolume {i} ({name}): PV {slot} slot empty, skipped')
                continue
            first_page = [be16(raw, src + 5 * 2), be16(raw, src + 6 * 2)]
            # bootingInfo[ordinal] <- the PV DiskFileID, verbatim.
            dst = lr + (LV_BOOTING_INFO + ordinal * DISK_FILE_ID_WORDS) * 2
            before = [be16(raw, dst + w * 2) for w in range(DISK_FILE_ID_WORDS)]
            src_words = [be16(raw, src + w * 2) for w in range(DISK_FILE_ID_WORDS)]
            # rootFile[ordinal] <- {fp: {id: fID[0:2], da: fID[2:4]}, page: firstPage}
            rf = lr + (LV_ROOT_FILE + ordinal * ROOT_FILE_WORDS) * 2
            rf_before = [be16(raw, rf + w * 2) for w in range(ROOT_FILE_WORDS)]
            rf_words = fid[0:4] + first_page
            if before == src_words and rf_before == rf_words:
                continue
            for w, v in enumerate(src_words):
                put16(raw, dst + w * 2, v)
            for w, v in enumerate(rf_words):
                put16(raw, rf + w * 2, v)
            changed.append(f'{slot}: fID={fid} firstPage={first_page[0] | (first_page[1] << 16)} '
                           f'firstLink={src_words[7:9]}')
        if changed:
            words = [be16(raw, lr + w * 2) for w in range(255)]
            put16(raw, lr + 255 * 2, pilot_checksum(words))
            changed_total += len(changed)
            for c in changed:
                print(f'subvolume {i} ({name}): installed {c}')
        else:
            print(f'subvolume {i} ({name}): already installed, no change')

    if changed_total and not dry:
        open(path, 'wb').write(raw)
        print(f'wrote {path} ({changed_total} entries)')
    elif changed_total:
        print(f'dry run: would write {changed_total} entries')


if __name__ == '__main__':
    main()
