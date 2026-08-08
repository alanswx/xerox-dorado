# Colour graphics — TODO and reference

Everything gathered 2026-08-06 about adding the Dorado colour display, in
one place: the reference material first, then the work list.

**Status: nothing implemented.** The control writes already reach our
display device and are dropped. Step 0 below decides how big the rest is.

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

## [ ] 1. DDC RAM loads (Mixer / BMap / CMap)

Implement the §1.4 protocol at the `display.c` TODO. Store into the
existing `mixer[]` plus new BMap/CMap arrays.

- Acceptance: replay `ColorDisplay.mc`'s `DoSomeTable` loop and land the
  right bytes at the right addresses; **read back a written entry through
  the STATUS input** and get it back (§1.6).
- Watch: the Mixer's two-halves-per-entry addressing.

## [ ] 2. Monitor type / colour monitor presence

Answer the STATUS inputs with an MType that says "colour monitor
attached", behind a config knob so mono worlds are unaffected.

- Acceptance: the guest's colour init proceeds past its monitor check.

## [ ] 3. Colour framebuffer — the invasive one

Today the framebuffer is **1 bit per pixel, packed** (`fb[byte] |= 1<<bit`),
128 bytes/row × 808. `display.c`, `dorado.c` (PGM writer), `dorado_sdl.c`,
`dorado_web.c` and **every gate** assume that — the gates count
"display-list pixels" out of that bitmap.

**Add a second RGB buffer** used only when a colour mode is programmed;
leave the mono `fb` untouched. That keeps every mono world bit-identical
by construction.

- Acceptance: Galaxian, Cedar desktop and Lyric all **byte-identical**;
  `make test`, `verify-lisp`, `verify-cedar-desktop` unchanged.

## [ ] 4. Item unpacking at 1/2/4/8 bpp

Honour `αItemSize` and `αResolution` in the FIFO → framebuffer path
(today: 16 mono pixels per fetched word).

## [ ] 5. The mixer itself

A/B items → 24-bit RGB per the mode bits. **24Bit is the mode Lisp's
8 bpp default wants.**

## [ ] 6. Frontends and output

SDL texture format, browser canvas, and PPM (P6) instead of greyscale PGM
for colour snapshots.

## [ ] 7. A colour gate

The existing gates are monochrome pixel counts and cannot see colour.
Needs its own, with `COLORDEMO` as the acceptance test.

---

# PART 3 — TEST PLAN

Everything needed is already on our disk:

1. `make run-lisp-lispusers-sdl` — Lyric already declares a DispM board.
2. `(FILESLOAD DORADOCOLOR)` → `\DORADOCOLOR.STARTCOLOR` → `COLORDEMO`.

That is an 8-bpp screen with an AMap the guest loads itself — exactly the
24Bit/BBypass path.

Cedar is the second candidate (`Cedar.mb` has the microcode, CedarChest 6.0
ships `SilColor.bcd`), but the shipped 6.1 desktop is configured for the
mono LF monitor and would need a colour monitor configured first.
**Start with Lisp.**

---

# PART 4 — RISKS

- **Nobody has seen this path run.** Step 0 exists for this reason.
- **Geometry is unverified.** Our framebuffer is 1024×808 mono; a colour
  raster at 8 bpp is a different width in words and may want different
  dimensions. The `active_w`/`active_h` per-world mechanism should extend,
  but the numbers are unchecked.
- **No known-good screenshot to compare against.** Unlike the Cedar and
  Lisp bring-ups there is no oracle — "looks plausible" is all we get for
  a while, which argues for leaning hard on `ColorDisplay.mc` and the
  schematics as the spec.
- **The gates are monochrome**, so a colour regression is invisible to
  them until task 7 exists.
