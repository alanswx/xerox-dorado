# Colour graphics — TODO and reference

Everything gathered 2026-08-06 about adding the Dorado colour display, in
one place: the reference material first, then the work list.

**Status (2026-08-09): the colour path is working end to end.** The emulator
models DispM presence, the colour RAMs, the A/B/C mixer path, RGB output, and
the second SDL/browser display surface. Cedar's saved colour checkpoint now
restores the board, paints a 640x480 RGB frame beside the 1024x808
monochrome frame, and the browser's Both / Color / Monochrome controls work.
Gargoyle also launches from the Cedar 6.1 tree. The remaining work is
validation: exercise the historical Koto Lisp path, validate cursor crossing
in both frontends, and add a stable guest-driven colour regression gate.

The browser fix was two-part: DispM is host-side state outside the snapshot
ABI, so the board is explicitly reattached after restoring the colour
checkpoint; the JavaScript blit must use the emsdk 6 `HEAPU8` runtime view,
not the nonexistent `Module.HEAPU8` property. The verified build is commit
`658f1b6`.

**Verdict from the scoping pass:** doable and better supported than
expected. The hardware is fully documented, we hold the board schematics,
the microcode is in hand *and already inside two worlds we boot*, and
there is period software on our own Lyric pack to test with.

---

# PART 1 — REFERENCE

## 1.1 CORRECTED 2026-08-06 — there ARE two boards

> **This section previously said "There is no separate colour board. The
> same DDC drives mono and colour... this is not 'emulate new hardware',
> it is 'finish the display controller we already have'." That was wrong**,
> corrected by the PARC veterans on the 2026-08-06 call and verified
> against the schematics. It was the premise this whole document was
> scoped from, so **re-read Part 2 with that in mind** — the estimates
> below were made under a false assumption and are likely optimistic.

**DispY is the monochrome board; DispM is the colour board.** Two boards:

| board | title page | contents |
|---|---|---|
| **DispY** = **DDC**, the display controller | "Display Y", K. Pier, 22-Oct-81, 31 sheets | FOUT interface, Pointers, Fifo, Item Permutation Logic, Channel A/B data paths, Next Line Control Block, Horizontal RAM, DWT Task WakeUp Logic, Cursor, Slow IO, **Alto Display Drivers**, **MiniMixer**, OIS Terminal Interface, Mufflers |
| **DispM** = **DDM**, the display mixer | "Display M", K. Pier, Nov 1982, 32 sheets | sheets 1-11 the Alto display controller, **sheets 12-31 the Mixer**: ABuf/BBuf/CBuf, BMap, CMap, mixer Blue/Red/Green bytes, **DACs Red Green Blue**, PLL |

**Confirmed by the Hardware Manual itself** (doc p.110 = PDF p.117), which
is project canon and settles it beyond the schematic title pages:

> The DDC is implemented on two Dorado main logic boards, called **DispY and
> DispM**. DispY contains all the logic necessary for vertical and horizontal
> sweep control, channel data paths, and video data for **binary and
> grey-level monitors** running at a fixed pixel clock rate. **DispM contains
> the color maps, the programmable pixel clock, and the three DACs for
> driving a color monitor.** Additionally, DispM contains an independent
> terminal controller ... specialized to driving a 7-wire terminal.
>
> ... On a Dorado with only a 7-wire terminal and no color monitor, **only
> the DispY board is present**; it is programmed for Alto terminal emulation,
> and only a small subset of its capabilities are used. However, on a Dorado
> with both a 7-wire terminal and a color monitor, **the DispM board is also
> present**.

So colour literally means adding a board that need not be there. The same
page also names the three DispY output interfaces (Alto monitor, seven-wire
1 bit/pixel, and an 8-bit DAC for grey-level), and the MiniMixer as a
256-word x 4-bit map.

The other decisive evidence is that **both boards carry a "DDC to DDM
Interface Table"** — they are two boards talking to each other, not one
board in two modes. DispY's **MiniMixer** is what gives the monochrome board its limited
mixing; the full Mixer, the colour maps and the DACs are on DispM.

So colour does mean bringing up a second board's worth of behaviour, and
the framing "finish the controller we already have" understated it.

**Also:** `DoradoDocs/schematics/DispY.pdf` is **mislabelled** — its pages
are `DskEth-Rev-Ce.ps`, the disk/ethernet signal cross-reference. Use
`DoradoDocs/doradodrawings/DispY-*.pdf` (8 revisions), consistent with that
directory's own note that its copies are the better ones.

## 1.2 Source material and where it lives

| what | where |
|---|---|
| **Colour board (DDM) schematics** | `DoradoDocs/doradodrawings/DispM-apcRev-Da.press!1.pdf` — 32 sheets, K. Pier, Nov 1982. Four earlier revisions beside it (Cf, Cg, Ch, mwRev-Ch). **Do not use** `DoradoDocs/schematics/DispM-apcRev-Da.press!1.pdf` — it is a degraded 164 KB rendering of the same file; the `doradodrawings/` copy is 1.7 MB. |
| **Mono board (DDC) schematics** | `DoradoDocs/doradodrawings/DispY-apcRev-Da.press!1.pdf` — 31 sheets, K. Pier, 22-Oct-81, plus 7 earlier revisions (Cg, Ci, Cj, Ck, Cl, mwRev-Cj, mwRev-Ck). **`DoradoDocs/schematics/DispY.pdf` is mislabelled** and actually contains `DskEth-Rev-Ce.ps`. |
| **Colour monitor** | **Conrac RGB** (per the PARC veterans, 2026-08-06). Bears on §1.6 — the MType field has to say this monitor is attached. Model and timings not yet pinned down. |
| Board photos | `DoradoDocs/photos/DISPM-APCREV-DA_F.jpeg` (front), `_B.jpeg` (back) |
| Microcode | `chm/doradomicrocode/doradomicrocodesources/ColorDisplay.mc!1` (Taft, 23 Sep 1981) |
| Definitions | `chm/doradomicrocode/doradomicrocodesources/DisplayDefs.mc!1` — device numbers, mode bits, RAM control constants |
| Hardware manual | HM §11, distilled in `docs/display-architecture.md` (CLCB/NLCB fields, mixer modes, 24-bit mode) |
| Interlisp driver | `chm/lisp/ftp-root/DORADOCOLOR.LCOM` (+ `COLORDEMO`, `LLCOLOR`, `DICOLOR`, `SKETCHCOLOR`, `COLOR`) — **already inserted on our Lyric pack** |
| Koto Interlisp driver lead | `[phylum]<LISP>KOTO>Library>DORADOCOLOR!1`, reported by Nick; source archive host: `xeroxparcarchive.computerhistory.org` |
| Cedar | CedarChest 6.0 `Sil/SilColor.bcd`; `Cedar.mb!6` carries the microcode |

### DispM sheet map (the colour half)

Pages 1–11 are the Alto display controller. **Pages 12–31 are the Mixer:**

| page | contents |
|---|---|
| 12 | Mixer Buffers ABuf, BBuf, CBuf |
| 13 | **BMap** |
| 14 | **CMap** |
| 15 | Mixer Address Drivers |
| 16 | Mixer Address Control logic |
| 17 / 18 / 19 | Mixer — Blue byte / Red byte / Green byte |
| 20 | Mixer Output Register and IOB drivers |
| 21 | **Slow IO Interface** |
| 22 | **DACs — Red, Green, Blue** |
| 23 | PLL Pulse Synthesizer |
| 24 / 25 | Clock Drivers / Pre Clock Drivers |
| 26 | Layout |
| 27 | **Mixer Block Diagram** |
| 28 | **DDC to DDM Interface Table** |
| 29 | **Slow IO Device Formats** |
| 30 / 31 | Configuration / Revision Record |

(PDF page = table-of-contents page + 1; sheet 1 is the Spruce banner.)

## 1.3 Device numbers (`DisplayDefs.mc`, DispM board)

| device | TIOA | | device | TIOA |
|---|---|---|---|---|
| TStatics | 367₈ | | AWTFlag | 363₈ |
| TNLCB | 366₈ | | **CMap** | **362₈** |
| **BMap** | **365₈** | | **Mixer** (AMap) | **361₈** |
| AHTFlag | 364₈ | | TStatus / VCOClock | 360₈ |

All inside `0360..0377`, which our display device **already claims** — so
these `Output←B` writes already arrive and are discarded at the `TODO` in
`display.c` (~line 652).

## 1.4 Slow-IO word formats (DispM sheet 29)

Bits MSB-first; control bits are **active low** (schematic primes them),
which is why the microcode constants are named *Dont*Keep / *Dont*Write.

| device | TIOA | format |
|---|---|---|
| MIXER | 361₈ | 0 `Keep'` · 1 `Write'` · 2 `LoadAddr` · 3 x · 5–14 `Addr.0-9` · 15 `Hi/Lo select` — **or** bits 4–15 = 12 bits of Mixer data |
| BMap | 365₈ | 0 `Keep'` · 1 `Write'` · 2 `LoadAddr` · 3 x · 8–15 `Address.0-7` **or** `Data.0-7` |
| CMap | 362₈ | as BMap |
| PIXELCLK | 360₈ | 4–11 Pixel Clock Rate · 12–15 Clock Divider |
| NLCB | 366₈ | 0–3 NLCB address · 4–15 NLCB data (12 bits) |
| AStatics | 367₈ | 14 AWT ShutUp · 15 AHT ShutUp |

**Inputs** (the readback path):

| TIOA | returns |
|---|---|
| 361₈ in | `MType.0-3`, `Green.0-7`, `Red.0-3` |
| 360₈ in | `Keyboard`, 1, 1, 1, `Red.4-7`, `Blue.0-7` |

**Cross-check** against `DisplayDefs.mc` (`RamCtrlShift` = 12):
`DontKeep` = 10₈<<12 = 0x8000 = bit 0 = `Keep'`; `DontWrite` = 4<<12 =
0x4000 = bit 1 = `Write'`; `LoadAddress` = (2<<12)|DontWrite = bits 2+1;
`ReleaseRam` = DontKeep|DontWrite. **Exact agreement between schematic and
microcode** — treat this as the spec.

Load sequence (`ColorDisplay.mc` `DoSomeTable`): one Output with
`LoadAddr` to set the address, then a stream of data words that
auto-increment, then `ReleaseRam`.

## 1.5 Sizes and modes

- **Mixer / AMap: 1024 entries × 24 bits**, written as **two 12-bit
  halves** (bit 15 selects) — which is why the microcode pushes
  0o3776 = 2046 = 2048−2 words.
- **BMap and CMap: 256 × 8**, one word each (0o376 = 254 = 256−2).
- **αItemSize** (unary): `Size1`=1, `Size2`=2, `Size4`=4, `Size8`=10₈ →
  1/2/4/8 bits per pixel.
- **αResolution**: full / half / quarter pixel-clock rate.
- **Mixer modes**: `A8B2Mode`=1, `BBypassMode`=2, `ABypassMode`=4,
  `24BitMode`=12₈ (= 24Bit OR BBypass).
  - A6B4 (default): mixer address = `AItem[0:5] || BItem[0:3]`
  - A8B2: `AItem[0:7] || BItem[0:1]`
  - BBypass: B skips the mixer into BMap, OR'd into blue
  - 24Bit: A alternately addresses AMap and CMap, B addresses BMap at half
    rate → **AMap red, BMap blue, CMap green**. True colour.

## 1.6 How the guest finds a colour monitor

Sheet 28 shows `MType.0/1/2` as a **monitor type field set by jumper
resistors on the backplane**, read back via the STATUS inputs above. So
"attach a colour monitor" = answer TIOA 360/361 inputs with the right
MType. Those reads also return DAC values, which gives a **free self-test:
write a mixer entry, read it back**.

Separately, `DisplayConfig` bit 0 = "DispM board installed" is already
modelled (`display_dispm_present()`, driven by `DORADO_DISPM_PRESENT`,
which the **Lyric world already sets**).

## 1.7 What the Interlisp driver will do

From `DORADOCOLOR.LCOM`: `DORADOCOLOR.BITSPERPIXEL` defaults to **8**,
`\DORADOCOLOR.LEFTMARGIN` = 80, `\DORADOCOLOR.ATABLEIMAGE` is the AMap
image it loads, `\RGB.TO.DORADO.RGB` converts colours,
`\DORADOCOLOR.STARTCOLOR` starts it, `\COLORDISPLAYFDEV` is the device.
Colour control block: `ColCBLow 176`, `ColCBSize 16`, `CMapPages 8`.

Which worlds carry the microcode: **`Mesa.mb!3` and `Cedar.mb!6` do**
(`COLORCTRLBLKPTR`, `COLORVSTOVSINIT`, `PROCESSAMAP`, `PROCESSCMAP`);
**`AEmu.mb` does not** — the Alto world has no colour.

### Nick's Koto Lisp lead

Nick reports that the colour display was also accessible from Lisp through
the Koto library:

```
File [phylum]<LISP>KOTO>Library>DORADOCOLOR!1
```

The source and compiled forms are now fetched and checksum-validated under
`chm/lisp/koto-color/`; these are still guest-validation work, not emulator
requirements:

- Koto's `DORADOCOLOR!1` is 17,132 bytes (CRC `9c2e369c`) and contains the
  low-level `\\DORADO\\STARTCOLOR` method. It is byte-identical to the
  broader Koto copy in `chm/lisp/koto/`.
- Lyric is not missing the feature: `DORADOCOLOR.LCOM` is a later compiled
  revision (`.;27`, 1986), with the same control-block layout and newer
  display-state/soft-cursor integration.
- Koto's `LLCOLOR` declares **two extra color-display pages**, defaults to
  4 bpp, and implements the 4/8-bit paths. No 24-bit Lisp implementation was
  found, although the board supports 24-bit mode.
- Herb Jellinek is credited in the Lyric driver and in the recovered Koto
  provenance; retain those credits.

---

# PART 2 — TODO

Ordered so each step is verifiable before the next.

## [x] 0. ANSWERED 2026-08-08 — no world we boot programs the colour RAMs

Done with `DORADO_DDC_TIOA=1`, which prints every TIOA the DDC received an
`Output<-B` on, naming what we decode and flagging the rest UNDECODED
(`dorado_display_dump_tioa_use`, `src/display.c`). The answer is a clean no,
and the microcode says exactly why, so this section is no longer guesswork.

**What the worlds actually touch.** Cedar 6.1's desktop and the Alto world
use the same four DispY devices and nothing else:

```
[ddc]   372 UNDECODED    256  first=000017 last=177417   <- MiniMixer
[ddc]   375 HRam         766  first=060000 last=140000
[ddc]   376 NLCB           6  first=000004 last=050000
[ddc]   377 Statics        6  first=043000 last=000000
```

**TIOA 0372 is the MiniMixer**, identified from PARC's own
`chm/dorado/aemu-src/DisplayMain.mc`. `JLoadMiniMixer` -- "Loads the
minimixer with the identity for Alto emulation. This is done ONCE in the
beginning of the world" -- walks an address in the top byte by `400C` per
iteration until it wraps, which is exactly **256 writes**, and emits `17B`
for white or `0` for black. First `000017` = address 0/white, last `177417`
= address 0377/white. Every field matches. It is DispY's own limited mixer,
not DispM's.

**And the microcode is explicit about why colour never comes up:**

```
* Skip over HRam and MiniMixer initialization if DispM present.
ResetDisplayConfig:
	PD_ NOT (DisplayConfig);
	TReg400C_ 400C, Branch[NoInitRams, ALU>=0];
	Call[InitHRam];
	Call[JLoadMiniMixer];
```

`DisplayConfig[0]` selects the board (0 = DispY, 1 = DispM) and
`DisplayConfig[13:15]` the monitor ({Alto, LF Alto-width, LF full screen}).
Our machines report DispY, so the microcode takes the DispY path every time.

**The flag already exists and it works.** `DORADO_DISPM_PRESENT=1` on a COLD
BOOT flips the whole thing: the Alto world moves off the DispY device set
(0373/0374/0376/0377) onto the DispM/terminal set, and stops loading
MiniMixer entirely --

```
[ddc]   363 AWTFlag   529507      [ddc]   366 TNLCB   840142
[ddc]   364 AHTFlag   128310      [ddc]   367 TStatics     4
[ddc] 0 undecoded address(es).
```

-- which is `ResetDisplayConfig` branching to `NoInitRams`, precisely as
written. **Two traps worth recording**: this must be tested on a COLD BOOT,
because a restored checkpoint ran `THTInit` long before the snapshot was
taken and flipping the flag afterwards changes nothing; and the Lisp gates
already set `DORADO_DISPM_PRESENT=1`, so Lyric is running the DispM path
today.

**So the honest scope.** Selecting DispM gets us its **Alto terminal
emulation** -- sheets 1-11 of the board. The Mixer, BMap, CMap and the three
DACs are sheets 12-31, and they are programmed by *colour application
software*, not by the terminal path any of our worlds runs. Nothing we can
boot has ever written them. Implementing the RAM-load protocol in section 1
therefore cannot be validated against a real guest until there is colour
software to run -- which makes "find or build a guest that drives colour"
the true step 1, ahead of everything below.

## [x] STEP 1 ANSWERED 2026-08-08 -- the Cedar colour application is GRIFFIN

Step 0 established that no world we boot programs the colour RAMs, which
left "find a guest that drives colour" as the real blocker. Swinehart,
Zellweger and Hagmann, *The Structure of Cedar* (ACM SIGPLAN 1985,
`https://worrydream.com/refs/Swinehart_1985_-_The_Structure_of_Cedar.pdf`)
names it, and confirms the hardware three separate ways.

**The hardware, from the people who built the system:**

- p.233, on the Dorado: "Input/output devices include a large (1024 x 808
  pixels) high-resolution bitmapped black-and-white display, a keyboard, a
  mouse pointing device, and an Ethernet interface. **A color display can be
  added.**"
- p.235, on Terminal: "There may be several instances of Terminal, each with
  its own full-screen bitmap and **optional color frame display memory**."
- p.236, on Imager: "Programs can render images in a device-independent
  fashion **on color or black and white display devices**, or on a variety of
  laser printers."

So colour is an option on a second frame buffer, reached through Imager --
which matches the netlist exactly (DispM's three DACs, sheets 12-31) and
matches the microcode (`DisplayConfig[0]` selecting the board).

**The application, p.238:** "Cedar applications in the area of computer
graphics include a program for producing **full-page color illustrations
(*Griffin*)**, a system for manipulating three-dimensional synthesized
graphical objects (*SolidViews*), programs for processing scanned images, and
programs for driving experimental printers." Figure 1 places **Griffin
(2-dimensional illustrator)** at the Applications level.

**And Griffin is in the archive, in the exact form we can already install.**
`chm/cross-reference.html` has 103 entries under `[Cyan]<CedarChest6.1>
CedarGriffin>`, including **`Griffin.bcd!5`**, `Griffin.config!1` and
`CompileDefs.cm!1` -- a compiled Cedar 6.1 binary, in a CedarChest package.
`tools/fetch_cedarchest_app.py` already mirrors CedarChest6.1 packages by
name, and the running desktop already installs them the period way:

```
Bringover -p [Cedar]<CedarChest6.1>Top>Griffin.df
Run Griffin
```

which is exactly how ChessHack got in (see the 2026-07-16 and 2026-07-21
entries in the top-level `CLAUDE.md`).

**Also worth knowing about, older and probably not directly usable:**
`[Indigo]<Griffin>colordisplaytest.dm!1_` is a *colour display test* -- but
it is dated **14-Nov-1980** and is Mesa 6-era Bravo/Mesa source, nineteen
months before Cedar existed in the form we run. `[indigo]<Griffin>color>` has
1982 sources. Treat these as documentation of what the colour hardware was
driven with, not as software to boot -- the same version wall that killed
`OthelloDorado.boot`. `CedarChest6.1>GriffinToIP` (Griffin to Interpress) is
the 6.1-era companion.

**So Griffin remains the Cedar route.** Fetch CedarGriffin, install it into
the desktop, and trace what it writes with `DORADO_DDC_TIOA=1`. The Koto Lisp
route below is the older, more direct low-level test and may be easier to use
for validating the emulator's 4/8-bit colour modes.

## [ ] STEP 1B — bring up the Koto Lisp colour path (Nick's lead)

Gargoyle is now known to launch on Cedar 6.1. This section remains the
historical low-level validation path and is still a test plan, not an
implementation blocker for the Cedar/browser colour demo.

1. **Done:** locate and fetch `[phylum]<LISP>KOTO>Library>DORADOCOLOR!1`,
   preserving its original version and source provenance in
   `chm/lisp/koto-color/`.
2. **Done:** identify the version-matched Koto boot set, including
   `Full.sysout!16`, `DORADOLISPMC.EB!1`, `AltoD1MC.eb!1`, `Lisp.run!1`,
   `Lisp.syms!1`, and `INIT.NOGREET!1`.
3. Boot Koto Lisp, load `DORADOCOLOR` and `COLORDEMO`, then use the public
   `(COLORDISPLAY T 8)` entry point (and `(COLORDEMO T)` for a visible test).
   `\\DORADO\\STARTCOLOR` is the private device operation that
   `COLORDISPLAY` invokes; it is not a user-level command. Trace the
   control-block writes, monitor selection, and first writes to the colour
   RAMs.
4. Verify that Lisp can address the **second monitor**, not merely that the
   colour board reports present. Confirm separate mono and RGB rasters in the
   frontend.
5. Reproduce the reported **two-extra-pages** allocation workaround and
   compare it with a run without it. Record whether the failure is guest
   allocation, a colour-microcode quirk, or an emulator defect.
6. Exercise 4-bit and 8-bit modes first; only then use the same path to test
   the hardware's 24-bit mode.

The reproducible Koto pack bake is now `make lisp-koto-color-pack`. It writes
the matched 15,002-page pack to
`dorado/build/good-packs/lisp-koto-color-15002.pack` and keeps all intermediate
disks under the `lisp-koto-color-work-*` prefix. The initial pack bake and
VMEM allocation completed successfully on 2026-08-09; a guest cold-boot
validation and snapshot remain.

The first Lyric experiment used the restored library checkpoint, loaded
`DORADOCOLOR.LCOM`, answered the `{DORADO}` service login with `Guest` /
`Guest`, and called the documented `(IL:COLORDISPLAY T 8)`. It returned cleanly
but produced zero DispM presence reads or RAM writes because that checkpoint
predates the color-head initialization. Lyric therefore still needs a cold
boot for a meaningful hardware test; this result is not evidence that the
driver or board is broken.

Acceptance is a guest-generated colour image plus a trace showing monitor,
control-block, and colour-table programming. A plausible RGB window alone is
insufficient.

## THE BOARD IS ALIVE IN THE GUEST (2026-08-08)

Cold boot with `DORADO_DISPM_COLOR=1`, run to the Cedar login prompt:

```
[dispm] board=installed type=standard 640x480  presence reads: 360B=3 361B=1
dorado: 28703 display-list pixels
```

**`ColorDisplayHeadDorado` asked, and accepted our answers.** It read the
board register at 360B three times and the device register at 361B once, and
the type code we return there (`0170000`, i.e. 17B in the top four bits)
is what makes it latch `displayType _ standard`. Before this the head had
never so much as looked; a Dorado in this emulator now has a colour board on
it as far as Cedar is concerned.

28,703 px is the login screen, which is the expected place to stop -- the
head starts long before login, so this is the cheapest possible test of the
presence path and it costs one cold boot rather than a desktop bake.

RAM writes are still zero, correctly: nothing has been told to turn the
display on yet. That is the next run -- cold boot, log in, install the
ColorDisplay package, `ColorDisplay on` -- and it is the first time
361B/362B/365B could ever have been written here.

## COLOUR NEEDS A COLD BOOT -- no checkpoint we ship can ever show it

Measured 2026-08-08 with the board installed (`DORADO_DISPM_COLOR=1`), the
full ColorDisplay install and `ColorDisplay on`, against the saved desktop:

```
[dispm] board=installed type=standard 640x480  presence reads: 360B=0 361B=0
[dispm] RAM writes: mixer=0 bmap=0 cmap=0
[dispm] the guest never asked whether a board is present
```

**Zero presence reads**, and that is diagnostic rather than a failure. The
detection lives in `ColorDisplayHeadDorado`'s **module initialization body** --
the fragment that ends the file, right before `END.` -- so it runs once, when
the configuration is STARTed at **boot**. Our desktop checkpoint was baked on
2026-07-15, years of emulator-time before DispM existed here, and it latched
`displayType _ none` then. Nothing typed into a restored world can undo that:
`ColorDisplay on` asks a head that has already decided there is no board.

This also settles a caution from the previous round. The earlier probe's
`ColorDisplay ?` answered `off, 8 bpp, left, 640x480, Dither`, and 640x480
looked like it might be detection reporting `standard`. It was not -- it is
the **profile default** (`ColorDisplay.Type`), exactly as suspected, because
detection never ran at all.

**Consequences, and they shape the rest of this work:**

1. **Every colour experiment must COLD BOOT** with the board installed. That
   is cheap for the presence test -- the head starts long before the login
   prompt, so a boot to login is enough to see `360B`/`361B` read.
2. **A colour desktop needs its own bake.** The ordinary Cedar checkpoints
   remain monochrome; the colour checkpoint is a separate matched snapshot/PDI
   pair baked with `DORADO_DISPM_COLOR=1` and `ColorDisplay on`. Heed
   `docs/sil-schematics-handoff.md` §5.2 and memory
   `cedar-desktop-bake-destroys-checkpoint`: the bake overwrites in place and
   exits 0 having snapshotted a LOGIN SCREEN if the timed login misses. Back
   up first, bake to scratch, check the pixel count and colour PPM, then install.
3. **This is the same trap as the counters, one level up.** There it was
   snapshotted *state* being read as live; here it is a snapshotted *decision*.
   Anything a guest determines once at boot -- device presence, configuration,
   sizing -- is frozen into every checkpoint taken afterwards, and no amount
   of driving the restored machine will re-ask the question.

## CORRECTION: the step-0 numbers for CEDAR were the CHECKPOINT's, not the run's

Recorded because it is the third instrument failure on this one question and
the pattern is worth more than the answer.

`dorado_display`'s per-TIOA counters are inside the struct `machine.c`
snapshots whole (`snap_wr(f, &m->display, ...)` / `snap_rd`). So on any run
started with `--snapshot-in` they come back **from the bake**. The "Cedar
6.1 desktop writes 256 words to the MiniMixer" figure was read off a restored
desktop that had, in that run, written nothing at all -- it was the
2026-07-15 bake's history being replayed as if it were live.

**The Alto number was genuine** (a cold boot, no checkpoint), and so was the
conclusion drawn from it: `DisplayMain.mc` really does load MiniMixer once per
world and really does skip it when DispM is present.

Three ways to be blind to the same thing, in one day:

1. **Counting inside a device.** `DORADO_DDC_TIOA` lives in `display.c`, so
   it can only see writes already routed to the display -- and display.c
   claims TIOA 0360..0377 for the DISPLAY TASKS only. Cedar's colour driver
   is Mesa code running in the EMULATOR task.
2. **Counting inside the router.** `DORADO_IO_CENSUS` moved the count to
   `io.c`, which is task-agnostic -- but BOTH `cpu.c` call sites guard on
   `dorado_io_has_write()` first, so a write to an address with no registered
   device never reaches the router either. The census proves what was
   ROUTED; it cannot see the unrouted.
3. **Counting in snapshotted state.** Anything in a struct the snapshot
   serializes measures the bake, not the run, the moment `--snapshot-in` is
   used.

**What actually works, and it is the thing `dispm.c` now does:** register the
device. An address with a device behind it is counted, decoded and traced;
an address without one is invisible at every layer. That is why installing
the board is the measurement as well as the feature.

Both instruments now say all this in their own comments, so the next person
reads the caveat next to the number rather than after it.

## RETRACTED: there IS a colour cursor, and it is a Terminal-level one

I concluded on 2026-08-08 that DispM has no cursor, from counting cursor
references in the heads: `TerminalHeadDorado.mesa` 14, the colour head 0,
`ColorDisplayFace.mesa` 0. **That was the wrong place to look**, and the
challenge that overturned it was the obvious one: if a viewer can live on the
colour screen, you must be able to get it back.

`Cedar6.1/Terminal/Terminal.mesa` has a complete colour cursor interface:

```
hasColorDisplay: BOOL,
GetColorCursorPosition: PROC [vt: Virtual] RETURNS [Position];
SetColorCursorPosition: PROC [vt: Virtual, position: Position];
ColorCursorBitmap: TYPE ~ TerminalDefs.Cursor;
GetColorCursorPattern: PROC [vt: Virtual] RETURNS [pattern: ColorCursorBitmap];
SetColorCursorPattern: PROC [vt: Virtual, pattern: ColorCursorBitmap];
ColorCursorPresentation: TYPE ~ {onesAreWhite, onesAreBlack};
```

and `Cedar6.1/Inscript/Interminal*.mesa` carries the geometry and the tracking:

```
GetColorDisplaySide / SetColorDisplaySide  [Side]
SetCursorPosition: PROC[posX, posY: INTEGER, enableTracking: BOOL _ TRUE];
GetMousePosition / SetMousePosition
```

`ColorDisplayImpl.mesa` uses both -- it imports
`Terminal.SetColorCursorPresentation` and flips the side with
`Interminal.SetColorDisplaySide[left|right]`.

**So the cursor is managed above the head, at the Terminal level, with its own
POSITION and PATTERN separate from the monochrome cursor.** The head knowing
nothing about it is expected: the head drives registers, Terminal owns the
presentation.

### AND THE CROSSING IS AN EDGE-PUSH, NOT AN EXTENDED DESKTOP

`Cedar6.1/Inscript/InterminalImpl.mesa` finishes the story, and it rules out
the model the emulator currently implements.

Both screens are **0-based and independent**:

```
color^ _ [xMin: 0, xMax: terminal.colorWidth-1,
          yMin: 0, yMax: terminal.colorHeight-1, color: TRUE];   -- line 297
right^ _ left^ _ [xMin: 0, xMax: terminal.bwWidth-1, ...];        -- line 323
```

There is no combined coordinate space. Each display has its own origin, and
crossing is detected by the pointer PUSHING PAST an edge by more than an
escape threshold:

```
mouse.x>display.xMax => {                        -- the mouse is moving right
  IF display=left AND (mouse.x-display.xMax >
       (IF display.color THEN vColorEscape ELSE vBWEscape)) THEN {
    ... mousePosition.x _ display.xMin }          -- lands at 0 on the NEW screen
  ELSE mousePosition.x _ display.xMax };          -- otherwise just clamp
```

**So injecting an absolute position can never work.** `machine.c` writes the
host pointer straight into `0424`/`0425`. Cedar clamps that to the current
display's `0..xMax` and only switches when the value exceeds `xMax` by more
than the escape -- and on switching it RESETS the position to the new
screen's origin. Push x to 1024+ and you get exactly what was observed: a
cursor appears on the colour screen for a moment, then the next absolute write
pushes past the edge again and the mapping is nonsense. The b/w screen keeps
reacting because the same coordinate is still being clamped into its space
whenever `display` is the b/w one.

**This is the delta path, definitively.** HM Table 24 message `06B` sends
mouse Δx/Δy as excess-200B -- *motion*, not position -- and `InterminalImpl`
is the accumulator that turns motion into a per-screen position and owns the
crossing. Feeding it deltas is not a fidelity nicety; it is the only input
shape in which two screens can work at all.

**Therefore `dorado_machine_set_mouse`'s widened clamp and the colour-window
coordinate offset should be REVERTED, not tuned.** They implement a combined
coordinate space that Cedar does not have. The work is task A6: deliver Δx/Δy
through the terminal serialiser and let the guest do the rest.

### ANSWERED: it is a SOFTWARE cursor, in colour-screen-LOCAL coordinates

`Cedar6.1/Terminal/TerminalImpl.mesa` settles both halves.

`SetColorCursorPosition` (line 524) hides the cursor, moves it, shows it
again -- and `ShowColorCursor` / `HideColorCursor` (lines 642, 687) are
TRUSTED procs that paint a **16 x 16** patch into the colour frame buffer
directly, bounded by `vt.colorWidth` / `vt.colorHeight` and gated on
`impl.colorMode.full OR impl.colorMode.bitsPerPixelChannelA > 0`.

So:

1. **It is composited into the frame buffer, not overlaid by hardware.** Our
   existing DispM render would therefore show it *if the guest drew it*. There
   is nothing to add to `dispm.c` for this.
2. **Its position is COLOUR-SCREEN-LOCAL** -- x and y inside
   colorWidth/colorHeight, i.e. 0..639 and 0..479 -- **not** a coordinate in
   some extended two-screen desktop.

**Point 2 means `dorado_machine_set_mouse`'s widened clamp is the wrong
model.** Extending the mono space rightward and sending x=1024+ gives Cedar a
monochrome coordinate off the right edge of the mono screen; it does not put
the pointer on the colour screen. Something above -- Interminal or Viewers,
using `Interminal.GetColorDisplaySide` -- decides the pointer has crossed and
calls `SetColorCursorPosition` with a LOCAL position. The
Terminal.mesa comment is explicit that clipping is the client's job: "It is
the responsibility of the client to clip the position ... to ensure that the
cursor remains on the visible area of the display."

**Next, and it is a read not a guess:** find who calls
`SetColorCursorPosition` in `Cedar6.1/Inscript/InterminalImpl.mesa` (it is one
of the five files that mention it) and what input drives it. That says what
the guest needs from us -- most likely just a mouse position it already
understands, with the crossing decided entirely inside Cedar, in which case
the emulator needs no coordinate work at all and the widened clamp should be
reverted rather than tuned.

## PGM or PPM? Two screens, two files -- and why the mono path must NOT change

Asked while the board was being built, and it is a real design decision.

**The monochrome framebuffer stays PGM, and that is the faithful choice.**
DispY is genuinely **one bit per pixel** -- `AltoTTLVideo`, a single video
line on the netlist. Greyscale PGM is already generous for it; making it PPM
would imply a colour capability the board does not have and would quietly
invite code that assumes every Dorado screen has colour in it.

**Colour is a SECOND SCREEN, so it gets its own file.** `dispm.c` writes
`<out>.color.ppm` (P6, 24-bit) at the colour monitor's own raster --
640x480 or 1024x768, neither of which is DispY's 1024x808. They are different
sizes because they are different monitors. `ColorDisplay left | right` is the
proof: the command tells Viewers which side of the black-and-white display
the colour one sits on, which only makes sense for two physical screens.

So the emulator's output for a colour-equipped Dorado is a **pair**: a
1024x808 PGM and a 640x480 PPM.

### What remains after the frontend work

- **Frontends: done.** SDL presents a second colour window; the browser uses
  one combined canvas with Both / Color / Monochrome controls. The guest's
  `ColorDisplay left|right` state determines the ordering.
- **Colour needs its own gate**, comparing PPMs. The existing gates `cmp` two
  PGMs and would not notice the colour screen at all.
- **Do not composite them into one image** to save a file. The guest decides
  which screen a viewer lives on; flattening that loses the one piece of
  information the arrangement carries.
- The RGB buffer is `DORADO_DISPM_MAX_W * DORADO_DISPM_MAX_H * 3` = 2.36 MB,
  file-scope in `dispm.c`. It is NOT a `dorado_machine` member and must not
  become one -- that changes the snapshot ABI and kills every baked
  checkpoint.

## THE PRESENCE TEST, VERBATIM -- three READS, and they gate everything

`ColorDisplayHeadDorado.mesa`'s module initialization ends with the colour
board detection. This is the gate `ColorDisplay on` is behind, and it is
small enough to implement exactly:

```
colorBoardAddr:  DoradoInputOutput.IOAddress ~ 360B;
colorDeviceAddr: DoradoInputOutput.IOAddress ~ 361B;
colorRevAddr:    DoradoInputOutput.DMuxAddr  ~ 3107B;
colorDeviceData: MACHINE DEPENDENT RECORD[type: [0..17B], junk: [0..7777B]];

base0[csb] _ [mcb: RNIL];
colorBoardData _ DoradoInputOutput.InputNoPE[colorBoardAddr];
IF colorBoardData#0 THEN {          -- Color display board is installed
  colorDeviceData _ LOOPHOLE[DoradoInputOutput.InputNoPE[colorDeviceAddr]];
  SELECT colorDeviceData.type FROM
    17B => displayType _ standard;        -- 640x480
    16B => displayType _ highResolution;  -- 1024x768
    ENDCASE;                              -- unknown display type
  colorRevData _ DoradoInputOutput.RWMufMan[[useDMD: FALSE, dMuxAddr: colorRevAddr]];
  oldRev _ (colorRevData.dMuxData=1);
  };
```

So to make Cedar believe a colour display exists, three values must come
back:

1. **`Input` at TIOA 360B must be non-zero** -- "board is installed".
2. **`Input` at TIOA 361B**: `type` is the **top four bits** (`[0..17B]`
   first in a MACHINE DEPENDENT RECORD is Xerox bit 0..3), so `0170000`
   selects `standard` (640x480) and `0160000` selects `highResolution`
   (1024x768). Anything else leaves `displayType` at `none`.
3. **The muffler at DMux address 3107B**: `oldRev` is TRUE iff `dMuxData=1`.
   *"Old revision boards can't run both channels at the same time"*, so
   returning anything but 1 gets a modern board.

**Note `InputNoPE` -- Input with No Parity Error.** The driver expects a
floating bus when the board is absent, which is exactly what `io.c` returns
for an unregistered cell (0xFFFF, parity flagged). That has an accidental
consequence worth checking rather than assuming: 0xFFFF is non-zero, and its
top four bits are 17B, so an unmodelled read could look like *"a standard
640x480 colour board is installed"* by accident. `ColorDisplay ?` in the
2026-08-08 probe did report `off, 8 bpp, left, 640x480, Dither` -- but
640x480 is also the profile default, so that is not evidence either way
until the census run distinguishes them.

### And the real driving mechanism is the CSB, not these Outputs

The `MixerOutput` / `BMapOutput` / `CMapOutput` calls in the head are a
short clear sequence (`keep`/`load addr 0`/`write data 0`/`keep F` for each
of the three RAMs). The **continuous** work is done the same way the
monochrome side does it: the driver writes `base0[csb]` -- the ColorCSB at
**177414B** -- pointing at a MonitorControlBlock, and the **microcode** walks
that chain, exactly as DHT/DWT walk the mono DCB chain at 0420.

That reframes the emulator work. Decoding 361B/362B/365B gets the RAM loads;
**making a picture appear also needs the colour display task's IOFetch path**,
which is the same shape as the existing `DWT/AWT` fast-I/O path in
`fastio.c`. Scope accordingly: register decode is small, the channel fetch is
the real work.

## THE WHOLE PATH IS NOW KNOWN, END TO END (2026-08-08)

The last unknown -- "what makes Cedar believe a colour display exists" -- has
a period answer, and it is a **user typing a command**.

### The head is ALREADY in our booted system

`Cedar6.1/HeadsDorado/HeadsDorado.config` lists it in `CONTROL`:

```
CONTROL ProcessorHeadDorado, TextBltImpl, TerminalHeadDorado,
        ColorDisplayHeadDorado, DiskHeadDorado, DiskHeadSA4000,
        EthernetHeadDorado, EthernetOneHeadDorado = ...
```

and `HeadsDorado.bootmesa` marks it `RESIDENT`. So `ColorDisplayHeadDorado`
is compiled into the Dorado boot file and is running in our Cedar desktop
right now. It simply sits at `displayType _ none` -- *"display type, 'none'
if display not available"* -- until something calls `SetDisplayType`.

### `ColorDisplay` is a CedarChest package, and it is fetched

`[Cyan]<CedarChest6.1>Top>ColorDisplay.df!18`, mirrored into the served tree
by `tools/fetch_cedarchest_app.py ColorDisplay.df!18`: `ColorDisplayImpl.bcd`
(37 KB), the `ColorDisplay` client interface, `ColorDisplay.load` and
`ColorDisplayDoc.tioga`. Written by Mik Lamming and Ken Pier, maintained by
Tim Diebert; the doc is stamped **CEDAR 6.1**.

`ColorDisplay.load` is four lines:

```
Run ImagerGrayImpl
Run ImagerForkContextImpl
Run ImagerMaskContextImpl
Run ColorDisplayImpl
```

### And then the user just types it

From `ColorDisplayDoc.tioga`, verbatim:

| command | effect |
|---|---|
| `ColorDisplay` | toggle; on restores the state it had when last turned off |
| `ColorDisplay on` / `off` | unconditional |
| `ColorDisplay <n>` | on, with **n bits per point, n = {1,2,4,8,24}** |
| `ColorDisplay +<m>` | on, with m bits per point on the **B channel**, m = {0,1,2} |
| `ColorDisplay left` / `right` | colour display logically left/right of the b/w display |
| `ColorDisplay default` | the mode described by the profile |
| `ColorDisplay gray` / `dither` | 8bpp non-dithered / dithered |
| `ColorDisplay ?` | report the mode |

There is also **a `Color` button in the message window**: *"Left-click to turn
the color display on and off. Middle-click to cause the color display to
sleep for a while. Right-click for more options, and to see the current
state."*

And it is configurable from the machine profile -- the same mechanism our
`DoradoWelcome` profile already uses:

```
ColorDisplay.Side:         [left/right]
ColorDisplay.BitsPerPoint: [1,2,4,8,24]
ColorDisplay.Type:         [1024x768, 640x480]
ColorDisplay.Gray:         [TRUE/FALSE]
ColorDisplay.BitsPerPointB: [0,1,2]
```

`ColorDisplay.Type` matches `SetDisplayType`'s two rasters exactly, which
closes the loop from the profile down to the hardware.

**"left" and "right" settle the two-monitor question for good.** The colour
display is a second physical screen and Viewers places windows on one side or
the other -- exactly as the netlist (DispY mono + DispM with three DACs) and
the Hardware Manual (doc p.110) say.

### Historical implementation path — now completed

1. `ColorDisplay.load` and `ColorDisplay on` were run from the Cedar
   colour cold-boot/bake path.
2. The three DispM registers, ColorCSB chain, ATable and RGB output are
   implemented in `src/dispm.c`.
3. SDL presents a second colour window; the browser presents a combined
   canvas with Both / Color / Monochrome controls.

The historical trace plan remains useful when validating new guest software,
but it is no longer an unimplemented emulator task.

## And GARGOYLE 6.1 is already in the served tree, with a ColorTool

The illustrator in the Cedar 13 screenshot has a 6.1 ancestor, and it was
mirrored here at some earlier point: `chm/cedar/stp-root/CedarChest6.1/
Gargoyle/`, **253 files**, with `Gargoyle.bcd` and `Gargoyle.load`.

`Gargoyle.load` is signed *Pier, October 31, 1986* and *Bier, January 14,
1987* -- Eric Bier, whose toolglass paper is the document open in that
screenshot -- and it ends:

```
Install PopUpSelection
Install BiScrollersButtonned
Install SlackProcess
Run InterpressPackage.bcd CubicSplinePackage.bcd TiogaButtonsImpl.bcd
    NamedColorsImpl.bcd ImagerMemoryImpl.bcd ImagerArtworkImpl.bcd
    GListImpl.bcd MathPackage.bcd Draw2dPackage.bcd
Run Gargoyle.bcd
-- Unbound imports are OK. Don't worry.
Run BasicCombiner.bcd ColorTool.bcd
```

**Current result (2026-08-09): Gargoyle launches.** Paul’s
`MASTER-web-2021_08` export supplied the missing 6.1 BiScrollers payload. The
served `BiScrollers.BCD` has CRC `ace20fa6` and header stamp `7f/89/f267cae2`,
matching Gargoyle’s import table. Cedar can now run Gargoyle from the
colour-enabled checkpoint; the remaining validation is to exercise an actual
drawing and capture it as a repeatable colour smoke test.

**`NamedColorsImpl` and `ColorTool`.** So the demo that would look most like
what Tim remembers is not Griffin but **Gargoyle with its ColorTool**, on a
colour display enabled by `ColorDisplay on` -- which is precisely the pairing
the Cedar 13 screenshot shows, a decade later and on X11.

Griffin stays useful as the simpler target (fewer packages, and its
`GriffinColor` palette is 30 fixed HSV entries, easy to recognise in a
trace); Gargoyle is the one to demo.

## What Cedar 13 shows, and why it is NOT our case (2026-08-08)

A screenshot arrived of **Cedar 13.0.79 from sylvester.parc.xerox.com**,
with obvious colour on screen: Gargoyle's palette of swatches and a purple
triangle, beside a Tioga viewer holding Eric Bier's toolglass/magic-lens
SIGGRAPH '93 paper.

That is **PCedar** -- Portable Cedar -- and the giveaways are in the picture:
the title bar says `X11Viewers`, and the CommandTool is running
`/usr/ccs/bin/ld` over `/project/cedar13.0/versions/gargoyle/sunos$/`. It is
Cedar compiled for **SunOS, displaying through X11**, roughly a decade after
the machine we emulate.

**Why the difference matters here.** On PCedar the whole screen is a colour
frame buffer because X11 gives it one; colour is not a separate device and
there is no DispM. On a **Dorado**, the main screen is DispY -- 1024x808,
**one bit per pixel** -- and colour is a physically separate board driving a
second monitor at 640x480 or 1024x768. That is not an emulator limitation; it
is what the hardware was. So a colour Cedar 6.1 desktop is not a thing that
existed: the desktop was monochrome and colour was where the illustrations
went.

Which the screenshot itself supports. Even in Cedar 13, the text editor, the
CommandTool and the icons are black on white; the colour is in **Gargoyle**,
the illustrator -- the successor to Griffin, and the same role Griffin plays
in 6.1. So the target is right even though the display architecture differs:
**colour on a Dorado means an illustration program on a second monitor.**

Worth flagging for anyone scoping from Tim's memory of the system: if his
book was produced on PCedar, he would remember a colour screen that a Dorado
never had. Ask which machine before promising a match.

## [x] THE DRIVER IS IN OUR SERVED TREE (2026-08-08) -- colour is now SPECIFIED

Fetching CedarGriffin led somewhere better than Griffin. Chasing what
`GriffinColor.mesa` actually calls -- `ImagerColor.ColorFromRGB` /
`RGBFromHSV` over a 30-entry HSV palette ("black", "dk brown", ... "white") --
runs to `TerminalExtras.SetColorArray` / `SetRedMapArray` / `SetGreenMapArray`
/ `SetBlueMapArray`, then to `ColorDisplayDefs`, and lands on

**`chm/cedar/stp-root/Cedar6.1/HeadsDorado/ColorDisplayHeadDorado.mesa`
(22 KB) and `ColorDisplayDorado.mesa` (6.6 KB)** -- Xerox's own DispM driver
and its register definitions, for the Dorado, in Cedar 6.1. **We have been
serving these to the guest all along.** The colour board does not have to be
reverse-engineered from the schematics; it can be read off the driver.

### The registers (`ColorDisplayDorado.mesa`, Doug Wyatt, 1-May-1985)

```
mixerRegister: DoradoInputOutput.IOAddress ~ 361B;
cMapRegister:  DoradoInputOutput.IOAddress ~ 362B;
bMapRegister:  DoradoInputOutput.IOAddress ~ 365B;
```

These are the three TIOAs `docs/color-graphics-todo.md` had guessed at, now
confirmed from the driver. **We decode none of them** -- `DORADO_DDC_TIOA=1`
would show them as UNDECODED.

### The control-block chain, exactly like the mono DCB chain at 0420

```
csb: -> ColorCSB ~ 177414B          -- colour controller status block
  mcb -> MonitorControlBlock (5 words, seal = 177456B)
           flags: m/vc/hc/clk/a/b/c -- which tables to (re)load
           channelA -> ChannelControlBlock (8 words)
           channelB -> ChannelControlBlock
           color    -> ColorControlBlock (16 words)
                         tableA -> ATable   (1024 entries x 2 words)
                         tableB -> BCTable  (256 entries)
                         tableC -> BCTable
                         miniMixer -> MiniMixerTable (256)
                         vc/hc/clk -- vertical, horizontal, clock control
```

**`mcbSeal = 177456B` is already in our source.** `machine.c:5429` tests
`seal == 0177456u` in the Interlisp colour probe, written in an earlier
session from the Lisp side. Cedar's driver defines the same constant -- which
is the strongest kind of confirmation, because it means both operating
systems are describing one piece of hardware, not each their own convention.

### Pixel formats and geometry

`ChannelControlBlock.scanControl` gives `mode24` (24 bits/pixel),
`aChannelOnly`, `bBypass`, `pixelMode` (`a6b4` or `a8b2` -- the `A8B2` signal
the netlist shows on BOTH display boards), `resolution`
(quarter/half/full) and `bitsPerPixel` (**must be 1, 2, 4 or 8**).

`ATable` entries are two words carrying **8 bits each of red, green and
blue** (red split `redL`/`redH` across the words) -- i.e. 24-bit colour,
which is what the three DACs on DispM are for.

`SetDisplayType` in the head gives the two supported monitors:

| type | raster | pixels/inch | clock mul/div |
|---|---|---|---|
| `standard` | **640 x 480** | 42 | 130B / 14B |
| `highResolution` | **1024 x 768** | 72 | 54 / 14 |

So the colour monitor is a *separate* raster from DispY's 1024x808 mono
screen, confirming the two-monitor reading of the netlist and the manual.

### The RAM-load protocol, in the driver's own words

`MixerDatum` and `BCDatum` are `keep` / `write` / `load` bits plus a variant
body that is either an address or data -- exactly the "two address sources
with a `Keep'` flipflop" handshake described in section 1.4 and in
`docs/io-systems-architecture.md`. Now with the actual bit positions.

### What this changes

Section 1 is now transcription rather than a proposal: `src/dispm.c` decodes
361B/362B/365B, walks the CSB chain at 177414B, and renders `ATable` through
to RGB. Cedar's cold-boot colour checkpoint proves the guest detection and
the complete path. The remaining work is a stable colour gate and the Koto
Lisp validation described in Step 1B.

The guest-detection blocker is closed: `ColorDisplayHeadDorado.mesa` detects
the installed board during cold boot, and the colour checkpoint preserves the
resulting ColorCSB plus its matching PDI. The remaining work is a stable
colour gate and the Koto Lisp validation described in Step 1B.

## [x] 1. DDC RAM loads (Mixer / BMap / CMap)

Implemented in `src/dispm.c`; the keep/load/write handshakes and ATable
addressing are cross-checked against the Cedar driver and the DispM netlist.

- Acceptance: replay `ColorDisplay.mc`'s `DoSomeTable` loop and land the
  right bytes at the right addresses; **read back a written entry through
  the STATUS input** and get it back (§1.6).
- Watch: the Mixer's two-halves-per-entry addressing.

## [x] 2. Monitor type / colour monitor presence

Implemented behind `DORADO_DISPM_COLOR=1`; cold boot reports the board and
latches the standard 640x480 monitor type. A colour checkpoint must be baked
from a colour cold boot because detection occurs during module initialization.

- Acceptance: the guest's colour init proceeds past its monitor check.

## [x] 3. Colour framebuffer — the invasive one

Today the framebuffer is **1 bit per pixel, packed** (`fb[byte] |= 1<<bit`),
128 bytes/row × 808. `display.c`, `dorado.c` (PGM writer), `dorado_sdl.c`,
`dorado_web.c` and **every gate** assume that — the gates count
"display-list pixels" out of that bitmap.

Implemented as a separate RGB buffer and `.color.ppm` output. The mono
framebuffer remains independent.

- Acceptance: Galaxian, Cedar desktop and Lyric all **byte-identical**;
  `make test`, `verify-lisp`, `verify-cedar-desktop` unchanged.

## [x] 4. Item unpacking at 1/2/4/8 bpp

Implemented in the colour display path; guest validation of the old Lisp
4/8-bit modes remains open.

## [x] 5. The mixer itself

A/B items → 24-bit RGB per the mode bits, including the AMap/BMap/CMap
colour path. The Koto test should still verify the historical 4/8-bit modes.

## [x] 6. Frontends and output

SDL second window, browser second canvas, and PPM (P6) colour snapshots are
implemented.

## [ ] 7. A colour gate

The existing gates are monochrome pixel counts and cannot see colour.
Needs its own guest-driven acceptance test. Prefer Koto's
`\\DORADO\\STARTCOLOR` plus a stable 4/8-bit image if Koto can be brought up;
keep `COLORDEMO` or Gargoyle as a second Cedar-side smoke test.

---

# PART 3 — TEST PLAN

The current test candidates are:

1. **Deferred primary test:** Koto Lisp plus
   `[phylum]<LISP>KOTO>Library>DORADOCOLOR!1` and
   `\DORADO\STARTCOLOR` (Nick's lead).
2. `make run-lisp-lispusers-sdl` — Lyric already declares a DispM board.
3. `(FILESLOAD DORADOCOLOR)` → `\DORADOCOLOR.STARTCOLOR` → `COLORDEMO`.

That is an 8-bpp screen with an AMap the guest loads itself — exactly the
24Bit/BBypass path.

The Cedar candidate is now demonstrated: Cedar 6.1 detects the board on a
cold boot, `ColorDisplay on` arms the ColorCSB, and Gargoyle produces a live
colour frame. The ordinary 6.1 desktop remains monochrome by design; colour
is an optional second monitor.
**When this work resumes, start by identifying Koto's matching sysout; do
not silently substitute Lyric.** If Koto cannot be made to boot, fall back
to Lyric and Cedar while documenting the version boundary.

---

# PART 4 — RISKS

- **The emulator path runs, but no period Lisp colour path has yet been
  reproduced here.** Koto is the next historical candidate; do not treat a
  Cedar/Gargoyle screenshot as proof that the Lisp hookup works.
- **High-resolution geometry is unverified.** The standard 640×480 colour
  raster is validated in SDL and WebAssembly beside the 1024×808 mono raster;
  the 1024×768 DispM mode still needs its own guest-driven check.
- **No known-good screenshot to compare against.** Unlike the Cedar and
  Lisp bring-ups there is no oracle — "looks plausible" is all we get for
  a while, which argues for leaning hard on `ColorDisplay.mc` and the
  schematics as the spec.
- **The gates are monochrome**, so a colour regression is invisible to
  them until task 7 exists.
