# Ethernet Architecture and Microcode Netboot

This note documents the Dorado Ethernet controller and the minimum fake
network service needed for Initial to load emulator microcode over Ethernet.
It is intentionally focused on the boot path; full Pup networking can come
later.

## Sources

- `DoradoDocs/manuals/Dorado_Hardware_Manual_Sep1981.pdf`, Ethernet chapter
  and DskEth board details.
- `DoradoDocs/schematics/DskEth.pdf`, for the combined disk/Ethernet board.
- `chm/dorado/expanded/BootstrapSources.dm/InitialEther.mc`, the Initial
  microcode that sends the request and receives the microcode stream.
- `DoradoDocs/manuals/Dorado_Booting_Operation_and_Mechanisms_Apr80.pdf` and
  `DoradoBootingImpl.press!1`, for boot-file numbering and LoadMB/EB context.
- CHM Pup gateway sources downloaded under
  `docs/research/ethernet-netboot/downloads/`:
  `PupMicrocodeBooter.mesa-2.html`, `BootServerDefs.mesa-2.html`,
  `EtherBoot.mesa-1.html`, `MicrocodeBooting.mesa-1.html`, and
  `PUPPAPER.PRESS-1.pdf`.
- CHM Pup/Alto boot specs mirrored under `chm/pup/` and `chm/altodocs/`:
  `ALTOBOOT.BRAVO!1.html`, `ETHERBOOT.BRAVO!1.html`,
  `EFTPSPEC.BRAVO!1.html`, `PUPDEF.MAC!1.html`,
  `PUPSPEC.PRESS!1.pdf`, `ETHERBOOT.TTY!2.html`, and
  `NETEXEC.TTY!2.html`.

Remote source URLs:

- https://xeroxparcarchive.computerhistory.org/_cd8_/pupgateway/calaveras/private/.PupMicrocodeBooter.mesa%212.html
- https://xeroxparcarchive.computerhistory.org/_cd8_/pupgateway/calaveras/private/.BootServerDefs.mesa%212.html
- https://xeroxparcarchive.computerhistory.org/_cd8_/pupgateway/calaveras/private/.EtherBoot.mesa%211.html
- https://xeroxparcarchive.computerhistory.org/_cd8_/pup/.PUPPAPER.PRESS%211.pdf

## Two Different Ethernet Boots

The Dorado boot documentation describes two Ethernet boot layers that should
not be conflated:

1. **Initial microcode boot.** Initial loads emulator microcode, such as
   `AltoMesaDorado.eb!1`, with `InitialEther.mc` and Pup types `264B`
   (`MicrocodeBootRequest`) and `265B` (`MicrocodeBootReply`). This is the
   path implemented by the current fake Dorado Ethernet controller.
2. **Alto-style software boot.** After `LoadRam`, the loaded Alto emulator
   performs an Alto-compatible software boot from disk or Ethernet. The 1984
   Dorado booting memo says these conventions are the same as the Alto: no BS
   boots from disk, while BS selects Ethernet boot and BS+Quote requests the
   NetExec. The local `EtherBoot.mesa!1` transcription shows this path starts
   with an Alto "Mayday" Pup (`typeMayday = 244B`) and then receives the boot
   file by EFTP Data/End packets (`30B`/`31B`) with Ack packets from the
   loader.

There is a third, Cedar-specific network route, but it is not the same as the
Alto software boot. The Dorado booting memo says Cedar microcode boots
software from local disk by loading the installed germ and physical volume
boot file. To fetch Cedar software from Ethernet, real users first booted
Alto/Mesa NetExec, then CedarNetExec, then the desired program. Keep that
separate from both Initial MicrocodeBoot and AltoBoot/EFTP.

The second path runs inside the Alto emulator, not Initial. It will require
the Alto Ethernet/SIO hardware surface that AEmu exposes to Alto code, plus a
minimal AltoBoot/EFTP server. Responding with Initial-style
`MicrocodeBootReply` packets is not sufficient for a NetExec or OS software
boot.

## Hardware Model

The Dorado uses a 3 Mbit/s Alto-style Ethernet controller on the DskEth board.
Packets are phase-encoded on a passive coax network. A well-formed packet is:

1. start bit,
2. 8-bit destination host,
3. 8-bit source host,
4. zero or more 16-bit data words,
5. 16-bit checksum/CRC word,
6. silence.

For emulator bring-up, we do not need bit-cell timing, phase encoding, or real
collision behavior. Initial only needs the Slow I/O visible behavior.

### Slow I/O Interface

The Ethernet controller uses two TIOA addresses:

| TIOA | Name | Meaning |
|---|---|---|
| `015B` | `EData` | Input reads the receiver bus register. Output writes the transmitter bus register. |
| `016B` | `EControl` | Input reads status. Output writes receiver/transmitter/test control bits. |

It uses two tasks:

| Task | Name | Meaning |
|---|---|---|
| 6 | EOT | Ethernet output task. |
| 7 | EIT | Ethernet input task; higher priority than EOT. |

Each direction has a 16-word FIFO plus a bus register and shift register. The
receiver places one CRC word and then one ending status word after each packet.
The ending status word is marked with `EthData.18`; `IOAtten` branches when EIT
reads that status word. Initial accepts only packets whose status low byte is
zero.

The transmitter wakes EOT when its bus register is empty and transmit wakeups
are enabled. After EOT writes the final data word, it writes `SendEOP` to
`EControl`. `IOAtten` on the output path means transmit abort: collision,
transmit data-late, or FIFO parity error. Initial treats all transmit aborts as
collisions and retries with a firmware backoff.

`EControl` input includes the local host address in the high byte. Initial does
`LHost_ RSH[Input, 10]`, so an emulator can return a stable fake host number
there.

### Control Commands

The hardware manual names the control bits but not the symbolic command words
used in Initial. `InitialEther.mc` uses these semantic commands:

| Command | Required fake behavior |
|---|---|
| `TurnOnRx` | Enable receiver wakeups after the next packet boundary. |
| `TurnOffRx` | Disable receiver and discard queued/current receive data. |
| `TurnOnTx` | Enable transmitter; EOT will start sending the request packet. |
| `TurnOffTx` | Disable transmitter and clear transmit abort state. |
| `WaitForBOP` | Discard the rest of the current receive packet; wake at the next beginning of packet. |
| `SendEOP` | End the currently captured transmit packet. |
| `CountDown` | Disable transmit data wakeups until the next 16 us pendulum tick. |

The exact octal command encodings are still an open item. For a first fake
controller, it is acceptable to decode by the values emitted by the running
microcode once traced, then bind those values to the behaviors above.

## Dorado Initial Microcode Boot Protocol

Initial calls `EtherMicrocodeBoot` with:

- `BootDataPtr`: first main-memory word where the EB payload should be stored.
- `T`: microcode boot-file number offset.

Initial broadcasts one Pup request and expects a stream of Pup replies. The
reply payload words are copied contiguously to `BootDataPtr`. A final
zero-length reply terminates the stream; Initial then calls `LoadRam` on the
memory image.

### Boot-File Numbers

The boot server reserves octal `3000` and up for microcode files. The Dorado
uses the `3100` range. Two documentation snapshots disagree slightly:

| Source | Dorado assignments |
|---|---|
| Apr 1980 booting memo | `3100` Mesa, `3101` Smalltalk, `3102` Lisp, `3103` Cedar |
| Later booting implementation memo | `3110` Mesa, `3111` Smalltalk, `3112` Lisp, `3113` Cedar, `3114` Test |

The later memo matches current Initial source comments and local CHM files
better. Initial sends the offset, not the full number, so Mesa is normally
requested as `110B`, and the server maps it to `3110B`.

Local candidate EB files:

| Offset | Full boot file | Documented name | Local candidates |
|---|---|---|---|
| `110B` | `3110B` | `DoradoMesa.eb` | `chm/dorado/AltoMesaDorado.eb!2`, `chm/microcode/AltoMesaDorado.eb!1` |
| `111B` | `3111B` | `DoradoSmalltalk.eb` | `chm/microcode/SmalltalkDorado.eb!1` |
| `112B` | `3112B` | `DoradoLisp.eb` | no obvious `.eb`; `chm/dorado/expanded/UnBug.bfs!1_/DoradoLisp.MB` is an MB source-format candidate |
| `113B` | `3113B` | `DoradoCedar.eb` | `chm/dorado/CedarDorado.eb!6` |
| `114B` | `3114B` | `DoradoTest.eb` | `chm/dorado/test/TestDorado.eb!5`, `chm/microcode/TestDorado.eb!1` |

There are also `InitialEther*.eb!3` files in `chm/dorado/`; those are
Ether-bootable Initial-stage variants, not the emulator microcode payload that
Initial requests after it is already running.

### Request Packet

Initial builds a 15-word Ethernet/Pup packet at VM `177400B` and EOT sends it.

| Word | Value | Meaning |
|---|---|---|
| 0 | `0 ,, LHost` | Ethernet destination host 0, source local host. Host 0 means broadcast in this boot protocol. |
| 1 | `1000B` | Ethernet type `typePup`. |
| 2 | `26B` | Pup length for a no-payload request. |
| 3 | `264B` | Pup type `MicrocodeBootRequest`. |
| 4 | `1B` | Pup ID high word; server expects microcode version 1. |
| 5 | `BFN` | Pup ID low word; requested microcode boot-file offset. |
| 6 | `0` | Destination network and host. |
| 7 | `0` | Destination socket high word. |
| 10 | `4B` | Destination socket low word: misc services socket. |
| 11 | `LHost` | Source network 0 and local host. |
| 12 | `1B` | Source socket high word. |
| 13 | `1B` | Source socket low word; Initial uses socket `[1,1]`. |
| 14 | `-1` | Nil Pup checksum. |

Initial retries 12 times. Each wait is about four seconds by RTC arithmetic;
the source comment rounds this to roughly five-second intervals.

### Reply Filtering

EIT receives every packet into VM locations `0..416` and filters only after the
full packet has arrived. A reply is accepted if:

- the receiver ending status low byte is zero,
- Ethernet destination host equals `LHost`,
- Pup type is `265B` (`MicrocodeBootReply`),
- Pup ID low word equals the current `SeqNo`,
- source host matches the first accepted reply's source host.

Initial does not appear to verify the reply Pup sockets or Pup checksum. For
realism, set destination to the requester address/socket and source to the boot
server's misc-services socket.

### Reply Length and Payload Layout

`PupMicrocodeBooter.mesa` is the clearest server-side reference:

- it requires the first word of the EB file to equal microcode version `1`,
- it skips the first 256 words of the EB file,
- it sends the remaining words as `MicrocodeBootReply` Pups,
- the chunk pattern is 255 words, 255 words, then 258 words, repeated,
- it sends a final `MicrocodeBootReply` with byte length zero as the Initial
  end marker.

Initial computes the number of payload words from `Pup.length` like this:

```text
words_minus_1 = (Pup.length - 30B) >> 1
```

The empty-packet branch is taken when `words_minus_1` is negative. Therefore,
for a reply containing `N` payload words, the safe formula is:

```text
Pup.length = 26B + 2 * N
```

Examples:

| Payload words | `Pup.length` | Initial result |
|---|---|---|
| 0 | `26B` | `words_minus_1 = -1`; end marker |
| 1 | `30B` | copy one payload word |
| 255 | `1024B` | copy 255 payload words |
| 258 | `1032B` | copy 258 payload words |

The payload starts at word 15 of the received Ethernet/Pup packet. Initial's
buffer limit comment is "258 data + 13 overhead + CRC = 420B", which matches
the server's maximum 258-word chunk.

## What We Need to Fake

The best first implementation is a fake in-process Ethernet controller, not a
wire-level Ethernet implementation and not a shortcut memory loader.

### Packet-Level Fake Controller

Implement the controller at the `EData`/`EControl` and EIT/EOT wakeup level:

1. Return a stable fake local host address from `EControl` input, for example
   host `42B` in the high byte.
2. Capture EOT writes to `EData` while transmit is enabled.
3. Treat `SendEOP` as the end of the outgoing request.
4. Parse request word 5 as the boot-file offset.
5. Map the offset to a local EB file.
6. Validate the EB header word is version `1`; then skip the first 256 words.
7. Queue `MicrocodeBootReply` packets with conservative 255-word chunks and a
   final zero-length reply. The source server uses a 255/255/258 pattern, but
   the current in-process fake stays below the maximum while the receive
   boundary model is being hardened.
8. Present queued receive packets through `EData` with normal data words first,
   then a dummy CRC word, then a status word whose low byte is zero and whose
   read asserts `IOAtten`.
9. Wake EIT at the first word of each queued packet when receive is enabled.
10. Simulate no collisions and no transmit data-late for the first bring-up
    path. EOT should see a normal transmit completion after `SendEOP`.

This path exercises Initial's real EOT/EIT microcode, packet filtering,
sequence-number logic, and main-memory copy loop.

### Shortcut Loader

A shortcut that copies EB payload words directly into memory can remain useful
for probes, but it does not test Ethernet task scheduling, `IOAtten`, packet
filtering, or the server protocol. It should not be the primary netboot path.

### Full Wire-Level Ethernet

A real 3 Mbit/s network model would include phase encoding, CRC, collision
classification, jamming, binary exponential backoff, loopback, and timing. This
is unnecessary for boot bring-up. It only becomes important for hardware tests,
performance-sensitive networking, or connecting the emulator to an external Pup
network.

## Fake Reply Packet Template

For request host `LHost`, fake server host `RHost`, sequence `S`, and `N`
payload words:

| Word | Value |
|---|---|
| 0 | `LHost << 8 | RHost` |
| 1 | `1000B` |
| 2 | `26B + 2 * N` |
| 3 | `265B` |
| 4 | `1B` |
| 5 | `S` |
| 6 | `LHost` |
| 7 | `1B` |
| 10 | `1B` |
| 11 | `RHost` |
| 12 | `0B` |
| 13 | `4B` |
| 14.. | payload words |
| after payload | dummy Ethernet CRC word |
| final read | receiver status word with low byte zero and `IOAtten` asserted |

The header/socket fields after word 5 are not all checked by Initial, but the
values above mirror the request/reply relationship: destination is the Dorado's
source socket and source is misc services.

Important emulator detail: do not queue an explicit Pup checksum word before
the payload for Initial's Ethernet input task. In the `InitialEther.mc` copy
sequence, `EIPtr_ (Fetch_ 14S)+1` acts as the prefetch for the first payload
word in the current memory pipeline. When the fake reply included a synthetic
`177777` checksum word here, EIT copied that checksum into the EB stream as
word zero and shifted the whole image.

## Open Items

- Decode the exact `EControl` command word values emitted by Initial. The
  source names are known, but the numeric encodings are not in the local
  documents reviewed so far.
- Confirm whether our EB reader already parses words in the right byte order
  for network reply generation. The protocol transfers 16-bit words.
- Decide the default fake host numbers. Host `0` is broadcast in the request;
  replies must be addressed to the Dorado's local host and must come from a
  stable non-negative remote host.
- Add trace points for request capture, reply enqueue, EIT status reads, and
  `SeqNo` advancement. These will be much more useful than raw packet dumps
  during boot bring-up.
- If later exposing packets outside the emulator, implement or reuse the
  Alto/Pup Ethernet CRC/checksum path. For the in-process fake, a dummy CRC word
  plus status zero is enough because Initial trusts the controller status.

## Bring-Up Notes

As of 2026-05-01, the emulator has a packet-level fake Ethernet controller and
the full bootstrap probe reaches `EtherMicrocodeBoot`. A CPU fix changed
non-emulator `IOAtten` to use the device attention callback rather than the
task wakeup bit; otherwise EOT treats every normal transmit wakeup as an abort.

The fake can queue the `AltoMesaDorado.eb!1` reply stream, and EIT reads that
stream. Initial still does not accept the boot image because EOT fetches zeros
from the VM `177400B` transmit packet buffer and EIT appears unable to
round-trip received packet words through its page-zero buffer. The next
hardware fidelity issue is therefore the non-emulator task memory path, not the
Pup packet format.

2026-05-02 update: Initial now accepts the packet-level fake well enough to
leave `AWAITETHERBOOTREPLY`, run the EB checksum/LoadRam path, and execute the
loaded Alto/Mesa microcode world. The fixes were:

- EIT needs device `IOAtten` to fire on the queued status word before the
  status read, not only after a latched status read.
- ALU branch-condition latches are task-local. EIT computes the terminator
  `ALU<0` condition, blocks, and resumes later; a single global latch lets
  other tasks clobber that condition before EIT's branch consumes it.
- Initial's `RSH 1` length calculation relies on the negative zero-data
  terminator staying negative in the next branch. The CPU model currently
  preserves the sign bit for that MicroD form; this is a source-compatibility
  observation that still needs reconciliation against the hardware-manual
  wording.
- The fake reply must expose 12 protocol words before payload, then the dummy
  Ethernet CRC/status trailer. A synthetic Pup checksum word before payload is
  copied as data.
- The fake currently uses fixed 255-word chunks. This is intentionally below
  the documented 258-word maximum while the receive model remains simplified.

The next missing piece is post-LoadRam software boot/device behavior, not
Initial MicrocodeBoot delivery. The focused run still shows sparse display
output and no meaningful framebuffer content, with the loaded disk/display task
mix active. A natural run needs roughly 61.4M Dorado cycles to enter the
loaded world, so the probe default and focused commands now use a 140M-cycle
budget; a 60M-cycle run stops in Initial's EB checksum loop before LoadRam.
