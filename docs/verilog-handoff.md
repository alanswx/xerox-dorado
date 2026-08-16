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
python3 tools/sil_backplane.py --ports     # boards present the ports PARC states
make -C verilog machine-test  # THE GATE: the assembled machine clocks
make -C verilog backplane MACHINE=--boards=ProcH,ProcL   # any subset
```

| piece | state |
|---|---|
| Netlist reader + Verilog generator | 16/16 boards, 67,960 lines (+2,658 top, +4,599 cells), **all lint clean** |
| Cell library | 50 cells with behaviour, **84.6%** of 3,771 logic packages |
| 6502 | Andrew Holme's netlist-derived core (via jotego), wired into `cell_MCS6502` |
| 6532 RIOT | MiSTer Atari 7800's, patched for Verilator. **CC BY-NC** -- see `verilog/vendor/LICENSES.md` |
| PROMs | **26 of 26** generated, **29 packages wired into the RTL and read back correctly** |
| Harness | Verilator + Dear ImGui, builds, runs, `--headless` CI mode |
| Board port lists | **from PARC's own `.bp`**, 1,920/1,922 exact, 0 spurious |
| Backplane top module | **generated**, 11 boards wired by name, lint clean |
| Synthesisability | **no `inout`, no multiply-driven net, no gated clock** |
| The machine | **instantiated, and SELF-CLOCKING**: it generates its own clock |

`verilog/generated/dorado_backplane.v` instantiates eleven boards and wires
501 nets between them; `dorado_machine` resolves the 407 external ports and
`sim.v` runs it. **Nothing is injected: the machine makes its own clock.**
The VCO is substituted for a fabric-clock divider -- an analog oscillator has
no digital model -- and everything after it is the BaseBoard's own logic:
h05/g05 (MC1660) shape two anti-phase clocks, four MC1690s divide them into
`StartClockPulse'` and `EndClockPulse`, and j02 fans those out. All 24 clock
nets toggle, and six downstream signals with them (`MemWEa`/`MemWEb`,
`LoadEcOut'`/`ShiftEcOut`, `LargeHold`, `TWReq15`).

It does not compute yet: 75 of 125 cell types are still skeletons. `make -C
verilog machine-test` counts how many signals move (30 today) and is a FLOOR
that should rise as the cell library fills in.

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

## Task A -- fill in the cell library, starting with the clock generator

The machine is assembled, self-clocking and gated; what stops it computing is
that 75 of 125 cell types are still skeletons with correct ports and no body.
`make -C verilog machine-test` measures the effect directly: 30 signals move
today, and each cell that gains behaviour should raise that.

The clock generator is done (MC1660, MC1690, and the VCO substitution), so
order by package count -- `python3 tools/sil_netlist.py --all
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
