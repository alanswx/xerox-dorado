#!/usr/bin/env python3
"""Extract logical RAM rows and physical M10K usage from a Quartus fit report.

Logical rows may share physical blocks. Count unique locations rather than
summing the per-row M10K allocations, and check against the fitter total.
"""
import argparse
import json
from pathlib import Path
import re


def audit(path):
    report = path.read_text(errors="replace")
    header, rows = None, []
    for line in report.splitlines():
        fields = [s.strip() for s in line.strip().strip(";").split(";")]
        if header is None:
            if fields[0] == "Name" and "Port A Depth" in fields and "MLAB cells" in fields:
                header = fields
        elif line.startswith(";") and len(fields) == len(header):
            rows.append(dict(zip(header, fields)))
        elif rows:
            break
    if not rows:
        raise ValueError("No fitter RAM rows found")
    owners, locations = {}, set()
    for row in rows:
        name = row["Name"]
        board = re.search(r":b_([^|]+)", name)
        owner = board[1] if board else next(
            (s for s in ("u_media", "u_ddram", "ascal", "u_network", "u_key_queue")
             if s in name), "other")
        entry = owners.setdefault(owner, dict(logical_bits=0, logical_m10k_allocations=0,
                                              mlab_cells=0, rows=0, locations=set()))
        entry["rows"] += 1
        for key, field in (("logical_bits", "Size"),
                           ("logical_m10k_allocations", "M10K blocks"),
                           ("mlab_cells", "MLAB cells")):
            entry[key] += int(row[field].replace(",", ""))
        physical = set(re.findall(r"M10K_[A-Z0-9_]+", row["Location"]))
        entry["locations"].update(physical)
        locations.update(physical)
    for entry in owners.values():
        entry["unique_m10k_locations"] = len(entry.pop("locations"))
    total = re.search(r";\s*(?:M10K blocks|Total RAM Blocks)\s*;\s*([\d,]+)\s*[/;]", report)
    if total and int(total[1].replace(",", "")) != len(locations):
        raise ValueError(f"RAM rows give {len(locations)} blocks, fitter says {total[1]}")
    return dict(source=str(path), rows=rows), dict(
        owners=owners, unique_m10k_locations=len(locations),
        note="Logical RAM allocations can share physical M10Ks; summed allocation counts are not physical usage.")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("report", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    rows, owners = audit(args.report)
    args.output.mkdir(parents=True, exist_ok=True)
    for name, data in (("ram-summary.json", rows), ("ram-by-owner.json", owners)):
        (args.output / name).write_text(json.dumps(data, indent=2) + "\n")
    print(f"{len(rows['rows'])} logical RAMs; {owners['unique_m10k_locations']} physical M10Ks")


if __name__ == "__main__":
    main()
