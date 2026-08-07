# TODO from the PARC veterans' call (2026-08-06)

Raw feedback from a video call with ex-Xerox PARC engineers who built and
used these systems, turned into a work list. Their observations are the
authority here: they are describing behaviour they know first-hand.

Ordered by **what blocks a visitor from using the emulator**, not by how
interesting it is. The input cluster (A) is first because four separate
items in the call were all "I could not drive the machine".

Legend: **[verified]** checked against code/docs this session ·
**[reported]** stated on the call, not yet reproduced here ·
**[open question]** needs an answer before it can be scoped.

---

## A. Input — keyboard and mouse (HIGHEST PRIORITY)

Four independent complaints, and this is the class of bug that makes
everything else look broken. Note the standing lesson in
`gates-must-drive-input-like-a-human`: three input bugs shipped green
because the gates held keys for millions of cycles and never moved the
mouse. **Every fix here needs a gate that drives input the way a person
does, and each gate must be validated against its own bug** (i.e. shown to
fail before the fix).

### A1. The Control key does not work [reported]

- **Symptom:** Control appears dead.
- **Scope:** unknown whether it is all worlds or one. Establish that first
  — Cedar, Lisp and Smalltalk take different delivery paths
  (`machine_cedar_io` vs the Alto path), so "Control is broken" could be
  one bug or three.
- **Start:** `src/typetext.c` owns the one canonical ASCII->Alto key map
  (`dorado_char_to_key`). Check whether Control is represented in the key
  map at all, versus represented but never delivered as a *modifier* held
  across another keystroke — those are different defects with the same
  symptom.
- **Acceptance:** a headless gate that sends Control+<key> and asserts the
  guest saw a control character, per world.

**EVIDENCE (2026-08-06): a modifier-timing defect is observable today, and
it may be the same bug.** Driving `(IL:FILESLOAD HELPSYS)` into the Lyric
Exec headlessly at `--key-hold 200000` produced, on screen:

```
1> (il:FILesLOaDHELPSYS)
Undefined car of form
IL:FILESLOADHELPSYS
```

Two distinct corruptions in one line: **the case is inconsistent**
(`il:FILesLOaD` — some letters shifted, some not) and **the space was
dropped**, so the reader saw a single symbol. Case comes from SHIFT being
held around a letter, so inconsistent case IS a modifier-timing failure —
the same shape as "Control does nothing". **Check whether A1 and this are
one defect before fixing them separately.**

Caveat, and it matters: 200000 cycles is ~3.2 ms of guest time, far shorter
than the 4,000,000 the Lisp recipes use, so some of this may simply be
"driven faster than the hardware samples" rather than a modelling error.
That is itself the question — a real keyboard's shift contact spans the
whole keystroke, so if our modifier is applied as a *separate timed event*
rather than a *held level*, fast typing will race it and a human will
occasionally see it too. Compare against Table 24's ordering guarantee
(A4): the terminal microcomputer reports keyboard transitions in
preference to mouse motion precisely so key state is never lost.

### A2. Middle-click on Cedar inserts instead of selects [reported]

- **Symptom:** middle button performs the insert action rather than a
  selection.
- This is a strong signal of a **button-number mapping error**, not a
  timing problem: Cedar's mouse is chorded (red/yellow/blue = left/middle/
  right), and inserting is what a *different* button means. Suspect the
  host middle button is landing on the wrong bit.
- **Start:** find where host buttons map to the Dorado's three bits, and
  check against `TerminalDefs.mesa` / the Cedar mouse conventions rather
  than against intuition. Confirm the bit order the hardware presents
  (active low, per UTILIN — see the Smalltalk keyset bug, where an
  unseeded active-low cell read as "every key held").
- **Acceptance:** a gate that middle-clicks in a Cedar viewer and asserts a
  selection, not an insertion.

### A3. Lisp: cannot pick a submenu item from the Exec menu [reported]

- **Symptom:** the Exec menu opens but a sub-menu item (e.g. choosing an
  Interlisp shell) cannot be selected.
- **Related known fact:** `lisp-region-sweep-needs-a-long-hold` — Interlisp
  menu/region interactions needed a **36.8 M-cycle** button hold where 16.8
  M hung. Interlisp sub-menus may need a hold-and-drag longer than anything
  we currently drive, so **check the hold duration before concluding the
  menu code is broken**. That memo also records that an earlier "the
  confirming button never lands" conclusion was wrong.
- **Open question:** is this the same hold-duration issue, a drag-across-
  submenu-boundary issue, or a genuine missing event? Reproduce
  interactively first.

**INVESTIGATED 2026-08-06 — menus work; they are on the RIGHT button.**

The first finding is about our TEST TOOLING, not the emulator: `--menu` had
`DORADO_MOUSE_MIDDLE` hardcoded, and **middle/yellow raises nothing in an
Interlisp window**. With the **right/blue** button the window menu appears
immediately and correctly:

```
Close  Snap  Paint  Clear  Bury  Redisplay  Hardcopy>  Move  Shape  Shrink
```

`Hardcopy>` carries the submenu arrow — that is the submenu in question.

**The input plumbing is correct**, established rather than assumed:

- `DORADO_MOUSE_TRACE` shows the press writing `utilin<-177776` (bit 0 clear
  = Yellow) and the release restoring `177777`, at `0177030..0177033`.
  (The `readback=` in that trace lags one line — it prints BEFORE the store.
  Not a bug; do not chase it.)
- The cursor tracks position, so the guest is reading MOUSELOC (`0424/0425`).
- The button enum already matches the Alto bit order —
  `LEFT 0x4`=Red=bit2, `RIGHT 0x2`=Blue=bit1, `MIDDLE 0x1`=Yellow=bit0 — so
  the Alto path's `~(buttons & 07)` is equivalent to the Cedar path's
  explicit per-button mapping. **Neither is wrong**, which also weakens the
  "button mapping error" guess for A2.
- Dragging with the button held keeps the menu up and moves the cursor
  within it.

**Tooling added:** `--menu-button left|middle|right` (also
`red|yellow|blue`), applying to `--menu` and `--drag`. It defaults to 0
meaning "each option's own default" (`--menu` middle, `--click`/`--drag`
left) so the existing GETREGION sweep recipes, which depend on `--drag`
being left, are unchanged.

**The submenu is NOT yet reproduced, and the blocker is our tooling.**
`--drag` presses and then starts moving 2 M cycles later, but the menu
takes longer than that to appear, so the menu is created *after* the
travel is over. Measured across three runs — the menu's top-left always
lands at the cursor's FINAL position, never the press position:

| test | press | dragged to | menu appears at |
|---|---|---|---|
| `--menu` (no drag) | 250,300 | — | x=252, y=295 |
| `--drag` | 250,300 | 300,361 | x=305, y=300 |
| `--drag` | 250,300 | 340,371 | x=345, y=365 |

So the pointer can never travel *within* an open menu: it arrives, then the
menu materialises under it with the cursor on the first item. A human does
the opposite — press, see the menu, then move — so this artifact does not
explain the user's report; it just means the current gate cannot test it.

**What is needed:** a press -> DWELL -> move -> release primitive (e.g.
`--drag-dwell CYCLES` inserted between the press and the travel), so the
menu is up before the pointer moves. That is the next concrete step on A3.

### A3 RESOLVED (2026-08-06): the menu works; the reporter's next step is a
### region sweep

The reporter clarified: press **outside** every window for a different menu,
and the Exec item there opens an Interlisp window. That is the **background
menu**, not the window menu tested above.

**Which button:** the **RIGHT/blue** one, and only that one. Left and middle
on the background produce byte-identical framebuffers to the idle screen —
i.e. nothing at all. Contents:

```
AR Edit>   FileBrowser>   CHAT   Idle   SaveVM>   Snap
Hardcopy>  EXEC>   PSW   TEdit   SendMail
```

**Selecting EXEC works.** Press right on the background, dwell so the menu
appears, travel onto `EXEC>` (~y=458), release — and the Prompt Window says:

```
Specify region for window "Exec"
```

So the menu, the submenu-bearing item, and the selection all function. **The
next step is `GETREGION`** — sweep the new window's rectangle — which is
exactly what memory `lisp-region-sweep-needs-a-long-hold` records as needing
a very long hold (36.8 M cycles worked where 16.8 M hung). That is the
likely place a user gets stuck, and it looks like "the menu did nothing"
because the prompt is easy to miss.

**Tooling this required, both now in `dorado`:**

- `--menu-button left|middle|right` — parsed sequentially, so one run can
  open a menu with the right button and then sweep with the left.
- **`--drag-dwell CYCLES`** — wait, button already down, before the pointer
  travels. Without it the travel finishes before the menu exists and the
  pointer can never move WITHIN a menu, which made every earlier submenu
  test meaningless.

### A3b. Caps Lock must work [reported — small, and the mapping is already right]

Wanted especially for Lisp. **The matrix entry already exists and is
correct**: `src/display.c:127` has `[DORADO_KEY_LOCK] = { 3, 0x0080 }`, i.e.
word 3, mask `0x0080` = C bit 7 = **Xerox bit 8 of `KBDAD+3`**, which is
exactly what the Alto Hardware Manual gives for `LOCK` (below).

**The gap is the frontends**: neither `src/dorado_sdl.c` nor
`src/web_shell.html` maps a host key to `DORADO_KEY_LOCK` — grep for
`capslock`/`SDLK_CAPS` finds nothing. So this is a frontend wiring job, not
a matrix fix.

Watch: host Caps Lock is a *toggle* with its own OS-level state, and the
Alto's LOCK is just another key in an active-low matrix. Decide deliberately
whether to mirror the host toggle state or treat press/release literally —
SDL reports both a keydown and a keyup for the toggle on most platforms, and
getting this wrong leaves LOCK stuck on.

### A4. Audit every keyboard mapping [reported — NOW SPECIFIED]

**The authority is `DoradoDocs/manuals/Alto_Hardware_Manual_Aug76.pdf`,
document page 27 = PDF page 34** (pointer from Tim). It gives the full
bit-by-bit layout for both Alto I and Alto II keyboards:

- The keyboard is **four 16-bit words at `KBDAD` = `177034B`** (so
  `177034B`-`177037B`).
- **Depressed keys read 0, idle keys read 1** — active low. (Same trap that
  produced the Smalltalk "keyset is stuck" bug when UTILIN was left
  unseeded.)
- Figure 6 tabulates bit -> keytop for each of the four words, for both
  keyboard generations. Alto I and Alto II differ only in the last few bits
  (Alto II adds FR1-FR5/FL1-FL4 function keys where Alto I has blanks).

**Which keyboard did the Dorado use?** Tim's guess is the Alto II version.
Worth confirming, but the two layouts are identical except in the function
keys, so the audit can proceed either way.

**How the Dorado gets there** —
`DoradoDocs/manuals/Dorado_Hardware_Manual_Sep1981.pdf`, document page 117 =
**PDF page 124** (the PDF is offset +7), **Table 24: Terminal Microcomputer
Messages**. This IS the "processor in the keyboard" of A6:

| message | meaning |
|---|---|
| `01B`-`04B` | Keyboard words 0-3, "corresponds to Alto memory location" **`177034B`-`177037B`** |
| `05B` | **Mouse buttons and keyset (Alto `177033B`)** |
| `06B` | 8-bit changes in X (0:7) and Y (8:15), **excess-200B** notation |
| `10B`/`11B` | Keyboard words 4-5, **Star keyboards only, no Alto analogue** |
| `17B` | Boot message (the boot button jams data to one continuously; up to 8 bits of garbage follow, and pushes under 10 ms must be ignored as contact bounce) |
| `00B`, `07B`, `12B`-`16B` | Illegal, ignored |

So Tim is right that the Alto mapping applies: the terminal microcomputer
serialises keyboard AND mouse into one message stream whose keyboard
messages land at the Alto locations. Note also the manual's remark just
above the table -- if a mouse-position change would be reported but a
keyboard transition is pending, "one keyboard word is reported instead of
the mouse position change; thus, the correct state of the keyboard is
eventually reported even if transitions are missed." That is a real
ordering guarantee worth honouring if A1-A3 turn out to be timing bugs.

**One discrepancy to resolve:** `177033B` is the **mouse/keyset** word per
Table 24, but the top-level `CLAUDE.md` says "Cedar's keyboard is delivered
to KeyBits at absolute `LONG[177033B]`". Those reconcile only if Cedar's
KeyBits structure starts at UTILIN and spans the four keyboard words. Check
it while doing the audit. Our `machine.c` seeds `base + 0177034..7`, which
matches the table.

### A4b. Was the Dorado keyboard the Alto II one? [open question]

- Not a bug report — a request for a systematic pass. Do it against the
  hardware key matrix and each world's expectations, and write the result
  down (A5).
- **Watch:** the map is per-world in effect. Cedar delivers to KeyBits at
  `LONG[177033B]`; the Alto path applies keys directly; Smalltalk needs
  UTILIN seeded. A mapping "correct" in one world can be wrong in another.

### A5. Keyboard mapping documentation — text AND a graphic [reported]

- Deliverable: a document plus an **image** showing the mapping
  graphically (host key -> Dorado key, per world where they differ).
- This is the artifact a new user needs most; it also makes A4's audit
  reviewable by the people on the call.

### A6. Model the keyboard's serializing processor [reported, historical]

- **Fact from the call:** the original keyboard contained a processor that
  **serialized data from both the keyboard and the mouse**.
- This is an architectural note, not a bug: it means keyboard and mouse
  events shared one serial path, which constrains their relative timing and
  ordering. Worth understanding before A1-A3 are called fixed, because it
  may explain event-ordering assumptions in the guests.
- **FOUND — it is the "terminal microcomputer", and its protocol is
  Table 24** (Dorado HW Manual doc p.117 = PDF p.124, tabulated under A4
  above). The 7-Wire Video Interface section that introduces it begins at
  document page 116 = PDF page 123. So the serialisation Tim described is
  fully specified: one message stream carrying keyboard words, mouse
  buttons/keyset, and X/Y deltas, with keyboard transitions given priority
  over position changes so key state is never lost.

---

## B. Cedar checkpoint / rollback crash

### B1. "Checkpoint" / "rollback" crashes [reported]

- **Symptom:** Cedar reports `Checkpoint` then `rollback`; the checkpoint
  is crashing.
- **Important distinction:** this is **Pilot's own checkpoint/rollback**
  (the guest writing its world back to the volume so it can restart from
  it), NOT our `--snapshot-out` emulator checkpoints. Do not conflate them
  in diagnosis — they share a word and nothing else.
- **Start:** it is a disk-write path, so instrument what Pilot writes at
  checkpoint time and where. Our write path goes through the PDI bridge in
  `machine.c`, which is a **shim over an incomplete controller** (gaps
  F1-F5, see `dorado/CLAUDE.md`), so a write pattern Pilot uses and
  Bringover does not is a plausible failure site.
- **Known-adjacent:** `rusty-backup` injection crashes Cedar's live FS;
  only Cedar's own install path works. A checkpoint is Cedar's own path, so
  this should be legitimate — which makes it worth fixing rather than
  avoiding.

### B2. "Do we have disk writing working?" [verified — YES, with limits]

Answer for the call: **yes, real writes work and persist.**

- A live Cedar `Bringover` plus `DORADO_PDI_SAVE` mutates and persists the
  PDI; that is how the demo and corpus checkpoints are baked.
- **Iago** creates real volumes through the emulated disk: `Create Physical
  Volume` + `Create Logical Volume` on a blank disk, a real `PhysicalRoot`
  at page 0 and `LogicalRoot` at page 3.
- `make cedar-diskvolume-software` puts 158 files / 19.94 MB onto a volume
  built from 60 MB of zeros.

**The limit worth stating:** writes go through the PDI *shim*, not a
cycle-accurate sequence-PROM/FIFO/ECC controller. So "writing works"
functionally; B1 may well be a case the shim does not cover.

---

## C. Browser persistence

### C1. Save to disk in WebAssembly [reported — new feature]

- **Need:** changes made in the browser are lost on reload; there is no
  persistence.
- **Mechanism:** the user suggested localStorage. **Prefer IndexedDB** —
  localStorage is typically capped around 5-10 MB and is synchronous, while
  our packs are ~60 MB (Lyric) and Cedar volumes larger still. Emscripten's
  IDBFS is the idiomatic route and already understands MEMFS mounts.
- **Design constraints already known:**
  - Snapshots and packs are fetched lazily and unpacked into MEMFS; the
    Cedar release arrives as a separate `cedar-src.tar.gz`.
  - `dorado_machine_restore` **clobbers ethernet state with the bake-time
    ftp root**, so every web boot must re-apply
    `dorado_machine_set_ftp_source` after restore. Any persistence layer
    has to survive that same hazard.
  - Do not add a `dorado_machine` member to carry persistence state — it
    changes the snapshot ABI and kills every baked checkpoint.
- **Scope decision needed:** persist the whole pack (simple, huge) versus
  persist only written sectors (smaller, needs a dirty-block layer).

---

## D. Colour display — and a documentation correction

### D1. CORRECT THE BOARD IDENTIFICATION [verified — our doc is wrong]

**From the call: `DispM` = colour board, `DispY` = monochrome board.**
Checked, and they are right:

| board | evidence |
|---|---|
| **DispY** — "Display Y", K. Pier, 22-Oct-81, 31 sheets | contents include FIFO, Channel A/B data paths, Cursor, **Alto Display Drivers**, **MiniMixer**, DWT Task WakeUp Logic |
| **DispM** — "Display M", K. Pier, Nov 1982, 32 sheets | sheets 12-31 are the **Mixer**: BMap, CMap, mixer Blue/Red/Green bytes, **DACs Red Green Blue**, PLL |

Both carry a **"DDC to DDM Interface Table"**, i.e. these are two boards
talking to each other: **DDC = the display controller (DispY)** and
**DDM = the display mixer (DispM)**.

**`docs/color-graphics-todo.md` §1.1 currently says the opposite** — "There
is no separate colour board. The same DDC drives mono and colour... this is
not 'emulate new hardware', it is 'finish the display controller we already
have'." That claim is the premise the whole scoping rests on, so it must be
re-examined: if colour needs the DDM as well as the DDC, the job is larger
than that document estimates. **Fix the doc before anyone scopes from it.**

Also: `DoradoDocs/schematics/DispY.pdf` is **mislabelled** — its pages are
`DskEth-Rev-Ce.ps` (disk/ethernet signal cross-reference). Use
`DoradoDocs/doradodrawings/DispY-*.pdf`, consistent with that directory's
own note that its PDFs are the better copies.

### D2. Conrac RGB monitor [reported]

- The colour monitor was a **Conrac RGB**. Feeds directly into the existing
  open question of how a guest detects a colour monitor: sheet 28 shows
  `MType.0/1/2` as a **monitor type field set by jumper resistors on the
  backplane**, read back via the STATUS inputs at TIOA 360/361.
- **Task:** find the MType value that says "Conrac RGB attached" and answer
  the STATUS reads with it, behind a config knob.
- Search the archive for Conrac to pin the exact model and its timing
  (resolution, pixel clock), which also bears on the unverified geometry
  risk in the colour doc.

### D3. The rest of the colour work

Already scoped in `docs/color-graphics-todo.md` — RAM loads, item unpacking
at 1/2/4/8 bpp, the mixer, an RGB framebuffer, frontends, a colour gate.
**Re-read it after D1 is fixed**, since D1 changes its premise.

Its step 0 stands and is still the right first move: **trace whether any
world actually programs the colour RAMs** (TIOA 361/362/365) before
promising anything.

---

## E. Archive: the "garage" directories

### E1. Find the hardware team's material [verified — it exists, not yet fetched]

**From the call: directories named "garage" are the hardware team's.**
Confirmed present in `chm/cross-reference.html`, and **we hold none of it
locally**:

| path | what |
|---|---|
| `_cd8_/doradosource/garagemidasmanual.dm` (also `indigo/doradosource/`) | **GarageMidasManual** — Midas is the Dorado's hardware debugger; a manual from the hardware team is directly relevant to the microengine and to diagnostics |
| `cyan/d0logic/garaged0drawings.dm` | **GarageD0Drawings** — D0 drawings (the Dorado's sibling machine) |

Contents visible in the index include `GarageMidasManual.Press`,
`GarageMidasListings.Press`, `MakeGarageMidasManual.cm`, and
`garagemidas1..4.bravo` (the Bravo source of the manual).

- **Task:** fetch both `.dm` archives per the `docs/chm-urls.md` convention
  and survey them; add anything Dorado-relevant to `docs/chm-archive.md`.
- **Why it matters:** we already run PARC's real hardware diagnostics
  (`build/rundiag`, all six pass). A hardware-team Midas manual is the
  natural companion to that work, and may also document the keyboard/mouse
  interface needed for A6.

---

## F. Sil design-automation tools — netlists from the schematics (PHASE 2 LEVER)

**The idea:** we hold the Dorado board drawings as Sil files. PARC also
wrote the tools that turn Sil drawings into netlists and wire lists. If
those run (or are re-implemented), the schematics become machine-readable —
a checkable oracle for the C emulator, and plausibly a generator path toward
the Verilog of Phase 2.

**It all survives, in `_cd6_/sil/`** — found 2026-08-06, none of it fetched
yet:

| file | what |
|---|---|
| `ANALYZE.RUN!1` (38,554 b, 10-Jul-1979) | **the netlist extractor** |
| `ANALYZESOURCES.DM!1` (108,349 b) | **its full BCPL source** (`anb.bcpl`, `anc.bcpl`, ...) |
| **`ECLDICT.ANALYZE!1`** | **the ECL component dictionary — the Dorado is an ECL machine** |
| `TTLDICT.ANALYZE!1` | the TTL dictionary |
| `BUILD.RUN!1` + `BUILD.DM!1` + `BUILD.SYMS!1` | wire-list / build tool, with source |
| `GOBBLE.RUN!1` + `GobSources.dm!1`, `VIEWGOBBLE.RUN!2` | with source |
| `SilSources.dm!1` | **Sil itself, in BCPL** |
| `SILMANUAL.DM!1` / `.PRESS!1`, `SILSUMMARY`, `SILUPDATES` | the Sil manual and updates |
| **`DORADODESIGNAUTOMATION.CM!1`** | **the Dorado's OWN design-automation procedure** |
| **`DORADOBUILDBACKUPTEMPLATE.CM!1`** | Dorado-specific build template |
| `DESIGNAUTOMATION.CM!1`, `EDBUILD.MEMO!1`, `EdBuild.dm!1` | the general flow + memo |
| `NETDELAYS.DM!1` | net delay data |
| `TestAnalyze.dm!1` | test cases for Analyze |
| `*.wl` — `aubopenpins.wl`, `aubshortedpins.wl`, `storopenpins.wl`, `storshortedpins.wl` | **wire lists, apparently real board output** ("stor" = storage board?) |
| `ecldatasheets.dm!1`, `ttldatasheets.dm!1` | part datasheets |

**Why this is worth real effort**

- **`DORADODESIGNAUTOMATION.CM` is the Dorado's own flow**, so the intended
  procedure is recorded rather than guessed — the same pattern that made the
  Cedar and Lisp bring-ups tractable.
- **`ECLDICT.ANALYZE` matters specifically**: the Dorado is ECL, and a
  netlist is only as good as its component dictionary.
- **We can run Alto programs.** `ANALYZE.RUN`, `BUILD.RUN` and `GOBBLE.RUN`
  are Alto executables and our Alto-on-Dorado world runs Alto binaries off a
  disk pack today. So the first experiment is "put ANALYZE.RUN and a Sil
  file on a pack and run it", not "port a BCPL program".
- **Sources exist for everything**, so where the period binaries prove
  awkward, re-implementation is informed rather than reverse-engineered.

**Suggested first steps**

1. Fetch the directory (it is small apart from the `.dm` archives) and read
   `SILMANUAL` + `DORADODESIGNAUTOMATION.CM` before touching code.
2. Run `ANALYZE.RUN` on one Dorado sheet inside our Alto world — start with
   a small one, not `ProcH`.
3. Compare the extracted netlist against the emulator for one well-understood
   block (the shifter or ALUFM would be good: small, and we already know
   what correct looks like from the HM tables).
4. Then the Sil -> Verilog path (see below — it is the point, not a stretch).

### F.1 Tim: build the Verilog FROM the Sil files, not from the manual

**This upgrades F from "nice verification tool" to the recommended Phase 2
route**, and it retires a caveat I had written here (that a netlist of ECL
parts is far from synthesizable RTL and Verilog generation was a stretch
goal). Tim's points:

- **PARC built the Dorado with the Sil tools.** The drawings are not
  documentation of the machine, they ARE the design input the boards were
  fabricated from.
- **They updated the Sil files as they fixed bugs.** So the drawings track
  the machine's real revision history, and the latest revision of each board
  is the definitive, as-shipped Dorado.
- **He considers this better than working from the manual text.**

That last point deserves care against this project's standing norm ("treat
the Hardware Manual as canon"). The reconciliation: **the manual is canon
for ARCHITECTURE and intent — it explains what the machine is meant to do
and stays the reference for the C emulator — while the Sil files are canon
for WHAT WAS BUILT.** Where they disagree at the gate level, the drawings
won, because the boards were made from them. The Sep-1981 manual is also a
snapshot; the drawings kept moving (our own tree has DispM at revisions Cf,
Cg, Ch, mwRev-Ch, apcRev-Da, and DispY at eight).

**Practical consequence: always take the LATEST revision of each board.**
Our `DoradoDocs/doradodrawings/` deliberately keeps every revision; for
netlist extraction that is a hazard, not a feature. Pick the newest per
board and record which was used.

### F.2 BLOCKER: we have no .sil source, only rendered PDFs

Checked 2026-08-06. `DoradoDocs/doradodrawings/` and
`DoradoDocs/schematics/` hold **PDFs of Press files** — renderings. **Zero
`.sil` files locally.** ANALYZE consumes `.sil`, so nothing in F can start
until the sources are fetched.

They exist: **1,891 `.sil` files are indexed in `chm/cross-reference.html`**,
of which **293 match Dorado board names** (`BaseBd*`, and the Proc/Mem/IFU/
Disp/DskEth sheets). Fetching those 293 (plus the `_cd6_/sil` toolchain from
F) is the real first task, ahead of everything else in this section.

Note the archive also holds D0 drawings (`cpe-FP*.SIL` and
`cyan/d0/testerdrawings.disk`), so filter by board name carefully — D0 is
the sibling machine, not the Dorado.

**Related:** `docs/sil-schematics-handoff.md` (we already display Sil
drawings inside Cedar), and the `garage` material in E1 — the hardware team
directories are the natural place for more of this.

## G. The CHM archive AS the PARC file servers — "you are at PARC"

**The idea:** make the guest see the real IFS hosts. From Cedar's
CommandTool or Lisp's Exec you would `List [Indigo]<Dorado>*`, `Bringover
[Cedar]<Cedar6.1>Top>...`, or open `{PHYLUM}<LISP>Lyric>Basics>` and have it
work — because the archive is mounted behind our in-process file servers
rather than a hand-curated local tree.

**Most of the hard parts are already built.** `src/ethernet.c` serves STP
(Cedar), Leaf (Lisp random access, IFS leader pages included), EFTP/Mayday,
and answers NetDir name lookups; STP **Enumerate** and the **LookupFile**
single-Pup exchange landed 2026-07-30, which is what makes `ls` and `OpenR`
work. So this is largely a **backing-store** change: point the existing
servers at the archive instead of `chm/cedar/stp-root` / `chm/lisp/ftp-root`.

**What the archive gives us for free**

- It **is** the IFS contents, organised by volume — `indigo/`, `phylum/`,
  `cyan/`, `eris/`, `_cd6_/`, `_cd8_/` — which map onto the real host names
  the guests already type.
- **`chm/cross-reference.html` is the index**: 568k lines of
  `filename -> [Host]<Dir>Name!version size checksum date author`. That is
  precisely the metadata an STP plist and a LookupFile reply need (we
  already synthesise version/createTime/length; the archive supplies the
  true values).
- Version numbers, dates and authors are already in the listing, so
  `!N` semantics and "print the true version" come along.

**Design questions to settle first**

1. **Lazy fetch vs mirror.** The full archive is far too large to ship;
   on-demand HTTP fetch with a local cache is the obvious shape, and the
   cross-reference gives the URL. Decide the cache location and whether a
   cold miss should block the guest (a Pup timeout is unforgiving) or fail
   fast and warm the cache in the background.
2. **Name mapping.** Guest names are `[Host]<Dir>Sub>Name!ver`; archive
   paths are lowercased directories. Needs a case-insensitive resolver plus
   the volume-name aliases (`[Indigo]` -> `indigo/`, and note `_cd6_`/`_cd8_`
   are CD images whose original host names differ).
3. **Offline and the browser.** Native can fetch; the wasm build cannot
   reach arbitrary hosts (CORS), so the browser probably keeps a curated
   subset or proxies. **Do not regress the current worlds** — they must
   still work with no network at all.
4. **Read-only.** Everything here is an archive; writes should be refused
   cleanly rather than half-implemented.

**Why it is worth doing:** it turns the emulator from "a machine with some
files on it" into the environment those machines actually lived in, and it
is the natural payoff of the file-server work already done. It also makes
every future bring-up cheaper — the recurring pattern this session was
"the file exists in the archive but not on our pack" (`LYRIC-PARC-INIT`,
`Released-Full.sysout!2`, the ExtendedVmem module, the Sil tools in F).

**Start:** `docs/cedar-file-server-plan.md` is the existing design note for
the STP side; extend it rather than starting fresh.

## Suggested order

1. **A1/A2** — Control key and Cedar middle-click. Highest impact, most
   likely simple mapping errors, and both are testable headlessly.
2. **A5** — the keyboard map document + graphic, written as A4's audit
   proceeds. Cheap, and it is what a new user needs most.
3. **E1** — fetch the garage archives. Cheap, and may inform A6.
4. **B1** — the checkpoint crash. Deeper; touches the disk shim.
5. **D1** — correct the colour doc. Minutes of work, but it prevents
   scoping colour off a false premise.
6. **C1 / D2 / D3** — features, once the machine is comfortably drivable.

## What NOT to do

- **Do not gate any of this on pixel counts.** Three runs of an identical
  Lisp failure spanned 14,308 px this same session, and a broken world sat
  within 2% of a good desktop. Convert the framebuffer and look at it, or
  read the wire.
- **Do not add `dorado_machine` struct members** (snapshot ABI). Use
  file-scope statics reset in the create/init path, as the key queue and
  the display-active cache do.
