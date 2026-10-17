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
| `70` | `D074` | flags, TX length, good TX, bad CRC, dropped TX, bad RX submit, aborted RX transaction, optional machine debug low/high and status |
| `71` | TX length | TX data words; reading leaves the packet pending |
| `72` | `D074` | acknowledges the pending TX after local server acceptance |
| `73` | RX ready | length then data; deselection commits a complete packet |

Flags: bit 0 = RX staging buffer ready, bit 1 = checked TX packet pending.
Counters are their low 16 bits. Words cross SPI as 16-bit values and cross
the socket as big-endian bytes. Neither side carries the physical CRC or
controller status. Buffers hold 512 data words. A busy TX buffer drops
later wire packets and counts them; Main retains a received server frame
until the RX staging buffer is ready. Partial, overlong and busy RX
transactions do not reach the cable.

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
