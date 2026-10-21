# Ready/front hardware startup regression: reproduced twice

RBF57a552af passes the preceding full source/memory/timing/assembly review,
but **does not qualify Cedar startup**. Both original-firmware attempts request
BFN113 twice, consume microcode replies and enter the same Mesa transfer/trap
loop. Neither reaches login. Actual second display:
[HDMI capture](raw/field/boot2/startup/mesa-loop.png).

50MHz/SYSPER4 remains unchanged:6.25M ordinary instructions/s ceiling before
waits, against the16.667M/s real-time target. Last qualified pinned idle-login
measurement is the prior MC1668 core's1,947,686.01 issues/s. No new speed result,
interactive Cedar, rollback, persistence, three-boot or stress acceptance.

Main21e67a3f/serverc5996249 and all their policies remain. First Main15708/
server15703 are retired; second Main16808/server16807 remain live for diagnosis.
Main alone owns SPI; the daemon retains its explicit direct-DDR grant.
The first closed63,578,624-byte volume still hashes to the original f3bce2a8
seed. Its complete3862-record disk trace has no gaps/errors and two loader
barrier_ends. The earlier MC1668 mutated marker volume also remains preserved.
The earlier volume's historical gap is documented separately, not hidden here.

Passive diagnostics through Main show128 PC samples per attempt, dominated
by task0 SPCIL0/XFERMD/MTRAP/transfer paths. The first daemon has2 requests/
126 replies queued, last BFN0113. The receive observer reads132 received/
126 consumed, last wire/CPU status0. This does not verify every received word
or prove the CPU's packet-buffer writes correct. `FTP_UNSERVED type264` is
printed before the downstream microcode-boot handler; it does not establish
an unanswered request.

Main's RAM trace reveals three15872-byte read/ack pairs, offsets0/233984/
311808. First delays3.462/3.162/3.168ms; repeat3.996/3.162/3.168ms. No buffered
write/error/drop. The disk file alone initially contained only preload events:
normal cache-hit events were still buffered. The debugger copies the trace
without flushing it or changing guest/cache state. Diagnostic debugger pauses
are not normal Main polling-latency measurements.

The repeat initially used an overlong launcher name. Truncation missed its INI
section and selected stock Main16708. The guarded driver stopped; process/log
inspection identified the re-exec. Stock Main stopped normally, descriptors
closed, seed hash verified, and a journaled resume used short name
`DoradoRFrontB2_20261021`. No extra FPGA reload was issued. Preserve this setup
interruption; it is not a successful fresh-boot qualification. Both genuine
C watchers completed on new BFN113s and released their keys.

The first paired Ethernet-only component diagnostics both terminate1 at170M:
3600 IM+256 IFUM mismatches, Cedar entry238 not reached. Their Initial-ROM
checks still pass. This rejects that broader receive fixture rather than
isolating a regression in the new RTL. The dependent generic-I/O Pilot runs
were stopped deliberately (-15). Their inherited older bench ignored the
runtime-limit option; V3 installs an explicit observer control. V2 failed
compilation for missing shared native cells; its source/errors are retained.

V3 control85741/candidate86221 compile and run with the SAME actual deployed
DskEth board, shared native cells, inactive administration,10-word Pilot labels,
original disk seed and response stream. Frozen inputs for both are included.
At collection both exact process commands were verified live. Their logs are
**prefixes**, not terminal or passing results. RX shows initial broadcast
status0020 and subsequent microcode replies0000, so globally bad wire CRC is
not established. Next inspect the original receive filter/sequence/RAM path.
These remain direct-ROM-entry component fixtures, without native BaseBoard,
SDRAM or ARM services; they cannot qualify a hardware boot.

`archive-manifest.json.gz` binds all971 members:36,714,889 original bytes,
10,011,817 stored bytes, every member read back losslessly. It records exact
live/absent process observations. Sources/logs and large JSON are gzip;
small JSON, existing gzip and PNG retain their original representation.
No executables, RBFs, Quartus databases, active pack copies, C machine state
or host completion of guest instructions/IOCBs. New helper scripts are in
`raw/tools/`; exact earlier executed variants remain under field/simulation.

Next: establish a passing matched receive/germ control, find the first changed
CPU/storage behavior, qualify same-clock Cedar/input, then resume accurate
clock acceleration and authentic Cedar rollback. Goal ACTIVE; complete FPGA
snapshots remain deferred.

A subsequent `inspection/` extension binds the live V5 control90671: bounded
CPU Ethernet-read trace and read-only register/map inspection at60M, ending at
65M. Compilation passes. Its input set differs from V3 only in the bench and
observer helper; remaining inputs hash-identical. Ten additional members read
back losslessly. V4's preparation-only assertion failure is retained; it
checked top-module text instead of bench text. No V4 compile/run occurred.
Current repository helper includes this correction; the main archive retains
its exact earlier executed versions. V5 logs are running prefixes, not results.
