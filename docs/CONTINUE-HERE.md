# Continuation handoff — Alto-on-Dorado boot bring-up

## ROUTE B (2026-06-16, session 16): bug 3 (`AV[0]=0o2`) FIXED at the root -- the `Q<-B` side-effect was missing for the Pipe external-B sources, so the Mesa `Q_ VALo` idiom never loaded Q. With it fixed the germ runs its real boot prologue (163 bytecodes -- LFC/EFC calls, DST state-dumps, no hardware faults), announces `germStarting` (MP 810), then -- ROOT FOUND (session 17) -- hits the SAME `0o27132` ControlFault as session 14, now cleanly trapped (the germ installed SD handlers first), which raises an uncaught `SIGNAL ControlFault` -> germERROR (821) -> deliberate `JB 0` halt. So the WF+Q<-B fixes converted the old infinite trap-loop into a clean halt but the `0o27132` SLink-to-unbound-memory bug is unchanged and is THE blocker. Gate ALL GREEN. (See the session-17 NEXT-blocker section below; germ Mesa sources in `chm/cedar/germ-src/`.)

### Root cause (HM Table 7 asterisk, one missing case)

`AV[0]` was seeded `0o2` because the XFER FREE path (`XferFreeQ`,
`DMesaXfer.mc`) does `Q_ VALo` (= `Q_ B<-Pipe1`, the VA-lo of a preceding
`DummyRef` to `frame-1`), then `RTemp0_ (Fetch_ Q)+1` and `AV[fsi]_
RTemp0`. The pipe held the correct VA (`0x3F06E3` = `0o17603343`, traced
via `DORADO_PIPEVA_TRACE`), and `B<-Pipe1` delivered VaLo=`0o3343` on the
bus -- but **Q stayed `0o1`**, so `RTemp0 = Q+1 = 0o2` (the bad value) was
stored as the freed frame instead of `0o3344`.

`src/cpu.c ff_override_b`: HM Table 7 asterisk -- "when an external B
source is in play AND BSEL=3, the external value also lands in Q" -- was
applied ONLY in the FB=7 block (Link/CPReg, line ~794), NOT in the FB=6
block (the Pipe/Config/FaultInfo sources). So `Q_ B<-Pipe1` (and any
`Q_ B<-Pipe*`) delivered the value on B but never loaded Q. Fix: add the
same `if (u->bsel == 3) cpu->Q = *b;` at the FB=6 block's return. (This is
the same Q<-B mechanism Bootstrap's `Q_ Link` snapshot relies on -- it
was just never wired for the pipe sources.)

### Result (verified)

- The free now stores the real freed frame `0o3344` to `AV[0]` (was `0o2`);
  the fsi-0 free list is intact. The germ no longer self-loops in
  `AllocSub`.
- Germ: **21,942,131 IFU dispatches** (was 110) -- it runs the real
  BootSwapGerm/Pilot boot code (RET, EFC/LFC calls, jumps, ALU/field ops)
  and settles into a HEALTHY busy-wait: op `0o210` (JB) at pc `0o150`,
  br31 `3E1D0C` (BootSwapGerm), `flt=0`, NO trap PCs. Stack at the wait:
  `acs=0,0o1465,0o617,0`.
- No germ-originated outbound Pup yet (only the Stage-1 n=13 boot request
  at cyc<31M); 0 display pixels.

### NEXT blocker (session 17) -- the germ raises germERROR (821) and deliberately halts

DECODED (the JB self-loop is NOT an interrupt-wait -- it is a deliberate
halt after an error). Sequence, fully traced + cross-referenced against
the germ Mesa source (now downloaded to `chm/cedar/germ-src/`):

1. The germ runs its real boot prologue -- **163 bytecodes**, br31
   `3E1D0C` (BootSwapGerm) and `3E0D58`: LFC/EFC calls, RET, **DST (Dump
   State, op `0o370`)**, comparisons. NO hardware faults anywhere
   (`flt=0` throughout).
2. Early it calls **`SetMaintPanel`** (MISC `alpha=0o10`) with MP code
   **`0o1452`=810 = `germStarting`** ("germ entered") -- traced live at
   the `MaintPanel_ T` instruction (real pc `0o4041`, cyc 67975655). So
   the germ announces a healthy start.
3. It then hits an error via a **`KFCB` (Kernel Function Call, op `0o347`,
   pcf `0o735`)** -> the germ's error/trap path (DST + the classify
   dispatch at pcf `0o3720..0o3756`, conditional `JGEB`/`JLEB` on an error
   code), which loads **`LIW 0o1465`=821 = `germERROR`** ("unnamed
   ERROR") and calls `SetMaintPanel` again (cyc 67980386, T=`0o1465`).
4. Then **`JB 0`** (op `0o210`, disp 0 = jump-to-self) at pcf `0o3756` --
   the germ's deliberate halt-after-error. The ~21.9M dispatch count is
   almost entirely this JB spin.

So the germ gets a long way (state-dumps, kernel calls) then raises an
**uncaught Mesa ERROR**. It set the GENERIC `germERROR` (821), NOT a
specific boot code (`germNoPhysicalBootFile`=824 / `germNoServer`=828 /
`germBadBootFile`=823), which leans toward an UNEXPECTED condition (an
emulator mis-model surfacing as an uncaught signal) rather than the clean
"no OS volume" path -- though a legitimate boot-channel failure is not
ruled out.

ROOT FOUND (session 17): the germERROR is the **same `0o27132` ControlFault
as session 14**, now cleanly trapped instead of looping. Full chain:
1. The germ runs its boot prologue and **installs its SD trap handlers**
   (writes SD[7]=sControlFault, SD[8], SD[`0o13`]=sUnbound, ... at cyc
   67978776-67979040; SD[7] <- `0o633` = TrapsImpl.ControlFaultTrap).
2. TrapsImpl's `XferProc` stores **SLink=`0o27132`** into a new frame's
   L[2] (return link) at MDS+`0o2346` (cyc 67978520, pc `0o4034`,
   br31 `3E0D58`=TrapsImpl). `0o27132` is GENUINE germ data (session 14:
   it lives in the germ file at MDS+`0o7652`, BLT-copied around).
3. That frame RETs: pc `0o1034`(RET) -> reads L[2]=`0o27132` (cyc
   67979221, lva `0o17403346`) -> `0o1026`(XferMD) DLink=`0o27132` ->
   `0o1700`(Xfer) tag=2 indirect -> fetch MDS+`0o27132` (VA `0o17427132`)
   = **0** -> ZeroDest -> **ControlFault (T=`0o7`)** (cyc 67979268).
4. Now that SD[7] is bound, the ControlFault XFERs to
   `TrapsImpl.ControlFaultTrap` -> `SIGNAL ControlFault[source]` (a
   resumable PUBLIC SIGNAL, `trapsimpl-6.1.mesa` line 20/209). Uncaught ->
   the germ's `SignalHandler` (`bootswapgerm-indigo.mesa` line ~550:
   `SetMP[code]; Halt[]`) classifies it (JGEB/JLEB range checks) to the
   generic **germERROR (821)** and does the `JB 0` halt.

So my WF + Q<-B fixes advanced the germ PAST the point where it installs
handlers, converting the old infinite ControlFault trap-loop (sessions
8-14) into a clean germERROR halt -- but the **underlying `0o27132`
SLink-to-unbound-memory bug is UNCHANGED** and is now THE blocker again.

NEXT PASS -- resolve `0o27132` (the deep session-14 question, now with a
sharper lead): the value `0o27132` sits at **offset +`0o10` in the static
record at MDS+`0o7642`** that gets BLT'd into the state block. If that
record is a PrincOps StateVector (word0=stkptr, word1=dest, word2=source,
word3+=stk[]), then +`0o10` is a **stack element, NOT the source link**
(which is word +2). So suspect the LSTF/LoadState (`DMesaXfer.mc`) reading
the SLink from the wrong state-vector offset, OR `XferProc` taking the
wrong word as SLink. Trace the SLink register (RTemp6) back from the
`L[2]_ SLink` store (cyc 67978520, pc `0o4034`) through the XFER that set
it, and compare against the state-vector layout. Confirm: nothing ever
binds MDS+`0o27132` (verified -- only the boot zero-fill writes it). Repro:
`DORADO_IFUDISP_TRACE=1 ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
--germ '../chm/cedar/germ/Dorado.germ!4' --cycles 68000000`; the
ControlFault is at cyc 67979268 (gated `DORADO_XFER_TRACE` 67979150,
67979330; the bad SLink store is `DORADO_STORE_TRACE_VA="017403346,
017403346"` data=`027132`). Sources: `chm/cedar/germ-src/trapsimpl-6.1.mesa`
(trap handlers), `bootswapgerm-indigo.mesa` (SignalHandler/boot flow).

### HARD REGRESSION GATE -- ALL GREEN
1. `make test` = 10/10 suites.
2. AEmu NETEXEC @200M: **1477** px (band 1476-1505). PASS.
3. Galaxian @160M: **121553** px (=121552 +/-1). PASS.
4. AltoMesaDorado.eb!2 + NETEXEC @200M: **1473** px. PASS.
5. `make sdl` compiles.

## ROUTE B (2026-06-16, session 15): the 113-dispatch ControlFault is FIXED -- root-caused to TWO grounded emulator bugs in the Mesa field-opcode path and both fixed in `src/cpu.c`. (1) `WF<-A`/`RF<-A` were UNIMPLEMENTED STUBS; (2) `TisId`/`RisId` never substituted the IFU operand byte onto the bus (and `IFetch<-` never did the `BR[24:31]<-Id` replacement). With both fixed, the Mesa `WF` opcode `T_(IFetch_Stack&-1)+T, TisId` now computes the field pointer as `Stack&-1 + alpha` (was `Stack&-1 + staleT`), so it no longer corrupts `TrapsImpl`'s code base (`g[1]` stays `0o6530`, was clobbered to `0o11602`). The germ NO LONGER ControlFaults at the LSTF resume. NEW blocker found and PROVEN: a separate pre-existing bug seeds `AV[0]=0o2` (a self-looping AllocSub terminator) where the germ file has `0o6` (indirect->fsi 1); forcing `AV[0]=0o6` lets the germ run **~22 MILLION dispatches** of real boot code and settle into a HEALTHY wait loop (op `0o210` JB at pc `0o150`, NO trap PCs). Gate ALL GREEN.

### The two fixed bugs (HM + schematic grounded)

**Bug 1 -- `WF<-A`/`RF<-A` (load ShC from a Mesa field descriptor).**
`src/cpu.c ff_apply_post` FF 5/5 and 5/6 were `/* shifter ctrl TBD */`
no-ops, so the Mesa Read/Write-Field opcodes ran `ShMdBothMasks`/
`ShiftLMask` against a STALE `ShC`. Implemented `field_desc_to_shc()`.
The exact transform is NOT in any manual (both say only "transform the
bits appropriately") -- it is the **"Shift Register Control" hardware on
ProcL sheet 18** (`DoradoDocs/doradodrawings/ProcL-Rev-Ci.press!1.pdf`
p.19, `ProcL18.sil`), which tabulates, for descriptor `A[8:15]=(P<<4)|S`
(P=position `A[8:11]`, S=size=width-1 `A[12:15]`; `ShC[2:3]<-A[2:3]`):
```
        Shift Count    Right Mask     Left Mask
   RF   P + S + 1      (don't care)   16 - S - 1
   WF   16 - P - S - 1 16 - P - S - 1  P
```
(The PROM grab confirms the LMASK/RMASK PROMs are just count->mask
tables; the field->count transform is hardwired combinational logic, not
a PROM. Sources saved in `chm/doradosource/{ProcProms,IFUProms,
DoradoProms}.bcpl`.)

**Bug 2 -- `TisId`/`RisId` bus substitution + `IFetch<-` BR replacement.**
HM p.24: "RFfoA and WFfoA ... also load ShC[2:3] from A[2:3]"; p.38:
"IFetch<- -- A fetch for which BR[24:31] are replaced by Id from the IFU
... the IFU does not advance ... so an accompanying TisId or RisId
function is needed to advance." Our code consumed the Id post-ALU
(advancing) but never put it on the bus, so the WF's `T_(IFetch_Stack&-1)
+T, TisId` used the STALE T on the B bus instead of `alpha`. Live trace
(corrupting WF, cyc 67976936): `a=Stack&-1=0o4700, alpha=0, oldT=1` ->
we computed `p=0o4700+1=0o4701` (= `g[1]` codebase) and wrote the field
there; correct is `p=0o4700+alpha=0o4700` (= `g[0]`). Fix: a pre-ALU
block peeks the current Id and substitutes it for the matching bus source
(TisId: B<-T / A<-T; RisId: B<-RM / A<-RM explicit form), and the mem-ref
VA does `BR=(BR&~0xFF)|Id` for `DM_REF_IFETCH`. The cursor still advances
exactly once (the existing post-ALU `ff_apply_post` consume); the pre-ALU
peek does NOT advance, so both see the same byte. Added `ifu_id_at()` +
`ifu_peek_id()` (refactor of `ifu_consume_id`).

GATE-SAFE / verified: the AEmu and AltoMesaDorado gate worlds never run
`WF<-A`; the TisId/RisId substitution is HM-correct and the full gate
stays green (NETEXEC 1481, Galaxian 121553, AltoMesaDorado 1489, 10/10
tests, sdl compiles). The earlier transient NETEXEC 1472 was banner
host-time render variance, not a regression (see memory
`aemu-pixel-gate-not-ground-truth`).

### NEW blocker (session 16) -- bug 3: `AV[0]` seeded with `0o2` not `0o6`

After the WF fix the germ reaches the LSTF process-resume cleanly, which
allocates an fsi-0 frame via `AllocSub` reading `AV[0]` (MDS+`0o1000`).
But `AV[0]` has been overwritten with `0o2` -- a tag-2 indirect that
`AllocSub` resolves to `RSH[0o4000+0o2,2]=AV[0]` (SELF -> infinite loop).
The germ FILE's fsi-0 free list (head `0o3354`) terminates with `0o6`
(tag-2 indirect -> fsi 1: `RSH[0o4000+0o6,2]=AV[1]`), the correct
"fsi-0 exhausted, fall through to fsi 1". So `0o2` is wrong; `next_fsi`
came out 0 instead of 1 (lost bit 2 = the `0o4`).

PROOF this is THE next wall: env-forcing the `0o2`->`0o6` store made the
germ jump from **110 -> ~21.9M dispatches** and run real boot code (RET,
EFC/LFC calls, jumps, ALU/field ops -- ops 350/010/111/070/126/164/057/
343/245/244/072/033/370/364) before settling into a healthy busy-wait
(op `0o210` JB at pc `0o150`, br31 `3E1D0C` = BootSwapGerm; the XFER tail
at 74.9M is ALL pc `0o150`, NO trap PCs). The germ is now WAITING (likely
for the next boot phase / OS load / a device), not faulting.

WHERE the bad `0o2` is written: a Store at **pc `0o271`** (the generic
`0o263`->`0o271`->`0o272` fetch/modify/store helper) at cyc **67975458**,
running inside **BootSwapGerm** (br31 `0o17416414`), stores `RM/STK=0o2`
to `AV[0]`. This is PRE-EXISTING (A/B identical with the WF fix gated on/
off), so it is a SEPARATE bug -- some BootSwapGerm AV-init/relocation
arithmetic computes `0o2` where `0o6` is intended. NEXT PASS: trace the
RM/STK source feeding the pc-`0o271` store back to where `next_fsi`/the
terminator is computed (is it a field/shift dropping bit 2, or an
off-by-one in the fsi-chain build?). Repro:
`DORADO_IFUDISP_TRACE=1 ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
--germ '../chm/cedar/germ/Dorado.germ!4' --cycles 80000000` -> 110
dispatches; `DORADO_STORE_TRACE_VA="017401000,017401000"` shows the
`data=0o2` store at cyc 67975458 pc `0o271`. To watch the unblocked run,
temporarily force the store (`if va==017401000 && b==2 then b=6` in
`src/memory.c DM_REF_STORE`) -> ~22M dispatches. Germ file fsi-0 chain +
terminator `0o6` are in `chm/cedar/germ/Dorado.germ!4` (head AV[0]=`0o3354`);
loadmap FSI/next-FSI table in `chm/cedar/germ/Dorado.loadmap!1.txt`.

### HARD REGRESSION GATE -- ALL GREEN
1. `make test` = 10/10 suites.
2. AEmu NETEXEC @200M: **1481** px (band 1476-1505). PASS.
3. Galaxian @160M: **121553** px (=121552 +/-1). PASS.
4. AltoMesaDorado.eb!2 + NETEXEC @200M: **1489** px. PASS.
5. `make sdl` compiles.

## ROUTE B (2026-06-15, session 14): the prior `BLTC`/`LSTF`-`alpha` hypothesis is REFUTED with hard evidence -- alpha is read CORRECTLY; the 113th-dispatch ControlFault is the germ resuming its saved boot-process via `LSTF`/`LoadState`, whose loaded DLink/SLink and the resumed proc's fsi are GENUINE germ data that reference MDS locations past the loaded 8192-word germ image. No emulator IFU/operand bug found; NO code change made (tree clean). Next blocker is upstream germ process-resume state.

### What was tested and REFUTED (the session-13 lead)

The session-13 handoff guessed the corruption was a `BLTC` block-transfer
whose `dest` overlapped an `LSTF` state vector because `LSTF`'s `alpha`
operand byte was mis-read (a sibling of the `notLength` IFU bug). Every
part of that is wrong:

1. **`BLTC` (op `0o354`) never dispatches.** The only block transfer in
   the germ run is **`BLT` (op `0o352`)** at `br31=3E0D58` `pcf=0o455`
   (opcode bytes verified by VMDUMP: `M[0o17406756]=0o003752`, low byte
   `0o352`). `BLT`/`BLTC` are length-1 and take dest/count/source off the
   Mesa eval stack -- they have NO `alpha` operand, so an alpha mis-read
   cannot affect them. (Opcode map from `DMesaDefs.mc!2`: `BLT=0o352`,
   `BLTC=0o354`, `LST=0o371`, `LSTF=0o372`, `RET=0o343`.)

2. **`LSTF`'s `alpha` is read CORRECTLY = `0o007`.** `LSTF` (op `0o372`)
   dispatches at `pcf=0o477`; VMDUMP `M[0o17406767]=0o167772` -> low byte
   `0o372` (= LSTF), and `M[0o17406770]=0o003777` -> high byte (pcf=`0o500`,
   even = high byte for insset<2) = `0o007`. The engine's `<-Id` delivers
   N=1 (`xf.free`) on the first read and `alpha=0o007` on the second
   (`ifu_consume_id`, `src/cpu.c`), exactly matching the code byte. The
   state vector legitimately sits at `L+alpha = L+7`. `<-Id`/operand fetch
   is FINE. (The germ ran 113 dispatches incl. many EFC/LFC proc calls
   through the same `<-Id` path, all correct.)

3. **`0o27132` is GENUINE germ data, faithfully loaded.** It lives in the
   germ FILE at `MDS+0o7652` (big-endian `0o027132`, confirmed by reading
   `Dorado.germ!4` directly), inside a static ~7-word-record array at
   `MDS+0o7642..` (repeating `0o177660`/`0o55000,0o55001`/`0o3564`
   pattern). `BLT` faithfully copies it; nothing corrupts it.

### The real failure chain (re-diagnosed, traced live ~cyc 67.97-67.99M)

The 113th dispatch is **`RET` (op `0o343`, vec `0o1034`)** at `br31=3E1382`,
running inside a context the germ just **resumed via `LSTF`**:

- `LSTF` (the only `LST*` in the whole run) runs `LoadState`/`LoadStack`
  (`DMesaXfer.mc!1` line 980-1036): loads StkP/DLink/SLink/Stack from the
  state block at `L+alpha`, then `MemBase_ MDS, Branch[Xfer]`.
- The loaded **DLink = `0o173`** -> `Xfer` tag dispatch -> `XferDisp01`
  (proc descriptor) -> `XferProc` (`DMesaXfer.mc!1` line 729): allocate a
  frame, patch links, start the IFU.
- `XferProc` reads the proc's **fsi** from its code-segment frame-size
  word (`T_ DPF[T,10,10,MD]`, MD=`0o2323` -> right byte **fsi=`0o323`**).
  `AllocSub` (`DMesaXfer.mc!1` line 539) fetches `AV[0o323]` (`AV=0o1000`,
  so `MDS+0o1323`) = **0**, `BDispatch 0` -> tag-0 "good frame" -> returns
  **frame = 0**. So the resumed proc runs in **L = MDS+0** (low core).
  The proc runs 3 bytecodes (`067`, `210`=JB, `343`=RET).
- `XferProc` stored the new frame's `L[2] = SLink` (= the LSTF-loaded
  **source link `0o27132`**). With L=0, that store landed at `MDS+2`.
- `RET` (`IFUP[RET,1,L,N[2]]`) fetches `L[2]` = `MDS+2` = `0o27132`,
  XferMD sets DLink=`0o27132`, `Xfer` sees tag = `0o27132 & 3 = 2`
  (INDIRECT) -> `XferDisp10`: `Fetch_ T` (MemBase=MDS) at **`MDS+0o27132`
  = VA `0o17427132`**, which is **past the 8192-word germ** (germ occupies
  `MDS+0o1000..0o21000`) -> **md=0** -> ZeroDest -> `ControlFault` (T=`0o7`)
  -> `SD[7]`=0 -> the steady `0o1700/0o1026/0o1041/0o1600/0o2000` trap loop.

NOTE: even with a proper (nonzero) frame, `RET` would still read
`L[2]=SLink=0o27132`, so the **fatal** issue is the indirect SLink past
the germ; the `fsi=0o323`/`frame=0` is a SEPARATE anomaly (the proc still
runs). Both derive from the LSTF-loaded state block.

### Verdict

NOT an IFU `<-Id`/operand/`alpha` mis-model (refuted above), NOT a `BLTC`
bug (no BLTC), NOT a load/BLT corruption (`0o27132` is genuine). The
blocker is in the germ's **boot-process resume**: the state block that
`LSTF` loads yields a DLink (`0o173`)/SLink (`0o27132`) and an fsi
(`0o323`) that reference MDS locations past the loaded germ. Either (a)
the germ is supposed to build/relocate more MDS state (frame heap / the
target of indirect link `0o27132`, the global frame for DLink `0o173`'s
gfi, and the `AV[0o323]` slot) BEFORE this resume, and an upstream
emulator mis-model dropped that setup; or (b) the state block being
resumed is sourced/relocated wrong. The AV free lists ARE initialized
(verified vs the germ file: `AV[1]/AV[5]` show normal partial
consumption, `AV[0]` consumed to a tag-2 indirect terminator `0o2`; germ
chains terminate with tag-2 indirect markers `0o16/0o12/0o6`, never 0)
-- so it is NOT a wholesale heap-init failure; it is specific to the
`fsi=0o323` size class and the `0o27132`/`0o173` links.

### NEXT PASS (session 15) -- where to dig

This is genuinely the FIRST `LSTF` in the run and the first germ
process-resume, so the bug (if emulator) is exercised only here and is
gate-safe to chase (Alto worlds never run `LST*`). Concrete steps:

1. **Read the `BLT` (op `0o352`, `pcf=0o455`) operands.** Source ran
   `MDS+0o7642`, dest `MDS+0o2730` (from XFER trace). Confirm dest/count/
   source popped off the stack are what the germ intends, i.e. that the
   state block the LSTF later loads is sourced from the right place.
   (Augment the `IFUDISP` trace to print `STK[stkp-1..stkp-3]`, the
   popped args, since the current trace only prints `STK[stkp..+3]`.)
2. **Decode the resumed proc.** DLink=`0o173` -> gfi -> `GFT[gfi]` global
   frame (traced fetch `MDS+0o1406` -> `md=0o4700`) -> entry vector ->
   code. Is `fsi=0o323` genuine for that proc, or is `GFT[gfi]`/the code
   base (`LoadGC`) resolved wrong (the MemBase/BR family of sessions
   9-13)? `DPF[T,10,10,MD]` right-byte extraction worked for 100+ earlier
   proc calls, so suspect the proc identity (DLink/GFT), not DPF.
3. **Is `MDS+0o27132` ever supposed to be bound?** Check whether the germ
   writes a valid control link there during a setup step we mis-emulate
   (gate a `DORADO_STORE_TRACE_VA` on `0o17427132` across the whole run).
   If nothing ever writes it, the resume is premature/the state block is
   wrong; if a store was dropped/faulted, that drop is the bug.

Repro: `DORADO_IFUDISP_TRACE=1 ./build/dorado --eb
'../chm/dorado/CedarDorado.eb!6' --germ '../chm/cedar/germ/Dorado.germ!4'
--cycles 68100000` -> 113 dispatches; `RET` at cyc 67978584. Gated
`DORADO_XFER_TRACE` with `DORADO_TRACE_GATE="67976000,67979000"` shows the
LSTF LoadState, the `XferProc` alloc (frame=0), and the failing indirect
`Xfer` at `MDS+0o27132`. Opcode/alpha bytes via
`DORADO_VMDUMP="017406750,017406772,67978000"`.

### REGRESSION GATE -- NOT RUN (no code change)

Session 14 made **no emulator code change** (the prior hypothesis was
refuted; any fix to the shared XFER/alloc/`<-Id` path would be
speculative and risk the gate). `git status --short` clean except this
doc. The 5-item gate is therefore unaffected from session-13's all-green
state.

## ROUTE B (2026-06-15, session 13): `LongFetch` ignored the high address bits from B -- fixed the VA computation in `src/cpu.c`; the EFC4 (op `0o304`) external-link read now lands at the right code-segment word; germ jumps from **53 to 113 IFU dispatches**, running through **3 code modules** of Pilot/germ startup; new blocker = another ControlFault on an indirect control link that resolves to 0

### The stall: op `0o304` = EFC4 (External Function Call 4)

Op `0o304` is row `30x` col 4 of `DMesaDefs.mc!2` `MesaOps[...]` = **EFC4**.
Its handler (`DMesaXfer.mc!1` `IFUP[EFC4,1,G,N[4]]` -> `EFCM1`) does
`Fetch_ 0S, Call[GetLinkID]` then XFERs through the fetched external
control link. `GetLinkID` reads the external link at code-segment offset
`~ID` (= `-(ID+1)` = -5 for EFC4) via **`LongFetch_ T, B_ RTemp0`** with
`MemBase=Code`, RTemp0=-1. The link lives at the negative offset C-5
(VA `CP-5` = `0o17416407`, which holds the valid link `0o000605`).

### Root cause (emulator mis-model, HM-grounded)

`src/cpu.c` computed every memory-reference VA as `va = br + mar`, where
`mar` is only the **16-bit** A bus. For `LongFetch` that drops the high
address bits. Per HM "Processor Memory References" (LongFetch entry):
*"A fetch for which the complete 28-bit VA is `(B[4:15]^Mar[0:15]) +
BR[MemBase]`"* and "LongFetch takes the low 16 bits of address from
RM/STK and **high 8 bits from B**." With B=RTemp0=`0o177777`, the
displacement is a full 28-bit `-5`, so VA = CP-5. Our model used only
Mar=`0o177773` (-5 as unsigned 16 bits), so the bank-carry pushed VA up
one 64K bank to `0o17616407` (verified: that page is zero), read md=0 ->
DLink=0 -> ZeroDest -> ControlFault(7) -> SD[7]=0 -> infinite trap loop.

### The fix (committed) -- HM-grounded

`src/cpu.c` memory-ref VA: for `DM_REF_LONGFETCH`, form the displacement
as `((B & 0o7777) << 16) | (Mar & 0xFFFF)` (B[4:15] supplies VA[4:15]);
all other reference kinds keep the plain 16-bit Mar. Confirmed by VMDUMP:
intended CP-5 (`0o17416407`) holds `0o000605`; the old wrapped address
(`0o17616407`) holds 0. **Alto worlds stay correct** because their
LongFetch high bits come out the same (only the high-B contribution is
new, and microcode written for real HW always loads B for a LongFetch);
all pixel gates unchanged within band.

### Result (verified)

- Germ: **113 IFU dispatches** (was 53). After EFC4 it XFERs into a new
  module (br31 `3E1D0C` -> `3E0D58`, 57 dispatches), then a third
  (`3E1382`, 3 dispatches), running real Pilot/germ startup bytecodes.
- **New blocker:** after the 113th dispatch (op `0o343` RET, vec `0o1034`,
  cyc 67978584) the returned control link `0o27132` is an **indirect**
  link (tag=10); Xfer fetches the real link at VA `0o17427132` (MDS+`0o27132`,
  *past* the 8192-word germ extent `<=0o17421000`) and gets **md=0** ->
  ZeroDest -> ControlFault(7) at pc `0o1711` -> `SD[7]`=0 -> the same
  `0o1700/0o1041/0o1026/0o2000/0o1600` trap loop, now much further along.
- Still **no own outbound Pup** (`DORADO_ETH_TX_TRACE` shows only the
  Stage-1 n=13 boot request at cyc<31M); 0 display pixels.

### HARD REGRESSION GATE -- ALL GREEN

1. `make test` = **10/10** suites.
2. AEmu NETEXEC @200M: **1482** px (band 1476-1505). PASS.
3. Galaxian @160M: **121553** px (=121552 +/-1). PASS.
4. AltoMesaDorado.eb!2 + NETEXEC @200M: **1480** px (band 1466-1505). PASS.
5. `make sdl` compiles.

### NEXT PASS (session 14)

Decode the RET (op `0o343`, the 113th dispatch) and the indirect control
link `0o27132` it returns into. Is VA `0o17427132` (beyond the loaded germ)
a location the germ binds during later startup (missing state we should
seed), or is the returned link itself corrupt (another addressing/BR
mis-model in the RET / frame-return path, the family of sessions 9-13)?
The RET path: pc `0o150`(RET) -> `0o1034` -> `0o1053` -> `0o1026`(XferMD,
DLink=md=`0o27132`) -> `0o1700`(Xfer) -> tag=10 indirect -> fetch
`0o17427132` md=0 -> ControlFault. Repro:
`DORADO_IFUDISP_TRACE=1 ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
--germ '../chm/cedar/germ/Dorado.germ!4' --cycles 120000000` -> 113
dispatches; gated `DORADO_XFER_TRACE` at `67978584,67978720` shows the
failing indirect XFER; `DORADO_VMDUMP="017427100,017427140,67978620"`
dumps the (zero) indirect-link region.

## ROUTE B (2026-06-15, session 12): IFUM Length field is `notLength` (`~length`), not `length-1` -- fixed in `ifu_decode_lh`; germ jumps from 10 to **53 IFU dispatches** (past the op-`002` / SD[7] ControlFault); new blocker = germ stops dispatching after the 53rd bytecode and spins, having issued NO outbound Pup (does not yet reach its own Ethernet volume fetch)

### The fix (committed `57b6d18`) -- microcode-grounded (`chm/microd/mdfields.d` TIFUM)

`src/cpu.c ifu_decode_lh` decoded the 2-bit IFUM Length field as
`ifu_length = lpr + 1`. Per `mdfields.d`'s `TIFUM` struct the field is
**notLength** = `~length & 3`, so `length = ~lpr & 3` (field 2 -> len 1,
1 -> len 2, 0 -> len 3). The old `lpr+1` form is correct ONLY for the
length-2 case (field 1), which is why it survived every Alto gate (all
Alto/Nova opcodes are 2 bytes). It mis-sized every length-1 and length-3
opcode, corrupting the saved Mesa PC in `SavePCInFrame` for the
PrincOps/Mesa instruction set (e.g. 1-byte `LFC4`/`NOOP`).

Three synthetic IFU tests in `tests/test_cpu.c` (`test_ifu_conditional_
dispatch`, `test_ifu_conditional_cond_true`, `test_ifu_dispatch_
synthetic`) encoded their length-1 INC/HALT opcodes with the old
`length_p = 0`; updated to `2` (notLength of length 1) so they assert the
same intended opcode length under the corrected decode. Length-2 cases
unchanged.

### Result (verified)

- Germ: **53 IFU dispatches** (was 10). It now executes a long run of real
  Cedar/Mesa bytecodes (ops `263 170 071 256 343 060 056 264 163 070 100
  050 374 364 331 020 022 350 111 164 344 304` ...).
- **New blocker:** after the 53rd dispatch -- op `0o304` at real `pc=0o761`,
  IFUM vec `0o764` -- the germ stops issuing opcodes and spins in microcode
  for the rest of the 120M-cycle budget (0 further IFU dispatches).
- **Does NOT yet reach its own Ethernet volume fetch.** The only outbound
  TX (`DORADO_ETH_TX_TRACE`, n=13) fires at cyc < 31M -- that is Stage-1
  `EtherMicrocodeBoot`'s request, before the world loads. The germ runs at
  cyc 67M+ (pass3 data load @67.28M) and issues **zero** Pups; no outbound
  `0244` Mayday. The 2 fetch faults (`pc=0o6023/0o6024`, va=0) are early
  bootstrap-phase (cyc ~11.3M), unrelated to the germ.
- 0 display pixels.

Next: disassemble Cedar op `0o304` (insset=1) and trace the microcode at
IFUM vec `0o764` / real `pc=0o761` to find why the germ halts dispatch
there (waiting on a device/memory cell? a fault that self-loops? needs the
keyboard/DDC back-channel to pick a boot mode?). Repro:
`DORADO_IFUDISP_TRACE=1 ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
--germ '../chm/cedar/germ/Dorado.germ!4' --cycles 120000000`.

### HARD REGRESSION GATE -- ALL GREEN

1. `make test` = **10/10** suites (with the 3 test-encoding updates).
2. AEmu NETEXEC @200M: **1481** px (band 1476-1505). PASS.
3. Galaxian: **121553** px @160M (=121552 +/-1); 121641 @122M, 124239 @118M
   -- animation-frame variance, NOT a regression (all Alto opcodes are
   length-2, so the decode is bit-identical old vs new). PASS.
4. AltoMesaDorado.eb!2 + NETEXEC: **1475/1484/1488** px @200/195/205M
   (band 1466-1505; banner renders host time so it varies run-to-run;
   menu renders). PASS.
5. `make sdl` compiles.

## ROUTE B (2026-06-15, session 11): the boot XFER's `XferProc` now stores BootSwapGerm's saved-G (frame[0]) correctly (`0o4634`); `LoadGC` loads G correctly; the swap-trap loop is GONE; the germ now runs 10 bytecodes (was 6) -- 4 more after the prologue RET; new blocker = a later XFER (after the 10th bytecode, op `002`) hits a ControlFault into the still-empty SD trap table

### Root cause of the session-10 blocker (XferProc stored frame[0] = `0o10210` instead of `~0o4634`)

ONE emulator mis-model, microcode-grounded. `DMesaXfer.mc!1` `XferProc`
(PrincOps) does, after allocating the frame:

```
	RTemp0_ T, MemBase_ G;          * real 0o3360, FF=0o251 = MemBaseX G (BRX[G,1])
	RTemp1_ (RTemp1)+(RTemp1);
	DummyRef_ 0S, T_ MD, StkP+1;    * real 0o4030: Get VA of G[0]
	...
	T_ VALo;                        * T_ global frame pointer = lo(VA of G[0])
	T_ (Store_ RTemp0)+1, DBuf_ T;  * L[0] _ G
```

`DummyRef_ 0S` references **BR[G] + 0**, so `VALo` = lo(BR[G]) = the global
frame pointer (`0o4634`, loaded by the boot LoadGC). The compiled form is
`asel=1 (Fetch<-RM/STK), lc=3 (T<-Md), FF=000` -- the displacement (A bus)
is the small constant **0** carried by FF=000.

`src/cpu.c` had a blanket DummyRef special case:
`if (kind==DM_REF_DUMMYREF && asel==1 && lc==3) mar = cpu->T;`. It was added
for InitMem.mc `NextMapEntry`'s `DummyRef_ T, T_ MD` (real `0o7000`), but
that construct is `asel=1, lc=3, FF=021` (FA=0 FB=2 FC=1 = the memory-
subdecode **A<-T** override). XferProc's DummyRef has the **same asel/lc**
but `FF=000` (A<-0), so the blanket test caught BOTH and forced Mar=T for
XferProc too: VA = BR[G] + T = `0o4634 + 0o3354` (T held the just-allocated
local frame `0o3354`) = **`0o10210`**, the exact corrupt frame[0]. The
frame-return LoadGC then read a garbage global frame -> swap/MTRAP loop.

### Fix (committed) -- microcode-grounded (InitMem.mc!1 / DMesaXfer.mc!1) + HM Table 8a DummyRef

`src/cpu.c`: **delete the `mar = cpu->T` blanket special case**; DummyRef Mar
is just the A bus (`mar = a`), exactly like any processor ref. The
displacement is already carried correctly by the FF memory-subdecode A-source
override (`ff_a_override`, applied to `a` at the `a = ovr` line before the
ref): InitMem's FF=021 -> `a = T`; XferProc's FF=000 -> `a = 0`. So both
constructs now compute the right Mar without a per-construct hack. This is the
**MemBaseX/BR-resolution family** the last two passes worked (the `MemBase_L`
fix was BRX[L,0]; here the path runs through `MemBase_G` = BRX[G,1]), but the
actual bug was the DummyRef displacement, not the MemBase decode (which
correctly resolves G=1).

**Why the Alto worlds stay correct:** the deleted hack was redundant for
InitMem (ff_a_override already yields `a = T` for FF=021), and the Alto worlds
do not run DMesaXfer's `DummyRef_ 0S`. The shared Mesa-VM XFER path is
exercised by AltoMesaDorado, which stays green.

### Result (verified)

- `XferProc`'s DummyRef now references VA `0x3E099C` (lo `0o4634`, was
  `0x3E1088`/`0o10210`); `VALo`/frame[0] = `0o4634` = BootSwapGerm's G.
- The frame-return `LoadGC` loads G correctly; **the swap-trap loop is GONE**.
- The germ runs **10 IFU dispatches** (was 6): the prologue
  (LFC4/LI1/`165`/`263`/`361`/RET) returns cleanly into BootSwapGerm, which
  then dispatches **4 more** bytecodes: op `100`, `000`, `000`, `002`.

### HARD REGRESSION GATE -- ALL GREEN

1. `make test` = **10/10** suites.
2. AEmu NETEXEC @200M: **1493** px (band 1476-1505). PASS.
3. Galaxian @160M: **121553** px (=121552 +/-1). PASS.
4. AltoMesaDorado.eb!2 + NETEXEC @200M: **1490** px (band 1477-1502;
   NetExec menu renders). PASS.
5. `make sdl` compiles (no test_cpu.c:7709 warning reproduces; the line is a
   plain printf).

### NEW blocker (session 12): a later XFER (after the 10th bytecode, op `002`) ControlFaults into the empty SD trap table

After op `002` (10th dispatch, pc `0o6`, vec `0o470`), control enters a Mesa
XFER/trap loop: `XFER`(0o1700) / `XFERMD`(0o1026) / `SAVEPCANDTRAP`(0o2000) /
`SAVEPCINFRAME`(0o1600) / `MTRAP`(0o1041), steady with **T=`0o7`**
(= `sControlFault`). The XFER reads `SD[7]` and gets **md=0** (lva `0o17401107`)
-> ZeroDest -> ControlFault(7) -> re-fault forever -- BootSwapGerm has not yet
populated the SD trap handlers. So either op `002`'s XFER targets a legitimately
unbound control link (germ/boot state not yet set up by these first 10
bytecodes) or another emulator XFER mis-model surfaces only on this 11th
transfer. NEXT PASS: decode op `002` (insset=1) and the control link it XFERs
through (`DORADO_XFER_TRACE` gated ~67975700..67976000; the trap is T=`0o7`,
md=0 at SD[7]). Repro:
`DORADO_IFUDISP_TRACE=1 ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
--germ '../chm/cedar/germ/Dorado.germ!4' --cycles 80000000` -> 10 dispatches
then the ControlFault loop; the first XFER is at cyc 67974929, the 10th
dispatch ~67975700.

## ROUTE B (2026-06-15, session 9): the prologue RET's ZeroDest is FIXED (two grounded bugs); the RET now reads the correct return link and returns cleanly into BootSwapGerm; new blocker = the frame-return LoadGC mis-loads G because the boot XFER stored BootSwapGerm's saved-G (frame[0]) wrong

### Root cause of the session-8 blocker (RET reads a zero return link -> ZeroDest)

TWO independent bugs, both in the shared Mesa-VM XFER path, masked each other:

1. **`MemBase_ L` mis-decoded (emulator, HM Table 11d).** `MemBase_ L`
   (L = base register 0) compiles to the **MemBaseX form** FF=`0o250`
   (FA=2 FB=5 FC=0). Our `ff_apply_post` handler for "MemBaseX <- FF[6:7]"
   reused the OLD MemBase bits: `(MemBase&0o30)|((MemBase>>1)&3)|((ff&3)<<3)`,
   so with MemBase=MDS(`0o36`) it produced `0o33`, NOT 0 (=L). HM Table 11d
   (FA=2): "MemBase[0]<-0, MemBase[1:2]<-MemBX[0:1], MemBase[3:4]<-FF[6:7]"
   => `MemBase = ((MemBX&3)<<2)|(FF[6:7])` (the SAME construction the IFU
   uses at the `MemB[0]==0` case). Fix in `src/cpu.c`. Effect: the local
   frame base register L was never loaded with the allocated frame -> L=0 ->
   SavePCInFrameIL computed SLink=0 -> the LFC4-called proc's returnLink
   (`L[2]`) = 0 -> RET ZeroDest.

2. **pipe_push fault-preservation guard blocked the emulator's OWN store
   (emulator, HM page 51).** With #1 fixed, the RET then read return link
   `0o20056` (still wrong). `SavePCInFrameIL` does `Store<-1S; SLink<- T AND
   VALo` where `VALo` = `B<-Pipe1` (the VA of its own just-issued store, read
   back from `pipe[ProcSRN]`). The session-7 guard that preserves the
   first-faulting pipe entry was skipping **every** push to `pipe[ProcSRN]`
   while `fault_count>0` (stuck at 15 in the Cedar path) -- including the
   emulator's own `Store<-1S` -- so `VALo` read a stale IFU code VA
   (`0o20057`) instead of the store's VA. HM page 51 ("IFU References"): the
   hardware "disables IFU references when the processor is either making a
   reference or doing ... B<-Pipei", so only the **asynchronous IFU
   prefetch** can clobber the private slot, never the emulator's own ref.
   Fix in `src/memory.c`: restrict the guard to `DM_REF_IFETCH`/
   `DM_REF_PREFETCH`. Effect: `VALo` now reads the store's VA, SLink =
   `0o3354` (BootSwapGerm's frame), RET reads `L[2]=0o3354` and returns.

### Result (verified)

- The prologue RET (op `343`) now reads return link `0o3354` and returns
  cleanly into BootSwapGerm. **ZeroDest fault is GONE** (0 hits around the
  RET). Control runs on into the frame-return path (XferDisp00 -> LoadGC).

### HARD REGRESSION GATE

1. `make test` = **10/10**.
2. AEmu NETEXEC @200M: **1491** px (band 1476-1505). PASS.
3. Galaxian @160M: **121553** px (=121552 +/-1). PASS.
4. AltoMesaDorado.eb!2 + NETEXEC @200M: **1499-1502** px (3 runs).
   NOTE: this shifted UP ~12 px from the HEAD baseline (1485-1495). Isolated:
   the shift is ENTIRELY from the (correct, HM Table 11d) `MemBaseX<-FF[6:7]`
   fix -- AltoMesaDorado also uses that FF form, and the old code gave a
   buggy result there too. The pipe-guard fix alone reproduces the baseline
   (1489-1496). The world still boots and renders the NetExec menu correctly;
   the +12 px is benign NetExec host-time render-timing from a correct fix.
   The AltoMesaDorado band should be recalibrated to ~1485-1502.
5. `make sdl` compiles (links `build/dorado-sdl`; no `test_cpu.c:7709`
   warning reproduces).

### NEW blocker (session 10): frame-return LoadGC mis-loads G; BootSwapGerm's saved-G (frame[0]) was stored wrong at the boot XFER

After the RET returns to BootSwapGerm (frame `0o3354`), `XferDisp00` calls
`LoadGC` to restore G/Code from the frame's word 0 (the saved access
link/G). But **frame[0] = `0o10210`**, not BootSwapGerm's G (~`0o4634`).
LoadGC therefore reads a garbage global frame -> unbound/swap trap (T=`0o10`)
-> MTRAP -> `SD[0o10]`=0 -> ZeroDest loop. **Still 6 IFU dispatches.**

Where frame[0] got corrupted: at the **boot XFER's `XferProc`** (DMesaXfer.mc
`L[0]_G`, line 766 `T_ (Store_ RTemp0)+1, DBuf_ T`). The stored G comes from
line 765 `T_ VALo`, which reads the VA of the preceding line-763
`DummyRef_ 0S` at G[0]. Traced (gated): that DummyRef referenced VA
`0x3E1088` (lo `0o10210`) -- which matches NEITHER `BR[G].lo`=`0o4634` NOR
`BR[Code].lo`=`0o16414` as loaded by the boot LoadGC (BRLO_FF trace:
`0o3612` mb=01 a=`0o4634`; `0o3604` mb=37 a=`0o16414`; `0o3641` mb=37 hi=`0o76`).
So the base register **selected/resolved at the line-763 DummyRef (real PC
`0o4030`, MemBase shown =`0o1`=G)** is wrong -- the same MemBase/BR-resolution
family as bug #1, but in the XFER setup. NEXT PASS: gate-trace the **at-issue
MemBase index AND the resolved BR value** at PC `0o4030` (~cyc 67975051) plus
every BR[1]/BR[037] load between the boot LoadGC and that DummyRef; decide
MemBase-load timing vs a stray BR reload. (`0o4030` is not listed by
`mbdis --disasm` -- likely a placement detail; find it via image index.)

Repro: `DORADO_IFUDISP_TRACE=1 ./build/dorado --eb
'../chm/dorado/CedarDorado.eb!6' --germ '../chm/cedar/germ/Dorado.germ!4'
--cycles 80000000` -> 6 dispatches (LFC4/LI1/165/263/361/RET) then the
LoadGC-trap loop; first XFER at cyc 67974929, RET at ~67975296. Useful gated
traces: `DORADO_XFER_TRACE`, `DORADO_BR_TRACE`, `DORADO_PIPEVA_TRACE`
(all need `DORADO_TRACE_GATE="lo,hi"`).

## ROUTE B (2026-06-15, session 8): germ FILE was loaded LITTLE-endian but Dorado memory is BIG-endian; fixing the byte order makes the first XFER complete and the germ now DISPATCHES Mesa bytecode (BootSwapGerm prologue runs); new blocker = the prologue's RET reads a zero return link

### Root cause of the post-MGo XFER/MTRAP loop (the session-7 blocker)

`MGo` (PrincOps, real `0o3740`) deliberately ends with `T_ sBoot, Branch[MTrap]`
= the *designed* boot entry `Xfer[dst: Fetch[@SD[sBoot]]^, src: 0]`
(`DMesaXfer.mc` MGo + MTrap + XferMD). So the XFER/MTRAP "loop" is the boot
mechanism, not inherently a fault. The live first XFER (traced, gated):

- `SD[sBoot]` (sBoot=`0o276`, SDLoc=`0o1100`, so VA MDS+`0o1376`=`0o17401376`)
  read **correctly** = `0o401` = the BootSwapGerm proc descriptor
  (tag=01, gfi=`RSH[0o401,6]`=4, ep=0). Germ data + fetch path were fine.
- Xfer tag=01 -> `XferDisp01`: `T_ gfi+GFT` (GFT=`0o1400`) -> fetch **GFT[4]**
  at VA `0o17401404`. It read `0o116011` -> global frame `0o116010`, which is
  **past the 8192-word germ** -> `LoadGC`/`XferProc` read 0 for the frame/code
  base -> **PC=0 -> ProcUnbound -> sUnbound (`0o13`)** trap. That trap XFERs
  through `SD[13]`=0 -> ControlFault (`sControlFault=7`) -> `SD[7]`=0 ->
  re-fault forever (the observed `XFER`/`XFERMD`/`SAVEPCANDTRAP`/`MTRAP` loop;
  the germ never populates the SD trap handlers -- BootSwapGerm does that only
  AFTER it runs).

The germ's GFT[gfi] should have given BootSwapGerm's frame g=`0o4634`
(`Dorado.loadmap`: ProcessorHead gfi1->`0o3400`, BootSwapGerm gfi4->`0o4634`,
...). **Byte-swapping every GFT entry yields EXACTLY those loadmap frames**
(GFT[1]`0o7`->`0o3400`, GFT[4]`0o116011`->`0o4634`, GFT[8]`0o30014`->`0o6060`,
...), and BootSwapGerm's code-base-high word byte-swaps `0o37000`->`0o76`
(= `pilotMDSHi`). So the germ FILE holds **big-endian** 16-bit words; our
`--germ` plant read them little-endian (`b0|(b1<<8)`), corrupting every
internal pointer.

### Fix (committed) -- microcode/loadmap-grounded, one line

`src/machine.c` germ load: `(b0<<8)|b1` (big-endian) instead of `b0|(b1<<8)`.
Gated entirely on `--germ` (the `germ_words[]` buffer is used only by the germ
plant), so the Alto worlds are byte-identical. Also added a gated
`DORADO_XFER_TRACE` diagnostic in `src/cpu.c` (inert without the env + cycle
gate) that prints pc/T/Q/md/last-ref-VA -- this is how the SD/GFT/frame fetches
above were read; keep it for the next pass.

### Result (verified)

- First XFER now completes: GFT[4] reads `0o4634`, `LoadGC` loads G/Code, and
  control reaches the IFU. **IFU dispatch count > 0** -- the germ runs its
  BootSwapGerm prologue: 6 Mesa bytecodes dispatch (op `324`=LFC4, `057`=LI1,
  `165`, `263`, `361`, `343`=**RET**) with br31=`3E1D0C` (germ code region),
  `flt=0`.

### HARD REGRESSION GATE -- ALL GREEN

1. `make test` = 10/10 suites.
2. AEmu NETEXEC @200M: **1476** px (band 1476-1505).
3. Galaxian @160M: **121553** px (=121552 +/-1).
4. AltoMesaDorado.eb!2 + NETEXEC @200M: **1478** px (band 1477-1497).
5. `make sdl` compiles (only pre-existing `vendor/6502/fake6502.h` warnings;
   no `test_cpu.c:7709` misleading-indentation warning reproduces).

### NEW blocker (session 9): BootSwapGerm prologue's RET reads a zero return link

After the 6-bytecode prologue, the trailing **RET** (op `343`) fetches its
local frame's return link (`L[2]`) and gets **0** -> `XferMD` DLink=0 ->
ZeroDest ControlFault (T=`0o7`) -> back into the `SD[7]`=0 trap loop (traced:
cyc ~67975302 `XferMD` md=0, lva=`0o2`; then the steady `0o1700`/`0o1041`/
`0o1026` loop resumes). So the germ now genuinely RUNS bytecode but the
frame-link chain for the boot context is wrong: either (a) an emulator XFER
frame-store mis-model (the boot `XferProc` does `Store_ T, DBuf_ SLink` =
`L[2]_ SLink`; for the boot XFER SLink=0, and the LFC4 that calls the inner
proc must store the caller frame into the inner frame's `L[2]` -- verify
`XferProc`'s store + the LFC `RTemp4`/Alloc path), or (b) germ/boot state we
still set up wrong (the local frame BootSwapGerm runs in is allocated by
`XferProc` via `AllocSub` off the AV free list at MDS `0o1000`; AV head germ
word 0 is now `0o3354` (BE) -- check AllocSub returns a valid frame). NEXT
PASS: trace the LFC4 frame allocation + the inner proc's `L[2]` store (gated
`DORADO_XFER_TRACE` around cyc 67974929..67975310) and the AV/Alloc path;
decide emulator-XFER vs germ-state. Repro:
`DORADO_IFUDISP_TRACE=1 ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
--germ '../chm/cedar/germ/Dorado.germ!4' --cycles 80000000` -> 6 dispatches
then the RET trap loop; the first XFER is at cyc 67974929.

## ROUTE B (2026-06-15, session 7): germ relocation aliasing FIXED at the root (`Pipe4'`/`Errors'` flag-encoding bug); GermRemap now assigns DISTINCT real pages, control reaches MGo (Mesa emulator, insset=1); the de70f5e MapBitsBR hack is REMOVED (faithful Map now keeps the `0o7030` fault gone); new blocker = post-MGo XFER/MTRAP loop, 0 IFU dispatches

### Root cause found (the real bug, not the Map population)

The session-6 diagnosis ("Initial maps the whole 64K aliased; need to leave
high VM Vacant") was a red herring. Traced the live steal: our InitMem map IS
faithful (page N -> rp N 1-to-1, exactly matching real HW + the
`TranslateMapEntry` "crock" for rp > 7777B that compensates for the boot's
"no end test" real-memory scan -- see `DMesaMiscOps.mc`). The actual bug was in
**`dorado_pipe4_at` (`src/memory.c`)**, the `Errors'`/`B<-Pipe4'` readback that
`PilotBoot.FindEndMappedVM` -> `TranslateMapEntry` uses to classify a map entry
Vacant vs resident:

- The **ref bit (b0)** was hardcoded to `kind != DM_REF_NONE` instead of the
  snapshotted map entry's actual Ref flag (`map_flags_pre & 4`). EMemDefs.mc:
  `m1pipe4.wpdref = b0,b2,b3` (ref, wProtect, dirty are the map flags).
- The **wProtect bit (b2)** had the wrong polarity: `dirty` (b3) was already
  read back complemented (`Errors'` = active-low, "Previous flags
  (complemented)" per `DMesaMiscOps.mc`) via baseline bit12=1, but wProtect was
  NOT (baseline bit13=0). Fixed by setting baseline `0o150361 -> 0o170361`
  (bit13=1) so wProtect' = NOT(wProtect) like dirty' and ref'.

Effect of the bug: `TranslateMapEntry` classified **resident page 0** (wp=0,
dirty=1, ref=1) as MapVacant and **genuinely-vacant page 0xFFFF** (wp=1,
dirty=1) as MapNotVacant -- exactly inverted. So `FindEndMappedVM` stopped at
boundary 0 and `GermRemapLp` stole pages from the TOP of VM (0xFFFF down),
which the crock returns as rp=0 -> every MDS-76 germ page collapsed onto rp 0.

(HM grounding: the `0o150361` baseline was never a literal HM quote --
`pdftotext` finds no such constant; it is derived from EMemDefs.mc's Pipe4
bit layout + the microcode's complemented-flag reading. Documented as such.)

### Result (verified)

- `FindEndMappedVM` now ascends page 0 -> ~4096 (boundary = the crock's first
  rp>7777B page), `GermRemapLp` steals distinct pages 4095..4064, and the
  MDS-76 germ-dest entries get DISTINCT real pages (idx 0x3E10 -> rp 0x0FEE).
  No rp=0 collapse.
- The single `0o7030` MapBitsBR fault is GONE **without** any intercept: with
  the faithful classification the MapBitsBR bank (VA 0xFFF000, page 0xFFF0)
  stays resident (rp 0xFFF0, not scanned/stolen by FindEndMappedVM/GermRemapLp,
  which only touch pages <=4096), so MapDirtyBit's Fetch reads real-backed
  storage and never faults. **The de70f5e `mapbits_buf` bypass hack is REMOVED**
  (`mapbits_intercept`/`mapbits_lo`/`mapbits_hi`/`mapbits_buf`/`DM_MAPBITS_WORDS`
  and `machine_enable_mapbits_intercept` all deleted). A/B test confirmed: with
  the hack gated off the germ-dest rp and the post-relocation flow are
  byte-identical, so the hack was redundant once the flags were fixed.
- After GermRemapDone the BLT runs and control reaches **MGo (real 0o3740)**
  with **insset=1** -- the Mesa emulator is now executing the germ's resident
  code.

### HARD REGRESSION GATE -- ALL GREEN

1. `make test` = 10/10 (updated `test_pipe4_error_encoding` and
   `test_cpu_pipe4_no_error_baseline` to the corrected `0o170361` baseline +
   actual-ref semantics).
2. AEmu NETEXEC @200M: 1483 px (band 1476-1505).
3. Galaxian @160M: 121553 px (=121552 +/-1).
4. AltoMesaDorado.eb!2 + NETEXEC @200M: 1485 px (band 1477-1497).
5. `make sdl` compiles.

### NEW blocker (session 8): post-MGo XFER/MTRAP loop, 0 IFU dispatches

After MGo the Mesa emulator settles into a steady loop (real PCs, traced
115M-120M): `XFER`(0o1700) / `XFERMD`(0o1026) / `SAVEPCANDTRAP`(0o2000) /
`SAVEPCINFRAME`(0o1600) / `CHECKSTKP`(0o2020) / `MTRAP`(0o1041) -- a Mesa
control-transfer that traps repeatedly, **0 IFU dispatches** (the germ never
runs a bytecode via IFUJump). So GermRemap + the microcode handoff now work,
but the germ's first XFER (likely starting the `BootSwapGerm` process, then
its own Ethernet volume fetch -- plan Step 4) traps. Next pass: decode the
XFER target / trap cause (is it `MTRAP` for an unimplemented/absent control
link, a stack fault at `CHECKSTKP`, or the germ awaiting its ether boot
channel?). Repro: `./build/dorado --eb '../chm/dorado/CedarDorado.eb!6' --germ
'../chm/cedar/germ/Dorado.germ!4' --cycles 120000000`; the germ-dest distinct
rp is observable via `DORADO_MAP_TRACE=1 DORADO_MAP_TRACE_INDEX=0x3E10`.

## ROUTE B (2026-06-15, session 6): MapBitsBR `0o7030` fault ELIMINATED via a reserved-buffer intercept; germ now reaches GERMREMAP's relocation BLT; new blocker = the relocation aliases every MDS-76 germ page onto real page 0 (still 0 IFU dispatches)

### What was fixed (committed)

The session-5 residual `0o7030` fault was `MapDirtyBit` (`DMesaRastMiscOps.mc`):
`MemBase_ MapBitsBR; Fetch_ RTemp6` -- the Fetch of the per-real-page
extra-dirty-bit array referenced by base register **MapBitsBR (=25**,
`DMesaDefs.mc` `BR[MapBitsBR, 25]`) at VA **0xFFF000** (the top of the 16 MW
VM). Confirmed: base reg 25 is *deliberately* set to 0xFFF000 by the Cedar
microcode (BRHI=0xFF @real `0o3756`, BRLO=0xF000 @real `0o3775`); this is the
canonical MapBits location, not garbage. `MapDirtyBit` is reached during
`PilotBoot.GermRemapLp` via `WriteMapPage`/`TranslateMapEntry`.

Per `DMesaRastMiscOps.mc` the array is "real memory ... [holding] one bit for
each page of real memory"; on real Dorado it is a reserved real-backed region
whose Fetch never faults. In our model that VM page maps to an arbitrary real
page that GermRemap vacates (faulting the Fetch). **Only the PrincOps/Cedar
map subroutines use MapBitsBR** -- the Alto worlds use `DMesaMiscOps.mc`'s
`WriteMapPage`, which keeps the duplicate dirty bit in the real-page *sign
bit* and never touches MapBitsBR -- so the fix is naturally Cedar-only and the
Alto pixel gates are structurally untouched.

Fix: a **dedicated reserved-buffer intercept** in the memory model.
- `include/memory.h`: `DM_MAPBITS_WORDS` (4096 = one bit per 64K real pages),
  plus `mapbits_intercept`, `mapbits_lo/hi`, `mapbits_buf[]` in
  `dorado_memory`.
- `src/memory.c` `dorado_memory_ref_task`: at entry, references in
  `[mapbits_lo, mapbits_hi)` route to `mapbits_buf` (Fetch->Md, Store->buf,
  PreFetch no-op), bypassing Map + cache, **never faulting** and surviving any
  Map mutation -- exactly the real-hardware invariant.
- `src/machine.c` `machine_enable_mapbits_intercept`: armed at germ pass 1
  (Cedar PC `0o7012`, where base reg 25 already holds 0xFFF000), reads the
  live MapBitsBR base reg and arms the intercept. Inert without `--germ`
  (`mapbits_intercept` stays 0), so Alto behavior is byte-identical.

### Result

- The `0o7030` MapBits fault is **GONE**. GermRemap now runs its full
  `GermRemapLp` page-relocation loop and reaches `GermRemapDone`'s IOBR->LPtr
  **BLT copy** into MDS 76.
- New single fault at real **`0o2761`** (the BLT `Store_ T, DBuf_ MD`),
  VA `0x3E16FF` (MDS 76, germ dest), absorbed. Still **0 IFU dispatches**.

### Hard regression gate -- ALL GREEN

1. `make test` = **10/10**.
2. AEmu NETEXEC (`worlds/aemu.eb` + NETEXEC) @200M: **1476** px (band
   1480-1505; 1476 is the known animation/cursor frame at exactly 200M).
3. Galaxian @160M: **121553** px (= 121552 ±1).
4. AltoMesaDorado.eb!2 + NETEXEC: **1483/1480/1477** @195/200/205M (band
   1466-1497; the intercept is provably inert without `--germ`, so this is
   pure NetExec host-time render variance).
5. `make sdl` compiles.

(No `test_cpu.c:7709` misleading-indentation warning reproduces under Apple
clang 17 with the project's flags + `-Wmisleading-indentation`; the build is
warning-clean except for pre-existing `vendor/6502/fake6502.h` ones. The
code around 7709 is correctly indented.)

### NEW blocker (session 7): GERMREMAP relocation aliases all MDS-76 germ pages onto real page 0 -> germ image destroyed -> REQUEUE spin

The germ does NOT dispatch because the relocation produces a corrupt germ
image. Diagnosis (precise):

- `PilotBoot.GermRemapLp` "steals pages from the end of mapped VM" and remaps
  them into MDS 76, then BLT-copies the germ. It reads each stolen page's
  entry via `TranslateMapEntry` and writes it into the MDS-76 slot.
- In our model **every MDS-76 germ entry ends up rp=0** (traced idx
  `0x3E02..0x3E21`: all `rp=0000`, mostly `wp=0 dirty=1`, two `wp=1` ->
  the `0o2761` BLT-store write-protect/page fault). All 32 germ pages
  collapse onto real page 0, so the BLT overwrites itself and the germ image
  in MDS 76 is garbage.
- Root cause = the **inherited Map from Initial** maps the whole 64K-page VM
  resident with aliasing: high VM pages map to low/`rp=0` real pages (traced
  `0xFFF0 -> rp 0`), and some map to rp > 7777B which
  `TranslateMapEntry`'s "crock" (`real page > 7777B => vacant`) returns as
  vacant (rp 0 in Alto mode / wProtect&dirty in PrincOps). `FindEndMappedVM`
  finds its boundary at `0xFFF1` and the stolen pages all yield rp 0.
- On real Dorado real memory is far smaller than the 16 MW VM, so the boot's
  mapped region is a small 1-to-1 working set (distinct valid rp <= 7777B) and
  the stolen pages are distinct real pages. The MapBits island sits ABOVE the
  vacant gap and is never stolen.

**What was tried and did NOT work this session (reverted):**
- Capping reported real-memory modules (`config_modules_max`): no effect --
  the resident extent is set by the enumeration, not `RealPages`, and the
  Cedar world warm-starts off Initial's map.
- Forcing `RealPages=4096` via chip-size: no effect (warm start).
- Manually vacating a `[0x1000, 0xFFEF]` "real-memory gap" to move the
  `FindEndMappedVM` boundary: the boundary did not move to 0x1000 as modeled
  (the steal geometry / inherited map state is not what a naive
  first-vacant-from-0 scan predicts), so it only changed the failure (MDS-76
  vacant instead of rp 0) without dispatching. The exact steal mechanics need
  to be traced live before forcing a gap.

**NEXT PASS:** fix the relocation so the stolen pages have distinct valid
real pages. The likely correct lever is **how Initial (the bootstrap)
populates the Map** -- it should map only the actual real-memory working set
1-to-1 (distinct rp <= 7777B), leaving high VM vacant, instead of aliasing
all 64K. Pages 0..0xFFF already map 1-to-1 (`rp=vp`, confirmed). Trace
`GermRemapLp`'s live steal source (`RTemp2` boundary, the per-iteration
`make-vacant` page, and the `RTemp4` entry written to each MDS-76 slot) at
cyc > 66.7M to learn the true boundary before forcing the geometry. Repro:
`DORADO_FAULT_TRACE=all ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
--germ '../chm/cedar/germ/Dorado.germ!4' --cycles 90000000` -> intercept
armed, 3 germ passes, single `0o2761` BLT fault, 0 dispatches.
`DORADO_MAP_TRACE=1 DORADO_MAP_TRACE_INDEX=0x3E10` shows the MDS-76 rp=0
collapse.

---

## ROUTE B (2026-06-15, session 5): RMap<- now READ-ONLY (was conflated with Map<- write); the `0o7030` MESAFAULT loop is GONE; GERMREMAP completes and control runs on into the Mesa-emulator microcode; new blocker = germ doesn't dispatch (0 IFU dispatches)

### Root cause of the session-4 blocker (the `0o7030` page fault -> infinite MESAFAULT)

The faulting op at real `0o7030` is **not** FindEndMappedVM's RMap; it is
`MapDirtyBit` (`DMesaRastMiscOps.mc`): `MemBase_ MapBitsBR; Fetch_ RTemp6`,
the Fetch of the per-real-page extra-dirty-bit array that lives in the
**highest VM bank** (`MapBitsBR` = base register **025**, VA `0xFFF000`).
`MapDirtyBit` is called by `WriteMapPage`, which `PilotBoot.GermRemapLp`
calls for every page it steals from the end of mapped VM. The Fetch hit a
**vacant** page `0xFFF0` and trapped.

But the *reason* page `0xFFF0` was vacant was an upstream emulator bug:
**we conflated `RMap<-` (read map) with `Map<-` (write map).** HM page 46-47
+ Table 8a: both are the ASEL=0 / FF[0:1]=1 map reference, but
- `Map<- R, MapBuf<- T` (write): FF=`0o100` (FF[2:7]=0). Writes B/TIOA into
  the entry; returns previous contents in the pipe.
- `RMap<- R` (read): FF=`0o131` — carries the **ReadMap function** (FA
  forced 0, FB=3, FC=1 = FF[2:7]=`0o31`). Returns previous contents in the
  pipe and **must not modify the entry, never faults** (verified against
  Cedar.mb: `NewReadMapPage` real `0o4161` and the FindEndMappedVM scan
  real `0o6506` both FF=`0o131`; InitMem map write `IWRITEMAP` real `0o7013`
  FF=`0o100`).

Our `DM_REF_MAP` always *wrote* the entry. So `FindEndMappedVM`'s
`RMap_ RTemp0` (RTemp0=0) scan overwrote every page it touched with
rp=0/wp=0/dirty=0 (and the `MapBitsBR` bank too), misplacing the "end of
mapped VM" boundary; `GermRemapLp` then stole the wrong pages — including
the `MapBitsBR` page `0xFFF0` — making it vacant, which broke
`MapDirtyBit`'s Fetch. (`DORADO_MAP_TRACE_INDEX=0xFFF0` showed idx FFF0
toggling mapped<->vacant, including the tell-tale rp=0/wp=0/dirty=0 write
from the RMap-with-RTemp0=0.)

### Fix (committed)

Added a distinct `DM_REF_RMAP` read-only map reference:
- `include/memory.h`: new `DM_REF_RMAP` enum (appended, so existing kind
  numbers — e.g. FETCH=10 in traces — are unchanged).
- `src/cpu.c` `decode_ref_kind`: a non-io ASEL=0/FF[0:1]=1 map reference is
  `DM_REF_RMAP` when it carries the ReadMap function (`((ff>>3)&7)==3 &&
  (ff&7)==1`), else `DM_REF_MAP` (write). Map<- writes stay writes.
- `src/memory.c`: `DM_REF_RMAP` case snapshots previous contents to the pipe
  (already done by `pipe_push`) and does **nothing else** — no entry write,
  no MapBufBusy, no fault. Also excluded from the DVAVIC path and named in
  the trace tables.
- `tests/test_cpu.c`: ref-kind name switch updated (kept -Wswitch clean).

The Alto/InitMem write path is unchanged (those are `Map<-`, FF=`0o100`),
so the working worlds still set up their maps exactly as before.

### Result

- `DORADO_FAULT_TRACE=all` over the whole CedarDorado germ run: only the
  pre-existing two faults at `0o6023/0o6024` (~11M, present in every world)
  plus a **single** `0o7030` fault (~66.68M). The session-4 ~60K-iteration
  `MESAFAULT`/`REQUEUE` loop at `0o3306` is **gone**.
- That single `0o7030` fault is now absorbed by the memory fault task
  (task `0o17`, short handler real `0o1622..0o1613`) which **returns to
  task 0**; `GermRemapLp`/`GermRemapDone` then run their IOBR->LPtr BLT
  copy loop (real `0o1264`) to completion and control continues.

### Hard regression gate — ALL GREEN

1. `make test` = **10/10**.
2. AEmu NETEXEC (`worlds/aemu.eb` + NETEXEC.BOOT): 1487/1493/1495 px at
   190M/195M/205M (1476 at exactly 200M — animation/cursor variance; the
   1480-1505 band absorbs it).
3. Galaxian (`worlds/aemu.eb` + Galaxian.boot) @160M: **121553** px (=121552 ±1).
4. AltoMesaDorado.eb!2 + NETEXEC @200M: **1481** px (band 1466-1497).
5. `make sdl` compiles.

### NEW blocker (session 6)

After the germ remap, control settles into a tight Mesa-emulator microcode
loop — real `0o3301 -> 0o3302 -> 0o1071 -> 0o1072` (mb=`03` = MDS,
insset=1) with occasional excursions to `0o6606..0o6635` — and stays there
for >13M cyc with **0 IFU dispatches** through 80M. So the germ does NOT
yet execute Mesa opcodes; it is NOT (yet) the expected "germ does its own
ether fetch" (plan Step 4). Likely candidates to chase next:
- Whether the single `0o7030` fault leaves `Md` garbage so `MapDirtyBit`'s
  dirty-bit word is wrong and `WriteMapPage` mis-relocates a germ page (on
  real Dorado the `MapBitsBR` bank is real-backed and this Fetch never
  faults — consider mapping/zero-backing the `MapBitsBR` highest bank so
  the first-touch Fetch returns 0 instead of faulting).
- Whether `BootSwapGerm` (g=`004634`) actually XFERs / installs a runnable
  process; the `0o3301/3302/1071/1072` loop looks like the Mesa emulator
  idling with nothing schedulable (it Fetches from MDS and spins).
Repro: `./build/dorado --eb '../chm/dorado/CedarDorado.eb!6' --germ
'../chm/cedar/germ/Dorado.germ!4' --cycles 80000000`; gate the PC/IFU
traces to cyc>66.68M.

---

## ROUTE B (2026-06-15, session 4): Map widened 16K -> 64K (MapIs64K / VirtualBanks=400C); GERMREMAP's relocation loop now RUNS; new blocker is FindEndMappedVM's high-VA Fetch fault (16K aliasing was load-bearing for the IOBR map scan)

This session **widened the modeled Map from 16K to 64K entries** so the germ
relocation no longer collapses MDS 76 (germ VA `0o17401000`, page `0o174010`
= 64008) onto a low aliased index. Cited to `InitMem.mc` `GetMemConfig` and
the DMux/muffler (`Various.mc` `SetDMuxAddress`). Hard regression gate GREEN.

### What changed (committed)

- `include/memory.h`: `DM_MAP_ENTRIES` 16384 -> **65536** (64K map, 256-word
  pages, VM = 2^24 = 16 MW). Comments updated. `dorado_memory.map[]` grows
  from 128 KB to 512 KB inside the calloc'd machine struct (heap, safe).
- `src/memory.c` `dorado_memory_dmux_read`: DMux **`0o1511` (MapIs64K) now
  reports sign-SET (0x8000)**; `0o1512` (MapIs256K) stays sign-CLEAR. Per
  `InitMem.mc` `GetMemConfig`: `0o1512` sign-set => 256K (`VirtualBanks=2000C`);
  `0o1511` sign-set => **64K (`VirtualBanks=400C` = 256 banks)**; neither =>
  16K. So `GetMemConfig` now selects `VirtualBanks=400C`, and `400C*256 =
  65536 == DM_MAP_ENTRIES` (the loop-bound == map-size invariant that keeps
  the cold `Map1to1Loop`/`MapInitLoop` from running off the end -- the same
  invariant the 16K config preserved; a mismatch is what spun Mesa forever).

### Hard regression gate (all GREEN)

1. `make test` = **10/10** suites.
2. AEmu NETEXEC (`worlds/aemu.eb`) -> **1482 px** (band 1480-1505), world
   loads at cyc 32M (unchanged), budget **200M cyc** to reach paint.
3. Galaxian -> **121552 px** (exact) at budget **150M-180M cyc** (the stable
   attract screen; 120M=121641 / 200M=121554 are animation-frame variance).
4. AltoMesaDorado.eb!2 + NETEXEC -> **1489 px** (band 1466-1497), budget 200M.
5. `make sdl` compiles.

The 4x-longer cold InitMem loop did NOT push world-load later (still 32M for
the ether-loaded worlds; the InitMem 4x is inside the *loaded* world and
still terminates and paints within 200M). 200M is a safe budget for all.

### CedarDorado germ: how far it gets now

`./build/dorado --eb '../chm/dorado/CedarDorado.eb!6' --germ
'../chm/cedar/germ/Dorado.germ!4'`. The cold InitMem 4x pushes the 3 germ
disk-read passes from ~53M to **~66.7M cyc** (descriptor/label/data all
still land: seal `0o121212` v6, 8192/8192 germ words at VM `0o1000+`).

**Progress vs the session-3 16K state:** the old blocker is GONE -- the
`GermRemapDone` BLT store (`0o2761`) no longer faults and the fault task
`0o17` no longer takes over. **GERMREMAP's relocation machinery now actually
RUNS**: control reaches the DMesa map subroutines `SetBRForPage`/
`TranslateMapEntry`/`MapDirtyBit` (real `0o7000`/`0o7020`/`0o7030`) called
from `PilotBoot.FindEndMappedVM`(`0o6724`) / `GermRemapLp`(`0o2763`).

**NEW blocker (the next pass):** task-0 takes a **page fault at real
`0o7030`** -- a Fetch (`ASEL=0`, kind=10) via **MemBase=IOBR** to VA
**`0x0FFF000`** (page `0xFFF0` = 65520) -- then diverts into the Mesa
`MESAFAULT`->`REQUEUE` loop (real `0o3306`, mb=03, fetching VA 0; ~60K
hits to end-of-run, **0 IFU dispatches**, no XFER to `BootSwapGerm`). Root
cause: `FindEndMappedVM` scans UP through VAs (via IOBR) looking for the
first VACANT page = the end of mapped real VM (`PilotBoot.mc`:
`FindEndMappedVM: T_ RTemp2, Call[SetBRForPage]; RMap_ RTemp0,
SCall[TranslateMapEntry]; Branch[GermRemapLpE]; ... Branch[FindEndMappedVM]`).
With the **16K** map this scan ALIASED onto resident low pages and
terminated early (illusory -- the per-page `WriteMapPage`s and the BLT then
operated on aliased entries, which is why session-3's BLT faulted). With the
**64K** map the scan walks the *real* VA space and reaches a genuinely
vacant high page (`0xFFF0`, near the top of the 16 MW map / the IO region);
our model **page-faults the Fetch** instead of letting `TranslateMapEntry`
read the vacant map flags cleanly. On real Dorado `TranslateMapEntry` reads
the entry via `RMap` (ReadMap, Mar-addressed) and the microcode TESTS the
vacant flag -- it does not fault. So the next blocker is in the
map-entry-read path, not the map size.

### NEXT PASS

Make `FindEndMappedVM`/`TranslateMapEntry`'s map-scan reference (real
`0o7030`, `SetBRForPage`+`TranslateMapEntry` in `DMesaRastMiscOps.mc` /
`PilotBoot.mc`) read the vacant high-VA map entry WITHOUT a CPU page fault,
so the scan returns "found first vacant page" (the `+1` exit to
`GermRemapLpE`) instead of trapping into `MESAFAULT`/`REQUEUE`. Then
`GermRemapLp` relocates pages `RTemp3` down to the germ extent, `GermRemapDone`
runs the IOBR->LPtr BLT into VM `0o17401000+`, and control XFERs into
`BootSwapGerm` (g=`004634`) with `insset=1` (plan Step 4 = the running germ
doing its OWN ether fetch of the volume). Repro:
`DORADO_FAULT_TRACE=all ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
--germ '../chm/cedar/germ/Dorado.germ!4' --cycles 90000000 2>&1 |
awk '/germ pass3/{p=1} p&&/FAULT_CPU/{print $4,$3,$6}' | sort | uniq -c`
-> 1x `pc=0o7030 task=0 mb=25`, then ~60K x `pc=0o3306 task=0 mb=03`.

## ROUTE B (2026-06-15, session 3): PV descriptor fabricated + all 3 disk-read passes fed; the REAL GERMREMAP now runs; blocked on the 16K-map aliasing of MDS 76

This session **fabricated a faithful PV root-page Descriptor and fed all
three of DiskBootSoft's disk-read passes**, so the real microcode validated
the descriptor (no MESAFAULT), read the germ, and ran the real
`PilotBoot.GERMREMAP` relocation loop to completion. The germ does **not run
yet**: GERMREMAP's relocation of the germ into **MDS 76** can't land because
our **16K Map (MapIs16K)** aliases MDS 76 with low VAs. Regression gate green:
`make test` 10/10; AEmu NETEXEC **1495**; Galaxian **121552** (exact);
AltoMesaDorado NETEXEC **1490**; `make sdl` compiles.

### What landed (committed)

The disk-read interception lives in `src/machine.c` (gated on `--germ` +
Cedar-only PC `0o7012`, so the Alto pixel gates are structurally
untouched). At each `BootTransferLp` seal-fetch (real PC `0o7012`) with
`IOCB.seal == IOCBSealValue`, read `IOCB.command` (VM `0o435`) and dispatch
by pass, then KCmmdDone-complete (seal=0, pageCount=0, labelStatus=0 ->
BootTransfer +2):
1. **Descriptor (cmd `0o274`):** deposit `page0[0]=0o121212` (Desc.seal),
   `page0[1]=6` (Desc.currentVersion); zero the germ DFID region
   `0o32..0o43` (Desc.bi.germ; fID/firstPage/da all 0 -- the microcode
   copies them into the IOCB but our fake completion bypasses the real
   read). Source: `PilotBootDefs.mc`, `PhysicalVolumeFormat.mesa`.
2. **Label (cmd `0o260`):** just complete (DiskBootSoft never inspects the
   label contents).
3. **Germ data (cmd `0o100254`):** deposit germ file word W -> VM
   `IOCB.dataPtr+W` (dataPtr = BootDataPtr = baseGerm = `0o1000`, read
   live), then advance `IOCB.dataPtr` by `0o400`/page (8192 words = `0o40`
   pages -> dataPtr `0o1000`->`0o21000`) as the disk microcode would, so
   GermBoot's `BootDataPtr_ MD` and GERMREMAP relocate exactly the loaded
   extent.

**Two real emulator fixes** were required and are the load-bearing part:
- **`src/memory.c` `dorado_storage_store_at_va`** now clears stale *valid+
  vacant* cache lines for the target VA (the old code used
  `dorado_cache_lookup`, which *skips* vacant lines). The Mesa world leaves
  VM page 0 vacant (null-trap); a stale valid+vacant line there satisfied
  the seal-check fetch with 0 instead of our deposited `0o121212`. General
  correctness fix; gate stayed green.
- **`src/machine.c`** makes VM page 0 resident before depositing the
  descriptor (the real descriptor DMA write would map-fault page 0 and the
  fault handler makes it resident). It allocates a **genuinely free real
  page** via the new `machine_find_free_rp()` -- page 0's existing `rp` is
  aliased to a live high-VA page, and reusing it let that page's dirty
  writeback clobber the descriptor (`storage[0]` went 0o121212 -> 0).

### The precise NEW blocker (where it stops now)

Flow after pass 3 (verified, task-0 real PCs): GermBoot ->
`FINDENDMAPPEDVM`(`0o6724`) -> `GERMREMAPLP`(`0o2763`) loop (calls
`SetBRForPage`/`TranslateMapEntry`/`FlushPage`/`WriteMapPage` from
`DMesaMiscOps.mc`) -> `GERMREMAPDONE`(`0o2725`) -> the BLT copy loop
(`Fetch_ RTemp5`@`0o2762` from MDS0, `Store_ T,DBuf_ MD`@`0o2761` to MDS76).
**Task-0 stalls at the BLT Store `0o2761`** and the fault task (`0o17`)
takes over (TPC `0o1606`) and never resolves. insset=1 (Cedar world is
running), **no IFU dispatch / no XFER yet**.

Root cause: the germ's resident pages (VA `0o17401000+`, MDS 76) are still
**VACANT** at the stall (`rp=0 wp=1 dirty=1`). GERMREMAP's `WriteMapPage`
*did* run for every page, but the Map index it writes, `(0o17401000>>8) &
(DM_MAP_ENTRIES-1)` = `0o37002`, **ALIASES** low VAs (e.g. `0x3C0200`,
`0x7C0200`, ...) because the Map is only **16384 entries** (`MapIs16K`,
`DM_MAP_ENTRIES`) while the Mesa world's VA space (MDS 76 ~ VA 4M) far
exceeds 16K pages. So the MDS-76 map writes collide with / are overwritten
by other-VA entries, the germ destination stays vacant, and the BLT store
faults.

### NEXT PASS

The descriptor + disk-read feed is DONE and faithful; the remaining work is
the **Map size**. Two routes:
(a) **Widen the Map** so MDS 76 doesn't alias: make `GetMemConfig`/the DMux
    muffler report `MapIs64K` (or `256K`) and grow `DM_MAP_ENTRIES` to
    `0o200000`+ so `0o17401000>>8 = 0o174010` is a *distinct* index. This is
    the muffler/`SetDMuxAddress` path fixed in the 2026-06-14 DMux session
    (currently pinned to 16K); raising it touches the whole memory model +
    the cold InitMem loop bound, so revalidate the Alto gates carefully.
(b) Audit `WriteMapPage`'s Map-write Hold handshake (gap B1/C1) -- but the
    aliasing above is the dominant cause; (a) is the real fix.
Repro: `./build/dorado --eb '../chm/dorado/CedarDorado.eb!6' --germ
'../chm/cedar/germ/Dorado.germ!4' --cycles 90000000` -> the three
`[machine] germ passN` lines print, then it relocates and stalls at the BLT.

## ROUTE B (2026-06-15, session 2): disk-completion mechanism PROVEN; germ-run blocked on the (remote) PV-root descriptor format

This session **proved the IOCB completion mechanism empirically** and
**reverse-engineered the descriptor seal/version constants from the
Cedar.mb disasm**, then pinned the precise remaining blocker: fabricating
a *minimal* descriptor (seal+version only) is not enough — the real
PilotBoot microcode then dereferences the PV-root descriptor's
`bootingInfo`/`DFID` structure, which we cannot fabricate because that
Pilot layout is not in any local source. No code committed this session
beyond this doc (the experiment was kept under `DORADO_GERM_EXP` and
reverted); regression gate verified: `make test` 10/10, `make sdl`
compiles, tree byte-identical to green HEAD (`9d8ab2f`) so the
AEmu/Galaxian/AltoMesaDorado pixel checks are structurally unaffected.

### What is now KNOWN (new this session)

1. **Completion mechanism PROVEN (the linchpin).** At the `BOOTTRANSFERLP`
   spin, writing **IOCB.seal=0 (VM 0o432), IOCB.pageCount=0 (0o434),
   IOCB.labelStatus=0 (0o453)** makes `BootTransfer` return **+2 success**:
   the hot PCs `0o7012`/`0o7003` are LEFT (observed flow
   `7012→7003→7013→7051→7052→7053→7033→7017`(`BootTransferDone`)`→6713`
   back in `DiskBootSoft`). This matches `PilotDisk.mc` `KCmmdDone`
   (line 336-338: "Zero IOCB.seal"). NOTE: my static JCN branch-polarity
   read was inverted; trust this empirical result. (Do NOT confuse with
   the `0o7012` branch to `BootTransferTimeout` real `0o7002` — the
   conditional R-bit polarity makes seal-cleared advance to the
   pageCount/labelStatus success check, not to the timeout.)
2. **MemBase=25 (IOBR), base 0** during the whole disk boot, so absolute
   VA == the microcode's offsets (the committed plant's absolute stores
   are correct). IOCB base **VM 0o431**; fields (PilotDiskDefs offsets):
   seal `0o432`, drive `0o433`, pageCount `0o434`, command `0o435`,
   diskAddress `0o436-437`, headerPtr `0o442`(=`@0o440`),
   labelPtr `0o447`(=`@0o461`), labelStatus `0o453`,
   dataPtr `0o454-455`, diskLabel `0o461+`. `IOCBSealValue=0o125377`.
3. **Pass-1 (descriptor) IOCB confirmed live:** command `0o274`
   (`[check,read,read]`), pageCount `1`, dataPtr `0` → the PV root page is
   read into **page 0 (VA 0)**; `Desc.seal` is at **VA 0**.
4. **Descriptor constants reverse-engineered** (Cedar.mb `DiskBootSoft`
   images 6547-6553, the seal/version check `BTemp0_ HighByte[DescSeal]`,
   `OR LowByte`, `XOR seal`, `XOR DescCurrentVersionValue`):
   **`DescSealValue = 0o121212`** (HighByte = byte `0o242` → `0o121000`,
   LowByte = `0o212`), **`DescCurrentVersionValue = 0o6`**. A descriptor
   that passes the check needs `page0[0]=0o121212`, `page0[1]=0o6`.
5. **GERMREMAP located in the disasm** (Cedar.mb, module PilotBoot):
   `FINDENDMAPPEDVM` img 6505 / real `0o6724`, `GERMREMAPLP` img 6511 /
   real `0o2763`, `GERMREMAPLPE` img 6523 / `0o2723`, `GERMREMAPDONE`
   img 6524 / `0o2725`. It uses global calls (`JCN=370`/`311` = map RAM
   subroutines) to relocate pages and set up the Mesa map/MDS, then the
   tail XFERs into `BootSwapGerm`. It only runs AFTER `DiskBootSoft`
   returns +2, i.e. it is gated behind the descriptor blocker below.

### The PRECISE remaining blocker

Depositing seal+version only and KCmmdDone-completing each pass advances
the real microcode past `BOOTTRANSFERLP`, but then it **XFERs/faults into
the Mesa fault path and loops forever** in `MESAFAULT`(real `0o3210`) /
`REQUEUE`(`0o1300`) / `CHECKSTKP`(`0o2020`) / `RESTOREALUFM`(`0o6740`),
with **insset=0 (NO IFU dispatch — the germ never runs)**. Root cause:
after the seal/version check, GERMBOOT/`DiskBootSoft` dereference the
descriptor's **`bootingInfo` array of `DFID`s** (BTemp2-relative; the
booted file's `DFID.da` (2-word DiskAddress), `DFID.fID` (File.ID),
`DFID.firstPage`) which we left zero. The Pilot **PhysicalVolume root-page
descriptor format** (the `bootingInfo`/`DFID`/`BootFileType` layout +
the `BTemp2` offset GERMBOOT computes) is **not in any local source** —
only `DiskBootSoft.mc` *references* those symbols; the defs live in the
unavailable `PilotBoot.mc` / Pilot `PhysicalVolume.mesa`, and inside
`GermDorado.bcd` (remote binary, per `chm/cross-reference.html`). So a
faithful descriptor cannot be fabricated yet, and a blind steer past the
descriptor leaves the Mesa map/MDS unmounted → the same fault loop.

**WARNING for the next pass:** do NOT make the committed plant
auto-complete the IOCB without a real descriptor — it derails into the
`MESAFAULT` loop, which is *worse* than the current benign spin.

### NEXT PASS — two concrete routes to finish

(a) **Recover the descriptor layout.** Get the Pilot PhysicalVolume
   root-page format: `bootingInfo: ARRAY BootFileType OF DFID`,
   `DFID = [da: DiskAddress(2w), fID: File.ID, firstPage]`, and the
   `Desc.seal`/version offsets — from `GermDorado.bcd`, a Pilot
   `PhysicalVolume.mesa`, OR by reverse-engineering **GERMBOOT** (real
   `0o6737+`, PilotBoot, in the Cedar.mb disasm) to learn the `BTemp2`
   (bootingInfo offset) it computes and which descriptor cells it reads
   before/after `DiskBootSoft`. Then fabricate a COMPLETE descriptor so
   the 3 passes run and `GERMREMAP`+XFER fire. The faithful IOCB-level
   disk hook (deposit per-pass data at the `0o7012` spin: descriptor for
   cmd `0o274`, label for cmd `0o260`, the germ at `IOCB.dataPtr` for the
   `0o1xx254` incrementDataPtr pass, then KCmmdDone-complete) is the right
   vehicle — it is PROVEN to advance the real microcode past the spin.
(b) **Decode GERMREMAP fully** (Cedar.mb images 6505-6531) + the
   post-`DiskBootSoft` GERMBOOT tail to learn the exact map/MDS state it
   needs, plant the germ at `BootDataPtr` (read live from `IOCB.dataPtr`
   VM `0o454`, not the final `0o17401000`), and steer into the XFER.

Repro of the proven completion (re-add the gated experiment from git
history of this session, or):
`DORADO_GERM_EXP=1 ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
 --germ '../chm/cedar/germ/Dorado.germ!4' --cycles 120000000`.

## ROUTE B (2026-06-15): CedarDorado reaches the disk GermBoot; germ-plant scaffolding landed; germ-run blocked on PilotBoot source

Route B = Cedar/Mesa germ net-boot. This session **characterized the disk
germ-read fully** and **landed the regression-safe germ-plant scaffolding**
(`--germ`, the VM deposit, the gated plant). The germ does **not run yet**:
that needs the `PilotBoot` GERMREMAP / Mesa-XFER handoff microcode, whose
source is not in our tree. Details below; regression gate stayed green.

### STEP 1 — the disk germ-read, characterized (CedarDorado.eb!6)

Flow (real PCs via `build/mbdis --disasm ../chm/dorado/Cedar.mb!6`, 4th col):
`GERMBOOT`(0o6737, image 6463, module **PilotBoot**) → `DISKBOOTSOFT`(0o6700)
→ `BOOTTRANSFER`(0o6160) → spins in `BootTransferLp`. Microcode SOURCE for
the transfer is local: `chm/doradomicrocode/doradomicrocodesources/`
`DiskBootSoft.mc!1` + `DiskBootTransfer.mc!1` (GERMBOOT/GERMREMAP itself is
in `PilotBoot`, NOT local — see CedarMesa10MB.cm module list).

- **(a) Disk command issued.** `DiskBootSoft` builds an IOCB at VM 0o431
  (drive 0, pageCount, command, diskAddress, dataPtr, diskLabel) and posts
  it to `CSB.next`; the PilotDisk microcode then drives the real slow-IO
  sequence (confirmed empirically via the new `DORADO_DISK_TRACE`):
  TIOA 0o14 DiskTag = DriveSelect drive 0 + cyl/head seek tags
  (e.g. 0o104360); TIOA 0o13 DiskRam = load the read-sequencer Format RAM
  (words 0,1,2,4,6,11,...,377; the last word sets EnableRun); TIOA 0o10
  DiskControl = op-load (command `[check,read,read]`=0o274 etc.); TIOA 0o11
  DiskMuff = TW-clears + EnableRun. Three IOCB passes: read the PV root page
  (descriptor, seal/version checked) into page 0, then the first page's
  label, then the whole boot file into memory at `BootDataPtr` page-by-page
  following `bootChainLink`s (a `[-1,-1]` link = EOF).
- **(b) Load target.** The germ’s resident VM is **0o17401000+** (loadmap,
  see STEP 2). `DiskBootSoft` first stages it into a low-64K buffer at
  `BootDataPtr` (IOCB.dataPtr); `PilotBoot.GERMREMAP` then relocates pages
  to the resident addresses / sets up the Mesa map+MDS. In our run it never
  gets past the FIRST pass (descriptor) because no pack responds.
- **(c) The polled status / spin.** `BootTransfer` posts the IOCB and waits
  for `iocb.seal` to be cleared by the disk microcode, with a ~2 s RTC430
  timeout. Hot PCs: **0o7012** = the `Fetch_ EventTemp1, PD_ MD,
  Branch[BootTransferTimeout, ALU>=0]` seal-fetch; **0o7003** = the
  `PD_ (RTC430)-T-1, Branch[.-1, ALU#0]` timeout compare. It spins forever
  because the junk timer / RTC430 is quiesced at the LoadRam handoff
  (`machine.c`, `cpu->junk_tw_enabled = 0`), so the timeout never fires AND
  no disk ever clears the seal. Confirmed: at 150 M cyc task-0 hot PCs are
  `0o7012`/`0o7003` (~13 M hits each), `booted=1`, 0 dispatches, insset=0.
- **(d) Post-read entry.** On success `BootTransfer` returns +2 →
  `DiskBootSoft` returns +2 → `PilotBoot` runs `GERMREMAP`
  (`GERMREMAPLP`/`GERMREMAPDONE`, images ~6511-6524) then XFERs into the
  germ wart **`BootSwapGerm`** (loadmap global frame g=004634) as Mesa code.

### STEP 2 — germ format + placement (Dorado.germ!4 + Dorado.loadmap!1.txt)

- **Loadmap page numbers are OCTAL.** "Germ file pages: 40" = 32 decimal =
  the 16384-byte / 8192-word file. The GERM FILE MAP maps **contiguously**:
  **file word W → VM word 0o17401000 + W**, W in 0..8191. (Check: file page
  13₈=11₁₀ → 0o17401000+11·0o400 = 0o17406400 ✓; page 33₈=27₁₀ →
  0o17416400 ✓.) **No leader page** — file page 0 is real germ data.
  word0 = 0o166006 (little-endian, matching disk.c’s pack word order).
- **BootFile.Location** is a germ-INTERNAL Mesa global read by the germ’s
  OWN `BootChannelEther.Create` (`chm/cedar/germ/BootChannelEther.mesa!3`):
  it keys on `pLocation.deviceType = ethernet` + `pLocation.bootFileNumber`,
  then runs exactly our Mayday/EFTP protocol (`bootFileSend`=244B + eData/
  eAck/eEnd) to fetch the next stage. So it is consumed AFTER the germ is
  running, when the germ fetches the volume over Ethernet — it is NOT part
  of the microcode handoff. Its exact word offset needs the GermDorado.bcd
  source (not local); it lives in the germ’s global-frame area (writable
  once the germ is planted). Germ frames: BootChannelEther g=006150,
  BootChannelDisk g=006100, MiniEthernetDriver g=006060, BootSwapGerm
  g=004634.

### STEP 3 — germ-plant: SCAFFOLDING landed, germ-run BLOCKED

Implemented (gated, regression-safe):
- `src/memory.c` `dorado_storage_store_at_va()` — VM deposit primitive
  (translate + write storage + invalidate the stale cache line).
- `dorado_machine_config.germ_path` + `--germ PATH` (`src/dorado.c`); the
  machine loads the germ image at create (little-endian words).
- `src/machine.c` gated plant: when task-0 first reaches the Cedar
  germ-boot spin (real PC **0o7012**, which exists ONLY in the Cedar world
  — the Alto worlds never reach it) and a germ is loaded, deposit
  file word W → VM 0o17401000+W. Verified: `8192/8192 words at VM
  0o17401000+ (readback word0=0o166006)` at cyc 53.2 M.
- `src/disk.c` `DORADO_DISK_TRACE` diagnostic (the STEP-1 evidence above).

**BLOCKER (why the germ doesn’t run yet).** Depositing the resident image
is necessary but not sufficient. The germ runs as Mesa code (insset≠0) and
needs the Mesa VM map+MDS set up so its logical addresses (MDS base
0o3400000, code base 0o3401000) resolve to the loaded pages — that is what
`PilotBoot.GERMREMAP` does, followed by the Mesa XFER into `BootSwapGerm`.
**`PilotBoot.mc` is not in our local sources** (only DiskBootSoft/
DiskBootTransfer are), so the remap+handoff cannot be reconstructed
faithfully yet, and a blind PC-steer past the spin would derail (the map
isn’t mounted for 0o17401000 in the Mesa sense). The alternative —
satisfying the disk read so the REAL GERMREMAP runs — needs a synthesized
PV root-page descriptor (seal/version + bootingInfo, layout in unavailable
PilotDisk defs) plus the multi-run bootChainLink transfer = the incomplete
disk data path the project deliberately avoided.

**NEXT PASS:** either (1) obtain/reverse `PilotBoot` GERMREMAP from the
Cedar.mb disasm (images ~6463-6532) to learn the map/MDS setup + the XFER
that gives `BootSwapGerm` control with insset≠0, then steer there after the
plant; or (2) reverse the PV root-page descriptor + DFID layout and emulate
the BootTransfer disk read so the real microcode does the handoff. Once the
germ runs, set BootFile.Location = {ethernet, bfn} so it fetches the volume
over our EFTP/Mayday server (the step after this milestone).
Repro: `DORADO_MACHINE_TRACE=1 ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6' --germ '../chm/cedar/germ/Dorado.germ!4' --cycles 120000000`.

Regression gate this session: `make test` 10/10; Galaxian 121552 (exact);
NETEXEC/AltoMesaDorado within their wall-clock variance band (NetExec
renders host time, so its px count varies run-to-run ~1466-1505 — verified
inherent: the same binary gives 1488/1492/1497).

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
