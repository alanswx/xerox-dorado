#!/usr/bin/env python3
"""Prepare an unqualified, default-off adjacent-beat SDRAM reply experiment.

Consume the preceding early reply while accepting exactly the read whose word
is arriving now in the SAME physical burst. The physical command address and
full burst/cache/refresh drain remain unchanged. No write is accepted early.
No native wrapper selects this experiment.
"""
import argparse
import hashlib
import json
from pathlib import Path

from prepare_storage_response_pipeline import replace_once

BACKEND = 'dorado_storage_sdram_burst.sv'
PARENT_SHA256 = '22f617b61f7153dfcb879c1b287bff65276e076ecadc9a4c7f83aa220f16227d'


def transform(raw):
    assert hashlib.sha256(raw).hexdigest() == PARENT_SHA256
    s = raw.decode()
    s = replace_once(s, 'parameter integer EARLY_READ_RESPONSE=0',
                     'parameter integer EARLY_READ_RESPONSE=0,\n'
                     '    parameter integer BURST_READ_SUCCESSOR=0')
    s = replace_once(s, '    assign req_ready=initialized && state==IDLE &&\n'
                     '        (!response_pending || (RESPONSE_OVERLAP && rsp_ready)) &&\n'
                     '        !refresh_due && !reset;', '''    wire [25:0] offered_address=26'(req_addr);
    // Only an actual preceding completion permits another owner. The
    // arriving beat is already in dq_capture; no stale or future word mux.
    wire burst_successor_ready=BURST_READ_SUCCESSOR && state==READ_WAIT &&
        !aborted && response_pending && rsp_ready && !req_write &&
        offered_address[25:3]==address[25:3] && offered_address[2:0]==fill_beat;
    assign req_ready=initialized && !refresh_due && !reset &&
        ((state==IDLE && (!response_pending || (RESPONSE_OVERLAP && rsp_ready))) ||
         burst_successor_ready);''')
    s = replace_once(s, '            if(fill_beat==address[2:0])begin', '''            if(req_valid && req_ready)begin
                // Clear-before-case priority replaces the consumed reply.
                // Keep address/data/writing as the original physical request.
                rsp_data<=dq_capture;
                response_pending<=1;
            end
            if(fill_beat==address[2:0])begin''')
    s = replace_once(s, '(EARLY_READ_RESPONSE && !CAPTURE_READ_WORD))',
                     '(EARLY_READ_RESPONSE && !CAPTURE_READ_WORD) ||\n'
                     '           (BURST_READ_SUCCESSOR!=0 && BURST_READ_SUCCESSOR!=1) ||\n'
                     '           (BURST_READ_SUCCESSOR && (!EARLY_READ_RESPONSE || !RESPONSE_OVERLAP)))')
    return s.encode()


def prepare(parent, output):
    assert not output.exists(), 'Inspect the existing experiment; do not overwrite it'
    p = json.loads((parent / 'provenance.json').read_text())
    assert p['early_read_response'] and p['physical_burst_drain_unchanged']
    files = {n: (parent / n).read_bytes() for n in p['generated_hashes']}
    for n, raw in files.items():
        assert hashlib.sha256(raw).hexdigest() == p['generated_hashes'][n], n
    files[BACKEND] = transform(files[BACKEND])
    output.mkdir(parents=True)
    for n, raw in files.items():
        (output / n).write_bytes(raw)
    p.update(status='PREPARED_BURST_READ_SUCCESSOR_UNQUALIFIED', scope=__doc__,
             immediate_parent_provenance_sha256=hashlib.sha256((parent / 'provenance.json').read_bytes()).hexdigest(),
             parent_hashes=p['generated_hashes'],
             generated_hashes={n: hashlib.sha256(raw).hexdigest() for n, raw in files.items()},
             burst_read_successor=True, new_state_bits=0,
             native_fit_or_firmware_qualified=False)
    (output / 'provenance.json').write_text(json.dumps(p, indent=2) + '\n')
    return p


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['parent', 'output']:
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    print(prepare(args.parent.resolve(), args.output.resolve())['status'])
