# rtl_core speed: why 0.59x, and the plan to 1x-3x

Date: 2026-11-01. State at writing: commit `6c325d125`, Cedar on MiSTer14 at
**4.98 fabric clocks per microinstruction at 48 MHz = 9.85 M/s = 0.59x** a
Dorado. This document looks at the gap with fresh eyes, from the Hardware
Manual's own description of the machine's timing, and plans the way across it.

## 1. The original machine's clock

HM 2.3 "Timing" (doc p.6):

- **clock**: 30 ns nominal. Footnote 1: "We actually operate with a clock speed
  of 32 ns ... production machines typically become unreliable at about a
  29 ns clock period."
- **cycle**: two clocks, 60 ns (64 ns as run). "Dorado initiates a new
  instruction every cycle."
- So the real rate is 15.6-16.7 M microinstructions/s. The project's yardstick
  for 1x is 16.67 M/s (the nominal 60 ns); everything below uses that.

The part that matters for us is the next sentence: **"Instructions are
pipelined, requiring a total of three cycles for execution."** Figure 7:

| when | what |
|---|---|
| t-2 | next instruction address determined, IM fetch begins |
| t0 | instruction loaded into MIR |
| t0-t1 | RM or STK read, loaded into a register at t1 |
| t1-t3 | ALU |
| t3 | result loaded into a register; RM written in the final clock |

"Since a new instruction begins before the previous one finishes, paths exist
to bypass the register being written if the following instruction specifies it
as a source."

Two more sections say the same thing about the other two units:

- **Memory, HM 5.4**: "Address and cache data sections can handle one reference
  per cycle ... barring io activity and cache misses, the processor can make a
  fetch or store reference every cycle and never be held." A reference enters
  the address section at t1 and the processor carries on; "an immediate <-Md is
  held in the cycle after a fetch or store" (one cycle), the deferred <-Md two
  instructions later is free.
- **IFU, HM 6.3**: "If a long stream of regular one-byte opcodes is being
  executed by the processor at the fastest possible rate (one
  instruction/opcode) ... then it will always have the next opcode ready for
  IFUJump." Two-byte opcodes with IX and no N: 25% NotReady; odd-aligned, 40%.

And the branch conditions (HM 4.4, Table 13): "ALU=0 and ALU<0 are the results
of the **last** ALU operation executed by the current task ... loaded into the
RAM at t3." A conditional branch tests the previous instruction's result, and
the low address bit is developed late ("the bank-select toggling trick, which
allows branch conditions to be developed very late, is valuable"). PARC
designed the control section so the next-address decision does not wait for the
current ALU.

**The consequence that this plan rests on: every microprogram we run (Initial,
AEmu, Cedar/Mesa, DSemu, Lisp) was scheduled by hand for exactly this pipeline.**
Deferred <-Md, branch-on-previous-result, IFUJump at the handler's last
instruction, the one-cycle register-load rules in Table 4: the microcode already
avoids the hazards. A core that implements the manual's pipeline gets the
manual's throughput from the microcode's own scheduling, without any speculation
of ours.

## 2. What rtl_core does instead

`rtl_core/dorado_rtl_cpu.sv` is a transliteration of the C emulator's
`execute_uinstr`, which is sequential by nature: fetch IM, decode, read RM,
execute, compute the next PC, write back, schedule. The RTL is one big always
block with a state register, and every block-RAM read is its own state because
an FPGA block RAM is synchronous: address at one edge, data at the next. The
Dorado's ECL RAMs (F10415A, MB7071H, 10-15 ns access) were asynchronous, which
let PARC read RM in half a cycle; and PARC *still* pipelined. We have slower
RAMs and did not.

So the per-microinstruction time is the **sum** of the latencies where the
Dorado pays the **max**. The measured profile (`sim_hps`, `HPS_HIST=1`, Cedar,
4.98 clocks):

| part of the sequencer | clocks per microinstruction | the Dorado |
|---|---|---|
| C_FETCH (IM read; with read-ahead also the execute) | 1.07 | overlapped |
| C_NEXT (next PC, IM read at the prediction) | 1.00 | overlapped |
| C_FINISH (write back, schedule, RM read-ahead) | 1.00 | overlapped |
| memory reference: M_MAPA 0.16 + M_MAPV 0.59 | 0.75 | runs beside the next instruction |
| IFU dispatch: OPW 0.26 + DEC 0.26 + AW 0.17 | 0.69 | prefetched before IFUJump |
| fallbacks (DEC/EXEC/NEXTRD when the read-ahead misses) | 0.27 | - |
| stores, I/O, writebacks, fills | 0.20 | mostly overlapped |

Three structural facts, in order of cost:

1. **The instruction itself is three serial clocks** (FETCH, NEXT, FINISH).
   The read-ahead work of the last session turned the top of the FSM into a
   de-facto three-stage pipeline for the common case, but the three clocks are
   still serial per instruction: the next instruction's IM read cannot start
   until this one's next-PC is known, and its RM read until the write-back is
   done. The Dorado overlaps all three.
2. **A memory reference is a sub-state-machine that stops the instruction.**
   MAPA (Map read) then MAPV (entry arrives, tag compare, cache data read),
   2-4 clocks during which nothing else happens, for ~60% of instructions.
   The Dorado's address section takes the reference at t1 and the processor
   proceeds; the result is wanted, at the earliest, one instruction later.
3. **The IFU decodes the next opcode when IFUJump asks for it**, not before:
   OPW (cache word), DEC (IFUM), AW (alpha). 26% of instructions are IFUJumps
   and each pays about 2.7 clocks. The Dorado's IFU is a six-stage prefetch
   pipeline that has had the handler's whole duration (about four
   microinstructions on average here) to get ready.

Secondary:

4. The fabric clock is 48 MHz because the one-always-block style puts the whole
   decode, operand select, ALU, shifter and Pd mask in one clock in the
   read-ahead path, and the Map RAM write-enable to `m_wbway` path misses 50 MHz
   by 0.016 ns. This design will not reach 67 MHz, which is what 1x would need
   at 4 clocks per microinstruction.
5. Everything -- CPU, memory engine, IFU, host assist -- shares one state
   register, so none of them can run while another is in a state.

Each of the last session's gains (8.15 -> 4.98) was a special case bolted onto
the FSM: skip this state when that condition holds. Those are getting more
expensive each time (two of them doubled the datapath until the call sites were
merged) and they converge on about 3 clocks, i.e. 1x at a clock this design
cannot close.

## 3. What a faithful pipeline would give

A core that implements HM 2.3 literally, with one fabric clock per Dorado
cycle:

| stage | clock | does |
|---|---|---|
| F | t-2..t0 | TNIA select (JCN, previous-instruction branch conditions, BNT task switch, IFU dispatch address); IM read |
| R | t0..t1 | RM/STK read (block RAM); B source select; task-specific registers; Md for <-Md |
| X | t1..t3 | ALU and shifter; Mar and the reference issued to the memory section; FF side effects |
| W | t3..t4 | RM/STK, T, Q, flags written; bypass to R |

plus the memory section as its own two-stage pipeline (address: Map read; cache
data: tag compare and data read, Md at the end; the existing fill/writeback
engine behind it for misses), and the IFU as its own prefetcher.

Throughput: one microinstruction per clock on hits, less the stalls the real
machine also takes (immediate <-Md: one clock; IFU NotReady; misses; plus ours:
I/O engine busy, host-port assists). Estimate 0.8-0.9 instructions per clock,
to be measured in Phase 0 below. Then the fabric clock sets the speed and
nothing else does:

| fabric clock | speed |
|---|---|
| 16.7 MHz | 1x |
| 33 MHz | about 1.7x |
| 50 MHz | about 2.5x |

The pipeline *decouples speed from timing closure*: if a stage does not close at
50 MHz, 33 MHz is still 1.7x. That is the opposite of the FSM, where every gain
needs a faster clock.

**Why the branch rule makes this feasible.** F needs instruction i's branch
condition from instruction i-1's ALU, whose X stage runs during F(i+1) -- the
same clock. Doing it PARC's way: keep IM as an even/odd pair of block RAMs,
read both words of the target pair every clock, and let the condition bit
select between them at the end of the clock. The late path is then ALU ->
zero/sign detect -> a 2:1 mux on the MIR input, roughly the depth of the
current read-ahead path, which closes at 48 MHz today. If it does not close at
50, the fallback is the clock, not the design.

**Why the memory rule makes this feasible.** The C emulator's semantics are
sequential: reference i's bookkeeping is visible to reference i+1 and Md is
readable in i+1. In the two-stage memory section, back-to-back references
pipeline (address section for i+1 while cache section for i), and an <-Md in
i+1 stalls one clock, which is exactly HM 5.4's rule, so the microcode rarely
does it. The C emulator counts that hold? No: its hold model
(`task_md_ready`) is for misses. So the one-clock stall is a fabric stall and
does not advance `cycles`; the lock-step harness compares at retirement and
never sees it. This distinction (modeled cycles versus fabric clocks) already
exists in the FSM and stays.

**Verification stays the same.** The lock-step harness (`sim_cedar`,
`cosim_sync.h`) compares architectural state after each retired
microinstruction on a `step_done` strobe. A pipeline retires in order, one per
clock, with the same strobe. `make -C rtl_core test`, the Cedar 270 M
lock-step and the Galaxian fpga-sim are the gates, unchanged.

## 4. Plan

Each phase is a verified milestone on its own and is useful even if the next
one never happens. Phases 1 and 2 are modules the FSM uses first, so they are
verified in lock-step long before the pipeline exists, and they take the FSM
to about 1x by themselves.

### Phase 0: measure the pipeline before building it (about a day)

Add a census to `sim_hps` (`HPS_HIST` already profiles states): per retired
microinstruction count references, misses, <-Md immediately after a
reference (the one-clock hold), IFUJumps and how many clocks the IFU would
have had to prefetch (handler length), opcode lengths, task switches, I/O-busy
waits, host assists. Output: the predicted instructions per clock of the
pipeline, with the stall budget itemised. Also the full-run figure over the
270 M listing run. This is what the 0.8-0.9 above becomes a measurement of.

Gate: a number, and a decision on the fabric clock to aim for.

**Done (2026-11-01).** `HPS_CENSUS`, `CEDAR_CENSUS`, `FPGA_CENSUS` print the
census (`rtl_core/sim_census.h`; the counters are `cen_*` in the CPU). Per
100 retired microinstructions:

| | Cedar rehearsal (3 M) | Cedar lock-step (3 M) | Galaxian (80 M) |
|---|---|---|---|
| Fetch/Store/LongFetch | 9.1 | 9.4 | 7.6 |
| other references (IO, Map, DummyRef...) | 6.5 | 6.4 | 6.3 |
| misses, per 1000 references | 2.4 | 2.1 | 3.6 |
| IFU word fetches | 43.7 | 45.6 | 17.9 |
| <-Md, deferred, the instruction after the reference | 8.8 | 9.2 | 4.4 |
| <-Md, immediate, the instruction after | 0.1 | 0.1 | 1.6 |
| IFUJumps | 26.3 | 27.4 | 9.0 |
| ...with a 1-instruction handler | 22% | 22% | 0% |
| opcodes 2 bytes / at an odd byte | 66% / 55% | 66% / 55% | 100% / 0% |
| PCF<-B restarts | 2.9 | 3.0 | 2.9 |
| IFU NotReady traps (the modeled 5-instruction warmup) | 8.6 | 9.1 | 8.5 |
| conditional branches | 10.0 | 9.6 | 5.4 |
| task switches | 1.6 | 0.4 | 0.8 |
| clocks waiting for the I/O engines | 11.7 | 0 | 2.0 |
| **predicted clocks per instruction, one-per-clock pipeline** | **1.12-1.36** | **1.00-1.25** | **1.05-1.11** |
| today (FSM) | 4.98 | 4.75 | 4.30 |

Three things the numbers decide:

- **The memory section must have the Dorado's timing, not a 2-clock one.**
  9% of Cedar's instructions are a deferred <-Md (T<-Md, RM<-Md) in the very
  instruction after the Fetch, which HM 5.4 makes free. A memory that
  delivers Md two clocks after issue would charge each of them a clock
  (0.09 per instruction); one that compares tags and reads all four ways in
  the clock after issue charges nothing. So: cache data as four way-RAMs
  read in parallel at the row and word from the exec clock's VA, tags
  compared in the next clock, way select into Md at its end.
- **The IFU is the uncertain term** (0 to 0.15 on Cedar): a fifth of
  Cedar's opcodes are handled in one microinstruction, two thirds are two
  bytes, and over half start at an odd byte. The prefetcher needs its own
  cache port and must run two to three opcodes ahead.
- **The I/O engines hold the CPU 0.12 clocks per instruction** on the
  rehearsal (the display copy engine and the mouse/keyboard path). That is
  outside the pipeline and is the next thing to make non-blocking after it.

The range, 1.0-1.36, puts the pipeline at **2.2-3x at 50 MHz** and
**1.5-2x at 33 MHz**.

### Phase 1: the memory section as its own module (1-2 weeks)

`dorado_rtl_mem.sv`: the address section (Map read, MAPA's work), the cache
data section (tag compare, bookkeeping, data read: MAPV/BOOK/RD's work) and
the existing fill/writeback/storage engine, with a request port (kind, VA, data,
task) and a per-task Md/ready result, exactly `task_md_ready`'s semantics.
Lifted from the FSM's M_* states and the tasks they call (`mem_row_task`,
`mem_book_task`), which are already written as tasks.

Attach it to the FSM first: the instruction issues the reference and goes on to
C_NEXT without waiting (as `x_pref_pending` already does for PreFetch); an
<-Md in the next instruction waits for ready. Expected: MAPA and MAPV leave the
serial path, about -0.7 clocks (4.98 -> ~4.3).

Gate: `make test`, Cedar lock-step 3 M and 270 M, Galaxian fpga-sim, Quartus
timing and area.

### Phase 2: the IFU as its own prefetcher (1-2 weeks)

`dorado_rtl_ifu.sv`: driven by PCF, fetches instruction words through the
memory module's IFU port (the real machine shares Mar: HM 6.3 "whenever the
processor is making a reference, the IFU cannot use Mar"; we give it a second
port into the cache data section, which costs nothing on an FPGA and removes a
class of NotReady), decodes with IFUM one opcode ahead, holds opcode, alpha and
beta, and presents ready / entry address / operands (`id_r0`, `id_r1` become
its outputs). The FSM's IFU states collapse to one; IFUJump consumes. The IFU
state the checkpoint carries (PCF, the byte queue, idcnt) maps onto the
module's registers.

Expected: about -0.6 clocks (-> ~3.7). Together with Phase 1 the FSM is at
about 3.7 clocks, 0.72x at 50 MHz; with the NEXT+FINISH merge from the 8.5
handoff, about 3.0, i.e. 1x at 50 MHz. That is the FSM's ceiling, and a
respectable fallback if Phase 3 is not pursued.

Gate: as Phase 1.

### Phase 3: the pipelined processor (3-5 weeks)

`dorado_rtl_cpu_pipe.sv`, selected by a top-level parameter so the FSM core
remains the oracle and the fallback. Stages F/R/X/W as in section 3, one
`always_ff` per stage, explicit stage registers, bypass W->R for RM/STK, T, Q,
Cnt, StkP, RBase, MemBase, Link, TIOA (HM Table 4 gives, per register, the
load time that decides which bypass each needs). IM as an even/odd block-RAM
pair for the late branch bit. Task switch: BNT registered, next-address source
is TPC[BNT] in F, zero cost, as hardware. Hold: X stalls with a bubble; a
pending task switch takes the held instruction's address into TPC, so "task
switching is invisible to hold" (HM 5.4) is literal. Slow paths (Read IM,
Write IM, TPC, host assists, I/O engines busy, faults): the pipeline drains and
runs them one at a time; they are rare and already written.

Order of bring-up, each step in lock-step against the C emulator:

1. Straight-line microcode, no memory, no IFU, no tasking: Initial's first
   instructions, the ALU test vectors.
2. Branches and the late bit; task switching (the Initial boot, `make test`).
3. Memory through the Phase 1 module; Galaxian.
4. IFU through the Phase 2 module; Cedar 3 M, then 270 M.
5. Quartus: timing at 33 MHz first (about 1.7x), then 50.

Gate: all existing gates; Cedar on MiSTer14 with the OSD speed reporting the
measured rate.

### Phase 4: the speed setting and real time (a few days)

An OSD choice of 1x / 2x / 3x / max. Implement it as a retirement throttle
(one token per N fabric clocks; retire only with a token), so the chosen speed
is exact on average whatever the clock and the instruction mix. Two cadences
have to move from `cycles` to real time when running above 1x, or the guest's
clocks run fast: the display controller's field timing and the junk task's
pendulum (Junk.mc's 32 us, HM Table 24 for the terminal). In lock-step mode
they stay on `cycles`, because the C emulator is the oracle at 1x; the
deployed core switches them to a fabric-clock base. Ethernet and the host port
are already paced by the ARM.

Storage: a miss costs the real machine 28 cycles (1.7 us); the DDR3 path on
MiSTer takes four single-beat reads and is about that today. Above 2x it will
show; `dorado_rtl_storage_ddr.sv` notes that bursts were never retested after
the shifted-beat bug, and that is the fix when it does.

## 5. What not to do

- Do not keep adding states-skipped-on-condition to the FSM. Each is cheaper
  to write than the last and worth less, and two of them doubled the datapath.
- Do not make the fabric clock the lever. 50 MHz is this design's practical
  ceiling and 1x needs 67 at the FSM's best.
- Do not speculate beyond what the manual gives. The read-ahead in the FSM
  predicts the next PC; the pipeline does not need to, because the Dorado's
  own rule (branch on the previous result, late bit) is a fact of the ISA the
  microcode was written for.

## 6. Summary

The Dorado ran at 16.67 M microinstructions/s on 1979 ECL because it overlapped
three instructions, ran its memory and IFU beside the processor, and scheduled
its microcode for that. rtl_core runs the same microcode one phase at a time
and pays every block-RAM read in sequence: 4.98 clocks at 48 MHz, 0.59x. The
plan is to build the manual's pipeline (sections 2.3, 5.4, 6.3), in three
verifiable modules, keeping the FSM as the oracle: Phases 1-2 alone take the
FSM to about 1x; Phase 3 makes speed a function of the fabric clock, 1x at
16.7 MHz and about 2.5x at 50, with an OSD setting choosing 1x/2x/3x under
that.
