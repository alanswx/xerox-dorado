# Native42 matched DDR control: boot in progress

Full FPGA load completed22:15:51UTC on MiSTer14. Final Main20496 is40e14b6b,
server20495 e237dffc, original seed f3bce2a8, pinned Pup time1792324800.
Candidate RBF45af2e30 passed29 summary/20 all-device path checks across four
conditions, minimum+0.115ns, no new cuts/illegal/unconstrained clocks.
Detailed frozen timing/source evidence is in the neighboring native42 report.

This control retains41's50MHz/SYSPER4 and logic/M10K placement and removes
the device confound from39's different placement/read cache: it uses raw
DDR with no line cache. New image SHA is checked unchanged after the
temporary loader/reexec Main stops and before final cache-ready/CoolBoot.
Main disk cache/trace and server match the measured fresh41c baseline.
**No hardware speed or boot qualification yet.**

Paths: Cedar42_20261018, _Computer/DoradoRTL_Cedar42_20261018/Cedar42.mgl;
Main `/tmp/dorado42-main.log`, disk `/tmp/dorado42-disk.jsonl`. Final Main
is the sole SPI owner. Setup uses Main-only framebuffer0 for core input/
video and instruction monitor view1. A new C watcher holds C until the new
BFN113; three actual CoolBoot pushes are requested afterward. Helpers and
exact runtime metadata are retained here; deployment is not blindly rerunnable.

Native41d has passed both cold marker reads and is retired/preserved.
After its normal guest Boot, the actual Main loader invokes cache durability
barriers before FPGA programming/restart. Temporary reexec inherits the old
O_EXCL trace path; it is normally stopped and final42 Main gets fresh RAM
log/observer paths. No guest state import or repair, no other board touched.
