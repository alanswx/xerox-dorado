# Checking the emulator against the Dorado's own design data

**2026-08-08. First pass: ProcH and ProcL, the two processor boards.**

Everything in this project has been checked against the Hardware Manual, the
schematics-as-PDFs, and PARC's microcode. This is the first check against the
**design input** — the files the machine was actually built from — which is
what Tim recommended when he pointed at the Sil tools.

`tools/fetch_dorado_sil.py` fetched them (see `docs/chm-archive.md`);
`tools/sil_netlist_report.py` reads them.

## What the files are

`chm/sil/<Board>-Rev-Xx.dm!N_/` holds, per board revision:

| file | what |
|---|---|
| `<Board>-Rev-Xx-C.nl`, `-E.nl` | the board's **backplane interface**, `Signal: #s05-C.12` |
| `<Board>-Rev-Xx.lc` | the **component list**: part number, pins, package, grid positions |
| `<Board>-Rev-Xx.wl` | the wire list (118 KB for ProcH) |
| `<Board>NN.sil` | the numbered drawing sheets, Sil's binary format |
| `Build.cm`, `Print<Board>.cm` | PARC's own build scripts |

The `.nl` files are plain text and use **the machine's own signal names** —
`IOB`, `RSTK`, `MemBase`, `Overflow'` — which are the same names the C
emulator uses. That is what makes a direct comparison possible.

```
$ tools/sil_netlist_report.py ProcH ProcL
== ProcH  (ProcH-Rev-Ce.dm!1_)  175 nets, 68 signals
== ProcL  (ProcL-Rev-Ci.dm!1_)  176 nets, 71 signals
```

## Structural facts the design settles

- **The 16-bit datapath is split across the two boards, with a parity bit
  each.** Every major bus appears as `.00-.07` plus `.16` on ProcH and
  `.08-.15` plus `.17` on ProcL: `IOB`, `dMD`, `BMux`. So bit 16 is high-byte
  parity and bit 17 low-byte parity.
- **The ALU is four MC10181 slices**, two per board (`.lc`: `MC10181/24/E6W
  x2` on each). The MC10181 is the ECL 4-bit ALU, and its control is exactly
  the `aluF0..aluF3`, `aluM`, `aluC0` and `aluCout` nets on both connectors —
  which is why an ALUFM entry is 6 bits (4 function + mode + carry-in) rather
  than an opcode.
- **The scale**: 419 packages on ProcH, 420 on ProcL, ~50 part types each.

## What the interface CONFIRMS about the emulator

Every field and register width the emulator implements appears in the
hardware interface at the same width. None of these was checked this way
before:

| emulator | netlist | |
|---|---|---|
| `RSTK` 4 bits | `RSTK[0-3]` | ✓ |
| `ALUF` 4 bits | `ALUF[0-3]` (ProcL) | ✓ |
| `BSEL` 3 bits | `BSEL'[0-2]` | ✓ |
| `ASEL` 3 bits | `ASEL'[0-2]` | ✓ |
| `LC` 3 bits | `LC[0-2]` | ✓ |
| `FF` 8 bits | `FF[0-7]` | ✓ |
| `MemBase←B[3:7]` (5 bits) | `MemBase[0-4]` | ✓ |
| `RBase←B[12:15]` (4 bits) | `RbAdr[0-3]` | ✓ |
| `StkP←B[8:15]` (8 bits) | `StkAdra[0-7]` | ✓ |
| `TIOA←B[0:7]` (8 bits) | `TIOA[0-7]` | ✓ |

And **every branch condition the emulator implements is a real signal on the
backplane**:

| JCN condition | net |
|---|---|
| ALU=0 | `ResEqZero'`, `aluOut=0'` |
| ALU<0 | `ResLtZero'` |
| Carry' | `ALUCarry`, `aluCout` |
| Cnt=0 | `Cnt=Zero'` |
| R<0 | `RmLtZero'` |
| R odd | `RmOdd'` |
| Overflow | `Overflow'` |

`Overflow'` is worth noting on its own: the Overflow branch condition was one
of the six microengine bugs the bring-up fixed, and here it is as a dedicated
pin.

## What the interface shows the emulator does NOT model

These are the findings. Each is a signal the hardware has and the emulator
has no counterpart for, verified by grepping `dorado/src` and
`dorado/include`.

### 1. A byte-parity network across the datapath

`IOB.16`/`.17`, `dMD.16`/`.17`, `BMux.16`/`.17` carry parity, and there are
dedicated error and control lines: **`IOPE`**, **`MdPE`**, **`RamPE`** and
**`CkMdParity'`**, on both boards.

We model microinstruction parity (IM's two parity bits) and memory ECC, and
`cpu.h` has a `parity_error` flag tied to the IOAtten fault. **We do not
model bus or register-file parity at all** — no `RamPE`, `MdPE` or `IOPE`
anywhere. A microprogram that deliberately induces or checks a parity fault
would see nothing happen. Given that the Hardware Manual lists faults as
visible to microcode, this is a real gap rather than a detail.

### 2. The Hold interface, at pin level

`PRhold`, `PrBlock'`, `PrHoldReq` and `SimHoldDis`. Hold has been a known gap
since the beginning (`dorado/CLAUDE.md`: "real engine stall on cache miss /
Pipe full"), but the netlist turns it from a concept into a four-signal
interface with a name for each part, including a *simulation disable*
(`SimHoldDis`) — the hardware had its own way of turning Hold off, which is
what boot-stage microcode does with `mcr.disHold`.

### 3. `SubTask.0/1`

On ProcL, unmodelled. Sixteen-way tasking we do model; subtasking we have
never touched.

### 4. `StkError`

A real signal on ProcL. In the emulator it exists only inside a comment in
`cpu.c` describing when the hardware would raise it. Stack overflow and
underflow are therefore undetected.

## How to repeat this

```sh
tools/sil_netlist_report.py ProcH ProcL       # interfaces, grouped by bus
tools/sil_netlist_report.py --chips ProcH     # the parts and where they sit
tools/sil_netlist_report.py --grep Hold ProcL # find a signal family
```

Fourteen more boards are sitting in `chm/sil/` unexamined: ContA, ContB,
MemC, MemD, MemX, IFU, DispY, DispM, DskEth, BaseBd, msa, PCMSA, IOTest,
Music. **IFU and the three memory boards are the obvious next targets** — the
IFU because its interface should settle several open questions about what the
emulator's IFU model owes the rest of the machine, and MemC/MemD/MemX because
the Pipe and Map are where the subtlest emulation bugs have lived.

## What this is not

It is a **coverage** cross-check, not an equivalence proof. The `.nl` files
give the boards' interface, not their logic; proving our datapath computes
what ProcH's 419 packages compute needs the `.wl` wire list and the sheets,
which is Phase 2's job. What this pass establishes is that the emulator's
*shape* — field widths, register widths, branch conditions — matches the
hardware exactly, and it names four specific things the hardware has that we
do not.
