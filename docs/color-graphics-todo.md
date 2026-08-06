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

## 1.1 The one fact that makes this tractable

**There is no separate colour board.** The same DDC drives mono and
colour; colour is a *mode*. So this is not "emulate new hardware", it is
"finish the display controller we already have".

## 1.2 Source material and where it lives

| what | where |
|---|---|
| **Board schematics** | `DoradoDocs/doradodrawings/DispM-apcRev-Da.press!1.pdf` — 32 sheets, K. Pier, Nov 1982. Four earlier revisions beside it (Cf, Cg, Ch, mwRev-Ch). **Do not use** `DoradoDocs/schematics/DispM-apcRev-Da.press!1.pdf` — it is a degraded 164 KB rendering of the same file; the `doradodrawings/` copy is 1.7 MB. |
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

## [ ] 0. Trace whether any world actually programs the colour RAMs

**Do this before promising anything.** Everything below assumes the guest
will drive the hardware once we model it, and that is unchecked. Log
`Output←B` at TIOA 361/362/365 and watch for the colour DHT entry
(`COLORVSTOVSINIT`).

- Cheap: no code beyond a trace flag; hours not days.
- **If they never fire**, something must configure a colour monitor first
  (see 1.6) and the job is bigger than the rest of this list assumes.
- Acceptance: a yes/no answer for Lyric and for the Cedar desktop.

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
