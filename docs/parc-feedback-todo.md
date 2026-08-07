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

## A. RESULTS (2026-08-07): A3 fixed, A1 fixed, A3b done, A4 audited

Read this before the per-item history below, which is kept because the
wrong turns in it are instructive.

### A3 — FIXED. Two writers of the UTILIN cell.

**Root cause: the running microcode is a second writer of `0177030`, and it
was writing ALL BUTTONS UP.** HM Table 24: the terminal microcomputer
serialises message `05B` = "mouse buttons and keyset (Alto `177033B`)", and
the Alto-terminal microcode stores what it receives. We *do* model that
stream — `display_terminal_keyboard_bit()`, where `keyboard_words[4]` is the
type-5 body — but nothing ever drove word 4 from the mouse, so it reported
all-up forever while `machine_seed_utilin` poked the same cell with the real
state. Measured: `task=4 pc=0o2442/0o2451` stores `177777` to
`0177033`/`0177030` every ~160,000 cycles, against our repair every 16,384.

The guest loses that race a few times per second, and in Interlisp-D that is
enough to break every pop-up menu. `\keyhandler1` (LLKEY!88) calls
`\domousechording` only when `\em.realutilin` **changes**, so one sampled
all-up restarts the chord state machine, which by its own comment "ignores
the down bit for now" and holds `\em.utilin` UP for a full
`\mousechordmilliseconds`. `MENU.HANDLER`'s `until (MOUSESTATE UP)` then
fires on whatever item the pointer is over — "you always get the first item
and we can't even see it".

Fix: `dorado_machine_set_mouse` now also sets terminal word 4, so both
writers agree. One line plus its explanation.

Measured A/B over one 250 M-cycle button hold, stores to `\em.utilin`:

| build | stores | of which all-up |
|---|---|---|
| before | 20 | 14 |
| after | 3 | 2 (the initial 50 ms chord wait, and the real release) |

And on screen: dragging onto `TEdit` leaves the row inverted with the menu
open (893/1066 black) instead of the menu already gone (297/1066).
**Submenus work**: rolling right out of `EXEC>` opens
`Xerox Common Lisp / Common Lisp / Interlisp`, which is exactly the choice
the reporter said was unreachable
(`docs/images/lisp-exec-submenu-2026-08-07.png`), and dwelling on an item
for `MENUHELDWAIT` prints its help ("Start a new Exec") in the Prompt
Window. Gate: **`make verify-lisp-menu`**.

**The instrument that cracked it, after five wrong theories, was pointing at
the right cell.** `MOUSESTATE` reads none of the words we had been tracing:

| hop | cell | address | written by |
|---|---|---|---|
| 0 | `\em.realutilin` | `0177030` | us, and the microcode |
| 1 | `\em.utilin` (fake) | **`0o1400074`** | `\domousechording`, only on a CHANGE of hop 0 |
| 2 | `keyboardevent.wu` | ring | `\keyhandler1` |
| 3 | `\lastkeystate.wu` | — | `\dobufferedtransitions`; `getmousestate` reads THIS |

Position is its own chain: we write `\em.mousex/mousey` (`0424/0425`),
`\trackcursor` clips and writes `\em.cursorx/cursory` (**`0426/0427`**), and
`getmousestate` reads those. `\em.utilin`'s address is not runtime-unknowable
as an earlier note claimed: `\setiopointers` binds it to
`(locf (fetch (ifpage fakemousebits) of \InterfacePage))`, `\InterfacePage`
is `(6 0)` = word 6·65536 = 393216, and `FAKEMOUSEBITS` is word 60 of
`IFPAGELAYOUT` (LLPARAMS!30). Verified in the running guest by two
self-check words in the same page: `MachineType` (word 13) reads `5` =
`\DORADO`, and `FAKEKBDAD4/5` (words 66/67) read `0177777`, which is what
`\setiopointers` plants there. `DORADO_LISP_MOUSE_CHAIN=1` prints the whole
chain; `DORADO_STORE_TRACE_VA=1400074,1400074` names the writer.

Two earlier conclusions were wrong and are retracted:

- **"The button is eliminated from the guest's side."** The 331 sampled
  reads all showed DOWN because the race window is small — the guest sees
  all-up about three times per hold, and a sparse read trace missed them.
- **"`DORADO_UTILIN_SCAN` shows the instrument is blind above 2^22."** The
  whole Lisp address space *is* 2^22 (that is the same 2^14-page boundary
  that closed `Full.sysout!6`), so identical counts at 2^22 and 2^24 are
  expected. The real defect was in phase 2, which scanned on the same tick
  our cell went up, before the guest could re-poll.

### A1 — FIXED for the browser; the native path was never broken.

`web_shell.html`'s `sendKey` began `if (e.metaKey || (e.ctrlKey && code
!== Control...)) return;` — so **every** Ctrl+<key> event was dropped before
reaching C. The C side was fine all along. Now only the chords the browser
owns (`R T N W L V P`, Tab, F5, F12, and Ctrl+Shift+anything) are left to
it; everything else goes to the guest.

Native was verified working, not assumed: in the Lyric Exec, typing
`abcdef ghijkl` then Ctrl-W leaves `abcdef ` — Interlisp's delete-word.
Gate: **`make verify-ctrl`**, validated in both directions (405 ink with no
chord, 439 with `--key-chord w` — the key delivered without its modifier,
which is the failure mode — and 260 with `--key-chord ctrl,w`).

The `il:FILesLOaD` mixed-case corruption is a *separate* issue and is not a
modelling error: `typetext.c` already presses a modifier a full `key_hold`
before the base key and releases it after, so at `--key-hold 200000`
(~3.2 ms of guest time) it is simply typing faster than the guest samples.

### A3b — DONE. Caps Lock, in both frontends, as a LATCH.

The matrix entry was already right (`display.c`, word 3 mask `0x0080`). Both
frontends now mirror the host's caps-lock **state** (`SDL_GetModState() &
KMOD_CAPS`, `e.getModifierState('CapsLock')`) rather than its press/release,
because both report a down/up pair per toggle and the naive wiring leaves the
Alto's LOCK stuck on.

### A4 — AUDITED. The 61-key matrix is correct; the reachability was not.

Every entry of `display.c`'s `key_map` was checked against **three
independent sources** and all four words agree in every position: Alto HW
Manual Figure 6 (doc p.27 = PDF p.34), ContrAlto's `Keyboard.cs`, and — best
of all, because it is a Xerox source that also names the keys — Cedar's
`TerminalDefs.mesa KeyName`, which enumerates the whole `KeyBits` block
starting at `177033`. That third source settles the one row where Figure 6's
scan is ambiguous: word 2 bit 10 is `Period` and bit 11 is `SemiColon`, as
we have it.

It also names the three unmarked keys from the Alto keytops — **Spare1
"Look"** (right of BS), **Spare2 "Next"** (right of RETURN), **Spare3
"Swat"** (lower right corner) — our BLANKTOP / BLANKMIDDLE / BLANKBOTTOM.
Interlisp reads all three in `\eventkeys` as mouse-event modifier bits, so
they are not decorative.

And it confirms the CLAUDE.md discrepancy A4 flagged: `TerminalHeadDorado.mesa`
has `keyboard: ... _ LOOPHOLE[LONG[177033B]]` and `mouse: ... _
LOOPHOLE[LONG[424B]]`, i.e. KeyBits **starts at** the UTILIN word and spans
forward over the four Alto keyboard words — exactly what
`machine_seed_cedar_keyboard` does.

What the audit did find is that correct bits were **unreachable**:

- SDL and the browser had no host key for the left arrow (`←`), LF, or Look
  / Next / Swat. Now on backquote and F2/F3/F4/F6, documented as stand-ins
  chosen for being free rather than for resembling the keytops.
- `dorado_char_to_key` could not produce TAB, ESC, DEL or BS, so `--type`
  and paste could not send them: TAB fell through to "cannot type", ESC
  (`033`) and DEL (`0177`) are outside the control-code range, and BS
  arrived as Ctrl-H, a different key.

### A2 — NOT a button-number error. Still open, needs a precise repro.

Checked against the period source rather than intuition, as the item asked.
`TerminalDefs.mesa` gives `Red(13)` = left, `Blue(14)` = right,
`Yellow(15)` = middle; in the `177033` word those are masks `0004`, `0002`,
`0001` — bit for bit what `machine_seed_cedar_keyboard` writes. And the
A3 two-writer race does **not** reach Cedar: a store trace on `0177033`
across a 120 M-cycle middle-button hold in the live desktop shows **zero**
guest stores, so on that path we are the only writer.

So either this is authentic Tioga behaviour (yellow does have copy/insert
semantics in Cedar) or it is something narrower than the button mapping.
Get the exact viewer, gesture and expected result from the reporter before
changing anything.

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

### A3 CORRECTED — it is the SUBMENU that never appears

The reporter, on seeing the above: *"you are supposed to be able to choose
things from that menu, but we always get the first item and we can't even
see it."*

That is a different and better-specified symptom than "cannot pick a
submenu item". Read it as: the parent menu is fine, but **moving onto an
item with a `>` arrow never opens its submenu**, so there is nothing to
choose from and releasing yields the parent's default — the first sub-item.

**Our own EXEC result is consistent with exactly that**, and I initially
mis-read it as success: releasing on `EXEC>` produced
`Specify region for window "Exec"` — the DEFAULT action — rather than a
choice between Interlisp / Common Lisp / XCL Execs. That is "you always get
the first item".

**BLOCKER: we cannot currently see the screen while a menu is up.**
`--click`, `--drag` and `--menu` each run their press/travel/hold inside
their own `dorado_machine_run_until` loops, which **bypass the periodic
`--shot-every` path**. Measured: a latency run with `--shot-every 4000000`
has a **151,000,000-cycle hole** in its frame sequence
(14,396,000,338 -> 14,547,000,354) covering the entire press. `--menu`
writes two shots of its own, but nothing else does, so a drag onto a
submenu parent is unobservable.

### A3 CHARACTERISED (tooling fixed, behaviour measured)

`--shot-every` now fires inside the click/drag/menu holds (commit
`7a339fb`), so the interaction is observable. With frames every 10 M cycles:

| cycle | event |
|---|---|
| 14.400 B | button press (right) |
| **14.410 B** | **menu appears**, +1,263 ink — 10 M cycles = ~162 ms of guest time. Reasonable, not pathological. |
| 14.410-14.460 B | menu stable through the dwell |
| ~14.465 B | pointer arrives on `EXEC>` |
| **14.470 B** | **menu VANISHES**, -794 ink — **button still held** |
| 14.520 B | `Specify region for window "Exec"` — the item has been acted on |
| 14.585 B | our release (far too late to matter) |

**The button is verifiably still down.** `DORADO_MOUSE_TRACE` prints only on
change and shows exactly three transitions for the whole run:

```
177777 (idle)  ->  177775 (right pressed)  ->  177777 (released)
```

So there is **no spurious release**: the emulator holds the button word at
`0177030..0177033` correctly for the entire interval in which the menu
closed and the item fired. Two mechanisms already excluded:
`machine_store_va_if_changed` re-reads the real cell and rewrites if the
guest clobbers it, and `mouse_present` is set once and never cleared.

**So the defect is: selection fires on pointer-ARRIVAL, not on release.**
That is precisely the reported symptom — you get whatever you first land on
and never see a submenu, because the menu is gone before you can navigate.

**AND THE MENU IS FINE WHEN THE POINTER STAYS INSIDE IT.** Repeating the
drag entirely within the menu bounds — (740,495) -> (740,458), menu spans
x=705..783 — the menu **stays up for the whole 120 M-cycle hold**:

```
14.410 B  +1263  menu appears
14.420 - 14.550 B  ink steady 619,459..619,478  (only the +/-19 cursor blink)
```

No collapse, no premature activation. So the emulator holds a menu open
correctly, and the earlier collapse was caused by the pointer's path, not by
a lost button.

**The remaining difference is the ENDPOINT, and it is a good lead.** The
run that collapsed ended at **x=775**, which is on the `>` submenu arrow
(arrows sit at x~779); the run that stayed open ended at **x=740**,
mid-item. So:

- pointer mid-item -> menu stays up, item highlights (correct)
- pointer **on the `>` arrow** -> menu closes and the item's **default
  action fires**, instead of opening the submenu

That is exactly the reported symptom, localised to the submenu-arrow
interaction. Note the earlier drag also STARTED 5 px outside the menu's left
edge, which is a second confound to eliminate — rerun the arrow test
starting inside.

### A3 ROOT CAUSE (2026-08-06): entering the menu selects immediately

The endpoint sweep inverts the arrow theory above. Measuring the menu's
real bounding box from a frame diff — **x=741..824, y=362..495**, not the
x=705..783 I eyeballed — and holding the start fixed at (740,495):

| endpoint | vs the menu box | menu during the hold |
|---|---|---|
| x=740 | **outside** (1 px left of the edge) | **stays up** the whole 120 M hold |
| x=765 | inside | **collapses on arrival** |
| x=775 | inside | **collapses on arrival** |
| x=780 | inside | **collapses on arrival** |

So it has nothing to do with the `>` arrow. **The pointer merely entering
the menu selects an item and closes it**, without waiting for the button
release. The reason `--menu` with no motion appeared to work is that the
menu opens to the RIGHT of the press point, leaving the cursor just outside
it.

That is exactly the report: you cannot navigate, because the first item you
touch is taken instantly, and the menu is gone before you can read it.

**Mechanism, from `chm/lisp/lispcore/sources/MENU!29`** (fetched from
`eris/lispcore/sources`, 76 KB, 27-Jan-1986 — the closest source to Lyric;
none of the local `lispcore/sources` had it):

- the tracking loop branches on **`(MOUSESTATE UP)`** vs
  `(MOUSESTATE (NOT UP))`, commented *"mouse hasn't been down but just went
  down"* (line ~3979);
- a submenu needs the item held for
  **`LOCALMENUHELDWAIT (OR (FIXP MENUHELDWAIT) 1200)`** — 1200 ms, i.e.
  ~74 M bb cycles — via `HOLDTIMER` / `(TIMEREXPIRED? HOLDTIMER)` and the
  comment *"same button in same region for MENUHELDWAIT"* (lines 3722,
  5174, 13891).

So the loop polls `MOUSESTATE`, and everything observed follows if
`MOUSESTATE` reads **UP** while the button is physically held: the menu
opens on the down-edge, then the first pointer-inside sample looks like a
click and selects. The 1200 ms submenu dwell can never be reached because
the item is taken on entry.

**But our UTILIN cell is correct** — `DORADO_MOUSE_TRACE` shows the word at
`0177030..0177033` holding `177775` for the whole interval, with only three
transitions in the run. So `GETMOUSESTATE` is NOT reading what we maintain,
or is not reading it the way we assume.

### A3 — the chording explanation below is RETRACTED (2026-08-06)

**Do not act on the chording section that follows.** Two checks kill it:

- `LLKEY!88` line 42: `(initvars (\mousechordticks) (\mousechordmilliseconds 50))`
  — `\mousechordticks` initialises to **NIL**, and `\domousechording`'s first
  test is `(or (null \mousechordticks) ...) -> real and virtual the same`. So
  chording is OFF unless something calls `mousechordwait`, and nothing is
  known to.
- More decisively: if a single right press were reported UP, **the menu
  could never open at all** — and it opens reliably.

The section is kept because the two-utilin structure it documents
(`\em.realutilin` = our `0177030` cell vs `\em.utilin` = the virtual word
`MOUSESTATE` reads) is real and useful. The *conclusion* drawn from it was
not.

**What is actually established, and it is narrow:**

| test | motion? | result |
|---|---|---|
| `--menu` at (740,495), hold 120 M | none | **menu stays up** the whole hold |
| `--drag` ending anywhere inside the menu box (765/775/780) | yes | **closes on arrival** |
| `--drag` ending at 740, one pixel OUTSIDE the box | yes | **stays up** |

So the trigger is **the pointer being inside the menu**, not motion as such
(x=740 moves too and survives). MENU's loop is
`(until (COND (MOUSEDOWN ... (MOUSESTATE UP)) ((MOUSESTATE (NOT UP)) ...)))`
— i.e. select on RELEASE — so `MOUSESTATE` must be reading UP once the
pointer is inside. Our cell at `0177030..0177033` is verifiably still
`177775` at that moment, and `dorado_machine_set_mouse` stores buttons
without clearing anything.

**So the gap is between our cell and `\em.utilin`.** Finding that cell is
the next step, and a brute-force memory scan **did not work** — recorded so
the next attempt does not repeat it.

`DORADO_UTILIN_SCAN` (in `machine_seed_utilin`) is a two-phase search:
phase 1 records every word matching the pressed pattern, phase 2 reports
those that follow the button back to all-up on release. Three variants all
returned **`phase1: 115 words hold 177775` / `phase2: 0 of 115`**:

| variant | result |
|---|---|
| scan at the press instant, low 2^22 | 115 / 0 |
| scan after 200 held ticks (~3.3 M cycles), low 2^22 | **exactly 115 / 0** |
| scan after 200 held ticks, full 2^24 | **exactly 115 / 0** |

**Getting the identical count after quadrupling the range is the tell:** the
scan is not seeing Lisp's high memory. Its VPs run to 65533, i.e. VAs to
~16.8 M words, but `dorado_visible_word_at_va` evidently does not return
live data for those pages here. So "no cell tracks the button" is a
statement about the instrument, **not** about the guest, and nothing should
be concluded from it.

(A single-phase scan is hopeless for a different reason worth noting:
`177775`/`177776` are just -3 and -2 and litter Lisp memory — the first
attempt returned dozens of matches per button with **zero overlap** between
right and middle.)

### A3: the BUTTON is eliminated from the guest's side (2026-08-07)

`DORADO_LOAD_TRACE_VA=177030,177033` already exists and logs guest READS of
a VA range — no new instrument was needed. Over a drag that reproduces the
bug it logged **3,857 reads** of `0177030`, all from `pc=0o1425`:

| window | value read | count |
|---|---|---|
| 14.4001-14.4014 B (just after the press) | `177777` (up) | 28 |
| **14.4020 B onward, through the collapse at 14.470 B** | **`177775` (down)** | **331** |

The 28 "up" reads are a startup transient before our seeding propagated —
the first correct DOWN read is at 14.4020 B, and **every read after that is
DOWN**, including throughout the dwell, the travel, and the moment the menu
closes.

**So the guest sees the button held, correctly, for the whole interaction.**
That eliminates the button as a cause from the GUEST's side, not merely
ours, and it retires the whole family of theories in this section: chording,
a lost release, a stale cell, cache coherency. The menu closes and the item
fires while `MOUSESTATE`'s underlying cell reads DOWN.

**What that leaves:** the trigger is the pointer's POSITION (established
earlier: inside the menu box collapses, one pixel outside does not), with
the button verifiably down throughout. So either MENU is reading position
in a way our `MOUSELOC` writes at `0424/0425` mislead, or `MOUSESTATE` does
not derive from `0177030` at all on this machine type.

**Do not spend more effort on the button.** The next instrument should be a
PC trace of the guest in the narrow window where the menu closes
(`DORADO_TRACE_GATE` around 14.465-14.475 B), to see what code path runs —
or `\SETIOPOINTERS` read directly to settle where `MOUSESTATE` actually
looks.

**Better next approaches, in preference order:**

1. **Follow `\SETIOPOINTERS` instead of searching.** LLKEY's Dorado arm
   plants the pointers; read what it stores and where, and dereference
   `\em.utilin` directly rather than hunting for its value. `machine.c`
   already cites this function for the keyboard/mouse addresses, so the
   groundwork is there.
2. **Trace guest READS of `0177030`.** If the guest never reads our cell
   during the hold, the propagation question is moot and the answer is
   upstream.
3. Only then consider instrumenting `MOUSESTATE`/`GETMOUSESTATE` directly.

### (RETRACTED) A3 MECHANISM: mouse CHORDING reports a single button as UP

From `chm/lisp/lispcore/sources/LLKEY!88` (fetched from
`eris/lispcore/sources`, 171 KB — the Lyric-era low-level keyboard/mouse
module our `machine.c` already cites for `\SETIOPOINTERS`).

**Interlisp keeps TWO utilin words**, which is the piece we were missing:

- `\em.realutilin` = `(emaddress utilin.em)` — the hardware cell at
  `0177030`, which is what our `machine_seed_utilin` writes;
- `\em.utilin` — a **virtual/"fake"** word that the rest of the system,
  including MENU's `MOUSESTATE`, actually reads.

The bridge is `\domousechording`, whose own docstring says it "Sets contents
of `\em.utilin` to reflect the **virtual** mouse state, which may contain a
middle mouse button even where there is only a two-button mouse."

And this is the path a single right-button press takes:

```
(t (* Either l or r or both are down, so have to decide about Middle)
   (setq realutilin (logor realutilin \mouse.lrbit))    (* Turn off the l and/or r bits)
   (cond ((eq lrstate \mouse.lrbit)
          (* Both l and r down at once, interpret as middle without waiting) ...)
         ((neq state \dlmouse.waiting)
          (* Only one of l and r down.  Set timer, and IGNORE THE DOWN BIT FOR NOW)
          (\boxiplus (\rclk (locf (fetch dlmousetimer of \miscstats))) ...
```

**One button down is deliberately reported as UP** — the l/r bits are OR-ed
out of the virtual word (utilin is active low, so OR = "up") — while a timer
runs to see whether the other button joins and makes a middle chord. So
MENU sees UP, and pointer-entry looks like a click. Every observation
follows.

The state only leaves `\dlmouse.waiting` when the timer expires, checked in
the poll loop as
`(igreaterp (\boxidifference (\rclk mousetemp) mousetimer) 0)`.

**So the question is now specific:** either the chord timer never expires
in our emulator, or chording is enabled when it should not be. Two things
to check, in this order:

1. **`\mousechordticks`.** The function's FIRST test is
   `(or (null \mousechordticks) (eq lrstate 0))` -> "real state and virtual
   state the same". **If `\mousechordticks` is NIL, chording is off and none
   of this happens.** Find out what it is in our world; if it is non-NIL,
   ask why (a Dorado has a real 3-button mouse and needs no chord).
2. **The timer.** It is built from `\rclk` and `dlmousetimer` in
   `\miscstats`. Our `\RCLK` is known-good (memory records the guest's own
   clock tracking within 0.1%), but `\boxiplus`/`\boxidifference` on a boxed
   value and the units of `\mousechordticks` are worth confirming before
   assuming the timer is fine.

Note the code is commented "Handles mouse transitions on a DLion" — this may
be a Dandelion-oriented path that a Dorado should not be taking at all, in
which case the fix is upstream of the timer.

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

### A5. Keyboard mapping documentation — text AND a graphic [DONE 2026-08-07]

Both are in the repo's **`README.md`**, which did not exist before:
a keyboard-shaped **`docs/images/alto-keyboard-map.svg`** with every keytop
labelled `word.bit` and its host key, the four `KBDAD` words as a bit table,
the host-key stand-ins, the mouse buttons, and the per-world notes
(Interlisp's right-button menus, Cedar's `KeyBits` at `177033B`, Smalltalk's
keyset).

Two things make it reviewable rather than merely present, which is what this
item was for:

- The graphic is **generated** (`tools/make_keyboard_svg.py`) from a table
  that is machine-checked against `display.c`'s `key_map` -- all 61 cells,
  no mismatches -- so the picture cannot drift from the emulator. The
  README's bit table is checked the same way.
- It has exactly **61 keys**, which is what HW Manual 5.1 says the Alto
  keyboard has, so nothing is missing.

Provenance is stated on the artifact: the `word.bit` labels are Figure 6
verbatim, while the physical arrangement is a reconstruction from the
placement notes in `TerminalDefs.mesa` ("CTRL (large key left of A)",
"LOCK (Alto lower left corner)", 'Spare1 "Look" (blank key right of BS)')
over the standard QWERTY body -- not a traced photograph.

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
