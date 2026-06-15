# Continuation handoff — Alto-on-Dorado boot bring-up

## STATUS (2026-06-14): canonical Mesa world boots NETEXEC; Mesa VM reached, stops at first XFER

End-of-session state, on top of the muffler/DMux fix below.

**IFUM half-word order fix (`a03c4c5`) -- the keystone.** `src/cpu.c`'s `IFUMLH<-B`/`IFUMRH<-B`
FF handlers were inverted (wrote the wrong IFUM half) and `src/mb2eb.c` pre-swapped IFUM
word0/word1 to compensate -- two bugs that cancelled for our self-built `worlds/aemu.eb` but
loaded a *canonical* archive world (`AltoMesaDorado.eb`) IFUM-transposed. Opcode `045`
(`LDAipc`) then dispatched to garbage vec `0o4470` (vs the real `0o1370`); the bogus fields
word set `TPause'` (halting the IFU) and `RBase=0`, and it spun forever. Fixed atomically:
swapped the four IFUMLH/IFUMRH read+write handlers to match `LoadRam.mc` (IFUMLH=address=
`ifum_lo`, IFUMRH=fields=`ifum_hi`), emit canonical item order in `mb2eb.c`, corrected
`test_cpu.c`'s `test_ifum_load_read` (it had encoded the bug), and REGENERATED `worlds/aemu.eb`.
Now `AltoMesaDorado.eb!2` + NETEXEC paints (~1466 px); AEmu still green (NETEXEC 1482, Galaxian
121552, suite 10/10). Foundational: every canonical Xerox world had been loading
IFUM-transposed, masked only by the cancelling `mb2eb` pre-swap.

**Mesa VM reached and works; op 362 is `@STOP` (designed), not an XFER trap [CORRECTED].**
Serving a Mesa-format (`0o345`) program (MazeWar) to `AltoMesaDorado.eb!2`: downloads fully,
runs the Nova relocation bootstrap, switches to `insset=2` (the Mesa VM), and runs a 6-bytecode
boot prologue at VM PC `0x5108` (`IWDC,RIL0,LL2,R0,WB,STOP`). The 6th, op `362`, is **`@STOP`** --
the *designed* Mesa->Nova handoff, executed faithfully (`DMesaXfer.mc`: "save state and exit to
Nova world", loads `T=MesaStopLoc=0o26`; no fault; the `insset>=2` byte order at `cpu.c:2159` is
confirmed correct). So the Mesa VM genuinely works -- "stops at first XFER" was the wrong framing.
The 13 `DMesa*` Mesa-VM modules (named `AltoMesa*` in `AltoMesa.cm`, `DMesa*` on disk) are local in
`chm/doradomicrocode/doradomicrocodesources/`.

**Real Mesa blocker: two separate, larger items (neither a gated one-liner).** After `@STOP` the
Nova world parks polling an I/O-completion control block; device confirmed = Ethernet (not
RTC/keyboard). It splits:
- **MazeWar / `0o345` worlds -- the Alto interrupt fabric is COMPLETE; the WORLD never arms it
  [PINNED].** The interrupt machinery is already correct in our microengine: NWW (an RM register,
  not memory `0o452`), the `AEmuReschedule` trap (`AEmuTrapBase+14`), the `Reschedule` FF, and the
  `PCLOC`/`INTVEC` (`0o500`/`0o501`) vector path. The receive-completion microcode (`AltoEther.mc`
  `EPost`) runs faithfully on the EIT (posts EPLOC `0o377`). But the world never set up the
  interrupt CHANNEL: traced over a full MazeWar run, `EBLOC` (`0o601`), `ACTIVE` (`0o453`) and
  `INTVEC` (`0o501`, the `EtherInterruptEntry` handler addr) are PERMANENTLY 0 and Nova `EIR` is
  never reached for the ether channel. So `EPost`'s `NWW <- NWW OR EBLOC` ORs 0 -> no interrupt
  ever fires. Forcing NWW from C would CRASH: `AEmuReschedule` vectors through `INTVEC(0o501)=0`
  -> jump to page 0 (the documented runaway). So there is NO faithful C fix. The bare `0o345`
  Pilot outload reaches its poll loop at VM `0x0FD2F` WITHOUT running the Pup interrupt-subsystem
  init (channel-allocate + `EIR` + install the handler at `0o501`) -- work the **Pilot germ/nucleus**
  is supposed to do. So this confirms the architectural conclusion: the `0o345` family needs the
  germ/Pilot boot (Route B below), not any Alto-side interrupt change.
- **CedarNetExec:** a different loop polling EPLOC directly (no interrupt); Rx works but it awaits
  Cedar-germ *server content*, so its path is the **Cedar germ/Pilot net-boot**
  (`InitialEtherCedarDorado.eb!3` + `CedarDorado.eb!6` + germ), per the FINDING below -- not the
  Alto receive interrupt.

(The `2026-06-13f` interrupt-cell framing was retracted by `13g`; don't rely on it.)

**Also landed.** NetExec game menu (`87834cd`): `--boot-dir-all` (default) advertises all 29
Alto B-format games via the `257B`/`260B` boot directory; boot NETEXEC and type a game name at
the `>` prompt to boot it. 26 CHM-Murray Alto programs + games committed (`7f07cce`), each with
a `make run-*` target. The Alto path stayed green at every step.

## FIX (2026-06-14): muffler/DMux model unblocks AltoMesaDorado.eb past InitMem -> STARTEMULATOR

**Root cause.** `AltoMesaDorado.eb` (= placed `chm/dorado/Mesa.mb!3`) entered `InitMap`
(real PC `0o1076`) and hung forever in the cold `InitMem` map/storage-init loop
(`MAP1TO1LOOP`/`IWRITEMAP`/`WAITFORMAPBUF`/`NEXTMAPENTRY`), never reaching `INITMEMDONE`
`0o5706` or `STARTEMULATOR` `0o5021`; 0 opcode dispatches, 0 pixels. `worlds/aemu.eb` ran the
byte-identical loop fine. The loop bound is `VirtualBanks` (entries enumerated =
`VirtualBanks * 256`), set by `GetMemConfig` (`InitMem.mc`) from two diagnostic-multiplexer
("muffler") reads issued by `SetDMuxAddress` (`Various.mc`): DMux `0o1512` sign-set ->
`MapIs256K` (`VirtualBanks=2000C`), DMux `0o1511` sign-set -> `MapIs64K` (`400C`), neither ->
`MapIs16K` (`100C` = 64 banks * 256 = 16384). The muffler bit arrives as the SIGN of
`SetDMuxAddress`'s trailing `T_ XTemp17_ ALUFMem, Return`. We modeled `ALUFMem` as the raw
ALUFM register, so the sign was world-dependent garbage: AEmu happened to land sign-CLEAR
(16K, matches our `DM_MAP_ENTRIES=16384` map, loop terminates) while Mesa landed sign-SET
(64K/256K, enumerates a space far larger than our map, never finishes). Both worlds were
faithful; AEmu booted by luck of its ALUFM contents.

**Fix.** Added a DMux/muffler model (`dmux_addr`/`dmux_pending` in `dorado_memory`,
`dorado_memory_dmux_strobe/_use_dmd/_pending/_read` in `src/memory.c`). `MidasStrobe<-B`
captures the DMux address (T[4:15]) on the leading strobe and arms a pending read/write;
`UseDMD` consumes it as a manifold WRITE (Initial `WriteManifold`); a `Pd<-ALUFMem`/`ALUFMRW`
read while pending returns the muffler word with the selected bit in the SIGN position. For
our 16K-map config both `0o1512` and `0o1511` read sign-CLEAR, so `GetMemConfig` falls through
to `MapIs16K` and the cold loop terminates. The `dmux_pending` gate (set only by a preceding
`SetDMuxAddress` shift, cleared by the muffler read or by `UseDMD`) keeps normal
`Pd<-ALUFMRW` ALUFM access (Bootstrap/Initial/emulator startup) untouched. `src/cpu.c` wires
the `MidasStrobe`/`UseDMD`/`ALUFMem` handlers.

**Result.** `AltoMesaDorado.eb!2` + NETEXEC now runs `InitMap` -> `TasksOff` ->
`GetMemConfig`/`SetDMuxAddress` -> past `InitMemDone`/`StartEmulator`; task-0 hot PCs are now
`GETPC`/`ABOOT`/`AMIDSTBOOT`/`READTERMINAL` (Alto-emulator routines only reachable via
`STARTEMULATOR`), the InitMem map-loop PCs are gone from the hot list. Next blocker: the
post-StartEmulator boot-selection wait (`ABOOT`/`READTERMINAL` reads the keyboard for the
boot choice; the DDC keyboard back-channel / NETEXEC pull-through, gap E2, is not modeled), so
still 1 IFU dispatch (`insset=0`) and 0 pixels. Regression gate green: `make test` 10/10;
AEmu NETEXEC 1501 display-list pixels; Galaxian 121552 pixels.

## FINDING (2026-06-13): Cedar boots from disk; the Mesa VM is in AltoMesaDorado.eb, not worlds/aemu.eb

Established this session from the Dorado Booting memo
(`chm/doradosource/DoradoBooting.tioga!2.txt`, sections 1.1–1.4). This overturns the prior
assumption that serving `CedarNetExec.boot` directly to `CedarDorado.eb` over EFTP is the
right Cedar bring-up route.

**Cedar microcode (`CedarDorado.eb`) boots software from disk, not Ethernet.** Memo §1.3:
"Ethernet booting of software is possible only with Alto emulator-based microprograms, not
with Cedar. To boot Cedar software from the Ethernet, you must first boot the NetExec using
Alto/Mesa, then use the NetExec to call the CedarNetExec, then use the CedarNetExec to call
the desired program." So serving `CedarNetExec.boot` over our EFTP/Mayday server directly
to `CedarDorado.eb` was a category error: Cedar never listens for an Ethernet software boot.
Observed: the Cedar microcode loads and enters at `pc=0o1070` (BOOTORSTART), then hangs in
`InitMem`'s cold map-write loop (`IWRITEMAP`/`WAITFORMAPBUF`, hot PC `0o7116`), expecting a
disk germ, zero opcode dispatches, never reaches GERMBOOT.

**The Mesa VM lives in `AltoMesaDorado.eb`, not `worlds/aemu.eb`.** `worlds/aemu.eb` is
Alto/Nova ONLY (no Mesa VM). That is why Mesa-format (`0o345`) boot files mis-run on it:
the Alto interpreter decodes Mesa bytecodes as Nova opcodes (`insset=0`), nothing paints,
nothing executes. The supported Ethernet route to Mesa or Cedar software is via
`AltoMesaDorado.eb` (which carries the full Mesa VM) chained through NetExec.

**Two routes going forward, both work-in-progress:**

- **Route A (recommended):** `AltoMesaDorado.eb` -> NetExec -> CedarNetExec -> target.
  Reuses the existing Ethernet transport. `AltoMesaDorado.eb!2` enters correctly at
  `pc=0o1076` cold-bootstrap (unlike `CedarDorado.eb`'s stuck `0o1070`). Blocked on boot
  orchestration in `src/machine.c` and `src/ethernet.c` being tuned for `worlds/aemu.eb`;
  served a Mesa-format file it completes only 1 dispatch. Route A needs that orchestration
  adapted — first validate with `AltoMesaDorado.eb + NETEXEC` (an Alto B-format file), then
  fix the leader-page prepend and boot-select for a Mesa-format second stage. Not a zero-code
  swap.

- **Route B:** fix the Cedar microcode path directly. Requires (a) fixing the `InitMem`
  map-write Hold handshake in `src/memory.c` (`IWRITEMAP`/`WAITFORMAPBUF` blocker; see
  `dorado/CLAUDE.md` gaps B1/C1), and (b) a germ-Ethernet or disk boot channel. The Pilot
  germ is present locally: `chm/cedar/germ/Dorado.germ!4`; the physical-volume boot file is
  `chm/cedar/OthelloDorado.boot!8`. The ether-germ route avoids the incomplete disk write
  path (`BootChannelEther.mesa` / `BootChannelDisk.mesa` sources are in `chm/cedar/`).

See the finding immediately below for the two-format boot-file split that accompanied this
session.

## FINDING (2026-06-13): Boot files come in TWO formats needing TWO different emulator microcodes; we had been using the wrong one for Mesa/Pilot files

Established this session, with evidence. Dorado boot files split into two
incompatible classes, distinguished by the first big-endian word of the
file (`od -An -to2 -N2 FILE`, which prints the two bytes swapped):

- **Alto B-format** — big-endian word0 = `0o405`; `od` shows `0o002401`.
  These run on **AEmu**, the Alto emulator microcode at
  `dorado/worlds/aemu.eb`. They WORK today. Files in `chm/bootfiles/`:
  `NETEXEC.BOOT!8`, `CRTTEST.BOOT!1`, `DMT.BOOT!22`.
- **Mesa/Pilot outload** — big-endian word0 = `0o345`; `od` shows
  `0o162400`. These CANNOT run on AEmu — they need the **Cedar/Mesa
  emulator microcode** at `../chm/dorado/CedarDorado.eb!6`. Files:
  `CedarNetExec.boot!4`, `MesaNetExec.boot!1`, `AlphaMesaMesaNetExec.boot!1`,
  `MazeWar.boot!1`, `NEWOS.BOOT!21`.

Why this matters: we had been trying to boot `CedarNetExec` on AEmu, which
is architecturally impossible. CedarNetExec is a Mesa/Pilot program — its
own source `NXControl.mesa` hard-codes
`microcodeFiles[Dorado] = "CedarDorado.eb"`. On AEmu the Ethernet EFTP
download completes fine, but the Alto interpreter just mis-runs the Mesa
bytecodes (the IFU instruction set stays `insset=0`), so nothing paints and
nothing executes. (This corrects the earlier 2026-06-13g..e sessions below,
which were chasing an EPLOC race / interrupt-cell hang while booting a
Mesa-format file on the wrong microcode.)

Switching `--eb` to `../chm/dorado/CedarDorado.eb!6` makes the Cedar
microcode LOAD, but it does not yet run: it enters at `pc=0o1070`
(RestartEmulator) with zero opcode dispatches. A real Cedar bring-up
additionally needs:

- the Cedar Initial image `../chm/dorado/InitialEtherCedarDorado.eb!3`,
- the Pilot germ, and
- a Stage-2 rework from the Alto BootViaNet / leader-page path to the
  Pilot/germ net-boot.

So **all Mesa-format boots (including MazeWar) are EXPERIMENTAL /
not-yet-running**, pending that Cedar bring-up.

Also fixed this session (both EFTP-transfer gating): commit `62a4fb9` added
an EICLOC rx-delivery gate, and `858857b` corrected it to
`rx_count > EICLOC + 2`. The off-by-two had been silently blocking the
entire EFTP boot transfer; with it fixed the transfer now completes
(1 boot Mayday + 254 Acks).

## CORRECTED (2026-06-13g): CedarNetExec derails on an EPLOC race; the broadcast injector floods ~250x too fast

Supersedes 2026-06-13f below (the "interrupt-driven cell" framing was WRONG
-- those page-0 cells 0o103/0o112/0o130/0o164 are constant JSR-coroutine
vectors, reached AFTER divergence, not a data-dependent wait). Real root
cause: CedarNetExec's FIRST Ethernet init after the EFTP transfer (~141M,
VM 0o634-0o777) does SIO TurnOnRx (waits for an input completion at
EPLOC=VM 0o600) then SIO TurnOnTx (waits for OutDone=0o777 at the SAME
EPLOC). Our periodic broadcast injector (dorado_ethernet_time_broadcast,
machine.c ~670-677) feeds NETEXEC-tuned input packets into that same EPLOC;
an input completion (or InBufOverflow 0o1377 from an over-large packet)
landing during the Tx poll clobbers OutDone -> CedarNetExec takes a reset
path (SIO=CmdAbort) -> JMP @0 into a page-0 coroutine dead loop. With
broadcast OFF it hangs EARLIER on the Rx poll, so it needs an early packet
but NOT during the Tx poll. NETEXEC's init tolerates the flood; CedarNetExec
does not (it never reaches interrupt/display config: M[0o453]=0 vs NETEXEC's
0o010403).

THE CORRECT PROTOCOL (from IFS, quoting the Pup spec): gateway-info should
broadcast ~ONE EVERY 30 SECONDS (IFS GatewayInformationProtocol.cs:135-149,
Thread.Sleep(30000); the comment notes MazeWar "expects periodic updates");
breath-of-life every few seconds. Our injector fires every 2M cycles
(~120ms) then 50M (~3s) -- ~250x too fast. FIX DIRECTION: deliver the early
packet CedarNetExec's Rx init needs, then back off to the real ~30s cadence
so nothing lands during its Tx poll; also size injected packets to its
input buffer (EICLOC=VM 0o604) so they post InDone 0o377 not InBufOverflow.
A naive slow-down starves the Rx init; a naive tx_on gate is insufficient
(the packet is already queued and read during Tx -- tried, did not help).
Repro: `DORADO_STORE_TRACE_VA="0600,0600"` (the EPLOC race);
`DORADO_VMDUMP="0450,0460,200000000"` (interrupt cells stay 0). Baseline is
clean: 10/10, NETEXEC unaffected.

## FRONTIER (2026-06-13f): CedarNetExec runs but busy-waits on an interrupt-driven page-0 cell that never updates

After the pipe-fault fix (below) CedarNetExec RUNS: it dispatches Alto
opcodes (IFUDISP insset=0 -- it is an Alto program, NOT a Mesa image),
relocates, advances, then parks in a tight ~6-PC poll loop. The mouse
cursor works and tracks (M[0o426/0o427] cursor cells set; M[0o420] DASTART
=0o31426), but it never paints its menu -- the display bitmap stays
uninitialized (page-0 garbage = the "noise"). User confirmed: typing,
clicking, mouse-move all leave the cursor fine but draw nothing else.

KEY FINDING (this session): the page-0 OS cells the loop reads
(0o103/0o112/0o130/0o164, in the 0o100..0o170 block) are written EXACTLY
ONCE -- at cyc ~63.79M by the loader (pc=0o1622) populating the boot
image -- and NEVER again during the poll. So CedarNetExec is busy-waiting
for an INTERRUPT HANDLER to update one of those cells, and that event never
fires in our model. (Store trace confirms; note DORADO_STORE_TRACE_VA is
NOT gated by DORADO_TRACE_GATE -- it shows all writes in the VA range.)
Since the cursor tracks the mouse, mouse handling works; the missing event
is most likely the RTC/interval-timer, keyboard, or display-vertical
interrupt. NETEXEC (same Alto OS) gets past this and paints, so the lever
is the delta: which interrupt/cell NETEXEC has that CedarNetExec waits on.

NEXT (where the prior agent stalled -- it needs a Nova disassembler, not a
microcode trace): decode the ~6-PC loop. VM PC = br31(hex) + pcf/2 from
IFUDISP. Sampled loop br31 values: 05088, 05801, 00001, 00B49, 0717.
CedarNetExec.boot loads 1:1 (file word N = VM word N), so disassemble it
(Alto/Nova: LDA/STA/JMP/JSR/ISZ/DSZ/SIO+ALC) at those VM addrs to find the
exact cell tested and branch taken; then identify which interrupt should
write it (RTC at the page-1 timer cells? keyboard? DVI?) and whether our
engine posts that interrupt to the running Alto world. Repro:
  DORADO_IFUDISP_TRACE=1 DORADO_TRACE_GATE="146000000,146010000" \
  ./build/dorado --eb worlds/aemu.eb \
    --eftp '../chm/bootfiles/CedarNetExec.boot!4' --cycles 146050000
  DORADO_STORE_TRACE_VA="0100,0170" ... (writes to the polled cells)

## FIXED (2026-06-13e): CedarNetExec's post-load hang was a Pipe-VA clobber on a page-377 map fault

Forensic trace of the CedarNetExec hang (after the leader-page fix it loaded
but its display was noise and it did NO further work). Root cause found and
FIXED (pipe_push in memory.c):

- After load, CedarNetExec's Nova relocation BLT stores into MDS bank-0
  page 377 (e.g. VA 0o177653 / 0xFFAB). AEmu intentionally write-protects
  page 377 (XMFaultTask.mc), so the store map-faults. The handler's design
  (Fault0) is: bank-register store (0o177740..) -> handle; any OTHER page-377
  store -> IgnoreStore (drop it, restart). CedarNetExec's BLT hits "other".
- Our engine instead ran Fault0 to `MapFault: Branch[.]` -- a self-loop --
  and task 0 spun there forever (hot uPC real 0o4046), dispatching ZERO Alto
  opcodes. The display "pixels" were stale residue; the VM was frozen.
- Why: Fault0 reads the faulting VA from pipe[ProcSRN] (VAHi/VALo). The
  faulting store pushed VA=0xFFAB into pipe[ProcSRN], but the very next IFU
  prefetch (also task 0, same fixed ProcSRN slot) overwrote it with the next
  opcode's VA (0x5034) BEFORE the fault task read it. So Fault0's page-377
  test failed -> MapFault. On real Dorado consecutive refs get distinct SRNs
  (16-entry ring), so the faulting entry survives at SRNFirstFault.
- Fix: pipe_push now refuses to overwrite the first-faulting ProcSRN slot
  while a fault is pending (released on dorado_fault_clear). ASRN ring
  unaffected. `make test` 10/10; NETEXEC still boots (1490 px, no regression).

RESULT: CedarNetExec now RUNS -- it dispatches Alto opcodes continuously,
the Alto VM PC advances through real code, and it settles into an input/poll
loop (br31 in {05033,05088,05801} + page-0 cells; it animates a counter at
display word 0x2D61 and re-stores 0o177653 each pass, all IgnoreStore'd).

NEW FRONTIER: it does not paint a clean menu. The display is still dense
noise and rx=0/tx=0 (no network). DASTART(M[0o420])=0o31426 but the chain
there does not parse as a clean Alto DCB (w2/SA=0o22 is page 0), so either
CedarNetExec lays out its display differently than NETEXEC's BCPL DCBs, or it
is parked waiting (keyboard/gap E2, or a clock/network event) before drawing.
Next: identify the poll's exit condition (what page-0 cell it waits on and
who should update it -- keyboard interrupt? RTC?), and whether the rasterizer
must follow a different display structure. Trace hooks added: DORADO_PIPEVA_TRACE
(prints pc/psrn/va at each B<-Pipe0/Pipe1 read) and a cycle stamp on FAULT_CPU.

## DIAGNOSED (2026-06-13d): CedarNetExec stall is a boot-loader handoff CRASH (fix in progress)

Forensic trace nailed the CedarNetExec stall (and all four other 000345
Mesa-format boot files -- MesaNetExec, AlphaMesaMesaNetExec, MazeWar,
NEWOS -- which all stall identically). It is NOT a device poll-wait: it is
a CRASH. After the verbatim EtherBoot loader (`eth_bol_loader[254]`,
ethernet.c:535; final `JMP@0` = the `02000` at ethernet.c:567) restores the
boot file's saved page-0 (file words 0o400..0o777 -> M[0..0o377]) and does
`JMP@0`, M[0] = file[0o400]:
- NETEXEC.BOOT (works): file[0o400]=0o001311, a real low-memory code entry.
- CedarNetExec.boot (crashes): file[0o400]=0o165054, a DATA pointer into
  its boot-menu/file-descriptor table (records + strings at 0o165000+).
  M[0o165054]=0, so JMP@0 runs a zero word -> trap -> the Alto PC runs away
  through the page-0 pointer table forever (the "page-0 spin").
CedarNetExec's header is 0o345 0o354 0o403 (NETEXEC: 0o405 0 0); file[0]=
0o345 points at a relocation bootstrap at file[0o345] (`LDA1 0o110; COM 1,3;
BLT; STA3 @0o111; JMP 3`) that is NEVER reached. Load is faithful (live VM
== file; full transfer lands). So: not corruption, not truncation, not a
device poll.

FIXED (commit a125bd4): it was neither (a) nor (b) -- it was a SERVER-side
boot-file format issue. The standard Alto Ethernet loader loads only
B-format files; a real boot server prepends a 256-word leader page to the
non-B-format (000345 Mesa/Pilot) files before serving (ETHERBOOT.BRAVO).
Our server (and the reference IFS server) sent them verbatim, so the image
loaded shifted one page and M[0] got file page 1 (garbage 0o165054).
`eth_queue_eftp_boot` now prepends a synthesized leader page (word0=0o405,
word1=0) for any file whose word0==0o000345; B-format files are untouched
(no regression). All five 000345 worlds now LOAD AND RUN into real code:
CedarNetExec builds a ~50K-pixel display (was 0); MesaNetExec/MazeWar/NEWOS
advance into real code with no page-0 runaway.

NEW FRONTIER: the Mesa-format UI. They run now but do not reach a clean
screen. CedarNetExec: DASTART != 0, ~50K px but the display is garbage/
noise (display buffer not yet filled / blocked on display-list install +
the keyboard back-channel, gap E2). MazeWar: DASTART != 0 but 0 px and PC
wandering page 0 -- LESS far than CedarNetExec. So CedarNetExec is the best
vehicle (furthest along, highest value). Trace hooks in machine.c:
DORADO_TRACE_GATE, DORADO_VMDUMP, DORADO_IFUDISP_TRACE, DORADO_STORE_TRACE_VA.

## RESOLVED + NEW FRONTIER (2026-06-13c): the whole NetExec chain works; CedarNetExec stalls on startup

The interrupt-driven Pup-receive blocker below (2026-06-13b) is FIXED. Root
cause: our socket-reply builders omitted the trailing hardware-CRC word
that the AEmu receive microcode (AltoEther.mc EIEnd) unconditionally
subtracts, so packetLength came out one short and EtherPupFilter rejected
every reply. Adding the dummy CRC word (matching append_reply /
append_eftp_packet) made it land. CONFIRMED end to end:
- NetExec learns its net (banner [0#42#] -> [1#42#]) and sets its clock
  (now the real host wall-clock time, derived from the host TZ/DST).
- `?` lists CedarNetExec; typing it Mayday-requests bfn 111B; the server
  serves CedarNetExec.boot; the EtherBoot loader takes over (screen clears).
So the documented Cedar-over-Ethernet path (DoradoBooting 1.3: NetExec ->
CedarNetExec) is functional through the load.

NEW FRONTIER: CedarNetExec.boot ("XEROX Cedar Net Executive 6.0c", an
Alto-format program -- readable Alto strings, references CedarDorado.eb /
.eg germ / Othello / PhysicalVolumeBoot) transfers in full (65280 words,
255 EFTP packets, eftp_q=257 seq=256) but does NOT come up: DASTART=0 (no
display list), rx=0/tx=0 (no network I/O), PC wandering in page 0/1
(0o101..0o665). It stalls BEFORE any network activity, so the receive fix
does not affect it -- this is a fresh early-startup bring-up problem, like
NetExec's page-zero bug was. Repro WITHOUT needing to type (serves it via
the breath-of-life path, same loader + image):
  cd dorado && ./build/dorado --eb worlds/aemu.eb \
    --eftp '../chm/bootfiles/CedarNetExec.boot!4' --cycles 350000000 \
    --progress 2>&1 | grep -iE 'pixels|DASTART|booted'
Next: trace what CedarNetExec is doing in its page-0/1 spin loop (is it
waiting on a device/memory cell that never changes, or did it crash into a
loop?). Same forensic approach that cracked the receive bug. NETEXEC.BOOT
disassembles 1:1 (file word = VM word); CedarNetExec.boot likewise.

## NEXT FRONTIER (2026-06-13b): NetExec's interrupt-driven Pup receive does not deliver socket replies (FIXED -- see above)

Goal: boot Cedar the documented way (DoradoBooting.tioga 1.3) -- get into
the Alto NetExec, type `CedarNetExec<CR>`, which net-boots CedarNetExec
(then Othello / the desired Cedar program). This needs NetExec's network
stack to actually RECEIVE Pup socket replies. It currently does not.

WHAT IS DONE AND COMMITTED (all spec-grounded, suites green, NETEXEC still
boots to its prompt and typing works):
- **Boot-directory protocol.** `--boot-dir NAME=BFN=PATH` (repeatable) ->
  the fake server answers NetExec's BootDirReq (257B) with a BootDirReply
  (260B) listing {bfn, date, BCPL name}, and serves the matching file by
  boot file number on the follow-up Mayday (244B). The 260B packet is
  byte-perfect vs the IFS boot server (PUP/MiscServicesProtocol.cs
  SendBootDirectory + BCPLString.ToArray): BFD blocks at the first Pup
  content word, name `[len][chars]` padded to even.
- **Real Pup checksum.** `pup_checksum()` in ethernet.c (ones-complement
  sum, end-around carry, left rotate, neg-zero normalize) ported from IFS
  PUP.CalculateChecksum, stamped on the bootdir/gateway/time replies. The
  old 0177777 "skip" sentinel is gone.
- **Documented routing/time replies** replacing the dead 0204 handler:
  GatewayInformationRequest 200B -> GatewayInformationResponse 201B whose
  destination net teaches NetExec its net (PupRoute.bcpl
  ProcessRouteInfoReply); AltoTimeRequest 206B -> AltoTimeResponse 207B
  (was wrongly 205B, which InstallTime rejects).
- `--boot-file-number` CLI flag; the STK boot-parameter block is now
  configurable (machine.c) instead of hardcoded.

THE DIAGNOSTIC THAT ISOLATES THE BLOCKER: with EVERY reply now byte- and
type-correct (routing, time, bootdir), NONE land -- the banner still shows
`[0#42#]` (net 0, UNLEARNED), "Date and time unknown", and `?` does not
list CedarNetExec. So it is NOT a packet-format problem. The replies are
READ (the DORADO_BOOTDIR_DEBUG "260b reply CONSUMED" line fires, which
requires the receiver armed) but never delivered to NetExec's contexts
(GatewayListener / GetTime / GetDir).

WHY EFTP WORKS BUT THIS DOES NOT: the EFTP boot loader (Taft EtherBoot)
POLLS ePLoc directly. NetExec's full driver is INTERRUPT-DRIVEN:
`EtherInterrupt` in chm/altosource/pupsources.dm!4_/PupAlEthb.bcpl. So the
EFTP transfer succeeding never exercised the interrupt-driven receive path
NetExec uses.

THE TWO CONCRETE SUSPECTS (both checkable by tracing the running NetExec):
1. `EtherInterrupt` never fires -- AEmu posts the received-packet status
   (the loader polls and sees it) but does not raise the Alto Ethernet
   interrupt, so NetExec's handler never runs. The received packet sits in
   the buffer unprocessed.
2. `EtherInterrupt` fires but rejects -- PupAlEthb.bcpl accepts a packet
   only when status `lastEPLoc eq 377b`; if AEmu posts any other value it
   is "bad packet, throw it away." Then it runs the predicate filters
   (EtherPupFilter: `(pup.length+5) rshift 1 eq packetLength &
   EtherPBI.type eq typePup`) and enqueues to pbiIQ; the PupLevel1 context
   (Pup1Init.bcpl) demuxes pbiIQ to the dest socket.

KEY SOURCE (now in the tree): chm/altosource/pupsources.dm!4_/ --
  PupAlEthb.bcpl (EtherInterrupt, FeedEther, EtherPupFilter),
  PupRoute.bcpl (GatewayListener, ProcessRouteInfoReply, the routing
  table), Pup1Init.bcpl (InitPupLevel1, the PupLevel1 demux context,
  pbiIQ/socketQ). IFS reference clone (gitignored) at repo-root /IFS:
  PUP/PUPProtocolDispatcher.cs (demux), PUP/Transport/Ethernet.cs,
  PUP/Gateway/GatewayInformationProtocol.cs, PUP/MiscServicesProtocol.cs,
  PUP/PUP.cs (PupType enum + CalculateChecksum).

NEXT STEP: trace whether NetExec's EtherInterrupt runs when our reply
arrives and what ePLoc status it sees. Find EtherInterrupt's VM address
(disassemble NETEXEC.BOOT 1:1, or trace from the InitAltoEther interrupt
setup), gate a STORE/IFUDISP trace around a 200B/206B/257B reply delivery,
and confirm (a) the interrupt is taken, (b) the status word presented is
0o377, (c) the packet reaches pbiIQ, (d) PupLevel1 demuxes it to the
socket iQ. The first link that fails is the bug. Reproduce with:
  cd dorado && DORADO_BOOTDIR_DEBUG=1 ./build/dorado --eb worlds/aemu.eb \
    --eftp '../chm/bootfiles/NETEXEC.BOOT!8' \
    --boot-dir 'CedarNetExec.boot=111=../chm/bootfiles/CedarNetExec.boot!4' \
    --cycles 150000000 2>/tmp/bootdir.log
(SDL: build/dorado-sdl, same flags; type `?` to list boot files, expect
CedarNetExec once the receive path delivers.)

## ROOT CAUSE FOUND (2026-06-13): the page-zero / divide-vector corruption is FIXED

The long-standing M[0o344] divide-vector clobber / page-zero BitBlt spray
(chased across sessions 8-10b, papered over by the divide-vector guard,
and the cause of the user's "types a little bit then stops responding")
was a single microarchitecture bug, now fixed (commit cf129c1):

  The RM/STK write address is latched at instruction issue, but the
  emulator applied a same-instruction `RBase<-FF` change BEFORE the write.
  AEmu BitBlt `BBNormal: BBTemp_ (BBTemp)+MD, RBase_ RBase[AEmRegs]`
  computes the destination base (= displacement + DBCA) and stores it to
  BBTemp in the BBRegs region; with the FF-changed RBase the result landed
  in AEmRegs instead, so BBTemp kept displacement-only and DBCA was
  DROPPED. The BitBlt destination then missed the bitmap and the
  bottom-to-top gray fill sprayed page zero. Fix: the LC write uses the
  issue-time RBase (cpu.c execute_uinstr).

  Found via a forensic BBT dump (DORADO_BBT_TRACE at real 0o3124): the
  BitBlt table was SANE (DBCA=0o122330, DBMR=46, DTY=0, DH=14) yet the
  destination dropped DBCA -- isolating the RBase-in-same-instruction
  write rather than garbage inputs. The single-opcode differential harness
  (altodiff-dorado sweep, 20768 vectors) had first proven the steady-state
  opcode emulation correct, narrowing the search to exactly this kind of
  intra-instruction timing.

VERIFIED: all 10 suites pass; with NO divide-vector guard
(DORADO_NO_DIVPROT=1) the world is stable to 200M -- M[0o344] stays
0o4155 (never 0o4154), Swat OutLdRet stays 0, typing "Probe" survives,
the banner renders. The old ~190M FillWithDash Swat is also gone (same
root). The divide-vector guard is now UNNECESSARY (being retired).

# (original handoff below) Alto-on-Dorado boot bring-up (2026-06-11, session 2)

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
DIVIDE-VECTOR GUARD keeps NetExec alive past the BitBlt crash
(2026-06-12): the periodic Title BitBlt that sprays the OS low core
in page zero corrupts the M[0o344] divide transfer vector, which is
what crashed the booted world into Swat ~124-190M (the user saw
"types a little bit then stops responding"). Mitigation: a
physical-cell write guard in memory.c (protect_phys/protect_val,
forced in the DM_REF_STORE path) armed by the machine once NetExec's
OS init has set M[0o344] to a plausible OS-resident code address
(0o2000..0o7777, captured ~63M). Verified: with the guard,
M[0o344] stays 0o4155 and Swat OutLdRet=0 at 160M (not crashed);
typing "Probe" at 110M echoes at the ">" prompt in a 200M snapshot.
This is a BRING-UP WORKAROUND. (SESSION-11 UPDATE: the BitBlt
destination-BR arithmetic is NOT the bug -- it is correct and
faithful to hardware; BR[0o22] wraps to page zero because the BBT
the caller passed already points there. The real fix is the
upstream BBT/context corruption. See SESSION-11 below.) It guards
only the divide vector; other
sprayed cells (0o412/0o460 MASKTAB/0o526/0o574/0o642) are not yet
protected but do not appear to crash the world.

LIVE SDL WINDOW + KEYBOARD + MOUSE (2026-06-12): the emulator now
has a windowed frontend. `make sdl` builds build/dorado-sdl (SDL2,
pure C; the core stays C99). It boots the machine and presents the
808x606 Dorado display in a window, rasterizing the Alto display
list from memory each frame; host keyboard is translated to the
Alto key matrix and host mouse to the Alto mouse cells, both
delivered to the running world once it is interactive. Run:
  make sdl && ./build/dorado-sdl --eb /tmp/aemu_only.eb
Flags: --eb --eftp --quote --no-alto-boot --scale N --speed CYCLES.
F1 pauses; Cmd/Ctrl+Q quits. Machine API added (machine.h):
dorado_machine_set_key (dorado_display_key), dorado_machine_set_mouse
(x,y,buttons; MOUSEX 0o424 / MOUSEY 0o425, buttons UTILIN
0o177030-0o177033 active-low, Left=4/Middle=1/Right=2 per ContrAlto),
dorado_machine_interactive. The machine forces the boot keys
(BS-down) until the EFTP transfer starts (eftp_max_seq>0 = boot
selection done), then releases to live host input -- so typing
reaches NetExec's command line. Mouse cells (MOUSEX/Y) and the
ReadTerminal 7-wire message map were taken from AEmu DisplayAux.mc
(msg types: 1-4 kbd 177034-177037, 5 buttons 177033/177030, 6 mouse
deltaX/Y, MouseXLoc=424) and salto/ContrAlto. Headless `dorado`
still renders the banner (2070 px) and all 10 suites pass after the
seed-block change. NOTE: NetExec is keyboard-driven and may not move
the cursor from the mouse; the mouse plumbing is correct for OSes
that read MOUSEX/Y. Next: a windowed live display naturally wants
the cycle-accurate DWT path (still iofetch=0) for an authentic
self-refreshing screen rather than the per-frame display-list
rasterizer; and an optional ImGui debug target.

STANDALONE BINARY BOOTS THE BANNER (2026-06-12): SOLVED. The
missing piece was the Mesa boot-parameter seed: the harness seeds
STK[1]=0110 (boot file number), STK[2]=056623 (BootParameterSeal),
STK[3]=0121045 during Initial (pre_pc 06170-06217 / 06406-06443,
force_ether_mesa_boot defaults ON) so the loaded world selects the
normal Mesa boot instead of falling into the cold/NoStorage path.
Ported into src/machine.c. The standalone `dorado` binary now boots
NetExec over Ethernet end-to-end: rx=1, bol=1, EFTP transfer
complete (88 packets, seq 87), DASTART=176762, and the
"-- XEROX BCPL Net Executive" banner rasterizes (2070 pixels,
identical to the harness). Run:
  ./build/dorado --eb /tmp/aemu_only.eb --cycles 130000000 \
                 --out screen.pgm
The diagnostic env knobs (DORADO_MACHINE_TRACE / _PCHIST /
_INITMEM / _NO_ALUFM_RESTORE, config_word in --progress) remain
for future bring-up. Decisive technique that found it: PC_COUNT +
PCWATCH side-by-side between harness (works) and machine (NoStorage
loop) - both hit InitMap 0o1011 with identical T=000025 but the
harness arrived via the warm path (STK boot parameter present) and
the machine via the cold path, diverging at the 0o1011 branch.

STANDALONE EMULATOR (2026-06-12): the first runnable binary outside
the test harness now exists. include/machine.h + src/machine.c lift
the boot orchestration out of test_cpu.c's probe_full_boot_with_
bootstrap into a reusable library (dorado_machine_create/run_until/
render_display_list/booted/debug); src/dorado.c is the CLI frontend
(build/dorado). Build: `make build/dorado`; run:
  ./build/dorado --eb /tmp/aemu_only.eb --cycles 124000000 \
                 --out screen.pgm --progress
Status: boots the full hard chain through STAGE 1 (BaseBoard cold
boot, 3 boot-button presses, Bootstrap swap @0o7740, canonical
Initial substitution @0o7717, PresetMap identity-map shim @0o6222,
CheckChecksumAndLoad @0o6460, LoadRam handoff -> world loaded @~32M,
booted=1, eth requests=1 replies=63). Library sizes storage to one
4MW module directly (no DORADO_STORAGE_MODULES dependency) and
seeds boot keys / parks I/O tasks in-code.
STAGE 2 remaining gap (precisely localized): at the LoadRam handoff
the machine now parks tasks 1..15 (invalid TPC, junk off) and gates
display wakeups on DASTART, so the EMULATOR task (0) runs in the
loaded world instead of being starved by stale-TPC I/O tasks (was
task 3 then task 2 spinning at 0o7003/0o234). Task 0 advances
0o5400 -> 0o7116 -> 0o7012 then STALLS at ~0o7012 before turning on
the Ethernet receiver (rx stays 0) or setting DASTART, so
breath-of-life is never accepted (bol=0) and the banner never
draws from the standalone path. FINDING-3 (ROOT LOCALIZED to InitMem NoStorage): a side-by-side
PC_COUNT comparison nails it. The harness and the standalone
machine BOTH hit AEmu InitMap (0o1076) exactly once at cycle ~31M,
but the harness then hits the loop addresses (0o7140/0o7167/0o5400/
0o7012/0o7116) ZERO times while the machine loops there millions of
times. The machine's init sequence shows it reaches 0o1011
(GOTMAPCONFIG) and branches to 0o1017 (NOSTORAGE) - InitMem.mc
decides NO storage modules are present and falls into the NoStorage
breakpoint loop. InitMem.mc GotMapConfig does:
  ModMask_ NOT(Config'); ... ModMask_ LSH[ModMask,10];
  ModMask_ T_ (ModMask) AND (170000C);
  ITemp1_ T, Branch[NoStorage, ALU=0];
RULED OUT (each measured): Config' itself is CORRECT - the machine
prints config_word=0o214 (= module-0 present + 64Kx1 chip), so
NOT(Config')=0o214 and after LSH/AND it should be nonzero (storage
present). Storage is correctly 1 module (4194304 words). Skipping
restore_standard_alufm (DORADO_NO_ALUFM_RESTORE=1) does NOT change
it. Fault-task park, identity-map shim: irrelevant. So config_word
is right but the GotMapConfig computation still yields ALU=0 ->
NoStorage. The remaining divergence is engine REGISTER STATE at the
~6 GotMapConfig instructions (ModMask/ShC/T/ALU flags) - the
machine and harness must differ in one register feeding the
LSH/AND/ALU=0 test, OR in the SetDMuxAddress muffler reads
(0o7000-0o7076) that run just before GotMapConfig (these set
VirtualBanks and clobber T/XTemp17 via MidasStrobe; if the
machine's muffler model returns different data the path differs).
NEXT (decisive): instrument BOTH at GotMapConfig - dump ModMask,
ShC, T, ALU=0 right before the Branch[NoStorage] in the harness
(works) vs the machine (NoStorage) and diff the first differing
register. Suspect the muffler/DMux read (SetDMuxAddress) since
that is the least-tested path and runs immediately before.
Tooling added: config_word in --progress debug;
DORADO_NO_ALUFM_RESTORE=1 to A/B the ALUFM restore.
OLD-FINDING-2 (DORADO_MACHINE_PCHIST init sequence):
the world starts at AEmu InitMap (1070->1076), runs a long init
clearing loop (4426 4436 4436 4437 x N), then DELEGATES to a shared
InitMem routine in INITIAL's address range and gets stuck in a
repeating loop: 7140 7167(x9) 7166 7172 7175 ... 7065 5400 5430
5434 5450 5454 5470 5474 7066 7064 7160 7171 (these addresses are
Initial/kernel leftovers; AEmu.mb has no code there, and
aemu_only.eb is AEmu-ONLY so it relies on the substituted
init_mc Initial/kernel/memMisc routines - the SAME setup the
harness uses to reach the banner). Confirmed NOT the cause:
storage is correctly 1 module (stwords=4194304); removing the
fault-task park did not change it; the identity-map shim ran.
So the loop is a shared InitMem/InitMap routine that terminates
in the harness but not here. The remaining divergence is in the
post-LoadRam world execution and needs a SIDE-BY-SIDE task-0 PC
comparison: run the harness (test_cpu) with the identical env
(DORADO_ALTO_BOOT_ETHERNET=1 DORADO_NO_DISK=1
DORADO_ETH_BOOT_110=/tmp/aemu_only.eb DORADO_STORAGE_MODULES=1)
and a focused PC trace over 5400/7140/7167, and diff against the
machine's init sequence to find the first instruction where they
diverge (likely a register/Cnt/ALUFM value feeding the loop's
exit test). Tooling: DORADO_MACHINE_PCHIST=1 --progress dumps the
machine's task-0 hot PCs + first-600 init sequence + storage size.
OLD-FINDING: the AEmu world DOES start (task 0 reaches AEmu
real 0o1070, which carries the Multiply FF), but it then LOOPS in
KERNEL-level microcode at 0o7003-0o7167 (these addresses are
present in kernel.mb/Initial.mb, ABSENT in AEmu.mb) instead of
completing its init and reaching EBoot. Hottest task-0 PC 0o7116
(BLOCK=1 emulator op). So the world boots and faults/waits into a
kernel handler rather than progressing - a different failure mode
from the harness, which reaches the banner. Tooling added:
DORADO_MACHINE_PCHIST=1 dumps the top task-0 PCs of the loaded
world; DORADO_MACHINE_TRACE=1 logs boot-state transitions.
NEXT: side-by-side compare the loaded-world execution against the
harness (same env, DORADO_PC_COUNT) to find where they diverge -
prime suspects: (a) my world-loaded detection fires mid-LoadRam
(32M; CLAUDE.md notes the ether boot costs ~61M) so the
task-park / ALUFM-restore happens too early and corrupts the
world's init; (b) the world's emulator loop dispatches to an I/O
task I parked (junk/fault) and waits in kernel for it; (c) an
unhandled fault traps to the kernel handler at 0o7116. Try: defer
the world-loaded handoff (task-park + ALUFM restore) until task 0
actually reaches AEmu's start region for a sustained run, rather
than the first non-Initial PC. OLD-NEXT: trace task 0 from world-load
to the 0o7012 stall (DORADO_MACHINE_TRACE + a PC histogram) and
compare against the harness's loaded-world init - the harness
NATURAL path did NOT reset TPCs yet still reached the banner, so
either (a) my world-loaded detection fires earlier than the
harness's (restoring ALUFM / parking tasks mid-init), or (b) the
loaded world needs an I/O task I parked (junk timer? fault task?)
to make progress past 0o7012, or (c) the keyboard BS-down seed
isn't seen at 0o7012's boot-decision read (MDS bank). The banner
ITSELF renders correctly from the test harness (commit 6bbdf7d);
this is purely about reproducing it from the standalone binary.

MILESTONE (2026-06-12): FULL SCREEN DRAW ACHIEVED. The NetExec
banner "-- XEROX BCPL Net Executive" + machine/date line now
rasterizes to the framebuffer. Mechanism: a direct display-list
rasterizer in probe_full_boot (tests/test_cpu.c, just before the
snapshot) walks DASTART(VM 420)->DCB chain straight from MDS memory
- Alto DCB format w0=next, w1=ctl(b15 res,b14 inv,b8:13 HTAB words,
b0:7 NWRDS), w2=SA bitmap word addr, w3=SLC (scanlines, rendered
x2 vertically) - and paints each DCB band with dorado_display_set_
pixel (MSB=leftmost, 1=black). At a 124M-cycle budget (BEFORE the
page-zero/divide crash at 124,025,617) NetExec is healthy and the
display list holds two 38-word x 6-SLC text bands (the title +
machine/date lines, DCB[1]@142540 DCB[2]@140602) over a 42-SLC
blank strip. dl_pixels=2070. Snapshot: /tmp/dorado_dcb.pgm; ASCII
reference checked in at docs/netexec-banner.txt.
This is a SNAPSHOT rasterizer (reads the display list NetExec
built); it is NOT the cycle-accurate DWT path. The real DWT word
task still barely runs (iofetch=0, dwt_wakeups=1) because the DHT
(task 3) does not repeatedly set the WCB/raster trigger flags that
dorado_display_dwt_wakeup() needs - that remains the open item for
a hardware-accurate live display. Two remaining gaps for a
self-sustaining boot: (a) DHT->DWT wakeup chain (display.c
next_wcb_flag/raster_next_wt_flag are set from DHTFlag/RAST_TASKCMD
writes but the DHT isn't issuing them per scanline), and (b) the
BitBlt page-zero destination crash at 124M (BBT with a page-zero
DBCA; see SESSION-10d).

SESSION-11 (2026-06-12): TWO RESULTS -- (a) implemented the stubbed
XorCarry/XorSavedCarry FF functions (cpu.c); (b) PROVED the BitBlt
destination-BR arithmetic is CORRECT, redirecting the page-zero hunt
upstream. Do NOT keep chasing the "BitBlt destination-BR math" -- it
is faithful to hardware. Evidence:
- XorCarry/XorSavedCarry (HM Table 11a, FA=0 FB=2 FC=4/5) were
  no-op stubs. Now implemented: they XOR the carry-in (bit 5 of the
  ALUFM entry) before the ALU runs -- XorSavedCarry with the SAVED
  (previous-instruction) carry, XorCarry unconditionally. The BitBlt
  bottom-to-top base math (T_ T-1, XorSavedCarry; BBNormal
  T_ T+EmuBRHiReg, XorSavedCarry) is the canonical user. All 10
  suites pass; banner pixel count unchanged (1478 px at 124M, pre ==
  post), so no regression.
- Forensics on the M[0o344] corruptor (DORADO_STORE_TRACE_PHYS=344,
  new phys-keyed store trace in memory.c): the legit OS write sets
  M[0o344]=0o4155 (pc=0o1622, mb=36/MDS, br=0, mar=0o344) at ~64M;
  the corruptor at cyc 122,993,593 is pc=0o1201 (GRAYLOOP), mb=22,
  br=0o1777700642 (=0xFFF81A2: br_hi=0xFFF, br_lo=0x81A2), mar=0x7F42.
  (br+mar) & 0x0FFFFFFF wraps at 2^28 to 0o344 -> writes 0o4154.
- The XSC trace (PC range 0o1405-0o1426) shows the math is correct:
  pc=0o1424 BBTemp-0x8000 BORROWS (savedC=0), so pc=0o1423
  T_ T-1, XorSavedCarry correctly yields T=0o177777 (the intended
  bottom-to-top bank decrement). The 12-bit br_hi + 28-bit BR-adder
  wrap then brings the bank back to 0 exactly as real hardware does.
  So va=0o344 is the FAITHFUL result of the given register inputs --
  the destination base the BBT specified is itself in page zero.
- CONCLUSION (confirms SESSION-10b/10d): the root is an upstream
  malformed BBT whose destination is page zero, traced to a corrupt
  BCPL context stack pointer (ctx!1) running with its frame in page
  zero. The divide-vector guard stays as the bring-up workaround
  (now also gated by DORADO_NO_DIVPROT for A/B testing); it is NOT a
  microcode arithmetic bug. NEXT: resume SESSION-10b's hunt for who
  writes the bad ctx!1 (STORE_TRACE_VA on the context block), i.e.
  the allocator/context-overlap corruption, not the BitBlt.

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
