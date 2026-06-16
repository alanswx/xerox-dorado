# Dorado Hardware Manual

> **This document is for internal Xerox use only.**

**by E. R. Fiala**

Contributions to the manual by R. Bates, D. Boggs, B. Lampson, K. Pier, E. Taft, and C. Thacker.
Other help by D. Clark, W. Crowther, W. Haugeland, G. McDaniel, and S. Ornstein.

**14 September 1981**

The document describes the architecture and hardware design of the Dorado computer at a level
appropriate for programming. At the date of this printing, approximately 22 systems have been
released to users.

This release incorporates a major revision of the Display Controller chapter, medium revisions to
the Disk Controller and Instruction Fetch Unit chapters, and minor revisions elsewhere.

**Revision history**

| Date | Change |
|------|--------|
| 14 February 1979 | First complete manual exclusive of io controller chapters. |
| 8 October 1979 | Chapters on io controllers added; major revisions. |
| 14 September 1981 | Major revision to the Display Controller chapter, medium revision to Instruction Fetch Unit and Disk chapters, minor revisions elsewhere. |

*Xerox — Palo Alto Research Center · Computer Sciences Laboratory · 3333 Coyote Hill Rd., Palo Alto, California 94304*

---

> ### About this Markdown conversion
> This file was converted from a scanned, OCR'd PDF of the original 1981 manual (169 printed pages).
> The running prose is the cleaned OCR text layer. **All 31 tables and 17 figures are reproduced as
> high-resolution scan images** for full fidelity, since the manual's column-aligned tables and
> notation (especially the Xerox `←` assignment arrow) do not survive OCR reliably. The seven core
> microcode field-decode tables are additionally transcribed to clean Markdown tables.
> HTML comments such as `<!-- PDF page N / printed page M -->` mark page boundaries for provenance.
> Minor OCR artifacts may remain in the body text.

---

## Table of Contents

- [1. Introduction](#1-introduction)
- [2. Overview](#2-overview)
- [2.1. Control](#21-control)
- [2.2. Registers, Memories, and Data Paths](#22-registers-memories-and-data-paths)
- [2.3. Timing](#23-timing)
- [2.4. Instruction Fields](#24-instruction-fields)
- [2.6. Notation](#26-notation)
- [3. Processor Section](#3-processor-section)
- [3.1. RM and STK Memories, RBase and StkP Registers](#31-rm-and-stk-memories-rbase-and-stkp-registers)
- [3.2. Cnt Register](#32-cnt-register)
- [3.3. Q Register](#33-q-register)
- [3.4. T Register](#34-t-register)
- [3.5. BSEL: B Multiplexor Select](#35-bsel-b-multiplexor-select)
- [3.6. ASEL: A Source/Destination Control](#36-asel-a-sourcedestination-control)
- [3.7. ALUF, ALU Operations](#37-aluf-alu-operations)
- [3.8. LC: Load Control for RM and T](#38-lc-load-control-for-rm-and-t)
- [3.9. FF: Special Function](#39-ff-special-function)
- [3.10. Multiply and Divide](#310-multiply-and-divide)
- [3.11. Shifter](#311-shifter)
- [3.12. Hold and Task Simulator](#312-hold-and-task-simulator)
- [4. Control Section](#4-control-section)
- [4.1. Tasks](#41-tasks)
- [4.2. Task Switching](#42-task-switching)
- [4.3. Next Address Generation](#43-next-address-generation)
- [4.4. Conditional Branches](#44-conditional-branches)
- [4.5. Subroutines and the Link Register](#45-subroutines-and-the-link-register)
- [4.6. Dispatches](#46-dispatches)
- [4.7. IFU Addressing](#47-ifu-addressing)
- [4.8. IM and TPC Access](#48-im-and-tpc-access)
- [4.9. Hold](#49-hold)
- [4.10. Program Control of the DMux](#410-program-control-of-the-dmux)
- [5. Memory Section](#5-memory-section)
- [5.1. Memory Addressing](#51-memory-addressing)
- [5.2. Processor Memory References](#52-processor-memory-references)
- [5.3. IFU References](#53-ifu-references)
- [5.4. Memory Timing and Hold](#54-memory-timing-and-hold)
- [5.5. The Map](#55-the-map)
- [5.6. An Automatic Storage Management Algorithm](#56-an-automatic-storage-management-algorithm)
- [5.7. Mesa Map Primitives](#57-mesa-map-primitives)
- [5.8. The Pipe](#58-the-pipe)
- [5.9. Faults and Errors](#59-faults-and-errors)
- [5.10. Storage](#510-storage)
- [5.11. The Cache](#511-the-cache)
- [5.12. Initialization](#512-initialization)
- [5.13. Testing](#513-testing)
- [6. Instruction Fetch Unit](#6-instruction-fetch-unit)
- [6.1. Overview of Operation](#61-overview-of-operation)
- [6.2. The IFUJump Entry Vector](#62-the-ifujump-entry-vector)
- [6.3. Timing Summary](#63-timing-summary)
- [6.4. Use of MemBX and Duplicate Stk Regions](#64-use-of-membx-and-duplicate-stk-regions)
- [6.5. Traps](#65-traps)
- [6.6. IFU Reset](#66-ifu-reset)
- [6.7. Rescheduling](#67-rescheduling)
- [6.8. Breakpoints](#68-breakpoints)
- [6.9. Reading and Writing IFUM](#69-reading-and-writing-ifum)
- [6.10. Continuing from Processor Faults](#610-continuing-from-processor-faults)
- [6.11. IFU Testing](#611-ifu-testing)
- [6.12. Details of Pipe Operation](#612-details-of-pipe-operation)
- [6.13. Timing Details](#613-timing-details)
- [7. Slow IO](#7-slow-io)
- [7.1. Input/Output Functions](#71-inputoutput-functions)
- [7.2. IOPcodes](#72-iopcodes)
- [7.3. Wakeup, Block, and Next](#73-wakeup-block-and-next)
- [7.4. SubTasks](#74-subtasks)
- [7.5. Illegal Things IO Tasks Must Not Do](#75-illegal-things-io-tasks-must-not-do)
- [8. Fast IO](#8-fast-io)
- [8.1. Transport](#81-transport)
- [8.2. Wakeups and Microcode](#82-wakeups-and-microcode)
- [8.3. Latency](#83-latency)
- [9. Disk Controller](#9-disk-controller)
- [9.1. Disk Addressing](#91-disk-addressing)
- [9.2. Sector Layout Considerations](#92-sector-layout-considerations)
- [9.3. General Firmware Organization](#93-general-firmware-organization)
- [9.4. Task Wakeups](#94-task-wakeups)
- [9.5. Control Register](#95-control-register)
- [9.6. Format RAM and Sequence PROMs](#96-format-ram-and-sequence-proms)
- [9.7. Tag Register](#97-tag-register)
- [9.8. FIFO Register](#98-fifo-register)
- [9.9. Muffler Input](#99-muffler-input)
- [9.10. Error Detection and Correction](#910-error-detection-and-correction)
- [10. Display Controller](#10-display-controller)
- [10.1. Operational Overview](#101-operational-overview)
- [10.2. Video Data Path](#102-video-data-path)
- [10.3. Horizontal and Vertical Control](#103-horizontal-and-vertical-control)
- [10.4. Pixel Clock System](#104-pixel-clock-system)
- [10.5. OIS Seven-Wire Video Interface](#105-ois-seven-wire-video-interface)
- [10.6. Processor Task Management](#106-processor-task-management)
- [10.7. Slow IO Interface](#107-slow-io-interface)
- [10.8. DispM Terminal Interface](#108-dispm-terminal-interface)
- [10.9. DDC Initialization Requirements](#109-ddc-initialization-requirements)
- [10.10. Speed and Resolution Limits](#1010-speed-and-resolution-limits)
- [11. Ethernet Controller](#11-ethernet-controller)
- [11.1. Ethernet Packets](#111-ethernet-packets)
- [11.2. Controller Overview](#112-controller-overview)
- [11.3. Receiver](#113-receiver)
- [11.4. Transmitter](#114-transmitter)
- [11.5. Clocks](#115-clocks)
- [11.6. Task Wakeups](#116-task-wakeups)
- [11.7. Muffler Input](#117-muffler-input)
- [11.8. IOB Registers](#118-iob-registers)
- [11.9. Control Register](#119-control-register)
- [11.10. Status Register](#1110-status-register)
- [12. Other IO and Event Counters](#12-other-io-and-event-counters)
- [12.1. Junk Task Wakeup](#121-junk-task-wakeup)
- [12.2. General IO](#122-general-io)
- [12.3. Event Counters](#123-event-counters)
- [13. Error Handling](#13-error-handling)
- [13.1. Processor Errors](#131-processor-errors)
- [13.2. Control Section Errors](#132-control-section-errors)
- [13.3. IFU Errors](#133-ifu-errors)
- [13.4. Memory System Errors](#134-memory-system-errors)
- [13.5. Sources of Failure](#135-sources-of-failure)
- [13.6. Error Correction](#136-error-correction)
- [14. Performance Issues](#14-performance-issues)
- [14.1. Cycle Time](#141-cycle-time)
- [14.2. Emulator Performance](#142-emulator-performance)
- [14.3. IFU Not-Ready Wait](#143-ifu-not-ready-wait)
- [14.4. Microstore Requirements](#144-microstore-requirements)
- [14.5. Cache Efficiency and Miss Wait](#145-cache-efficiency-and-miss-wait)
- [14.6. Performance Degradation Due to IO Tasks](#146-performance-degradation-due-to-io-tasks)
- [14.7. Cache and Storage Geometry](#147-cache-and-storage-geometry)
- [15. Glossary](#15-glossary)

---

## List of Tables

*Every table is reproduced as a high-resolution scan image at its location in the text. The seven core microcode field-decode tables (6–10, 12, 13) are additionally transcribed to Markdown.*

- Table 1: Memories
- Table 2: Registers
- Table 3: Data Paths
- Table 4: Load Timing
- Table 5: Instruction Fields
- Table 6: RSTK Decodes for Stack Operations
- Table 7: BSEL Decodes
- Table 8: ASEL Decodes
- Table 9: ALUFM Control Values
- Table 10: LC Decodes
- Table 11: FF Decodes
- Table 12: ALUF Shift Decodes
- Table 13: Branch Conditions
- Table 14: Reserved Locations in the Microstore
- Table 15: Timing of a Dirty Miss
- Table 16: Map Configurations
- Table 17: Fault Indications
- Table 18: IFUM Fields
- Table 19: Operand Sequence for ←Id
- Table 20: IFU FF Decodes
- Table 21: IO Register Addresses
- Table 22: Task Assignments
- Table 23: T-80 Specifications and Characteristics
- Table 24: OIS Terminal Microcomputer Messages
- Table 25: DDC Muffler Signals
- Table 26: Ethernet Muffler Signals
- Table 27: Error-Related Signals
- Table 28: Double Error Incidence vs. Repair Rate
- Table 29: Utilization of the Microstore
- Table 30: Execution Time vs. Cache Efficiency
- Table 31: Cache Geometry vs. LRU Behavior

## List of Figures

*The 17 figures are full-page schematic and timing diagrams, collected in the [Figures](#figures) appendix at the end of this document, as in the original.*

- Figure 1: Dorado: Programmer's View
- Figure 2: Card Cage
- Figure 3: Processor Hardware View
- Figure 4: Shifter
- Figure 5: Control Section
- Figure 6: Next Address Formation
- Figure 7: Instruction Timing
- Figure 8: Overall Structure of the Memory System
- Figure 9: Cache, Map, and Storage Addressing
- Figure 10: The Pipe and Other Memory Registers
- Figure 11: Error Correction
- Figure 12: Instruction Fetch Unit Organization
- Figure 13: Disk Controller
- Figure 14: Display Controller
- Figure 15: Display Controller IO Registers
- Figure 16: Ethernet Controller
- Figure 17: Programmers' Crib Sheet

---

<!-- PDF page 8 / printed page 1 -->

## 1. Introduction

Dorado is a high performance, medium cost microprogrammed computer designed primarily
to implement a virtual machine for the Mesa language, as described in "The Mesa
Processor Principles of Operation," and to provide high storage bandwidth for picture
processing applications.   Dorado aims more at word processing than at numerical
applications.

The microprocessor has a nominal cycle time of 60 ns, and most Mesa opcodes will
execute in one or two cycles; the overall average opcode execution time will be subject to
a number of considerations discussed later.        Dorado will also achieve respectable
performance when implementing virtual machines for the Alto, Interlisp, and Smalltalk
programming systems, although simple instructions for these run three to five times slower
than Mesa.

Dorado is implemented primarily of MECL-10K integrated circuits; storage boards use MOS
and Schottky TTL components primarily. Backplanes and storage boards are printed
circuits; other logic boards are stitchweld in prototypes and multiwire or PC in production
machines. The mainframe is divided into sections called Control, Processor, Instruction
Fetch Unit (IFU), and Memory, and peripheral control is accomplished by the Disk,
Ethernet, and Display Controller sections, as discussed in chapters of this manual. The
main data paths, shown in Figure 1, are 16-bits wide (the word size). The control section is
shown in Figure 5. The Baseboard section, used to control the mainframe, is discussed in
the "Dorado Debugging Interface" document.

The processor is organized around an Arithmetic and Logic Unit (ALU) whose two inputs
are the A and B data paths (Figure 1), and whose output is normally routed to the Pd data
path. Inputs to A, B, and Pd include all registers accessible to the programmer. In
addition, 16-bit literal constants can be generated on B. B appears on the backplane for
communication with the IFU, Control, and Memory sections.

The processor also includes a 32:bit in/16-bit out shifter masker optimiz;ed for field
. insertion and extraction and with specialized paths for the bit-boundary block transfer
(BitBlt) instruction.

An instruction fetch unit (the IFU) operating in parallel with the .processor can handle up to
four instruction sets with 256 opcodes each; opcodes may independently be specified as
one, two, or three bytes long.

Emulator and IFU references to main memory are made through a 4k-word high-speed
cache. Main storage can be configured in various sizes up' to a maximum of 222 16-bit
words when 64k x 1 RAMs.

The processor initiates data transfers between main memory and fast input/output devices.
16 16-bit words are then transmitted without disturbing the processor data paths in about
1.68 JLs (28 cycles). New references can be initiated every 8 cycles, so total bandwidth of
the memory, 533 mHz, is available for devices with enough buffering.

<!-- PDF page 9 / printed page 2 -->

## 2. Overview

Experience suggests that programmers will gradually develop a mental model something
like Figure 1; until this mental model is well established, it is probably desirable to

Read the fol/owing with Figure 1 in view.

Dorado has Processor, Control, Memory, IFU, and 10 controller sections.

10 controllers are independent of each other and of the other sec~ions-you will have to
understand a particular io controller iff you are going to write microcode that controls it.

The memory and IFU are "slaves" to the processor/control section. In most situations,
their external interface is simple relative to internal details of operation, and effective
programming is usually possible without detailed understanding.

However, programmers will have to understand the processor thoroughly because the
different parts of the processor are controlled directly by instruction fields, and most of the
processor will be used, even in a small program.

Programmers must also understand most of the control section, although fairly simple
assembly language contstructs are transformed into the complicated branch encodings
needed by Dorado, so detailed understanding of Dorado branching is not required.

### 2.1. Control

Dorado supports up to 16 independent tasks at the microcode level. Each task has its own
program counter (TPC) , and other commonly-used registers are also replicated on a per-
task basis. Tasks are scheduled automatically by the hardware in response to wakeup
requests, where task 15 is highest priority, task 0, lowest.

Emulator microcode runs entirely in task 0 (lowest priority); fault conditions normally
wakeup task 15, the "fault task" (highest priority). Other tasks are normally paired with io
devices that issue wakeup requests when they need service. Task switching, discussed in
"Control Section", is in most cases invisible to the programmer, because commonly-used
registers are duplicated for each task.

In this manual, "instruction" refers to a microinstruction in the control store, as opposed to
an opcode in the higher level language interpreted by a microprogram. The JCN field in an
instruction encodes a variety of jumps, calls, conditional jumps and calls, instruction
dispatches and returns for the current task.

### 2.2. Registers, Memories, and Data Paths

Tables 1, 2, and 3 describe memories, registers, and data paths in Dorado; these are
diagrammed in Figure 1. The first two tables below focus on a particular register or
memory and tell how it is used and where it connects; the third table focuses on particular
data paths and shows how they connect various parts of the machine.

<!-- PDF page 10 / printed page 3 -->

> **Table 1: Memories**

![Table 1: Memories](tables/table-01-memories.png)

IM       IM is a 4096-word x 34-bit (+ 2 parity) RAM used to store instructions. When written, the address is
taken from Link and data from B 16 bits at-a-time (1 extra bit and parity from RSTK field). When
read, the address is taken from Link, and data is delivered to Link 9 bits at-a-time. The read or
write is controlled by the JCN field and two or three low bits of RSTK.

ALUFM    ALUFM is a 16-word x 6-bit ALU control RAM addressed by the 4-bit ALUF field. Five ALUFM bits
specify 16 boolean or 5 arithmetic operations on A and B. One bit is the input carry for arithmetic
operations (modifiable by several functions). ALUFM[ALUF] is read onto Pd by the ALUFMEM
function or both read onto Pd and loaded from B by the ALUFMRW← function.

RM       RM is a 256-word x 16-bit (+ 2 parity) RAM used for general storage by all tasks. The normal
address is RBase[0:3]"RSTK[0:3]. Data can be read onto A or B and loaded from Pd or Md
without using FF.    Together with T, RM forms the input to the Shifter.

STK      STK is a 256-word x 16-bit (+ 2 parity) stack accessible only to the emulator, used instead of RM
when the BLOCK bit In the instruction is 1. Its address comes from StkP, modified by -4 to +3
under control of RSTK.

IFUM     IFUM is a 1024-word x 24-bit (+3 parity) decoding memory containing 256 words for each of four
instruction sets. The instruction set can be set by the InsSetOrEvent← function. The low 8 address
bits are normally an opcode fetched from the cache, but can be loaded from B. by the Brklns←
function to read or write IFUM itself. The IFUMLH← and IFUMRH← functions load, and the
B←IFUMLH' and B←IFUMRH' functions read different bits of IFUM. During normal operation IFUM
controls decoding of the stream of opcodes and operands fetched from memory' relative to BR 31,
the code base.

MAIN     Main storage consists of a 64-row x 4-column x 16-word virtual cache coupled with one to four
2S6k x 16-bit memory modules (using 16k-bit storage chips). The IFU and processor independently
access the cache, with IFU references deferring to the processor. The processor has two dissimilar
methods of reference, one primarily to the cache (with "misses" initiating main memory action) and
one directly to main memory (invalidating cache hits on writes, using dirty cache hits on reads).
Fetch←, Store←, IFetch←, LongFetch←, and PreFetch← are cache references. Md can be loaded
into T or RM (LC field), routed onto B (BSEL field), onto A (FF field), or used in a shift-and-mask
operation (ASEL and ALUF fields). IOFetch← and IOStore← (ASEL field) initiate a 16-word transfer
between an io device and memory without further processor interaction (using F!n or Fout bus).
Virtual addresses are transformed to absolute using the Map memory.            All references leave
information in the Pipe memory.

BR       A 32-word x 28-bit base register memory addressed by the MemBase register. The virtual address
for any memory reference is BR[MemBase] + Mar. BR is loaded from Mar by the Brlo←A and
BrHi←A functions and can be read indirectly onto B via the virtual address left in the Pipe after a
memory reference (PipeO and Pipe1 functions).

Pipe     The 16-entry x 6-word pipe contains trace information left by memory references. This information
includes the virtual address, map stuff, single-error and double-error information, cache control stuff,
task and subtask. It is automatically loaded during any memory reference and can be read onto B
by the PipeO. Pipe1. ...• PipeS' functions.

Map      The Map is a 16k or 64k-word x 19-bit (+ parity) memory used to transform virtual addesses to
absolute. Addressed by VA[10:23]. map entries contain 16 bits of real page. write protect. dirty.
and referenced bits. They can be written from B with Map← (ASEL) 'and read from the Pipe after
main storage references.

<!-- PDF page 11 / printed page 4 -->

> **Table 2: Registers**

![Table 2: Registers](tables/table-02-registers.png)

T*         16-bit (+ 2 parity) T sources either A (ASEL field or FA field with memory ops) or B (BSEL field), or
the Shifter (ASEL) and loads from either Pd or Md (lC field).

RBase     4-bit RBase"RSTK field forms addresses for RM. R8ase can be loaded from FF[4:7] or from
8[12:15] by the R8ase fo SC,RBase fo 8, or PointersfoB functions; it is read onto Pd[12:15]. by the
PdfoPointers function. RBase is loaded with 0 or 1 when the IFU dispatches to the first instruction
for an opcode.

StkP       The emulator uses STK instead of RM when the BLOCK bit is 1. a-bit StkP holds the address for
STK. The RSTK field is interpreted as an adjustment to StkP, which can be modified 4 to +3 in
conjunction with testing for overflow and underflow.    This mechanism implements the Mesa
evaluation stack. StkP can be loaded by the StkPfoB function and read onto Pd[8:15] by the
fo TIOA&StkP function (Stack overflow and underflow indicators are read into Pd[8:9] by the
PdfoPointers function.).

Q          16-bit Q is used as a shift register by multiply and divide. Q can be read onto A (FF field or FA
with Fetchfo or Store fo ) or 8 (BSEL field) and loaded from any B source except a constant (8SEl
and FF fields).     Functions implement Q Ish 1 and Q rsh 1.

Cnt        Cnt is a 16-bit counter that can be both decremented and tested for zero by a branch condition.
Cnt can be loaded from FF[4:7] with 1 to 16 or from 8 (FF field) and can be read onto Pd (FF).

TIOA      TIOA is an 8-bit io address register (see "Slow 10") loaded by the TIOAfoB function and read onto
Pd[O:7] with the PdfoTIOA&StkP function.        TIOA[5:7] may also be loaded from FF[5:7].

ShC        16-bit ShC controls the shifter-masker (see "Shifter"). RFfoA, WFfoA, and ShCfoB functions load
ShC in various ways.      ShC can be read onto Pd by the PdfoShC function.

Mem8ase Mem8ase is a 5-bit register addressing 8R for memory references. The MemBasefon functions load
it from FF[3:7]; the Mem8aseX fo n functions load it from O"MemBX[O:1}"FF[6:7]. The IFU loads
Mem8ase with a value between 0 and 3 relative to MemBX or with 34 to 37, as specified in IFUM,
prior to executing the first instruction of an opcode. Mem8ase is read onto Pd[3:7] by the
PdfoPointers function and loaded from 8[3:7] by tne PointersfoB and Mem8ase fo B functions.

Mem8X      Mem8X is a 2-bit register used like a stack pointer in conjunction with Mem8ase. The ideas behind
this are discussed in "Memory Section".

Link      16-bit Link holds subroutine return addresses, address modification for dispatches, IM address for IM
reads/writes, and data for TPC reads/writes. It can be read onto or loaded from B[0:15] by the
BfoLink or LinkfoB, BigBDispatchfoB, or 8Dispatch foB functions, or from CIA + 1 by CAlls and
RETURNs.

PC         16-bit PC contains the byte displacement of the next opcode relative to BR 31, the code base. The
IFU maintains this register, so only conditional jumps that don't jump and opcodes of type "pause"
have to load it with the PCFfoB function.           The BfoPCX' function reads PC.

TPC       TPC contains the address of the next instruction for each task. It is addressed from B[12:15] and
read/write control is in JCN. Data is read from/written into Link under control of the JCN field of
the instruction.

Mcr        Memory control register-disables parts of memory system for initialization and checkout.

<!-- PDF page 12 / printed page 5 -->

> **Table 3: Data Paths**

![Table 3: Data Paths](tables/table-03-data-paths.png)

A       The 16-bit high-true A bus (called "alua" in hardware drawings) may be driven from T, RM, STK, Q,
Id, Md, a small constant between 0 and 178 , or the shifter. It is also possible to 'or' the low-true
shifter output with one of th'3 other A sources. The A bus is totally inside the processor section,
not connected to any other sections of Dorado, and it is one of the two Alu inputs. The RF←A and
WF←A functions, which load ShC for subsequent shift operations, receive data from A.

Mar     The 16-bit Mar bus transmits the displacement for a memory reference from the processor or IFU
section to the memory section. The CFlags register, some bits of the Mcr register, and the BR
memory in the memory section are also loaded from Mar. The processor drives Mar only when it is
starting a reference or executing one of the functions between 120a and 1278 (Le., CFlags ← A' and
loadMcr[A,B] are in this group of functions); during other instructions, the IFU may use Mar to
initiate instruction fetches. Mar is driven low-true; when driven by the processor, it receives the
same data as are driven onto A (but the shifter cannot drive Mar).

B       The i6-bit B bus consists of one data path inside the processor section (called "alub" in hardware
drawings) and another on the backplane (called "Bmux" in hardware drawings); the IOB bus is
driven from Alub on Output operations, when it also is an extension of B. Alub and Bmux may be
directly driven high-true from registers inside the processor; alternatively, Bmux may be driven low
true from other sections, in which case the processor receives the data onto alub through inverters
(so the data appears high-true on alub). The BSEL field in an instruction can specify that either T,
RM/STK, Q, or Md sources B; other sources and destinations loaded from B are specified in the FF
field; BSEL and FF are used in combination to specify that a literal a-bit constant (in either the left
or right byte of the word with O's or 1's in the other byte) sources B. Alub is one of the two Alu
inputs. The processor computes odd byte parity on alub; Bmux and IOB destinations may store or
check the parity computed by the processor.

Pd      The Pd path ("~rocessor Qata") receives data from an a-input multiplexor whose inputs are the Alu
output, possibly shifted left or right one b~t on Alu shift functions or masked on a shifter operation,
io device input data, and the infrequently read registers in the processor section. Pd may be
written into the T register or the RM or STK memories.

Id      The Id path ("!FU Qata") is used to send arguments from the IFU to the processor for interpretation.
It can be routed onto A using ASEL (A←Id, Fetch←Id, Store←Id, or IFetch←RM/STK); alternatively,
the Tlsld or Rlsld functions can be used to replace data from T or from RM/STK by IFU
data-these functions provide a roundabout method of getting Id onto B.

Md      The Md path ("Memory Qata") moves data from the cache in the memory section into the
processor. The processor latches Md and can route it onto A or B, load it into T and RM/STK, or
use it in a shift-and-mask operation.

lOA     The lOA bus ("!nput~utput ~ddress") is driven from the TIOA register; it specifies the io device
affected by a Pd←Input 'or Output←B function.

IOB     The IOB bus ("!nput~utput !!us") is driven from alub on an Output←B function or received on Pd
by a Pd←Input function; it transmits data to or from an io device.

Fout    ("East output bus") transmits data from the error corrector to a fast output device.

Rn      ("East !!!.put bus") transmits data from a fast input device (Presently, there are no fast input
devices) to the syndrome generator.

Sout    ("~torage   output bus") transmits data from the syndrome generator to storage.

Sin     ("~torage   !!!.put bus") transmits data from storage to the error' corrector.

<!-- PDF page 13 / printed page 6 -->

### 2.3. Timing

The terminology used in discussing timing is as follows:

clock       The 30 ns (nominal) atomic time period of the machine. Clock period can be
controlled by the baseboard microcomputer or through the manifold system as
discussed in the "Dorado Debugging Interface" document. 1
cycle       The duration of instructions-two clocks or 60 ns except for instructions that
read/write IM or TPC.
to          The instant at which MIR (Micro!nstruction Begister) is loaded-the beginning of
a cycle.
t1          The next instant after to-always one clock later.
t2          The instant following t 1-one clock after t1 except for instructions that
read/write IM or TPC. Additional clocks intervening for these special cases,
which only affect the control section, are denoted by t 1a, t 1b , etc.
t3, t4      Subsequent instants for a instruction. ~ of the previous instruction coincides
with t1 of the current instruction; t4 with t 2.
First half cycle
The interval from to to t1 (or t2 to ~).
Second half cycle
The interval from t1 to t2 (or t3 to t4 ).

As implied by this terminology, Dorado initiates a new instruction every cycle. Instructions
are pipelined, requiring a total of three cycles for execution. Timing for a typical
instruction is shown in Figure 7. At t_2' the next instruction address is determined and
instruction fetch from IM begins; at to' the instruction is loaded into MIR from IM. During
the first half cycle, the selected register is read from RM or STK, and at t1 is loaded into a
register. During the next two clocks (t1-t3), addition is performed in the ALU; at t3 the
result is loaded into a register for writing into RM/STK or T. During the final clock, RM is
written.

Since a new instruction begins before the previous one finishes, paths exist to bypass the
register being written if the following instruction specifies it as a source (These paths,
inaccessible to the programmer, are not shown in Figure 1).

Most registers load from 8 at t3 (i.e., at the mid-clock of the cycle following the load
instruction).   These may source 8 in the instruction after they are loaded. The load
information and data are pipelined into the next cycle, as described above. Registers
loaded at t2 may be used during the first half-cycle of the following instruction. Usually,
this type of register is used for some type of control information, since control registers are
normally clocked at to (= t2 of previous instruction), data-oriented registers at t1 (t3 of
previous instruction).

> **Table 4: Load Timing**

![Table 4: Load Timing](tables/table-04-load-timing.png)

<!-- PDF page 14 / printed page 7 -->
<!-- PDF page 15 / printed page 8 -->
### 2.4. Instruction Fields

The 34-bit instruction is divided into the following fields:

> **Table 5: Instruction Fields**

![Table 5: Instruction Fields](tables/table-05-instruction-fields.png)

The above instruction layout emphasizes compactness at the expense of programming
flexibility. The following comments explain some of these tradeoffs

1. The RSTK field specifies only four of the eight address bits needed for addressing RM.
The other four are taken from the' RBase register (loaded by a function). In the emulator
task, BLOCK causes STK to be used instead of RM, and RSTK is decoded to cause
modifications of StkP.

2. ALUF addresses the 16-word ALUFM memory in which 16 of 26-odd useful ALU
operations are stored. For the shift operation decode of ASEL, the first three bits of ALUF
select the kind of shift, while the ALUFM address is forced to 168 or' 178 ,

3. BSEL decodes the most common data sources for B. Less common B sources are
selected by FF, and then BSEL encodes one of several destinations for the source.

4. ASEL specifies the source and destination for A. The default source is the RM address
selected by RSTK. Four ASEL decodes specify the most common memory operations,
where the virtual address is BR[MemBase] + A. These decodes consume the two leading
bits of FF to specify alternate sources (T or Id) or less frequent memory operations. The
remaining four ASEL decodes select alternate sources T, Id, or the shifter, where the shifter
decodes work in" combination with ALUF, as discussed later.

5.   LC specifies loading of RM/STK and T from Pd and Md.

<!-- PDF page 16 / printed page 9 -->

6. FF is the catch-all field in which operations or data not otherwise specifiable can be
encoded. Operations encoded in FF are called "functions". There are five ways FF is
used:
a. To extend the branch address encoded in JCN (long goto, long call).
b. To form a constant on B as selected by BSEL.
c. To specify one of 64 common functions and branch conditions while the two leading bits modify the
memory reference operation specified in ASEL.
d. To specify one of 256 functions and branch conditions, some of which use low bits of FF as literal
values.
e. As a shift control value when ASEL decodes to "shift" and BSEL to a constant.

When FF is used as a function, it sometimes modifies the interpretation of other fields in
the instruction. For example:
a. 16 FF decodes modify RM write address bits which would otherwise have come from RSTK or StkP.
b. 16 FF decodes modify RM write-address bits which would otherwise come from RBase.
c. 16 FF decodes select less common B sources, causing ASEL to encode a destination rather than a
source for B.

7. JCN (in conjunction with current address) encodes the next instruction address as
follows:
a. One of 64 global Calls.
b. One of 60 local Gotos.
c. One of 4 local Calls.
d. One of 14 local conditional branches with 7 branch conditions.
e. One of 16 long Gotos/Calls (use FF field for rest of address).
f. One of 4 IFU jumps for next opcode (high 10 address bits from IFU).
g. Return.
h. TPC read/write.
i. IM read/write (Use low bits of RSTK also).

8. PO and P1 are odd parity on the left and right halves of IM. When wrong, these give
rise to error signals (see "Theory of Operations") which stop the machine after
(unfortunately) the instruction with bad parity has been executed.     The artifice of
deliberately loading both parity bits incorrectly is used to implement breakpoints.

### 2.6. Notation

The notation used in referring to fields in the instruction is that the left-most bit of the field
is denoted as O. Hence, the fields in the instruction are as follows: RSTK[0:3], ALUF[O:3],
BSEL[O:2], LC[O:2], ASEL[O:2], BLOCK[O], FF[O:7], JCN[O:7].

The BLOCK bit is also called StackSelect, for its use in choosing STK instead of RM for the
emulator task.

<!-- PDF page 17 / printed page 10 -->

## 3. Processor Section

The processor section implements most registers accessible to the programmer and
decodes all instruction fields except JCN. The FF field of the instruction is also decoded
by the control, memory, and IFU sections.

Read this chapter with Figure 1 in front of you.

The processor section contains the Q, ShC, Cnt, StkP, and MemBX registers, the T, RBase,
MemBase, and TIOA task-specific registers, and the ALUFM, RM, and STK memories. It
contains the arithmetic and logic unit (ALU) and the shifter.

The processor communicates with the control, memory, and IFU sections via B; with io
devices via the IOB bus. It exports MemBase and Mar to the memory system for
addressing, lOA to devices for io addressing, and branch conditions to the control section.
It imports Md from the memory system and Id' from the IFU.

### 3.1. RM and STK Memories, RBase and StkP Registers

RM ("Register Memory," sometimes called "R") is the memory most easily. available to
microprograms; it stores 256 words x 16 data bits with odd parity on each byte of data.
RM is read at to and latched at t 1 • Data may be routed to A, B, or the shifter, and branch
conditions (see "Control Section") test the sign bit (R<O) and low bit (R Odd). RM may be
written between t3 and t4 with data from Md or Pd.

The RM read address is RBase[0:3]IlRSTK[0:3]. For io tasks SubTask[0:1] (discussed in
"Slow 10") are or'ed with RBase[2:3]. Each task can thus select from 16 RM registers in
the block pointed to by RBase.

Normally, this read address is also used for the write part of the instruction (if any).
However, two groups of FF decodes discussed below modify the write address.

The RBase'-SC function loads RBase with FF[4:7], selecting any block of 16 registers;
RBase.-B loads RBase from B[12:15]; Pointers'-B loads RBase from B[12:15] while also
loading MemBase from B[3:7] (Previous RBase value is used for both the read and write
portions of the instruction.). The IFU initializes the emulator task's RBase to 0 or 1 before
dispatching to the first instruction of an opcode.

The STK memory (sometimes called "stack") is accessible only to the emulator (task 0).
Since the emulator cannot block, the instruction bit interpreted as BLOCK for io tasks is
instead interpreted as StackSelect; when StackSelect is 1, RM is disabled and STK used
instead. Like RM, STK stores 256 words x 16 data bits with odd parity on each byte of
data. STK is addressed by the 8-bit StkP register, and RSTK controls the adjustment of
StkP; StkP may be decremented or incremented by any value between - 4 and + 3.

Unadjusted StkP is always the read address and normally the write address, but the
ModStkPBeforeW FF decode forces adjusted StkP to be used for the write. STK is divided
into four separate regions, each 1008 words long. Valid addresses are 1 to 778 within each
region. That is, StkP[0:1] select the region, stack overflow occurs at the onset of a
instruction that would increment StkP[2:7] > 778 , and underflow occurs when location 0 is

<!-- PDF page 18 / printed page 11 -->

either read or written or when StkP[2:7] is decremented below O.

StkP[2:7] are initialized to 0, denoting the empty stack. A push could do StkP←StkP + 1 and
write in one instruction. A pop does StkP←StkP -1, and the item being popped off can be
referenced in the same instruction if desired.

> **Table 6: RSTK Decodes for Stack Operations**

![Table 6: RSTK Decodes for Stack Operations](tables/table-06-rstk-decodes.png)

**RSTK[0]:** `0` = no underflow on `StkP = 0` at start or end; `1` = underflow when `StkP` originally 0 or finally 0.

| RSTK[1:3] | Meaning |
|:--:|:--|
| 0 | no `StkP` change |
| 1 | `StkP←StkP+1` |
| 2 | `StkP←StkP+2` |
| 3 | `StkP←StkP+3` |
| 4 | `StkP←StkP−4` |
| 5 | `StkP←StkP−3` |
| 6 | `StkP←StkP−2` |
| 7 | `StkP←StkP−1` |

RSTK[O]       o = no underflow on StkP = 0 at start or end
1 =   underflow when StkP originally 0 or finally O.

RSTK[1:3]     Meaning

o          no StkP change
1          StkP←StkP + 1
2          StkP←StkP + 2
3          StkP←StkP +3
4          StkP←StkP - 4
5          StkP←StkP - 3
6          StkP←StkP - 2
7          StkP←StkP -1

In other words, RSTK[1 :3] treated as a signed number are added to StkP[2:7] (StkP[0:1]
don't change.). In the emulator, an attempt to underflow or overflow the stack generates
the signal StkError:                                                                   .

StkError = (BLOCK eq 1) & Emulator &
[(StkP[2:7] + RSTK[1 :3]) < 0) % «StkP[2:7] + RSTK[1 :3]) > n 8) %
«RSTK[O] eq 1) & {(StkP[2:7] eq 0) % «StkP[2:7] + RSTK[1:3]) eq 0)))]

StkError generates HOLD and wakes up the fault task (task 15) to deal with the situation,
so the instruction causing StkError has not been executed when the fault task runs.
StkUnd and StkOvf are remembered in f1ipflops read by the Pd←Pointers function. These
get cleared (i.e., recomputed) when the next stack operation is executed by the emulator.
The fault task can read them to decide whether stack underflow or overfl.ow action is
necessary.

Interpretation of underflow: StkP eq 0 denotes the empty stack. A stack adjustment may
occur either by itself or with a read or write stack reference. StkP originally equal 0
underflows if the top of stack is read or written; decrementing StkP below 0 is always an
underflow error; StkP equal 0 after modification underflows iff writing at the modified
address. Consequently, the assembler sets RSTK[O] equal 1 for a stack reference only
when either reading STK and incrementing the pointer or writing at the modified address
and decrementing the pointer.

In other words, the microassembler must tell the hardware when to make the StkP equal 0
underflow checks, and it must do this correctly when the ModStkP8eforeW FF decode is
used.

StkP can be loaded from 8[8:15] using the StkP←8 function; however, this is illegal in
conjunction with a STK read or write in the same instruction (e.g., T ←Stack, StkP← T leaves
StkP unchanged).

<!-- PDF page 19 / printed page 12 -->

StkP is saved at t2 of an instruction dispatched to by the IFU. The saved value may be
reloaded into StkP at t2 by the RestoreStkP function; RestoreStkP is illegal in conjunction
with a STK read or write in the same instruction.

RestoreStkP is useful only if ope odes are restarted after servicing map faults. However, we are also
arranging for the IFU state, branch conditions, etc. of an interrupted ope ode to be readable and
reproducible, so that it will be possible to simply contInue from the instruction that faulted.
RestoreStkP will be useless if the continue-method of restarting is adopted.

The opcode-restart method effectively prevents use of the IFU entry vector scheme discussed in "IFU
Section," degrading performance perhaps 2%, so it is desirable to continue from rather than restart
from faults. Also, complicated opcodes may require special-case code in the fault handler before
opcode restart is possible, so continuing from the instruction that faulted is likely to be simpler
overall.

Two groups of FF decodes change the RM address for the write portion of an instruction.

The first group of 16 FF decodes forces the write address to come from
RBase[0:3]"FF[4:7]. This allows different registers in the same group of 16 to be used for
the read and write portions of the instruction, or allows STK[StkP] to be used for the read
portion and any of the 16 registers pointed' to by RBase in the write portion.

The second group of 16 FF decodes forces the top four write address bits to come from
FF[4:7]. The complete RM write address becomes FF[4:7]"RSTK[0:3]. This allows an
arbitrary RM address to be written without having to load RBase in a previous instruction.
Alternatively, if the i'th register in a group of 16 is read from RM, it permits the i'th register
in a different group of 16 to be written in the same instruction. In conjunction with a read
of STK, RSTK[0:3] will encode the StkP modification, and whatever RM word this happens
to point to will be written (Programmers will have to struggle to use this with a STK read.).
Note: SubTask does not affect the write address for these functions.                   '

Note that there is no way to read RM and write STK in one instruction.

The Risld FF decode causes Id to be substituted for RM/STK in the A, B, or shifter
multiplexing.

There are branch conditions to test R[O] (R(O) and R[15] (R odd). These branch conditions
are unaffected by the Risld FF decode; actual data from RM/STK is tested.

### 3.2. Cnt Register

The 16-bit ent register is provided for use as a loop counter. Since it is not task-specific,
io tasks must save and restore it.

ent can be decremented and tested for 0 by the ent =0& - 1 branch condition; loaded from
B[0:15] or from small constants 1 to 16 (FF decodes), and read onto the Pd path (into T or
RM/STK) by an FF decode.

<!-- PDF page 20 / printed page 13 -->

### 3.3. Q Register

The 16-bit Q register is provided primarily for use as a shift register with multiply and
divide, but will probably be used more widely by the emulator. Since it is not task-specific,
io tasks must save and restore it.

Q can be read onto B (BSEL) or onto A (FF); it can be loaded from B (FF) and when FF
specifies an external B source in the memory, ifu, or control sections, it can also be loaded
from B (BSEL). Q can be left-shifted or right-shifted one (bringing 0 into the vacant bit) by
two FF decodes.

### 3.4. T Register

The 16-bit T register is the primary register for data manipulation in the processor. Since it
is task-specific io tasks do not have to save and restore it. T can be read onto B (BSEL) or
A (ASEL); it can be loaded from Pd or Md (LC).

### 3.5. BSEL: B Multiplexor Select

BSEL normally selects one of the "internal" processor sources for B, as shown in the
"Primary" column in the table below (Note that although Md originates in the memory
section, it is latched by the processor and appears as an internal B source.). However, the
FF field can be used to substitute some other source external to the processor-there are
many "external" sources in the control, IFU, and memory sections, and the codes for these
are given in Table 11. When an external source is specified, then BSEL instead encodes
the destination for B, as shown in the "External" column of the table below.

The sources selected by BSEL are:

> **Table 7: BSEL Decodes**

![Table 7: BSEL Decodes](tables/table-07-bsel-decodes.png)

| BSEL | Primary | With External Source |
|:--:|:--|:--|
| 0 | `Md` | — |
| 1 | `RM/STK` | — |
| 2 | `T` | — |
| 3 | `Q` | `Q←B` * |
| 4 | `0,,FF` | Inapplicable (FF not available to encode an external source) |
| 5 | `377₈,,FF` | Inapplicable |
| 6 | `FF,,0` | Inapplicable |
| 7 | `FF,,377₈` | Inapplicable |

\* *BSEL decode for `Q←B` is needed when initializing Dorado from the baseboard or Alto. (`,,` is the concatenation operator; `377₈` is octal.)*

ALUFM contents may be unknown, and data from the Alto is transmitted via the B←Link FF decode,
some other field is needed to encode a destination that can then be routed into ALUFM.

The values selected by BSEL =4 7 are 16-bit constants obtained by concatenating the 8-bit
FF field with zeroes or ones. When this is done, normal effects of functions are disabled,
so external B sources are impossible. In conjunction with a shift operation on A, BSEL =
4 to 7 will cause the shifter controls to come directly from FF rather than from ShC as

<!-- PDF page 21 / printed page 14 -->

discussed in "Shifter"; the Q-register sources B when an FF-controlled shift is carried out.

The Tisld and Risld FF decodes may be used with the B←T or B←RM/STK BSEL decodes,
respectively, to accomplish B←Id.

The "External" decode of BSEL applies with Link, DBuf, PipeO-PipeS, Faultlnfo, PCX,
Declo, DecHi, and other functions that source B on the backpanel, as selected by the FF
decode. For these external sources, BSEL is interpreted as the destination for B rather
than the source.

Note: When the memory or control section sources the external B bus, it is illegal to
execute arithmetic alu operations; these sources are not electrically stable soon enough to
permit the extra 10 ns required for carry propagation. But: if you are sure carries will not
propagate into the high 8 bits of AlU result, then the hardware is fast enough.

However: Arithmetic is permitted when the IFU sources the external B bus, provided the
previous instruction was not one of the slow B sources from the memory or control
sections. This permits {ld)-(PCX')-1, common in emulator microcode.
This implies that an io task must never block on an instruction that reads B from a slow external
source.

Hardware Implementation

The processor's internal version of B, called Alub, is driven by a 4-input multiplexor when sourced from within
the processor; in this case an identical multiplexor drives the external bus, called Bmux (high-true). When the
B source is external, both of these multiplexors are disabled, and the backpanel Bmux (low true) is inverted
through a gate onto Alub.        The multiplexor arrangement is shown in Figure 3.

The IFU section is on/off of Bmux by 11 +6 ns and the processor section is off by t1 + 7 ns, but the memory
and control sections are not on/off until t1 + 16 ns; hence, a slow Bmux source in the previous instruction
prevents Bmux from stabilizing until t1 + 16 ns of the current instruction, allowing insufficient time to propagate
Bmux onto Alub and finish carry propagation. However, because Bmux is gated onto Alub, and the gate s~uts
off quickly, arithmetic on internal Alub sources is always permissible.

Bmux sources in this manual are given high or low-true names that agree with the way signals appear on Alub.
For external sources this is inverted with respect to the sense of these signals on Bmux. However, because
external sources cannot feed external destinations (no way to encode this in an instruction), the signal
inversion is invisible to programmers.

<!-- PDF page 22 / printed page 15 -->

### 3.6. ASEL: A Source/Destination Control

The AMux drives the A input to the ALU, and is the data source for the read-field (RF←)
and write field (WF←) methods of loading She. The shifter also drives A, in which case the
AMux is usually disabled.

A copy of the AMux drives the backplane Mar bus on processor memory references. The
IFU may also drive Mar, when the processor isn't using it.

The three-bit ASEL field controls the source and destination for A as follows:

> **Table 8: ASEL Decodes**

![Table 8: ASEL Decodes](tables/table-08-asel-decodes.png)

**Table 8a — ASEL Decodes When FF is ok\*** (the three-bit ASEL field controls the source/destination for A)

| ASEL | FF[0:1] | Meaning |
|:--:|:--:|:--|
| 0 | 0 | `PreFetch←RM/STK` |
| 0 | 1 | `Map←RM/STK` (emulator or fault task) *-or-* `IOFetch←RM` (io task) |
| 0 | 2 | `LongFetch←RM/STK` |
| 0 | 3 | `Store←RM/STK` |
| 1 | 0 | `DummyRef←RM/STK` |
| 1 | 1 | `Flush←RM/STK` (emulator or fault task) *-or-* `IOStore←RM` (io task) |
| 1 | 2 | `IFetch←RM/STK` |
| 1 | 3 | `Fetch←RM/STK` |
| 2 | 0 | `Store←Md` |
| 2 | 1 | `Store←Id` |
| 2 | 2 | `Store←Q` |
| 2 | 3 | `Store←T` |
| 3 | 0 | `Fetch←Md` |
| 3 | 1 | `Fetch←Id` |
| 3 | 2 | `Fetch←Q` |
| 3 | 3 | `Fetch←T` |
| 4 | — | `A←RM/STK` |
| 5 | — | `A←Id` — see "Instruction Fetch Unit" |
| 6 | — | `A←T` |
| 7 | — | Shift operation — see "Shifter" (uses ALUF) |

**Table 8b — ASEL Decodes When FF is not ok\***

| ASEL | Meaning |
|:--:|:--|
| 0 | `Store←RM/STK` |
| 1 | `Fetch←RM/STK` |
| 2 | `Store←T` |
| 3 | `Fetch←T` |
| 4 | `A←RM/STK` |
| 5 | `A←Id` |
| 6 | `A←T` |
| 7 | Shift operation — see "Shifter" (uses ALUF) |

\* *FF is ok when not used in a long goto, long call, as a BSEL constant, or in an FF-controlled shift.*

Map←RM/STK (emulator or fault task) or IOFetch←RM (ia task)
2         long Fetch ←RM/STK
3         Store←RM/STK
o         DummyRef←RM/STK
Flush←RM/STK (emulator or fault task) or IOStore←RM (io task)
2         IFetch←RM/STK
3         Fetch←RM/STK
2        o         Store←Md
1         Store←Id
2         Store←Q
3         Store←T
3        o         Fetch←Md
1         Fetch←Id
2         Fetch←Q
3         Fetch←T
4                  A←RM/STK
5                  A←Id--see "Instruction Fetch Unit"
6                  A←T
7                  Shift operation-see "Shifter" (uses AlUF)

When FF is ok and ASEL = 0 to 3, the decoding of FF as a function is forced to be in the
range 0 to 63. In other words, FF[O:1], stolen to modify the memory operation on A, do not
participate in the FF decode. Hence, only functions 0 to 63 can be used in the same
instruction with a memory reference.

<!-- PDF page 23 / printed page 16 -->

In the above tables, each instance where the source for A is RM/STK can be overruled by
one of the 4 FF decodes for A sources or the FF decodes that put FF[4:7] on A. These FF
decodes are illegal with the ASEL or ASEL-FF[0:1] values that select Id or T, and the
source for A is undefined when this restriction is violated.

The notation "Fetch ← A", "Store← A", etc. in the above table is compatible with the
microlanguage. These routing expressions mean, for example, that the displacement
originating on A is routed onto the Mar bus on the backplane, added to BR[MemBase] in
the memory section and loaded into the memory address register. Then the Fetch, Store,
etc. is started as detailed in "Memory Section".

ASEL does a pretty thorough job of encoding possible actions on A: Store← and Fetch←
references take the address from RM/STK, T, Md, Id, or Q; other references take the
address from RM/STK; LongFetch ← takes the low 16 bits of address from RM/STK and
high 8 bits from B.

The FF field can be used to select any of the following sources:

FF[4:7] (small constant)
RM/STK
Q
T
Md

These functions are illegal except on shifts (ASEL =7) or when the source otherwise
selected would be RM/STK (ASEL =0, 1, or 4). On shifts these functions cause the A
source to be wire-or'ed with the shifter output (otherwise the A source would be disabled);
with references, these functions overrule RM/STK as the source.

Hardware Implementation

A is driven by a 4-input multiplexor as shown in Figure 3. A similar arrangement drives Mar, which is disabled
except on memory references or when one of the 8 FF decodes that use Mar is executed; the IFU may use
Mar when the processor does not. The 4-input multiplexors are usually disabled on shifts, wl;1ich OR onto A
. independently.

However, the A multiplexor is not disabled when the source for A is encoded in FF, so it is possible to OR any
A input except Id with the (complemented) shifter data-this is useful for BitBlt and other complicated uses of
the shifter. Since shifter data on A is low true, and since the normal ALU operation is NOT A on shifts, the
effect of enabling both the shifter and the normal A multiplexor is [Shiftdata and not A].

<!-- PDF page 24 / printed page 17 -->

### 3.7. ALUF, ALU Operations

The 4-bit ALUF field controls the ALU operation_ It addresses a RAM (ALUFM) containing
control for the MC10181 ALU chips.

ALUFM is 8-bits wide, of which 6 bits are used. ALUFM[O] controls the carry-in for
arithmetic ALU operations. It is a "don't care" for the 16 logical ALU operations. The
XorSavedCarry function causes the saved carry-out of a previous operation to be xor'ed
with this bit. The XorCarry function complement~ the value from ALUFM. ALUFM[3:7]
select the ALU function performed as below. The carry-out Uask-specific) changes
whenever an arithmetic operation is performed in the ALU unless explicitly disabled by the
FreezeBC function (freeze branch conditions).

The Carry20 function forces the bit 12 carry-in to one. Assuming that this carry-in would
otherwise have been zero, then this function adds 20s to the (arithmetic) ALU output.
Adding 20s is expected to be useful because the cache, fast input bus, and fast output bus
deal with 20s-word munches.

The table below shows the logical and (useful) arithmetic ALU operations.

> **Table 9: ALUFM Control Values**

![Table 9: ALUFM Control Values](tables/table-09-alufm-control-values.png)

Field values are octal. `*` = system microcode can count on these being defined; `**` = emulator task can count on these being defined.

| Logical | | Arithmetic (No Carry) | | Arithmetic (With Carry) | |
|:--:|:--|:--:|:--|:--:|:--|
| \*1 | `NOT A` | \*0 | `A` | \*0 | `A+1` |
| 3 | `(NOT A) OR (NOT B)` | \*\*6 | `2*A` | 6 | `2*A+1` |
| 5 | `(NOT A) OR (B)` | \*14 | `A+B` | \*14 | `A+B+1` |
| 7 | All-ones output | \*22 | `A−B−1` | \*22 | `A−B` |
| 11 | `(NOT A) AND (NOT B)` | \*36 | `A−1` | 36 | `A` |
| \*13 | `NOT B` | | | | |
| 15 | `A XNOR B` (assembler: `EQV`, `=` are synonyms) | | | | |
| 17 | `A OR (NOT B)` | | | | |
| 21 | `(NOT A) AND B` | | | | |
| \*23 | `A XOR B` (assembler: `#` is synonym) | | | | |
| \*25 | `B` | | | | |
| \*27 | `A OR B` | | | | |
| 31 | All-zeroes output | | | | |
| \*\*33 | `A AND (NOT B)` | | | | |
| \*35 | `A AND B` | | | | |
| 37 | `A` | | | | |

On a barrel shift (selected by ASEL =7), the first three ALUFM address bits are forced to 1
(ALUF[0:2] selects the I<.ind of shift in this case). The intent of this arrangement is that
ALUFM(16a] selects the "NOT A" ALU operation. Nearly aI/ shifter operations use this ALU
function to route shifter output through the ALU. ALUFM[17a] is loaded with assorted
controls (i.e., used as a variable) by BitBlt or other opcodes that do more complicated
things.

ALUFM can be read onto Pd by the ALUFMEM function or both loaded from B and read
onto Pd by the ALUFMEMRW function ..

<!-- PDF page 25 / printed page 18 -->

External B sources from the IFU and internal sources are ready in time for arithmetic, but
external sources from the memory and control sections are not (see the earlier section on
"BSEL: B Multiplexor Select"). Internal A sources except shifter are ready in time for
arithmetic. Unless explicitly disabled by the FreezeBC function, the branch conditions
ALU<O, ALU =0, Carry' (ALU carry out'), and Overflow are available for testing on the
control card at t3'

The Overflow branch condition, defined as carry-out from bit 0 unequal to carry-out from
bit 1, is true iff a signed arithmetic operation yields an incorrect result.

Normally, the ALU is routed directly onto Pd, and Pd is then written into either T or
RM/STK. However, several functions route ALU output shifted left or right 1 position onto
Pd. Note that the ALU output of this instruction are used (not the previous one) and -that
ALUcarry is undefined on a logical ALU operation. The right shifts are:

ALU rsh 1      (0 onto Pd[O])
ALU rcy 1      (ALU[15) onto Pd[O))
ALU arsh 1     (ALU[O] onto Pd[O) preserving. the sign)
ALU brsh 1     (ALUcarry onto Pd[O])
Multiply       (ALUcarry onto Pd[O]).

The left shifts are:

ALU Ish 1      (0 onto Pd[15»
ALU Icy 1      (ALU[O] onto Pd[15))
Divide         (0[0) onto Pd[15»
CDivide        (0[0) onto Pd[15]).

Multiply, Divide, and CDivide have other effects as well discussed later.

Note: The barrel shifter discussed in the "Shifter" section also use the Pd multiplexor for
masking, so it is illegal to combine barrel shifts and ALU shifts in the same instruction.

Note: ALU<O, ALU =0, Carry', and Overflow branch conditions test the ALU .output of the
previous instruction executed by the task and any shifting or masking that takes place in
the Pd input multiplexor does not affect the result of these branch conditions.

Note:  The value of Carry' and Overflow change only on arithmetic ALU operations.
However, ALUt-A may be either an arithmetic or a logical operation; in order to use
XorCarry with ALUt-A, we will probably use the arithmetic form of ALUt-A, but the
consequence of this is that Carry' will change on ALUt-A. Programmers will have to be
wary of this.

Note:   Overflow is implemented correctly only for the A + B, A + B + 1, A-B, and A-B-1
operations; other arithmetic ALU operations (A + 1, A-1, 2A, 2A + 1, etc.) may modify the
branch condition erroneously.

<!-- PDF page 26 / printed page 19 -->

### 3.8. LC: Load Control for RM and T

This field controls the loading and source selection for the RM/STK memory and T register.
The eight combinations are:

> **Table 10: LC Decodes**

![Table 10: LC Decodes](tables/table-10-lc-decodes.png)

| LC | Meaning |
|:--:|:--|
| 0 | No Action |
| 1 | `T←Pd` |
| 2 | `T←Md, RM/STK←Pd` |
| 3 | `T←Md` |
| 4 | `RM/STK←Md` |
| 5 | `T←Pd, RM/STK←Md` |
| 6 | `RM/STK←Pd` |
| 7 | `T←Pd, RM/STK←Pd` |

The only missing combination is `T←Md, RM/STK←Md`, accomplished by combining `LC=5` with the `TgetsMd` FF decode (illegal with other LC decodes).

The only missing combination is T←Md, RM/STK←Md. T←Md, RM/STK←Md can be
accomplished by combining an LC value of 5 with the TgetsMd FF decode. It is illegal to
use TgetsMd with other LC decodes.

### 3.9. FF: Special Function

This field is the catch-all for functions not otherwise encoded in the instruction. For
consistency with the hardware implementation, the 8-bit FF field is shown below as a two
bit field FA (= FF[0:1]) and two 3-bit fields, FB (= FF[2:4]) and FC (= FF[5:7]). Field
values are given in octal.

The FF field is interpreted as a function iff:

(BSEL not selecting a constant) and
JCN does not select a "long" goto or call

When ASEL selects one of the memory references, the FF decode is forced to be that of
FA =0 because the FA field specifies the source for A or alternate memory reference in this
case.

The decoding assignments have been made with the following considerations:

Functions that source the external BMux are grouped for easy decode of the signal
that turns off the processor's B-multiplexors.

Operations that might be useful in conjunction with a memory reference are put in
the first 64 decodes (FA =0) since FA is decoded as zero on memory references.

Functions decoded by different hardware sections are arranged in groups to
reduce decoding logic.

<!-- PDF page 27 / printed page 20 -->

> **Table 11: FF Decodes**

![Table 11: FF Decodes (part 1)](tables/table-11-ff-decodes-1.png)

![Table 11: FF Decodes (part 2)](tables/table-11-ff-decodes-2.png)

![Table 11: FF Decodes (part 3)](tables/table-11-ff-decodes-3.png)

<!-- PDF page 30 / printed page 23 -->

### 3.10. Multiply and Divide

The Multiply, Divide, and CDivide functions operate on unsigned 16-bit operands. Unsigned
rather than signed operands are used so that the algorithms will work properly on the extra
words of multiple-precision numbers.

The actions caused by these functions are as follows:

Multiply:
Result .. ALUCarry .. ALU/2
Q .. ALU[15] .. 0/2
Next branch address .. whatever it is OR 2 if 0[14] is 1.

Divide, CDivide:
Result .. 2* ALU ..Q[OO]
Q .. 2*0.. ALUCarry -or- 2*Q.. ALUCarry'

Complete examples for Multiply and Divide subroutines are given in the microassembler
document. The inner loop time is 1 cycle/bit for multiply and 2 cycles/bit for divide.

### 3.11. Shifter

See Figure 4.

Dorado contains a 32-bit barrel shifter and associated logic optimized for field extraction,
field insertion and the BitBlt instruction.

The shifter is controlled by a 16-bit register ShC. To perform a shift operation, She is
loaded in one of three ways discussed below with 14 bits of control information, and one of
eight shift-and-mask operations is then executed in a subsequent instruction. Alternatively,
(a limited selection of) shift controls may be specified in FF and BSEL concurrent with a
shift; in this case, She is not modified. ASEL =7 causes a shift and ALUF[0:2] select the
kind of masking.

The execution of a shift instruction (after ShC has been loaded in a previous instruction)
proceeds as follows:

ShC[2] selects between T and RM/STK for the left most 16 bits input to the shifter;
ShC[3] selects between T and RM/STK forthe rig ht most 16 bits. Using the Risld
or Tisld FF decode in the same instruction allows Id to replace either T or RM/STK
in the shift. This 32-bit quantity is then left-cycled by the number of positions (0-
15) given by ShC[4:7]. When ShC[2] and ShC[3] are both 1, then the shifter left
cycles T; when both 0, RM/STK. In these cases it operates as a 16-bit cycler.
When ShC[2] and ShC[31 are loaded with complementary values, then it left-cycles
the 32-bit quantity R..T or T.. A.

The low order 16 bits of shifted data are placed complemented on A by the shift,
and normal A source is disabled (except when the source for A is encoded in
FF-see the ASEL section).

ALUF[0:2] select one of eight mask operations (see below) and the first three

<!-- PDF page 31 / printed page 24 -->

ALUFM address bits are forced to 1, so that the ALU operation in either ALUFM
168 or ALUFM 178 can be performed. This must be a logical ALU operation using
the shifted data on A and data on B because there is insufficient time to propagate
carries for an arithmetic operation. The intent is that ALUFM 168 contain the
control for the "NOT A" ALU operation normally desired, while ALUFM 178 is used
by BitBlt and other opcodes that need computed ALU operations.

ALU output passes to the masking logic. The mask operation determines which of .
two independent masks in ShC are applied to the data. LMask contains 0 to 15
ones starting at bit 0, RMask 0 to 15 ones starting at bit 15. The masked area(s)
of ALU output corresponding to 1's in the mask are replaced either with zeroes or
with corresponding bits from Md according to the shift-and-mask function selected.
Replace-with-Md generates HOLD if Md isn't ready yet, and the timing for this is
the same as Md onto B (i.e., data is never ready sooner than the second
instruction after the Fetch←).

Masked data is routed onto Pd, then sent to the destination specified by LC.

Note: The Pd input multiplexor is used to carry out masking, so it is illegal to
combine a shifter operation with an ALU shift in the same instruction.

Three functions load ShC: RF←A and WF←A treat A[8:15] as a Mesa field descriptor and
transform the bits appropriately before loading ShC; they also load ShC[2:3] from A[2:3].
ShC←B allows an arbitrary value to be placed in ShC (used by BitBlt).

Microcode for the Mesa RF (Read Field) and WF {Write Field} opcode is shown as an
example of the use of the shifter. In these examples. a and 13 are the two operand bytes
for the opcode. as discussed in "Instruction Fetch Unit." RF and WF both take a pointer
from the top of the stack and add a to it as a displacement. RF fetches the word, and
pushes the field specified by 13 onto the stack; WF fetches the word, and inserts a field
from the rightmost bits of the word in the second position of the stack into it, then restores
the word to memory.

RF:       IFetch←Stack, TIsld;              Calculate the pointer.     a replaces BR[MemBasej (MOS);
this value is then added to Stack to compute the
'"address for the pointer.
Stack←Md. RF←Id;                 *IFU supplies p.    the field descriptor
IFUJump[Oj. Stack←ShiftLMask;     '"Right-justify & mask the field, IFU to next instruction

WF:       T←(IFetch←Stack&1) + T. TIsld; '"Start fetch of word containing field
WF←Id. RTemp ←T;                *IFU supplies  p.  the field descriptor
T ←ShMdBothMasks[Stack&1j;
IFUJump[Oj. Store←RTemp. OBuf←T;

The shift controls come directly from FF if ASEL =7 (a shift) and if BSEL = 4, 5, 6, or 7,
selecting a constant. This specifies complete shift control in the instruction which does the
shift, so ShC doesn't have to be loaded in a previous instruction, and ShC isn't clobbered,
so io tasks don't have to save and restore it. When BSEL controls a shift in this way, the B
source is forced to be Q.

The mask operations are as follows:

<!-- PDF page 32 / printed page 25 -->

> **Table 12: ALUF Shift Decodes**

![Table 12: ALUF Shift Decodes](tables/table-12-aluf-shift-decodes.png)

`ALUF[3]` selects the ALU operation in either ALUFM `16₈` or `17₈`.

| ALUF[0:2] | Operation |
|:--:|:--|
| 0 | `ShiftNoMask` |
| 1 | `ShiftLMask` — masked bits on the left-hand side replaced with 0's |
| 2 | `ShiftRMask` — masked bits on the right replaced with 0's |
| 3 | `ShiftBothMasks` — masked bits on both sides replaced with 0's |
| 4 | `ShMdNoMask` — unused (falls out of decoding) |
| 5 | `ShMdLMask` — masked bits replaced with Md |
| 6 | `ShMdRMask` — masked bits replaced with Md |
| 7 | `ShMdBothMasks` — masked bits replaced with Md |

ShiftLMask implements right shift and load-field operations; ShiftRMask implements left
shift; Shift80thMasks deposits the selected field into a word of zeroes; ShMd80thMasks
deposits the selected field into data coming from memory; and ShiftNoMask implements
various cycle operations.

Note:     On a shift the ALU branch conditions apply to the unmasked ALU output.

### 3.12. Hold and Task Simulator

The hold and task simulators are provided for hardware checkout (programmers skip this
section).

Hold&TaskSim~8 loads HOLDSIM[0:7] from 8[8:14] .. 0 and TASKSIM[O:6] from 8[1:7].
HOLDSIM is a reCirculating shift register in which the presence of a 1 in bit 7 causes HOLD
two instructions later. For example, Hold&TaskSim←200s will complete three instructions
after the Hold&TaskSim~, HOLD the next cycle, and HOLD every seventh instruction (i.e.,
every eighth cycle) thereafter. Since this register cannot be loaded with all l's and since its
clocks are not disabled by HOLD, HOLD of infinite duration is impossible.

To disable this debugging feature, the register must be loaded with O.

TASKSIM is a seven-bit counter which determines the number of cycles before a task
wakeup occurs. The task selected for wakeup must be jumpered on the backplane (else no
op), Whenever T ASKSIM is loaded with a non-zero value, it counts up to 177s' then
generates a wakeup request when the counter overflows to 2008' The wakeup request
remains true until TASKSIM is reloaded.

<!-- PDF page 33 / printed page 26 -->

## 4. Control Section

The control section interfaces the mainframe to the baseboard microcomputer or Alto which
controls it as detailed in the "Dorado Debugging Interface" document. In addition, the
control section stores instructions in 4k x 34-bit (+ 2 parity) IM ("!nstruction Memory") and
contains logic for sequencing through instructions and switching among tasks.

The current instruction is clocked into the MIR register at to and exported to the processor,
memory, and IFU sections for decoding. The control section itself decodes the JCN field,
the BLOCK bit, and its own FF decodes (Wakeup, Bt-Link,. B~RWCPReg, Link~B,
TaskingOn, Tasl<ingOff, BDispatch~B, BigBDispatch~B, Multiply, MidasStrobe~B, UseDMD,
and branch conditions).

The control section also exports the task number via the Next bus, which somewhat after t.2
contains the task number that will execute an instruction at to'

Figure 5 shows the overall organization of the control section. Figure 6 shows how branch
control is encoded in JCN. Figure 7 shows the timing for regular instructions and for the
multi-cycle TPC and IM read/write instructions.

### 4.1. Tasks

Dorado provides sixteen independent priority-scheduled tasks at the microcode level. Task
15 is highest priority, task a lowest. Task 15 (the "fault task") is woken by StkError and by
memory map and data error faults. Tasks 1-14 provide processing functions for io
controllers implemented partially in hardware, partially in firmware; the present assignment of
these tasks to device controllers is given in the "Slow 10" chapter. Task a (the "emulator")
implements instruction sets (Mesa, Alto, etc.). In the absence of io activity, task 0 (always
awake) controls the processor.

Essentially, io devices are paired to tasks when built, and a device controller can assert a
wakeup request for the task with which it is paired. A program cannot modify the
assignment of controllers to tasks (although the hardware change for this is easy).
Additional flexibility in this area is not thought to be worth additional hardware cost.

Each task has its own program counter and subroutine return link, stored in the (task
specific) TPC and TLINK registers when the task is inactive. TPC may also be treated as a
memory, so program counters for tasks other than the current task can be read and written
by a program. This is discussed later in this chapter.

### 4.2. Task Switching

When device hardware requires service from a task, it activates its wakeup request line at to'
Wakeup requests are priority-encoded, and the highest priority request (BNT or "gest t!ext
Iask") is clocked at t2 and competes with the current task (CTASK) for control of the
machine. If BNT is higher priority than CTASK, or if the current (non-emulator) instruction
has BLOCK = 1, a task switch will take place; in this case, CT ASK will be loaded from BNT
at t4 • This implies that the shortest delay from a wakeup request to the first instruction of

<!-- PDF page 34 / printed page 27 -->

the associated task is two cycles.

The 16 Wakeup[task] FF decodes allow any task to be woken, just as though a hardware
device had activated its wakeup line. A minimum of two cycles elapses after the instruction
containing Wakeup before the task executes its first instruction. The task responding to a.
Wakeup must not block sooner than the second instruction, or it will get reawakened.

When a task has been woken by Wakeup[task] or has executed one or more instructions and
then deferred to a higher priority task, the fact that it is runnable is remembered in a Ready
flipflop. The Ready flipflop is cleared only when the associated task blocks. In other words,
there is no way to deactivate a task, after its ready flipflop has been set, except by forcing it
to execute an instruction that blocks. The Wakeup[task] function must be executed with
tasking off, if it is possible that the specified task might be waking up for some other reason
(e.g., due to a wakeup request from an external device, or due to a wakeup issued by yet
another task). Otherwise, the control section may get horribly confused, and the machine
will hang in the same ta3k forever.

An acceptable sequence is:

TaskingOff;
Wakeup[task];
TaskingOn;
The baseboard and Alto controllers may also clear the Ready flipflops by another mechanism,
discussed in "Dorado Debugging Interface".

The emulator has no Ready flipflop and cannot block; t11e BLOCK bit in the instruction is interpreted
as StackSelect for the emulator.

Task switching may occur after every instruction unless explicitly disabled by the TaskingOff
function. The TaskingOn function reverses the effect of TaskingOff. TaskingOff is "atomic";
an instruction containing TaskingOff will be held if a task switch is pending; the next
instrcution will be executed in sequence without any intervening task switches. TaskingOn is
not immediately effective; at least two more instructions will be executed by the same task
before task switching can occur.
It would be a programming error for a task to block with tasking off, but if it did, the block would fail,
and it would continue execution.

It is illegal for a task to block in an instruction that might be held, if the wakeup line for the
task might be dropped at to of the instruction. If this occurred, the instruction might
inadvertently be repeated before the block occurred.
Remark

Multiple tasks seem better than a more conventional priority interrupt system because interference by
input/output tasks is substantially reduced. As to the exact implementation, variations are possible. The current
scheme requires more hardware than one in which the program explicitly indicates when a task switch is legal
(as on Alto and DO). . However, because Hold may last for about 30 cycles, ;J reliance upon explicit tasking
would result in inadequate service for high priority tasks.

<!-- PDF page 35 / printed page 28 -->

### 4.3. Next Address Generation

This section gives a low-level view of jump control. Because the microassembler
and loader handle details of instruction placement automatically, programmers need
not struggle with the encodings directly. For this reason, programmers may wish to
skim this section while concentrating on high-level jump concepts described in
"Dorado Microassembler".

Read this with Figure 6 in front of you.

For the most part, instruction memory (IM) addressing paths are 16 bits wide, although only
12 bits are presently used; the extra width allows for future expansion to 13 or 14 bits, when
sufficiently fast 4kx1 ECl RAMS are economically available; there are no plans to utilize the
remaining 2 bits, but since nearly all hardware components ,in the control data paths are
packaged 4/can, the extra two bits are almost free. Also, the 16-bit wide Link register can
be used to hold full word data items.

The various registers and data paths that contain IM addresses are numbered 0:15, where
bits 4:15 are significant for the 4k-word microstore, while the quadrant bits 2:3 are ignored.
This numbering conveniently word-aligns the bits while also allowing for future expansion.
The discussion below assumes a 4k-word microstore.

Dorado does not have an incrementing instruction-address counter. Instead, the address of
the next instruction is determined by modifying the current instruction address (CIA) in
various ways. The Tentative Next Instruction Address (TNIA) is determined from JCN[0:7] in
the instruction according to rules in Figure 6. TNIA addresses IM for the fetch of the next
instruction unless a task switch occurs. If a task switch occurs, the program counter for the
highest priority competing task (BNPC or "Best Next PC") addresses IM.

A 16k-word microstore is viewed as consisting of four 4k-word quadrants; each IM quadrant
is viewed as containing 64 pages of 64 instructions. Values in JCN are provided for the
following kinds of branches:
Local branches to any of the 64 locations in the current page;
Global branches to location 0 on any of the 64 pages of the current quadrant;
Long branches to any location in the quadrant using the 8-bit FF field to extend JCN
(normal interpretation of FF is disabled);
Conditional branches to any of 14 even locations in the current page, if the selected
condition is false, or to the adjacent odd location, if the condition is true (7 branch
conditions are available);
IFU jumps to a starting address supplied by the IFU; JCN selects anyone of up to 4
entries in the starting address vector (This is motivated by an entry-vector scheme
discussed in "Instruction Fetch Unit".);
read/write IM and read/write TPC, after which execution continues at . + 1;
Return to the address in Link;

<!-- PDF page 36 / printed page 29 -->

Branch conditions may also be specified in FF, as discussed below. Several dispatches may
also be specified in FF. These 'OR' bits into the branch address computed by the following
instruction.

If IM is expanded to 16k words, branching from one quadrant to another will only be possible
by loading the Link register with a 14-bit address and then returning; jumps, calls, and
IFUJumps will be confined to the current 4k-word IM quadrant.

Remarks on JCN Encoding

JCN cleverly encodes in a bits almost as much programming flexibility as would be possible with an arbitrarily
large and general field. The main disadvantage is that MicroD is needed to postprocess assemblies and place
instructions.

The earliest prototype of Dorado used. a 7-bit JCN encoding that had fewer global and conditional branch
targets, so programming was harder and additional instructions had to be inserted in a few places. This was
slightly worse than the a-bit encoding, but it would have been feasible to stay with the 7-bit encoding and
employ the bit thus saved for some other use in the instruction.

local, global, and long branches are analogous, respectively, to local, page-zero, and indirect branches used on
many minicomputers. However, Dorado scatters its global locations over the microstore rather than concentrating
them in page zero; this is better than the minicomputer scheme for the following reason. During instruction
placement, when a cluster of instructions is too large to fit on one page, a global allows it to be divided between
two pages; but if all globals were in page zero, then page zero itself would quickly fill up. In other words,
dispersing the globals is theoretically more powerful than concentrating them in page zero; because MicroD does
all the tedious work of placing instructions, this theoretical advantage is made practical; minicomputers have not
employed any program like MicroD, so they have used the less powerful but simpler page zero scheme.

local branches on Dorado are within a 64-word page, where minicomputers usually branch relative to the current
PC. Relative branching is probably more powerful, but it cannot be used on Dorado because of insufficient time
for addition.

long branches on Dorado use 4 bits of JCN in conjunction with the a-bit FF field to specify any location in the
4k-word quadrant. Since BSEL never selects a constant in this case, an improvement on our scheme would
have used 3 bits of JCN in coniunction with BSEL.O and the a-bit FF field; this would have freed a values of
JCN to encode some other kind of branch. In addition, 5 of the 256 values of JCN are unused and 1 is a
duplicate (See Figure 6 for the 5 unused decodes; the replicated decode is the Global call on the local page.).
We have variant JCN decodings that correct these problems, but they were not ready when the design was
frozen.

### 4.4. Conditional Branches

IM is organized in two banks, with odd addresses in one bank, even in the other. The
address is needed shortly after to' but the bank-select signal not until 15 ns after the
address.    For this reason conditional branches select between an even-odd pair of
instructions (Le., between the two banks) according to branch conditions that need not be
stable until a little after t1•

Alternatively, a conditional branch may be encoded in FF in crmjunction with any addressing
mode except a long branch in JCN. When this is done, the result of the branch test is ORed
with TNIA[15].

This implies that for both FF-encoded and JCN-encoded branch conditions, the false target
address is even and the true target is odd.

Hence, it is possible to conditionally branch using only JCN, while using FF for an unrelated

<!-- PDF page 37 / printed page 30 -->

function, or to encode a branch condition in FF while using any addressing mode in JCN. If
branch conditions are encoded in both FF and JCN, the branch test results are OR'ed,
providing further flexibility.

The branch condition encodings are:

> **Table 13: Branch Conditions**

![Table 13: Branch Conditions](tables/table-13-branch-conditions.png)

| JCN[5:7] | FF | Branch Condition |
|:--:|:--:|:--|
| 0 | 60 | `ALU = 0` |
| 1 | 61 | `ALU < 0` |
| 2 | 62 | `ALUcarry'` |
| 3 | 63 | `Cnt = 0&−1` (decrements count *after* testing) |
| 4 | 64 | `R<0` (RM or STK, whichever selected, *not* overruled by RIsId) |
| 5 | 65 | `R Odd` (RM or STK, whichever selected, *not* overruled by RIsId) |
| 6 | 66 | `IOAtten'` (non-emulator) or `ReSchedule` (emulator) |
| — | 67 | `Overflow` |

ALU =0 and ALU<O are the results of the last ALU operation executed by the current task.
ALUcarry' (the saved carry out of the ALU) and Overflow are the result of the last arithmetic
ALU operation executed by the current task (ALU←A may be stored in ALUFM as either an
arithmetic or logical operation, so programmers should be wary of smashing these branch
conditions when ALU←A is used.). These are saved in a RAM and may be frozen by the
Freeze8C function for one cycle. In other words, the branch conditions are ordinarily loaded
into the RAM at t3 , but if Freeze8C is present, then the RAM is not loaded and values from
the previous instruction for the same task will apply.

The 10Atten' branch condition tests the task-specific 10Attention signal optionally generated
by the io device associated with the current (non-emulator) task.

Remark on Target Pairs

The bank-select toggling trick, which allows branch conditions to be developed very late, is lIaluable. Without
this trick, it would be necessary to choose between slowing the instruction cycle or restricting branch conditions
to signals stable at to'      Neither of these alternatives is palatable.

A more traditional implementation of conditional branches would go to the branch address, if a condition were
true, or fall through to the instruction at . + 1, if it were false. This traditional scheme is never faster but is
sometimes more space-efficient than the target-pair scheme because the target'pair requires a duplicated
instruction for every instance of a conditional branch to a single target, which is fairly common. The traditional
scheme does not allow DblGoto and DblCall constructs discussed in "Dorado Microassembler," but these are
infrequent.

### 4.5. Subroutines and the Link Register

Dorado provides single-level subroutines by means of the (task-specific) Link register. A Call
occurs on any instruction whose destination address is 0 mod 16 qefore any modification of
TNIA due to branch conditions or dispatches. On a Call, Return, or IFUJump, Link is loaded
with CIA+ 1.
Because Return loads Link with CIA+ 1, CoReturn constructs are possible. Because IFUJump also
loads Link with CIA + 1, the conditional exit feature discussed in the "Instruction Fetch Unit" chapter is
possible.

<!-- PDF page 38 / printed page 31 -->

CIA + 1 is used rather loosely in discussion here; the actual value loaded into Link by a call
or return is [(CIA & 177700a) + ((CIA + 1) & 77a)]. In other words, a call or return in location
778 of any page loads Link with location 0 of that page.

Link may be loaded and read by programs, so deeper subroutine nesting is possible, if Link
is saved/restored across calls.
The functions Linkt-S and St-RWCPReg and the S dispatch functions discussed below, all of which
load Link from S, overrule a call. In other words, if there are conflicting reasons for loading Link,
Linkt-S wins over Linkt-CIA+1.

The St-RWCPReg function (= Linkt-B, Bt-CPReg') is provided primarily for initialization from the
baseboard computer and for use by the Midas debugging program. Since the CPReg register clock is
asynchronous to the Dorado clock system, a Dorado microprogram that reads CPReg (e.g., to receive
information from the baseboard) must use some synchronization method to ensure that CPReg is
stable during the cycle in which it is read.

Note: it is illegal to use an ALU branch condition in the instruction after Pdt-RWCPReg, if CPReg
might have been loaded during the cycle in which it is read-this might result in an unstable IM
address being presented to the control store.

Remark on Call/Jump

Deciding between call and jump based on target address saves one bit in the instruction and _costs little for the
following reasons. Instructions can be divided into three groups: those always jumped to, those always called,
those for which Link can be smashed (i.e., "don't care" about call or jump), and those both jumped to and
called.

A realistic guess is that over half of all instructions will be "don't care"; namely, these will be executed at the
top level, not inside a subroutine, and the Link register will not contain anything of importance. Assembly
language declarations make this information available to MicroD.

n,e hardware makes 1/16 of the locations in each page "call locations". It is estimated that this is somewhat
more than real programs will need, on the average (although we vacillated about whether 118 or 1116 of the
targets should be calls).

In each page, MicroD first places instructions that must be called or must be jumped to. Because there are so
many "don't care" instructions, it is unlikely that either call or jump slots in a page will be exceeded.
Consequently, it will nearly always be possible to complete allocation of the call and jump targets without
overflowing due to the call/jump restriction. After this "don't care" instructions fill in the remaining slots.

The remaining situation, with which Dorado cannot cope, is an instruction both called and jumped to. This
would arise in a subroutine whose entry instruction closed a loop (uncommon). On Dorado, this situation
requires duplicating the entry instruction, so it costs one location but no extra time.

<!-- PDF page 39 / printed page 32 -->

### 4.6. Dispatches

Several FF decodes are dispatches which OR various bits with TNIA[8:15] during the
following instruction. The dispatch bits must be stable by t 2 •

Dispatches are:

BigBDispatch←8    8[8:15) (256-way dispatch)
BDispatch←8       8[13:151 (8-way dispatch)
Multiply           OR's 0[14) into TNIA[14] (The value of 0[14] is captured in a flipflop at t2 of the
instruction containing the Multiply function and is OR'ed into TNIA[14) during the
next instruction for the same task.)

Example:

BDispatch ← T;
Branch[300];      *branches to 300 OR T[13:15]

The two B dispatches load Link register from B, then OR appropriate bits of Link into TNIA
. during the next instruction for the task. Since Link is task-specific, this works correctly
across task switching. The Q-bit is only loaded during a multiply, and tasks other than the
emulator are not allowed to use the multiply function.

The decision between call and jump in the instruction after a dispatch is unaffected by
dispatch bits-it depends only upon JCN. In other words, the instruction following a
dispatch is a Call if its unmodified target address is 0 mod 16, else a jump.

It is possible to neutralize any bits in a dispatch by placing target instructions at locations
with 1's in the neutralized bits. In other words, a dispatch on B[8:10] could be accomplished
by locating the 8 target instructions at IM locations whose low five address bits were 1, e.g.
at 378 , 778 , 1378, 1778 , 2378 , 2778 , 3378 , and 3778, and by branching to 378 in the
instruction after the BigBDispatch←B.

Note: Methods discussed later for resuming a program interrupted by a page fault do not
permit continuation when a fault occurs between a dispatch and the following instruction; for
this reason, programmers should ensure that no fault can possibly occur by holding for
memory faults with ←Md prior to or concurrent with the dispatch; also, stack operations that
might overflow/underflow may not be used in the instruction after a dispatch.

Note: When the PC for another task is loaded using the LdTPC← operation discussed later,
any pending dispatch conditions for that task are cleared. The debugging program Midas
does not clear pending dispatches, however, so it should be ok to put a breakpoint on the
instruction after a dispatch or to single-step through a dispatch.

### 4.7. IFU Addressing

The IFU supplies ten bits of opcode starting address to the processor. During the last
instruction of every opcode, exit to the next opcode is accomplished by IFUJump[n] (n = 0
to 3) which selects among four entry locations for the next opcode. The starting address
supplied by the IFU is used for TNIA[4:13] and TNIA[14:15] are set to n. If the IFU is
unprepared, it supplies a trap address instead of a starting address, and control goes to the

<!-- PDF page 40 / printed page 33 -->

nth location in a trap vector.

IFUJump's always load Link with CIA + 1.               This is necessary to implement the following
conditional exit feature for opcodes.

If an FF-encoded branch condition is true in the same instruction as an IFUJump, IFU
advance to the next opcode is disabled. This kludge allows an opcode with common and
uncommon exit conditions to finish, for example, with IFUJump[2,condition]. If the condition
is false (common case), then the IFU advances normally to the next opcode, starting at
location 2 of the entry vector. Otherwise (uncommon case), control continues at location 3
of the entry vector, but the IFU does not advance, so emulation of the current opcode can
continue.

Utilization of IFUJump and conditional IFUJump is discussed in "Instruction Fetch Unit."

IFU trap addresses and other reserved locations in the microstore are as follows:

> **Table 14: Reserved Locations in the Microstore**

![Table 14: Reserved Locations in the Microstore](tables/table-14-reserved-microstore-locations.png)

Upon dispatch to the first instruction for the opcode affected
by the fault, this trap occurs.
Midas Call command             7n6
Midas Crash detect             nn
*Ifu traps OR the 1's complement of the instruction set into bits 8:9 of the trap address, so actual trap
locations for Reschedule, for example, are 14-17, 114-117, 214-217, and 314-317. The trap vector is 1
to 4 instructions long according to the IFUJump programming convention, as discussed in the
"Instruction Fetch Unit" chapter.

### 4.8. IM and TPC Access

See figures 6 and 7.

IM is read and written by programs using a special decode of JCN in conjunction with the
ASTK field of the instruction; TPC is also read and written using a special JCN decode.
TaskingOff must be in force, and anything that might cause hold is illegal in the same
instruction; hold is also illegal in the instruction after an IM or TPC read, when the data is
accessed using B(-Link.                                               •

It has been reported that IM←Md doesn't work because ←Md causes hold at unexpected times.

After the read or write instruction, control passes to the next sequential instruction, i.e., to
CIA + 1 (with wrap-around at 64-word page boundaries). CIA + 1 also winds up in Link.

<!-- PDF page 41 / printed page 34 -->

Note: The hardware does not actually load Link with the IM or TPC data; instead B←Link in the next
cycle routes inverted data onto B using an alternate path. The Link register itself is smashed with
CIA + 1 as discussed above, and this value would be read (assuming it wasn't overwritten) in later
instructions.

This implies that continuation from a breakpoint or program-interrupt halt on the instruction following
an IM or TPC read (i.e., on the B←Link instruction) won't work correctly.

Total time for an IM or TPC read or write operation is 6 clocks (Le., thrice as long as a
normal instruction).

A 34 ( + 2 parity)bit IM word is read as four 9-bit quantities. The read address is taken from
Link. Data must be read from Link[7: 15] in the instruction immediately after the IM read; this
data is inverted; Link[0:6] contain 1's, so that when the entire word is 1's complemented the
desired data will have leading O's. The byte select is RSTK[2:3].

IM writes also take the write address from Link, 16 bits of data from Band 2 bits from RSTK;
the half-word affected is also specified in RSTK.

Any task can read or write TPC for an arbitrary task other than itself (an attempt to set TPC
of the running task is unpredictable). The task number is B[12:15], and data is taken from or
written into Link. The assembly language notations for these are RdTPC←-B and LdTPC←-B.
After RdTPC←-B, the 16 bits of data in Link are 1's complemented.

Note:   The dispatch-pending conditions for a task whose TPC is loaded by LdTPC←- are
cleared, so LdTPC←- works even when that task has just executed a BDispatch←-B or
BigBDispatch ←- B.

### 4.9. Hold

Many events in the memory system, StkError and the hold simulator in the processor, and
several IFU error conditions generate hold (The IFU error conditions cause a one-cycle hold
iff an IFUJump occurs on the first cycle of the error.). The control section itself forces hold
when a task switch occurs concurrent with TaskingOff. This signal, clocked at t 1, occurs
when the current instruction cannot be completed. Its effect on the hardware is to suspend
the current instruction, while completing parts of the previous instruction that have been
pipelined into the current cycle. Approximately, it converts ~he current instruction into a
Goto[.] while preserving branch conditions and some other stuff.

Higher priority tasks are not prevented from running when the current task is experiencing
Hold.

Remark

The fact that the address of the next instruction is needed at to' while Hold is not generated until t1 means that
concurrence of Hold and BLOCK with a switch to a lower priority task produces an anomalous situation called
"Next Lies". The hardware disables clocks to CIA, TPC, and MIR when this occurs, so that the current
instruction is repeated. This results in some hardware complications discussed in the "Slow 10" chapter, but
programmers need not worry about it.

### 4.10. Program Control of the DMux

<!-- PDF page 42 / printed page 35 -->

Dorado contains a large number of multiplexors called mufflers which allow a selected signal
from a set of up to 2048 signals to be observed on a one-wire bus called the DMux. This
provides a passive method by which the Baseboard section or the external Midas debugger
can examine internal control signals and registers not otherwise observable.

The particular DMux signal is selected by shifting in an 11-bit address one bit at-a-time.
Each board with mufflers contains a 12-bit address register that responds to the shifted
address bits; the highest bit is ignored for the purposes of selecting the signal to be read.
"Dorado Debugging Interface" discusses a clev~r. generator algorithm that allows all 2048
signals to be read into a table in 2048 + 11 shift-read cycles..

In addition, the DMux address can also be executed as a control function. In this case the
fuJI 12-bit address determines what function is executed. This "manifold" mechanism is
used to control power supplies, set clock rate, enable/disable error halt conditions, and test
IM without involving other hardware.

The DMux facility can also be controlled directly by Dorado programs by means of the
MidasStrobe←8 and UseDMD functions. Essentially, the DMux address mechanism is
controlled externally by the Baseboard or by Midas operating through the Baseboard when
Dorado isn't running, and by Dorado when Dorado is running.

The MidasStrobe←B function causes B[4] to be shifted out as an address bit. This takes
three cycles, so the program must execute three more instructions before doing another
MidasStrobe←B function. The DMux. signal selected by the last 11 address bits shifted out is
read on B[O] when the Pd ← ALUFMEM function is executed.

The UseDMD function causes the current DMux address to be executed as a manifold
operation.

The following subroutine reads the DMux signal selected by the address in T:

Subroutine;
ReadDMux:
Cnt←13S;
RdDMuxLp:
MidasStrobe← T;      "Shift out address in T[4]
Noop;
Noop;
T←(1) Ish 1, Goto{RdDMuxLp,Cnt #0&-1];
T ← ALUFMEM;         *T[O] returns selected DMux address
Return;

<!-- PDF page 43 / printed page 36 -->

## 5. Memory Section

Dorado supports a linear 22-bit to 28-bit virtual address space and contains a cache to
increase memory performance.        All memory addressing is done in terms of virtual
addresses; later sections deal with the map and page faults. Figure 8 is a picture of the
memory system; Figure 9 shows cache, map, and storage addressing. As Figure 8
suggests, the memory system is organized into three more-or-Iess independent parts:
storage, cache data, and addressing.

Inputs to the memory system are NEXT (the task that will control the processor in the next
cycle) from the control section, subtask from io devices, Mar (driven from A or by the IFU),
MemBase, B, the fast input bus, and an assortment of control signals. Outputs are B, Md
to the processor, the FIG registers for the IFU, the fast output bus (data, task, and
subtask), and Hold.

The processor references t:1e memory by providing a base register number (MemBase) and
16-bit displacement (Mar) from which a 28-bit virtual address VA is computed; the kind of
reference is encoded in the ASEL field of the instruction in conjunction with FF[O:1].
Subsequently, cache references transfer single 16-bit words between processor and cache;
fast io references independently transfer 256-bit munches between io devices and storage.
There is a weak coupling between the two data sections, since sometimes data must be
loaded into the cache from storage, or returned to storage.

The storage pipeline allows new requests every 8 cycles, but requires 28 cycles to
complete a read. The state of the pipeline is recorded in a ring buffer called the pipe,
where new entries are assigned for each storage' reference. The processor can read the
pipe for fault reporting or for access to internal state of the memory system.

### 5.1. Memory Addressing

Processor memory references supply (explicitly) a 16-bit displacement 0 on Mar and
(implicitly) a 5-bit task-specific base register number MemBase. Subtask[O:1] (See "Slow
10") are OR'ed with MemBase[2:3] to produce the 5-bit number sent to the memory.
MemBase addresses 1 of 32 28-bit base registers. The full virtual address VA[4:31] is
BR[MemBase] + D. D is an unsigned number.
The 28 bits in SR, VA, etc. are numbered 4:31 in the discussion here, consistent with the hardware
drawings.    This numbering conveniently relates to word boundaries.

Note that although the VA path is 28 bits wide, limitations imposed by cache and map geometry limit
usable virtu.al memory to only 222 or 224 words in most configurations, as discussed in "The Map"
section later.

MemBase can be loaded from the five low bits of FF, and the FlipMemBase function loads
MemBase from its current value xor 1. In addition, MemBase can be loaded from
O.MemBX[O:1].FF[6:7], where the purpose of the 2-bit MemBX regi:ster is discussed in "IFU
Section. " The IFU loads the emulator task's MemBase at the start of each opcode with a
MemBX-relative value between 0 and 3.

The intent is to point base registers at active structures in the virtual space, so that
memory references may specify a small displacement (usually 8 or 16 bits) rather than full

<!-- PDF page 44 / printed page 37 -->

28-bit VA's. In the Mesa emulator, for example, two base registers point at local (MDS + L)
and global (MDS + G) frames.

In any cycle with no processor memory reference, the IFU may make one. IFU references
always use base register 31, the code base for the current procedure; the D supplied by
the IFU is a word displacement in the code segment.
Programmers may think of Mar as an extension of A since, when driven by the processor, Mar
contains the same information as A.

The base register addressed by MemBase can be loaded using BrLo←A and BrHi←A
functions. VA is written into the pipe memory on each reference, where it can be read as
described later. The contents of the base register are VA-D on any reference.

### 5.2. Processor Memory References

Memory references are initiated only by the processor or IFU. This section discusses what
happens only when references proceed unhindered. Subsequent sections deal with map
faults, data errors, and delays due to Hold.

Processor references (encoded in the ASEL and FF[O:1] instruction fields as discussed in
the "Processor Section" chapter) have priority over IFU references, and are as follows:
Fetch←            Initiates one-word fetch at VA. Data can be retrieved in any
subsequent instruction by loading Md into R or T, onto A or B
data paths, or masking in a shift operation.
Store←            Stores data on B into VA.
LongFetch←        A     fetch  for    which   the    complete         28-bit    VA     is
(B[4:15]"Mar[O:15]) + BR[MemBase].
IFetch←           A fetch for which BR[24:31] are replaced by Id from the IFU.
When BR[24:31] are 0 (Le., when BR points at a page boundary),
this is equivalent to BR + Mar + Id, saving 1 instruction in many
cases. Note: the IFU does not advance to the next item of ←Id
for IFetch←, so an accompanying Tisld or Risld function is
needed to advance.
PreFetch←         Moves the 16-word munch containing VA to the cache.
DuminyRef←        Loads the pipe with VA for the reference without initiating cache,
map, or storage activity.
Flush←            Removes a munch containing VA (if any) from the cache, storing
it first if dirty (emulator or fault task only).
Map←              Loads the map entry for the page containing VA from Band
clears Ref; action is modified by the ReadMap function discussed
later (emulator or fault task only).
IOFetch←.         Initiates transfer of munch from memory to io device via fast
output bus (io task only).

<!-- PDF page 45 / printed page 38 -->

IOStore←              Initiates transfer of munch from io device to memory via fast
input bus (io task only).
(Inside the memory system, there are three other reference types: IFU reads, dirty cache victim
writes, and FlushStore fake-reads that result from Flush..- references which hit dirty cache entries.)

The notation for these memory references has been confusing to people who first start
writing microprograms. The following examples show how each type of reference would
appear in a microprogram:
Fetch..-T;                   Start a fetch with"O coming from T via Mar
T..-Md;                      Read memory data for the last fetch' into T
Store"-Rtemp, OBuf..-T;      Start a store with 0 coming from an RM
address via Mar and memory data from T via B.
PreFetch..-Rtemp;
Flush"-Rtemp;
IOFetch..-Rtemp;
IOStore"-Rtemp;
Map..-Rtemp, MapBuf..-T;     Start a map write with 0 coming from an RM
address (Rtemp) via Mar, data from T via 8
RMap"- Rtemp;                Start a map read with 0 coming from an Rm
address (Rtemp) via Mar.
LongFetch..-Rtemp, B..-T;    Start a fetch reference with
VA = BR[4:31] + (T[4:15]"Rtemp[O:15]).
IFetch..-Stack;              Start a fetch reference with Id replacing BR[24:31]
and with 0 coming from Stack.
IFetch"-Stack, Tisld;        Start a fetch as above and also advance the IFU to the
next item of ..-Id.

The tricky cases above are Store←, Map←, and LongFetch←, which must be accompanied
by another clause that puts required data onto B. DBuf← and MapBuf← are synonyms for
B←, and do not represent functions encoded in FF; these synonyms are used to indicate
that the implicitly loaded buffer registers (DBuf on MemD and MapBuf on MemX) will wind
up holding the data.

The encoding of these references in the instruction was discussed in the "Processor"
section under "ASEL: A Source/Destination Control". The ten possible memory reference
types have the following properties:

Fetch (-, fFetch (-, and LongFetch f-

These three are collectively called "fetches" and differ only in the way VA is computed. In
any subsequent instruction, memory data Md may be read. If Md isn't ready, Hold occurs,
as discussed below. If the munch containing VA is in the cache and the cache isn't busy,
Md will be ready at t3 of the instruction following the fetch, with the following implications:

If Md is loaded directly into RM or T (loaded between t3 and t 4), it can be read in
the instruction after the fetch without causing Hold. This is called a deferred
reference.

If Md is read onto A or B (needed before t2) or into the ALU masker by a shift
(needed before t3 ), it is not ready until the second instruction after the fetch (Hold
occurs if Md is referenced in the first instruction.). This is called an immediate
reference.

<!-- PDF page 46 / printed page 39 -->

The above timing is minimum, and delays may be longer if data is not in the .cache or if the
cache is still busy with an earlier reference.

Md remains valid until and during the next fetch by the task. If a Store← intervenes
between the Fetch← and its associated ←Md, then ←Md will be held until the Store←
completes but will then deliver data for the fetch exactly as though no Store← had
intervened.

StDre~

Store← loads the memory section's DBuf register from B data in the same instruction. On a
hit, DBuf is passed to the cache data section during the next cycle. On a miss DBuf
remains busy during storage access and is written into the cache afterwards.

Because DBuf is neither task-specific nor reference-specific, any Store←, even by another
task, holds durin~ DBuf-busy.       However, barring misses, Store←'s in consecutive
instructions never hold. A fetch or ←Md by the. same task will also hold for an unfinished
Store←.

PreFetch~

PreFetch← is useful for loading the cache with data needed in the near future. PreFetch←
does not clobber Md and never causes a map fault, so it can be used after a fetch before
reading Md.

IOFetch~

An 10Fetch← is initiated by the processor on behalf of a fast output device. When ready to
accept a munch, a device controller wakes up a task to start its memory reference and do
other housekeeping.

An 10Fetch← transfers the entire munch of which the requested address is      a  part (in 16
clocks, each transferring 16 data + 2 parity bits); the low 4 bits of VA are ignored by the
hardware. If not in the cache, the munch comes direct from storage, and no cache entry is
made. If in the cache and not dirty, the munch is still transferred from storage. Only when
in the cache and dirty is the munch sent from the cache to the device (but with the same
timing as if it had come from storage). In any case, no further interaction with the
processor occurs once the reference has been started. As a result, requested data not in
the cache (the normal case) is handled entirely by storage, so processor references
proceed unhindered barring cache misses.

The destination device for an 10Fetch← identifies itself by means of the task and subtask
supplied with the munch (= task and subtask that issued 10Fetch←). The fast output bus,
task, and subtask are bussed to all fast output devices. In adc;lition, a Fault signal is
supplied with the data (correctable single errors never cause this fault signal); the device
may do whatever it likes with this information. More information relevant to IOFetch← is in
the "Fast 10" chapter.

<!-- PDF page 47 / printed page 40 -->

IOFetch'" does not disturb Md used by fetches, DBuf used by Store"', or MapBuf used by
Map'" .
There is no way to encode either IOFetch← or IOStore← in an emulator or fault task instruction, and
there should never be any reason for doing this.

10Store4-

IOStore'" is similar to IOFetch.... The processor always passes the reference to storage.
The cache is never used, but a munch in the cache is unconditionally removed (without
being stored if dirty). A munch is passed from device to memory over the fast input bus,
while the memory supplies the task and subtask of the IOStore← to the device for
identification purposes. The device must supply a munch (in 16 clocks, each transferring
16 bits) when the memory system asks for it.

The Carry20 function may be useful with IOFetch'" and IOStore←. This function forces the
carry-in to bit 11 of the ALU to be 1, so a memory address D on A can be incremented by
16 without wasting B in the same instruction.

Map~

This is discussed later.

Flush~

Flush← unconditionally removes a munch containing VA from the cache, storing it first if
dirty. It is a no-op if no munch containing VA is in the cache; it immediately sets Vacant in
the cache entry and is finished on a clean hit; it gives rise to a FlushStore reference on a
dirty hit.                                      .
Only emulator or fault task instructions can encode Flush←, using the private pipe entry (0 or 1)
pointed at by ProcSRN. The FlushStore triggered, if any, uses the ring-buffer part of the pipe.
FlushStore turns on BeingLoaded in the cache entry and trundles through a (useless but harmless)
storage access to the item being flushed; when this finishes Vacant is set in the cache entry; then
the dirty-victim is written into storage.

Unfortunately, Flush← clobbers the Victim and NextV fields in the cache row, which causes the cache
to work less efficiently for awhile.

Some applications of Flush ← are discussed later in the Map section. Note: it is necessary
to hold until any preceding private pipe entry fetch or Store← has finished by issuing
←Md-reasons for this are discussed in "The Pipe" section.

DummyRef4-

DummyRef← writes VA into the pipe entry for the reference without initiating cache, map, or
storage activity. This is provided for reading base registers and so diagnostic microcode
can exercise VA paths of the memory system without disturbing cache or memory. Note: it
is necessary to hold until any preceding private pipe entry fetch or Store← has finished by
issuing ... Md-reasons for this are discussed in "The Pipe" section.

<!-- PDF page 48 / printed page 41 -->

### 5.3. IFU References

The F and G data registers shown in the IFU picture (Figure 11) are physically part of the
memory system. The memory system fetches words referenced by the IFU directly into
these registers. The IFU may have up to two references in progress at-a-time, but the
second of these is only issued when the memory system is about to deliver data for the first
reference.

An IFU reference cannot be initiated when the proGessor is either using Mar or referencing
the Pipe; for simplicity of decoding, the hardware disables IFU. references when the
processor is either making a reference or doing one of the functions 1208 to 1278
(CFlags←A', Brlo←A, BrHi←A, LoadTestSyndrome, or ProcSRN←B); or 1608 to 1678
(B←Faultlnfo', B←Pipei, or B←Config').

The IFU is not prevented from making references while the processor is experiencing Hold,
unless the instruction being held is making a reference or doing one of the functions
mentioned above.

### 5.4. Memory Timing and Hold

Memory system control is divided into three more or less autonomous parts: address, cache
data, and storage sections. The storage section, in turn, has several automata that may be
operating simultaneously on different references. Every reference requires one cycle in the
address section, but thereafter an io reference normally deals only with storage, a cache
reference only with the cache data section. Address and cache data sections can handle
one reference per cycle if all goes well. Thus, barring io activity and cache misses, the
processor can make a fetch or store reference every cycle and never be held.

If the memory is unready to accept a reference or deliver Md, it inhibits execution with hold
(which converts the instruction into a GotG[.] while freezing branch conditions, dispatches,
etc.). The processor attempts the instruction again in the next cycle, unless a task switch
occurs. If the memory is still not ready, hold continues. If a task switch occurs, the
instruction is reexecuted when control returns to the task; thus task switching is invisible to
hold.

In the discussion below, cache references are ones that normally get passed from the
address section to the cache data section, unless they miss (fetches, stores, and IFU
fetches), while storage references unconditionally get passed to storage (IOFetchc-,
IOStore←, Map←, FlushStore ariSing from Flush← with dirty hit, and dirty-victim writes).
PreFetch← and DummyRef← don't fall into either category.

Situations When Hold Occurs

A fetch, store, or ←Md is held after a preceding fetch or store by the same task has missed
until all 16 words of the cache entry are loaded from storage (about 28 cycles).

Store← is held if DBuf is busy with data not yet handed to the cache data or storage
sections. LongFetch← (unfortunately) is also held in this case. Since DBuf is not task
specific, this hold will occur even when the preceding Store← was by another task.

<!-- PDF page 49 / printed page 42 -->

An immediate ~ Md is held in the cycle after a fetch or store, and in the cycle after a
deferred ~Md.
Because the task-specific Md RAM is being read t2 to t3 for the deferred ←Md in the preceding
cycle, and to to t1 for the immediate ←Md in the current cycle, which are coincident, hold is
necessary when the tasks differ. Unfortunately, hold occurs erroneously when the immediate and
deferred ←Md's are by the same task.

Any reference or ←Md is held if the address section is busy in one of the ways discussed
below.
←Md is erroneously held when the address section is busy, an unfortunate consequence of the
hardware implementation, which combines logic for holding ←Md on misses with logic for holding
references when the address section is busy.

8~Pipei is held when coincident with any memory system use of the pipe. Each memory
system access uses the pipe for one cycle but locks out the processor for two cycles. The
memory system accesses tt,e pipe t2 to t4 following any reference, so 8~Pip~i will be held
in the instruction after any reference. Storage reads and writes access the pipe twice
more; references that load the cache from storage access the pipe a third time.

Map 4- , LoadMcr, LoadTestSyndrome, and ProcSRN~ are not held for MapBuf busy; the
program has to handle these situations itself by polling MapBufBusy or waiting long
enough, as discussed in the Map section.

Flush~, Map~, and DummyRef~       are not held until a preceding fetch or store has finished
or faulted. The emulator or fault task should force Hold with ~Md before or coincident with
issuing one of these references, if it might have a fetch or store in progress.

In the processor section, stack overflow and underflow and the hold simulator may cause
holds; in the control section TaskingOff or an IFUJump in conjunction with the onset of one
of the rare IFU error conditions may cause one-cycle holds; there is also a back panel
signal called ExtHoldReq to which nothing is presently connected-this is reserved for
input/output devices that may need to generate hold in some situation. All of these
reasons for hold are discussed in the appropriate chapters.

Address Section Busy

The address section can normally be busy only if some previous reference has not yet
been passed to the cache data section (for a cache reference that hits) or to storage (for a
storage reference, or a cache reference or PreFetch4- that misses). A reference is passed
on immediately unless either its destination is busy or the being-loaded condition discussed
below occurs.

The address section is always busy in the two cycles after a miss, or in the cycle after a
Flush~, Map~, IOFetch~, or IOStore~.

Hardware note: This allows Asrn to advance; for emulator and fault task fetch and store misses,
which do not use Asrn, this hold is unnecessary. Unfortunately, the display controller's word task
finishes each iteration with IOFetch← and Block, so many emulator fetches and stores will be held
for one cycle when a high-bandwidth display is being driven. Asrn is the internal register that
contains the pipe address for storage references.

<!-- PDF page 50 / printed page 43 -->

There are six other ways for the address section to be busy:
(1) A cache reference or PreFetch.- that misses, or a FlushStore, transfers storage
data into the cache. At the end of this reference, as the first data word arrives,
storage takes another address section cycle.
(2) The preceding cache reference hit but cannot be passed to the cache data section
because the data section is busy transferring munches to/from storage (or to an io
device if an IOFetch.- finds dirty data in the cache). Total time to fetch a munch
from storage is about 28 cycles, but the cache data section is busy only during the
last 10 of these cycles (9 for PreFetch or IOFetch.- with dirty hit), while data is
written into the cache. The cache data section is free during the interim.
(3) The preceding storage reference, or cache reference or PreFetch'- that missed
has not been passed on to storage because the storage section is busy. Storage
is busy if it received a reference less than 8 cycles previously, and may be busy
longer as follows:
successive cache references must be 10 cycles apart;
successive write references must be 11 cycles apart;
with 4k storage ic's, successive references must be' 13 cycles apart.
(4) A cache write (caused by a miss with a dirty victim or FlushStore) ties up the
address section until the storage reference for the write is started; this happens 8
cycles after the storage reference for the miss or FlushStore is started. Note that
the new munch fetch starts before the dirty victim store and that hold terminates
right after the store is started.
(5) A reference giving rise to a cache write that follows any other cache miss will tie
up the address section until the previous miss is finished.
(6) The address section is busy in the cycle after any reference that hits a cache row
in which any column is being loaded from storage.
Any reference except IOFetch'-, DummyRef.-, or Map'- that hits a cache row in
which any column is being loaded from storage remains in the address section
until the 8eingLoaded flag is turned off-i.e., for the first 19 of the 28 cycles
required to service a miss, the reference is suspended in the address section;
during the last 9 cycles of the miss, when the munch is transferred into the cache
data section, the reference proceeds (except that a fetch or store will still be held
because the cache data section is busy during these 9 cycles). This is believed to
be very infrequent.
A more perfect implementation would suspend a reference in the address section only
when the hit column. rather than any column in the row, was being loaded. However, the
situation is only costly when the suspended reference is by another task; since there are
64 rows, ~ 1.5% of all references will be held whenever any task is experiencing a miss.
There is more discussion of this in the "Performance Issues" chapter.

References to storage arise as follows:
a cache miss (from a cache reference or PreFetch.-) causes a storage read;
a cache reference or PreFetch'- miss with dirty victim also causes a storage write
immediately after the read;

<!-- PDF page 51 / printed page 44 -->

a Flush ← which gets a dirty hit causes a FlushStore read reference which in turn
causes a storage write of tile dirty victim;
every io reference causes a storage read or write;
a Map← causes a reference to storage (actually only the map is referenced, but the
timing is the same as for a full storage reference).

The following table shows the activity in various parts of the memory system during a fetch
that misses in the cache and displaces a dirty vi~Um; the memory system is assumed idle
initially and nothing unusual happens.

> **Table 15: Timing of a Dirty Miss**

![Table 15: Timing of a Dirty Miss](tables/table-15-dirty-miss-timing.png)

STOP! The sections which follow are about the Map, Pipe, Cache, Storage, Errors, and
other internal details of the memory system. Only programmers of the fault task. or
memory system diagnostic software are expected to require this information. Since there
are many complications, you are advised to skip to the next chapter. .

### 5.5. The Map

VA is transformed into a real address by the map on the way to storage. The hardware is
easily modifiable to create a page size of either 256, 1024, or 4096 words and to use either
16k, 64k, or 256k ic's for map storage. The table below shows the virtual memory (VM)
sizes achievable with different map configurations. However, the cache configuration limits
VM size independently, as discussed later, and this limit may be smaller than the Map limit.

<!-- PDF page 52 / printed page 45 -->

> **Table 16: Map Configurations**

![Table 16: Map Configurations](tables/table-16-map-configurations.png)

Larger page sizes increase the virtual memory size limit. Since the 4k-"Yord cache imposes
a 225-word size limit (226 if the parity bit in th~ address section is converted into another
address bit), the largest VM sizes are only achievable in conjunction with a 16k-word
cache. Larger page sizes might reduce map and storage management overhead; our
experience in this area is inconclusive but suggests that 4k-word pages would only be
desirable with very large storage configurations.
o

Note that the physical storage size limit is unaffected by either cache parameters, map
RAM size, or page size because RP is large enough to address the largest possible storage
configuration (4 modules using 218-bit MaS RAM components), even when the smallest
page size is used; this maximum size is 224 words.

The cache handles virtual addresses, so the map is never involved in cache references
unless they miss.                    .

A consequence of virtual addresses in the cache is that it is illegal to map several virtual
pages into the same real page (unless all instances are write-protected). This restriction
prevents cache and storage from becoming inconsistent.

A map entry contains a 16-bit real page number (AP) and three flags called Dirty, Ref, and
WP, which have the following significance:

WP           write-protects the page; a fault occurs if a write is attempted.

Dirty        indicates that storage has been modified; set by any IOStore'" or by a
dirty-victim write; Store'" does not set Dirty.

Ref          indicates that the page has been referenced; set by any storage reference
except Map"'; cleared by Map ....
.
The combination WP =true with Dirty =true makes no sense, and encodes the Vacant state
of the map entry. A map entry is vacant if it has no corresponding page in real memory.

<!-- PDF page 53 / printed page 46 -->

Faults
Every storage reference causes a mapping operation. If mapping reveals something other
than Vacant, tile reference proceeds normally. Otherwise, the storage reference is aborted,
and MapTrouble is reported as discussed later. There are two kinds of faults:
Page fault     reference to a vacant map entry (WP = true, Dirty = true)
WP fault       Store" that misses, IOStore", or dirty-victim write with WP true.
(Dirty-victim WP faults should not occur if the map and cache are
handled as proposed below.)

Writing the Map

Map", which can only be encoded in an emulator or fault task instruction, is used to write
the map; like other storage references, it returns previous map contents in the pipe, where
they can be read. For reasons discussed in "The Pipe" section later, Map" should not be
issued if a preceding fetch or Store" might be in progress; normally issue a "Md to hold
until preceding references cannot fault.

Map" first writes B[0:15] and TIOA[0:1] into MapBuf (a buffer register on the MemX board)
and turns on MapBufBusy in the pipe; 9 cycles later (barring delays) MapBuf has been
written into the Map entry addressed by the appropriate bits of VA and MapBufBusy is
turned off.

B[0:15] are the real page number, TIOA.O is WP, and TIOA.1 is Dirty. Map" zeroes Ref,
and there is no direct way to write a map entry with Ref= 1; a fetch, Store", or PreFetch ..
to the appropriate page after loading the map entry will set Ref = 1. Note that if the real
address space is less than 224 words (2 16 pages), high order RP bits are ignored during
references though they are kept in the map and appear in the pipe.

Map" never wakes up the fault task.
If previous map contents indicated Vacant or had a parity error, Map Trouble will be true .in the pipe
but not reported to the fault task. Quadword and syndrome in the pipe, not written by Map4-,
contain previous values.

For all programming purposes, Map" is complete on the cycle when MapBufBusy is turned
off; at this time, previous map contents are valid in the pipe entry. Polling MapBufBusy
until it is false is the only way to find out when the pipe entry is valid.

Since Map .. never faults and doesn't use any pipe information clobbered by an overlapping
reference, another reference may be started without waiting for Map" to finish, unless the
following reference is another Map 4- • Also, Map← does not interfere with Md or DBuf, so
its only interference with other kinds of references is its use of the private pipe entry (0 or
1) pointed at by ProcSRN. However, it is illegal to issue another Map←, LoadMCR,
LoadTestSyndrome, or ProcSRN← without waiting for Map← to finish. These functions
(discussed later) share the MapBuf register with Map←; there is no Hold arising from
MapBufBusy, so the microprogram must ensure that MapBuf is free when one of these
functions is executed.

<!-- PDF page 54 / printed page 47 -->

8 is latched in Map8uf during t2 to band TIOA[O:1] are clocked into Map8uf at t2 for all of these;
then Map8uf is written into MeR, TestSyndrome, or ProcSRN at t3 or into the Map at t14 (if no
delays). In other words, Map8uf is a buffer register for all registers on the MemX board that are
loaded from 8.

Reading the Map

Every storage reference causes mapping and returns old contents of the relevant map entry
in the pipe. I.e., Ref and Dirty may change as a re~ult of a reference-old values appear in
the pipe.                                         .

A ReadMap function accompanying Map← prevents the map entry from being modified, so
that old contents can be read from the pipe without also smashing the map entry.

Flushing One Page From the Cache

Any cache reference or PreFetch← that misses or any IOFetch← or IOStore←sets Ref in the
map; IOStore←'s set Dirty as well. If the victim for the miss or hit for the Flush← is dirty,
Ref and Dirty for its map entry also get set. However, Store← does not set Dirty in the map
entry until that munch is chosen as victim.

For this reason, any calculation based upon Dirty must first validate the map Dirty bit by
flushing associated cache entries, as discussed below.

In addition, almost any change to a map entry requires a flush, again because of problems
with dirty cache entries. The following examples illustrate this point:
Before changing RP, a flush prevents dirty victims from being written into the
previous real page (if the old page had WP false).
Before turning on WP, a flush prevents dirty cache entries from being written into
the now write-protected page.
Before turning off WP, a flush prevents multiple cache entries for a munch, one
write-protected, the other not (The cache will not work correctly, if there are
multiple entries for a munch.).
Before sampling Ref, flushing is required so that subsequent references to the
page will set Ref =1 and so that dirty munches in the cache will not erroneously
set Ref =1 when they are displaced.
Before clearing Dirty, a flush prevents dirty munches subsequently displaced from
the cache from erroneously setting Dirty again.

To flush a 256-word page from the cache, 16 Flush← references may be made, one to each
munch of the page (64 with 1024-word pages). Flush← invalidates any existing cache entry
for the munch (and stores the munch if dirty).
This succeeds iff there are no anomalous multiple cache entries for a particular munch. Multiple
cache entries for a munch should never occur except prior to system initialization or when some 01
the debugging features are turned on in Mcr.

<!-- PDF page 55 / printed page 48 -->

Flushing the Entire Cache

Depending upon what kind of storage management algorithm is used, it may be desirable to
clear out the entire cache; for example, this might be useful before looping through all the
map entries to sample Ref. It would be extremely expensive to do this with Flush ← one
page at-a-time (2 16 Flush←'es for IM words of storage). The cleverest method which we
have thought of for doing this is as follows:

Designate 4 consecutive 256-word pages (64-row cache) or 16 consecutive pages (256-row
cache) that contain vacant map entries; the munch VA's in these pages will span every row
in the cache. Make one pass through the cache for each column; before each pass, load
Mcr with UseMcrV true and McrV equal to the column-even though it is usually illegal to
modify Mer while the memory system is active, it should be safe to change these particular
fields. Then do PreFetch←'es for all 64 or 256 munches in the designated pages; these
PreFetches will all miss and map fault, leaving the selected column filled with vacant cache
entries. After clearing all four columns, restore Mcr to its previous value. 'v',(hile this flush
is going on, io tasks may continue to reference memory, but they will experience more
misses and longer miss wait than usual. The total time for this algorithm is about 9
cycles/PreFetch or about 138 p.s with 64 rows or 553 p.s with 256 rows in the cache at 60
ns/cycle.

Map Hardware Details

The map and its control are on the MemX board. Physically, map storage consists of 21 16k, 64k, or 256k x 1
MOS RAM's; in addition to the 19 bits discussed earlier, there are a duplicate of the Dirty bit and an odd parity
bit.

Dirty is duplicated so map parity won't change when both' Dirty bits are set. Ideally Ref should also be
duplicated, but it is not, and Ref is not checked by map parity. The parity written on Map" is the exclusive-or
of the two 8 byte parity bits and TIOA.O (Le., WP). Parity failure on any map read will cause Map Trouble and
MemError and wakeup the fault task when appropriate.

On a cache reference that misses or on an 10Fetch" or 10Store", the map read starts at t4 and the real
address is passed to storage at t14.

The MOS RAM's in the map require refresh, carried out like the storage refresh discussed later.

### 5.6. An Automatic Storage Management Algorithm

We envision for Mesa, Lisp, etc. an automatic storage manager that will pick pages in storage which have not
been referenced recently for replacement by new ones. This manager will use the Ref bits in map entries to
determine which pages have not been referenced for a long time.

The storage manager discussed here controls N pages, where N is some subset of all pages in storage; in
general N will vary. A procedure called DeliverPage() returns RP for one of the N pages to the caller and
removes that page from N; a procedure called ManagePage(RP,Age) adds a page to N. A page returned by
DeliverPage has been removed from the virtual space; pages accepted by ManagePage mayor may not be
vacant.

Entries for the N pages are sorted into 8 bins, such that entries in the bin 0 have age 0, bin 1 age 1, etc.
Whenever DeliverPage has been called NIB times or after a specified elapsed time has occurred, all N pages
are aged, which means:                                                          •

(a) Entries originally in bin 7 wind up in bin 0 if they have been referenced, bin 7 if not referenced;

(b) Entries in bin i (i #    7) wind up in bin 0 if referenced or bin i + 1 if not referenced.

<!-- PDF page 56 / printed page 49 -->

This aging is performed by first clearing the entire cache using the clever algorithm discussed earlier, then
sampling and zeroing Ref.

DeliverPage first returns the RP of any page on the vacant queue. If the vacant queue is empty, it next scans
entries on the disk write-complete queue; if one is found that has not been referenced in the interim, its map
entry is cleared and its RP is returned; if referenced, it is moved to bin O. If the disk write-complete queue is
empty, entries in bin 7 are scanned; if this bin is exhausted, bin 6 is scanned, etc., until finally bin 0 is
scanned. When an entry has been referenced, it is moved to bin 0; when unreferenced but dirty, it is put on
the disk write queue; when unreferenced and clean it is returned.

The caller of DeliverPage will frequently be a disk read or new page creation procedure. It should complete its
work and then call ReturnPage(RP,O) to restore the page to the storage manager. ReturnPage will put the
page on the vacant queue, if it is vacant, or into bin O.

### 5.7. Mesa Map Primitives

Basic Mesa mapping primitives are:

Associate[vp,rp,flags] adds virtual pages to the real memory, or removes them if flags = Vacant.

SetFlags[vp,flags] RETURNS oldValue: flags reads and sets the flags.      If flags = Vacant, the page is
removed from the real memory.

GetFlags[vp] RETURNS [rp,flags].

These are defined as indivisible operations and are implemented trivially on a machine with. no cache (e.g.,
Dolphin). For example, if a SetFlags clears Dirty and sets WP, the returned value of Dirty tells correctly
whether the page has been changed-no store into the page may occur between reading Dirty and setting WP.

One intended use of the primitives is illustrated by the following Mesa sequence for removing a virtual page
from real memory:
oldFlags fo SetFlags[v,WP];
IF oldFlags.Dirty THEN WritePage[...]
SetFlags[v,Vacant]

This sequence prevents the page from being changed during the write. Another possibility would be just to
clear Dirty, and then to check it again after the write. This must be done properly, however, to avoid a race
condition:
WHILE (oldFlagsfoSetFlags[v, Vacant]).Dirty
DO SetFlags[v, [Dirty: false]]; WritePage[...]; ENDLOOP

To avoid inconsistent map and cache entries, SetFlags[v, ... ] must remove entries for page v from the cache.
Unfortunately, since we don't want to make the cache removal process atomic, parts of the page already
passed over by the removal process could be brought back into the cache before the process is complete.
The implementation of the primitive must allow for this.

On Dorado it is, unfortunately, difficult to implement these primitives as indivisible operations because almost
any change to map flags must be preceded by clearing all cache entries in the page. However, it is
unacceptable to do this with TaskingOff because the time required might be as long as 16*10 cycles with 256-
word pages or 64*10 cycles with 1024-word pages (if every munch in the page is in the cache and dirty),
which is too long. Consequently, io tasks will be active during the removal process, and one of them might
move a munch back into the cache after it has been passed over by the removal process. For this reason, the
present Mesa code flushes once with tasking on and then again with tasking off.

The implementation of SetFlags(v, ...) proceeds as follows (Associate is similar.):
Flush all cache entries for the page in question.   If any entry is dirty, removal will cause a write and
set Dirty in the map, as discussed earlier.
Disable tasking.
Flush all cache entries for the page again.

<!-- PDF page 57 / printed page 50 -->

Dorado Hardware Manual                    Memory Section                   14 September 1901             50

old Flags ← map[v).Flags
If turning on WP:      map[v].flags ← [WP: true, Dirty: false, Ref: false).
If setting Vacant:   map[v].flags ← [WP: true, Dirty: true, Ref: false].
If turning off WP:     map[v].flags ← [WP: false, Dirty: false, Ref: false).
These are done with Map← after which old data is retrieved from the pipe (possibly followed by
PreFetch to set map[v].Ref true).

Note: These primitives do not support the complete cache flush discussed earlier; another primitive will
probably needed to do this. Also, we really want a primitive. that will allow the flags to be sampled and Ref
zeroed without changing the value of WP or Dirty. And efficiency may demand primitives particularly tailored
to the needs of whatever storage management algorithm is employed.

<!-- PDF page 58 / printed page 51 -->

### 5.8. The Pipe

Information about each reference is recorded in the 16-word pipe memory. Pipe layout is
shown in Figure 10, which you should have in front of you while reading this section. The
processor reads the pipe with the B←PipeO, ... , B←Pipe5 functions. You should note that
PipeD, 1, and 5 are read high-true, while Pipe2 and 3 are read low-true; Pipe4 contains a
mixture of high and low-true fields; 150361 8 xor Pipe4' produces high-true values for a/l
fields in Pipe4. The discussion in this section assumes that aff low-true fields have been
inverted.

It is illegal to do ALU arithmetic on pipe data (not valid soon enough for carry propagation).
and B←Pipei is illegal in the same instruction with a reference because Hold won't be
computed properly.
The EmulatorFault, NFaults, and SRNFirstFault stuff in Pipe2, which duplicate what Bf-Faultlnfo would
read back, is not part of the pipe, although it is read by Bf-Pipe2'; Bf-Pipe2' is simply a convenient
decode for reading it bacfo.-this will be discussed in the section on fault handlinQ., not here.
Similarly, Dirty, Vacant, WP, BeingLoaded, NextVictim, and Victim stuff in PipeS is not part of the
pipe and is read back by Bf-PipeS purely for decoding convenience. This information, used primarily
for debugging, is discussed later.

The Task, Sub Task, VA, and cache control stuff in PipeO, 1, 2, and 5 is used both internally
by the memory system and externally by the processor. Map and error stuff in Pipe3 and 4
is solely for memory management and diagnostic activities carried out by the processor.

Two main problems in dealing with the pipe are:
Finding the pipe entry for a particular. reference;
Knowing when various bits become valid;

How the Pipe Is Addressed

System microcode is expected to use the pipe in only two situations: fault handling by task
15 (the "fault task") and reading map or base registers by task 0 (the "emul~tor"). Other
tasks will not read the pipe. This rigid view of how the pipe will be used during system
operation has motivated the implementation discussed below.

Pipe entries are addressed by 4-bit storage reference numbers, or SRNs, assigned to each
storage reference. All task 0 and task 15 references except PreFetch← with miss (and
implicit FlushStore and Victim references) use the SRN contained in ProcSRN exclusively;
all other references share SRN's 2 to 15, which form a ring buffer addressed by an invisible
register called ASRN.

To read a pipe entry, first ProcSRN←B addresses the pipe entry, then the contents of that
entry are read with B←Pipei. In system microcode, the emulator is expected to keep the
value 0 in ProcSRN to avoid smashing the ring buffer on references; if the fault task needs
to make a reference, it will normally load ProcSRN with 1 and. use that SRN for the
reference; the fault task will manipulate ProcSRN however it likes to examine the pipe but
always restore it to 0 before blocking; other tasks will not use ProcSRN.              This
implementation is welded to the assumption that only the fault task will probe the pipe
when io tasks are running.

<!-- PDF page 59 / printed page 52 -->

To io task references and emulator PreFetch←'es that miss, the cache address section's
SRN, called ASRN, is assigned at t2 . ASRN will be advanced to the next ring value iff the
reference starts the map. In all other cases ASRN remains unchanged and is used by the
next reference as well.

A reference starts the map unless it is a DummyRef←, a cache reference or PreFetch← that
hits, or a Flush← that misses or gets a clean hit. A convenient way to guarantee that the
map is started without worrying about the contents of the cache is to do a Map← .in the
emulator or an 10Fetch← in any other task. The reasoning behind this treatment of ASRN
is explained in the section on fault reporting.

Tasks 1 to 14 generally cannot find out the SRN for their last reference. Even if this were
determined somehow by polling all the pipe entries, there would be no assurance that,
meanwhile, a higher priority task didn't clobber the pipe entry.

Because of its single pipe entry, the emulator must wait for an earlier reference to finish or
fault, before starting another. Of all emulator references, only a fetch, Store←, or
PreFetch← might fault. However, PreFetch← doesn't use the private pipe entry, so only a
preceding fetch or Store← might still be in progress when a new reference is issued. If the
new reference is another fetch or Store←, it will hold until the preceding one finishes (no
problem). Hence, the only restriction imposed by the private pipe entry is that" the emulator
must cause hold with ←Md before issuing Map←, Flush←, or DummyRef←, if a fetch or
Store← might still be in progress.
Timing constraints do not permit generating Hold in the above case. It has been observed that
issuing Map" without holding for a previous Store .. to finish will result in infinite DBufBusy (i.e.,
infinite Hold), so do not fail to issue "Md before or concurrent with Map← or RMap".

When the Pipe is Accessed

Conceptually, the pipe is three different memories. First, VA, task, subtask, and cache
control bits in PipeO, 1, 2, and 5 are written during the reference. Next, the 20 bits of map
information in Pipe3 and Pipe4 are written following the map read-write (if any). Finally, the
error correction-detection stuff in Pipe4 is written following the storage read (if any). The
memory system needs one cycle for each of these accesses.

However, the hardware treats the pipe as only two separate memories internally, or as only
a single memory for purposes of holding the processor. In other words, within the memory
system PipeO, 1, 2, and 5 may be accessed by one part of the pipeline, while another part
independently accesses Pipe3 and 4. But processor accesses by B←Pipei are held, if the
memory system wants any part of the pipe. Worse, the memory system uses the pipe
between even clocks (to to t2), the processor between odd clocks (t1 to ~), so the
processor is locked out for two cycles during each of these intervals.

Programs can safely read PipeO, Pipe1, Pipe2, or Pipe5 (i.e., task, subtask, VA, and cache
control stuff) in the cycle after any reference, since these are updated at the end of the
cache address section cycle. B←Pipei in the cycle after a reference will hold for one cycle
while the memory system uses the pipe.

Values in a pipe entry are not reset at the onset of a reference and Pipe3 and Pipe4 are
not written at all unless storage is accessed. Consequently, Pipe3 and Pipe4 may refer to a

<!-- PDF page 60 / printed page 53 -->

previous reference '* Caution '*.

The control bits in Pipe2' and Pipe5, used by the memory system, also indicate (to the fault
task) what kind of reference is described in the pipe, as follows:

CacheRef            a fetch or Store←
Store'              Store←-
IFURef              IFU fetches
RefType             distinguishes read, write, M~p←, and other references
FlushStore          dirty victim write triggered by Flush←     .
ColVic              cache column of a hit, or of the victim on a miss

DummyRef4- finishes immediately and only VA in PipeD and Pipe1 and the stuff in Pipe2 are
relevant. For Flush←, cache information in Pipe5 is also valid. Flush← finishes immediately
because the resulting FlushStore and dirty-victim write references (if any) are started in
ring-buffer pipe entries.

Programs can read map stuff (Pipe3 and Ref, WP, Dirty, Map Trouble, and MemError in
Pipe4) as soon as that part of the reference is complete. For Map←, completion of the map
read is coincident with MapBufBusy going false, determined by polling. For a fetch or
store, there is no way to distinguish completion of the map read from completion of the
entire reference. Consequently, Pipe3 and Pipe4 are normally read by doing ←Md (which
holds for completion), then reading the pipe.

For IOFetch←, IOStore←, and PreFetch← there is no way to tell when the reference has
finished, except by waiting longer than the memory can possibly take to complete the
reference.

IOStore←'s and dirty victim writes zero the Syndrome and EcFault fields in Pipe4. Hence,
the only reference that leaves junk in these bits is Map←; the fault task can distinguish pipe
entries for Map← by means of the RefType field.
All data in PipeO, 1, 2, and 5 except FlushStore and ColVic are written at     ta,
and can be read
immediately after a reference. However, FlushStore and ColVic are written at t 4 , Ordinarily, this
would mean that their values could not be read safely; however, since B←Pipei is held in the cycle
after a reference, the values will always be ok.

In the best case, map information in Pipe3 and Pipe4 will be loaded at t14, fault and error corrector
information in Pipe4 at t48'

### 5.9. Faults and Errors

Remember that high-true values for all fields in the Pipe are used in the following
discussion.

Errors

Several events cause memory errors from which the system does not recover. Errors halt
the processor if the MemoryPE error is enabled (see "Error Handling"). If MemoryPE is
disabled, the program will continue without any error indication. MemoryPE conditions are:

<!-- PDF page 61 / printed page 54 -->

Byte parity errors from the cache data memory (checked on write of a dirty victim, not
on ←Md or IFU reads); the processor checks Md parity (see "Error Handling") and the
IFU checks FIG parity;
Byte parity errors from fast input bus;
Cache address memory parity errors.

Faults

Other events cause faults. A fault condition is indicated in the Map Trouble, MemError, and
EcFault fields of Pipe4 when it occurs; in addition, the fault task is woken to deal with the
situation unless No Wake is true in Mcr. The encoding of the various errors is as follows:

> **Table 17: Fault Indications**

![Table 17: Fault Indications](tables/table-17-fault-indications.png)

In the above table, WPFIt and PageFIt have the same encoding; these must be distinguished
by means of the Store' bit in Pipe5 and the WP. bit in Pipe4; WPFIt can only occur for
Store←, 10Store←, or dirty-victim stores that encounter WP true .

. MapTroubJe might be true and reported to the fault task on a fetch or store that misses or
an 10Fetch←, 10Store←, FlushStore, or dirty-victim write. Flush← and DummyRef← never
cause MapTroubJe. Map←, PreFetch←, or IFU fetches might record Map Trouble in the pipe
but never wake the fault task. Map faults on IFU fetches are reported instead to the IFU,
which buffers the fault indication until an IFUJump occurs to an opcode with at least one
instruction byte in the word affected by the map fault; then a trap occurs, as discussed in
"Instruction Fetch Unit".
In system microcode, we expect a WPFIt and PageFJt due to IOFetchfo , IOStorefo , FlushStore, or a
victim write to indicate a programming error; however MapPE might occur. Note that if any kind of
MapTroubJe occurs on a storage write (Le., on an IOStorefo , FlushStore, or victim write), storage is
not modified and contains the old value; however, the map's Dirty bit will be true, even though the
storage write has not completed.

SE and DE may occur on any cache reference or PreFetch← that misses or on an
. 10Fetch←. Map←, 10Store←, DummyRef←, and Flush← never cause these errors. Also note
that fault task wakeup on an SE requires not only NoWake false but also ReportSE true in
Mcr; the fault indication transmitted with the munch for an 10Fetch← is set only for DE,
never for SE.
Unlike map faults, data errors on IFU fetches and PreFetchfo'es are reported to the fault task. This
must be done for DE's, which are fatal; for corrected SE's, the fault causes no disruption to the
program because the fault task, after logging the failure, simply lets the task that faulted continue.

<!-- PDF page 62 / printed page 55 -->

The special things about a fault are:

If a program obeys the rules given earlier, hold will occur until any fault is reported
or until the program can proceed safely.
EmulatorFault in B←Faultlnfo is set true if a fault is described by the emulator or
fault task pipe entry (0 or 1) pointed at by ProcSRN;
FirstFaultSRN in B← Faultlnfo is loaded if FaultCnt is 1 (indicating no faults) or if
FirstFaultSRN was previously zero;
FaultCnt in 8←Faultlnfo is incremented;
8←Faultlnfo stuff is updated and the fault task is woken at the end of the storage
pipeline, but sufficiently in advance of hold termination that it will surely run first.
For this reason, any operation that might fault is illegal with tasking off.
References leave the pipeline in the order that they entered.
Subtleties: In the event of a miss with a dirty victim, the new cache entry read starts and
finishes before the victim write. However, data transport of the victim to storage finishes
before data transport of new data into the cache starts-storage actually reads new data
first. but meanwhile transports the victim into a holding register on the storage board. from
which it is written into storage after the read.

Pipe entries identified by EmulatorFault, FirstFaultSRN, and FaultCnt represent
complete storage references;
The task that faulted is not blocked; hold terminates as though no fault had
occurred; the task will continue unless the fault task changes its PC.

The fault task is expected to read B←Faultlnfo, service all faults it describes, service stack
underflow or overflow, then block. Because it is highest priority, the fault task cannot do
much computing (io tasks that are lower priority have to be serviced); probably it should
not make any memory references itself. Its normal actions are:

crash (uncorrectable data errors, map faults by tasks other than the emulator);

block letting the task that faulted continue (correctable data errors); or

change the TPC of the emulator to an appropriate trap routine (emulator map
faults, stack overflow or underflow).

EmulatorFault and FaultCnt are automatically reset by 8←Faultlnfo.                   These can be read
without reset in B←Pipe2 (primarily for use by' Midas).

Several faults could occur while the fault task is running (due to references initiated before
the fault task was awakened). In this case, when the fault task blocks, it will continue
because of the pending wakeup, and so service the faults. Only while the fault task is
running or while tasking is off is it possible for FaultCnt to become greater than one.

<!-- PDF page 63 / printed page 56 -->

Remarks

The careful scheme in which ASRN is advanced only for storage references, and fnults reported in precise
order is essential. If faults were reported out of sequence, then the fault task might see PipeO to Pipe2 stuff
inconsistent with Pipe3 and Pipe4 error indicators for a previous loop through the ring buffer.

The hardware must not and does not ;"eport MapTroub/e until the end of the pipeline. If this were not tr"ue,
then an SRN might report MapTroub/e before its predecessor reported SE or DE; this could screw up the fault
task.

In tasks other than the emulator, map faults will probably represent programming errors. In the emulator, page-
not-in-memory and write-protect faults are expected, and the" fault task will trap the emulator to a fault-handling
Mesa program. Information saved by the trap microcode must be sufficient to cOQtinue the faulted opcode at
the instruction that faulted.

The B←DBul FF decode permits the fault task to retrieve data being written when a Store← faults.

Error Correction Faults

For error correction purposes, munches are divided into four quadwords, each containing
64 data and 8 check bits.

At the end of a storage read, the hardware indicates DE after a double-error or SE after a
single error as discussed earlier. The SE or DE indication is unambiguous assuming at
most two bits in error in any 64-bit quadword; for an odd number of errors greater than 2,
the hardware erroneously reports an SE; for an even number of errors greater than 2, DE is
reported. If several quadwords in a munch suffer errors, the hardware reports the first DE,
if any, or the last SE, if no DE's.

Error correction can be enabled/disabled by the LoadTestSyndrome function discussed
later; when enabled, the hardware will complement (= correct) any SE; for DE's the
hardware does not modify any bits from storage.

The absolute address of the quadword containing the reported error is
RP[O:15} .. VA[24:27} .. quadword[0:1}        with       256-word         pages,        or
RP[2:15} .. VA[22:27} .. quadword(0:1] with 1024-word pages. I.e., the word address of the
first word in the quadword would be these 22 bits with two low-order zeroes appended.

SE and DE are derived from the 8-bit syndrome field in Pipe4. Syndrome = 0 means no
error; neither DE nor SE should be true in this case. Syndrome non-O with an odd number
of 1's should have SE indicated. Syndrome non-O with an even number of 1's or an invalid
word code (discussed below) should have DE indicated.

See Figure 11 for the correspondence between syndrome and bits within the quadword.

<!-- PDF page 64 / printed page 57 -->

For SE's, syndrome specifies exactly which of the 64 data bits or 8 check bits was in error.
If syndrome has a single one in it, then the corresponding checkbit was in error. When
syndrome contains more than one 1, then syndrome[4:6] indicate which word in the
quadword suffered the error as follows:

word 0      011
word 1      101
word 2      110
word 3      111

The other four values of syndrome[4:6] are impossible for an SE and are reported as a DE.

Syndrome[0:3] indicates the bit position within that word; unfortunately these bits are
reversed, so that the bit number is given when the bits are taken in the order 3, 2, 1, O•
. Syndrome[7] is the parity of the syndrome, and a double error is indicated by a non-zero
syndrome having even pa~ity.
Storage writes leave garbage in the EcFault and SY!1drome fields of the Pipe; the fault task must
distinguish these cases by means of the RefType field in Pipe2.

As discussed below in the "Testing" and "Initialization" sections, TestSyndrome is xor'ed with check
bits that would otherwise be written on storage writes. This means that Syndrome-of-read equals
TestSyndrome-of-write is an exact indication of no-error. However, the hardware always reports non-
zero syndrome as an error, as discussed above, regardless of what's in TestSyndrome.

Dirty is set in the cache after a Store'" that misses, despite any fault, so when that munch
is chosen as victim, it will be written back into storage. Consequently, if the fault task
attempts recovery from a double error on a Store ...., it may have to clear the cache address
section's Dirty bit for the munch using the tricky sequences discussed later.

### 5.10. Storage

Storage is organized into modules consisting of two identical boards per module. The
modules appear in the chasis as shown in Figure 2. Depending on whether 1.6k-bit or 64k
bit IC's are used, a module stores 256k or 1m 64-bit (+ 8 check bit) quadwords. A Dorado
can have up to 4 modules, for a maximum of 16m words. Every module must be the same
size-it is illegal to mix module sizes.

The module in slot 0 supplies the first quarter of real memory; slot 1, second quarter; slot
2, third quarter; and slot 3, fourth quarter. In other words, real memory addresses are not
interleaved among modules and the address range covered by a particular module cannot
be controlled by the firmware.

The S"'Config' function (Figure 10) returns MO, M1, M2, and M3 which are true only when
a module is plugged into the corresponding storage board slot. ChipSize indicates what
size ic's are used on the storage boards. The memory system automatically adjusts itself to
operate according to the IC size in use on the storage boarods.

When 256k x 1 MOS storage ic's become available, we plan to replace the 4k and 16k
wires on the backplane by an extra address wire and a 256k wire; at this time we will lose
the ability to handle 4k x 1 and 16k x 1 ic's and the hardware will allow either 64k or 256k
storage ic's to be used.

<!-- PDF page 65 / printed page 58 -->

MOS RAMs used on storage boards (and in the map) must be refreshed at regular
intervals, else they drop data. This occurs during refresh references once every 16 p.s.
Every MOS RAM on every storage board participates in every refresh reference, and one
row of data is refreshed each time. This means that 64 (4k RAMs), 128 (16k RAMs), or 256
(64k RAMs) refresh references are required to refresh all data (So the refresh period is 2 or
4 ms-the specification on both 16k and 64k RAMs is a 2 ms refresh period at the
maximum operating temperature (85° C). The dominant leakage term is exponential in
temperature so the refresh period can be doubled each 5° C drop in operating
temperature. Because the specification is conservative and because we have no intention
of operating anywhere near 850 C, a 4 ms refresh period should be adequate.

The time for each refresh reference is 8 cycles (13 cycles with 4k-bit RAMs), same as
normal references. Refresh hardware competes for storage access with the cache data
section and fast io references. During the first 8 p,s of a 16 p,s period, refresh defers to
normal references; during the last 8 p,s, it preempts normal references.

### 5.11. The Cache

The physical cache structure consists of 256 entries in an array of 64 rows by 4 columns.
Each entry holds 15 address bits, a parity bit for the address bits, four flag bits, and one
munch of data (= 256 data bits + 32 check bits). Hence, the cache holds a total of 4k
words of data.
The address section is implemented with 256-word RAM's, but only 64 words are presently used.
The data section uses 1kx1 RAM's for storage. When sufficiently fast 4kx1 EeL RAM's become
available, we plan to use them in the cache data section and utilize all 256 words in the address
section. In this case, the cache geometry will be 256 rows by 4 columns (16k words in the data
section).

The cache address section stores 4 flag bits discussed below, 15 VA bits, and 1 parity bit.
The way the VA bits are assigned depends upon whether or not 4k x 1 ECl RAM's are
used in the cache data section. VA[7:19] are stored in the address section for all
configurations. Two other bits are either VA[5:6] or VA[20:21]; VA[5:6] are used with 4k
ic's in the cache data section (VA[20:21] then appear in the row address of the cache, so
they don't have to be stored). The hardware is also arranged so that the parity bit may be
replaced by VA[4].

In other words, the cache initially implements a 225 word virtual memory with provision for
expanding this to 227 words when 4k x 1 RAM's are available or to 228 words at the cost of
eliminating the parity bit in the address section. However, the map organization also limits
virtual memory, probably to a smaller size than the cache limit, as discussed earlier.

Normally, the cache is invisible to the programmer except for problems with map/cache
consistency discussed in the map section.      However,    features discussed below in
"Testing" allow more direct access for checkout, initialization, and error recovery.

An address VA, if in the cache at all, must be in one of the four columns of the row
addressed by VA[22:27] (or VA[20:27] if the cache is expanded). References compare the
appropriate 15 or 16 bits of VA[4:21] with the values stored in each of the 4 columns to
determine which cache entry, if any, contains VA.

<!-- PDF page 66 / printed page 59 -->

The VNV memory contains two two-bit entries for each row of the cache. The Victim field
specifies the cache column displaced if a miss occurs in this row. The NextV field is the
next victim. When a miss or a hit in Victim occurs, Victim←NextV is done. When a miss,
hit in Victim, or hit in NextV occurs, NextV←Victim.0'"NextV.1' is done (Le., NextV is loaded
with a value different from both the original NextV and Victim). This strategy is not quite
LRU, since there is a 50-50 guess on the ordering of the third and fourth replacements.
This treatment of VNV is used for fetches, Store←, PreFetch←, and IFU fetches but not for
10Fetch←, 10Store←, or Map←, which don't use the cache.

On a Flush←, Victim is written with 0 on a miss or with the column .of the hit and NextV is
written with Victim.O'"NextV.1'. If the Flush← hit a dirty cache entry, then a FlushStore
reference is fabricated which will wind up writing Victim (= column hit by the Flush←) back
into storage.          The FlushStore reference will also do Victim←NextV and
NextV ← Victim.O' "NextV.1' again. This means that the VNV entry for the row touched by a
Flush← is effectively garbaged, which probably won't affect performance much.
A better strategy for Flush~ and 10Store~ would be as follows: On a miss, Victim and NextV remain
unchanged; on a hit in a column different from Victim, Victim~hit column, NextV~Victim; on a hit in
Victim. no change.

The UseMcrV feature discussed in "Testing" allows Victim and NextV to be replaced by MarV and
McrNV.

Associated with each cache en!ry are four flag bits that keep track of its state, as follows:
DirtY - set by Store←, cleared when loaded from storage.
This bit does not imply
anything about the map's Dirty bit. The cache Dirty bit causes a storage write
when the entry is chosen as victim, and the map's Dirty bit is set at that time.
Vacant - set by hit on Flush←, hit on 10Store←, or Store← into a write-protected
entry, cleared when the entry is loaded from storage. Vacant is not set after an SE
or DE. Vacant prevents the entry from matching any VA presented to the cache.
Write Protect - a copy of the map's WP bit. It is copied from the map when the .
cache entry is loaded and not subsequently changed. If a Store← is attempted into
a write protected entry, the entry is invalidated, there is a cache fault, and a write
protect fault will be reported by the map.
BeingLoaded . set while an entry is waiting for data from storage.   Any reference
that hits in the same row will remain in the cache address section until the bit goes
off; any reference or ←Md following the one which hit a row being loaded will be
held.

Remark

At the end of a miss, data from the error-corrector is loaded into the cache 16 bits/clock. Not until all 16
words of the munch have been loaded is Md loaded and the task (which has been held) allowed to continue.
A scheme whereby the word being waited for is loaded into Md concurrent with writing it into the 1kx1 RAM's
has been considered but rejected as too complicated. This would reduce average miss time from about 28
cycles to about 24.

<!-- PDF page 67 / printed page 60 -->

### 5.12. Initialization

This section outlines the order in which parts of the memory system can be initialized.

Clocks

The instruction d~coding flipflops of the memory section are enabled when the processor
clocks are enabled. All other memory clocks are enabled by a signal called RunRefresh, as
discussed in "Dorado Debugging Interface".

When RunRefresh is true, clocks internal to the memory system always happen, even if the
processor is halted. When RunRefresh is false, memory clocks run with the processor.
Except for lOW-level debugging of the memory system itself, RunRefresh should be true.
Otherwise, storage will not retain data at breakpoints.

Mer Register

The Memory Control Register (Mcr) contains fields that affect the memory system (see
Figure 10). Mcr is intended to facilitate testing, and in some cases initialization. The
register can be loaded with the Mcr .. function and read back over the DMux. Bits in Mcr
are as follows (Some of these bits are loaded from A and others from B, as indicated in
Figure 10):
dVM-Vic         On each reference, write the cache address entry selected by the row
of VA and column of Victim (Note: Victim determines the column, even
on a hit) into VA of the pipe,' so that VA[4:21] in the pipe contain the
address from the cache.         Also prevent both map and storage
automata from starting (which prevents ring buffer pipe entries from
being allocated to these as well). FDMiss should always be true when
dVA" Vic is true.
FDMiss          "Force dirty miss" forces each cache reference to miss and store the
victim, even if not dirty. Misses caused by FDMiss do not cause Hold
(*details*).
UseMcrV         Use McrV as victim and McrNV as next victim for all cache misses
instead of Victim and NextV from VNV.
McrV            The two-bit victim, or cache column used on a miss, when UseMcrV is
true.
McrNV           The two bit next-victim when UseMcrV is true.
DisBR           "Disable base registers" prevents base registers from being added to
o in computing VA and prevents BR from being written.
DisCF           "Disable cache flags" forces cache flags to read out as zeroes and
prevents them from being written.
DisHold         "Disable Hold"    unconditionally prevents hold and BLretry from
occurring.

<!-- PDF page 68 / printed page 61 -->

NoRef               Disable storage references.
WMiss               Wakeup fault task on every miss.
ReportSE'           Don't wake up fault task after (correctable) single errors.
No Wake             Never wakeup fault task.

During normal operation every bit in Mcr should be 0, except possibly ReportSE', if
correctable errors are not being monitored. It is illegal to load Mcr while references are in
progress (Changing DisH old is known to cause problems).

System Initialization

System initialization must get the map initialized as desired and the cache in agreement
with the map. Initialization firmware should allow for cache rows containing several entries
for the same address, which might occur after power up or after running diagnostics.

There are many ways to carry out this initialization.                One is as follows:
1. Set NoWake and DisHold true in Mcr, so the fault task won't disturb initialization, and so that
BeingLoaded conditions won't cause trouble.

2.   Clear TestSyndrome.

3. Load the map as desired. Clear the cache as discussed in the Map section.        After this the
cache will be empty and Ref and Dirty in map entries will be smashed.

4.    Reload the map as desired.

5.    Read Faultlnfo to kill any pending wakeup for the fault task.

6.    Setup Mcr for normal activity (0 or ReportSE').

<!-- PDF page 69 / printed page 62 -->

### 5.13. Testing

This section outlines the order in which parts of the memory system can be tested, so that
only a few new components are involved at each step.

VA, Adder, BR's

The first step is to set NoWake, FDMiss, and DisBR to true. Now processor references will
deposit Mar in VA of pipe entry 0 (in the emulator), or of every other pipe entry (in other
tasks), so that this part of the pipe can be tested (LongFetch← allows all the VA bits to be
tested). Next, setting DisBR false, loading BR's, and making more processor references
will allow BR's and adder to be tested.

Cache Address Storage

Then set NoWake, FDMiss and UseMcrV to true and use McrV and McrNV to select one
column of the cache at-a-time. Each processor reference will store its VA into that column,
and into the pipe, and will read out the old VA into the next ring buffer pipe entry (as the
victim because FDMiss is true). This allows the VA bits in the address memory to be
initialized and tested. The column number in Pipe2 should read back the value in McrV in
this case.

Above, address memory values are read using FDMiss, then VA is checked in the pipe
entry created for the victim. A simpler method of reading any address section VA is as
follows: Turn on DisBR, UseMcrV, and dVA←Vic. On processor references, the cache
entry addressed by Mar[6:11] (the row) and McrV (the column) will then have its VA[7:21]
written into VA[7:21] of the pipe entry for the reference.

The flag bits in the address section can be directly tested using B←Pipe5 and CFlags←A.
These functions operate on the cache entry addressed by the row of the last reference .and
column of the hit or victim on a miss. Since the IFU or another task could have issued the
last reference, these functions are realistically limited to initialization and checkout, where
the last reference is known. Normally these will be used with UseMcrV and FDMiss true in
Mcr, so McrV will select the column.

B←Pipe5 also reads V and NV from the selected row. CFlags←A won't work if DisCF is
true, and 8←Pipe5 will read zeroes for all four flags in this case.

CFlags←A requires that Mar data continue without glitching during the preceding
instruction as well. This means that data originating in RM or T must not have been loaded
during either of the two previous instructions (else a glitch might occur when the
multiplexor switched from the bypass to direct path) and that no higher priority tasks may
intervene between the two instructions. Issuing CFlags← A in both instructions is the
easiest way to drive Mar continuously for two cycles.

Cache Data Storage

Next, initialize the cache address section VA's and flags so that the cache data section can
be tested. To do this turn off FDMiss while leaving on NoWake, dVA←Vic, UseMcrV.

<!-- PDF page 70 / printed page 63 -->

Initialize the address section to a convenient range of virtual addresses by Store←'s to each
munch with appropriate McrV values. In the instruction after each reference, write the
flags to WP = false, BeingLoaded = false, Vacant = false with CFlags←A.

At the end of this setup, the address section will be loaded and have write access to the .
c;lesired virtual addresses. Hence, Fetch←'es and Store←'s to these VA's will not miss, and
will access the 4k of cache data memory, which can thus be systematically tested.

Map

Next, turn off UseMcrV, leaving only NoWake turned on and use Map← to test the map. At
the end of this test initialize the map, say, to map virtual addresses into c(;>rresponding real
addresses.

Main Storage

Then finally the storage can be accessed and tested with fetches and Store←. FDMiss can
be used to force storage references.

Fault Reporting

NoWake can be turned off and methods similar to the above can be used to test fault
reporting.

IOFetch~.   IOStore~.   Fast 10 Busses

Special hardware is needed to test these (the IOTest board).

Error Correction

In normal operation TestSyndrome contains 0 and Syndrome, written by the error corrector,
should be 0 if no error was corrected or detected. For test purposes, TestSyndrome can
be loaded with any non-zero value and one bit disables error correction altogether. If there
are no storage failures, TestSyndrome should wind up in Syndrome after a storage read.

The error-corrector, MemError, ECfault, ReportSE', and fault reporting can be tested using
TestSyndrome.

The LoadTestSyndrome function causes TestSyndrome to be loaded from DBuf.                  This
should normally be done after a Store←, as follows:

TaskingOff;
Store← RMAddr, DBuf← T; "'DBuf←data for TestSyndrome
LoadTestSyndrome;
TaskingOn;

TaskingOff is required because an intervening higher priority task might change the
contents of DBuf.

<!-- PDF page 71 / printed page 64 -->

## 6. Instruction Fetch Unit

The instruction fetch unit, or IFU, decodes a stream of bytes from memory into a sequence
of 8-bit opcodes and operands using a writeable decoding memory, and presents the
results to the processor for efficient interpretation. The next section contains an overview
of IFU function, supplemented by details in later sections.

Read this chapter with Figure 12 in front of you.

### 6.1. Overview of Operation

The IFU handles four independent instruction sets. Opcodes are a-bit bytes, which may be
followed in memory by 0, 1, or 2 operand bytes. Hence, the total length of an operation is
1, 2, or 3 bytes. The first operand byte is called a, the second {3.
One method of dAaling with operations longer than 3 bytes is to encode them in IFUM as 1-byte
jumps to the next operation. This gives up the possibility of referencing N, a, orpwith ←Id but
avoids having to restart the IFU. The processor' then must compute the proper place in the
instruction stream and reference a, {3, y, etc. without help from the IFU.

The term PC refers to the displacement of an opcode byte from the codebase, which is BR
31. PC's are 16-bit items, where 0:14 are an unsigned word displacement relative to the
codebase, and bit 15 selects the byte. In other words, codebase points at a 32k segment
of virtual memory; a PC selects a byte in this segment. The PC's are named PCF, ... ,
PCM, and PCX, where the final letter in the name denotes the level in the IFU pipeline.
Since the IFU's PC is only 16 bits, overflowing either end of the code segment causes wraparound.
This programming error is not detected by the hardware.

For Alto compatibility reasons, we currently have the following kludge. Instruction sets 0
and 1 treat byte 0 in the selected word as bits 0:7, 1 as bits 8:15; instruction sets 2 and 3
treat byte 0 as bits 8:15, 1 as 0:7. Eventually, this may be changed so that all instruction
sets use 0 for the byte in 0:7 and 1 for 8:15.

The IFU is started by first selecting an instruction set (InsSetOrEvent ... S function) and then
loading the F-level PC (PCF... B function). The IFU then starts fetching the byte stream
starting at the word BR(31J + PCF(0:14], byte PCF[15J, from the cache and prepares
opcodes for interpretation by the processor.

Bytes from the cache then march through the IFU pipeline beginning with the F and G full
word buffer registers on the MemO board; single bytes from FIG then move into J or H on
the IFU board. InsSet[0:1J and the opcode byte in J address the decoding memory, IFUM,
a 1024-word x 24-bit (+ 3 parity) RAM containing the information in the table below.
Although. IFUM is writeable, it will normally be loaded with the microprogram and not
subsequently changed (Diagnostics are, of course, an exception.).

<!-- PDF page 72 / printed page 65 -->

> **Table 18: IFUM Fields**

![Table 18: IFUM Fields](tables/table-18-ifum-fields.png)

Length'    2     Opcode length: 1, 2 or 3 bytes (0 length is illegal).
TPause'    1     The opcode is of type pause.
TJump'     1     The opcode is of type jump.
IFaddr'   10     TNIA[4:13] of the first instruction to be executed in interpreting this opcode
(TNIA[14:15] from the IFUJump in the exit of the previous opcode).
RBaseB'    1     RBase initialization, discussed below.
MemB       3     MemBase initialization, discussed below.
Sign             Operand sign extension, discussed below.
Packeda          Packed a, discussed below.
N          4     Operand encoded in the opcode, discussed below.

Length', TPause', TJump', Sign, Packeda, and N are used by the IFU to prepare operands
and to sequence correctly to the next opcode; IFaddr' is passed to the control section; and
the processor uses MemB and RBaseB' to initialize MemBase and RBase when the
microcode for the opcode commences.

Length' determines the number of operand bytes; a for a two or three-byte instruction will
be in H, while fJ for a three-byte instruction will be in F/G, when the assembled instruction
is ready to proceed. The assembled instruction and a then drop into .the M level.

IFUJump[n] (see "Control Section ") transfers control to the starting instruction for the
opcode assembled in M, where TNIA[4:13]t-IFaddr, TNIA[14:15]~n (n is 0 to 3) is the
location of the entry instruction. A 4-long entry vector, rather than a single starting
address, can be utilized for faster execution, as discussed later. IFaddr may be overruled
by a trap address when appropriate.

At to of the starting instruction, the processor initializes RBase to RBaseB (i.e., to 0 or to 1)
and MemBase to 0.. MemBX[0:1] .. MemB[1 :2] if MemB[O] = 0, or to 348 + MemB[1 :2] if
MemB[O] = 1. MemBX is interpreted as a stack pointer to a 4-entry stack with 4 base
registers in each entry, and MemB[1 :2] in IFUM select a particular base register from the
current entry. The MemBX kludge may reduce computation on procedure call/return, as
discussed later. Other information about the opcode and a are copied into the X level.

Instructions that implement the opcode then reference operands in sequence using the
A~ld, Risld, or Tisld operations discussed in "Processor Section" or the IFetch~
operation
discussed in "Memory Section," which read operands from the X level. The operand
sequence delivered by the IFU in response to ~ Id is as follows:

<!-- PDF page 73 / printed page 66 -->

> **Table 19: Operand Sequence for ←Id**

![Table 19: Operand Sequence for ←Id](tables/table-19-operand-sequence-load-id.png)

Pac{(eda and N are unused; sign extends the sign of a for the
jump displacement.
Jump        3                 Illegal
Regular                       N if N ne 178, Length, Length, Length,
Packeda and sign are unused.
Regular     2       o         N if N ne 178 , a, Length,      Length,
a   is sign-extended if sign   = 1.
Regular     2                 N if N ne 178 , a [0:3], a[4:7], Length, Length,
Sign is unused_
Regular     3       o         N if N ne 178 , ex,    p, Length, Length,
ex is sign-extended if sign = 1.
Regular     3                 N if N ne 178, a[0:3], a[4:7],        p, Length, Length,
Sign is unused.
Pause       x       x         Same as regular
Regular and pause opcodes have an optional 4-bit operand N that is delivered first (N isn't supplied
when N = 178), This is followed by a and     p,
if they exist; a is sign-extended when sign:: 1 or
split into two 4-bit nibbles if Packeda = 1. Subsequently, I-Id delivers Length. For jumps, all of
these operands are consumed in computing the jump displacement, and I-Id delivers Length.

The normal opcode references all of its N, a, and p operands; however, except on three-
. byte .opcpdes, the IFU hardware does not require that these operands be referenced-the
.processor CQuid exit to the next opcode without reading all the operands, if that was
desirable for some reason. However, for opcodes of length 3, the processor must consume
the a byte with .-Id (both a[O:3] and a[4:7] if Packeda =1) before going to the next opcode
with an IFUJump-it does not suffice to consume the last a byte with .- Id concurrent with
IFUJurnP. An opcode must never do more than 7 '-Id's for reasons that will be discussed
later.

The types of opcodes are distingujshed as follows: A pause has no successor, and the IFU
must be restarted with PCF.-B before the next IFUJump. A regular's successor is the byte
following its last operand; a jump's successor is determined by adding a displacement to
the current PC as follows:

If Length =1, then Sign.Packeda.N forms a six-bit signed displacement. In other
words, the jump is to any byte in the range PC - 408 to PC + 378 ,

If Length =2, then Packeda and N are unused; the jump displacement is a, if sign
is 0, or sign-extended a, if sign is 1.

A jump with Length =3 is illegal.

The IFU pipeline follows the instruction stream and fills up when it is five or six bytes ahead
of the current opcode. When a pause opcode is recognized, further memory references
are not made. When a jump opcode is recognized in J, the IFU discards any bytes in F, G,

<!-- PDF page 74 / printed page 67 -->

and H and refills these pipe levels with bytes along the jump path ..

The 8←PCX' function reads PC (invertt?d) for the current opcode. Note that PCF←8 does
not affect the value of PCX; B←PCX' continues to read the displacement of the current
opcode, which does not change until an IFUJump is done.

An opcode that conditionafly jumps can be encoded in IFUM with type either jump or
regular. If encoded as type jump, when the condition is false, the program must issue
PCF←B to restart the IFU at the fall-through address. Similarly, if regular, PCF←8 must be
issued to restart at the jump address.

The Length argument delivered by ←Id after other operands have been referenced is useful in
conditional jump calculations. Note that the fall-through address for a conditional jump is
Length + PCX, so:
T ← (ld) - (PCX') - 1;   *Id   = Length for type jump
PCF←T;
Noop;
IFUJump[O];
restarts the IFU at the fall-through address for type jump.

Following PCF←8, the IFU flushes its pipeline; it is illegal for either the instruction
. containing PCF←8 or the one immediately after it to do an IFUJump, but any subsequent
instruction can issue an IFUJump; however, the processor will spin uselessly at the IFU
"NotReady" trap until the fifth cycle after PCF←8 (earliest) or later (longer opcodes, cache
misses, Mar traffic).

<!-- PDF page 75 / printed page 68 -->

> **Table 20: IFU FF Decodes**

![Table 20: IFU FF Decodes (part 1)](tables/table-20-ifu-ff-decodes-1.png)

![Table 20: IFU FF Decodes (part 2)](tables/table-20-ifu-ff-decodes-2.png)

<!-- PDF page 77 / printed page 70 -->

something more explicit, if an appropriate convention is followed by all opcodes.

The following example shows how an instruction set with four opcodes (Push, Add, Store,
and JNZ) is implemented using a four-long entry vector. The opcodes in this example deal
with the stack like Mesa opcodes do, and the first three entry conventions are, in fact, ones
which might be used by the current Mesa emulator.

%Entry
0: Stk[StkP] holds top-af-stack (if any-garbage if stack empty), T holds garbage
1: T and Stk[StkP-1] hold previous top of stack (garbage if stack empty),
Stk[StkP] garbage, Md holds top-of-stack.
2: T and Stk[StkP + 1] hold top-of-stack,
Stk[StkP] holds previous top of stack (garbage if stack empty).
3: Results in same form as entry 2, but restart IFU at NewPC = (Id) - (PCX') - 1
Note that Stack& + 1 references must not check for underflow when the stack may legitimately be
empty.
%

Push the memory location pointed to by N.
Push:    Fetch ←Id , T←StackNoUFL& + 1, IFUJump[1j;
Fetch←Id, T←StackNoUFL& + 1 ←Md, IFUJump(1);
Fetch←Id, StkP + 2, IFUJump(1);
T ←(Id) - (PCX') - 1, StkP + 1, Return;

Replace the top two stack entries by their sum.
Add:     T←Stack& - 1, Branch[. + 2];
Stack←Md;
T←Stack& -1 ←T + (Stack& -i), IFUJump[2];
T←(Id) - (PCX') -1, StkP + 1, Return;

Store the top-af-stack into the memory location pointed to by N and pop the stack.
Store:   Stare←Id, DBuf←Slack& -1, IFUJump[O);
Stack←Md, Branch[Storex);
Store←Id, DBuf←T, IFUJump[O];
T ← (Id) - (PCX') - 1, StkP + 1, Return;
Storex:  Store←Id, DBuf←Stack& - 2, IFUJump[2);

Pop the stack and branch if the top-of-stack was zero, else fall through
This opcode is of type jump.
JNZ:     Pd ←Stack& - 1, Branch[ZTest);
Pd←Md, SlkP-1, Branch[ZTest);
Pd ←T, Branch[ZTest);
T ←(Jd) - (PCX') - 1, StkP + 1, Return;
ZTest:   T←Stack& -1, IFUJump(2,ALU #0);
"Return here when the jump doesn't take.
T←Stack&-1, PCF←T;
IFUJump(2);

Push thus requires 1 execution cycle; Store and Add take either 1 or 2 cycles depending
upon the entry point; JNZ takes 2 cycles when the jump takes or 9 cycles when the opcode
falls through (because the IFU isn't ready until the fifth cycle after PCF←8).

Although every opcode in an instruction set must have an entry vector following the same
conventions, it is not necessary that the vector be four-long. In the above example, a
single-entry scheme would probably use the entry 2 convention followed above. In that
event, Push, Add, Store, and JNZ would require 2, 1, 2, and 3 cycles (common case),

<!-- PDF page 78 / printed page 71 -->

respectively, compared to 1, 1 or 2, 1 or 2, and 2 or 3 cycles for the four-entry scheme
above.

Since Mesa requires about 120 IFU entries for its 256 opcodes, the cost of the second
entry in the vector is between 0 and 120 locations, and 120 locations each for the third and
fourth entries. Since Mesa is implemented by about 1044 instructions using entry vectors
of length 1, a vector of length 2 scheme would require ~ 1100, length 3 ~ 1220, and length
4 ~1340 instructions. The implementor of an instruction set should decide when the
additional locations expended for larger entry vectors are no longer worth the additional
speed.

Although we originally hoped for as much as 8% faster inner loops and 4% overall speed
improvement, Gene McDaniel measured only 2% faster execution for Mesa (excluding disk
wait) using a length 3 entry vector; microstore increased about 120 locations. Investigation
revealed that increased traffic on Mar (by overlapped Fetch"" and ""Md) was causing IFU
not ready to occur more often, offsetting the fact that fewer processor cycles were needed.
Forwarding saved about .2 cycles/opcode.

Note: IFU trap locations discussed below must also be entry vectors that follow the same
convention.

### 6.3. Timing Summary

From the detailed timing discussion at the end of this chapter, the following generalizations
about IFU timing can be drawn:

Assuming no misses and no delays because the processor uses Mar, IFUJump will
successfully dispatch to the entry instruction of the next opcode on the fifth cycle
after PCF"" B if the new ope ode either is one byte long or is two bytes long and
starts at an even byte; otherwise it will succeed on the sixth cycle.

A jump opcode causes a 3 cycle gap in the IFU pipe. The effect of the gap would
be a 3 cycle delay if each opcode were executed in exactly one cycle. However,
the gap can overlap with extra cycles taken on the jump opcode itself or either of
the two preceding ope odes. As usual in timing considerations, a 3-byte opcode
counts as two normal opcodes.

If a long stream of regular one-byte opcodes is being executed by the processor at
the fastest possible rate (one instruction/opcode), and if the IFU neither misses
nor faults nor waits for the processor's use of Mar or the cache, then it will always
have the next opcode ready for IFUJump. If the IFU waits one cycle for the
processor to use Mar, it will shortly fill its pipe again, so scattered Mar references
by the processor will not result in IFU NotReady.

If a long stream of regular two-byte opcodes, each of which has an IX but no N
(This is the worst case.), is being executed by the processor at the fastest possible
rate (one instruction/opcode), and if the opcodes in the stream start at the even
bytes in words, and if the IFU neither misses nor faults, and if the processor never
uses Mar, then the IFU will give 25% NotReady. Each cycle in which the processor

<!-- PDF page 79 / printed page 72 -->

uses Mar adds one cycle of delay. If the opcodes in the stream start at the odd
bytes in words, then the. processor will get NotReady 40% of the time.

Three-byte opcodes are not as bad as two-byte opcodes because, in the worst
case, the processor cannot reference both a and f3 in less than 2 instructions.
Hence, a stream of three-byte opcodes has timing approximately the same as a
stream in which each three-byte opcode is replaced by a one-byte opcode followed
by a two-byte opcode.

Mar traffic may be an important timing factor if many opcodes finish in one or two cycles_
Whenever the processor is making a reference, the IFU cannot use Mar, and the IFU must
make one reference for every two bytes in the instruction stream. Note that if a processor
reference is held, the IFU will also be prevented from making references (but the IFU is not
prevented from making references when ←Md is held).

### 6.4. Use of MemBX and Duplicate Stk Regions

The present Mesa implementation requires 34 cycles for a local XFER and 54 cycles for an
external XFER, excluding memory wait, and measurements made on the Mesa compiler
showed that 38% of all cycles were spent in XFER. For this reason, speed improvements in
XFER are an important objective.

Since about 70% of all calls return before calling any other procedure, if a caller's base
registers and stack were left untouched, then this information would neither have to be
saved during the call nor restored during the return in most cases.

The hardware that supports this idea consists of the MemBX register, pointing at one of
four blocks of 4 base registers each, and StkP, pointing at one of four stacks of 64
registers each. During a procedure call, StkP and MemBX may be advanced by 1 region,
leaving the caller's state intact; if the callee makes nested calls, then eventually the MemBX
and Stk regions would be exhausted and some would have to be saved and (eventually)
restored. However, if the callee returns without too many nested calls, then its caller's
state would still be intact.

We have not constructed examples that use this idea, but a savings of 50% in average
XFER timing has been projected for Mesa.

### 6.5. Traps

The IFU may trap for not ready, reschedule request, map faults, cache data errors, and
IFUM parity errors. When a trap condition occurs, the IFU substitutes a trap address for
IFaddr on the next IFUJump. Hence, the next IFUJump sends control to one of the entries
in the trap vector.

Locations assigned to these trap vectors are given in "Control Section"; note that each
instruction set has independent trap locations.

<!-- PDF page 80 / printed page 73 -->

Each trap vector is dispatched into by IFUJump exactly as though it were an opcode.
B~PCX' reads  the PC of the opcode that would have been executed if the trap had not
occurred and RBase, MemBase, and ~/d stuff are set according to that opcode (in every
case except NotReady-all are undefined at a NotReady trap).

The relative priority of traps is as follows: IFUM parity error is highest. then NotReady.
reschedule. cache data parity error. and map fault.

The NotReady trap occurs whenever the IFU does not have both an opcode and its
associated operands (a. p> ready for the processor. Since PCX. MemBase. and RBase are
invalid. the trap microcode must wait for the IFU to become ready. The following code
sequence will work for all instruction sets that do not use a conditional exit:
NotReady:
FreezeBC.   IFUJump[O];
FreezeBC,   IFUJump[1];
FreezeBC,   IFUJul1'p[2];
FreezeBC,   IFUJump[3];

For the sample instruction set given earlier, which uses entry 3 as a conditional exit, the
following sequence would be appropriate:
NotReady:
IFUJump[O];                        Can't convert to IFUJump[2] because stack may be empty
T"Stack! - 1"Md. IFUJump[2];       Convert case 1 to case 2
IFUJump[2];
T"(ld) - (PCX') -1. StkP"StkP + 1, Return;  *Resume the opcode which didn't really exit

If the IFU detects bad parity on any read of IFUM. the IFUJump to the opcode affected by
this parity error will trap to the IFUM parity error trap location.

The IFU will trap at the cache data parity error location, if it detected invalid parity on any
byte sent by the memory system. PCX will always correctly point at the opcode that would
have been executed next had the trap not occurred; however. the opcode and operands
pointed at by PCX are not necessarily the ones that suffered the parity error. This occurs
because the pipe has continued ahead of PCX. The most confusing case occ-urs when the
opcode following PCX was a jump; in this case the opcode fetched by the jump may have
caused the parity error, in which case PCX + / - jump displacement is limited to the range
PCX - 4008 to PCX + 3778 ,

The IFU will hold an IFUJump in the cycle prior to a cache data parity error or IFUM parity
error trap.

Note that IFUReset must be given after an IFUM or cache data parity error and before
restarting the IFU.

The Reschedule function is used by io tasks to request service by the emulator. The IFU
will honor this trap request on the second IFUJump after it is executed, as discussed in a
later section. The RescheduleNow function is like the Rescheduie function, but the IFU
honors it on the first IFUJump after it is executed. rather than the second (RescheduleNow
was intended for use when continuing an opcode which previously experienced a fault).

<!-- PDF page 81 / printed page 74 -->

An IFU fetch may experience a map fault. The memory system does not report IFU map
faults to the fault task. Instead, it signals the IFU that a map fault has occurred, and the
IFU passes this indication through its pipeline. Eventually, the IFUJump that would have
sent control to the opcode affected by the map fault will instead transfer to the map fault
trap vector.
Although IFU map faults are not reported to the fault task, the fault task must be careful to pass
over any pipe entries that were created by IFU map faults when it is woken for some other reason.

Erroneous bytes fetched after a pause or jump opcode might cause map faults, but the IFU discards
these before they reach the end of the pipeline, so. the processor is never informed. Consequently,
erroneous references interfere with processor memory activity and delay the IFU's efforts to refill its
pipe on a jump, but don't have any disastrous effect.

An IFU fetch may experience single or double storage failures. Unlike map faults, these are
reported to the fault task just as on processor fetches. The memory system pipeline will
finish loading the cache munch just as though the data were ok, and the cache entries will
have valid byte parity. The IFU will continue running just as though no error had occurred.

However, the fault task will be woken soon enough that it will run before the IFU's F
register is loaded with a byte from the bad munch. Hence, the fault task will run before
the emulator can possibly execute an IFUJump to the byte that suffered the error.

For a recoverable error, the fault task can simply carry out some logging action and block;
no harm will occur because the IFU will actually have gotten valid data, and the cache will
contain valid data. For an irrecoverable error, the fault task must clear the bad cache
munch and use the RescheduleNow function to trap the next IFUJump to code for dealing
with the irrecoverable error.
Erroneous bytes fetched after a pause or jump opcode might suffer irrecoverable errors. The fault
task has no reasonable way to distinguish these from bytes really in the instruction stream, so it will
cause a Reschedule trap anyway.

Remark

Although independent trap vectors for each instruction set are probably inessential, performance should be
better when the NotReady trap, which occurs frequently, is distinct for each instruction set. This allows the
various IFUJump exits to be transformed into the form most likely to be convenient for the next opcode.

The other traps could have been implemented to use a common trap for all locations. This would be more
economical for IFUM and FG parity error traps, if these simply result in an uncontinuable crash when running
system microcode. However, different trap vectors for each instruction set are probably more convenient for
Reschedule and Map fault traps, which have to save the state of the emulator currently running.

In any case, reserving locations for these traps costs at most 5 traps • 4 instruction sets * 4 entries/trap =
1008 locations, and realistically is much less than this because many instruction sets will not need 4 entries
and there will probably be fewer than 4 instruction sets concurrently active.

<!-- PDF page 82 / printed page 75 -->

### 6.6. IFU Reset

The processor can reset the IFU by executing the IFUReset function. This clears all IFU
error conditions, prevents further IFU memory references, clears the Brklns'- feature
discussed earlier and the test features discussed later, and generally puts the IFU in a
clean and operable state. The Reschedule feature is not affected by IFUReset.

IFUReset should be executed after power-on to get the IFU shut off. A single IFUReset will
make the IFU passive with respect to operating the. rest of Dorado. However, the IFU itself
might not be operable until a second IFUReset is executed because of a pathological
condition (If Brldns is loaded and Testing is true, then the first IFUReset will clear Testing
but not Brklns; a second IFUReset is required to clear Brklns in this case).

If the IFU has any outstanding memory references pending at the time the first IFUReset is
executed, those references will complete and disturb the top part of the IFU pipeline. A
second IFUReset must be issued after these references have all finished prior to reading or
writing IFUM. If the second IFUReset is executed 36 or more cycles after the first, then it
will for sure completely reset the IFU.                                                 .
The worst case is when a miss has just started the storage pipeline with an IFU reference in the
cache address section. In this case the IFU reference does not enter the storage pipeline until the
8th cycle and then takes 28 cycles to complete.

IFUReset should be , executed prior to using Brklns'-. It should also be executed after
reading or writing IFUM (to reset the BrkPending condition that is still lurking).

### 6.7. Rescheduling

10 tasks request service from the emulator by first indicating a request in some way
(Presently an RM location is used as a 16-bit table in which 1's indicate requests.), then
executing the Reschedule function, and finally blocking. The IFU and the processor store
the reschedule condition in flipflops which remain set until the NoReschedule function
turns them off.

The next IFUJump after Reschedule transfers to the entry vector for the opcode as usual;
the reschedule trap address will drop into the IFAddr register at t2 of this instruction, and
the first IFUJump after that will dispatch into the reschedule trap vector. This means that
second IFUJump will trap unless the second IFUJump occurs on the instruction
immediately after the first IFUJump, in which case the trap will not occur until the third
IFUJump. IFUJump's that experience a NotReady trap are not counted.

The entry vector at the reschedule trap location is entered as though it were the next
opcode. When Reschedule is used by io tasks to request the wakeup of another process,
this fact is unimportant. However, the other use of Reschedule is in continuation from map
(and other) faults. In this application, the reschedule trap will wind up restoring the IFU
state by executing an appropriate number of '-Id's and eventually branching back to the
instruction that experienced the fault. The continuation method is discussed later.

Opcodes which might execute for a long time, such as block transfer and BitBlt, must
check for rescheduling explicitly, and the (emulator only) Reschedule branch condition

<!-- PDF page 83 / printed page 76 -->

makes this check easier. If such opcodes did not check for rescheduling, then service to
the io device might be postponed for too long.

The reschedule flipflops are not cleared by IFUReset, so the NoReschedule function must
be executed as part of system reset.

When the reschedule trap vector is entered, the IFU is in an undefined state except for
PCX', and PCFfo is needed to restart the IFU at the continuation address.

### 6.8. Breakpoints

BrklnsfoB implements debugging breakpoints straightforwardly. The idea is that a one byte
opcode, BrkP, is used to transfer control to a debugger while saving emulator state needed
to continue later, and another opcode, Continue, is used to continue from breakpoints (For
Mesa, BrkP and Continue are special cases of Xfer.).

BrkP may be substituted for any opcode in a program. The debugger gets control when
BrkP is executed, saves state, and eventually can execute Continue to restore state from
values saved by BrkP.

Continue first restores registers, then loads Brklns with the opcode for which BrkP was
substitued; then it uses PCFfoB to restart the IFU at the breakpoint. The IFU will then start
running; the first opcode fetched will again be the BrkP opcode, but the contents of Brklns
will be substituted for the one fetched from memory, and the program will continue
correctly.

Without BrklnsfoB the debugger would have to simulate the broken opcode before
continuing at the following opcode, which would be harder. The example below shows a
code sequence for the final part of Continue.
Continue:

IFUReset;                      Stop future IFU fetches and clear pipe'
T fo 41C;
CntfoT;
IFUReset, Goto[.,CntHO&1];    Reset after previous IFU fetches complete
BrklnsfoOpcode;                Load opcode which BrkP replaced
PCF fo BreakAddress;           Restart IFU at address of BrkP
Noop;                          *No-op required after PCFfo before IFUJump
IFUJump[O);                    Resume program

Note: IFUReset is required before Brklns fo , even when an opcode of type Pause is in
progress.

### 6.9. Reading and Writing IFUM

.
In addition to its function related to breakpoints, BrklnsfoB is used to address IFUM when
reading or writing that memory.

When IFUM is loaded, it is addressed by the instruction set InsSet[O:1] and Brklns. The
data must remain on B for two cycles, so tasking must be disabled and the instruction

<!-- PDF page 84 / printed page 77 -->

following the one with IFUMLH/RH← must put the same data on B. If this data comes from
RM or T, the register must not have been loaded in the cycle preceding the IFUMLH/RH←
(because the bypass logic will change the B select from Pd or Md to RM or T, possibly
glitching data on B). The following subroutines illustrate loading and reading back IFUM.
WriteiFUM:
IFUResel;                      -Stop future IFU fetches and clear the pipe
Tf-41C;
Cntf-T;
IFUResel, GoTo[.,Cnt#O& -1];   -Reset after previously issued fetches complete
InsSetOrEvent f- RMaddrO;      -Load 2 instruction set bits forming IFUM address
Brklnsf-RMAddr1;               *Load 8 opcode bits forming IFUM address
TaskingOff;                    *Ensure no B glitch below and let Brklnsf- settle for 1 cycle
IFUMLH f-RMdalaHi;             -Write high part of IFUM
Bf-RMdataHi;                   -Keep data good a little longer (mustn't glitch)
lFUMRH f-RMdataLo;             -Write low part of IFUM
Bf-RMdataLo, TaskingOn;        -Keep data good a little longer
IFUReset, Return;              *Clear Brklns

ReadIFUM:
IFUReset;                      *Stop future IFU fetches and clear the pipe
Tf-41C;
Cntf-T;
IFUReset, GoTo[.,Cnt#O& -11;   Reset after previously issued fetches complete
. Brklnsf-RMaddr1;               Load 8 opcode bits forming IFUM address
InsSetOrEventf-RMaddrO;        Load 2 instruction set bits forming IFUM address
Noop;                          *Two instructions must elapse after loading Brklns
*one after loading InsSet (?Two noops after loading InsSet
might be better since this is a tight path?)
RMdataHi f-IFUMLH;              Read IFUM into RM.
RMdataLof-IFUMRH;
IFUReset, Return;               *Clear Brklns

### 6.10. Continuing from Processor Faults

Saving and restoring the state of an interrupted program requires some cleverness not only
for the IFU, but also for the Control, Processor, and Memory sections. The emulator might
fault for a data error, map fault, or stack overflow/underflow; for io tasks, stack
overflow/underflow is impossible and map faults will probably be illegal, so only data error
faults are legitimate. The discussion here will concentrate on map faults, though the same
approach could be used for other fault conditions as well.

The fault task must use as few instructions as possible so that io tasks won't be preempted
for too long. The minimum is to copy all pipe entries that contain memory faults into RM or
Stk buffers, preserve DBuf, and save the emulator's TPC; the fault task must itself deal with
data error faults by io tasks; it then restarts the emulator at a trap address. The emulator
microprogram then saves the rest of the emulator state and deduces the nature of the
fault(s) using methods discussed in "Memory Section".

The emulator fault microcode first saves ALU branch conditions and task-specific registers,
then other information of interest. The saved information is stored where the Mesa (or
whatever) program can get at it; then the trap microcode restarts Mesa at a trap procedure
that will service the map fault (probably swap in a page from the disk); eventually, state will
be restored and the opcode that faulted will be resumed at the instruction that faulted.

<!-- PDF page 85 / printed page 78 -->

The IFU state may be saved via 8←IFUMLH' and 8←PCX'. 8←IFUMLH' reads the current
instruction set and IdCnt from B[0:4]; 8[5:15] are IFUM bits which are not of interest when
saving the state of the program, so the tricky code sequence given earlier for reading IFUM
is not required. B← PCX' reads the current PC.

The 3-bit counter, IdCnt, keeps track of how many ←Id's have been done; to avoid
overflowing this counter, no more than 7 ←Id's should be done when executing any
opcode. This is one (harmless) restriction on coding emulators. The other is that
emulators never map fault on the instruction after a dispatch (BDispatch←B,
BigBDispatch←8, or Multiply); this can be assured by doing ←Md prior to or concurrent with
any dispatch.

Sample microcode for saving emulator state is as follows:
%Must first save the volatile branch conditions; Overflow and Carry won't change unless an arithmetic
AlU operation is executed, so saving them can be deferred. T, the first item saved, is written into the
RM region reserved for Save using the change-RBase-for-write FF decode.
%
Save:     FreezeBC, DbIGoTo[ALUls,ALUge,ALU<O];
ALUls:    SavedT'-T;
T.-OC, GoTo[SaveBC];
AlUge:    SavedT .-T, DbIGoTo[AlUgr,AlUeq,ALU #0);
ALUgr:    T"1C, GoTo[SaveBC];
AlUeq:    T"2C;
*Have a code, 0, 1, or 2, in T indicating the state of the ALU<O and AlU = 0 branch conditions.
SaveBC: SavedALULEZ'-T;                     Save the branch condition code
T"Pointers;                       *T "MemBase, MemBX, and RBase
T.-T Or (100000C);                *Make negative
RBase'-RBase[SaveRMRegion];
*Now choose two numbers such that their sum produces the correct ALUcry and Overflow branch
*conditions.
SavedPointers fo T, MemBase'-SaveBaseReg, DbIGoto[Cry,NoCry,Carry];
Cry:      DbIGoTo[CryOvf,CryNoOvf,Overflow];
NoCry:    DbIGoTo[NoCryOvf,NoCryNoOvf,Overflow);
CryOvf: SaveA 1 "1 OOOOOC;
SaveA2.-1QOOOOC, GoTo[SaveRest]; Numbers such that SaveA1 +SaveA2 produces
*Overflow and Carry result
NoCryNoOvf:
SaveA2.-0C, GoTo[. +2);
CryNoOvf: SaveA2'-1C;
SaveA 1"1 TlTl7C, Go To[SaveRest);
NoCryOvf: SaveA 1'-TlmC;
SaveA2'-mTlC, Go To[SaveRest);

SaveRest:
SavedPCX'-Not(PCX');
T"Not(IFUMlH');                   *Read IdCnt and InsSet in IFUMLH[O:4)
SavedldCnt"LdF[T,O,2);
T"T and (14000C);
T"RSh[T,2);
SavedlnsSet"T + (100000C);        *Set up word for InsSetOrEvent.- below
*Code to save rest of state (alf easy)

Sample microcode for continuing is given below:
Resume:                                       *Restore all processor registers except T, Cnt, RBase,
and MemBase.
InsSetOrEvent '-Saved InsSet;     "Restore the IFU instruction 'set number.
PCF "SavedPCX;                    "Restart IFU at address of the opcode that faulted
WakeUp[ContTask);                 "Wakeup the special task used for continuation.
Noop;                             *No-op required so that the instruction after the IFUJump

<!-- PDF page 86 / printed page 79 -->

below will be executed by the continuation task.
Cnt←SavedldCnt, IFUJump[O];      *Continue execution in the continuation task at ContO

Resume1: Skip[Cnt = 0& -1], At[Resume1Loc]; *Reissue the appropriate number of ←Id's to put
A←Id, GoTo[. -1];                *the IFU in the state it was in at the fault.
Cnt←SavedCnt;                      *Restore Cnt
*Restore Md by fetching from a convenient storage
*Iocation.   Then repeat the Fetch ← or Store← that
"faulted using a convenient base register and restore
"the base register (complicated code here needs careful
thought).
T←SaveA1;
Pd←T +SaveA2;                      "Restore Carry and Overflow branch conditions.
T←SavedT, TaskingOff;              "Restore T register
Selow, the TaskingOff, WakeUp, TaskingOn sequence insures that precisely one emulator instruction will
'be executed after the TaskingOn before the continuation task runs.
BDispatch←SavedALULEZ;             *Dispatch to 0, 1, or 2 in table based on
*ALU>O, ALU<O, or ALU=O.
WakeUp[ContTask];                   *Wakeup the special task reserved for continuation.
Link←SavedLink, At[ConTab,O);      *Restore Link and ALU>O
TaskingOn;
Pd ←Not(Pointers ←SavedPointers), GoTo[COK];
Link←SavedLink, At[ConTab,1];      Restore Link and ALU<O
TaskingOn;
Pd ← Pointers←SavedPointers, GoTo[COK];
Link←SavedLink, At[ConTab,2);      Restore Link and ALU =0
TaskingOn;
Pd←(SavedPointers) xor (Pointers~SavedPointers), GoTo[COK];
COK:      FreezeSC, GoTo[.];

The special restart task needed for continuation
Continuelnit:
RBase←RBase[SavedTPC];      Initialization code for the task
*First of two wakeups comes here-change emulator's TPC to Resume1 and block.
Con to:   Block;
T ← Resume1 Loc;
Link←T, TaskingOff;
LdTPC←OC;                   'Restart emulator at Resume1
TaskingOn;
Block;
Second of two wakeups comes here.       Reload emulator TPC with continuation address.
ConU:     Link←SavedTPC;
LdTPC←OC;                   Restart emulator at saved continue address
Branch[ContO];

### 6.11. IFU Testing

The IFU test control register is loaded by the IFUTest←B function; when not testing, this
register should contain 1, and it is loaded with 1 by the IFUReset function. IFUTest.15
disables the periodic wakeup request to the Junk task discussed in the "Slow 10" chapter;
when IFUTest.15 is 0, the junk wakeups occur 60 times/sec and are dismissed by any
IFUTest← function.

IFUTest.14 (TestEn) enables IFU test mode; it is illegal for this bi~ to change from 0 to 1
when the IFU is active because, if this occurred in the same cycle that an IFU memory
reference was issued, then the IFU would pollute the Mar bus indefinitely, making the
memory system unusable by the processor.

<!-- PDF page 87 / printed page 80 -->

The test features aim at two situations. First, they allow the IFU clocks to be controlled by
a program, so a diagnostic can slowly step the IFU pipeline through its stages. Secondly,
they allow data supplied by a diagnostic to be substituted for signals that would otherwise
come from the memory system. This allows the IFU to be tested in the absence of the
memory system, which allows scope probes to be inserted easily and decouples IFU
problems from memory system problems.

The TestFH' and TestSH' bits in the IFUTest register enable the first-half-cycle and second-
half-cycle clocks, respectively, which will occur between t2 and t4 of the cycle after the one
issuing the IFUTick function. Thus, the IFU can be stepped through a PCFf-B function as
follows:
TaskingOff;
IFUTest←TestEn;
IFUTick;
PCF←value;

where PCFf-value is just an example-any other IFU function or an IFUJump could be used
instead.

The IFU's memory interface is simulated by the TestFG, TestParity, TestFault, TestMemAck,
and TestMakeFf-D bits in IFUTest. Memory references are not issued by the IFU when
TestEn is true. TestFG and TestParity are substituted for the FG byte and parity bit from
the memory system; the other signals are control signals sent by the memory system in
response to IFU references. They are supposed to work as follows:

MemAck occurs at t2 of a cycle in which the IFU makes a reference at t 1, iff the memory
system accepted the reference; if the memory system was busy and did not accept the
reference, then Me mAck does not occur, and the IFU should repeat its reference. The
absence of MemAck serves approximately the same purpose for the IFU that Hold serves
for the processor.

MakeF~D occurs at t1 of a cycle in which the memory system loads F at t3; in the event of
a map fault, MakeF~D occurs at t1 of the cycle in which the memory system would have
loaded F at t3 if the map fault had not occurred. The IFU can try to start a reference at t1,
even though it has an unfinished reference in progress. The memory system will accept
the reference iff MakeFf-D occurs; otherwise, it will refuse the reference. In other words,
the IFU's second reference starts at t1 iff the first reference will deliver data at t3'

Fault is concurrent with (?) MakeF~D and indicates that the IFU reference experienced a
map fault.

In other words, a memory reference can be simulated with the IFU test feature by (1)
ticking the IFU through a cycle in which it makes a reference; (2) ticking the TestMemAck
response of the memory system with IFUTestf-B and IFUTick; (3) ticking TestMakeF~D; (4)
ticking with TestFG and TestParity holding simulated memory data.

### 6.12. Details of Pipe Operation

The lFU is a six-stage pipeline, starting with words fetched from memory, and ending with
opcode starting addresses delivered to the control section and operands delivered to the

<!-- PDF page 88 / printed page 81 -->

processor. The levels are named: F, G, H, J, M and X. Each level has a data-valid bit
indicating whether or not it contains something useful.

PCF, PCJ, PCM, and PCX are PC's for the corresponding pipe levels (except that PCF is a
word PC rather than a byte PC). PCF, PCM, and PCX are independent of each other sin.ce
jumps and PCF← may result in these all being different; PCJ is related to PCF by the
number of valid bytes in the F/G/H levels; the hardware also uses PCFG, which contains
PCF plus the number of valid bytes in the F/G levels. Operationally, F/G are a FIFO in
which PCF is the write pOinter, incremented as. words are fetched from the cache, and
PCFG is the read pointer, incremented as bytes are moved from F(G into J/H. Note that
there is no PCH because PCH would equal PCJ + 1.

Pipe control is straightforward in principle. The F and G levels are l6-bit registers filled
from the cache. Following PCF←B, if there is space in the pipeline for another word, the
IFU will start a reference at t1 of any cycle in which the processor is not using Mar (so as
many as 2 IFU references can be outstanding). Cache words are stored in F at t1, then
dropped into G at t2 ; bytes drop into H at t3 or J at t 4 ; there are bypass paths to get bytes
directly from FIG into J when H is invalid. As the processor executes opcodes, F and G
become invalid, and the IFU refills them from memory automatically. This continues until
the IFU is reset by the processor, or encounters a pause opcode.
The F and G registers are physically located on the MemO board. The four bytes in F/G are inputs
to a multiplexor controlled by the IFU, and the multiplexor output is sent across the backplane to the
IFU. Brklns[O:7j or IFUTest[O:7j replace F/G data when using breakpoints, reading/writing IFUM, or
using IFU test features.

While following the opcode stream, a jump will invalidate data in F. However, if a reference is in
progress and F has not yet been filled by the memory system, then the IFU will invalidate the data
when it arrives and restart the next reference immediately. In other words, the IFU cannot abandon
the useless fetch; it must wait for it to finish and discard the result.

The J and H levels are one byte wide. For one-byte opcodes it is possible to consider H
and J as independent levels of the pipe; however for two or three byte opcodes, it is
appropriate to consider J/H as a single level in which J holds the opcode and H holds a.

If J is invalid, then it will be loaded from the next opcode (which may be in G, F, or H
according to various conditions) at an even clock (to> and H will be loaded from the byte
after the opcode (which is always in G) at the following odd clock (t 1); if the byte after the
opcode isn't ready, it will drop into H at the next odd clock after it is ready. The InsSet
and J registers address IFUM and IFUM outputs reveal whether the byte in H is a (Length
= 2 or 3) or the next opcode (Length = 1).

The conditions under which the M level can be loaded from J are that M is invalid (or
about to become invalid) and:

Length     = 1 or
Length     = 2 and H is valid or
Length     = 3 and H is valid and either F or G is valid.
If these conditions are met, then the M level is loaded (t 2) with information from IFUM and
with a, if Length = 2 or 3. If Length = 3, then f3 will drop from G into H (ta).

<!-- PDF page 89 / printed page 82 -->

If Length < 3, then the HI J level is now free to work on the next opcode.. If Length = 1
and the next opcode happens to be in H, then H will drop into J at the same time (t2);
otherwise, J will be loaded from the next opcode in F/G when it is ready.

When the processor does an IFUJump[n], level M presents information needed by the next
opcode as follows:

IFaddr is TNIA[4:13] for the IFUJump;
MemBase is set to O.MemBX.MemB[1 :2] or 348 + MemB[1 :2];
RBase is set to 0 or 1;
N, Sign, Length, Packeda, and a are loaded into the X level;
f3 is loaded into the M level if Length = 3.
Referencing IFU operands with Atold, Tisld, or Risld affects the IFU in two ways: it causes
the IFU to advance to the next item of Id, and for a 3 byte instruction when a is taken
(a[4:7] when Packeda = 1) it causes f3 to drop from M to X, freeing tv! for the next
instruction.

IFetch to also uses Id, as discussed in memory section, but does not advance the IFU to the
next item of Id.

For a one or two-byte opcode, it is permissible for the processor to do an IFUJump before
referencing any operands with t-Id; this will advance normally to the next opcode.
However, for a three-byte opcode the processor must reference all of a, so that f3 drops
into X, before doing an IFUJump.

When a pause or jump is recognized, the IFU may already have filled the F and G levels
erroneously (i.e., 4 bytes ahead). These levels are flushed and refilled along the jump path.

### 6.13. Timing Details

This section discusses timing details of the IFU pipeline assuming that all IFU references hit
in the cache and are never deferred for processor references.

First case:        Restart IFU at even byte
to:     An instruction with PCF←Foa is started, where FOa is even.
12:     F, G, H, J, and M levels are made invalid.
13:     Reference the word containing Foo.
t5:     Reference word containing Foo + 2.     .
t7:     Load F with data from the Faa reference; reference the word containing Faa + 4.
t8:     Load the first byte from F into J; load G from F; F becomes invalid; start reading the IFUM
entry for J.
19:     Load the putative operand byte from G into H; G becomes invalid; load F from the Foo + 2
reference ..
t10:    Distinguish 5 cases below.

Faa is a one-byte regular opcode

<!-- PDF page 90 / printed page 83 -->

t10:   Load M from IFUM; IFUJump will now succeed; load J from H (FOO+ 1); load G from F
(FOO + 2 and FOO + 3); F and H become invalid; start reading the IFUM entry for J.
t11:   Load H from G (FOO + 2); load F from FOO + 4 reference.
t12:   -   (The FOO + 1 opcode would pop into M if IFUJump were done at t10.)
IFU is quiescent; F has two useful bytes. G one byte. J/H has two bytes; M level is ready
and waiting for IFUJump.

FOO is a two-byte regular opcode
t10:   Load M from IFUM and M[a) from H; IFUJump will now succeed; load J from F (FOO+2);
load G from F (garbage and FOO + 3); F and H become invalid; start reading the IFUM entry
for J.
t11:   Load H from G (FOO+3); G becomes invalid; load F from FOO+4 reference; reference the
word containing Foo + 6.
t12:   Load G from F; F becomes invalid.
t15:   Load F from the FOO+6 reference; now quiescent.

FDa is a three-byte regular opcode
t10:   Load M from IFUM and M[a] from H; IFUJump will now succeed; load G from F (FOO+2
and Foo +3); Hand F become invalid; J goes to special state (fl in H).
t11:   Load H from G (FOO+2 = /3); load F from the FOO+4 reference; now quiescent.
t12:   -   (The FOO + 2 byte would pop from H into M[,B] if IFUJump were done at tlO.)

FDa is a one-byte jump opcode
t10:   Load M from IFUM; IFUJump will now succeed; J. H. G. and F become invalid.
t11:   Discard the FOO + 4 reference; reference the first word along the jump path.
t13:   Reference the second word along the jump path.
t15:   Load F from the first word along the jump path.
t16:   Load J from F. etc.

FDa is a two-byte jump opcode
t10:   Load M from IFUM and M[a] from H; IFUJump will now succeed; G and F become invalid; J
and H are in a special jump state. computing the jump address.
t11:   Discard the FOO + 4 reference; reference the first word along the jump path.
112:   J and H become invalid.
113:   Reference the second word along the jump path.
t15:   Load F from the first word along the jump path. etc.

Second case:         Restart IFU at odd byte
to:    An instruction with PCF←Foo is started. where FOO is odd.
t2:    F. G. H. J. and M levels are invalid; IFUJump will trap at NotReady.
13:    Reference the word containing Foo.
t5:    Reference word containing FOO + 1.
t7:    Load F with data from the FOO reference; reference the word containing FOO + 3.
t8:    Load the second byte from F into J; F becomes invalid; start reading the IFUM entry for J.
t9:    Load F from the FOO + 1 reference.
t10:   Distinguish 3 cases below (and the one and two-byte jump cases which are not repeated
below).

<!-- PDF page 91 / printed page 84 -->

Faa is a one-byte opcode
t10:   Load M from IFUM; IFUJump will now slicceed; load J from F (FOO + 1); load G from F
(garbage and FOO + 2); F becomes invalid; start reading the IFUM entry for J.
t11:   Load H from G (FOO + 2); G becomes invalid; load F with the FOO + 3 reference; reference
the word containing FOO + 5.
t12:   Load G from F; F becomes invalid.
t15:   Load F from the FOO+5 reference; now quiescent.

FDa is a two-byte opcode
t10:   Load G from F (FOO + 1 and FOO + 2); F becomes invalid.
t11:   Load H from G (FOO+1); load F with the FOO+3 reference.
t12:   Load M from IFUM and M[a] from H; IFUJump will now succeed; load J from G (FOO + 2);
load G from F; F and H become invalid; start reading the IFUM entry for J.
t13:   Reference the word containing FOO+5; load H from G (FOO+3).
t17:   Load F with data from the FOO + 5 reference; now quiescent.

Faa is a three-byte opcode
t10:   Load G from F (FOO + 1 and FOO + 2); F becomes invalid.
t11:   Load H from G (FOO+1); load F from the FOO+3 reference.
t12:   Load M from IFUM and M[a] from H; IFUJump will now succeed; H becomes invalid; J is in
a special state (fJ in H).
t13:   Load H from G (FOO+2):load G from F (FOO+3 and FOO+4); F becomes invalid; reference
the word containing FOO + 5.
t17:   Load F from the FOO+S reference; now quiescent.

<!-- PDF page 92 / printed page 85 -->

## 7. Slow IO

The slow io facility allows data transfers between the processor 'and any of up to 256
independently addressed io registers. It is intended that the slow io facility will be used to
load and read control information associated with high speed io devices (> 20 x 106
bits/sec), which will then use the fast io system for their data transfers. Low speed devices
« 20 x 106 bits/sec) will use the slow io bus for all phases of their operation. Very slow
or polled devices may be driven directly from an emulator.

Device controllers for Dorado interact with the processor by exchanging data over a 16-bit
bidirectional bus IOB ("!nputlQutput gus"). There may be a total of up to 256 io registers
in all controllers connected to a single system. The unique 8-bit device numbers assigned
to particular devices or uses that appear in every system are discussed in subsequent
chapters and summarized in the table below.

> **Table 21: IO Register Addresses**

![Table 21: IO Register Addresses](tables/table-21-io-register-addresses.png)

<!-- PDF page 93 / printed page 86 -->
### 7.1. Input/Output Functions

In most cases, a task will need to do many sequential io operations to the sallie io register.
The 8-bit task-specific register TIOA holds the device address being referenced by each
task.

TIOA is loaded at t2 from B[0:7] by the TIOA←B function, or TIOA[5:7] can be loaded from
FF[5:7] while preserving TIOA[0:4] by the TIOA←small constant function.        Pd←Input,
Pd←lnputNoPE, or Output←B functions can be issued in the instruction immediately
following the one that loads TIOA.

Most input registers include odd byte parity with IOB data. The Pd←Input function reads
IOB data and checks parity. The Pd←lnputNoPE function reads IOB data without a parity
check; this is useful when determining whether a device exists (IOB has bad parity if a
nonexistent register is selected). The enabling and timing of parity error halts is discussed
in the "Errors" c.hapter.

The Output← B function sends 16 bits of data with parity to the io register selected by TIOA.
Many controllers check the parity and report parity errors as part of their status.

The tasks reserved for standard peripherals are given in the table below.

> **Table 22: Task Assignments**

![Table 22: Task Assignments](tables/table-22-task-assignments.png)

### 7.2. IOPcodes

The Mesa instruction set has two opcodes for dealing with the slow io system:

INPUT:
TIOAI-a;
Stkpl-Stkp + 1;
Stackl-Input, IFUJump[O];

OUTPUT:

<!-- PDF page 94 / printed page 87 -->

TIOMa;
Output←Stack& - 1, IFUJump[O];

These opcodes allow a Mesa program to have full access to the io system. The intent is
that these instructions will be used to set up registers in firmware-driven devices, and do all
the service required by polled slow devices.        In many cases, the use of an INPUT or
OUTPUT instruction is not sensible (doing io to a device normally driven by firmware, for
example), but the capability should prove useful for testing and diagnostics.

### 7.3. Wakeup, Block, and Next

The "Control Section" chapter discussed task switching, and the material which follows is an
elaboration of that discussion.

Note that a task for which a wakeup request is issued at to cannot commence its next
instruction until t4 ; i.e., at least two cycles elapse after a wakeup before the next instruction
is executed. The task then runs until it does a Block; in order to avoid an erroneous extra
wakeup, the task must lower its wakeup request at least one cycle before issuing Block.

Consequently, an io device may turn off its wakeup request according to one of three
strategies:

The first is to turn off the request when Next becomes equal to its task number; in
this case the wakeup request is lowered at to of the first instruction executed for the
task, and it must not block until the second instruction to prevent an erroneous
second wakeup. The special situation in which Next is invalid ("Next lies") must be
dealt with by device controllers that do this. This situation occurs as follows:

Suppose that a task blocks with the following instruction:

Branch[Loop], Fetch←Address, Block;      *Fetch next word

This generates Switch and the task in Bnt is broadcast over the Next bus.
If the Fetch← causes hold and Bnt < Ctask, then no task switch will occur.
However, the Next bus is incorrectly broadcasting Bnt. Since hold occurs
after t1 , there is insufficient time to change the Next bus back to Ctask in
this case.

Consequently, controllers using Next detect "Next lies" and disable any
actions that would otherwise be performed when it occurs.

A pathological lockout problem should be noted: Since task T's wakeup
request was lowered at t2 when Next =T was noted at to' the Next Lies
condition will (correctly) result in repeating the held instruction at t 2;
however, some task of lower priority than T may erroneously execute at t4 •
This might be a problem if some high demand task of higher priority is
coded so that it always creates Next Lies (say, by doing Block and
immediate ←Md in the instruction after a Fetch←).

Another consequence of "Next Lies" is that IOAtten may be incorrect when

<!-- PDF page 95 / printed page 88 -->

"Next Lies" is occurring. Consequently, branch on IOAtten is illegal during
an instruction that blocks and might cause hold.

The second strategy monitors TIOA becoming equal to a particular device value. In
this case the wakeup request is lowered at to of the second instruction following a
wakeup, and the task must not block until the third instruction. The disk controller
has used this strategy, which has the draw back that if TIOA inadvertently assumes
the particular device value for any other task, the hardware will malfunction. A
consequence of any device using this strategy is that all tasks must be careful to
initialize TIOA properly when first awakened.

The third strategy waits for some Output←B or Pd←Input operation to reset the
wakeup condition. This would reset the condition at t3 or ts of the Output←B
instruction, and the wakeup would be lowered at t4 or ts; in this case the task must
not block until the third or fourth instruction after the Outputf-B or Pdf-Input to
avoid an erroneous wakeup.          The exact requirement depends uppn the io
controller-the disk controller, for example, lowers its wakeup request at t4 and can
block in the third instruction after Output←B, while the display controller horizontal
task lowers its wakeup request at ts and can block in the fourth instruction.

If loops naturally run for at least three instructions, use of TIOA is more economical than use
of Next because TIOA decoding is mandatory in any case, while Next is needed only for
short loop devices, devices that use the fast io system, and devices that drive the SubTask
lines.

### 7.4. SubTasks

When an io device sees Next becoming equal to its task, it can (optionally) present a two bit
SubTask number as well.

The processor, control, and memory sections clock SubTask into f1ipflops at           to.
The
processor OR's SubTask [0:1] into RBase[2:3] and into MemBase[2:3]. This allows the same
firmware to control several identical io devices concurrently-each device, represented by a
SubTask, gets its own RM region with 16 RM locations and its own pair of MemBase
registers; if only SubTask[O] is driven, then two RM regions and four MemBase registers are
available to each subtask. Note that the 16 change-RBase-for-write functions do not OR
SubTask into the changed address, so they cannot be used; also, if RBase is read by the
processor the value read out has SubTask OR'ed in. However, the 16 change-RSTK-for
write functions do work.

Note also that when the debugging processor (Baseboard microcomputer or Alto running
•   Midas) asserts the Freeze Signal, the affect of the subtask on RBase[2:3] is disabled, but
subtask continues to affect MemBase[2:3].

In the memory section, the task and SubTask that issued an IO(=etch← is bussed to fast
output devices with data from storage. The device receiving the data identifies itself by
means of this information. IOStore←'s are handled similarly.

A task presenting SubTask signals generally must Block at the same location each iteration

<!-- PDF page 96 / printed page 89 -->

since there is only a single TPC value for all of the SubTasks. Hence, the full generality of
tasking is unavailable-the microcode for these tasks must be coded as though the wakeup
mechanism were a priority interrupt.

### 7.5. Illegal Things IO Tasks Must Not Do

(1) It is illegal to Block in an instruction that does B← ExternalSource, where ExternalSource
is anything except one of the sources on the IFU board. This restriction is needed so that
the emulator will be able to do arithmetic on B←PCX'.

(2) The IOAtten branch condition is illegal in an instruction that Blocks and might be held,
because NextLies might occur, as discussed above.

(3) A task may not Block on an instruction that might be held, if its wakeup request might be
dropped at to of the instruction. If this occurred, the instruction might inadvertently be
repeated before the Block took effect.

(4) It is illegal to Block with TaskingOff in force.

(5) A task must not Block until one cycle after its wakeup request is turned off.

(6) It is illegal to issue Wakeup(n] if task n might run in the next cycle. Wakeup(n] must be
executed with TaskingOff in such circumstances.

<!-- PDF page 97 / printed page 90 -->

## 8. Fast IO

The fast input/output system provides high-bandwidth data transfers between storage and
io devices. Transfers occur in units of one munch (= 16 words); the addresses of the 16
words must be i, i + 1, ... , i + 15, where i mod 16 = O. One word is transferred every clock,
for a peak bandwidth of 533 x 106 bits/second. A fast device is also interfaced to the slow
io system, from which it receives its control information, since there is no way for the
device to communicate directly with the processor using the fast io system.

A single transaction of the fast io system transfers exactly one munch. Successive
transactions are completely independent of each other, whether they involve the same or
different devices, as far as the io system is concerned. The only relationship between
transactions is that storage references of two transactions occur in the order that they were
issued.

Each fast io transaction is initiated by an 10Fetch t- or 10Storet- reference coded in ASEL.
Once this instruction has been executed, the transaction proceeds without further
interaction with the processor (except for fault reporting). The transaction itself involves a
storage reference, and transport of the data between main storage and the device. In the
case of a fetch, transport happens at the end of the reference, after the mU/Jch has been
error-corrected. For a store, transport happens at the beginning of the reference, in
parallel with mapping the VA and starting the storage chips. As a result of this difference,
the transport for a fetch may overlap or even follow the transport for a following store.

### 8.1. Transport

The device is only concerned with the transport of the data, and has no way of knowing
exactly how or when the storage reference take place. The transport happens in 16
clocks, each transporting a single word using the Fin bus (IOFetcht-'es) or Fout bus
(I0Storet-'s). The two busses are independent, and transport can be happening on both of
them simultaneously.

The two busses have much in common. Both have Task and Subtask lines, on which the
memory presents the task and subtask involved in the transport about to begin and a Next
signal used for synchronization. The Fout bus has a Fault line which is high at the time the
last word of the transaction is delivered if there was a memory fault during the fetch (other
than a corrected single error).

Both data busses are 18 bits wide: 16 data bits, numbered 0 .. 15, and two byte partiy bits,
numbered 16 (bits 0.. 7) and 17 (bits 8 .. 15). The parity bits have the same timing as the
data bits. A device is invited to check the parity of data on Fin, and is required to generate
parity for data on Fout.

### 8.2. Wakeups and Microcode

The normal interface between a device and its task involves one wakeup for each munch
transferred. The device must keep track of the number of wakeups it has issued, since
data may not arrive from storage for several microseconds, but there is no way to stop the

<!-- PDF page 98 / printed page 91 -->

data from arriving once the task has started the memory reference.

Typical microcode for a fast output device is given in the "Display Controller" chapter.

### 8.3. Latency

Suppose that the highest priority'fast io task issues its wakeup request at to; then it will
execute its first instruction at t4 • Some other task can cache fault with clean victim in the
cycle starting at to' and another task can cache fault with dirty victim in the cycle starting
at t2. The first reference gives rise to one storage reference and the second to two storage
references; each of these three storage references takes 8 cycles to handle, so the fast io
reference will not begin for about 24 cycles. From the time it begins until the last data
word is delivered to the device is 23.5 cycles, for a total of 47.5 cycles, to which 2 cycles
must be added for the time between the wakeup and the first executed instruction. In this
situation, the transport is pot finished until 49.5 cycles after the wakeup. Lower priority
tasks are delayed by an additional 8 cycles for each reference which might be made by a
higher priority task.

The above is one possible worst case. Another is the execution time of higher priority
tasks; a wakeup might be delayed by sum of the longest normal execution of 'the fault task
and of other higher priority tasks. The fault task execution time is presently unknown.

A store reference is slightly better, since its transport is finished 8 cycles after the
reference starts, for a total latency of 40 cycles.
All these numbers assume that a reference' can be started every 8 cycles. If
successive references are to 4k modules, however, they can happen only every 13
cycles, and the calculations must be adjusted accordingly. Also, data is returned
from a 4k module 3.5 cycles later.

'.

<!-- PDF page 99 / printed page 92 -->

## 9. Disk Controller

This chapter describes the Dorado disk controller, which Llses the Slow 10 system to
control up to four Century Data Trident disk drives. Either the 80x106 byte T-80 or the
300x106 byte T-300 drives can be used. An extension of the controller onto a second logic
board (not designed) would allow control of up to 31 disk drives; alternatively, duplicating
the present controller (with different TIOA, task, and muffler assignments) would allow
independent control of four additional drives.

Keep Figure 13 in view while reading this chapter.

The disk controller uses task 148 and the first five values of the TIOA addresses in block
108 . 178 (The Ethernet controller, on the same logic board, uses two of the other three.).
Either the task or TIOA block can be modified by changing a SIP component on the logic
board. TIOA assignments are as follows:

DiskControl    Output4-B to control register
DiskMuff       Output4-B muffler control and Pd4-lnput to read muffler
DiskData       Pd4-lnput to read FIFO or Output4-B to write FIFO data
DiskRam        Output4-B to format RAM
DiskTag        Output4-B to tag register
Note: other tasks must not select these TIOA addresses at any time; doing so may cause the disk
controller to malfunction.

The controller is interfaced to the disk drives by a daisy chain cable bussed to all drives
and by an independent radial cable to each drive. The radial cables contain the following
signals:

data line (bidirectional, differentially driven)
data clock (from drive, differentially driven)
subsector/index line (from drive)
selected line (from drive)
select line (from controller)
sequence line (from controller, controlled by the baseboard for drive 0 and grounded
for other drives)
two vce lines and scope trigger (from controller)

The daisy-chain cable contains the following signals:

16 control "tags" driven by the controller and received by the selected drive
9 error and status signals from the drive as follows:
CylOffset'
ReadOnly'
NoTerminator
HeadOvfl'
Seeklnc'
DevCheck'
NotOriLine
NotReady
Index'

<!-- PDF page 100 / printed page 93 -->

The controller or's the NoTerminator error (which means that the daisy-chain cable isn't
terminated) into the NotOnLine error; the other error indications are discussed later.

### 9.1. Disk Addressing

The disk system is accessed through a many level addressing scheme. First a particular
disk drive is selected. Then a data surface or head and a cylinder are selected (5 surfaces,
815 cylinders on a T-80). Each cylinder is further divided into sectors which consist of
blocks.

Firmware may control the following parameters:

Sector size (1378 words max., limited by 4-bit subsector counter)
Number of blocks within one sector (1 to 4)
Block sizes (2 to 2684 words)
Note: Various limits on the sizes of blocks and sectors will be discussed.   The processor interface allows
a six-bit subsector counter of which only four bits are presently implemented, and this is the most
significant length limit at present (1378 words). If the subsector counter .were enlarged to six bits, then
the block size limit imposed by the error correction algorithm (2684 data words) would apply. We are,
however, unlikely to find any of these length limits significant unless we enlarge the memory page size to
4096 words. Jumpers in the disk unit could also be set to vary the spacing between subsector pulses.

Because sector formats an~ flexible, firmware can adjust the controller to system needs.
The sector formats specifically envisioned in the design of the controller include 28 256-
word sectors for Alto Diablo emulation and Pilot, 16 512-word sectors for Juniper, and 9
1024-word sectors for Alto Trident emulation.

### 9.2. Sector Layout Considerations

Each block within a sector can be either read, written, or checked. However, once any
block is written, later blocks in that sector cannot be read during that disk revolutipn.
(Later blocks should be readable on subsequent disk revolutions, though this is not
guaranteed and no existing software depends on this.) Reading or writing must start with
the first block in the sector and continue; since check bits are stored at the end of each
block, the entire block must be read to verify its data or correct errors; however, one does
not have to read or write subsequent blocks in the sector. After a check-block operation is
started, the controller inhibits writing later blocks within a sector without a specific "OK"
from the firmware.

Our general plan is to use the first block in a sector as a header identifying the disk
address; all headers will be written when a disk pack is initialized; subsequently, the disk
task compares the header with the disk address it thinks it is accessing. The header not
only provides a useful safeguard against positioning errors but also allows faster sector
determination when switching to a new drive, as discussed later.

The second block might identify information stored in the sector (e.g., the Label block in
Alto format). The third block might be the data block. The fourth block could hold
reference, backup, or archiving information.     All of these choices are a matter of
programming convention.

<!-- PDF page 101 / printed page 94 -->

Feasible sector layouts are determined by several considerations. First, each disk drive is
configured to generate 117 subsector pulses/revolution.       The disk controller has a
subsector counter for each drive that is initialized to N when an index pulse is received
from the drive; it then counts down to 1, generates a sector pulse, and reinitializes itself.
The firmware can specify N (0 to 178 ) independently for each disk drive and thus create
117/(N+1) sectors/revolution. If this division leaves any remainder, then there will be one
or more unused subsectors at the end of the cylinder.

Note that the quantization of cylinders into subsectors allows a sector size to be specified
in units of 10,080/117 = 86.15 words/subsector.

Various delays must be provided at the beginning and end of each block to allow for
electrical and mechanical tolerances within the disk drive. To define a sector format, one
simply needs a summary of "words lost" for each block:

Total words/track    =                          10,080
Words lost for tne 1st block in a sector =      38
Words lost for successive blocks =              14"
Required gap at end of sector      =            (micrQcode-dependent)

A track is the path swept through one revolution by a single head at a single cylinder.
"Words lost" for each block include 2 words of error detection and correction (32 bits of
ECC code) which are always added at the end of the data written, plus preamble,
postamble, and various other delays required by the controller and drive electronics.
These are detailed later under "Format RAM and Sequence PROMs". Additionally, to
enable the microcode to process consecutive sectors, there must be some gap between
the end of the last block and the end of the sector; the number of words required depends
on the amount of time the microcode requires to complete processing the last block and
issue a command for the next sector.

For the Alto Trident format there is a 2-word Header block, 10-word Label block, and 1024
word data block; total words lost for disk formatting is 38 for the first blo~k, 14 for the
second, and 14 for the third; altogether, this requires 1100 words/secto"r. The next larger
multiple of the subsector size is 86.15*13 = 1119 words, leaving 19*1.65 = 31_35 JLs of
gap at the end of the sector. Thus 13 subsectors/sector are required, yielding 117/13 = 9
sectors/revolution.

Using this kind of analysis, reasonable sector layouts on the T-80 are as follows:

29 sectors of 256 data words each (4 subsectors/sector),
16 sectors of 512 data words each (7 subsectors/sector), or
9 sectors of 1024 data words each (13 subsectors/sector).
Note: The 29-sector and 16-sector formats do not divide the disk evenly but rather yield an unusable
leftover fraction of a sector; the g-sector format does divide the disk evenly. The g-sector format is
compatible with the Alto Trident g-sector format (used by BCPL Trident software such as IFS). The 16-
sector format is not compatible with the Alto Trident 16-sector format (used by Juniper), though it is
usable if interchangeability of disk packs with Altos is not required. The 29-sector format has no Alto
analogue.

<!-- PDF page 102 / printed page 95 -->

> **Table 23: T-80 Specifications and Characteristics**

![Table 23: T-80 Specifications and Characteristics](tables/table-23-t80-specifications.png)

Cylinder positioning time        6 ms cylinder to cylinder maximum (3 ms typical)
30 ms average
55 ms maximum
Rotational speed                 3600 rpm (16.66 ms/revolution)
Sector length selection          12-bit increments through jumpers on sector board
Densities                        370 cylinders/inch
6060 bits/inch max. recording density
Disk pack characteristics        IBM 3336-type components
5 recording surfaces plus 1 servo surface
815 cylinders/surface
Operating methods                Modified frequency modulation recording
Linear positioning motor with cylinder following servo
Mechanical specifications        Size - 17.8" wide x 10.5" high x 32" deep
Weight - 230 Ibs.
Error rate                       Recoverable: 1 error/10 10 bits
Irrecoverable:  1 error/10 13 bits
Positioning:   1 error/106 seeks
Pack starVstop time              20 sec start time
20 sec stop time (with dynamic braking)
Controls and indicators          Ready Indicator
Off = disk not splnmng
Flashing = spinning up/down
On = Ready
Fault Indicator
Start/Stop switch
Read-only switch
Degate switch (inside the drive; takes disk off-line for testing)

### 9.3. General Firmware Organization

This section gives a general overview of how the disk controller firmware is organized;
more detailed descriptions follow later.

The disk drive generates subsector and index pulses on one line in the radial cable; the
controller distinguishes these according to pulse width. In the normal Idle loop, the
controller looks only at these pulses from the connected drives. A four-bit counter for each
drive counts down subsector pulses and generates sector pulses. Upon either a sector or
an index pulse from the selected drive, the controller generates a disk task wakeup. The
disk task then either increments (sector wakeup) or zeroes (index wakeup) its firmware
sector counter, clears the wakeup condition, checks for a new command, and blocks.

Because there are no hardware sector counters, the disk task must maintain a sector
counter itself; this implies that the rotational position is generally unknown on all
deselected drives.

<!-- PDF page 103 / printed page 96 -->

When first selecting a drive, there are two strategies for determining the sector position: (1) Wait for
an index wakeup, at which time the sector position becomes known; (2) Wait for a sector wakeup
and then read the sector number stored in the header block (This can only be done if the disk is
not moving to a new cyclinder.). The most efficient strategy appears to be a combination: Select
the drive and start a seek to the correct cylinder; if an index wakeup arrives before the seek is
finished, then the sector position is synchronized with no loss of time. If the seek finishes first, then
read the next header to determine the sector number.

When a new disk operation is noted, firmware will perform the following steps:

Execute a drive-select command, if the drive differs.
Load the sector size only if different, and block until index.
Load the format RAM only if word count or commands differ.
Execute a Control Tag (seek) command only if the cylinder differs, and wait (continuing
to count sectors) until the drive becomes ready again.
Execute a Head Tag command.
Block until, at a sector wakeup, the next sector is the one wanted.
Load the appropriate transfer command into the control register
Block until the next sector wakeup.

At the start of the next sector, the controller will become active and sequence through
commands under control of the format RAM and two sequence proms (one for reading,
one for writing).

The sequence proms define what operations the controller must go through, and the format
RAM contains all parameters that might change from one implementation to another.
Actual commands for the Trident disk are stored in the format RAM along with count
values such as words/block, words of ECC, and words of delay before some operation; the
commands are loaded into the tag register and executed by the controller during the
transfer.

Once a transfer has started, the disk task will be woken according to the number of wo~ds
in the FIFO, and it will send or receive the appropriate number of words. Read and
compare operations are performed by firmware, as well as detecting checksum errors at
the end of reading. During writing, firmware must provide one word of sync bits (201 8
standard, 0018 for Alto Trident emulation) followed by the specified number of words for
that block (the controller will append 2 words of checksum). During read, the controller
will look for, and discard, the first word of sync bits, then firmware must accept the
specified number of words for that block, followed by two words of checksum to be
discarded, followed by the ECC remainder to be used for error detection/correction.

### 9.4. Task Wakeups

The controller may wakeup the disk task for many conditions; the disk task must detemine
the cause and take appropriate action, which must in some way cause the wakeup to go
away.

In general, there are two ways to determine the wakeup condition: read the wakeup
condition, or assume the condition knowing the state of the disk task (which implies the
state of the controller). When expecting a sector or index wakeup, the disk task must test
carefully to count sectors reliably, but in the middle of word transfer operations, it will

<!-- PDF page 104 / printed page 97 -->

assume the wakeup reason to minimize overhead. The various conditions .are as follows:
IndexTW, SectorTW, TagTW, RdFifoTW, and WrFifoTW; these wakeup conditions are
detailed in the "Muffler Input" section.

### 9.5. Control Register

The DiskControl register is a collection of flip-flops defining the state of the controller; on
Output to DiskControl, IOB is interpreted as follows:

8[51         Clear EnableRun
8[61         Set DebugMode
8[71         Set 810ckTillndex
8[8:9]       Operation for first block of sector, where the operations are:
o = Done (finished with aU blocks in this sector)
1 = Write
2 = Read and check
3 = Read
8[10:11]     Operation for second block of sector, as above.
8[12:13]     Operation for third block of sector, as above.
8[14:15]     Operation for fourth block of sector, as above.

EnableRun determines whether the controller is active at all.   It is initially cleared by
10Reset, and can only be set by completing the loading of the format RAM (see below).

DebugMode allows the controller to be exercised by diagnostics when no disk is present; in
this case, diagnostic firmware provides fake disk bit-clocks and data. The flip-flop is
cleared by DisableRun.

BlockTillndex can be set to disable sector and index task wakeups until (a) the selected
drive is ready, and (b) an index pulse is received from the drive. It is cleared by an index
wakeup. This is useful after switching drives or executing a ReZero operation, either of
which causes the controller to lose sector synchronization with the drive. BlockTilllndex
prevents the wakeup conditions from being set until these conditions are met, but does not
clear any such wakeups that have already occurred. To prevent races, it is necessary to
clear SectorTW and IndexTW, then set BlockTilllndex, then clear SectorTw again.

A request for a sector transfer is initiated by loading bits 8 and 9 of the control register
with a non-zero value. Then the controller will wait until the next sector pulse to set the
"Active" flip-flop and execute the transfer. Once a transfer has been started, it may be
aborted by loading a new value into the control register twice. The first will clear the
Active flip-flop, and the second will load the control register. (When Active, the control
register is enabled for shifting commands rather than loading of io data.)

### 9.6. Format RAM and Sequence PROMs

The format RAM is a 16-word by 12-bit register that holds commands and delay counts
used by the contrqller during a transfer. Words within the RAM are used according to the
following table; the example values are appropriate for Alto Diablo disk emulation (2-word
. header, 8-word label, and 256-word data record).

<!-- PDF page 105 / printed page 98 -->

Example
Addr   Description                                                           Value

00    Word count of the first block                                          0001
01    Word count of the second block                                         0007
02    Word count of the third block                                          0377
03    Word count of the fourth block                                         0000
04    Control tag command for a read operation                               0104
05    Control tag command for a write operation                              0204
06    Control tag command to set Head Select                                 0004
07    Control tag command to zero the tag bus                                0000
08    Word count to write zeroes before writing the 1st block of a sector    0033
09    Word count to write zeroes before writing the sucessive blocks         0006
10    Word countto wait before reading the 1st block of a sector             0011
11    Word count to wait before read ing the sucessive blocks                0002
12    Word count of ECC words plus one                                       0002
13    Word count of 2                                                        0001
14    Word count of 1 (minimum count)                                        0000
15    Notused                                                                0000

Notice that the format RAM contains both word counts and tag commands. Word counts
are 1 less that the desired count. Tag commands will be loaded into the tag register (see
below) and then used as a "control tag function" by the Trident disk. The values in the
right column are those used for the Alto Diablo emulation format. Notice that all but the
first 4 values are determined by characteristics of the drive being used as opposed to the
specific sector format. The meaning of the tag command values can be found in the "Tag
Register" section.

The format RAM is addressed in two ways. During a transfer, sequence PROMs move data
from the RAM into either a tag register or a count register. At other times, the Dorado may
address the RAM with the RAM Address register, which is zeroed when the control register
is written; executing an Output to the DiskRam register writes 108 into the RAM at the
current address and then increments the address. Loading the last word in the format
RAM turns on the EnableRun flip-flop allowing normal disk control activity. The format
RAM may be read via the muffler scheme discussed later.

There are two sequence PROMs, one for reading (or checking) and one for writing. The
PROMs are addressed by a program counter that is initialized to zero at the beginning of a
. sector and is incremented upon completion of each PROM program action. Either the read
PROM or the write PROM is selected according to the operation being performed on the
current block.

The sequence PROMs are clocked by WordClock, which is derived from the disk bit clock,
which in turn is derived from timing information pre-recorded on the disk pack. The
subsector pulses generated by the drive are also derived from this timing information. This
enables very precise placement of the data on the disk, in a manner that is independent of
the disk's rotational velocity or the Dorado's clock rate.

<!-- PDF page 106 / printed page 99 -->

The read and write sequence PROMs are described in the following tables.

Write Sequence PROM                                                               Duration
Addr Description                                                                  (WordCloc/<'s)

00    Issue tag command in RAM[6] (head select)                                   1
01    Delay (wait for head sele!..t to settle)                                    RAM[13] +1
02    Issue tag command in RAM[5] (write command)                                 1
03    Write long preamble for first block                                         RAM[8]+1
04    Write sync word                                                             1
05    Write data for first block                                                  RAM(O)+1
06    Write first ECC word                                                        RAM[14] +1
07    Write second ECG word and 2 postamble words                                 RAM[12] +1
08    Advance control register to the operation for the next block                RAM[14] + 1
09    Issue tag command in RAM[5] (write command)                                 1
10    Write short preamble for second block                                       RAM[9]+1
11    Write sync word                                                             1
12    Write data for second block                                                 RAM[l] +1
13    Write first ECC word                                                        RAM[14] + 1
14    Write second ECG word and 2 postamble words                                 RAM(12) +1
15    Advance control register to the operation for the next block                RAM[14]+1
16-22 Same as 09-15, except step 19 uses RAM[2] + 1
23-29 Same as 09-15, except step 26 uses RAM(3) +1
30    Zero the tag bus                                                            1
31    Notused

Read Sequence PROM                                                                Duration
Addr Description                                                                  (WordClocks)

00    Issue tag command in RAM[6] (head select)                                   1
01    Delay (wait for head select to settle)                                      RAM[13] +1
02    Delay (skip over early part of preamble)                                    RAM[10] +1
03    Issue tag command in RAM[4] (read command)                                  1
Note: WordCloc/<.s cease until control/er has read sync word from dis/<,
04    Read data for first block                                                   RAM[O] +1
05    Read ECC words                                                              RAM[13] +1
06    Compute first word of EGG remainder, issue tag command in RAM[6]            1
07    Compute second word of ECC remainder                                        RAM[14] +1
08    Advance control register to the operation for the next block                RAM[14] +1
09    Delay (skip over early part of preamble)                                    RAM[11] +1
10    Issue tag command in RAM(4] (read command)                                  1
Note: WordCloc/<.s cease until control/er has read sync word from disk
11    Read data for second block                                                  RAM(1) + 1
12    Read EGC words                                                              RAM[13]+1
13    Compute first word of ECG remainder, issue tag command in RAM(6)            1       .
14    Compute second word of ECG remainder                                        RAM[14] +1
15    Advance control register to the operation for the next block                RAM[14] +1
16-22 Same as 09-15, except step 18 uses RAM[2] + 1
23-29 Same as 09-15, except step 25 uses RAM[3] + 1
30    Zero the tag bus
31    Notused

### 9.7. Tag Register

The 16-bit tag register drives the tag bus on the daisy chain cable; all disk drive commands
are initiated through the tag register. The tag register is sometimes loaded from IOB via an
Output command to DiskTag, sometimes from the format RAM. Loading a Head Tag,
Cylinder Tag, or Control Tag into the tag register (from either source) activates a timing
circuit that handles all timing requirements of the Trident drive as follows: Only the tag bus
bits are enabled for the first 200 ns; then the Tag[0:3] bits are also enabled for 1.2 p.s;
finally, the Tag[0:3] bits are disabled again and the TagTW f1ip-f1op is set to wakeup the
disk task (indicating completion of the Tag instruction). The Drive Select Tag (Tag[O]) does
not activate the timing circuit, since the timer counts disk clock cycles, but disk clocks are
invalid during drive select changes.

<!-- PDF page 107 / printed page 100 -->

Bits 4 through 15 of the tag register are interpreted according to the following table:

Tag[O] Drive select and subsector count
Tag[4:15] are. interpreted by the controller to effect drive select or
subsector counter changes. The tag timing and wakeup circuit is not
activated; firmware must take care of the timing by first loading Tag[4:15]
as desired but with Tag[0:3] equal 0, then or-ing in the Tag[O] bit and
outputting again.
4:9     Subsector count
Divide the 117 subsector pulses from disk by subsector count + 1 to form Sector
pulses (Tag[4:5] are presently unimplemented).
Tag[4:9] = 3 yields 29 sectors large enough for 256-word data blocks
Tag [4:9] = 6 yields 16 sectors large enough for 512-word data blocks
Tag[4:9] = 148 yields 9 sectors large enough for 1024-word data blocks
10     Load subsector from Tag[4:9] for the drive selected prior to the execution of this
tag instruction.
11: 15   Drive select
The basic controller handles up to 4 disk drives; additional units may be
accommodated by adding drive dependent logic on an additional board and
connecting it in in place of drive 3. To allow this, the 5 bit drive select field is
interpreted as follows.
o. 3       select drive 0 to 3, respectively
4 . 368    select drive 3
378         don't select any drive

Tag[1] Head Tag
Loads a register in the drive that selects the head to be used during
subsequent read/write commands. A Tag wakeup occurs at completion
(1.6 ILs).
4:7     Unused
8     Off Cylinder-may be activated during a read to attempt recovery of unreadable
data.   It causes cylinder positioning to be offset 80 micro-inches.
9     Determines direction of offset if bit 8 is set.
10:15    Head number-values from 0 to 4 are valid for a T-80, 0 to 19 for a T-300. The
drive will turn on "EndOfCylinder" (alias HeadOverflow) error if an invalid head
address is issued.

Tag[2] Cylinder Tag
Causes the drive to seek to the specified cylinder. A Tag wakeup occurs
after the tag timing sequence has completed (1.6 ,.,.s), and the NotReady
status bit is raised until the seek has completed (3 to 55 ms depending on
the seek distance).
4:15    Cylinder number (O to 814 for Trident disks presently in use). An illegal cylinder
number will cause DeviceCheck to be raised.

Tag [3] ContrQI Tag
A Tag wakeup occurs at command completion (1.6 ,.,.s) and upon
completion of the last read/write operation in a sector. Generally, Control
Tag commands are issued only by the controller itself (using tag
commands from the format RAM) rather than by the microcode; Device
Check Reset and ReZero are an exception.

<!-- PDF page 108 / printed page 101 -->

4      AltoLeader-special flag to the controller that allows disks written by an Alto
Tricon Controller to be read. This bit should only be used for the Alto Trident
simulation.
5      Unused
6      Strobe Late-causes data recovery circuits within the drive to sample data early
within the data bit time (for recovery when the drive is experiencing excessive
read errors).
7      Strobe Early-like StrobeLate except in the obvious way.
8      Write-turns on the write circuits.
9      Read-turns on the read circuits.
10     Unused
11     Reset Head register-zeroes the head address register in the drive.
12     Device Check Reset-resets all latched error conditions in the drive.
13     Head Select-turns on the head selection circuits, in conjunction with a Read or
Write.
14     ReZero-repositions the heads to cylinder 0 (if the heads are loaded) and resets
the head address register; resets Seeklncomplete and DeviceCheck error
conditions.
15     Head Advance-increments the head address register in the drive.

### 9.8. FIFO Register

Data to/from the disk is buffered through a 16-word FIFO (25 JLs of buffer), which is
read/written with Pd←lnputiOutput←B when TIOA selects DiskData. Each FIFO word holds
16 data bits, 2 parity bits, and a 2-bit field indicating that the next word to be read is either
write, read, or read-and-check type data. During output to the disk, the controller checks
parity both when receiving data on the io bus and again when reading the FIFO. During a
disk read, parity is computed before writing into the FIFO, is passed through the FIFO, and
is then written on the io bus for the processor to test.

### 9.9. Muffler Input

Dorado uses a multiplexor scheme called the muffler system fer reading miscellaneous
logic signals during debugging from the Alto or baseboard. The disk controller also allows
a muffler address to be specified on an Output to the DiskMuff register; in this way, any
DskEth board signal available through the multiplexors (mufflers) is also available for
firmware sampling. Other bits of the DiskMuff register output specify other operations as
follows:
8[0]             Simulate read data of 1 for 1 cycle (for use by diagnostic programs)
8[1]             Simulate read clock of 1 for 1 cycle (for use by diagnostic programs)
8[2]             Clear CompareErr-done by disk task if a read&compare is found to be OK
8[3]             Set ReadDataErr-done by disk task to inhibit future writes
8[4]             Clear the index wakeup flip-flop
8[5]             Clear the sector wakeup flip-flop
8[6]             Clear the tag wakeup flip-flop

<!-- PDF page 109 / printed page 102 -->

B(7]            Clear all error flip-flops within the controller (not the disk drive)
B[8:15]         Muffler address-signals are enumerated below

Following an output to the DiskMuff register, the firmware must wait one cycle before
inputting the selected muffler signal with Pd←Input. The state of the signal selected will be
driven on 108[15], and the remaining bits will be zero. For the purpose of examination
from Midas, the signals are grouped into l6-bit words, as shown in the following table. The
bits within each word and an appropriate explanation follow:

KSTATE      various bits indicating the state of the controller
000     TempSense         see "Dorado Debugging Interface" document
001     IndexTW           disk task wakeup is due to an index pulse; index pulses occur once/disk
revolution (16.7 ms) and are used to synchronize the hardware subsector
counter and the firmware sector counter. An index pulse also causes a
SectorTW.
002     SectorTW          disk task wakeup is due to a sector pulse. To maintain a reliable sector
count in a race-free manner, the microcode must (a) check for SectorTW,
and upon finding it set increment the sector number and clear SectorTW;
(b) check for IndexTW, and upon finding it set zero the sector number and
clear both IndexTW and SectcirTW.
003     TagTW             disk task wakeup is due to completion of a Head Tag, Cylinder Tag, or
Control Tag command. This occurs 1.6 p.s after issuing an Output to the
DiskTag register, and also upon completion of the last read/write transfer
in a sector.
004     RdFifoTW          disk task wakeup is due to presence of at least 3 words in the FIFO during
a normal read or 1 word during a read-and-check. During a normal read,
an Input that reduces the FIFO below 3 words will drop RdFifoTW in time
for a Block to take effect on the 5th cycle following the Input; this permits
a 2-cycle loop (Input, Block). During a read-and-check,an Input that
empties the FIFO will drop RdFifoTW in time for a Block to take effect on
the 3rd cycle following the Input; this permits a 4-cycle loop (Input, no-op,
no-op, Block).
005     WrFifoTW          disk task wakeup is due to space for at least 4 words in the FIFO. An
Output that reduces the free space below 4 words will drop WrFifoTW in
time for a Block to take effect on the 5th cycle following the Output; this
permits a 2-cycle loop (Output, Block). WrFifoTW is enabled to occur by
selecting TIOA[DiskData] when a write command is in progress; it is
disabled by TIOA[DiskControl], which the microcode executes after
outputting the last data word of a block. One more WrFifoTW will occur
after all data has actually been sent to the disk.
006     ReadData          Data bit from the disk (available for diagnostics)
007     WriteData         Data bit to the disk (available for diagnostics)
010     EnableRun         Format RAM has been written, and wakeups are enabled
011     DebugMode         Controller has been placed in debug mode
012     RdOnlyBlock'      The controller is processing a block in normal read mode
013     WriteBlock'       The controller is proceSSing a block in write mode
014     CheckBlock'       The controller is processing a block in read and check mode
015     Active            The controller is processing a command for the current sector
016:017   Select.0.. 1      The address of the currently selected drive unit

KSTAT       various bits indicating the status of the drive/controller. The controller will
turn on Writelnhibit for the remainder of the sector after any of the following
errors are detected, but will still go through all the motions of word
transfers.

<!-- PDF page 110 / printed page 103 -->

020      Seeklnc          The disk drive has not correctly positioned the heads within the last 700
ms.      A ReZero command must be issued to clear this error.
021      HeadOvfl         The head address given to the disk drive is invalid (i.e., greater than 4 for
a T-80 drive).
022      DevCheck         One of the following errors occurred:
Head select, Cylinder select, or Write command and disk not ready            .
Illegal cylinder address.
Offset active and cylinder select command.
Read-Only and Write.
Certain errors during writing, such as more than one head selected, no
transitions of encoded data or heads more than 80 micro-inches off
cylinder.
A ReZero command may be necessary to clear this error.
023      NotSelected      The selected drive is in "off line" test mode or the selected drive is not
powered up
024      NotOnLine        The drive is in test mode or the heads are not loaded
025      NotReady         There is a cylinder seek in progress or the heads are not loaded
026      SectorOvfl       The controller detected that a command was active when the next sector
pulse occurred. This error implies either a hardware malfunction or a
discrepancy between the sector format of the drive and the word count the
program thinks is appropriate.
027      FifoUnderflow    Either the FIFO became empty while writing (task got behind) or the FIFO
had too many words taken out of it while readng (microcode word count or
wakeup error).
030      FifoOverflow     Either the FIFO became full while reading (task got behind) or the FIFO
had too many words put into it during writing (microcode word count or
wakeup error).
031      ReadDataErr      A flip-flop in the controller for latching one of three errors:
CompareErr      a read-and-check operation was executed on a block,
and the microcode did not issue ClearCompareErr before
the beginning of the next block.
ECCError        the microcode can set the ReadDataErr flag if it
determines that the ECC words after reading one block
are non-z:ero in order to inhibit future writes_
ECCComputeErr The ECC hardware within the disk controller failed to
generate a single "1" bit (Le., a hardware malfunction).
032      ReadOnly         The "Read-Only" switch on the drive is on.
033      CylinderOffset   The cylinder position is currently offset. This is a mode used for recovery
of bad data.                                                .
034      IOBParityErr     The controller detected bad parity on the IOB bus.
035      FifoParityErr    The controller detected bad parity on the data out of the FIFO.
036      WriteErr         OR of errors on muffler addresses 020-035
037      Read Err         OR of errors on muffler addresses 020-031 and 034-035

KRAM        contents of the format RAM
040:043   Address of format RAM word
044:057   contents of format RAM word

KTAG        contents of the tag register

060:077   20 bit value last loaded into the tag register

KFIFO       state of the io control logic

<!-- PDF page 111 / printed page 104 -->

100      Shiftln           The controller is currently shifling data into the FIFO
101      ShiftOut          The controller is currently shifting data out of the FIFO
102      ComputeECC        The controller is currently shifting data and computing the ECC checksum
103      NextBlock         Occurs between blocks within a sector
104      LoadTag           Indicates that the next word read from the format RAM should be loaded
into the tag register as opposed to the count register
105      CntOone'          Indicates that tlie count register is again zero, and a new value from the
format RAM will be loaded next
106      OutRegFull        The holding register on the input to the FIFO has been loaded, but not
transferred into the FIFO.
107      InRegFull         The holding register out of the FIFO has been loaded, but not read via
Pdl-Input or loaded into the output shift register.
110:113    FifoWaddr         The 4-bit address indicating where the next word will be written into the
FIFO
114:117   FifoRaddr          The 4-bit address indicating where the next word will be read from the
FIFO. if FifoWaddr equals FifoRaddr then the FIFO is defined as empty.

### 9.10. Error Detection and Correction

To allow high data density and a few surface imperfections during manufacture, Trident
disk packs are not required to be perfect. A disk pack is defined as suitable when no more
than three bad areas occur on any data surface; a bad area is defined as one which could
potentially cause read errors of no more than 11 bits in length. To correct errors arising
from these imperfections as well as other (infrequent) read errors, the controller implements
an error detection and correction scheme which will detect (with very high probability)
errors of any length, and will allow correction of any burst error of 11 bits or less.
Warning: If an error burst longer than 11 bits occurs, there is a significant possibility that the error
correction algorithm detailed below will fail and double the number of bad bitsl Consequently, disk
handling programs should try other methods of error recovery before invoking the error-correction
algorithm.

To avoid problems, it is good practice to run diagnostic programs on new disk packs; note bad
sectors and don't use these during normal operation.

When an error does occur, the first step is to try rereading the offending sector several times. One
of these reads may succeed. If not, try rereading with the cylinder position offset or with the data
strobe early or late as discussed in the "Tag Register" section. If these attempts all fail, then try
error correction.

Error correction is accomplished through a mixture of disk controller hardware (for ECC
generation and checking) and system software/firmware (for error recovery). This is a
compromise between capability, speed, and cost. The basic capabilities and restrictions of
the 32-check-bit scheme are summarized below.

1) A single error burst of length less than 12 data bits (Le., a scattering of error
bits within the bit stream, all of which fit within an 11-bit span) can be corrected in
blocks shorter than 2685 data words. (Example: for the data "000.:!.1OQ101 ", the
data "000Q101101" contains a single burst error of length 4.).              The code
implemented will detect errors in arbitrarily long blocks, but not enough
information exists to correct longer blocks.

<!-- PDF page 112 / printed page 105 -->

2) Simple error detection-two words are returned by the hardware which are both
zero if the read is successful.

3) Software/firmware error correction can be completed in less than one disk
revolution. The correction procedure is well suited to a mixture of software and
firmware. If done entirely in firmware, error correction would take less than 1 ms.

4) Not all uncorrectable errors will be detected as such. An uncorrectable error
requires two bad spots on the disk surfac,e within one sector (the pack is
bad-throw it outl), an electronic error in a sector with a bad spot, or two
electronic errors within one sector. If such an error has occurred, it can, with a
probability of say 20 percent, result in an error pattern and displacement that
seems valid. This will result in leaving the error bits uncorrected and changing
some bits which were in fact correct. This means that for high data security, a
check code should be generated and imbedded as part of the data file before
writing on the disk.

The error-correcting code (ECC) generated is referred to as a Fire Code (see Error-
Correcting Codes by Peterson). The following is a detailed description of this code and
recovery procedure.

The code calls for dividing the outgoing data stream by a polynomial of the form:

P(X) = P1(X)(Xm + 1)

Where P1{X) is an irreducible polynomial of degree n (n = burst length) and m is > 2*n.
For this particular application the polynomials chosen are:

P(X)    = (X11 + X2 + 1)(X21 + 1)
During a write, the two polynomials are multiplied together and implemented by hardware in
the form:

P(X) = X32 + X23 + x 21          + X11 + X2 + 1

The data stream is premultiplied by X32 to make room for the 2 word ECG and then
reduced modulo P{X). This is accomplished by the normal feedback shift register
technique with the difference that to perform premultiplication, the output of the register is
exclusive-or'd with the incoming data and then fed back. After all data bits have been
shifted out, the contents of the EGG shift registers are appended to the disk block.

During a read, the feedback shift register is reconfigured such that the two original
polynomials are iiTlplemented separately. The incoming data stream, including the 2
appended words of ECC, is independently reduced modulo Po (X) and P1{X), where

PO{X)   = X21    + 1

<!-- PDF page 113 / printed page 106 -->

After reading in all words off the disk, the contents of the two polynomial shift registers are
read out of the FIFO. If the data is recovered without error, then reducing it modulo PO(X)
and P1(X) results in the registers containing all zeroes.

If the data contains an error, then the two registers will be non-zero. If one but not both
registers is non-zero, then the error is irrecoverable.

To recover from an error, a procedure is undertaken which determines the pattern of bits
which are in error, and the displacement of ~his pattern from the end of the record. I am
simply going to present the magic equation to be solved, and some magic constants to be
used for solving this equation. Much of the polynomial implementation and the equations,
which use the "Chinese Remainder Theorem" are discussed in technical reports from
CALCOMP (Calcomp Technical Report TR-1035-04, by Wesley Gee and David George) and
XEROX (Xerox XOS preliminary report "Error Correction Code for the A.M. Subsystem," by
Greg Tsilikas, 28 March 1972.).

The basic equation is:

where:
Ei = modulus of the polynomial
LCM = least common multiple of EO and El
Mi = LCM/E i
Ai = a constant such that Ai'" Mi modulo Ei = 1
Q    = smallest integer to make 0 positive
Si = number of shift operations to the appropriate polynomial remainders as
described below.
o   = displacement of right-most incorrect bit from the end of the record.

The values of EO and El were found by programming the procedure outlined in the
CALCOMP report. and yielded the following result:

EO   = 21             E1   = 2047
The least common multiple (LCM) of EO and El is simply the product of EO and El since
the two numbers have no factors- in common. Thus the LCM, which is also the record
length which can be corrected, is 42,987 bits, or 2686 - 2 words. '

Knowing LCM and EO and E1, the values of MO and M1 are easily found to be

MO   = 2047
The values of AO and A1 are next determined using a trial and error approach that I put in
a small program. The results can easily be confirmed, and are given below:

<!-- PDF page 114 / printed page 107 -->

AO   = 19
All of the above values derived so far are constants determined for the particular
polynomials chosen. The values of So and Sl are determined in the software from the
error patterns returned at the end of a disk transfer.

So is first determined by a software procedure using the following steps:

1) The remainder from dividing the input data by X21 + 1 is found in ECC[ll :31];
if this remainder is zero, then the error is uncorrectable.

2) Test the low order 10 bits for all zeroes, and if not then perform a left circular
shift on the 21 bits. When the low order 10 bits are all zeroes, the error pattern is
in the upper 11 bits of the word, and So is the number of times the circular shift
was performed.,
r
3) If the low order 10 bits don't become all zeroes within 20 shifts (1 full cycle), the
error is uncorrectable.

Sl is then determined in microcode as follows:

1) The remainder from dividing the input data by X11 + X2 + 1 is found in
ECC[0:10]; if this remainder is zero, then the error is uncorrectable.

2) Test this number to see if it is equal to the error pattern determined in step 3 of
So' and if not reduce this number modulo X11 + X2 + 1 (left shift and XOR
feedback). When the contents of this word equals the error pattern (it is
guaranteed to happen before 2047 reductions), Sl is determined as the number of
reductions performed (In the hardware implementation of switching from the write
polynomial to the read polynomials, it was easier to implement a polynomial that
premultiplied by X11. This means that the remainder returned by the hardware
already has had 11 shifts performed. To compensate, when Sl has been
determined by the above procedure, you must add 11 to the value, and subtract
2047 if the result is greater than or equal to 2047.).

The basic equation for the displacement now looks like

D = Q*42,987 -        19*2047*SO -       195*21 *8 1

where:
o ~ So ~ 20
o ~ Sl ~ 2046
Notice that the straightforward solution to this equation cannot be done with single-
precision arithmetic on the Dorado; to avoid double precision, the following manipulation of
the equations is useful:

<!-- PDF page 115 / printed page 108 -->

D = 0*2047*21 - 19*2047*80 - 4095*8 1
D = 0*2047*21 - 19*2047*80 -. 2*2047*8 1 -              81
D' = 0*21 - 19*80 - 2*8 1

where:
o ~ D' < 20
D    = 2047*D' -    81      (add 42,987 if D'    = 0)
For some reason that we don't understand, the actual required calculation must be 0 =
2047*(0' + 1) - S1 in the last step. Also 0' is conveniently calculate.d as (215*21 - 19*5o
- 2*S1) rem 21.

<!-- PDF page 116 / printed page 109 -->

## 10. Display Controller

The Dorado Display Controller (DOC) uses the fast io system to obtain representations of
video images from storage; it then transforms these representations into control signals for
monitors. Its three design objectives are:

(1) To handle a variety of color, grey-level, and binary (black-and-white) monitors;
(2) To utilize the full power of the fast io system in producing high-bandwidth
computer graphics;
(3) To allow various compromises in color and spatia-temporal resolution for
experimental purposes. Clock rates, video signals, and other monitor waveforms
should be controllable by firmware.

There are two independent video channels capable of running in a variety of modes. Two
channels allow text to be displayed on one channel, graphics on another, or the main
picture on one, cursor on the other.

The DOC must readily handle Alto-style and LF (large format) monitors which we expect to
be standard for most systems. Bit maps, display control blocks, and monitor control
blocks, similar to those used on the Alto, provide the software interface to the DOC. The
"seven-wire" video interface makes provision for one or more low bandwidth input devices
(keyboard, pointing device, etc.); our current provisions for keyboard and mouse input are
also discussed in this chapter.

Keep Figure 14 in view while reading this chapter.

### 10.1. Operational Overview

Video scan lines are encoded in bitmaps, which are contiguous blocks of virtual memory;
the two channels, A and B, have independent bitmaps and data paths in the DOC. The
high-priority DWT (Display Word Task) runs on behalf of either A or B using the subtask
mechanism; it transmits each bitmap to a FIFO consisting of 15 munches/channel. The
bitmap stream emerging from the FIFO is then sorted into items (1, 2, 4, or 8 bits wide) for
each channel which are combined, mapped, and transformed into pixels (picture cells) on
the screen.

In addition to the two channels, the DOC supports a programmable cursor that is 16 pixels
x 1 bit/pixel wide.

A lower priority DHT (Display Horizontal Task) handles horizontal and vertical retrace and
sets up starting addresses and munch counts, cursor data, and formatting information in
• the NLCB (Next Line Control Block) for the DOC. The NLCB is then copied into the CLCB
(Current Line Control Block) during horizontal retrace prior to the next scan line.

The rate-of-flow of" items is governed by the resolution and pixel ciock period. Resolution
may be independently programmed for each channel so that items flow at 1/4, 1/2, or 1
times the pixel clock period. If the DispM board is present, then the pixel clock period is
also progammable; otherwise, it is determined by a crystal oscillator on the DispY board,
which must have a frequency appropriate for the monitor being driven.

<!-- PDF page 117 / printed page 110 -->

Items can be treated in one of three ways: First, an Alto monitor can be driven. Second,
items can be mapped through the 256-word x 4-bit MiniMixer into video data for a black-
and-white or grey-level monitor.
Three separate interfaces are provided on the DispY board. An Alto monitor interface ORs one-bit
items from the A and 8 channels with the cursor, and then XORs by polarity to produce one-bit
pixels for an Alto display. A seven-wire interface outputs 1 bit/pixel for a binary monitor. And an 8-
bit digital-lo-analog converter (DAC) produces grey-level video.

Third, items may be mapped by the Mixer (or A color map), a 1024-word x 24-bit RAM, into
signals for a color or grey-level monitor. A variety of modes determine which bits from the
A and B items address the mixer. Mixer output consisting of 8 bits for each of the red,
green, and blue guns is then digital-to-analog converted for color monitors. Additionally,
there is a 24-bit/pixel mode in which the Dorado supplies 8 bits for each of the three
colors; the colors are independently mapped through the Mixer and two additional 256-
word x 8-bit RAMs called the BMap and the CMap.

The DDC is implemented on two Dorado main logic boards, called DispY and DispM.
DispY contains all the logic necessary for vertical and horizontal sweep control, channel
data paths, and video data for binary and grey-level monitors running at a fixed pixel clock
rate. DispM contains the color maps, the programmable pixel clock, and the three DACs
for driving a color monitor. Additionally, DispM contains an independent terminal controller
that is structurally similar to a one-channel, one bit/pixel DispY but is specialized to driving
a 7-wire terminal.

Thus there are two principal DDC configurations. On a Dorado with only a 7-wire terminal
and no color monitor, only the DispY board is present; it is programmed for Alto terminal
emulation, and only a small subset of its capabilities are used. However, on a Dorado with
both a 7-wire terminal and a color monitor, the DispM board is also present; all of DispY
and the color hardware on DispM are used to drive the color monitor, and the independent
controller on DispM is used to drive the 7 -wire terminal.

### 10.2. Video Data Path

Fast 10 Interface and FIFO

The fast io system delivers data to the DDC at a rate of 16 bits/clock; words are received
alternately in the REven (t 1 ) and ROdd (t2) registers shown in Figure 14, then written into
the FIFO, a 256-word x 32-bit RAM, during the first half of the next Dorado cycle (t2 to t3),
leaving the second half of the cycle free for read access by the video channels. In other
words, the REven and ROdd registers widen the data path from 16 to 32 bits to allow
sufficient time to both write and read the FIFO in one cycle.

The 256 double-words in the FIFO are divided evenly among the two channels, so each has
buffer storage for 16 munches. Each channel has write and read pointers that address the
FIFO when appropriate.

Write pointers are initialized once during vertical retrace and then sequence through
addresses for the entire display field; a write pointer is incremented after each double-word
write for its channel, so that the next word to be written is addressed at all times. Since

<!-- PDF page 118 / printed page 111 -->

the fast io system delivers only one munch at a time, there is never any problem in
deciding which of the two write pointers should address the FIFO.

Read pointers, however, are initialized during each horizontal retrace, so that the correct
first double-word is read at the start of every scan line. This is required because the fast .io
system always delivers complete munches, but unused double words may appear at the
end of the last munch for the previous scan line, or at the beginning of the first munch for
the current scan line; the read pointer has to be reinitialized to skip over these. FIFO reads
alternate between channels A and B, so the da.ta rate for one channel is limited to 32
bits/2 cycles (= 16 bits/cycle).

Note that bitmaps are required to start at even addresses because the FIFO is 32 bits wide.

Item Formation

At the output end of the FIFO there is a multiplexor shared by both channels and, for each
channel, two intermediate buffers (FIB and SIB), and a shift register SR. The multiplexor
permutes the 32-bit quantity emerging from the FIFO so that when the double-word has
marched through FIB and SIB and is finally loaded into SR, successive shifts will produce
successive items of the selected size (8, 4, 2, or 1 bits).

The SR is tapped as follows:

SR.a                                  Item[a] for item sizes 1, 2, 4, or 8;
SR.16                                 Item[l] for sizes 2, 4, or 8, gated to a for size 1;
SR.8, SR.24                           Item[2:3] for sizes 4 or 8, gated to a for sizes 1 or 2;
SRA, SR.12, SR.2a, SR.28              Item[4:7] for size 8, gated to a for sizes 1, 2, or 4.

. All eight Item bits are gated to a if the channel is off. It is useful to think at this point that,
regardless of a channel's item size, an 8-bit wide item is produced, whose bits contain non-
zero data only in those positions dictated by the item size; i.e., for size 1 only the most
significant bit may be non-zero; size 2 allows data in the topmost two. bits, etc.

The SR loads on the item clock after its last item has been used; the item clock rate is the
pixel clock rate divided by the resolution (1, 2, or 4 for full, half, or quarter, respectively).
Hence, for 8,4, 2, or l-bit items, SR will be shifted 3, 7, 15, or 31 times, repectively, and be
reloaded from SIB on the following item clock.
Synchronization of SR, which uses the item clock, with FIB and SIB, which use the Dorado system
clock, is a little tricky. SIB"FIB will occur no later than (4.6 ns) + C + (1.1 ns) + C + C = 3-C + 5.7 ns
atter SRc-SIB, where C is the period of the Dorado system clock and 4.6 ns and 1.1 ns are the worst
case propagation delay and setup time of the components in the synchronizer; FIBc-FIFO will occur
at this time or on one of the next three Dorado clocks, depending upon which of these four clocks
corresponds to t2 of the cycle in which this channel can read the FIFO. Allowing for propagation
delay through SIB (5.0 ns) and setup time for SR (1.7 ns), the worst case minimum spacing between
loads of SR is 3-C+(5.7 ns)+(6.7 ns) = 3-C+12.4 ns. This must be less than the time for
emptying SR which is 1(32/ltemSize), where I is the period of the item clock.                Hence, I >
(3-C + 12.4)/4 for ItemSize = 8, or I > 25.6 ns for a Dorado clock period of C = 30 ns.

The 8-bit items from the two channels are then presented to either the Mixer section on the
DispM board or the MiniMixer or Alto video interface on the DispY board.

<!-- PDF page 119 / printed page 112 -->

Mixer

The Mixer is controlled by the ABB2, BBypass, and 24Bit mode controls. It is a 1024-word
x 24-bit RAM for which the 10 bits of address required may be obtained from h..,o possible
source distributions, depending upon the A8B2 mode. When A8B2 is true, the address
consists of Altem[O:7] and Bltem[O:1]; when false (called A6B4), the address is Altem[O:5J
and Bltem[O:3J.

Another mode, the BBypass mode, can be enabled independently for the B channel. If B is
bypassed, none of its bits contribute to the Mixer address. Instead, they bypass the mixer
and address a 256 x 8 RAM, the BMap, whose outputs are ORed with the mixer outputs for
the blue DAC. For example, with ASize =8, BSize =4, BBypass true, and A8B2 true, and
with appropriate values in the Mixer RAM, the controller may be thought of as three 4/bits
pixel channels driving three color guns. One channel is bypassed data from B, while the
other two are mapped through the Mixer.

24Bit mode, used in conjunction with BBypass mode, is used to run a three-channel color
display directly from memory. In this mode, items from the A channel alternately address
the Mixer (called the AMap in this mode) and another 256 x 8 RAM called the CMap.
Meanwhile, the B channel runs at half the A channel rate and addresses the 8Map as
described above. (That is, the B channel must be set to one-half the resolution of the A
channel.)    With suitable values in the color maps, the AMap, BMap, and CMap
independently generate outputs for the red, blue, and green DACs respectively.
Note: when the A channel is turned on, the tirst Altern addresses the AMap and the second Altern
addresses the CMap. For the A and B pixels to align properly on the display in 24Bit mode, the left
margin counts must be set to start the B channel one pixel clock earlier than the A channel. The
blue and green portions of the AMap must be entirely zeroed, since the blue and green outputs are
ORed with the BMap and CMap.

After routing as dictated by the mixer modes, chosen items are loaded into the map
address registers, causing the color maps to produce a new video value every pixel clock
(every two pixel clocks in 24Bit mode), and these values are latched in the three 8-bit mixer
output registers. Three very fast DAC modules then produce a Red-Green-Blue triple of
analog signals for a color monitor, or up to three grey-level video signals. In conjunction
with the sync, blank, and composite waveforms produced by the monitor control circuitry,
these Signals can drive a wide variety of monitors attached to the Dorado.

Alto Video Interface

A small circuit on the DispY board produces video for an Alto monitor. This circuit ORs
CursorData, Altem[O], and Bltem[OJ, then XORs by the polarity, and finally ORs with the
vertical and horizontal blanking signals. This interface is obsolete and is no longer in
active use.

MiniMixer

A small video mixer on the DispY board, not to be confused with the large Mixer on the
DispM board, can drive either a DAC or the seven-wire interface discussed later. The
MiniMixer is a 256 word x 4-bit RAM addressed by a combination of Altern, Bltem, and state
bits, as shown in Figure 14. On every pixel clock, dDAC[O:3] are loaded from MiniMixer

<!-- PDF page 120 / printed page 113 -->

output, while dDAC[4:7] are loaded directly from Altem[4:7]. The MiniMixer aims at
experiments with mixing channels and driving grey level monitors.

### 10.3. Horizontal and Vertical Control

Every monitor requires horizontal synchronizing and blanking waveforms.          Interlaced
monitors must be able to distinguish fractions of a scan line to implement interlacing. In
general, the duration and phasing of sync/blank waveforms is unique to a given monitor.
The DOC uses the 1024-word x 3-bit HRam (Horizontal RAM) to control horizontal
sync/blank.

The DOC has a set of registers called the CLCB (Current Line Control Block) which
controls video generation for the current scan line. The DHT sets up parameters for the
next scan line in NLCB (Next Line Control Block), a 16-word x 12-bit RAM. The first 32
pixel clocks of horlzontal blanking are called the HWindow; during HWindow parameters for
the next line are copied from NLCB into CLCB. Vertical control is also handled through the
NLCB.

The interpretation of fields in NLCB and HRam are shown in Figure 15 and loading will be
discussed in the "Slow 10 Interface" section; the use of the different information is
discussed here. The top part of Figure 14 shows how horizontal timing is controlled.

Line Control Blocks

The fields in NLCB/CLCB are interpreted as follows, where a denotes that the item is
channel-specific (i.e., copies exist for both A and B channels):

aPolarity. A single bit, used only for binary monitors, that inverts black and white
(APolarity and BPolarity are or'ed by the hardware).

aResolution. A 2-bit field that controls item clock generation; values of 0,_ 2, and 3
cause quarter, half, and full resolution, respectively.

altemSize. A 4-bit field unary encoded as aSize1, aSize2, aSize4, or aSize8,
denoting bits/pixel for the channel; setting multiple bi~s is illegal.

aLeftMargin. A 12-bit field in units of pixel clocks specifying 31 less than the
number of pixel clocks to wait after HWindow completes before turning the
channel on. This value is not a straightforward constant, but depends upon
monitor-specific horizontal blanking time. If the horizontal blanking time is B pixel
clocks and the desired beginning of data is L pixel clocks after the end of
horizontal blanking, then aLeftMargin should be loaded with B + L - 32 - 31 =
B + L - 63, independent of resolution. Since L may be 0, this implies that the
horizontal blanking time for the monitor must be greater than 63 pixel clocks.
Since high-speed monitors typically have greater than 4 p.s horizontal blanking
times, and are this fast only with high speed pixel clocks, this restriction is not
expected to be significant.

<!-- PDF page 121 / printed page 114 -->

Note: For a monitor connected via the 7-wire interface, aLeftMargin must be B+L-68,
rather than B + L - 63, because video signals are delayed from horizontal control waveforms
by 5 pixel clocks.

Note: The value loaded into aLeftMargin must actually be the negative of the left margin
count computed above.

aWidth. A 12-bit counter that counts at the pixel clock rate as soon as the
channel turns on; when the counter runs out (or when horizontal retrace starts,
whichever is earliest), the channel is turned off. Precisely, if the channel is to run
for W pixel clocks, the width counter must be loaded wit~ - (W + 255).

aFifoAddr. An 8-bit quantity pointing to the munch and word within the munch for
the first FIFO read for the next scan line; this must be an even number because
doublewords are fetched from the FIFO. Firmware must keep track of the number
of used munches for any given line and advance aFifoAddr by exactly the right
amount, adjusting for munch boundaries, interlacing, and data breakage. The
CLCB register for aFifoAddr is the channel read pOinter itself.

MixerModes. A set of bits that control the mixer; these are not channel-specific.
These will normally be changed infrequently, maybe at the field rate or during
display initialization. However, they are in the NLCB to allow modes to change on
the fly.

Vertical Control Word (VCW). A word controlling the vertical retrace operation of
the monitor; it contains the vertical blank bit, vertical sync bit, and interlace field
bit discussed in the "Vertical Waveform Generator" section below.

Cursor and CursorX. The 12-bit CursorX value is loaded into a counter which
starts counting at the end of HWindow. When the counter runs out, the l6-bit
Cursor value is shifted out onto the CursorVideo line. This is used by the Alto
video interface and in the MiniMixer address. Precisely, if horizontal blanking is B .
pixels in duration, and the leftmost bit of the cursor is to appear X pixels beyond
the end of horizontal blanking, then the CursorX register mllst be loaded with
- (B + X + 226), or - (B + X + 221) when using the 7-wire interface.

Horizontal Waveform Generator

The 1024-word x 3-bit HRam contains control information for these waveforms. Under
normal operation, HRam is addressed by a 12-bit counter (HRamAddr[O:ll]) which is reset
at the leading edge of horizontal sync and then increments every pixel clock until the next
leading edge of horizontal sync; HRamAddr[l :10] address the RAM, and the output is
loaded into the HRamOut register every other pixel clock. The three bits in HRamOut
control horizontal sync, horizontal blank, and half-line; these three bits are combined and
level shifted by a logic network appropriate for the monitor being driven.
The 1024-word HRam imposes the uninteresting restriction that there be fewer than 2048 pixels/scan
line.

As shown in the diagram at the top of Figure 14, horizontal blanking (HBlank) is true from
the end of one scan line to the beginning of the next. During horizontal blanking, HSync is
turned on to initiate the horizontal retrace and turned off again when horizontal retrace is

<!-- PDF page 122 / printed page 115 -->

finished. HBlank then continues for a monitor-specific interval. Note that if a channel's
visible left margin is non-zero, then the horizontal scan will begin before that channel is
producing any data; in this case, the video channel outputs zero items to the mixing stages
until the channel is turned on.
Due to an implementation error, when the 7-wire interface is being driven from DispY, the value of
HBlank[/1 may difler from HBlank[i -1] only when i is even, where i is HRamAddr[1 :10).

Vertical Waveform Generator

Only 2:1 interlaced monitors are supported in this design, but more complicated vertical
control could be provided, if desired. To support 2:1 interlace, HRam contains a waveform
called HalfLine, which is a pulse at the horizontal line frequency, 1800 out of phase with
HSync.

Vertical control is handled by DHT through the NVCW word in the NLCB, which specifies
whether or not ver.ical blank or retrace should begin or end during the next scan line. The
DHT microcode must keep track of scan lines to enable vertical signals at the appropriate
times.

The three VCW bits are called VBlank, VSync, and OddFieJd. VSync enables- vertical sync
to begin on the next line, and the Odd Field bit chooses either HSync or HalfLine on which
to do vertical syncing (Odd Field =1 implies HalfLine phasing for vertical sync). This phase
will alternate from the start of the line to the middle of the line and back for successive
fields. The blanking signal for the monitor is VBlank ORed with HBlank.

### 10.4. Pixel Clock System

The programmable pixel ctock on the DispM board, if present, determines the fundamental
video data rate for a given monitor. The pixel clock is controlled by loading the PixelClk
register via the slow io system. The pixel clock frequency is (312.5* (241 - M»/(16 - D)
KHz, where M is PixeIClk[4:11] and D is PixeIClk[12:15]. Note that the pixel clock will not
stabilize until about 1/2 second after the PixelClk register is loaded.

The parts of the DDC synchronized to the rest of Dorado do, of course, use the Dorado
system clock. As discussed earlier, the synchronization logic for refilling SIB after SR←SIB
puts a lower bound on the pixel clock period of (3*C + 12.4)/4 ns (= 25.6 ns for a Dorado
clock period of C = 30 ns), for an item size of 8 on either channel. We anticipate that
pixel clock rates in the range 10 to 50 MHz (100 to 20 ns/pixel) will be required, so the
lower bound is approximately consistent with this.

<!-- PDF page 123 / printed page 116 -->

### 10.5. OIS Seven-Wire Video Interface

So that a number of different controller and terminal types may be freely interconnected in
Dolphin and Dorado-based systems, a common interface between terminals and controllers
has been defined. This interface assumes that a terminal contains a raster-scanned bitmap
display and one or more low bandwidth input devices (keyboard, pointing device, etc.) The
DDC transmits digital video and sync to the terminal over six pairs of a seven-pair cable.
The input data is encoded by a microcomputer in the terminal and sent back serially over
the seventh pair (the "back channel"). Video and control (sync) are time-multiplexed, and
four bits are transmitted in parallel to reduce the cable bandwidth required.

While the description in the following sections assumes a display having one bit/pixel, the
basic signalling mechanism may be extended to support gray-level or color displays.

Video Output .

The four output lines are interpreted as either a 4-bit nibble of video or four control signals
according to the phases of the two clock signals; the DDC places data on the data lines at
the falling edge of ClkA, and the terminal samples this data on the rising edge of ClkA. If
ClkB is 1 at this time, the nibble is interpreted as four bits of video, else. as sync and
control information. ClkA and ClkB are transmitted in quadrature so that the terminal can
reconstitute a clock at the video bit rate.

When a nibble is interpreted as control information, bit 2 is reserved for horizontal sync
and bit 3 for vertical sync, while 0:1 are undefined; different types of terminals may use 0:1
for any purpose.

A circuit on the DispY' board drives the seven-wire interface from the MiniMixer.
MinMixer[O] is serial-to-parallel converted into four-bit nibbles, which are held in a register
for transmission. Sync, blank, and clock phases are generated in accordance with the
seven-wire interface specification.

Back Channel

Data from low bandwidth input devices at the terminal are transmitted serially over the back
channel. Data are clocked by the terminal on the rising edge of the horizontal blank pulse
and are sampled by DHT during the subsequent scan line after HWindow.

By convention the terminal microcomputer encodes 32-bit messages (delivered in 32 scan
lines); each message begins with a 1, and after the 32nd bit of the message the DHT
ignores the backchannel until the start of another message is indicated by another 1. The
message consists of a start bit, 3 unused bits, a 4-bit message type, a 16-bit message body,
and finally an 8-bit trailer which must be 2008 ,

The terminal microcomputer perpetually cycles through all possible keys on the keyboard
(as well as mouse buttons and keyset paddles), detecting changes in state of the keys; the
state of the keyboard then exists in seven l6-bit words, and a back channel message is
defined for each. Whenever one of these words changes value, it is sent to the Dorado in
a message. Additionally, changes in mouse x,y coordinates are reported once per field
(Le., twice/frame or typically 60 times/sec). If the mouse has not changed position during

<!-- PDF page 124 / printed page 117 -->

a field, then one keyboard word is reported instead of the mouse position change; thus, the
correct state of the keyboard is eventually reported even if transitions are missed.

> **Table 24: OIS Terminal Microcomputer Messages**

![Table 24: OIS Terminal Microcomputer Messages](tables/table-24-ois-terminal-messages.png)

### 10.6. Processor Task Management

This section outlines the implementation requirements of DHT and DWT and discusses the
hardware associated with task wakeups and DWT subtask arbitration between the two
channels.

Since DHT must do a lot of processing, it runs at low priority and is awakened once/scan
line at the end of HWindow. When it runs, it must calculate all parameters for the next
scan line (i.e., the one after the scan line that is just starting), load the NLCB appropriately
for each channel, and set up the munch address and count for each channel in the RM
registers aNextAddr and aNextCount referred to in the DWT sample code below; then it
sets the aNextWCBFlag flags discussed below. The DHT wakeup will remain active until
any NLCB output command is executed, so the DHT must execute at least one NLCB
output command every time it wakes up, and this must occur at least three instructions
prior to blocking.

DWT is a very high priority task which may run on behalf of either channel: channel A is
subtask 0; channel B, subtask 2. Since it uses the subtask mechanism, DWT must always
block at the same instruction each iteration. DWT does not explicitly know the channel for
which it is executing at any given time; its two parameters, a start address and munch
count, are received from DHT in RM registers specific to the subtask. In the normal case,
DWT initates an IOFetch and blocks. The following is the main line DWT microcode
presently in use:

<!-- PDF page 125 / printed page 118 -->

%RM registers for channel A, indicated by names beginning with" A" below, are used in the Program, but the
corresponding set of registers for channel B, in a different RM region, will be referenced when SubTask is 2.

Note that TIOA selects the DWTFlag register and T contains 20 at the beginning of the loop, so the second
instruction is used both to increment the munch address and to signal the hardware that an IOFetch Is
commencing.
%
DWTStart:        ACount←(ACount) - T, Branch[DWTCheck, R(O];
AAddress←(IOFetch←AAddress) + (Output←   n,
Block, Branch[DWTStart];

%AAddress will be even if we just exhausted a scan line. AAddress will be odd If we have just been awakened to
start a new scan line. In either case, isolate flag In AAddress[15] for use In adjusting the WCB flags.
%
DWTCheck:          AAddress← (AAddress) AND (1C), Branch[DWTAdjustWCBFlags, R even];
%Note that the change-RSTK-for write function used below is ok, but the change-RBase-for-write functions are
illegal because of subtasking.
%
DWTRefill:       ACount←ANextCount;       from DHT, # munches to fetch-1 in 0:11
BrLo← ANextAddrLo;       flrst munch address
BrHI ← ANextAddrHi;

%Now adjust WCB flags, as follows: If we just exhausted a scan line, AAddress = 0 now; execute Output←O to
clear the CurWCB flag, and set AAddress to - 1 for the next wakeup. If we are starting a new scan line,
AAddress = 1 now; execute Output←1 to set the CurWCB flag and clear the NextWCB flag, and set AAddress to 0
for the first IOFetch.
%
DWT AdjustWCBFlags:
AAddress← (AAddress) - 1. Output4- AAddress. Block, Branch[DWTStart];

DWT lowers its wakeup request at the onset of the DWTStart instruction, and the DDC
remembers that DWT is in progress. No further DWT wakeups will be generated while the
task is running or is preempted by a higher priority task. Whenever DWT blocks, a counter
is initialized to a constant value N and counts once per Dorado cycle; when the counter
runs out. DWT wakeups are allowed again. This counter has two purposes. First, within a
munch loop it spaces out IOFetch references to the memory system by 8 or more cycles
(depending upon N, which is adjustable through a hardware SIP component), so as not to
clog the memory pipeline. Second, the decision to generate subsequent DWT wakeups is
based upon the state of flags that may be altered by output commands; these commands
take time to get from the processor to the DOC and alter the state. Other tasks may have
the processor while these state changes take effect.

After N cycles have elapsed, DWT will be woken whenever aWantsDWT is true for one of
the channels.     Two channel-specific flags are involved in DWT wakeup control:
aCurrentWCBFlag is true when a is actively moving words into the FIFO; aNextWCBFlag is
set true by DHT after it has loaded the munch address and munch count into DWTnextaddr
and DWTnextcount for a. After fetching the last munch for a scan line, DWT clears
aCurrentWCBFlag and blocks unless aNextWCBFlag is true. In other words, aWantsDWT
when

(aNextWCBFlag & not aCurrentWCBFlag) %
(aCurrentWCBFlag & aFifoAvaiiable).

If only AWantsDWT or only BWantsDWT, no conflict arises and the requesting channel gets
DWT. However, if both channels want DWT, the channel that ran least recently will run
next.

<!-- PDF page 126 / printed page 119 -->

Two observations must be made about the DWT microcode. First, because the final
instruction is normally an 10Fetch←, the next instruction executed (by another task) will be
held one cycle if it initiates any memory reference. Secondly, the two instruction loop
above requires that the hardware cope with the NextLies condition discussed in the "Slow
10" chapter; a pathological lockout problem could occur if a high demand task of higher
priority is coded so that it always creates NextLies (say, by doing Block and immediate
←Md in the instruction after a fetch). This would result in the DWT wakeup being
frequently delayed by 2 cycles.

Note:   Neither DWT nor DHT drives the 10Atten branch condition.

### 10.7. Slow IO Interface

DDC manages all control functions via the slow io system. At this point you should study
Figure 15, which ~hows the format of the various output and input commands; there are six
output devices and one input device on the DispY board, and eight output devices and one
input device on the DispM board (if present). Output commands are handled uniformly:
TIOA is clocked into a register at t1 ; the register output is decoded and identified as one of
the DDC commands; if the processor is doing an Output←B, then at                 ta
IOB data from the
processor is clocked into a register and one of the "TIOA command" pulses occurs from t3
to ts' at which point the desired action is complete.

The IOB data received at      ta
of an Output←B will remain in the DDC buffer register (RIOB)
until the next output command. This is useful for debugging and for muffler readout of the
NLCB (because an NLCB address can be loaded into RIOB for multiple cycles).

The HRam, MiniMixer, Mixer, BMap, and CMap are RAMs that will generally be loaded
during system initialization and not often changed while pictures are being displayed. The
programmable pixel clock will also be loaded during initialization, if it is being used instead
of the fixed crystal oscillator.

The HRam, Mixer (AMap), BMap, and CMap addresses each have two' independent
sources: the Dorado slow io system and the video system. Video system addressing is
disabled during loading from the Dorado. The output commands to each of these RAMs
are interpreted as follows: The Keep' bit is saved in a flipflop loaded by every RAM output
command; as long as Keep' is true (i.e., low), video system addressing is off. If LoadAddr
is true, then IOB[4:15] are loaded into the RAM address register. If Write' is true (i.e., low),
the currently-addressed word of the RAM is written from the data field; additionally, the
RAM address register increments after writing, so the RAM can be loaded sequentially at
high speed. A RAM output command with Keep' false (i.e., high) releases the RAM from
Dorado control and returns it to the video system.

Note: the LoadAddress and Write' bits of a RAM output command take effect only if the Keep'
flipflop is already true (i.e., set to zero by a previous RAM output command).

Note: in the case of the Mixer, the RAM address is loaded from 108[4:14] and a HilLo Select bit is
loaded from 108[15]. The latter bit determines which 12 bits of the 24-bit wide mixer word will be
loaded by .the next Write'. The HilLa Select bit behaves as a low-order extension of the Mixer
address counter, so successive Write' commands will alternate between the halves of one mixer word
before advancing to the next.

<!-- PDF page 127 / printed page 120 -->

The MiniMixer is loaded by a single output instruction that specifies both the address and
data to be loaded. During the command pulse from t3 to t5 of the Output← B instruction,
the video channel address to the MiniMixer is replaced by the address being loaded, so if
the video channel is active, garbage may appear at the output during this cycle.

The 16-word x 12-bit NLCB is also loaded by single output instructions that specify both
the address and data. For the NLCB, output instructions are only effective when HWindow
is not occurring-during HWindow the RAM address is supplied by a counter that
successively copies the NLCB words into CLCB. ,The format of each of the words in NLCB
is shown in Figure 15. Note that any NLCB output operation w!ll dismiss the wakeup
request for DHT, and DHT must not block any sooner than the fourth instruction after the
first NLCB output operation is issued.

The Statics output command is used for debugging and initialization: Two bits in the
Statics register called DHTShutUp and DWTShutUp are discussed in the "DOC Initialization
Requirements" section below. Three other fields called FakePClk, UseFakePClk, and
MufAddr are used for debugging. When UseFakePClk is true, the regular pixel clock is
degated; if FakePClk is true, then a pixel clock will occur at t5 of the Statics output
command; otherwise no clock occurs. Every Statics command also loads the hardware
signal addressed by MufAddr into a flipflop (at t5> which can be read by the Status input
command discussed below. In combination, the fake pixel clock and muffler readout
features allow diagnostic firmware to checkout most of the internal data paths in the
DOC-by simulating a very slow pixel clock and "stepping" the DOC through various states,
the diagnostic can check nearly all of the data paths between fake pixel clocks. The
hardware signals selected by MufAddr[5:11] are given in the table below.

> **Table 25: DDC Muffler Signals**

![Table 25: DDC Muffler Signals](tables/table-25-ddc-muffler-signals.png)

Muffler 106 (MonitorType) is the only one of interest during normal operation. It identifies
the type of monitor connected via the 7 -wire interface: zero denotes an Alto-style monitor;
one denotes an LF (large format) monitor.

<!-- PDF page 128 / printed page 121 -->

A single input device called Status is implemented. It is used to return the currently
selected muffler bit and the seven-wire interface received data bit.

The MaplnLo and MaplnHi input devices read the current values output from the color
maps (Mixer, BMap, and CMap, whichever are active). When the color maps are controlled
by the video system, these outputs change too rapidly for reading them to be useful (unless
the DDC is being single-stepped by means of UseFakePixeIClk). However, when the color
maps are controlled by the Dorado, this input device can be used to read out the color
map entries addressed by their respective RAM address registers.

MaplnHi[O] is the 7-wire terminal input bit for the independent terminal interface on DispM;
its position corresponds to Status[O] on DispY (see below). MaplnHi[l] is a constant 1 if a
DispM board is installed; if DispM is not installed, an Input from the nonexistent register
yields a zero value. This enables firmware to detect the presence or absence of a DispM
board. MaplnLo[0:3] are a 4-bit color monitor type jumpered on the Dorado backpanel.
Note: the MaplnLc and MaplnHi input devices do not generate IOB parity, so they must-be read by
the Pd--lnputNoPE function to disable parity checking.

### 10.8. DispM Terminal Interface

The independent terminal interface on the DispM board functions much the same as a
single-channel DispY board, but is specialized to driving a binary monitor via a 7-wire
interface. The data path is one bit/pixel; the resolution is full; there is no MiniMixer; and
the horizontal waveforms are fixed by a PRom (which must be changed when a different
type of 7-wire terminal is installed).

Aside from these limitations, the DispM terminal interface operates almost identically to the
A channel of DispY. In particular, the io addresses are grouped parallel to the ones on
DispY, and the data formats are identical; so a microprogram can initialize TIOA to the
correct group and subsequently use the function that changes only TIOA[5:7] to select
registers within that group. This enables practically all the microcode for driying a 7-wire
terminal to be shared between DispY and DispM.

In Figure 15, the DispY io operations that are also defined for DispM are marked with an
asterisk. Note that outputs to unused NLC8 addresses are ignored.

Due to hardware diffl;lrences between DispY and DispM, the ALeftMargin and CursorX
values must be computed slightly differently. For DispM driving the 7 -wire interface,
ALeftMargin = - (B + L - 130) and CursorX = - (8 + X + 190).
Note: DispM does not have a muffler system.    In particular, the MonitorType muffler value is always
read from DispY. By convention, this refers to the type of 7-wire terminal attached to the Dorado,
whether that terminal is connected to DispY or to DispM. Also by convention, the 7-wire terminal is
always connected to DispM if DispM is installed.

<!-- PDF page 129 / printed page 122 -->

### 10.9. DDC Initialization Requirements

The two low-order bits in the Statics register are called DWTShutUp and DHTShutUp. They
are forced true by IOReset and prevent the respective task wakeups from happening. They
are individually set or cleared by the Statics output command. In addition, IOReset sets the
signal DoradoHasHRam; this will prevent horizontal sync from being sent to monitors until
the HRam has been loaded and released by firmware. Blanking is sent to monitors as long
as DHTShutUp remains true. It is anticipated that DHTShutUp will be left true until all DOC
initialization has been completed by the emulator (or by the DHT running in response to a
Notify).

Some other initialization requirements are as follows: aLeftMargin should be loaded with a
large negative value in case one of the channels remains unused forever; the Cursor in
NLCB should be zeroed in case the cursor is completely off-screen forever; HRam must be
loaded with monitor-specific waveforms; the pixel clock rate must be set; mixer modes must
be set; the MiniMixer must be loaded. In addition, the DHT must explicitly set the aAddress
registers to zero on behalf of the DWT, which cannot initialize itself completely for each
subtask.

### 10.10. Speed and Resolution Limits

High performance color monitors are typified by the following performance limits:

22 p.s     horizontal scan time
5 p.s      horizontal blanking time
800 p.s     vertical blanking time

Parameters for a particular monitor can be modified slightly through hardware adjustments,
but cannot be controlled by the DOC, which must provide control signals with timing
appropriate for the monitor. Consequently, a monitor must be chosen that conforms to the
speed limitations of the DOC.

. One important speed limitation is how fast bits can be moved from storage through the
DOC. This limit is derived using the following parameters:
F   Frame update rate. High speed phosphors require a minimum update rate of
30 frames/sec with interlaced operation for reasonable visual effects; this is
marginal and faster update is desirable.

S   Scan lines/frame.
VR Vertical retrace time; with interlaced operation, there will be two vertical
retraces/frame.

HB Horizontal blanking time.

HS Horizontal scan time. The FIFO must not go empty during the horizontal scan
or garbage will be displayed.

T   Time/munch or the rate at which storage can deliver data for IOFetches; this
is 1 munch/8 cycles = 1 munch/O.4 p.s.

<!-- PDF page 130 / printed page 123 -->

M   Munches/scan line that the fast io system can deliver.

The time required to fill the FIFO for both channels is a little longer than 30*8 + 20 cycles
{= 276 cycles} or about 13.8 J.Ls at a Dorado clock period of 25 ns; this follows from the
fact that there are 15 munches/channel or a total of 30 munches of FIFO storage, and the
fast io system can deliver one munch per 8 cycles with the first munch arriving 20 cycles
after the first 10Fetch←. 13.8 J.Ls is much smaller than the vertical blanking time and longer
than the horizontal blanking time, so the FIFO will start out full at the beginning of a field
and will be actively refilling itself during HS + HB pf each scan line. If the memory system
keeps up with the demands of the video channels, then the FIFO .will tend to refill itself
after momentary transients in which it empties out a little.

Consequently, we know that HS + HB = 1/{S*F) - 2*VR, and that M = {HS + HB)/T less
corrections for refresh references, storage references by other tasks, hold, and delays for
tasks of higher priority than OWT. At F = 30 frames/sec, VR = 800 J.Ls, and S = 1000
scan lines, we get HS + HB = 31.7 ILs and M = 31.7/0.4 = 79 munches less corrections.
There will be an average of two refresh references/scan line, so we get an upper bound of
77 munches = 19,712 bits/scan line from storage.

However, the DWT will not get all storage bandwidth. The DWT wakeup spacing is
controlled by a SIP; the smallest reasonable spacing would result in one IOFetch every 8
cycles-closer spacing would result in hold while a preceding IOFetch completed, so more
processor cycles would be consumed without improving data rate. At this tightest spacing,
DWT runs for 2 cycles out of every 8. Conceivably, worst case memory activity discussed
in the "Fast 10" chapter could occur during these 6 cycles (a clean miss 3 cycles before
the IOFetch, followed by a dirty miss 2 cycles before the 10Fetch, each by a different task).
However, the large amount of storage in the FIFO allows us to rely upon statistics to
average out memory competition, so it is probably reasonable to allow OWT at least 80% of
storage bandwidth or about 16,000 bits/scan line in the above example, which would
accommodate 1000 line x 1000 pixels/line x 16 bits/pixel. For HB = 5 J.Ls this is equivalent
to a pixel clock period of 26.7 ns.

This is only one speed limitation. Since the 32-bit wide FIFO is accessed once/cycle
alternately by the A and B channels (i.e., 16 bits/cycle/channel), and since exactly three
doublewords are fetched before the horizontal scan begins for each channel, the maximum
bits/scan line for each channel is about (3*32 bits) + [(26.7 ns/pixel)*(16 bits/50 ns)*(1000
pixels/line)] = 8640 bits/scan line. This means that unless both channels are running at
the same data rate, the data rate will be significantly below the upper bound determined
above. For example, in 248it mode, if the A channel runs at full resolution and gets 8640
bits/scan line, the B channel will run at half resolution and get only 4320 bits/scan line, so
the maximum data rate would be about 1000 lines x 538 pixels/line x 24 bits/pixel.

<!-- PDF page 131 / printed page 124 -->

## 11. Ethernet Controller

An Ethernet is the principal means of communication between a Dorado and the outside
world. An Ethernet is a broadcast multi-access packet switched network which can
connect up to 256 stations separated by as much as 1 kilometer with a 3 mHz channel.
The 'Ether' is a passive coaxial cable to which each station is connected through a                ,
transceiver that is high-impedance when receiving, low impedance when driving.

Readers unfamiliar with the general concepts behind the Ethernet should refer to
"Ethernet: Distributed Packet Switching for Local Computer Networks," by R. M. Metcalfe
and D. R. Boggs, CACM, 19(7):395-404, July 1976; or to Design and Performance of Local
Computer Networks, by John Shoch, published by University Microfilms, August 1979.

Read this chapter with Figure 16 in view_

### 11.1. Ethernet Packets

Ethernet data are encoded in packets. Packets are preceded by a low signal (i.e., silence)
on the Ether; they begin with a one-bit prefixed by the transmitter, called the start bit. Bits
in the packet are phase encoded, where the bit cell time is nominally 340 ns; phase
encoded signals have one data transition per bit cell and its direction (Iow-to-high. = 1) is
the value of the bit. Midway between these there may be a setup transition, so that the
next data transition can be in the correct direction.

Packets end when no transitions are detected for more than 1.5 bit times and the Ether is
low.     Collisions are transmissions that overlap in time and cause malformed and
undecodable bits. Transmitters jam the Ether with a continuous high for several bit times
after participating in a collision. Collisions are of four types: too many transitions, in which
two transitions occur within .25 bit times; too tew transitions, in which a transition occurs
between 1.25 and 1.5 bit times after the last one; end-ot-packet (EOP), in which no
transitions occur for more than 1.5 bit times and the Ether is low; and jam, which is the
same as EOP except that the Ether is high.

In a well-formed packet that does not experience a collision, the start bit is immediately
followed by an 8-bit destination host number, then an 8-bit source host number. This is
followed by an indefinite number of 16-bit data words, a 16-bit checksum, and finally
silence.

Even when transmitted without a source-detected collision, a packet may fail to reach its
destination; packets are delivered only with high probability. Stations requiring a lower
residual error rate must follow mutually agreed upon communication protocols.

When the sender of a packet detects a collision, some method is needed to arbitrate
(without communication) its use of the Ether with other stations contending for it. The
algorithm used on the Ethernet, called the 'binary exponential backoff collision algorithm,'
is discussed in the above references. It involves waiting a random interval and then
reattempting transmission. The (ideal) distribution of the random intervals depends upon
many factors.

<!-- PDF page 132 / printed page 125 -->

Remarks

From the method of collision detection, it follows that in a noise free Ether with ideal transmitters and
receivers, a bit cell time between 0.75"T and 1.25*T, where T is the nominal bit cell time (340 ns), can be
decoded correctly.

Phase encoding has the undesirable property that only 50% of the transmission medium's theoretical bandwidth
is utilized. A number of reasonably simple encodings are known that more nearly approach the theoretical
limit, though phase encoding is simple to implement. If at some time we were willing to abandon compatibility
with the existing Ethernet, we should reconsider the use of phase encoding.

A promising alternative to phase encoding is bit-stuffing, which averages 67%, 86%, or 93% of theoretical
bandwidth for Oth, 1st, and 2nd order codes. This encoding outputs data bits in a cell time equal to 112 of
the phase-encoded cell time; when 1 (oth order), 2 (1st order), or 3 (2nd order) data bits have been output
without a transition, then a non-data transition is inserted into the bit stream. The 1st order encoding (86%)
could be implemented with a few changes to the current controller_

### 11.2. Controller Overview

The Ethernet controller is a slow 10 device packaged with the disk controller on the DskEth
logic board. These two devices require more edge pins than are available in an MSA-IO
slot, so the board must be mounted in a Fast 10 slot (see Figure 2).
It would be possible to package two Ethernet controllers on one logic board using different task and
TIOA assignments for each. This might be appropriate if Dorados are ever used as Ethernet
gateways.

A cable connects the controller to a transceiver outside the Dorado enclosure; this
transceiver is almost identical to the ones used for Altos and other computers, the
difference being that it uses + 12 volts rather than + 15. Dorado transceivers are painted
bright red and have large block lettering saying "Dorado only". Plugging in the wrong type
of . transceiver will not damage anything; it just won't work. The cable between the
controller and the transceiver contains twisted pair signals for receiver data, transmitter
data, collision, + 5 v, and + 12 v.

The controller has independent transmitter and receiver sections. Becau~e these two
sections are completely independent, the Dorado can receive its own transmissions. This
is an important aid in hardware and software debugging and simplifies the device driver,
which need not check for sending to itself. Furthermore, the receiver can receive
consecutive packets separated by the minimum inter-packet spacing (510 ns). This means
that the Dorado can receive, without loss, streams of packets directed to it by mulitple
hosts and packets that immediately follow broadcasts. This capability is important for
servers and other high-performance applications.

The controller uses two tasks, one for the transmitter (EOT for ~thernet Output Iask) and
one for the receiver (EIT for gthernet input Iask). The receiver task is higher priority. To
permit two instruction/wakeup loops, a wakeup request is removed whenever the Next bus
says the task is about to run. This simple strategy can be fooled into removing a request
when NextLies occurs, but this is harmless since the required service rate is low. To avoid
a spurious wakeup, a wakeup is not requested again until after the task has blocked. A
debugging control bit can be set which prevents wakeups even when all other conditions
are satisfied.

<!-- PDF page 133 / printed page 126 -->

The transmitter and receiver each have 16-word x 20-bit Fifos. The bits are 16 data + 2
parity + 2 spare (the receiver uses one of the spare bits). Each Fifo has read and write
pointers, multiplexed into the address inputs of the storage chips, to select the next
location to be read or written; these pointers are zeroed by 10Reset. A Fifo is empty when
the pointers are equal and full when (WritePtr+ 1) mod 16 equals ReadPtr. There are bus
registers between the Fifos and 108. Service requests from the Ether side of a Fifo are
given priority. The Fifos are synchronous to t 1•

The basic clock for transmitting and receiving .data from the Ether, called EtherClk,
originates from a 23.5 MHz crystal oscillator (i.e., the period is 42.5 ~s or 1/8 of the 340 ns
bit cell time). The memory system's Pendulum clock (period 16 ms) is also used to time
retransmissions after a collision, as discussed later.

The receiver runs continually; its phase decoder (PD) samples the Ether every EtherClk; a
finite state machine (FSM) driven by the samples detects the presence or absence of
packets on the Ether, zer%ne transitions, and collisions. Another FSM accumulates the
status of the packet and controls a shift register that assembles 16-bit words from the
incoming data. Words in the shift register are written into the receiver'S Fifo together with
odd parity on each byte; the status is written into the Fifo after the last word of each
packet and marked to distingUish it from data words. This allows the receiver to handle
back-to-back packets; firmware decides what to do with each packet as it is read from the
Fifo. EtherClk is used for receiver stages through the shift register; data in the shift
register is synchronized to the Dorado system clock as it is written into the Fifo.

When the transmitter is turned on, it attempts to send one packet and then must be
restarted by firmware. The EOT fills the Fifo; the transmitter FSM loads the shift register
from the Fifo and supplies a serial bit stream to the phase encoder (PE). Transmitter status
is read directly from the controller status registers (unlike receiver status, which travels
through the data path). Data is synchronized to EtherClk between the output of the shift
register and the input of the PE. A collision may be detected by either the transceiver or
the PD. The occurrence of a collision is captured, synchronized, and used to abort the
outgoing packet after jamming the Ether briefly.

The controller has a number of features to help debugging. All of the interesting internal
state is available via the 108 and the muffler system. The transceiver can be disconnected
and PE output internally connected to PO input under firmware control. Task wakeups can
be disabled permitting the controller to be driven entirely from emulator-level software. The
internal clock can be single-stepped.        These features permit the construction of a
simulation program which compares its predictions with what the controller is actually
doing.

<!-- PDF page 134 / printed page 127 -->

### 11.3. Receiver

Most of the receiver runs continuously, tracking traffic on the Ether. The PO reports what
it sees to the receiver FSM, which assembles packets in the shift register and buffers them
in the Fifo. As words emerge from the Fifo into the bus register, they are either discarded
or generate a wakeup request under control of the wakeup logic. Following the last data
word of each packet as it travels through the Fifo are the CRC word and a status word.
10Atten branches when a status word is present in the receiver bus register. Data and
status are synchronized to the Dorado clock between the output of the shift register and
the input of the Fifo.
The peculiar placement of status bits in Figure 16 eases emulation of the Alto Ethernet controller.

The PO is a FSM which takes in raw phase encoded serial data and produces phase
decoder events and carrier. Phase decoder events are 'saw a zero bit', 'saw a one bit',
and 'saw a malformed bit'. Carrier indicates that the PO is seeing. transitions on the Ether
(i.e. the Ether is if' use). Since the PO is completely digital, it can be sing~e-stepped for
debugging. Receiver collision detection, a by-p~oduct of this decoding technique, works as
well as transceiver collision detection.

The receiver control is another FSM that takes in PO output and produces control and
status signals. RxSRCtrl controls the shift register and the bit counter. The bit counter
decrements when a data bit is shifted into the shift register and resets to 1 when the
status is parallel loaded into the shift register. RxSRFull' is low when the next shift will
make the register full. RxEOP travels in parallel with each Fifo word and is true if the word
is an ending status word. This bit is called EthData.18 when it is in the bus register where
it can be tested ~ith 10AJten.                      .

Writing data or status from the shift register into the Fifo has priority over loading the bus
register from the Fifo. Byte parity is computed at the shift register output and travels with
the data through the Fifo and the bus register, down IOB and into the processor where it is
checked.

The optimum point at which to synchronize received data with the Dorado clock system
would be at the input to the PO, where there is only one signal to synchronize, except that
this would make proper operation of the PO depend upon the Dorado clock period. The
next best sync paint is the PO output where the number of signals has only grown to three.
The problem here is that the PO can produce events faster than they can be synchronized
to the Dorado clock without buffering. Consequently, synchronization takes place after the
shift register where the number of signals exceeds 20. This is not as unfortunate as it
seems because status and data use the same paths and can share a single synchronizer,
RxSRDump, which produces RxFifoWE' each time RxFSM pulses RxSync'. This leaves only
RxCollision and PDCarrier which must be synchronized for the transmitter. RxCollision
shares a synchronizer with XcCollision, and PDCarrier's is a simple level synchronizer.

A receiver data-late occurs when the receiver FSM requests a Fifo write and the Fifo is full.
In this case the write does not happen and the data is lost. RxDataLate is cleared after an
end-at-packet status word is successfully written into the Fifo. This status has the data late
error bit set so that the EIT is notified that the preceding packet was bad.

<!-- PDF page 135 / printed page 128 -->

EIT wakeup requests occur when the bus register contains an interesting word (provided
that the EIT is currently blocked, as discussed earlier). Words are interesting if they
emerge from the Fifo into the bus register while RxOn and RxBOP are true and NoWakeups
is false. RxBOP is set after the status word for a packet is discarded, so that the next word
out of the Fifo (presumably the first word of the next packet) can generate a wakeup. It is
reset by the EIT to discard the remaining words of a rejected packet (usually because the
address didn't match). The receiver may be reset at any time by clearing RxOn. No more
wakeups are generated and every word is discarded as it emerges from the Fifo. When
RxOn is next set, the receiver will continue to discard words until it has discarded a status
word. It will then set RxBOP, and the next word (first word of the first packet after turning
on the receiver) will cause a wakeup.

### 11.4. Transmitter

When the transmitter is turned on, it attempts to send one packet and then must be
restarted by firmware. At the request of the wakeup logic, the EOT fills the Fifo using
Output←B to the bus register. The transmitter FSM loads the shift register from the Fifo
and supplies a serial bit stream to the PE. Transmitter status is read directly from the
controller status registers (unlike receiver status, which travels through the data path).
Data is synchronized to the Ether clock between shift register output and PE input.

EOT wakeups occur when the bus register is empty, TxOn is true, and TxEOP, TxCntDwn,
and NoWakeups are false (provided that EOT is blocked, as discussed earlier). After
delivering the last word of a packet, EOT wakeups are disabled by setting TxEOP. While
counting down a collision retransmission interval, firmware can disable wakeups until the
next tick of Pendulum by setting TxCntDwn. The transmitter may be reset at any time by
clearing TxOn, which stops wakeup requests and shuts down the PE within 2 bit times.

The binary exponential backoff collision algorithm must be implemented in firmware. The
controller merely' provides a way to generate a wakeup on the next rising edge of
Pendulum, making the grain size of countdown intervals 16 f.Ls for the Dorado (compared to
38 f.LS for Altos and Novas). Note that setting TxCntDwn prevents a wakeu'p; for one to
actually occur when Pendulum clears it, the bus register must be empty and TxEOP must
be false. Pendulum is considered to be a foreign signal so it is synchronized before being
applied to the reset input of TxCntDwn.

Loading the shift register from the Fifo has priority over writing into the Fifo from the bus
register. Byte parity is computed in the processor and travels with the data down 108 into
the bus register, and through the Fifo to the shift register where it is checked.

The transmitter control is a FSM which takes in start, end, and abort signals and produces
control signals. TxSRCtrl controls the shift register and bit counter. The bit counter
decrements when a data bit is shifted into the shift register and resets to -1 when the next
word is parallel loaded into the shift register. TxSREmpty' is low when the next shift will
make the register empty. TxData wire-or's the start bit at the beginning of each packet.
TxGone clears TxEOP to cause a wakeup at the end of each packet. The transmitter starts
when the Fifo is full or, if the packet is less than 15 words long, when TxEOP is true. The
transmitter ends normally when the Fifo is empty and TxEOP is true. The transmitter aborts
when a collision, Fifo parity error or data late occurs. TxAbort can be tested with 10Atten.

<!-- PDF page 136 / printed page 129 -->

A transmitter data late occurs when the TxFSM requests a Fifo read and the Fifo is empty
but TxEOP is false. The PE sends one random bit and then stops. The resulting. packet
has an illegal length and probably a bad CRC.

The PE inverts and latches TxData at the start of each bit cell and inverts the latched value
112 bit time later. TxGo, synchronized to the beginning of a bit cell, enables the PE. The
PE assumes that a data bit is available long before it is needed and acknowledges each bit
after latching it by generating TxGotBit.

A collision may be detected by either the transceiver or PD. The oc;:currence of a collision
is captured, synchronized, and used to abort the outgoing packet. The output of the first
stage of the TxCollision synchronizer is wire-or'ed with PD output to jam the Ether after a
collision. The jam lasts for one or two bit times, being the delay through the TxCollision
synchronizer, TxFSM, and TxGo synchronizer.                             .

### 11.5. Clocks

The controller needs a clock with a nominal frequency of eight times the Ether bit rate.
The SingleStep control bit selects either the 23.53 mHz crystal oscillator or Single Dorado
clocks injected under program control. The clocks for the Ether-synchronous parts of the
controller are constructed from this basic clock.

The slowest Dorado clock period at which the transmitter works is 42.5 ns. Disabling the
Dorado system clocks while TxOn is true causes a transmitter data late. If TxGo is true,
the packet is chopped off, causing an incomplete transmission and probably a runt bit.
When the clock is reenabled, the PE sends a few fragmentary bits and then the data late
aborts the packet.

The slowest Dorado clock period at which the receiver works is 85 ns. Disabling the
Dorado system clocks causes a receiver data late. The next packet that arrives after the
clock is reenabled reports the data late.

### 11.6. Task Wakeups

The controller is designed for two completely independent tasks, with the receiver higher
priority. Two IOAs select data and status/control registers. IOAtten may be tested to
decide whether a wakeup request is just for another word or something special (ending
status for the receiver, or PE aborted for the transmitter).

Task wakeups must, on the average, be serviced within 5.44 p.s. The transmitter and
receiver each have 17 words of buffering (bus register + 15 Fifo + shift register) so the
variance can be quite large-accumulated delay of up to about 90 p.s is tolerable, while
longer delay will cause a data late error.

<!-- PDF page 137 / printed page 130 -->

### 11.7. Muffler Input

All muffled signals on the DskEth board are accessible to Dorado firmware. The method by
which a particular signal is selected and read out is discussed in the "Muffler Input"
section of the "Disk Controller" chapter. Signal addresses 1208 to 1778 for the Ethernet
controller are enumerated below. Unless it is obvious, signals which are specific to the
receiver or transmitter have Rx or Tx respectively somewhere in their names.

> **Table 26: Ethernet Muffler Signals**

![Table 26: Ethernet Muffler Signals](tables/table-26-ethernet-muffler-signals.png)

<!-- PDF page 138 / printed page 131 -->
### 11.8. IOB Registers

TIOA equals 158 selects the IOB registers (called EthD). The transmitter bus register is
loaded by Output←B and the receiver bus register is read with Pd←Input. At end-of-packet,
after the last data word, the receiver delivers first the CRC word and then a status word
containing the following bits:

RxColiision           Receiver-detected collision occurred (can happen only if ReportCollisions has
been set in the control word).
RxOataLate            Receiver data-late occurred-one or more words of the last packet were lost.
RxCRCError            CRC was incorrect in last packet.
RxlncTrans            Last packet did not end on a word boundary.

### 11.9. Control Register

TIOA equals 168 selects either the (write-only) control register (EthC), discussed here, or
the (read-only) status register (also called EthC), discussed in the next section. The control
register has three fields: transmitter, receiver, and test. Bits in a field are decoded only if
the command-enable bit for the field is true. Control bits with a single quote as their last
character are true when zero.

TxCmdEnbl'           enables decoding of transmitter commands.
TxOn                  enables the transmitter. The transmitter may .be reset at any time by clearing
this bit.  Cleared by IOReset.
TxEOP                disables transmitter wakeups. EOT sets this bit after outputing the last word of
a packet. It is cleared by the controller when the PE shuts down after an abort
or normal end.       Cleared by TxOn =0.
TxCntDwn             disables transmitter wakeups. Set by EOT to time a retransmission interval after
a collision; cleared by the controller when the next rising edge of Pendulum
occurs (period = 16 II-s). N.B. the binary exponential backoff is done by
firmware.    Cleared by TxOn = O.
RxCmdEnbl'           enables decoding of receiver commands.
RxOn                 enables the receiver, which may be turned off at any time by clearing this bit.
Cleared by IOReset.
RxBOP'               disables receiver wakeups. Cleared by EIT to discard the currently arriving
packet; set by the controller when the first word of the next packet is available.
Cleared by RxOn =O.
TestCmdEnbl'          enables decoding of test commands
LoopBack              disconnects the transceiver, loops PE output to PO input, and enables TestColl'.
Cleared by IOReset.
SingleStep            disables the 23.53 mHz oscillator. Changing this bit can produce a runt clock.
Reset the transmitter first and expect an occasional bad receiver status.
Cleared by IOReset.
NoWakeups             disables all controller ,wakeups.   Cleared by IOReset.
TestClock             injects a single Dorado clock pulse (t3 of the Output instruction) into the
EtherClk logic.   SingleStep must already be set.
TestColl'             injects a single Dorado clock pulse (t3 of the Output instruction) into the
collision synchronizer. LoopBack must already be set.
TestData              wire ORs with PO input. LoopBack must already be set and TxOn must already

<!-- PDF page 139 / printed page 132 -->

be false. Do not issue TestClock in an instruction that changes TestData.
Cleared by IOReset.
ReportCollisions     allows the PD to report malformed bits as collisions.   Cleared by IOReset.

### 11.10. Status Register

TIOA of 168 also selects the (read-only) status register. The bits in this register are the
most interesting to the microcode. Less interesting state is available from the mufflers.

Host Addr            the host address set by pullups on the backplane.
RxOn                 the receiver is enabled.
TxOn                 the transmitter is enabled.
Loop8ack            the interface is looped back.
TxColl               the current output packet was aborted by a collision.
NoWakeups            all wakeups are disabled.
TxDataLate           the current output packet was aborted by a data late.
SingleStep          the 23.53 mHz oscillator is disabled.
TxFifoPE             the current output packet was aborted by a parity error..

<!-- PDF page 140 / printed page 133 -->

## 12. Other IO and Event Counters

In addition to the disk, ethernet, and display controllers discussed in earlier chapters,
Dorado contains a general input/output interface and a junk task wakeup located on the
IFU board; the two registers used in this interface may alternatively be used as event
counters in performance monitoring, and that use is also discussed here.

Since the IFU board is not interfaced to the IOB, it cannot use the slow io system to control
these features, so functions are used instead.

### 12.1. Junk Task Wakeup

The IFU board contains a circuit which wakes up the junk task (task 1) every 32 fLs. The
wakeup is dismissed by the AckJunkTWf-B function; this function interprets B[15] as follows:
a 1 enables wakeups; a 0 d:sables them; B[0:14] are ignored. The junk task can dismiss the
wakeup by doing IFUTestf-B with any value on B (but B[15] must be 0 to reenable the
wakeup at the next 32 fLs tick).

Junk task microcode will, among other things, maintain a Real Time clock.

### 12.2. General IO

A 16-bit register called Genln (synonym EventCntA) is used for general input; it can be read
with the Bf-Genln (synonym Bf-EventCntA) function but cannot be written by firmware.
When used for general input, Genln is written with information that is TTl-to-ECl converted
from the backpanel.

A 16-bit register called GenOut (synonym EventCntB) is used for general output; it can be
either read with the Bf-GenOut (synonym Bf-EventCntB) function or written with the
GenOutf-B (synonym EventCntBf-B) function.     GenOut is connected to the backpanel
through ECl-to-TTl converters.

The plan is that devices such as Diablo printers can be connected to the Genln and/or
GenOut signals "vi"a"" backpanel connectors.

The choice of using one of these registers for general io or for event counting is determined
by the InsSetOrEventf-B function discussed below.

### 12.3. Event Counters

The Genln and GenOut registers can alternatively be used as event counters. They cannot,
of course, be used simultaneously for general io. The registers are setup for either io or
event counting by" the InsSetOrEventf-B function, where B[0:15] are interpreted as follows:

<!-- PDF page 141 / printed page 134 -->

If 8[0] is 1, then InsSet[0:1] are loaded as discussed in the "Instruction Fetch Unit" chapter.
If 8[0] is 0, then its the general io/event counters as follows:

8[4] enables counting of EventCntA
8[5] enables counting of EventCnt8
8[8:10] select the event type to be counted by EventCntA as follows:
o      True (i.e., every cycle)
1      Hold
2      Processor memory reference (not held)
3      Good IFUJump (i.e., not held and not an exception)
4      Miss
5-7    Backpanel events A, C, and E, respectively
8[12:14] select the event type to be counted by EventCntB as follows:
o      True
1      Hold
2      Successful IFU memory reference
3      IFUJump that wasn't ready
4      Miss
5-7    Backpanel events 8, C, and 0, respectively
8[15] causes the event to be counted for all cycles if 1 or only for emulator or fault task cycles if O.

To use the event counters, you first stop them counting and read their current values; then
you tell them what to count and start them counting and your system running. Note that
they never get reset, but just keep countihg from wherever they are-it's. up to the user to
worry about counter turnover.

The expected mode of operation is that the junk task will detect counter overflow and
update double or triple-precision vectors in RM that count events; even if the counter is
counting once per 60 ns cycle, counter wraparound only occurs every 3.93 ms, so a
double-precision vector could count for at least 255 seconds and triple-precision for 228
days. Sample microcode for maintaining a double-precision counter is given in the
example below:
The double-precision vector consisting of two RM locations, CountHi and CountL.o
'"is initialized such that CountHi eq 0 and CountLo contains minus the value in
the event counter, and another RM location called CountFlag is initialized to O.
'"The microcode below increments CountHi whenever the event counter cycles.
• At any instant, the high part of the total count is in CountHi and the low part
'"is' CountLo + event counter; CountHi has to be incremented by 1 if the counter
'"just overflowed.

(CountLo) - (EventCntB') - 1;                  *CountLo   +   event counter
Pd"CountFlag, Branch[. + 2,alu> =0];
CountFlag4-T-T-1, Branch[. +3];                '"Set CountFlag to -1 in 2nd half of the counter cycle.
CountFlag" T-T, Branch[. + 2,alu> = 0];        '"Set CountFlag to 0 in 1st half of the counter cycle,
CountHi4-(CountHi) + 1;                        '"and increment CountHi, if we were in the 2nd half
*of the counter cycle last time.

The microcode for reading the counter when it is updated like this is as folows:
'"Return to caller high part of event count in T, low part in Q.

TaskingOff, . Pd"CountFlag;
T4-(CountLo) - (EventCntB') - 1, Branch[. + 3,alu> = OJ; CountLo + event counter = low part of result
TaskingOn, 8ranch[. + 3,alu(0];                      Low part ovf iff CountFlag<O and low sum > = 0
T4-(CountHi) + 1, Q"T, Return;                       High part of result = CountHi+1
TaskingOn;                                           High part of result = CountHi

<!-- PDF page 142 / printed page 135 -->

TfoCountHi, QfoT, Return;

<!-- PDF page 143 / printed page 136 -->

## 13. Error Handling

In addition to single-error correction and double-error detection on data from storage,
Dorado also generates, stores, and checks parity for a number of internal memories and
data paths. The general concepts on handling various kinds of detected failures are as
follows:

(1) Failures of the processor or control sections should generally halt Dorado because
these sections must be operational before any kind of error analysis or recovery firmware
can be effective.

(2) Failures arising from memory and io sections should generally result in a fault task
wakeup and be handled by firmware. In some situations, such as map parity errors, it is
especially important to report errors this way rather than immediately halting because
firmware/software may be able to bypass the hardware affected by the failure and continue
normal operation until a cor.venient time for repair occurs. In other situations, the firmware
may be able to diagnose the failure and leave more information for the hardware
maintainers before halting.

(3) IFU section failures and memory section failures detected by the IFU should generally
be buffered through to the affected IFUJump, then reported via a trap; in this way, if it is
possible to recover from the failure, then it will be possible to restart the IFU at the next
opcode and continue.

(4) Memories and data paths involving many parts should generally be parity checked. It
is not obvious that this is always a good idea because extra parts in the parity logic will be
an additional source of failures, but instantly detecting and localizing a failure seems
preferable to continuing computation to an erroneous and undetected result.

(5) When Dorado halts due to a failure, information available on mufflers and in the 16-bits
of passively available error status (EST AT) should localize the cause of the error as
precisely as possible.

Since the MECL-10K logic family has a fast 9-input parity ladder component, the hardware
uses parity on 8-bit bytes in most places; there is usually insufficient time to compute parity
over larger units. IM and MIR, two exceptions, compute parity over the 17 -bits of data in
each half of an instruction; and the cache address section computes parity over the 15
address bits and WP bit.

Odd parity is used throughout the machine, except that the cache address section and
IFUM use even parity. Odd parity means that the number of ones in the. data unit,
including the parity bit, should be odd, if the data is ok.

The control processor (Midas or the baseboard microcomputer) independently enables
various kinds of error-halt conditions by executing a manifold op~ration discussed in the
"Dorado Debugging Interface" document. It also has to initialize RM, T, the cache address
and data sections, the Map, and IFUM to have valid parity before trying to run programs.
Reasons for this will be apparent from the discussion below.

When Dorado halts, error indicators in ESTAT indicate the primary reason for the halt, and

<!-- PDF page 144 / printed page 137 -->

muffler signals available to the control processor further define the halt condition; EST AT
also shows the halt-enables. Midas will automatically prettyprint a message describing the
reasons for an error halt. The exact conditions that cause error halts are detailed in the
sections below; the table here shows the ESTAT and muffler information which is relevant.

> **Table 27: Error-Related Signals**

![Table 27: Error-Related Signals](tables/table-27-error-related-signals.png)

### 13.1. Processor Errors

The processor has parity ladders on each byte of the following:

input to RM/STK     generate parity for write of RM/STK
input to T          generate parity for write of T
8                   generate parity for D8uf←B, Map8uf←B, Output←B, IM←B
108                 check parity for Pd←Input and Output←B
Md                  check parity for ← Md
R                   check parity for ←RM/STK (unless bypassed from Pd or
Md or replaced by ←Id)
T                   check parity for ←T (unless bypassed from Pd or Md or
replaced by ←Id)

Input ladders to RM/STK and T generate parity stored with data in the RAM; these ladders
are not used for detecting errors.

The processor computes parity on its internal 8 bus (alub). The generated parity may be
transmitted onto 108 when an Output←B function is executed; Store← references write B
data and parity in the cache; parity for IM writes and map writes is computed from 8 parity.
None of the other B destinations either check or store 8 parity. External 8 sources do not

<!-- PDF page 145 / printed page 138 -->

generate parity.

Parity on the RfT ladders is checked only when the R/T data path is sourced from the
RAM, not when bypassing from Md or Pd is occurring, and not when R/T is sourced from
Id. A detected failure causes the RAMPE error halt, which indicates that some byte of RM,
STK, or T had bad parity. The muffler signals that further describe this error are in the
PERR word: StkSelSaved is true if the source for R was STK, false if the source for R was
RM; each processor board has RmPerr and TmPe" signals; RmPerr is true if the RM/STK
byte on that board had bad parity, TmPerr if th~.T byte had bad parity. Note that if an
instruction beginning at to suffered an error, Dorado halts immediat.ely after t4; the muffler
signals apply to the instruction starting at to' The Task2Bk muffler signals show the task
that executed the instruction at              to.
Md parity is checked whenever ←Md is done; a failure causes the MdPE error halt when
enabled. The ←MOSaved muffler signal in PERR is true when a deferred ←Md caused the
error (T←Md, RM/STK←Md), false when an immediate ←Md (A←Md, B←Md, or ShMdxx)
caused the error. On a deferred ←Md error, Dorado halts after t6 and Task3Bk shows the
task that executed the instruction starting at to; on an immediate ←Md, Dorado halts after
t4 , and Task2Bk shows the task. The MOPe" muffler signals on each processor board
show which byte of Md was in error.

10 devices (optionally) compute and send odd parity with each byte of data; the processor
checks parity when the Pd←Input function is executed, but not when the Pd←lnputNoPE
function is executed. When enabled, an IOBPE error halts the processor at t4 of the
instruction that suffered the error; Task2Bk shows the task that executed the instruction.
The processor also checks IOB parity on Output← B, and an error halts at t4 as for
Pd←Input. The IOBoutSaved muffler signal distinguishes Pd←Input from Output←B errors;
an lOPe" muffler signal on each processor board shows which byte of IOB was in error; all
of these are in the PERR muffler word.
The processor generally does not pass parity at one stage through multiplexing to the next stage, so
any failure in the multiplexing between one stage and the next will go undetected (exception: B
parity passed through to 108).

For example, the processor could write Md parity sent by the cache into the T RAM, when T is
being written from Md. Instead, however, it checks Md parity independently, but then recomputes
the parity written into T with the input ladder. Hence, a parity failure detected on a byte of T can
only indicate a failure in either (1) the input parity ladder; (2) the output parity flipflop; (3) the output
parity ladder; (4) one of three 16x4 TRAM's; (5) one of two 4-bit latches clocked at t1 (Figure 3)
through which the output of the T RAM passes; (6) one of two 4-bit latches clocked by preSHC'.

Parity is handled similarly for writes of RM/STK.

Parity is similarly recomputed on B.

The processor does not generate or check parity on the A, Mar, or Pd data paths. Any
failures of the A, Mar, B, Pd, or shifter multiplexing or of the ALU go undetected; failures of
Q, Cnt, RBase, MemBase, ALUFM, or branch conditions go undetected.

Remark

Since 256x4 and 16x4 RAM's are used for RM, STK, and T, and since the processor is implemented with the
high byte (0:7) on ProcH and the low byte (8:15) on ProcL, byte parity requires an additional 4-bit storage
element on each board, of which only 1 bit is used. We could conceivably have used all 4 bits to implement a
full error'correcting code for each byte of Rand T data. However, there is insufficient time to correct the
data.    (Also. we use 256x1 RAM's instead of 256x4 RAM's for the RM and STK parity bits.)

<!-- PDF page 146 / printed page 139 -->

Alternatively. parity could be computed over each 4-bit nibble rather than each a-bit. byte; the MC170
component allows nibble parity to be computed iust as economically as byte parity. If this were done, then a
parity failure would be isolated to a particular nibble. With byte parity, a detected failure could be any of 9 +
components; with nibble parity. it would be isolated to one of 6 + components. Implementing nibble parity for
RM/STK and T would require about 4 more ic's per board than byte parity.

It is hard to say whether the additional precision of nibble parity would be worth the additional parts.

### 13.2. Control Section Errors

The control section stores parity with each 17-bit half of data in IM. When IM is written,
the two byte-parity bits on 8 are xor'ed with the 17th data bit to compute the odd parity bit
written into IM. It is possible to specify that bad (even) parity be written into IM, and this
artifice is used to create breakpoints; bad parity from both halves of IM is assumed to be a
deliberately set breakpoint by Midas.

IM RAM output is loaded into MIR and parity ladders on each 17. bit half give rise to error
indicators that, when enabled, will halt the processor after t2 of the instruction suffering an
error. For testing purposes, halt-on-error can be indepenc!ently enabled for each half of
MIA. 80th the unbuffered output of the MIR parity ladders and values buffered at t2 appear
in ESTAT. The buffered values show the cause of an error halt, and the unbu!fered signals
allow Midas to detect parity errors in MIR before executing instructions or when displaying
the contents of IM.

The special MIRDebug feature discussed in the "Dorado Debugging Interface" document
prevents MIR from being loaded at t2 when MIR parity is bad. In other words, when the
MIRDebug feature is being used, all of the t2 clocks in the machine will occur except the
ones to MIA. This feature prevents the instruction that suffered an error from being
overwritten at the expense of being unable to continue execution after the error.
MIRDebug can be enabled/disabled by the control processor.

### 13.3. IFU Errors

The IFU never halts the processor; any errors it detects are buffered until an IFUJump
transfers control to a trap location. The errors it detects, discussed in "IFU Section", are
parity failures on bytes from the cache, IFUM parity failures, and map parity failures on IFU
fetches.

### 13.4. Memory System Errors

There is no parity checking on Mar or on data in 8R, so any failure in the address
computation for a reference goes undetected. However, valid parity is stored with VA in
the cache, and any failure detected will cause the MemoryPE error to occur, halting the
system (if MemoryPE is enabled).

Parity is also stored in the Map (computed from 8 parity) and an error causes a fault task
wakeup in most situations (Exceptions: IFU references and Map← references do not
wakeup the fault task when a map parity error occurs).

<!-- PDF page 147 / printed page 140 -->

The cache data section stores valid parity with each byte of data. When a munch is loaded
from storage, the error corrector carries out single-error correction and double error
detection using the syndrome and recomputes parity on each 8-bit byte of data stored in
the cache. When a word from B is Store←'d in the cache, byte parity on B is stored with
the data.

A MemoryPE error occurs if, when storing a dirty victim back into storage, the memory
system detects bad parity on data from the cache.

The IFU and processor also check parity of data from the cache, as discussed previously.

### 13.5. Sources of Failure

In a full 4-module storage configuration, Dorado will have 1173 MOS storage, about 700
Schottky-TTL, 30eO MECL-10K, and 60 MECL-3 DIPs, and about 1500 SIPs (7 -resistor
packages).    This logic is connected with over 100,000 stitch-welded or multiwire
connections to sockets into which the parts plug; logic boards connect to sidepanels
through about 2500 edge pins. Sockets are used for all the RAM DIPs in the machine;
other parts are soldered in. Given all these potential sources of failure, reliable operation
has been a surprising achievement.

Initial debugging of new machines has been slow and difficult, requiring expertise not easily
available in a production environment. In addition to mechanical assembly, board stuffing,
and testing for shorts and opens both before and after stuffing, each machine has
averaged about one man month of expert technician time to repair other malfunctions
before it could be released to users.

Once released, the Dorados have been pretty reliable. During a 100-day period (6 October
1980 to 14 January 1981) the CSL technicians kept records of service calls made for
approximately 15 Dorados in service at that time. The following summarizes the 43 service
calls that were made.

37 days    mean time between service calls per machine.
45 days mean time between failures (some service calls were for microcode or
software problems).
2.5 hours per machine per month average service time.

13% of failures and 5% of time reseating logic boards in the chasis (connectors not
making contact).
11 % of failures and 17% of time on open nets.

13% of failures and 12% of time repairing 16k MOS RAM failures (standard
configuration was 2 modules).
37% of failures and 28% of time replacing other DIPs and SIPs.

5% of failures and 10% of time on T80 problems.

<!-- PDF page 148 / printed page 141 -->

13% of failures and 11 % of time on power supply failures.
2% of failures and 2% of time on Terminal and display problems.
4% of failures and 20% of time on repairing boards damaged during manufacturing
or overheating.

The power supply failures were due to problems that have since been corrected, and most
of the service calls for microcode or software problems would not happen in the more
mature environment we have today. However, the other failures are believed to be
representative. Note that none of the MOS RAM 'failures was the reason for a servi~e call.
These were found when testing a machine with diagnostics after a service call had been
made for some other reason.

### 13.6. Error Correction

Reliability has been improved by error-correction on storage. The Dorado error-correction
unit of 64 data and 8 check bits (quadword), guards 1152 MOS RAMs from single failures,
but almost no other parts on storage boards or in the error corrector are guarded.

Our Alto experience suggests that some machines repeatedly fail under normal use due to
undiagnosable failures. For this reason, error correction should be viewed as guarding not
only against new failures but also against imperfect testing of parts that are either already
bad or subject to noise (e.g., cosmic rays) or other kinds of intermittent failure. The latter
may be more important in our environment.

The failure summary above indicates, for a small sample, that 16k MOS RAMs, accounting
for 6% of all DIPs and SIPs (because the 15 Dorados had 2-module configurations, half the
maximum) average about 4 times the failure rate of other parts and account for about 1.5
failures/year/Dorado - this would become 3 failures/year with a 4-module configuration. If
we continue to do this well, a Dorado with error correction should run for years without
uncorrectable MOS RAM failures. The manufacturer's literature indicates that the dominant
failure mode appears to be single-bit failures with row and column addre13sing failures
affecting many bits somewhat less frequent, but we don't know the distribution of these.

If MOS failures do become significant, different strategies may be needed for single- and
multi-address failure modes. With a multi-address failure, another failure in the same
quadword causes a double error; but many single-address failures can occur in the same
quadword without double .errors.

The failure model used below shows that with no periodic testing and replacement of bad
MOS RAMs, fatal failure statistics of the 1152 RAMs would approximate those of a 108
RAM uncorrected. store. By thoroughly testing storage and replacing bad parts 4 times
more often than the mean time to total failure of a part (defin~d below), the likelihood of an
uncorrectable RAM failure crashing the system can be made insignificant compared with
other sources of failure.

Although system software could bypass all pages affected by a multi-address RAM failure,
the entire module, 25% of storage, would be eliminated, so this is impractical except on an
emergency basis. Continuing execution despite a multi-address RAM failure will result in a
double error when any other coincident storage failure occurs in the same quadword; 1/16

<!-- PDF page 149 / printed page 142 -->

of future failures will do this.

Some interesting questions are: How does MTBF vary with the EC arrangement? MTBF is
pertinent if we let Dorados run until they fail. Alternatively, how likely is a failure in the
next day, week, or month, if we test the memory that often and replace bad RAMs? These
questions can be asked assuming perfect testing (no failures at t =0) or imperfect testing
(some likelihood of failures at t =a because diagnostics didn't find them).

To answer them, MOS RAM failures are modelled as one of two types: those affecting a
single address in the RAM (called SF's), and those affecting all addresses (called TF's).
We assume that TF's occur about 1/4 as often as SF's in 4Kxl RAM's. RAM failures are
assumed exponentially distributed, correct if the failure rate doesn't change with time; over
the time range of interest, this is reasonable. Finally, perfect testing is assumed, so there
are 0 failures at t = O. These assumptions give rise to the following:

let p = prob that an ic has a TF = 1 - e- at
let q = prob that an ic has a SF = 1 - e- bt
let n = number of MOS RAMs in the 'memory

Without error correction, MTBF is the integral from 0 to infinity of [(1 - p)(l _ q)]n =
1/n(a+ b). With b = 4a, in our 4-module system with n = 1024, this is 1/5120a =
.00018/a.

With error correction, failure occurs when, in a single EC unit, a TF coincides with either
another TF or an SF. This ignores two coinciding SF's which is about 4000 (16k RAMs) or
16000 (64k RAMs) times less likely.

let n = number of RAMs in an error correction unit
then Prob[no failure] = Prob[no TF] + Prob[1 TF and 0 SF]

Prob[no TF] = (1 _ p)n
Since failure modes are independent,
Prob[l TF and 0 SF] = np[(l - p)(l _ q)]n-1
Prob[no failure] = Pok = (1 - p)n + np«l - p)(l _ q»O-1
Pok = e- nat + n(1_e- at)(e-(a+b)(n-1)t)

This is the probability for a single EC unit, so mean time to failure for all MOS storage is
Pok raised to a power equal to the number of EC units. In other words, the argument of
the integral for a 4-module x 4 quadwords/module system is Pok 16 with n = 64 + 8; it is
Pok4 with n   =   256 + 10 for a one munch EC unit.

•   Then, expected time to failure for our 16 x n = 64 + 8 memory system, is about:

(1/n) * (1/16a + 16a/(16a+b)2 + 240a2 /(16a+2b)3 + 3360a3/(16a+3b)4)
= (1 Ian) * -(1/16 + 1/25 + 5/288 + 105/17208)
= (1/16an) * (1 + .64 + .28 + .006) = 1.93/16an
= 1.93/16*72*a = .00168/a
In other words, mean time to failure is about 1.93 times longer than the time to the first TF
= 9.5 times better than with no error correction = as often as 1024/9.5 = 108

<!-- PDF page 150 / printed page 143 -->

uncorrected storage ic's.

The results don't change much when imperfect testing is assumed. The effect of this is to
replace densities for p and q by 1 - Ae - at, where A would be .999 if there was a 1/1000
chance of aMOS ic being bad at t =O.

Remarks

On each storage board, data from MemO is transported to a shift register consisting of B flipflops which are
then written into the MOS RAM's after transport has been completed. This arrangement is unfortunate - any
failure in one of these components will cause a multiple error, and there are about 250 of these parts in a full
storage configuration.

One way to eliminate this problem while simultaneously reducing the part count on each storage board would
be to make modules consist of four storage boards, rather than two, so that only four flipflops receive data on
each bit path during transport; since each of these is in a different quadword, single failures would not cause
multiple errors.

The Dorado EC operates on quadwords, requiring 8 check-bits/64 data bits, or a 12.5% storage penalty.
Alternative schemes are: 10 check bits/256 data bits (3.9%); 9 check bits/12B data bits (7.4%); 7 check bits/32
data bits (22%); and no error correction at all (0%).

The implementation of the EC pipeline is such that wider correction units significantly increas~ the time for a
miss. The current quadword error corrector requires 7 clocks (3 clocks for setup and correction, 1 clock per
word of the quadword); this would become 11 clocks with a 12B-bit EC scheme or 19 clocks with a 256'bit EC
scheme. Although cache hit rate seems to be above 99%, some implementation avoiding this delay would still
be needed to make larger correction units attractive.

If our quadword correction unit were replaced by a 4 x n = 256 + 10 scheme:

1I4na + 4a/n(4a+b)2 + 3a2I2n(2a+b)3, where for b               = 4a this is
(1/4na)*(1 + 1/4 + 1136) = 1.28/4na = .0012/a

In' other words, MTBF is about 1.28 times longer than the time to the first TF. So error correction has
increased MTBF by a factor of 6.2 over no error correction; alternatively, a 10M-RAM corrected memory fails
as frequently as a 1064/6.7 = 159 RAM uncorrected memory.

Surprisingly, the 64 +8 EC scheme has only 42% longer MTBF than a 256 + 10 EC scheme. This improvement
may not be worth the 96 additional MOS RAM and BO other DIPs required for address buffering; the BO
additional DIPs might cause more failures than they save, being a net loss.

The other method of maintaining our systems is to regularly test storage and replace bad RAMs. Then the
likelihood of no double error before replacement is simply the value of the probability distribution (Pok4 and
Pok16 above) at the selected instant. This reduces to an approximation of the form Pok = [e - x + xe - x]m
where x   =nat, m is 4 or 16, and n    =            =                =
72 for m 4 or 266 for m 16. If this is evaluated at t        =
lImna,
1/2mna, 1I4mna, etc. the following results are obtained:

> **Table 28: Double Error Incidence vs. Repair Rate**

![Table 28: Double Error Incidence vs. Repair Rate](tables/table-28-double-error-incidence.png)

The interpretation of this table is as follows: Measure mean time to total failure (TF) of a MOS RAM and call
this time lIa; then assume 4 SF's per TF. Then the rate at which TF's occur in storage will be lImna. So
the above tables show probability that the Dorado hasn't suffered a double error when tested and fixed as
often, 112 as often, 114 as often, or 118 as often as the mean rate of TPs.

<!-- PDF page 151 / printed page 144 -->

## 14. Performance Issues

This chapter discusses two issues:

(1) How rapidly will Dorado be able to execute Mesa, Lisp, SmaliTalk, etc. macroprograms;

(2) What relationship do some of the design parameters bear to performance;

### 14.1. Cycle Time

The first issue is cycle time. Dorado was designed for a 50 ns cycle time; the first three
prototypes used stitchweld technology for interconnections and operated correctly at 55 ns
cycle time; however, subsequent machines are being built using multiwire technology and
will not operate faster than about 60 ns cycle time. The baseboard at present initializes the
clock period to 64 ns for all machines during a boot, although there is some indication that
design changes made recently and repair of a few lingering slow path problems would
permit 5 to 10 ns faster operation.

With respect to achievable cycle time, the two important differences between stitchweld
and multiwire technology are that stitchweld uses point-to-point wiring and has wire
impedance of about 100 ohms (which is ideal), but multiwire uses Manhattan (square-
corner) wiring with wire impedance of about 50 ohms on the inner layer and 70 ohms on
the outer layer of wiring (Most signals are in the outer layer.); longer wires and imperfect
impedance matching result in slower speed.

### 14.2. Emulator Performance

Gene McDaniel's measurements of the Alto Mesa compiler have been adjusted to make
them compatible with Pilot Mesa and are summarized below. It must be pointed out that
the compiler makes heavier use of short pOinters than do Pilot Mesa programs; programs
being developed now are heavily biased toward long pointers and would be slower than the
execution rate below indicates. Average execution rate was about 5.6 cycles/opcode
excluding disk wait. About 38% of all cycles are consumed by XFER opcodes (i.e.,
subroutine call or return) and account for about 6% of opcodes executed. If these are
excluded, the remaining 94% average about 3.1 cycles/opcode; if jumps and conditional
jumps are also excluded (about 14% of executions), the others average 2.5 cycles/opcode.
These times include all memory and IFU delays.

These excellent results indicate that there are no unusual delays due to problems with the
memory or IFU and that the processor is completing most opcodes quickly. Since XFER
opcode take 34 (local) to 54 (external) cycles/opcode excluding memory delays, speeding,
respecifying, or reducing executions of XFER seem to be the most promising ways of
improving performance.

In the above results, instruction forwarding has saved an average of about .25
cycles/opcode or about 4% overall, in agreement with our expectations.

<!-- PDF page 152 / printed page 145 -->

For SmallTalk and Lisp instruction sets, performance is much worse than Mesa (averaging
over 30 cycles/opcode on Smalltalk 76). Careful studies should be made to understand the
reasons for this fully, but one reason is that the 16-bit word size is a serious limitation.
Long storage pointers are used extensively, so execution would be substantially faster on a
machine with, say, 32-bit data paths.

### 14.3. IFU Not-Ready Wait

For the Mesa compiler, 19.5% of all cycles were in IFU not-ready wait; 16% due to
incorrectly predicted jumps, 2.5% to cache miss wait, and 1% to other causes. The 16%
due to incorrectly predicted jumps might be improved.

The Mesa microcode presently predicts that all conditional jumps will not jump; it is
desirable to predict not-jump unless more than 75% of executions jump due to the
overhead of restarting the IFU an extra time. 40% of the time the prediction is wrong and a
jump occurs, so it seems that the microcode is doing the best it can'.

However, some loops ("while J ne 0 do," for example) are compiled as a normally false
conditional jump at the beginning of the loop and an unconditional jump from the end of
the loop back to the beginning; a faster sequence is a normally true conditional jump at the
end of the loop, eliminating the unconditional jump altogether. The general objectives in
changing the compiler would be as follows: (1) Eliminate unnecessary jumps and
conditional jumps; (2) Make the jump/not-jump execution of conditional jumps be as
predictable as possible; and (3) Make the not-jump path be the most likely, unless this
conflicts with objective (1).

### 14.4. Microstore Requirements

Speed is not the only issue-some reduction in microstore requirements might be possible
through design changes. Space requirements for a 1981 release of the Alto/Mesa
emulator system were as follows:

> **Table 29: Utilization of the Microstore**

![Table 29: Utilization of the Microstore](tables/table-29-microstore-utilization.png)

<!-- PDF page 153 / printed page 146 -->
Since we do not require that more than two emulators be loaded in the microstore at one
time, there is presently a little space left for extensions. MicroD is able to utilize well over
99% of the available microstore.

The third performance issue is cache efficiency and miss wait; the fourth is available io
bandwidth and io task cycle consumption. These are discussed in sections below.

### 14.5. Cache Efficiency and Miss Wait

The value of shortening the wait for a storage read is roughly proportional to miss
likelihood. Suppose that the prototypical opcode was a one-byte opcode implemented by
the following microcode:

Fetcht-Id, StkP+1;
Stackt- Md, IFUJump[O];

For this example, execution time on a hit is 2 cycles; on a miss, 28 cycles. Delay for IFU
misses must be added to this. Since the IFU is 6 bytes ahead of the current opcode, its
misses delay 28 cycles less execution time for preceding 6 bytes; if any of the 6 bytes itself
causes a miss, IFU delay will be 0 because it will catch up; the IFU never gets two misses
(in this example) because it crosses at most one munch boundary. Hence, execution time
will be 2 + 26*(1 - H) + (28-12)*H 6 *(1 - H), with the following results:

> **Table 30: Execution Time vs. Cache Efficiency**

![Table 30: Execution Time vs. Cache Efficiency](tables/table-30-execution-time-vs-cache-efficiency.png)

This ~rude analysis shows the importance of cache efficiency in determining system
performance.    Fortunately, measurements made by Doug Clark and Gene McDaniel
indicated the following surprisingly high cache hit statistics:

Overall cache hit rate on three Mesa programs was 99.2% to 99.8%. 4.9% to 8.1 %
of all cycles were held. 10% to 19% of references were Storet-'s, the rest fetches.
16% to 66% of misses had dirty victims, which cause additional cycles to be held
while the cache address section is busy.

Another measurement showed a 99.7% hit rate for IFU references.

The processor obtains a word from the cache in 16% of all cycles and the IFU in
32% of all cycles; the processor actually shuts out the IFU by making its own

<!-- PDF page 154 / printed page 147 -->

reference about 20% of the time.

Provision has been made to expand the Dorado cache to 16k words, when 4k x 1 MECL
RAM's are economically available, but the existing cache is so efficient that this may never
be necessary.

### 14.6. Performance Degradation Due to IO Tasks

To first approximation, only the display controller word task (DWT) uses enough storage
bandwidth to interfere Significantly with emulators. Since it uses tlie fast io system, DWT
requires service once/munch and will require two instructions/wakeup in the ordinary
case. In addition, if the next instruction (by another task) issues a memory reference, it will
always be held one cycle while the DWT's IOFetchi- advances ASRN.

A quick calculation shows that at an io bandwidth of 256 x 106 bits/sec (106 munches/sec)
the display controller will use 48% of storage bandwidth and 12% of processor cycles at 60
ns/cycle.

The earlier example showed that with no io interference and a 99% hit rate, the emulator
spent 17% of cycles in miss wait, 83% in useful execution. With a 256 x 106 bit/sec display
active, emulator misses are slowed about 2 cycles each, so the overall effect of the display
would be that about 78% of all cycles are emulator executions, 12% display task
executions, and 16% hold; the one cycle holds for IOFetchi- would make performance
somewhat worse than this.

An IOFetchi- by the display task to the same cache row as an emulator miss will remain in
the address section, increasing display task latency and requiring more buffering.
However, this won't degrade emulator performance.

The Alto monitor only uses 14.7 x 106 bits/sec (1/17 of the above) and would not interfere
appreciably with emulators.

The disk controller is the fastest "slow" io device among standard peripherals. When
running, its word interrupt task reads a double word from the cache every 3.2 p.s in a 3
instruction/interrupt inner loop, consuming about 5.6% of all cycles at 60 ns/cycle. Its
memory references consume the cache at a rate of .04 munches/p.s, low enough that
storage interference with the emulator isn't significant. However, a 256-word disk transfer
displaces about 1/16 of the cache entries, so the emulator may experience a lower hit rate.

### 14.7. Cache and Storage Geometry

The current geometry was chosen without measurements or simulation of programs, but
measurements made since then have indicated a surprisingly good cache performance, so
not much could be gained through changes.

The following parameters are relevant:

1 word as the unit of storage inside the memory pipeline;
16-word munch;

<!-- PDF page 155 / printed page 148 -->

256 munches in the cache (expandable to 1024);
4 columns in the cache.

Munch Size

A 16-word munch size was chosen primarily because 8 cycles for transport balances 10
cycles for storage access, avoiding loss of bandwidth. The use of 256x4 RAM's to
implement the cache address section allows the original 4k-word cache (implemented with
lkxl RAM's) to be expanded to 8k words or 16k words, when 4kxl RAM's are economically
available-this is possible because only 64 of the 256 words in the address section are
being used with the 4k-word cache. Miss wait is about 28 cycles and storage bandwidth
about 533 x 106 bits/sec with 16-word munches.

8-word munches would lower the storage bandwidth to about 262 x 106 bits/sec, probably
unacceptable. Also 8-word munches would limit cache expansion to 8k words. However,
miss wait would be reduced to about 24 cycles because transport would require only 4
cycles. 32-word munches would not allow gr~ater storage bandwidth to fast io devices
because bandwidth is already limited by transport with 16-word munches. Nor would it
allow expansion to a larger cache data section because we have no way to build a data
section larger than 16k words. Also, miss wait would be slowed to 36 cycles, so it does not
seem that this munch size is attractive.

For a given size of the cache data section, with smaller munches the cache will tend to
stabilize with a larger amount of useful information; however, when a program is changing
contexts, larger munches might bring the new context into the cache more quickly. Also,
fast io tasks will interfere less with the emulator on 'larger munches because fewer wakeups
and IOFetch←'es will be required. However, the extra buffering and longer miss wait offsets
this advantage somewhat.

Considered together, these factors suggest that the 16-word munch we are using is
substantially better than either 8 or 32-word munches.

Data Path Width

Having only 16 bit wide data paths slows misses. Doubling the paths to 32 bits would
reduce EC time by 1 cycle and transport time into the cache by 4 cycles (Le., delay on
misses would be 23 cycles instead of 28). There were not enough edge pins to do this.
However, if a method of doubling the path width were found, the storage system would
probably be arranged as two modules of four storage boards each rather than four
modules of two boards each, and 32-word munches might be better than 16-word
munches.

<!-- PDF page 156 / printed page 149 -->

Cache Columns

The reason for multiple columns is to approximate LRU reloading; the columns are
moderately expensive because separate hit logic has to be provided for each one; the V-NV
stuff also costs a few ic's iNith more than two columns. Altogether the current 64x4 cache
is about 40 ic's larger than a 128x2 cache (Because of its 50-50 LRU behavior on the
fourth column, our cache is somewhere between the 64x4 and 128x2 or 128x3 caches
below.). The table below shows likelihood that the Nth LRU munch is no longer .in the
cache for various geometries:

> **Table 31: Cache Geometry vs. LRU Behavior**

![Table 31: Cache Geometry vs. LRU Behavior](tables/table-31-cache-geometry-vs-lru.png)

These numbers are computed from a binomial distribution using the following formulae:

let R = rows in cache
let C = columns in cache
then p = (R - 1)/R = probability that a munch of VA is in its row
then q = 1/R = probability that a munch of VA is not in its row
then probability of a miss for the nth element is:

C       P(miss)

1       1   _   pn
2       1   _   pn _ nqpn - 1
3       1   -   pn _ nqpn-1 _ n(n_1)q2pn-2/2!
4       1   -   pn - nqpn-1 - n(n _1)q2pn-2/21 -          n(n -1)(n _ 2)q3pn- 3/31
etc.

Without extensive measurements on programs, it is impossible to know how much better,
say, a 32x4 cache is than a 64x2 cache, or to know whether a 128x2 cache is better or
worse than a 32x4 cache, for example. If a particular program is confining itself to a very
small set of munches, then more closely approximating LRU reloading is most important.
However, if the likelihood of reference flattens out after a small N, then it won't matter
much that LRU reloading isn't very well approximated-the total size of the cache will be a
more important determinant of performance.

<!-- PDF page 157 / printed page 150 -->

## 15. Glossary

a - the first 8-bit operand of a t\<lJo-byte or longer opcode.

f3 - the second 8-bit operand of a three-byte or longer opcode.
bypassing - a number of memories and task-specific registers in Dorado (RM, STK, and T,
for example) are written with data that might be needed before the write occurs. These are
implemented so that data about-to-be-written is substituted for data read from the register
or memory when appropriate. This substitution is called bypassing. and enables Dorado to
run considerably faster than would otherwise be possible.

cache entry - a munch together with VA of the munch and 4 flag bits. For a 64 row x 4
column cache, VA[28:31] are the word in the munch, VA[22:27] address the row, and
VA[7:21] are stored in the cache entry.

column - one of 4 groups of 64 (expandable to 256) cache entries. The cache column in
which a word with VA resides is determined by comparing VA[7:21] with the corresponding
bits stored in the four columns at row VA[22:27]. Thus a memory word may occupy one of
4 locations in the cache.

control processor - the microcomputer on Dorado's baseboard, or the Midas program
operating Dorado from an Alto.

dirty - a cache entry is dirty if the information in it differs from information in storage,
because a store has been done into the cache, and storage has not yet been updated. A
page is dirty if a store has been done into the page since its map dirty bit was cleared.

emulator - the lowest priority task, number 0, always awake. The emulator is distinguished
by the fact that it cannot block, can use Stk, and has a private pipe entry. Primarily the
emulator task will implement instruction sets.

entry vector - the exit microinstruction of an opcode sends controt to the first
microinstruction of the next opcode by means of IFUJump[n] (n = 0 to 3), where n
chooses one of 4 entry microinstructions for the next opcode; these four microinstructions
are the next opcode's entry vector.

fault task - the highest priority task, number 15, woken whenever a memory fault or stack
error occurs.

hit - a reference which finds the desired word in the cache.

Midas - the Alto' program used for loading and debugging Dorado remotely.

miss - a reference which does not find the desired word in the cache.

module - the unit in which storage is packaged, either 64K, 256K, or IM words. A machine
may have 1 to 4 modules.

<!-- PDF page 158 / printed page 151 -->

MTBF - mean time between failures.

munch - 256 bits, or 16 machine words; the unit of data for main storage.

parity - the parity of a data unit is the exclusive-or of all bits in the data unit; parity has the .
property that changing any single bit in the data unit will also change the parity, so it can
be used to detect single failures. A data unit has odd parity when the number of 1's in the
unit is odd, eveil parity when the number of 1's is even. Dorado uses odd parity
everywhere, which means that the number of 1's in the data unit including its associated
parity bit should be odd when data is correct.

PC - "program counter". In this manual PC refers to the 16-bit byte displacements relative
to SR 31 (the codebase) which are maintained by the IFU for the current instruction set.
This term should be distinguished from TPC, which refers to the address of the next
microinstruction for a task.

pipe - a 16-entry memory which records the state of the last few storage references.

quadrant - one of the four 4k-word regions in a 16k-word control store.

RAM - "random access memory"; selected words in the memory can be both read and
written.

reference - a reference to the memory, initiated by the processor or by the IFU. A
processor reference transfers a single word between the cache and the processor; an ;0
reference transfers a munch between storage .and an io device.

ROM - "read-only memory"; the contents of the memory are specified when the hardware
is constructed and cannot be modified during program execution. ROM elements used on
Dorado can be reprogrammed with a speCial device constructed for the purpose.

row - one of the 64 or 256 groups of 4 cache entries. The cache row in which a word
resides is determined by bits 20 .. 27 of its virtual address.

storage - the main memory of the machine, organized in munches of 256 bits, or 16
machine words.

storage reference - a reference to the storage, initiated as a result of a memory reference.
A processor reference causes a storage reference if there is a cache miss or if the FDM;ss
control is true in the memory control register; an io reference always causes a storage
reference.

•   storage reference number (SRN) - an address of a pipe entry which identifies a particular
storage reference.

subtask - a two-bit number presented by an io device to the processor and memory system
while its task is running.    The processor OR's subtask with RSase[3] .. RSTK[1] in
determining the RM address and with MemSase[2:3] in determining the base register
selection. The memory system buffers the subtask for fast io devices, and then sends it
over the Fin or Fout bus as part of device identification.

<!-- PDF page 159 / printed page 152 -->

tag - The extra bit in Md readout which complements for successive Fetchf-'es and
Store←'s by the same task.    Agreement of the bit in Md with the current value equals
reference finished.

task - one of the 16 priority scheduled tasks. Special tasks are the emulator (task 0, lowest
priority) and the fault task (task 15, highest priority). Other tasks are paired with io
controllers.

VA . virtual address.

Vacant . a cache entry or map entry which does not contain valid data.

Victim (Vic) memory • stores 4 bits for each cache row. Two of the bits specify the victim
which will be chosen if a reference to that row results in a miss, and the other two are the
next victim.

victim . on a processor reference that causes a cache miss, the cache entry chosen to be
replaced by the referenced data.

WP • write protected.     Map entries and cache entries have bits with this name.

---

# Figures

*These are the full-page schematic, data-path, and timing diagrams from the back of the original manual (drawn with the Xerox PARC SIL logic-drawing program). Reproduced here as high-resolution scans.*

## Figure 1: Dorado: Programmer's View

![Figure 1: Dorado: Programmer's View](figures/figure-01-programmers-view.png)

## Figure 2: Card Cage

![Figure 2: Card Cage](figures/figure-02-card-cage.png)

## Figure 3: Processor Hardware View

![Figure 3: Processor Hardware View](figures/figure-03-processor-hardware-view.png)

## Figure 4: Shifter

![Figure 4: Shifter](figures/figure-04-shifter.png)

## Figure 5: Control Section

![Figure 5: Control Section](figures/figure-05-control-section.png)

## Figure 6: Next Address Formation

![Figure 6: Next Address Formation](figures/figure-06-next-address-formation.png)

## Figure 7: Instruction Timing

![Figure 7: Instruction Timing](figures/figure-07-instruction-timing.png)

## Figure 8: Overall Structure of the Memory System

![Figure 8: Overall Structure of the Memory System](figures/figure-08-memory-system-structure.png)

## Figure 9: Cache, Map, and Storage Addressing

![Figure 9: Cache, Map, and Storage Addressing](figures/figure-09-cache-map-storage-addressing.png)

## Figure 10: The Pipe and Other Memory Registers

![Figure 10: The Pipe and Other Memory Registers](figures/figure-10-pipe-and-memory-registers.png)

## Figure 11: Error Correction

![Figure 11: Error Correction](figures/figure-11-error-correction.png)

## Figure 12: Instruction Fetch Unit Organization

![Figure 12: Instruction Fetch Unit Organization](figures/figure-12-ifu-organization.png)

## Figure 13: Disk Controller

![Figure 13: Disk Controller](figures/figure-13-disk-controller.png)

## Figure 14: Display Controller

![Figure 14: Display Controller](figures/figure-14-display-controller.png)

## Figure 15: Display Controller IO Registers

![Figure 15: Display Controller IO Registers](figures/figure-15-display-controller-io-registers.png)

## Figure 16: Ethernet Controller

![Figure 16: Ethernet Controller](figures/figure-16-ethernet-controller.png)

## Figure 17: Programmers' Crib Sheet

![Figure 17: Programmers' Crib Sheet](figures/figure-17-programmers-crib-sheet.png)

