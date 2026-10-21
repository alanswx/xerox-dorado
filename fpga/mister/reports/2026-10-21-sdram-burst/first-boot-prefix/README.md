# First BL8 original-firmware boot prefix

The reviewed RBFf46eb4f5 is loaded on MiSTer14 with fresh seedf3bce2a8.
Unchanged Main9242/server9241 are observed live and detached from debuggers;
disk cache/direct-DDR grant are ready. Main retains sole SPI ownership.

The previous V6 pack/logs were preserved through normal Main barriers and
temporary Main exit. Their preservation record explicitly includes a
`trace_dropped` entry with `bytes=5792`; it is not loss-free trace evidence or
a clean guest shutdown. No active pack is copied into this receipt.

Normal keyboard C is held until a new original BFN113 request, then released.
All three normal boot-bit3 pulses show the actual BaseBoard pin asserted and
released. Both input jobs finish. The live log prefixes and process identities
are captured at the time recorded in `prefix/live-observation.json`.

The native screenshot is only a1024x4 raster. It contains no readable OS text.
Cedar entry/login/desktop, input correctness and instruction-speed improvement
are unqualified. Do not interpret the correct request as a completed boot
transfer, or restart this machine merely because a polling session expires.

14 members are losslessly preserved:

```
python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-sdram-burst/first-boot-prefix
```
