# Dorado glossary

Short definitions of the acronyms and jargon you will run into in the
Hardware Manual, the schematics, and the microcode. Skim this once;
refer back as needed.

The authoritative glossary is Hardware Manual §15. This is the practical
working version.

---

## Buses and data paths

- **A** — 16-bit "alua" bus inside the Processor section. ALU input #1.
- **B** ("alub" inside, "Bmux" on the backplane) — 16-bit second ALU
  input. Crosses the backplane to IFU/Control/Memory.
- **Pd** ("processor data") — 8-input mux carrying ALU output (and a few
  rare register reads) to T / RM / STK / IO.
- **Md** ("memory data") — cache → processor.
- **Id** ("IFU data") — IFU → processor (operand bytes during opcode
  execution).
- **Mar** — 16-bit memory-address-register / displacement to memory.
  `BR[MemBase] + Mar` = virtual address.
- **IOA / IOB** — 8-bit slow-IO address bus / 16-bit slow-IO data bus.
- **Fin / Fout** — 16-bit fast-IO buses (bypass the processor).
- **Sin / Sout** — storage buses between EDAC and DRAM.

## Memories

- **IM** — Instruction Memory. 4096 × 34-bit + 2 parity. The microcode
  store. Loaded with the chosen emulator microprogram at boot.
- **IFUM** — Instruction Fetch Unit Memory. 1024 × 24-bit + 3 parity.
  256 entries × 4 instruction sets. Decodes opcodes into IM dispatch
  addresses.
- **ALUFM** — ALU Function Memory. 16 × 6 bits. Indexed by ALUF.
  Microcoder pre-loads it with the ALU operations the emulator needs.
- **RM** — Register Memory. 256 × 16-bit + 2 parity. The general
  register file. Address = RBase[0:3] ‖ RSTK[0:3].
- **STK** — Stack memory. 256 × 16-bit + 2 parity. Used by the emulator
  in place of RM when the BLOCK bit is 1. Address from StkP, modified
  −4..+3 by RSTK.
- **MAIN** — main memory (DRAM), up to 4 M words.
- **BR** — Base Register memory. 32 × 28 bits. Indexed by MemBase.
- **Pipe** — 16-entry × 6-word memory-reference trace ring. Read via
  Pipe0..Pipe5'.
- **Map** — 16 K (or 64 K) × 19-bit virtual-to-physical translation
  table. Indexed by VA[10:23].
- **Cache** — 4096 words, 64 sets × 4 ways × 16-word lines.

## Registers

- **T** (per-task) — primary 16-bit working register.
- **Q** — 16-bit shift register used by Multiply / Divide.
- **Cnt** — 16-bit decrement-and-test counter.
- **RBase** — 4-bit selector for the high nibble of an RM address.
- **StkP** — 8-bit stack pointer for STK.
- **TIOA** (per-task) — 8-bit IO-address register (Slow IO).
- **ShC** — Shift Control. 16-bit register controlling the shifter-masker.
- **MemBase** (per-task) — 5-bit pointer into BR.
- **MemBX** — 2-bit stack-pointer-like extension on MemBase.
- **Link** (per-task) — 16-bit subroutine-return / dispatch / IM-write /
  TPC-read register.
- **PC** — 16-bit byte displacement of next opcode within BR[31].
- **TPC** (per-task) — Task Program Counter. The "where am I" pointer
  for the current task; saved/restored on task switch.
- **Mcr** — Memory Control Register. Disables parts of the memory
  system for initialization and testing.
- **CFlags** — control-flag register (set of state bits influencing
  memory references).

## Microinstruction fields

- **JCN** — Jump Condition / Next. 8-bit field encoding jumps, calls,
  conditional jumps, dispatches, and returns.
- **RSTK** — RM Stack adjust. Either provides the low nibble of the RM
  address, or (with BLOCK=1) adjusts StkP by −4..+3, or supplies parity
  bits when writing IM.
- **ASEL** — A-bus source selector.
- **BSEL** — B-bus source selector.
- **ALUF** — 4-bit index into ALUFM.
- **LC** — Load Control: where to deliver Pd (T, RM, STK, …).
- **FF** — "Special function" — large multiplexed field that picks
  among constants, register loads, IO ops, Map ops, etc.
- **BLOCK** — 1 bit. Switches RM addressing to STK; enables stack
  adjustments; affects task switching.

## Sections (boards)

- **BaseBoard** — auxiliary 6502 + EPROMs that boots the machine,
  monitors power, runs the front-panel button, drives the green LED.
- **ProcH / ProcL** — the two halves of the Processor board (high /
  low bits of the data path).
- **Control_A / Control_B** — the two halves of the Control board
  (microsequencer, JCN decoder, task scheduler).
- **IFU** — Instruction Fetch Unit board.
- **MEMC / MEMD / MEMX** — Memory Cache / Data / eXtension boards.
- **DskEth** — combined Disk + Ethernet controller board.
- **DispM / DispY** — Display, Monochrome / Y-channel (color/extended).
- **PCMSA / MSA** — Memory Storage Array.

## Subsystems / concepts

- **IFU prefetch** — the IFU runs ahead of the processor, decoding the
  next opcode and computing its IM dispatch address. `IFUJump` consumes
  the prefetch.
- **IFUJump entry vector** — table of microcode addresses that the IFU
  jumps to when an opcode dispatch happens.
- **Hold** — combinational signal that stalls the processor when a
  cache miss / IFU-not-ready / fault is in flight.
- **Task switch** — the Control section can switch to a higher-priority
  task at every microcycle. Per-task registers are duplicated; Link
  and TPC are the per-task state needed to resume cleanly.
- **Wakeup** — IO-controller signal that asserts a per-task wake bit.
  Highest-priority awake task wins arbitration.
- **CPReg** — Control/Parameter Register: the back-channel between the
  BaseBoard and the main Dorado.
- **Pipe stage** — index into the Pipe ring; Pipe0 is the oldest entry.

## File and tool names

- **`.MB`** — micro-binary, the canonical loadable microcode format.
  Output of MicroD.
- **`.EB`** — Ether-bootable wrapping of an MB (with a single-page
  overhead used by the EtherBoot protocol).
- **`.SB`** — Soft-bootable variant (bootable from disk, eventually).
- **`.DIB`** — output of the Micro assembler, before MicroD placement.
- **`.DM`** — IFS dump-format archive.
- **`.PRESS`** — Xerox PARC document/print format. Most "manual" PDFs
  in this repo started life as `.press` files.
- **Micro** — the microassembler (front end).
- **MicroD** — the placer / linker (back end). Produces `.MB`.
- **Midas** — the Alto-resident debugger / loader for the Dorado.
  Accesses the Dorado via the BaseBoard.
- **LoadRam** — microcode procedure that copies microcode from main
  memory into IM (and IFUM).
- **LoadMB** — Alto/Dorado program that reads a `.MB` file and either
  loads it into the Dorado's IM or repackages it as `.EB` / `.SB`.
- **APNew** — EPROM programmer driver.
- **Pup, EtherBoot** — protocols used to fetch microcode boot files
  from a Gateway / IFS.

## Operating systems / virtual machines

- **Alto OS** — the original Alto operating system. Runs under the
  Alto emulator microcode (the smallest target for our emulator).
- **Mesa** — Xerox's strongly-typed systems language. Pilot is the OS
  written in Mesa; the Dorado was designed primarily to host Mesa.
- **Cedar** — Mesa's evolution; runs on top of Pilot.
- **Interlisp-D** — Lisp environment (uses the Lisp microcode).
- **Smalltalk-76 / Smalltalk-80** — runs under the Smalltalk microcode.
- **D0 / Dolphin** — Dorado's smaller sibling. Same MicroD .MB
  toolchain; useful for cross-reference.
- **Dandelion / Star / 8010** — productized successor in the
  Alto-Dorado-Star research-to-product lineage.
