#!/usr/bin/env python3
"""Factor the actual native ready comparator after response selection.

Compare both possible tags in parallel, then select the Boolean result at the
original response boundary. Keep every register, state priority, accepted
payload and ready capture edge unchanged. This is not a native qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path

from prepare_storage_response_pipeline import replace_once

GROUP = 'dorado_storage_group.sv'
GROUP_SHA = 'b126a0d33063c6615129998fedab20809666378d0e09b9bbf230fababf8ad81e'


def transform(raw):
    s = raw.decode()
    s = replace_once(s, 'reg [ADDR_BITS-1:0] pred_cache_addr;',
                     'reg pred_hold_match,pred_advance_match;')
    s = replace_once(s, 'pred_cache_addr=cache_addr;pred_write_done=write_done;',
                     'pred_hold_match=cache_addr==addr;\n'
                     '        pred_advance_match=cache_addr==addr_after_edge;\n'
                     '        pred_write_done=write_done;')
    s = replace_once(s, 'pred_waiting=0;pred_valid=1;pred_cache_addr=saved_addr;',
                     'pred_waiting=0;pred_valid=1;\n'
                     '            pred_hold_match=saved_addr==addr;\n'
                     '            pred_advance_match=saved_addr==addr_after_edge;')
    s = replace_once(s, 'pred_cache_addr==addr &&', 'pred_hold_match &&')
    s = replace_once(s, 'pred_cache_addr==addr_after_edge &&', 'pred_advance_match &&')
    return s.encode()


def prepare(generated, group, out):
    assert not out.exists()
    r = json.loads((generated / 'provenance.json').read_text())
    assert r['request_lookahead'] and r['parallel_lookup'] and r['defaults_off']
    files = {}
    for n, h in r['generated_hashes'].items():
        b = (generated / n).read_bytes()
        assert hashlib.sha256(b).hexdigest() == h
        files[n] = b
    original = group.read_bytes()
    assert hashlib.sha256(original).hexdigest() == GROUP_SHA
    files[GROUP] = transform(original)
    out.mkdir(parents=True)
    for n, b in files.items():
        (out / n).write_bytes(b)
    r.update(status='PREPARED_REQUEST_LOOKAHEAD_PARALLEL_READY_UNQUALIFIED', scope=__doc__,
             parent_hashes=dict(r['parent_hashes'], **{GROUP: GROUP_SHA}),
             generated_hashes={n: hashlib.sha256(b).hexdigest() for n, b in files.items()},
             immediate_lookahead_provenance_sha256=hashlib.sha256((generated / 'provenance.json').read_bytes()).hexdigest(),
             parallel_ready_compare=True, registers_or_capture_edges_changed=False)
    (out / 'provenance.json').write_text(json.dumps(r, indent=2) + '\n')
    return r


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['generated', 'group', 'output']:
        p.add_argument('--' + n, type=Path, required=True)
    a = p.parse_args()
    print(json.dumps(prepare(a.generated.resolve(), a.group.resolve(), a.output.resolve()), indent=2))
