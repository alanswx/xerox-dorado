# Verilog from Sil: handoff

2026-08-16. This file is written to be picked up cold. Read
`docs/verilog-from-sil.md` for the background and why each decision was made.

---

## Where it stands

```
make -C verilog boards     # 16 boards from PARC's wire lists   -> generated/
make -C verilog cells      # cell skeletons for any new part    -> cells/
make -C verilog lint       # THE GATE: every board, and the machine, elaborates
make -C verilog/verilator  # the imgui harness
./verilog/verilator/obj_dir/Vemu --headless --cycles 5000
python3 tools/dorado_proms.py --check      # 26/26, every one property-checked
python3 tools/dorado_proms.py --placement  # which package holds which PROM
make -C verilog proms      # proms/*.mem and the per-package images
make -C verilog prom-test  # THE GATE: PROMs read back what the machine expects
python3 tools/sil_backplane.py             # what the backplane is, measured
make -C verilog alu-test   # THE GATE: the ALU matches its datasheet
make -C verilog alu-diff   # THE GATE: the ALU matches the C EMULATOR
make -C verilog cpreg-diff # THE GATE: the BOOT INTERFACE matches the C emulator
python3 tools/sil_backplane.py --ports     # boards present the ports PARC states
make -C verilog machine-test  # THE GATE: the assembled machine clocks and SETTLES
make -C verilog baseboard-test  # THE GATE: the BaseBoard's 6502 BOOTS
make -C verilog cell-check    # THE GATE: cells wire the inputs PARC says they do
make -C verilog loop-check    # THE GATE: no combinational feedback but the known one
make -C verilog backplane MACHINE=--boards=ProcH,ProcL   # any subset
```

| piece | state |
|---|---|
| Netlist reader + Verilog generator | 16/16 boards, 67,960 lines (+2,658 top, +4,599 cells), **all lint clean** |
| Cell library | 80 cells, **97.7%** of the eleven-board machine's logic packages |
| 6502 | Andrew Holme's netlist-derived core (via jotego), wired into `cell_MCS6502` |
| 6532 RIOT | MiSTer Atari 7800's, patched for Verilator. **CC BY-NC** -- see `verilog/vendor/LICENSES.md` |
| PROMs | **26 of 26** generated, **29 packages wired into the RTL and read back correctly** |
| Harness | Verilator + Dear ImGui, builds, runs, `--headless` CI mode |
| Board port lists | **from PARC's own `.bp`**, 1,920/1,922 exact, 0 spurious |
| Backplane top module | **generated**, 11 boards wired by name, lint clean |
| Synthesisability | **no `inout`, no multiply-driven net, no gated clock** |
| The machine | **instantiated, and SELF-CLOCKING**: it generates its own clock |
| BaseBoard 6502 | **BOOTS ITS OWN FIRMWARE** -- reset vector, then the ROM's instruction stream |

`verilog/generated/dorado_backplane.v` instantiates eleven boards and wires
501 nets between them; `dorado_machine` resolves the 407 external ports and
`sim.v` runs it. **Nothing is injected: the machine makes its own clock.**
The VCO is substituted for a fabric-clock divider -- an analog oscillator has
no digital model -- and everything after it is the BaseBoard's own logic:
h05/g05 (MC1660) shape two anti-phase clocks, four MC1690s divide them into
`StartClockPulse'` and `EndClockPulse`, and j02 fans those out. All 24 clock
nets toggle, and six downstream signals with them (`MemWEa`/`MemWEb`,
`LoadEcOut'`/`ShiftEcOut`, `LargeHold`, `TWReq15`).

One board of it now COMPUTES: the BaseBoard's 6502 comes out of reset,
fetches `0xF3A7` from `0xFFFC/D`, and executes the ROM's own reset routine.
See "The BaseBoard's 6502 boots" below. The rest is still filling in.

**And it settles.** `machine-test` passes with the BaseBoard running: 24/24
clock nets toggling, 37 signals moving, stable over 200,000 cycles as readily
as 20,000. It did not, for a while, and the cause was not the design's
gate-delay tricks but six cells modelled as transparent latches that are not,
plus one counter's carry that PARC's dictionary says is registered. See "the
machine did not settle" below.

### What landed on 2026-08-15/16

- **The last two PROMs.** `EtherRcvr` and `EtherXmtr` are ported, so all 26
  now generate. They are the only PROMs that are genuine state machines, and
  each has a property check asserting the behaviour the source's comments
  describe -- reachability, no undefined next state, the transitions
  themselves. The checks were then **mutation-tested**: fourteen deliberate
  transcription errors were injected (dropped inversions, swapped low-true
  polarities, transposed `srCtrl` encodings, a lost BCPL switchon
  fall-through, a field shifted one bit) and all fourteen are caught. The
  first draft of the checks caught only eleven; the three misses are why the
  checks now pin the srCtrl encodings and the two low-true inputs
  behaviourally rather than just rejecting the illegal value.
- **The backplane, found rather than inferred.** Every board directory states
  its interface THREE times and all three agree: `<Board>.bp` (`ALUCarry:
  E179`), bare `E179` tokens in the `.wl` itself, and slot-qualified
  `#s05-C.5` in `-C.nl`/`-E.nl` -- 2,052 of 2,054 pins identical, the five
  exceptions being ground nets the `.wl` numbers individually (`GND-26`) and
  the `.bp` collapses (`GND`). The Reference section below has what this means
  for wiring boards together.
- **The port lists now come from `.bp`.** They were inferred before, by
  treating a net whose only consumers were `Term100` pins as leaving the
  board. That was wrong at the root: **`Term100` is a 100-ohm terminating
  resistor network, not a connector** -- ECL terminates every line, and ProcH
  has 139 of them against 175 backplane nets. The inference missed 703
  backplane nets (emitted as internal wires, so those signals could never have
  reached another board) and invented 833 ports that are not on the backplane.
  All sixteen boards now emit exactly what PARC states, and
  `tools/sil_backplane.py --ports` is the gate: 1,920 of 1,922, 0 spurious.
  A **third port direction** falls out: 512 nets are `inout`, because the
  board both drives and senses them. `wire`/`output` there would let a board
  read back only its own contribution to a wired-OR bus instead of the bus.

- **The backplane top module is generated.** `make -C verilog backplane`
  writes `verilog/generated/dorado_backplane.v`: eleven boards, **501
  internal nets** (83 of them `wor`), **407 top-level ports**. Every port
  carries a comment saying whether it goes to a connector on the backplane
  (274 -- disk tag bus, ethernet transceiver, monitor, keyboard) or is
  waiting for a board this configuration does not have (133). `--boards`
  takes any subset, so the machine can be brought up a board at a time, and
  every subset tried lints clean.
- **DispM plugs INTO DispY; they are not alternatives.** 42 nets are shared
  by the two display boards and by no other board -- `AItem.0-7`, `BItem.*`,
  `AOff`, `BByPass` -- so a colour machine has BOTH, and adding DispM turns
  those 42 dangling ports into internal nets. This agrees with the Hardware
  Manual (doc p.110: "on a Dorado with only a 7-wire terminal and no color
  monitor, only the DispY board is present") and with the C emulator, where
  the colour display is a second screen rather than a colourisation of the
  first.

- **The PROM contents are in the RTL, and read back correctly.** The handoff
  used to say "give `cell_MB7071H` a `$readmemh` array". That was the wrong
  part: `MB7071H` has `WE'`, block selects and separate IN/OUT pins -- it is
  a **RAM**, and its 70 packages sit on DispM (32, the colour tables), MemC
  (21) and ProcH/ProcL (4 each). The PROMs are three other parts, and the
  counts settle it exactly: **`SG10139`** (32x8 ECL) has 15 packages =
  DskEth 2 + IFU 1 + MemX 8 + Proc 2+2, precisely the 32x8 entries;
  **`MCM10149`** (256x4 ECL) has 15 = DskEth 11 (disk and ethernet) +
  DispY 2 + DispM 2; **`SN74S288`** has the one 32x8 TTL part, the disk
  drive-select. All three now model the part and take an `INIT_FILE`, and
  the generator passes each of the **29 placed packages** its own image.
  Gate: `make -C verilog prom-test`, 1,360 checks, mutation-tested with five
  injected wiring errors (address order, output order, chip-enable sense).
- **Bit order is one rule on both axes, and it is now tested.** PARC's
  structures are MSB-first, so **A0 is the most significant ADDRESS bit and
  Q0 the most significant OUTPUT bit**. Both are stated in the sources --
  `EtherProms` marks `pdCarrier bit // A0 pin 4` for a field that is address
  bit 7, and `DiskProms` has `Pin1 = #200`, bit 7 of a byte, where Pin1 is
  Q0. A neat confirmation fell out: `SN74S288` wires only six outputs
  (Q0..Q5), and `MakeDriveSelect` uses only bits 7..2 -- exactly those six.

---

## Reference: the FPGA shape, and why it is not the physical one

The target is an FPGA, which rules out two things the Dorado does natively.
Both are handled in the GENERATOR, so the RTL stays derived rather than
hand-adjusted.

**Wired-OR buses become OR trees.** MECL 10K outputs are open emitters and
the machine ties them together: 115 backplane nets are driven by more than
one board, the B bus `BMux.00-15` among them (ContA, IFU, MemC, MemD, MemX
and ProcH/ProcL all drive it). An earlier pass modelled that with `inout`
ports and `wor` nets, which simulates correctly -- and synthesises nowhere,
because an FPGA has no wired-OR outside its I/O ring. Now each board exports
its CONTRIBUTION and reads the resolved bus back:

```verilog
    output wire BMux_00__drv,      // in the board
    input  wire BMux_00
    ...
    assign BMux_00 = BMux_00__ContA | BMux_00__IFU | ... ;   // in the machine
```

which is exactly what the open emitters compute, in one LUT level. The gate
is `verilator --lint-only` with `-Wno-MULTIDRIVEN` REMOVED: it reports zero.

**Distributed clocks become clock enables.** The Dorado fans a clock out to
every board and every flip-flop is clocked from that ECL net -- 1,201
packages across ten part types. Synthesised literally that is 1,201 gated
clocks off combinational logic, which no fabric can route. Every clocked cell
now runs on `sys_clk`, the fabric clock, and uses the modelled clock net as
an ENABLE:

```verilog
  reg  ck_d;
  always @(posedge sys_clk) ck_d <= p9;
  wire ck_en = p9 & ~ck_d;
  always @(posedge sys_clk) if (ck_en) q <= {p12, p11, p10, p7, p6, p5};
```

Standard oversampling, faithful as long as `sys_clk` is faster than the clock
net -- `dorado_machine` divides by four to guarantee it. Asynchronous inputs
(MR, S/R, CL') are level-tested on the same edge, which keeps them out of the
fabric's reset network. The two DRAM cells got the same treatment and their
writes are synchronous now, so they INFER BLOCK RAM instead of 309 packages'
worth of latches.

`sys_clk` is threaded by the generator: a cell that declares the port gets it
connected, and boards and the top module pass it down. It is not a Dorado
signal and does not appear in any `.bp`.

## Reference: where the clock comes from

Worth knowing, because the obvious answer is wrong. **The master clock is
GENERATED ON THE BASEBOARD, not fed to it**, and the chain is:

```
h06 MPQ3303   an analog VCO -- transistor quad, control voltages from a cable
  -> h05 MC1660    shapes VCOPhase0/1
  -> g05 MC1660    one input, both outputs: two ANTI-PHASE clocks
  -> g04+h04 MC1690  cross-coupled, a Johnson counter dividing by four
     g03+h03 MC1690  the same, delayed on the other phase
  -> StartClockPulse' (g04.Q') and EndClockPulse (h03.Q)
  -> j02 MC10210 ORs them -> l01/k01/j01/i01/h01 -> CLK.ph', CLK.mc', ...
```

`CLK.InBase` is how the BaseBoard receives that distributed clock BACK on its
own C9 -- the same pin every other board takes it on -- so the wrapper loops
it from `CLK.OutBase'`, which is what the backplane does. Driving C9 from
outside reaches ten nets and stops.

**Only the VCO is substituted**, and it is the one substitution in the design:
an analog oscillator has no digital behaviour to transcribe (resolved as
logic, its loop simply latches), and an FPGA has no VCO either -- a clock
comes from a pin or a PLL. `cell_MPQ3303` is therefore a divider off
`sys_clk`, and everything downstream of it is the board's own logic.

An earlier version injected a clock at `dStartClockPulse` (a real backplane
input into j02) because MC1690 had no model. That is gone. If it is ever
needed again, note the trap it produced: once MC1690 WAS modelled,
`StartClockPulse'` idled high, the OR at j02 stuck high, and the injected
clock stopped reaching anything -- the toggle count went from 27 to 0.

## Reference: how the backplane was derived, and why by name

`tools/sil_backplane.py` builds the top module; this is the evidence behind
it, kept so nobody has to re-derive it.

**The backplane is NOT straight-through.** The clock distribution proves it in
one line -- the BaseBoard drives each board's clock from a different pin while
every receiver takes it on pin **C9**:

```
CLK.ph'   BaseBd=C16 -> ProcH=C9      CLK.mc'   BaseBd=C20 -> MemC=C9
CLK.pl'   BaseBd=C13 -> ProcL=C9      CLK.mx'   BaseBd=C21 -> MemX=C9
```

Of 639 net names on more than one board only 446 sit at the same pin, and 182
pin positions carry different nets on different boards (`E90` is `MemBase.2`
on the processor and memory boards, `TNIA.06` on Control). **A pin number
cannot identify a signal. The name is the connection** -- which is what PARC's
flow assumed: boards were drawn against agreed names, the router assigned pins
per board, and the backplane was wired to suit.

Name matching then produces a machine that hangs together, which is the real
check on it. ProcH, ProcL, ContB, MemC and MemD have **2-5 unpaired nets each**
out of ~175, and the boards with many unpaired ones are unpaired for a reason
you can name: DskEth's 72 are the disk tag bus, the differential data pairs
and the ethernet transceiver; DispY/DispM's are monitor DACs and sync; the
BaseBoard's 80 are its own ACP bus, serial lines and grounds. Those go to
connectors ON the backplane, not to another board. The BaseBoard's paired
nets are the whole of how it reaches the machine, and they read exactly as
the Hardware Manual describes it: the clock star, `CPAddr`/`CPIn`/`CPOut`/
`CPStrb` to Control only (that is CPReg), and `DMuxClk`/`DMuxData` at E186/
E187 on all ten boards.

**The caution the earlier plan did not have: 115 shared nets are driven by
more than one board.** That is not a conflict to resolve, it is ECL
open-emitter wired-OR working as designed -- `BMux.00-15`, the B bus, is
driven by ContA, IFU, MemC, MemD, MemX and ProcH/ProcL together. In Verilog
these want `wor` nets. Verilator 5.044 accepts `wor` but raises MULTIDRIVEN,
which is fatal by default, so the emitted file brackets the declarations:

```verilog
/* verilator lint_off MULTIDRIVEN */
wor BMux_00;
/* verilator lint_on MULTIDRIVEN */
```

That works -- with the bracket in place and MULTIDRIVEN *not* waived on the
command line, the assembled machine reports it on none of the 83 wired-OR
nets. It reports it on eight others; see the known defect below.

Mind which boards can be present together when reading that count.
**msa/PCMSA are two generations of storage-module board** and appear to
co-drive 19 nets only because the listing reads all sixteen board files; a
machine has one or the other. **DispM and DispY are NOT such a pair** -- see
above, the colour board plugs into the monochrome one and a colour machine
has both.

`tools/sil_backplane.py --emit` does exactly this, and `--json OUT` dumps the
same netlist as data -- every net with the boards and pins it touches, its
drivers, and a `wired_or` flag.

**The mechanism is verified, not assumed.** Verilator 5.044 was checked to
simulate this shape correctly -- a cell instance output tied straight to a
module `inout`, two such boards on one `wor`, both reading the resolved value:

```
a=0 b=0 -> bus=0 sa=0 sb=0      a=1 b=0 -> bus=1 sa=1 sb=1
a=0 b=1 -> bus=1 sa=1 sb=1      a=1 b=1 -> bus=1 sa=1 sb=1
```

Two things NOT to use:

- **The slot numbers.** `Slot` and the `.nl` headers give one, but BaseBd,
  ContB and IFU all claim `s01`: it is whatever the last build was run with,
  not the machine's layout. It affects only the `#sNN-` prefix in the `.nl`
  node names, which the name-matching does not need.
- **`DoradoDocs/schematics/Backplane.pdf` as a data source.** It IS the real
  backplane wiring chart (8 pages, `BPRight01-04.sil` / `BPLeft01-04.sil`,
  Mike Overton, 7/24/80, pin rows x board columns) and its `.sil` source is
  NOT in the archive -- `[IO]<DoradoBuildDocs>` has only `bpwirelist01/02.sil`
  and cable drawings. The PDF is a scan run through Acrobat OCR, and the OCR
  mangles exactly what matters (`Mel.08` for `Md.08`, `106.14` for `IOB.14`,
  `BO` for `80`). Use it the way it was used here: as an INDEPENDENT SPOT
  CHECK. Its pin 24 = `RSTK.0` and pin 5 = `StartCycle'`, and `ProcH.bp` says
  `RSTK.0: C24` and `StartCycle'a: C5`. That agreement is what confirms the
  `.bp` reading is right.

## Fixed: BaseBd's `MCD_0..7` were double-driven

Found while linting the assembled machine, and pre-existing (the committed
board file had the same eight). BaseBd's internal 6502 data bus `MCD_0..7`
each had two continuous drivers: `emit()` built its on-board wired-OR stubs
from the pins the WIRE LIST marks `o` and redirected only those, so a pin the
wire list marks `i` whose CELL declares an output still connected straight to
the net -- the `assign MCD_7 = stub | stub | ...` racing that cell output.

Same shape as the ASSIGNIN bug below, same fix: `Generator.drivers_in_rtl()`
is now THE definition of "this pin drives", used by `classify()` for port
direction and by `emit()` for wired-OR resolution, so the two cannot disagree.
MULTIDRIVEN with the waiver removed went 8 -> 0.

It moved five nets from `inout` to `input`, and that is the right answer:
`OISData`/`OISData'` (BaseBd), `Collision`/`RcvData` (DskEth) and `Syn+5V`
(Music) are marked `o` in the wire list at pins belonging to **`AUGATCG16`,
an Augat connector block, and `SIPpackage`, a resistor network**. Neither is
a logic part, so nothing in RTL drives them -- `Collision` and `RcvData`
arrive from the ethernet transceiver over a cable. They are board inputs, and
the top module exposes them.

## Tim found a whole class of bug: the COMMON pin

He read `cell_MC10100` and saw that pin 9 -- the strobe MC10100 feeds into all
four NOR gates -- had been wired into the first gate only, and that
`cell_MC10101` had the same fault with its common pin 12. Both cells were
written from EclDict's PIN BLOCK, which names a common input ONCE, on the
first gate, and leaves you to know it is shared.

**The dictionary says it a second way, unambiguously.** Beside the pin blocks
is a gate summary per part, with every gate's complete input list:

```
MC100
[G (4 5 9)>(2), (6 7 9)>(3), (10 11 9)>(14), (12 13 9)>(15) : 1 3.3 ]
```

`tools/sil_check_cells.py` (`make -C verilog cell-check`) compares every
combinational cell against those lists, inlining intermediate wires so a
factored cell is checked like a flat one. It found **seven** bugs across
**270 packages** -- Tim's two and five more:

| part | pkgs | what was wrong |
|---|---|---|
| `MC10101` | 54 | common pin 12 in one gate of four (Tim) |
| `MC10100` | 25 | common strobe pin 9 in one gate of four (Tim) |
| `MC10125` | 66 | each channel read one of its two DIFFERENTIAL inputs, and the fourth channel was not modelled at all -- its output tied to zero |
| `MC10195` | 47 | common pin 9 in one gate of six, AND the wrong function: it is an XNOR, not a NOR |
| `MC10124` | 41 | common strobe pin 6 missing from all four gates, AND the two output senses swapped |
| `MC1660` | 28 | my own: gate a drove pins 3 and 15, gate b drove 14 and 2 -- the two halves crossed |
| `MC10118` | 9 | two independent gates modelled as one term feeding both outputs |

The data book settled the ones where the FUNCTION mattered, not just the
wiring -- `DoradoDocs/datasheets/ON_MECL_DataBook_DL122_rev7.pdf`, 474 pages,
which is worth having for everything else too:

- **MC10124**: "When the common strobe input is at the low logic level, it
  forces all true outputs to a MECL low logic state" -- so each channel is
  AND(data, strobe), both senses brought out.
- **MC10125**: the pin assignment shows differential pairs, `AIN`/`A-bar-IN`
  on pins 3 and 2 and so on, with one TTL output each.
- **MC10195**: its truth table is an XNOR, and EclDict names pin 9 twice,
  `Invert'` and `Buffer` -- the common pin CHOOSES which the part is. That is
  the "Inverter/Buffer" in its name.

**What the MC1660 fix then exposed.** With that cell correct, the machine
stopped clocking -- and the earlier "self-clocking" result turned out to have
depended on the bug. The BaseBoard's VCO is a transistor quad (`MPQ3303`)
cross-coupled with an MC1660, and its job on `VCOPhase0`/`VCOPhase1` is to
PULL THEM DOWN. A wired-OR resolves as an OR, which can only pull up, so the
loop latches high and the clock never starts. Since the VCO is already a
documented substitution -- an analog oscillator has no digital model -- the
substitute now drives those nets outright: `Generator.OVERRIDE_DRIVERS`, one
part, one net pair, stated in the generator rather than hidden in a cell.

**Then the same audit on the SEQUENTIAL cells**, which the first version
skipped. The dictionary has a parallel summary for them, and it names the
clock and asynchronous set/reset outright:

```
MC231
[FF 10 {1.1 .8}>(14 15) : CLK (9 11) (1.5 3.7) RS (12 13) ...]
[FF  7 {1.1 .8}>(2 3)   : CLK (9 6)  (1 3.7)   RS (5 4)   ...]
```

**Pin 9 is in BOTH clocks** -- Tim's class again, in the biggest flip-flop in
the machine. `cell_MC10231` (152 packages) and `cell_SE10231` (8) used only
each half's own clock pin and explicitly dismissed pin 9 as unused. The data
book says how they combine (DL122 rev 7, MC10231): "Each flip-flop may be
clocked separately by holding the common clock in the low state and using the
enable inputs for the clocking function. If the common clock is to be used to
clock the flip-flop, the Clock Enable inputs must be in the low state." Either
can clock it while the other is held low, which is an OR.

Fixing it moved the machine: `machine-test` went from 27 signals to **34**,
and the new ones are a coherent group -- `LoadSoutE'`/`LoadSoutO'`,
`ShiftSinE`/`ShiftSinO`/`ShiftSoutE`/`ShiftSoutO`, the memory system's
even/odd shift path.

The check is bounded by each cell's DECLARED pins, because a cell only
declares what the boards wire: MC1690 has a second clock input that no Dorado
board uses, and reporting that would be noise.

**What the checker cannot do**, so that its silence is not over-read: it
checks CONNECTIVITY, not function -- a gate that ORs where it should AND uses
the same pins. And 19 cells legitimately read MORE than `[G]` lists, because
that summary omits selects, enables and carries (MC10158's pin 9 is SELECT,
MC10174's pin 14 is ENABLE, MC10180's carry-in); those are reported
separately, not as errors.

## Reference: OR/NOR output polarity -- SETTLED from the datasheets

A third of the packages in this machine are OR/NOR gates, and getting the
sense backwards produces a machine that almost works. The rule is now
confirmed against Motorola's own sheets, so cells can be written mechanically.

**EclDict role `OUT` is the INVERTING (NOR) output. Role `o` is the
non-inverting (OR) output.**

| part | EclDict | datasheet pin labels |
|---|---|---|
| MC10101 | `a,OUT,2 > a,o,5` (and b,c,d) | pin 2 = **A-bar-OUT**, pin 5 = AOUT |
| MC10212 | `a,OUT,3,4 > a,o,2` | pins 3,4 = **A-bar-OUT**, pin 2 = AOUT |

Eight gates across two parts, unanimous. (MC10101 from
`futurlec.com/Datasheet/Motorola/MC10101.pdf`, MC10212 from the same
directory; both are the Motorola technical data sheets with the DIP pin
assignment drawn out.)

**The one exception, and it is not really one.** A part with only a single
output sense uses `OUT` for all of them, so the letter cannot discriminate:
MC10110 (3-output OR) and MC10111 (3-output NOR) have IDENTICAL dictionary
entries. There the PART NAME decides, which is what `cell_MC10210` and
`cell_SE10211` do.

**What this fixed.** `cell_MC10105` had its two senses the wrong way round --
34 packages, 31 of them in the machine -- and `cell_MC10103` gave one gate's
two outputs the same expression, which cannot be right for a gate the
datasheet says "has both OR and NOR outputs". Both corrected.

**What did not settle it, so nobody repeats the work:** PARC's net naming
looks decisive and is not (on MC10212, eight gates put the primed net on the
`nout` pin and five on `out` -- a gate fed already-inverted inputs is named
for its function, not its pin sense), and the MECL Pocket Book scan that
`cells/PARTS.md` cites carries functions and schematics but no pinout tables.
Go to the per-part Motorola technical data sheets instead.

## The BaseBoard's 6502 boots (2026-08-17)

`make -C verilog baseboard-test` is the gate. It asserts four things at once,
and they were derived separately, from four different sources:

* the 6502 addresses `0xFFFC`/`0xFFFD`;
* `0xA7` and `0xF3` come back on `MCD`, so the vector reads `0xF3A7`;
* it then FETCHES from `0xF3A7`; and
* the bytes that follow are the ROM's own reset routine --
  `CLD`, `LDX #$00`, `LDA #$00`, `STA $00,X`, `DEX`, `BNE $F3AC`, byte for byte
  what `chm/disassembly/bb_F000-FFFF.s` disassembles at that address.

The fourth check is the one that matters. A 6502 that fetches its vector but
latches nothing still walks a plausible-looking address bus -- during bring-up
it walked `0x0000` upward for thousands of cycles, with a healthy 5,941
transitions -- so the gate reads the OPCODES, not the addresses.

Six things had to be true together, and each was mutation-tested by breaking
it deliberately and confirming the gate fails, each with its own message:

**1. Resistor packs are not a cell, and the machine's constant 1 comes from
one.** `SIPpackage` pins DRIVE on some boards and are the tie point on others
-- seven of the eight are used both ways -- so no fixed set of port directions
fits, and the cell that existed declared all eight as inputs and drove
nothing. So `TTLTrue.A`..`E`, which BaseBd g47 supplies and every TTL counter,
flip-flop and enable on the board counts from, sat at zero. The generator
resolves these now (`sil_to_verilog.py sip_pull`): a SIP is resistors from a
COMMON pin to the rest, the common is found by what it is CONNECTED to -- a
power net, or a reference the board makes for the purpose (`True`,
`ECLTrueA`) -- and every other pin is held there. Two exclusions matter. A net
held by a pull-UP pack and a pull-DOWN pack at once is a resistive DIVIDER, a
bias network at an analog input, so neither contributes and the net stays
open: that is DskEth's `RcvData`, the Ethernet receiver's own input, which it
would otherwise force high. And the common may never be a net the pack itself
drives -- a pack that MAKES a reference calls it something like `TTLTrueA`,
and taking that as its own common emits `assign X = X;`, a wire that inputs
its own output, which Verilator accepts and which then fails to settle tens of
thousands of cycles later on a different board.

**2. The supply rails have to be stated.** A wire nobody assigns reads zero,
and 1,010 nets across the sixteen boards are a gate's enable, preset or count
input tied straight to VCC. `VCC`/`VDD` are 1; `GND`, `VEE` (-5.2 V), `VTT`
(the -2 V ECL terminator) and `VBB` (-1.3 V) are 0.

**3. The ROM decode is a wire-wrap strap, and the netlist states it
geometrically.** `Rom0'`..`Rom7'` come from the '138 at g11, whose selects
`RSA.0/1/2` are driven by nothing at all: they arrive on c07, an Augat header.
What a header does is decided by which wires a technician wrapped onto it, and
that is not in the netlist -- but the GEOMETRY is. Every pin in the wire list
carries an `{x,y}`, and a jumper position is a COLUMN of pins at one x. At
c07, `MCA.11`/`MCA.12`/`MCA.13` sit directly across from
`RSA.0`/`RSA.1`/`RSA.2`. `tools/firmware_eproms.py` had derived exactly that
strapping by asking what tiles the address space with 2K parts; the header
says it outright.

The generator takes a strap only where the netlist forces it (`jumper_straps`):
a column of exactly two pins where one net has no other source on the board.
A column of THREE is a choice and is left alone -- MemX's b14 offers
`RTMapAd.1a` or `VCC-47`, msa's e26 picks `ChipsAre4k` against `ChipsAre16k` --
because guessing there would be inventing a machine configuration.

**4. The EPROM bytes are stored BIT-REVERSED, and the 1987 chip dumps prove
it.** The wire list puts the 2716's pin 9 on `MCD.7` and its pin 17 on
`MCD.0`, while the 6502's pin 26 (DB7) is on `MCD.7` and pin 33 (DB0) on
`MCD.0`. The data sheet calls 2716 pin 9 `O0` and pin 17 `O7`, so the ROM's
least significant output feeds the processor's most significant data bit: the
byte arrives reversed, and the chips were blown reversed. The address lines
are NOT crossed -- pin 19 is A10 on both the board and the data sheet -- so
this is a deliberate data-bus reversal, not a numbering artifact.

`firmware/B-08.BIN`, read bit-reversed, is byte-for-byte identical to
`doradobaserom.mb!13`'s `0xF000` block, all 2048 of them; `B-10.BIN` reversed
gives NMI=`0xF000`, RESET=`0xF3A7`, IRQ=`0xF2A2`, that image's own vectors. Two
artifacts sharing no lineage -- a chip read in 1987 and a `.MB` from the
archive -- agreeing once the bits are put back. **That retires a note in
`tools/firmware_eproms.py` that those dumps were "a different set or a
different layout" because none had a plausible 6502 vector triple.** They had
one all along, and they land on exactly the four populated sockets:
B-08 -> c61 (0xF000), B-10 -> b61 (0xF800), C-08 -> f60 (0xC000),
C-10 -> e60 (0xC800). Only B-08 matches byte for byte; the other three are a
later build than the archived `.MB`, which the 1987 date on the chips would
predict.

**5. Tri-state parts must contribute NOTHING when they are not driving.** The
shared nets here resolve as an OR of their drivers, which is right for MECL
open emitters. A TTL bus is different: one part drives and the rest are in
high impedance, so a part that is not driving has to contribute zero and leave
the active driver's value intact. The 2716 already did this with its CS'/PD';
the 6502 and the 6532 did not, so every part on the bus contributed at once,
`MCD` read `0xFF` at every address, and the ROM might as well not have been
fitted.

**6. A ROM is a registered block RAM, and here that is also the part's
timing.** The BaseBoard enables its EPROMs from `MCClk'`, so a 2716 drives
only while phase 2 is high -- and the processor latches the byte as that phase
FALLS. On the real board the '138 and the 2716 take tens of nanoseconds to let
go, which is exactly the data hold the 6502 needs. In zero-delay RTL a
combinational output vanishes in the same instant the CPU latches: the reset
vector was read correctly off the bus, `0xA7` then `0xF3`, and the processor
still started at `0x0000` every time. One fabric clock of registered output
supplies the hold, and is what an FPGA block RAM gives you anyway.

**And a seventh, which is the power-on reset.** `PwrGood` is the comparator
saying the supplies have reached their thresholds, and it is the clear input
of the flip-flop at j08 that produces `MCReset'`. `cell_MPQ3303` asserted it
true from the first cycle -- "the supplies are up, which for a simulated
machine they are" -- which released the netlist 6502 before it had run a reset
sequence at all: it came up executing `BRK` off an all-zero bus and never
fetched a vector. Supplies ramp; it comes up low and rises.

Also fixed on the way: the netlist 6502 core needs the FAST fabric clock on
`clk` and the board's `MCPreClk` on `phi`. Both were wired to `MCPreClk`,
which gives the 1,725-node relaxation one settling step per phase, and it
never converges.

### The parts that had to be modelled

The whole BaseBoard clock and reset chain, from PARC's `TtlDict.Analyze` pin
lists: `SN74LS04`, `SN74LS74`, `SN74LS163`, `SN74LS175`, `SN74LS259`,
`MC12061` (the crystal oscillator -- a documented SUBSTITUTION, like the VCO,
driving a divider off the fabric clock), plus the plain TTL family the board
uses around them: `SN74LS00`, `SN74LS01`, `SN74LS08`, `SN74LS32`, `SN7486`,
`SN7438`, `SN74LS139`, `SN74LS151`, `SN74LS251`, `SN74LS157`, `SN74LS253`,
`SN74LS85`.

**`SN74LS01` and `SN7438` are OPEN COLLECTOR, and the dictionary does not say
so.** TtlDict groups parts by PINOUT, so the '01 sits with the totem-pole '02
and '28; only the part NUMBER says the output is a pull-down with no pull-up.
It matters because a shared net here resolves as an OR -- right for emitter
followers, which pull UP -- and these pull DOWN onto a net a resistor holds
high, which is an AND. There are 15 such nets across two boards and each has
effectively one driver, so a single open-collector output plus its pull-up
computes the gate's own function and the cells drive that; the wired-AND only
bites if a second driver is ever added. `BootMC'` is one of them, and it is
the D input of the 6502's reset flip-flop: with the '01 unmodelled that net
sat low and the processor was held in reset forever.

`make -C verilog cell-check` now reads **both** dictionaries. It was reading
only `EclDict.Analyze`, which left all eighteen new 74-series cells unchecked;
`TtlDict.Analyze` states its gates in the same `[G ...]` form. 50 cells
checked, 0 ignoring an input.

### A part that reads a bus it also drives

`cell_MCS6502` and `cell_MCS6532` present the two directions separately --
`dbo` out, `dbi` back -- because the RTL has no `inout` anywhere by design.
Nothing in the wire list can say which companion port goes with which pins;
that is a fact about the PART, so it is named in the generator's `READBACK`
table and connected to the resolved nets.

It must be connected to what the OTHER drivers put on the bus, not to the
resolved net. Inside the netlist 6502 `dbo` is combinational on `dbi` -- both
come out of the same relaxation -- so handing back the whole net is a wire
that inputs its own output. It settles perfectly while the part is off the bus
and stops settling the instant it drives one: the machine ran thousands of
cycles and then failed to converge at the 6502's first `STA`, reported against
a different board entirely. Excluding the part's own contribution is also what
the hardware means -- a driver does not read its own drive to learn what it is
driving -- and the wired-OR already gives every driver a private stub, so it
costs nothing and no delay. (Registering the readback instead works too and is
wrong: it delays the read path enough to miss the ROM's hold window.)

## The BOOT INTERFACE cross-checks against the C emulator (2026-08-17)

`make -C verilog cpreg-diff`: **176 strobes from the C emulator, 44 of them
loading CPReg, 0 mismatches.**

This is how a Dorado starts, and it is the second thing in the machine to be
checked against the software model rather than only against the netlist it came
from. The BaseBoard's 6502 has no access to IM or to the processor's registers.
It drives nine data bits (`CPOut.0-8`), a three-bit function code
(`CPAddr.0'-2'`) and a strobe (`CPStrb'`) across the backplane, and CPReg, the
microinstruction register, run and single-step are all built out of that.

Two derivations meet, neither taken from the other:

* the RTL, generated from PARC's wire list -- an MC10161 at ContA a01 decodes
  the function code into `CPStrb0'`..`CPStrb3'`, and MC10176 hex flip-flops at
  a03, f02, g02 and h02 latch `CPReg.00`-`.15` from `CPOut.0-7`;
* `apply_mcp_strobe()` in `dorado/src/baseboard.c`, written from PARC's
  `doradoio.mdefs` and `doradocpint.masm`.

`dorado/tests/cpreg_vectors.c` includes `baseboard.c` directly, so the vectors
come from the emulator's real decoder, and `tb_cpreg.sv` feeds the same strobes
through the gates. Four mutations were tried and each fails: leaving the
function code uncomplemented, reversing its bit order, sending the data byte
LSB-first, and latching on the leading edge of the strobe.

**Three things the netlist settles**, all of which had to be right for a single
vector to pass:

* **The function code arrives COMPLEMENTED and the strobes are active low.**
  ContA's decoder takes `CPAddr.0'` on its most significant select, so function
  0 presents 7 and comes out on Q7' -- which the board calls `CPStrb0'`. The
  naming is self-consistent once you see why.
* **MSB-first, here as everywhere.** `CPOut.0` is the data byte's bit 7: it
  traces back through `TCPBus.00` to `MCPBus.00`, which is a RIOT's PA7. And
  `CPReg.00` is the high byte's most significant bit -- which is what makes
  ContA's `CPStrb2'` loading `CPReg.00`-`.07` the same statement as the C
  emulator's "ABMux0 latches the HIGH byte".
* **`SetRun` comes from data bit 0.** The BaseBoard latches `TSetRun` from
  `TCPBus.07`, and MSB-first over `.00`-`.07` makes `.07` the least significant
  bit. The C emulator tests `data & 0x01`. Same bit, from opposite directions.

**And two things the netlist adds, which are gaps in the C model rather than
disagreements:**

* **Function 1 is not a no-op.** The C emulator calls it "Clock" and ignores
  it. ContA's `CPStrb1'` latches three real control bits at i02:
  `CP=UseCPReg`, `ClrReady` and `GetTLink`, from `CPOut.2`, `.3`, `.4` and
  `.7`.
* **A REMOTE host can drive this bus instead of the local 6502.** Three
  SN74LS157 multiplexers on the BaseBoard (d06, e06, f06) select between
  `MCPBus`/`MCPABus`/`MCPStrb` and `RCPBus`/`RCPABus`/`SelStrb`, which arrive
  on AM2615 line receivers from the `ACP*` cable, under a software-set
  `AHasCP` latch -- "A has the Control Processor". The C emulator models only
  the local path, which is enough to boot but is not the whole interface.

One correction to an internal comment, worth having: `baseboard.c` says each
MIR byte's extra bit "rides on the SetSS line during the strobe -- this is a
hardware multiplex". It is the same physical RIOT pin, PB7, but it is not a
multiplex. The netlist fans it out: continuously to `MCPBus.08` -> `CPOut.8`,
the ninth bit of the nine-bit slot, and separately into an SN74LS175 at g07
which latches it into `SetSS'` on the Control strobe. Same pin, two
destinations, one of them registered.

Functions 4-7, the four microinstruction bytes, are strobed elsewhere -- ContA
a01 leaves Q0'-Q3' unconnected -- and are the obvious next test.

## The passive packages, and what each one turned out to be (2026-08-17)

Three of the `.lc` part types are not logic and cannot be cells, because what
they do is decided by what a BOARD put across them, not by the part. All three
are resolved in the generator instead, and in every case the wire list's
per-pin `{x,y}` coordinates are what settle the geometry.

| type | what it is | how it is resolved |
|---|---|---|
| `SIPpackage` | resistor pack, common pin to the rest | hold every other pin at the common's level; a net held by a pull-up pack AND a pull-down pack is a bias divider and gets neither |
| `AUGATCG16` | wire-wrap header, a field of jumper positions | a column of exactly two pins where one net has no other source; three-pin columns and contested targets are a CHOICE and are refused |
| `PLAT1816` | resistor platform, eight series resistors | pin N to pin 17-N, where the far net has no other source and the near one is driven by a digital part |

**`PLAT1816` was 24 dead address lines.** The pairing is not inferred: pin N
and pin 17-N share an x coordinate, unanimously, 122 pairs across the machine
with no exception. On MemX three platforms carry the map DRAM's address and
strobes through series damping resistors -- `TMapAd.0a` in on pin 1 and
`RTMapAd.0a` out on pin 16, and so on for 24 signals -- and with the part
unmodelled every one of those lines sat at zero.

The display boards' platforms are why the conditions are there. Theirs sit in
the video DAC's supply filtering, across `GNDBlue`, `RegVCCB`, `FilterVEEB`, a
DAC output and two references; passing a level through those would be inventing
a signal. What separates them is the DICTIONARY, not our modelling progress: a
digital part carries a `[G ...]`, `[FF ...]` or `[M ...]` summary and an analog
one carries none, and DispM's `DACBlue` comes off an MC10318, which has none.
24 series passes taken, 98 left alone.

That needed a fix in `sil_ecldict.py`, which had walked straight past the `#`
that starts the dictionary's third section: every behaviour summary in the file
was being attributed to whichever pin block came last, so only 3 parts had one
instead of 168. It was also parsing those bracket lines as pin clauses, which
invented gates named after pin numbers.

**A jumper target offered more than one source is a choice.** Each column is
two pins, so the two-pin rule fires on each -- but MemX's b13 offers
`RamA1orVCCa` a ground while b14 offers it VCC-47 and the map address line
`RTMapAd.1a`. The names say what that is: "RamA1 OR VCC", the size of RAM
fitted. Taking several at once asserted a contradictory configuration, that net
strapped to GND and VCC together. Contested targets are refused now and
counted, along with the three-pin columns.

## Fixed: the machine did not settle, and it was two latches that are not

`make -C verilog machine-test` failed the moment the BaseBoard genuinely ran:
Verilator's settle loop no longer terminated, at 500 iterations as readily as
at 100. Verilator named 40 circular combinational paths across ProcH, ProcL,
MemC, MemD, MemX, DskEth, DispY and the IFU -- and named the same 40 before and
after the machine came alive, so it could not say which mattered. The first
reading of that was that these were the design's own gate-delay tricks, which
would have meant substituting delays. That reading was wrong.

`tools/sil_loops.py` builds the graph directly, from the cell files the RTL is
emitted from: nets are nodes, and an edge exists where a package has a
combinational path from one pin to another. Two modelling mistakes accounted
for all of it, and the archive settles both.

**Six cells were transparent latches.** `F10145A`, `F10415A`, `F10470` and
`i2125` are memories whose write is LEVEL-sensitive -- while the write and chip
enables are asserted, the stored bit follows the data input -- and `MC10173`
and `SN74LS259` are latches. Written as `always @*` they pass a level straight
through, so every read-modify-write path in the machine was a combinational
loop, which is most of a datapath. F10145A alone is 405 packages.

Rewriting them on `sys_clk` with the part's own level as an ENABLE is not a
new idea here: it is the convention this design already uses for every clocked
element, and the same change its two DRAM cells already carried. `sys_clk`
heavily oversamples every signal on the board, so the behaviour while
transparent is the same to a fabric clock's precision -- and unlike a latch it
synthesises. That took the graph from 1,333 back edges to 40.

**A second theory was tried and is wrong**, and is recorded because it is a
plausible misreading of the dictionary. `F10016`'s carry out is a gate,
`~(&q & ~CE')`, which puts a path from a package's own count enable to its own
carry -- and the dictionary lists pin 4 only in an `[FF ...]` output list,
beside the four Q pins, with no `[G]` entry for the part at all. That looks
like a statement that the carry is clocked, and registering it removed three
loops.

It is not one. **An `[FF]` entry is a TIMING ARC from the clock, and a gate
after the register is folded into it** rather than given its own [G]. `S169`
shows this outright: RC' gets a SECOND [FF] block with a clock-to-output delay
of 30.8 ns against 16.5 ns for the Q outputs, and the extra 14 ns is the carry
gate. A synchronous counter's carry has to be combinational anyway, or a
cascaded stage counts a clock late. The change was reverted, and the machine
settles without it -- the latch fixes were the whole of it.

**Two false loops were in the analysis, not the design,** and are worth knowing
because they would mislead the next person the same way. Every MC10181 slice
reported a two-net cycle between its own result bits -- the pin numbers on the
left of `assign p6 = ...` were being read as pins the `always @*` block
consumes; a part's own outputs are not its inputs. And the BaseBoard's
`VCOPhase0`/`VCOPhase1` pair is the analog VCO's relaxation loop, which the
MPQ3303 substitution already replaces, so nothing else on those nets is a path.

**Four structural loops remain, and all of them settle.** The processor's own
multiplexer chain feeding back:

```
DMuxData -> DMData    (h17, MC10158)
DMData   -> Pdata.00  (f03, MC10164)
Pdata.00 -> MuxData2  (d11, MU10164)
MuxData2 -> DMuxData  (l24, MU10164)
```

and the same shape on DispM, DispY and MemD, where a counter's carry returns to
its own count enable through an MC10195:

```
StopWakeCount -> KillDWTWakeup   (e24 F10016, CE' to CO')
KillDWTWakeup -> StopWakeCount   (d24 MC10195)
```

None is a loop in operation. `StopWakeCount` is a wired-OR that d24 also drives
from two other gates, so either of those holds it; the multiplexer chain's
selects never route all four legs at once; and the carry gate only closes at
terminal count. They are left alone rather than cut, because a delay inserted
there would be an invention, and the machine converges with all four present,
over 200,000 cycles as readily as 20,000.

**`make -C verilog loop-check` is the gate**, and it costs a fifth of a second.
It reports every board's back edges against a list of the loops that are
understood, so a cell modelled as a latch is caught where it is written rather
than as a non-convergence on some other board tens of thousands of cycles
later. Reverting `cell_MC10173` to `always @*` makes it fail with ProcH and
ProcL named.

`cell-check` briefly gained a property from the second theory -- a pin listed
only under `[FF]`/`[L]` may not be computed from an input pin -- and it was
removed with the theory. `sil_check_cells.py` records why, so it does not get
re-derived.

## Reference: why `machine-test` is not a toggle count

The harness reports how many probed signals move, and that reads like a
progress bar for the cell library. **It is not, and treating it as one would
punish correct work.** Modelling four more gate types and fixing MC10105's
polarity against the datasheet took the count from 31 DOWN to 27: an undriven
net reads 0 and flaps with whatever else moves, while correct logic holds it
steady. Isolating it showed the polarity fix alone accounted for 2 of the
drop.

So the gate asserts the CLOCK instead -- all 24 nets the BaseBoard fans out
to the slots must toggle, by name, against the probe map the generator emits.
That cannot improve by luck, and if it fails nothing downstream can compute.
`tools/rtl_machine_check.py`, mutation-tested by killing the oscillator.

The toggle total is still printed, as information rather than a threshold.

## Task A -- fill in the cell library

The machine is assembled, self-clocking and gated; what stops it computing is
that 63 of 125 cell types are still skeletons with correct ports and no body.
`make -C verilog machine-test` asserts the clock still reaches every slot;
see the note above on why it does NOT gate on how many signals move.

The clock generator is done (MC1660, MC1690, and the VCO substitution) and so
is `MB7071H`, the 256x4 RAM that is the machine's REGISTER FILE -- ProcH h06
is `RbAdr`/`SelectRm'` (that is RM), i06 is `StkAdr`/`SelectStk'` (STK), four
packages per board for the 16-bit width, and the other 62 are MemC's cache
tags and DispM's colour tables. It writes synchronously and reads
combinationally, which is a distributed/LUT RAM and is what the part does.

Seven more landed: `SE10211` (11), `MC10161` (21, Binary to 1-8 Decoder Low),
`MC1668` (12, dual clocked flip-flop), then with the polarity settled
`MC10109` (21), `MC10212`+`SE10212` (40) and `MC1664` (19). Coverage
86.5% -> 89.8%. `MC1664`'s FUNCTION came from the rule rather than a
datasheet: all four of its gates have a single `o` output, and `o` is the
non-inverting sense, so every output is an OR -- which matches the MECL III
pairing where MC1662 is the NOR part.

`MC10117` (28) and `MC10180` (13) then landed from their data sheets, which
are filed in `DoradoDocs/datasheets/` -- see the README there. MC10117 also
settled EclDict's `u`/`v` role letters, which the OR/NOR rule does not reach:
`u` is the inverting output, `v` the non-inverting, exactly as `OUT`/`o` are.
MC10180's sheet gives the mode table, and it is worth knowing what the part
does -- a select input INVERTS its operand, so one adder does add, subtract,
reverse-subtract and negate, with the carry-in supplying the +1.

**The ALU is modelled and verified** (`MC10181`, 8 packages: two slices on
ProcH, two on ProcL for the 16-bit datapath, four more on the IFU). Written
as a 16-way mux straight from the datasheet's function table -- one row per
line, so it reads against the sheet -- which is also one LUT level on an
FPGA. `make -C verilog alu-test` checks it EXHAUSTIVELY against independent
expressions (`A + B`, `A - B`, `A & B` written directly, not the table it was
built from): 4,880 checks, and mutation-tested with six injected errors
(operand order, select order, carry sense, a swapped table row, mode ignored,
propagate using AND for OR).

Two things worth keeping from writing it:

- **The pin indices look reversed and are not.** EclDict names `D0` for pin
  10, which the datasheet calls `A3`. Xerox numbers bit 0 as the MOST
  significant throughout this machine, the same convention the PROMs use.
- **A confirmation of the architecture docs.** `CLAUDE.md` says ALUF is a
  4-bit pointer into ALUFM, "16 x 6 bits". Six bits per entry is exactly what
  this part takes: S0-S3, M and Cn. The netlist and the microcode
  documentation agree without either having been derived from the other.

One mutation was NOT caught, and that was right: gating the adder's carry-in
by M is dead code, because M already selects the logic result and forces the
carry out. The test found redundancy rather than a bug, and the cell is
simpler for it.

**Six more cells landed after the audit**: `MC10162` (10, the decoder's
active-high sibling), `SN74LS174`/`SN74S174` (16+28, hex D flip-flop -- PARC's
TtlDict gives the whole part in one line, `[FF 3>2, 4>5, ... : CLK 9 RS 1]`),
`MC10136` (9, the hexadecimal counter, whose four modes decode straight out of
EclDict's `SU'`/`SD'` naming), `SN74LS138` (7, the 3-to-8 decoder) and
`i2716` (8, the EPROM).

**The BaseBoard's EPROMs are LOADED, and the socket map is established.**

`make -C verilog proms` now fills them. The contents are NOT `firmware/` --
those five 1987 chip dumps look like the obvious source and are not usable, as
no image has a plausible 6502 vector triple in its tail (B-10 gives
NMI=0x0F00, IRQ=0x4F45) and nothing in this repository reads them. The C
emulator boots `chm/dorado/doradobaserom.mb!13`, so a small program
(`dorado/tests/baseboard_rom_dump.c`) dumps it through the emulator's OWN
loader and `tools/firmware_eproms.py` slices it into sockets. Whatever the
RTL's 6502 fetches is then byte-for-byte what the C one fetches.

The map was derived, not guessed, and it validates itself:

- The eight sockets are chip-selected by `Rom0'`..`Rom7'` from the SN74LS138
  at g11, whose select is `RSA.0/1/2`.
- Each socket's address pins take `MCA.10` down to `MCA.00` -- a clean 2K
  window. (PARC names those MSB-first: the dictionary's `A0` is pin 19, the
  sheet's A10. `cell_i2716` had that backwards until this was traced; a
  reversed address would have scrambled the image silently.)
- `RSA.0/1/2` are driven by no gate. They arrive on `c07`, an Augat STRAP
  BLOCK which also brings `MCA.11/12/13/14` out. With 2K parts the only
  strapping that tiles the space is RSA = MCA.11/12/13.
- So Rom<n> sits at 0xC000 + n*0x800, and `baseboard.h` puts the ROM area at
  exactly those 16K.

Then the ROM agrees: its vectors are NMI=0xF000, RESET=0xF3A7, IRQ=0xF2A2,
all inside the ROM area, and its 6,955 non-zero bytes fall in exactly four 2K
blocks -- C000, C800, F000, F800, which is the 1981 Hardware Manual's "4
EPROMs at F000, F800, C000, C800". Four chips in eight sockets.

**The 6502 does not run yet, and the reason is specific.**
`verilog/verilator/tb_baseboard.sv` instantiates the board alone and watches
the address bus: it never changes. The core itself is real (Holme's
netlist-derived 6502) and so are the RIOTs -- what is missing is the CLOCK AND
RESET CHAIN around them, every part of it still a skeleton:

| part | pkgs | role |
|---|---|---|
| `SN74LS74` | 3 | g08 makes `MCPreClk`, the 6502's clock; j08 drives `MCReset'` |
| `SN74LS04` | 5 | hex inverter, incl. `MCClk` -> `MCClk'` for the ROM decoder |
| `SN74LS163` | ? | e07 drives `Max`, which clocks g08 -- the chain runs back to here |
| `MC14521B` | 1 | 24-stage divider off `MCPreClk` |
| `SN74LS175` | 2 | latches on the reset path |
| `SN74LS259` | 1 | addressable latch on the reset path |

**That is the next task, and it is worth doing**: they are small standard TTL
parts, and with them the BaseBoard would execute its own boot firmware in RTL
-- the same code the C emulator runs, which makes it diffable the way the ALU
now is. `tb_baseboard` is already written to report it: it looks for the 6502
addressing 0xFFFC/D, which would mean the core, the decoder, the socket
mapping and the image are all right at once.

**What is left in the machine** is a short list, and none of it is a gate:

| part | pkgs | what it is |
|---|---|---|
| `CA3140` | 9 | **op-amp** -- analog, like the VCO; expect a substitution, not a model |
| `F100181` | 8 | the Fairchild 100K ALU on MemC -- active-low carries and an output enable, so it needs its own sheet |
| `MC10172` | 7 | |
| `AM2615` | 7 | |
| `SN74LS251` | 6 | |

## The cross-check works: the netlist and the C emulator agree

**This is what generating RTL from PARC's netlists was for**, and it now
returns a number. `make -C verilog alu-diff`:

```
tb_alu_vs_c: 10752 vectors from the C emulator, 0 mismatches
```

Four `cell_MC10181` slices chained into the 16-bit ALU -- the way ProcH and
ProcL build it -- fed vectors that `dorado/tests/alu_vectors.c` dumps from
`cpu.c`'s own `alu_op()`. All 21 operations the emulator implements, every
pair of 16 edge-case operands, both carry-in values. **Neither side was
derived from the other**: the cell is a transcription of Motorola's data
sheet, `alu_op()` was written from the Hardware Manual's Table 9, forty-five
years apart.

The encoding agrees too, which is a result in itself. The 6-bit ALUFM entry
decodes as **`{Cn, S3, S2, S1, S0, M}`** -- carry-in, the part's four select
lines, then the mode bit -- and under that mapping all 21 of `alu_op()`'s
cases match the data sheet's function table exactly, sixteen logical and five
arithmetic. `CLAUDE.md` describes ALUFM as "16 x 6 bits" without saying what
the six bits ARE; they are this part's control inputs.

Two things about how it is built:

- `alu_vectors.c` **includes `src/cpu.c`** rather than linking it, because
  `alu_op` is static. That is deliberate -- the test has to run the
  emulator's real code, not a copy that could drift. The Makefile rule links
  `LIB_OBJS` minus `cpu.o` to avoid duplicate symbols.
- It is the only test that exercises the **carry ripple between slices**.
  Four correct 4-bit adders still give a wrong 16-bit answer if the chain is
  wired backwards, and mutation-testing confirms it: reversing the chain
  gives 1,409 mismatches, reversing a slice's bit order 5,776, and corrupting
  one row of the function table 512.

**The obvious next targets** are the other places the two models overlap.
`memory.c` against MemC/MemD/MemX (the cache's four operations are already
generated as the `EC-1`/`EC-2` PROMs), and the shifter, whose masks the
LMASK/RMASK PROMs already cross-check.

The biggest remaining parts overall -- `DS3649` (32), `SN74S174` (28),
`SN74H04` (28) -- have ZERO packages in the eleven-board machine. They are on
IOTest, msa/PCMSA and Music. Skip them unless those configurations matter.

For the rest, order by package count -- `python3 tools/sil_netlist.py --all
chm/sil` ranks them -- and prefer the parts the processor boards use, since
that is where the C emulator can check the answer.

The incremental path is supported directly, if a smaller machine is easier to
reason about:

```
make -C verilog backplane MACHINE="--boards=ProcH,ProcL --module=dorado_proc"
```

That gives 76 internal nets and 164 ports, every one labelled with the board
it is waiting for.

The PROMs are done: `make -C verilog proms` writes both the per-PROM images
and the per-package slices, and the generator wires all 29 placed packages.
Six PROM images have nowhere to go yet, and each for a stated reason
(`--placement` prints them): the three display-timing PROMs sit on a small
board the Sil tree does not include, and `Mouse-Motion`/`Keyboard-Map` are
blown at IFU-i03/k05/l05 where **IFU Rev Ch has ordinary logic** -- by the
1981 manual the terminal microcomputer decodes the keyboard and mouse and
serialises them to 177034B (Table 24), so those belong to an earlier IFU.

## Task B -- test against the C emulator

This is the reason the whole approach is worth it: **we have a working
software Dorado to diff against**, and the two were derived from different
sources 45 years apart.

Cheapest first:

1. **PROM contents vs emulator behaviour.** Already demonstrated: LMASK/RMASK
   generated from PARC's BCPL match the shifter masks the C emulator computes
   from the Hardware Manual. The same trick is available for the memory
   system (`EC-1`/`EC-2`'s four cache operations against `memory.c`), the
   disk (a sector is four blocks), the keyboard (`Keyboard-Map` against
   `display.c`'s 61-key matrix, which has been checked three other ways), and
   now the ethernet: `EtherRcvr`/`EtherXmtr` are the receiver and transmitter
   state machines whose behaviour `dorado/src/ethernet.c` models at packet
   level. **Note the keyboard one is a layer question**: the PROM is what the
   IFU applies, `display.c` is what the terminal delivers, so a mismatch would
   be interesting rather than automatically a bug.
2. **Clock a board and compare a signal.** Once ProcH is instantiated, drive
   a known microinstruction and compare an ALU output against the C
   emulator's at the same point.
3. **Boot something.** A long way off, and the honest ordering is 1, 2, then
   worry about 3.

---

## Things that will bite, learned the hard way

- **`make -C verilog lint` after every change.** It is seconds, and it caught
  every structural error here -- including two of mine in one batch.
- **Verilator compiles inside `obj_dir`**, so relative `-CFLAGS` include
  paths need `../`.
- **A comment line beginning with the tool's own name is parsed as a pragma**
  (`BADVLTPRAGMA`). Do not start a comment with it.
- **Hand-written cells must declare every pin the BOARDS wire**, not just the
  ones the dictionary names -- p1/p8/p16 are power on a 16-pin MECL package,
  and omitting them is `PINNOTFOUND`.
- **Net names are not unique after naive sanitising.** `CTask.0` and
  `CTask=0` are different nets on ContA and both mapped to `CTask_0`; the
  mapping is injective now and verified collision-free, but the same trap
  waits in any new name mangling.
- **Cell port directions are global; net classification is per-board.** A pin
  that drives on MemC and is only sensed on IFU is `output` in the shared
  cell.
- **Check a transcription structurally, not by eye.** Every PROM generator
  has a property test for this reason, and they have now paid for themselves
  three times.
- **Then check the CHECK.** The Ethernet property checks passed on their
  first run and still missed three of fourteen injected errors. A gate that
  has never failed has not been shown to work; break the thing it guards and
  watch it go red.
- **Xerox files are CR-terminated.** `wc -l` reports 0 lines for every `.bcpl`
  and `.bp` in the archive, and a naive read gets one enormous line.
- **A sampled probe sees one clock phase.** The harness reads probes after
  `tick()`, which leaves the clock HIGH, so anything that follows the clock
  combinationally reads as a constant and looks dead. That is why the machine
  clock is divided from `sys_clk` rather than being `sys_clk`: the cells need
  to see an edge, and the observer needs to sample between them.
- **zsh does not word-split unquoted parameters.** `$FLAGS` reaches the
  program as ONE argument, and Verilator reports the whole string as an
  unknown warning. Use an array, or write the flags out.
- **Two passes over the same netlist must agree about what a driver is.**
  `classify()` decided port direction from the CELL's port directions while
  `emit()` decided on-board wired-OR from the WIRE LIST's `o` pins. On five
  nets across three boards the two disagreed, and the generator declared a
  module input and then assigned to it: `%Error-ASSIGNIN`. If two functions
  answer the same question, have one of them ask the other.
- **A checker can be the thing that is broken.** The port gate reported 514
  missing ports right after the fix landed; its regex matched `input|output`
  and the new direction is `inout`. Check a red gate against a hand count
  before believing it.
- **The comma goes before the comment.** Emitting
  `input wire X  // why,` puts the separator INSIDE the comment, where it
  vanishes. Generated Verilog with per-port comments needs the punctuation
  placed deliberately.
- **Verilator resolves a module by FILENAME.** `-I<dir>` finds `cell_X.v` for
  module `cell_X`, but board module `ProcH_m_Rev_m_Ce` lives in
  `ProcH-Rev-Ce.v`, so the top module cannot be linted with `-I` alone -- the
  board files must be passed on the command line. That is why the lint rule
  treats the top module separately.
- **zsh does not word-split unquoted parameters.** `$FLAGS` reaches the
  program as ONE argument, and Verilator reports the whole string as an
  unknown warning. Use an array, or write the flags out.

---

## The one open licence question

`verilog/vendor/riot/M6532.sv` is **CC BY-NC 4.0** -- more restrictive than
anything else here. DECISION: keep it, the project is not shipping
commercially, and the note exists so the constraint is known rather than
discovered. If it ever binds, the preferred fix is porting our own:
`dorado/src/baseboard.c` already models the 6532 well enough to run the real
BaseBoard EPROMs, so it is a validated spec written in this repository with
no third-party licence at all.
