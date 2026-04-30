# Dorado display architecture — reference

Detailed reference for HM §11 (Display Controller). Companion to
`docs/io-systems-architecture.md` (high-level I/O overview) and
`dorado/include/display.h` / `src/display.c` (current implementation).

Sources: HM §11 pages 109–120, plus the IFU/Memory chapters where
the DDC's Fast-I/O path is described, and Figure 14/15.

## At a glance

```
   ┌───────────────────────────────────────────────────────────────┐
   │  Microcode tasks                                               │
   │   DHT  task 3   — Display Horizontal Task (low priority)       │
   │     once per scan line: compute next-line params, load NLCB    │
   │     for both channels, set αNextWCBFlag to release DWT         │
   │   DWT  task 13₈ — Display Word Task (high priority)            │
   │     issues IOFetch← per munch; per-channel via SubTask          │
   │     0 = channel A, SubTask 2 = channel B                       │
   │   AHT  task 4   — DispM terminal interface horizontal task     │
   │   AWT  task 11₈ — DispM terminal interface word task           │
   └───────┬──────────────────────────────────────────────┬─────────┘
           │ slow IO (NLCB/HRam/Mixer/Statics/PixelClk)   │ fast IO
           ▼                                               ▼
   ┌──────────────────────┐                       ┌──────────────────┐
   │  DDC (Display Data   │  16-word munch FIFO   │  Fin / Fout       │
   │       Channel)       ├◄──────────────────────┤  18-bit data busses│
   │   • CLCB (current)   │                       └────────┬──────────┘
   │   • NLCB (next)      │                                │
   │   • HRam (1K × 3 bit)│                                ▼
   │   • Mixer (1K × 24)  │                       ┌──────────────────┐
   │   • Color maps       │                       │  Main storage    │
   │   • Cursor sprite    │                       └──────────────────┘
   │   • PixelClk         │
   │   • Statics          │
   └─────────┬────────────┘
             │
             ▼
   ┌────────────────────────────────────┐
   │  Mixing stages → DACs → monitor    │
   │  (binary or grey/color via Alto    │
   │  video iface, MiniMixer→7-wire     │
   │  iface, or DispY MiniMixer)        │
   └────────────────────────────────────┘
```

The DDC pulls pixel data from main storage via Fast I/O (no processor
in the data path), formats it through configurable mixers, and emits
video to one of three monitor interfaces:

- **Alto-style** (binary monitor, single DAC, obsolete by 1981).
- **DispM** (the original Dorado mono monitor on its own board).
- **7-wire** (modern shared-cable interface; DispY board).

## Geometry

The Dorado is monitor-agnostic — the DDC is fully programmable. The
**Alto monitor** (the most common target) is **808 × 606 mono**:

| Property              | Value                                 |
|-----------------------|---------------------------------------|
| Pixels per scan line  | 808                                   |
| Scan lines per frame  | 606                                   |
| Bytes per scan line   | 101 (= 808/8, MSB = leftmost)         |
| Bytes per frame       | 61,206                                |
| Refresh rate          | ~60 Hz interlaced (or 30 Hz progressive) |
| Pixel clock           | ~20 MHz (50 ns/pixel) typical         |

For higher-resolution monitors (LF "large format" / Star), the same
DDC drives larger frames; the muffler signal at MufAddr=106 reports
MonitorType (0 = Alto, 1 = LF).

## Data path: storage → FIFO → Mixer → monitor

1. **DHT** computes the start address and word count of the next
   scan line, writes them into `αNextAddr` / `αNextCount` (per-channel
   RM registers), updates NLCB, sets `αNextWCBFlag`.
2. **DWT** wakes at the start of HBlank, dequeues `αNextAddr`, and
   issues `IOFetch←` per munch. Each munch (16 words) drops into the
   per-channel FIFO via the Fin bus.
3. The DDC's serial-input register (SIB) pulls items from the FIFO
   at the **item rate** (= pixel clock / αItemSize × αResolution).
4. The mixer routes items to one of two channels (A/B), through
   AMap/BMap/CMap color RAMs, into the three 8-bit DAC output
   registers (RGB).
5. DACs drive the monitor.

## CLCB / NLCB fields (HM page 113–114)

The DDC has two 16×12-bit RAMs: **CLCB** (Current Line Control Block)
controls video for the line currently being drawn, **NLCB** (Next
Line Control Block) is loaded by DHT for the line after that. At the
start of HBlank, CLCB ← NLCB (per-channel).

Channel-specific fields use the Greek prefix `α` (i.e., AItemSize and
BItemSize are both legal):

| Field         | Bits | Description |
|---------------|------|-------------|
| αPolarity     | 1    | Inverts black/white. APolarity OR BPolarity drives the DAC. |
| αResolution   | 2    | 0/2/3 = ¼ / ½ / full pixel-clock rate. |
| αItemSize     | 4    | Unary: 1 / 2 / 4 / 8 bits per pixel. Setting multiple bits is illegal. |
| αLeftMargin   | 12   | Pixel clocks to wait after HWindow before turning channel on. Loaded as `-(B + L − 63)` (or `-(B + L − 68)` for 7-wire). |
| αWidth        | 12   | Pixel-clock counter; channel turns off when it expires. Load `-(W + 255)` to run W pixels. |
| αFifoAddr     | 8    | Munch+word pointer for the first FIFO read of the next line. Even (doubleword fetched). |
| MixerModes    | 8    | Mixer mode bits — A8B2 / BBypass / 24Bit. *Not* channel-specific. |
| VCW           | 3    | Vertical Control Word: VBlank, VSync, OddField. |
| Cursor        | 16   | Cursor sprite row (16 bits = 16 cursor pixels). |
| CursorX       | 12   | X-position counter load: `-(B + X + 226)` or `-(B + X + 221)` for 7-wire. |

Programmer-visible packing within the 16×12-bit NLCB RAM is laid out
in HM Figure 15.

## Mixer modes (HM page 112)

Three orthogonal modes select how items reach the DACs:

- **A8B2 mode**: 10-bit Mixer address = `Altem[0:7] || BItem[0:1]`.
  Channel A drives the high 8 bits of the mixer index, channel B
  the low 2.
- **A6B4 mode** (A8B2 false, default): 10-bit address =
  `Altem[0:5] || BItem[0:3]`. 6-bit A + 4-bit B.
- **BBypass**: B's bits skip the mixer and address a separate 256×8
  RAM (BMap), whose output is OR'd with the blue DAC's mixer
  output. Lets the controller act as three 4-bit channels driving
  three color guns.
- **24Bit**: Combined with BBypass. A items alternately address the
  Mixer (now called AMap in this mode) and a 256×8 CMap. B runs at
  half the A rate and addresses BMap. AMap → red, BMap → blue,
  CMap → green. Used to drive a true-color monitor.

## Cursor (HM page 114)

The 12-bit `CursorX` is loaded into a counter that starts counting
at the end of HWindow. When the counter expires, the 16-bit
`Cursor` value is shifted out onto the `CursorVideo` line. Used by
both the Alto video interface and the MiniMixer.

## Horizontal waveform generator (HM pages 114–115)

A 1024-word × 3-bit `HRam` controls the HSync / HBlank / HalfLine
waveforms.

- HRam is addressed by a 12-bit counter `HRamAddr[0:11]`.
- The counter resets at the leading edge of HSync.
- It increments every pixel clock until the next leading edge.
- `HRamAddr[1:10]` (= 10 bits) addresses the 1024-word RAM. The output
  is loaded into `HRamOut` every other pixel clock.
- Three bits in HRamOut: HSync, HBlank, HalfLine. They're combined
  and level-shifted by a logic network specific to the monitor.

The 1024-word HRam means there are at most 2048 pixels per scan line.

## Vertical waveform generator (HM page 115)

Only 2:1 interlaced monitors are supported. The HRam contains a
`HalfLine` bit at the horizontal frequency, 180° out of phase with
HSync. Vertical control is via NLCB's `VCW`:

- **VBlank**: vertical blank during this line.
- **VSync**: enables vertical sync to begin on the next line.
- **OddField**: chooses HSync (= 0) vs HalfLine (= 1) phasing for
  vertical sync. OddField=1 is the half-line phase, used for
  vertical-sync alternation between fields.

DHT must keep track of scan-line counts to enable vertical signals
at the right times.

## Pixel clock (HM page 115)

The DispM board has a programmable pixel clock. Frequency:

```
     312.5 × (241 − M)
f =  ─────────────────  KHz   where M = PixelClk[4:11], D = PixelClk[12:15]
         16 − D
```

Stabilizes ~½ second after the PixelClk register is loaded.

The 7-wire interface adds a synchronization bound: pixel-clock
period must be at least `(3·C + 12.4) / 4` ns where C is the Dorado
clock period (= 25.6 ns at 30 ns Dorado clock). 10–50 MHz pixel
clocks (100–20 ns/pixel) are anticipated to be sufficient for any
practical monitor.

If `UseFakePClk` is true (in Statics), the regular pixel clock is
disabled and a `FakePClk` toggle drives the DDC at diagnostic-rate
single-stepping speed. Used for muffler-driven debug.

## 7-wire video interface (HM pages 116–117)

Six pairs carry digital video and sync from DDC to terminal; one
pair (the "back channel") carries serial input data from the
terminal back to the Dorado.

### Video output (4 lines, interpreted per ClkB)

- Two clock pairs: ClkA, ClkB.
- ClkA falling edge: DDC places data on the four output lines.
- ClkA rising edge: terminal samples.
- If ClkB = 1 at this time → 4 bits of video.
- Else → control nibble:
  - bit 2 = HSync
  - bit 3 = VSync
  - bits 0:1 = undefined; terminal-specific use.

A small DispY-board circuit drives the 7-wire interface from the
MiniMixer. MinMixer[0] is serial-to-parallel converted into
4-bit nibbles, held in a register, and clocked out at the video bit
rate.

### Back channel (terminal → Dorado)

Serial input clocked by the terminal on the rising edge of HBlank,
sampled by DHT during the scan line *after* HWindow.

By convention, terminal microcomputer encodes 32-bit messages,
delivered one bit per scan line:

- 1 start bit
- 3 unused bits
- 4-bit message type (Table 24)
- 16-bit message body
- 8-bit trailer (must be 200₈)

After 32 bits, DHT ignores the back channel until the next start
bit.

### Table 24: Terminal microcomputer messages

| Type | Body                                                                  |
|------|----------------------------------------------------------------------|
| 00B  | Illegal — ignored                                                     |
| 01B  | Keyboard word 0 (Alto memory location 1077034B)                       |
| 02B  | Keyboard word 1 (Alto 177035B)                                        |
| 03B  | Keyboard word 2 (Alto 177036B)                                        |
| 04B  | Keyboard word 3 (Alto 177037B)                                        |
| 05B  | Mouse buttons + keyset (Alto 177033B)                                 |
| 06B  | 8-bit Δx (msg[0:7]) and 8-bit Δy (msg[8:15]), excess-200₈              |
| 07B  | Illegal                                                               |
| 10B  | Keyboard word 4 (Star keyboards only)                                 |
| 11B  | Keyboard word 5 (Star)                                                |
| 12B–16B | Illegal                                                            |
| 17B  | Boot message (boot button held: data jams to 1; trailer ignored)      |

## DHT and DWT microcode

### DHT (low-priority, once per scan line)

DHT runs at scan-line frequency (= ~38 kHz on Alto monitor). Wakes
at the *end* of HWindow, computes parameters for the *next* scan
line, loads NLCB for both channels, sets `αNextWCBFlag`. The wakeup
remains active until at least one NLCB output command is executed,
so DHT must execute at least one Output←B every time it wakes,
and that must occur within at most three instructions before
blocking.

DHT also samples the back channel for 32 scan lines per message and
decodes terminal events (keyboard, mouse) into Alto-format memory.

### DWT (high-priority, per-munch)

DWT is a very-high-priority task that runs once per channel per
munch. It uses the SubTask mechanism: SubTask 0 = channel A,
SubTask 2 = channel B. SubTask is OR'd into RBase[2:3] so each
channel reads its own RM region for `αNextAddr`/`αNextCount`/etc.

The main-line DWT microcode (HM page 118 verbatim):

```
DWTStart:    ACount←(ACount) - T,  Branch[DWTCheck, R<0];
             AAddress←(IOFetch← AAddress) + (Output←T), Block,
                                                Branch[DWTStart];

DWTCheck:    AAddress←(AAddress) AND (1C), Branch[DWTAdjustWCBFlags, R even];

DWTRefill:   ACount←ANextCount;        ; from DHT, # munches to fetch -1 in 0:11
             BrLo←ANextAddrLo;          ; first munch address
             BrHi←ANextAddrHi;
DWTAdjustWCBFlags:
             AAddress←(AAddress) - 1, Output←AAddress, Block, Branch[DWTStart];
```

### DWT wakeup logic (HM page 118)

DWT lowers its wakeup at the onset of `DWTStart`. The DDC remembers
DWT-is-running. While running or preempted, no further DWT wakeups
fire. When DWT blocks, a counter starts (initialized to a constant
N, configurable via SIP component); when it expires, DWT wakeups
are allowed again. Two purposes:

1. **Memory pacing**: spaces IOFetch← references by ≥ 8 cycles
   (depending on N) so the memory pipeline isn't clogged.
2. **Output-effect propagation**: lets output commands settle (which
   alter `αCurrentWCBFlag`/`αNextWCBFlag`) before the wakeup-decision
   re-fires.

After N cycles, DWT is woken when `αWantsDWT` is true for one of
the channels. The flag is:

```
αWantsDWT = (αNextWCBFlag AND NOT αCurrentWCBFlag)
          OR (αCurrentWCBFlag AND αFifoAvailable)
```

If both channels want DWT, the channel that ran least recently runs
next.

> **Note:** Neither DWT nor DHT drives the IOAtten branch condition.
> They use Block + wakeup latch alone.

### Hold + NextLies pathology (HM page 119)

Because DWT's normal final instruction is `IOFetch←`, the next
instruction (executed by another task after the block) may be held
one cycle if it initiates any memory reference. Also, the two-
instruction DWT loop (DWTStart and DWTAdjustWCBFlags) requires the
hardware to cope with `NextLies` from §7 — a high-demand higher-
priority task that always creates NextLies (Block + immediate ←Md
after Fetch) would delay DWT wakeups by 2 cycles per iteration.

## Slow I/O interface (HM pages 119–120)

DDC's six output devices + one input device (DispY) or eight + one
(DispM) are addressed via TIOA. The protocol per output:

1. t₁: TIOA latched into a register, decoded as one of the DDC
   commands.
2. t₃ → t₅: IOB clocked into RIOB, one TIOA-command pulse fires.
3. t₅: action complete.

The IOB value persists in RIOB until the next output command, which
is useful for debugging and for muffler readout (an NLCB address
can be parked in RIOB for multiple cycles).

### RAM-loading protocol (Keep'/LoadAddr/Write')

HRam, Mixer (AMap), BMap, CMap each have **two address sources**:
- The Dorado slow-IO bus (load path).
- The video system (display path).

A `Keep'` flipflop on each RAM gates the address mux:
- `Keep' = 0` (false) → Dorado owns the RAM (loading).
- `Keep' = 1` (true) → video system owns it (displaying).

Output commands to a RAM include:
- **`LoadAddr`**: if `Keep'` is *already* true (set by a previous
  command), then IOB[4:15] is loaded into the RAM's address
  register.
- **`Write'`**: if `Keep'` is already true, write the data field
  into the currently-addressed word; the address auto-increments
  for fast sequential loading.
- A RAM output command with `Keep' = false` (high) releases the RAM
  back to the video system.

For the **Mixer**, the address is loaded from IOB[4:14] and a
**Hi/Lo Select** bit comes from IOB[15]. Hi/Lo behaves as a low-
order extension of the Mixer counter, so successive `Write'`
commands alternate between the two halves of one 24-bit Mixer
word before advancing.

### Statics output

Two bits in `Statics` are `DHTShutUp` and `DWTShutUp` (enable/
disable wakeups for those tasks). Three other fields drive
diagnostics:

- **FakePClk**: toggle to single-step the pixel clock.
- **UseFakePClk**: disable the regular pixel clock; use FakePClk.
- **MufAddr** (7 bits): selects which DDC internal signal is
  fed into the muffler readout (DiskMuff-style — see Table 25).

### Table 25: DDC muffler signals (HM page 120)

| MufAddr | Signal              | MufAddr | Signal              |
|---------|---------------------|---------|---------------------|
| 0       | ACurrentWCBFlag     | 70      | AFifoFull           |
| 01-07   | AReaderPtr[1:7]     | 71      | BFifoFull           |
| 10      | ANextWCBFlag        | 72      | ASize8              |
| 11-17   | AWriterPtr[1:7]     | 73      | ASize8-4            |
| 20      | BCurrentWCBFlag     | 74      | ASize8-4-2          |
| 21-27   | BReaderPtr[1:7]     | 75      | BSize8              |
| 30      | BNextWCBFlag        | 76      | BSize8-4            |
| 31-37   | BWriterPtr[1:7]     | 77      | BSize8-4-2          |
| 40-47   | AItem[0:7]          | 100     | AOn                 |
| 50-57   | BItem[0:7]          | 101     | BOn                 |
| 60-63   | AServicePtr[1:4]    | 102:103 | ARes[0:1]           |
| 64-67   | BServicePtr[1:4]    | 104:105 | BRes[0:1]           |
|         |                     | 106     | MonitorType         |

`MufAddr=106` (MonitorType) is the only one normally exercised in
production: 0 = Alto-style, 1 = LF (large format / Star).

## What we have today

`dorado/include/display.h` + `src/display.c`.

**Phase 1**:
- **Framebuffer**: 808×606 mono, 61206 bytes, MSB-leftmost packing.
- **Slow-IO catch-all**: registered on tasks DHT/AHT/AWT/DWT. All
  Output←B accumulate into a buffered RIOB; counter tracks them.
  Pd←Input currently returns the headless keyboard idle word.
- **State buckets**: per-channel NLCB/CLCB (16×12-bit, A and B),
  HRam (1024×3-bit), Mixer (1024×24-bit), PixelClk, Statics.
- **Per-channel FIFO** (256 words) for IOFetch← munches.
- **PGM snapshot helper**.
- **Headless keyboard words**: five complemented Alto-style words
  (`1 = key up`, `0 = key down`). SDL or another frontend should
  translate host key events into these same core APIs.
- **VBlank frame counter**: `dorado_display_frame()` reports completed
  vertical blanks. `dorado_display_advance_pixels()` advances the
  synthetic raster and increments only when it crosses the modeled
  vblank boundary. A frontend can render the framebuffer mid-frame
  without changing frame count; headless tests can call
  `dorado_display_vblank()` when they intentionally mark a completed
  frame for snapshotting.

**Phase 2**:
- **`dorado_display_render_fifo()`**: drains the per-channel FIFO
  and lays 16 pixels per word into the framebuffer at (0..808,
  dst_y), advancing dst_y as full rows are consumed. MSB =
  leftmost pixel. 1-bit-per-pixel only (Alto monitor convention).

What's not yet wired:

- TIOA→command decoder (we don't yet know the numeric TIOA values
  microcode emits; Phase 3 will trace them).
- Pixel clock / waveform generation. The current vblank boundary is
  synthetic at the end of the modeled visible 808×606 raster; real
  blanking intervals still need to come from the DDC timing model.
- 7-wire interface back channel (keyboard/mouse → 32-bit messages).
- 24Bit / A8B2 / BBypass mixer modes.
- αItemSize handling for grayscale (2/4/8 bits per pixel).
- HSync / VSync / VBlank waveforms driving the render timing.

See `docs/io-systems-architecture.md` for a higher-level view of
how display fits into Slow I/O / Fast I/O / Tasking.
