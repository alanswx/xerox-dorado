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

### A4. Audit every keyboard mapping [reported]

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
- **Find the documentation first.** Check the Hardware Manual's I/O
  sections and the schematics for the keyboard/mouse interface before
  modelling anything (project norm: do not invent behaviour).

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
