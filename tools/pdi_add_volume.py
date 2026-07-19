#!/usr/bin/env python3
"""Add a logical volume to an existing Pilot PDI, the authentic Dorado layout.

A PARC Dorado disk carried several Pilot LOGICAL volumes inside one PHYSICAL
volume -- the Cedar volume, optionally a Debugger volume, optionally an Alto
partition for emulation (Cedar 6.1 ReleaseMessage.tioga asks "How many Alto
partitions?" and "Do you want a Debugger volume?").  Our images have all been
single-LV, so Cedar has only ever seen one.

This grows an image in place-ish (source is never modified): the existing
physical volume, its subvolume and every one of its pages stay byte-identical
at the same physical page numbers, and the new volume is appended in fresh
space beyond them.  That keeps a bootable volume bootable -- the risky part of
a multi-volume disk is the volume you already have, so we don't touch it.

Requires the CHS-aware PDI loader (dorado/src/pdi.c, 2026-07-19): the header
stores cylinders/heads/sectors and the page count is their product, so growing
past 65535 pages means giving the image a real geometry.  A Trident T-300 is
815 x 19 x 9 sectors of 1024 words = 278,730 512-byte pages.

Usage:
    pdi_add_volume.py SRC.pdi OUT.pdi --name CedarData [--pages N]
                      [--geometry CYL,HEADS,SECTORS]
"""

import argparse
import struct
import sys

MAGIC = b"PARCDISK"
HEADER_BYTES = 512

PR_SEAL, PR_VERSION = 0o121212, 6      # physical volume root
LR_SEAL, LR_VERSION = 0o131313, 5      # logical volume root
PSM_SEAL, PSM_VERSION = 0o141414, 0    # physical subvolume end marker

ATTR_SUB_VOLUME_MARKER = 4
ATTR_LOGICAL_ROOT = 5
ATTR_FREE_PAGE = 9728

SUBVOL_COUNT_WORD = 64
SUBVOL_BASE_WORD = 75
SUBVOL_WORDS = 13
MAX_SUBVOLS = 6
VOLUME_LABEL_LEN = 40                  # bytes
VOLUME_TYPE_CEDAR = 3


def rdw(b, wi):
    return struct.unpack_from(">H", b, wi * 2)[0]


def wrw(b, wi, v):
    struct.pack_into(">H", b, wi * 2, v & 0xFFFF)


def rdlong(b, wi):                     # Mesa LONG: low word first
    return rdw(b, wi) | (rdw(b, wi + 1) << 16)


def wrlong(b, wi, v):
    wrw(b, wi, v & 0xFFFF)
    wrw(b, wi + 1, (v >> 16) & 0xFFFF)


def pilot_checksum(words):
    cs = 0
    for w in words:
        t = cs + w
        cs = (t + 1) & 0xFFFF if t > 0xFFFF else t & 0xFFFF
        cs = ((cs << 1) | 1) & 0xFFFF if cs & 0x8000 else (cs << 1) & 0xFFFF
    return 0 if cs == 0xFFFF else cs


def set_page_checksum(data, page_words):
    wrw(data, page_words - 1, pilot_checksum([rdw(data, i)
                                              for i in range(page_words - 1)]))


def make_label(label_words, file_id, file_page, attributes):
    """Pilot page label: fileID[5], filePage (LONG), attributes, dontCare[2]."""
    lab = bytearray(label_words * 2)
    for i, w in enumerate(file_id):
        wrw(lab, i, w)
    wrlong(lab, 5, file_page)
    wrw(lab, 7, attributes)
    return lab


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("src")
    ap.add_argument("out")
    ap.add_argument("--name", required=True, help="new logical volume name")
    ap.add_argument("--copy-from", metavar="PDI",
                    help="populate the new volume from this image's first "
                         "logical volume instead of leaving it blank")
    ap.add_argument("--pages", type=int, default=0,
                    help="size of the new LV in pages (default: fill the disk)")
    ap.add_argument("--geometry", default="815,19,9",
                    help="CYL,HEADS,SECTORS of the grown image "
                         "(default: Trident T-300)")
    ap.add_argument("--pages-per-sector", type=int, default=2,
                    help="512-byte Pilot pages per hardware sector "
                         "(T-300 sectors are 1024 words = 2 pages)")
    args = ap.parse_args()

    raw = bytearray(open(args.src, "rb").read())
    if raw[:8] != MAGIC:
        sys.exit(f"{args.src}: not a PDI (bad magic)")

    label_bytes = rdw(raw, 12)
    data_bytes = rdw(raw, 13)
    record = label_bytes + data_bytes
    label_words = label_bytes // 2
    page_words = data_bytes // 2
    src_cyl, src_heads, src_sec = rdw(raw, 8), rdw(raw, 9), rdw(raw, 10)
    src_pages = src_cyl * max(src_heads, 1) * max(src_sec, 1)
    have = (len(raw) - HEADER_BYTES) // record
    if have < src_pages:
        sys.exit(f"{args.src}: header says {src_pages} pages, file holds {have}")

    cyl, heads, sec = (int(x) for x in args.geometry.split(","))
    total = cyl * heads * sec * args.pages_per_sector
    if total <= src_pages:
        sys.exit(f"target geometry holds {total} pages, "
                 f"not more than the source's {src_pages}")

    def page(img, n):
        off = HEADER_BYTES + n * record
        return (memoryview(img)[off:off + label_bytes],
                memoryview(img)[off + label_bytes:off + record])

    # --- read the physical root, and find where the existing volumes end ---
    _, pr = page(raw, 0)
    if rdw(pr, 0) != PR_SEAL:
        sys.exit("page 0 is not a physical-volume root (bad seal)")
    count = rdw(pr, SUBVOL_COUNT_WORD)
    if count >= MAX_SUBVOLS:
        sys.exit(f"already {count} subvolumes; Pilot allows {MAX_SUBVOLS}")
    subs = []
    for i in range(count):
        wi = SUBVOL_BASE_WORD + i * SUBVOL_WORDS
        subs.append(dict(lv_id=[rdw(pr, wi + k) for k in range(5)],
                         lv_size=rdlong(pr, wi + 5),
                         lv_page=rdlong(pr, wi + 7),
                         pv_page=rdlong(pr, wi + 9),
                         n_pages=rdlong(pr, wi + 11)))
    pv_id = [rdw(pr, 3 + k) for k in range(5)]
    end = max(s["pv_page"] + s["n_pages"] for s in subs)
    end += 1                                    # existing end-of-subvolume marker

    # A new volume id that cannot collide with the ones already here. Every
    # page label carries it, so a duplicate would make Pilot treat the second
    # volume as a clone of the first and reject it.
    used = {tuple(s["lv_id"]) for s in subs} | {tuple(pv_id)}
    lv_id = None
    for cand in range(0x4C56, 0x4C56 + 64):     # "LV", then keep walking
        if (cand, 0, 0, 0, 0) not in used:
            lv_id = [cand, 0, 0, 0, 0]
            break
    if lv_id is None:
        sys.exit("could not pick a free logical-volume id")

    new_pv_page = end
    avail = total - new_pv_page - 1              # leave room for our marker

    src2 = src2_lv = None
    if args.copy_from:
        src2 = bytearray(open(args.copy_from, "rb").read())
        if src2[:8] != MAGIC:
            sys.exit(f"{args.copy_from}: not a PDI")
        if rdw(src2, 12) != label_bytes or rdw(src2, 13) != data_bytes:
            sys.exit("--copy-from image has a different sector layout")
        _, pr2 = (memoryview(src2)[HEADER_BYTES:HEADER_BYTES + label_bytes],
                  memoryview(src2)[HEADER_BYTES + label_bytes:
                                   HEADER_BYTES + record])
        if rdw(pr2, 0) != PR_SEAL:
            sys.exit(f"{args.copy_from}: page 0 is not a physical-volume root")
        if rdw(pr2, SUBVOL_COUNT_WORD) < 1:
            sys.exit(f"{args.copy_from}: no logical volume to copy")
        wi2 = SUBVOL_BASE_WORD
        src2_lv = dict(lv_id=[rdw(pr2, wi2 + k) for k in range(5)],
                       lv_size=rdlong(pr2, wi2 + 5),
                       lv_page=rdlong(pr2, wi2 + 7),
                       pv_page=rdlong(pr2, wi2 + 9),
                       n_pages=rdlong(pr2, wi2 + 11))
        n_pages = src2_lv["n_pages"]
    else:
        n_pages = args.pages if args.pages else avail
    if n_pages < 4 or n_pages > avail:
        sys.exit(f"new volume needs 4..{avail} pages, asked for {n_pages}")
    marker = new_pv_page + n_pages

    # --- grow the image ---
    img = bytearray(HEADER_BYTES + total * record)
    img[:len(raw)] = raw
    for p in range(src_pages, total):            # zeroed pages are "not a page"
        pass                                     # (already zero from allocation)

    img[16:18] = struct.pack(">H", cyl)
    img[18:20] = struct.pack(">H", heads)
    img[20:22] = struct.pack(">H", sec)
    if args.pages_per_sector != 1:
        # The geometry words describe hardware sectors; the loader multiplies
        # them for the page count, so fold the pages-per-sector into heads.
        img[18:20] = struct.pack(">H", heads * args.pages_per_sector)

    _, pr = page(img, 0)

    # --- the new subvolume descriptor ---
    wi = SUBVOL_BASE_WORD + count * SUBVOL_WORDS
    for k, w in enumerate(lv_id):
        wrw(pr, wi + k, w)
    wrlong(pr, wi + 5, src2_lv["lv_size"] if src2_lv else n_pages)  # lvSize
    wrlong(pr, wi + 7, 0)                        # lvPage: LV root is logical 0
    wrlong(pr, wi + 9, new_pv_page)              # pvPage
    wrlong(pr, wi + 11, n_pages)                 # nPages
    wrw(pr, SUBVOL_COUNT_WORD, count + 1)
    set_page_checksum(pr, page_words)

    name = args.name[:VOLUME_LABEL_LEN]
    if src2_lv:
        # Copy the donor volume verbatim. Page labels carry FILE ids (and 0
        # for free pages), not the volume id, so the file system moves
        # unchanged; only the logical-volume root names the volume, and only
        # it needs renumbering. File ids may repeat across volumes -- a
        # logical volume is their namespace.
        base = src2_lv["pv_page"]
        for lp in range(n_pages):
            so = HEADER_BYTES + (base + lp) * record
            do = HEADER_BYTES + (new_pv_page + lp) * record
            img[do:do + record] = src2[so:so + record]
        lab, d = page(img, new_pv_page)
        if rdw(d, 0) != LR_SEAL:
            sys.exit("copied logical page 0 is not a logical-volume root")
        lab[:] = make_label(label_words, lv_id, 0, ATTR_LOGICAL_ROOT)
        for k, w in enumerate(lv_id):
            wrw(d, 2 + k, w)
        wrw(d, 7, len(name))
        d[8 * 2:8 * 2 + VOLUME_LABEL_LEN] = \
            name.encode().ljust(VOLUME_LABEL_LEN, b"\0")
        set_page_checksum(d, page_words)
    else:
        # --- logical page 0 of the new volume: its logical-volume root ---
        lab, d = page(img, new_pv_page)
        lab[:] = make_label(label_words, lv_id, 0, ATTR_LOGICAL_ROOT)
        wrw(d, 0, LR_SEAL)
        wrw(d, 1, LR_VERSION)
        for k, w in enumerate(lv_id):
            wrw(d, 2 + k, w)
        wrw(d, 7, len(name))
        d[8 * 2:8 * 2 + VOLUME_LABEL_LEN] = \
            name.encode().ljust(VOLUME_LABEL_LEN, b"\0")
        wrw(d, 28, VOLUME_TYPE_CEDAR)
        wrlong(d, 29, n_pages)
        set_page_checksum(d, page_words)

        # --- the rest of the new volume: free pages ---
        for lp in range(1, n_pages):
            lab, _ = page(img, new_pv_page + lp)
            lab[:] = make_label(label_words, lv_id, lp, ATTR_FREE_PAGE)

    # --- end-of-subvolume marker ---
    lab, d = page(img, marker)
    lab[:] = make_label(label_words, pv_id, 0, ATTR_SUB_VOLUME_MARKER)
    wrw(d, 0, PSM_SEAL)
    wrw(d, 1, PSM_VERSION)
    for k, w in enumerate(pv_id):
        wrw(d, 2 + k, w)
    set_page_checksum(d, page_words)

    open(args.out, "wb").write(img)
    print(f"{args.out}: {count + 1} logical volumes, {total} pages "
          f"({total * data_bytes // (1024 * 1024)} MB), geometry "
          f"{cyl}x{heads * args.pages_per_sector}x{sec}")
    for i, s in enumerate(subs):
        print(f"  lv[{i}] id={s['lv_id'][0]:#06x} "
              f"pv_page={s['pv_page']} pages={s['n_pages']} (unchanged)")
    print(f"  lv[{count}] id={lv_id[0]:#06x} pv_page={new_pv_page} "
          f"pages={n_pages} name={name!r} (new)")


if __name__ == "__main__":
    main()
