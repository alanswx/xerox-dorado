#!/usr/bin/env python3
"""Qualify real Bootstrap IM writes/startup with parity enabled throughout."""
import subprocess
import sys
from pathlib import Path

binary, fixtures = map(Path, sys.argv[1:3])
common = [str(binary), "+start=4064", "+imparityon", "+keepmirdebug"]
cases = [
    ("CP load", [f"+vectors={fixtures}/boot.vec", "+hunklimit=16",
                 f"+expectim={fixtures}/im.txt", "+cycles=350"], False),
    ("Bootstrap execution", [f"+preload={fixtures}/im.txt", "+cycles=100000"], False),
    ("Corrupt parity rejected", [f"+preload={fixtures}/im.txt", "+cycles=100000",
                                 "+impareven"], True),
]
for name, args, expect_failure in cases:
    result = subprocess.run(common + args, text=True, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT)
    output = result.stdout
    if expect_failure:
        valid = result.returncode != 0 and "the machine stopped -- with the parity enables on" in output
    else:
        valid = result.returncode == 0 and "the machine executes microcode out of IM." in output
        valid &= "parity enables IMLH=1 IMRH=1, StopMIRClkEn=1" in output
        addresses = next((s.split(":", 2)[-1].split() for s in output.splitlines()
                          if "addresses executed:" in s), [])
        valid &= {"fe0", "fe1", "fc0"}.issubset(addresses)
        if name == "CP load":
            valid &= "CP-LOAD PASS: 50 original microinstructions and both odd-parity bits match" in output
    if not valid:
        print(output)
        raise SystemExit(f"BOOTSTRAP PARITY FAIL: {name}, exit {result.returncode}")
    print(f"BOOTSTRAP PARITY PASS: {name}")
