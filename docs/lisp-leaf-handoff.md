# Handoff — Interlisp-D on the network (Leaf), 2026-08-01..03

**Read §1 first.** Eleven commits sit on branch `lisp-lyric-library-and-leaf`
and **none are pushed**. There was a hold on pushing at one point (a live
demo was running off GitHub Pages); confirm with Alan before pushing.

---

## 1. State in one paragraph

Interlisp-D Lyric now loads files it does not have on its disk, over **Leaf**,
the IFS random-access file protocol. `make verify-lisp-leaf` proves it:
`AISBLT` is deliberately absent from the pack, and the gate watches it open
and stream in 33 reads. That lifts the pack's hard 22,736-page ceiling, which
was the thing blocking a bigger Lisp demo. Separately the pack itself went
from 119 to 204 packages plus 14 fonts. **One thing is unfinished:** HELPSYS
(the online Interlisp Reference Manual, and the "docs on a button" the demo
wants) does not open, because `IRM.HOST&DIR` is unbound in the booted world
even though the init file that sets it is on the pack intact. §5 has the
precise diagnosis and the next thing to try.

---

## 2. What works, with the commands

```sh
cd dorado

# The Lyric desktop with 204 Interlisp packages + 14 display fonts on disk
make run-lisp-lispusers-sdl      # click the Prompt Window, then type
make run-lisp-sketch-sdl         # auto-loads SKETCH (~3 min of wall clock)
make run-lisp-filebrowser-sdl    # auto-loads FILEBROWSER (+TABLEBROWSER)

# Gates
make verify-lisp                 # desktop restores, 208985 px
make verify-lisp-leaf            # loads a package NOT on disk, over the network
make verify-lisp-lispusers       # loads one that IS on disk
make test                        # 12 suites, 193 assertions
```

Verified loading: `GREP`, `HRULE`, `PACMAN`, `FILEBROWSER` (pulls
`TABLEBROWSER`), `HELPSYS` (pulls `DINFO` + `TEDIT`), `PLOT`, and **`SKETCH`**,
which resolves SKETCHELEMENTS/SKETCHEDIT/SKETCHOBJ/SKETCHBMELT/SCALEBITMAP off
`{DSK}` by itself. `(IL:FILEBROWSER)` — **no arguments** — opens the browser.

**Loading is proved; driving is mostly not.** `(IL:SKETCH)` returns without a
window. Interlisp tools call `GETREGION` and expect you to sweep out a
rectangle; `--drag X1,Y1,X2,Y2` exists in the headless frontend for that but
did not produce a Sketch window. Unfinished.

---

## 3. The Leaf implementation

All in `src/ethernet.c`, guarded by `DORADO_PUP_TYPE_LEAF` (0260).
`chm/leaf/` holds the two sources everything was checked against:

| file | what it is |
|---|---|
| `IfsLeaf.decl`, `IfsLeaf*.bcpl` | the **IFS server**, from xeroxalto `_cd8_/ifs/ifsleaf.dm!2_` |
| `LEAF.lisp-client` | **Interlisp's client**, `eris/lispcore/sources/LEAF!33` |

The client is the more useful of the two: it is the half we must satisfy, it
names constants the BCPL only implies, and `AnswerSetOp` lives in the IFS
Sequin module which survives only as compiled `.BR`.

Implemented: **Reset, Params, Open, Read, Close, ErrorAnswer**. Not
implemented: Write, Delete, Truncate, CloseTransaction, Telnet (the served
tree is read-only).

### Three things that were not guessable from the header

1. **Sequin keeps its state in the Pup ID field.** From `LEAF!33`:
   ```
   (BLOCKRECORD SEQUINSTART ((NIL 2 WORD)
      (ALLOCATE BYTE) (RECEIVESEQ BYTE) (SEQCONTROL BYTE) (SENDSEQ BYTE)
      (* Sequin uses ID fields of PUP for control info)))
   ```
   `\SEQUIN.DATA 0`, `\SEQUIN.ACK 1`, `\SEQUIN.OPEN 5`. The first request
   carries seqcontrol 5, so **echoing the request ID replies with another
   OPEN** and the client sits in `\SS.OPENING` retransmitting forever, with no
   error anywhere. Answer as DATA, acknowledging their sendseq.
2. **The op word is `code bit 5; answer bit 1; length bit 10`**, BCPL bit
   numbering from the MSB, length in BYTES: `code = (w>>11)&037`,
   `answer = (w>>10)&1`, `len = w&01777`. Confirmed independently — the first
   request's `0040023` gives code 8, length 19, against a 19-byte body, and
   `LEAF!33` has `\OPCODE.SHIFT 13Q`.
3. **String encoding contradicts the header.** `IfsLeafOpen.bcpl` declares
   `String: [length byte; char^1,1 byte]`, but the wire carries a length
   **word** then that many bytes padded to a word, and the pad byte is not
   zeroed (the first string reads `000005 "Guestu"` — five chars and one of
   leftover). **Where the declaration and the wire disagree, the wire wins.**

Cross-checks that all matched: `\LEN.READANSWER 12Q` = 10 = our read header,
`\LEN.FILEREQUEST 12Q` = 10, `\LEN.RESETLEAF 4`, `\IFSERROR.FILE.NOT.FOUND
317Q`.

---

## 4. The init file, and why the directory spelling matters

`chm/lisp/ftp-root/INIT.DORADO` is `INIT.NONET` (the stock greeting file,
which is what the pack ships as `INIT.LCOM`) with `{FLOPPY}` replaced by
`{DORADO}<>`. Installed with
`make lisp-lispusers-pack LISP_INIT_OVERRIDE=../chm/lisp/ftp-root/INIT.DORADO`.

Two facts worth not rediscovering:

- **The pack's `INIT.LCOM` is SOURCE despite the name** (byte-identical to
  `INIT.NONET!1`). Interlisp LOADs it interpreted, so changing it needs no
  compiler.
- **`{DORADO}<>`, not `{DORADO}`.** With no directory component the client
  resolves bare names in the LOGIN USER's directory — the trace read
  `STP_LOOKUP_MISSING <GUEST>AISBLT.LCOM` — and `FILESLOAD` then reported
  "not found" without ever sending a Leaf Open. `<>` is the empty-directory
  form, the same spelling Lyric's own sysout retrieve uses.

**There is a login prompt, and it blocks silently.** With `{DORADO}` on the
path, `(IL:FILESLOAD X)` prints `{DORADO} Login:` and waits. Until credentials
are typed there is *zero* file traffic, which reads exactly like "the client
never tried". Any scripted run must answer it (`Guest` / `Guest`).

---

## 5. THE OPEN PROBLEM: HELPSYS

`(IL:IRM.LOOKUP 'CAR)` reports **"IRM.DINFOGRAPH not found on IRM.HOST&DIR"**
and — the diagnostic that matters — **never asks the server for it**. Zero
lookups for that name on the wire.

What is already proven, so do not re-test it:

- `IL:IRM.HOST&DIR` is **unbound** in the booted world.
- The name form is fine: `INFILEP '{DORADO}<>IRM.DINFOGRAPH` resolves over the
  wire (`STP_LOOKUP IRM.DINFOGRAPH -> !1 47688 bytes`), and `PACKFILENAME
  'NAME 'IRM 'EXTENSION 'DINFOGRAPH 'BODY {DORADO}<>` builds exactly that
  string.
- All the files HELPSYS needs are in the served root already: 32
  `CHAP*.TEDIT`, `IRMTOP.TEDIT`, `IRM.DINFOGRAPH` (47 KB), `IRM.HASHFILE`
  (331 KB).
- The init on the pack is intact — 3,181 bytes, byte-identical to source.
- That same file's ORIGINAL `RPAQQ`s DO take effect:
  `IL:LISPUSERSDIRECTORIES` reads `({DSK} {DORADO}<>)`.

So: **forms appended to this file-package format are not evaluated at greet
time**, regardless of position relative to `(DECLARE%: DONTCOPY (FILEMAP …))`.
This also means `\FTPAVAILABLE` never took either — `FILESLOAD` succeeded via
`LookupFile`, not the FTP enumeration I had credited it to.

**Next thing to try:** `USERGREETFILES`. The init already lists
`{DSK}INIT.USER`, which is the documented user hook and a plain form file with
no COMS or FILEMAP to fight. Put the three settings there instead:

```lisp
(RPAQQ IRM.HOST&DIR {DORADO}<>)
(RPAQQ \FTPAVAILABLE T)
(ADDTOVAR NETWORKOSTYPES (DORADO . IFS))
```

Setup is per `HELPSYS.tedit` (mirrored at `chm/lisp/lyric-docs/`): "copy all
these files to one directory, and set the variable IRM.HOST&DIR to the name of
this directory … This should be set in your site init file." The doc warns the
hash file "must be on a random access filing device" — Leaf **is** one, which
is the whole reason Interlisp uses it over FTP, so the 331 KB hash file can
stay on the server rather than cost pack pages we do not have.

Payoff when it works: the doc also says *"selecting DInfo from the Background
Menu will raise a menu which will contain an item named Interlisp-D Reference
Manual"* — that is the docs-button demo, native, no UI work needed.

---

## 6. The pack cannot be made bigger

Architectural, from `AemuSources.dm!82_/AltoDiabloDisk.mc`:

- the Diablo disk address has **one drive bit and one head bit**;
- cylinders are hard-coded — `ASeek: T_ (K400)+(226C); * 626B = 406 = number
  of cylinders`, past it is `ABadCylinder`;
- `Dorado cylinder = 406*(Diablo drive) + Diablo cylinder + 3`, so two drives
  reach 814 and **the T-80 has exactly 815 cylinders**.

2 drives × 2 heads × 406 cyl × 14 sectors = **22,736 pages**, and
`LISP.VIRTUALMEM.` takes **15,003** of them. For scale, PARC's own
`NewUserBigDisk.cm` gave VMEM *20000* and noted it "should leave about 1000
free pages" — they were tighter than we are.

The only lever is deleting Alto utilities a Lisp pack never uses:
`altofs --delete` (added this session, runs before the inserts) and
`LISP_PACK_DROP` in the Makefile. That frees 1,590 pages — Bravo alone is 458.
**`Fonts.Widths` and `Gacha10.al` must NOT be dropped**: `INIT.NONET` names
`{DSK}FONTS.WIDTHS`, and deleting it produced file-not-found errors.

Package selection is by dependency **closure**, not size —
`tools/lisp_pack_closure.py`, which reads the `(FILES A B C)` names straight
out of the compiled `.LCOM`s where they survive as ASCII. The first cut sorted
by size to maximise the package count, which is exactly the wrong objective: a
package whose dependencies did not fit is not a package, it is an error
window. HELPSYS needs DINFO; FILEBROWSER needs TABLEBROWSER.

---

## 7. Archive facts, and three search errors to not repeat

`docs/interlisp-archive-survey.md` and `docs/xerox-systems-archive-survey.md`
have the full per-release inventories. The load-bearing ones:

- **`.DCOM` is Harmony, `.LCOM` is Lyric.** `eris/lispusers` is a 1982-84 tree
  whose code Lyric rejects outright ("Bad compiled function" *after* printing
  the file's true creation date). The Lyric library is under
  **`phylum/lisp/lyric`** — the one release not filed under `eris/lisp/`.
- **Version choice is per-tree.** `FILEBROWSER.LCOM` is `!1` (Mar-1987) in the
  release tree and `!2` (Jun-1988) outside it, and the 1988 build wants a
  TABLEBROWSER the 1987 one does not. `tools/fetch_lispusers.py` takes the
  release tree outright.
- **"Cedar 7 was never built for Dorado" is wrong** —
  `pixel/ncp/top/BasicCedarDorado.boot!1` is a 771,584-byte Dorado Cedar world
  dated 10-Dec-1987. The 7.0 tree is still missing, so it cannot be assembled.

Three ways I wrongly concluded "not in the archive", each costing a cycle:
matching a **single file extension** across releases that changed it;
counting **file rows in a directory root** while the content sat one level
down (`phylum/lispusers/` lists zero files and two subdirs holding 871); and a
**case-sensitive** match.

---

## 8. Tools added this session

| tool | what it is for |
|---|---|
| `tools/pgm_text.py` | reads text off a framebuffer by glyph-matching. **A pixel count cannot gate this work** — a failed Lisp load is 210,163 px and a successful one 211,318, and the failing screen is sometimes larger. Teach new glyphs with `--learn`. |
| `tools/lisp_pack_closure.py` | picks packages by dependency closure |
| `tools/fetch_lispusers.py` | mirrors `.LCOM`s, release tree first |
| `tools/check_lisp_leaf_gate.sh` | the Leaf gate's assertions |
| `altofs --delete` | frees pack pages |
| `--click`/`--mouse` in SDL, `--drag` headless | scripted pointer |

---

## 9. Traps that cost real time

- **`FTP_UNSERVED` is traced BEFORE the handlers that answer some of those
  packets.** It flags served traffic. `0223` is AddressLookup and we do answer
  it; I spent a cycle "implementing" something already implemented.
- **The Pup length is `tx_words[2] - 026`, not `tx_words[1]`.** Reading the
  wrong word made a 6-byte name lookup look like a 512-byte transfer.
- **A background command that `cd`s wrongly silently skips `make`.** A trace
  edit was never compiled in, and I read "zero replies" off a binary that did
  not contain the tracing. `strings build/dorado | grep MY_TRACE` before
  trusting a run.
- **Unmapped keyboard characters used to vanish silently.** `&` was missing,
  so `IRM.HOST&DIR` typed as `IRM.HOSTDIR` and reported unbound — a keyboard
  gap reading as a broken init. Fixed, and unmapped chars now warn.
- **SDL and headless parse options separately.** A target written against one
  errors on the other.

---

## 10. Web build

`make web-lisp-src` produces `web/lisp-src.tar.gz` — **5.4 MB, the whole
served tree** (285 `.LCOM`s, 94 display fonts, the greeting file, the 35-file
IRM), fetched lazily only when a Lisp world is chosen, mirroring how
`cedar-src.tar.gz` works. A DENY-list, not an allow-list: the Cedar bundle
shipped with an allow-list of extensions and silently dropped both version
maps and every capitalised variant, because `find -name` is case-sensitive.

**Not yet wired up.** `web_shell.html` does not fetch it, and the Lisp world's
`--ftp-root` is not pointed at the unpacked MEMFS tree. That is the remaining
work to get netboot into the browser build.
