# Direct DDR control integration candidate

Main remains the sole SPI owner. This milestone adds the opt-in DDR packet
ownership mux and explicit daemon STOP/drain/fresh-epoch/RESUME control;
it is not a deployed DDR system or a complete machine snapshot.

Actual C service/client/manager tests use guarded anonymous RAM and an
independent SPI ordering oracle. The Linux and dynamic ARM lifecycle runs
compile prologues from the actual pinned Main after all five patches, then
replace hardware mutation with a counter. Late STOP ACKs, disk failure and
physical drain prevent loader/reboot/reexec/init actions. Default/DDR-enabled
but ungranted daemon regression tests retain byte-exact boot and BSP recovery.
The static ARM test failed during test-thread joining; its log is retained,
its cause is undiagnosed, and only the production dynamic runtime passed.

The daemon reconnect test caught an actual old-owner EOF/accept ordering race.
The fix peeks EOF/quiesces before considering a replacement, while rejecting
a second live owner. Service tests and Mac ASan/UBSan pass after the change.
The older service-arm-test.log predates this fix; main-manager-arm-dynamic-
test.log links the final service and covers prompt reconnect on ARM.

The pinned Main and daemon build without new source warnings; daemon build
logs retain existing ethernet.c snprintf truncation warnings. These builds
remain isolated on misterubuntu. No running executable or live FPGA changed.
Native46 all-device timing is archived in the sibling native46 report.

Remaining gates: original-wire/engine/UIO combined test, physical reservation
and coherence, boot/STP bytes, original-firmware Cedar desktop/input,
reset/reload and packet progress with delayed Main disk work. Snapshot4a
additionally needs all guest/device/clock/disk/server state, not merely STOP.
