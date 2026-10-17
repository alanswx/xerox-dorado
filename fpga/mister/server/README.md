# ARM-local boot/file server

`dorado-server` reuses the C project's Pup microcode boot, Mayday/EFTP,
directory, time and STP server code. It links `ethernet.c` and the small I/O
router, **no CPU emulator or guest memory**. It does not access SPI.

The process/socket regression passes natively and on MiSTer14's ARM:
18,728 microcode payload bytes and 7,080 EFTP bytes match their source files;
dropping the first EFTP packet produces an identical retransmission.

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
stays in the packet. Server timers advance with monotonic time; alternating
breath-of-life and routing/time broadcasts are attempted once per second,
only when the underlying server is idle. They do not inspect guest memory.

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
