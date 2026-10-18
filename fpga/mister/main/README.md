# Main's Dorado packet transport

This is a small hook in Main's existing polling loop. Main retains exclusive
SPI ownership and moves raw packets between the Dorado UIO extension and
the ARM-local Unix-datagram server. It does not execute guest code or
implement Pup, EFTP, STP, controller registers or receiver status.

The isolated build is based on **the installed Main release 20260912**,
commit `47221c18987e101f50caafeb3b615f53b62722ca`. Existing build-machine
checkouts include much older and newer versions; neither should silently
replace that release. The integration patch adds an include, a poll call
in `user_io_poll()` and a reset call in `user_io_init()`. The reset covers
same-name reloads so pending packets cannot cross machines. Main's wildcard
build picks up `dorado_net.cpp`.

```sh
# In an isolated checkout at the pinned Main revision:
git apply /path/to/main-260912.patch
cp /path/to/dorado_net.{cpp,h} .
# Use Main's GCC 10.2.1 ARM toolchain and existing Makefile.
make -j4
```

A temporary modified Main runs from `/tmp/dorado-main/MiSTer` on MiSTer14.
The stock `/media/fat/MiSTer` binary is unchanged. The isolated build is
`misterubuntu:/home/alans/mister/Main_DoradoNet_20261017/bin/MiSTer`.
Transport tests pass natively and on MiSTer's ARM: the real server returns
18,728 microcode bytes in 38 frames while the fake SPI receiver is held
busy; bytes and order match and TX is acknowledged once per accepted frame.
Closing the client, waiting through idle broadcasts, then reconnecting
repeats the transfer correctly. These tests qualify transport, not RTL reception.

```sh
make -C fpga/mister/main test
```

The service pathname defaults to `/tmp/dorado-net.sock`, overridable by
`DORADO_NET_SOCKET` for isolated tests. `DORADO_NET_TRACE=1` prints link
diagnostics once per second. The client uses its own owner-only socket and
registers with an empty datagram. Only FPGA cores whose original config-string name is `Dorado` are probed
(`user_io_get_core_name(1)`, since MGL `setname` overrides `CoreName`), and
an older core lacking capability magic `D074` gets no packet commands.

| Command | Command response | Subsequent words |
| --- | --- | --- |
| `70` | `D074` | flags, TX length, good TX, bad CRC, dropped TX, bad RX submit, aborted RX transaction, optional BaseBoard debug low/high, machine status, processor debug low/high |
| `71` | TX length | TX data words; reading leaves the packet pending |
| `72` | `D074` | acknowledges the pending TX after local server acceptance |
| `73` | RX ready | length then data; deselection commits a complete packet |

Flags: bit 0 = RX staging buffer ready, bit 1 = checked TX packet pending,
bit 2 = 1024-word frame buffers (otherwise the legacy 512-word limit).
Counters are their low 16 bits. Words cross SPI as 16-bit values and cross
the socket as big-endian bytes. Neither side carries the physical CRC or
controller status. The Cedar configuration buffers 1024 data words to carry its advertised
1478-byte Pup payloads. Legacy cores retain 512-word buffers. A busy TX buffer drops
later wire packets and counts them; Main retains a received server frame
until the RX staging buffer is ready. Partial, overlong and busy RX
transactions do not reach the cable.

The updated RTL snapshots the two32-bit debug words and machine status on
command70's first word. Both halves therefore describe the same observation,
including across counter rollovers; no Main protocol/binary change is needed.
The new UIO regression changes these inputs during the transaction and
fails on the old live-per-word implementation. Native29's frozen RBF still
has the older implementation; Native30 is the separate candidate for this.

Native30 adds a native-core `Debug monitor` option. In `Clock counts` mode,
Main's `machine=` value is enabled fabric rising edges and `cpu=` is total
host fabric rising edges, both modulo2^32. A medium fault retains priority
over that mode. For samples less than one wrap apart (107s at40MHz), use
`delta(enabled)/delta(host)` to measure the enabled fraction. These are
**not retired instructions**. Dividing enabled edges by `2*SYSPER` gives
only an instruction ceiling; original Hold and extended IM/TPC sequences
can lower actual throughput further. Normal CPU monitoring returns with
the option set to `CPU`. Native31 includes these counters and runs on hardware at50MHz (85.9s wrap;
use intervals<60s). Its Cedar loader sample is16.9255% enabled; see the
Native31 report. Native30 itself remains an undeployed control candidate.

The FPGA serializes the sync bit, data and real CRC and observes DskEth's
actual transmit wire. **DskEth includes the sync bit in its CRC.** The
captured FileSystem lookup ends in `7C50`; CRC over sync, data and that
check word is zero. Omitting sync gives residue `3FFE`. A fixture in
`tb_ethernet_wire.sv` guards this independently of the adapter's own encoder.

The Ethernet oscillator must receive `SYSPER=2*board_SYSPER`: the board
counts samples per 30 ns clock, the crystal cell per 60 ns instruction.
`ethernet-clock-test` checks the actual board at ratios 4, 8 and 16; before
the fix all three emitted 3,530 edges, making the current ratio half-speed.
They now emit 7,059 / 3,530 / 1,765 edges in the same sample window.

The actual receive path is separately tested at eight relative input-clock
phases. The controller FIFO returns all payload words, its CRC word, then
status `0000`; flipping one wire CRC bit gives status `0008`. This required
pre-edge D/CWE capture on the receiver's j05 F9401: sampling its new D value
after the clock edge had fed zeros to the checker and accepted corruption.
The transmit chip retains its previously qualified capture behavior.

Main already supports a per-core `main=` INI setting. A binary launched from
`/tmp` without that setting immediately restarts the installed stock Main in
`user_io_init()`. The isolated test uses a dedicated MGL name
`DoradoNetwork_20261017` and INI section with `video_mode=8`, `vsync_adjust=0`,
`main=/tmp/dorado-main/MiSTer`. Other launchers retain their existing settings.
The hardware TX test delivers twelve real CRC-checked guest frames to the
local service. Full supported Pup reception/boot is still under test.

Never replace a running Main executable, including by atomic rename. Main
reads `/proc/<pid>/exe` when it re-executes during a core load; replacing the
inode leaves that pathname ending in ` (deleted)` and its restart falls back
to a board reboot. Stage each binary at a fresh versioned path, then change
only the corresponding core's `main=` setting. Stock Main remains intact.

### Live video modeline parser correction

Apply `main-video-mode-260912.patch` to the same pinned checkout before
using extended raw-PLL modelines. The original parser passes a zero pixel
clock to `setPLL` for that form and spins, stopping Main's disk/network
service. The patch preserves the supplied PLL registers and rejects an
explicit zero clock in the short forms. It does not fix or qualify HDMI
mode switching on a running FPGA.

The regression compiles the exact parser from Main, with only its PLL
programming boundary stubbed. It checks raw-register preservation, zero
clock rejection, normal 9/11-item forms, and an unpatched failure control:

```sh
python3 tests/test_video_modeline.py /path/to/pinned/Main/video.cpp
```

Run it on the unpatched source; the test applies the patch to a temporary
copy. GCC 10.2.1 cross-build also passes in the isolated build-machine copy
`Main_DoradoVideo_20261017`; binary SHA256
`6358f31033c4dfc6fab209c67e2a1d3d65f9ea374fae210e1e230012b9ccee63`.
This candidate is not deployed. The running immutable Main remains
`1fb2e098...`, preserving the current Cedar FPGA boot.
