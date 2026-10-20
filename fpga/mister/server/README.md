# ARM-local boot/file server

`dorado-server` reuses the C project's Pup microcode boot, Mayday/EFTP,
directory, time and STP server code. It links `ethernet.c` and the small I/O
router, **no CPU emulator or guest memory**. It does not access SPI.

The process/socket regression passes natively and on MiSTer14's ARM:
18,728 microcode payload bytes and 7,080 EFTP bytes match their source files;
dropping the first EFTP packet produces an identical retransmission.

Direct DDR transport is integrated and qualified on the Native59 baseline.
The daemon exclusively maps the reserved 64 KiB aperture at 0x1ff00000 after
Main grants a validated epoch; Main remains the sole SPI owner. The bounded
ring and RTL engine pass stall/full/wrap/reset and2048-byte payload tests,
including ARM endpoint tests. `DORADO_NET_DDR=1` enables the control listener;
the socket path remains available when no DDR grant is active. The daemon
never maps guest memory or completes guest instructions. Protocol and commands:
[dorado-ddr-protocol.md](../dorado-ddr-protocol.md).

```sh
make -C fpga/mister/server test
# On Linux with the ARM cross compiler:
make -C fpga/mister/server CC=arm-linux-gnueabihf-gcc \
  LDFLAGS='-static -Wl,--gc-sections' OUT=/tmp/dorado-server-arm
```

Run with absolute paths (the socket directory must already exist):

```sh
dorado-server /tmp/dorado-net.sock /path/aemu.eb /path/NETEXEC.BOOT /path/stp
```

The final STP-root argument is optional. Explicitly configured microcode
BFN `0110B` and the default EFTP file are currently exposed; the other
microcode BFNs still have the C project's relative defaults. Only the two
configured files are checked at startup. For Cedar, launch with cwd at the
staged `dorado` directory so BFN113 resolves to `../chm/dorado/CedarDorado.eb!6`.
`make cedar-test` qualifies the captured Cedar socket-2 routing probe with
STP enabled, that mapping, and an actual BSP/STP retrieve:
31,376 microcode bytes in 62 frames and the 66,296-byte CedarSource.VersionMap
in 45 data frames, including 1,504-byte frames. Payloads match their files
byte-for-byte on both Mac and the MiSTer ARM. These are server/transport
tests; they do not qualify a Cedar FPGA boot.

Transport is local `AF_UNIX/SOCK_DGRAM`, with one Main client binding its
own pathname and sending a zero-length registration datagram. Each later
datagram is one raw Ethernet frame: big-endian 16-bit words starting with
destination/source and EtherType, **without Manchester preamble, hardware
CRC, or receiver status**. Incoming frames are limited to the controller
server's 1024-word transmit buffer. Both socket paths must be unique; the
server refuses to replace an existing pathname. Its socket is owner-only.
After the first client registers, other peers are ignored until restart.
If that client's socket disappears or refuses delivery, the server cancels
its old transfers and accepts a fresh registration. Main also disconnects
on core initialization, including same-name Dorado reloads.

The adapter strips the C controller queue's dummy CRC and attention/status
entries. It adds the missing nil Pup checksum to that code's old microcode-
boot packets, whose length already includes it. EFTP's actual Pup checksum
stays in the packet. Server timers advance with monotonic service time, excluding
explicit snapshot pauses; alternating
breath-of-life and routing/time broadcasts are attempted once per second,
only when the underlying server is idle. After a Cedar BFN113 request,
unsolicited boot/routing broadcasts stop: Cedar's routing initializer needs
a quiet receive interval to finish. Solicited replies and BSP recovery
probes continue. A replacement Main resets this policy with the rest of
the server session. `make cedar-test` checks both quiet and reset behavior;
this preserves the policy of the previously qualified1d5e05e9 daemon.
The server does not inspect guest memory.

STP retains up to 64 unacknowledged stream packets per connection, including
property lists and completion marks. Retransmission has its own cursor;
the producer's sequence and file position stay at their high-water mark.
This prevents a delayed forward ACK during replay from underflowing the byte
window and wedging the next retrieve (Native41's AMEvents-to-PrintTV stall).
Old ACKs cannot move the committed cursor backwards, and ACKs for unsent
bytes are ignored. History is retired by ACK, reset on context reuse, and
released on service reset/free. Native and ARM tests cover lost metadata,
data and tails, the delayed-ACK race across sequence wrap, a 200,960-byte
transfer across history-ring wrap, and closed-context reuse:

```sh
make -C fpga/mister/server bsp-retry-test bsp-ack-race-test
```

History is host-side state outside the legacy C snapshot ABI. Legacy restore
discards later live history; full mid-transfer restore requires serializing
it in the planned complete-machine snapshot bundle (milestone 4a). A daemon
restart does not preserve live BSP connections.

Snapshot pause is a separate control operation from lifecycle STOP/RESUME.
Commands 4/5 are PAUSE/CONTINUE for the existing epoch and fixed aperture. A
successful PAUSE reply has flags 3 (active grant plus paused); CONTINUE has
flags 1 and must match the same epoch. Duplicate requests do not remap or repeat
lifecycle transitions. Paused STATUS and ring calls make no DDR accesses.
CONTINUE validates the retained header/cursors and rejects stale/invalid state.
STOP or controller EOF still revokes the grant and requires the normal lifecycle
barrier; neither is a snapshot continuation.

While paused, the sole daemon thread services control without pumping timers,
consuming/publishing frames, probing connections or resetting Pup state. Its
local retry clock excludes paused wall time, while Pup wall-clock replies keep
their original meaning. Main's control client checks the exact pause/continue
flags and epoch. The full Main snapshot coordinator, FPGA/raw-service draining,
heap/history export/import and disk-generation pairing are still pending; this
daemon ACK alone is not a whole-machine snapshot barrier. The new pause feature
is host-qualified and is not yet deployed on MiSTer.

```sh
make -C fpga/mister/server ddr-control-test service-clock-test snapshot-pause-test
make -C fpga/mister/main control-test
python3 fpga/mister/server/test_snapshot_pause_mutations.py --output /tmp/dorado-pause-proof
```

The real process test pauses pending EFTP data/ACKs and a BSP/STP retrieve, then
requires all 7,080 boot bytes and 66,296 file bytes to continue exactly. Guarded
memory tests forbid MMIO while paused; broken lifecycle reset, timer and MMIO
variants reject. The mapper fixture is link-only and absent from production.
These tests neither serialize a server checkpoint nor restore an FPGA.

The MiSTer Main/RTL packet bridge now delivers actual guest TX packets to
this service on MiSTer14. Main owns SPI and Dorado-specific UIO queues; RTL
serializes Manchester data and checks hardware CRC through the Sil-derived
controller. Full supported Pup reception and network boot remain under test.
The service never supplies a controller status word or accesses SPI.

Cedar's cold routing probe uses Pup type `0200` on socket 2. File lookup
uses the same type on file-service sockets, including socket 3 through our
NetDir alias. Dispatch must respect the socket: treating every type `0200`
as a filename silently drops the empty routing probe. The regression sends
the captured request with a unique ID so idle routing broadcasts cannot pass.

The daemon now sends at most one empty BSP acknowledgement probe per second,
rotating through open STP connections with unacknowledged output. This is
`PupStreamImpl.SendProbe`'s aData/pushId/zero-byte packet, with the full
client socket address. It spends no stream bytes or output fingers. Duplicate
acknowledgements invoke the existing file/completion-tail rewind logic.
Probe timers belong to the ARM service; Main still owns SPI, and the FPGA
still supplies Manchester framing, CRC and controller status.

The observed FPGA failure was a lost20-byte Helvetica7 transfer-completion
tail after all font bytes arrived. With no later acknowledgement-requesting
packet, Cedar and the server waited indefinitely at the gray Viewers screen.
Protocol probes elicited the stalled byte cursor; duplicate ACK replayed the
tail and the desktop resumed. Probes elicit the stalled byte cursor; the history
ring described above supplies the metadata/data/completion replay. This makes
recovery possible when an unrequested final packet is lost.

`make bsp-retry-test` drops that tail, supplies ACKs only when BSP requests
them, and verifies recovery plus all1036 file bytes. It passes natively and
on MiSTer's ARM. An independently compiled old-loop control receives the
body but fails with zero probes. The service adds no machine-state members
to the shared C controller structure, preserving its snapshot ABI. Complete
FPGA snapshot bundles will also need the daemon's probe timer/cursor state.
