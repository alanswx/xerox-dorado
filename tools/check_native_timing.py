#!/usr/bin/env python3
"""Check native core slack AND internal clock coverage in TimeQuest reports.

Usage: check_native_timing.py Dorado.sta.summary native-unconstrained.rpt
The second file comes from fpga/mister/audit_native_clocks.tcl. Framework
input/output delays remain a separate qualification; do not hide a missing
internal clock behind those expected I/O limitations.
"""
import argparse
from pathlib import Path
import re


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("summary", type=Path)
    parser.add_argument("unconstrained", type=Path)
    args = parser.parse_args()
    checks = re.findall(r"Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)",
                        args.summary.read_text())
    if not checks:
        raise SystemExit("FAIL: no timing checks in summary")
    bad = [(name, value) for name, value in checks if float(value) < 0]
    if bad:
        raise SystemExit("FAIL: negative slack: " + repr(bad))
    report = args.unconstrained.read_text()
    for category in ("Illegal Clocks", "Unconstrained Clocks"):
        match = re.search(r";\s*" + category + r"\s*;\s*(\d+)\s*;\s*(\d+)\s*;", report)
        if not match:
            raise SystemExit(f"FAIL: missing {category} coverage report")
        if any(int(value) for value in match.groups()):
            raise SystemExit(f"FAIL: {category}: setup/hold {match.groups()}")
    print(f"NATIVE TIMING PASS: {len(checks)} checks, minimum slack "
          f"{min(float(value) for _, value in checks):.3f} ns; "
          "zero illegal or unconstrained clocks. Framework I/O delay coverage is separate.")


if __name__ == "__main__":
    main()
