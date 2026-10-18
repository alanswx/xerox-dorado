# Native42 matched DDR control: interactive Cedar desktop

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
Original firmware -> all34 loadees -> fonts -> Viewers Ready succeeds.
Mouse motion/click opens an independent second CommandTool. `echo rtl42
keyboard ok` and `echo abcdefghijklmnopqrstuvwxyz0123456789` print
the exact text and return to `%`. Captures and real
virtual-USB input logs are retained. The first CommandTool continues the
seed profile's optional application Bringovers; test typing uses only the
independent second tool.

Three adjacent10s idle-login windows before Guest average1,447,771 qualified
StartCycle issues/s,8.6866% of nominal60ns ordinary cadence. Matched SDRAM41c
is1,595,727 issues/s,10.2196% faster in this ONE boot comparison. Adjacent
windows are not three independent boots or drained retirement. Separate
cause views show76.7900% storage pause,23.2095% enabled, zero pack pause;
they are not one atomic attribution. Pinned loader/desktop performance and
repeated paired comparisons remain required.

Paths: Cedar42_20261018, _Computer/DoradoRTL_Cedar42_20261018/Cedar42.mgl;
Main `/tmp/dorado42-main.log`, disk `/tmp/dorado42-disk.jsonl`. Final Main
is the sole SPI owner. Setup uses Main-only framebuffer0 for core input/
video and instruction monitor view1. The C watcher and
three CoolBoot pushes completed; C released at new BFN113. Do not restart
the watcher. Thirteen known Main GDB control pauses precede Guest; exclude
those from poll-latency claims. Helpers and
exact runtime metadata are retained here; deployment is not blindly rerunnable.

Native41d has passed both cold marker reads and is retired/preserved.
After its normal guest Boot, the actual Main loader invokes cache durability
barriers before FPGA programming/restart. Temporary reexec inherits the old
O_EXCL trace path; it is normally stopped and final42 Main gets fresh RAM
log/observer paths. No guest state import or repair, no other board touched.
