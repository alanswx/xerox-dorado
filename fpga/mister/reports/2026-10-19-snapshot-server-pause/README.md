# Connection-preserving daemon pause: Mac and Linux host qualification

PAUSE/CONTINUE commands 4/5 retain the existing epoch, mapping, queue cursors
and Pup connections. Paused ring calls and STATUS make no MMIO accesses.
CONTINUE validates header, own cursors and window geometry before use. The
single server thread continues control service while packet pumping, broadcasts,
probes and retry time are paused. STOP/EOF retain lifecycle revocation semantics.
The Main control client validates exact flags and epoch; no Main coordinator
or FPGA pause integration is included.

The passing isolated Mac v3 and Linux v3 runs execute the exact same 86 source/
data inputs. Seven gates pass: ring/control/clock/map; legacy server with real
boot/STP/retry/ACK-race traffic; fragmented Main client; actual Main lifecycle
manager with guarded memory/SPI oracle; Main transport build; real process
transport; pause mutations. Pending EFTP continues all 7,080 bytes; a paused
BSP retrieve continues all 66,296 bytes. Broken connection-reset and retry-clock
variants reject; unguarded paused MMIO faults against protected memory.

Linux attempts 1/2 are preserved as failed TEST compiles (GCC signedness in the
existing transport test; strict ISO C hiding POSIX/GNU server declarations).
The test fixes are an unsigned comparison and matching GNU C compile mode.
The production server is not changed to accommodate the test compiler.

The baseline receipt checks all 56 original files against the actual deployed
source closure behind server c59962490772f12d. The archive contains raw executed
source/data/commands/logs and terminal process receipts; ELF/Mach-O executables
and objects are excluded. All 874 members read back losslessly. Exact archived
input/log hashes, Mac/Linux identity and candidate/mutation outcomes recheck.

This is HOST qualification only. No MiSTer Main/daemon deployment, real FPGA/
raw-memory drain, ARM32 runtime test, serialized heap/history export, paired
disk generation or complete machine snapshot restore is claimed.
