---
gap: F1, F2, F3, F4, F5, F6
status: research
---

# F-series — Disk Phase 3 source sweep

## What got pulled

Three new authoritative sources from
`_cd8_/doradomicrocode/doradomicrocodesources/`:

| File | Size | Purpose |
|---|---|---|
| `DiskDefs.mc` | 161 lines | Canonical disk-controller register/bit definitions |
| `DiskSubrs.mc` | 322 lines | Shared disk subroutines (WaitForSector, UpdateSector, SendTag, ReadECC, ClearDisk, …) |
| `DiskBootSoft.mc` | 122 lines | Software-side boot loader (loads emulator from disk) |
| `DiskBootTransfer.mc` | 113 lines | Transfer routine called from `DiskHardMicrocodeBoot` |

Combined with the pre-existing `PilotDisk.mc`, `PilotDiskDefs.mc`,
and `InitialDisk.mc`, we now have the entire disk-microcode call
graph end-to-end.

## Canonical wire formats from DiskDefs.mc

```
* Muffler addresses (Output to DiskMuff selects which signal Input returns):
  muffIndexTW    = 1
  muffSectorTW   = 2
  muffSeekTagTW  = 3
  muffRdFifoTW   = 4
  muffWrFifoTW   = 5
  muffEnableRun  = 010 = 8
  muffActive     = 015
  muffsState/Status/Ram/Tag/Fifo at 0/20/40/60/100 base

* DiskMuff output bits (clears on write):
  clearCompareErr  = 0o20000 = LSB bit 13
  setCheckSumErr   = 0o10000 = LSB bit 12
  clearIndexTW     = 0o4000  = LSB bit 11
  clearSectorTW    = 0o2000  = LSB bit 10
  clearSeekTagTW   = 0o1000  = LSB bit 9
  clearErrors      = 0o400   = LSB bit 8
  muffAddr         = 0o377   = bits 7..0 (the address selector)
  clearAllTWs      = clearIndexTW | clearSectorTW | clearSeekTagTW = 0xE00
  clearAll         = clearAllTWs | clearErrors                     = 0xF00

* DiskControl output bits:
  clearEnableRun   = 0o2000  = LSB bit 10
  debugMode        = 0o1000  = LSB bit 9
  blockTilIndex    = 0o400   = LSB bit 8
  commands         = bits 7..0 (4 × 2-bit blocks: write/check/read)

* DiskTag output groups (high nibble selects tag type):
  tagDrive         = 0o100000 (bit 15)
  tagCylinder      = 0o40000  (bit 14)
  tagHead          = 0o20000  (bit 13)
  tagControl       = 0o10000  (bit 12)
  tagBus           = 0o7777   (bits 11..0)

* Control-tag bits within tagBus (when tagControl is high):
  tagAltoLeader    = 0o4000   (bit 11)
  tagWrite         = 0o200    (bit 7)
  tagRead          = 0o100    (bit 6)
  tagDiskReset     = 0o10     (bit 3)
  tagHeadSelect    = 0o4      (bit 2)
  tagReZero        = 0o2      (bit 1)
  tagHeadAdvance   = 0o1      (bit 0)
```

## Discrepancy noticed: DiskMuff output convention

Our `dorado_disk_controller`'s DiskMuff output handler (in
`src/disk.c::disk_output_b` case `DORADO_DISK_TIOA_DISKMUFF`) is
written to expect:

- Muffler address in the **high** byte (= bits 8..15).
- Clear-TW bits at LSB bits **0..3** (low nibble).

The DiskDefs.mc spec is the opposite:

- Muffler address in the **low** byte (bits 7..0; `muffAddr = 0o377`).
- Clear-TW bits at LSB bits **8..11** (`clearIndexTW = 0o4000`,
  `clearSectorTW = 0o2000`, `clearSeekTagTW = 0o1000`,
  `clearErrors = 0o400`).

`tests/test_disk.c::test_diskmuff_address_select` is written
against the *original* convention (`010 << 8` to select muffler 010).
Switching to the spec's convention breaks the test.

Possible explanations:

1. **The compiler picks BSEL=FF,,0 form for muffler-address constants**,
   so values like `muffSectorTW = 2` end up at `0x0200` on the bus.
   Our `(data >> 8)` decode reads the address from there. If true,
   then the OLD code is consistent with what real microcode does,
   even though the spec literal says "low byte."
2. **The constants `clearIndexTW`/etc. likewise get loaded via
   BSEL=FF,,0** — but they're 16-bit (e.g., `0xE00` for
   `clearAllTWs`), so they don't fit in a single byte. The microcode
   would need a multi-instruction load (RM/STK init + OR), and the
   final bus value would have the bits exactly where `DiskDefs.mc`
   says.
3. The OLD code is wrong but happens to produce zero results that
   don't break the boot probe (because clear-TW operations never
   actually fire, so SectorTW stays asserted, allowing UpdateSector
   to keep counting up — even if it overshoots).

The handoff says PilotDisk reaches `KCheckSeek` and falls into
`Read1Muff` (= the failure return of `WaitForSector`). With the OLD
convention, `clearSectorTW` would never fire, but `sector_tw` stays
asserted, so UpdateSector increments Sector on every call. The
microcode in WaitForSector then races past the target sector. After
64 iterations the loop times out — exactly what we observe.

## Where the subroutines live

Several PilotDisk.mc references were unresolved in the
`bootstrap.dm/Initial.mb` that ships with our archive — the
boot-stage Initial uses `InitialDisk.mc` which only does the high
level `BootTransfer` flow. The shared subroutines that PilotDisk.mc
calls (`WaitForSector`, `UpdateSector`, `SendTag`,
`SeekAndWaitForReady`, `DoMuffOutput`, `OutputGetsT`,
`ReadECC`, `ClearDisk`, `Read1Muff`, `SendDriveTag`) all live in
**`DiskSubrs.mc`** — newly pulled.

## Plan

The right fix for F5/F6 (status readout + tag decode unification)
is to:

1. **Per-cycle trace** one full `WaitForSector` iteration in
   `probe_full_boot_with_bootstrap`. Capture every `Output_ T` and
   `Input` on TIOA[DiskMuff] / TIOA[DiskControl] / TIOA[DiskTag],
   along with T and the resulting muff_addr / clear behavior.
2. Compare to what `DiskSubrs.mc::WaitForSector` and
   `UpdateSector` actually do.
3. Reconcile the wire format: either fix our cpu.c to push spec-shape
   constants, or fix our disk.c to consume the actually-observed
   convention. Update `test_disk.c` to match whichever wins.

For F1 (sequence PROM) and F2 (Fire ECC), the canonical reference is
the format-RAM contents loaded by `InitRamPilot` (PilotDisk.mc lines
579–614). They define block lengths, tag commands, and read/write
delays — these drive the sequence-PROM-style state machine.

For F3 (write side) and F4 (sector-pulse timing), the reference is
the loops in `KCmmdWrite` (line 426) and the sector-cadence math in
`PilotDiskDefs.mc` ("Drive 0 is assumed to be jumpered for 117
sectors" with sub-sector dividers).

## Pre-conditions / dependencies

- F1-F2 depend on the format-RAM-driven sequencer being modeled.
  Currently the Read tag short-circuits and dumps header+label
  contiguously into the FIFO (Phase 2 stub).
- F3 (write) is gated on getting a working read first.
- F4 timing is partially modeled (synthetic sector pulses driven by
  the probe), but the actual cycle counts come from the Format RAM
  values written by `InitRamPilot`, which we currently just record
  without acting on.
- F5/F6 — the wire-format reconciliation discussed above is the
  immediate blocker.

Because the F-series sub-gaps depend on each other, the natural
order is F5 (status readout) → F6 (tag unification) → F4 (timing)
→ F1 (sequence PROM) → F2 (ECC) → F3 (write).
