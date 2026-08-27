# Verilog from Sil: handoff

2026-08-18. This file is written to be picked up cold: "Where it stands" is
the state of the machine, "START HERE" is the open work, and "How to read
the rest" maps the sections below. Read `docs/verilog-from-sil.md` for the
background and why each decision was made.

---

## Where it stands

**A four-board Dorado runs microinstruction cycles, executes microcode out of
IM, and COMPUTES.** The write path and the datapath are both proven end to end
against the C emulator. Parity is the one thing left in the boot chain. Everything is
generated from PARC's own Sil wire lists, and every claim below is a gate you
can run.

### The boot chain, rung by rung

| rung | state | gate |
|---|---|---|
| BaseBoard 6502 boots from its own EPROMs | **works** | `baseboard-test` |
| BaseBoard drives the control-processor bus | **works**, matches the C emulator | `cpreg-diff` |
| All 36 microinstruction bits decode | **works**, matches the C emulator | `mir-diff` |
| A microinstruction lands in the MIR | **works**, matches the C emulator | `mirreg-diff` |
| The Control section executes cycles | **works** | `run-test` |
| Four boards, microinstruction on the datapath | **works** | `datapath-test` |
| A jammed Write-IM deposits into IM | **works** (mechanism + half-select) | `writeim-test` |
| ...with the DATA from CPReg | **works** | `operand-test` |
| ...with the ADDRESS from CPReg | **works** -- Link -> TNIA -> the array's address lines | `operand-test` |
| The machine SINGLE-STEPS microinstructions | **works** | `step-test` |
| PARC's SendViaMIR loads words into IM | **works** -- Boot0's inner loop | `sendmir` |
| PARC's BLOCK LOADER walks REAL MICROCODE into IM | **works**, and it MATCHES THE C EMULATOR -- 16 hunks, 128 half-words | `boot0-test` |
| **THE MACHINE EXECUTES MICROCODE OUT OF IM** | **works** -- free-running, sequencing, 1,242 cycles | `exec-test` |
| **THE MACHINE COMPUTES** | **works** -- Q takes 25 octal from CPReg, holds it, ALUFM[0] stores it | `compute-test` |
| ...and T loads through the ALU | **works** -- 1234 gives 1234, a55a gives a55a, exact | `compute-test` |
| **TWO OPERANDS: the ALU as the BOARDS wire it** | **works** -- all 24 entries of HM Table 9 match the C emulator, carry chain and all | `compute-test` |
| **RM, the per-task register file** | **works** -- four addresses written and read, and each lands where the address pins say | `compute-test` |
| **the MEMORY section is in a machine and clocked** | **works** -- seven boards; each memory board's local clock follows its `MemClkEnable'` | `mem-test` |
| the REAL firmware runs the machine | boots itself past power-up and pacifies its watchdog; reaches `SetManifold`, then ends up executing filler ROM | `firmware-probe` |
| **A WHOLE WORLD PRELOADS INTO IM** | **works** -- 2,148 microinstructions of AEmu.mb, every one read back | `exec-world` |
| ...through a field map DERIVED from the wire list | **works** -- 65 addresses, all 8 fields, against a CP-bus load | `boot0-test`, `im-map-check` |
| **ALUFM**, the sixteen ALU operations the world uses | **works** -- derived from ProcL e13/e14, confirms HM Table 11d | `exec-world9` |
| **IFUM**, the opcode decode tables | **works** -- 256 entries, layout confirms cpu.c's Table 20 bit for bit | `exec-world9` |
| **NINE BOARDS DISPATCH AN ALTO OPCODE** | **works** -- START, the IFU traps, RESTARTIFU, then AND1 and SKPC | `exec-world9` |
| ...and then it STOPS FETCHING | **open** -- `IfuMemRef` makes 6 transitions and the opcode changes ONCE | `exec-world9` |
| **IM PARITY, the long-open question** | **ANSWERED** -- ODD over the 17-bit half, array stores the COMPLEMENT | `im-parity-check` |
| **THE MACHINE RUNS WITH IM PARITY ENABLED** | **works** -- Error propagated on 0 of 400,000, Stop never set | `exec-parity` |
| **THE FAULT TASK SERVICES THE STACK UNDERFLOW** | **works** -- RepeatCur 0, task 15 runs, 25 addresses, longest run 3 | `exec-tasking` |
| **THE FAULT TASK RUNS ITS OWN HANDLER** | **works** -- TPC[15] set, task 15 runs BEGINENUMERATEMAP / IWRITEMAPFLAGS | `exec-init` |
| **PARC'S BOOT CHAIN RUNS** | **works** -- Initial calls Bootstrap's READBB, which waits for the control processor | `exec-boot` |

### Every gate

```
make -C verilog boards          16 boards + 4 machine configurations -> generated/
make -C verilog proms           proms/*.mem and the per-package images
make -C verilog cells           cell skeletons for a new part (never overwrites)

make -C verilog lint            every board and all four machines elaborate
make -C verilog case-variants   BACKPLANE_CASE_ALIASES re-derived from the .bp
                                files: merge a case variant only where every
                                board using it agrees on the pin
make -C verilog cell-check      cells wire the inputs PARC's [G] lists say they
                                do, and a both-sense gate's two outputs are
                                complements of each other
make -C verilog loop-check      no combinational feedback beyond the known ones
make -C verilog prom-test       PROMs read back what the machine expects
make -C verilog alu-test        the ALU matches its datasheet
make -C verilog alu-diff        the ALU matches the C EMULATOR, 10,752 vectors
make -C verilog cpreg-diff      the BOOT INTERFACE matches the C emulator
make -C verilog mir-diff        all 36 microinstruction bits match
make -C verilog mirreg-diff     a jammed microinstruction reads back off the MIR
make -C verilog run-test        THE MACHINE RUNS -- it executes cycles
make -C verilog datapath-test   four boards, microinstruction on the datapath
make -C verilog writeim-test    a jammed Write-IM deposits into IM
make -C verilog operand-test    ...with its DATA and ADDRESS from CPReg
make -C verilog step-test       the machine SINGLE-STEPS microinstructions
make -C verilog sendmir         PARC's SendViaMIR loads words into IM
make -C verilog boot0-test      PARC's BLOCK LOADER walks real microcode into IM
make -C verilog exec-test       THE MACHINE EXECUTES MICROCODE OUT OF IM
make -C verilog compute-test    THE MACHINE COMPUTES -- PARC's ALU prologue:
                                25 octal from CPReg into Q, held, then stored
                                into ALUFM[0]
make -C verilog baseboard-test  the BaseBoard's 6502 BOOTS
make -C verilog muffler-test    exactly one address selects the BaseBoard
make -C verilog strap-test      each board's SIP straps give the I/O address
                                and task number its schematic states
make -C verilog osc-test        the three crystal positions run 1 : 2 : 5
make -C verilog task-test       the TASK PRIORITY ENCODER matches the C emulator
make -C verilog taskrun-test    BNT loads in a running machine, THE MACHINE
                                SWITCHES TASKS, and each task keeps its own
                                PC and Link
make -C verilog refdecode-test  the MEMORY SECTION's front door -- ASEL 0-3 is
                                a storage reference, against the C emulator
make -C verilog memrun-test     THE MEMORY RUNS: DRAM cycles from PARC's own
                                startup, the microcode makes a Flush, a Store
                                dirties a line, THE CACHE MISSES, <-Map writes
                                an entry and MapTrouble clears, a storage cycle
                                starts
make -C verilog mem-test        the memory section is in a machine and clocked
make -C verilog msa-test        A WORD GOES INTO THE STORAGE ARRAY: the msa
                                board standalone, where the bench owns the DRAM
                                strobes -- Sout registered, translated to TTL,
                                and written into the MK4096s
make -C verilog ifu-test        THE IFU is in a machine: IFUM is 27 x F10415A =
                                1024 x 27 bits (256 opcodes x 4 instruction
                                sets, HM Table 18), its slot clock reaches it,
                                and IfuData resolves
make -C verilog ifufetch-test   THE IFU COMES OUT OF HOLD when real microcode
                                runs -- released on 3000 of 3000 samples,
                                where the static bench holds it
make -C verilog storage-test    THE STORAGE ARRAY is in a machine: PARC's msa
                                board with its 144 MK4096 DRAMs, its address
                                path inverting through the SN74H04s, and the
                                memory size arriving over pin E55 from the
                                board rather than from the testbench
make -C verilog converge-test   the assembled machine SETTLES and its clock runs
make -C verilog machine-test    the same under the imgui harness

make -C verilog startseq        DIAGNOSTIC, not a gate: PARC's boot sequence
                                replayed, printing the Control section's state
make -C verilog parity-probe    DIAGNOSTIC, not a gate: do PARC's IRTable
                                entries satisfy our IM parity generators? (no)
```

Plus, outside the Makefile: `tools/dorado_proms.py --check` (26/26 PROMs
property-checked), `--placement` (which package holds which PROM), and
`tools/sil_backplane.py` / `--ports` (what the backplane is, measured).

### The machine configurations

`tools/sil_backplane.py` wires any subset of boards by name. `make boards`
emits six:

| module | boards | used by |
|---|---|---|
| `dorado_backplane` | the eleven of a working monochrome machine | `machine-test`, the imgui harness |
| `dorado_baseboard` | BaseBd alone | `baseboard-test` |
| `dorado_control` | ContA + ContB | `run-test`, `mirreg-diff` |
| `dorado_proc` | ContA, ContB, ProcH, ProcL | `datapath-test`, `writeim-test`, `operand-test` |
| `dorado_boot` | BaseBd + ContA/B + ProcH/L | `firmware-probe` |
| `dorado_mem` | ContA/B, ProcH/L, MemC/D/X | `mem-test`, `memrun-test`, `refdecode-test` |
| `dorado_storage` | `dorado_mem` **+ msa** | `lint` -- the storage array in a machine |

**THE MSA DID NOT NEED WRITING, IT NEEDED WIRING (2026-08-23).** The task
list said "write the behavioural MSA storage array"; that premise was wrong.
`chm/sil/msa-Rev-Bg.dm!1_/msa.wl` is PARC's own storage-array board and the
generator has been emitting `generated/msa.v` all along -- 291 packages, 768
nets, **144 MK4096 DRAMs** (4K x 1), and every part type it uses already had a
cell. What was missing is that no machine configuration instantiated it.

It hangs off the memory section exactly as it should: of its **66 backplane
nets**, 38 are shared with MemD, 23 with MemX, 4 with MemC, and **zero** with
ContA/ContB/ProcH/ProcL. The MemD interface is the storage data path itself --
`Sin.00-15` and `Sout.00-15`, the 16-bit word in and out, plus `EcIn`/`EcOut`
for ECC.

**And two things had to be fixed before it could work.**

**The coverage check compared the wrong name.** `known_cells` reads part names
off the FILENAMES, which are already sanitised (`cell_MK4096P_6.v`), while the
check used PARC's raw name (`MK4096P-6`). So any part whose name needs
sanitising was marked "NO MODEL" in the generated Verilog -- and counted as
missing -- while being correctly instantiated all along. **154 false comments
across 8 boards, 144 of them the MSA's DRAMs**, which have a complete model.
Fixed by comparing `vpart(ptype)`.

**`SN74H04` was a skeleton, and it is what drives the DRAM address lines.**
All 28 of its packages are on the MSA. The board takes its multiplexed
address off the backplane as `TtlA0'`..`TtlA6'` and inverts it here into
`A0a0`..`A6a0`, which fan out to the 144 MK4096s -- so unmodelled, **every
address line on the storage board read 0 and no location could be selected**.
The netlist proves the function without a data sheet: every package takes a
PRIMED net in and drives the UNPRIMED one out, in the standard 7404 pin pairs.

With both fixed, **the MSA is 265/265 logic packages modelled -- 100%**.

**The storage data path, mapped end to end (2026-08-23).** One bit followed
through the wire list, which is what `storage-test` and any future write test
have to sequence:

```
Sout.00-03  -> b01 D0-D3   (MC10176 hex D FF, clk msa01.sil+4)
            -> msa04/05.sil+*  -> the SN74166 shift chain
            -> f01 (MC10176, clk msa01.sil+8, ECC on EcOut.0/4)
            -> f02 (MC10125 ECL->TTL)  -> MK4096 pin 2, DIN

MK4096 pin 14, DOUT -> ... -> c01 D-inputs
            -> c01 Q (MC10176, clk msa01.sil+3) -> Sin.00-03, EcIn.0
```

**`Sout` is the write side and `Sin` the read side** -- the MSA drives `Sin`
while MemD drives `Sout`, which is worth stating because the names do not give
it away. And the word is **not presented in parallel**: it is registered, then
shifted.

**A WORD NOW GOES INTO THE ARRAY (2026-08-23).** `msa-test` drives the board
**standalone**, where the bench owns the DRAM strobes -- `storage-test` cannot,
because in a machine they come from MemX and need a real memory reference. The
sequence is: present `Sout`, pulse `LoadSoutE'` with `Mod0StrEn'` low to
register it, then a DRAM cycle -- row on `RAS'` falling, column on `CAS'`
falling, `WE'` low for an early write. Measured:

```
Sout -> b01 Q (msa04.sil+32/33) = 11 -> b02 (msa04.sil+8) = 1 -> b05 DIN = 1
writing 0000 -> DIN=0, 0 cells set        writing FFFF -> DIN=1, 1 cell set
```

Exactly one cell, at one location.

**AND IT COMES BACK.** The read path is the shifted one, and the '166 model
loads `q[7]` from pin 2 -- which is b05's DOUT -- so the bit is at the serial
output the moment the load happens, no shifts required:

```
b05 DOUT=1 -> SLa=0 (load) -> a13 Qh (msa04.sil+29)=1
           -> c02 (msa04.sil+31)=1 -> Sin.00=1

round trip: wrote 1, read Sin.00=1 ; wrote 0, read Sin.00=0
```

The zero case is there so the gate cannot pass on a stuck-high output.
Mutation-tested four ways -- breaking the MC10125 translator, disabling the
MK4096's write, disabling its read, and stopping the SN74166 loading -- all
caught.

**`LoadSinE` goes low then high.** The other way round leaves `SLa` high, the
'166 stays in **shift** mode and never takes the DRAM outputs, and the read
returns 0 while `b05`'s DOUT is plainly 1 -- which reads as a dead read path
and is a polarity mistake in the bench.

**Probe a DATA bit, not an ECC bit.** The first attempt watched `u_b04` and saw
a permanent 0 that looked like a dead write path. b04 is an **ECC** DRAM, fed
from f01/f02 off `EcOut`, which the bench was holding at 0. `u_b05` is the data
bit carrying `Sout.00`, and it was working all along. The two sit side by side
in the same package row.

**And the two sides are not symmetric -- the shift registers are on the READ
side only.** An SN74166 is parallel-in / serial-out (pin 15 SH/LD', pin 7
clock, pin 13 serial out), and a13 takes its eight parallel inputs **straight
from eight MK4096 DOUT pins** (`msa04.sil+1` is b06 pin 14), then shifts them
out through c02 (MC10124 TTL->ECL) into c01, which drives `Sin`. Twenty of
them at eight bits each is 160 lines for the 144 DRAMs plus ECC.

The **write** side has no shift register at all: b01 registers `Sout` and b02
(MC10125 ECL->TTL) drives the DRAM DIN pins directly. **So getting a word in
is a register-and-strobe; only reading it back needs the '166 load-then-shift
sequence.** That asymmetry is the useful fact for the next rung -- the hard
half is the read.

**Those register clocks are the board's own -- and the board needs its slot
clock.** b01 clocks off `c1`/`c2` and c01 off `SO`, and all three are internal
nets driven by MC10176/MC10210 packages on the MSA itself: an on-board
sequencer fed by MemX's `LoadSinE`/`ShiftSinE`/`LoadSoutE'`/`ShiftSoutE` --
**but also, and first, by `CLK.ms0Even'`, the MSA's own slot clock.**
Everything on that sequencer traces back to it.

**A first version of `tb_storage` did not drive it**, measured 0 edges on both
registers, and concluded the board was correctly quiet until a reference ran.
It was not: the storage board simply had **no clock**, and an unclocked board
makes every one of its signals look properly gated. That is the same trap
`dorado_mem` sets with `CLK.mc'`/`CLK.md'`/`CLK.mx'`, one board later. Driven,
the registers take **1250 and 2500 edges**, and the gate now asserts edges
rather than mere definedness. Mutation-tested: tying the slot clock low is
caught.

**And wiring it in found a seventh two-spelling backplane line.** MemX spells
it `ChipsAre64K`, the storage board `ChipsAre64k`, and both sit on pin **E55**
-- one wire, and the case rule applies. That is the very signal `tb_memrun`
has to drive by hand because "there is no MSA in this configuration".

Its neighbour is **recorded but deliberately not merged**: MemX's
`ChipsAre256/16K` and the MSA's `ChipsAre16k` are both on **E54** and are
plainly the same signal, but they differ by more than capitalisation, and this
table's discipline is that a NAME is the connection. PcMsa puts a *third* name
on the same pin (`ChipsAreA`), which is what a pin-based merge would have to
reconcile.

**The table now checks itself.** Its comment had promised
`sil_backplane.py --case-variants` long before that flag existed; it exists
now, is a make target, and re-derives the whole table from the `.bp` files --
10 case-variant groups, 7 agreeing on a pin (exactly the table), 3 not. It
must read the `.bp` files **raw**: `load_backplane()` returns names already
through `canon_net`, so a first attempt using it pronounced all seven aliases
stale, including `PrHold` -> `PRhold`, which the wire lists document with three
pins of evidence.

**And it corrected the comment it was written to confirm.** The old text said
`IOIn'` (E70) vs `IOin'` (E71) and `IOOut'` (E71) vs `IOout'` (E74). Measured
from the files:

```
IOIn'   DispM, DispY            E71
IOin'   DskEth, Music           E71   <- SAME pin as IOIn'
IOin'   ProcL                   E70   <- the odd one
IOOut'  DispM, DispY            E74
IOout'  DskEth, IOTest, Music   E74   <- SAME pin as IOOut'
IOout'  ProcL                   E71   <- the odd one
```

The spellings mostly **agree**; it is **ProcL** that sits on different pins
from everyone else. The group still must not be merged -- three pins are in
play -- but not for the reason given.

`make -C verilog backplane MACHINE=--boards=ProcH,ProcL` builds any other.

| piece | state |
|---|---|
| Netlist reader + Verilog generator | 16/16 boards, all lint clean |
| Cell library | **97** cells with behaviour, covering **97.7%** of the eleven-board machine's logic packages |
| 6502 | Andrew Holme's netlist-derived core (via jotego), in `cell_MCS6502` |
| 6532 RIOT | MiSTer Atari 7800's. **CC BY-NC** -- see `verilog/vendor/LICENSES.md` |
| PROMs | **26 of 26** generated, 29 packages wired in and reading back correctly |
| Board port lists | **from PARC's own `.bp`**, 1,920/1,922 exact, 0 spurious |
| Synthesisability | **no `inout`, no multiply-driven net, no gated clock** |
| Harness | Verilator + Dear ImGui, `--headless` CI mode |

Of the 64 packages still unmodelled, **42 are analog or mixed-signal** (op-amps,
comparators, a DAC, a transistor array, an ADC, analog muxes) and belong as
substitutions if at all. The 22 digital ones need datasheets this repository
does not hold: `F100181` (8, the MemC ALU), `MC10163`, `MC10182`, `MC10179`,
`F9401`.

## A WHOLE WORLD IN IM, and the machine dispatches an Alto opcode (2026-08-26)

`make -C verilog exec-world9 EXECARGS='+cycles=200000'` preloads all three
memories a microcode world needs and runs it on nine boards:

```
tb_exec: PRELOAD -- 2148 microinstructions written, 2148 read back, 0 wrong
tb_exec: PRELOAD -- 16 ALUFM entries written, 0 wrong
tb_exec: PRELOAD -- 256 IFUM entries written, 0 wrong
tb_exec: IFU -- IfuMemRef 6 transitions, IfuMemAck 6, opcode J changed 1 times
tb_exec: 14 distinct IM addresses executed; last TNIA=0fc; longest run on one address=6233
```

**Why preload at all.** The control-processor bus is the REAL loader and
`boot0-test` gates it; but sixteen hunks already cost a couple of million
fabric cycles, so 4,096 microinstructions of it will not simulate. Preloading
skips nothing unproven -- provided the map is right, which is the point of the
next paragraph.

**The map is DERIVED and GATED.** `tools/sil_im_map.py --verilog` emits
`verilog/generated/im_preload.vh` from ContB's, ProcL's and the IFU's own wire
lists. `boot0-test` then checks it against a control-processor-bus load of the
same microcode: 65 addresses, all eight fields, 0 wrong. Three mutations of the
generator are caught -- a swapped bank ordering (23 wrong), LSB-first field
numbering (50), and using the whole address as the index (49).

### The three memories

| memory | where | size | how the layout was fixed |
|---|---|---|---|
| IM | ContB, 144 F10415A | 4096 x 36 | bank = `{addr[11], addr[0]}`, EVALUATED from the CS gate polarities |
| ALUFM | ProcL e13/e14, 2 F10145A | 16 x 6 | data pins come off the B bus; confirms HM Table 11d |
| IFUM | IFU, 27 F10415A | 1024 x 27 | two write enables = the .MB's two words; confirms cpu.c's Table 20 |

**Knowing WHICH BITS pick a bank is not knowing which VALUE picks which bank.**
`CS0'..CS3'` are net NAMES; nothing says `CS0'` is the corner where both
address bits are 0. The generator evaluates the two MC10101s' gate polarities
-- d21 puts `RA.11a` through OR into CS0'/CS2' and NOR into CS1'/CS3', d22 puts
`RA.00a` through OR into CS0'/CS1' and NOR into CS2'/CS3' -- and gets exactly
one selecting value per bank.

**ALUFM confirms HM Table 11d from the netlist alone.** `ALUFdec.0..5` are
`B.08, B.11, B.12, B.13, B.14, B.15` in order: the entry's top bit is the ALU's
CARRY IN and is NOT part of a contiguous field. Three independent
confirmations, none derived from another -- `cpu.c` spells out the same
correspondence in LSB order; ALUFM[0] in AEmu.mb is 0x15 = **25 octal**, the
value PARC's own ALU prologue stores in `tb_compute`; and ALUFM[e] is 01 =
"NOT A", HM Table 9.

**IFUM's two halves are two write enables**, `DecHi'` and `DecLo'`, which is
why a .MB stores two words per entry. Comparing bit positions against `cpu.c`
settles which is which, and the agreement is exact and complete: `DecLo'`
carries all sixteen bits of `ifu_decode_lh`'s Table 20 layout (Sign 00, IPar
01-03, Length' 04-05, RBaseB' 06, MemB 07-09, TPause' 10, TJump' 11, N 12-15),
and `DecHi'` carries the eleven bits `cpu.c` masks as `ifum_lo & 0x07FF`
(TwoAlpha 05, InstrAddr 06-15). **The names cross over -- DecLo' holds what
the C emulator calls `ifum_hi` -- and the bits do not.**

### What the machine does, and what it does not

On FOUR boards the world spends all 124,994 microinstructions in the trap
vectors at the bottom of IM. That is correct for a machine with no memory
section and no IFU, not a bug.

On NINE it runs START, passes through `AEMUIFURAMPE` / `AEMUIFUFGPARITY` /
`AEMUIFUMAPFAULT`, reaches `RESTARTIFU`, and dispatches into **`AND1`** -- a
real Alto-emulator opcode implementation. Loading IFUM adds a second,
**`SKPC`**, which is how you know the dispatch through IFUM is real.

**IT IS NOT RUNNING A PROGRAM, AND THE PROBE SAYS SO PRECISELY.** Over 12,500
microinstructions `IfuMemRef` makes SIX transitions and the decoded opcode `J`
changes ONCE. The IFU makes a handful of references at startup and then stops
fetching, so the emulator dispatches on one stuck opcode and loops -- 6,233
microinstructions on a single address. Varying the seeded memory pattern
changes nothing now, where before IFUM was loaded it did.

**IT IS ALSO NOT A HOLD**, which is the obvious first theory and is wrong.
Sitting on one address looks exactly like the memory section freezing the
processor, so the whole chain is printed -- `Hold`, `PRhold`, `CBHold`,
`IfuHold`, `IOHold`, `MXHold`, `DisHold`, `CHoldReq`. All eight are 0 for the
entire run.

### ANSWERED: it was PARITY, twice -- and one of them is the first parity
### agreement between the two models

**The IFU was sitting in a permanent RAM parity error.** `RamPe` high on
200,000 of 200,000 samples, `SawRamParityErr` latched on 199,523.

**THE .MB DOES NOT CARRY IFUM PARITY.** The real machine computes the three
IPar bits in its LOAD microcode -- `cpu.c` cites `ifuRamSubrs.mc:ifuAddParity`
-- and 248 of AEmu's 256 entries FAIL `cpu.c`'s own `ifum_parity_ok` as stored,
which is why the C emulator only checks it behind `DORADO_IFUM_PARITY_TRAP`. A
preload that copies the stored bits hands the IFU 248 bad entries. Computing
them per HM Table 20's three disjoint even-parity groups takes `RamPe` and
`SawRamParityErr` to **ZERO**.

**That is the first parity agreement established between the two models.** The
RTL's parity generators come from PARC's wire lists; `cpu.c`'s grouping comes
from `ifuRamSubrs.mc` and the manual. Neither was derived from the other, and
feeding the RTL a word whose parity satisfies `cpu.c` makes the RTL's error
signal go clean. **IM parity is the same question** -- the machine still runs
only with the enables cleared -- and this is a tractable second instance of it.

**Then FG parity, over the instruction BYTES -- and this one is only PARTLY
settled.** The cache word is 16 data bits plus TWO parity bits, D.16 and D.17,
one per byte. Odd parity per byte -- D.17 over D.08-15, D.16 over D.00-07 --
minimises `FGParityErr` identically (176 residual) across six different
constant patterns: 52045, 49155, 0, 65535, 21845, 4660.

**IT IS NOT ESTABLISHED AS THE HARDWARE CONVENTION, and chasing it is what
showed why.** Three structural hypotheses were raised and all three REFUTED
from the netlist and from measurement:

- *The cache's two columns wire-OR different words.* MemD's a03 and d03 both
  drive D.00 but are separate 4096x1 arrays -- a03 on `Dad0.10-12` with enable
  `D0ACE'`, d03 on `Dad1.10-12` with `D1ACE'`. Refuted by counting: D0 only on
  199,603 samples, D1 only 0, BOTH 0, addresses never differ. They are
  perfectly exclusive.
- *The F/G mux takes the byte from one register and the parity from the
  other.* Refuted structurally: `i22` selects `FG.8` from
  `{G.16, F.16, G.17, F.17}` under the SAME `PcFG.15`/`GDv'` as the eight data
  muxes, so parity tracks both the byte choice and the F/G choice.
- *`G.16`/`G.17` are never loaded.* Refuted: `i21`, an F10016, loads them from
  `F.16`/`F.17` on the same `GLd'` that loads the data bits.

**What finally gave it away is that the winning assignment DEPENDS ON THE
DATA.** With `+cprog` giving each line its own word, the plain rule is clean
for a byte-swapped word (`+cprogfn=3`) and dirty for the plain one, and the
reverse for `+cprogfn=0`; `+cprogfn=1` is nearly clean under both. No real
parity convention can do that.

**So the counter is not measuring what it looks like.** `IfuMemRef` makes TWO
to SIX transitions over a whole run -- there are essentially no fetches -- and
a count of samples where a combinational signal sits high over 200,000 cycles
is an IDLE LEVEL, not a per-fetch check. Contrast the IFUM parity above:
`RamPe` went to EXACTLY ZERO, and IFUM entries are read continuously, so there
the level is meaningful. **A level is not an event. Before believing a sample
count, check that the thing it counts actually happened.**

Settling FG parity needs the error sampled AT a fetch, which needs a machine
that fetches -- so it is downstream of the same question, not beside it.

With both clear the machine reaches three addresses it had never touched --
octal 100, 435 and 464, the last of which `mbdis` names `CVEND` and whose ASEL
is `Fetch<-RM/STK`. It is executing microcode that issues storage references.

**HOW IT WAS FOUND, because the order mattered.** The chain was measured from
the far end inward. The byte path from the cache (MemD `e06` -> `f22` -> `f23`
-> `FG`) MOVES in a running world -- `Fclk'a` 10 edges, `GLd'` 4, `FG` changed
7, where `tb_ifufetch`'s synthetic loop got 0 for all three -- and it stops in
lockstep with `IfuMemRef`. So nothing downstream was broken and memory was not
failing to serve; the IFU had stopped ASKING. The run visits `AEMUIFURAMPE`
early, which named the suspect.

### ANSWERED: it is a HOLD, and the stack pointer is at zero

The chain is measured end to end, every link a counter over 400,000 samples:

| link | evidence |
|---|---|
| `StkP` = 0x00 | 399,435 samples (and 0x3f for 565 at the start) |
| ProcL asserts `PrHoldReq` | **399,435** -- exactly co-extensive with StkP=0 |
| MemC latches `MiscHold` | 399,419, sixteen samples later = one microinstruction |
| `Hold` on the backplane | 399,419, fanned out as PRhold/CBHold/IfuHold/IOHold/MXHold, all the same count |
| ContA f20 makes `RepeatCur` | 399,419 |
| the MIR holds, TNIA frozen at 0x040 | 12,481 microinstructions on one address |

**`StkP = 0` is stack UNDERFLOW.** ProcL j20 (an MC10117) drives `PrHoldReq`
from `StkP.6/7` and the RSTK field, which is the Dorado's stack
overflow/underflow hold: a stack operation that would run off either end holds
the processor instead of corrupting STK.

**And it sustains itself.** MemC c24 ORs `PrHoldReq, ExtHoldReq, CHoldReq,
WantCR'` and **`MXHold`** -- and `MXHold` comes from `Hold`. So once set the
hold feeds its own request. The reset, `DisHold`, is a MODE BIT in a control
register (k08, clocked by `LdMcr'` from the MAR), not a per-cycle clear, so
nothing releases it on its own.

**`MDhold` and `RefHold` are both 0**, so the memory section is not waiting for
data or refresh -- the processor is holding itself.

**THE EARLIER "NO HOLD ANYWHERE, EVER" WAS STALE AND THE METHOD WAS WRONG.**
That reading came from printing the eight hold signals at TEN INSTANTS and
seeing zeros -- true of the machine BEFORE IFUM was loaded and the parity
fixed, and long superseded by the time it was being quoted. Holds are counted
now, not sampled. **Sampling a level at ten points is not measuring it**, and a
conclusion drawn that way needs re-taking whenever the machine changes.

### RESOLVED: StkP = 0 is CORRECT, and the RTL is more faithful than the C emulator

`cpu.c`'s own transcription of HM Table 6 settles it:

> `RSTK[0]`: 0 = no underflow check; 1 = underflow if StkP originally 0 OR
> finally 0. Underflow / overflow set StkUnd / StkOvf flags ... and **would HOLD
> + wake fault task 15 on real hardware -- we just track the flags**.
>
> `StkP[0:1]` selects one of 4 stack regions (each 100 octal words).
> `StkP[2:7]` is the per-region offset, valid range 1..77 octal.
> **`StkP[2:7] = 0 denotes empty stack.`**

So offset 0 is the documented EMPTY-STACK sentinel, not a wrong value, and the
hold is the documented hardware response. The measurements line up exactly:

- StkP powers up at `0x3f` -- region 0, offset 63, the top of the region.
- At cycle 565 it reaches `0x00` -- region 0, offset 0, EMPTY.
- The instruction there carries **RSTK = 8**, i.e. `RSTK[0] = 1`: the underflow
  check ENABLED, which is precisely the condition Table 6 names.
- ProcL j20 gates `PrHoldReq` on `StkP.6/7` and the RSTK field, and asserts.

**Our RTL does what the manual says. The C emulator explicitly does not** --
"we just track the flags" -- which is a deliberate simplification there and a
divergence worth knowing about when the two are compared.

**And the hold was permanent for a reason the bench controlled.** The manual's
response is HOLD *and wake fault task 15*; task 15 is what services the
underflow and releases it. The bench started the machine with `Return#`, which
is `TaskingOff,Return` -- its FF is `0o142` = FA 1, FB 4, FC 2, which `cpu.c`
names TaskingOff -- so the fault task could never run.

### AND WITH TASKING ON, IT DOES

FC 3 is TaskingOn. Re-encoding `Return#` with FF = `0o143` through the encoder
that reproduces all eight of PARC's IRTable entries byte for byte gives
`70 13 E1 4A 43`, parity included, re-decoding to the same fields with only FF
changed. `make -C verilog exec-tasking`:

| | tasking OFF | tasking ON |
|---|---|---|
| `RepeatCur` | 399,419 of 400,000 | **0** |
| CTask occupancy | t0 = 400,000 | t0 = 181, **t15 = 399,819** |
| distinct IM addresses | 15 | **25** |
| longest run on one address | **12,481** | **3** |

The fault task runs, the hold goes away, and the machine sequences instead of
freezing. And what it executes are real Alto opcode handlers -- `mbdis` names
`LDAIZ`, `JMPI2`, `DOCRYS`, `SKP1C`, `TRAP17` and `JSRIIX`, the last of which
has ASEL = `Fetch<-T`, a genuine storage reference.

**This is the manual's mechanism working end to end in generated RTL**: a stack
operation on an empty stack holds the processor, wakes the fault task, and the
fault task clears it.

### But the fault task is requested from RESET, and never released

`TWReq.15` -- MemX's fault-task wakeup -- is high on **400,000 of 400,000**
samples. So with tasking on the machine switches to task 15 at cycle 181 (as
soon as TaskingOn takes effect; HM p.27 gives it a two-instruction delay) and
never comes back:

```
PER TASK -- t0:2 addresses  t15:23 addresses
```

**Task 0 executes TWO instructions.** And the 23 addresses task 15 executes are
ALTO EMULATOR handlers -- `LDAIZ`, `JMPI2`, `DOCRYS`, `SKP1C`, `TRAP17`,
`JSRIIX` -- not a fault handler, which means `TPC[15]` was never initialised and
the fault task is running whatever code it landed on, in a 23-address cycle.

**Two candidate causes tested and BOTH ELIMINATED:**

- *An empty Map entry failing its parity check.* `tb_readback` documents that at
  length as the cause of `MapTrouble`, so its 21-array MemX map seeding was
  lifted into this bench. `TWReq.15` is unchanged: 400,000 of 400,000 with the
  seed and without it (`+nomapseed` keeps the control).
- *The wrong entry point.* AEmu has named entries -- `START` 0, `STARTMB` 53,
  `AEMUNEXT` 51, `BOOT` 652, `STARTEMULATOR` 656 (octal) -- and `BOOT`/
  `STARTEMULATOR` looked far more plausible than 0. Sweeping all five with
  `+start=`, **0 is the best of them** (25 addresses, longest run 3); every
  other lands back in the 0x0c4 spin with 12,490+ on one address. START is the
  right entry.

### FOUND: `Faults` is a TERMINAL COUNT, and the counter sits at 0000

MemX h03 is an MC10104 (quad AND) wired

    TWReq.15 = (Faults & WakeEnable) | (WakeEnable & StkWake)

`StkWake` -- the stack wakeup HM Table 6 names -- reads **0** all run, so it is
the `Faults` term. And `Faults` is MemX **k09 pin 4**, which on an F10016 is the
**TERMINAL COUNT** output: `~(&q)`, LOW only at `1111`.

Its four parallel inputs are all `TrueBD`, a hardwired true. So the design is:

    LOAD 1111  = no fault          (PE' = `_FaultInfoDly'`)
    COUNT      = a fault reported  (CE' = `ReportFault'`), reaching 0000

**`Faults` is high because the counter is at 0000, not because a fault is
currently happening.** Measured: it reads `0001` by the time the run starts, so
ONE FAULT IS REPORTED DURING STARTUP -- and nothing ever reloads it, because on
real hardware the fault task reads the fault info and the load re-arms it, while
here the fault task runs emulator code (`TPC[15]` was never initialised).

**Presetting it proves the chain.** `+faultinit` sets k09 to `1111` *after*
startup -- an `initial` is too early, the counter counts away from it:

| | default | `+faultinit` |
|---|---|---|
| `TWReq.15` | 400,000 of 400,000 | **108,939** |
| first task switch | cycle 181 | cycle 661 |
| task 0 addresses | **2** | **20** |
| task 15 addresses | 23 | 10 |
| `RepeatCur` | 399,419 | 72,635 |

The emulator task actually runs. It is not a clean state yet -- the stack
underflow at cycle 565 still holds it intermittently and the fault task still
services it with the wrong PC -- so `+faultinit` is a diagnostic, not the fix.

### And StkP is LOADED with 0, not decremented into it

`+stkinit` presets the stack pointer to region 0, offset 32 -- ProcL l14 is an
MC10176 holding StkP.0-5 as `{p15,p14,p13,p4,p3,p2} = q`, so StkP.n is q[n];
l15 is an MC10231 with StkP.6 = qb and StkP.7 = qa.

The preset takes (the held value goes 0x3f -> 0x20) **and StkP still reaches 0
at the same cycle, 565**. That rules out a decrement: HM Table 6's RSTK[1:3]
adjusts StkP by -4..+3 only, so 32 could not reach 0 in one step, and the change
detector records TWO transitions in the whole run rather than the eight a
walk-down would give. **Something LOADS StkP with zero.**

Which is consistent with a world initialising an empty stack -- and then the
next stack operation with the check enabled underflows, because `cpu.c`'s Table
6 note says underflow fires "if StkP originally 0 OR finally 0". So the world
must raise StkP off zero before it uses the stack, and this bench never gets it
that far.

`+stkinit` alone therefore changes nothing measurable (task 0 still runs 2
addresses); it is `+faultinit` that moves the machine. Both are kept, because
the negative result is what pins the load.

### THE FAULT TASK RUNS ITS OWN HANDLER

When a task is woken it resumes at its own `TPC` -- there is no separate vector
-- so an uninitialised `TPC[15]` makes the fault task run whatever it lands on,
which is why it was executing EMULATOR handlers. AEmu's own handler is
`FAULTTASK`, octal 3747.

ContA i13/j13/k13/l13 are F10145A holding TPC in four-bit slices -- l13
TPC.00-03, i13 TPC.04-07, j13 TPC.08-11, k13 TPC.12-15, with
`q = {q3,q2,q1,q0} = {p14,p15,p1,p2}` -- and TPC.04-15 is the 12-bit IM address,
MSB first.

**AND THE ADDRESS PINS ARE PRIMED**, `TPCAd.0-3'`, so `a = ~task` and `TPC[15]`
lives at `mem[0]`. The same reversal CLAUDE.md records for RM's `~RSTK`. That is
not taken on faith: `+tpcslot=15` writes the un-reversed slot and has **no
effect at all** -- counts identical to not presetting -- while slot 0 changes
everything. The measurement settles it.

`make -C verilog exec-init` arms MemX's fault counter and presets TPC[15]:

| | `+taskingon` | `+faultinit` | `+faultinit +tpcinit` |
|---|---|---|---|
| task 0 addresses | 2 | 20 | 15 |
| task 15 addresses | 23 | 10 | 12 |
| `Hold` | 399,419 | 72,635 | **96** |
| `RepeatCur` | 399,419 | 72,635 | **64** |
| task 15's code | emulator handlers | emulator handlers | **octal 3700-3733** |

Those addresses are the fault-handler region, and `mbdis` names two of them:
**`BEGINENUMERATEMAP`** (3722) and **`IWRITEMAPFLAGS`** (3732), one carrying
`ASEL = Fetch<-RM/STK`. That is a fault task doing its actual job -- walking the
map and writing map flags -- in RTL generated from the wire lists.

**Two gate thresholds were widened, both with the reason measured rather than
assumed.** The microinstruction-clock floor assumed ~one instruction per SYSPER
fabric cycles with nothing stalling; a machine issuing real memory references
legitimately issues fewer, and it falls to 16,676 of a 25,000 ceiling while
`Stop` stays clear and `Hold` is on 96 of 400,000 samples -- work, not a stall.
And the clk0'/clk1' slack went from one to two, because once instructions can
REPEAT a second window boundary lands the same way (16,676 against 16,678). Two
is a property holding; a real divergence is thousands.

### PARC'S OWN BOOT CHAIN RUNS (2026-08-27)

Register-patching goes only so far; `exec-init` pokes three registers, which is
not how a Dorado starts. The real chain is BaseBoard -> Boot0 -> Initial ->
the world, and **the first two stages of it now run in generated RTL.**

`make -C verilog exec-boot`:

```
PRELOAD -- 976 microinstructions written, 976 read back, 0 wrong
addresses executed: f40 fc0 fe1 fe2 fe6 fe7
BOOTCHAIN -- INITIAL(0xf40)=1 READBB(0xfc0)=1, 4 addresses in Bootstrap's poll loop, longest run 0
PARC'S BOOT CHAIN RUNS -- Initial calls Bootstrap, which waits for the control processor.
```

**They are designed to coexist, and the addresses prove it.** `Initial.mb`
occupies real `0xc00-0xfbf` (926 microinstructions) and `Bootstrap.mb`
`0xfc0-0xfff` (50) -- adjacent and disjoint, the top of IM, so a world can be
loaded beneath them. Both load through the same derived field map, 976 written
and 976 read back.

**And the call is checkable arithmetic.** `INITIAL` is image 1 = real `0xf40`,
and `IM[0xf40]`'s JCN is `0xff` -- a GLOBAL CALL. The formula gives

    TNIA = CIA[2:3] || JCN[2:7] || 000000
         = 0b11 || 0b111111 || 0b000000
         = 0xfc0

which is Bootstrap image 54, **`READBB`** -- read BaseBoard. The machine then
cycles images 55/56/57/60 (real `0xfe7/0xfe2/0xfe1/0xfe6`) with **longest run on
one address = 0**: it never repeats, it polls.

**THAT IS THE CORRECT PLACE TO STOP.** This configuration has no BaseBoard --
`dorado_world` deliberately excludes it, because the bench drives the CP bus
itself -- so nothing is sending, and Bootstrap waits. It is not stuck; it is
doing its job.

Two things fall out. **Initial does not underflow the stack**: StkP holds `0x3f`
for the whole run, where AEmu reached 0 at cycle 565. And Initial's symbol table
names exactly the three things `exec-init` was patching by hand -- `RMINITL`
(35), **`STKINITL`** (56), `IFUMINITL` (61), **`TASKINITLOOP`** (106),
**`GETTASKINITPC`** (112), `TASKINIT` (113), `BOOTEMULATOR` (133). The machine
initialisation this project has been supplying from the testbench is a routine
sitting in the microcode, waiting to be run.

**The next step is to feed Bootstrap.** `boot0-test` already walks real microcode
into IM over the control-processor bus using PARC's own hunk format and
`SendViaMIR`; pointing that at Bootstrap's `READBB` loop is what turns this from
"the boot chain starts" into "the boot chain completes".

**And the register-patching route's fix is still upstream: nothing has
initialised this machine.** A real boot
does not jump a cold world at `START` with the fault counter unarmed and every
task PC unset; Initial and the world's own `InitMem` do that first. That is what
`exec-world9` skips by preloading IM and jumping straight in.

### Traps found on the way

- **THE ADDRESSES ARE OCTAL.** `mbdis` prints IM addresses in octal -- its
  listing runs `...207, 210...` -- so a visited set collected in hex must be
  converted before it is looked up. Reading them as decimal named the wrong
  trap handler in a commit message.
- **`find_word` SEARCHED FOR THE BANK**, which is wrong whenever the wanted
  half is `0000`: a WIPED bank matches it too, so the search returned bank 0
  and read that bank's secondary bit. `IM[0x805]`'s right half in AEmu.mb IS
  `0000`. It computes the bank now; the search survives as the diagnostic that
  says where a missing word actually landed.
- **A VECTOR SET CONFINED TO LOW ADDRESSES** leaves `addr[11]` always 0 and
  exercises half the interleave, so `boot0-test` also loads four hunks from
  above 2048.
- **`$sscanf` RETURNS -1 ON A PARTIAL MATCH**, not the number of conversions it
  made -- while still correctly filling the arguments it DID convert. A
  three-field line read with a ten-field format comes back as -1 with every
  field right, so gating on the count silently skipped all of them.
- **`$fgets` RETURNS 0 AT END OF FILE** and leaves its buffer alone, so the
  `while (!$feof(fd))` idiom processes the LAST line TWICE -- 17 ALUFM entries
  out of a 16-entry file.
- **A GENERATED INCLUDE IS SHARED**, so anything in it that reaches into a
  board must be guarded: the IFUM tasks name `m.b_IFU`, and `tb_boot0` and the
  default `tb_exec` run on four boards that have no IFU.


## IM PARITY, ANSWERED (2026-08-27)

The machine has always had to run with the IM parity enables CLEARED, and this
file put the alternatives plainly: "either PARC's IRTable entries carry parity
that satisfies the generator, or our MC10170 parity generators on ContB j20/j21
compute something different". **It is the second, by exactly one inversion --
and that inversion is already known from elsewhere.**

    PARC's convention is ODD parity over the 17-bit half, and the IM ARRAY
    STORES ITS COMPLEMENT -- exactly as `dBlock'` does.

Three steps, none of them assumed, and each independent of the others.

**1. WHICH BITS EACH HALF COVERS, fitted rather than guessed.** `im_image` now
emits, for every present address, both the decoded fields AND the two 17-bit
half-words parity is computed over. Fitting one against the other across 2,148
AEmu addresses gives a UNIQUE match for every one of the 34 bits, with no
ambiguity and no inversions:

    left  half = RSTK.1-3, ALUF, BSEL, LC, ASEL   + RSTK.0 as the secondary
    right half = FF, JCN                          + BLOCK  as the secondary

**2. THE SENSE THE ARRAY WANTS, measured on the running machine.** Preloading
IM parity and sweeping sense against secondary-inclusion, over 200,000 samples:

| rule | IMLHPE |
|---|---|
| **even, secondary included** | **125** |
| odd, secondary included | 200000 |
| even, secondary excluded | 199680 |
| odd, secondary excluded | 445 |

**3. THE SENSE PARC USES, read off PARC's own hand-coding.** The IRTable's
five-byte format carries explicit `P015` and `P1631` bits -- and they VARY
(1/1 for `Nop#`, 1/0 for `Return#`, 0/1 for `CPRegToLink#`), so they are real
computed parity, not padding. All eight entries carry ODD parity over the same
17 bits: **8 of 8, 16 of 16 bits.**

Steps 2 and 3 are complements, which is the answer.

### What the check CANNOT tell you

Mutating `im-parity-check` four ways, two do NOT fail -- and both for
principled reasons rather than fixable ones, so the PASS must not be read as
covering them. The tool prints both limits beside its own result.

- **Whether the SECONDARY bit is included.** Every one of PARC's eight entries
  has `RSTK.0 = 0` and `BLOCK = 0`, so the dataset cannot separate 16-bit from
  17-bit parity. Step 2's machine measurement is what settles it, and the two
  steps are therefore not redundant.
- **The bit ORDER within a field.** Reversing it is a PERMUTATION of the bits
  parity is taken over, and parity is invariant under permutation. No parity
  check can ever detect it.

### The encoder now reproduces PARC's microinstructions EXACTLY

`mi()` in six benches used to set `P015` and `P1631` to 1 unconditionally, on
the reasoning that "a jammed instruction fails IM parity anyway -- that is the
jam mechanism". It computes them now, and the halves turn out to be simply the
fields, since parity does not care about order:

    P015  = odd parity over  RSTK, ALUF, BSEL, LC, ASEL   (4+4+3+3+3 = 17)
    P1631 = odd parity over  BLOCK, JCN, FF               (1+8+8     = 17)

With that, the encoder reproduces **all eight of PARC's hand-coded IRTable
entries byte for byte, parity INCLUDED** -- where its comment previously had to
say "parity bits aside". `im-parity-check` gates the round trip, which is a
stronger check than comparing parity bits alone: regenerating all five bytes
pins the BYTE LAYOUT too, so a wrong field position is caught where parity,
being permutation-invariant, would not catch it. 46/46 gates stay green.

### AND THE MACHINE RUNS WITH THE ENABLES ON

`make -C verilog exec-parity`:

```
tb_exec: LATE parity enables IMLH=1 IMRH=1, Stop=0
tb_exec: WHEN -- first parity error at cycle 0, last at 148; Stop first set at -1
tb_exec: IM PARITY ENABLED -- Error propagated on 0 samples, Stop=0, 24991 clk0' edges
tb_exec: THE MACHINE RUNS WITH IM PARITY ON.
```

**What was in the way was not the parity VALUES.** Bucketing the errors by TIME
rather than totalling them settles it: they run from cycle 0 to **148** and then
**clear by themselves, never returning across 400,000 cycles**. The preloaded
array's parity is correct; what fails is the machine's own power-up transient.

With the enables on from the start, `Stop` latches at cycle **125** -- inside
that window -- and **`Stop` gates the clock that would clear it** (`bCLKEnable'
= Stop | Run'` gates `clk2'`, the stop latch's own clock), so a momentary error
is permanent. Switching the enables on at cycle 400 instead gives zero
propagated errors and a machine that free-runs.

**A TOTAL SAID "parity is broken". A DISTRIBUTION SAID "the enables are on 148
cycles too early".** That is the third time in this project that reading a total
instead of a distribution produced a wrong conclusion.

### The jam's parity routing WORKS -- a claim of mine, retracted

**Retracted:** I wrote that "the jam never delivers a parity bit at all",
because capturing `IMLH` inside `parc_micro` reads 0 after all four data
strobes even for `Nop#`, whose `P015` is 1. Sweeping the actual routing refutes
it.

**Byte 0 is FOUR BITS, and they ride as the NINTH CP-BUS BIT of the four data
strobes** -- which is why its low four bits are always zero:

    b0[7] RSTK.0 -> fn 4        b0[6] P015  -> fn 5
    b0[5] JCN.7  -> fn 6        b0[4] P1631 -> fn 7

`make -C verilog/verilator exec-world9 EXECARGS='+jamsweep'` drives the ninth
bit alone on each function code and reads the MIR back:

```
JAMSWEEP fn=5  ninth=0 -> IMLH=0 IMRH=0   ninth=1 -> IMLH=1 IMRH=0
JAMSWEEP fn=7  ninth=0 -> IMLH=0 IMRH=0   ninth=1 -> IMLH=0 IMRH=1
```

Exactly what the byte layout implies, and it holds under every variant tried:
one strobe or two (`+jamsweep1`), preceded by fn 4 or not, and with any data on
the eight data bits (`+swdata=`).

**So the open question is narrower than I said**: the path delivers the bit,
and something in `parc_micro`'s full sequence clears it between the strobe and
the capture. That is a testbench-timing question about where to sample, not a
missing connection -- and it is not what makes `exec-parity` work, since that
switches the enables on after the startup transient and needs no jam parity at
all.

Both checkers work out to the same rule, derived from the netlist and the cell:

    ContB j20/j21 (left)   IMLHPE' = ~( XOR(17 field bits) ^ IMLH )
    ContA e18/e19 (right)  IMRHPE' = ~( XOR(17 field bits) ^ IMRH )

-- and they reach it by different routes, the left taking `IMLH'` and three
`LC'` (four inversions, which cancel), the right taking `IMRH` true. So NO ERROR
requires the stored bit to be the EVEN one, which is exactly what the preload
measurement found.

With a MIR parity bit of 0, an entry passes only if its field-XOR is zero --
true of `Nop#` alone among PARC's eight, and NOT of `Return#`, which is what the
startup jams (XOR(rh) = 1). Forcing `IMRH=1` across the jam (`+jampar`) drops
the error count 133 -> 85, which confirms the arithmetic even though the
startup transient is what actually gates the enables.

### The remaining inversion

`+imparityon` still stops the machine after two `clk0'` edges -- but with only
133 parity samples out of 400,000, so what fails is the jammed `Return#` in the
MIR at startup and nothing else.

The discrepancy is sharp: **the ARRAY wants EVEN (preloading even parity clears
IMLHPE; that is measured) while the JAM supplies PARC's ODD (Return# carries
P015=1, P1631=0, which is odd parity over its halves; that is read off PARC's
source).** In the real machine both paths satisfy one generator, so our RTL has
an inversion in one of them that the hardware does not.

`cell_MC10170` is the obvious suspect and was NOT changed, deliberately: it
matches its data sheet (A is odd parity over the nine data bits, B is even with
the controls folded in) and its header records that it was already tuned
against PARC's IRTable, fixing eleven of thirteen entries. Changing a
datasheet-correct cell on a hunch is how the last round of parity confusion
started. The place to look is where the two paths differ -- the CP-bus write
complements its data (`BMux` is the complement of `CPReg`, which is why
`SendViaMIR` uses `SetCPReg~` for IM data and the plain form for T), and
whether the jam path does the same is the question.

## START HERE: two open questions

The machine loads IM from real `.MB` hunks, matches the C emulator word for
word, **executes** microcode out of it, and **computes** -- Q, ALUFM and T all
verified against PARC's own boot sequence (`boot0-test`, `exec-test`,
`compute-test`). ONE thing in the boot chain is open. Section 2 below is kept
because what it cost is worth not paying twice, but it is a gate now, not a
question.

**And the MEMORY SECTION is traced end to end (2026-08-22/23)**, from a
microinstruction's ASEL field to one gate short of the DRAM array. It has its
own long write-up further down -- search for **"the memory reference-kind
chain"** -- and the short version is:

- every hop is gated: `WantProcRef'`, `FFok'`, `WantCR`, `Flush'`, the `Store←`
  that dirties a line, `HitColDirty` → `FlushStore` → `ForceMiss`, **the cache
  missing**, `←Map` writing an entry so `MapTrouble` clears, and `StartMem'`;
- one real RTL bug came out of it -- **VBB was modelled as 0**, which strobed
  off the whole map read path through five MC10124s. Fixed per-pin in
  `tools/sil_to_verilog.py`: *VBB loses to a real signal and beats an open
  pin*. 41 packages;
- what is still open is `MemWEa`, and it is **not an RTL fault**. `MemWEa`
  needs MemState 4; the bench's hand-built loop delivers 3, because a
  four-microinstruction loop beats against the map's own accept cadence
  (MapFnc=00 every 256 samples, MapState=3 every ~1024, coinciding once). A
  one-shot reference already gives 4x the window. The fix is a **short window
  of authentic reference cadence** walked into IM -- see the task list, and
  note PARC's memory diagnostics are millions of steps, far too long to run
  whole under Verilator.

### 1. Parity: the RULE is now known, and the fix is one line with a catch

`make -C verilog parity-probe` jams each of PARC's thirteen IRTable
microinstructions and reads the parity generators with the jam STILL IN THE
MIR: **all thirteen fail, both halves.**

**PARC'S RULE IS ODD PARITY OVER EACH 17-BIT HALF.** This is settled by pure
computation against PARC's own table -- no RTL involved. `doradoboot.masm`
states both the FIELDS and the BYTES for all thirteen entries, and P015/P1631
live in byte 0 (bits 6 and 4), so the rule can simply be fitted:

```
left  17 bits = RSTK(4) ALUF(4) BSEL(3) LC(3) ASEL(3)  -> P015
right 17 bits = BLOCK(1) FF(8) JCN(8)                  -> P1631
parity bit = ~(XOR of those 17)   i.e. ODD parity including the bit itself
```

That reproduces PARC's stated **P015 for 13/13 and P1631 for 13/13**; even
parity matches 0/13. The split is exactly the hunk format's two 17-bit halves,
and the checkers' own input lists confirm the domains -- ContA e19+e18 take
JCN, FF, CABlock and **IMRH**; ContB j21+j20 take BSEL, LC, ASEL, RSTK, ALUF
and **IMLH'**. Those two ARE the parity bits, so each checker sees 17 data + 1.
(All thirteen entries have RSTK.0 = 0 and BLOCK = 0, so the data cannot say
whether those two bits are inside the domain or outside it.)

**THE ONE-LINE FIX, AND WHY IT IS NOT COMMITTED.** `cell_MC10170`'s B output is
`~(par9 ^ p13 ^ p14)`. The data sheet (1978 MECL book p.123) draws B as a PLAIN
three-input XOR of the two controls with the odd-parity tree -- no bubble, the
same symbol as the four gates building the tree. The inversion its truth table
shows comes from the NOMINAL control levels the same diagram labels, "Control
(1) 13 High, Inputs (2) 14 Low": at pin 13 HIGH and pin 14 LOW,
`B = XOR(1,0,A) = ~A`, the table exactly. Baking that inversion into the gate
is right only at those levels and the Dorado never uses them -- ContA e18
leaves pin 13 unconnected and takes the cascade on pin 14. There is also an
argument needing no data sheet: PARC ran with IM parity errors ENABLED, so a
correct microinstruction MUST read `PE' = 1`. With `p15 = par9 ^ p13 ^ p14`
**all thirteen do**; with the `~`, none do.

Changing it, however, **regresses five jam-based gates** -- `datapath-test`,
`operand-test`, `step-test`, `sendmir` and `compute-test` -- measured with that
cell as the ONLY change, while `parity-probe` itself goes green. Of the 41
MC10170 packages, only FOUR use pin 15 at all:

| package | pin 15 drives | what it is |
|---|---|---|
| ContA e18 | `IMRHPE'` | right-half IM parity check |
| ContB j20 | `IMLHPE'` | left-half IM parity check |
| ContB e01 | `ContB03.sil+1` -> d05 -> **`RBMuxP`** | B-BUS parity; its controls are `BMux.16/17`, the parity bits riding that bus |
| ProcH d13 | `SignedCarry` | **not a parity signal at all** |

**THE CHEAP EXPLANATION WAS TESTED AND IS WRONG.** The obvious guess is that
those five jam SYNTHETIC microinstructions whose P015/P1631 were arbitrary, so
a correct checker rightly errors on them. Giving `mi()` the odd-parity rule
changes nothing -- all five still fail -- and `tb_datapath`'s hand-built
instruction already carries the correct bits (1 and 1). Do not spend time
there again.

**THE COUPLING IS THROUGH B-BUS PARITY, and the failure signature says so.**
With the fix, `compute-test` reports `T took 1200, not 1234`: ProcH's half of T
is right and **ProcL's half is zeroed**. ProcL uses no MC10170 pin 15 at all,
so this is not a local effect -- the bridge is ContB e01, whose two CONTROL
inputs are `BMux.16`/`BMux.17` and whose output reaches ContB d05, an MC10102
driving `RBMuxP`, the B-bus parity bit itself. Flip the cell and the processor
boards see a bad-parity B bus.

**So the next step is the B-bus parity chain, not the IM checkers.** Make
`RBMuxP` and the `*.Perr'` generators on ProcH/ProcL (`R.Perr'`, `T.Perr'`,
`Md.Perr'`, `IOB.Perr'`, all MC10170 pin-2 outputs, which this change does NOT
touch) consistent with the corrected B output, then re-run. `SignedCarry` at
ProcH d13 needs its own check against the C emulator's overflow logic (HM
section 3.7, QW7). And `mi()`'s parity computation should land TOGETHER with
the cell fix, never before it -- with the present cell a CORRECT parity bit is
exactly what the checker rejects.

**A trap worth recording, because it cost a wrong diagnosis here.** Register 0
of the manifold is not "the parity enables" -- `12'h030` is
`DisableDoradoErrors`, and writing `12'h000` to the same register does not
"turn parity off", it re-ENABLES every Dorado error class. Patching the
jam-based testbenches that way broke four of them, which then looked exactly
like fallout from the cell change and was not: the same four still failed with
the cell reverted. Change ONE thing at a time when a fix touches 41 packages.

### The real firmware runs the machine

Every other gate here has the TESTBENCH play BaseBoard, strobing the
control-processor bus by hand from sequences transcribed out of
`doradoboot.masm`. `make -C verilog firmware-probe` does not: it brings up
**BaseBd + ContA + ContB + ProcH + ProcL** as one machine (`dorado_boot`,
generated like the other configurations) and lets the 6502 run its OWN firmware
out of its OWN EPROMs. A diagnostic, not a gate. Two switches:
`+define+LONG_RUN` (260 M sys_clk, enough to cross a watchdog interval) and
`+define+G22_DISARMED` (an experiment that disarms the watchdog at time zero,
to separate it from everything else).

| | 4 M armed | 4 M disarmed | 260 M armed |
|---|---|---|---|
| `MCReset'` assertions | 19 | 1 | 397 |
| `CPStrb'` edges | 37 | 450 | 27,674 | (Control and MIR3 only — see below) |
| `PACIFYWATCHDOG` F692 | 0 | 5 | 240 |
| `DMuxClk` edges | 0 | 0 | 0 |

**THE BASEBOARD BOOTS ITSELF PAST POWER-UP.** In the 260 M run, resets bucketed
by watchdog window (each Q21 edge starts a bucket):

| window | from | g22 FF1 `Q'` | resets |
|---|---|---|---|
| 0 | 0 | 1 — **ARMED** | **397** |
| 1 | 83,886,119 | 1 — ARMED | **0** |
| 2 | 167,772,199 | 0 — disarmed | 0 |
| 3 | 251,658,279 | 0 — disarmed | 0 |

Every reset is in window 0. After the first watchdog interval there are none,
across 176 M sys_clk — **including window 1, which is still ARMED**. The
firmware gets far enough to pacify, the XOR stays 0 from then on, and the
watchdog is satisfied for the rest of the run.

#### What made it reach the Dorado

One fix, and it belongs in the **generator**, not a cell: `WEAK_PORT_DRIVERS`
in `tools/sil_to_verilog.py`, symmetric to the existing `OVERRIDE_DRIVERS`.

A 6532 port pin is high-Z with an internal pull-up when its DDR makes it an
input, and the core states that directly — `PA_out = out_a | ~dir_a`, with the
comment that the output "must be fed back to input ... for the chip to read
properly". That is a **wire-AND** convention; these nets resolve as
**wired-OR**, so the pull-up won instead of losing and pinned every such net
high.

**It has to be per-net.** Masking inside the cell with the DDR holds the 6502
in reset, because `WatchdogOut`'s only driver *is* the RIOT and its pull-up is
real. The rule: the pull-up loses where something else drives, stands where
nothing does — information the generator already has.

It is sound because of what those nets are. Across the machine **33 nets** have
a 6532 port pin sharing with another driver, and in every one the other driver
is a real totem-pole part ('174, '259, '01, '157, '175, '74, MC10125) or a
strap, with the 6532 pin as the READER: **`RCPReg.00-15`** — how the BaseBoard
reads the Dorado's CP register back — plus `MCManif.0-3`, `TCPI.0-3`, the
temperature senses and `WatchdogIn`. All were stuck high.

#### The watchdog

Every part of the chain is in the RTL:

| package | part | role |
|---|---|---|
| g21 | **MC14521B** 24-stage divider | **the timer**: Reset tied low, `MCPreClk` into In2, **Q21** (pin 13) out. Interval = MCPreClk / 2^21, of order a second. Was an unmodelled skeleton; the cell now comes from the data sheet |
| g22 | SN74LS74 pair, both TOGGLES | FF1 is clocked by that Q21, so it alternates ARMED (`Q'`=1) and disarmed every interval |
| g23 | SN7486 | XOR of `WatchdogIn` against `WatchdogOut` — the pacify test |
| j17 | SN74LS01 (open collector) | NANDs those two to drive `BootMC'`, wire-ORed with a strap at h07 |
| j08 | SN74LS74 | D = `BootMC'`, clock = `MCClk`, async `PwrGood`, output `MCReset'` |
| f63 | MCS6532 RIOT | PA7 out as `WatchdogIn`, PA6 in as `WatchdogOut`; `PacifyWatchdog` echoes one onto the other |

**What trips it is a one-cycle glitch, not the timer.** `WatchdogOut` is driven
low for exactly one MCClk cycle (215,518 → 215,598) while the firmware sets up
the RIOT — the DDR is written before the output register, so the pin drives
whatever `ORA` holds. That transient spikes the g23 XOR, pulls `BootMC'` low,
and j08 latches a reset 521 sys_clk later. It only gets through because g22's
FF1 `Q'` is 1; in the disarmed half j17 masks the XOR entirely. Hence the 397
resets confined to window 0 and none afterwards.

`SkipWait'` reads 1, which *would* make `PacifyWatchdogIfJumper` skip — but
that entry is never used; the firmware calls `PacifyWatchdog` directly.

#### The manifold path — and what the CP-bus traffic actually is

Traced by **routine**, in the disarmed build:

| routine | addr | visits |
|---|---|---|
| `SETMANIFOLD` | F95A | 4 |
| `SETMUFFLERADDRESS` | F9D0 | 4 |
| `TRYGETTINGMUFMANCONTROL` | FA0E | 52 |
| `WAITFORCPCONTROL` | FA1F | 4 |
| `DATUMTOMANIFOLD` | F977 | 0 (different entry, unused here) |

`SetManifold` calls `SetMufflerAddress`, which calls
`TryGettingMufManControl` and gives up on carry set. That routine reads
`MCPBusL & SetRunIn` (bit 2, the net `TSetRun`) and fails when the bit is SET —
it reads **0**, so the gate passes.

The shift does **not** use the `DMux*` nets. `SetMufflerAddress`'s inner loop
at F9F6 sets `MCPBusL = $10` — **function code 1, `Clock`** — then writes
`MCPBusH` and pulses `MCPStrb` twelve times. `i62` is that RIOT: PA drives
`MCPBus.00-07`, PB7 `MCPBus.08`, PB6-4 `MCPABus.0-2`, PB0 `MCPStrb`. So a
manifold word goes out as ordinary CP-bus transactions, and the BaseBoard's own
k22/k17 decode them into `CPDMuxData`/`CPDMuxClk`, through l19 and the l24
TTL-to-ECL translator, onto the backplane as `DMuxData`/`DMuxClk`.

**Measure the PC with SYNC, not the address bus.** The 6502's SYNC output marks
an opcode fetch. Ungated, the address bus shows data reads too — which is how
`FF00` (1024) and `FF80` (669) came to look like a PC parked in filler, when
`FEF0..FF10` disassembles as **data**. Gated on SYNC, the most-executed
addresses are all F84A..F865, the ADCONVERT loop.

**And one generator fix made the manifold chain run: a 6532 port pin reads its
own pin.** `read_excluding` drops a package's own contribution — right for a
gate, which does not read its own output; wrong for a port pin, where the chip
reads the **pin**, and an output pin is whatever the chip itself drives. The
core says so outright: *"NOTE that port output must be fed back to input ... in
order for the chip to read properly."* `READBACK_OWN_PIN` in
`tools/sil_to_verilog.py` now wires `pa_in`/`pb_in` from the **resolved net**.

What it cost while wrong: every read-modify-write on a port read **zero** for
its own output bits. BaseBd i62 is MCPBusL, and `SetMufflerAddress` pulses the
CP-bus strobe with `INC $0582` / `DEC $0582` — reading 0x00 instead of 0x10
turned those into 0x01 and 0xFF, so the three `MCPABus` function bits read 0
then 7 instead of a constant 1, and k22/k17 never decoded a DMux pulse.

**With it, the muffler address shifts out:**

```
strobe 2 (fetch f9fd) dir_b=f1 out_b=13: fn=1 data=001000000
strobe 3 (fetch fa08) dir_b=f1 out_b=13: fn=1 data=000000000
... 24 strobes, all fn 1 (Clock) = twelve bits, two strobes each

CPDMuxClk edges 24    the BaseBoard's own decode fires
TDMuxClk  edges 24    through l19
DMuxClk   edges 24    onto the BACKPLANE, to every board
READMUFFLER F986 1 visit, SETMUFFLERADDRESS F9D0 1
```

`out_b = 0x13` is `$10` plus the input bits reading back — exactly what a
read-modify-write on a 6532 should give. `MCPBus.00` is the data bit and
`MCPBus.01` the clock; `CPDMuxData` stays still here only because the address
bits shifted so far are zeros.

**So the BaseBoard now clocks the muffler chain on the backplane.**

**The far end is measured, and the measurement is inconclusive** — worth
stating rather than reading either way. ContB's twelve-bit shift register and
its latch:

```
DMuxClk edges 24            the clock reaches the backplane
ContB DMD changes 0         its shift register never moves
ContB ManClk.0' pulses 1    at time 1 only, i.e. power-up, DMD=000
```

A dead register looks **exactly** like a register being fed zeros. Every data
bit shifted in this window was 0 — `MCPBus.00` is the data bit and it is never
set across the 24 strobes, so the muffler address being sent is all zeros — and
shifting zeros into a zero register changes nothing. This says nothing yet
about whether ContA/ContB act on the chain.

**To settle it,** catch a shift whose address is *not* zero: run
`+define+G22_DISARMED +define+LONG_RUN` so the firmware walks more muffler
channels, and watch `CPDMuxData` (the BaseBoard's own data bit) alongside
ContB's `DMD`. If `CPDMuxData` toggles and `DMD` still does not, the far end is
genuinely not shifting; if neither toggles, the addresses are still zero and
the run was too short.

#### Also worth knowing

The firmware's hot loop (F84A..F864) is a **successive-approximation A/D
conversion**: write `DAC = 400+PA`, settle, read `Comparators = 480+PA`, shift.
Its analog chain is unmodelled — CA3140 op-amps (g18, i19, i20, i21, j21),
CD4051 analog muxes (i2125, j24, k24), AUGATCG16 resistor platforms. That is a
fact about the RTL; it is a **cycle count and not a proven blocker**, since the
firmware demonstrably proceeds past it.

**Three instrument traps this probe walked into**, all the same shape — a proxy
measured instead of the thing, each producing a confident wrong conclusion that
survived several rounds of reasoning:

* counting `0xFFFC` on the address bus as a **reset**. It is not one; the
  firmware reads ROM there. Count `MCReset'` falling edges.
* reading a **total** as a **distribution**. "397 resets over 260 M" looked
  like a machine that never settles; bucketed by window, all 397 are in the
  first one and it settles permanently after.
* inferring the manifold path from a plausible-looking net (`DMuxClk`) instead
  of reading what the firmware **writes**. The path is the CP bus.

### The memory section, and a backplane line that was not connected

`make -C verilog mem-test` is the first rung for MemC/MemD/MemX, and
deliberately the same one the processor started on: a machine that elaborates,
with the boards clocked. `dorado_mem` is **ContA + ContB + ProcH + ProcL +
MemC + MemD + MemX** -- the Control and Processor boards are there because a
memory reference does not exist without them.

**The memory clocks are GATED**, which is the first thing to know. Each board
fans its backplane clock (`CLK.mc'`, `CLK.md'`, `CLK.mx'`) out through a first
stage that ANDs it with `MemClkEnable'`, driven by a pair of MC10231 flip-flops
on ContA. So a dead local clock out of reset is CORRECT, and "the clock runs"
is the wrong thing to assert. The gate asserts the consistency instead -- a
board's local clock runs **iff** its enable is asserted:

```
MemC  MemClkEnable'a=1  local clock 0 edges     correctly gated off
MemD  MemClkEnable'a=1  local clock 0 edges     correctly gated off
MemX  MemClkEnable'c=0  local clock 1250        running
```

**MemX is a finding:** its enables are undriven. ContA drives `MemClkEnable'a`
and `'b`; MemX asks for `'c`, which no board drives, so its clock runs only
because an unconnected net floats to the enabled state. The pins do not settle
it either -- all three memory boards take theirs on C12 while ContA drives on
C17 and C20, exactly the non-straight-through backplane this machine models by
wiring on NAME. Whether `'c` is a third fan-out copy spelled differently or a
separate line is not established.

**What turns the memory clocks on, and THE FIX FOR `machine-test`.** `MemClkEnable'a` is a wired-OR of two MC10231s on
ContA: one latches **`dMemRun`** (set by `RunRefresh`, which comes from
`WantRunRfsh`, which comes from **`SetRunRfsh` -- a backplane line the
BASEBOARD drives**), the other latches **`dStop`**. So the memory clocks come
on when the machine RUNS and refresh is asked for, which is why they are off in
every jam-mode test.

`+define+MEM_RUN` runs run-test's own start sequence and then asserts
`SetRunRfsh` as the BaseBoard would. That first produced
`Active region did not converge` -- the same failure `machine-test` had on the
eleven-board machine, reproduced on seven, which is what made it findable.

**IT WAS `cell_F10016`'s TERMINAL COUNT, and the data sheet settles it.**
Verilator's `UNOPTFLAT` warning (which the gates suppress) named the loop:
MemD's `d14` is an F10016 counter whose `TC` output (pin 4) runs through `d20`,
an MC10195 whose select pin is unconnected -- so an inverter -- straight back
to that same counter's own `CE` (pin 6). The cell had

```
assign p4 = ~(&q & ~p6);      // TC gated by CE
```

which at terminal count reduces to `TC = ~TC`. An oscillator. The Fairchild
data sheet (`DoradoDocs/datasheets/F10016.pdf`) names the pins outright --
**`CE` Count Enable (LOW to Count)**, **`TC` Terminal Count (10016 LOW at
HHHH)** -- so TC is a function of the Q state ALONE. The part carries its own
expansion logic ("INTERNAL COUNT ENABLE - FOR HIGHEST SPEED EXPANSION"), and
the eight `TC -> CE` cascades in the machine (IFU has a three-stage chain, plus
ProcH, MemX, DispM) work correctly that way: TC goes low at HHHH, CE is low to
count, so the upper stage counts exactly when the lower one is at terminal.
MemD's own arrangement is then a stable count-and-stop.

With `assign p4 = ~(&q)`: the memory boards clock (`MemClkEnable'a=0`, 1250
edges on MemC, MemD and MemX), the seven-board machine converges, and
**`machine-test` passes** -- 24/24 clock nets toggling. 226 F10016 packages
across the machine, one line.

#### SIX BACKPLANE LINES WERE SPELLED TWO WAYS, and one was the memory hold

Found while chasing the above, and it is the first real defect in the memory
section. PARC's draughtsmen were not consistent about capitalisation, and this
backplane is wired by NAME, so a spelling difference silently leaves a line
unconnected:

**`PrHold` on MemC and `PRhold` on ProcH/ProcL are ONE WIRE** -- `#07-E.42`,
`#s05-E.42`, `#s04-E.42`: same connector, same pin, three slots. Before this,
MemC drove `PrHold` into nothing and the processor read `PRhold` from nothing,
so **the memory section could not hold the processor at all.**

| one line, two spellings | genuinely separate (different pins) |
|---|---|
| `PrHold` / `PRhold` — MemC → ProcH, ProcL | `CLKEnable'a` C16 vs `ClkEnable'a` C8 |
| `MxHold` / `MXHold` — MemX ↔ MemC | `IOin'` E71 vs `IOIn'` E70 |
| `HoldMapbuf` / `HoldMapBuf` — MemX ↔ MemC | `IOout'` E74 vs `IOOut'` E71 |
| `Subtask.0`, `Subtask.1`, `FoutSubtask.0` — MemX ↔ ProcL, DispY | |

**Case-insensitive matching would be WRONG**, which is why this is a six-entry
table (`BACKPLANE_CASE_ALIASES` in `tools/sil_netlist.py`) and not a rule.
Among backplane nets there are nine case-variant groups and **three sit on
different pins** -- separate lines that merely look alike. Outside the backplane
it would be far worse: **63** net names differ only by case, mostly per-board
LOCAL clock fan-out such as `Clk0'Aa` on MemX against `clk0'Aa` on IFU, and
merging those would tie every board's clock distribution together. The rule is
narrow and checkable: merge a case variant only where every board using it
agrees on the pin. No board carries both spellings, so no rename collides.

### RM, and a general microinstruction encoder

`compute-test` writes RM (`LC[6]` = "RM/STK <- Pd") and reads it back
(`ASEL[4]` = A<-RM/STK with ALUFM[0] = 37 octal = A), at four addresses with
four values.

**It also checks the PHYSICAL address**, because a write-then-read alone proves
little: RM's address pins take `RbAdr.0-.3` unprimed but `RbAdr.4'-.7'`
PRIMED, and a mis-modelled polarity there is a consistent PERMUTATION -- it
round-trips perfectly and is still wrong. That is the trap that hid the IM
address reversal until Boot0 compared IM against something external, and
**nothing external ever sees RM**.

**The low four address bits are `~RSTK`.** `RbAdr.4'` comes from an MC1662 NOR
at ProcH k08 -- a 2:1 read/write address mux whose other input is `RbWadr` --
so the RSTK half reaches the RAM complemented, as its primed name says. Three
independent things agree: `mir-diff` proves RSTK is right in the MIR, the wire
list shows the NOR, and the gate measures `~RSTK` end to end. Harmless to the
machine (a permutation within each RBase bank) but **it must be applied if RM
is ever diffed against the C emulator**, whose `RM[n]` is index n. The high
four bits are RBase, checked only for constancy; they come from an MC10231's
true output on ProcL g10 across the backplane and read 15 here, and nothing in
the IRTable loads RBase, so which value that represents is not settled.

**A GENERAL MICROINSTRUCTION ENCODER**, which is what made this reachable.
`tb_compute.sv`'s `mi()` builds the five bytes from
`(rstk, aluf, bsel, lc, asel, ff, jcn, block)` per the layout `doradoboot.masm`
states, and it **reproduces all thirteen IRTable entries byte for byte** --
parity bits aside, which PARC states per instruction. So it is checked against
PARC's own hand-coding rather than trusted, and any future microinstruction can
be built with it instead of hand-assembled.

(That validation also, incidentally, extracts `P015`/`P1631` for all thirteen
entries alongside their field values -- which is the dataset the open parity
question needs.)

### The ALU, in the machine

`compute-test` ends by computing: **A from T, B from CPReg, the function from
ALUFM, result back into T** -- and sweeping every entry of HM Table 9, all 16
logical and all 8 arithmetic, against the C emulator's own `alu_op()`.

This is the first thing to exercise the ALU AS THE BOARDS WIRE IT. `alu-diff`
matches the C emulator on 10,752 vectors but builds four MC10181 slices in a
TESTBENCH; here they are ProcH's and ProcL's own with the carry chain running
`f61 -> e61 ->` across the backplane, and `A+B = b78e` only comes out right if
carries ripple through all four.

Getting to the A side took changing **one field** of PARC's `TFromCPReg#`:
`ASEL[4]` is A<-RM/STK -- which is why `alua` read 0000 in every earlier probe
-- and `ASEL[6]` is A<-T, so byte 4 goes `C0` -> `E0`. The check that the
encoding is right is that `doradoboot.masm`'s stated byte layout reproduces
PARC's own `C0` for `ASEL[4]`.

**THE ALUFM ENTRY IS NOT A CONTIGUOUS FIELD OF B**, and this cost an hour.
HM Table 11d: **"ALUFMEM <- B.8, B[11:15]"**. The entry's MSB -- the ALU's
CARRY IN -- comes from `B.08`, the other five from `B[11:15]`. Every LOGICAL
entry is <= 037 octal and lands entirely in those five bits, so the logical
half of the sweep passed while `A+1` did not: writing the carry at bit 5 of B
puts it nowhere. The wire list says so directly (ALUFM's data pins carry
`alub.08/11/12/13/14/15`, not six contiguous bits) and `cpu.c` documents the
same mapping from the same table. Reversing the sense of the MC10107 that
makes `aluCin` is caught by the arithmetic half and NOT by the logical half --
nor by `alu-test` or `alu-diff`, which never see that gate.

**Two more conventions closed on the way.** `cell_MC10173` (81 packages) had
carried a `VERIFY` note saying its select sense and its transparent-clock level
were read from pin names rather than a truth table. Both are now read off the
data book (p.3-79) -- SB high selects the datasheet's D_0 pins, and the latch
is transparent while the clock is LOW -- and both are GATED: reversing either
makes `compute-test` fail, because T is loaded through that part.

#### What getting here cost, and the two cell bugs it found

Both were invisible to all eighteen other gates; every one of them passed with
both bugs in place.

* **`cell_MC10141` loaded its PARALLEL ENTRY rotated one place** -- wired
  `Q0<-D3, Q1<-D0, Q2<-D1, Q3<-D2` where the truth table says `Q0<-D0 .. Q3<-D3`
  flat. That is the whole of the old "Q = 008a where 25 octal is 0015": run 0015
  through a per-nibble rotate and you get 008a exactly. **60 packages.**
  The cell had carried a `VERIFY` note saying its mode encoding came from the
  pin names rather than a truth table, and it was right to.

* **`cell_MC10119` modelled a SHARED pin as a standalone AND term.** Pin 10 of
  the four-wide OR-AND belongs to TWO of its OR groups -- the part is a
  4-3-3-3, thirteen input slots across twelve pins, and the thirteenth is pin
  10 counted twice. ANDing it separately instead pulls the output low whenever
  pin 10 is low; on ProcH b17/c17 that pin is `FA=0'`, so **every**
  microinstruction with FA=0 -- PARC's own `Nop#` among them -- asserted
  `QshiftL'`/`QshiftR'`, and both asserted is PARALLEL ENTRY on the MC10141. Q
  was reloaded from a dead B bus every cycle. **10 packages.** This is the same
  class as the MC10121 shared pin, and `cell-check` cannot see it: the cell
  reads all the right PINS, it just groups them wrongly.

**PARC's DICTIONARY DISAGREES WITH MOTOROLA ON THE MC10141's PIN NAMES, and
neither is wrong.** EclDict calls pin 5 `DL` and pin 13 `DR` (Motorola: `DR`
and `DL`), and pin 10 `SL'`, pin 7 `SR'` (Motorola: `S1`, `S2`). The two swaps
are consistent with each other -- the mode PARC calls shift-left is selected by
its `SL'` and consumes its `DL` -- so it is only which end of a 16-bit register
you call "left". The board reads correctly under PARC's names; the CELL must
implement Motorola's function per PIN NUMBER. Do not reconcile them.

**THE NOPS IN PARC'S SEQUENCES ARE NOT PADDING.** The Qshift controls come out
of a register (ProcH b15) clocked by `Clock1'`, while the MC10141's own clock
`QClock'` is gated off `PreClock1'`, which is EARLIER. So the controls in force
at any load edge are the ones the PREVIOUS instruction latched: decode in cycle
N, act in cycle N+1. **Q is not loaded by `QFromCPReg#`; it is loaded by the Nop
after it.** PARC's comment on `DoIRTableInstAndNop` -- "the Nop holds CPReg
constant through T3 of the previous instruction" -- is load-bearing, and a
probe that samples right after an instruction is reading one cycle early.

**A KNOWN HOLE.** Mutating the MC10141's two SHIFT modes against each other
still passes `compute-test`, because this prologue only ever uses parallel
entry and nothing in the suite drives a Q shift. The corrected cell agrees with
three independent sources (both Motorola books and EclDict's own `[FF]` arcs,
which pair DR/pin 5 with Q3/pin 3 and DL/pin 13 with Q0/pin 14), but that is
ARGUED, not gated. Closing it is contained: `sil_check_cells.py` already parses
the `[FF]` lines for their CLK and RS pins and **throws the per-output
data-input arcs away** -- and those arcs are exactly the per-output dependency
list it already checks combinational parts against.

**Read PARC's IRTable field comments before decoding bytes by hand.** They
state the fields outright -- `TFromCPReg#` is
`RSTK[0],ALUF[0],BSEL[0],LC[1],ASEL[4],FF[176],JCN[201]` -- and an arithmetic
slip reading byte 3 sent this chase off after the wrong FF decode for a while.

### Then

Three cells landed from datasheets Alan supplied (`F10016.pdf`, `F100181.pdf`,
`ADC-MC8BC.pdf`) plus one identified from a bitsavers schematic note. The
F10016 fixed `machine-test`. **`cell_F100181`** (8 packages, all MemC -- the
memory board's address arithmetic) is modelled but **nothing exercises it
yet**, and two of its conventions are assumed rather than confirmed: the
carry-in polarity and the output-latch sense, both flagged in the cell.
**`cell_K1115A`** is the 20 MHz crystal oscillator on DispY a05 and DskEth j20
(driving `EClk0`), a SUBSTITUTION like the VCO with the divisor a documented
choice, not a measurement.

Still missing as datasheets: nothing critical. `PLAT1816` is an Augat resistor
platform, not a chip. Eleven parts remain unmodelled but their datasheets are
already in `DoradoDocs/datasheets/` -- CA3140 x9, AM2615 x7, MPQ6002 x5,
MC1650 x4, MC10163 x4, CD4051 x3, 8T98 x3, MC1672 x3, MC10182 x3 -- so those
are work, not a blocker.

With the datapath gated, the natural next rungs are **RM and STK** (the MB7071H
register file at ProcH h06/i06, which `datapath-test` reaches but nothing
exercises) and **a microinstruction that computes on TWO operands** -- every
gate so far moves ONE value, so an A operand has never been driven.

Also open: `machine-test` is the one red gate (the assembled machine does not
converge under the C++ `eval()` model though it does under the event
scheduler); `memory.c` against MemC/MemD/MemX is the next C cross-check; the
cell library is at 97.7% with datasheets for almost everything missing now in
`DoradoDocs/datasheets/`; and `sil_check_cells.py` still throws away the
per-output data-input arcs in the dictionary's `[FF]` lines, which is the check
that would have caught both of this session's cell bugs mechanically.

**Nine things that will otherwise cost you a day:**

* **MEASURE PARITY WITH THE JAM STILL IN THE MIR.** The generators are
  combinational off it, so the machine must be STOPPED. Running first reloads
  the MIR from IM and measures the parity of whatever was there -- that read
  the left half passing and the right failing, and both were an artifact.
* **`Error'` = 0 IS an error**, settled by function not by the prime in the
  name: it feeds `dStop` and stops the machine.
* **ONCE `Stop` SETS IT GATES THE CLOCK THAT WOULD CLEAR IT.**
  `bCLKEnable' = Stop | Run'` gates `clk2'`, the stop latch's own clock, so
  `dStop`=0, `Run'`=0, `Stop`=1 is a real state and only ClrStop escapes.
* **CONTROL STROBES MUST BE SPACED** -- the BaseBoard is a 1 MHz 6502 and
  `SetRun` must survive three `RunClk'` edges. Without a gap nothing loads.
* **`SetCPReg~` writes the COMPLEMENT**, and `SetCPReg` does not: the ALU
  function code goes in uncomplemented, the address and data complemented.
* **A hunk is 17 bytes and that is exact**; **IM is INTERLEAVED**, the low
  address bit picking the bank; **the right half's secondary bit is stored
  COMPLEMENTED**, BLOCK being the one field bit the MIR wires inverted.
* **The first microinstruction out of a cold stop runs only its `clk0` half.**
* **Check more than the obvious, and pick asymmetric data.** Four mutations
  passed against these gates before they were finished.

## How to read the rest

The sections below are in the order they were written, which is roughly the
order the machine came up. Grouped:

* **Reference** -- the FPGA shape and why it is not the physical one; where the
  clock comes from; how the backplane was derived; OR/NOR polarity; why
  `machine-test` is not a toggle count.
* **The bring-up, in order** -- the BaseBoard's 6502 boots; the boot interface;
  all 36 microinstruction bits; the MIR; the reply path; the machine runs; four
  boards; IM mapped; PARC's hand-coded microinstructions; the Write-IM.
* **Classes of bug worth knowing** -- Tim's COMMON pin; the passive packages;
  the machine not settling; `MCD_0..7` double-driven.
* **Tasks and traps** -- filling in the cell library; testing against the C
  emulator; things that will bite; the one open licence question.

## Historical: what landed on 2026-08-15/16

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

## Reference: OR/NOR output polarity -- for the OR/NOR family only

> **BOUNDED 2026-08-18.** This rule is right for the OR/NOR parts it was
> derived from and **does not generalise** -- MC10104's `OUT` pins carry the
> AND and MC10121's `OUT` pin the plain OR-AND. Nor can PARC's net naming
> decide it, unanimity notwithstanding. See "Correction: role `OUT` is the
> INVERTING output does NOT generalise" above for what does: the data book's
> logic diagram, read by coordinates.

A third of the packages in this machine are OR/NOR gates, and getting the
sense backwards produces a machine that almost works. The rule below is
confirmed against Motorola's own sheets for that family, so those cells can be
written mechanically.

**For an OR/NOR part, EclDict role `OUT` is the INVERTING (NOR) output and
role `o` the non-inverting (OR) output.**

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

## ALL 36 BITS of the microinstruction load path agree (2026-08-17)

`make -C verilog mir-diff`: **88 microinstruction strobes, 0 field mismatches.**

Functions 4-7 are the four microinstruction bytes, and ContA's a01 leaves
Q0'-Q3' unconnected because they are decoded a different way. Six **MC10172**
dual decoders -- ContA g01/h01/i01 and ContB h04/i03/i04 -- are wired as 1-to-4
**demultiplexers**: the data bit enters on each half's ENABLE pin and the two
select lines route it to one of four outputs. Nine `CPOut` lines times four
function codes fan out to the thirty-six field lines `sRSTK.0-3`, `sALUF.0-3`,
`sBSEL.0-2`, `sLC.0-2`, `sASEL.0-2`, `sBLOCK`, `sFF.0-7`, `sJCN.0-7`, `sIMLH`
and `sIMRH`.

Against that stands `dorado_decode_mir()` in `dorado/src/disasm.c`, written from
the Hardware Manual's microinstruction format, byte by byte, from a document
rather than a netlist. Every bit lands in the same place:

| strobe | `CPOut.0` .. `CPOut.7` | `CPOut.8` |
|---|---|---|
| MIR0 (fn 4) | RSTK.1 RSTK.2 RSTK.3 ALUF.0 BLOCK FF.0 FF.1 FF.2 | RSTK.0 |
| MIR1 (fn 5) | ALUF.1 ALUF.2 ALUF.3 BSEL.0 FF.3 FF.4 FF.5 FF.6 | IMLH |
| MIR2 (fn 6) | BSEL.1 BSEL.2 LC.0 LC.1 FF.7 JCN.0 JCN.1 JCN.2 | JCN.7 |
| MIR3 (fn 7) | LC.2 ASEL.0 ASEL.1 ASEL.2 JCN.3 JCN.4 JCN.5 JCN.6 | IMRH |

which is `dorado_decode_mir`'s byte table exactly. Two of its comments are
confirmed outright -- "MIR0's extra bit is RSTK[0]" and "MIR2's extra bit is
JCN[7]" -- and the other two are named: what the C emulator calls the parity
bits P015 and P1631 are the board's `sIMLH` and `sIMRH`, the IM left and right
half.

`cpreg_vectors.c` strobes ONE byte at a time from a cleared MIR, so the fields
the C decoder reports are exactly the bits that strobe sets. Five mutations
were tried and each fails: assembling FF LSB-first, reversing RSTK, sampling
after the strobe releases instead of during it, leaving the function code
uncomplemented, and swapping BSEL for LC.

Two things worth keeping. **The field lines are SET inputs, not register
outputs** -- a decoder output is a level that stands while the strobe is
asserted, so they are read with the strobe low and released afterwards. And
**PARC's `.0` is the most significant bit of a field**, here as everywhere:
`sRSTK.0` is the RSTK bit the Hardware Manual puts in iw2 rather than iw0.

## A microinstruction is JAMMED INTO THE MIR AND READ BACK (2026-08-17)

`make -C verilog mirreg-diff`: **144 microinstructions jammed and read back, 0
field mismatches.**

`mir-diff` proved the decoders -- which `CPOut` bit reaches which field line.
This proves the REGISTER: that the bits stick, that four strobes accumulate,
and that what comes out is the microinstruction the C emulator says it is. It is
what the BaseBoard does 475 times during a cold boot, since it cannot write IM
directly.

**The MIR is a bank of set/reset flip-flops**, which the software model does not
show. Every field bit is half an MC10231:

```
  S = s<FIELD>    the BaseBoard's jam path, from the MC10172 decoders
  D = d<FIELD>    the execute path, from IM
  R = rMIRa       the reset
  C = clk0'..     the microinstruction clock
  Q = <FIELD>     the signal the datapath runs on
```

So a jam is: reset the lot, then SET the ones the four bytes call for. That is
exactly why the Control function carries a `ClrMIR` bit and why the four strobes
accumulate rather than each loading a register. `ClrMIR` is `CPOut.5`, the data
byte's bit 2 -- `data & 0x04`, precisely what the C emulator tests. All five
Control bits check out the same way: `rStop` from `CPOut.1` (0x40, ClrStop),
`StopAtT1` from `CPOut.2` (0x20), `Jam` from `CPOut.3` (0x10), `rCT` from
`CPOut.6` (0x02). The sixth is a naming difference worth knowing: the bit the C
emulator calls **Freeze** (0x08) the board calls **`NoDispatch`**.

**Three things this test got wrong first, all of them instructive:**

* **`rMIRa` crosses the BACKPLANE.** ContA makes it and ContB receives it, and
  both boards drive it, so it resolves as a wired-OR. Instantiating the two
  board modules side by side leaves ContA's own reset unconnected to its own
  flip-flops, and nothing clears. The fix is to use
  `sil_backplane.py --boards=ContA,ContB`, which resolves it -- so the test runs
  a real two-board machine, and `make -C verilog lint` now elaborates that
  configuration too.
* **BLOCK is wired INVERTED, and it is the only field bit that is.** For every
  other bit `s<FIELD>` goes to S and `<FIELD>` comes off Q. For BLOCK, `sBLOCK`
  goes to **R** (on both e23 and g16), `rMIRa` goes to **S**, and `Block` comes
  off **Q'**. The two inversions cancel, so it means the same thing -- but until
  the backplane reset reaches that S input, `Block` sits at 1 no matter what,
  which is what the first run showed on every vector.
* **The claim that this test holds the microinstruction clock stopped was
  false**, and is now stated correctly. Driving `CLK.ca'`/`CLK.cb'` with a
  square wave toggles them 999 times and moves `clk0'Bc` ONCE: the Control
  board's clock generator is gated by the run/stop state, which is idle here. So
  the machine being STOPPED is why a jam sticks, and this testbench does not
  exercise the Freeze path at all. Measured, not assumed -- the mutation that
  should have failed passed, which is how it came to light.

Five mutations were tried; four fail as they should (no ClrMIR between vectors,
ClrMIR never released, BLOCK polarity flipped, one of the four strobes dropped)
and the fifth is the clock one above.

## The REPLY path, specified -- and the C emulator does not model it (2026-08-17)

The BaseBoard writes the machine through `CPOut`/`CPAddr`/`CPStrb'` and reads it
through **`CPIn.0-3`**, four bits at a time. That side is not in the C emulator
at all, so there is no oracle to diff against and no test here -- writing one
from the netlist would only check the netlist against itself. What follows is
the specification, which is the useful output: it is what the read side needs
if it is ever implemented, and it is complete.

Four **MC10164** eight-input multiplexers on ContB (d04, d03, c03, b03) drive
`CPIn.0`, `.1`, `.2`, `.3`. The select is `CPOut.0/1/2` -- the TOP THREE BITS of
the byte the BaseBoard is presenting, MSB-first as ever, so `data >> 5`. No
strobe is involved: the path is combinational, and the BaseBoard reads it by
setting `CPOut` and looking.

That gives a 32-bit status window read as eight nibbles:

```
select | CPIn.3        CPIn.2         CPIn.1        CPIn.0
-------+---------------------------------------------------------
   0   | RBMux.03      RBMux.02       RBMux.01      RBMux.00
   1   | RBMux.07      RBMux.06       RBMux.05      RBMux.04
   2   | RBMux.11      RBMux.10       RBMux.09      RBMux.08
   3   | RBMux.15      RBMux.14       RBMux.13      RBMux.12
   4   | RamPEenable   MDPEDly        IMLHPEDly     IMRHPEDly
   5   | MemPEenable   MemPEDly       RAMPEDly      IOPEDly
   6   | MDPEenable    DoradoStopped  bIMLHPE       bIMRHPE
   7   | StopMIRClkEn  IOPEenable     IMLHPEenable  IMRHPEenable
```

Selects 0-3 are the 16-bit **`RBMux`** read-back bus a nibble at a time -- how
Midas reads the machine's registers -- and 4-7 are the parity-error enables and
delays plus **`DoradoStopped`**, which is the counterpart of the C emulator's
`dorado_running`. It is read on `CPIn.2` with select 6, and that was checked:
the multiplexer routes it.

**Two things measured while establishing this**, both worth keeping:

* **The Control latch bits are where the C emulator says.** Strobing function 0
  with one-hot data and watching ContA's j02: `rStop` follows 0x40, `Jam`
  follows 0x10 and `NoDispatch` follows 0x08 -- the emulator's ClrStop, Jam and
  Freeze masks exactly, with `NoDispatch` being the board's name for the third.
* **`DoradoStopped` stays asserted throughout**, which is correct and not a
  fault: the machine has no clock and no microcode, so it IS stopped. The run
  logic develops only once `clk0'` runs, which needs a machine that is going.

Nothing here contradicts the C emulator. Function 1 -- `CP=UseCPReg`,
`ClrReady`, `GetTLink` -- is what selects between these sources and CPReg for
some reads, and remains the one function the C model treats as a no-op.

## THE MACHINE RUNS (2026-08-17)

`make -C verilog run-test`. Over 40,000 fabric cycles after PARC's own boot
sequence: **`clk0'` 2,493 edges, `clk2'` 4,987 -- exactly twice -- `Phase0`
2,494, `StartCycle'a` 2,494, and `DoradoStopped` clear.** The Control section is
executing microinstruction cycles.

Everything before this proved the machine could be WRITTEN. This is the first
time it does something by itself, and it reports so on the readout bit PARC's
`doradoio.mdefs` calls `Stopped`.

**One thing unlocked it, and it is the only mutation that fails the test:
ClrStop and SetRun must go in the SAME Control byte, 0x41.** `rStop` is a LEVEL
out of the latch at ContA j02 and it lasts only until the next Control strobe.
PARC's `DoDoradoMicroInst` issues ClrStop and SetRun as separate strobes, so by
the time SetRun lands ClrStop has been withdrawn -- and `Stop` re-latches
`dStop` before the machine gets going. Issued together, the stop latch is held
clear across the moment the run latch sets.

That the real machine does not need this is presumably a timing matter, or
something a board outside ContA/ContB supplies; it is the obvious loose end.

### Three earlier conclusions this corrects

The previous section here said the machine could not start because "the clock
enable is clocked by the clock it enables" and "`preRunClk'Bb` is an OR that
`bCLKEnable'a` gates", so nothing could ever tick. **That was wrong, and
measurement is what showed it.** `preRunClk'Bb` toggles 187 times per 3,000
fabric cycles and `RunClk'a` with it; `Run'` DOES clear when `SetRun` arrives.
The stuck term was never `Run'` -- it was `Stop`, re-latching `dStop`.

Two hypotheses recorded there are still correctly eliminated (`SetRunRfsh`, and
a complementary power-up state), and one recorded next step turned out to be the
right one: **work forward from the free-running clock rather than back from the
stall.** ContA's l01 takes only `CLK.ca'`, so `prepreclk'a/c/d` run
unconditionally, and following them forward is what showed the run latch working
all along.

### Two things that look load-bearing and are not

Both were measured, because both were assumed first:

* **The microinstruction PARITY BITS.** The extra bits of MIR1 and MIR3 are the
  left- and right-half parity -- `sIMLH`/`sIMRH` on the board, "P015"/"P1631" in
  the C emulator -- and they really do reach two MC10170 parity generators on
  ContB: `IMLHPE'` tracks the MIR1 extra bit exactly, and `Error'` responds. But
  setting either wrong does NOT stop the machine here; it runs 40,000 cycles
  either way. The path needs `IMLHPEenable`, which comes off the Midas
  diagnostic-mux chain and is 0 in this configuration.

  > **This one turned out to be load-bearing after all, and the last sentence
  > is why.** Enable it -- `DisableDoradoErrors`, one of the two manifold
  > words PARC's boot ROM writes at power-up -- and the parity path becomes
  > the thing that FREEZES THE MIR, which is what lets a jammed
  > microinstruction execute at all. See "What holds the MIR" above. It is
  > still true that it does not stop the machine; it stops the MIR clock.
* **The `Clock` function.** `DoDoradoMicroInst` opens with
  `DoClock(InhibitCAHolds+ClrReady)`; removing it changes nothing.

### What the run test asserts

Not merely that something wiggles. `clk2'` must run at exactly twice `clk0'`,
which is the two-phase relationship the whole machine is built on, and `Phase0`
and `StartCycle'a` must each step once per `clk0'`. Those ratios came out
2,493 / 4,987 / 2,494 / 2,494 -- within an eighth of exact on every one.

## FOUR BOARDS, and the microinstruction is on the datapath (2026-08-17)

`make -C verilog datapath-test`. `dorado_proc` is ContA, ContB, ProcH and ProcL
wired by the backplane; the boot sequence is tb_run's, but the microinstruction
jammed is a distinctive one rather than all zeros. Two questions, both answered:

**The clock reaches the datapath, exactly.** ProcH's `Clock0'Bc` and ProcL's
`Clock0'Bd` each moved 2,493 times against the Control boards' 2,493 -- asserted
as EXACT equality, not a tolerance. The processor boards step in lockstep with
the Control section.

**And so do the fields.** Jamming `MIR0=0xE0+extra`, `MIR1=0xE0+extra`,
`MIR2=0xC0`, `MIR3=0x70+extra` puts, at the processor:

```
  RSTK = 1111    ALUF = 0111    BSEL = 011    ASEL = 111
```

which is exactly what the `mir-diff` table predicts. That one read exercises the
whole path end to end: the BaseBoard's nine-bit bus, the function decode, the
MC10172 demultiplexers, the MC10231 register, and the backplane.

**Some fields cross COMPLEMENTED and some do not.** `BSEL'` and `ASEL'` are
primed on the backplane; `RSTK`, `ALUF` and `LC` are not. That is the boards'
own choice rather than a convention, and it is read off the generated port names
rather than assumed -- reading BSEL uncomplemented is one of the three mutations
that fails this test, along with dropping the MIR2 strobe and splitting ClrStop
from SetRun.

## IM, mapped -- and it confirms the C emulator's Write-IM model exactly

**IM is on ContB, and it is 144 `F10415A` packages.** Each is a 1024 x 1 ECL
RAM, so 144 x 1024 = 147,456 bits = **4096 words x 36 bits**, which is the
34-bit microinstruction plus its two parity bits. That is the same 36 the `.MB`
format implies -- `MB_MEMORY` declares a storage width of 64 for a logical 36 --
now confirmed from the hardware.

The array's 144 packages resolve to **36 distinct data outputs, four packages
each** (four banks of 1024), and those 36 outputs are the microinstruction, one
signal per bit:

```
dRSTK.0-3  dALUF.0-3  dBSEL.0-2  dLC.0-2  dASEL.0-2   (17)
dBlock'  dFF.0-7  dJCN.0-7                            (17)
dIMLH  dIMRH                                          (2 parity)
```

**Those outputs are the `d<FIELD>` lines that feed the MIR's D inputs.** So the
whole core is now one picture: IM -> `d<FIELD>` -> the MC10231 microinstruction
register -> `<FIELD>` -> the backplane -> the datapath. And `dBlock'` comes out
of IM COMPLEMENTED, which is exactly consistent with the MIR flip-flop for BLOCK
being wired inverted (`sBLOCK` to R, output from Q') -- two independent oddities
that agree.

### The half-write is the C emulator's, bit for bit

The write enables split into `WEL'` and `WER'`, and each covers **exactly 18
bits**:

```
WEL'  dRSTK.0-3 dALUF.0-3 dBSEL.0-2 dLC.0-2 dASEL.0-2 dIMLH   = iw0 + RSTK.0 + parity
WER'  dBlock' dFF.0-7 dJCN.0-7 dIMRH                          = iw1 + JCN.7  + parity
```

which is `cpu.c`'s Write-IM comment verbatim:

> `RSTK[3]` -- half-select. 1 = LH (writes iw0 + RSTK[0] of the destination),
> 0 = RH (writes iw1 + JCN[7] of the destination). `RSTK[2]` -- value of the
> secondary bit. `RSTK[1]` -- parity bit.

The data inputs confirm the rest of it. `RBMux.00`-`.15` each feed EIGHT
packages (four banks x two halves) -- the 16 bits from B. `RBMuxP` feeds eight
more: the parity bit, `RSTK[1]`. And two further nets feed four packages each,
one per half: `MidasOrRSTK.2` and `ContB04.sil+1` -- the SECONDARY bit, which is
`RSTK[0]` for the left half and `JCN[7]` for the right, exactly as the comment
says.

So the same `RBMux` bus the BaseBoard READS through `CPIn` is what IM is WRITTEN
from. That is the whole jam mechanism in one sentence.

## PARC's hand-coded microinstructions, decoded (2026-08-17)

`doradoboot.masm` carries an **IRTable**: microinstructions hand-written in the
five-byte format `DoDoradoMicroInst` jams, "to be jammed into IM and executed
for their side effects". Its own comment states the byte layout --

```
0:  RSTK.0,P015,JCN.7,P1631,0,0,0,0
1:  RSTK.1,RSTK.2,RSTK.3,ALUF.0,BLOCK,FF.0,FF.1,FF.2
2:  ALUF.1,ALUF.2,ALUF.3,BSEL.0,FF.3,FF.4,FF.5,FF.6
3:  BSEL.1,BSEL.2,LC.0,LC.1,FF.7,JCN.0,JCN.1,JCN.2
4:  LC.2,ASEL.0,ASEL.1,ASEL.2,JCN.3,JCN.4,JCN.5,JCN.6
```

-- which is the `mir-diff` table exactly, arrived at from the other side, and it
names the two parity bits `P015` and `P1631` just as `cpu.c` does.

Decoded (the source is `.RDX 2`, and `X^n.` is X shifted left n):

```
Nop#            70 01 0F 4C 40      IFUReset#        60 02 0F 44 40
CPRegToLink#    30 13 EF 04 40      Return#          60 13 E1 42 43
IMLHRSTK.0Is0#  60 33 EF 03 4F      IMLHRSTK.0Is1#   20 73 EF 03 4F
IMRHBLOCKIs0#   20 13 EF 03 4F      IMRHBLOCKIs1#    60 53 EF 03 4F
QFromCPReg#     30 13 EF C4 40      TFromCPReg#      70 03 0F 04 C0
ALUFM[0]FromQ#  30 05 09 C4 40      SetMcr#          30 02 0B 84 60
SetHoldTaskSim# 70 13 EF 84 40
```

**All ten with field comments decode to exactly what PARC says they are** --
`CPRegToLink#` really is `RSTK[0],ALUF[17],BSEL[0],LC[0],ASEL[4],FF[176],
JCN[201]`, and so on. That validates the whole five-byte format independently,
and gives a reusable vocabulary of real microinstructions to jam.

Two of them are the Write-IM pair, and their `JCN=177` octal is precisely the
encoding `cpu.c` derives from the Hardware Manual (JCN[1]=1, JCN[2:4]=7,
JCN[5:7]=111). Their RSTK values confirm the half-select and secondary bit:
`IMLH...Is0#` has RSTK=1 (RSTK[3]=1, left half), `IMLH...Is1#` RSTK=3 (left
half, secondary 1), `IMRH...Is0#` RSTK=0 (right half), `IMRH...Is1#` RSTK=2.

## A jammed Write-IM deposits into IM, and RSTK[3] selects the half

`make -C verilog writeim-test`. From a wiped array, the right-half Write-IM sets
17 right-half cells and **0** left-half; the left-half one sets 16 left-half and
**0** right. Checked both ways across all 147,456 cells. `RSTK[3]` selects the
half, precisely as `cpu.c` says.

**Getting the BASELINE right was the whole difficulty, and the first version of
this test was worthless because of it.** IM comes up with 17 cells set: from the
all-zero initial state the ContB write logic asserts its active-low enables for
a few cycles until the combinational logic settles, and the array takes a write
of whatever `RBMux` then held. The first test counted non-zero cells from time
zero, found 17, and called it a successful write. Four mutations passed, and a
no-stimulus control reproduced it exactly with the machine stopped.

**The enables are not stuck**, which was the next wrong theory and is worth
recording as such. MEASURED: after 2,000 idle cycles `WER'Aa`, `WEL'Aa` and
`preWE'a` all read 1, de-asserted, and across the next 20,000 cycles the array
takes nothing at all. It is a settling transient, not a stuck line -- and not a
fault in the hardware either, since Boot0 exists to load IM and the machine
never assumes IM holds anything at power-up. The fix was to the TEST: settle,
wipe, then measure.

(A third wrong theory, tried and discarded on the way: that PARC's
`PrepareProcessor` -- which really does exist, and whose comment is "do a lot of
microinstructions to clear out the cobwebs", forty of them -- was what settled
the enables. It is not. Pure idle settles them just as well.)

### What it does not prove -- and why: A JAM MUST BE SINGLE-STEPPED

The DATA does not come from CPReg (zeroing CPReg leaves the written pattern
unchanged) and nor does the ADDRESS (every write lands at IM[0]). Tracing that
produced the most useful correction of this whole sequence.

**The CPReg-to-B path, fully identified.** `CPReg.00/01/08/09` feed an MC10159
quad multiplexer at ContA b02, which drives `BMux.00/01/08/09`. Its SELECT is
`UseCPReg` and its ACTIVE-LOW ENABLE is `B_Link'`, and both come from a clocked
register at ContA c17 fed by the FF-field decoders. The decode itself is
`FF=Link_CPReg` at a17, an MC10100 over **`FA=1'`, `FB=7'`, `FC=6'`** -- exactly
the FA=1 FB=7 FC=6 that `cpu.c` documents for `B <- RWCPReg`. The hardware
decodes FF into FA/FB/FC sub-decodes and recombines them, just as
`ff_override_b()` does.

**Why it never asserted: free-running destroys the jam.** Measured, with the jam
in MIR and the machine STOPPED, `FF` reads `01111110` -- 176 octal, exactly
PARC's `CPRegToIM#` -- and `FF=Link_CPReg` asserts. Start the machine with
ClrStop+SetRun and one microinstruction clock later `FF` reads `11111111`: **the
MIR has reloaded from IM**, because `d<FIELD>` is the execute path and IM is
empty. The jam survives only as long as the machine is stopped.

**So a jam must be SINGLE-STEPPED, and that is what `SetSS` is for.** PARC's
`DoDoradoMicroInst` ends with `Control(SetRun, SS=ShouldSingleStep)` -- SetRun
WITHOUT ClrStop, plus SetSS -- which executes exactly one microinstruction from
the jam. Measured: single-stepping leaves `FF` at `01111110` and
`FF=Link_CPReg` asserted, where free-running does not.

**This corrects `tb_run`'s conclusion**, or rather bounds it. That test found
ClrStop and SetRun had to go in one Control byte, and that is right FOR
FREE-RUNNING -- it is how you get the machine to sequence continuously. It is
the wrong mode for a jam. Two different operations, and conflating them is what
cost the operand.

### The operand: it was never the FF decode, it was the MIR CLOCK

`make -C verilog operand-test`. **The jammed microinstruction's DATA comes from
CPReg**: `CPReg=a53c` puts `5ac3` on BMux and RBMux, `CPReg=1234` puts `edcb`,
and IM takes a different pattern for each.

The section this replaces said the open task was "work out which of b16's gates
drives `ContA04.sil+2`". **Three things about that were wrong**, and all three
came from following the wrong net.

* **`B<-Link'` does not come from b16 at all.** It is ContA **a13** pin 2, an
  MC10102 gate a = `NOR(FF=ReadLink, FF=Link_CPReg)`, driving `ContA04.sil+4`
  into c17 pin 7 -- and MC10176's `[FF 5>2, 6>3, 7>4, ...]` makes pin 7 the D
  input whose Q is pin 4, which is `B<-Link'`. b16 feeds c17's OTHER inputs:
  pins 5 and 6 (both `ContA04.sil+2`, giving `Link<-BMuxa` and `Link<-BMuxb`)
  and pin 10 (`ContA04.sil+5`, giving `UseCPReg`).
* **b16's decode needed no work.** The dictionary states it outright --
  `MC103: a,IN,4,5>a,o,2 ; b,IN,6,7>b,o,3 ; c,IN,12,13>c,OUT,9>c,o,15 ;
  d,IN,10,11>d,o,14`. Gates a AND b both drive `ContA04.sil+2`, an on-board
  wired-OR making one 4-input OR of `FF=WriteLink`, `FF=Link_CPReg`,
  `FF=BigBDispatch` and `FF=BDispatch`; gate c gives
  `ContA04.sil+5 = OR(CP=UseCPReg, FF=Link_CPReg)`.
* **The combinational half was already right.** `ContA04.sil+4` goes 1 -> 0 at
  the exact moment `FF=Link_CPReg` asserts. Measured.

**What was actually wrong is that c17 never clocked.** `clk1'Ca` gets ZERO
edges while a jam is in the MIR -- single-stepped and free-running alike. The
clock tree is fine: free-running it runs 1,240 edges, in step with `clk0'Ca`.

**Because within one microinstruction the MIR loads FIRST.** The order is
`h*clk0'` rises and the MIR takes `d<FIELD>` from IM, then `clk1'` rises and
c17 latches what the FF field decodes to. Coming out of a stop, clk0 is always
first. So IM overwrites the BaseBoard's jam half a microinstruction before the
register that would act on it ever clocks. Measured: free-running, `FF` goes
`01111110` -> `11111111` at fabric cycle 206 and `clk1'` does not pulse until
214.

### What holds the MIR: PARC's boot ROM sets it at power-up, through the manifold

`h*clk0'Ca` is ContA **c19**, and it is an OR of three things:

```
h*clk0'Ca = RepeatCurC | StopMIRClk | preclk0'Cb
```

`RepeatCurC` is the microcode's own hold (ContA f20, off `CAHold'` and
`SwitchUp`) and the BaseBoard cannot reach it. `StopMIRClk` is ContB **k02**:

```
StopMIRClk = NOR(ContB09.sil+3, StopMIRClkEn')
```

where `ContB09.sil+3` (ContB l03, an MC10121) is the **IM parity-error** term,
built from `IMLHPE'`/`IMRHPE'` -- the MC10170 generators at j20/j21 -- and their
enables; and `StopMIRClkEn` is ContB **g03**, an MC10176 whose D is `DMD.06`
and whose clock is `ManClk.7'`. Both come off the **manifold**, and PARC's boot
ROM sets both, once, in `doradomufman.masm`'s `InitManifolds` -- the group
"invoked as a group just after the power supplies are up and stable":

```
DisableDoradoErrors  .ADR (ParityEnables+030)^4   ; all except IM parity errors disabled
SetMidasStopMIRClk   .ADR (MidasStopMIRClk+20)^4  ; turn on MIR debug feature
```

**So the "MIR debug feature" -- freeze the MIR on an IM parity error, so the
failing microinstruction is still there for Midas to read -- is ALSO what makes
a jam executable.** A microinstruction the BaseBoard put into the MIR did not
come from IM and does not satisfy IM's parity, so the MIR clock stops and the
jam survives its own first clock. Measured: with the jam in the MIR,
`IMLHPE'` and `IMRHPE'` both read 0.

### The manifold, decoded -- and it agrees with PARC's table three ways

`tb_operand` drives it the way the BaseBoard does, so the decode is a gate
rather than a reading. `SetMufflerAddress` shifts **twelve bits, MSB first**,
each on a Clock strobe carrying `ShiftDMD` plus the bit in `DAddrBit`; the
BaseBoard turns those into `DMuxData`/`DMuxClk`; ContB's l22 and l20 (two
MC10176s) are the shift register, clocked by `bDMuxClk'` = `~DMuxClk`, so the
FIRST bit in ends at `DMD.00` and the last at `DMD.11`. Then `DoClock(UseDMD)`
/ `DoClock(0)` pulses `ManClk`.

| DMD | what it is | checked by |
|---|---|---|
| `.00 .01 .02` | board select; ContB answers to 000, which is `UseDMDEnable' = DMD.00\|.01\|.02` | matches `ControlBManifold = 0` |
| `.03 .04 .05` | register number, decoded by k01 into `ManClk.0'`..`.7'` | `ParityEnables` is 0 and `MidasStopMIRClk` is `7^6`; both land |
| `.06` .. `.11` | the six data bits, MSB first | `030` sets EXACTLY `IMLHPEenable` and `IMRHPEenable` and none of the other four -- PARC's comment, bit for bit |

`ManClk.0'` clocks ContB **c04**, which IS the ParityEnables register
(`IMRHPEenable`, `IMLHPEenable`, `IOPEenable`, `RamPEenable`, `MemPEenable`,
`MDPEenable` off its six Qs); `ManClk.7'` clocks g03, which is `StopMIRClkEn`.
Five mutations fail the gate: dropping either manifold word, shifting the
twelve bits LSB-first, reading BMux uncomplemented, and dropping a MIR strobe.

**BMux carries the COMPLEMENT of CPReg**, and that is the part, not a bug:
EclDict gives ContA b02's MC10159 `Q0'..Q3'`, four inverting outputs. Reading
it uncomplemented is one of the mutations.

`DMuxData`, `DMuxClk` and `UseDMD` are FORCED in the testbench because
**BaseBd l24 drives all three** and `dorado_proc` has no BaseBoard. They are
backplane signals a BaseBoard would supply, not internal state. (ContA drives
them too, from the microcode's `FF=UseDMD` and the Midas connector, which is a
different path and gated by `SetRun`.)

### Two cell bugs found on the way, both on this path

* **`cell_MC10121`, 44 packages** -- and one of them is ContB l03, which gates
  the MIR clock. Pin 10 was ANDed in as a term of its own; it is **shared
  between two OR groups**. The data book's DIP table labels it `A2IN, A3IN`,
  exactly as MC10117's pin 9 is `A2IN, B2IN` (which `cell_MC10117` already
  handles), and EclDict says it the way it always says a common input -- once,
  under role `c`. **Tim's common-pin class again**, and `cell-check` could not
  see it: the gate does read all eleven pins, only the grouping was wrong.
* **`cell_MC10104` gate d** gave BOTH its outputs the same expression -- the
  fault the handoff already records for `cell_MC10103`. p15 is the AND, p9 the
  NAND.

`make -C verilog cell-check` now also runs **`tools/sil_check_polarity.py`**,
which asserts the two properties that ARE universal, both by evaluating the
cells' own expressions over every input assignment:

* where the dictionary gives one gate both an `OUT` pin and an `o` pin, the two
  must be exact complements; and
* two parts that share a pinout and a `[G]` summary but are NOT second sources
  of each other must not have the same model -- which is what `cell_MC1662`
  copied from `cell_MC1664` was.

Second sources are recognised by part number with the family and speed letters
stripped (`MC10164`/`MU10164`, `SN74LS01`/`SN74S01`), not by the shared pin
block -- TtlDict groups by PINOUT, which is why `H01, H02, LS01, LS02, LS28`
sit on one line. Two genuinely-different pairs that legitimately share a model
are named with their reasons rather than silenced. 56 cells, 28 both-sense
gates, 47 distinct pinouts, 0 wrong; mutation-tested by putting the MC10104,
MC10102 and MC1662 faults back.

It also checks ADDRESS BIT ORDER on the memories -- see "The memories had their
address bits backwards" above.

It reports rather than asserts the OUTPUT DIRECTION, and two false positives of
its own are worth knowing: an expression it cannot evaluate (a ternary multiplexer,
a concatenation) means the pair cannot be COMPARED -- calling that "the same"
made MC10158/MC10159 look like copies when they are correctly two models.

### The IM ADDRESS, traced -- and `cell_MC1662` was the OR part's model

Every jammed Write-IM landed at IM[0], and the cause was one cell.

The path, from the array back:

```
IM (F10415A)  addressed by RA.01a..RA.10a + four chip selects
   RA.nn      ContB a12/b12/../l12, MC10211 NOR buffers of
   dRA.00'-11'  ContB e20/f20/f21/g20/g21/h20, SIX MC1662 packages:
                dRA.nn' = gate(TNIA.mm, SWa) wired-OR gate(SW'a, BNPC.mm)
   TNIA.04-15   from ContA -- twelve bits, 4096 words
   BNPC         the per-task saved PC; `SW` selects between them
```

**`cell_MC1662` computed OR.** MC1662 and MC1664 are a complementary pair with
the SAME pinout and the same `[G]` summary, and the dictionary separates them
by role letter alone -- `OUT` on the '62, `o` on the '64 -- which is exactly
what that letter is for. `cell_MC1664`'s own comment already said it ("the
MECL III pairing where MC1662 is the NOR part"); this file had simply been
written as a copy of it. The data book is no help: the MC1662 sheet was DELETED
from DL122 rev 7.

The circuit settles it independently. Those six packages are a 2:1 multiplexer,
which they are only as NORs. As ORs the second leg is `SW'a | BNPC`, and `SW'a`
is 1 whenever the machine is not task-switching, so every `dRA'` sticks high and
IM is addressed at 0 whatever TNIA holds. **33 packages**, across ContA, ContB,
IFU, MemC, MemX, ProcH and ProcL -- ProcH/ProcL's are `alub.00-15`, the ALU's B
input, and `RbAdr.4-7'`, the RM address.

With it fixed, `dRA` tracks `TNIA` exactly and `CPRegToLink#` works end to end:
`CPReg=002A` -> `BMux=ffd5` -> `Link[4:15]=fd5`.

### The memories had their address bits backwards

Three memory cells assembled their address LSB-first when PARC wires them
MSB-first. **`cell_F10415A` is IM** -- 144 packages -- and it is the one with
proof that owes nothing to convention:

* the board wires pin 2 to `RA.01a` and pin 12 to `RA.10a`;
* `RA.01a` comes from `dRA.01'`, which ContB's multiplexer takes from
  `TNIA.05`, the SECOND MOST significant address bit, while `RA.10a` traces to
  `TNIA.14`, the second least;
* so pin 2 carries the more significant bit -- and the cell had it as the
  least.

Measured: a Write-IM whose address lines read **195** deposited at **780**,
which is 195 with its ten bits reversed. Fixed, it deposits at 195.

`cell_F10145A` (**405 packages**, the biggest cell in the machine -- the 16x4
ECL register file behind TLink) and `cell_F10470` (the DRAM) had the same
inversion and are fixed with it. **The other seven addressed memories were
audited and are right**: `MB7071H` (RM and STK -- ProcH h06 takes `RbAdr.0` on
the dictionary's A0 pin), `F10414` (STK, which says `// A0..A7, MSB first` in
its own comment), `i2716`, and the three PROM parts, whose bit order
`prom-test` already pins. `cell_i2125` is the one genuine exception and is
named as such: its address is the BaseBoard's own `RA0..RA9` off a 6502 bus,
where 0 is the LEAST significant.

`sil_check_polarity.py` checks this now, so it cannot come back: for every
cell whose dictionary entry names `A0..An`, the address expression's first
element must be the pin the dictionary calls `A0`, with `i2125` exempted by
name and reason. Mutation-tested by reversing five memories' addresses in turn;
each is caught.

**The convention is stated by the repository's own cells.** `cell_F10414` --
the same family, the same board, the STK register file -- carries the comment
`// A0..A7, MSB first`, and `cell_i2716` was corrected for exactly this before
("PARC names those MSB-first: the dictionary's A0 is pin 19, the sheet's A10
... a reversed address would have scrambled the image silently").

**Why nothing caught it.** A consistently reversed address is a PERMUTATION:
writes and reads use the same mapping, so a machine that only reads back what
it wrote behaves identically, and `writeim-test` counts bits rather than
addresses while `operand-test` asserts `dRA == TNIA` rather than where the bits
land. It bites the moment IM is compared against something external -- a `.MB`
file, or the C emulator -- which is exactly what Boot0 is for.

### The machine single-steps, and `rStop` is why it took three tries

`make -C verilog step-test`. Steps two through five are each **`clk0'` 2,
`clk1'` 2, `clk2'` 4, `Stop` 1** -- one microinstruction, stopping again -- and
`Link[4:15]` reads `fd5` throughout, which is `CPReg=002A` complemented: the
jammed `CPRegToLink#` executed.

Three things had to be understood, and each first presented as the machine
being broken:

* **`SetRun` alone runs half a microinstruction.** PARC's
  `DoDoradoMicroInst` ends with SetRun and SS and no ClrStop, and that gives
  `clk0'` twice and `clk1'` not at all. It is not a fault: the phase generator
  comes out of reset with `sPhase0` held and `StartCycle` cleared, so the first
  window is the clk0 half. Every step after it is whole -- and PARC's
  `DoIRTableInstAndNop` says as much, jamming a Nop after every IRTable entry
  because "the Nop holds CPReg constant through T3 of the PREVIOUS
  instruction".
* **`ClrStop+SetRun` on its own FREE-RUNS.** `rStop` is a LEVEL out of ContA
  j02 latched from `CPOut.1`, and it lasts until the next Control strobe.
  Issued once and left, it holds the stop latch reset for ever: 26 `clk0'`
  edges in a window where a step gives 2, with `Stop` still 0. A following
  Control strobe without ClrStop is what lets the machine stop again. This is
  the other half of `run-test`'s finding -- sharing the byte is how you START,
  letting ClrStop go is how you STOP.
* **The machine runs during the strobes.** The first version counted clocks in
  a window opened after the last strobe and measured zero of everything, which
  looks exactly like a machine that never started. Count continuously.

And one correction made by a mutation that PASSED: the comment first claimed
all three of PARC's strobes were load-bearing. Dropping the middle one still
steps correctly, because `Control(0)` withdraws ClrStop just as
`Control(SetRun)` does -- it is the FOLLOWING STROBE that matters, not its
data. The gate now mutates by removing every following strobe, which does fail.

### `machine-test` was testing a week-old binary, and the machine does not settle

Two separate things, and the first hid the second.

**The gate ran a prebuilt `obj_dir/Vemu` and never rebuilt it.** On 2026-08-18
that binary was dated a week earlier -- it had been reporting on RTL from
before several cell fixes, and passing. `machine-test` depends on `all` now.

**Rebuilt, the assembled eleven-board machine does not converge.** It is
PRE-EXISTING and not the cell work above: stashing all four of this session's
cell changes and rebuilding gives the identical failure. Recorded as measured,
with the three things that narrow it:

* `--converge-limit 2000` does not help, so it is an OSCILLATION, not slow
  settling.
* **The same RTL settles fine under Verilator's event scheduler.**
  `make -C verilog converge-test` builds `dorado_machine` with the same eleven
  boards and runs 20,000 cycles with `CLK.ph'` toggling 2,501 times, both with
  an `always #1` clock and with an explicit `sys_clk = 0; #1; sys_clk = 1; #1;`
  loop. So the boards are not obviously at fault -- the difference is
  `--binary` (event scheduler) against `--cc` plus C++ `eval()`.
* Sweeping `probe_sel` the way the harness does is not it either; that was
  tried.
* `loop-check` still passes, so the candidates are the four structural loops it
  knows about and accepts: the processor's multiplexer chain feeding back, and
  a counter's carry returning to its own count enable on DispM, DispY and MemD.

`converge-test` builds in about five seconds against the harness's several
minutes of imgui C++, so it is the one to iterate on. The pair of gates is the
finding: one green, one red, same RTL.

### And `cell_MC10159` drove its outputs HIGH when disabled

With the address moving, `TNIA` still was not `Link`. Probing each of TNIA's
wired-OR contributions separately -- the generator gives every driver its own
stub, `TNIA_04__g24_3` beside `TNIA_04__g22_14` -- answered it in one run:

```
bit  Link  TNIA  contributions
 .04    1     1    g24_3=1 g22_14=1
 .05    0     1    h24_3=0 g22_15=1        <- Link says 0, the bus says 1
 .06    1     1    g23_3=1 g22_2=1
 .07    0     1    h23_3=0 g22_1=1
 .08    1     1    h22_3=1 i22_2=0 i21_14=1
 ...
```

**All twelve MC10121 selectors carried Link EXACTLY RIGHT.** Every wrong bit
came from two packages, ContA g22 and i21, both MC10159 multiplexers, both
reading 1 where they should have been silent.

`DoradoDocs/datasheets/MC10159.pdf` has the truth table, and its last row is
the one that was missing:

```
Enable  Select  D0  D1   Q
   L       L     X   L   H
   L       L     X   H   L
   L       H     L   X   H
   L       H     H   X   L
   H       X     X   X   L      <- DISABLED: the outputs go LOW
```

A disabled MECL part drives its outputs LOW, which is the entire point of an
enable on a part whose open-emitter outputs are tied into a wired-OR: a
disabled driver must contribute NOTHING. The cell had `~(en & mux)`, which
reads 1 when disabled and so held every bus it sat on to all ones. **67
packages**, 25 of them on ContA. With it fixed, `TNIA` is `Link[4:15]` bit for
bit.

The same sheet also confirms the select polarity that `cell_MC10158` and
`cell_MC10159` both carried a "VERIFY" comment about: SELECT high routes
D00/D10/D20/D30 (pins 6, 4, 13, 11), which is what both cells already did.

**The lesson is about the probe, not the part.** Four sessions of "TNIA is
wrong" would have said nothing; one run printing each driver's own stub said
which package and left eleven others exonerated. When a wired-OR reads wrong,
split it.

### Two gates were passing for the wrong reason, and this exposed both

`datapath-test` and `writeim-test` both jam and then FREE-RUN, which the
handoff says destroys a jam. They passed only because the MIR kept reloading
IM[0] -- the same cell bug. With a working address the machine walks IM and
they failed.

Both now do what a BaseBoard does before it ever jams: write the two manifold
words that hold the MIR. `datapath-test` also had to move its clock probe from
ContB's `clk0'Bc` to ContA's `clk0'Ca`, because the former IS the MIR clock
(ContB j05 takes `StopMIRClkBD`) and the hold stops it on purpose. And
`writeim-test` needed its `setcpreg(16'hFFFF)` changed to `16'h0000`: BMux
inverts, so `FFFF` reached the array as `0000` and a left-half write of zeros
counted as no write. It now reports 34 right-half cells for the right-half jam
and 16 left-half for the left, with `RBMux=ffff` at a real address.

### The gates were not gating: `$fatal` left every rule green

Every test rule ended in `| grep tb_x`, and a pipeline's exit status is the
LAST command's. A testbench that hit `$fatal` printed its assertion and the
rule still succeeded -- which is how the two tests above reported PASS in a
sweep and FAIL when run by hand, in the same minute.

**The obvious fix does nothing here.** `SHELL := /bin/bash` with
`.SHELLFLAGS := -o pipefail -c` is the standard answer, and macOS ships **GNU
Make 3.81** -- `.SHELLFLAGS` arrived in 3.82, so make ignores it silently. The
same failure mode twice in one afternoon. All twelve rules capture the status
explicitly instead:

```make
{ out=$$(./verilog/verilator/obj_x/tb_x 2>&1); rc=$$?; \
  echo "$$out" | grep tb_x; exit $$rc; }
```

which works in any make and any sh. Mutation-tested by turning three tests'
`$finish` into `$fatal` and watching the rules go red.

### Correction: "role `OUT` is the INVERTING output" does NOT generalise

The Reference section below states that rule. It holds for the OR/NOR family it
was derived from and **fails on two parts checked here**: on MC10104 the `OUT`
pins carry the AND and `o` the NAND, and on MC10121 pin 2 (`OUT`) is the plain
OR-AND while pin 3 (`o`) is the OR-AND-INVERT.

**PARC's net naming cannot settle it either, and this is worth knowing because
it looks as though it can.** Of the eighteen MC10121 packages that use both
outputs, nine name them as a complementary pair, and all nine put the primed
name on pin 2 -- unanimous, four different boards, and WRONG. Nine packages
likewise prime pin 9 of an MC10102, whose pin 15 is certainly the NOR. A net is
named for the sense its designer treats as primary, not for which pin inverts.

**What does settle it is the data book's LOGIC DIAGRAM, read by coordinates.**
The pin labels' overbars are in a font the PDF does not embed -- they render as
empty boxes and extract without the bar -- but the inverting BUBBLE rasterises
fine and `pdftotext -bbox` gives every pin label's y in points. Match the two:

```
pdftotext -f <page> -l <page> -bbox DL122_rev7.pdf -    # label y, in points
pdftoppm  -r 400 -f <page> -l <page> -x .. -y .. -png DL122_rev7.pdf out
```

For MC10121 (book page 284, PDF page 286) the labels sit at y=326.5 and
y=336.1 and the arrows at y=324.9 (plain) and y=335.2 (bubbled); for MC10104
(PDF page 251) gate d's bubble is on pin 9, whose label is above pin 15's.

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

## Configuration straps: where each board's identity lives

A board's slow-I/O address and task number are not in its wire list. They are
set by cutting legs off a resistor SIP, and the netlist still lists the pin --
it just has no resistor behind it. The fact lives on the board's configuration
sheet, and `SIP_BROKEN_LEGS` in `tools/sil_to_verilog.py` carries the tables.

Gated by `make -C verilog strap-test` and `make -C verilog muffler-test`:

| board | pack | cut | field | reads | source |
|---|---|---|---|---|---|
| DispM | g41 | 6 | `DDMTIOA` | 36B -> 0360-0367 | "making DDMTIOA = 360B" |
| DispM | b52 | 3,4 | `AltoWTask` | 1001 = 9 | "for Task 9D = 11B" |
| DskEth | e41 | 4,5,6,7 | `TIOA-Ad` | 1 -> 010-017 | "* Standard addresses are 10-17" |
| DispY | g41 | 4,5 | `WakeupWait` | 1100 | DispY31.sil Rev Cl |
| DispY | g42 | none | `DDCTIOA` | 37B -> 0370-0377 | DispY31.sil Rev Cl |
| DispY | k51 | 4,5 | `DDCDMD` | 1100 | DispY31.sil Rev Cl |
| DispY | k52 | 3 | `DWTTask` | 1011 = 11 | DispY31.sil Rev Cl |
| BaseBd | l49 | 2,4,5,7 | `Midas` | 1001 = 9 | "all pins cut except 1, 3, 6, & 8" |

Three of them state the ANSWER on the sheet, so they check themselves. The
encoding comes from the DskEth table, which lists all 32 ranges: **a cut leg
is 0, an intact one is 1, MSB first**, and the five-bit field is the top of an
eight-bit address whose low three bits select the register within the device.

**The C emulator agrees and was not consulted.** `include/dispm.h` defines
`DORADO_DISPM_TIOA_BOARD 0360`; `include/disk.h` defines
`DORADO_DISK_TIOA_DISKCONTROL 010` with the comment "Disk uses task 14 octal
exclusively, on TIOA 10-14 octal". Two models, no shared code, same addresses.

### Three traps

1. **Take the sheet the wire list names.** Every `.wl` header lists its
   constituent `.sil` files with Rev and Date. The scanned
   `DoradoDocs/schematics/DispY.pdf` is Rev Ci 11/02/79 while the wire list is
   Rev Cl 3/25/82, and g41's cut list CHANGED between them (3,4,5 -> 4,5). The
   per-revision scans in `DoradoDocs/doradodrawings/` have the built one.
2. **A resistor leg has no direction.** Sil marks pin 4 of an 8-pin SIP `in`
   as a matter of drawing convention. Requiring `out` left 18 nets with no
   driver at all, including the IFU's `TTLHigh` and six active-low DskEth
   drive-status lines that then read ASSERTED -- a disk that is not there.
3. **A pull-up against a pull-down is not always a divider.** For DskEth's
   `RcvData` it is (an analog bias at the receiver input). For the BaseBoard's
   `Midas` straps it is not: the pull-down pack sets the default 0 and the
   pull-up pack has legs only where a 1 is wanted.

### Pins broken off logic chips

Not only resistor packs. MemX's "Stuffing and Configuration Instructions"
(`Memx23.sil`, 10/29/79) gives two, and only one is ours to apply:

* **Applied.** "2. Break h20.10 before stuffing." h20 is an MC10105 and pin 10
  is `MapPerr`, so `MemPE` is `STPerr | HitPerr`. MapPerr keeps its other two
  consumers (g49.2, k08.5); only this term goes. An open MECL 10K input has an
  internal pulldown to VEE and reads 0. `BROKEN_PACKAGE_PINS` in the generator.
* **NOT applied, on purpose.** "1. If 256 chips are NOT installed in the MSA,
  then break g10.6." That is conditional on how much memory the machine was
  built with, and `ChipsAre256/16K` / `ChipsAre64K` are backplane INPUTS to
  MemX rather than straps on it -- the configuration comes from outside the
  board. The sheet's blue wire (k4.6 / k4.5 / k4.4 to k4.16), the b14 and c12
  jumper sockets, and the PLAT capacitor values are all the same 16K / 64K /
  256K choice. Decide the machine's memory configuration first, then apply one
  column of that table as a set.

### The crystal oscillators run at their own frequencies

Four K1115A positions across three boards, and the configuration sheets give
each a different value. Until `CELL_PARAMS` existed the generator instantiated
every cell without parameters, so all four ran at one rate.

| position | value | source |
|---|---|---|
| DispY a05 | 50 MHz | DispY31.sil Rev Cl note 5 (the Rev Ci scan says 20) |
| DispM c05 | 10 MHz, the VCO | DispM30.sil Rev Ch |
| DispM d13 | 20 MHz | same sheet, "20 MHz for Alto / 50 MHz for LF" |
| DskEth j20 | not stated | Ether12.sil draws it on EClk0 with no value |

d13 is taken as the Alto-style 20: DispM's two monitor modes are 640x480 and
1024x768, a 2.5x pixel-rate ratio which is exactly 50/20, and the C emulator's
head reports the 640x480 "standard" one.

`cell_K1115A` is a PHASE ACCUMULATOR, not an integer divider. sys_clk is
266.667 MHz (16 to a 60 ns microinstruction), so the divisors for these would
be 13.33, 6.67 and 2.67; rounded, they give 1 : 1.86 : 4.33 where the parts
are 1 : 2 : 5. The accumulator carries the remainder, and `osc-test` measures
9.99 MHz at c05 with the ratios exact.

### Not yet read

Only two sheets are headed "Configuration Information" -- the BaseBoard's is
"Stuffing Information" and DskEth's IOA table is on a plain reference sheet --
so search by content. Outstanding: MemC's "cut the 4 107 legs marked X" (an
OPTIONAL conversion of parity into VA.4; the built board is the un-converted
one, so do NOT apply it), MemX's configuration PLATs, ProcL's parallel
discrete resistors, and per-position oscillator frequencies (DispY a05 50 MHz,
DispM c05 10 MHz, DispM d13 20/50 MHz -- all four currently share one divisor).

## The test jumper that is correctly absent

ProcH's Task Simulator sheet says "To enable this test circuit be sure there
is a jumper to connect TestTW (109) to ContA pin (140)". `TestTW` is a ProcH
backplane OUTPUT and ContA has nothing of that name, so the jumper is NOT in
the generated machine -- which is right, it is fitted by hand when you want
the test circuit. Worth knowing before someone reads a board output that goes
nowhere as a missing connection.

## The memory reference-kind chain, derived end to end (2026-08-22)

Three gates on MemC decide what kind of memory reference a microinstruction
makes, and all three are now read off PARC's own wire list rather than
guessed:

| gate | part | function |
|---|---|---|
| `b24` c | MC10103 | `WantProcRef' = IgnoreProc \| ASEL.0` -- is a reference wanted at all |
| `d22` a | MC10117 | `WantCR = ~[(FF.0mem' \| WantProcRef') & (ASEL.1' \| WantProcRef')]` -- cache reference, or alternate |
| `b24` | MC10103 | `WantAltRef' = WantProcRef' \| WantCR` -- the enable on the alternate decoder |
| `j24` | MC10161 | `{EmuOrFT', ASEL.2, FF.1mem}` -> `Flush_'` (Q3) / `Map_'` (Q1) |
| `a24` | MC10162 | `{ASEL.1, ASEL.2, FF.1mem}` -> `IFetch_` / `Lfetch_` |

So an ALTERNATE reference -- Map or Flush -- requires `ASEL.1 = 0` and
`FF.0 = 0` (which makes `WantCR` low) on top of a reference being wanted.

**Both MC10103 and MC10117 name their outputs backwards from the roles here.**
On `b24` pin 9 carries the name `WantProcRef` and is the NOR; pin 15 carries
`WantProcRef'` and is the OR. On `d22` pin 3 is named `WantCR` and is the
inverting output while pin 2, named `WantCR'`, is the non-inverting one. Net
naming settles nothing about polarity -- read the roles and the datasheet.

**And the C emulator agrees, independently.** `EmuOrFT'` is the
emulator-or-fault-task condition, and `cpu.c` comments `DM_REF_MAP` /
`DM_REF_FLUSH` as "emulator/fault" against the IO kinds as "io task". Neither
model was derived from the other, and the reference kind is task-dependent in
both.

`make -C verilog memrun-test` measures the chain, and over 3000 samples the
alternate path decodes correctly 149 times for 149 -- exactly as the `d22`
algebra predicts, on every sample where the flush-shaped FF field is in force.

**The front door is proven open.** With the ASEL=1 reference actually
executing -- 2851 of 3000 samples -- `WantProcRef'` is low on **every one of
them**, from `IgnoreProc = 0` and `ASEL.0 = 0`, which is `b24` gate c's
algebra exactly. Gated in `memrun-test` and mutation-tested.

Getting there meant identifying the running instruction by the right field.
An earlier pass conditioned on `FF.0mem'`, which selected the 149 **startup**
cycles instead of the 2851 running ones and made the whole chain look dead.
**IM is four interleaved banks** -- the low two address bits pick the bank --
so reading IM[0..3] back settles what is actually there: IM[0] and IM[1] both
hold the bench's reference (`L=0101 R=0081`), the machine loops over those
two, `TNIA` 000 is the startup state and 001 the running one, and the
instruction is identified by its ASEL.

Two things remain, both small and both stated by measurement:

1. **RETRACTED -- all four of `build_hunk`'s copies do land.** An earlier
   note said IM[2] and IM[3] read back as zero. That came from reading
   `rd_L2(0)`/`rd_L3(0)` as "IM[2]" and "IM[3]", which they are not:
   `find_word` *searches* all four banks precisely because the bank-to-address
   mapping is not obvious. Scanning every bank at several idx gives the real
   mapping:

   | address | bank | idx |
   |---|---|---|
   | 0 | 0 | 0 |
   | 1 | 1 | 0 |
   | 2 | 0 | 1 |
   | 3 | 1 | 1 |

   i.e. `bank = addr & 1`, `idx = addr >> 1`. Banks 2 and 3 are a different
   part of the store and are correctly empty here. All four microinstructions
   are present, and `build_hunk4` now packs four *distinct* ones.

2. **RESOLVED -- and the microcode now makes a Flush.** `FF.0mem'` and
   `FF.1mem` are inputs only on MemC; ProcH and ProcL drive them from
   `d24`/`d23`, a pair of MC10101s whose pin 12 is the **common** input:

   ```
   FF.0mem' = ~(FFok'a | FF.0)      FF.0mem = FFok'a | FF.0
   FF.1mem  =   FFok'a | FF.1
   ```

   With `FFok'a` high both bits are **forced high**, so the memory section
   sees `ff01 = 3` whatever the microinstruction holds. That is why packing
   `FF = 0o100` changed the IM word (right half `4081` instead of `0081`, so
   the write path was never at fault) and changed nothing at MemC.

   **`FFok'` comes from ContA `f24`**, an MC10211 (NOR) whose two gates are
   **wired-OR** onto each of `FFok'a/b/c`:

   ```
   FFok' = ~BSEL.0'  |  ~(JCN.0 | JCN.1 | JCN.2or3)
   ```

   so it goes low only when **BSEL.0 = 0 and one of those JCN bits is set**.
   The bench used `BSEL = 4` -- BSEL.0 = 1 -- which held `FFok'` high and
   masked the FF field off. **The FF field is only a function field when BSEL
   and JCN are not claiming those bits**, exactly the kind of interdependence
   HM Table 11 encodes.

   With `BSEL = 0` and `JCN = 0o201`, measured over the 2851 cycles the
   reference runs: `FFok'a` high on **0**, `WantCR` low on **2851**,
   `WantAltRef'` low on **2851**, and **`Flush'` asserted on 2851**. All four
   gated; reverting BSEL to 4 is caught.

   **The tension this exposes:** BSEL >= 4 is what puts the A leg on MAR (per
   `compute-test`), and BSEL >= 4 is exactly what masks the FF function off.
   A reference carrying an FF function cannot take its address that way --
   where the address comes from for BSEL < 4 is the next question.

3. **RESOLVED -- the cache misses, and a Store is what makes it.** The rest
   of the chain below `Flush'`:

   | gate | part | function |
   |---|---|---|
   | `l19` d | MC10100 (pin 9 common) | `FlushStore = ~(FSinPair' \| EcHasAb)` |
   | `k21` | MC10176 hex D FF, clocked by `LdPair'` | `FSinPair'` is Q5, fed by D5 on pin 12 |
   | `j23` | MC10117 second gate (pins 10-13, 9 common) | that D input is `FlushInA & HitColDirty` |

   `HitColDirty` is the point: **flushing a clean line needs no write-back**,
   so only a dirty line has anything to store. A single flush against a clean
   cache could never reach the storage path.

   So the bench runs **two instructions**, alternating -- `build_hunk2` packs
   them (copies 0,2 are A and 1,3 are B, so IM[0] = A and IM[1] = B; byte 0
   carries RSTK.0 and BLOCK for all four, so those must match). IM[0] stores
   to dirty a line, IM[1] flushes it, the Local Jumps bouncing 0 -> 1 -> 0.

   **The Store encoding comes out of the gates, not a table.** `j22`
   (MC10105) gate a gives `Store← = ~(CacheRef' | Store←IfCR')`, and `b24`
   gate b gives `Store←IfCR' = Q0 | ASEL.2`, where Q0 is the `a24` decoder's
   zero output addressed `{ASEL.2, FF.0mem', FF.1mem}`. So ASEL.2 = 0, and
   `CacheRef'` low wants `WantCR = 1` which wants FF.0 = 1, and Q0 = 0 with
   FF.0 = 1 then wants FF.1 = 1 -- giving **ASEL = 000 with FF = 0o300**.

   And the whole path runs. Over one 3000-sample run:

   | signal | cycles |
   |---|---|
   | `Store←` asserted | 1440 |
   | `HitColDirty` | 160 |
   | `FSinPair'` fell | 320 |
   | `FlushStore` | 320 |
   | `ForceMiss` | 1088 |
   | MISS(a) / MISS(b) | 1043 / 1043 |

   All gated in `memrun-test`; collapsing the Store back into a second Flush
   is caught.

4. **And the miss starts a storage cycle.** MemX `i14` is an SG10139 PROM
   making `preStartMem'` from `{MapFnc.0', MapFnc.1', MapState.0..2}` -- the
   storage cycle is a **PROM state machine**. Measured over the run: MapState
   takes **7 of its 8 values**, MapFnc 2 of 4, `preStartMem'` is low on 2712
   samples and `StartMem'` on 2680. All gated.

5. **The write is scheduled, and an empty Map entry is what stops it.**
   `MemWEa` is 0 for the whole run, and it is **not a gate**. MemX `c02` is an
   MC10176 clocked by `Clk1'Aa` whose Q0 is `MemWEa`, fed from MemX `i10`, an
   F10016:

   ```
   C = Clk0'Ba    CE' = TrueBD    PE' = MemIdle    MR = STPerr
   H0 -> MemWEa   H2 -> MakeMemCAS
   ```

   **`TrueBD` is a hardwired TRUE** -- `g04` is an MC10195 with pin 12 open,
   the same trick as ProcH's `TrueA` -- so CE' is permanently high and **this
   F10016 never counts**: a parallel-load *register*. Measured: in load on
   2936 samples, allowed to count on 0. (Do not reason about its terminal
   count; that is the bug that once oscillated the whole machine, and it is
   not in play here.)

   So `MemWEa` is D0 registered, and D0 comes from `i11`, an MC10105 gate b --
   a three-input NOR:

   ```
   D0 = ~(WriteInMem' | MemX07.sil+10 | MapTroubleInMem)
   ```

   with each term low on: `WriteInMem'` **1280** (a write *is* in the
   pipeline), `MemX07.sil+10` 96, `MapTroubleInMem` **0** -- asserted the
   entire run.

   **And `MapTrouble` is asserted because the Map entry fails its parity
   check.** `j11` (an MC10176) registers it at `StartMemClk0'` from
   `MapTrouble`, which is `g14`, an MC10121 4-wide OR-AND whose **common**
   input is `ReadOrWriteInMap'`:

   ```
   y = (CheckWP' | MapWP'    | ROWIM')
     & (MapWP'   | MapDirty' | ROWIM')
     & (TrueBD   | ROWIM')                  <- pinned to 1
     & (MapEven' | preRfshInMem | ROWIM')
   ```

   **and `MapTrouble` is the INVERSE of that.** MC10121 puts the plain OR-AND
   on pin 2 and the OR-AND-INVERT on pin 3 -- the *opposite* of the "role
   `OUT` is the inverting output" rule, settled from the MECL data book's own
   logic diagram (PARC's naming here agrees with the roles and is wrong).
   `MapTrouble` is pin 3, so **it clears only when all four groups are 1**.

   Measured over the run, low on: `ReadOrWriteInMap'` 3000, `MapWP'` 0,
   `MapDirty'` 0, `MapEven'` 3000, `CheckWP'` 2691, `preRfshInMem` 3000 --
   with `MapTrouble` high on 3000 and `MapTrouble'` on 0.

   **So the blocker is the fourth group**, and an earlier version of this note
   blamed the wrong one: `MapWP'` and `MapDirty'` being *high* makes groups 1
   and 2 equal 1, which is exactly what is wanted. Group 4 is
   `(MapEven' | preRfshInMem | ROWIM')` and all three are low.

   **`MapEven'` is the map parity check.** MemX `e11` is an MC10170, the
   9-bit parity part, taking `RP.00`..`RP.08` -- the map entry's real-page
   bits -- with `MemX13.sil+1` on the carry-in, and pin 15 is its EVEN output.
   An uninitialised, all-zero map entry fails that check, and a failed map
   parity is exactly what `MapTrouble` should mean. (The entry lives in
   `MosRam` packages d11/d13, reached through MC10124 TTL-to-ECL translators
   at e17/e10, so `MapWP` and `MapDirty` are literally the stored bits.)

6. **FIXED -- the map read path was strobed off by one net, and VBB was
   why.** Writing a map entry would not have helped; the entry never reached
   the logic.

   The map array *is* strobed (RAS' 31 edges, CAS' 21, read condition on
   579/3000) and its output *is* live -- but preloading all 21 bit planes
   changed nothing downstream, so the break was after the array.

   It was the MC10124 TTL-to-ECL translators. The cell is right
   (`p12 = ~(p10 & p6)`) and **pin 6 is the common strobe**: *"when the common
   strobe input is at the low logic level, it forces all true outputs to a
   MECL low logic state and all inverting outputs to a MECL high logic
   state."* Exactly what we saw.

   That strobe is `THi`, feeding pin 6 of five MC10124s -- the whole map read
   path -- and it comes from e15 (MC10125) as `THi = p15 & ~p14`, pin 15 being
   `VBBe15` and pin 14 **open**.

   **`VBB` is the ECL switching threshold, not a logic signal**, and all 39
   VBB nets in the machine have zero drivers. A differential pair with VBB on
   one side follows the *other* side, so **VBB must lose to a real signal and
   beat an open pin** -- and the EclDict roles already distinguish them:
   `common` (EclDict's `c`) is the complement input, where VBB reads 0 so the
   true input decides; `in` is the true input, where it reads 1 against an
   open complement. A single constant per *net* cannot do this, because one
   VBB net reaches both sides: on e15, channel a has VBB on `common` with a
   real signal opposite, while channel d has VBB on `in` with an open
   complement and **manufactures the constant TTL high**.

   The rule is in `tools/sil_to_verilog.py` now, beside `OVERRIDE_DRIVERS` and
   `WEAK_PORT_DRIVERS`, and gated: `THi` must be high on every cycle and the
   map outputs must vary. Reverting the rule is caught. With it, `MapWP'` and
   `MapDirty'` go low on 2434 samples where they were stuck. **All 29 gates
   stay green** across the machine-wide change.

7. **The microcode writes its own Map entry, and the fault clears.** With
   the read path open, the entry's *content* decides `MapTrouble`: e17 takes
   `THi` as data and plane d13 as its common strobe, so `MapWP' = ~d13`, and
   likewise `MapDirty' = ~d11`; group 4 additionally wants `MapEven' = 1`.

   **Do not plant the entry from the bench.** The preload *lands*
   (`d13.mem[0]` reads `000` right after the loop) and is *gone* by the
   measurement window, with no write strobe inside it -- **the machine writes
   the map itself during startup**, which is the write path working.

   So the entry is written the way the hardware does it. The loop is **four
   instructions** now (`build_hunk4`), running 0 → 1 → 2 → 3 → 0:

   | addr | reference | encoding |
   |---|---|---|
   | IM[0] | `←Map` | ASEL = 000, FF.0 = 0, FF.1 = 1 (j24 Q1) |
   | IM[1] | `Store` | ASEL = 000, FF.0 = 1, FF.1 = 1 |
   | IM[2] | `Flush` | ASEL = 001, FF.0 = 0, FF.1 = 1 (j24 Q3) |
   | IM[3] | none | ASEL = 100, `WantProcRef'` stays high |

   all with BSEL = 0 so `FFok'` stays low, every JCN keeping a JCN.0 bit set.

   **And it works.** The map planes now read 0 where they read 1 -- the `←Map`
   wrote them -- `MapEven'` is **high on 2354** samples where it was low on
   all 3000, and **`MapTrouble` clears on 2354 of 3000 cycles** where it was
   asserted the entire run. `MapTroubleInMem` is low on 2339, and
   `WriteInMem'` and a clear map **coincide on 352**. Gated; turning the
   `←Map` into a second Store is caught.

8. **The memory size is a backplane input, and it picks the DRAM timing
   PROM.** `MemX07.sil+10` -- the third term of `MemWEa`'s D0 -- is Q0 of
   j13/j14, SG10139s addressed by `{RfshInMem, MemState.0..3}` with pin 15 as
   CE'. Those enables are `Use256/16KProm'` (j13) and `Use64KProm'` (j14),
   driven from **`ChipsAre256/16K` / `ChipsAre64K` -- backplane inputs from
   the MSA**, which is not in this configuration. Undriven, **neither 16K PROM
   is enabled** and the memory state machine has no timing table at all --
   which looks exactly like a sequencer bug and is not one. Gated; leaving
   them undriven is caught.

   Swept: only `ChipsAre256/16K` selects (`ChipsAre64K` moves neither enable).
   With it 0, j14 is enabled and `x10` is low on 96 cycles; with it 1 -- the
   historically correct build -- j13 is enabled and `x10` is never low.

   **A note on the two sockets.** PARC's BCPL names them `MX16k-j13` and
   `MX4k-j14`, and `MemProms.bcpl` records the switch: *"change to
   memx-16k-j13 from -j14. comment-out the memx-4k option. September 26,
   1979."* But **this board is Rev Ch**, later, and calls j14's enable
   `Use64KProm'` -- the socket was re-purposed for a 64K table that does not
   exist in the PROM source we hold. So on a late board j13 is the live socket
   and **j14 should be empty**; our 4K image sitting in it is harmless only
   because the enable keeps it off.

9. **What stops MemState is `StartMem'` itself, and my earlier reading of it
   was backwards.** MemX `j16` is an F10016 with

   ```
   C = Clk0'Dd    CE' = MemIdle    PE' = StartMem'
   H0-H3 -> MemState.0-3,  AND ALL FOUR D INPUTS OPEN
   ```

   Parallel load overrides count, and an open MECL input sits at VEE. So
   **every cycle `StartMem'` is low, the state machine is loaded with zero.**
   Measured: `StartMem'` is low on **2680 of 3000** samples -- 89% -- so the
   counter is free on only **288**, and it reaches all 3 of its observed
   states inside that window (non-zero while held: 32, the transition
   residue).

   An earlier gate read "preStartMem' low on 2712, StartMem' on 2680" as *"the
   storage cycle is being started"*. It is the opposite: **a start held as a
   level is a hold.** This loop restarts the storage cycle faster than it can
   finish one, because it issues a reference every four microinstructions
   forever. Real microcode issues a reference and then does other work while
   the memory completes.

   **Spacing the references was the wrong theory, and measuring it said so.**
   A second hunk of four non-references at IM[4..7], jumps running 0→…→7→0,
   made it **worse**: `StartMem'` free on 192 samples instead of 288, MemState
   still 3 of 16, and the flush stopped working entirely (`FlushStore`,
   `ForceMiss` and both MISS counts all 0, the dirty line no longer surviving
   to its Flush). `WriteInMem' & !MapTrouble` did rise from 352 to 1091, so
   spacing helps *that* pair -- but it does not touch `StartMem'`. Reverted.

   **So `StartMem'` is not driven by our reference rate**, and MemX `j22` says
   why. It is another F10016 with `TrueBD` on CE' -- so it **never counts** and
   is a parallel-load register:

   ```
   C = Clk0'Dc   CE' = TrueBD (constant 1)   PE' = MapWait
   D1 = preStartMem'  ->  H1 = StartMem'
   D0 = MemX06.sil+3  ->  H0 = preRfshInMem
   ```

   i.e. **`StartMem'` is `preStartMem'` latched on `MapWait`**, and
   `preStartMem'` is Q2 of `i14`, the MapState PROM. **The map sequencer
   drives the memory sequencer.** MapState does walk (7 of its 8 values), so
   the question is which of its states assert `preStartMem'` and for how long
   -- and that is **readable, not guessable**: the PROM images come from
   PARC's own BCPL and are gated by `prom-test`, so decode `i14`'s table
   against the MapState values actually visited.

   **`i14`'s table is decoded, the machine is read against it, and the whole
   chain now adds up.** The PROM is addressed `{MapState.2, MapState.1,
   MapState.0, MapFnc.1', MapFnc.0'}` with CE' open, and `preStartMem'` is its
   Q2. Q2 is high at exactly **three** addresses -- 3, 11, 19 -- all with both
   MapFnc bits set (no map function pending) at MapState 0, 2 or 4. So
   `StartMem'` low is the *default*, and being j16's PE' it is a **reset**.

   **Logged against the machine, all three are visited:** address 3 on 672
   cycles, 11 on 64, 19 on 64 -- ~800 where `preStartMem'` is high.
   `StartMem'` is high on only **288** of those, because j22 latches it on
   `MapWait`. And that 288 is **one contiguous window**, not fragments.

   **Inside that window the counter gets about three counts, which is exactly
   what it shows.** `Clk0'Dd` has **18 edges** there (nine rising), and CE'
   (`MemIdle`) is low on only **64 of 288**. Nine clock edges with the enable
   down four fifths of the time is ~3 counts, and MemState reaches 3 values.
   **Nothing is broken in this path** -- the window is simply too short for a
   16-state cycle.

   An earlier prediction from this same table -- that our `←Map`, re-issued
   every fourth microinstruction, was holding the sequencer -- was **wrong**.
   Running the Map once (IM[3] → IM[1]) leaves the window at exactly 288 and
   MemState at 3, while losing the map entry. Reverted.

   **And the sequencer is a closed loop, now mapped end to end.** Two things
   limit MemState, both out of MemX `g13`, an MC10104 whose `OUT` pins carry
   the AND:

   ```
   MapWait = StartMap' & MapFree      (gate b, pins 6,7 -> 3)
   MemIdle = StartMem' & MemFree      (gate d, pins 12,13 -> 15)
   ```

   `MemIdle` is the CE' on j16, the MemState counter, so **inside** the free
   window counting needs `MemFree` low -- and it is low on only **64 of 288**.
   That is the inner limiter, under the window length.

   `MemFree` comes from `j12`, a **fourth** F10016 with `TrueBD` on CE' --
   another parallel-load register -- clocked by `Clk0'Ba`, loading when
   PE' = `MemIdle` is low, with `D1 = MemX07.sil+1 -> H1 = MemFree`. And
   `MemX07.sil+1` is an **output of j13, the DRAM timing PROM**, which is
   itself addressed by MemState:

   ```
   MemState -> j13 (16K timing PROM) -> MemX07.sil+1
            -> j12 (latched on MemIdle) -> MemFree
            -> g13 gate d -> MemIdle -> j16's CE' -> MemState
   ```

   **A proper microsequencer**: the PROM output feeds back to pace its own
   counter, and `MemIdle` is both j16's count enable and j12's load enable. It
   has to bootstrap, which is why a short window with a mostly-high `MemFree`
   leaves it turning over three states.

   **Walking the tables answers it -- but only after reading the cell.**
   `cell_SG10139` is **MSB-first**: "A0 is the most significant *address* bit
   and Q0 the most significant *data* bit", grounded in DiskProms'
   `Pin1 = #200`. A first pass used LSB-first on **both** the address and the
   data, for **both** PROMs, and produced two wrong answers -- a bootstrap
   paradox on j13 that does not exist, and "preStartMem' is high at 3, 11 and
   19" on i14, also wrong. **Read the cell before reading the table.** Third
   time in this file a PARC convention has inverted a conclusion.

   **j13** (16K DRAM timing), address `{RfshInMem, MemState.0..3}`, Rfsh = 0:

   | output | behaviour |
   |---|---|
   | Q5 → `MemFree` | 0 at every **even** MemState, 1 at every odd |
   | Q0 → `x10` | 0 at MemState **4 and 8**, nowhere else |

   `MemIdle = StartMem' & MemFree`, so counting needs an even state, and since
   j12 *latches* `MemFree`, **each memory state takes two clocks**. Nine
   rising `Clk0'Dd` edges in the 288-cycle window ≈ 4 states; 3 observed. The
   model is self-consistent. **`MemWEa` needs MemState 4; the window delivers
   3.**

   **i14** (map PROM), address `{MapFnc.0', MapFnc.1', MapState.0,
   MapState.1, MapState.2}`: Q2 → `preStartMem'` is high at **exactly one
   address, 3** -- both MapFnc bits **low** (a map function *is* pending, the
   opposite of the first reading) with **MapState = 3**. A memory cycle starts
   in one precise state of the map sequencer.

   **The decode and the machine now agree exactly:** address 3 is visited
   **192** times and `preStartMem'` is high on **192**. That equality is the
   check that the convention is finally right.

   **And there is only ever one window, at sample 54** -- `StartMem'` goes
   high once, for 288 cycles, never again. So it is not "make the window
   longer" but **"why does the loop never start another memory cycle?"**

   The address log says where to look. `preStartMem'` is high on 192 cycles,
   **all before sample 213**, while `MapWait` is low on 2776 -- the latch is
   wide open and it is the **PROM input** that stops. The machine then sits at
   address 24 (672 visits) = MapState 0 with no map function pending, i.e.
   idle, plus 28 (483), 8 (448), 0 (384).

   **And the two halves stop coinciding -- neither one stops.** Address 3
   needs **both** MapFnc bits low **and** MapState = 3. Measured separately
   over 3000 samples:

   | condition | cycles | last sample |
   |---|---|---|
   | MapFnc = 00 (function pending) | 597 | 2133 |
   | MapState = 3 | 320 | 2581 |
   | **both** (address 3) | **192** | **213** |

   So the map function keeps being issued and the sequencer keeps reaching
   state 3, but after sample 213 **they never line up again**. That is a
   **phase** relationship, not a missing signal -- the classic shape of two
   things free-running at rates that no longer agree: this loop presents a
   reference every four microinstructions while the map sequencer walks at its
   own pace.

   (Full distributions -- MapFnc `{0',1'}`: 00=597, 01=832, 10=32, 11=1539.
   MapState visits all eight: 0=1536, 1=128, 2=128, 3=320, 4=547, 5=149,
   6=128, 7=64.)

   **And it is a beat frequency, measured.** Logging the onset of every run:

   ```
   MapFnc=00 onsets : 1, 406, 662, 918, 1558, 1814, 2070
                      intervals 405, 256, 256, 640, 256, 256
   MapState=3 onsets: 22, 1494, 2518
                      intervals 1472, 1024
   ```

   MapFnc=00 recurs every **256** samples, MapState=3 every ~1024-1472. The
   first MapState=3 run, at 22, falls inside the MapFnc window opened at 1 --
   **that single overlap is the only memory start in the whole run.**
   Afterwards MapState=3 lands in the gaps: 1494 between the window at 918
   (ending ~1003) and the next at 1558; 2518 past 2070's.

   **And 256 samples is four loop iterations** (4 microinstructions × 16
   sys_clk = 64 each), so the map function is accepted only on every *fourth*
   `←Map`. The map has its own accept cadence and this bench asks faster than
   it answers.

   **The loop is phase-fragile in both directions.** Adding **one**
   non-reference at IM[4] -- after the Flush, leaving Store and Flush adjacent
   -- breaks `HitColDirty` just as the earlier four-instruction insertion did.
   So the dirty line's survival is not about adjacency either; it depends on
   the phase this particular loop sits in. At four instructions the
   Store/Flush pair works and the map window is missed; at five the map phase
   moves and the pair breaks.

   **This is an artefact of an artificial loop, not an RTL fault.** Nothing is
   broken: MapState visits all eight values, the reference machinery is gated
   end to end, and the PROM tables agree with the running machine to the cycle
   (192 = 192).

   **And two experiments confirm it, with numbers -- including real Xerox
   microcode.** First, point IM[3]'s JCN at *itself* so Map/Store/Flush run
   once and the machine parks. Second -- **`+realucode`**, wired up in the
   bench -- skip the overwrite entirely and execute the four hunks of
   `AEmu.mb!2` already loaded at IM[0..15]. Xerox's own microcode at its own
   cadence, no hand-built loop:

   | | looping | one-shot | **real AEmu** |
   |---|---|---|---|
   | `StartMem'` high | 288 | 1187 | **2584** |
   | longest window | 288 | 899 | **1589** |
   | in-window `Clk0'Dd` edges | 18 | 74 | **162** |
   | `preStartMem'` high | 192 | 832 | **1205** |

   **Nine times the window and nine times the clock edges.** The beat was
   entirely an artefact of the four-microinstruction loop.

   **And it separates the two limiters for good.** Even with 162 clock edges,
   MemState *still* reaches only 3 -- because CE' (`MemIdle`) is low on just
   **85 of those 1589** samples. `MemFree` is high almost all the time **no
   matter what the cadence is**. So:

   - the **window length** was a bench artefact, now understood and gone;
   - the **inner limiter** -- `MemFree` holding CE' high -- is not, and it is
     what actually stops MemState reaching 4.

   (The AEmu snippet makes references but no write-back: `WriteInMem'` and the
   Store/Flush path read 0 throughout, and no `←Map` runs. It exercises the
   *cadence*, which is what it was used for.)

   **So the next question was `MemFree`, not the loop -- and probing it shows
   the feedback settling, which is by design.** Across a window, `MemFree` and
   `MemIdle` each change **exactly once** (one edge apiece in 288 samples,
   MemFree high on 224). They do not alternate.

   The structure says they cannot, and it is **not** a modelling error -- g13's
   cell is right and PARC's naming agrees, `MemIdle'` on the NAND pin 9 and
   `MemIdle` on the AND pin 15:

   ```
   MemFree = 0                 -> MemIdle = 0 -> j12 LOADS (MemFree follows Q5)
   MemFree = 1, StartMem' = 1  -> MemIdle = 1 -> j12 HOLDS, MemFree stuck high
   MemFree = 1, StartMem' = 0  -> MemIdle = 0 -> j12 loads: ESCAPE
   ```

   **The escape is `StartMem'` going low** -- which is also what reloads
   MemState with zero. So a storage cycle is **self-limiting**: it runs a few
   states, parks when `MemFree` latches high, and is **re-triggered** by the
   next reference. It is not meant to free-run through all sixteen states, and
   MemState reaching 3 in a window is the machine behaving, not stalling.

   **Which re-frames the `MemWEa` question one last time.** `x10` is 0 at
   MemState 4 and 8, so the write phase is **one state past** where these
   windows park. The question is no longer "why does the sequencer stop?" --
   it stops on purpose -- but **"what does a real write-back reference do
   differently that carries it one state further?"** The AEmu snippet makes
   references but never a write-back (`WriteInMem'` reads 0 throughout), so it
   cannot answer that; a microcode window that actually stores to memory can.
   The same conclusion the cadence work reached, now for a second and
   independent reason.

   **Worth generalising, and the count is now four:** i10, j22 and j12 all
   have `TrueBD` -- a hardwired constant 1 -- on CE', so all three are
   parallel-load **registers** that never count. **j16 is the only real
   counter** of the four, and its CE' is `MemIdle`. On MemX this part is a
   load register more often than a counter, so read PE' and the D inputs
   **first**, and do not reason about counting or terminal count until CE' is
   shown to go low.

**Three sampling traps in one file.** The first read an instant instead of
counting edges; the second read the end of a run instead of the interesting
moment; the third conditioned on a field that picked the wrong instruction
entirely. Count edges, and identify the running instruction by a field you
have read back out of IM.


## Where the boot stands (2026-08-22)

The five-stage chain, what is done and what blocks the rest, is the ROADMAP TO
BOOT at the top of `docs/verilog-from-sil.md`. Short version: stages 0 and 1
are done and cross-checked against the C emulator; stage 2 runs but only with
IM parity disabled; stage 3 is started -- the memory section is clocked, in a
running machine, and being asked for storage -- and stage 4 needs I/O device
backends that do not exist. 29 gates.

**The single thing blocking stage 3, stated exactly.** The microcode ASKS for
storage and nothing COMPLETES. `WantProcRef'` asserts, MAR carries a real
register value, and the reference is ACCEPTED -- `RefHold'` reads 1, so it is
not stuck at the front door. But the Pipe pointer does not move over a whole
run, and `MDhold'`/`MiscHold'` read 0: the processor is held waiting for
memory DATA. `DdataGood'` is properly connected (MemX e19 drives it) and says
the data is not good, because nothing has ever completed a storage access.

So the next thing to build is a behavioural storage array behind MemD's
interface. That is forced, not a shortcut: main storage lives on separate
**MSA** boards (144 `MK4096P-6` DRAMs each) for which the archive has only
`.lc`/`.bp`/`.nl` and no wire list, so it cannot be generated from Sil even in
principle.

**Two capacity questions, one settled.** Memory is not a risk on MiSTer:
`tools/sil_ram_groups.py` measures 246 M10K worst case against the DE10-Nano's
557, so the machine fits without merging the bit-sliced RAMs at all (merging is
a ~40% saving, not a gate). Logic is the open one, and the estimate is
arithmetic rather than a fit -- synthesise to find out.

**A full OS boot will not happen in gate-level simulation.** Simulation proves
the chain stage by stage, each gate cross-checked against the C emulator; an
FPGA is where anything boots. Do not spend effort trying to make a simulation
boot an OS.

