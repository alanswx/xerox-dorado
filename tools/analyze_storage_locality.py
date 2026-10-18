#!/usr/bin/env python3
"""Replay completed raw-storage operations to evaluate backing address layout.

Input is tb_storage_board's optional +raw_trace=PATH CSV, including its
independent final-memory oracle and end marker. This is an offline
locality experiment, not an RTL implementation, latency model or FPGA speed
measurement. Check each replayed read against its captured actual reply.
"""
import argparse
from collections import OrderedDict
import csv
import hashlib
import json
from pathlib import Path


def replay(operations, address_bits, group_bits, interleave, policy, lines):
    backing, cache = {}, OrderedDict()
    hits = reads = writes = 0
    for op in operations:
        raw, value = op['address'], op['value']
        row, group = raw & ((1 << address_bits)-1), raw >> address_bits
        physical = (row << group_bits) | group if interleave else raw
        line, lane = physical >> 2, physical & 3
        index = ((physical >> 2) ^ (physical >> address_bits)) & (lines-1)
        key = index if policy == 'direct' else line
        if op['write']:
            writes += 1
            backing[physical] = value
            # Match the current DDR buffer's write-through/index invalidation.
            cache.pop(key, None)
            continue
        reads += 1
        entry = cache.get(key)
        if entry and entry[0] == line:
            hits += 1
            actual = entry[1][lane]
            cache.move_to_end(key)
        else:
            data = tuple(backing.get((line << 2)+n, 0) for n in range(4))
            actual = data[lane]
            if policy != 'none':
                cache[key] = (line, data)
                cache.move_to_end(key)
                if len(cache) > lines:
                    cache.popitem(last=False)
        assert actual == value, (op, interleave, policy, actual)
    canonical = {}
    for physical, value in backing.items():
        if interleave:
            row, group = physical >> group_bits, physical & ((1 << group_bits)-1)
            raw = (group << address_bits) | row
        else:
            raw = physical
        if value:
            canonical[raw] = value
    return dict(reads=reads, writes=writes, read_hits=hits,
                line_fetches=reads-hits, read_hit_fraction=hits/reads if reads else 0), canonical


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('trace', type=Path)
    parser.add_argument('--address-bits', type=int, default=16)
    parser.add_argument('--group-bits', type=int, default=4)
    parser.add_argument('--lines', type=int, default=128)
    args = parser.parse_args()
    assert 1 <= args.address_bits <= 24 and 1 <= args.group_bits <= 10
    assert args.lines > 0 and args.lines & (args.lines-1) == 0
    operations, oracle = [], {}
    complete = False
    words = 1 << (args.address_bits+args.group_bits)
    for row in csv.DictReader(args.trace.read_text().splitlines()):
        assert not complete, 'records after final state marker'
        kind = int(row['kind'])
        op = dict(host_cycle=int(row['host_cycle']), write=kind,
                  address=int(row['address'], 16), value=int(row['value'], 16))
        assert kind in (0, 1, 2, 3) and 0 <= op['value'] <= 65535
        if kind == 3:
            assert op['address'] == words and op['value'] == 0
            complete = True
            continue
        assert 0 <= op['address'] < words
        if kind == 2:
            assert op['value'] and op['address'] not in oracle
            oracle[op['address']] = op['value']
            continue
        assert not oracle, 'operations after final state scan'
        assert not operations or op['host_cycle'] > operations[-1]['host_cycle']
        operations.append(op)
    assert operations and complete, 'missing final state oracle/end marker'
    result = dict(trace_sha256=hashlib.sha256(args.trace.read_bytes()).hexdigest(),
                  operations=len(operations), address_bits=args.address_bits,
                  group_bits=args.group_bits, lines=args.lines, layouts={},
                  final_nonzero_words=len(oracle), row_locality={},
                  limits='Synthetic one-MSA fixture only. Software64-bit read-buffer replay; '
                         'Row counts omit refresh/reset and are uncached geometry locality only; '
                         'no latency/timing/area or FPGA throughput prediction. '
                         'No RTL layout or SDRAM burst change is implemented by this tool.')
    golden = None
    for interleave in (False, True):
        layout = 'interleaved-groups' if interleave else 'group-major'
        rows = {}
        row_hits = row_misses = 0
        for op in operations:
            raw = op['address']
            physical = (((raw & ((1 << args.address_bits)-1)) << args.group_bits) |
                        (raw >> args.address_bits)) if interleave else raw
            # Current XSDS controller: rank25,bank24:23,row22:10,col9:0.
            bank, row = physical >> 23, (physical >> 10) & 0x1fff
            if rows.get(bank) == row:
                row_hits += 1
            else:
                row_misses += 1
                rows[bank] = row
        result['row_locality'][layout] = dict(hits=row_hits, misses=row_misses,
                                               hit_fraction=row_hits/len(operations))
        result['layouts'][layout] = {}
        for policy in ('none', 'direct', 'lru'):
            stats, state = replay(operations, args.address_bits, args.group_bits,
                                  interleave, policy, args.lines)
            if golden is None:
                golden = state
            assert state == golden == oracle, (layout, policy, 'final contents differ from physical oracle')
            result['layouts'][layout][policy] = stats
    print(json.dumps(result, indent=2))


if __name__ == '__main__':
    main()
