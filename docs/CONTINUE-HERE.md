# Continuation handoff — Alto-on-Dorado boot bring-up (2026-06-11)

Read this first if you are a fresh Claude session resuming the Dorado
Alto-emulator boot work. Project-wide context is in `/CLAUDE.md` and
`dorado/CLAUDE.md`. This file supersedes the 2026-06-10 version: every
issue in that handoff is FIXED, plus seven more found behind it. The
session theme was "the emulator took shortcuts; verify against the HM
and the real microcode sources" — every fix below cites its source.

## One-line status

The boot now runs: BaseBoard -> Initial -> Ethernet microcode boot ->
AEmu world -> InitMap -> ABoot -> EBoot -> breath-of-life packet
received -> **the emulated Alto executes the real Taft EtherBoot loader**
(parity sweep, page-1 copy, Mayday packet built, eHLoc/eOPLoc/eOCLoc all
correct) — and derails at the loader's `mkone 0 0; sio` (start
transmitter): the SIO control bits arrive as 0, so the transmitter never
starts, the loader times out through its 24 retries and crashes to
`jmp 0`. THE OPEN QUESTION is why AC0=0 after `mkone` (details below).

## One-command repro

```
cd dorado && make build/test_cpu build/mb2eb && \
./build/mb2eb '../chm/dorado/AEmu.mb!2' /tmp/aemu_only.eb 01076 && \
DORADO_NO_DISPLAY_WAKE=1 DORADO_ONLY_FULLBOOT=1 DORADO_NO_DISK=1 \
DORADO_STORAGE_MODULES=1 DORADO_ETH_BOOT_110=/tmp/aemu_only.eb \
DORADO_ALTO_BOOT_ETHERNET=1 \
DORADO_ETH_EFTP_BOOT='../chm/bootfiles/NETEXEC.BOOT!8' \
DORADO_BOOT_BUDGET=200000000 ./build/test_cpu 2>&1 | \
grep -E 'Stage-2|task cycles|Final PC|Ethernet:'
```

Expected today: `eftp_requests=0` (no Mayday reaches the server),
task-0 hot PCs show AEmu JMPZ/JMPX/NOSKIP at mar=0 (the `jmp 0` crash),
`bol>=1` (breath-of-life delivered and consumed). When fixed: the EOT
transmits the 13-word Mayday (`eftp_requests=1`), the EFTP server
streams NETEXEC.BOOT, the loader chains packets and jumps into NetExec.

Notes:
- `DORADO_NO_DISPLAY_WAKE=1` is currently REQUIRED: with display wakeups
  on, the (garbage-DCB-driven) display tasks wander into DWTREFILL with a
  stale MemBase and a wild `BrHi` clobbers a BR (see "Parked issues").
- The budget must be >=200M BB cycles; the loader's interruptible BLT
  parity sweep is slow (one word per Reschedule-check round trip).

## Fixed this session (all suite-green, in working tree, NOT committed)

1. **RM write-address FF groups** (`src/cpu.c lc_write_address`). HM
   Table 11d (FA=2 FB=2-3): write address = `FF[4:7],,RSTK` (was using
   the Table 11a `RBase,,FF[4:7]` form for both groups). This was the
   2026-06-10 open question: `EmuBRHiReg_ T` (FF=0o221) now hits RM[0o30]
   and the BRs come up in bank 0. Also added HM §3.6 gating (FF function
   only when BSEL non-constant and JCN not long) + `ff_is_table11a()`.

2. **Ethernet controller control register** (`src/ethernet.c`). Replaced
   the Stage-1 heuristics with the real EthC decode per HM §11 Figure 16
   (TxCmdEnbl'/TxOn/TxEOP/TxCntDwn, RxCmdEnbl'/RxOn/RxBOP', NoWakeups);
   packet transmit completion = the SendEOP command (TxOn+TxEOP, with
   TxGone clearing TxEOP for the end-of-packet wakeup); wakeup mask per
   HM ("TxOn && !TxEOP && !TxCntDwn", "RxOn && word available"). The
   observed command constants match InitialEther.mc exactly (TurnOffRx
   0o170377, TurnOffTx 0o7777, TurnOnRx 0o173377, TurnOnTx 0o47777,
   SendEOP 0o67777). This stopped EOT/EIT wakeup storms that starved the
   junk task (RTC) and made ABoot hang.

3. **Breath-of-life server** (`src/ethernet.c eth_bol_loader[254]`,
   `dorado_ethernet_breath_of_life()`, harness rebroadcast in
   `tests/test_cpu.c`). EBoot transmits nothing: it waits for a
   broadcast boot-loader packet (ether type 0o602). The 254-word loader
   was extracted from `EtherBoot.mesa` (research downloads; "Snitched
   from Taft's EtherBoot.asm") — word-for-word, with the inline listing
   as provenance. Served when rx is on and idle, rebroadcast ~2M cycles.

4. **IOAtten is a level, not a latch** (`src/ethernet.c eth_attention`,
   `src/cpu.c io_atten_at_issue`). HM §11: "IOAtten branches when a
   status word is present in the receiver bus register". Sampled at
   instruction issue so `Input + IOAtten` in one instruction tests the
   word being read, while the input main loop's store instruction tests
   the next unread word (both AltoEther.mc patterns).

5. **FaultInfo encoding** (`src/memory.c dorado_fault_info`). Per
   EMemDefs.mc + HM §5: b8=EmulatorFault (0x0080), b9:11=FaultCnt where
   the 3-bit field holds count-1 (all ones = none), B[12:15]=SRN.
   XMFaultTask.mc's dispatch now reaches the XM write-protect store
   emulation instead of wedging at NotEmuFault.

6. **IFUM IFaddr' is low-true; halves and routing** (`src/cpu.c`,
   `src/mb2eb.c`). Dispatch = `(~RH & 0x3FF) << 2 | n` — verified:
   AEmu opcode 045 (LDAipc) stores RH=0o1772, ~0o1772&0x3FF=5, LDAIPC
   is at IM[0o24]. IFUMRH<-B writes the address half (ifum_lo), IFUMLH<-B
   the fields half (ifum_hi) per HM Table 11c; mb2eb emits items as
   (word0=LH, word1=RH) because LoadRam.mc loads word0 via IFUMLH.
   Before this, post-LoadRam opcode dispatches landed in leftover
   Initial code (everything after looked like progress but was garbage).

7. **Reschedule trap consumes the opcode** (`src/cpu.c`). HM Table 20:
   "The trap instruction is executed as though it were the first
   instruction of the rescheduled opcode" — the trapped IFUJump still
   advances PCX/PCF and latches operands; only the dispatch target is
   replaced. (Before: AEmuReschedule's RestartIFU re-ran the same opcode
   forever.)

8. **Reschedule branch condition is HIGH-true for the emulator**
   (`src/cpu.c eval_branch_condition` + new `reschedule_cond` flipflop,
   set by Reschedule, cleared only by NoReschedule). HM Table 13 names
   it "IOAtten' (non-emulator) or ReSchedule (emulator)" — only IOAtten
   carries the prime. (Before: inverted sense made AEmu's BLT bail to
   AEmuReschedule on every word; the parity sweep never advanced.)

9. **Small constants fully replace A on memory references**
   (`src/cpu.c`). HM §3.5 p16: FF[4:7] small constant "overrules RM/STK
   as the source" — `Store_ EBLoc`, `Fetch_ 2S` address the 0o600 block
   with the 4-bit constant; the previous low-nibble-overlay left R400's
   bits in Mar.

10. **Memory-ref FF subdecode executes the FULL Table 11a set**
    (`src/cpu.c ff_apply_post`). HM §3.9: with ASEL 0-3, "the decoding
    of FF as a function is forced to be in the range 0 to 63". The old
    3-entry whitelist dropped FlipMemBase (et al.) — AEmu's JSR-indirect
    `Fetch_ T, FlipMemBase` (FF=0o337) needs the flip from MDS to CODE
    before JSRX's `BrLo_ T`; without it the jump target was written into
    the MDS base register and the world shifted by the target address.

## THE OPEN QUESTION (resume here)

The loader (Alto code, running correctly) reaches `DoEtherOutput`:
`mkone 0 0; sio` — AC0 must be 1 (etherOutputCommand) so ESIO
(AltoEtherEmu.mc) dispatches "01 -- Start transmitter" -> TurnOnTx ->
EOT sends the 13-word Mayday at eOPLoc=0o37/eOCLoc=13 (verified correct
in memory). Observed: no TurnOnTx is ever written; the flow behaves
like `Branch[SIONop, ALU=0]` (control bits 0) — i.e. **AC0 = 0 after
`mkone`**.

`mkone 0 0` = Nova `subzl 0 0` (0o102520): AC0 <- (0 - 0) with the
carry trick, shifted left, = 1. AEmu emulates Nova arith opcodes
(A-Group.mc, EmIFUArith entries, dispatch op=0o205 -> vector 0o274) via
the Dorado shifter + the `Cry` register (AEmRegs) + XorSavedCarry /
Carry20 ALU controls. Suspects, cheapest first:

1. Our `XorSavedCarry` / `Carry20` FF decodes (HM Table 11a FB=2
   FC=4/5/6) — are they implemented at all? (grep cpu.c: they sit in
   the fa==0 fb==2 switch; check what they do to the ALUFM carry.)
2. The shifter L/R/extended-17-bit semantics for the Nova carry||AC
   rotate (A-Group computes result and skip from the Dorado shifter).
3. The ALUFM entries A-Group expects (RestoreALUFM only restores
   ALUFM[15]/[17]; the rest must hold the standard convention — see
   `test_alufm_canonical_decoding`).
4. `lda 1 k777` immediately before (PC-relative, k777 is an inline
   literal at .+3) — verify AC1=0o777 and the delay loop `negl 1 0;
   inc 0 0 szr; jmp .-1` terminates with AC0=0 as designed (it should:
   the SIONop value 0 is then REPLACED by mkone — so if the sio sees 0,
   maybe the mkone opcode 0o205's dispatch/handler is at fault, or the
   skip semantics consumed the sio).

Debug recipe: `DORADO_IFUDISP_TRACE=1` prints every dispatch (op, pcf,
br31, IFUM halves, vector). Find `op=205` dispatches with br31 around
the DoEtherOutput page (the routine runs at Alto VM 0o712+; br31 will
be 0o712-ish after the fixed jsr-indirect). Then `DORADO_PCWATCH=<vec>`
`DORADO_PCWATCH_AFTER=<cyc>` dumps the trailing 64 and following 24
micro-PCs around the first hit. STK[1..4] hold AC0..AC3 (spAC0..3 in
AEmRegs give the StkP values).

## After that (the rest of the chain, already built and unit-tested)

Mayday -> `eth_tx_packet_done` recognizes Pup type 0o244 -> EFTP stream
of `DORADO_ETH_EFTP_BOOT` (NETEXEC.BOOT) -> the loader's
ReceiveEFTPPacket/Ack loop (page-1 code, all in the served BoL image) ->
packet 1 overlays page 0, packet 2 read to page 2, `jmp @.ContinueBoot`.
Watch for: the loader Acks each packet (the server pre-queues and
ignores Acks — fine), and after End it dallys for a second End — the
server sends one End; if the dally hangs, queue the End twice in
`eth_queue_eftp_boot`. Keyboard: the probe holds BS (and Quote) down;
`lda 0 @c177035; com 0 0` makes the Mayday BFN from keyboard word 2 —
with our seed 177035=0xFFF7 the BFN = 0x0008; the server currently
serves ANY bfn (`eth_queue_eftp_boot` ignores it).

## Parked issues (not blocking the Mayday)

- **Display tasks with garbage DCBs** corrupt a BR: task 3 (DHT) ends up
  at DWTREFILL (`BrHi_ ANextAddrHi` with whatever MemBase is current —
  observed 0o31=ECBR getting bank 0xE19). Run with
  DORADO_NO_DISPLAY_WAKE=1 until real software installs a display list,
  or chase why DHT's flow reaches the DWT entry. Pixels DID render once
  the IFUM fix landed (A_pixels=3808 in one run) so the render pipe works.
- **Junk task RTC cadence**: JNK gets few wakeups in heavy-load phases;
  ABoot's 100ms wait took ~4M BB cycles. OK but worth a look at
  DORADO_JUNK_TICK_CYCLES accounting (ticks on instructions, not time).
- **Disk task leftovers**: with DORADO_NO_DISK=1 nothing wakes it now
  that the wedges are fixed; without the flag, Initial-era TPC leftovers
  plus the attached pack can still run Initial disk code post-LoadRam.
- mb2eb does not emit ALUFM items (LoadRam has no ALUFM type); the
  loaded world inherits Initial's ALUFM + RestoreALUFM. Verify AEmu's
  full convention matches Initial's if arith bugs appear (suspect #3).

## Debug instrumentation added this session (all env-gated)

- `DORADO_INSSET_TRACE` — InsSet loads (task/pc/B/old->new).
- `DORADO_IFUDISP_TRACE` — every IFU dispatch (op, pcf, br31, rh/lh, vec).
- `DORADO_BR_TRACE` — BR hi/lo loads via FF (BRHI_FF/BRLO_FF lines with
  task/pc/MemBase/A) and via memory.c (BRLO/BRHI).
- `DORADO_PCWATCH=<octal pc>` + `DORADO_PCWATCH_AFTER=<cycles>` — one-shot
  micro-PC flight recorder (trailing 64 + next 24), all tasks.
- `DORADO_STORE_WINDOW="lo,hi"` — REF_W lines for every memory ref in a
  BB-cycle window. CAVEAT: refs in instructions that end with IFUJump
  are reported as the IFETCH (last_ref overwritten) — use
  `DORADO_STORE_TRACE_VA="lo,hi"` (octal VA range, memory.c-level,
  catches everything) for stores.
- `DORADO_ETH_TRACE=<N>` — first N ethernet ops (was fixed 256).

## Sources fetched this session

`chm/doradosource/AEmuSources-cedar6.0.dm!1_/` — the COMPLETE AEmu
source set from xeroxparcarchive (cyan/cedar6.0/microcode/dorado/
AEmuSources.dm!1_): AltoEther.mc, AltoEtherEmu.mc (EBoot + ESIO!),
EtherDefs.mc, Junk.mc, XMFaultTask.mc, A-Group/M-Group/S-Group.mc,
AltoMode.mc, ATraps.mc, Start.mc, LoadRam.mc, DisplayMain.mc, etc.
The .mc files are single-line Bravo text: read with
`perl -pe 's/;/;\n/g' file | less` or targeted perl regex windows.
The Alto loader binary + listing: `EtherBoot.mesa` (already in
docs/research/ethernet-netboot/downloads/, extracted to /tmp/EtherBoot.txt
during the session with `perl -pe 's/    -- /\n-- /g'`).

## Working norms (from CLAUDE.md)

No emojis in code/docs. Use original Dorado terminology. Cite HM
sections in tricky code/commits. Commit only when asked; end commit
messages with `Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>`
(update to your model name). Push only when asked. Run `make test`
(expect all suites passing) before any commit.
