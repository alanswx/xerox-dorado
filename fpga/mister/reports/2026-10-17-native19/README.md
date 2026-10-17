# Native19 qualification and FPGA test

RBF SHA256: `57bb42028e3441d0d82c972e7e864e4280cf513e15b44c40928c2abba1185de8`.
Build-machine, local, and MiSTer14 copies match. The functional source and
boot evidence are in `../2026-10-17-map-clock-leg/`.

Quartus17.0.2 Lite fitting passes (25m20s). Setup is +2.776ns for the core,
+4.616ns for HDMI. Before review, slow hold fails by0.050ns on two paths,
both MemX e20's F10145A a_lat[1] to that same RAM's two observable write
address bit1 endpoints. Fast hold passes (minimum+0.007ns).

The cell captures a_lat only while !p13, and commits mem[a_lat] only while
p13&&!we_d&&ce_lat. Same-edge address capture/write is impossible. The exact
one-source/two-sink hold-only exception in reviewed.sdc asserts those
collection sizes. Setup, reads, data, enables and other transfers remain
timed. Preserve both complete before-review hold reports and sta-before.
The RBF did not change. Repeated full STA and clock audit pass29 checks,
minimum+0.128ns, with zero illegal/unconstrained internal clocks. Framework
I/O delays remain separate (3 input/50 output ports without delay coverage).

Loaded on allocated MiSTer14 about15:29 UTC with its own fresh Cedar19 pack
(baseline SHA256 c0cd6784...), fixed720p60 profile and unchanged immutable
Main/server. The Native18 log was copied to main-before-native19.log, then
the open log moved to Cedar19/main.log before loading. Three real boot
pushes requested original BaseBoard CoolBoot once. The C-key watcher holds
the boot-selection key until a new BFN113 request, then releases it.
Firmware startup is in progress; a Cedar OS screen is still required.

Actual result: the original Cedar download succeeds, but the FPGA returns
to RequeueSearchSQ239/23a/6c1/6c2 with a white raster and tiny cursor. A
read-only DDR snapshot now contains the corrected germ roots06ec/09d0/09e0;
the broader decoder/cache view remains unqualified. A user-suggested Linux
reboot at15:38 and proper core reload/boot at15:42 reproduce the queue.
Current log is Cedar19/main-after-linux-reboot.log. Main/server binaries,
arguments, cwd and Dorado environment were restored unchanged. No Cedar OS
boot is claimed. The passing component used one module and square board
clocks; the FPGA has four modules and the measured BaseBoard pulse shape
(one tick low/three high at SYSPER4). Separate runs now isolate those two
differences. Preserve the authentic BaseBoard pulse model while diagnosing.
