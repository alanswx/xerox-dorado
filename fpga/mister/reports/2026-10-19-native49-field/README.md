# Native49 physical interleaving: fresh hardware boot

Normal active-lease Main loader completed02:41:13UTC19Oct. Old Main3781 and
daemon3780 retired normally; temporary Main6399 exited normally, and the new
f3bce2a8 seed remained unchanged before caching. Actual old disk/network
barriers precede FPGA programming. The old disk trace ends barrier_end,
without file errors or trace loss. Native48's63578624-byte pack remains at
its original path, SHA f4548e6c53382aeb37e28e458a8c43dfc9dc77592f2208b63059733f2ffb2855.
Normal guest Boot had returned to Basic loader/profile; no germ812 capture.

Fresh Main6414 is21e67a3f, daemon6413 isc5996249. RBF6e7866f3 runs50MHz/
SYSPER4, SDRAM/no raw cache/direct DDR. Only physical group/word interleaving
differs from48; response-boundary arbitration remains enabled, deferred reads
disabled. Native49 timing31/36/four conditions passes, minimum+0.115ns.

Actual Main-owned SPI74 reports flags0x16, fault/errors0, different fresh
epoch5cea2e754bd0665f. Daemon maps exactly64KiB at1FF00000; Main does not map
that ring. Main's existing FPGA/framebuffer mappings remain expected.
One setup GDB pause selects core video/input0 and issue view1, and checks
capability/epoch with the matching ELF. No probe DSO in the new Main.

Original BaseBoard CoolBoot pulses completed; C watcher21086 waits for a
fresh BFN113 then releasesC. Do not restart the watcher or boot pulses on an
observation timeout. Original-firmware login, matched idle speed and full
desktop/input remain pending. Current helpers: build/rtl-executive/cedar49-launch;
Main /tmp/dorado49-main.log, disk /tmp/dorado49-disk.jsonl. No speed gain or
complete snapshot claim is made for this candidate yet.
