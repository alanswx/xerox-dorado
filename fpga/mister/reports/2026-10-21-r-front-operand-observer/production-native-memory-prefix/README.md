# Full production native memory: immutable running prefix

This receipt preserves 1,656 members / 530 unique blobs, with 473 blobs shared
from the parent. Lossless readback passes using
`python3 tools/archive_r_front_ready_followup.py --verify DIR`.
The manifest is an immutable prefix, not the latest running log or a terminal
test result. No compiled simulator, disk pack, FPGA image or debugger core is
included.

- V36 failed compilation because a harness identifier was a reserved SV keyword.
  No simulation ran.
- V37 compiled and completed physical SDRAM initialization, but was explicitly
  rejected and terminated because packet timers used enabled guest edges.
  The deployed daemon uses an independent host clock. Its rejection and terminal
  process result are preserved; this is not evidence of a guest fault.
- V38 corrects the host timer argument to `host_ticks * 20` ns and enables
  immediate log flushing. At collection driver79190 / simulator79583 were live.
  It uses all 513 frozen stage files and 171 actually-used source HDL files,
  original EPROMs, production eight-board SDRAM storage and normal device pins.
  The prefix shows storage zero-fill completed and original BaseBoard execution.
  It does not show a Cedar request, login, desktop or terminal diagnostic result.

The frozen packet-adapter checks pass for both compiled attempts: one captured
13-word hardware113 request produces 63 reply frames and 31,376 bytes identical
to the original Cedar EB payload. A separate link-symbol check excludes the C
CPU/machine/memory emulator. Full Main/Linux/direct-DDR transport is absent.
Block writes are volatile. Periodic daemon STP probes are not implemented here.
Progress `guest` counts enabled fabric edges, not microinstructions or MIPS.

The hardware video check records accepted CLI OSD commands and a black capture;
OSD visibility was not independently established. A normal Main command changed
live HDMI from 1080p30 to progressive1080p60; capture remained black. The previous
1080p30 mode was restored and both applications are confirmed in Main's log.
No persistent INI, production core, Main or daemon change resulted. Other MiSTers
and the shared capture service were untouched. Native capture still shows812.

Firmware correctness, interactive desktop, speedup, persistence, authentic FPGA
rollback and final goal qualification remain open. See the current handoff for
later observations; this prefix does not supersede terminal evidence.
