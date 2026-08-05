# dorado/ — the C emulator

This directory holds the C-language Dorado emulator. See the parent
`CLAUDE.md` for the project mission. This file captures what we have
*built* so far, plus the format-level findings that aren't in the
Hardware Manual but were dug out of the MicroD BCPL source.

## Build

```
make           # builds the emulator (build/dorado), tools, and test binaries
make test      # runs the integration tests (11 binaries)
make sdl       # windowed SDL frontend (build/dorado-sdl)
make web       # WebAssembly frontend (needs emcc); make run-cedar boots Cedar
make clean
```

The core emulator is pure C99, no external dependencies (SDL2 only for
`make sdl`; Emscripten only for `make web`). clangd users: `compile_commands.json`
in this directory keeps the language server happy.

## Layout

```
dorado/
├── Makefile
├── compile_commands.json
├── CLAUDE.md             ← this file
├── include/
│   ├── mb.h              .MB loader API
│   ├── disasm.h          microinstruction decoder
│   ├── microcode.h       placement / IM array
│   ├── cpu.h             microengine
│   ├── memory.h          cache/Map/Pipe/BR
│   ├── baseboard.h       6502 BB model
│   ├── io.h              slow-I/O device routing (HM §7)
│   ├── display.h         display + DDC stub (HM §11)
│   └── disk.h            Trident T-80/T-300 + DSK controller (HM §9)
├── src/
│   ├── mb.c              .MB parser
│   ├── disasm.c          model-1 unshuffle + microinstruction field decoder
│   ├── microcode.c       image→real placement, builds hardware-ready IM[4096]
│   ├── cpu.c             microengine (tasking + IFU + memory)
│   ├── memory.c          memory subsystem
│   ├── baseboard.c       BB 6502 + RIOTs + analog comparators
│   ├── io.c              slow-I/O routing (per-(task,TIOA) device table)
│   ├── display.c         framebuffer + DDC slow-IO catch-all
│   ├── disk.c            disk pack + drive + DSK controller (slow-IO at TIOA 10₈-14₈)
│   ├── typetext.c        canonical ASCII→Alto key map + paced typing/paste queue
│   └── mbdis.c           CLI: dump / symbolic disasm
└── tests/
    ├── test_mb.c         loader
    ├── test_disasm.c     decoder
    ├── test_microcode.c  placement + ALUFM canonical decoding
    ├── test_memory.c     memory subsystem
    ├── test_baseboard.c  BB cold boot + CPReg + MCPBus
    ├── test_display.c    framebuffer + IO routing + PGM snapshot
    ├── test_disk.c       pack create/save/load + controller TIOA routing
    └── test_cpu.c        CPU smoke tests + slow-I/O routing + ALUFM/carry tests + Bootstrap/AEmu probes
```

## What's done

### `mb` — `.MB` loader (src/mb.c, include/mb.h)

Reads a MicroD-output `.MB` file into structured form: per-memory data
arrays, symbol table, external-fixup table. Tested against every Dorado
microcode file in `chm/dorado/` (Mesa, Cedar, AEmu, DSemu, IfuComplex,
kernel, DoradoLisp, Bootstrap, Initial, ftest). All load cleanly.

The loader does *not* unshuffle the model-1 storage into logical
iw0/iw1/iw2 — it preserves the on-disk layout. Field decoding is the
disassembler's job.

### `disasm` — microinstruction decoder (src/disasm.c, include/disasm.h)

`dorado_decode_model1(stored[4], &uinstr)` undoes MicroD's storage
shuffle and extracts all eight microinstruction fields:
`RSTK`, `ALUF`, `BSEL`, `LC`, `ASEL`, `BLOCK`, `FF`, `JCN`. Verified
on every Dorado microcode binary in `chm/dorado/` (Mesa, Cedar, AEmu,
DSemu, IfuComplex, kernel, DoradoLisp, Bootstrap, Initial, ftest —
~25,000 microinstructions total, all decode without field-width
violations).

`dorado_format` produces a one-line symbolic rendering. Field names
(`Md`, `RM/STK`, `T←Pd`, `A←Id`, `local`/`global`/`long`/`fast`, etc.)
come from HM Tables 7, 8b, 10, and the `mdfields.d` JCN encoding tags.

### `mbdis` — dumper / disassembler CLI (src/mbdis.c)

```
mbdis [--summary] [--memory NAME] [--disasm] file.MB
```

Without flags: octal listing of every memory's contents, symbol-
annotated. With `--disasm`: each IM entry also gets a one-line
symbolic decode (RSTK/ALUF/BSEL/LC/ASEL/FF/JCN/BLOCK).

### `mb2eb` — `.MB` → Ethernet-boot `.eb` converter (src/mb2eb.c)

```
mb2eb in.mb out.eb [start_addr_octal=1076]
```

Converts a MicroD `.MB` into an `.eb` image that the *real* Initial
microprogram loads over Ethernet via `LoadRam` — the correct way to bring
up a standalone emulator (e.g. `AEmu.mb`) with full Initial setup
(MDS/BRs/map/RTClock) instead of a boot-bypass. Emits the `LoadRam.mc!1`
Item array (IM `word0=iw0`, `word1=FF,,JCN`, extraIM `LHpar,,RSTK0,,RHpar,,
BLOCK`; IFUM `word0/1=ifum_lo/hi`; RM `word0=value`), a 256-word header
(`word0=1`), and a balancing End item whose checksum makes the 16-bit sum
of all payload words zero (per Initial's `CheckChecksumAndLoad`). Serve the
result with `DORADO_ETH_BOOT_110=out.eb` in the full-boot probe; Initial
passes the checksum, `LoadRam`s it, and jumps to `InitMap` (`0o1076`).

### `bbdis` — BaseBoard 6502 ROM disassembler (src/bbdis.c)

Standalone NMOS-6502 disassembler that loads a `.MB` ROM image
(typically `chm/dorado/doradobaserom.mb!13`) into a 64K byte array
and emits annotated assembly for a chosen address range.

```
bbdis [--start HEX] [--end HEX] [--labels FILE] [--vectors] PATH
```

`--vectors` dumps NMI/RESET/IRQ targets and exits. `--labels FILE`
reads a tab-separated `HEX<tab>name` symbol table to annotate calls
and branches. Output is suitable for diffing against the BB source
in `chm/dorado/expanded/doradobaserom.dm!12_/*.masm`.

Pre-rendered dumps live at `chm/disassembly/bb_C000-D7FF.s` (Boot0/
Boot1 data region) and `chm/disassembly/bb_F000-FFFF.s` (BB program
code). These were generated for gap A1 research; see
`docs/research/A1-bootstrap-streaming.md`.

The raw-octal output matches ftest.DLS's stored-word presentation;
the symbolic line is our addition. To match ftest.DLS's W0/W1 column
exactly (which uses a re-shuffled display form combining iw0/iw1/iw2
bits), reconstruct as:
- `displayed_W0 = orig_iw0`
- `displayed_W1 = (orig_iw1 << 1) | JN1bit7`  (where `JN1bit7 = (iw2 >> 14) & 1`)

### `microcode` — image→real placement (src/microcode.c, include/microcode.h)

`dorado_microcode_load(mb_file *, dorado_microcode *)` walks the loaded
.MB and produces hardware-ready arrays:
- `im[4096]` — `dorado_uinstr` indexed by **real** address (post-MicroD placement)
- `im_present[]`, `im_emulator[]`, `im_breakpoint[]` — per-slot flags
- `image_to_real[4096]` — the permutation MicroD computed
- `ifum[1024]`, `alufm[16]`, `rm[256]` — populated from `.MB` data

The real placement comes from each IM data record's storage word 3
(awd): bits 0..11 = `IM.W0` (the 12-bit assigned address), bit 12 =
emulator-task flag, bits 14..15 = breakpoint pair.

`dorado_microcode_symbol_at_real()` resolves a real IM address back to
its source-level label (going via `image_to_real`).

### `cpu` — Dorado microengine (src/cpu.c, include/cpu.h)

Single-task interpreter built around `dorado_cpu_step()`. Each step:

1. Fetch `dorado_uinstr` from `mc->im[real_PC]`.
2. Compute the A-bus from ASEL (RM/STK, T; memory & shifter still TBD).
3. Compute the B-bus from BSEL (Md TBD; primary sources + 4 constant
   forms 0,,FF / 0o377,,FF / FF,,0 / FF,,0o377 work).
4. Run ALUFM[ALUF] on (A, B). The 6-bit ALUFM entry is split into
   carry-in (bit 5) + 5-bit op (Table 9). Logical and arithmetic
   variants implemented; updates ALU=0 / ALU<0 / Carry / Overflow
   flags.
5. Route Pd (= ALU output, no shifter routing yet) via LC (Table 10).
6. Compute next PC via JCN per HM Figure 6 / `docs/jcn-encoding.md`:
   - **Local Jump/Call** ✓
   - **Global Call** ✓ (always a Call — low 6 bits forced 0)
   - **Long Jump/Call** ✓ (FF supplies 8 of 12 address bits)
   - **Conditional Jump/Call** ✓ — Table 13 conditions 0..6 supported
     (ALU=0, ALU<0, Carry', Cnt=0&-1, R<0, R odd, IOAtten' stub).
     Calls when JCN[3:4]=00.
   - **Subroutine Return** ✓ (TNIA = Link[2:15]; Link reloads CIA+1)
   - **Read/Write IM, Read/Write TPC, IFU Jump** — halt (`UNSUPPORTED_JCN`)

Calls (long, local, global, conditional) load `Link ← CIA + 1` when
the unmodified target is `0 mod 16` (HM §4.5).

Halt reasons surface what real microcode hits — every unimplemented
combination raises a specific `cpu_halt_reason` rather than silently
falling through, so `test_cpu`'s `probe_bootstrap` reports exactly
which microinstruction we couldn't execute.

**Current real-microcode reach:** The full BaseBoard ↔ Dorado
boot handshake works end-to-end up through Boot0 starting to free-
run from IM. probe_full_boot in `tests/test_cpu.c` runs the BB and
the Dorado microengine tick-by-tick from cycle 0 with empty IM
(matching real hardware power-up):

  1. BB cold-boots (Reset → WaitForInitialBoot, ~5 M cycles).
  2. 3 boot-button presses → CoolBoot dispatch → RebootDorado.
  3. RebootDorado walks PowerUp → LoadDoradoCode at 11.4 M cycles.
  4. LoadDoradoCode jams 475 microinstructions into the Dorado via
     DoDoradoMicroInst (MIR strobes + SetSS); the IRTable's
     IMLH/IMRH variants deposit Boot0 into IM[0o7700..0o7777]
     using Write IM. 64 IM entries are written.
  5. BB sets Link=Boot0GoLoc (0o7740) via CPRegToLink#, then jams
     `Return` without single-step → Dorado free-runs Boot0 from IM.
  6. Dorado executes ~10 Boot0 microinstructions (PC walk:
     0o7740 → 0o7761 → 0o7746 → 0o7707 → 0o7744). At 0o7744 it
     hits an all-zero IM entry (a real Boot0 trap-reservation
     slot) and the embedded long-jump goes to IM[0o4000], which
     is empty — halt.

**Major correctness fixes (2026-04):**

- **`src/microcode.c` ALUFM extraction** now recovers the carry_in bit
  from storage word bit 15 (was masked off, conflating
  ALUFM[2]=A+B-c0 with ALUFM[3]=A+B-c1). Locked in via
  `test_alufm_canonical_decoding` — verifies Bootstrap.MB and
  AEmu.mb standard convention is recovered.
- **`src/cpu.c alu_op`** returns `is_arith` so the caller can preserve
  Carry'/Overflow on logical ops (HM page 30: "the result of the last
  *arithmetic* ALU operation"). Locked in via
  `test_carry_preserved_on_logical`.
- **`include/cpu.h CPU_QUADRANT_SIZE`** was 0o4000 (2K) — should be
  0o10000 (4K) per HM §4.3 (one full IM = one quadrant). Was masking
  off real address bits, sending Global Calls / Long jumps to
  out-of-range targets. **Side effect: probe_aemu now executes real
  AEmu microcode** (STARTEMULATOR → SETUPBRS → DOBRS×12 → IFU
  dispatch loop) instead of halting at fictional addresses.
- **`src/cpu.c B←RWCPReg`** legacy stub now matches the BaseBoard
  path: returns `~cpu->cpreg` and does NOT increment per call. Lets
  synthetic tests hold CPReg constant for AMSync polling.
- **`src/cpu.c Pd←ALUFMRW`** B-to-ALUFM bit mapping was reversed.
  HM Table 11d: `ALUFMEM ← B.8, B[11:15]` maps manual `B[8]=B_C[7]`
  to entry top (carry), `B[11:15]=B_C[4..0]` to entry op[4..0]. Our
  code had this mirrored. Locked in via `test_alufmrw_bit_mapping`.

**probe_bootstrap_pure** now reaches the genuine loader behavior:
- Walks BOOTSTRAP (0o7740) through 16 instructions of ALUFM
  initialization (0o7771 writes ALUFM[8]=A XOR B, 0o7773 writes
  ALUFM[10], 0o7776 writes ALUFM[14], etc. — Bootstrap.MB self-
  initializes ALUFM at runtime even though .MB declares it).
- Reaches READBB at 0o7700, then enters the spin loop
  (0o7747 → 0o7742 → 0o7741) waiting for a CPReg byte from the BB.
- With `cpu.cpreg=0x8000` (mimicking BB's `SetCPReg(0x80,0)` AMSync
  preset), the loop exits on first iteration and proceeds through
  0o7746 → 0o7715 → 0o7702 → 0o7700 (re-enter loader). Re-enters
  the loop because the test environment doesn't drive CPReg with
  fresh bytes — needs a BB feeding the byte stream to make further
  progress.

**probe_full_boot** still hits a trap at 0o7744 because the BB ROM
contains a NEWER Boot0 binary that takes a different path than
Bootstrap.MB. The newer binary doesn't appear to do runtime ALUFM
init the same way; it lands in reserved trap slots given our
zero-init RM/T. This is now isolated from the rest of the model —
the microengine is correct; the BB-loaded binary has different
semantics we'd need to disassemble piece-by-piece to follow.

Beyond this would need either a much fuller hardware model
(memory subsystem, proper STK push/pop, ALUFM init from Midas)
or per-microinstruction analysis of Boot0's intended flow.

Shifter coverage in `shifter_output()`:
- 32-bit barrel cycle of `(SHA||SHB)` left by ShC[4:7] (or FF[4:7] when
  BSEL[0]=1).
- ALUF[0:2] mask op: ShiftNoMask, ShiftLMask, ShiftRMask, ShiftBothMasks.
  ShMd* variants stubbed with Md=0.
- A bus = ~shifter_output (low-true); ALUFM index forced to 14+ALUF[3]
  during shift; ALUFM[14]="NOT A" by convention completes the inversion.
- BSEL[0]=1 with ASEL=7: B is forced to be Q.
- FF-controlled mode supplies count (FF[4:7]), RMask (FF[4:7]), and
  LMask (FF[0:3]).

FF dispatcher (`ff_override_b()` + `ff_apply_post()`):
- Decodes FA = FF[0:1], FB = FF[2:4], FC = FF[5:7] (HM §3.9).
- "FF interpreted as a function iff (BSEL not constant) and
  (JCN not long)" — gated by `ff_is_function`.
- B-source overrides (FA=1, FB=6 or 7):
  - **`B←FaultInfo'`** (FB=6 FC=0) — wired to `dorado_fault_info()`
    inverted; reports NFaults + first-fault SRN. Same data as
    `B←Pipe2'`.
  - **`B←Pipe0`/`B←Pipe1`** (FB=6 FC=1/2) — VaHi/VaLo of the pipe
    entry at slot `ProcSRN` (HM page 51: microcode loads `ProcSRN←B`
    first, then reads).
  - **`B←Pipe3'`** (FB=6 FC=4) — inverted snapshot of pre-ref map
    flags (WP/Dirty/Ref) at slot `ProcSRN`.
  - **`B←Link`** (FB=7 FC=7) — the per-task Link register.
  - **`B←RWCPReg`** (FB=7 FC=6) — Link←B, B←CPReg' from the BB.
  - **`B←Pipe4'`** (FB=6 FC=5) — inverted per-slot error state.
  - **`B←Config'`** (FB=6 FC=6) — storage configuration word
    (`dorado_memory_config_word`, per EMemDefs.mc: ASRN, ChipSize,
    module-present mask).
  - **`B←Pipe5'`** (FB=6 FC=7) — cache flags, Victim/NextVictim and
    **MapBufBusy in the sign bit**, which is what InitMem.mc's
    `WaitForMapBuf` polls (`PD_ PRef, Branch[., ALU<0]`).
  - EventCnt / IFUMRH / DBuf stubs return 0.
- B-sink post-effects (FA=1 FB=2 FC=…):
  - **`ProcSRN←B[12:15]`** (FC=7) — sets the pipe-slot index for
    subsequent `B←Pipei` reads.
  - **`BrLo←A`** (FC=3), **`BrHi←A`** (FC=4) — load the BR.
- Tasking + IFU FF functions (Phase D + C.1/C.2):
  - **`TaskingOff`** (FA=1 FB=4 FC=2), **`TaskingOn`** (FC=3) —
    gate the task scheduler; TaskingOn delays 2 cycles.
  - **`Wakeup[task]`** (FA=3 FB=6-7, task=FF[4:7]) — assert wakeup.
  - **`InsSetorEvent←B`** (FA=1 FB=3 FC=0) — B[0]=1 loads
    IFU's InsSet from B[6:7].
  - **`BrkIns←B`** (FA=1 FB=3 FC=7) — load IFU's Opcode from B[0:7].
  - **`IFUMRH/LH←B`** (FA=1 FB=3 FC=4/5) — write
    `ifum_lo`/`ifum_hi`[InsSet||Opcode] from B.
  - **`B←IFUMRH'/LH'`** (FA=1 FB=7 FC=2/3) — read inverted IFUM
    halves from the same address.
  - **`IFUReset`** (FA=1 FB=3 FC=6) — clear IFU addressing regs.
  - **`PCF←B`** (FA=1 FB=0 FC=0) — load byte cursor + arm IFU.
  - **`A←Id`** (ASEL=5), **`TIsId`/`RIsId`** (FA=0 FB=3 FC=4/5) —
    consume next operand byte (N, α, β, then Length forever).
- IFU JCN dispatch:
  - **`IFUJump[n]`** (JCN `0 0 1 _ _ 1 1 1`, n=JCN[3:4]) — read
    opcode at PCF, look up IFUM, dispatch to entry vector slot n
    (TNIA = (IFaddr' << 2) | n). Sets MemBase/RBase per the
    IFUM entry's MemB/RBaseB' fields.
- HM Table 7 asterisk: when an external B source is in play and
  BSEL=3, the external value also lands in **Q**. Critical for
  Bootstrap's `Q ← Link` snapshot trick.
- Post-ALU side effects implemented: `Q←B`, `MemBase←B[3:7]`,
  `RBase←B[12:15]`, `Pointers←B`, `StkP←B[8:15]`, `Cnt←B`, `Link←B`,
  `Q lsh 1` / `Q rsh 1`, `TIOA←B[0:7]`, `ShC←B`, `RBase←FF[4:7]`
  (FA=2 alt encoding), `Cnt←small constant` (FA=3 FB=4-5),
  `MemBase←FF[3:7]` (FA=3 FB=0-3), `FlipMemBase`,
  **`Pd←ALUFMRW`** (read+write ALUFM[ALUF] from B.8 || B[11:15]).
- Tasking ops, IFU ops, memory side-effects (BrLo/BrHi, CFlags,
  LoadMcr, ReadMap, …) silently honored as no-ops.

### Tests

`make test` runs 11 binaries: `test_pdi`, `test_mb`, `test_disasm`,
`test_microcode`, `test_memory`, `test_display`, `test_disk`, `test_ethernet`,
`test_fastio`, `test_cpu`, `test_baseboard`. Highlights:
- `test_mb`        — loader against ftest.MB + every microcode in chm/dorado/
- `test_disasm`    — decoder + field-width checks across ~25k µinstrs
- `test_microcode` — placement (image→real) verified against ftest.DLS
- `test_cpu`       — synthetic micrograms (T←const, increment loop, RM
                     round-trip, unsupported-halt) + Bootstrap/AEmu probes
- `test_pdi`       — Pilot/Cedar PARC Disk Image loader/inspector

## Format findings (not in the Hardware Manual)

These came out of reading `chm/microd/mdfields.d`,
`chm/microd/mddump.bcpl`, `chm/microd/printmb.bcpl`, and empirical
probing.

### `.MB` file structure

A flat sequence of typed blocks. Each block is a sequence of 16-bit
**big-endian** words. Type codes (1 word at the start of each block):

| Code | Block       | Body                                                        |
|-----:|-------------|-------------------------------------------------------------|
| 0    | `MB_END`    | (none)                                                      |
| 1    | `MB_DATA`   | `sourceline, data[width_words]` — append at current address |
| 2    | `MB_ADDRESS`| `memory, addr`                                              |
| 3    | `MB_FIXUP`  | `memory, addr, (firstbit<<8 \| lastbit), value`             |
| 4    | `MB_MEMORY` | `memory, width_in_bits, NUL_terminated_string`              |
| 5    | `MB_SYMBOL` | `memory, addr, NUL_terminated_string`                       |
| 6    | `MB_EXT_FIXUP`| `memory, addr, bits, NUL_terminated_string`               |

Several **non-obvious points** that bit us during implementation:

1. **Strings on disk are NUL-terminated, no length prefix.** The BCPL
   "first byte = length" convention applies only to *in-memory*
   strings; the on-disk form is just packed bytes terminated by a
   zero. `printmb.bcpl`'s `readsymbol` *computes* the length while
   reading and writes it to the in-memory buffer's first byte.
   - Bytes are packed two-per-16-bit-word, big-endian.
   - Always consume entire words so the stream stays word-aligned.

2. **Memory IDs are file-local.** They are assigned by MicroD when it
   emits the `.MB` and declared via `MB_MEMORY` blocks. We discover
   each memory's role (IM / RM / IFUM / ALUFM / STK / BR / …) by name,
   not by ID. Every Dorado file we've seen uses IDs 1–9 in this order:
   `IM, RM, IFUM, ALUFM, STK, BR, BRX, DEVICE, TASKN`.

3. **`width_bits` is *storage* width, not data width.** For Dorado
   model-1 IM, MB_MEMORY declares `width=64` even though the logical
   microinstruction is only 36 bits (34 + 2 parity). Storage uses
   4 × 16-bit words per IM entry; the data is bit-shuffled across
   those words. See "model-1 bit shuffle" below.

4. **`MB_DATA` always follows an `MB_ADDRESS`.** The address
   post-increments. MicroD's writer (`mddump.bcpl Dump()`) emits one
   `MB_ADDRESS` at the start of each memory's data and then a stream
   of `MB_DATA` blocks.

5. **Instructions are stored in IMAGE (declaration) order, not REAL
   (placed) order.** The address that goes into the `.MB` file is the
   image index — i.e., the order the source file declared the
   instruction. The REAL placed address only appears *inside* encoded
   branch targets within iw1/iw2, and inside the listing-only
   ftest.DLS file (under the "Real" column).
   - Symbols (`MB_SYMBOL`) are bound to image addresses too.
   - The `.DLS` listing is sorted by image (rows are image-ordered);
     the first column is image, the second is real.
   - This means the loader's `IM[image]` array is not yet ready to
     execute — we still need to consume MicroD's placement decisions
     by reordering / fixing branch targets. **TBD.**

### Microinstruction bit layout

From `mdfields.d` (BCPL "bit X" = X-th bit from MSB):

```
iw0   (16 bits, all in C-LSB-first):
  bits 15..13   RSTK[2..0]   (low 3 bits; high bit lives in iw2)
  bits 12..9    ALUF[3..0]
  bits 8..6     BSEL[2..0]
  bits 5..3     LC[2..0]
  bits 2..0     ASEL[2..0]

iw1   (16 bits):
  bit  15       BLOCK
  bits 14..7    FF[7..0]
  bits 6..0     JCN[6..0]    (low 7 bits; bit 7 lives in iw2)

iw2   (16 bits, model-1 only):
  bit  15       RSTK1top     (high bit of RSTK)
  bit  14       JN1bit7      (low bit of JCN)
  bits 13..12   brkP1[1..0]  (breakpoint / bad-parity)
  bits 11..0    blank
```

**Full reconstruction:**

```c
RSTK  = ((iw2 >> 15) & 1) << 3 | ((iw0 >> 13) & 7);   /* 4 bits */
ALUF  = (iw0 >> 9) & 0xF;                              /* 4 bits */
BSEL  = (iw0 >> 6) & 7;                                /* 3 bits */
LC    = (iw0 >> 3) & 7;                                /* 3 bits */
ASEL  = iw0 & 7;                                       /* 3 bits */
BLOCK = (iw1 >> 15) & 1;                               /* 1 bit  */
FF    = (iw1 >> 7) & 0xFF;                             /* 8 bits */
JCN   = (iw1 & 0x7F) << 1 | ((iw2 >> 14) & 1);         /* 8 bits */
```

Total: 4+4+3+3+3+1+8+8 = **34 bits** — matches Hardware Manual Table 1.

### Model-1 storage bit-shuffle

`mddump.bcpl` (`DMachine eq 2` branch) shuffles iw0/iw1/iw2 across
storage word boundaries before writing. The on-disk layout for one IM
entry is **4 × 16-bit words**:

```
stored[0] = (orig_iw2 & 0x8000) | (orig_iw0 >> 1)
stored[1] = ((orig_iw0 & 1) << 15) | (orig_iw1 >> 1)
stored[2] = ((orig_iw1 & 1) << 15) | (orig_iw2 & 0x7FFF)
stored[3] = awd        /* control: brkP, emulator, etc. */
```

Reverse to recover the logical words (do this in the disassembler):

```c
uint16_t iw0 = ((stored[0] << 1) & 0xFFFE) | ((stored[1] >> 15) & 1);
uint16_t iw1 = ((stored[1] << 1) & 0xFFFE) | ((stored[2] >> 15) & 1);
uint16_t iw2 = (stored[0] & 0x8000)         | (stored[2] & 0x7FFF);
```

For Dorado **model 0** (DMachine == 1) the storage is 3 words and no
shuffle is performed; for **D0** (DMachine == 0) it's also 3 words.
Every microcode file we have so far is model 1; we have not yet
written or tested the model-0 / D0 paths.

### IFUM bit layout

From `mdfields.d`'s `TIFUM` struct, IFUM entries are 2 × 16-bit words
on disk (width 32 in MB_MEMORY) but only ~24 logical bits + 3 parity:

```
iw0:  PA bit 6  (only low bit used) | notIFADr2 bit 10
iw1:  Sign bit 1 | IPar bit 3 | notLength bit 2 | notRBaseB bit 1 |
      MemB bit 3 | notTPause bit 1 | notTJump bit 1 | N bit 4
```

We will revisit when implementing the IFU.

## Conventions for working in this directory

- **C99**, host-portable, no external libs. Use `<stdint.h>` types
  (`uint16_t`, `uint32_t`) for hardware values.
- **Octal where the manual uses octal.** Microinstruction addresses,
  IM contents, RM register values — all octal.
- **Symbol names** in code mirror the manual: `RSTK`, `ALUF`, `BSEL`,
  `LC`, `ASEL`, `FF`, `JCN`, `BLOCK`, `IM`, `IFUM`, `ALUFM`, `RM`,
  `STK`, `BR`, `MemBase`, `TPC`, `Pd`, `Md`, `Mar`. Don't rename.
- **Cite manual section numbers** in non-obvious code (e.g.
  `// HM §3.7 ALUF Table 9`). Cite MicroD source when the code touches
  the .MB format.
- **No backwards-compat code.** The .MB format hasn't changed since
  1981; we don't need to handle hypothetical variants.
- **Tests live in `tests/`** and consume real microcode from
  `../chm/dorado/`. They are integration tests by design.
- **Don't conflate image vs real addresses.** Always say which one in
  comments and field names.

## What's next

The Bootstrap-era foundation is all done: shifter, Write IM, real
BaseBoard 6502 + CPReg, the LoadDoradoCode handshake, FF function table,
memory subsystem, IFU, and tasking are implemented and test-covered. The
full BaseBoard -> Bootstrap -> Initial chain runs, and **Stage 1 Ethernet
microcode boot works end to end** (Initial -> `EtherMicrocodeBoot` ->
fake Pup server serves `AltoMesaDorado.eb` -> EB checksum -> `LoadRam` ->
Alto/Mesa emulator world starts). For history on those earlier milestones
see git log and `docs/handoff.md`.

**Both boot paths now run.** Stage 2 Alto software boot works end to end
(NetExec + games come up, render, and take keyboard input over the fake
Mayday/EFTP server), and **Cedar 6.1 boots all the way to its Viewers
desktop** (2026-07-15) from the Pilot PDI disk image — login, the full
remote install over the in-process STP server, demand-fetched Tioga fonts,
and a live CommandTool prompt (`make run-cedar-work`; screenshot at
`docs/images/cedar-desktop-first-boot-2026-07-15.png`). The frontends
(SDL + WebAssembly) present each world at its native raster. See the
top-level `CLAUDE.md` status and `docs/CONTINUE-HERE.md` for the live
state.

**Update (2026-07-21): Cedar boots to the desktop, runs apps offline, and
plays chess.** The login/desktop bring-up below is DONE; the current
frontier is a clean system volume (Othello/Iago) and the deep-fidelity disk
controller. See the top-level `CLAUDE.md` 2026-07-21 status,
`docs/CONTINUE-HERE.md`, and memory `othello-dead-end-iago-is-the-path` /
`cedar-font-install-attach`.

**Remaining work, roughly ordered:**

1. **Clean system volume via Othello/Iago** (was "Cedar login / system
   volume"). The desktop works from the checkpoint, but a cold-bootable,
   fully-installed volume needs Iago (standalone `OthelloDorado.boot!8` is
   version/format-dead). Path is MAPPED: reach a running Iago by injecting
   `Booting.switches[l]=TRUE` at germ boot, then drive Format/Install. Iago
   writes via normal Pilot IOCBs -- which the shim's write path already
   models (a live Cedar Bringover + `DORADO_PDI_SAVE` mutates and persists
   the PDI today; that is how the demo/corpus checkpoints are baked). Note:
   forcing arbitrary files onto the volume with rusty-backup INJECTION
   crashes Cedar's live FS -- only Cedar's own install path works.

2. **Faithful disk controller, not the PDI shim (Phase-2 fidelity).**
   `--pilot-disk` completes PDI-backed SA4000 IOCBs through a narrow bridge
   in `machine.c` over the still-incomplete disk sequence-PROM / data-
   transfer path (gaps F1-F5). This is not a functional blocker (read+write
   both work through the shim); replacing it with a cycle-accurate
   sequence-PROM/FIFO/ECC controller is a Verilog-fidelity item.

3. **Live display + interrupts for Cedar.** The display is rasterized in C
   (`dorado_machine_render_display_list`) and the field interrupt is injected
   directly (`machine_cedar_io` ORs `CSB.wakeupMask` into NWW). The authentic
   path -- waking the Cedar display microcode tasks so the field handler posts
   the notify itself -- is gated by `display_active` reading `MDS+0420` while
   Cedar's CSB is at absolute `0420` (MDS!=0); reconcile that if the real
   display-task path is pursued.

4. **Germ DoInLoad over Ethernet.** The EFTP/Mayday server serves Cedar boot
   files byte-exact; if the germ is driven to request one (`--germ-netboot-bfn`)
   it would boot over the net instead of the disk. Currently the disk path is
   the working one.

**Parallel / later gaps** (full list in `docs/handoff.md` "Known gaps"):

- **Hold semantics (B1/C1)** - real engine stall on cache miss / Pipe
  full. Boot-stage microcode runs with `mcr.disHold` so it doesn't need
  Hold, but post-boot emulators (e.g. `probe_aemu`) do.
- **Disassembler polish** - sharper FF/JCN sub-decoding, ALUFM cross-
  reference, `.DLS`-format `--listing` mode.

**Don't regress:** the EFTP RxOn-clear in `src/ethernet.c eth_write` is gated
to the Cedar path (`eftp_wait_for_rx_arm`); ungating it drops the held
lock-step EFTP packet on every Alto RxOn toggle and stalls the Alto boot
mid-stream (Galaxian -> 0 px).

**Trace-flag discipline (perf).** Per-step trace checks go through
`dorado_trace_flag()` -- declared in `include/trace.h`, which is the ONE
place (memory.c, display.c and disk.c each used to carry their own
`extern`, the same diverging-copies trap that put two ASCII key maps in
the frontends). Never a raw `getenv()` in a hot path.

It is a two-level test now. `dorado_trace_env_present`, probed once from
`environ` by `dorado_trace_init()`, is 0 when NO `DORADO_*` variable is
set at all; since all 114 keys are `DORADO_`-prefixed, that answers every
one of them and the inline collapses to a load and a branch. Otherwise it
falls through to `dorado_trace_flag_lookup()`, whose pointer-keyed memo is
an open-addressed 1024-entry hash (each call site's string literal is its
own key, so the table must exceed the ~250 sites: as a 128-entry linear
scan, overflowed sites fell back to a full scan plus a real `getenv()` per
call -- 40% of runtime, found 2026-07-18).

**Beware the cliff:** setting ANY `DORADO_*` variable turns the fast path
off, so every gate run pays the memo. Watch for it when timing something.
And keep `dorado_trace_flag_lookup` free of extra tests -- a redundant
`if` there cost 9% on the Cedar path when it was measured on 2026-07-31.

**Profile before optimizing** (`sample <pid> 20 -file out.txt`, then the
"Sort by top of stack" section), and gate on BYTE-IDENTICAL framebuffers
(`cmp` two .pgm files), not pixel counts.

**Current speed (2026-08-05, `make pgo` build): Alto 1.33x real hardware,
Cedar 1.23-1.26x; wasm 0.74x and 0.80x.** Both native paths now beat the
real machine. Every run prints the honest figure -- emulated Dorado
seconds per CPU second, from microinstructions -- so **quote THAT, never a
cycles/s number.** The old "29.1 M cycles/s = 1.75x" claim was bb.cycles/s
read as microinstructions/s, wrong by exactly the 3.70 factor, and the
truth at the time was 0.46x.

Full account: `docs/performance-plan.md`. The measurement method and the
traps that produced two wrong answers: `docs/performance-methodology.md`.

What actually paid, each a byte-identical A/B:

- **PGO + LTO, 1.95x.** `make pgo` (opt-in, two-stage). PGO alone is 1.7x:
  an interpreter dispatch loop is close to its ideal case. Emscripten
  cannot COLLECT a profile but can USE one, so the browser build consumes
  the native profile.
- **The idle BaseBoard 6502, +19.7%.** It ran ONE 6502 INSTRUCTION PER
  DORADO MICROINSTRUCTION -- ~62x the real 1 MHz part relative to a
  16.67 MHz Dorado -- spinning in its idle loop forever. Suppressed after
  boot, master clock advanced synthetically at the same 3.70/uinstr so no
  cycle constant changes meaning. Wakes on any CPReg touch.
- **The germ I/O bridge, Cedar 0.93x -> 1.26x.** It polled guest memory
  every microinstruction for an answer that changes at device rates; now
  every `GERM_POLL_INTERVAL` (64) cycles.
- **The trace-flag cliff, Cedar 0.48x -> 0.73x.** Every Cedar and Lisp
  recipe sets `DORADO_*` config variables, and ANY of them disabled the
  trace fast path for the whole run. `dorado_trace_init()` now skips a
  config allowlist. **A name on that list must be read with a cached
  `getenv`, never `dorado_trace_flag()`** -- through the memo it would
  answer 0 whenever it is the only `DORADO_*` set, which is a behaviour
  change, not a speedup.
- **Four raw `getenv`s and two blocks of trace bookkeeping on hot paths.**
  Three of the getenvs were in `display.c` alone. The worst was eight
  global stores plus a `dorado_br_get()` call at the top of
  `execute_uinstr`, per microinstruction, preparing context for output
  that was off: 7.3%. Grep for raw `getenv` before assuming there is not
  a fifth.

What is left, and it is now genuinely the interpreter: `execute_uinstr`,
`next_pc`, `task_schedule`, `b_bus`, `apply_lc`, `lc_write_address` --
~44% together on Alto with no single item above 8%. The one big
non-interpreter item is Cedar's `eth_ftp_pick_busy_conn` at 14.3%; see
`docs/stp-scan-design.md`.

**Never leave an expensive artifact in a temp directory.** `/tmp`,
`/private/tmp` and any per-session scratchpad DO NOT survive a reboot, and a
Cedar checkpoint bake is ~25 minutes. The instant a run produces a snapshot,
disk image or pack, copy it into `build/good-packs/` (gitignored, so it
stays local but persists) under a name that says what it is, and say where
it went. Scratch is for logs, PGM->PNG conversions and one-shot scripts.
This has nearly cost real work twice: an ad-hoc PDI, and the
`cedar-sil-wip` checkpoint.

**Baking a checkpoint overwrites the shipped one, in place, and can ship a
login screen.** `make cedar-desktop-snapshot` writes straight over
`build/good-packs/cedar-desktop.{snap,pdi}` and the tracked
`snapshot-assets/*.gz`. It types the login at a FIXED cycle; if the prompt
is not up by then the keys are lost, the bake runs its full budget and
snapshots the LOGIN SCREEN (~28,570 px, not ~167,000) -- exit status 0.
Back both files up first, and check the pixel count before trusting the
result. Better: run the recipe's command by hand with `--snapshot-out` and
`--out` pointed at scratch, verify, then install. The recipe's `--type-at`
went stale on 2026-07-31 when serving the LookupFile Pup protocol removed
four retries and a 30 s negative cache from the cold boot and moved the
prompt past it; no gate caught it because every Cedar gate RESTORES a
checkpoint and none cold-boots. See `docs/sil-schematics-handoff.md` §5.2.

**Keyboard buffer (input reliability).** Cedar samples the physical key
matrix once per display field (`CEDAR_FIELD_INTERVAL_CYCLES`); a key whose
down and up fall inside one field is never seen. `dorado_machine_set_key`
therefore ENQUEUES into a file-scope FIFO (`machine_key_queue`), and
`machine_cedar_io` applies one transition every `KEY_FIELDS_PER_TRANSITION`
(3) fields at a field boundary -- so fast typing and paste never drop keys
(measured: sub-field typing dropped 4+/36 chars unbuffered, 0/36 with the
buffer). The queue is a STATIC, not a `dorado_machine` member, on purpose:
adding a struct member changes the snapshot ABI and every baked checkpoint
fails to restore. Reset in `dorado_machine_create`. Engages only for the
live Cedar world (Alto path applies keys directly). Paste pacing is 800K
cyc (~= the 3-field drain rate), down from 1.6M.

**`--cycles` does NOT count Dorado microcycles.** Every cycle number in
this project -- `--cycles`, `--type-at`, `--paste-at`, the gate budgets,
`DORADO_TRACE_GATE` -- is `m->bb.cycles`, the BASEBOARD 6502 cycle
counter. `baseboard_cycles_per_uop` is 1, but `baseboard_run()` executes a
whole 6502 INSTRUCTION and adds all of its cycles, so bb.cycles advances
by the average 6502 instruction length -- **measured 3.70 per Dorado
microinstruction** in the Lyric world (2,704,685 microinstructions per
10,010,624 bb.cycles).

So to convert a cycle budget to emulated Dorado time: divide by 3.7, then
multiply by the 60 ns microcycle. 10 M cycles is ~162 ms of guest time,
not 600 ms. This matters whenever you reason about anything the GUEST
times -- timeouts, retries, interrupt cadence, the Interlisp mouse-chord
window -- and getting it wrong makes the emulator's clock look 3.7x slow
when it is in fact correct: over that same interval the guest's own
`\RCLK` advanced 162.4 ms against 162.3 ms of microinstructions.

Cross-check the guest clock with `DORADO_RCLK_RATE=1`, which samples what
`LOPS.mc opRCLK` returns (VM 0o430 high, the `RTClock` RM register low;
`Junk.mc` maintains it by DDA, and only `RTClock[0:9]` is clock -- the low
6 bits are DDA fraction, so a naive 16-bit read is off by 64x).

**When the snapshot ABI DOES have to change, sweep every checkpoint.**
The rule above avoids the change where it is avoidable, but sometimes a
struct legitimately grows -- and then `dorado_machine_restore` refuses
every checkpoint baked before it, correctly, by comparing the header's
struct sizes. The ones nothing exercises are the ones that stay broken:
`e5ecba1` (Leaf) grew `dorado_ethernet` by 66,496 bytes and both
SAVED-LOGIN checkpoints were dead for a week, native and wasm, because
every Cedar gate restores the DESKTOP and none touches a login image.

`make verify-snapshot-abi` now reads the sizes out of each header and
diffs them against `dorado --print-abi`. It costs a second, boots
nothing, and covers `snapshot-assets/` (native) plus `web-assets/`
(wasm32, via the node build -- skipped, with a warning, when emsdk is
not on PATH). **Run it after any change to a struct a snapshot
serializes.** The two ABIs legitimately differ (`sz_eth` 86,392 vs
86,288), so each set is checked against its own build; a wasm
checkpoint read against the native report looks broken and is not.
Rebake with `make cedar-login-snapshot`, `cedar-bestof-snapshot`,
`cedar-login-web-snapshot`, and the other `*-web-snapshot` targets.

**Frontend input.** `src/typetext.c` owns the ONE canonical ASCII->Alto
key map (`dorado_char_to_key`) plus a non-blocking paced typing queue.
`dorado.c` and `dorado_sdl.c` used to carry diverging static copies (the
SDL one lacked `_`, Cedar's `<-`). Clipboard paste rides the queue in
both frontends (SDL `Cmd/Ctrl+V`, browser paste event ->
`dorado_web_paste`), and `--paste-at CYCLES --paste TEXT` exercises the
same path headlessly. Keep the map in one place.

**Serving files to the Cedar guest** (`chm/cedar/stp-root/`): text files
must be **CR-terminated** and data files need a plain `Bringover`, not
`Bringover -p`. Both mistakes fail identically and silently ("1 files
acted upon" = the `.df` alone). See `docs/running-the-emulator.md`.
