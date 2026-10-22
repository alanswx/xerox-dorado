#!/usr/bin/env python3
"""Require rejection of damaged copies of an actual native trace prefix."""
import argparse
import copy
import hashlib
import json
from pathlib import Path

from inspect_pack_window_trace import inspect


def run(source, output):
    data = source.read_bytes()
    positive = inspect(data)
    original = [json.loads(line) for line in data.splitlines()]
    write = next(i for i, r in enumerate(original) if r['event'] == 'write_request')
    ack = next(i for i, r in enumerate(original) if r['event'] == 'write_ack')
    pwrite = next(i for i, r in enumerate(original) if r['event'] == 'pwrite_end')
    fsync = next(i for i, r in enumerate(original) if r['event'] == 'fsync_end')

    def outside(rows):
        rows[write]['offset'] = rows[0]['bytes'] - 512
        rows[write]['bytes'] = 512

    def wrong_ack(rows):
        rows[ack]['offset'] += 512

    def short_write(rows):
        rows[pwrite]['result'] -= 1

    def failed_sync(rows):
        rows[fsync]['result'] = -1

    def lost_trace(rows):
        rows.append(dict(rows[-1], event='trace_overflow', result=1))

    def missing_ack(rows):
        del rows[ack]

    def backwards_time(rows):
        rows[write]['ns'] = 0

    cases = []
    for name, mutate in [('outside-loaded-track', outside), ('wrong-ACK-offset', wrong_ack),
                         ('short-backing-write', short_write), ('failed-fsync', failed_sync),
                         ('lost-trace-records', lost_trace), ('missing-ACK', missing_ack),
                         ('nonmonotonic-time', backwards_time)]:
        rows = copy.deepcopy(original)
        mutate(rows)
        damaged = b''.join((json.dumps(r) + '\n').encode() for r in rows)
        try:
            inspect(damaged)
        except AssertionError as error:
            cases.append(dict(name=name, rejected=True, reason=str(error)))
        else:
            raise AssertionError('Accepted damaged trace: ' + name)
    assert source.read_bytes() == data
    assert inspect(data) == positive
    result = dict(status='PASS_NATIVE_PACK_WINDOW_TRACE_REJECTION_CONTROLS',
                  source_sha256=hashlib.sha256(data).hexdigest(), cases=cases,
                  original_unchanged=True)
    output.write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], len(cases))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    run(args.input, args.output)
