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

Original BaseBoard CoolBoot and C watcher21086 COMPLETED: fresh BFN113 and
immediate C release. Pilot and real Cedar login pass, shown in boot-progress.png.
A second GDB control selects the same issue view before timing. Three adjacent
10s windows average1,947,687.412 issues/s: +14.9453% versus48, +22.0639% versus
clean47b and11.6861% of nominal16.667M/s. All three means are ONE fresh boot
each; adjacent windows are not independent boots or end-to-end qualification.

After timing, read-only loader observer6672/Mac47764 uses the same policy as48.
It has now COMPLETED after the visible profile prompt; no reader remains.
The list plus33 BCDs,1,345,843 source bytes and exact final guest ACK qualify
312.55875..312.87163s, versus48's373.07180..373.32199s: preliminary16.136..
16.276% less time (19.241..19.441% rate gain), ONE boot each. All34 actual
remote source hashes match AFTER timing; this is not physical payload hashing.
The capture shows every Load ok. Profile No, BootEssentials/fonts/FontMetrics
and module STARTs complete. Full Viewers Ready, WatchTool/EditTool/Clock and
the first profile CommandTool paint. Real mouse motion/Cmd click/focus open
a ready independent second tool; exact short/alphabet/digit echo and new
prompts pass. Guest rtl49persist.txt create/close/readback and normal guest
Boot/germ812 pass. This marker has not been tested after a cold reload.
See desktop-input.json and captures. Native49 is now retired normally through
the50 loader; preserved volume/actual disk barriers are in native50-field.
Do not restart completed boot/watchers. Current helpers: build/rtl-executive/cedar49-launch;
Main /tmp/dorado49-main.log, disk /tmp/dorado49-disk.jsonl. Gains cover idle
login and the bounded base-loader sequence; no full-desktop speed, complete
snapshot or final repeated/stability qualification is claimed.
