# Dorado disk architecture — reference

Detailed reference for HM §9 (Disk Controller) and the Trident
SMD drives. Companion to `docs/io-systems-architecture.md`
(high-level I/O overview) and `dorado/include/disk.h` /
`src/disk.c` (current implementation).

Sources: HM §9 pages 92–108, Table 23 (T-80 specs), the Tag Register
section (page 99), Format RAM table (page 98), Sequence PROM tables
(page 99), KSTATE/KSTAT muffler signals (pages 102–104), and the
Fire Code ECC derivation (pages 104–108). Also ContrAlto2's
TridentDrive.cs / TridentController.cs / DiskPack.cs as reference
for sector layout.

## At a glance

```
   Microcode (DSK task 14₈)                        Fast IO (Fin/Fout)
        │ slow IO via TIOA 10₈-14₈                          ▲
        ▼                                                   │
   ┌──────────────────────────────────────────┐  ┌──────────┴───────┐
   │   Disk Controller (Memory section)       │  │  Main storage    │
   │                                          │  │  via IOFetch←/   │
   │  • DiskControl  (control register)       │  │  IOStore←        │
   │  • DiskMuff     (muffler in/out)         │  └──────────────────┘
   │  • DiskData     (16-word FIFO i/o)       │           ▲
   │  • DiskRam      (16×12 Format RAM)       │           │
   │  • DiskTag      (16-bit tag register)    │  16-word FIFO
   │                                          ├──────────┘
   │  • Sequence PROMs (read + write)         │
   │  • Fire Code ECC: P(X)=X³²+X²³+X²¹+X¹¹+X²+1 │
   │  • Sector counter (per drive)            │
   └─────┬────────────────────────────────────┘
         │  daisy-chain cable: 16 control tags
         │  + 9 status signals
         ▼
   ┌────────────────────────────────────────┐
   │  TridentDrive[0..3]  (Century Data)    │
   │   T-80:  82 MB,  815 cyl × 5 heads     │
   │   T-300: 290 MB, 815 cyl × 19 heads    │
   │   3600 RPM, 9.67 Mb/s, ~30 ms avg seek │
   │   117 subsector pulses + 1 index/rev   │
   └────────────────────────────────────────┘
```

The disk system is one of the more elaborate parts of the Dorado.
Per HM §9 page 92, it uses **task 14₈ (DSK)** and the **first five
TIOA addresses in block 10₈-17₈** (the Ethernet controller, on the
same logic board, uses two of the other three). The full pipe from
microcode to platter is:

1. Microcode loads the **Format RAM** (16 × 12-bit) with word counts
   and tag commands describing the desired sector layout.
2. Microcode loads the **DiskControl** register with operation codes
   for each of the (up to 4) blocks within a sector.
3. Microcode issues a **Tag** command to seek / select drive / set
   head, etc., via the daisy-chain cable.
4. At the next sector pulse, the controller becomes "Active" and
   sequences through the **Sequence PROMs** (one for read, one for
   write), driving the tag register from the Format RAM and
   reading/writing the 16-word **FIFO**.
5. The **DSK task** is woken on each FIFO threshold to feed/drain
   data via Pd←Input / Output←B.

## Geometry

### Trident T-80 specs (HM Table 23, page 95)

| Property                  | Value                              |
|---------------------------|------------------------------------|
| Capacity                  | 82.1 MB unformatted (8-bit bytes)  |
| Transfer rate             | 9.67 × 10⁶ bits/sec (1.65 µs/16-bit word) |
| Cylinder positioning time | 6 ms cyl-to-cyl, 30 ms avg, 55 ms max |
| Rotational speed          | 3600 RPM (16.66 ms/revolution)     |
| Subsectors/revolution     | 117 (drive-generated pulses)       |
| Densities                 | 370 cyl/in, 6060 bits/in           |
| Disk pack                 | IBM 3336-type, 5 surfaces + 1 servo, 815 cyl/surface |
| Recording                 | MFM (Modified Frequency Modulation) |
| Error rate                | Recoverable: 1 / 10¹⁰ bits         |
|                           | Irrecoverable: 1 / 10¹³ bits       |
|                           | Positioning: 1 / 10⁶ seeks         |
| Pack start/stop time      | 20 sec                             |

### T-300

Same physical dimensions and specs as T-80, but 19 surfaces / heads
instead of 5 → 290 MB capacity. Otherwise mechanically identical.

### Sector layouts (HM page 94)

The controller is fully programmable via the Format RAM. Common
layouts on a T-80:

| Format            | Sectors/track | Subsectors/sector | Data words/sector |
|-------------------|---------------|-------------------|-------------------|
| Alto Diablo emul. | 28            | 4                 | 256               |
| Pilot             | 16            | 7                 | 512               |
| Alto Trident      | 9             | 13                | 1024              |

The 9-sector / 13-subsector format divides the disk evenly (117/13
= 9). The 16-sector format leaves an unusable leftover fraction of
a sector. The 28-sector format leaves no leftover.

> **Note**: the 16-sector Pilot format is *incompatible* with the
> 16-sector Alto Trident format used by Juniper, even though both
> are 16 sectors. Use 9-sector for IFS interoperability.

Within a sector, blocks (1-4 of them) are sequenced by the read or
write Sequence PROM. Each block can be Read, Write, Read+Check, or
Done. Conventional Alto Trident layout is:

```
| Header (2 words) | Label (10 words) | Data (1024 words) |
```

= 1100 words/sector + 38 words for first-block delays + 14 words for
each of blocks 2 and 3 + 31.35 µs intersector gap = 1119 words/sec ×
13 subsectors = 117 subsectors. ✓

### Pack image format (ContrAlto2 / Bitsavers)

ContrAlto2 stores Trident packs as a flat byte stream:

```
For each (cyl, head, sec):
  2 bytes dummy
  4 words (= 8 B) header
  20 words (= 40 B) label
  2048 words (= 4096 B) data
  Total: 4146 bytes/sector
```

T-80 image: 815 × 5 × 9 × 4146 = 152,053,470 bytes (~152 MB), the
extra-versus-spec slack being the dummy and header/label overheads.
Words are little-endian on disk.

`dorado_disk_pack` (in `include/disk.h`) reads/writes this format.

## TIOA register map (HM §9 page 92)

All on task 14₈. Other tasks **must not** select these TIOA addresses
or the disk controller will malfunction.

| TIOA | Name        | Direction | Function                         |
|------|-------------|-----------|----------------------------------|
| 010  | DiskControl | Output←B  | Control register + zero RAM addr |
| 011  | DiskMuff    | Output←B  | Muffler control + clear-flags    |
|      |             | Pd←Input  | Read selected muffler signal     |
| 012  | DiskData    | Pd←Input  | Read 16-word FIFO                |
|      |             | Output←B  | Write 16-word FIFO               |
| 013  | DiskRam     | Output←B  | Write Format RAM (auto-incr)     |
| 014  | DiskTag     | Output←B  | Write tag register (drive cmd)   |

## DiskControl register (HM page 97)

Output to `DiskControl` zeros the Format RAM address register so
subsequent `DiskRam` writes start at index 0.

| B-bits | Meaning                                                       |
|--------|---------------------------------------------------------------|
| B[5]   | ClearEnableRun                                                |
| B[6]   | SetDebugMode                                                  |
| B[7]   | SetBlockTillIndex                                             |
| B[8:9] | Op for 1st block: 0=Done, 1=Write, 2=Read+Check, 3=Read       |
| B[10:11] | Op for 2nd block (same encoding)                            |
| B[12:13] | Op for 3rd block                                            |
| B[14:15] | Op for 4th block                                            |

- **EnableRun**: gated globally — if `EnableRun = 0`, the controller
  is idle. Cleared by IOReset. Set only by completing a Format RAM
  load (writing the last word, see below).
- **DebugMode**: enables fake bit-clock generation when no drive is
  present (so diagnostics can run without a physical drive).
  Cleared by `DisableRun`.
- **BlockTillIndex**: masks sector and index task wakeups until the
  next index pulse. Used after switching drives or executing a
  ReZero, since the controller has lost sector synchronization.
  Does *not* clear wakeups already pending.

A request for a sector transfer is initiated by writing
`DiskControl` with non-zero op fields. The controller waits until
the *next* sector pulse, sets the `Active` flipflop, and executes.
Once Active, the control register is repurposed to shift in tag
commands rather than load IO data; to abort a transfer, load the
control register *twice* (first load clears Active, second loads
the new value).

## Format RAM (HM page 98)

16-word × 12-bit RAM. Loaded via `DiskRam` writes (auto-incrementing
address, zeroed by every `DiskControl` write). Loading the **last**
word (address 15) sets `EnableRun`.

Word counts in the table are 1 less than the desired count.

| Addr | Description                            | Alto Diablo example |
|------|----------------------------------------|--------------------:|
| 00   | Word count of 1st block                | 0001                |
| 01   | Word count of 2nd block                | 0007                |
| 02   | Word count of 3rd block                | 0377                |
| 03   | Word count of 4th block                | 0000                |
| 04   | Control tag command for read op        | 0104                |
| 05   | Control tag command for write op       | 0204                |
| 06   | Control tag command to set Head Select | 0004                |
| 07   | Control tag command to zero tag bus    | 0000                |
| 08   | Words to write 0s before 1st block     | 0033                |
| 09   | Words to write 0s before successors    | 0006                |
| 10   | Words to wait before reading 1st       | 0011                |
| 11   | Words to wait before reading succ.     | 0002                |
| 12   | ECC words plus one                     | 0002                |
| 13   | Word count of 2 (ECC fixed)            | 0001                |
| 14   | Word count of 1 (minimum count)        | 0000                |
| 15   | Not used                               | 0000                |

Words 0-3 are sector-format-specific (data sizes); words 8-14 are
drive-/timing-specific.

## Sequence PROMs (HM page 99)

Two PROMs (one for read, one for write) are addressed by an
internal program counter that's reset to 0 at the start of each
sector and incremented on each step's completion. Steps are
clocked by `WordClock` (derived from the disk bit clock from
pre-recorded timing on the pack).

### Write Sequence PROM

| Addr | Description                                       | Duration (WordClocks) |
|------|---------------------------------------------------|----------------------:|
| 00   | Issue tag command in RAM[6] (head select)         | 1                     |
| 01   | Delay (wait for head select to settle)            | RAM[13]+1             |
| 02   | Issue tag command in RAM[5] (write command)       | 1                     |
| 03   | Write long preamble for first block               | RAM[8]+1              |
| 04   | Write sync word                                   | 1                     |
| 05   | Write data for first block                        | RAM[0]+1              |
| 06   | Write first ECC word                              | RAM[14]+1             |
| 07   | Write second ECC word + 2 postamble words         | RAM[12]+1             |
| 08   | Advance control register to next-block op         | RAM[14]+1             |
| 09   | Issue tag command in RAM[5] (write command)       | 1                     |
| 10   | Write short preamble for second block             | RAM[9]+1              |
| 11   | Write sync word                                   | 1                     |
| 12   | Write data for second block                       | RAM[1]+1              |
| 13   | Write first ECC word                              | RAM[14]+1             |
| 14   | Write second ECC word + 2 postamble words         | RAM[12]+1             |
| 15   | Advance control register to next-block op         | RAM[14]+1             |
| 16-22 | Same as 09-15 except step 19 uses RAM[2]+1       |                       |
| 23-29 | Same as 09-15 except step 26 uses RAM[3]+1       |                       |
| 30   | Zero the tag bus                                  | 1                     |

### Read Sequence PROM

| Addr | Description                                       | Duration (WordClocks) |
|------|---------------------------------------------------|----------------------:|
| 00   | Issue tag command in RAM[6] (head select)         | 1                     |
| 01   | Delay (wait for head select to settle)            | RAM[13]+1             |
| 02   | Delay (skip over early part of preamble)          | RAM[10]+1             |
| 03   | Issue tag command in RAM[4] (read command)        | 1                     |
|      | *WordClocks cease until controller has read sync word* |                  |
| 04   | Read data for first block                         | RAM[0]+1              |
| 05   | Read ECC words                                    | RAM[13]+1             |
| 06   | Compute first word of ECC remainder, issue tag in RAM[6] | 1               |
| 07   | Compute second word of ECC remainder              | RAM[14]+1             |
| 08   | Advance control register to next-block op         | RAM[14]+1             |
| 09   | Delay (skip over early part of preamble)          | RAM[11]+1             |
| 10   | Issue tag command in RAM[4] (read command)        | 1                     |
|      | *WordClocks cease until controller has read sync word* |                  |
| 11   | Read data for second block                        | RAM[1]+1              |
| 12   | Read ECC words                                    | RAM[13]+1             |
| 13   | Compute first word of ECC remainder, tag in RAM[6] | 1                    |
| 14   | Compute second word of ECC remainder              | RAM[14]+1             |
| 15   | Advance control register to next-block op         | RAM[14]+1             |
| 16-22 | Same as 09-15 except step 18 uses RAM[2]+1       |                       |
| 23-29 | Same as 09-15 except step 25 uses RAM[3]+1       |                       |
| 30   | Zero the tag bus                                  | 1                     |

The PROMs implement a state machine that issues tag commands at
the right moments and counts WordClocks per step.

## Tag register (HM pages 99-101)

The 16-bit tag register drives the daisy-chain to all drives. All
drive commands flow through here. Loaded from IOB (via DiskTag
output) or from the Format RAM (during PROM-driven sequences).
Loading **Head Tag**, **Cylinder Tag**, or **Control Tag** activates
a timing circuit that:

- Enables only the tag-bus bits for the first 200 ns.
- Then enables Tag[0:3] for 1.2 µs.
- Then disables Tag[0:3] and sets the **TagTW** flip-flop to wake
  the disk task (= command completed).

The Drive Select Tag (Tag[0]) does NOT activate the timing circuit
— the timer counts disk clock cycles, but disk clocks are invalid
during drive-select changes.

### Tag bit field interpretation

Tag[0:3] = command type (4 bits = 16 commands, but only 4 used).

| Tag[0:3] | Type            | Tag[4:15] meaning                     |
|----------|-----------------|---------------------------------------|
| 0        | Drive select / subsector count | (see below)             |
| 1        | Head Tag        | Head select                           |
| 2        | Cylinder Tag    | Seek to cylinder                      |
| 3        | Control Tag     | Read/Write/Reset/etc.                 |

#### Tag[0]: Drive select and subsector count

| Tag[..] bits | Meaning                                               |
|--------------|-------------------------------------------------------|
| 4:9          | Subsector count: divides 117 subsector pulses by (count+1) to form sector pulses. |
|              | Tag[4:9] = 3 → 29 sectors (256-word data)             |
|              | Tag[4:9] = 6 → 16 sectors (512-word data)             |
|              | Tag[4:9] = 14₈ → 9 sectors (1024-word data)           |
| 10           | Load subsector count from Tag[4:9] for the drive selected *prior to* this tag. |
| 11:15        | Drive select (5 bits): 0..3 = drives 0-3 directly; 4..36₈ = drive 3 + extension; 37₈ = none. |

#### Tag[1]: Head Tag

Loads a register in the drive that selects the head used for
subsequent read/write commands. Tag wakeup occurs at completion (1.6
µs).

| Tag[..] bits | Meaning                                              |
|--------------|------------------------------------------------------|
| 4:7          | Unused                                               |
| 8            | Off Cylinder — may be activated during read to recover unreadable data; offsets cylinder positioning by 80 µin. |
| 9            | Determines direction of offset if bit 8 is set.      |
| 10:15        | Head number (0..4 for T-80; 0..18 for T-300). Invalid → "EndOfCylinder" / HeadOverflow error. |

#### Tag[2]: Cylinder Tag

Causes the drive to seek to the specified cylinder. Tag wakeup
occurs after the timing sequence (1.6 µs); NotReady status bit
remains until the seek physically completes (3-55 ms depending on
seek distance).

| Tag[..] bits | Meaning                                              |
|--------------|------------------------------------------------------|
| 4:15         | Cylinder number (0..814 for Trident). Illegal → DeviceCheck. |

#### Tag[3]: Control Tag

Tag wakeup occurs at command completion (1.6 µs) AND upon
completion of the last read/write operation in a sector. Generally
issued by the controller itself (using Format RAM tag commands)
rather than directly by microcode. DeviceCheck Reset and ReZero
are exceptions.

| Tag[..] bits | Meaning                                              |
|--------------|------------------------------------------------------|
| 4            | AltoLeader — special flag for reading Alto-Tricon-format disks. Should only be used in Alto Trident simulation. |
| 5            | Unused                                               |
| 6            | Strobe Late — causes drive data-recovery circuits to sample data early within the bit time (for recovery). |
| 7            | Strobe Early — like StrobeLate but the obvious way.  |
| 8            | Write — turns on the write circuits.                 |
| 9            | Read — turns on the read circuits.                   |
| 10           | Unused                                               |
| 11           | Reset Head register (zeros the head address in the drive). |
| 12           | Device Check Reset (resets all latched error conditions in the drive). |
| 13           | Head Select — turns on head selection circuits, used with a Read or Write. |
| 14           | ReZero — repositions the heads to cylinder 0 (if loaded), resets head address, clears SeekIncomplete and DeviceCheck. |
| 15           | Head Advance — increments the head address register in the drive. |

## FIFO register (HM page 101)

Data flows through a 16-word FIFO (= 25 µs of buffering). Each FIFO
word holds 16 data bits + 2 parity bits + a 2-bit tag indicating
whether the next word is write, read, or read-and-check data.
Read/written via Pd←Input / Output←B at TIOA `DiskData`.

- During output to disk: controller checks parity both when receiving
  data on IOB *and* when reading from the FIFO.
- During disk read: parity computed before writing into the FIFO,
  passed through the FIFO, then written on IOB for the processor
  to test.

## DiskMuff register (HM pages 101-102)

The Dorado uses a multiplexor scheme called the **muffler system**
for reading miscellaneous logic signals during debugging. Writing
DiskMuff selects a muffler address; the next Pd←Input from
DiskMuff returns the value of that signal on IOB[15].

DiskMuff output bits also drive other operations:

| Bit  | Action                                                     |
|------|------------------------------------------------------------|
| B[0] | Simulate read data of 1 for 1 cycle (diagnostic)           |
| B[1] | Simulate read clock of 1 for 1 cycle (diagnostic)          |
| B[2] | Clear CompareErr — done by disk task if read+compare OK    |
| B[3] | Set ReadDataErr — done by disk task to inhibit future writes |
| B[4] | Clear the index wakeup flip-flop                           |
| B[5] | Clear the sector wakeup flip-flop                          |
| B[6] | Clear the tag wakeup flip-flop                             |
| B[7] | Clear all error flip-flops within the controller (not the drive) |
| B[8:15] | Muffler address (selects which signal to sample)        |

After writing DiskMuff, microcode must wait *one cycle* before
reading via Pd←Input.

## Muffler signals (HM pages 102-104)

For diagnostic readout via DiskMuff. Grouped into 16-bit words for
Midas examination.

### KSTATE — controller state bits

| Addr | Signal       | Meaning                                              |
|------|--------------|------------------------------------------------------|
| 000  | TempSense    | Temperature sense                                    |
| 001  | IndexTW      | Wakeup due to index pulse (16.7 ms / rev). Also forces SectorTW. |
| 002  | SectorTW     | Wakeup due to sector pulse                           |
| 003  | TagTW        | Wakeup due to tag command completion (1.6 µs after Output to DiskTag, plus end-of-sector after last read/write transfer) |
| 004  | RdFifoTW     | Wakeup due to ≥3 words in FIFO during normal read or 1 word during read+check. Disabled by DiskControl op. |
| 005  | WrFifoTW     | Wakeup due to ≥4 words of free space in FIFO (write mode). |
| 006  | ReadData     | Data bit from disk (for diagnostics)                 |
| 007  | WriteData    | Data bit to disk                                     |
| 010  | EnableRun    | Format RAM has been written, wakeups enabled         |
| 011  | DebugMode    | Controller in debug mode                             |
| 012  | RdOnlyBlock' | Block in normal read mode                            |
| 013  | WriteBlock'  | Block in write mode                                  |
| 014  | CheckBlock'  | Block in read+check mode                             |
| 015  | Active       | Controller is processing a command                   |
| 016:017 | Select.0..1 | Address of currently selected drive                 |

### KSTAT — drive/controller status

| Addr | Signal      | Meaning                                              |
|------|-------------|------------------------------------------------------|
| 020  | SeekInc     | Heads not positioned within last 700 ms — needs ReZero |
| 021  | HeadOvfl    | Head address invalid (>4 for T-80)                   |
| 022  | DevCheck    | Various command/state errors — needs ReZero          |
| 023  | NotSelected | Drive in off-line test mode or not powered up        |
| 024  | NotOnLine   | Drive in test mode or heads not loaded               |
| 025  | NotReady    | Cylinder seek in progress / heads not loaded         |
| 026  | SectorOvfl  | Command active when next sector pulse occurred (hardware fault or format mismatch) |
| 027  | FifoUnderflow | FIFO empty during write or read got behind         |
| 030  | FifoOverflow  | FIFO full during read or write got ahead           |
| 031  | ReadDataErr | Set on CompareErr / ECCError / ECCComputeErr         |
| 032  | ReadOnly    | "Read-Only" switch on drive                          |
| 033  | CylinderOffset | Cylinder positioning currently offset             |
| 034  | IOBParityErr | Bad parity detected on IOB                          |
| 035  | FifoParityErr | Bad parity detected from FIFO                      |
| 036  | WriteErr    | OR of errors at muffler addresses 020-035            |
| 037  | ReadErr     | OR of errors at muffler addresses 020-031 + 034-035  |

### Other muffler ranges

- **KRAM** (040-057): Format RAM contents (40-43 = address, 44-57 = word)
- **KTAG** (060-077): Tag register contents (last 20-bit value loaded)
- **KFIFO** (100-117): FIFO state — ShiftIn, ShiftOut, ComputeECC,
  NextBlock, LoadTag, CntDone', OutRegFull, InRegFull, FifoWaddr
  (4 bits), FifoRaddr (4 bits). FifoWaddr == FifoRaddr → FIFO empty.

## Wakeup flow

The disk task wakes for several reasons; microcode must determine
the cause via Pd←Input on DiskMuff at addresses 001-005 and clear
the corresponding TW (DiskMuff output bits 4-6).

The controller distinguishes index pulses from sector pulses by
pulse width (single radial-cable line). On either pulse from the
selected drive, controller increments (sector wakeup) or zeros
(index wakeup) the firmware sector counter, then wakes the task.

For a DSK task scheduled at the start of a sector transfer:
1. Test SectorTW — increment sector counter, clear SectorTW.
2. Test IndexTW — zero sector counter, clear *both* IndexTW and
   SectorTW.

After loading a transfer command into DiskControl, the task blocks
until the next sector wakeup, decides whether the sector now
selected matches the desired one, and either retriggers or proceeds.

## Fire Code ECC (HM pages 104-108)

The 32-bit ECC scheme detects errors of any length, corrects bursts
≤ 11 bits.

### Polynomial

```
P(X) = (X¹¹ + X² + 1) · (X²¹ + 1)
     = X³² + X²³ + X²¹ + X¹¹ + X² + 1
```

During a write, the data stream is multiplied by X³² (to make room
for the 2-word ECC) and reduced modulo P(X). The resulting 32-bit
remainder is appended to the disk block.

During a read, the feedback shift register is reconfigured to
implement the two factors separately:

```
P₀(X) = X²¹ + 1
P₁(X) = X¹¹ + X² + 1
```

The read stream (data + appended ECC) is reduced modulo each. If
both remainders are zero, the read is error-free. If exactly one
is non-zero, the error is irrecoverable. If both are non-zero, an
error-pattern decoder runs in software to recover.

### Recovery (Chinese Remainder Theorem)

After the controller computes the remainders, microcode reads them
from the FIFO and runs:

```
D = Q · LCM − (A₀ · M₀ · S₀ + A₁ · M₁ · S₁)
```

where:
- E₀ = 21 (modulus of P₀)
- E₁ = 2047 (modulus of P₁)
- LCM = E₀ · E₁ = 42,987 (correctable record length in bits)
- M₀ = LCM/E₀ = 2047
- M₁ = LCM/E₁ = 21
- A₀ = 19, A₁ = 195 (constants such that Aᵢ · Mᵢ mod Eᵢ = 1)
- S₀ = number of left-circular shifts on ECC[11:31] until low 10
  bits are all zero (uncorrectable if not within 20 shifts)
- S₁ = number of reductions of ECC[0:10] modulo X¹¹ + X² + 1 until
  the result equals the error pattern (uncorrectable if not within
  2047)
- Q = smallest integer to make D positive
- D = displacement of rightmost incorrect bit from end of record

To avoid double precision, microcode uses:

```
D' = Q · 21 − 19 · S₀ − 2 · S₁
D  = 2047 · D' − S₁    (add 42,987 if D' = 0; HM notes the actual
                         calculation must use 2047·(D'+1) − S₁)
```

where 0 ≤ D' ≤ 20 and 0 ≤ S₁ ≤ 2046.

The error pattern (11 bits) and displacement D together let
software XOR the corrupted bits back to correct values.

> **Warning** (HM page 104): if a burst > 11 bits occurs, the
> algorithm will mis-correct, doubling the bad bits. So disk
> handling code should try other recovery (rereads, strobe early/
> late) before invoking ECC correction.

## What we have today

`dorado/include/disk.h` + `src/disk.c`.

**Phase 1**:
- **Pack image** read/write in ContrAlto2/Bitsavers format. T-80 and
  T-300 geometry constants.
- **Drive** struct: per-drive online/ready/RO/select state, current
  cyl/head/sec, seek-in-progress, index-pulse latches.
- **Controller** registered on task 14₈, TIOA 10₈-14₈:
  - DiskControl bit decode (ClearEnableRun, SetDebugMode,
    SetBlockTillIndex, ops 1-4).
  - DiskRam auto-increment + EnableRun on last word.
  - DiskData 16-word FIFO push/pop.
  - DiskTag register capture.
  - DiskMuff input packs the wakeup TWs + EnableRun + Active for
    microcode to read back.

**Phase 2**:
- **Tag decoder**: dispatches by Tag[0:3] (HM pages 99-101):
  - Tag[0:3]=0 (Drive Select): updates `selected_drive` + per-drive
    flags from Tag[11:15].
  - Tag[0:3]=1 (Head Tag): sets `cur_head` from low 6 bits, raises
    `tag_tw` wakeup.
  - Tag[0:3]=2 (Cylinder Tag): seeks to `cur_cyl` from low 12 bits,
    clears sector sync, raises `tag_tw`.
  - Tag[0:3]=3 (Control Tag): handles ReZero (cyl=head=sec=0),
    HeadAdvance (cur_head++), Read (loads FIFO with
    header+label+(data prefix) from current (cyl,head,sec), sets
    Active + `rd_fifo_tw`), Write (clears FIFO, sets `wr_fifo_tw` +
    Active).
- **Synthetic sector-pulse**: `dorado_disk_controller_advance_sector()`
  increments the sector counter (wraps at sectors-per-track), sets
  `sector_tw`, and reloads FIFO if the controller is in an active op.

What's not yet wired:

- **Sequence PROM execution**: the 30-step state machine that drives
  reads/writes per Format RAM. Currently the Read tag short-circuits
  by directly populating the FIFO from sector storage, bypassing
  the PROM/preamble/sync-word/postamble sequence.
- **Fast-IO transport**: FIFO ↔ main memory via IOFetch← / IOStore←
  munches. DSK microcode currently has to drain FIFO via Pd←Input.
- **Fire Code ECC**: polynomial P(X) implementation in software.
- **Real sector-pulse timing**: 117 subsector + 1 index pulses per
  16.66 ms revolution drives DSK task wakeups. Phase 3 needs a
  cycle-accurate simulation clock.
- **Block transitions per Format RAM**: the format-RAM-driven
  multi-block sector layout (1st block at offset N, gap of M words,
  2nd block at offset...) — currently we just dump header+label+data
  contiguously into the FIFO regardless of Format RAM contents.

See `docs/io-systems-architecture.md` for a higher-level view of
how disk fits into Slow I/O / Fast I/O / Tasking.
