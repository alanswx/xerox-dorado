#!/usr/bin/env python3
"""Build one clean, self-contained distribution per Interlisp-D release.

Reads the Xerox 1108/1186 distribution floppies we hold (see
docs/xerox-media-catalog.md), reassembles each release's multi-floppy
sysout, and lays the release out as

    <outdir>/<release>/
        LISP.SYSOUT          reassembled from the "Lisp Sysout #N" disks
        library/             the Library disks
        lispusers/           the LispUsers disks
        other/               installation utility, system files, fonts, ...
        MANIFEST.txt         source disk for every file, plus sizes

Sysout reassembly is exact and self-checking: each chunk's leader page
carries HUGEPAGESTART / HUGEPAGELENGTH / HUGELENGTH, so the chunks are
ordered by the medium, not by us, and a short or gapped result is
reported rather than silently written.  Validated against a known-good
artifact: the Lyric sysout rebuilt from its five floppies is
byte-identical to chm/archiveorg/_chm-parc_interlisp-lyric/LISP.SYSOUT!1,
which the emulator boots.

Usage:  build_lisp_distributions.py MEDIAROOT OUTDIR [release ...]
where MEDIAROOT is a directory tree of unpacked .IMD / .DMK images.
"""

import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from pilot_floppy import to_image, volume_label, files, PAGE   # noqa: E402

# Which unpacked media directory belongs to which release.  Matched as a
# substring of the image's path, first match wins.
RELEASES = [
    ("carol-1983",      ["1108_carol_1983"]),
    ("fugue-1983",      ["1108_fugue_1983"]),
    ("harmony",         ["InterlispReleaseFloppies/HARMONY", "/HARMONY/"]),
    ("intermezzo-1985", ["1108_intermezzo"]),
    ("koto",            ["/KOTO/"]),
    ("lyric",           ["/LYRIC/"]),
    ("medley-1.0",      ["Medley_1.0_1108", "1108_medley_1.0", "1186_Medley1.0"]),
    ("medley-1.1",      ["1108_medley_1.1"]),
    ("medley-copies",   ["medley_copies"]),
    ("loops",           ["/LOOPS/", "LOOPS_110987"]),
    ("xde-5.0",         ["8010_xde5.0", "XDE_5.0_imd"]),
]

SYSOUT_RE = re.compile(r"sysout", re.I)
LIB_RE = re.compile(r"librar", re.I)
USERS_RE = re.compile(r"lispusers|users packages|lisp users", re.I)


def classify(label):
    if SYSOUT_RE.search(label):
        return "sysout"
    if USERS_RE.search(label):
        return "lispusers"
    if LIB_RE.search(label):
        return "library"
    return "other"


def release_of(path):
    for name, pats in RELEASES:
        for p in pats:
            if p in path:
                return name
    return None


def main(argv):
    if len(argv) < 3:
        print(__doc__)
        return 2
    root, outdir, only = argv[1], argv[2], set(argv[3:])

    images = []
    for dirpath, _dirs, fs in os.walk(root, followlinks=True):
        for f in fs:
            if f.lower().endswith((".imd", ".dmk")):
                images.append(os.path.join(dirpath, f))
    images.sort()

    by_release = {}
    for p in images:
        rel = release_of(p)
        if rel and (not only or rel in only):
            by_release.setdefault(rel, []).append(p)

    for rel in sorted(by_release):
        paths = by_release[rel]
        dest = os.path.join(outdir, rel)
        os.makedirs(dest, exist_ok=True)
        manifest = []
        chunks = {}
        counts = {"library": 0, "lispusers": 0, "other": 0}
        for p in paths:
            try:
                img = to_image(p)
            except Exception as exc:                    # noqa: BLE001
                manifest.append("!! %s container error: %s" % (os.path.basename(p), exc))
                continue
            vol = volume_label(img)
            label = vol["label"] if vol else "(no volume descriptor)"
            kind = classify(label)
            for f in files(img):
                if f["spanning"] or kind == "sysout":
                    key = (f["name"], f["huge_bytes"], f["huge_pages"])
                    chunks.setdefault(key, {})[f["huge_start"]] = f["data"]
                    manifest.append("   chunk %-14s page %6d  %s [%s]"
                                    % (f["name"], f["huge_start"],
                                       os.path.basename(p), label))
                    continue
                sub = kind if kind in ("library", "lispusers") else "other"
                os.makedirs(os.path.join(dest, sub), exist_ok=True)
                safe = f["name"].replace("/", "_").replace(";", "_")
                with open(os.path.join(dest, sub, safe), "wb") as fh:
                    fh.write(f["data"])
                counts[sub] += 1
                manifest.append("%-10s %-40s %8d  %s [%s]"
                                % (sub, f["name"], f["bytes"],
                                   os.path.basename(p), label))

        notes = []
        for (name, huge_bytes, huge_pages), parts in sorted(chunks.items()):
            buf = bytearray()
            expect, gaps = 0, []
            for start in sorted(parts):
                if start != expect:
                    gaps.append((expect, start))
                buf += parts[start]
                expect = start + len(parts[start]) // PAGE
            blob = bytes(buf[:huge_bytes]) if huge_bytes else bytes(buf)
            safe = name.replace("/", "_").replace(";", "_")
            # A release can carry two sysouts whose names differ only in
            # case (e.g. 1108 "Lisp.Sysout" and 1186 "lisp.sysout").  macOS
            # is case-insensitive, so the second write silently destroyed
            # the first; disambiguate by byte length.
            if os.path.exists(os.path.join(dest, safe)) or \
               any(x.lower() == safe.lower() for x in os.listdir(dest)):
                stem, _dot, ext = safe.partition(".")
                safe = "%s-%db.%s" % (stem, len(blob), ext) if ext else \
                       "%s-%db" % (stem, len(blob))
            with open(os.path.join(dest, safe), "wb") as fh:
                fh.write(blob)
            ok = (not gaps) and (not huge_bytes or len(blob) == huge_bytes)
            notes.append("%-16s %9d bytes  %d chunks  %s"
                         % (safe, len(blob), len(parts),
                            "OK" if ok else "INCOMPLETE gaps=%s got=%d want=%d"
                            % (gaps, len(blob), huge_bytes)))

        with open(os.path.join(dest, "MANIFEST.txt"), "w") as fh:
            fh.write("release: %s\n%d source disks\n\n" % (rel, len(paths)))
            for n in notes:
                fh.write("SYSOUT  " + n + "\n")
            fh.write("\n")
            fh.write("\n".join(manifest) + "\n")

        print("%-16s %2d disks  library=%-4d lispusers=%-4d other=%-4d  %s"
              % (rel, len(paths), counts["library"], counts["lispusers"],
                 counts["other"], "; ".join(notes) or "(no sysout)"))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
