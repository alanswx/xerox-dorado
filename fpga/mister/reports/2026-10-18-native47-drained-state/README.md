# Native47 drained-state correction: timing PASS, hardware in progress

The isolated build freezes Native46 architectural/CPU/storage sources and
constraints and changes only dorado_ethernet_ddr.sv: stopped is decoded from
the drained FSM state rather than a redundant flag. Standalone and production
wrapper/wire tests and all failure controls pass; see sibling DDR drained-state
and wire integration reports. Native46 startup refused the disabled/not-stopped
indication; no daemon physical mapping or RESUME occurred before board recovery.

The builder includes four-condition setup/hold/recovery/removal, SDRAM I/O,
minimum pulse width and clock coverage audits. Its collector requires31 summary
checks,36 detailed categories, all nonnegative slack, zero illegal/unconstrained
clocks and exact source hashes. All31/36/four-condition checks PASS, minimum+0.111ns; zero illegal/
unconstrained clocks and no added cuts.30,480 ALMs (73%),52,639 registers,
330 RAM blocks. RBF3df4a799. Actual startup/grant PASS: normal temporary Main exit, fresh epochfdfe0a77fc6c87b5,
daemon-only64KiB physical mapping, FPGA flags0x16/fault+errors0. Final Main1509
and server1506 received original BFN113 over real DDR; watcher releasedC.
Pilot850 VM initialized is visible. Full Cedar, active STOP/reload and stress
remain pending. Exact deployment/capability/capture evidence is included.

Actual final daemon c5996249 also passes boot/Cedar/STP/quiet/reexec/loss/wrap/
history-ring byte regressions on MiSTer14 ARM, with DDR control enabled but
ungranted. This uses Unix sockets and is not physical DDR qualification.

Deploy-from-menu is a one-shot helper: it verifies stock Main/MENU, candidate
hashes/timing, immutable Main21e6/serverc599 and fresh seedf3bce2a8; uses the
normal loader, stops its temporary Main normally, rechecks the untouched seed,
then starts final processes with cache/DDR enabled and requires their real grant.
The helper deliberately refuses reruns over an existing preparation. Inspect
actual processes/metadata on failure. Only MiSTer14 is authorized.

Original firmware has now reached Cedar login; Guest.pa accepted via the
normal keyboard and Basic.Loadees retrieval begun. Idle login averages
1,595,641 qualified issues/s (9.57385% of original nominal cadence), one
boot/three adjacent windows. This matches the SDRAM baseline closely but
uses different Main/server/network, so it is not a matched performance or
three-boot gate. Full desktop/active STOP/stress remain pending.
