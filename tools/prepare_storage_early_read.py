#!/usr/bin/env python3
"""Return a captured requested SDRAM beat while the remaining BL8 drains.

The selected-word capture already exists. A default-off option exposes its
response earlier; state/physical commands and the full-burst request exclusion
remain unchanged. Cache validity and read_return still wait for the last beat.
No native wrapper selects this experiment.
"""
import argparse
import hashlib
import json
from pathlib import Path

from prepare_storage_response_pipeline import replace_once

CAPTURE_BACKEND = 'e959e44c120cf02a640ab63971c0b5da2f6d9bbcb02aa189f2319dc7ce22d3ff'
BACKEND = 'dorado_storage_sdram_burst.sv'


def transform(raw):
    assert hashlib.sha256(raw).hexdigest() == CAPTURE_BACKEND
    s=raw.decode()
    s=replace_once(s,'parameter integer CAPTURE_READ_WORD=0',
                  'parameter integer CAPTURE_READ_WORD=0,\n    parameter integer EARLY_READ_RESPONSE=0')
    s=replace_once(s,'                if(CAPTURE_READ_WORD)rsp_data<=dq_capture;',
                  '''                if(CAPTURE_READ_WORD)begin
                    rsp_data<=dq_capture;
                    // The external burst keeps draining in READ_WAIT. No
                    // successor request can enter until state returns IDLE.
                    if(EARLY_READ_RESPONSE && !aborted && !reset)
                        response_pending<=1;
                end''')
    s=replace_once(s,'                    response_pending<=1;line_valid[fill_line]<=1;',
                  '''                    // An early response may already have been consumed;
                    // the last beat must never issue a second completion.
                    if(!EARLY_READ_RESPONSE)response_pending<=1;
                    line_valid[fill_line]<=1;''')
    s=replace_once(s,'(CAPTURE_READ_WORD!=0 && CAPTURE_READ_WORD!=1))',
                  '(CAPTURE_READ_WORD!=0 && CAPTURE_READ_WORD!=1) ||\n'
                  '           (EARLY_READ_RESPONSE!=0 && EARLY_READ_RESPONSE!=1) ||\n'
                  '           (EARLY_READ_RESPONSE && !CAPTURE_READ_WORD))')
    s=replace_once(s,'            // Do not expose a completion until the entire physical burst drains.',
                  '            // Default mode completes at full drain; early mode at the requested beat.')
    return s.encode()


def prepare(parent,out):
    assert not out.exists()
    p=json.loads((parent/'provenance.json').read_text())
    assert p['capture_read_word'] and p['response_valid_timing_unchanged']
    files={n:(parent/n).read_bytes() for n in p['generated_hashes']}
    for n,b in files.items():assert hashlib.sha256(b).hexdigest()==p['generated_hashes'][n],n
    files[BACKEND]=transform(files[BACKEND])
    out.mkdir(parents=True)
    for n,b in files.items():(out/n).write_bytes(b)
    p.update(status='PREPARED_EARLY_SELECTED_READ_COMPONENT_UNQUALIFIED',scope=__doc__,
             immediate_parent_provenance_sha256=hashlib.sha256((parent/'provenance.json').read_bytes()).hexdigest(),
             generated_hashes={n:hashlib.sha256(b).hexdigest() for n,b in files.items()},
             early_read_response=True,response_valid_timing_unchanged=False,
             new_state_bits=0,physical_burst_drain_unchanged=True,
             native_fit_or_firmware_qualified=False)
    (out/'provenance.json').write_text(json.dumps(p,indent=2)+'\n')
    return p


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();print(prepare(a.parent.resolve(),a.output.resolve())['status'])
