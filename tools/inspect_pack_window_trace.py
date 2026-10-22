#!/usr/bin/env python3
"""Audit native window requests; quantify SPI bytes, never elapsed speedup.

Main's read/write_request and corresponding ACK are the core-facing transfers.
pread/pwrite are the separate ARM cache's backing-file operations. A poll_gap
records scheduling latency; it is not a lost trace record.
"""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path


def inspect(data):
    assert data.endswith(b'\n'), 'Incomplete trace record; preserve/recollect prefix'
    rows = [json.loads(line) for line in data.splitlines()]
    assert rows and rows[0]['event'] == 'mount'
    known = {'mount', 'poll', 'poll_gap', 'read_request', 'read_ack',
             'write_request', 'write_ack', 'pread_begin', 'pread_end',
             'pwrite_begin', 'pwrite_end', 'fsync_begin', 'fsync_end',
             'barrier_begin', 'barrier_end'}
    pending = {}
    io = {}
    span = None
    counts = Counter()
    sizes = Counter()
    actual = whole = 0
    last_ns = -1
    for index, row in enumerate(rows):
        event = row['event']
        assert event in known, ('Unreviewed/fault/overflow event', index, event)
        assert row['ns'] >= last_ns, ('Nonmonotonic trace', index)
        last_ns = row['ns']
        counts[event] += 1
        if event == 'mount':
            assert index == 0 and row['result'] >= 0 and row['bytes'] > 0
            pack_bytes = row['bytes']
        elif event.endswith('_request'):
            kind = event.split('_')[0]
            assert not pending, ('Overlapping core requests', index, pending)
            assert row['offset'] % 512 == row['bytes'] % 512 == 0
            assert 0 <= row['offset'] < row['offset'] + row['bytes'] <= pack_bytes
            assert row['result'] == 0
            if kind == 'read':
                assert row['bytes'] in (31 * 512, 32 * 512), row
            else:
                assert span is not None, ('Write without loaded track', index)
                assert span[0] <= row['offset']
                assert row['offset'] + row['bytes'] <= sum(span)
                assert 0 < row['bytes'] <= span[1]
                sizes[row['bytes']] += 1
                actual += row['bytes']
                whole += span[1]
            pending[kind] = row
        elif event.endswith('_ack'):
            kind = event.split('_')[0]
            assert kind in pending, ('ACK without request', index)
            request = pending.pop(kind)
            assert (row['offset'], row['bytes']) == (request['offset'], request['bytes'])
            assert row['result'] == 0
            if kind == 'read':
                span = (row['offset'], row['bytes'])
        elif event.endswith('_begin') and event != 'barrier_begin':
            kind = event[:-6]
            assert kind not in io, ('Overlapping backing-file operation', index)
            io[kind] = row
        elif event.endswith('_end') and event != 'barrier_end':
            kind = event[:-4]
            assert kind in io, ('Backing-file completion without begin', index)
            begin = io.pop(kind)
            assert (row['offset'], row['bytes']) == (begin['offset'], begin['bytes'])
            assert row['result'] == (row['bytes'] if kind in ('pread', 'pwrite') else 0)
    assert not pending and not io, ('Prefix ends during operation', pending, io)
    assert counts['write_ack'] and actual < whole, 'No reduced native writes observed'
    return dict(status='PASS_NATIVE_PACK_WINDOW_TRACE_PREFIX',
                trace_sha256=hashlib.sha256(data).hexdigest(), records=len(rows),
                first_ns=rows[0]['ns'], last_ns=last_ns, pack_bytes=pack_bytes,
                events=dict(sorted(counts.items())),
                core_write_sizes=dict(sorted(sizes.items())),
                core_write_bytes=actual,
                same_requests_whole_track_bytes=whole,
                core_write_byte_reduction=1 - actual / whole,
                scope='Complete observed request/ACK pairs in a live prefix. '
                      'Whole-track bytes use each write request\'s actual loaded '
                      'track span. This is a byte comparison, not a pinned '
                      'workload, elapsed speedup, final close, cold persistence '
                      'or fault-free sustained-operation qualification.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    result = inspect(args.input.read_bytes())
    args.output.write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result, indent=2))
