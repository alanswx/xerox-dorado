# What it would take to add the colour display

Scoping pass, 2026-08-06. No code written. **Verdict: genuinely doable, and
better supported by our archives than expected — the hardware is fully
documented, the microcode is in hand AND already inside two of the worlds
we boot, the control writes already reach our display device, and there is
period software on our own Lyric pack to test it with.**

The work is real but bounded: roughly five pieces, one of which (the
framebuffer format) touches every frontend and every gate.

---

## 1. What the hardware is

There is no separate colour board. **The same DDC drives mono and colour** —
colour is a *mode* of the display controller, which makes this far smaller
than it sounds. From HM §11 and `docs/display-architecture.md`:

- Items are pulled from the per-channel FIFO at the item rate and routed
  through **AMap/BMap/CMap** colour RAMs into three 8-bit DACs (RGB).
- **αItemSize** selects 1 / 2 / 4 / 8 bits per pixel (unary: `Size1`=1,
  `Size2`=2, `Size4`=4, `Size8`=10₈).
- **Mixer modes** (`DisplayDefs.mc`): `A8B2Mode`=1, `BBypassMode`=2,
  `ABypassMode`=4, `24BitMode`=12₈ (which is 24Bit OR BBypass).
  - A6B4 (default): 10-bit mixer address = `AItem[0:5] || BItem[0:3]`
  - A8B2: `AItem[0:7] || BItem[0:1]`
  - BBypass: B skips the mixer into a separate 256×8 BMap, OR'd into blue
  - 24Bit: A alternately addresses AMap and CMap, B addresses BMap at half
    rate → AMap red, BMap blue, CMap green. True colour.

Sizes: **Mixer/AMap 1024 entries × 24 bits** (2 words each — the microcode
loads 0o3776 = 2046 = 2048−2 words), **BMap and CMap 256 × 8** (0o376 =
254 = 256−2).

## 2. What we already have

This is the encouraging part.

| | status |
|---|---|
| Hardware reference | HM §11 distilled in `docs/display-architecture.md`, incl. CLCB/NLCB fields, mixer modes, 24-bit mode |
| **Board schematics** | **`DispM-apcRev-Da.press!1.pdf`, 32 sheets, K. Pier, Nov 1982** — pages 12–31 are the Mixer drawings: ABuf/BBuf/CBuf, BMap, CMap, mixer address drivers and control, the Blue/Red/Green byte slices, output register, DACs, PLL, block diagram, and the Slow IO formats. Five revisions held (Cf, Cg, Ch, mwRev-Ch, apcRev-Da) in `DoradoDocs/doradodrawings/`. NOTE the copy in `DoradoDocs/schematics/` is a degraded 164 KB rendering — use the 1.7 MB `doradodrawings/` one. |
| Microcode source | `chm/doradomicrocode/doradomicrocodesources/ColorDisplay.mc!1` (Taft, Sept 1981), complete |
| Device numbers | `DisplayDefs.mc!1`: **Mixer 361₈, CMap 362₈, BMap 365₈** on the DispM board |
| Colour code in the worlds | `Mesa.mb!3` and `Cedar.mb!6` both contain `COLORCTRLBLKPTR`, `COLORVSTOVSINIT`, `PROCESSAMAP`, `PROCESSCMAP`. (`AEmu.mb` does not — the Alto world has no colour.) |
| The writes reach us | our display already claims TIOA `0360..0377`, so Mixer/BMap/CMap `Output←B` **already arrive** — and are dropped on the floor at the `TODO` in `display.c` (~line 652) |
| "DispM installed" | already modelled: `display_dispm_present()` answers DisplayInitConfig with bit 0 set, driven by `DORADO_DISPM_PRESENT`, which the **Lyric world already sets** |
| Test software | **`DORADOCOLOR.LCOM` is already on our Lyric pack**, plus `COLORDEMO.LCOM`, `LLCOLOR`, `DICOLOR`, `SKETCHCOLOR`, `COLOR` |

`DORADOCOLOR.LCOM` is Interlisp-D's Dorado colour driver and it tells us
exactly what the guest will do: `DORADOCOLOR.BITSPERPIXEL` defaults to
**8**, `\DORADOCOLOR.LEFTMARGIN` 80, `\DORADOCOLOR.ATABLEIMAGE` is the AMap
image it loads, `\RGB.TO.DORADO.RGB` converts colours, `\DORADOCOLOR.STARTCOLOR`
starts it, and the colour control block is `(ColCBLow 176) (ColCBSize 16)
(CMapPages 8)`.

## 3. What is missing

Five pieces. Only the fourth is invasive.

**(a) The DDC RAM-load protocol — and we have the bit-level spec.**
`DoradoDocs/doradodrawings/DispM-apcRev-Da.press!1.pdf` page 29 ("Slow IO
system formats", K. Pier, 7/15/81) gives the exact word layouts, and they
agree with `DisplayDefs.mc` bit for bit. Bits are numbered MSB-first, and
the control bits are ACTIVE LOW (the schematic primes them), which is why
the microcode constants are named *Dont*Keep / *Dont*Write:

| device | TIOA | format (bit 0 = MSB) |
|---|---|---|
| MIXER | 361₈ | 0 `Keep'` · 1 `Write'` · 2 `LoadAddr` · 3 x · 5–14 `Addr.0-9` · 15 `Hi/Lo select` — **or** bits 4–15 = 12 bits of Mixer data |
| BMap | 365₈ | 0 `Keep'` · 1 `Write'` · 2 `LoadAddr` · 3 x · 8–15 `Address.0-7` **or** `Data.0-7` |
| CMap | 362₈ | same as BMap |
| PIXELCLK | 360₈ | 4–11 Pixel Clock Rate · 12–15 Clock Divider |

Cross-check against `DisplayDefs.mc` (`RamCtrlShift` = 12): `DontKeep` =
10₈<<12 = 0x8000 = bit 0 = `Keep'`; `DontWrite` = 4<<12 = 0x4000 = bit 1 =
`Write'`; `LoadAddress` = (2<<12)|DontWrite = bit 2 + bit 1. **Exact
agreement**, which is about as good as a spec gets here.

Note the Mixer's 24 bits are written as **two 12-bit halves** selected by
bit 15, over 1024 addresses (Addr.0-9) — consistent with the microcode
loading 2048−2 words. BMap and CMap are 256×8, one word each.

Sequence: one Output with `LoadAddr` to set the address, then a stream of
data words auto-incrementing, then `ReleaseRam` (DontKeep|DontWrite).
Small, self-contained, testable against `ColorDisplay.mc`'s `DoSomeTable`.

**(b) Item unpacking at 1/2/4/8 bpp.** Today the DWT path lays 16
monochrome pixels per fetched word. It needs to honour `αItemSize` and
`αResolution` (full/half/quarter pixel-clock rate). Contained in the FIFO →
framebuffer path.

**(c) The mixer.** Map A and B items to 24-bit RGB per the mode bits. The
four modes above; 24Bit is the one Lisp's 8-bpp default will want.

**(d) A colour framebuffer — the invasive one.** Today it is 1 bit per
pixel, packed (`fb[byte] |= 1<<bit`), 128 bytes per row × 808. Every
consumer assumes that: `display.c`, `dorado.c` (the PGM writer),
`dorado_sdl.c`, `dorado_web.c`, and **every gate**, which counts
"display-list pixels" out of that bitmap.

The safe shape is a *second* buffer rather than a reinterpretation: keep
the mono `fb` exactly as it is, add an RGB buffer used only when a colour
mode is actually programmed, and have the frontends prefer it when
non-empty. That keeps every mono world bit-identical by construction —
which matters, because the pixel-count gates are the regression net for
five worlds.

**(e) Frontends and output.** SDL texture format, the browser canvas, and
the snapshot writer (PGM is greyscale; colour needs PPM/P6). Plus a colour
equivalent of the pixel-count gate.

## 4. How to test it

The path exists today, on hardware we already emulate, with software
already on our disk:

1. Boot Lyric (`make run-lisp-lispusers-sdl`) — it already declares a DispM
   board via `DORADO_DISPM_PRESENT`.
2. `(FILESLOAD DORADOCOLOR)` then `\DORADOCOLOR.STARTCOLOR`, then
   `COLORDEMO`.

That gives a self-contained target: an 8-bpp screen with an AMap the guest
loads itself, which is exactly the 24Bit/BBypass path.

Cedar is the second candidate — `Cedar.mb` carries the microcode and
CedarChest 6.0 ships `SilColor.bcd` — but the shipped 6.1 desktop is
configured for the mono LF monitor, so it would need a colour monitor
configured before anything is visible. **Start with Lisp.**

## 5. Risks and unknowns, honestly

- **Nobody has seen this path run.** The colour microcode is present in the
  worlds but we have no evidence any world we boot has *executed*
  `COLORVSTOVSINIT`. The first task is a cheap trace: watch for writes at
  TIOA 361/362/365 and for the colour DHT entry point. If they never fire,
  the work is bigger than this document assumes, because something has to
  configure a colour monitor first.
- **How the guest discovers a colour monitor — ANSWERED by the schematic.**
  DispM page 28 ("DDC to DDM Interface") shows `MType.0/1/2` as a
  **monitor type field set by jumper resistors on the backplane**, and page
  29 shows it read back through the STATUS *inputs*: TIOA 361 in =
  `MType.0-3, Green.0-7, Red.0-3`, TIOA 360 in = `Keyboard,1,1,1,
  Red.4-7, Blue.0-7`. So making a colour monitor "present" means answering
  those two input registers with the right MType — a small, concrete
  change, not an unknown. (The same reads hand back DAC values, which is
  also a free self-test: write a mixer entry, read it back.)
- **Geometry.** Our framebuffer is 1024×808 mono. A colour raster at 8 bpp
  is a different width in words and may want different dimensions; the
  `active_w`/`active_h` mechanism already exists for per-world rasters and
  should extend, but the sizes are unverified.
- **The gates are monochrome.** Every existing gate counts bits in the mono
  buffer. Colour needs its own gate, and the mono ones must be proven
  unchanged (byte-identical) after the framebuffer work.
- **This is emulation of an option nobody has exercised in 40 years.**
  Unlike the Cedar/Lisp bring-ups, there is no known-good screenshot to
  compare against. "Looks plausible" will be the only oracle for a while,
  which argues for leaning hard on the microcode source as the spec.

## 6. Rough shape of the effort

Ordered so each step is verifiable before the next:

1. **Trace first** (hours). Prove or disprove that any world programs the
   colour RAMs. Cheap, and it decides everything else.
2. **RAM loads** (a) — small, testable against `ColorDisplay.mc`.
3. **Colour framebuffer** (d) as a second buffer, with the mono path
   proven byte-identical and one frontend showing it.
4. **Item unpacking + mixer** (b, c) — the real emulation work.
5. **A colour gate**, and the Lisp `COLORDEMO` as the acceptance test.

Step 1 is the one to do before promising anything: everything above assumes
the guest will drive the hardware if we model it, and that assumption has
not yet been checked against a running world.
