#!/usr/bin/env python3
"""Replay captured raw requests; count traffic, not elapsed FPGA time."""
import gzip
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROWS = [tuple(map(int, line.split(','))) for line in
        gzip.decompress((HERE / 'raw.csv.gz').read_bytes()).decode().splitlines()]


def replay(layout, update_write_hit=False):
    cache = {}
    hits = fills = writes = 0
    for _, _, write, address in ROWS:
        physical = layout(address >> 16, address & 65535)
        line = physical >> 2
        index = ((physical >> 2) ^ (physical >> 16)) & 127
        if write:
            writes += 1
            if not update_write_hit or cache.get(index) != line:
                cache.pop(index, None)
        elif cache.get(index) == line:
            hits += 1
        else:
            fills += 1
            cache[index] = line
    return dict(hits=hits, read_fills=fills, writes=writes)


result = dict(
    scope='Synthetic 16-group MSA fixture; not a Cedar workload or FPGA speed measurement',
    requests=len(ROWS), reads=sum(not row[2] for row in ROWS),
    writes=sum(row[2] for row in ROWS),
    current=replay(lambda group, cell: group << 16 | cell),
    group_low2=replay(lambda group, cell: (group >> 2) << 18 | cell << 2 | (group & 3)),
    row_column_transpose=replay(lambda group, cell: group << 16 | (cell & 255) << 8 | cell >> 8),
    current_write_hit_update=replay(lambda group, cell: group << 16 | cell, True),
)
assert result == json.loads((HERE / 'results.json').read_text())
print(json.dumps(result, indent=2))
