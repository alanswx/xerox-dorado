# dorado/ — the C emulator

This directory holds the C-language Dorado emulator. See the parent
`CLAUDE.md` for the project mission. This file captures what we have
*built* so far, plus the format-level findings that aren't in the
Hardware Manual but were dug out of the MicroD BCPL source.

## Build

```
make           # builds build/mbdis and build/test_mb
make test      # runs the integration tests
make clean
```

Pure C99, no external dependencies. clangd users: `compile_commands.json`
in this directory keeps the language server happy.

## Layout

```
dorado/
├── Makefile
├── compile_commands.json
├── CLAUDE.md             ← this file
├── include/
│   └── mb.h              .MB loader API
├── src/
│   ├── mb.c              .MB parser
│   ├── disasm.c          model-1 unshuffle + microinstruction field decoder
│   ├── microcode.c       image→real placement, builds hardware-ready IM[4096]
│   ├── cpu.c             microengine (single task, no IFU/memory/tasking yet)
│   └── mbdis.c           CLI: dump / symbolic disasm
└── tests/
    ├── test_mb.c         loader
    ├── test_disasm.c     decoder
    ├── test_microcode.c  placement
    └── test_cpu.c        CPU smoke tests + real-Bootstrap probe
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

**Current real-microcode reach:** Bootstrap.mb runs against a real
**BaseBoard 6502** (from `chm/dorado/doradobaserom.mb!13`, reset
vector at 0xF3A7). The Dorado microengine and the BaseBoard both
execute correctly side-by-side through their respective reset paths.
Bootstrap's poll loop (6 PCs at 7700/7715/7741/7742/7746/7747) is
still spinning because the boot-button hasn't been pressed and the
BaseBoard hasn't transitioned into Initial-upload mode. See item 4
in "What's next" for the handshake gap.

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
- B-source overrides (FA=1, FB=6 or 7): Pipe / FaultInfo / EventCnt /
  IFUMRH / DBuf are all stubbed to 0; **B←Link**, **B←RWCPReg**
  (cpreg state register, currently 0) are wired up.
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

`make test` runs four binaries:
- `test_mb`        — loader against ftest.MB + every microcode in chm/dorado/
- `test_disasm`    — decoder + field-width checks across ~25k µinstrs
- `test_microcode` — placement (image→real) verified against ftest.DLS
- `test_cpu`       — synthetic micrograms (T←const, increment loop, RM
                     round-trip, unsupported-halt) + Bootstrap probe

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

Bootstrap probe now reaches the **shifter** at cycle 16. Priority is
extending coverage of what real microcode hits — each step here pushes
the probe further.

1. ~~Shifter — done.~~ See `shifter_output()` in `src/cpu.c`. ShC- and
   FF-controlled paths both work; ShiftNoMask / ShiftLMask /
   ShiftRMask / ShiftBothMasks all implemented. ShMd* variants stub
   Md=0 until memory lands.
2. ~~Read/Write IM, Read/Write TPC — stubbed (advance to .+1).~~ Real
   IM mutation needs to land before Bootstrap can produce a working
   Initial; see item 3.
3. ~~CPReg I/O — **done correctly via real 6502**.~~ See
   `src/baseboard.c`. We dropped in fake6502 (CC0 from
   `C-Chads/MyLittle6502`), built the BaseBoard's 64K memory map
   with five 6532 RIOT chips at the canonical addresses (0x400,
   0x480, 0x500=CPReg, 0x580, 0x600), and load
   `chm/dorado/doradobaserom.mb!13` as the canonical 64K ROM image.
   The Dorado's `B←RWCPReg` reads from RIOT #3's PA||PB latches.
   Bootstrap probe now runs against a real BaseBoard 6502 from its
   reset vector at 0xF3A7.

4. **BaseBoard handshake — current blocker** ★ . Bootstrap's poll
   loop and the BaseBoard 6502 are both running but not handshaking
   to start the Initial upload. Likely needed:
   - Boot-button press (BaseBoard.MiscByte's Boot' bit). Without a
     press, the BaseBoard sits in its watchdog/idle loop and never
     transitions into the upload state. We have
     `baseboard_press_boot()` but the probe doesn't call it yet.
     Need to model the timing per the booting memo (0.25–2.5s
     window; 1/2/3/4-push semantics).
   - 6532 timer accuracy. The BaseBoard times boot-button presses
     in subticks; if our timer model is too loose, the boot-decoder
     won't recognize the press.
   - MCPBus (RIOT #4 at 0x580). The control bus that the BaseBoard
     drives to halt/release the Dorado. We don't model it yet —
     once the BaseBoard tries to issue a control command it'll
     write to address 0x580+ and our handler will silently swallow
     it. Need to model the bits per `doradoio.mdefs`.

5. **Actual IM writes** — currently advancing PC without writing.
   For getting Bootstrap→Initial handoff working it matters: once
   the BaseBoard is uploading Initial via CPReg, Bootstrap needs to
   actually deposit the bytes into IM. Encode the 9-bit slice per
   RSTK[2:3] (HM Figure 6) back into the `dorado_uinstr` at
   `mc->im[Link & 0xFFF]`. Each Write IM updates 18 bits (one half)
   selected by RSTK[3].
4. **FF function table** — Tables 11a-e. Bootstrap is exercising a
   handful right now (we silently ignore most of them, which is part
   of why the spin loop doesn't make progress). Audit a probe trace
   and implement the FF functions Bootstrap actually uses
   (`Cnt←B`, `Cnt←small`, `Pointers←B`, `B←Link`, `Link←B`,
   `B←RWCPReg`, `MidasStrobe←B`, `RBase←FF[4:7]`).
5. **Memory subsystem** — by the time we boot Initial we'll have
   touched `Fetch←` / `Store←`. See `docs/memory-architecture.md` for
   the design plan; defer the cache / Map / Pipe until microcode
   demands them. Initial first uses a flat 16-bit memory image.
6. **IFU + tasking + Hold** — needed for emulator microcode (Mesa,
   Cedar, Alto). Big chunk; defer until Initial runs end-to-end.
7. **Disassembler polish** (lower priority — CPU consumes
   `dorado_uinstr` directly):
   - Sharper FF/JCN sub-decoding into named operations.
   - ALUFM cross-reference (`ALUF=04(A+B)`).
   - `.DLS`-format `--listing` mode for line-for-line MicroD diff.
