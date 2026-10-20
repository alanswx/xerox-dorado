# Main service permissions

This component protocol replaces virtual permission inputs with clocked,
Main-owned registers. It is not yet wired into the native Core or the Main
disk/daemon coordinator. Input/video pause and physical DDR fencing remain
required before complete state access. The running FPGA is unchanged.

Main uses its original `Dorado` config-string identity and existing SPI owner
thread. The endpoint requires a nonzero build ID and board schema. A coordinator
must compare both against the selected bundle before changing the machine.
The separate board-only ABI2 schema remains `af0b647cb43a3a62`.

`DoradoSnapshotClient::request_hold()` verifies the host capability, then sends
the existing eight-word command77 with hold=1. Main waits for the actual guest
and global holds before Begin. A transmitted command is not an acknowledgment:
Main reads status and checks nonce, phase, grants and the error counter before
advancing. Every individual transaction contains at most22 sixteen-bit words.

| Command | Action |
| --- | --- |
| 7A | Read22-word coherent capability/status |
| 7B | Begin a fresh nonzero lease nonce after the board hold is established |
| 7C | Set the five host service permissions for that nonce |
| 7D | Clear permissions and request native release through retained coordinators |
| 7E | Finish after native release and real disk/daemon Continue ACKs |
| 7F | Quarantine that active lease, including after release has begun |

Action packets contain exactly16 words, committed only on deselection:
command, `c04d`, ABI1, four little-word-first build-ID words, four schema words,
four lease-nonce words and a payload. Only7C accepts a nonzero payload; its upper
eleven bits must be zero. A partial, overlong, unknown-valued or mismatched
packet cannot change the lease. Reusing the last completed nonce is rejected.

Service bits are memory0, disk1, network2, input3 and video4. Each requested bit
must be both locally available and locally acknowledged. These ACKs do not
replace ARM leases: Main must first finish issued FPGA disk work, pause its
image generation, obtain the daemon pause ACK, and establish physical DDR
visibility. Input and video remain unavailable until real endpoints exist.
The current component tests advertise only mask7.

Status words are:

| Words | Value |
| --- | --- |
| 0..3 | `d0c0`, ABI1, phase, domain count5 |
| 4..7 | Build ID |
| 8..11 | Board schema |
| 12..15 | Current/last completed lease nonce |
| 16..18 | Host permissions, local available mask, local ACK mask |
| 19 | Boundary bits: host hold0, board command hold1, guest held2, global hold3, board busy4, board fault5 |
| 20..21 | Rejected-packet/action count |

All mutable status words are captured on the first command word. Phases are
Idle0, Held1, Releasing2 and Fault3. Held and Fault retain `host_hold`; native
coordinators retain the physical hold through Releasing. ACK loss, an unexpected
board-hold release, a device fault or parser/PLL reset during a lease quarantines
the machine and revokes permission. Fault remains readable, reasserts the board
hold when needed and requires genuine machine reseed. Neither a parser reset
nor Finish recovers an imported or faulted machine.

The endpoint adds332 required implementation bits. The component inventory
accounts for every field; native export/import and the restore policy for the
live admission lease remain open. The tests use modeled local/native ACK
inputs, actual service RTL, actual board UIO and the actual Main client adapter.
