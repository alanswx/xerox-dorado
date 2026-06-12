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

## SESSION 4: THE PUP STACK IS ALIVE (2026-06-11 evening)

The fake server now speaks enough Pup to wake NetExec's whole network
stack. Sequence of unblocks, each verified by the TX trace:

1. **The raw routing probe accepts a directed packet.** NetExec's
   pre-context park (the VM 0o3205 raw listen) wants a DIRECTED Pup
   (nonzero dest host), dest socket 60B, type in 200B..203B (the
   GatewayInfo family - NOT time), and a length-consistent packet.
   dorado_ethernet_time_broadcast() (misnamed now) sends a
   GatewayInfoReply (201B) shaped for it, alternating dest socket
   60B / psRouteInfo(2). Once accepted: THE CONTEXTS START, the
   cursor changes from the boot-loader sprite to a solid 16x16 block
   (fb_nonzero 53 -> 74), and NetExec begins SOCKET-LEVEL
   TRANSMISSION - type 204B routing-info requests to net1/host1
   psRouteInfo, from its socket 60B, retried continuously.
2. **eth_tx_packet_done answers type-204B requests** by dest socket:
   socket 2 -> GatewayInfoReply 201B with one routing tuple
   <net 1, gw net 1, gw host 1 (server), hops 0>; socket 4 -> Alto
   time reply 205B with an NTime body. Replies are directed to the
   requestor's source port with dnet=0 ("this net") to dodge the
   localNet bootstrap deadlock (a net-1-addressed reply gets
   FORWARDED, not delivered, until localNet is learned - which the
   reply itself teaches).
3. NetExec LEARNED ITS NET: its requests changed from dnet=0/dhost=1
   to net1/host1 after the socket-2 routing broadcasts.

SESSION-5 STRUCTURAL DISCOVERY: the module occupying VM ~0o1664-
0o3667 with statics at M[0o1010..0o1113] is the FullBootBase
RESIDENT (BuildNetExec.cm links "FullBootBase/J ... DiskBoot.Run/B
300/N FullBootBase.xc/S 1411/V"): it provides MoveBlock
(static M[0o1013] = 0o3513 - verified), the interrupt-window sub
(0o2014), the channel handlers, the RCLK timer pump (M[0o1113] =
0o2330), and the raw ether exchange service (M[0o1012] = 0o3203,
entry chain 0o3203 -> JSRII @M[0o1017]=0o3530 -> the SIO+poll at
0o3205-0o3232). The eternal loop never calls the Context package's
Block (0o47701) - all its JSRII statics resolve inside the module -
so the coroutine ring stays frozen (ring resume PCs identical across
every run) while the bootbase service spins. NetExec's socket-level
204B requests are emitted from THIS machinery (pre-context), not
from the GatewayListener context. The received replies are stored at
VM 0o3531+ (verified EIT stores) - into what appears to be the
service's own buffer/return area; the per-exchange pointer cells
(M[0o3144..0o3150] = 600B..610B) are also REUSED as scratch at
runtime, so static disassembly of the @-operands is unreliable -
trace live.

SESSION-6: THE RAW SERVICE'S STATE MACHINE IS DECODED. Live dispatch
tracing through one full round (gate 92222590+) proves our reply
passes EVERY filter check and dispatches by Pup type:
- 201B -> handler at @M[0o3355]: inverts the 16-word cursor bitmap
  (the observed solid-block cursor!), TURNS THE PACKET AROUND
  (word0 byte-swap, dPort/sPort exchange, type <- M[0o3505]) and
  transmits it back - the "verbatim echo" fully explained, it is the
  service ANSWERING a ping.
- 202B -> handler at @M[0o3407]: arms the deadline cell M[0o3315] =
  RTC430 + M[0o3313](=207B ticks ~ 5.4 s).
- 203B -> handler at @M[0o3325]: if armed, restores the saved ether
  block (two MoveBlocks from the 0o3613/0o3646 save areas) and
  RETURNS TO THE CALLER - the clean exit. (Sending 203B as FIRST
  contact derails: nothing saved yet. A 203B mid-stream after 201B
  pings showed no visible exit - sequencing not yet right; possibly
  202B must arm first, or data must arrive between.)
- The machine's own 204B requests to psRouteInfo share the same
  socket 60B - they are THIS module's requests; body [0o401, 0o1000]
  (= net1-host1 port + 0o1000?). WORKING HYPOTHESIS: FullBootBase
  fetches something over the net at startup (page/image fetch or a
  server handshake): request 204B, responses typed 200B..203B with
  202B=start(arm timeout), data, 203B=done(restore+return). Next:
  decode handlers @M[0o3352] (type 200B) and the 202B-then-data path
  (where does a post-202B packet's BODY get copied?), and try the
  sequence 201B, 202B, <payload?>, 203B. Identify the protocol by
  finding FullBootBase's source (BuildBoot /B 300/N switches;
  search AltoSource for FullBootBase.asm/bcpl or BuildBoot docs).

SESSION-7 BREAKTHROUGH: THE MACHINE IS SITTING IN SWAT. The raw
module is **TeleSwat** (TeleSwat.asm fetched into
chm/altosource/buildboot.dm!2_/ along with FullBootInit/BootBase/
SaveState): socket 60B, ptSwatStore=200B (poke), ptSwatFetch=201B
(peek - the "echo" was a memory-read reply!), ptSwatSwap=202B,
ptSwatSwapReply=203B; the solid-block cursor is the SWAT CURSOR
(userCursor/swatCursor exchange); the eternal loop is TeleSwat's
il1/il2 receive loop; OutLd at VM 0o3317, OutLdRet at VM 0o3323.

THE CRASH: NetExec called CallSwat during startup. The swat-saved
state (page 0o700 area: AC1=0o142476 = the first context,
AC2=0o142502 = its CTX.StackMin VALUE, saved-PC-ish 0o47720 = inside
CallContextList's runCtx `jsr @1 2`) plus the ring dump
(ctx[0].Stack == 0) give the mechanism: **the first context's
CTX.Stack word (VM 0o142477) is 0/corrupt at first resume**, so
`jsr @(AC2=0)+1` jumps through M[1] (boot-loader residue) into
garbage -> trap -> CallSwat -> TeleSwat. The OTHER contexts'
CTX.Stack values are CORRECT (ctx[1] = pupLevel1Ctx has
stack = ctx+0o253 = region+175-4 exactly per InitializeContext), so
InitializeContext works in general - something specifically
corrupts ctx[0] (FeedEther, the 40-word eFSS context, the FIRST
allocated+enqueued) between its init and the first resume.

SESSION-10 (2026-06-12): FOUR more microarchitecture fixes, each
verified and committed (d40d6aa, 30cbf0e):
1. Multiply consumed the LATCHED Carry' instead of the live ALU
   carry-out (MulSub's A0 entry steps are logical ZERO ops) - every
   product gained a phantom 100000B partial term.
2. BLOCK now clears the task's wakeup-request FF unconditionally
   (HM p27). EOT's `Output_ TurnOffTx, Block` used to re-wake and
   retransmit every packet ~13x; once the BCPL driver cleared
   eOCLoc, the microcode posted CountZero (2377B) and EtherInterrupt
   SysErr'd (the 92.3M-cycle crash). EFTP now sends exactly one
   copy per Ack (88 replies for an 88-packet file, was 484).
3. Divide/CDivide implemented (were no-ops): Pd<-ALU[1:15],,Q[0];
   Q<-Q[1:15],,ALUcarry (CDivide: carry'). Stub made lenPup
   unhalved -> 25 PBIs of 564 words -> sysZone exhausted -> Alloc
   error 1801 at Title's 461-word display-line Allocate.
4. ALU=0/ALU<0 branch flags latch from the ALU output, NOT from the
   FF-transformed Pd (HM p30) - DivSub's FinalAdd remainder fixup
   was being skipped.
Probes added to the full-boot summary: BCPL stack walk from the
swatted AC2, runtime code windows, sysZone boundary-tag census
(zone base 123240B, min/max in hdr words 8/9, signed length word
per block, negative = allocated). Forensic recipe that worked,
repeatedly: STORE_TRACE_VA="700,707" -> swat cycle; then
TRACE_GATE + IFUDISP_TRACE -> br31(hex)=Nova word PC of each
dispatched opcode; disassemble NETEXEC.BOOT (1:1 file-word = VM
word) at those addresses; JSRII (64400B) = call through M[M[.]];
M[1303B]=lvSysErr; CallSwat=16251B; M[2..7]=77400B trap words;
Sys.Errors (in OSSOURCES.DM!2, fetched local) decodes error codes
(1801=zone full, 4267B=ecBadEtherStatus...).
STATE at end of session: 400M-cycle boot has healthy sysZone
(15858 alloc/6268 free), BOTH Title text bands in the DCB chain
(42-line strip + 2x 38-word x 6-scanline bands at 142544B/140606B
+ tab bands), eftp_replies=88, fb still cursor-only (DWT renderer
never paints the DCB text - separate gap).
SESSION-10d: REF_W (symbol-independent) CONFIRMS it. Per-ref dump
(DORADO_STORE_WINDOW) at cyc 124,025,621: STORE (kind=5) at uPC
1201B, MemBase=22B (Alto MDS bank, correct), BBDst=0xE4=344B,
data=4154B; paired StoreLastDst at 361B writes 345B. Destination
writes land at SCATTERED page-zero cells (413B,344B,345B) across
scanlines, so the BBT's DBCA/DBMR are garbage. The store IS BitBlt
(BITBLT=61024, BitBltA SD400+24); the UpdateSector symbol mismatch
was external-symbol-file straddling. So the bug is upstream: a
malformed BBT with a page-zero destination base. NEXT: dump the
BBT words at BitBlt entry (Fetch 2S=DBCA,3S=DBMR off the BBT
pointer) and trace who built that base.

SESSION-10c: THE PAGE-ZERO CLOBBER IS A BAD BitBlt DESTINATION.
The store that flattens M[344B] is BitBlt microcode: uPC 1201B=
GRAYLOOP / 361B=STORELASTDST (AltoBitBlt.mc), i.e. a Nova BitBlt
gray-fill writing word after word starting at BBDst~344B. BBDst is
computed in BitBlt setup as DstY*DBMR (scan-line * raster, via
Call[MulSub]) + DBCA(base), MemBase=BBDstBR. So the destination
landed in page zero because EITHER (a) the DstY*DBMR offset or the
base-add wrapped, or (b) NetExec passed a BBT whose DBCA/DBMR were
already garbage. Recurs at EXACTLY 124,025,617 and 187,450,772 (a
periodic Title-banner redraw via EraseBits->BitBlt); earlier
redraws built the DCB bands correctly, so it is DATA-DEPENDENT - an
arithmetic edge case, not a blanket BitBlt failure. BitBlt setup
uses the just-fixed MulSub AND a `Branch[.+2, ALU=0]` skip-multiply
test - both in recently-touched paths; re-audit in BitBlt context.
NEXT STEP (concrete): gate to the failing BitBlt (cyc ~124,025,000),
capture the BBT it reads. Setup fetches BBT words via Fetch_ 2S/3S/
4S/5S/7S (DBCA/DBMR/DLX/DTY/DH) off MemBase=BBT pointer; print
T/Q/DstY/DRast at GRAYLOOP entry (uPC ~1175-1202) to recover DstY,
DBMR, DBCA and the final BBDst. If BBDst != DBCA+DstY*DBMR by hand,
the bug is our address arithmetic; if it equals but DBCA~0, walk
back to who built that BBT (CmdScanDisplay EraseBits / a display-
stream bitmap base) and whether that base came from an earlier
mis-multiply/divide. Identify the BitBlt caller via IFUDISP br31
just before uPC enters the BitBlt trap / 1140B SETUPBRS.

SESSION-10b: THE FillWithDash SWAT IS FULLY TRACED - it is page-zero
corruption, not a divide bug:
1. M[344B] is the OS page-zero DIV transfer vector (jsr @344,z from
   compiled BCPL x/y). FullBoot init sets it to 4155B (the divide
   routine; word 4154B before it is a `jmp @1,2` thunk).
2. At cycle 124,025,617 (and again 187,450,772) Nova code running
   with its BCPL FRAME IN PAGE ZERO (AC2 ~ 270B-324B!) executes a
   normal-looking store sequence (function at 12672B..13105B,
   GetFrame size 42B, vec buffers IN FRAME at frame+const) whose
   in-frame "region" writes land on 344B/345B, leaving M[344B]=4154B.
3. Every divide thereafter goes jsr @344 -> jmp @1,2 -> resumes at
   the previous call's nargs word -> executes 000002 = jmp 2,z ->
   M[2]=77400B trap -> Swat at 190M. (The AC-save 2/135B/7 is just
   the divide args/remainder mid-veneer; MulSub/DivSub/DIVx are all
   CORRECT - test_mulsub_aemu, test_divsub_aemu, test_divx_aemu.)
4. WHY is a frame in page zero - CORRECTED after tracing: the
   "IST with Stack=0" reading was WRONG. InitAltoEther
   (PupAlEthInit.bcpl) redirects interruptVector!i to
   `lv ndb>>EtherNDB.asmProlog` - M[502B]=160224B is the Ethernet
   NDB's asm prologue, and the "Stack/InitPC" words I read were NDB
   queue fields (STORE_TRACE on 160233-160240 shows them written
   constantly by Enqueue/Dequeue at br31=50033B/50050B and a
   counter at 50142B - normal). The REAL IST is at
   ndb+lenEtherNDB, not yet inspected.
   The corrected suspect: routine 4551B is the Context package's
   Block/yield (AC2 <- ctx arg, ctx!1 <- suspended frame). The
   bad slice ran inside a context resumed at cyc 124,016,575 whose
   StkMin (M[335B]) was set to a HEALTHY 155303B, yet the BCPL
   frame was ~270B: i.e. **the context's CTX.Stack field (ctx!1)
   held ~270B garbage while CTX.StackMin (ctx!2) stayed intact**.
   Context resume trusts ctx!1 with no bounds check, so execution
   continued at a page-zero frame and its in-frame vec stores
   flattened M[344B].
   NEXT STEP: the corrupt context block is at ~155277B (StackMin
   155303B = ctx+3+extra). Run STORE_TRACE_VA="155277,155304" to
   catch who writes the bad ctx!1 (one 400M boot, watch values
   ~270B), then trace that writer. Candidate mechanisms: a Block()
   while already running on a clobbered frame (cascading), an
   interrupt-path save storing a bad frame, or one of our
   remaining microarchitecture gaps corrupting AC2 mid-slice.
   Also identify which context 155277B is (walk the ctx ring at
   probe end and print each ctx's Next/Stack/StackMin/proc).
5. Probe additions this session: IntVec/IST dump, sysZone census,
   BCPL stack window, code windows. ETHC control-write trace env:
   DORADO_ETHC_TRACE=1 (+TRACE_GATE).

UPDATE (same session, post-commit): with the Divide fixes the boot
now runs to cycle ~189.97M before the SAME FillWithDash swat
(STORE_TRACE_VA="700,707" -> AC-save at 189968878-189970790;
AC0=2 AC1=135B AC2=7 AC3=157167B PC=21071B). That is ~97M cycles
(11+ emulated seconds) of healthy NetExec running - many Title
banner redraws succeed before one divide dies, which all but
proves an ASYNC interference (a task interleaving mid-DivSub).
Junk.mc's `Q_ T` is in an emulator-only subroutine (ReadCounters),
NOT the junk task body; DisplayMain.mc and AltoEther.mc contain no
Q_/Cnt_ at all - so the obvious Q/Cnt clobber suspects are
exonerated at the source level. NEXT: capture the microinstruction
interleaving in the ~2000 cycles before the swat (gate
189966000,189971000; need a per-cycle task+uPC trace - add one if
DORADO_PCWATCH doesn't fit) and watch DivSub's Cnt/Q/latched-Carry
across the interleaved task slices; verify our task-switch
save/restore of the latched ALU branch flags vs HM, and whether
Return/SCall skew interacts with a task switch at the Return
boundary. The swat cycle moved 92.9M -> 190M with identical
signature across the last two binaries, so it reproduces
deterministically per build.

OPEN: one remaining swat, PC=21071B = return from `jsr @344,z`
(the BCPL divide veneer) in FillWithDash's dash-count divide
(NetExec.bcpl Title path, code at 21050B-21105B): AC-save shows
AC0=2 (correct remainder of 93/7), AC1=135B (=93, quotient NEVER
stored), AC2=7, AC3=157167B. BUT test_divx_aemu drives the exact
same S-Group DIVx wrapper (STK-resident ACs, 93/7) and PASSES.
So the in-vivo failure involves state the unit test lacks - prime
suspects: (a) a mid-DivSub task switch clobbering shared Q or Cnt
(EOT/EIT/display tasks interleave in vivo; check whether their
microcode touches Q/Cnt and whether Cnt must be per-task), (b) the
runtime veneer at M[344B] doing something beyond Nova DIV, (c) an
IFU/Reschedule interaction at DIVx's IFUJump[0] tail. NEXT STEP:
STORE_TRACE_VA="700,707" for the new swat cycle, then gated
IFUDISP+ETHC trace around it; also dump runtime M[344B] veneer
code via the code-window probe. After that swat falls: check
whether Title's PutTemplate writes glyphs into the band bitmaps
and make the DWT/display pipeline render the DCB chain (probe
"Display render" still shows A_pixels=0).

SESSION-9 ROOT CAUSE + FIX: **the Dorado Multiply FF function was
a TBD no-op** (cpu.c FA=1 FB=7 FC=2). AEmu's Nova MUL microcode
(Various.mc MulSub) steps on it; HM p23 gives the full semantics,
now implemented: Result <- ALUcarry,,ALU/2; Q <- ALU[15],,Q/2; next
branch address OR 2 if pre-shift Q[14] (per-task, via the
task_dispatch plumbing). Every BCPL multiply was garbage before -
CreateKeywordTable allocated 15 words instead of ~82 (verified via
the SB length word at 0o142450: -16), LoadKT's second entry then
overran into the first coroutine's CTX. Post-fix the boot moves
STRICTLY FORWARD: the KT-overrun crash is gone, contexts run
further, CallContextList returns to its caller (CtxRunning =
dummyCtx at probe end), and a NEW later CallSwat happens with crash
site ~VM 0o16277-0o16306 (the BCPL call veneer region; saved frame
~0o172142). fb_nonzero=59 (a third cursor state). NEXT: same
forensic loop on the new crash - dispatch-gate around the new
OutLd call (find its cycle via STORE_TRACE on 0o3323/OutLdRet or
the swat AC-save writes), identify the SysErr, fix the next
emulation gap. Each iteration of this loop has removed exactly one
fundamental microarchitecture bug (BDispatch, RBase, Map Mar, junk
timer, Multiply...).

SESSION-8 FORENSICS (led to the above) (the corruption, three layers down): the
4-word Zero at VM 0o142474 is `Zero(ptr,4)` (BCPL veneer at VM
0o4536) called from the keyword/boot-directory entry machinery at VM
0o31675-0o32100 (timeline = AfterJunta's CreateKeywordTable/LoadKT,
~92.09 M). The getter at 0o31675 walks a keyed chain at q!5
(node = [next@0, key@1, ...]; on key match `resultis node+1`) and
MATCHED key 0o100000 against STALE memory: node = 0o142473 points
into a just-recycled allocation. The legitimate history of that
memory: a 4-word object (data 0o142471-74, sb 0o142470, length -5 -
all CORRECT zone arithmetic, verified by store traces at cyc
92060802+) was built with packed-field stores (field merges verified
CORRECT - dorado/CLAUDE.md's "ShMd stubbed" note is STALE, the
implementation merges real Md), then released at cyc 92078882+
(-1 writes at 0o142470/71 from br31=0o50764 ~ AltoTimer region, then
Enqueue(q, 0o142470) at br31=0o50033). The mystery: the lookup chain
head q!5 contained 0o142473 (not the enqueued 0o142470) - find who
wrote 0o142473 into the chain head. NEXT: identify q (frame!4 of the
0o31675 call - dump via a PCWATCH at 0o31701 reading AC3, or trace
the Enqueue at 92080914's queue argument), then STORE_TRACE the
chain-head cell. Every software step verified so far is legal -
expect ONE wrong emulated instruction upstream (the remaining
unverified exotic: `dsz/isz @indirect`, the BCPL veneer's numargs
walk, or a carry case in the 0o31707 sub# compare chain).

PREVIOUS (superseded) NOTE: THE CORRUPTOR IS CAUGHT (the one-command probe ran): the CTX life
in stores: InitializeContext writes Next=0, StackMin=ctx+4,
**Stack=0o142542 CORRECTLY** (cyc 92053271, br31=0o47764), ctx!3=ndb,
Enqueue sets Next - and then at cyc 92095284 a Zero()/BLKS issued
from caller code at ~VM 0o4536 wipes ctx+0 AND ctx+1 (CTX.Stack!).
Later enqueues restore Next but Stack stays 0 -> the first resume
crashes into Swat. The Zero belongs to a LATER Allocate (timing
matches InitPupLevel1's `pupRT = Allocate(zone, n); Zero(pupRT, n)`,
the 20+1-entry routing table) - **the zone allocator (Alloc package)
returns a block overlapping the earlier FeedEther-context
allocation on our emulator**. NEXT: REF_W window 92095200-92095400
to get the exact zeroed range; reconstruct the Allocate sequence;
find the emulation defect in the allocator's arithmetic (the Alloc
package's compare/carry chain - the same instruction-semantics class
as the BDispatch/RBase/Map bugs). Fix it, and the contexts run:
Title paints the banner.

ALSO AVAILABLE NOW: TeleSwat gives a REMOTE DEBUG CHANNEL into the
parked machine (Fetch/Store/Swap) - the fake server can peek/poke
arbitrary VM and even resume; and CallSwat's message argument (a
BCPL string) is reachable from the swat frame for an exact error
identification.

STILL OPEN (the current frontier):
- The 204B routing requests never STOP - the replies reach the
  machine (InDone + EIT consumption verified) but the requesting
  context never consumes them. Removing the dummy-CRC trailer word
  from replies reduced the retry rate substantially (EtherPupFilter's
  length equality is sensitive to the trailer shape) but did not
  satisfy it. Calibrate the exact on-wire length convention the
  PBI/EtherPupFilter math expects: packetLength must equal
  (pup.length+5) rshift 1; derive the eELoc/stored-words accounting
  from AltoEther.mc's receive loop and match the trailer exactly.
- ANOMALY worth chasing: NetExec re-transmits our replies VERBATIM
  (ether word0 preserved with src=1, impossible for a normal send) -
  every reply bounces once. Either our model's EOT path leaks rx
  into tx, or the driver's eOB/eOPLoc gets pointed at the input PBI.
  Find who builds that transmission (trace the EOT data-write era
  after a delivery).
- The banner DCB still never appears (Title context still parked
  behind the unfinished routing exchange).

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

0a3. **Final session-3 narrowing: the ether output never starts.**
   Hard facts from store traces: M[0o452] (WW) is NEVER stored
   post-LoadRam - so Interrupt.asm's CauseInterrupt (dir; WW |= mask;
   eir) NEVER RUNS, hence SendEtherPacket's kick (`if eOB eq 0 then
   CauseInterrupt(mask)`, PupAlEthb.bcpl line 70) is never reached or
   its guard fails; consequently no ether interrupt, no output start,
   no Mayday/routing-probe ever transmitted, and every higher layer
   (GetTime/GetDir/GetName, the boot fetch) waits forever. The
   dispatch trail shows SendEtherPacket's broadcast-loopback branch
   executing (MoveBlock x3 + filter walk) and then the flow entering
   an SIO + poll sequence at VM 0o3205-0o3232 whose shape matches
   PupAlEtha.asm's StartEther start2 path (zero @ePLoc, zero @eLLoc,
   lda outCmd, sio) followed by a deadline/event poll. The window
   sub at VM 0o2014-0o2022 (lda @X; sub; sta @Y; EIR; DIR; sta @Z;
   ret) runs to completion - the EIR;DIR pair is by-design and the
   sub returns normally; the system legitimately runs
   disabled-with-windows. 47 of 49 windows found nothing pending
   BECAUSE WW never got the ether bit. NEXT: single-step the FIRST
   SendEtherPacket call (the LocateNet routing probe during
   InitPupLevel1, before 92 M) - find why it never executes
   CauseInterrupt's `sta 1,@.Wakeups`: RESOLVED ONE MORE STEP: the
   probe now scans VM for the EtherNDB's ePLoc pointer table
   (consecutive 600B..610B) and dumps eIB/eOB/eState - ALL ZERO at
   probe end. eOB=0 means the guard would have passed; therefore the
   flow diverts BEFORE the `Enqueue(oQ); if eOB eq 0 CauseInterrupt`
   lines - inside the broadcast-loopback branch's filter walk
   ((pf>>PF.predicate)(ipbi) = EtherPupFilter in PupAlEtha.asm), or
   the MoveBlock cluster belongs to CompletePup's header/checksum
   construction and SendEtherPacket is never reached at all. Probe
   accordingly: trace the Alto PC from the MoveBlock cluster
   (~92.199 M) forward instruction by instruction and identify the
   divergent branch; suspect list: the Pup software checksum
   (add-and-left-cycle per word - carry/cycle semantics), and
   EtherPupFilter's hand-coded compares. The Alto-PC regions: SendEtherPacket around
   VM 0o315x-0o320x, CauseInterrupt would store @0o452.

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
