# tioga2txt — read Xerox PARC Tioga files as plaintext

Many original Xerox Cedar/Mesa source files in the CHM PARC archive (the
`.mesa`, `.config`, etc. files under `chm/cedar/`) are stored in **Tioga**
format — the Cedar text editor's rich-text on-disk format. They are **not**
plain text: naive `cat`/`grep` prints a binary trailer of formatting bytes
**and silently drops every comment** (comments live in a separate block from
the code), and it destroys the line/statement structure we need to read the
code. This tool extracts clean, readable Mesa source with comments
re-interleaved and indentation reconstructed.

## Usage

```sh
python3 tools/tioga2txt/tioga2txt.py <tioga-file>     # clean text to stdout
python3 tools/tioga2txt/tioga2txt.py in.mesa > out.txt
```

Dependency-free (Python 3 stdlib only). If a file is already plaintext (some
archive files are), it falls back to a verbatim charset-fixed dump.

## Converted docs in the repo

Pre-converted `.txt` copies of the germ/Cedar sources we reference live next
to their originals (same name + `.txt`):

- `chm/cedar/os-src/*.txt` — the head modules + PrincOps/PrincOpsUtils
  (ProcessorHeadDorado, DiskHeadDorado, EthernetOneHeadDorado, BootSwapGerm,
  HeadStartChain, GermDorado.config, SoftwareTextBlt).
- `chm/cedar/germ-src/*.txt` — bootswapgerm-6.1, germopsimpl, trapsimpl-6.1,
  mpcodes-6.1, pilotmp, and the older indigo variants.
- `chm/cedar/pilot/*.txt` — Boot.mesa (the boot Request/Location/Action
  types) and PhysicalVolumeFormat.

To regenerate them all:

```sh
for d in chm/cedar/os-src chm/cedar/germ-src chm/cedar/pilot; do
  for f in "$d"/*; do
    case "$f" in *.txt) continue;; esac
    python3 tools/tioga2txt/tioga2txt.py "$f" > "$f.txt"
  done
done
```

## Format

The full Tioga on-disk format (text/comment/control/trailer blocks, the
quirky length encoding, the control-stream opcodes) is documented in the
module docstring at the top of `tioga2txt.py`. It was reverse-engineered from
Rochus Keller's `TiogaReader.cpp` (https://github.com/rochus-keller/Cedar),
itself a port of the 1993 Xerox Cedar reader.
