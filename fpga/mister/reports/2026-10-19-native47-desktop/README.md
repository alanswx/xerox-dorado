# Native47: physical DDR network to interactive Cedar

MiSTer14 passes the original firmware boot to Viewers Ready and an interactive
second CommandTool. All34 base loadees, profile No, BootEssentials, TiogaFonts,
FontMetrics and package STARTs complete. The first tool continues the original
seed profile's optional application Bringovers. Real mouse motion/Cmd click
opens the second tool; a click focuses its idle body. Both commands print
their exact text and return to a new `%` prompt:

```
echo rtl47 keyboard ok
echo abcdefghijklmnopqrstuvwxyz0123456789
```

Input uses the actual virtual USB path,0.2s key-down and0.2s key-up cadence.
The immediate capture can precede the final guest keyboard drain; the settled
capture contains both output lines and the returned prompt. Mouse ends near
native737,608. Core video/input0 remains selected. No guest instruction,
RAM/IOCB or server cursor is patched, and no simulation state is imported.

The full FPGA load at23:47:39UTC18Oct used original f3bce2a8 seed checked after
the temporary Main exited and before final cache-ready. Current Main1509 is
21e67a3f; server1506 isc5996249. RBF3df4a799 runs50MHz/SYSPER4 with SDRAM/no
raw cache and no43/44/45 request optimizations. Direct DDR uses the actual
daemon64KiB mapping at1FF00000; Main remains the sole SPI owner and does not
map packet payloads. Frozen timing/deployment/idle evidence is in
`../2026-10-18-native47-drained-state/`. Actual Main delayed-worker/disk/DDR
packet progress evidence is in `../2026-10-18-native47-worker-ddr/`.

Latest observed counters:2388 good packets,24 TX drops, zero CRC/RX/abort
faults. TX drops are not zero and remain part of the qualification record.
Three known GDB setup/probe pauses are excluded from host-latency claims.
The qualification DSO `/tmp/dorado47-loader-probe.so` remains resident in
Main1509 until process exit; do not unload it. Use a fresh Main for later
clean performance comparisons. Saved Main/server prefixes are observational
copies while the machine continues running.

The complete disk-trace prefix contains93282 records:2805 read ACKs and17577
write ACKs, no file/barrier error or trace loss. It contains six recorded
poll gaps above20ms, including the documented debugger setup/probe activity;
this whole-prefix observation is not a host-latency qualification. The separate
controlled worker window has its own conservative timing/oracles.

This qualifies original-firmware desktop and input over physical DDR.
Independent physical payload/file byte hashes, active-DDR STOP/reload,
completed input during delayed worker work and sustained/final repeated
qualification remain open. Full FPGA snapshots are not implemented; the
separate snapshot RAM-bank prototype is not in this bitstream. Before the
prepared active-STOP test, use normal guest Boot and visibly reach germ812;
never reset the local server's live session in the middle of this boot.
