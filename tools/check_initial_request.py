#!/usr/bin/env python3
"""Check Initial's real Manchester boot request, including its hardware CRC."""
import argparse
from pathlib import Path
from decode_xmt import decode, words
from eb_replies import crc16


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("recording", type=Path)
    parser.add_argument("--sysper", type=int, default=4)
    parser.add_argument("--station", type=lambda s: int(s, 0), default=0o42)
    parser.add_argument("--bfn", type=lambda s: int(s, 0), default=0o113)
    args = parser.parse_args()
    transitions = [tuple(map(int, line.split()))
                   for line in args.recording.read_text().splitlines()]
    # InitialEther.mc SetupEtherBootRequest, including the legal nil Pup
    # checksum. CRC is checked over the physical sync/data/check bits.
    expected = (args.station, 0o1000, 0o26, 0o264, 1, args.bfn,
                0, 0, 4, args.station, 1, 1, 0xffff)
    valid = set()
    for polarity in (0, 1):
        for skip in (False, True):
            for bits in decode(transitions, 340 / (30 / args.sysper), polarity, skip):
                packet, remainder = words(bits)
                if bits[0] == 1 and len(packet) >= 8 and not remainder and crc16(bits) == 0:
                    valid.add(tuple(packet[:-1]))
    if expected not in valid:
        raise SystemExit("FAIL: Initial boot request differs: " + repr(sorted(valid)))
    print(f"INITIAL NETWORK PASS: 13 exact request words, station {args.station:o}B, "
          f"BFN {args.bfn:o}B, nil Pup checksum and zero hardware CRC residue")


if __name__ == "__main__":
    main()
