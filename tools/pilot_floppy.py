#!/usr/bin/env python3
"""Read Xerox Pilot 8-inch floppy images (ImageDisk .IMD) and extract files.

These are the Xerox 1108/1101 distribution floppies -- e.g. the 70 disks in
chm/lisp/release-floppies/InterlispReleaseFloppyImsges.zip (LYRIC, KOTO,
HARMONY, LOOPS, SOURCE).  Two layers:

  1. ImageDisk (.IMD), Dave Dunfield's container: ASCII header ending 0x1A,
     then per track  mode/cyl/head/nsec/sizecode + sector-numbering map +
     per-sector type byte (1 literal, 2 run-compressed, 3-8 deleted/error
     variants).  These disks are 77 cyl x 2 heads; cyl 0 head 0 is FM
     26x128 (the IBM 3740 label track, EBCDIC HDR1), cyl 0 head 1 is MFM
     26x256, and every other track is MFM 15x512.

  2. The Xerox Pilot floppy volume.  Record layouts are Xerox's own, from
     the Interlisp FLOPPY package -- chm/lisp-archive/koto/lisp/sources/
     FLOPPY!1 (Kelly Roach, 1985), DATATYPE PSECTOR9 / PMPAGE / PLPAGE and
     BLOCKRECORD PFILELIST / DATATYPE PFLE, with
        SEAL.PSECTOR9  = 49932 (0xC30C)
        SEAL.PMPAGE    = 13003 (0x32CB)
        SEAL.PFILELIST = 45771 (0xB2CB)
        SEAL.PLPAGE    = 43690 (0xAAAA)
     and \\FLOPPY.CYLINDERS 77 / TRACKSPERCYLINDER 2 / SECTORSPERTRACK 15.

Files are laid out contiguously as
    [PMPAGE marker] [PLPAGE leader] [data pages ...] [PMPAGE] [PLPAGE] ...
so walking the PLPAGE seals recovers every file without having to model
Pilot's sector numbering.  Verified against Lyric Library #1: leader at
byte 12032 declares 23 pages / 11563 bytes, and 512 + 11563 rounded up to
a page boundary lands exactly on the next PMPAGE.

Usage:
    pilot_floppy.py list    IMAGE[.IMD|.DMK] ...
    pilot_floppy.py extract  OUTDIR IMAGE[.IMD|.DMK] ...
    pilot_floppy.py assemble OUTDIR IMAGE[.IMD|.DMK] ...   # rejoin spanning files
"""

import os
import struct
import sys

SECTOR_SIZE = {0: 128, 1: 256, 2: 512, 3: 1024, 4: 2048, 5: 4096, 6: 8192}

SEAL_PSECTOR9 = 0xC30C
SEAL_PLPAGE = 0xAAAA

PAGE = 512


def imd_to_image(path):
    """Flatten an .IMD into the linear sector image, in sector-number order."""
    d = open(path, "rb").read()
    i = d.find(b"\x1a") + 1
    out = bytearray()
    while i < len(d):
        mode, cyl, head, nsec, szc = d[i], d[i + 1], d[i + 2], d[i + 3], d[i + 4]
        i += 5
        has_cyl_map, has_head_map = head & 0x80, head & 0x40
        smap = list(d[i:i + nsec])
        i += nsec
        if has_cyl_map:
            i += nsec
        if has_head_map:
            i += nsec
        size = SECTOR_SIZE[szc]
        sectors = {}
        for s in range(nsec):
            t = d[i]
            i += 1
            if t == 0:
                rec = b"\x00" * size
            elif t in (1, 3, 5, 7):
                rec = d[i:i + size]
                i += size
            elif t in (2, 4, 6, 8):
                rec = bytes([d[i]]) * size
                i += 1
            else:
                raise ValueError("bad IMD sector type %d at %d" % (t, i))
            sectors[smap[s]] = rec
        for s in sorted(sectors):
            out += sectors[s]
    return bytes(out)


def dmk_to_image(path):
    """Flatten a .DMK (David Keil) raw-track image into linear sectors.

    Header: [0] write-protect, [1] tracks, [2:4] track length LE (incl. the
    128-byte IDAM table), [4] flags (0x40 single sided, 0x10 single-density
    only), [12:16] 0 = real disk.  Each track begins with 64 little-endian
    IDAM pointers: bits 0-13 = offset from the start of the track to the
    0xFE ID address mark, bit 15 set = double density (MFM).  In a
    mixed-density image, single-density bytes are stored DOUBLED, so an FM
    track is read with a stride of 2.
    """
    d = open(path, "rb").read()
    ntracks = d[1]
    tracklen = d[2] | (d[3] << 8)
    flags = d[4]
    heads = 1 if (flags & 0x40) else 2
    sd_only = bool(flags & 0x10)
    out = bytearray()
    pos = 16
    for _ in range(ntracks):
        for _h in range(heads):
            track = d[pos:pos + tracklen]
            pos += tracklen
            if len(track) < 128:
                continue
            secs = {}
            for i in range(64):
                v = track[i * 2] | (track[i * 2 + 1] << 8)
                if v == 0:
                    continue
                off = v & 0x3FFF
                step = 1 if (bool(v & 0x8000) or sd_only) else 2
                if off + 7 * step > len(track) or track[off] != 0xFE:
                    continue
                szc = track[off + 4 * step]
                size = 128 << (szc & 3)
                sec = track[off + 3 * step]
                # The data address mark (FB normal, F8 deleted) follows the
                # ID CRC after a gap; scan a bounded window for it.
                p, dam, limit = off + 7 * step, None, off + 64 * step
                while p < min(len(track), limit):
                    if track[p] in (0xFB, 0xF8):
                        dam = p
                        break
                    p += step
                if dam is None:
                    continue
                start = dam + step
                data = track[start:start + size * step:step]
                if len(data) == size:
                    secs[sec] = bytes(data)
            for s in sorted(secs):
                out += secs[s]
    return bytes(out)


def to_image(path):
    """Dispatch on container: .IMD or .DMK."""
    head = open(path, "rb").read(4)
    if head[:3] == b"IMD":
        return imd_to_image(path)
    if path.lower().endswith(".dmk"):
        return dmk_to_image(path)
    # IMD headers can carry a different herald; fall back on extension.
    return imd_to_image(path) if path.lower().endswith(".imd") else dmk_to_image(path)


def words(buf):
    return struct.unpack(">%dH" % (len(buf) // 2), buf[:len(buf) // 2 * 2])


def volume_label(img):
    """PSECTOR9: SEAL, VERSION, CYLINDERS, TRACKSPERCYLINDER, SECTORSPERTRACK,
    ... , \\LABELLENGTH at word 21, \\LABEL (106 words) from word 22."""
    w = words(img)
    for i in range(len(w) - 24):
        if w[i] == SEAL_PSECTOR9 and w[i + 1] == 1:
            n = w[i + 21]
            if n > 106:
                continue
            base = (i + 22) * 2
            return {
                "offset": i * 2,
                "cylinders": w[i + 2],
                "tracks_per_cyl": w[i + 3],
                "sectors_per_track": w[i + 4],
                "filelist_start": w[i + 5],
                "filelist_length": w[i + 8],
                "label": img[base:base + n].decode("latin-1"),
            }
    return None


def files(img):
    """Walk PLPAGE leader pages.  SWAPPEDFIXP fields are low word first:
    words 7-8 PAGELENGTH (pages of THIS chunk), 9-10 HUGEPAGESTART,
    11-12 HUGEPAGELENGTH (pages of the WHOLE file), 13-14 HUGELENGTH
    (bytes of the whole file); word 15 \\NAMELENGTH, 16 NAMEMAXLENGTH,
    17.. \\NAME.

    A file too big for one floppy is written in HUGEPILOT/SYSOUT mode as
    one chunk per disk, each with the same name, HUGEPAGELENGTH and
    HUGELENGTH, and its own HUGEPAGESTART (IRM section 24.10).  So `bytes`
    here is this chunk's payload; `huge_*` say where it belongs.  Use the
    `assemble` command to put a spanning file back together."""
    w = words(img)
    out = []
    for i in range(len(w) - 70):
        if w[i] != SEAL_PLPAGE or w[i + 1] != 1:
            continue
        namelen = w[i + 15]
        if not 0 < namelen <= 100:
            continue
        base = (i + 17) * 2
        name = img[base:base + namelen].decode("latin-1")
        if not name.isprintable():
            continue
        pages = w[i + 7] | (w[i + 8] << 16)
        huge_start = w[i + 9] | (w[i + 10] << 16)
        huge_pages = w[i + 11] | (w[i + 12] << 16)
        huge_bytes = w[i + 13] | (w[i + 14] << 16)
        spanning = huge_pages > pages
        data_at = i * 2 + PAGE
        # This chunk's payload is PAGELENGTH pages; for a self-contained
        # file that is the whole thing, trimmed to the byte length.
        nbytes = pages * PAGE if spanning else huge_bytes
        nbytes = min(nbytes, max(0, len(img) - data_at))
        out.append({
            "name": name,
            "pages": pages,
            "bytes": nbytes,
            "huge_start": huge_start,
            "huge_pages": huge_pages,
            "huge_bytes": huge_bytes,
            "spanning": spanning,
            "leader_at": i * 2,
            "data": img[data_at:data_at + nbytes],
        })
    return out


def assemble(images, outdir):
    """Reassemble files that span several disks, in HUGEPAGESTART order.

    Order of the input images does not matter: each chunk carries its own
    offset.  Self-contained files are written as-is."""
    chunks = {}
    for path in images:
        try:
            img = to_image(path)
        except Exception as exc:                       # noqa: BLE001
            print("%-28s  container error: %s" % (os.path.basename(path), exc))
            continue
        for f in files(img):
            key = (f["name"], f["huge_bytes"], f["huge_pages"])
            chunks.setdefault(key, {})[f["huge_start"]] = (f["data"], path)
    os.makedirs(outdir, exist_ok=True)
    made = []
    for (name, huge_bytes, huge_pages), parts in sorted(chunks.items()):
        buf = bytearray()
        gaps = []
        expect = 0
        for start in sorted(parts):
            data, _src = parts[start]
            if start != expect:
                gaps.append((expect, start))
            buf += data
            expect = start + len(data) // PAGE
        blob = bytes(buf[:huge_bytes]) if huge_bytes else bytes(buf)
        safe = name.replace("/", "_").replace(";", "_")
        out = os.path.join(outdir, safe)
        with open(out, "wb") as fh:
            fh.write(blob)
        status = "OK" if (len(blob) == huge_bytes and not gaps) else "INCOMPLETE"
        if gaps:
            status += " gaps@pages=%s" % gaps
        if len(blob) != huge_bytes:
            status += " got %d of %d bytes" % (len(blob), huge_bytes)
        made.append((safe, len(blob), len(parts), status))
        print("  %-24s %9d bytes  from %d chunk(s)  %s"
              % (safe, len(blob), len(parts), status))
    return made


def main(argv):
    if len(argv) < 3 or argv[1] not in ("list", "extract", "assemble"):
        print(__doc__)
        return 2
    mode = argv[1]
    if mode == "assemble":
        assemble(argv[3:], argv[2])
        return 0
    if mode == "extract":
        outdir, images = argv[2], argv[3:]
    else:
        outdir, images = None, argv[2:]
    if not images:
        print(__doc__)
        return 2

    total_files = total_bytes = 0
    for path in images:
        try:
            img = to_image(path)
        except Exception as exc:                       # noqa: BLE001
            print("%-28s  IMD ERROR: %s" % (os.path.basename(path), exc))
            continue
        vol = volume_label(img)
        fs = files(img)
        label = vol["label"] if vol else "(no Pilot volume descriptor)"
        print("%-24s  %-40s %3d files" % (os.path.basename(path), label, len(fs)))
        for f in fs:
            total_files += 1
            total_bytes += f["bytes"]
            if mode == "list":
                print("      %-40s %7d bytes  %4d pages" % (f["name"], f["bytes"], f["pages"]))
            else:
                sub = os.path.join(outdir, os.path.splitext(os.path.basename(path))[0])
                os.makedirs(sub, exist_ok=True)
                safe = f["name"].replace("/", "_")
                with open(os.path.join(sub, safe), "wb") as fh:
                    fh.write(f["data"])
    print("\n%d files, %d bytes total" % (total_files, total_bytes))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
