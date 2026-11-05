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

**Done (2026-11-01): `rtl_core/dorado_rtl_mem.sv`.** The Map, cache, Pipe,
FaultInfo, munch transfers and storage port are a module beside the
sequencer, with the C model's bookkeeping (`mem_book_task`) moved in
verbatim. The interface:

- `spec_req/spec_va`: the address a reference *this* clock would use, a
  combinational copy of exec's VA (and the IFU byte's, in the launching
  states). The section reads the Map entry, loads the cache row and reads
  all four ways' words at it, before the processor has committed.
- `req_*`: the reference, held until `accepted`. If its address is the one
  read ahead, it is booked in the accept clock; a hit is complete then
  (`done_now`, `md`). Otherwise the reads happen in the accept clock and the
  booking the clock after.
- `done`: a level from completion to the next accept. A reference that does
  not complete in its booking clock shows its result for one clock
  (`S_DONE`) before the section takes another request, so a request already
  waiting cannot hide it.
- The processor keeps the Md register file, the fault wakeup and the
  MapBuf-busy count (the C model ties it to the instruction count); the
  section reports each booking as it happens (`book_now`, `book_srn`,
  `book_push`, `book_is_map`) so the count follows the C model's
  `pipe_push`-then-`Map<-` order exactly.

The sequencer issues a reference and goes on to C_NEXT; C_FINISH waits only
if the reference has not completed; the I/O tick fires in the clock
C_FINISH retires, not the clocks it waits in (the first Galaxian run caught
that: the display got no lines). Two same-instruction orderings had to be
kept: FaultClear before the reference (`x_fcnt_before` sees the clear) and
CFlags<- before the reference (the row read ahead is patched).

| | before | after |
|---|---|---|
| Cedar rehearsal (3 M, `sim_hps`) | 4.98 | **4.30** |
| Cedar lock-step (3 M) | 4.75 | **4.04** |
| Galaxian fpga-sim (80 M) | 4.30 | **3.99** |

What remains in the rehearsal: C_FETCH 1.06, C_NEXT 1.00, C_FINISH 1.10
(0.10 of it waiting for munch transfers and misses), IFU dispatch 0.88
(C_IFU_W 0.45, C_IFU_DEC 0.26, C_IFU_AW 0.17).

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

**Done (2026-11-01), differently from the sketch above, and better: 4.30 ->
3.40.** The C model fetches each IFU byte on demand at the dispatch, as a
reference with side effects (a Pipe entry at ASRN, the line's LRU, Md,
faults on a miss), and the lock-step compares all of that; so a prefetcher
cannot simply fetch ahead. What it can do is move the *data* and the IFUM
decode ahead, and leave the references to the dispatch:

- **Peeks.** A second request port on the memory section
  (`req2`/`spec2`): the word at a VA if the line is resident, with no side
  effect at all (no booking, no Md, no Map read), served when the first
  port has nothing, complete in its accept clock (or `peek_retry` when its
  address was not read ahead: never a clock in the section's booking
  state). The prefetch buffer in the processor (`pf_*`) fetches the two
  words covering `pcf..pcf+2` this way after every dispatch, and reads the
  opcode's IFUM entry through a second IFUM port.
- **The buffer is a cache of the cache**, so it is invalidated by the
  section's snoop: any booking in the word's row but a plain hit (a miss
  changes a tag; Flush, IOStore and CFlags<- the flags; the assist a valid
  bit), and any word write (`cw_we`). A reference in flight to the row in
  the dispatch clock itself blocks the fast path for that clock
  (`pf_hazard`), since its change would come too late to see. The buffer
  restarts when PCF or InsSet moves, and is dropped when IFUM is written.
  A word the buffer holds is therefore what the dispatch's own fetch would
  return; a peek that misses is simply not resident, and the dispatch's
  real fetch fills it.
- **The dispatch in C_NEXT** (`nx_fast`): opcode, entry (and its length),
  alpha and beta from the buffer; C_IFU_DEC's decode (`ifu_decode_task`)
  and `ifu_end_task` run in that clock; the IM prediction knows the target
  from `pf_rh`. Then **the bookkeeping queue** books the C model's
  references, one per byte, in order, after the instruction's own
  reference and before the next instruction's (`ifu_bk_busy` holds the
  execute): each is a normal IFETCH through the first port, read ahead in
  the clock the previous one is taken, so they go at one per clock. Only
  when the MCR has none of its test modes on (`fdmiss`, `noref`, `dvavic`,
  `usemcrv` change what a fetch returns); the slow path (C_IFU_W, DEC, AW,
  BW) remains for everything else, including IFUTest feeds and
  breakpoints.
- Requests carry a tag the section echoes (`TAG_PROC`, `TAG_IFU`,
  `TAG_BK`), so a result is consumed by its owner and a completion is never
  hidden behind a queued request; an IFU launch while a request is still
  waiting is deferred to C_IFU_W.

- **The compare point moves.** The byte references are booked after the
  IFUJump instruction retires (the processor's next instruction is held
  until they are), so at `step_done` the Pipe, LRU and Md are not yet what
  the C model has at that instruction's end. `cmp_done` is the strobe the
  lock-step waits for: `step_done`, or the clock the last owed reference is
  booked -- never later than the next instruction's execute. The I/O tick
  stays on `step_done`. The pipelined core will need exactly this.
- Two bytes in one word are one reference: the C model makes the same
  reference twice (same Pipe entry, same LRU touch, same Md), so the second
  booking is omitted exactly. An even-aligned two-byte opcode then owes
  nothing after the dispatch.
- For the one-instruction handlers (22% of Cedar's dispatches: the next
  IFUJump is three clocks after the last), the entry is taken from IFUM's
  second port as it arrives and word1 from the section as it arrives, and
  word1's peek goes out in the clock word0's is taken.

| | Phase 1 | Phase 2 |
|---|---|---|
| Cedar rehearsal (3 M, `sim_hps`) | 4.30 | **3.51** |
| Cedar lock-step (3 M) | 4.04 | **3.22** |
| Galaxian fpga-sim (80 M) | 3.99 | **3.68** |
| dispatches from the buffer | - | 99.8% |
| Quartus ALMs | 59% | 61% (`u_mem` 8.8K, `u_cpu` 7.6K) |
| 48 MHz slack | +0.14 ns | -0.99 ns (46 MHz closes) |

The area took one detour worth recording: the first build came out at 79%,
with `u_mem` at 16.6K ALMs. Two causes, both in the pipelined read-ahead:
a row loaded in the clock a booking writes the same arrays back made
Quartus build a 96-bit bypass for all 64 rows; and `load_row` called at
four sites with four addresses was four 64:1 muxes of 96 bits. Now there
is one row load per clock at one site, from the arrays as they stood at the
start of the clock, and when the read-ahead's row is the one being booked
the row registers already hold it and are kept. 16.6K became 8.8K.

The -0.99 ns path is a false one, structural: the tag compare -> `res_tag`
-> `mem_fin` -> the fault wakeup -> the task scheduler (C_FINISH) -> `ctask`
-> the BR read -> the VA adder -> `mem_req_va` (an execute clock). The
scheduler and the execute never run in the same clock, but Quartus cannot
know it. **Cut (the following commit):** the processor consumes its
reference's result from registers only (`res_valid_r`, `res_tag_r`,
`md_r`...), one clock after a hit completes -- the hit completes in
C_NEXT, C_FINISH consumes it from registers at the top of its clock and
retires in the same clock, so nothing stalls; a pending request of the
processor's own tag means the result shown is an older one. The booking
strobe stays combinational but is applied at the *end* of the sequencer's
clock, after the execute, so nothing reads it in the clock it happens
(neither the scheduler nor the execute runs in a booking's clock anyway,
and a tick before the booking leaves the MapBuf count's final state the
same). Result: **48 MHz +0.84 ns, 62% ALMs**, and the worst paths are
MiSTer's scaler. Two false starts recorded for the next person: a
registered booking strobe made the execute wait a clock for it (3.22 ->
3.49), and gating only the compare on it let the compare run one clock
late, after the next execute (an `alu_zero` mismatch at step 23,951).

The mailbox's status beat 8 now carries a free-running 24-bit fabric
clock count above the 40-bit cycle count, and `dorado-hps` prints
"clocks each" in its 20-second report: the clocks per microinstruction on
the real board, which the rehearsal can only estimate.

Rehearsal profile: C_FETCH 1.14 (0.14 waiting: byte references 0.09, I/O
engines, the host port), C_DEC 0.13 (0.11 waiting for the I/O engines),
C_EXEC 0.10, C_NEXT 1.00, C_FINISH 1.10 (0.10 waiting for a reference:
munch transfers, misses, Map writes). The sequencer's floor is three
clocks; 0.5 is waiting, and the NEXT+FINISH merge is next.

Mistakes on the way worth keeping: C_NEXT's trailing `x_np = np` overwrote
the dispatch's next PC (the suite's Galaxian boot caught it); the peek's
"issue" decision was taken in the clocked block *after* it had cleared
`mem_req`, while the read-ahead was decided combinationally from the
register, so every peek retried (one combinational decision now feeds
both); a peek's word went out on `md` only with `done_now`, which excludes
peeks, so word1 came back as the last *booked* word -- and the first 3M
run had "passed" with the fast path never taken, so the first real
dispatch was the first test of it. The lock-step at 3M, with the
dispatch-outcome census beside it, is the loop that found all of these in
minutes each.

### Phase 2.5: the NEXT+FINISH merge (the FSM's last step)

With the memory section and the IFU beside it, every microinstruction is
three sequencer clocks: the execute (C_FETCH with the read-ahead, or
C_EXEC), C_NEXT (the next address) and C_FINISH (writes, scheduling, the
read-ahead for the next). Two of these can be one:

- **The IM read-ahead moves into the execute clock.** The prediction
  (`pred_np`) depends on the executing instruction's JCN and on the
  *previous* instruction's branch conditions -- the Dorado's own rule (HM
  4.4) -- plus Link, the dispatch register and, for an IFUJump, the
  prefetch buffer's entry; all registers at the start of the execute clock.
  The IM port is free in that clock (C_FETCH read the instruction already
  in ex_u; only Read IM / Write IM need it, and they keep the C_EXEC path).
- **C_NEXT's work and C_FINISH's run in one clock (`C_NF`).** In that
  clock the predicted next instruction has arrived from IM (`cur_u`), so
  the RM/STK read-ahead at its operand address (`fin_*`) happens as it does
  in C_FINISH today, the LC writeback goes to RM/STK through the same port
  (write-first bypass as now), the branch condition reads the register
  C_NEXTRD / the execute clock read for it, the next PC and the schedule
  are computed, and `spec_hit`/`rm_pre_ok` are set for the next execute.
- A reference still in flight (a miss) holds `C_NF` whole: nothing of it
  runs until the result is in the registers (the registered consumption
  makes that a register test), so no side effect of C_NEXT's repeats.
- The IFU slow path (C_IFU_W, DEC, AW, BW) ends in the C_FINISH work as a
  task; the fast dispatch is part of `C_NF`.

Two clocks per microinstruction on the common path: rehearsal 3.51 ->
about 2.5. At 48 MHz that is 19 M/s, **1.15x a Dorado**, within the FSM.
The risk is the merged clock's depth (next PC -> scheduler -> chosen ->
read-ahead address, beside the LC writeback); if it closes at 40 MHz
rather than 48, 16 M/s is still 0.96x.

**Done (2026-11-01), and the board measured right at last.** `C_NF` is
the merged state; the execute reads IM at its own prediction (`pred_np`
is computed for `ex_u` in the execute clock from the registers as the
execute will leave them -- Link, the dispatch register, a Cnt<- of the
same instruction -- and the *previous* instruction's branch conditions,
HM 4.4, which is also how `cpu.c` orders `next_pc` before the flag
update); `C_NF` runs `next_task()` (C_NEXT's body) then
`finish_task()` (C_FINISH's) and reads IM again at the recomputed
prediction for the read-ahead. The execute runs *before* the state case
now, so the scheduler is never in its cone. Four follow-ups, each from
the merge-miss counters the census grew (`merge:` line):

- **RM/STK branch conditions predicted in the execute clock** -- the
  condition register IS the operand register (`rm_address_fn` for both),
  so with the operand read ahead (`fexec`, `rm_pre_ok`) it is in `rm_q`
  already; unless the instruction moves StkP or the subtask (checked after
  the execute, as before). 13.9 -> 1.0 per 100 instructions take C_NEXT
  for this.
- **A dispatch in C_NF reads its first byte's address ahead** (the
  `mem_spec_*` terms had C_NEXT only): byte-reference waits 35 -> 23 per
  100.
- **The prefetch buffer keeps its words across a dispatch.** It restarted
  from scratch whenever PCF moved -- every dispatch -- so 77 peeks per 100
  instructions and no entry ready for a short handler. Now word0 is kept
  when the new PC is still in it, word1 moves down when the PC is there,
  and only the IFUM entry is re-read. Peeks 46 per 100; dispatches from
  the buffer 99.8%; IFUJumps that cannot dispatch at their execute 20 ->
  8.5 per 100 (the one-instruction handlers: the entry is addressed the
  clock after the dispatch and arrives the one after).
- **The execute overlaps the I/O engines.** Only an instruction that reads
  or strobes I/O state (`uinstr_touches_io_fn`: Input<-/Output<-,
  TIOA<-, a branch on IOAtten', a fast-I/O reference) waits for `io_busy`;
  the retire -- whose tick drives the engines -- waits for their jobs
  (`io_busy_tick`, the registered terms of `io_busy`).

| | Phase 2 | merge | +RM cond | +C_NF spec | +buffer | +I/O overlap |
|---|---|---|---|---|---|---|
| Cedar rehearsal (2 M) | 3.52 | 3.25 | 3.23 | 2.96 | 2.88 | **2.87** |
| Cedar lock-step (3 M) | 3.22 | 2.97 | 2.96 | - | **2.59** | 2.59 |
| Galaxian fpga-sim (80 M) | 3.68 | 3.12 | - | - | 2.92 | |

Where the rehearsal's 2.87 goes: 2.0 is the floor; 0.35 waiting for a
dispatch's byte references (the next item); 0.13 C_EXEC (a fetch that
could not execute from the read-ahead); 0.11 C_NF repeats (a reference in
flight: munch transfers, misses, Map writes); 0.10 + 0.10 C_NEXT/C_FINISH
(the one-instruction handlers); 0.08 I/O-engine waits; 0.04 other.

**The board, measured right.** `dorado-hps` now reads the fabric clock
against the board's 50 MHz (a Gray-coded count of `CLK_50M` crosses into
`clk_sys`; status beat 13 carries it and the PLL's `locked`), and rates
over the interval between the status reads the counts came from. The
first merge build on MiSTer14: **48.000 MHz (0.9600 x the reference, PLL
locked), 3.16 clocks per microinstruction, 15.2 M/s = 0.91x a Dorado**,
desktop live. The "37 MHz fabric clock" of the day before was the host
dividing a count read seconds earlier by the wall clock at the report --
a poll on the ARM takes seconds -- and the "10.9 M/s = 0.65x" every
build had shown since Phase 1 was the same arithmetic. The board was
never slow; the ruler was. (A 240 MHz second PLL output to "fix the VCO"
was built and refuted on the way: the IP chose the same 480 MHz VCO, and
the PLL had always been locked.)

**Timing at 48 MHz: -1.58 ns on the merge**, as the risk paragraph above
predicted. The path is the memory section's hit compare (`ms.S_IDLE` ->
`row_lookup` -> `hit` -> `res_tag`, 5.3 ns after an 8.6 ns clock
network) feeding the whole finish: scheduler -> `bnt` -> RBase -> the
`rm_pre_ok` compare, 30.4 ns against 28.8. The C_NF clock waits on
`done_now` so a hit's `task_md_ready` carries the right `cycles` and a
fault's wakeup reaches the same clock's scheduler, and that puts the tag
compare in series with the scheduler. The core runs the desktop at 48
with that violation (room temperature; the model is the slow corner);
builds at 44 and 48 of the current RTL are the next measurement, and the
structural fix is to take memory-referencing instructions through
C_NEXT/C_FINISH (consuming from registers, as before the merge) and only
the rest through C_NF -- or to register the hit in the execute clock,
which the VA adder's path may not afford.

**Timing closed at 44 MHz (2026-11-02).** C_NF consumes a reference's
result from the registers again (`mem_fin` without `done_now`, `md_r`,
`res_latency_r`), which takes the hit compare out of the scheduler's
cone at a cost of 0.16 clocks (rehearsal 2.87 -> 3.03, 3M 2.59 -> 2.75,
Galaxian 2.99). 48 MHz still misses by 1.71 ns; **44 MHz closes
(+0.32 ns, 71% ALMs)**, and on MiSTer14 it runs **14.95 M/s = 0.90x a
Dorado at 2.95 clocks, timing met**. The wait exposed a latent bug in the
RM-condition shortcut: a C_NF that waits on a reference lets the RM port
move on to the next instruction's operand, so the condition was re-read
wrong (a branch on RM odd at step 604,046); the shortcut now requires no
reference outstanding.

Mistakes worth keeping: narrowing the byte-reference gate to "touches
memory" let the RTL retire the next instruction before the lock-step's
compare point (a `StkP`/`alu_zero` mismatch at step 15 -- the RTL was one
instruction ahead), which is why that change needs a two-point compare
(processor at `step_done`, memory section at `cmp_done`) and the retire
holding for the bookings, not a narrower gate alone; and Quartus 17 rejects
default values on input ports (`input logic x = 1'b0`), which Verilator
accepts.

**The byte-booking overlap (2026-11-02).** An IFUJump dispatched from the
prefetch buffer still makes the C model's references for its bytes (one
per word, booked after the dispatch). The next instruction used to wait
for all of them; now only an execute that would interact with them waits
(`ifu_bk_gate`): one that references memory, reads Md, reads the section's
state (HM Table 11c FA=1 FB=6), or reads Id. Any other instruction executes
beside them, as the real processor runs beside its IFU (HM 6.3), and its
RETIRE waits for them, so the tick and the next dispatch stay ordered
after them. The rehearsal goes from 3.03 to 2.91 clocks (clocks waiting
for the byte references 34.6 -> 23.1 per 100 instructions); 44 MHz
still closes (+0.008 ns, 70% ALMs); on MiSTer14 it runs **15.57 M/s =
0.93x a Dorado at 2.83 clocks**, Cedar desktop live (167,657 px).
Galaxian is unchanged at 2.99 (the Alto emulator's dispatches rarely
leave bookings to overlap).

How it is checked. The lock-step compares in two parts: the processor at
`step_done`, then `rtl_settle` clocks to `cmp_done` (the bookings landed,
read from registers only) and compares the memory section. During the
settle the lock-step holds the next instruction (`hp_hold`): its I/O
answers and the host's pokes come from the C model's next step, which has
not run yet, so letting it run ahead there would compare against inputs
the RTL could not have had. The run-ahead itself is checked by the
free-running boot: a rolling hash of (cycles, PC, task) every 100,000
steps (`RTL_STEP_LOG` in `sim_cosim`) is identical to the committed core's
over all 80 M steps of the Galaxian boot.

Three things it took. **Id readers must wait**: `id_r0`/`id_r1` are
registered from the IFU state of the clock before, and the full gate had
always given them that clock after a dispatch; without it an `A<-Id`
straight after the dispatch read the previous opcode's operand (step 6
of the 3M). **The compare point is registers only**: an earlier
`cmp_done` that counted a booking completing in its accept clock compared
`Mar` one reference early. And Suite 6 failed inside `make test`
while the same boot run alone (`RTL_BOOT_ONLY`) passed: the likely cause is
a static flag in the harness (`rtl_step_seen`) left set by an earlier
suite's settle, making the boot's first step return at once. It is now
cleared at every RTL reset and Suite 6 passes in `make test`; the fix went
in together with the settle hold, so the cause is inferred, not isolated.

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

**The first pipeline: C_PIPE (2026-11-02).** Not a new file after all: the
pipeline lives in `dorado_rtl_cpu.sv` behind a `PIPELINE` parameter (default
0, so the FSM build is unchanged; `make PIPE=1 ...` and `PIPELINE=1
fpga/mister_rtl/build.sh` select it), because it hands everything it does
not take to the FSM's own states, and a fork would have had to carry all of
them. One new state, `C_PIPE`, does in one clock the finish of `x_u` (W:
`next_task` and `finish_task`, unchanged), the execute of `ex_u` (X:
`exec_task`, unchanged, after the W in program order -- cpu.c's order), the
RM/STK read of the instruction in `cur_u` (D) and the IM read of the one
after it (F). The FSM enters it from `C_NF` (`pent`) and it drains back to
`C_FETCH`, or hands the next instruction to `C_NF`, whenever something is
not its to take.

What makes it work, each found by the lock-step or a gate:

- **The execute reads its own view of the registers** (`T_x`, `Link_x`, the
  flags, `pc_p`, `cycles_x`, `sp_wx`, `membase_x`, and `ex_u`, Id count, Cnt,
  Link-read state as the clock found them), reloaded just before
  `exec_task` and checked against what the finish actually left (a
  `$display` that has never fired). Without the reloads the finish, the
  scheduler and C_NF's work sat in front of the execute and the core missed
  44 MHz by 7 ns; with them it closes with **+0.97 ns** (the FSM: +0.008),
  69% ALMs (one execute datapath, not two).
- **F needs no branch condition**: a successor that depends on one (a
  conditional jump, an FF condition, a BDispatch) hands the instruction to
  the FSM, which re-enters at its C_NF. IFUJumps are fetched for from the
  prefetch buffer's entry two clocks ahead; their finish runs alone (a
  bubble) and the dispatch is checked from registers before the handler's
  first instruction executes, so the buffer's 28-bit compare is not in the
  execute's enable.
- **Id is computed at the end of the clock** (`id_r0/id_r1` from the state
  the clock leaves), so consecutive Id readers need no gap; this also holds
  for the FSM, where it changes nothing.
- **The operand forward is registered** (`p_fwd`, the write-address compare
  made the clock before): it was the last critical path.
- **Passing conditions bubble, they do not drain** (`p_bub`): byte
  references still booking, a reference completing this clock (an Md reader
  waits a clock: the Md file is booked at the clock's top), the host's hold.
- **The I/O tick beside an execute.** cpu.c applies a tick's wakeups and the
  display word task's subtask write at the next execute, which in C_PIPE ran
  beside the tick, before its outputs; the finish applies them instead.
- **The entry must find IM holding the instruction** (`im_q_addr`): a C_NF
  that waited may have read IM elsewhere meanwhile. The FSM recovers because
  C_FETCH reads IM again; C_PIPE does not. Found by bisecting the free-running
  Galaxian boot to a five-step overlap window (`RTL_OV_WINDOW`).

How it is checked. The lock-step compares at each retire as before; after a
retire with the next instruction executed beside it, the processor is
compared from `snap_*` (taken between the finish and the execute) and the
memory section at the next retire without one. `sim_cedar` now runs the RTL's
step once the C's devices have also run (so it knows whether the host will
poke before the next step, and holds the overlap if so), and skips its
diff-based pokes after an overlapped step (the diff would see the execute's
own effects -- a reschedule cleared -- as pokes; 251 M steps in, the first
270 M run caught exactly that). `RTL_OV=1 RTL_LOCKSTEP_N=2000000` runs the
three world lock-steps of `make test` with the overlap. The free-running
boots cannot be compared step for step with the FSM: the boot server reacts
in fabric clocks, so a wakeup lands an instruction earlier or later.

| | FSM | C_PIPE |
|---|---|---|
| Cedar lock-step (3 M) | 2.75 | **2.49** |
| Galaxian fpga-sim (80 M) | 2.99 | **2.41** |
| Cedar rehearsal (2 M) | 2.91 | **2.65** |
| MiSTer14, 44 MHz | 15.57 M/s, 0.93x | **17.15 M/s = 1.03x a Dorado**, 2.57 clocks |

Where the rest goes (rehearsal census): the finish waits for its own
reference (17 per 100 instructions -- it must, for a fault's wakeup and the
Md-ready time), an IFUJump's bubble and its byte references (~23), and the
FSM still runs about a quarter of the instructions: those after a
conditional successor or an IFUJump the buffer could not predict. Next, in
order of size: IM as an even/odd pair so F can fetch through a conditional
branch (HM 4.4's late bit); the handler's first instruction beside the
IFUJump's finish (the dispatch checked from registers a clock earlier); and
the finish decoupled from a reference that cannot fault.

**Two board faults the simulations could not see (2026-11-02).** Both
MiSTers froze within minutes of using the network. (1) The mailbox's
receive fill and transmit copy hold the CPU with `ap_hold` and wait for
`cpu_between` (C_FETCH); C_PIPE counted `ap_hold` as a passing condition,
so it bubbled in place for ever and the two waited on each other. A hold
now drains the pipeline as a host operation does (`x_host`), and C_NF does
not enter C_PIPE under one. The sims missed it because their packets never
arrived while an eligible execute was in flight. (2) At 1.03x the ARM's
host loop (`dorado-hps`) is slower than the core: each `run_until` covered
the time the last one took, so the runs grew without bound, the host
stopped polling, and the guest's packets went unanswered (the status line's
"8.74x" was this). `dorado_cpu_skip_steps` advances the skipped
microinstructions' clocks in one addition, and `hps_poll` caps a catch-up at
1 M BaseBoard cycles; the host now holds 0.2-0.3 s behind.

**A third, and it was the CPU after all (2026-11-03).** `Schematic.cm`
never started on the board: the click's process entered a monitor, took a
stack-error trap inside it, and the catch path re-entered the same monitor
and deadlocked. The trap was real -- the core ran the wrong microinstruction.
A C_NF that waits (a reference in flight) can read IM elsewhere meanwhile;
`pent` refuses C_PIPE unless `im_q_addr == x_pred_np` for exactly that
reason, but the finish's read-ahead flag (`pre0`) only compared the
predictions, so `finish_task` latched the stray word as the next
instruction. Seen at ME's `IFUNext0` dispatching JZEQB (real 0174): C_NF
waited, IM read 3073, and C_FETCH executed 3073's local jump at PC 0174,
landing in JGB (0120), which popped twice. `pre0` now also requires the IM
address. Only the board's DDR timing makes that C_NF wait at that point, so
lock-step (which matched for 54.5 M microinstructions) never saw it; the
FSM core has C_NF too, hence the same symptom there. Found by tracing
process 2154's Mesa opcodes in sim_hps and C-only sim_cedar from the
restore (`HPS_OPTRACE` / `CEDAR_OPTRACE`, with StkP) -- 217,473 opcodes
agree, then the trap -- and then the sequencer clock by clock
(`HPS_FLT`, `HPS_FLT_ALL`). The earlier note blaming the host services was
wrong.

**The remaining Schematic/Moon stalls were the host, not the core
(2026-11-03/05).** With the C_NF fix, `Schematic.cm` and `Moon.cm` started
but stalled part-way, after 55-85 packets, with the cursor still moving.
Three commits:

- **A stall report** (d9926af49). A host-port operation the core does not
  answer within 2^26 clocks writes `dbg_stall` (CPU and memory-section
  states, booking/request flags, task, PC) and the storage DDR client's
  state to status beats 14-15. It repeats every 2^26 clocks while the wait
  lasts, and dorado-hps prints it. If no report arrives, the DDR client is
  not serving the mailbox's aux port.
- **Accounting between the ARM and the core** (ac33c0c47).
  - The host posts NWW bits as edges, the bits set since its last push,
    instead of OR-ing back its stale copy.
  - dorado_rtl_io latches an `rx_load` that arrives while the receiver is
    skipping, and counts the loads it applies. Beat 11 carries that count
    in the same 32-bit half as `rx_drained`, so a fill can never be paired
    with a stale drained bit.
  - The host re-reads each result beat to catch a torn 64-bit read, and
    reports `RX LOST` when the load counts disagree.
  - `DORADO_HPS_RXCHECK=1` checks each received packet against the IOCB
    Cedar armed for it.
  - A periodic accounting line lists every counter. Across about 20 board
    runs every counter stayed at zero: nothing was being dropped.
- **The cause: a race in the PDI disk bridge** (9d0239812, `machine.c
  machine_germ_complete_disk_iocb`).
  - `DiskHeadDorado.mesa` Initiate appends an IOCB in two steps:
    `IF csb.head # nil THEN csb.tail.next _ new`, then
    `IF csb.head = nil AND new.seal = sealValid THEN csb.head _ new`.
    That is safe only because the disk microcode reads `iocb.next` and
    stores `csb.head` indivisibly.
  - The bridge read `next` when a completion began and stored it into
    `csb.head` at the end. In one process that is indivisible too. On the
    MiSTer the bridge runs on the ARM while Cedar keeps executing on the
    FPGA, so Cedar could append in between. It saw head # nil, left the
    IOCB to the controller, and the bridge's store then dropped it. Its
    client waited forever, and the file system with it.
  - Seen in the Pilot PDA dump at the stall: CSB head (VA 177520) = 0 while
    IOCB 1275B's `next` = 1135B, sealed.
  - The completion now re-reads `next` after storing head and takes a
    non-nil value: the same store the guest's own second step would have
    made. On MiSTer14 the race fired 3-12 times a run and was repaired each
    time, and both pictures painted. The C emulator never interleaves like
    this, so its behaviour is unchanged.

Debugging hooks in dorado-hps, all used for this:

- `SIGUSR2` syncs RM and dumps the Pilot PDA (the process table), with
  `DORADO_PDA_DUMP_ALL=1` for every PSB. That is slow: it blocks the host
  loop.
- `/tmp/hps-poke` takes "va value" lines (octal), applied once.
- `/tmp/hps-dump-va` takes "va count" lines, BOTH octal. A count of `8` is
  not octal and dumps nothing. The file is not removed after use, so a big
  dump left behind repeats every poll; delete it.
- `DORADO_DISK_IOCB_TRACE=1` logs every IOCB completion, and each race the
  bridge repairs.

**OPEN: a second, separate core hang.** Two of the four verification runs
(at about 30 s and about 128 s) ended differently:

- a host-port READ went unanswered;
- no stall report arrived;
- every status beat froze, including beat 13's free-running CLK_50M count.

The mailbox writes all of those through `dorado_rtl_storage_ddr`'s aux
port, which is served only from `S_IDLE`. So the DDR client is stuck in a
wait: a lost `DOUT_READY`, or `BUSY` held. A core reload clears it. This is
probably also the idle freeze d9926af49 was written for.

The stall report cannot catch it, because the report also goes through DDR.
The next step is a watchdog inside `dorado_rtl_storage_ddr` that records
which wait has run too long and reports it by a path that does not need
DDR. One such path is the HDMI screen, which MiSTerKVMS can read.

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
