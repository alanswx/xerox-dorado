# AltoInfo/ — Alto emulators surveyed

The user dropped four Alto emulator artifacts into `AltoInfo/`. This is
what is in there and what we should lift, reference, or ignore.

```
AltoInfo/
├── ContrAlto-mono/         pre-built mono binary (older ContrAlto v1, ~2018)
├── Contralto2-2.0-Beta/    ★ SOURCE TREE for ContrAlto v2.0 (C# / .NET 8)
├── ContrAlto2-beta/        pre-built ContrAlto v2 binary (Avalonia .NET)
├── salto/                  ★ SOURCE TREE for Juergen Buchmueller's "salto" (C, GPL, ~2008)
└── salto-0.4.2.tar.gz      same source as a tarball
```

Two of the four are source trees. The two binary drops are useful for
running Alto images alongside our work to compare behavior; everything
worth lifting comes from the two source trees.

---

## TL;DR — what to lift

| What | Source | Why |
|---|---|---|
| **Trident T-80 / T-300 SMD pack image format** | `Contralto2-2.0-Beta/ContraltoLib/IO/DiskPack.cs` | The Dorado uses the same physical drive family. The Bitsavers-style image format (2 dummy bytes + 2 header words + 10 label words + 1024 data words = **2074 bytes/sector**, T-80 = 815×5×9, T-300 = 815×19×9) is the de-facto preservation standard. We can read and write the same images. |
| **Trident drive emulation logic** | `Contralto2-2.0-Beta/ContraltoLib/IO/TridentDrive.cs` (+ `TridentController.cs`, `TridentTask.cs`) | Seek, sector-clock, head/cylinder/sector indexing — all reusable. The *controller microcode protocol* is Alto-specific (the Dorado has its own DskEth controller); we only port the drive surface, not the controller. |
| **Sample Trident pack images** | `ContrAlto2-beta/Disks/spruce-server-t300.zip`, `spruce-server.dsk300` (276 MB), `spruce-server.dsk` | Real preserved Spruce print-server pack we can use to validate our Trident drive emulation byte-for-byte against ContrAlto2. |
| **3 Mbit Alto Ethernet** | `salto/src/ether.c`, `Contralto2-2.0-Beta/ContraltoLib/IO/EthernetController.cs` | Dorado supports both 3 Mb and 10 Mb Ethernet; 3 Mb is identical to Alto. Useful as a reference once we get to §11. |
| **Alto microcode listings (real microcode sources)** | `salto/docs/altoIIcode3.mu` (58 KB), `salto/docs/altocode24.mu` (50 KB), same files in `ContrAlto2/...Disassembly/` | Documentation of microcode patterns. Cross-reference when writing the Alto-emulator microcode for Dorado. |
| **Alto microengine structure** | `salto/include/cpu.h`, `salto/src/cpu.c`, `Contralto2-.../CPU/CPU.cs`, `MicroInstruction.cs` | Useful **architectural template** for a microcoded engine in C: how to lay out tasks, microinstruction decoding, branch prediction, hold logic. |
| **Disassembler output examples** | `salto/docs/cpu.txt` (10 KB Alto microinstruction decode table), `Contralto2-.../CPU/UCodeDisassembler.cs` | Reference for what a microcode disassembler looks like. Our Dorado disassembler will be a different format. |

## What is NOT reusable

- **Alto microinstruction format** (32-bit RSEL/ALUF/BS/F1/F2/T/L/NEXT)
  is not the Dorado's (34-bit RSTK/BSEL/ASEL/ALUF/LC/FF/JCN/BLOCK).
  Different decoders, different ALU model (Alto has fixed ALUF table,
  Dorado has programmable ALUFM).
- **Diablo disk** code (`salto/src/disk.c`, `drive.c`, `DiabloDrive.cs`)
  is for the Alto's removable Diablo 31/44 cartridges, not the Trident
  packs the Dorado uses. Skip.
- **Alto display** code (`salto/src/display.c`,
  `Contralto2-.../Display/`) targets the Alto's 606×808 monochrome
  framebuffer driven by the DWT/DHT/DVT tasks. The Dorado's DispM/DispY
  display works differently (own DDC chip, its own task model). Don't
  port; use as inspiration for the SDL framebuffer plumbing only.
- **Alto IO controllers** (Orbit raster image processor, Dover ROS,
  organ keyboard, audio DAC) — Alto-specific. Skip.
- **BCPL / Alto OS / Mesa-on-Alto** images. The Alto's Mesa
  ("MesaROM.mu") was running the Mesa-Alto runtime; the Dorado runs
  Mesa via dedicated microcode. Don't conflate them.

## Useful realisation: the Alto-emulator microcode for Dorado

The Dorado's `AltoMesaDorado.eb` (on CHM, see `chm-archive.md`) is a
microprogram that makes the *Dorado* present the *Alto* opcode set. Once
we can run it, ContrAlto2 and salto become **byte-level reference
oracles**: for any Alto opcode our Dorado-Alto emulator interprets,
ContrAlto2 must produce the same architectural state. That is a much
shorter feedback loop than waiting until Mesa boots.

So, dependency order with this in mind:

1. Microengine core + IM decoder.
2. Memory + Map + Pipe + cache.
3. IFU + tasking.
4. Alto-emulator microcode running → cross-validate against ContrAlto2.
5. Trident drive emulation (lift from ContrAlto2).
6. Display (DispM stub framebuffer).
7. Mesa microcode → boot Pilot.

## File-by-file notes

### `Contralto2-2.0-Beta/` (★ primary source reference)

Modern, clean C# / .NET 8. License: 2-clause-ish (see file headers — Living
Computers Museum + Josh Dersch, redistribution permitted with attribution).

- `ContraltoLib/CPU/`
  - `CPU.cs`, `MicroInstruction.cs`, `UCodeMemory.cs`, `ALU.cs`,
    `Shifter.cs`, `ConstantMemory.cs`, `UCodeDisassembler.cs`,
    `NovaDisassembler.cs` — everything CPU.
  - `Tasks/` — one file per Alto task (CursorTask, DiskTask,
    DisplayHorizontalTask, DisplayVerticalTask, DisplayWordTask,
    EmulatorTask 25 KB, EthernetTask, MemoryRefreshTask, OrbitTask,
    ParityTask, **TridentTask**, plus `Task.cs` (30 KB) base class).
- `ContraltoLib/IO/`
  - `DiskController.cs`, `DiabloDrive.cs`, `DiskPack.cs` — Diablo (Alto only)
  - **`TridentController.cs`** ("TriCon"), **`TridentDrive.cs`** — Trident T-80/T-300 drives ★
  - `EthernetController.cs`, `HostEthernetEncapsulation.cs`,
    `UDPEncapsulation.cs`, `IPacketEncapsulation.cs` — Ethernet,
    including how to bridge to a host network for inter-emulator traffic
  - `OrbitController.cs`, `DoverROS.cs`, `OrganKeyboard.cs`,
    `MouseAndKeyset.cs`, `Keyboard.cs`, `Printing/`, `AudioDAC.cs`,
    `IAudioSink.cs` — Alto-only IO
- `ContraltoLib/Memory/` — small (Memory.cs, MemoryBus.cs,
  IMemoryMappedDevice.cs). Alto-flat 64-KW memory; not a model for the
  Dorado's cache/Map.
- `ContraltoLib/Display/`, `Logging/`, `Scripting/` — UI and infra.
- `ContraltoLib/ROM/AltoI/`, `AltoII/` — Alto microcode and PROM dumps.
  Reference, not directly loadable.
- `ContraltoLib/Disassembly/altoIIcode3.mu`, `altocode24.mu` —
  human-readable Alto microcode listings (~2k lines each). Same content
  as in `salto/docs/`.
- `Contralto/Disks/` and `ContrAlto2-beta/Disks/` — eleven `.dsk` images
  (Diablo) plus **`spruce-server-t300.zip`, `spruce-server.dsk300`**
  (Trident T-300 pack, 276 MB). The Trident image is the one we care
  about.

### `salto/` (★ primary C reference)

GPL-2 C source by Juergen Buchmueller, ~2008. ~30 source files,
SDL-based. Builds with GCC + GNU make.

- `src/cpu.c` (63 KB) — Alto microengine core. **Read this for layout
  patterns** when starting our Dorado microengine in C.
- `src/emu.c` — Alto BCPL emulator microcode driver / dispatch.
- `src/memory.c` — Alto memory subsystem (no cache/Map; reference only).
- `src/disk.c` (53 KB) + `drive.c` (50 KB) — Diablo. Skip for Dorado, but
  the *driver-vs-drive split* is a good pattern.
- `src/ether.c` (25 KB) — 3 Mbit Ethernet (relevant when we get there).
- `src/display.c` (17 KB) — Alto display (skip; different from Dorado).
- `src/jkfflut.c` (22 KB) — JK flip-flop lookup tables (Alto-specific
  microcode side-effect tables; skip).
- `src/curt.c, dht.c, dvt.c, dwt.c, ksec.c, kwd.c, mrt.c, part.c` — one
  file per Alto microcode task. Naming convention is worth borrowing.
- `src/ram.c` — Alto microcode RAM (bank select, parity).
- `src/zcat.c`, `src/png.c`, `src/md5.c`, `src/zlib/` — utility helpers
  (zip-stream reader, screenshot output, hashing). Skip; pull modern
  equivalents instead.
- `docs/altocode24.mu`, `altoIIcode3.mu` — Alto microcode listings.
- `docs/cpu.txt` — exhaustive Alto microinstruction decode table.
- `docs/regusage.txt` — register-usage conventions in Alto microcode.
- `roms/` — 53 ROM/PROM dumps for Alto I and Alto II (control PROMs,
  ALU PROM, MADR address PROMs, display + Ethernet timing PROMs, XM bank
  PROMs). Alto-specific; the Dorado has its own ECL-era ROMs.
- `tools/` — assorted utility programs (didn't enumerate; secondary).
- `pics/` — screenshots; ignore.

### `ContrAlto-mono/` and `ContrAlto2-beta/` (binaries)

Pre-built ContrAlto v1 (mono) and v2 (Avalonia .NET). Ignore for
porting; useful as reference oracles when running same-image tests.

`ContrAlto-mono/Disassembly/MesaROM.mu` exists but is identical to the
ContrAlto2 file. The companion `ContrAlto2-beta/ROM/AltoII/` etc. ROMs
are the same content as `Contralto2-2.0-Beta/ContraltoLib/ROM/`.

## Licensing notes (skim before lifting code)

- **salto** — GPL v2 or later. If we copy code, our project becomes GPL.
  We probably don't want that — re-implement from the source rather than
  copying lines.
- **ContrAlto v2** — 3-clause-BSD-flavoured (see file headers: must
  retain notice, no use of contributor names to endorse). Compatible
  with whatever license we choose for our emulator. Can be lifted with
  attribution.

For the **Trident drive port specifically**, lift from ContrAlto2, not
salto (and not from `salto/src/drive.c`, which is Diablo anyway).

## Concrete next-step opportunities

1. Open `Contralto2-2.0-Beta/ContraltoLib/IO/TridentDrive.cs` and
   `TridentController.cs` and write down the disk image API surface we
   want in C: `trident_pack_open(path)`, `trident_pack_read_sector(pack,
   cyl, head, sec, header_buf, label_buf, data_buf)`, etc.
2. Clone the geometry constants verbatim. Start with T-80 (the Dorado's
   primary drive) and add T-300 later.
3. Use `spruce-server.dsk300` as the test fixture — it's already in
   `ContrAlto2-beta/Disks/` and is a known-good T-300 pack.
4. When we begin the IFU/tasking work, mirror the file split pattern
   from salto (one C file per microcode task) — good for navigation.
