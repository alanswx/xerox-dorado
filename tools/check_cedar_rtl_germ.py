#!/usr/bin/env python3
"""Check observed PilotBoot copy reads and the first Mesa fetch in an RTL log.

This checks sampled CPU reads, not an entire-memory checksum. PilotBoot
legitimately changes its request words before copying the original germ.
Run tb_exec with +bootio, a window covering the copy, and +ifutrace.
"""
import argparse
import json
from pathlib import Path
import re


def check(log, germ):
    raw = germ.read_bytes()
    if len(raw) != 17920:
        raise ValueError("expected the matched Cedar 6.1 germ (8960 words)")
    expected = [int.from_bytes(raw[i:i + 2], "big") for i in range(0, len(raw), 2)]
    # PilotBoot.mc: pRequest=1360B, action=bootPhysicalVolume,
    # deviceType=sa4000. These are writes by the running microcode.
    expected[0x2f0 - 0x200] = 2
    expected[0x2f1 - 0x200] = 3
    words = {}
    entered = False
    fetches = acknowledgements = 0
    initial_ok = image_ok = False
    read = re.compile(r"BOOTIO @(\d+) CIA=5f1 task=0 .* MD=([0-9a-f]+) VA=([0-9a-f]+).* Hold=0")
    for line in log.open():
        if "CIA=7e0 task=0" in line:
            entered = True
        if not entered and (match := read.search(line)):
            cycle, word, address = (int(match[1]), int(match[2], 16), int(match[3], 16))
            if 0x200 <= address < 0x200 + len(expected):
                words[address] = (word, cycle)
        if match := re.search(r"IFU -- IfuMemRef (\d+) transitions, IfuMemAck (\d+)", line):
            fetches, acknowledgements = map(int, match.groups())
        initial_ok |= "INITIAL ROM PASS:" in line
        image_ok |= "LOADED EB PASS: 3600 original IM and 256 IFUM" in line
    mismatches = [dict(address=f"{address:04x}", expected=f"{expected[address - 0x200]:04x}",
                       actual=f"{word:04x}", cycle=cycle)
                  for address, (word, cycle) in sorted(words.items())
                  if word != expected[address - 0x200]]
    result = dict(sampled_words=len(words), germ_words=len(expected),
                  unexpected_words=len(mismatches), first_mismatches=mismatches[:16],
                  germ_handoff=entered, ifu_fetches=fetches, ifu_acknowledgements=acknowledgements,
                  original_initial_pass=initial_ok, original_cedar_image_pass=image_ok)
    result["pass"] = (len(words) >= 8900 and not mismatches and entered
                      and fetches > 0 and acknowledgements > 0 and initial_ok and image_ok)
    return result


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("log", type=Path)
    ap.add_argument("--germ", type=Path, default=Path("chm/cedar/germ-alt/Dorado.germ-6.1.6"))
    args = ap.parse_args()
    result = check(args.log, args.germ)
    print(json.dumps(result, indent=2))
    raise SystemExit(0 if result["pass"] else 1)


if __name__ == "__main__":
    main()
