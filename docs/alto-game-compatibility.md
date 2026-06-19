# Alto game compatibility (Alto-on-Dorado / AEmu world)

Status of the `io/murray` Alto games served over the fake EFTP boot server
(`make run-<game>` / the web build). Updated 2026-06-18.

## Matrix

| Game | Behavior | Notes |
|------|----------|-------|
| Galaxian | Works | attract animates, renders ~121553 px (the gate value) |
| Reversi | Works | playable end-to-end |
| Boggs | Works | eventually draws the Boggs photo (slow) |
| PinBall | Partial | instructions screen renders; **gameplay (post-start) does not** |
| MissileCommand | Broken | playfield clears to black, then frozen; no content |
| Pool | Broken | installs 6 DCBs but renders 0 px (all white), frozen |
| StarWars / Trek | Unverified | in the "no display in budget" bucket |

## What is and isn't the problem (evidence)

The renderer is **not** the gap. `dorado_machine_render_display_list` re-walks
the Alto DCB chain from memory every frame, so anything a game writes to its
bitmap is re-rendered.

Two diagnostics, both env-gated and committed:

1. `DORADO_DCB_TRACE` (machine.c) -- dumps the DCB chain + a per-render bitmap
   hash. Result: the broken games have a **frozen bitmap hash** (their
   per-field draw loop stops updating the bitmap), while Galaxian's hash
   changes every frame. So a display list IS installed and rendered; the game
   just isn't drawing into it.
   - MissileCommand: 1 DCB, mostly-black, hash frozen.
   - Pool: 6 DCBs, all-white (0 px), hash frozen.
   - PinBall instructions: 46 DCBs, 89372 px, correctly static.

2. `DORADO_MACHINE_PCHIST` (machine.c, needs `--progress` on `build/dorado`)
   -- hottest task-0 (AEmu) microcode PCs. Result: the distributions are
   **nearly identical** across MissileCommand, Galaxian, and Pool (same hot
   PCs 0o4000/0o334/0o622/0o4147/0o736/0o755/0o31/0o1245, same magnitudes;
   the round 2^22 / 2^20 counts are deterministic infrastructure loops, not
   game logic). The Dorado microengine is healthy and runs all three games
   the same way.

Interactive check (web build): driving mouse + clicks + keys into the frozen
MissileCommand delivers input to the machine (our XOR mouse-arrow tracks the
cursor) but the game never reacts -- so it is **stuck, not idle-waiting for
input**.

## Deep trace of the MissileCommand stall (session 2026-06-18/19)

Instrumented the Alto-program level (`DORADO_IFUDISP_TRACE` for the Alto PC,
`DORADO_FETCH_TRACE` for read VAs, `DORADO_MACHINE_PCHIST` extended to all
tasks + a field/RTC handler watch, `DORADO_STORE_TRACE_VA` for writes). The
stall is now fully characterized, and several hypotheses were tested and
**ruled out**:

- **Alto PC**: MC's emulated Alto pins in a 5-segment busy loop
  (`br31` 0x5F2/0x366/0x360/0x60C/0x35E) for 1.3+ display fields -- no other
  Alto code ever runs. Galaxian (works) runs 20+ segments continuously
  (busy-drawing), and passes *through* 0x5F2/0x60C without blocking, so those
  are a shared runtime routine MC gets stuck in.
- **Polled words**: the loop reads exactly 9 low-memory words
  (0o576,0o600,0o1153,0o1466,0o1537,0o1637,0o3016,0o3017,0o3020). All are
  **constant across 2.4M cycles (~4 fields)**.
- **Writers**: the 9 words ARE written during init -- boot clear (~24M),
  game-image load (`task=7 pc=0o6642`, ~30M), and further rewrites through
  ~49M+ (incl. a `pc=0o4000` clear that is infrastructure -- identical 2^22
  hits for working Galaxian, so not the bug). The point is that **during the
  stall (150M+) the values are stable** (verified by gated reads at 150.0M /
  151.2M / 152.4M): whatever should change one of them next never does.
  (Correction: an earlier draft said "nothing writes them after boot" -- that
  was a subset-range artifact; they are written through init, then freeze.)
- **RULED OUT - field/RTC**: the AEmu field/RTC handlers run for MC at the
  *same* rate as Galaxian (ENDOFFIELD 96 vs 88, EVENFIELD/THTNEWFIELD ~97 vs
  ~89, RTCCARRY 34 vs 34). The display field and RTC are processed fine.
- **RULED OUT - Alto interrupts**: `INITIATEINT`/`RESCHEDPENDING` run **0**
  times for *every* game incl. Galaxian -- no game uses Alto-level interrupts;
  they busy-poll.
- **RULED OUT - input polling**: MC reads **nothing** in the Alto I/O region
  (0o177000+) during the stall -- it is not waiting on keyboard/mouse.

### Conclusion
MissileCommand has **deadlocked on internal state**: it set up 9 words during
init, then entered a busy-wait for one of them to change, but the code that
would change it never runs (it is itself stuck in the wait). Nothing external
(field, RTC, interrupt, input) is involved. The most likely cause is that
**MC's initialization computed a wait-loop value wrong** -- an Alto-opcode /
AEmu emulation subtlety MC exercises that Galaxian does not -- leaving the
loop's exit condition permanently unsatisfiable. Pool is the same shape
(all-white, frozen); PinBall gameplay is presumably the same after start.

### Next step (a real harness, not a quick patch)
Two ways to find the exact wrong opcode/value:
1. **ContrAlto cross-validation** (the project's stated strategy): run the
   same boot file under ContrAlto2 and diff emulated-Alto architectural state
   until it diverges. Pinpoints the opcode but needs a boot-file load path
   into ContrAlto.
2. **Manual back-trace**: disassemble MC's 5-segment loop, identify the exact
   word + value its exit test wants, then back-trace (with the new traces) how
   that word got its stuck value during MC init -- the opcode that produced
   the wrong value is the bug.

### Cross-validation result (2026-06-19, option 1)
Used the in-tree differential harness `altodiff-dorado` (runs one Alto opcode
on the real AEmu microcode, diffs the architectural result -- AC0-3, carry,
PC, memory writes -- against the spec-derived `altoref.c`; this is the
in-tree equivalent of the ContrAlto opcode diff, and sidesteps ContrAlto's
need for a host boot server to run a `.boot` file). `altodiff-dorado sweep`
runs **20768 vectors with 0 mismatches** across:
- all 8 ALC functions x shift/cycle/no-load/skip/carry,
- STA/LDA/ISZ/DSZ page-zero, single-level indirect, and indexed
  (AC2/AC3-relative, direct + indirect).

**So MC's deadlock is NOT a per-opcode bug in the common Nova/Alto ISA.**
MC's loop reads 0o576..0o3020 (not the auto-increment range 0o20-0o37), so
auto-increment is not implicated either.

Decoded MC's loop opcodes (from the IFU index distribution over the stall):
`op=0o001` (96x, dominant) and `0o003` are **JMP** (PC-rel / AC3-rel);
`0o202`/`0o245`/`0o344` are ALC; `0o041`/`0o045`/`0o055`/`0o071`/`0o131`/
`0o142`/`0o151` are memory-reference. **All are opcodes the sweep validated
or that are demonstrably working** -- the loop is *stable* (same 5 segments
forever), so even its JMP targets compute correctly.

### Refined conclusion
The bug is **not** an Alto-opcode emulation error. MC's loop executes
correctly; it spins because its **data** (the 9 words) holds wrong values
set up earlier during init. Since the opcodes are correct, the wrong data
must come from a **non-opcode source**: a memory mapping/aliasing slip, an
I-O instruction (SIO/RCLK/keyboard) returning a wrong value during init, or a
timing-dependent init path. Galaxian (works) avoids whatever MC's init does.

### Back-trace result (option 2): exit flag = M[0o1637], producer never sets it
Decoded MC's loop: `op=0o202` = ALC `MOV# 0,0` no-load + skip-if-nonzero =
"skip if AC0 != 0", where AC0 is loaded from a polled word. So the loop waits
for a word to become **nonzero**. Two polled words are zero (0o1637, 0o600).

Added `DORADO_POKE="va,value,cycle"` (one-shot VM write, env-gated). Poking
**M[0o1637]=nonzero** at the stall makes MC execute **new code segments**
(br31 0x41D/0x4D5/0x526 that never ran before) -- so **0o1637 is the loop's
exit flag**. (Poking 0o600 does almost nothing.)

Full write history of 0o1637 (to 150M): boot-clear 0 (24M); game-image load
**0o31236** by the boot task (30M); then **every** subsequent write is 0 --
pc=0o4000 (49M, the 64K boot clear), pc=0o1622 (75.9M, br31=3), task-7
pc=0o2424 (77.7M), pc=0o42 (108M, br31=0o1736, the last writer before the
stall). So in our run the flag is only ever nonzero as raw loaded image data;
the **producer that should compute+store a nonzero value never does** -- it
stores 0.

Caveat: a *one-shot* poke does NOT unfreeze the display (MC runs a brief burst
then re-blocks -- 368448 px unchanged). So 0o1637 is one gate in a **chain**;
the real bug is whatever computes the value stored at 0o1637 (and siblings)
as 0 when it should be nonzero. The last writer is pc=0o42 (an Alto STA, the
producer storing AC -> 0o1637); AC is 0 there.

### Forcing test (decisive negative): 0o1637 is NOT the root
Extended `DORADO_POKE` with a hold mode ("va,value,cycle,1" = enforce every
step). **Holding M[0o1637] nonzero from cyc 100M still does not render**
(368448 px frozen at frames 300/400/500). So although poking 0o1637 changes
control flow (new segments run), it is a downstream gate, not the blocker --
the single-flag hypothesis is falsified. MC also never renders content at any
point (blank -> black right after boot), so the failure is early-init.

### Honest status
The MC deadlock is a **multi-factor early-init failure**, not a single
flag/opcode. Back-tracing it further is a full reverse-engineering effort on
a 1980 game binary with **no source**, and forcing individual gates does not
converge. Expected value per additional back-trace level is now low.

### Realistic options (pick by appetite)
1. **Get a reference** running MissileCommand (ContrAlto/salto) to learn what
   it SHOULD do at the divergence point -- but ContrAlto net-boot needs a host
   boot server, and neither trivially loads a `.boot`. Without a reference we
   are guessing "correct" behavior.
2. **Try a different broken game** (e.g. Pool -- distinct failure: 6 DCBs,
   all-white) in case its root is simpler/shared.
3. **Ship as-is**: the WASM build works with Galaxian, Reversi (playable),
   Boggs, Asteroids, Invaders, and Pinball (instructions). MC/Pool/Pinball-
   gameplay are known-hard early-init cases documented here.

This is separate from the Cedar germ bring-up tracked in `CONTINUE-HERE.md`.

## ContrAlto reference cross-validation (2026-06-19)

Built a headless ContrAlto2 harness (`AltoInfo/contralto-headless/`, gitignored)
that disk-boots an Alto pack and dumps the display + memory. Downloaded the
bitsavers Alto disk images (`AltoInfo/bitsavers/`); `allgames.dsk` contains the
Murray games as `.RUN` files and disk-boots cleanly in ContrAlto.

DECISIVE: typing `missile` at the Executive runs **Missile Command 3.0** to a
full attract screen in ContrAlto -- so **MC works on a reference Alto; its
failure is our emulator's bug**, not the game/hardware. Our
`MissileCommand.boot` is the *same build* (same title/strings).

Findings:
- **Layout**: our `.boot` loads MC's program image +0o22400 above ContrAlto's
  `.run` (title strings line up at a constant offset). Address-level diffs are
  only valid for fixed low-memory variables, not the relocated image.
- **The exit flag is disk-written.** Instrumented ContrAlto memory: writes to
  `M[0o1637]` are `0,100002,174575` (Emulator task) then **`1122` by the
  DiskWord task**. MC's busy-wait flag is set by the **disk task** -- MC does a
  disk op and waits for completion. Our emulator (Ethernet boot, incomplete
  disk read path) never completes it, so 0o1637 stays 0 and MC hangs. Reframes
  MC from a CPU/AEmu bug (0 per-opcode mismatches) to a **disk dependency**;
  likely explains the whole "boots but never renders" class.
- CAVEAT: ContrAlto runs `missile.run` loaded from disk by the OS, so that
  DiskWord write could be the OS file-load, and boot/run layouts differ. To
  confirm: run our exact `.boot` in ContrAlto (needs a boot-server harness) or
  finish our disk-read path and watch 0o1637 reach a nonzero completion.

### RETRACTION: the disk-dependency conclusion was wrong
A read-watch on ContrAlto refutes it. ContrAlto's MC **Emulator task reads
`0o1637` only 23x with the game vs 23x at idle** (OS baseline) -- it does NOT
poll it; the *disk task* does (4586 -> 8500 reads). So in the run version
`0o1637` is a disk-task variable, while in our boot version MC polls `0o1637`
heavily as its own flag. Because the boot image is relocated +0o22400 and the
OS occupies low memory differently, **`0o1637` is a different variable in the
two builds** -- the "1122 written by DiskWord" finding does not apply to our
boot MC. Disk dependency is NOT established.

### Methodological wall + the only valid path
ContrAlto running `missile.run` is a sound *behavioral* reference (proves MC
works -> our bug) but is **not address-comparable** to our `MissileCommand.boot`
(different layout; confirmed by the read-watch). A valid architectural diff
needs the **same binary in both**:
1. Run our exact `.boot` in ContrAlto -- needs a small Alto-3Mb boot-server
   over ContrAlto's UDP encapsulation (our `ethernet.c` already implements the
   boot protocol; port it). Then 0o1637 et al. map 1:1.
2. OR disassemble `MissileCommand.boot` directly (same binary, no mapping
   issue): decode the wait loop + the producer (Alto STA near br31=0o1736 /
   the cyc-108M write) to see why the flag is computed 0. A small Nova
   disassembler suffices.

Either gives a clean root cause; the run-version reference alone cannot.

## ContrAlto boot server -> valid 1:1 cross-validation (2026-06-19)

Built an in-process Alto-3Mbit netboot server INTO ContrAlto
(`AltoInfo/Contralto2-2.0-Beta/ContraltoLib/IO/BootServer.cs`, an
IPacketEncapsulation, env CA_BOOTSERVER/CA_NETBOOT/CA_BOOTSILENT). It
broadcasts a standard breath-of-life (ether 0602) carrying the EtherBoot
loader, answers the Alto's Mayday (Pup 0244) and streams the file via EFTP
(Data 030/End 032, acked) -- mirrors `dorado/src/ethernet.c`. Result: **our
EXACT `MissileCommand.boot` net-boots in ContrAlto and runs its full attract
screen.** Same binary in both emulators => addresses correspond 1:1.

Findings from the valid comparison (ContrAlto running OUR boot vs our stalled
emulator):
- **0o1637 was a red herring** -- it is 0 in the WORKING version too. All the
  earlier 0o1637 analysis (poke, producer back-trace) was chasing a non-gate.
- 6 of 9 polled words MATCH. The genuine differences are three boot-state
  words: **0o600 (good 2777 / ours 0), 0o576 (13207 / 31), 0o3016 (2616 /
  1537)**.
- 0o600 is written by the **Ethernet task** during boot; it is set once to
  2777 and persists (MC reaches the attract even with the boot server fully
  silent afterward -- so NOT an ongoing-network dependency).
- Poking 0o600=2777 in our emulator CHANGES MC's behavior (frozen-black ->
  blank), confirming it is causal, but the three words are set together so a
  single poke doesn't reach the attract.

### Root (narrowed, valid)
The divergence is **boot-environment state**: our boot path (Dorado Initial ->
AEmu world -> NetExec -> EFTP) leaves MC's low-memory ethernet/boot words
(0o600/0o576/0o3016) different from a real Alto boot-ROM netboot (BOL ->
Mayday -> EFTP). MC reads those during init and diverges into the wait loop.

### ROOT CAUSE (found): EPLOC ethernet-completion posting
`0o600` is **EPLOC** (Ethernet Post Location; `0o604`=EICLOC, Alto HW Manual
Sec 7). MC posts an ethernet input op and polls EPLOC for the completion
status. Comparing the writes to EPLOC (same binary):
- **ContrAlto (working):** the Ethernet microcode posts `377`, `777`, then the
  final completion **`2777`** (Ethernet task). MC reads it and proceeds; EPLOC
  ends at 2777.
- **Ours:** our ethernet posts only the intermediate `377` (task 7) and `777`
  (task 6) and **never the final `2777` completion**. MC's emulator task
  (pc=0o42) therefore keeps **clearing EPLOC to 0 (204x)** and retrying, so it
  ends at 0 and MC busy-waits forever.

So MissileCommand hangs because **our emulator's ethernet never posts the
final input-completion status to EPLOC** that a real Alto's ethernet
microcode does. (The 0o576/0o3016 differences are downstream of this -- MC's
ethernet-status bookkeeping.) This is consistent with the existing EPLOC
caveats in machine.c (~1126) and ethernet.c (~626, "EtherBoot.asm's poll loop
re-arms ePLoc").

### Fix direction
Make our ethernet path post the proper EPLOC input-completion (the `2777`
status the Alto ethernet microcode posts after the boot input completes),
rather than leaving MC to re-arm/poll forever. Likely in `src/ethernet.c` /
the AEmu input-completion handling.

CONFIRMED the fix is at the ethernet-EVENT level, not memory: holding all
three words (0o600=2777, 0o576=13207, 0o3016=2616) at ContrAlto's values from
cyc 100M does NOT reach the attract (MC goes blank, not its menu). So MC
depends on the actual ethernet input-completion *flow/state*, not just the
status words -- forcing the words pushes MC past the EPLOC poll but it then
lacks the real completion and diverges. The fix must make the ethernet
operation genuinely complete (post InDone -> EPLOC the way a real Alto does),
not patch memory.

Note the boot-path asymmetry: ContrAlto boots MC via the standard EtherBoot
loader (our BootServer's breath-of-life), which leaves EPLOC=2777; our
emulator boots via Dorado Initial -> AEmu -> NetExec, which leaves a different
EPLOC state. MC (same binary) expects the EtherBoot completion convention.

### Traced to the microcode routine (2026-06-19)
The EPLOC post is done by the AEmu microcode routine **EPOST** (real PC
`0o3460`; also EIPOST `0o2435`, EOPOST `0o2535` -- from `mbdis --disasm
AEmu.mb`). EPOST reads a sequence of Dorado ethernet FF-functions (`163`,
`360`, `036`, `004`, `304`, `077`) to compute the status word it posts to
EPLOC. Our EPOST posts the intermediate `377`/`777` but **never the terminal
`2777`** (which adds bit `0x400`, and bits 1-2 = the IOCMD-cleared 0o6 that
ContrAlto's status register sets). So one of those FF-functions returns the
wrong ethernet status/completion in our emulator (`cpu.c`/`io.c`/`ethernet.c`
handling of the Dorado ethernet status functions EPOST reads).

REFUTED fix attempts (do not re-try): poking the 3 words (memory, not event);
disabling post-boot broadcasts (makes MC blank -- MC NEEDS the intermediate
completions to reach black-fill).

### Remaining work (precise, but deep)
Trace EPOST's value computation against the live ethernet state to find which
FF-function/status read diverges (377/777 vs 2777), then correct that handling
in ethernet.c so the terminal completion posts. Success test: EPLOC settles at
2777, MC reaches its attract, Galaxian (121553) / NetExec unchanged. A wrong
change here risks the regression gate, so it needs the EPOST microcode trace
first, not a guess.

### ROOT CAUSE (definitive, from the AEmu source)
`chm/doradosource/AEmuSources-cedar6.0.dm!1_/EtherDefs.mc` defines the EPLOC
post codes (high byte): **InDone=377, OutDone=777, InBufOverflow=1377,
CountZero=2377, CmdAbort=2777**. So 2777 = "Command aborted by SIO".

`AltoEtherEmu.mc` ESIO (the SIO emulator): function bits `11` = "Reset
interface" -> `SIOReset: T_ ECmdBits; T_ T XOR (CmdAbort); Call[EPost]`.
ECmdBits = EInCmd|EOutCmd = 6, so **AEmu posts ECmdBits XOR CmdAbort = 6 XOR
2777 = 2771**. The real Alto (ContrAlto) posts the clean **2777**.

MissileCommand's init does an ethernet receive that should time out, then
SIO-resets and polls EPLOC for the CmdAbort post. **MC requires the clean
2777; AEmu's 2771 (with the ECmdBits status bits set) is rejected, so MC
re-arms and busy-waits forever.** Confirmed by the time-ordered EPLOC trace:
MC's clears (pc=0o42/0o53) interleave with and wipe the posts, ending at 0.

Two compounding factors, both AEmu/real-Alto emulation gaps:
1. **CmdAbort value**: AEmu posts 2771, real Alto 2777 (the ECmdBits-XOR).
2. **The re-arm race** (ethernet.c ~626): MC's EPLOC clear races/wipes the
   InDone/CmdAbort post, so EPLOC ends 0.

Forcing 2771->2777 at EPLOC advances MC past the poll but it then goes blank
-- MC also needs the ethernet to be ACTUALLY reset by the SIO, not just the
post value patched. So the fix is faithful emulation of the Alto ethernet
SIO-reset/abort (reset the receiver + post clean CmdAbort + correct timing),
not a value rewrite.

### Why it's hard / risky
The 2771-vs-2777 difference is in AEmu.mb microcode (a fixed Xerox constant,
`T_ ECmdBits`), and the receiver-reset + race timing live in the AEmu<->Dorado
ethernet path that Galaxian/NetExec also use. A correct fix means matching the
real Alto's ethernet SIO-reset semantics without regressing them -- either a
targeted AEmu microcode patch (SIOReset) or a faithful ethernet-reset emulation
in ethernet.c. Both are deep; not a value rewrite.
