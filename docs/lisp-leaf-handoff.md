# Handoff — Interlisp-D on the network (Leaf), 2026-08-01..04

**Read §1 first.** Everything below is merged to `main` and the GitHub
Pages deploy is live (the old push hold was lifted 2026-08-03).

---

## 1. State in one paragraph

Interlisp-D Lyric loads files it does not have on its disk over **Leaf**,
the IFS random-access file protocol, and **the docs demo works end to
end, native and in the browser**: `(IL:FILESLOAD HELPSYS)` then
`(IL:LOAD '{DORADO}<IRM>IRMDEMO)` (login `Guest`/`Guest`) opens the 1987
Interlisp-D Reference Manual to the CAR page — hash-index lookup, chapter
streamed over Leaf, horizontal rules drawn
(`docs/images/lisp-irm-car-dinfo-2026-08-03.png`). Getting there took,
in order: a colon in a comment that killed the init at greet (§5), the
IRM moving to a served `<IRM>` directory (§5), and the real server-side
blocker — the unserved **IFS leader page**, which the client reads at
every open (§5, §3). The pack carries 204 packages + 14 fonts; Leaf
lifts its hard 22,736-page ceiling. The web build ships the library
world with all three assets fetched lazily (§10). Open items, all
minor: a late `ARG NOT PROCESS` break minutes AFTER success (client
cache-flush idle path, §5), the mouse region-confirm click (input gap,
§5), and optionally baking a demo snapshot with HELPSYS preloaded.

---

## 2. What works, with the commands

```sh
cd dorado

# The Lyric desktop with 204 Interlisp packages + 14 display fonts on disk
make run-lisp-lispusers-sdl      # click the Prompt Window, then type
make run-lisp-sketch-sdl         # auto-loads SKETCH (~3 min of wall clock)
make run-lisp-filebrowser-sdl    # auto-loads FILEBROWSER (+TABLEBROWSER)

# The reference-manual demo, inside the running desktop (click the Exec):
#   (IL:FILESLOAD HELPSYS)
#   (IL:LOAD '{DORADO}<IRM>IRMDEMO)     -- {DORADO} Login: Guest / Guest
#   (IL:IRM.LOOKUP 'ANYTHING)           -- further lookups

# Gates
make verify-lisp                 # desktop restores, 208985 px
make verify-lisp-leaf            # loads a package NOT on disk, over the network
make verify-lisp-lispusers       # loads one that IS on disk
make test                        # 12 suites, 204 assertions
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

Implemented: **Reset, Params, Open, Read, Close, ErrorAnswer**, a
**16-slot handle table** (HELPSYS holds the hash file open while fonts,
the graph and chapters come and go — one slot answers BAD HANDLE to the
second lookup), **honest Sequin behavior** (a duplicate data request is
answered by RETRANSMITTING the cached answer with its ORIGINAL sequence
number — LEAF!33 matches answers to requests strictly by order against
its done queue, and a fresh answer desynchronizes it; bodyless
NOOP/RESTART/DESTROY/QUIT controls are recognized and answered), and the
**IFS leader page** (§5 — dates, BCPL name with `!version`, author,
type, served at 27-bit addresses ≥ 2^27−2048, plus dontExtend EOF
clamping and the answer's `newEOF` bit). Not implemented: Write, Delete,
Truncate, CloseTransaction, Telnet (the served tree is read-only).

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

## 5. HELPSYS: the init mystery was one colon (RESOLVED 2026-08-03)

`IRM.HOST&DIR` was unbound **because of the comment that introduced it**.
Interlisp `(* ...)` comments are list data the reader parses, and the Lyric
reader (the Xerox Common Lisp era) parses the token `HELPSYS:` as a package
prefix: *"Can't find package HELPSYS to look up symbol"* — and the load of
the file dies right there, so every form after the comment was never read,
in either position relative to the `(DECLARE%: ...)`. At greet time the
error is swallowed, which is why it looked like a file-format property.
A manual `(IL:LOAD '{DSK}INIT.LCOM)` at the Exec printed the reader error
on screen and cracked it in one run. The fix is one character
(`HELPSYS --`), then `make lisp-lispusers-snapshot
LISP_INIT_OVERRIDE=../chm/lisp/ftp-root/INIT.DORADO` to rebake. Both gates
pass against the rebaked snapshot, and the previous section-5 diagnosis is
corrected as follows:

- **The `INIT.USER` plan would have backfired.** `USERGREETFILES` entries
  are alternatives for ONE user greet file, tried in order, first found
  wins (Medley's SIMPLE-INIT pairs `(dir "INIT" COM)/(dir "INIT")` =
  compiled-else-source). `{DSK}INIT.USER` would have SHADOWED `{DSK}INIT`,
  silently dropping the whole main init.
- **"Never asks the server" had a second cause stacked on the first**:
  HELPSYS's own `(RPAQ? IRM.HOST&DIR)` initializes the unbound variable to
  NIL at load, after which `INFILEP` of the packed (host-less) name
  searches only `{DSK}`; and a bare `INFILEP` on a `{DORADO}` name raises
  the silent `{DORADO} Login:` prompt (probe2 caught it mid-echo).
- **The archive agrees this setup was always site-specific**: the only init
  in the whole archive that sets `IRM.HOST&DIR` is `INIT.MAIKO` (1988,
  Maiko), inside its `INITCOMS`; PARC's own site inits
  (`chm/lisp/inits/INIT.{CSL,CIS,ISL,LISPCORE,KSALISPCORE,NEW-CSL}`) never
  wired the IRM in. Medley's `docs/dinfo/README.md` documents the same
  three steps we implement, and HELPSYS itself registers
  `("Interlisp-D Reference Manual" (IRM.GET.DINFOGRAPH T))` in
  `DINFO.GRAPHS` — that is the Background-menu docs button.

**Proven working now, in one scripted run** (`build/helpsys-live2.log`):
`(IL:FILESLOAD HELPSYS)` loads HELPSYS+DINFO+TEDIT off `{DSK}`, fetches its
display fonts over Leaf, `(IL:IRM.LOOKUP (QUOTE CAR))` passes the
`{DORADO} Login:` gate (Guest/Guest), resolves
`STP_LOOKUP IRM.DINFOGRAPH -> !1 47688 bytes`, and raises "Specify region
for IRM DInfo Window" with the 540x400 ghost box **tracking the cursor**
(bottom-left anchored — Interlisp Y grows upward).

**What is still open, precisely: the region-confirm CLICK.** The box tracks
(mouse POSITION flows through raw `MOUSEX.EM` 0424/5), but the confirming
button never lands. `\TRACKWITHBOX` exits on a LEFT|MIDDLE change in
`LASTMOUSEBUTTONS`, and `GETMOUSESTATE` takes that NOT from UTILIN but from
`\LASTKEYSTATE` — the last KEYBOARDEVENT `\KEYHANDLER` pushed. Typed keys
produce those events; a UTILIN-only button change apparently never wakes
the handler. The bit encoding is verified correct (LLKEY `\MOUSE.LEFTBIT 4
/RIGHTBIT 2/MIDDLEBIT 1`, active low — exactly what `machine_seed_utilin`
writes), so the gap is in event/wake delivery. The same gap is what left
Sketch's `GETREGION` sweep dead, and it gates SDL too — real clicks flow
through the same `dorado_machine_set_mouse`. Beware the decoy that cost
two runs here: the region prompt lags the login by ~6 B cycles (14
font-name LookupFile misses, each burning a guest timeout), so clicks at
18.5 B and 22.0 B both fired BEFORE the prompt existed; on this timeline
it appears between 22.0 and 23.0 B.

**The demo does not have to wait for the click.** `GETBOXREGION` runs only
because `IRM.GET.DINFOGRAPH` was given no window; giving `DINFO` an
explicit `(CREATEW (CREATEREGION ...))` skips it. And do NOT type the long
form -- one of the trailing `))` reliably drops on the wire and the XCL
reader silently swallows everything after into the open form (one whole
run died this way). The forms live in a served file instead, so the demo
is ONE short line:

```
(IL:LOAD (QUOTE {DORADO}<IRM>IRMDEMO))
```

**The IRM must be served from a NAMED directory, not the root.** DInfo
rebuilds each node's file name by copying host AND directory from
`IRM.HOST&DIR`, and an empty `<>` directory is DROPPED in that copy -- the
node files then resolve in the login user's directory
(`STP_LOOKUP_MISSING <GUEST>CHAPLISTS.TEDIT`). The 35 IRM files now live
in `chm/lisp/ftp-root/IRM/` and `IRM.HOST&DIR` is `{DORADO}<IRM>` (the
server maps `<A>B` to `A/B`, same as the Cedar tree). This is also the
authentic shape -- `HELPSYS.tedit` says "copy all these files to one
directory". Note `CHAPLISTS.TEDIT` IS in the mirror -- an `ls | head`
truncation made it look missing for half a session.

With that, the scripted chain (`build/irmdemo2.log`) runs: IRMDEMO fetched
over Leaf, graph (47,688 bytes) streamed, DInfo window opens with no
region prompt, `IRM.LOOKUP` probes the 331 KB hash file by random access,
finds CAR, streams `<IRM>CHAPLISTS.TEDIT!1` (74,141 bytes) plus the
document's own Modern display fonts, and **DISPLAYS THE CAR MANUAL PAGE
in the DInfo window** -- "(CAR X) [Function] Returns the first element of
the list X ...", scrolled to the node the hash index named. Screenshots:
`docs/images/lisp-irm-car-dinfo-2026-08-03.png` (the lookup) and
`lisp-irm-chapter-tedit-2026-08-03.png` (the whole chapter open in plain
TEdit via `{DORADO}<IRM>TEDDEMO`).

**What actually blocked it was the IFS LEADER PAGE** -- after two red
herrings whose fixes were still worth keeping (a handle TABLE, since
HELPSYS holds the hash file open while fonts and chapters come and go;
and honest Sequin behavior: cached-answer retransmit for duplicate
requests, NOOP/RESTART/DESTROY/QUIT control handling). The real bug,
from `IfsLeafRead.bcpl SetModeLength`: a 27-bit read address at or above
`2^27-2048` (`maxAddress = [#3777; #174000]` -- the "weird"
`addr=134215680` that had been sitting in every trace) is a NEGATIVE
offset into the file's 2 KB leader page, and LEAF!33 reads it at EVERY
OPEN: 3 Alto dates at leader offset 0, the full file name (BCPL string)
at 512, author at 636, filetype at 680. Our server clamped those
addresses against the file length and answered ZERO bytes, so
`GetBcplString`/`ALTO.TO.LISP.DATE` parsed uninitialized pup buffer:
garbage names in every stream record, `PACK*` exploding on a garbage
length byte ("IR SYMBOL-NAME-TOO-LONG"), and the Leaf watcher process
eventually dying on the poisoned records (`ARG NOT PROCESS
#<UNBOXEDHUNK2 @ 74,0>` -- constant address because the garbage was
deterministic). `leaf_build_leader` in `src/ethernet.c` now synthesizes
the page (mtime as all three dates, the opened name with a `!version` so
`\LEAF.READFILENAME` finds its bang, author "Guest", type unknown), and
ordinary reads answer with SetModeLength's dontExtend semantics: address
clamped to EOF and the answer's `newEOF` bit reporting address+length =
EOF, which is how the client's page machinery finds the end of file.

Still open, both minor: a LATE `ARG NOT PROCESS` break, now ~4 emulated
minutes AFTER the page renders (the client's `\LEAF.FLUSH.CACHE` idle
path around a soft-closed font -- the deferred wire Close never gets
sent; it no longer harms the demo but wedges later network use, and a
`BT` typed into that break in a TEDDEMO-style run is the next probe);
and the "Unknown IMAGEOBJ" boxes where the chapter has horizontal rules
-- `(FILESLOAD HRULE)` is now the demo file's first form (HRULE.LCOM is
on the pack) which should let TEdit render them.

Setup remains per `HELPSYS.tedit` (mirrored at `chm/lisp/lyric-docs/`,
now alongside the Lyric-vintage `dinfo.tedit` user doc): the 331 KB hash
file stays on the server because Leaf is exactly the "random access filing
device" the doc demands.

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

- **Interlisp `(* ...)` comments are parsed, and a colon is live syntax.**
  `(* HELPSYS: the online ...)` read as a package-qualified symbol and
  silently killed every later form in the init at greet time (§5). No bare
  `word:` tokens outside strings in anything Interlisp will LOAD; Interlisp
  itself writes `%:` (see `DECLARE%:`). When later forms in a file "don't
  take", do a manual LOAD at the Exec — it prints the reader error that
  greet swallows.
- **A snapshot taken with an open Leaf connection restores dead.** The
  server's Sequin/handle state is file-scope static, deliberately outside
  the snapshot ABI; after restore the guest retries its in-flight reads
  against a server that has never heard of them, then declares
  `[DORADO not responding ...]` and unwinds. Mid-chain checkpoints cannot
  be used to iterate on anything past the first network file op — bake
  desktops BEFORE any Leaf traffic, and script full live chains for the
  rest.
- **Clicks race prompts.** `--click` at a fixed cycle silently does nothing
  if the guest's prompt/tracker is not up yet, and the thing you are
  clicking for may lag its trigger by billions of cycles (font-miss
  timeouts, §5). Bracket the appearance with `--shot-every` before
  scheduling the click.
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

## 10. Web build (WIRED UP 2026-08-03)

The browser's Lyric world is now the LIBRARY world: `make
lisp-lispusers-web-snapshot` bakes the wasm-ABI checkpoint from the
lispusers pack under the node-hosted binary (same boot flags as the native
bake; paired with the native run's flushed pack), and the world's three
assets are all fetched LAZILY when the dropdown entry is chosen --
`lisp-lispusers.snap.gz` + `lisp-lispusers.pack.gz` (~7.3 MB, was
preloaded in index.data, which shrank ~6 MB for every visitor) and
`web/lisp-src.tar.gz` (5.0 MB, the whole served tree: 285 `.LCOM`s, the
display fonts, INIT, and the 38-file `IRM/` -- `COPYFILE_DISABLE=1` on
the tar or macOS bsdtar pollutes it with `._` AppleDouble twins).
`web_shell.html` untars it into MEMFS at `/lisp-stp` in the background
and `dorado_web_boot_lisp` points `ftp_root` there.

**The trap that cost the bring-up hour: `dorado_machine_restore` clobbers
the ethernet state with the BAKE-TIME ftp root** -- a native path that
does not exist in MEMFS, so every lookup missed (login still worked:
NetDir and Leaf Reset/Params never touch the root, which makes the
failure read as "file not found", three packets after the actual cause).
Every Cedar web boot already re-applied `dorado_machine_set_ftp_source`
after restore; the Lisp boot now does the same. Verified with the native
binary serving the UNPACKED tarball tree (`STP_LOOKUP IRM/IRMDEMO -> !1`)
and in Chrome against the local build. Note for browser testing: a
HIDDEN window suspends requestAnimationFrame and the machine crawls --
keep the tab visible while driving it.
