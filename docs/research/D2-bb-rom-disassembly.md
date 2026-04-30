---
gap: D2
status: landed
---

# D2 — BaseBoard ROM disassembly + label cross-reference

## Source citations

- `chm/dorado/doradobaserom.mb!13` (48738 bytes; 7492 of those are
  populated 6502 bytes in the C000–FFFF region).
- `chm/dorado/expanded/doradobaserom.dm!12_/` — the `.masm` source
  set for the BB ROM, plus `doradoio.mdefs` (hardware-register
  definitions) and the `.cm` build scripts.
- The `.MB` loader carries an embedded **symbol table** with 76
  exported labels and their absolute addresses. `mbdis` already
  prints these inline; we extracted them to a flat TSV.

## Spec / observations

- The BB ROM has 9 source files compiled with `bca/el` and linked.
  `.LOC` directives place each file's data and code segments at
  named anchors (`ResetCode`, `BootCode`, `MidasCode`, etc.). The
  linker map is not in the archive, so we recovered addresses from
  the `.MB` symbol table.
- File-level layout (from the symbol table):
  - **`C000`–`D7FF`** — Boot1 hunk data (8 KB). `BOOTBLOCKTABLE` at
    `$C000` is the 22-byte index pointing at `BOOT0BLOCK` /
    `BOOT1BLOCK`; `BOOT1` raw hunk stream begins at `$C016`.
  - **`F000`–`FD2A`** — BB program code, in this order:
    `MidasCode` (`F000`), `ResetCode` (`F2A2`), `ContinuousCode`
    (`F4F3`), `MufflerTable` (`F6A9`), `AnalogCode` (`F6F5`),
    `CPIntCode` (`F86F`), `BootCode` (`FA6D`), `Boot0` data block
    (`FE00`).
  - **`FFFA`–`FFFF`** — vectors. NMI=`F000` (MidasCode),
    RESET=`F3A7` (`Reset:` inside `ResetCode`), IRQ=`F2A2`
    (`Interrupt:` inside `ResetCode`).
- IRTable canned microinstructions live at `$FA77`–`$FAA9`. Each
  entry is 5 bytes (one packed Dorado microinstruction):
  `CPRegToLink#`, `IMLHRSTK.0Is0#`, `IMLHRSTK.0Is1#`,
  `IMRHBLOCKIs0#`, `IMRHBLOCKIs1#`, `Return#`, `QFromCPReg#`,
  `ALUFM[0]FromQ#`, `SetHoldTaskSim#`. The first 4 instructions
  alternate Write IM left-half / right-half against either
  `RSTK[0]=0` or `RSTK[0]=1` / `BLOCK=0` or `BLOCK=1` — exactly
  matching the four hunk-byte-0 bit patterns described in the
  research note for **A1**.
- Boot1 hunk data at `$C016` matches the format documented in
  `doradoboot.masm` `SendAHunk:`: 17-byte hunks, 4 microinstructions
  per hunk, sharing RSTK.0/BLOCK in byte 0.

## Implementation

- **`chm/disassembly/bb_labels.tsv`** — 76 labels in
  `HEX<tab>NAME` form, extracted from the `.MB` embedded symbol
  table via `mbdis`. Captured for `bbdis --labels`.
- **`chm/disassembly/bb_F000-FFFF.s`** — re-rendered with labels;
  every JSR / Bxx / branch into a known label gets a `; NAME`
  annotation.
- **`chm/disassembly/bb_C000-D7FF.s`** — re-rendered; the only
  symbol in this range is `BOOT1BLOCK` and the data labels.

The label TSV was pulled with the following one-liner, kept here
because the format is fragile (octal addresses in the mbdis dump,
and bash awk lacks `strtonum`):

```sh
./dorado/build/mbdis "chm/dorado/doradobaserom.mb!13" \
  | awk '/^[ \t]+[0-9]+:[ \t]+[0-9]+[ \t]+[A-Za-z]/ {
        addr_oct = $1; gsub(":", "", addr_oct);
        n = 0; for (i=1;i<=length(addr_oct);i++)
          n = n*8 + (substr(addr_oct,i,1)+0);
        printf "%04X\t%s\n", n, $3;
      }' | sort -u > chm/disassembly/bb_labels.tsv
```

To regenerate annotated dumps later:

```sh
./dorado/build/bbdis --labels chm/disassembly/bb_labels.tsv \
    --start F000 --end FFFF "chm/dorado/doradobaserom.mb!13" \
    > chm/disassembly/bb_F000-FFFF.s
```

## Verification

- `bbdis --vectors` reports vectors that match the `.LOC 0fffc` /
  `.LOC 0fffa` entries in `doradoreset.masm` and
  `doradomidasint.masm`.
- `Reset:` in `doradoreset.masm` begins with `CLD` followed by a
  zero-page-clear loop with `BNE` back-edge — bit-exact match
  against the disassembly at `$F3A7`.
- `LoadDoradoCode:` (the BB-side master driver) starts at `$FAAE`
  with `JSR WaitForCPControl` (`$FA1F`) followed by
  `JSR StopDorado` (`$F948`) — matches the source.
- `SendAHunk:` at `$FC5E` and `SendViaMIR:` at `$FCFD` align with
  the comment-block packed-byte spec in `doradoboot.masm`. This is
  the reference for gap A1.

## Follow-on

The 76 globally-exported labels are sufficient for Phase-2 work
(A2, B2, H2, B8, A1). Local labels (per-file branch targets,
helper sub-routines, table indices) are not exported — recovering
them would require either:
- assembling the source ourselves with a 6502 cross-assembler
  matching the `bca` syntax, or
- hand-tracing branches from the disassembly using the source as a
  guide.

For now the global labels carry the day. Continue Phase 2 with A2
(decode the Boot0 hunks at `$FE00` to find the slow-IO TIOAs Boot0
polls).
