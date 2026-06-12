# Continuation handoff — Alto-on-Dorado boot bring-up (2026-06-11, session 2)

Read this first if you are a fresh Claude session resuming the Dorado
Alto-emulator boot work. Project-wide context is in `/CLAUDE.md` and
`dorado/CLAUDE.md`. This file supersedes the earlier 2026-06-11 version:
every issue in it is FIXED and committed. The Stage-2 Alto Ethernet
software boot now completes end to end and NetExec runs; the open
frontier is NetExec's runtime behavior (display list, heavy fault-task
activity).

## One-line status

The full chain works: BaseBoard -> Initial -> Ethernet microcode boot ->
AEmu world -> InitMap -> ABoot -> EBoot -> breath-of-life -> Taft
EtherBoot loader -> Mayday -> **the complete NETEXEC.BOOT transfers over
EFTP in a single attempt** (one Mayday, 87 data packets + End + dally,
`pos=21820/21820`) -> the loader jumps into NetExec -> **NetExec runs**
(installs a DASTART display list, takes vertical field interrupts, does
EWait ether activity, manipulates its DCB over time). The framebuffer is
still blank: NetExec never builds a non-zero-width text DCB, and in the
latest run the fault task (17) consumes 25 M cycles and NetExec clears
DASTART — that is the open question.

## One-command repro

```
cd dorado && make build/test_cpu build/mb2eb && \
./build/mb2eb '../chm/dorado/AEmu.mb!2' /tmp/aemu_only.eb 01076 && \
DORADO_ONLY_FULLBOOT=1 DORADO_NO_DISK=1 \
DORADO_STORAGE_MODULES=1 DORADO_ETH_BOOT_110=/tmp/aemu_only.eb \
DORADO_ALTO_BOOT_ETHERNET=1 \
DORADO_ETH_EFTP_BOOT='../chm/bootfiles/NETEXEC.BOOT!8' \
DORADO_BOOT_BUDGET=400000000 ./build/test_cpu 2>&1 | \
grep -E 'Stage-2|task cycles|Alto display|Alto DCB'
```

Expected: `eftp_requests=1 ... state=0 seq=87 max_seq=87 pos=21820/21820`
(the whole boot file transferred), `bol=1`, an `Alto display:` line
showing DASTART/WW/ACTIVE and a DCB chain walk. Note
`DORADO_NO_DISPLAY_WAKE` is NO LONGER set — the display tasks run and
the boot still completes.

## What was fixed this session (all committed + pushed)

1. **Per-task BDispatch (THE big one, commit 10d6d0a).** The B-dispatch
   latch was a single global; HM §4.4 p32: the B dispatches load Link
   from B and OR Link bits into TNIA "during the next instruction for
   the task. Since Link is task-specific, this works correctly across
   task switching." With the global latch, the interleaved EIT consumed
   the emulator's dispatch, so AEmu's Nova skip tables (BigBDispatch ->
   LSKP/LNSK exits) broke whenever the EIT was busy: the EtherBoot
   loader's `sz 0 0` / `se 3 0` never skipped, looping
   boot3->boot4->boot6->boot3 forever, wiping every InDone post via
   boot3's re-arm. Fix: `task_dispatch[16]` in dorado_cpu, the dispatch
   functions also load Link per the manual, and LdTPC<- clears the
   target task's pending dispatch (HM p32 note).
2. **EFTP sender retransmission (same commit).** The lock-step fake
   server resends the unacked packet after 3 M wakeup ticks (EFTPSPEC
   ~1 s). The loader's protocol depends on sender resend to recover
   lost posts. 1 M ticks is TOO SHORT — it lands mid-Ack and breaks
   the exchange; keep >= 2x the loader's ~650 K-cycle process+Ack
   window.
3. **Boot shims released after use (a1a369a).** The forced BS(+Quote)
   keyboard chord is released (all-up) once EBoot (AEmu real 0o2006) is
   entered; breath-of-life rebroadcast stops once `eftp_max_seq > 0`.
   NetExec now sees a clean keyboard and a quiet wire.
4. **DispM false-positive (31fa85f).** The boot-button serial-bit jam
   applied to ANY TIOA TStatus read, so DisplayAux.mc's
   DisplayInitConfig ("Nonzero => DispM board installed") saw a phantom
   DispM and assigned the terminal task to AHT (task 4), which never
   gets scanline wakeups. The jam now applies only to DHT/AHT
   terminal-task reads. The THT now runs on task 3: per-field
   DASTART/DCB fetches, cursor bitmap reads, vertical field interrupts
   (NWW OR mask + Reschedule), ~940 K NLCB writes per boot.
5. **IFUReset enables the junk timer (9af147d).** HM p67: IFUReset ==
   IFUTest<-0, and junk wakeups occur when IFUTest.15 = 0. We had it
   disabling the timer, so ABoot's IFUReset silenced the RTClock
   forever (VM 430 ticked 4 times in 385 M cycles; NetExec sat in
   timed startup waits). Also: tick period corrected 1000 -> 533
   cycles (32 us at the 60 ns microcycle; Junk.mc's RTCDelta math
   assumes exactly 32 us per wakeup).

## FIRST PIXELS (2026-06-11, session 3)

The virtual screen renders: the DispY NLCB cursor path is implemented
in src/display.c (register select in the top 4 bits of each NLCB
output; CursorLo = the per-scanline marker, CursorHi closes a cursor
line and draws the 16-bit row; the per-field CursorX rides an
IM-derived constant whose tag decodes as register 13B, screen x =
2063 - value, calibrated against the EtherBoot loader's cursorX of
531B). /tmp/dorado_boot_display.pgm now shows the boot loader's
cursor sprite (the Ack-packet-as-bitmap PARC trick) at x=345 plus
NetExec-era cursor rows. Vertical placement is approximate (the
line counter tracks CursorLo writes since VSync; TopBorder biasing
not yet calibrated). The probe prints "Display cursor:
rows_drawn/last_x_raw/line/field"; DORADO_NLCB_TRACE=1 dumps decoded
NLCB writes.

## Open questions (the active work)

0a. **NetExec auto-runs its EtherBoot command.** NetExec1.bcpl's
   MyEtherBoot ends with DisableInterrupts(); StartIO(3); copy the
   boot loader to VM 1; goto 6 - and the observed window-opening loop
   at VM 0o1747 (poll cell, SIO, jsr 0o2014) plus the EFTP machinery
   (OpenEFTPSoc/ReceiveEFTPBlock with 1 s/10 s timeouts at NetExec1
   lines 268-273) are boot-fetch machinery. NetExec is likely
   auto-dispatching a boot (keys/BFN-derived) rather than sitting at
   a prompt; the wire is silent because its sends stall on the dead
   interrupt path below.

0a2. **The parked context is FeedEther; the full causal chain.**
   (Session-3 final state.) The context ring is live (CtxRunning
   valid, 8 contexts walked by the probe) and the RUNNING context has
   CTXStack=0 = first resume, never Blocked: that is FeedEther
   (PupAlEthb.bcpl), the first context on ftpCtxQ. Its body:
   CauseInterrupt to kick input arming, the tx-timeout branch
   (DisableInterrupts; StartIO(reset); @ePLoc = 3 lshift 8;
   EnableInterrupts), then Dismiss(4) repeat. The last EIR (92198761)
   followed by the last DIR (92198965) is an Enable-of-pass-N then
   Disable-of-pass-N+1 whose matching Enable never came - the flow
   died inside that bracket or in Dismiss. Dismiss's expiry rides on
   AltoTimer time, which advances off the 60 Hz vertical interrupt -
   circular once interrupts latch disabled. Title/Command never get
   their first resume, hence no banner DCBs.

   EXPERIMENT (committed, env-gated): DORADO_FORCE_EIR_AT=<cycle>
   clears NWW bit 0 (RM[023]) once at the given cycle. Result at
   93 M: interrupt fabric comes alive - 1496 deliveries (was 2),
   EIR/DIR cycling (1183/1181), cursor repainted continuously
   (rows_drawn 657 -> 3712) - but BRI executes 1.87 M times
   (delivery/dismiss storm; PCLOC re-delivery loop suspected) and the
   banner still does not appear. Next probes: (1) why the BRI count
   is 1000x the delivery count (check our BRI/PCLOC semantics:
   BRI fetches WW then PCLOC with the T_ MD, Fetch_ T at-issue
   pattern - verify the second fetch's Md routing); (2) catch the
   exact death between FeedEther's Disable/Enable bracket with
   IFUDISP gates at 92198965-92200000; (3) the Interrupt package's
   IntEnt saves/restores the Nova CARRY through MOVL/MOVZR around
   handlers - verify our A-Group carry for those forms (a corrupted
   carry in the interrupted mainline would derail comparisons).

0b. **NetExec stops opening interrupt windows at ~92.2 M cycles.**
   (Newest finding, post dispatch-RBase fix.) NetExec's idle loop
   polls cells (VM 0o1747/0o1755 poll loops, RCLK deadline compares
   at 0o2330, dispatcher at 0o3214) and opens interrupt-delivery
   windows via an `EIR; DIR` pair in a subroutine at VM 0o2014-0o2022.
   `DORADO_PC_COUNT="3100,3101,3102,3113,756,314,307"` shows DIR=49,
   EIR=51, BRI=2 executions ALL before 92.2 M, then never again -
   while AEmuReschedule traps keep firing per display field (1264
   traps, 1262 take RestartIFU because NWW has bit0 set = disabled).
   1262 vertical interrupts go undelivered; NetExec's display/queue
   machinery (which syncs DCB swaps to the vertical interrupt) never
   completes, so the text DCB never appears. The delivery mechanism
   itself is verified working (EIR's RescheduleNow trap delivered
   interrupts at 91.8 M, handler ran, vectors at INTVEC are
   installed). A 1.5 G-cycle run (23+ virtual seconds, RTC430
   reaching 0o1121) shows NO further progress: the post-92.2 M state
   is a quiescent polling dispatcher (VM 0o3214 loop + JSRII @M[1113]
   = 0o2330, the RCLK deadline-compare sub) with NOTHING queued -
   M[0o3315] = 0 (no timer armed; M[0o3316] = 0o430 is a stale
   deadline), @M[0o3344] = 0 (event queue empty), NWW = 0o110402
   (disabled + pending vertical bits). NetExec's keyboard/event
   inflow rides on the vertical-interrupt handler (delivered twice,
   at 91.8 M only), and its last critical section (DIR at 92198965,
   VM 0o2020 inside the EIR;DIR sample-and-update sub at 0o2014-22)
   never re-enables. RESOLVED TO A TIGHT TARGET via the
   BCPL sources (now in chm/altosource/netexec.dm!4_/ and
   contextsources.dm!1_/): NetExec.bcpl's AfterJunta builds exactly
   our observed spacer DCB (height=42), enqueues six coroutine
   contexts (Title, Command, GetTime, GetDir, GetName, WatchDog) on
   ftpCtxQ, and loops CallContextList. Title() paints the banner
   display streams on its FIRST resume after an initial Block() - and
   it never resumes. Context.asm (the OS-less Context package) shows
   the switch mechanics: resume = `jsr @1 2` (JSR indirect through
   CTXStack frame offset 1, with an inline data word after the jsr);
   Block() does `inc 3 3; sta 3 1 2` (save return+1 in the frame),
   saves the stack pointer in CTXStack, and chains to CTXNext. The
   observed "dispatcher" loop (VM 0o3214 + the RCLK compare at
   0o2330) is ONE context spinning in `Block() repeatuntil
   TimerHasExpired(...)` - its Block() is returning without cycling
   the other contexts. NEXT: unit-test our emulation of the exact
   instruction sequence in Context.asm (plain Nova `jsr @disp,2`
   indirect-indexed EA, `inc 3 3`, frame stores via sta n,2; check
   the JSR return-address convention against the inline-data word
   after `jsr @1 2`) and trace one Block() round at the microcode
   level. The statics CtxRunning/CtxCaller (.srel cells) likely map
   to the dumped cells around M[0o1113]. ContrAlto ground truth
   remains the fallback.


1. **Fault task storm + DASTART cleared.** With timing fixed, NetExec
   progresses further and the FLT task (17) burns 25 M cycles;
   DASTART reads 0 at probe end (earlier runs left a zero-width spacer
   DCB at 0o176762 whose SLC evolved 42 -> 1). DIAGNOSED ONE LEVEL
   DOWN: the FLT loops 1.48 M times at AEmu FAULTTASK/GETEMUFAULTPC
   (real 0o4151/0o4040/0o4133/0o4134/0o4141) with mar=0o177756 — the
   Alto-II XM bank-register page (bank reg for task N at 0o177740+N).
   NetExec probes extended memory through those registers; AEmu
   emulates them via the XM write-protect fault path (XMFaultTask.mc),
   and our fault/restart plumbing re-faults the same access forever
   instead of completing the emulated store/read. The emulation flow
   (XMFaultTask.mc): the FLT saves FaultVal from DBuf and the
   emulator's TPC, restarts the emulator at Fault0; Fault0 reads the
   faulting VA from the Pipe, then XMBStoreOnly clears the page-0o377
   write protect via `Map_ T, MapBuf_ FaultMapVal` + WaitForMapBuf,
   performs the store itself, restores WP, and resumes at the NEXT
   instruction via AEmuReschedule. A persistent loop therefore means
   the WP-clear or the emulated store is not taking effect in our Map
   model (the emulation's own Store_ re-faults), or the Pipe VA the
   FLT reads is wrong. First check: does our `Map<-`/`MapBuf<-` write
   path update the map entry's WP bit, and does `B<-Pipe0/Pipe1` at
   the FLT's ProcSRN return the faulting VA?
2. **No text DCB / blank framebuffer.** NetExec never created a
   non-zero-width DCB. May resolve once (1) is fixed. Independent
   quick win: the cursor bitmap at VM 0o431-0o446 IS populated and the
   THT feeds cursor X/Y + data words to the NLCB every field —
   modeling DDC cursor rendering into the framebuffer would give the
   first visible pixels.
3. **EOT duplicate transmissions.** Every Ack is transmitted ~13 times
   (OutDone storm; LoadOverflow 0o1777 posts seen historically). The
   transfer converges anyway (dup Acks just resend the current packet
   and the loader re-acks). Root cause is in the EOT output path / our
   always-empty bus-register wakeup model. Cosmetic for now.

## Debug tooling added this session

- `DORADO_STORE_TRACE_VA="lo,hi"` (octal) now prints the issuing
  microcode PC, IFU PCX, BR31, and prefetched opcode word per store.
- `DORADO_TRACE_GATE="lo,hi"` (cycles) gates cpu.c-level traces that
  lack a cycle counter (currently DORADO_IFUDISP_TRACE).
- `DORADO_ETH_TX_TRACE=1` dumps every completed transmit packet (and
  TX_ACK lines with the server's lock-step seq).
- `DORADO_JUNK_TRACE=1` logs junk-timer enable/disable transitions.
- `DORADO_PC_COUNT="a,b,..."` (octal, max 8) counts task-0 executions
  of specific real uPCs in the full-boot probe, printed at probe end
  with last-execution cycles. AEmu reference points: DIR=3100,
  EIR=3101, BRI=3102, DIRS=3113, interrupt delivery (PCLOC store)=756,
  AEmuReschedule=314, RestartIFU=307.
- `DORADO_RM_WATCH=<octal>` logs writes to one RM address with cycle,
  uPC, RBase, PCX (cpu.c rm_stk_write).
- `DORADO_RBASE_TRACE=1` + `DORADO_TRACE_GATE` logs task-0 RBase
  transitions per instruction.
- The probe summary prints `Ethernet Stage-2 ... state/seq/max_seq/pos`
  and an `Alto display:` block (DASTART, vmask, WW, ACTIVE, cursor,
  DCB chain walk).
- `/tmp/realmap` pattern: a tiny standalone C tool linking mb.c +
  microcode.c + disasm.c that prints real-address -> symbol for
  AEmu.mb, and a sibling that dumps decoded fields at real addresses.
  CALL `mb_init(&mb)` BEFORE `mb_load` or im_id/dmachine are garbage.

## Hard-won caveats (do not relearn these)

- REF_W (harness trace) misattributes refs in IFUJump-tailed
  instructions as the IFETCH — stores can vanish from REF_W entirely.
  The memory.c-level STORE_VA trace is authoritative.
- The EtherBoot loader executes 0o30 words below its listing addresses
  (the BoL payload lands at VM 1; the listing's load origin is 0o31).
  PC-relative code and literal cells make this self-consistent; when
  matching dispatch traces to the listing, add 0o30.
- IFUDISP br31 = the last IFU rebase target (word address), pcf = byte
  offset from it.
- Alto ether: ePLoc (VM 0o600) is the SHARED post cell for input and
  output. InDone=0o377, OutDone=0o777, SIO-reset abort = CmdAbort XOR
  ECmdBits = 0o2771. boot6 accepts only post == 0o377 exactly.
- AEmu sources are one-line Bravo files: read with
  `perl -pe 's/;/;\n/g; s/\r/\n/g'`. EBoot/EBLoop live in
  AltoEtherEmu.mc; the boot dispatch and ABoot in AEm0.mc; Nova skips
  (NoSkip/DoSkip, PCF <- PCX+4) in Start.mc; the RTClock math in
  Junk.mc ("Junk wakeup interval = 32 us").
- ABoot's 100 ms keyboard-settle wait polls VM 0o430 (RTClock high
  word, ticks every ~39 ms of virtual time) — a slow RTC stretches the
  entire boot timeline, and timed waits in booted software stall.
- Run the repro from `dorado/` — a stray `cd` to the repo root makes
  `./build/test_cpu` silently produce empty pipelines.
