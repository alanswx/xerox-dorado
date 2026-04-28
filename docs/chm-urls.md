# Pulling files from CHM's Xerox PARC archives

CHM hosts **two** Xerox PARC archive servers on Azure Blob Storage,
each with a thin Apache-style directory layout. They are not mirrors
of each other — they expose different IFS volumes / collections.

| Domain | What's there |
|---|---|
| `xeroxalto.computerhistory.org`     | Alto-era IFS volumes (`Indigo`, `Io/Murray`, `_cd6_/sil`, `_cd8_/alto…`). Has Dorado **microcode boot files** (`.eb` / `.pb`) under `Io/Murray/` plus the **MicroD** source under `Indigo/AltoSource/`. |
| `xeroxparcarchive.computerhistory.org` ★ | The fuller PARC IFS dump — including `_cd8_/dorado/` with **all the Dorado microcode source** (`Mesa.mb`, `Cedar.mb`, `AEmu.mb`, `DSemu.mb`, `Ifu.dm`, `bootstrap.dm`, `doradobaserom.dm`, `LoadMB.run`, `ReadMB.run`, etc.), `indigo/dorado/`, and `io/doradologic/`. |

If a file isn't on the first domain, try the second. The cross-reference
HTML in `DoradoDocs/manuals/` was generated from the second
archive's contents. Each archived file has at least two URL forms — a
**rendered HTML viewer** and a **raw binary**. Get the URL form right
and you avoid downloading 100k-byte octal dumps.

This doc is the result of empirically probing both servers with
`curl -I` in April 2026.

---

## URL convention (the rule)

Given an IFS path like `[Io]<Murray>AltoMesaDorado.eb!1`, the live URL
is constructed as:

```
https://<server>/<volume>/<dir>/<filename>!<version>
```

where `<server>` is `xeroxalto.computerhistory.org` or
`xeroxparcarchive.computerhistory.org` (try the latter first when
looking for Dorado source).

Where:

- `<volume>` is the IFS volume in **lowercase** for some volumes (`_cd8_`,
  `_cd6_`) and **MixedCase** for others (`Io`, `Indigo`). The casing in the
  cross-reference HTML (`[_CD8_]`, `[Io]`, `[Indigo]`) is **not** what the
  URL uses — case is normalized per-volume.
- `<dir>` matches the cross-reference path (lowercase generally for `_cd8_`
  subtrees, MixedCase for `Indigo` and `Io` subtrees).
- `<filename>` keeps original case.
- `!<version>` is the IFS version suffix and is **mandatory** — there is
  no "current" symlink without a version.

For each file there are several derived URLs:

| URL form | What it returns | Use it when |
|---|---|---|
| `dir/{name}!{ver}` | **raw bytes** (`application/octet-stream`) | Downloading. ★ |
| `dir/.{name}!{ver}.html` | HTML page, an octal-dump (`od`) of the file | Browsing in a web browser only. |
| `dir/.index.html` | directory listing in HTML | Discovering filenames in a directory. |
| `dir/{archive}!{ver}_/.index.html` | listing of an *expanded* dump-format archive | Browsing the contents of a `.dm` after the server has unpacked it. |
| `dir/{archive}!{ver}_/{member}` | raw bytes of a member of the expanded archive | Downloading individual files inside an expanded `.dm`. |

### Critical detail: the leading dot

- **Leading dot + `.html` suffix** ⇒ HTML viewer (octal dump).
- **No leading dot, no `.html` suffix** ⇒ raw binary download.

The leading dot is a Blob-Storage trick to make Azure serve a generated
HTML page without polluting the directory listing. **If you copy a URL
out of the cross-reference HTML, you will get the HTML viewer URL** —
strip the leading `.` and the trailing `.html` to get the raw URL.

### Critical detail: expanded vs. packed archives

A `.dm` (IFS dump-format archive) at `[Io]<Murray>foo.dm!3` lives at
`Io/Murray/foo.dm!3` as a single binary. If the CHM mirror has run
"expand" on it, the same content is **also** browsable at
`Io/Murray/foo.dm!3_/.index.html` (note the trailing underscore that
turns the filename into a directory name). The expanded directory
contains individual member files, each with its own raw URL like
`Io/Murray/foo.dm!3_/inner.bcpl` (no version suffix on members,
generally — they're already de-versioned by the unpacker).

Not every `.dm` is expanded. If `dir/{name}.dm!{ver}_/.index.html`
returns 404, the archive is only available as the raw `.dm` blob.

## What is and isn't actually mirrored

The local `chm/cross-reference.html` (in
`DoradoDocs/manuals/`) was generated against the **fuller**
xeroxparcarchive collection. The original xeroxalto server is a
narrower Alto-era mirror; many Dorado paths in the cross-reference
404 there but resolve fine on xeroxparcarchive.

| Cross-ref path | xeroxalto | xeroxparcarchive |
|---|---|---|
| `[_CD8_]<dorado>` (`AEmu.mb`, `Cedar.mb`, `bootstrap.dm`, etc.) | 404 | **200** |
| `[Indigo]<Dorado>` (older `doradobaserom.dm!11`, etc.) | 404 | **200** |
| `[IO]<DoradoLogic>Debug>` (`DoradoBaseDebug.dm`) | 404 | **200** |
| `[Io]<Murray>` (`AltoMesaDorado.eb`, etc.) | **200** | (not checked, redundant) |
| `[Indigo]<AltoSource>MICROD.DM!2_/` (MicroD source) | **200** | (not checked, redundant) |

Casing on xeroxparcarchive: lowercase volume + lowercase directory
(`_cd8_/dorado`, `indigo/dorado`, `io/doradologic`). Uppercase 404s.
File names retain their original case.

Note: xeroxalto's `_cd8_/.index.html` returns 200 but only lists 11
non-dorado subdirs — the `dorado` subdir lives only on xeroxparcarchive.

What IS verified to exist for Dorado purposes:

| Path | Contents |
|---|---|
| `Io/Murray/` | 95 files: `.eb` / `.pb` / `.boot` / `.germ` / `.db` / `.eg` / `.cm` / `.txt` for Dorado, D0, and DLion (Dandelion). |
| `Indigo/AltoSource/MICROD.DM!2_/` | 31 files: full BCPL source for MicroD. |
| `_cd8_/altodocs/microd.tty!2` | MicroD user manual (4.6 KB). |
| `_cd8_/alto/microd.run!2`, `microd.syms!2` | Built MicroD Alto executable + symbols. |
| `_cd6_/sil/` | SIL/Route/Build design-automation toolchain + Dorado board-route binaries. |

The canonical Dorado microcode source files (`AEmu.mb!2`, `Cedar.mb!6`,
`Ifu.dm!51`, `bootstrap.dm!20`, `doradobaserom.dm!12`, …) referenced
by the cross-index are **not on the live server.** To get them we will
need to source from Bitsavers, Al Kossow's collection, or contact CHM
directly. See `chm-archive.md` "Things absent" section.

We do have `doradobaserom.mb!13` locally already (in
`DoradoDocs/manuals/`) — pulled from somewhere offline, not
fetchable now.

## Curl recipes

Set a base directory and a base URL, then loop. Quote the `!` —
zsh treats it as a history-expansion character if not quoted.

```sh
DEST=/Users/alans/Documents/development/Dorado/chm/microcode
BASE=https://xeroxalto.computerhistory.org/Io/Murray

# A single file
curl -sS --max-time 60 -o "$DEST/AltoMesaDorado.eb!1" \
    "$BASE/AltoMesaDorado.eb!1"

# A batch
for f in AltoMesaDorado.eb!1 SmalltalkDorado.eb!1 TestDorado.eb!1 \
         Initial.eb!1 BootMeAgain.eb!1 BasicCedarDorado.pb!1; do
  curl -sS --max-time 60 -o "$DEST/$f" "$BASE/$f"
  printf "%8d  %s\n" "$(wc -c < "$DEST/$f")" "$f"
done
```

For an expanded `.dm` archive:

```sh
DEST=/Users/alans/Documents/development/Dorado/chm/microd
BASE='https://xeroxalto.computerhistory.org/Indigo/AltoSource/MICROD.DM!2_'

# First get the index to learn member names
curl -s "$BASE/.index.html" | grep -oE 'href="[^"]+"' | sort -u

# Then download each member (no version suffix on members)
for f in mdmain.bcpl mdfields.d printmb.bcpl mdadefs.d mddecl.d mdout.bcpl; do
  curl -sS -o "$DEST/$f" "$BASE/$f"
done
```

To download every member of an expanded archive in one shot:

```sh
DEST=/path/to/local/dir
BASE='https://xeroxalto.computerhistory.org/Indigo/AltoSource/MICROD.DM!2_'

curl -s "$BASE/.index.html" \
  | grep -oE 'href="[^"]+"' \
  | sed -E 's/^href="([^"]+)"$/\1/' \
  | grep -vE '^(\.|http)' \
  | while read f; do
      curl -sS -o "$DEST/$f" "$BASE/$f"
    done
```

To fetch a directory listing as a flat list of files:

```sh
curl -s "https://xeroxalto.computerhistory.org/Io/Murray/.index.html" \
  | grep -oE 'href="[^"]+"' \
  | grep -v '^href="\.' \
  | grep -v http \
  | sort -u
```

## File-extension cheat-sheet

| Extension | Meaning |
|---|---|
| `.mb` | MicroD output: micro-binary, loadable into IM via Midas/LoadMB. |
| `.eb` | Ether-bootable: `.mb` wrapped with a single overhead page so it can be served by the Pup EtherBoot protocol. |
| `.sb` | Soft-bootable: like `.eb` but bootable from disk. |
| `.pb` | Packed-binary world: a complete Pilot/Cedar world — microcode plus a memory snapshot. |
| `.boot` | Alto-style boot file (BCPL world). |
| `.germ` | Pilot "germ" — the small bootstrap that loads the rest of Pilot. |
| `.db` | Pilot debugger image. |
| `.eg` | Ether-loadable Germ. |
| `.dm` | IFS dump-format archive. May or may not be expanded by the server. |
| `.dib` | "D-machine Intermediate Binary" — output of the Micro assembler, *before* MicroD placement. |
| `.dls` | MicroD listing file (text). |
| `.midas` | Midas command file (text). |
| `.bcpl` | BCPL source. |
| `.bcd` | BCPL compiled output. |
| `.run` | Alto executable. |
| `.syms` | Symbol table for a `.run`. |
| `.cm` | Command file (text, like a shell script). |
| `.tty` | Documentation in TTY-printable text. |
| `.press` | Xerox PARC document/print format (we have these as PDFs already). |
| `.mc`, `.mu` | Micro-assembler source / disassembler output. |

## Archive layout summary

```
xeroxalto.computerhistory.org/
├── _cd6_/                   ← CD volume 6: SIL/Route design automation
│   └── sil/                 (Dorado board route binaries; ECL/TTL data sheets)
├── _cd8_/                   ← CD volume 8: Alto + altodocs + boot files
│   ├── alto/                (Alto executables incl. microd.run)
│   ├── altodocs/            (Alto documentation incl. microd.tty)
│   ├── basicdisks/
│   ├── bootfiles/
│   └── ... (8 more)
├── Indigo/                  ← server volume Indigo: source code
│   ├── AltoSource/          (BCPL sources, incl. MICROD.DM!2_/)
│   ├── BravoX/              (Bravo source)
│   └── ... (16 more)
├── Io/                      ← server volume Io
│   └── Murray/              (Murray Edwards' boot/microcode collection — Dorado lives here)
└── (other volumes — Cyan, Pixel, Phylum, Eros, Eris, … — mostly Mesa/Cedar source)
```

## Pitfalls

1. **Don't follow links from the cross-reference HTML directly.** Most
   of them point to HTML viewers; many point to directories that 404.
   Use them only as filename hints.
2. **Quote URLs containing `!`** in zsh / bash. `!` is history expansion.
3. **Expect 404 for anything labeled `[_CD8_]<dorado>`,
   `[Indigo]<Dorado>`, `[IO]<DoradoLogic>`.** These directories are in
   the cross-reference index but not in the live mirror.
4. **The HTML viewer for a binary is HUGE.** A 35 KB `.eb` file
   becomes a ~600 KB octal-dump HTML page. Always download the raw form.
5. **The server normalizes some URLs but not all.** Verified:
   `/_CD8_/...` 404s while `/_cd8_/...` 200s. Stick with the casing
   shown in the live `.index.html` listings.
