# Phase 2: what the Sil files give the Verilog, and how to test it

**2026-08-08.** Written while cross-checking the C emulator against the board
netlists (`docs/sil-netlist-crosscheck.md`). That pass answered "does our
model have the right shape". This one records what the same files mean for
the RTL, because the answer turned out to be much more than expected and it
should not have to be rediscovered.

**The headline: we do not have to infer the Dorado's logic from the manual or
the schematic PDFs. We have the gate-level netlist of every board, with pin
directions.**

---

# ROADMAP TO BOOT (current: 2026-08-21)

Read this first; everything below it is the history of how the generator and
cell library got built. Gate names are `make -C verilog <name>`; there are 29.

## The IFU is in a machine (2026-08-23)

`dorado_ifu` is ContA + ContB + ProcH + ProcL + MemC + MemD + MemX + **IFU**.
The memory boards are there because that is where the IFU's connections go: of
its 137 backplane nets it shares **56 with MemC**, 38 with ContA, 35 with
ProcL, 34 with MemD, 32 with ProcH, 23 with MemX and 21 with ContB. An IFU
with no memory to fetch from would be a board in a vacuum.

`make -C verilog ifu-test` gates three things:

- **IFUM is there and the right shape** -- 27 F10415A packages at 1024 x 1
  each, so **1024 x 27 bits**: the 24-bit IFUM entry of HM Table 18 plus
  parity, and 1024 = 256 opcodes x 4 instruction sets.
- **the slot clock reaches the board** -- `CLK.ifu'` fans out to `clk0'Aa` and
  friends, 1250 edges over 20,000 sys_clk. Note what is deliberately NOT
  asserted: the memory boards' gate is "local clock runs IFF `MemClkEnable'`
  says so", but the IFU's `CLKEnable'a` is a BACKPLANE INPUT with no driver in
  this configuration -- the same finding `tb_mem` already records for MemX's
  `MemClkEnable'c`. Demanding consistency would fail on a net nothing drives.
- **`IfuData[7:0]` resolves** -- read at MACHINE level, because it is a
  wired-OR bus and the board exports `IfuData_N__drv`; probing the stub inside
  the board gives the unconnected readback.

Mutation-tested: tying `CLK.ifu'` low is caught.

**Why the IFU matters more than its package count suggests.** The Dorado has
no instruction set in hardware -- "the Mesa instruction set" is a MICROPROGRAM
in IM. What turns an opcode into a microcode entry point is this board, and
`IFUJump` consumes what it produces. No emulator microcode dispatches without
it, so no world runs without it.

## IFUM decoded, and what the gate does NOT prove (2026-08-23)

The IFUM write and read paths are mapped off the wire list:

```
address   {InstrSet.0a, InstrSet.1a, J.0a..J.7a}    2 bits of instruction set
                                                    + 8 of opcode = 1024
write     RcvdBMux.NN -> pin 15, enabled by DecHi_'/DecLo_' (IFU c21)
                                        -- the two halves of `<-IFUM`
read      pin 1 of each of the 27 packages
```

and those 27 outputs **are** HM Table 18's entry fields: `TypeJumpK'`,
`TypePauseK'`, `LengthK.0'/.1'`, `RBaseSelK'`, `MemBK.0`/`MemBK34`, `NK.0/.1`,
`SignK`, `TwoAlphaK`, and `InstrAddrK.2'/.3'` -- the microcode entry address
the emulator jumps to.

`ifu-test` now also fills the array and watches those move: **all-zero gives a
decode of `00000`, all-one gives `11111`**. Mutation-tested -- breaking the
F10415A read is caught.

**AND THE GATE SAYS WHAT IT DOES NOT PROVE.** Nothing is driving the IFU to
fetch yet, so the machine holds `{InstrSet, J}` at zero: **1 distinct address
across 2000 samples**. The per-location check that follows is therefore a
consistency check against the array, NOT proof that decoding works -- a
stuck-at-zero address would pass it identically, and the gate prints the
distinct-address count and says so rather than letting the PASS imply more
than it earned.

**The next rung is what makes the address move**: the IFU prefetching against
base register 31, and `IFUJump` consuming the entry. The C emulator is the
oracle -- it can dump the expected IFUM entry and entry point for a given
opcode, the way `alu-diff` and `boot0-test` compare against `cpu.c`.

## MemState's bits are reversed -- confirmed, and NOT fixed (2026-08-23)

`MemProms.bcpl` is what cracks this. It builds j13 as **four groups of eight**
-- 'Read or write', 'Idle state', 'Refresh', 'Idle state' -- so of the five
address bits the top two `{RfshInMem, MemState.0}` pick the GROUP and the low
three `{MemState.1, MemState.2, MemState.3}` index within it. **A memory cycle
is eight steps, not sixteen**, and `MemState.0 = 1` means idle.

Decoding the read/write group MSB-first (Q0 = bit 7):

| step | byte | x10 | MemFree |
|---|---|---|---|
| 0 | 353 | 1 | 0 |
| **1** | 053 | **0** | 0 |
| **2** | 153 | **0** | 0 |
| 3 | 373 | 1 | 0 |
| 4 | 352 | 1 | 0 |
| 5 | 340 | 1 | 0 |
| 6 | 306 | 1 | **1** |
| 7 | 303 | 1 | 0 |

So `MemWEa`'s write phase is **steps 1 and 2**, and `MemFree` parks the counter
only at step 6.

**Read in PARC's order, the machine visits steps 0 and 4 -- it JUMPS.** Because
j16's F10016 wires **H0, its counter LSB, to `MemState.0`**, which the j13
address makes the **high** bit. Incrementing flips read/write to idle and back
instead of walking the sequence, so the cycle can never reach steps 1 and 2
where the write lives.

**But `cell_F10016` is NOT the bug -- thirty packages say so.** Every F10016
counter on MemX maps its H pins to PARC's field the same way:

```
a21  H0=Atask.0    H1=Atask.1    H2=Atask.2    H3=Atask.3
c22  H0=FinTask.0  ...            h12  H0=STState.0 ...
f19  H0=Asrn.0     ...            k05  H0=FaultSrn.0 ...
j16  H0=MemState.0 H1=MemState.1 H2=MemState.2 H3=MemState.3
```

H0 ↔ `.0`, uniformly. H0 is the counter's LSB by the data sheet and by
EclDict's own `D0,11 > H0,14` pairing, so **for these counter fields PARC
numbers LSB-first** -- unlike microinstruction fields such as `RSTK.0` or
`ASEL.0`, which are MSB-first. **Both conventions live in this machine**, and
which applies depends on whether the thing is a *field of a word* or the *bits
of a counter*. That is worth knowing well beyond this bug.

So the reversal is in how j13's **address** is assembled: its pins are
A0=`RfshInMem`, A1=`MemState.0` … A4=`MemState.3`, and `cell_SG10139` is
MSB-first, which puts the counter's LSB at the address's high end -- the index
is the counter value **bit-reversed**, and that is why counting jumps through
the table instead of walking it.

**What is not yet settled** is which of the two is wrong: the SG10139 address
convention (from that cell's own header, grounded in DiskProms' `Pin1 = #200`)
or the j13 pin reading. **Do not guess** -- `cell_F10016` is in 226 packages
and `cell_SG10139` in 15, and gates lean on both, including the terminal-count
fix that once stopped the machine converging. Settle it where the answer is
known independently, as the IM reversal was.


## A decode that agreed with the symptom, and was wrong (2026-08-23)

Hand-decoding j13, the 16K DRAM timing PROM, said its Q5 (`MemFree`)
alternates with MemState parity and its Q0 (`x10`, the third term of
`MemWEa`) is 0 at states 4 and 8. **Measured per state in the running
machine, it is neither:**

```
state 0:  MemFree = 0   x10 = 1
state 1:  MemFree = 0   x10 = 1
state 2:  MemFree = 1   x10 = 1
```

The alternating story predicted a walk of 0 -> 1 -> 2 and a park -- and three
states is exactly what the machine shows, so it looked confirmed. It was not:
`MemFree` is 0,0,1, not 0,1,0. **A decode that merely agrees with the symptom
is not evidence.** The fix is to read the signal per state rather than derive
it, which `memrun-test` does now.

What the measurement does establish, and it is worth having:

- the machine walks **0 -> 1 -> 2 and parks there**, because `MemFree` goes
  high at state 2 and `MemIdle` is `StartMem' & MemFree`, so CE' rises and
  j16 stops counting;
- **`x10` is HIGH in every state reached**, so `MemWEa`'s write phase is
  somewhere the cycle never gets to;
- `RfshInMem` is 0 across the whole window, so the half of j13's table being
  read is the one that was assumed -- that part was right.

The open question is therefore sharper: **what does a real write-back
reference do that carries MemState past 2?** And it is worth remembering that
with the 4K timing PROM enabled instead (`ChipsAre256/16K` = 0) `x10` DOES go
low on 96 cycles -- so the two tables disagree about where the write phase
sits, and which one a Rev Ch board should be reading is worth re-checking
against `MemProms.bcpl` before assuming.

## The IFU comes out of hold -- and is blocked on the memory (2026-08-23)

`ifufetch-test` runs the IFU with REAL microcode: it is `tb_memrun` reused
wholesale for its machinery -- the startup that walks `AEmu.mb!2` into IM
through the control-processor bus and executes it -- pointed at `dorado_ifu`,
with tb_memrun's own Flush assertions relaxed to displays because they are not
this bench's subject.

**`IfuHold` is the handshake**: the IFU asks with `WantIfuHold'`, MemC grants
with `IfuHold` (MemC d23.4). Static, with no microcode, it reads **1 -- the
IFU held**. With real microcode executing it is **released on 3000 of 3000
samples**. That is the rung, and tying it off is caught.

**AND IT STILL DOES NOT FETCH, which reorders the work.** The loop issues an
`IFetch<-` (IM[3]: ASEL = 001, ff01 = 2, FF = 0o200 -- the encoding
`refdecode-test` pinned against `cpu.c`'s `DM_REF_IFETCH`), and over the run
`J` takes **one** distinct value and so does `IfuData`. The IFU is enabled and
idle.

It cannot fetch because **memory cannot serve the reference yet** -- the same
run reports `MemWEa` 0 and the storage cycle not completing, which is exactly
where the memory work stopped. So **the IFU's fetch rung is not an independent
task: it is downstream of the memory section serving a real reference.** Do
that first; this gate starts moving on its own when it lands, and the count of
distinct `J` values is what to watch.

(Also worth noting: `CLK.ifu'` is its own slot clock and has to be driven. That
trap -- an unclocked board looking perfectly gated -- has now been hit on
three separate boards.)

## The clock generator is a PHASE COUNTER now (2026-08-23)

`cell_CLOCKGEN` replaces seven BaseBoard packages -- h06 (the MPQ3303 VCO),
g05/h05 (MC1660 shaping) and g03/g04/h03/h04 (MC1690 dividers) -- with a
counter on `sys_clk` that emits `StartClockPulse'` and `EndClockPulse`
directly. It is a deliberate extension of the substitution already made at the
VCO: everything between the oscillator and those two nets exists ONLY to make
clock phases, and an FPGA can make them directly.

**The waveforms were measured, not invented.** `tb_baseboard` captured the real
chain over two periods at 16x:

```
SCP'  1111 0000 0000 1111      high 0-3, LOW 4-11, high 12-15
ECP   1100 0000 0011 1111      high 0-1, LOW 2-9,  high 10-15
```

Both 50% duty on a 16-cycle period with `EndClockPulse` leading by 2, and the
substitute reproduces exactly that -- same period, same duty, same 2-cycle
offset. (Its absolute phase sits one cycle later, which is only where the
counter happened to start.) `SYSPER` is threaded from the machine down to the
board, so the ratio is a parameter rather than a cell edit.

**IT DID NOT MOVE THE RATIO FLOOR, and that is the useful result.** The sweep
is identical before and after:

| ratio | gates failing |
|---|---|
| 16x | none |
| 8x | `taskrun-test`, `memrun-test` |
| 4x | + `step-test` |
| 2x | + `exec-test` |

So the cascade of oversampled dividers was **not** what set the floor. Tracing
the rest of the chain shows why: after the counter, `CLK.xx'` comes off
MC10210 buffers and each board's local `clk0'`/`clk1'`/`clk2'` off SE10210
buffers -- all COMBINATIONAL. Every clock net already changes only on a
`sys_clk` edge.

**What actually sets the floor is the number of DISTINCT CLOCK PHASES the
Dorado uses within one microinstruction.** `clk0'` takes about one transition
per microinstruction and `clk2'` about two, and the machine also uses `clk1'`
and `PreClock1'` -- the "nops are not padding" finding turns on exactly that
ordering, controls latching on `Clock1'` while the datapath latches on the
earlier `PreClock1'`. Resolving N distinct phase points needs at least N
`sys_clk`, and in practice more for the setup and hold between them. Four to
eight phases is what the measurements imply, and 8x working while 4x is
marginal and 2x is not fits that exactly.

**So 2x needs a different transform, not a better clock generator.** The
phases would have to stop being SIGNALS that cells detect edges on, and become
a decoded phase index each cell uses as its enable directly -- master clock
plus enables in the full sense. That is a change to how every clocked cell is
generated, not to one block, and it is the honest next step if period speed
matters.

**What the substitution did buy**: one less cascade stage, seven analog and
divider packages off the critical path, a clock tree that is provably
synchronous to `sys_clk`, and the ratio exposed as a parameter. 33 gates green.

## THE CLOCK, AND WHY IT WAS SLOW (2026-08-23)

**The PLL was asking for 7.04 MHz.** `rtl/pll` came across from
RCAStudioII_Mister with `output_clock_frequency0("7.040229 MHz")` -- the
CDP1802's clock, not ours. Two things follow, and the second is the useful one:

- the Dorado was running at 7.04 / 16 = **0.44 MHz of microinstructions, 0.026x
  the real machine** -- not the 0.11x quoted below, which was computed from the
  ACHIEVED Fmax rather than the CONFIGURED clock;
- and Fmax of 30.27 MHz was achieved **under no pressure at all**.
  `derive_pll_clocks` takes its target from the PLL's own parameters, so
  Quartus met 7.04 MHz trivially and stopped optimising.

**Asking for more gets more.** Retargeting the PLL to 50 MHz took Fmax from
**30.27 to 46.84 MHz** with no other change -- same RTL, same device. Setup
slack at 50 MHz is -1.349 ns, so 50 does not close; 45 MHz is the number that
does, with margin.

| PLL target | Fmax achieved | ALMs | closes? |
|---|---|---|---|
| 7.04 MHz (inherited) | 30.27 MHz | 31,601 (75%) | yes, trivially |
| 50 MHz | 46.84 MHz | 33,110 (79%) | no, -1.349 ns |
| 45 MHz | -- | -- | expected |

**What that means for real-time.** The Dorado's microinstruction rate is
`sys_clk / ratio`, and the real machine is 16.67 MHz:

| ratio | at 45 MHz | vs real |
|---|---|---|
| 8x | 5.6 MHz | 0.34x |
| 4x | 11.3 MHz | 0.67x |
| 2x | 22.5 MHz | **1.35x** |

So **2x at 45 MHz would exceed period speed**, and 4x gets two thirds of the
way. The ratio is worth pushing for that reason, not only for simulation.

**THE FLOOR ON THE RATIO IS SET BY THE FASTEST DERIVED CLOCK NET, not by the
microinstruction rate.** Every one of them is recovered by oversampling, so
sys_clk has to beat the quickest by Nyquist and then some. Measured over
40,000 fabric cycles at 16x:

```
clk0'  2493 edges     ~1 transition per microinstruction
clk2'  4987 edges     ~2 transitions per microinstruction   <- fastest
```

`clk2'` completes a FULL PERIOD per microinstruction, so at 2x it is sampled
exactly at Nyquist -- no margin for its gated and derived variants. **4x is
the realistic floor**, and the empirical sweep agrees: 8x nearly works, 4x
breaks `step-test`, 2x breaks more.

| ratio | gates failing |
|---|---|
| 16x | none |
| 8x | `taskrun-test`, `memrun-test` |
| 4x | + `step-test` |
| 2x | + `exec-test` |

Both benches now carry a `SYSPER` parameter and express every fixed wait as a
multiple of it, so a wait means the same amount of DORADO time at any ratio --
necessary, and not sufficient: they still fail at 8x, on per-task TPC/Link
state and on the dirty-line check. Those look like sampling margin rather than
arithmetic, which is what the clk2' figure predicts.

## IT FITS, AND IT RUNS AT 0.11x -- the MiSTer core (2026-08-23)

`verilog/` is a normal MiSTer core now: `Dorado.qsf` (device settings and 211
pin assignments carried over verbatim from RCAStudioII_Mister, a working core
on the same board), `Dorado.sv` (the `emu` wrapper, with `CONF_STR` and
`hps_io` intact), `files.qip`, `sys/`, `rtl/pll`. Build with
`tools/quartus-build.sh {map|all|clean}`.

**THE TOP LEVEL IS `sys_top`, NOT `emu`.** sys_top holds the physical pin
constraints and instantiates emu; making emu the top turns its whole port list
into package pins -- 319 output pads against the device's 315 -- and the
fitter rejects it for a reason that has nothing to do with the design.

**HPS is wired from the start** -- `ps2_key`, `ps2_mouse`, the
`sd_lba`/`img_mounted` disk interface and `ioctl`. Nothing consumes them yet,
but the Dorado needs all three (Trident packs, the Alto 61-key matrix, the
mouse the guest reads from UTILIN), and having the bus present makes that a
wiring job rather than a rebuild.

**THE REAL FIT, with placement:**

| resource | usage | DE10-Nano | |
|---|---|---|---|
| ALMs | 31,601 | 41,910 | **75%** |
| Registers | 55,181 | | |
| Block memory | 897,477 bits | 5,662,720 | 16% |
| RAM blocks | 163 | 553 | 29% |
| Pins | 145 | 314 | 46% |

**Two false readings on the way, both worth not repeating.** A synthesis
*estimate* of 26,964 ALMs (64%) is not a fit, and the fitter rejected that
same design outright. And a fit of 1,024 ALMs (2%) meant the machine had been
**optimised away**: with `probe_sel` tied to a constant and one probe bit
reaching an LED, Quartus swept the design and fitted an empty shell.
`probe_sel` comes from the OSD status word now and every `probe_val` bit
reaches a pin.

**AND THE CLOCK RATIO IS THE HEADLINE PROBLEM.** `clk_sys` closes at **30.27
MHz**. The cells recover each distributed ECL clock net by oversampling it, at
16 sys_clk per microinstruction, so:

| ratio | microinstruction rate | vs the real Dorado |
|---|---|---|
| 16x | 1.89 MHz | **0.11x** |
| 8x | 3.78 MHz | 0.23x |
| 4x | 7.57 MHz | 0.45x |
| 2x | 15.1 MHz | 0.91x |
| 1x | 30.3 MHz | 1.8x |

So the ratio sets **how fast the FPGA Dorado runs**, not just how fast
Verilator does -- and 16x is nine times slower than the machine it models.
Real time needs `sys_clk >= 16.67 MHz x ratio`, which even 2x misses at the
current Fmax.

**Measured, the ratio can already come down to 8x**, with two benches left to
fix:

| ratio | gates failing |
|---|---|
| 16x | none |
| 8x | `taskrun-test`, `memrun-test` |
| 4x | + `step-test` |

`exec-test` failed at 8x on `clk1' 2493` against `clk0' 2492` -- one apart out
of ~2492, which is the fixed-length sample window closing between the two
edges of one microinstruction. That is the property holding, not failing, and
the assertion now says so. The remaining two are likely the same shape:
benches with hard-coded fabric-cycle counts. **Note the discipline: fix the
bench's statement of the property, do not loosen the property.**

(A methodological trap that cost two wrong sweeps: **zsh does not word-split
unquoted variables**, so `for t in $GATES` passes the whole string as one
target and every gate "fails". Use an explicit list.)

## Synthesis: the first real Quartus run (2026-08-23)

Quartus is not installed natively; it runs in `raetro/quartus:mister`
(Quartus Prime 17.0.2 Lite, linux/amd64 under emulation). One script drives
it:

```
tools/quartus-build.sh map     # analysis & synthesis -- the numbers
tools/quartus-build.sh all     # + fit + sta
tools/quartus-build.sh clean
```

**Never run `quartus_sh --flow compile`.** Quartus' parallel helpers crash
under emulation on Apple Silicon -- they appear as `<defunct>` beside
`[crashreporter]` -- and the parent then deadlocks forever on their named
pipes at ~4% CPU, which looks like a slow build and never ends. The script
passes `--parallel=1` to each stage. **A healthy build sits at ~100% CPU; at
~4% it is hung, not slow.** Quartus also REWRITES `verilog/quartus/dorado.qsf`
every run, so sources live in `files.qip` and the qsf wants
`git checkout --` afterwards. (All of this is owed to the working setup in
`RCAStudioII_Mister`, which had solved it already.)

Three findings from the first run, in the order they surfaced:

**1. Two power rails were multiply driven, and Quartus rejects that outright.**
`VCC62` and `GND346` on BaseBd each had both a rail constant and a wired-OR
driver tree. The drivers are pins Sil marks `out` on ANALOG transistor quads
sitting on a supply -- MPQ6002 c05 and MPQ3303 h06 -- which are not logic
drivers at all. Verilator tolerated it; Quartus stopped with *"Can't resolve
multiple constant drivers for net VCC62"*. The generator now suppresses the
tree on a rail. Two nets in the whole machine, and only synthesis found them.

**2. The two big DRAM cells were twelve times too wide.** `MosRam` and
`MK4096P-6` declared `reg [11:0] mem [0:4095]` while storing a single bit --
functionally right, since the write zero-extends and the read takes bit 0, and
309 packages of it. Now `reg mem [0:4095]`.

**3. FIXED, AND THE MACHINE FITS. The blocker was an ASYNCHRONOUS READ.**

`F10415A` (IM), `F10470` (the DRAM) and `i2125` all ended with

```verilog
assign p1 = (!p14) ? mem[a] : 1'b0;     // ASYNCHRONOUS read
```

and **M10K is synchronous-read only**. An async-read array falls back to MLAB
for small ones -- which is why the 16x4 `F10145A` and 256x4 `MB7071H` infer
cleanly, and why 3 of 174 `F10415A` did -- and to REGISTERS otherwise.

**How the original differed, and why this is safe.** The real parts read
asynchronously: put an address on the pins and the data appears after a
propagation delay -- 10-25 ns for a 10K-series ECL RAM, comfortably inside the
Dorado's 60 ns microinstruction, with no clock involved in the read at all.
An FPGA cannot do that at this density. But **the latency a registered read
adds is one `sys_clk`, not one microinstruction**: this design already runs 16
sys_clk to a microinstruction -- the oversampling introduced so the
distributed ECL clocks could become enables -- so it costs about **3.75 ns of
Dorado time**, well inside the budget the real part had. It is the same
transform, applied to the read instead of the strobes. All 32 gates pass
unchanged, which is what says the rest of the machine tolerates it.

**`i2125` was the whole story.** ONE of its packages took **1,445 ALUTs**
built out of logic, and its 32 packages -- all on BaseBd -- were essentially
the entire BaseBoard's 47,775, which was 73% of the machine's combinational
logic. For a 1024x1 static RAM on a support board.

| | before | after | DE10-Nano |
|---|---|---|---|
| **ALMs** | 50,012 (119%) | **26,964 (64%)** | 41,910 |
| Combinational ALUTs | 65,220 | 21,280 | |
| Dedicated registers | 70,228 | 37,459 | |
| Block memory bits | 487,664 | 520,432 (9.1%) | 5,702,400 |

**The eleven-board Dorado fits on a DE10-Nano at 64% of its ALMs and 9% of its
block RAM**, with Analysis & Synthesis clean at 0 errors. That is the first
real answer to the capacity question, and it leaves room for the IFU work, the
I/O controllers and the MSA, none of which are in this configuration yet.

Two things tried first and ruled out by measurement rather than argument:
splitting the write and the conditional read into separate `always` blocks,
and rewriting `MosRam`/`MK4096P-6` to the canonical MiSTer `dpram.sv`
template. Neither helped, because neither touched the read's *asynchrony*.
Worth knowing that template is pure INFERENCE, not an `altsyncram`
instantiation, which is why these cells still simulate in Verilator unchanged.

Also fixed on the way: `MosRam` and `MK4096P-6` declared `reg [11:0] mem
[0:4095]` while storing a single bit -- twelve times the memory, 309 packages.


## What "boot" means here

The real machine's chain has five stages, and the RTL is partway through the
second.

| stage | what happens | status |
|---|---|---|
| 0 | BaseBoard 6502 powers up, sets manifolds, takes the CP bus | **done** -- `baseboard-test`, `muffler-test`, `firmware-probe` |
| 1 | It jams microinstructions and walks **Boot0** into IM via the MIR | **done** -- `boot0-test` walks real `AEmu.mb` hunks in; IM matches the C emulator |
| 2 | Boot0 **runs inside the Dorado** and takes Boot1 over CPReg | **partial** -- the machine executes from IM (`exec-test`), but only with IM parity DISABLED |
| 3 | **Initial** sizes storage, builds the Map, loads a world | **started** -- the memory section runs and is being asked for storage; no access completed yet |
| 4 | The world runs: disk or ethernet, then display | not started -- the I/O boards exist, nothing is on the other end |

## What is solid

- **100% cell coverage**: 3771 of 3771 logic packages across the eleven-board
  machine, in 125 part types. No skeletons left.
- **The machine assembles and clocks**: eleven boards elaborate; `machine-test`
  shows the clock reaching all 24 clock nets.
- **The datapath computes**, against an independent oracle: all 24 ALU
  functions agree with `cpu.c` over 10,752 vectors (`alu-diff`), plus T, Q,
  ALUFM and RM (`compute-test`).
- **The whole write path is proven**: CP bus, all 36 MIR bits, jams,
  single-stepping, Write-IM, and Boot0's block loader -- each diffed against
  the C emulator, which shares no code with the RTL.
- **Board identity is right**: the configuration straps set each board's
  slow-I/O address and task number (`strap-test`), two of them confirmed by the
  C emulator independently.
- **Tasking works**, which as of this morning was the biggest untested risk:
  wakeup routing, priority encode, the BNT register, the switch, and per-task
  PC and Link (`task-test`, `taskrun-test`).

## What is left, in dependency order

### 1. IM parity -- the immediate blocker, and narrow

`exec-test` has to turn IM parity off to run at all, and PARC's boot leaves it
ON (`DisableDoradoErrors = ParityEnables+030`, "all except IM parity errors
disabled"). Everything around it has been measured and cleared: the single-step
chain is sound, the write path stores even parity correctly, and the checker
computes exactly `~(XOR(17 data) ^ IMRH)`. What is left is the **CPReg-to-B
path**, which the always-on parity error has been propping up, plus a SetSS
polarity blind spot that `taskrun-test`'s harness can now reach. Written to be
read cold: the header of `verilog/verilator/tb_parity.sv`, including one
confident conclusion that PARC's own source later refuted.

### 2. Tasking -- essentially DONE (2026-08-21)

Five of its six layers are gated:

| layer | gate |
|---|---|
| wakeup routing | was connected to NOTHING -- see `BACKPLANE_WAKEUP_JUMPERS` |
| priority encoder | `task-test`, 23 patterns vs `cpu.c`'s `task_bnt()` |
| BNT register | `taskrun-test`, in a machine executing from IM |
| the switch (CTask) | `taskrun-test`, all 15, controlled against `TaskingOff` |
| per-task PC and Link | `taskrun-test`, separate storage proven |
| T and MemBase | remaining -- they live with the memory work below |

The wakeup finding is the one worth knowing: every I/O board puts its request
on the same two connector pins under its own local name, and the backplane
routes each SLOT to a different `TWReq` line by JUMPER ("for desired Task wake
up"). A board's task number is therefore a property of its slot, which is why
each board carries a task-number strap -- and the strap is the authority for
which line to wire.

### 3. The memory subsystem -- STARTED, and further than expected

Where it stands:

- **The front door.** A reference enters through ASEL. MemC b24 makes
  `WantProcRef' = IgnoreProc | ASEL.0`, so a reference is exactly ASEL <= 3 --
  the C emulator's rule (`memory.h`: "ASEL = 0..3 with FF[0:1] decoding the
  kind"), independently derived. Gate: `refdecode-test`.
- **The boards run and are being asked.** `memrun-test` puts all seven boards
  (ContA, ContB, ProcH, ProcL, MemC, MemD, MemX) on tb_exec's startup, with
  MemC clocked in step with the processor -- and the running microcode presents
  ASEL=0 with `WantProcRef'` asserted.
- **Two reference kinds match `cpu.c`.** `LFetch<-` at (ASEL 0, ff01 2) and
  `IFetch<-` at (ASEL 1, ff01 2), each asserting in its own cell of sixteen.
- **MAR is mapped.** A four-way mux (T / R / Q / Ain) that shares its source
  select with the ALU's A input -- so the reference address IS the A operand.
  Its enables are registered, i.e. set up by the previous instruction.
  `compute-test` gates BOTH halves: no reference means no leg enabled and MAR
  all ones, and a reference built with `mi()` enables the A leg (BSEL >= 4)
  with **MAR carrying the selected register exactly** -- 16 cases, MAR = R =
  c000 against T = f333.

The next three steps, in order:

1. **The rest of the kind table.** `Store<-` is not a raw decoder output and
   the IO kinds are qualified by whether the current task is an I/O task --
   which `cpu.c` conditions on identically -- so they need the machine running
   in such a task. Tasking now makes that reachable.
2. **DONE (2026-08-22): the memory section runs DRAM cycles** -- RAS and CAS
   strobe, MapState steps, no holds, from PARC's own startup. These are REFRESH
   cycles, so a PROCESSOR reference reaching the Pipe is the next step, then
   the storage array. Superseded note: the microcode asks (`WantProcRef'` asserts)
   and nothing completes -- the Pipe pointer does not move over a whole run,
   and `PRhold` -- the memory-to-processor hold -- is CLEAR before the machine
   starts and comes UP during the run, with none of the three hold requests
   ever set. So it is asserted by something the machine does while executing,
   which for a reference that never completes is what one would expect. It is the MD hold and the MISC hold that are up, not the reference hold
   (`RefHold'`=1, `MDhold'`=0, `MiscHold'`=0) -- the processor held waiting for
   memory DATA, which is right when a reference is accepted and never
   completes. So the remaining work is **the storage side**: MemD's DRAM path
   with its RAS/CAS and refresh, and the Map on MemX. The C emulator is a ready-made oracle, as it was for the ALU and IM.

### 4. IFU

The board generates and elaborates; there is no gate. Needed for emulator
microcode (opcode dispatch), so it blocks stage 4 rather than stage 3. Note it
also drives `RefOutstanding'`, which the seven-board memory machine therefore
leaves undriven.

### 5. I/O device backends -- routinely underestimated

The RTL models the CONTROLLER boards (DskEth, DispY, DispM). A boot needs
something on the OTHER END: a Trident pack, an Ethernet peer, a monitor. Those
have to be written as bus-functional models for simulation, or as fabric
peripherals on an FPGA. The C emulator implements all three, so the behaviour
is specified, but the plumbing is new work.

## What is actually in the archive

`chm/sil/<Board>-Rev-Xx.dm!N_/`, sixteen boards:

| file | what it gives Phase 2 |
|---|---|
| `<Board>-Rev-Xx.wl` | **the wire list — the whole board, net by net** |
| `<Board>-Rev-Xx.lc` | package → part number (the cell library to build) |
| `<Board>-Rev-Xx-C.nl`, `-E.nl` | backplane interface = **the module's port list** |
| `<Board>NN.sil` | the drawing sheets, Sil's binary format (need Sil/ANALYZE) |
| `<Board>-Rev-Xx.bp` | **the same port list, bare pins** (`ALUCarry: E179`) |
| `<Board>-Rev-Xx.ad` | addendum: every sheet's revision and `MARKED BUILT` flag |
| `Build.cm`, `Print<Board>.cm` | PARC's own build scripts |

### The wire list is the design

`ProcH-Rev-Ce.wl` is 118 KB and 3,379 lines. After a header naming every
`.sil` sheet with its revision date and a `MARKED BUILT` flag, it lists every
net:

```
ALUCarry: <367> (381)
    f49.5i {205,370}      f17.15o {217,364}     g17.12i {268,364}     g18.10i {260,388}
    E179 {457,525}
```

Read that as: net `ALUCarry` connects pin 5 of package `f49` **as an input**,
pin 15 of `f17` **as an output**, pins on `g17` and `g18` as inputs, and
backplane connector **E pin 179**. `{x,y}` is the physical position on the
board.

So each net comes with:

- its **name**, and the names are the machine's own (`ALUCarry`, `aluCout`,
  `ALUF.0`, `aluC0`) — the same vocabulary the C emulator and the Hardware
  Manual use;
- every **package and pin** it touches;
- the **direction of each pin** (`o` = driver, `i` = receiver), which is what
  makes the graph directed and therefore synthesisable;
- backplane pins where the net leaves the board.

Paired with `.lc`, which maps every package to a part number
(`f17` → `MC10176/16/E`), that is a **complete structural netlist**. Writing
the RTL becomes: model ~50 ECL part types, then emit one module instance per
package and one wire per net.

### The cell library is small and completely documented

Across all sixteen boards: **5,563 packages, 127 part types.** Strip
terminators (`Term100`, 1,641 of them), spare sockets and passives:

- **3,771 packages, 125 types**
- of which **7 types / 745 packages are memory or PROM** — these become
  inferred RAMs, not gate models
- leaving **3,026 packages of pure logic in 118 types**, and **50 types cover
  90% of them**

The parts are the Motorola MECL 10K family and its Fairchild equivalents —
`MC10176` (hex D flip-flop, 307 packages), `MC10231` (dual master-slave D),
`SE10210`, `MC10158`, `MC10173`, `MC10102`, `MU10164`, `MC10181` (the 4-bit
ALU) — all 1970s parts with published truth tables. The memories:

| part | packages | what |
|---|---|---|
| `F10415A` | 174 | 1K × 1 ECL RAM |
| `MosRam` | 165 | main storage array |
| `MK4096P-6` | 144 | 4K × 1 MOS DRAM |
| `F10145A` | 105 | 16 × 4 ECL register file |
| `F10470` | 72 | 4K × 1 ECL RAM |
| `MB7071H` | 70 | PROM (sequence PROMs, decode tables) |
| `MCM10149` | 15 | 256 × 4 ECL RAM |

Fifty hand-written cell models is a bounded, mechanical job. It is far less
work — and far less guessing — than deriving the same logic from Figure 17
and thirty sheets of scanned schematic.

### The netlists are the module boundaries, already drawn

The `.nl` files are each board's backplane interface, and a board is exactly
the right size for a Verilog module. The port list is already written --
**three times**, in three different files:

| where | form |
|---|---|
| `<Board>.bp` | `ALUCarry: E179` |
| `<Board>.wl` | a bare `E179` among the net's pins (see the example above) |
| `-C.nl` / `-E.nl` | slot-qualified: `StartCycle'a: #s05-C.5` |

Across all sixteen boards they agree on **2,052 of 2,054 pins**; `.bp` and
`.nl` agree exactly, and the five differences against the `.wl` are ground
nets it numbers individually (`GND-26`) where `.bp` collapses them (`GND`).
Three independent statements of the same interface is about as much
confirmation as an archive can give. Measured with `tools/sil_backplane.py`:

| module | backplane pins | notes |
|---|---|---|
| ProcH / ProcL | 175 / 176 | datapath, split high byte / low byte |
| ContA / ContB | 170 / 130 | sequencer, tasking, parity collection |
| IFU | 137 | |
| MemC / MemD / MemX | 168 / 174 / 184 | cache control / data / storage |
| DispY / DispM | 122 / 117 | monochrome / colour |
| DskEth | 119 | both I/O controllers |
| BaseBd | 125 | mostly its own ACP bus; 45 reach the machine |

(An earlier version of this table gave 68 for ProcH. That is what
`sil_netlist_report.py` calls *signals* — bus groups after collapsing bit
runs, so `BMux [0-7,16]` counts once. A port list needs the pins.)

`tools/sil_netlist_report.py` prints them grouped into buses with bit runs,
which is close to a port declaration already.

**The port lists come from `.bp` (done 2026-08-16).** They used to be inferred
from the wire list, by treating a net whose only consumers were `Term100` pins
as leaving the board — wrong in both directions, and wrong at the root, since
`Term100` is a 100-ohm *terminating resistor* network, not a connector. That
inference missed 703 backplane nets (emitted as internal wires, so they could
never have reached another board) and invented 833 ports. All sixteen boards
now emit exactly the ports PARC states, and `tools/sil_backplane.py --ports`
is the gate:

```
TOTAL        1922     1920   1920        2         0
          (stated) (emitted) (agree) (missing) (spurious)
```

The two are DskEth's `GND`/`GND-`, which the wire list numbers individually
(`GND-26`), so there is no net of that name to make a port of.

Direction comes from the wire list, and a third case appears that a
two-direction port list cannot express: **a net the board both drives and
senses is `inout`**. 512 of the 2,052 ports are. Declaring those `output`
would let a board read back only its own contribution to a bus instead of the
bus — see the wired-OR note below.

## What this changes about the plan

`CLAUDE.md` says Phase 2 should map from the C emulator's structure. That is
still true for the *testbench* and for anything behavioural, but for the
datapath itself there is now a better source. Concretely:

1. **Do not hand-write the datapath from the manual.** The manual is the
   reference for *intent*; the wire list is the design. Where they disagree,
   the wire list is what was built — the `.wl` header even records which
   sheets were `MARKED BUILT` at that revision.
2. **A netlist-to-Verilog generator is the high-leverage tool**, not a
   nice-to-have. Input: `.wl` + `.lc` + a cell library. Output: one module
   per board with the `.nl` port list, one instance per package, one wire per
   net. That tool plus 50 cell models is most of the structural RTL.
3. **Revisions are tracked.** `chm/sil/` holds one revision per board, but
   `DoradoDocs/doradodrawings/` has ~50 PDFs covering every revision of every
   board. Tim's point about the Sil files tracking bug fixes matters here:
   pick the revision that matches the microcode we run.

## How to test it — this is the part we are unusually well set up for

The C emulator is not just a reference implementation; it is a **differential
test oracle that already boots five operating systems.** That is a stronger
verification position than most RTL projects start from.

### Tier 1 — cell library, in isolation

Truth tables from the MECL data sheets. Cheap, mechanical, and it is where
sign/polarity errors get caught. Note that ECL is frequently active-low and
the netlist names say so (`Overflow'`, `Cnt=Zero'`, `BSEL'`); a cell model
that quietly normalises polarity will produce a machine that almost works,
which is the worst outcome.

### Tier 2 — per-board, against the C emulator's section boundaries

The C emulator is already split along the same lines the boards are:
`cpu.c` ↔ ProcH/ProcL/ContA/ContB, `memory.c` ↔ MemC/MemD/MemX, `display.c`
↔ DispY/DispM, `disk.c` + `ethernet.c` ↔ DskEth. So each board module can be
driven with the same stimulus as its C counterpart and compared.

The one caveat found during the cross-check: **the C model is not
cycle-accurate everywhere it is functionally correct.** Hold is the clear
case — the hardware has seven independent requesters and we assert Hold from
one source, with the engine's response behind `DORADO_HOLD`. Comparing
per-cycle timing there will diverge legitimately. Compare *architectural
state at instruction boundaries* first, and treat timing as its own tier.

### Tier 3 — run the real microcode, diff against the C emulator

`make test` already runs 11 test binaries against ~25,000 microinstructions
of genuine Xerox microcode. `tools/tracepcdiff` exists. The escalation is:

1. `ftest.MB` — MicroD's own worked example
2. the six PARC hardware diagnostics that `build/rundiag` already passes
   (`docs/running-diagnostics.md`) — these were written to test *the
   hardware*, which is precisely what the RTL is
3. Bootstrap → Initial → LoadRam, the full boot chain
4. a real world: Alto/Galaxian is the deterministic one

The six diagnostics deserve emphasis. `eventCounters`, `TestTW` and the rest
were written by the people who debugged the actual boards, they exercise the
event counters, HOLDSIM/TASKSIM and the tasking hardware, and they already
pass against the C emulator. They are the natural first real workload for the
RTL and they came with the machine.

### Tier 4 — the frame buffer, byte for byte

The project's existing discipline already applies: gate on **byte-identical
framebuffers** (`cmp` two `.pgm` files) across Galaxian, the Cedar desktop
and Lyric — never on pixel counts. An RTL Dorado that produces the same
121,553-pixel Galaxian frame as the C emulator, byte for byte, is correct in
a way no unit test demonstrates.

### Tier 5 — the DMux, if we want the period debugger

`DMuxClk`/`DMuxData` reach ten of the eleven boards in a working machine (all
but DispM). That serial scan chain is how **Midas**, PARC's own debugger,
read machine state. Implementing it in RTL would let Midas — an independent,
period-correct checker nobody on this project wrote — inspect the design.
The C emulator answers exactly two DMux addresses today, so this would need
building on both sides; worth it only if the RTL reaches the stage where an
independent oracle is the bottleneck.

## What is still missing

- **No `.sil` sheet renderer.** The sheets are Sil's binary format. `ANALYZE`
  and Sil survive in `_cd6_/sil` with BCPL source (see
  `docs/parc-feedback-todo.md` F.2) and `DoradoDocs/doradodrawings/` has PDF
  renderings, but nothing here reads `.sil` directly. **This is less
  important than it looks** — the `.wl` is derived *from* the sheets and is
  plain text, so the sheets are for human reading, not for extraction.
- **No timing data.** The netlist gives connectivity, not propagation delay.
  The 60 ns microcycle and the t₀..t₃ phase structure come from the Hardware
  Manual.
- **The five remaining boards** — BaseBd, msa, PCMSA, IOTest, Music — are
  uncrosschecked. BaseBd is the interesting one: we already emulate its 6502
  from the real EPROM dumps in `firmware/`, so its netlist is a direct check
  on `baseboard.c`.
- **The gaps the cross-check found are RTL work items, not emulator bugs**:
  bus and IM parity (`IOPE`/`MdPE`/`RamPE`/`IMLHPE'`, all collected on ContB
  into `Error'`), the seven-requester Hold network, `CountMiss`, DRAM
  refresh, ECC generate/check. The RTL will have them whether we model them
  in C or not, because they are wired.

## Suggested order

1. `.wl` parser + `.lc` reader → an in-memory netlist graph. Small, and it
   makes everything after it mechanical.
2. Cell models for the top ~20 parts, verified against data-sheet truth
   tables.
3. Generate ProcH + ProcL, simulate against `cpu.c`'s ALU/register-file
   behaviour on the microcode we already run.
4. ContA/ContB next — the sequencer and tasking, checked with `TestTW` and
   `eventCounters`, which already pass in C.
5. Memory section, then IFU, then I/O.

Start with the processor boards because they are the ones whose behaviour the
C emulator models most confidently, and because `docs/sil-netlist-crosscheck.md`
already confirmed every field width, register width and branch condition on
them matches.

---

# Step 1 is DONE (2026-08-15): the netlist reader and generator

The `.wl` + `.lc` reader this document called "self-contained, testable on
its own, and makes everything after it mechanical" exists and runs. All
sixteen boards parse, generate Verilog, and **elaborate under Verilator**.

## The tools

| tool | what it does |
|---|---|
| `tools/sil_netlist.py` | reads `.wl` (nets, pins, directions) + `.lc` (parts). `--all chm/sil` summarises every board; `--net NAME` prints one net |
| `tools/sil_ecldict.py` | reads PARC's **own** part dictionary, `EclDict.Analyze` / `TtlDict.Analyze` -- pin numbers, gate grouping, and for complex parts the DATASHEET signal names |
| `tools/sil_emit_cells.py` | emits a Verilog cell skeleton per part type, ports generated from the dictionary + observed directions |
| `tools/sil_to_verilog.py` | emits one module per board, instantiating a cell per package and wiring it with the board's own net names |
| `tools/sil_gen_all.py` | all sixteen boards in one go |

`verilog/Makefile`: `make boards`, `make cells`, `make lint`.

## Measured, and it corroborates this document's own sizing

```
16 boards   12,841 nets   5,563 packages   52,865 pin references
            127 part types;  48 logic types cover 90% of logic packages
            67,960 lines of generated Verilog, 16/16 lint clean
```

The 5,563 packages and 127 part types match the figures derived
independently earlier in this document, which is a useful check that the
reader is seeing the whole design and not a subset.

## The find that removed the guesswork

**PARC's own part dictionary survives**, at
`chm/sil/msa-Rev-Bg.dm!1_/ecldict.analyze` (K. Pier, 12-Sep-1978), with
`ttldict.analyze` beside it -- the ECL file's own header points at the TTL
one ("REMOVE first semicolon of this line if you have TTL logic"). It is
what ANALYZE, Xerox's design-rule checker, used to know what each package
IS, so the cell library's pinouts do not have to be reconstructed:

```
MC10181:  2=H3 3=H2 4=Gg 5=COUT 6=H0 7=H1 8=Pg 9=E0 10=D0 11=E1
          13=F0 14=F3 15=F1 16=D1 17=F2 18=D2 19=E2 20=E3 21=D3 22=CIN 23=M
```

-- the 4-bit ALU with its datasheet pin names. Merging both dictionaries
took the parts with no definition from 57 down to 17, and those remaining
are memories and analog (`MosRam`, `LM3911+20K`, `CA3140`), not logic.

Division of authority, which matters: **pin NAMES come from the dictionary,
pin DIRECTIONS from the wire lists.** The `.wl` marks every pin `i` or `o`
per instance, which is both authoritative and finer-grained than a part-level
rule -- a pin can drive on one board and be sensed on another.

## Three things the generator refuses to do silently

Each would produce plausible-looking, wrong RTL:

- **Wired-OR.** MECL 10K open emitters are legitimately tied together (91
  such nets on ProcH alone). Verilog forbids multiple continuous drivers, so
  those nets are emitted as an explicit OR of their drivers and every one is
  reported, rather than being quietly reduced to one driver.
- **Missing cell models.** A package whose part has no model becomes a named
  stub with its real ports, and is counted. Logic is never dropped.
- **Undriven nets.** A net with no driver on this board is a board INPUT
  arriving over the backplane; it becomes a module port, not a floating wire.

## A level-sensitive part is not a latch in this design

Every clocked element here runs on the fabric clock `sys_clk` with its own
control as an ENABLE, because the Dorado clocks 1,201 packages from
distributed ECL clock nets and an FPGA cannot route 1,201 gated clocks. That
convention has a second consequence that took a while to surface: **it applies
to LEVEL-sensitive parts too, not only edge-triggered ones.**

`F10145A`, `F10415A`, `F10470` and `i2125` are memories whose write is
level-sensitive; `MC10173` and `SN74LS259` are latches. Written the obvious way
-- `always @*`, with a Verilator LATCH waiver -- they pass a level straight
through, so a read-modify-write path becomes a COMBINATIONAL LOOP, which is
most of a datapath. The machine had 1,333 such back edges and would not settle
once anything started moving. On `sys_clk` with the level as an enable there
are none, and a latch is not synthesisable on most FPGAs anyway.

**Do not read an `[FF ...]` entry as "this output is registered".** It is a
TIMING ARC from the clock, and a gate after the register is folded into it
rather than given its own `[G]`. `S169` shows it: RC' gets a second [FF] block
with a clock-to-output delay of 30.8 ns against 16.5 ns for the Q outputs, and
the extra 14 ns is the carry gate. `F10016`'s carry was briefly registered on
that misreading; a synchronous counter's carry has to be combinational, or a
cascaded stage counts a clock late.

`make -C verilog loop-check` finds any loop that survives, in a fifth of a
second, from the cell files -- so a part written as a latch is caught where it
is written rather than as a non-convergence on another board.

## Two bugs the elaboration caught, both worth knowing

- **Net-name collisions were silently merging signals.** `CTask.0` and
  `CTask=0` are DIFFERENT nets on ContA, and the first name mapping sent both
  to `CTask_0`. Verilog reported a duplicate declaration -- but only by luck:
  had one of them not been declared, the board would have elaborated with two
  signals shorted and no diagnostic at all. The mapping is injective now
  (`_` escaped first so no escape sequence is reachable another way) and
  verified collision-free across all sixteen boards.
- **Cell port directions are global, net classification is per-board.** A
  part pin that drives on MemC and is only sensed on IFU is `output` in the
  shared cell, so IFU would declare that net a module input and then connect
  an output to it (`%Error-ASSIGNIN`). The classifier reads the cell library
  back and treats a net touched by any cell output as internal.

## What is NOT done

**No cell has behaviour yet.** All 125 are skeletons with correct ports and a
`TODO` body, so the boards elaborate but do not compute. Filling them in is
the next work, and the order is given by usage: 48 logic types cover 90% of
logic packages. Each model should cite its part function when it lands.

The harness for running them is `verilog/verilator/` -- Verilator + Dear
ImGui, carried over from the Apple-IIgs MiSTer `vsim` framework with its
`sim/` support library intact (sim_video, sim_bus, sim_input, sim_clock,
sim_console, sim_audio, sim_blkdevice). The top module is MiSTer's `emu`,
deliberately: RTL that runs in the harness runs on MiSTer without a second
port.

---

# The PROMs survive too, as the PROGRAM that generates them

2026-08-15. The 70 `MB7071H` packages were the largest unmodelled part, and
the one that needs *data* rather than a model. That data exists.

`<DoradoSource>DoradoProms.dm!14_` (mirrored into `chm/doradoproms/`) holds
`DoradoProms.bcpl` plus one file per board section -- `ProcProms`,
`MemProms`, `IFUProms`, `DispProms`, `DiskProms`, `EtherProms` -- and a
`.help`. From the help file:

> "DoradoProms is a program for defining, blowing, and listing any and all
> proms used the the DORADO. ... DoradoProms.run is run to produce a
> microbinary format file (called DoradoProms.mb) which contains the actual
> PROM definitions"

and it emits a `Prom.cm` carrying, per PROM, "the 'name' of the prom, and a
label containg board name and location". So we get **contents and placement**,
and in a better form than binaries would be: the source says what each PROM
*means*.

**26 PROMs**, extracted from the sources rather than retyped
(`tools/dorado_proms.py --list`):

| PROM | size | where |
|---|---|---|
| LMASK, RMASK | 32 x 16 | PrH-b07/b08, PrL-b07/b08 |
| ST, EC-1, EC-2, Map-Mem, Map-Map, 4k-Mem, 16k-Mem | 32 x 8/16 | MemX |
| Data-Select, **Mouse-Motion**, **Keyboard-Map** | 32-256 | IFU-a06/i03/k05/l05 |
| DisPromA, DisPromB, LFProm-Low, AltoProm | 256 x 4 | Display |
| DiskWrite, DiskRead, DiskTag, DiskFifo, DiskUnits | 32-256 | Disk |
| EtherFifo x2, EtherPD, EtherRcvr, EtherXmtr | 256 x 4/12 | Ethernet |

Note **Mouse-Motion** and **Keyboard-Map** on the IFU board -- the same
terminal path the A6 serialiser work went through. Those PROMs are the
authority on what the keyboard map actually is, against which
`display.c`'s 61-key matrix can be checked a fourth time.

## The generators port cleanly, and one is already cross-checked

They are small closed-form loops, so porting is transcription, not
reimplementation. `ProcProms.bcpl MakeLMask`:

```
Zero(buff,32); let Lbit = #100000; let Lmask = 0
for address = 1 to 31 do        //address 0 should be zero
    [ Lmask = Lmask % Lbit; Lbit = Lbit rshift 1; buff!address = Lmask ]
```

which yields `0000 8000 C000 E000 F000 ...` -- the shifter's LEFT edge mask.
The C emulator computes the same masks independently, from the Hardware
Manual (`shifter_output()`, ShiftLMask/ShiftRMask). `dorado_proms.py --check`
compares them and they agree, including the edge case the BCPL comment calls
out ("address 0 should be zero").

That is the cross-check worth repeating for each PROM as it is ported: the
PROM is what the hardware HOLDS, the emulator is what we THINK it holds, and
they were written 45 years apart from different sources.

One detail not to mistake for a bug: entries 16..31 saturate at `FFFF`,
because it is a 32-entry part holding a 16-bit mask.


---

# Session close, 2026-08-15

| piece | state |
|---|---|
| Boards generated + elaborating | **16 / 16** (67,960 lines, plus 4,599 of cells) |
| Cell models with behaviour | **97**, covering **97.7%** of the eleven-board machine's logic packages. Of the 64 left, 42 are analog or mixed-signal |
| 6502 / 6532 | netlist-derived 6502 (Holme, via jotego); MiSTer 7800 RIOT (CC BY-NC, noted) |
| PROMs generated from PARC's BCPL | **26 / 26**, all property-checked |
| Harness | Verilator + Dear ImGui, builds, runs, `--headless` gate |
| Backplane | **stated per board in `.bp`/`.nl`** -- no schematic needed, and no inference either |

**Nothing computes as a machine yet.** Boards elaborate, most cells have
behaviour, but no board is instantiated in `sim.v` and nothing is wired
between boards. That is the next step and it is unblocked.

Continue from **`docs/verilog-handoff.md`**, which is written to be read
cold: Task A is the two remaining Ethernet PROMs (self-contained), Task B is
wiring the machine together and testing it against the C emulator in four
steps that can land separately.

## What made this go quickly, worth repeating

Three of PARC's own artifacts did the work that would otherwise have been
reconstruction:

1. **The `.wl` wire lists** give every net, every pin, and the DIRECTION of
   each pin -- so the RTL is placed and wired from the design input, not from
   a reading of the schematics.
2. **`EclDict.Analyze` / `TtlDict.Analyze`** give pin numbers and, for the
   complex parts, the DATASHEET SIGNAL NAMES. MC10181 came out as the 4-bit
   ALU with `D0-D3 E0-E3 F0-F3 CIN COUT M Gg Pg` without a datasheet in hand.
3. **`DoradoProms.bcpl`** is a PROGRAM that computes the PROM contents, so
   the PROMs arrive documented -- `EtherPD` is a Manchester phase decoder
   with its timer thresholds commented, and the disk PROM is a 32-step
   sequencer with every step explained.

And the discipline that caught the errors: **check transcriptions
structurally, not by eye.** Every PROM generator has a property test, and the
size check alone found two real errors in one batch -- including a bug in the
tool's own `Header()` parser that was silently dropping a PROM from the map.

### RESOLVED, and it retracts a milestone (2026-08-23)

`cell_F10016` **was** reversed, and the previous note here -- that PARC uses
LSB-first for counter fields -- is **wrong and withdrawn**. PARC is MSB-first
consistently; what misled me is that EclDict names this part's pins MSB-first
too, so `H0` reads like bit 0 when it is the *most* significant.

The Fairchild connection diagram (`DoradoDocs/datasheets/F10016.pdf` p.1, DIP
top view) gives the silicon:

```
pin 3 = Q0 (LSB)   pin 2 = Q1   pin 15 = Q2   pin 14 = Q3 (MSB)
pin 7 = P0 (LSB)   pin 9 = P1   pin 10 = P2   pin 11 = P3
```

EclDict names those `H0,14 · H1,15 · H2,2 · H3,3` and `D0,11 · D1,10 · D2,9 ·
D3,7` -- **exact reversals of both lists**. A cell implements the data sheet's
function per PIN NUMBER (the rule the MC10141 established), so pin 3 carries
the LSB.

Two further confirmations, neither appealing to convention:

- **`RfshAd` is cascaded across MemX g05/g06/g07 and the carry direction is
  visible**: `g07.CO' -> g06.CE'`, so g07 is the LOW-order package. It carries
  `RfshAd.5-.8`, which puts the whole counter's LSB on an `H3`.
- **`MemProms.bcpl` builds j13 as four groups of eight** split on
  `{RfshInMem, MemState.0}` -- refresh-vs-read and active-vs-idle -- which is
  only coherent if `MemState.0` is the counter's high bit.

**And fixing it retracted "the memory section runs DRAM cycles."** That gate
passed *because* the bits were reversed: MemState entered j13 permuted, the
sequencer read the wrong entries of PARC's own timing table, and the RAS/CAS
strobes that came out were not a memory cycle. With the counter corrected they
stop. What is actually true is that **MemState never counts at all** -- its
enable `CE' = MemIdle` is low on 0 of 3000 samples -- so the sequencer is not
started, which is exactly open task #17. The same chain clocks the MSA's write
register, so `storage-test`'s write-clock assertion was the same artifact.

Both gates now print a loud `OPEN (task #17)` line instead of asserting a cycle
that does not run.

### And then it ran, for the right reason (2026-08-23)

The retraction above stands -- the strobes really were an artifact -- but the
cause was a SECOND bug, and fixing it starts the sequencer properly.

**PARC's PROM blower does not store a table entry verbatim.**
`DoradoProms.bcpl Body()` left-justifies it into a 16-bit word and keeps the
top `width` bits:

```bcpl
mask = 0
for i = 1 to MemWidth do mask = (mask rshift 1) % #100000
data = Buff!address
data = data lshift adjust    // left justify it
data = data & mask           // mask off the unused bits
```

so the byte blown is `(value << adjust) >> (16 - width)`. **Every `Header()`
in the sources passes `adjust = 16 - width`, which makes that the identity --
except one:**

```bcpl
Header("Map-Mem", 8, buff, 32, 10)     // ten, where eight is standard
```

MemX-i14's bytes are PARC's table values **shifted left by two**, and we were
transcribing them raw. That put every output on the wrong pin. `preStartMem'`
is i14 pin 3 = bit 5; raw, it reads asserted at exactly one address, so
`StartMem'` -- which loads from it -- sat **low on 2787 of 3000 samples**,
holding the `MemState` counter in parallel load forever. The memory could
never sequence.

Shifted, the same bit asserts at `MapState=3` for **Refresh, Read and Write**
and never for a Map write -- a reference starts a storage cycle, a pure map
write does not. `tools/dorado_proms.py _table()` implements PARC's arithmetic
now and takes `adjust` from the `Header()` call, so this is fixed by
construction rather than by patching one table; **only `Map-Mem`'s image
changed**, which is the check that the formula is right.

The chain, all off the wire list: `MemIdle = StartMem' AND MemFree` (g13 gate
d, pins 12/13 -> 15, and PARC names both senses so the polarity is not
inferred); j16 takes `PE' = StartMem'` and `CE' = MemIdle`; `StartMem'` is
j22 pin 15, loaded from `preStartMem'`; `preStartMem'` is i14 pin 3.

**`memrun-test` asserts the DRAM cycle again** -- RAS 2, CAS 2 edges. Still
open (task #17): the WRITE-BACK is not scheduled in the bench's window, so
`WriteInMem'` never asserts. Those assertions are demoted to `OPEN` lines
rather than deleted; they passed only while the sequencer was jammed, so
restoring them as gates would pull the jam back.

### The write-back is a STIMULUS problem, not a hardware one (2026-08-23)

With the sequencer running, the remaining half of task #17 is localised to a
single flip-flop stage, and the chain above it is proven correct.

The path, entirely off the wire lists:

```
WriteInA' = NOR(VictimInA, IoStoreInA)              MemX k23 gate a
VictimInA = NOR(VicInPair', EcHasA)                 MemX k23 gate d
VicInPair' latches, on LdPair', MemC j23 gate 1 -- an MC10117 OR-AND whose
  pins 5 and 9 are OPEN, so it reduces to:
     VicInPair' = ~[ (DirtyVicOrAB | ForceDirtyMiss) & WantVic ]
WriteInA' -[StartMapClk0'a]-> WriteInMap' -[StartMemClk0']-> WriteInMem'
  (two MC10176 hex D flip-flops, MemX h14 and j11)
```

Measured over the 704-sample reference window:

| term | count |
|---|---|
| `WantVic` | 192 |
| `DirtyVicOrAB` | 512 |
| `VicInPair'` asserted | 128 |
| `VictimInA` | 128 |
| **`WriteInA'` asserted** | **128** |
| `WriteInMap'` asserted | **0** |

**The victim is real and reaches `WriteInA'`.** What fails is the next hop:
`StartMapClk0'a` has **four edges** in the entire window -- it ticks only when
a map cycle starts -- and on none of them was `WriteInA'` asserted, so h14
never latches. The remaining work is to make the dirty victim exist AT a
StartMap edge, which is about the microcode the bench runs, not about the
boards.

**And this cost a repeat of a mistake already on the books.** The first
measurement counted LEVELS: "`StartMapClk0'a` high on 680 of 704" reads like a
clock that is running constantly, and it is four edges. COUNT EDGES, and count
the COINCIDENCE with the D input -- a clock edge and an asserted D are the
only thing that can move a flip-flop.

### A word comes out of storage (2026-08-23)

`make -C verilog readback-test`. Eight boards -- ContA, ContB, ProcH, ProcL,
MemC, MemD, MemX and the MSA -- with real `AEmu.mb!2` microcode walked into IM
and executing, the memory section sequencing a DRAM cycle of its own accord,
and a **known word coming back out of the MK4096 array onto the backplane**.

The path is BIT-SERIAL, and every hop is off the wire lists:

```
8 x MK4096 DOUT -> SN74166 (parallel-in, serial-out) -> MC10124 (TTL->ECL)
                -> c01 (MC10176) -> Sin.00-15 -> MemD c02 (clk2'Aa) -> SinD
```

a13's eight stages and the DRAM on each: `A b05, B c05, C d05, D e05, E e06,
F d06, G c06, H b06`.

**How the word is seeded, and why it needs no address.** Every one of the 4096
cells in each of those eight DRAMs is preloaded with that DRAM's own bit of a
fixed pattern, so the array answers the SAME word at every address. Whatever
the running microcode chooses to reference, a load of the '166 must produce
exactly it -- which sidesteps steering the machine to a particular address,
the genuinely hard part.

**Mutation-tested, and the first pattern had a blind spot.** Four mutations:
reversing the '166 load order, driving QH from the A stage, seeding one DRAM
wrong, and zeroing a DRAM. Three were caught immediately; **QH-from-A sailed
through**, because the first pattern was `8'b10101101`, whose bit 0 and bit 7
are both 1. The pattern is `8'b10101100` now -- A=0, H=1 -- and all four are
caught. A pattern has to distinguish every failure it claims to catch.

**Scope, stated honestly:** this is gated against PARC's own netlist and the
seeded data, NOT against the C emulator. `memory.c` is a functional model with
no bit-serial storage path, so there is nothing at this level to diff it
against; the C cross-check that does exist for the memory front door is
`refdecode-test`, against `cpu.c`'s reference-kind rule.

### The return path reaches the processor (2026-08-23)

The other half of the reference. `readback-test` now also carries a word from
the CACHE to `Md`, the register microcode actually reads.

The path, every hop off the wire lists:

```
SinD -> b03/k22/f16 (MU10164 muxes) -> D0in/D1in
     -> 72 x F10470 = the cache data memory, 36 of them driving D.00-17
        two per bit (the columns wire-OR)
     -> h05 (MC10197 hex AND, common strobe MD_D) -> dMD.00-17     [MemD]
     -> i01 (MC10175) -> Md.00-17          [ProcH bits 0-7,16; ProcL 8-15,17]
```

Two structural facts worth keeping:

- **`Dbuf` is NOT the cache read data.** MemD b01 takes `BMux` IN and gives
  `Dbuf` OUT, and b02 muxes `Dbuf`/`Pipe4` back ONTO `BMux` under
  `DriveBMux' = NOR(_Pipe4, _Dbuf)`. That is the diagnostic re-drive path for
  `B<-Dbuf` / `B<-Pipe4`, not the memory data return. Following `BMux` looked
  promising and was a dead end.
- **`MD_D` selects the MD source**, and the two enables are complementary:
  h05 (the cache data) is enabled when it is high, h04 -- an F10145A, the
  small MD register memory addressed by `MDMad.0-3'` -- when it is low.

**How it is gated.** The cache is seeded the same way the DRAM is: every one
of the 4096 cells of all 36 D-driving F10470s gets that bit's value, so the
cache answers the same word at every address and the return path can be gated
without first landing a fill at a known address. With `MD_D` asserted, the
16-bit data word matched on **176 of 176** samples at `dMD` and **160 of 176**
at `Md` -- the 16 difference is i01's register latency, since `Md` lags `dMD`
by a clock. Bits 16-17 are parity, which the board generates rather than
passes through, so they are counted separately.

**Why this one matters beyond the memory section:** `Md` is what microcode
reads (`T<-Md`, `B<-Md`) and what `cpu.c` models directly, so it is the first
memory result the two models express in the same terms.

**Diagnosis note.** Before the cache was seeded, `D.00` was high on 165
samples and `MD_D` on 176, and `dMD.00` was dead. Both inputs live and the
output dead means one thing: COUNT THE COINCIDENCE. It was zero -- h05 is an
AND, and the cache simply held 0 at the addresses `MD_D` strobed.

Mutation-tested: inverting one bit of the cache seed, forcing the MC10197
outputs low, and making the MC10197 ignore its common strobe are all caught.

### The write-back is a PHASE, and the number says so (2026-08-23)

The remaining half of task #17, now quantified rather than guessed at.

`StartMapClk0'a` is made by MemX **i18**, an SE10210 OR:
`StartMapClk0'a = preClk0'Dc | StartMap'`. So it only ticks while a MAP CYCLE
IS STARTING, which is why there are so few edges.

Over a 40,000-sample run (`+define+RUNLEN=40000`):

| term | count |
|---|---|
| `WantVic` | 2496 |
| `DirtyVicOrAB` | 7456 |
| `VicInPair'` asserted | 2432 |
| `WriteInA'` asserted | 2432 of 9952 (24%) |
| `StartMapClk0'a` edges | 40 |
| **edges with `WriteInA'` asserted** | **0** |

**That is not chance** -- at 24% duty over 40 edges, missing every one is
about 1e-5. Measuring the phase says why: at 39 edges the victim had last been
asserted **100 samples earlier on average, closest 96**, against a loop period
of ~250. The victim is made and RETIRED well before the map stage clocks it.
The bench's four-instruction loop (`<-Map`, `Store`, `Flush`, non-reference)
issues another reference immediately after the Flush, and `VicInPair'` is a
k21 latch that the reload clears.

So the fix is to keep the victim alive across the map start -- quiet slots
after the Flush rather than another reference.

**Two things that are NOT the answer**, both tried:

- **Running longer.** Ten times the samples gives ten times the edges and
  still zero coincidence; that is what turned "probably chance" into "phase".
- **Disabling holds.** `DisHold` reads like "holds off" and is nothing of the
  kind here -- it is load-bearing for the sequencer,
  `WantMapWait' = (MapFnc.1' & MapFnc.0') | DisHold`, and without it `MapWait`
  never falls and `MapState` never steps at all.

And do not read the level counters as a substitute for edges:
"`StartMapClk0'a` high on 9640 of 9952" is **40 edges**.

### ...and tuning the loop is the wrong lever (2026-08-23)

The obvious fix for the phase above -- more quiet slots after the Flush -- was
tried at EIGHT instructions (Flush at IM[2], five quiet slots, Local Jumps
`0->7->0`; the jumps work, TNIA visits all eight). The victim survives longer
and `MemRASa` goes 2 -> 4, and then **`FSinPair'` never falls**: the Flush
stops being latched into the pair at all.

That reproduces, at eight, exactly what this bench's header already recorded
at five: *"at four instructions the Store/Flush pair works and the map window
is missed; at five the map phase moves and the pair breaks."* The loop is
phase-fragile in BOTH directions, and the map accepts a function only every
FOURTH `<-Map` -- the bench asks faster than the map answers.

**So the conclusion is not "tune the loop harder."** Nothing in the RTL is
broken: MapState visits all eight of its values, MapFnc all of its, the
reference machinery is gated end to end, and the read direction is proven all
the way to `Md`. The write-back needs a REAL workload whose store and eviction
fall where the map cadence puts them (`+realucode` is the hook), not more
surgery on an artificial four-instruction loop.

**Process note, twice in one session.** Both failed hypotheses here -- that
`DisHold` was letting the processor outrun the memory, and that quiet slots
would hold the victim -- are refuted by comments already in the bench. Read
the header before designing the experiment; it is long because it is a
lab notebook.

### The IFU's byte stream is traced and wired (2026-08-23)

The IFU's instruction bytes come off the CACHE, every hop on MemD:

```
cache D.00-03,16,17 -> e06 (F register, clocked by Fclk'a)
                    -> f22 (G register, loaded from F on GLd')
                    -> f23 (MC10174 dual 4:1 selecting G.00 / G.08 / F.00 /
                       F.08 under PcFG.15 and GDv', enable EnableFG')
                    -> FG.0-8 -> the IFU
```

**`IfuData` is not the way in.** It is generated ON the IFU by f05 from
`AlphaX` and goes OUT to the processor; chasing it as the opcode input cost a
detour. The IFU's multi-bit board INPUTS are `FG` (9 bits) and `GenIn` (16),
and it is `FG` that MemD drives.

With the cache seeded (`tb_ifufetch` now does this, same trick as
`readback-test`):

| measurement | value |
|---|---|
| `D.00` high | 253,038 -- the cache IS producing the seeded data |
| `EnableFG'` high | 0 -- the FG mux is enabled throughout |
| `Fclk'a` RISING edges | 3, **of which with `D.00` high: 0** |
| `F.00` / `G.00` / `FG.0` edges | 0 / 0 / 0 |
| **Pipe pointer moved** | **0 times** (against 9 in `tb_memrun`) |

That last row is the real one. The data path is wired and live all the way to
F's D inputs; what is missing is that **this bench records no memory reference
at all**, so F's clock barely ticks and never while data is present. The
blocker is now "tb_ifufetch's loop does not put a reference into the Pipe",
which is a much smaller question than "the IFU does not fetch".

Third time in one session that the answer was **count the coincidence**, not
the levels.

### ...and the IFU BOARD is what stops the references (2026-08-23)

Isolated by making `tb_ifufetch`'s loop **byte-identical** to `tb_memrun`'s --
the same four instructions, `<-Map` / `Store` / `Flush` / quiet slot:

| | `tb_memrun` | `tb_ifufetch`, identical loop |
|---|---|---|
| `MapFnc.0'` / `MapFnc.1'` | 0 / 0 | 0 / 0 (now matches) |
| Pipe pointer moved | **9** | **0** |
| `MemRASa` | 2 | 0 |

Same microcode, same seven boards. **The only difference is that
`dorado_ifu` adds the IFU**, and with it in the machine the processor's
references stop landing in the Pipe altogether. `IfuHold` is released on 3000
of 3000 samples, so the IFU is free-running and prefetching; the obvious
suspicion is that it is taking the memory section's attention -- but that is a
suspicion, not a measurement. The next step is to find what the IFU asserts
that stops a processor reference being RECORDED.

Two dead ends on the way, worth not repeating:

- Moving the `IFetch<-` from IM[3] to IM[2] and restoring the quiet slot does
  not help; `MemRASa` goes to 0 and the Pipe still does not move.
- Four references back to back (IFetch in IM[3], no quiet slot) is worse:
  `MapFnc` never leaves 1/1, so no map function is requested at all. The quiet
  slot is load-bearing here exactly as it is in `tb_memrun`.

The bench's loop is left identical to `tb_memrun`'s on purpose, because that
is the clean demonstration. Restoring the `IFetch<-` is one FF field:
IM[2] `0o100` -> `0o200`.

### RETRACTED: there is no clock difference -- it is `UseAsrn` (2026-08-23)

The two entries that stood here claimed MemC's `clk0'A` ran "379x too fast"
with the IFU present. **That was a measurement artefact and is withdrawn.**
With both benches counting free-running over the same 272,747 sys_clk, every
hop of the clock chain is identical:

```
CLK.mc' 34093 = 34093 | MemClkEnable'a 41 = 41 | CLKEnable'b 262953 = 262953
ppclk2'a 34089 = 34089 | preSH'x 17045 = 17045 | preClk0'B 17045 = 17045
k02's clk0'A 17045 = 17045
```

**What actually differs is the mux select:**

| | `tb_memrun` | `tb_ifufetch` |
|---|---|---|
| **`UseAsrn` high** | **270,089 (99%)** | **41 (0.015%)** |
| `Asrn` changed | 8 | 9 |
| `PEsrn` changed | 0 (stuck `0000`) | 0 (stuck `0000`) |
| `dPipe02Ad` changed | 14 | 2 |
| Pipe pointer moved | 11 | 0 |

MemX h23 selects `Asrn` or `PEsrn` on `UseAsrn`. With the IFU in the machine
the mux sits on `PEsrn`, which never leaves `0000`, so the pointer is
**pinned** -- nothing about the clock or the reference machinery is wrong.

**And `UseAsrn` is driven by MemC f24, an MC10121 4-wide OR-AND whose `e`
input group is pins 4,5,6 = `IfuAckIfHit'`, (open), `Hit'a`.** So an IFU
signal gates which source the Pipe pointer takes -- which is exactly why the
difference appears when the IFU board is added and nowhere else.

**Whether this is a bug at all is open.** Without the IFU, `IfuAckIfHit'` is
undriven and reads 0 in the OR tree, so `tb_memrun` is the ARTIFICIAL
configuration and `tb_ifufetch` may be showing real behaviour. Read HM
section 5 on the Pipe and the A/B slots before calling it broken.

**Methodology, and it cost two wrong commits.** The "379x" came from counters
in DIFFERENT CONTEXTS -- free-running in one bench, inside the window-gated
sampling loop in the other -- so they ran over 272,747 and 704 sys_clk and
were never comparable. Worse, the first correction moved only PART of the
probe block and left `n_k02` behind, so the retraction appeared to CONFIRM the
artefact. Any cross-bench comparison of a raw count needs its denominator
printed beside it, and when you move a probe, move all of it.


### RESOLVED: not a bug -- the IFU makes the Pipe source select CORRECT (2026-08-23)

`IfuAckIfHit'` holding `UseAsrn` low is **right**, and the no-IFU
configuration is the broken one. The two SRN sources are named in the netlist
and match the C emulator's model exactly, from an independent derivation:

```
PEsrn <- MemX h10, an MC10158 2:1 selecting ProcSrn or Ec1Srn on EcWantsA
Asrn  <- MemX f19, an F10016 COUNTER -- the auto-advancing I/O ring

include/memory.h:  proc_srn "4-bit; emulator + fault tasks. Default 0."
                   asrn     "4-bit; I/O ring. 2..15. Default 2."
```

And `docs/memory-architecture.md` states the rule, derived from the microcode
and the manual with no reference to this RTL: **ASRN is for IOFetch, IOStore
and PreFetch-with-miss; everything else uses ProcSRN.**

The bench loop is `<-Map` / `Store` / `Flush` -- none is an I/O reference or a
prefetch -- so all must use ProcSRN, i.e. `UseAsrn` LOW. `tb_ifufetch`
measures 0.015%; `tb_memrun` measures 99%. **`tb_ifufetch` is correct.**

The Pipe pointer follows: ProcSRN is a FIXED slot, so successive references
all overwrite it and **the pointer must not move**. `tb_ifufetch`'s 0 is
right; `tb_memrun`'s 11 is the anomaly, and it happens because without the IFU
board `IfuAckIfHit'` is undriven and reads 0 in the OR tree, freeing
`UseAsrn` to sit high.

**So the IFU does not break the memory section. Adding it DRIVES an input that
was floating.** Three commits chased this as an IFU fault; it was the absence
of the IFU all along, and an undriven input reading 0 is the same trap the
`sip_drives` and VBB fixes already record.

Now gated (`ifufetch-test`): `UseAsrn` must be low and the Pipe pointer must
hold, cross-checked against `memory.h`'s documented rule. Mutation-tested --
inverting f24's MC10121 output reproduces `tb_memrun`'s exact 99%, and making
k02 count instead of parallel-load is caught. (The first attempt at the second
mutation silently did not apply, and its "pass" was meaningless: **check that
a mutation actually changed the file.**)

### IM parity: the MC10170 polarity is settled, three ways (2026-08-23)

`cell_MC10170`'s `p15` is written `~(par9 ^ p13 ^ p14)` and should be
`par9 ^ p13 ^ p14`. Three independent confirmations, none owing anything to
the others:

1. **The data sheet's logic diagram** draws B as a plain three-input XOR --
   no bubble. The inversion its truth table shows comes from the nominal
   control levels the same diagram labels, which the Dorado does not use.
2. **PARC's thirteen IRTable microinstructions** must read `PE' = 1`, because
   `InitManifolds` leaves IM parity errors ENABLED. With the fix all thirteen
   do; with the inversion none do.
3. **`cpu.c`'s overflow logic**, new here. ProcH d13 is an MC10170 wired as a
   signed-overflow detector -- its inputs are the two operand sign bits, the
   result sign, `aluCout`, and `aluF0` (= S3: it lands on pin 13 of both
   MC10181s). `tb_alu_vs_c` now runs it against `alu_op`'s own `ovf`:

   | ALUFM entry | p15 inverted (today) | p15 not inverted (the fix) |
   |---|---|---|
   | 14 | 0 of 256 | **256 of 256** |
   | 22 | 0 of 256 | **256 of 256** |
   | 54 | 0 of 256 | **256 of 256** |

   768 vectors, unanimous both ways. Nothing else in the table splits cleanly
   because `alu_op()` uses a DIFFERENT overflow formula per operation (HM
   section 3.7) while d13 computes one fixed function -- 14/22/54 are the
   entries where the C's formula IS the hardware's.

`dorado/tests/alu_vectors.c` already computed `ovf` and threw it away; it
emits it now, as a sixth column.

**What remains is not the polarity but five benches** -- `datapath-test`,
`operand-test`, `step-test`, `sendmir`, `compute-test` -- which break when the
cell is corrected. On this evidence they are calibrated against the BUG,
exactly as `msa-test` turned out to be for the SN74166 stage order. Re-derive
them one at a time against the corrected cell. Do not weaken them, and do not
revert the cell to make them green.

### Three leads chased, all three already answered in the benches (2026-08-24)

Recorded as a warning, not a result. Trying to fix the five benches that break
under the corrected `cell_MC10170`, I chased three causes and every one was
already documented in the bench sources I was editing:

1. **The `DoControl` tail.** `DoDoradoMicroInst` ends with THREE DoControls
   (the first two byte-identical). Adding them, spaced and unspaced, changes
   nothing and one variant free-runs `step-test`. Reverted.
2. **A missing `SetMidasStopMIRClk`.** All thirteen benches already write
   `manifold(12'h1E0)`. (The value does derive correctly from
   `doradomufman.masm`: `MidasStopMIRClk = 7^6 = 0x1C0`, `+0x20`; and the
   derivation self-checks against `DisableDoradoErrors = 0x030`.)
3. **`StopMIRClkEn` reading 0.** Deliberate. `tb_exec`'s startup -- reused by
   the others -- turns the hold ON to load IM and then OFF again before
   free-running: `manifold(12'h1E0)` … `manifold(12'h1C0); manifold(12'h000)`.
   `tb_memrun`'s header also already states `StopMIRClk =
   NOR(parity-error term, StopMIRClkEn')`.

Each was read off a single measurement without checking the surrounding
startup. **Read `tb_exec`'s and `tb_memrun`'s startup in full before
continuing this** — what holds a jam, and when each bench turns that hold on
and off, is written down there.

What is genuinely known: the polarity is settled three independent ways, and
correcting it breaks five benches whose jams currently survive only because
PARC's IRTable entries FAIL our parity check. Nothing about why those five
need re-deriving has been established beyond that.

### PARC's second DoControl is NOT the fix (2026-08-23, negative result)

`doradocpint.masm`'s `BasicStopDorado` is **two** DoControls and our `jam_step`
had only the first:

```
BasicStopDorado:
  LDAI SetRun ; SEC ; JSR DoControl   keep SetRun, assert SetSS
  LDAI 0      ; SEC ; JSR DoControl   CLEAR SetRun, keep SetSS, no ClrStop
```

The second is added now in all ten benches that define `jam_step`, because it
is what PARC does and the tree stays green with it. **But it does not repair
the five benches** that break under the corrected `cell_MC10170`: they still
fail with their original messages, and spacing the two strobes apart does not
help either.

So the jam sequence needs re-deriving **as a whole** against
`doradocpint.masm`, not repairing one strobe at a time. And note what holds a
jam today: with the inverted `p15`, PARC's own IRTable entries FAIL IM parity,
the MIR freezes, and the jam survives — **these benches have been depending on
a parity failure to do the single-step chain's job.**

One measurement trap: `tb_step.sv` defines neither `GAP` nor `WT`, so a
spacing edit copied from the other benches is a **compile error** there, and
its "FAIL" said nothing about the logic. Read the failure before counting it.

### The per-task T file is located and traced (2026-08-24)

T is held in four more F10145A files, addressed by `CurrLast'` like the rest of
the processor's per-task state:

```
ProcH l03  T.00-03     ProcH l04  T.04-07
ProcL l03  T.08-11     ProcL l04  T.12-15
```

Traced end to end on ProcH: `l03`'s Q0 is `ProcH02.sil+4`, which goes to `j03`
pin 5, out as `dT.00`, into `i03` pin 5, and out as **`T.00`**. `tb_taskrun`
has a `t_of(task)` reader for it now, beside `tpc_of` and `link_of`.

**It is displayed, not asserted, and that is deliberate.** Both slots read
`0000` in this sequence -- nothing ever writes T -- so "task 15's T is
unchanged" would be satisfied by an all-zero file and prove nothing. That is
the same vacuity the bench already guards against for TPC, and shipping the
assertion anyway would have been a gate that measures nothing.

**And T cannot be loaded with a jam.** Putting PARC's `TFromCPReg#` through
`parc_micro` between the task switches was tried and it destroys the thing
being measured: `DoDoradoMicroInst`'s first Control byte is
`ClrStop+ClrMIR+ClrCT+Freeze`, and **`ClrCT` clears the current task** --
`TPCAd` then reads 0 while task 7 is supposed to be running, and the existing
PC and Link claims start failing too.

So T must be written by RUNNING MICROCODE: put a `T<-` microinstruction in IM
and let each task execute it, the way `tb_exec` walks hunks in and free-runs.
The file is located, the reader is written, and that is all that is missing.

(`set_cpreg_plain` was added to `tb_taskrun` on the way -- B and T carry
opposite senses of CPReg, so T must be loaded through the plain form, not the
tilde one.)

### ...and MemBase's, which is addressed by `LastNext'` (2026-08-24)

The processor's per-task files are now fully mapped, and the two halves use
DIFFERENT task addresses:

| state | files | addressed by | bypass |
|---|---|---|---|
| **T** | ProcH l03/l04, ProcL l03/l04 (16 bits) | `CurrLast'` | `TbBypass` |
| **MemBase** | ProcH j16 (4 bits), j17 (1 bit) | `LastNext'` | `MBBypass` |

That split is not an oddity -- the Dorado pipelines the task switch and the
stages do not agree on which task is current, which is exactly why ProcH
carries two task addresses at all. **Both are already gated for ADDRESSING**
by `taskrun-test`'s existing check that every per-task file is indexed by the
running task (TPCAd, TLinkAd, CurrLast, LastNext).

Traced: h16 (an MC10158 2:1) selects j16's outputs against `ProcH25.sil+2..5`
under `MBBypass`, h17 does the same for j17's against `+1`, and both feed
g23's D inputs -- g23 being the MC10231 that drives `MemBase.0`/`MemBase.1`.

`membase_of(task)` reads it now. Like `t_of`, it is **reported and not
asserted**: MemBase reads `1f` in both slots (the uninitialised default) and T
reads `0000`, so an "unchanged" assertion on either would be vacuous. What
both need is microcode that WRITES them -- see the T entry above for why a jam
cannot do it (`ClrCT`).

### Writing T from IM: the specification, derived by trying it (2026-08-24)

Three requirements, each found by hitting it:

1. **The write must come from IM, not a jam.** `DoDoradoMicroInst`'s first
   Control byte is `ClrStop+ClrMIR+ClrCT+Freeze`, and **`ClrCT` clears the
   current task** — `TPCAd` then reads 0 while task 7 should be running, and
   the existing PC and Link claims start failing.
2. **And so must the IM load.** `send_a_hunk` goes through the same jam path,
   so building the hunk in the middle of the task-switch section breaks it the
   same way. Load it in the startup, beside the AEmu hunks.
3. **And the loop must keep tasking ON.** With a loop of plain `TFromCPReg#`
   (FF = `0o176`) `CTask` sticks at 15 and every later task is refused —
   `Switcha`/`BNTGtCT'` need `FF = TaskingOn`, which is why PARC's own
   `Return#` is "TaskingOff,Return".

**The encoding is in hand.** PARC's `TFromCPReg#` is `70 03 0F 04 C0`, and
through `mi()`'s byte layout those decode to

```
rstk 0, aluf 0, bsel 0, lc 1, asel 4, ff 0o176, jcn 0o201, block 0
```

which `build_hunk4` takes directly. Its value comes from **CPReg, which is not
per-task** — and that is what makes the test possible: set CPReg to one word
while task 15 runs and another while task 7 runs, and the two slots must end
up holding different values. (`tb_compute`'s variant changes ASEL 4 → 6 so the
ALU gets a real second operand, and notes `TFromCPReg#` "requires ALUFM[0]=B",
so the ALUFM prologue may be needed too.)

### Gate audit by global mutation (2026-08-24)

Today produced two gates that passed *because* of the bug they should have
caught (`msa-test`/SN74166; `memrun-test`'s DRAM cycle from a mis-shifted
PROM), and two more that would have shipped vacuous (T, MemBase). So: break
the most-instantiated cell in the machine and see which gates notice.

```
sed -i '' 's|if (ck_en) q <= {p12, p11, p10, p7, p6, p5};|if (1'"'"'b0) q <= {p12, p11, p10, p7, p6, p5};|' \
    verilog/cells/cell_MC10176.v      # 307 packages frozen
```

**16 of 35 gates caught it.** The 19 survivors were examined and every one is
legitimately independent of a flip-flop:

| survivor group | why it is unaffected |
|---|---|
| `alu-diff`, `alu-test` | the ALU is MC10181 |
| `prom-test`, `cell-check`, `backplane`, `loop-check`, `converge-test` | generation / structural checks |
| `strap-test`, `muffler-test`, `osc-test`, `task-test`, `refdecode-test` | straps, oscillators, combinational logic |
| `mir-diff` | tests the COMBINATIONAL demux fan-out -- six MC10172s wired 1-to-4; no flip-flop in the path |
| `machine-test`, `mem-test`, `ifu-test`, `firmware-probe`, `baseboard-test` | clock-reaches-every-slot and enable-consistency claims |
| `storage-test` | all four assertions are plumbing -- address inversion, strobes not X, the read register CLOCKED, a backplane alias. Its data claim lives in `msa-test` and `readback-test`, both of which WERE caught |

**No gate was found falsely claiming data behaviour**, which is the reassuring
half. The method's limitation is the other half: **a blanket mutation
over-reports**, because most gates legitimately do not touch any one part. A
real audit needs a mutation chosen per gate -- which is what the individual
mutation tests in `readback-test`, `msa-test` and `ifufetch-test` do.

Worth repeating after any large cell change: 16 is the floor, and a drop in
that number means a gate stopped biting.

### What a real cache fill still needs (2026-08-24, task #23)

`readback-test` seeds both the DRAM and the cache. Removing the cache seed
(`+nocacheseed`, `+zerocache`, both new) separates what each one supplies:

- **The storage read does NOT depend on the cache seed.** Without it the
  '166s still load twice and `Sin` still carries the seeded DRAM word — 64
  samples high, against 16 with the cache seeded. "A word comes out of
  storage" stands on its own.
- **What the cache seed supplies is the RETURN:**

  | | `dMD` | matched |
  |---|---|---|
  | cache seeded | `011100101101001101` | 16 |
  | cache empty | `110000000000000000` | **0** |

  Data zero with both parity bits set — `dMD` is reporting an **empty cache**.
  The fetched word reaches `SinD` and stops there.

- **And the cache IS being written.** a03's `WE'` falls **28** times, and
  `D0in.00` agrees with `SinD.00` on 25 of those (16 with the seed). The data
  path into the array is live and carrying the fetched word.

What reads back is still zero — and the address turns out **not** to be
broadly wrong. a03's twelve address pins read the SAME at the write and at the
read (`110000000000` both), so the fill is not landing in an unrelated row.

**THE FILL WORKS. What overwrites it is the bench's own loop.**

With `+onesdram` every DRAM cell is 1, so every cache write carrying the
fetched word is visible. Logging all 28 `WE'` falls with address *and* data
tells the whole story:

```
WE#18-21   3072,3073,3074,3075   D0in=1 SinD=1    <- THE LINE FILL
WE#26-29   3072 x4               D0in=0 SinD=0    <- the clobber
```

and the qualifiers separate them completely:

| | `MISSa` | `CacheRefInA` | |
|---|---|---|---|
| WE#18 (fill) | **1** | 0 | a MISS — the fill |
| WE#26 (clobber) | 0 | **1** | a HIT — the loop again |

So the fill deposits the fetched word into **all four words of the line on a
miss**, exactly as it should. The loop then comes round, references the same
line, **hits** it now that it is present, and writes zero into word 0. The
reference reads 3072 afterwards and sees that zero — which is why `dMD`
reported an empty cache.

**That is stimulus, not RTL.** The four-instruction loop is `←Map / Store /
Flush / quiet` and its Store carries no real data. To see a real fill return,
sample `dMD` between the fill and the next iteration, or run a loop with no
Store in it.

Ruled out on the way, each by measurement: the **address** (write and read
agree at `110000000000`); the **bank** (`D.00` wire-ORs a03/D0 and d03/D1 —
the fill goes to D0, which is also the enabled one, `CE'` low on 84,639
samples against d03's 1,296); and the **data path** (`SinD` reaches `D0in` and
the write port).

(The qualification that made all of this visible: `D0in == SinD` is also true
when both are zero, so an earlier 25-of-28 agreement was mostly zero matching
zero. **Qualify a data match on the data being non-trivial.**)

**A trap that cost three wrong comparisons here:** the bench binary must be run
from the **repo root**, the way the Makefile runs it (`cd ../..`). Run from
`verilog/verilator/` it cannot find `boot0.vec` or the PROM images, loads no
microcode, makes no references, and every counter reads zero — which looks
exactly like a behavioural difference and is not one. Second time today.

### The monochrome display board is in a machine (2026-08-24)

`make -C verilog display-test`. A new `dorado_display` config -- ContA, ContB,
ProcH, ProcL, MemC, MemD, MemX, msa and **DispY** -- derived from
`tb_readback`, so it brings the whole storage and memory section with it and
runs real `AEmu.mb!2` microcode out of IM.

**DispM is deliberately absent.** It plugs INTO DispY rather than replacing it
(42 nets are shared by the two boards and no other), so a monochrome machine
is both smaller and the right first target. Colour is not on the path.

Measured over 85,735 sys_clk:

| | |
|---|---|
| DispY local clock edges | **5,356** |
| `WakeDHT` high | **85,734** — the board is asking for the bus |
| `TWReq.11` high | **0** |

**And the head task is wired now.** `include/display.h` names all four
display tasks — DHT 3, DWT 13₈, AHT 4, AWT 11₈ — and **the table cross-checks
itself**: 13₈ = 11 and 11₈ = 9 are exactly the two word-task numbers the
boards' own straps give (DispY `DWTTask = 1011`, DispM `AltoWTask = 1001`).
Two independent derivations agreeing on the pair we already knew is what makes
the pair we did not — the head tasks, **3 and 4** — safe to take from the same
table. So `WakeDHT → TWReq.03` and `WakeAHT → TWReq.04` join
`BACKPLANE_WAKEUP_JUMPERS`, and DispY's head-task wakeup now reaches ContA.

`TWReq.11` staying low is expected: the WORD task needs a display list to
fetch, and nothing in this bench installs one.

**A side effect worth noting.** With DispY present and its head task wired,
`MemState` reaches **8 of 8** steps against `tb_readback`'s 2, and `MemIdle`
falls 1,752 times — the display task is driving real memory activity. That
activity overwrites the seeded cache exactly as the four-instruction loop's own
Store does, so `tb_readback`'s inherited assertions are relaxed to displays
here, the way `tb_ifufetch` relaxes `tb_memrun`'s.

`CLK.display'` is the fifth board on which leaving a slot clock undriven would
have looked exactly like a gating bug.

### What the display WORD task needs (2026-08-24)

Traced, so the next step is specified rather than explored:

```
TWReq.11      <- d03, an MC10231 D flip-flop: D = DWTWantsProc, clocked by
                 clk0'Ab, killed by KillDWTWakeup
DWTWantsProc  <- g11, an MC10117 OR-AND, per channel:
                   A: ACurrentWCBFlag' AFifoNotFull' ACurrentWCBFlag ANextWCBFlag'
                   B: the same four
KillDWTWakeup <- e24 (an F10016) from CurTaskIsDWT', StopWakeCount,
                 WakeupWait.0-3 and DWTShutUp -- a wakeup HOLDOFF so the task
                 cannot re-ask immediately -- wire-ORed with f24's
                 IOHold/NextSaysDWT' term
```

**WCB is the Word Control Block — the display list.** So the word task asks
when a channel's FIFO has room *and* its WCB flags say there is work. The next
step is therefore **slow-I/O writes to set up a WCB**: DispY's registers are at
`DDCTIOA = 37B` → **0370-0377**, from the board's own strap (`strap-test`), and
the C emulator's `display.c` claims exactly those four addresses — two
independent statements of the same range.

The HEAD task is simpler, which is why it already asks: `TWReq.03` is f01, an
MC10135 JK flip-flop gated by `DHTShutUp` and `NLCBCommand'`, with no FIFO or
WCB in the way.

### The word task's condition is confirmed structurally (2026-08-24)

Setting up a real WCB needs slow-I/O writes and microcode to drive them. What
*can* be settled now is that the traced chain is right, by driving channel A's
four inputs and watching `DWTWantsProc`:

| channel A | `DWTWantsProc` |
|---|---|
| WCB flags set, FIFO **has room** | **1** |
| same, FIFO **full** | **0** |

So g11's OR-AND reading is correct and the word task asks exactly when a
channel has work and somewhere to put it.

**One trap, and it is the OR.** g11 ORs the **two channels**, so channel B has
to be held inactive or it satisfies `DWTWantsProc` on its own. The first
attempt measured **1 with A's FIFO empty and 1 with it full** — B was
answering both times, and the result looked like "the traced condition is
wrong" when the bench was simply asking the wrong question. Hold the other
channel.

`KillDWTWakeup` is forced inactive for the measurement too, so the D flip-flop
is not being cleared while the question is asked.

### DispY's slow-I/O address decodes, and only at 37B (2026-08-24)

A WCB is set up by writing the board's registers, and the board answers only
when TIOA matches its strap. e02/e03 (MC10113 XORs) compare `DDCTIOA.00-04`
against `TIOADly.00-04` and wire-OR the result into **`TIOASaysDDC'`**, low
when they match.

Sweeping all 32 five-bit values: **exactly one selects the board, and it is
31 = 37B.** That is the same `muffler-test` shape, and it agrees with two
independent statements of the range — the board's own strap (`strap-test`:
DispY g42, no legs cut, `DDCTIOA = 37B` → 0370-0377) and the C emulator's
`display.c`, which claims those four addresses.

So the write path into DispY is open and addressable. What remains for a real
WCB is microcode to drive `TIOA←` and `IOB←` at those addresses.

### The two slow-I/O microinstructions (2026-08-24)

Derived from `cpu.c`'s FF decode and cross-checked:

| operation | FA | FB | FC | FF |
|---|---|---|---|---|
| `TIOA ← B[0:7]` | 1 | 5 | 2 | **0o152** |
| `Output ← B` (the IOB write) | 0 | 3 | 6 | **0o036** |

The FF field packs as `{FA(2 bits), FB(3), FC(3)}`, and that is checkable
against a case already on record: `←Map` is FA=0 FB=3 FC=1 and its FF subfield
is documented as `0o31` — and `00 011 001 = 0o031` exactly. So both encodings
above are trustworthy without needing a second source.

`build_hunk4()` and `mi()` are already in `tb_display` (inherited from
`tb_memrun` via `tb_readback`), so a WCB write is a `TIOA←` carrying 0370-0377
followed by an `Output←` carrying the WCB word.

**The three requirements from the T-write attempt apply unchanged**: the
instructions must come from IM rather than a jam (`DoDoradoMicroInst`'s first
Control byte carries `ClrCT`); the IM load must happen in the startup for the
same reason; and the loop must keep tasking ON or `CTask` sticks.

### The processor addresses the display board (2026-08-24)

`TIOA ← B[0:7]` takes B's HIGH byte, and DispY's select is TIOA[0:4] = 37B, so
B must be `0xF800`. **The constant cannot come from the FF field** — BSEL=6 is
`FF,,0`, but FF is consumed by the opcode (`TIOA←B` is FA=1 FB=5 FC=2 =
FF `0o152`), so the two conflict. It comes from **T** instead, loaded the way
`compute-test` does it: PARC's `TFromCPReg#` with the **plain** CPReg sense,
because B and T carry opposite senses of CPReg.

Measured:

```
T        = f843        high byte f8, as loaded
TIOA     = 11111000    370B -- exactly right
TIOADly  = 11111       the address crossed the backplane intact
IgnoreCommands = 1     <- and this is what blocks the select
```

The first three are gated. The fourth is the next step: with the address
correct at the board, `TIOASaysDDC'` stays deasserted because
`IgnoreCommands` is high — which is why the TIOA sweep above forces it low.
Find what sets it: a reset state the microcode clears, or a control-register
write.

(The `f843` low byte is left over from the startup's `T=0043`; only the high
byte matters to TIOA, but it is worth knowing `TFromCPReg#` did not clear it.)

### Why the board ignores a jammed command (2026-08-24)

`IgnoreCommands` is DispY **d01**, an MC10176 whose D5 is **`IgnoreProcb`**,
clocked by `clk1'Ab` — a *registered copy of `IgnoreProc`*. That is the same
signal the memory section uses (MemC b24: `WantProcRef' = IgnoreProc |
ASEL.0`), and it is asserted **while the processor is being stepped**.

So a jam can set `TIOA`, because TIOA is processor state, and `display-test`
proves it does — `TIOA = 11111000`, and DispY sees `TIOADly = 11111`. What a
jam **cannot** do is make a device answer: every I/O board is told to ignore
commands for exactly as long as the processor is single-stepped.

**Slow I/O must come from running microcode.** That is a fourth entry on the
list the T-write attempt started:

1. the write must come from IM, not a jam (`ClrCT` clears the current task)
2. the IM load must also be in the startup, for the same reason
3. the loop must keep tasking ON, or `CTask` sticks
4. **and a device write must not be jammed, because `IgnoreProc` blocks it**

All four are properties of the jam mechanism, and together they say the same
thing: a jam is for setting up processor state, not for driving the machine.

### The display board answers a command (2026-08-24)

`display-test +slowio` loads a slow-I/O loop into **IM[0..3]** and runs it:

```
IM[0]  TIOA <- B     FA=1 FB=5 FC=2 -> FF 0o152, BSEL=2 (B <- T)
IM[1]  Output <- B   FA=0 FB=3 FC=6 -> FF 0o036, BSEL=2 (B <- T)
IM[2]  quiet         ASEL=4
IM[3]  quiet, jumping back to 0
```

with T preloaded to `0xF800` by a jam before the run — a jam is exactly right
for that, because T is processor state; what a jam cannot do is make a device
answer.

| | board SELECTED | of which with the processor free |
|---|---|---|
| baseline | 448 | 448 |
| **`+slowio`** | **928** | **928** |

**The board answers, from running microcode**, and every select happens while
`IgnoreCommands` is low. Both are gated: a select with the processor stepped
would mean the model lets a jammed instruction drive a device, which the
hardware does not.

All four jam limitations were applied in advance rather than discovered:
the loop is in IM, its **load** is in the startup, it keeps quiet slots, and
the device write is not jammed. That is the first piece of this work where the
constraints were known before the first attempt.

Measuring **during** the run matters: the post-run section jams, so
`IgnoreCommands` reads 1 there by definition, and an earlier reading taken
there looked like the board was refusing commands when it was simply being
asked at the wrong moment.

### A second register for the data, and where it stops (2026-08-24)

FF is consumed by the opcode, so B cannot carry a literal — the data has to
come from a register, and **Q** is the second one available (BSEL=3). PARC's
`QFromCPReg#` loads it, with its own caveat from `tb_compute`: **Q is not
loaded by `QFromCPReg#`, it is loaded by the Nop after it**, because the
controls in force at a load edge are the ones the *previous* instruction
latched.

With T = `0xF800` (the address) and Q = `0x5A5A` (the data), the loop becomes
`TIOA←B` (BSEL=2, from T) then `Output←B` (BSEL=3, from Q). Q reads back
`5a5a` before the run, and the board now selects on **992** samples against
928 for the T-only loop and 448 baseline.

**The address half works, and so does the STROBE.** ProcH h01 is an MC10197
hex AND — `IOB = IOBout & alub` — so `IOBout` asserting is the write actually
happening:

| | `IOBout` high | `alub` at that moment |
|---|---|---|
| baseline | **0** of 85,735 | — |
| `+slowio` | **32** of 140,559 | `0015` |

Those 32 match IOB's 32 non-zero samples exactly. **The `Output←` instruction
decodes and strobes**, and both directions are gated (asserting with the loop
loaded, silent without).

**Open, and narrowed to one thing: B does not carry Q.** At the moment
`IOBout` is high, `alub` reads `0015`, not the `5a5a` that Q demonstrably
holds. So the strobe is right and the **data selection** is wrong — BSEL=3 is
not delivering Q.

That was the first of three candidates; **the other two are now ruled out** —
the IOB drive needs no qualifier beyond `IOBout`, and bit order cannot explain
a value that is not Q at all.

`cpu.c`'s `b_bus` case 3 is *"Q (or Q←B with external)"* — two behaviours
sharing one BSEL, and the external qualifier is not driven here. Worth noting
what `0015` **is**: `0o25 = 0x15`, and `0o25` is exactly the value PARC's own
`QFromCPReg#` sends in `tb_compute` — so `0015` may be a stale or default B
rather than a wrong register.


### THE PROCESSOR WRITES A WORD TO THE DISPLAY BOARD (2026-08-24)

`display-test +slowio`. Running microcode, nine boards, no forcing:

```
TIOA <- B        370B on the bus, DispY sees TIOADly = 11111
Output <- B      IOBout strobes 960 times, alub = 5a5a at every one
IOB at DispY     carries 5a5a on exactly 960 samples
board selected   928, every one with the processor free
```

`IOBout` count and IOB-carrying count agreeing exactly is the write landing —
ProcH h01 is an MC10197 hex AND, so `IOB = IOBout & alub`. All gated.

**What made it work was ORDERING, not logic.** The first version loaded T and
Q *after* the map preload — late — while the loop had been running since the
IM send. **Zero of 32 strobes** coincided with Q holding its value, `alub` read
a stale `0015`, and it looked exactly like BSEL=3 failing to select Q.

It wasn't. The mux is right: ProcH **b01** is an MC10174 whose X0 is Q and
whose selects `Bmux0'`/`Bmux1'` are **primed**, so BSEL=3 drives both low and
picks X0. Moving the operand load to immediately after the IM send took the
strobes from **32 to 960** and `alub` from `0015` to `5a5a`.

**Operands before the loop that uses them.** Obvious in hindsight, and it cost
a wrong hypothesis about the B multiplexer — one that had survived ruling out
two other candidates and looked well-supported.

### One NLCB write is not enough — the board needs initialising (2026-08-24)

`include/display.h`'s register map:

```
0370 DDCSTATUS   0373 DWTFLAG   0374 DHTFLAG
0375 HRAM        0376 NLCB      0377 STATICS
```

and it says writing NLCB *"load NLCB, set αNextWCBFlag"* — one of g11's four
inputs. So `display-test +slowio +nlcb` points the write at 0376.

**The write happens** — 960 `IOBout` strobes carrying `5a5a`, all with Q
loaded — **and `TWReq.11` still does not assert.** Counting g11's channel-A
inputs separately, because a conjunction that never holds says nothing about
*which* term is at fault:

| over 140,559 samples | with NLCB | without |
|---|---|---|
| `ACurrentWCBFlag` | 0 | 0 |
| `ANextWCBFlag'` low | 0 | 0 |
| **`AFifoNotFull'` low** | **0** | **0** |

All three inactive either way. **The FIFO reporting full throughout** is the
interesting one — that looks like a reset state, so the board very likely
needs a real **initialisation sequence** (0377 STATICS and/or a reset) before
it will accept work. One register write was never going to be enough, and the
bench now says so with numbers rather than failing silently.

### Disk: a starting point, deliberately not a gate (2026-08-24)

`dorado_disk` — ContA, ContB, ProcH, ProcL, MemC, MemD, MemX, msa and
**DskEth** — generates and lints, and `tb_disk` (derived from `tb_display`)
compiles and runs the whole inherited apparatus on it. The slow-I/O write loop
executes: **`IOBout` strobes 960 times with `alub = 5a5a`**, the same as on the
display machine.

**`disk-test` is NOT in the gate list.** Its inherited assertions are all
relaxed, and a bench whose assertions have been relaxed is not gating anything
— shipping one as a gate would be exactly the failure mode the gate audit went
looking for.

**The first thing to chase:** `TIOA ← B` leaves TIOA at `00000000` here, where
the identical sequence on `dorado_display` gives `11111000`. So the **address
write itself does not reproduce on this machine**, and everything downstream —
the board select, the compare against `TIOA-Ad` — is moot until it does. **Do
not start from the DskEth end.**

DskEth's own facts, for when that is fixed: its strap is an I/O **address**
(`TIOA-Ad = 1` → 010-017; the sheet says *"Standard addresses are 10-17"*, and
`include/disk.h` independently has `DISK_TIOA_DISKCONTROL 010`). e01 is an
MC10166 comparing `TIOA.0-4` against the strap with X>Y and X<Y wire-ORed, so
that net is **low when they match**. And DskEth compares **TIOA directly** —
it has no `TIOADly` of its own, unlike DispY, which is a difference worth
confirming rather than inheriting.

### ...and the disk anomaly was a SAMPLING ARTEFACT (2026-08-24)

Resolved, and there was never a board difference. `TIOA ← B` works on the disk
machine exactly as on the display machine; what differed was **where the bench
looked**:

| | TIOA over a window | final fixed-offset sample |
|---|---|---|
| `dorado_display` | `11111000` | `11111000` |
| `dorado_disk` | **`11111000`** | `00000000` |

The register **is** written on both. `tb_disk` carries more probes and so
reaches its single sample later, by which point TIOA has been rewritten.
Gating on the window — the honest measurement — makes it pass.

**Three explanations were offered before this one and all three were wrong:**

1. *"the board's presence changes the TIOA net"* — no. Neither DskEth nor
   DispY drives **any** of `TIOA.0-7` (all eight checked, after the first
   version of this claim generalised from bit 0 alone); ProcH g10 does.
2. *"this bench writes the disk's address 010B"* — no. That edit silently did
   not apply, so both machines ran identical stimulus throughout.
3. *"the jam is being blocked"* — no. Both report 180 `clk0'` edges, `Stop=0`,
   every hold line inactive.

The actual cause was the **fourth** instance in one session of sampling at the
wrong moment — after the memory fill, the display's IOB, the Pipe pointer and
the clock chain. **When a value looks absent, capture it over a window before
concluding it was never there.**

### Disk: a starting point, deliberately not a gate (2026-08-24)

`dorado_disk` — ContA, ContB, ProcH, ProcL, MemC, MemD, MemX, msa and
**DskEth** — generates and lints, and `tb_disk` (derived from `tb_display`)
compiles and runs the whole inherited apparatus on it. The slow-I/O write loop
executes: **`IOBout` strobes 960 times with `alub = 5a5a`**, the same as on the
display machine.

**`disk-test` is NOT in the gate list.** Its inherited assertions are all
relaxed, and a bench whose assertions have been relaxed is not gating anything
— shipping one as a gate would be exactly the failure mode the gate audit went
looking for.

**The first thing to chase:** `TIOA ← B` leaves TIOA at `00000000` here, where
the identical sequence on `dorado_display` gives `11111000`. So the **address
write itself does not reproduce on this machine**, and everything downstream —
the board select, the compare against `TIOA-Ad` — is moot until it does. **Do
not start from the DskEth end.**

DskEth's own facts, for when that is fixed: its strap is an I/O **address**
(`TIOA-Ad = 1` → 010-017; the sheet says *"Standard addresses are 10-17"*, and
`include/disk.h` independently has `DISK_TIOA_DISKCONTROL 010`). e01 is an
MC10166 comparing `TIOA.0-4` against the strap with X>Y and X<Y wire-ORed, so
that net is **low when they match**. And DskEth compares **TIOA directly** —
it has no `TIOADly` of its own, unlike DispY, which is a difference worth
confirming rather than inheriting.

### ...and the disk anomaly is real but NOT the board (2026-08-24)

Two retractions on this one, both worth keeping.

Measured, identical stimulus — both benches load `0xF800` and jam the same
`TIOA←B`:

| | T at the write | TIOA after |
|---|---|---|
| `dorado_display` | `f800` | **`11111000`** |
| `dorado_disk` | `f800` | **`00000000`** |

**1. "The board's presence changes the TIOA net" — no.** `TIOA.0` is *driven*
by ProcH g10 (an F10000) and merely *loaded* by both DskEth (e01.10) and DispY
(e01.5). Neither I/O board drives it, so there is no driver difference to
appeal to.

**2. "This bench writes the disk's address 010B" — no.** The edit meant to
retarget it **silently did not apply**; only its comment and its
"want 00001000" message did. It still loads `0xF800`. The two machines have
been running the same stimulus all along — which is what makes the comparison
valid, but for a different reason than was claimed.

**So the anomaly is real, and every obvious explanation is now eliminated:**

- **Identical stimulus** — the two benches' TIOA sequences are byte for byte
  the same, and T reads `f800` on both at the write.
- **Identical machine behaviour** — both report 180 `clk0'` edges, `Stop=0`,
  all four hold requests 0 and all three hold lines 1. Nothing is holding, so
  the jam is not being blocked.
- **No board drives TIOA** — all **eight** bits checked on both boards this
  time, rather than one and generalised. Neither DskEth nor DispY drives any of
  `TIOA.0-7`; ProcH g10 is the driver.

What is left is the TIOA register's own write enable on ProcH, or **the capture
instant** — both benches sample right after the trailing nop, but the disk
bench carries more probes and a different amount of simulated time before that
point, and *"sampled at the wrong moment"* has been the most expensive error in
this effort. Capture TIOA over a **window** around the jam on both machines
before concluding the register is not being written. Start by asking whether the
**jam itself executes** there — DskEth drives hold lines the display board does
not, and a jam that never runs writes nothing. Measure the hold lines and the
`clk0'` edge count on both machines before touching TIOA again.

**And check that an edit applied.** Two silent no-op edits in this one file
produced two wrong conclusions. An edit that does not throw is not an edit that
landed.

### Disk: a starting point, deliberately not a gate (2026-08-24)

`dorado_disk` — ContA, ContB, ProcH, ProcL, MemC, MemD, MemX, msa and
**DskEth** — generates and lints, and `tb_disk` (derived from `tb_display`)
compiles and runs the whole inherited apparatus on it. The slow-I/O write loop
executes: **`IOBout` strobes 960 times with `alub = 5a5a`**, the same as on the
display machine.

**`disk-test` is NOT in the gate list.** Its inherited assertions are all
relaxed, and a bench whose assertions have been relaxed is not gating anything
— shipping one as a gate would be exactly the failure mode the gate audit went
looking for.

**The first thing to chase:** `TIOA ← B` leaves TIOA at `00000000` here, where
the identical sequence on `dorado_display` gives `11111000`. So the **address
write itself does not reproduce on this machine**, and everything downstream —
the board select, the compare against `TIOA-Ad` — is moot until it does. **Do
not start from the DskEth end.**

DskEth's own facts, for when that is fixed: its strap is an I/O **address**
(`TIOA-Ad = 1` → 010-017; the sheet says *"Standard addresses are 10-17"*, and
`include/disk.h` independently has `DISK_TIOA_DISKCONTROL 010`). e01 is an
MC10166 comparing `TIOA.0-4` against the strap with X>Y and X<Y wire-ORed, so
that net is **low when they match**. And DskEth compares **TIOA directly** —
it has no `TIOADly` of its own, unlike DispY, which is a difference worth
confirming rather than inheriting.

### ...and the disk anomaly is the BOARD, not the bench (2026-08-24)

Measured side by side, same jam, same operand:

| | T at the write | TIOA after |
|---|---|---|
| `dorado_display` | `f800` | **`11111000`** |
| `dorado_disk` | `f800` | **`00000000`** |

**So it is not the bench and not the processor.** T is identical, the jam is
identical, and only the I/O board differs — the board's *presence* is changing
the TIOA net.

Where to look: TIOA crosses the backplane as a wired-OR, and DskEth is a board
with SIP pull networks — the same class of thing that once made six active-low
drive-status lines read ASSERTED and fabricated a disk that was not attached
(`sip_drives`). Check what DskEth contributes to `TIOA.0-4`'s driver tree, and
whether it is input-only on those nets as DispY effectively is.

(Separately: `tb_disk`'s post-run section still loads `0xF800`, so its
"want 00001000" message compares against the wrong expectation — it should
want 370B there. The `00000000` is the real anomaly; the expectation string is
a second, smaller bug.)

### The processor addresses the disk board at its own 010B (2026-08-24)

`disk-test` gates it now. DskEth's strap is an I/O **address** (`TIOA-Ad = 1`
→ 010-017), not a task number like the display boards', so the loop loads
B = `0x0800` and `TIOA←B` takes the high byte. e01 is an MC10166 comparing
`TIOA.0-4` against the strap with X>Y and X<Y wire-ORed, low on a match.

**The board selects on 1,920 samples**, every one with the processor free —
against **0** before the retarget.

That retarget is the same one-line change that **silently failed twice**
earlier: the line is indented eight spaces, and both previous attempts assumed
six. Fixing it by line number, and *checking the result*, took the select from
0 to 1,920 immediately.

Two boards now answer the processor from running microcode: DispY at 370B and
DskEth at 010B, each at the address its own strap dictates, and each
cross-checked against the C emulator's independent claim of the same range.

### The ethernet wakeups are wired -- and the disk's cannot be (2026-08-24)

`WakeEthTx → TWReq.06` and `WakeEthRx → TWReq.07` join
`BACKPLANE_WAKEUP_JUMPERS`, from `include/ethernet.h`:

```
DORADO_ETHERNET_TASK_EOT  06     Ethernet Output Task -- Tx
DORADO_ETHERNET_TASK_EIT  07     Ethernet Input  Task -- Rx
```

DskEth's own strap cannot settle these — unlike the display boards' it is an
I/O **address** (`TIOA-Ad = 1` → 010-017), not a task number — so the numbers
have to come from elsewhere, and `ethernet.h` is genuinely independent:
it registers its slow-IO handlers on exactly those two tasks.

**That completes every wakeup jumper the boards drive:**

| net | line | source |
|---|---|---|
| `WakeAWT` | `TWReq.09` | DispM strap, and DispM30.sil says "Task 9D = 11B" |
| `WakeDWT` | `TWReq.11` | DispY strap |
| `WakeDHT` | `TWReq.03` | `display.h` DHT |
| `WakeAHT` | `TWReq.04` | `display.h` AHT |
| `WakeEthTx` | `TWReq.06` | `ethernet.h` EOT |
| `WakeEthRx` | `TWReq.07` | `ethernet.h` EIT |
| `TWReq15` | `TWReq.15` | the fault task, HM §4.1 and `memory.h` |

**And the disk's wakeup is wired too — it was there all along under a
different naming convention.** An earlier version of this note said DskEth
drove no disk wakeup net; that was a search for `Wake*`, and **this board uses
`*TW`** (Task Wakeup) instead. It carries nine:

```
ClearIndexTW  ClearSectorTW  DiskTW  IndexTW  RdFifoTW
SectorTW      SeekTagTW      SetTagTW  WrFifoTW
```

and exactly **one leaves the board** — `DiskTW`, driven by e04 (an MC10231).
The other eight are internal, which is what makes `DiskTW` identifiable as the
backplane wakeup rather than one of the board's own strobes. Its number is
`include/disk.h`'s `DORADO_DISK_TASK = 014₈ = 12` — the same file whose
`DISK_TIOA_DISKCONTROL 010` independently confirms the `TIOA-Ad` strap.

So `DiskTW → TWReq.12` completes the table.

**Two naming conventions for the same thing** is the lesson: the display
boards say `WakeDWT`, DskEth says `DiskTW`. Search for both.

### The disk's register decode, and it matches the C emulator exactly (2026-08-25)

DskEth registers its board select and the low TIOA bits in **e02** (an MC10175
clocked by `FHCP`): the select becomes `TIOA=Us'` and TIOA.5-7 become
`TIOA.5a/6a/7a`. Those then drive **f07**, an MC10161 1-of-8 decoder — and its
outputs, in pin order, are exactly `include/disk.h`'s registers in address
order:

| decoder output | net | address | `disk.h` |
|---|---|---|---|
| Q0' | `TIOA=Cont'` | 010 | `DISK_TIOA_DISKCONTROL` |
| Q1' | `TIOA=Muff'` | 011 | `DISK_TIOA_DISKMUFF` |
| Q2' | `TIOA=Data'` | 012 | `DISK_TIOA_DISKDATA` |
| Q3' | `TIOA=Ram'` | 013 | `DISK_TIOA_DISKRAM` |
| Q4' | `TIOA=Tag'` | 014 | `DISK_TIOA_DISKTAG` |
| Q5'/Q6' | `TIOA=EthData'`/`EthCtrl'` | 015/016 | (the ethernet half) |

Two independent derivations — a 1979 wire list and a C emulator written from
the manual — agreeing on all five names *and* their order.

**Gated:** with the loop writing 010B, `TIOA=Cont'` asserts on 1,921 samples
and Muff/Data/Ram/Tag on **zero**. That matters beyond the count: without it,
"the board is addressed" would not imply "DISKCONTROL is addressed", and every
later register write would be unverifiable.

### TIOA is a per-task register file, and one inverted gate hid it (2026-08-25)

The disk board decoded `DISKCONTROL` correctly and the data appeared on IOB
correctly, but **never at the same moment** — 960 `Output<-` strobes, and not
one of them found the board's address still on TIOA. At the strobe TIOA carried
`132` octal, which is `0x5A`: the *high byte of the data*. TIOA was being
reloaded from B by every instruction.

**What TIOA actually is.** The name says it — **T**ask **I/O A**ddress — and
ProcH builds it exactly that way:

| package | part | role |
|---|---|---|
| h13 | MC10158 | mux on `FFdly.4`: `alub` (new value) or `TIOAdly` (recirculate) |
| g15 | **F10145A** | the **16x4 RAM**, addressed by `LastNext` — *the task* — stored by `TIOAWrite'` |
| g14 | MC10158 | mux on `TIOABypass`: the just-computed value, or the RAM's stored one |
| g10/h10 | F10000 | the output register (PE' is an open pin, so it parallel-loads every clock) |

`TIOABypass` comes from h20, an MC10100 NOR of `Curr=Next'` with g19's output —
and g19 is an **MC10118** whose pin-2 term is `(FB=4' + FA=2') & (FB=5' + FC=2'
+ FA=1'a)`. That goes low for exactly FA=1/FB=5/FC=2, which *is* `TIOA<-B`. So
the bypass means "this instruction writes TIOA and the task is not switching" —
a textbook write-forward, with the RAM holding the address the rest of the time.

**The bug was one `~`.** `cell_MC10118` inverted pin 2, by the OR/NOR family's
"EclDict role `OUT` is the inverting output" rule — a rule already recorded here
as one that **does not generalise**. Inverted, the bypass was high on 136,406 of
140,559 samples instead of 991, so TIOA followed `alub` combinationally whatever
the instruction was, and no I/O address ever survived to the `Output<-` using it.

Settled three ways, and the datasheet is decisive:

1. The part is titled **"DUAL 2-WIDE 3-INPUT OR-AND GATE"** (1978 Motorola MECL
   Data Book p.62). Its sibling the MC10117 is titled *OR-AND/OR-AND-INVERT* —
   Motorola names the inverting variant, and this is not it.
2. Read by **geometry**, the method already established here: pins 2 and 15 each
   end in a plain emitter-follower wedge with **no bubble**. The MC10117 is the
   control, in the same drawing convention — it puts an unmistakable open circle
   at pins 3 and 14 and a bare wedge at 2 and 15, and EclDict calls pin 3 `OUT`
   and pin 2 `o`. So `OUT` does mark the inverting output *there*; the MC10118
   has no inverting output to mark, and its `OUT`/`o` pair merely distinguishes
   two gates that each have one output.
3. Functionally two-sided: non-inverting makes `TIOA<-B` bypass ON *and*
   `Output<-` bypass OFF. Inverting got both backwards.

The MC10117 sheet also states in prose what the netlist showed — **"Pin 9 is
common to both gates"** — confirming the cell's grouping `(6|7|9)` and
`(9|10|11)`.

**Two gates were passing because of it.** `display-test` was not even running
its `+slowio` loop (the plusarg was never passed), so every select it counted
came from the broken bypass; and both benches asserted that a **jammed**
`TIOA<-B` leaves the address in TIOA. It cannot: measured across the jam window,
`TIOAWrite'` asserts **0** times and `TIOABypass` **0** times, so a jam reaches
neither the RAM nor the forward path. That is the same rule already recorded for
device writes — a jam asserts `IgnoreProc`, which becomes `IgnoreCommands`.

Both now gate the IM-executed loop instead, where a device write belongs: TIOA
holds its address on 128 samples, and **32 of the strobes find address and data
together** where zero did before. Three mutations confirm the gates bite —
restoring the `~`, dropping `+slowio`, and pointing the loop at a wrong address.

*Lesson, and it is the same one twice now:* a rule derived from one part family
is not evidence about another. Take the sheet, and read the bubble.

### The disk controller obeys a command (2026-08-25)

With the per-task TIOA holding its address, the `+slowio` loop's `Output<-`
reaches DskEth's control register and the board acts on it. c18 (SE10212) turns
`TIOA=Cont'` + `bIOout'` into **`ControlRegCl`**, which clocks:

| package | part | what it makes |
|---|---|---|
| e14 | MC10231 | `DisableRun`/`EnableRun` from `bIOB.05`, `DebugMode` from `bIOB.06` |
| e15 | MC10231 | `BlockTillIndex`/`SectorOvfl` from `bIOB.07` |
| b21 | — | `RamAddr.0-3`, the format-RAM address |
| e13 | — | `Active`/`Idle` |

**`src/disk.c`'s handler names every one of those**, from the Hardware Manual:
"Output to control register zeros the format-RAM address register (HM page 98)
... ClearEnableRun is honored. SetDebugMode is honored. SetBlockTillIndex is
honored ... while Active, the first DiskControl output aborts the current sector
transfer". And `include/disk.h` gives the bit positions:

```
B[5] = ClearEnableRun    B[6] = SetDebugMode    B[7] = SetBlockTillIndex
```

which is exactly `bIOB.05`, `.06`, `.07` into exactly those flip-flops. Measured:
one `ControlRegCl` edge, all while DISKCONTROL was addressed, `bIOB.05/06/07 =
010` (IOB bits 5-7 of `5a5a`, so `bIOB` is a straight buffer), and the register
settles at `{DebugMode, BlockTillIndex, EnableRun} = 101` — set, clear, still on,
which is what those three bits ask for.

One detail confirms the pairing beyond the bit numbers: **e14 FF-b's RESET pin is
`DisableRun`**, so clearing EnableRun also clears DebugMode — and `disk.c`'s
`CLR_ENABLE_RUN` branch clears `debug_mode` in the same breath.

Two traps on the way, both already on the list and both hit anyway:

* **Count edges, not levels** — `ControlRegCl` is a level held across many
  samples; it is ONE edge.
* **Read a flip-flop after it settles, and on ITS OWN edge.** e14's DebugMode
  half clocks on `ControlRegCl'`, the opposite edge from the one counted, so a
  post-edge window keyed to `ControlRegCl` rising read it before it moved and
  showed `DebugMode = 0` for a bit the manual says is set.

The gate asserts the RELATION (`DebugMode` follows `bIOB.06`, `BlockTillIndex`
follows `bIOB.07`, `EnableRun` is the complement of `bIOB.05`) rather than the
constant, and mis-wiring the check to a neighbouring bit fails it.

*Also corrected:* tb_disk's "IgnoreCommands LOW on N of N" line was inherited
from tb_display and is a CONSTANT — DskEth has no such net. It is labelled as
measuring nothing, and the control-register gate qualifies on `TIOA=Cont'`.

### The decode DISCRIMINATES: the same loop, one address higher (2026-08-25)

One decoder output asserting proves little next to the other four going quiet
when the address changes. `+tag` points the `+slowio` loop at **DISKTAG (014B)**
instead of DISKCONTROL, and the two runs are exactly complementary:

| | `disk-test` (010B) | `disk-tag-test` (014B) |
|---|---|---|
| f07 decode | **Cont 127**, Muff/Data/Ram/Tag 0 | **Tag 127**, Cont/Muff/Data/Ram 0 |
| TIOA at the strobe | held `010`, 32 of 960 strobes | held `014`, 32 of 960 strobes |
| `ControlRegCl` | **1 edge** | **0** |
| `Tag_IOB` / `TagClock` | **0 / 0** | **1 / 1** |

The seek path is the control path's twin: b17 (SE10211) makes `Tag_IOB` from
`TIOA=Tag'` + `bIOout'` exactly as c18 makes `ControlRegCl` from `TIOA=Cont'` +
`bIOout'`, and d16 turns it into **`TagClock`**, which shifts the seek tag out
to the drive.

Three bench defects surfaced doing this, all worth keeping:

* **The gates ran BEFORE the diagnostics**, so a failing gate aborted with none
  of the numbers needed to diagnose it. Print everything, then assert.
* **The register counters were unqualified**, and every mode counted exactly one
  stray `Cont`. It was at **sample 0**, board not selected: every net is still 0
  and the active-low `Q0'` reads asserted before anything settles. f07's *enable*
  is the board select, so a decoder output means nothing while the board is not
  addressed — and "of N selects" in the report already claimed that
  qualification. Qualified, the strays vanish.
* **`$test$plusargs` looked broken and was not.** `for m in "+slowio +tag"` in
  zsh passes ONE unsplit argument, so the binary never saw `+tag` — the same
  no-word-splitting trap already recorded. Separate invocations, or `bash -c`.

And a fourth, on the mutation testing itself: a `python3 -c` one-liner carrying
nested quotes failed to parse, the edit never applied, and both mutations
reported a meaningless PASS. **Print a verification that the edit landed before
believing the result** — the "silent no-op edit" failure mode, hit again.

### The other direction: the board drives IOB back (2026-08-25)

Everything above is processor -> device. `+input` puts a **`Pd<-Input`** in the
loop's third slot -- **FA=0 FB=3 FC=2 = FF `0o032`**, from `cpu.c`'s own
dispatch, the same FA/FB as `Output<-B` (`0o036`) one FC apart.

The read path is the write path's mirror, package for package:

| | write | read |
|---|---|---|
| strobe/enable made by | c18 (SE10212) | **b11 (MC10103)** |
| from | `bIOout'` AND `TIOA=Cont'` | **`bIOin'` AND `TIOA=Us'`** |
| driving | `ControlRegCl` | the **E'** of ten MC10174 muxes |
| which select | — | `DskData` / `Host` / `EthData` by `{TIOA.5a, TIOA.7a}` |

Measured: with `+input`, `bIOin'` asserts **960** times -- exactly the count of
`Output<-` strobes, so the Input executes and reaches the board -- and the IOB
output enable asserts on **32**, the same 32 samples where the address and the
access coincide. Without it, both are **zero**.

`IOB` reads `0000` there, and that is honest: `DskData`/`Host`/`EthData` are all
zero because no drive is attached. The enable is what this rung proves; the data
needs a drive model.

**The negative half is the stronger gate.** A board that drives the bus when
nobody is reading it is a bus fight, so `disk-test` and `disk-tag-test` now
assert that the enable *never* asserts and `bIOin'` never fires. Both mutations
bite: removing the `Pd<-Input` fails `disk-input-test`, and reading the enable in
the wrong sense fails `disk-test`.

*Which was nearly a false bug report.* `DskEth03.sil+1` is **ACTIVE LOW** --
b11's pin-2 role is `o`, the non-inverting OR, so it is `bIOin' OR TIOA=Us'` with
both inputs active low, and it feeds an active-low `E'`. Counted in the wrong
sense it read "enable high on 140559 of 140559" *including when no Input existed
at all*, which looks exactly like a stuck enable. It was a correct board read
backwards.
