# Passive first-error observer evidence

Reviewed observer build and assembly pass; original-firmware Cedar boot and speedup do not. MiSTer14 captured CIA 0xD44 with both IM parity flags: the original BaseBoard ROM deliberately installs that breakpoint. The embedded instruction branches to 0xCD2, matching the stopped processor. This establishes a failed-boot stop; packet acceptance, checksum and raw-memory data integrity remain unresolved.

The content-addressed manifest preserves the full terminal fit/source receipt, timing review, ten rejected evidence controls, observer simulations, actual hardware first-fault capture, ROM table proof and original sources. Shared blobs reference the unchanged previous SDRAM burst archives. No FPGA binaries, live packs or imported machine state are included.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-boot-fault-observer`
