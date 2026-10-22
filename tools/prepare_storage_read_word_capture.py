#!/usr/bin/env python3
"""Capture the requested SDRAM beat directly, preserving completion timing.

The measured corrected-native half-cycle path ends at rsp_data through the
last-beat mux. An optional mode writes rsp_data on the requested beat instead;
rsp_valid still waits for the complete BL8 drain. A preceding response has
already been consumed before a new miss can enter READ_WAIT. Invalid response
data can change earlier; valid data, backpressure, refresh, reset and physical
commands must remain equivalent. Native wrapper/build stays unchanged.
"""
import argparse
import hashlib
import json
from pathlib import Path

from prepare_storage_response_pipeline import replace_once

BACKEND='dorado_storage_sdram_burst.sv'
QUALIFIED_BACKEND='61f99c6a4013db1d0289882ec77157d2cb7388ffc15ab7c84202c0d5d62c4d38'


def transform(raw):
    assert hashlib.sha256(raw).hexdigest()==QUALIFIED_BACKEND
    s=raw.decode()
    s=replace_once(s,'parameter integer RESPONSE_OVERLAP=0',
                  'parameter integer RESPONSE_OVERLAP=0,\n    parameter integer CAPTURE_READ_WORD=0')
    s=replace_once(s,'            if(fill_beat==address[2:0])selected_data<=dq_capture;',
                  '''            if(fill_beat==address[2:0])begin
                // The response remains invalid until the original last-beat
                // edge. Capturing the requested beat removes a second data
                // mux from the falling-input to rising-response path.
                if(CAPTURE_READ_WORD)rsp_data<=dq_capture;
                else selected_data<=dq_capture;
            end''')
    s=replace_once(s,'                rsp_data<=address[2:0]==7 ? dq_capture : selected_data;',
                  '                if(!CAPTURE_READ_WORD)rsp_data<=address[2:0]==7 ? dq_capture : selected_data;')
    s=replace_once(s,'(RESPONSE_OVERLAP!=0 && RESPONSE_OVERLAP!=1))',
                  '(RESPONSE_OVERLAP!=0 && RESPONSE_OVERLAP!=1) ||\n'
                  '           (CAPTURE_READ_WORD!=0 && CAPTURE_READ_WORD!=1))')
    return s.encode()


def prepare(parent,out):
    assert not out.exists()
    r=json.loads((parent/'provenance.json').read_text())
    assert r['split_selection'] and r['parallel_ready_compare'] and r['request_lookahead']
    files={}
    for n,h in r['generated_hashes'].items():
        b=(parent/n).read_bytes();assert hashlib.sha256(b).hexdigest()==h,n;files[n]=b
    assert hashlib.sha256(files[BACKEND]).hexdigest()==QUALIFIED_BACKEND
    files[BACKEND]=transform(files[BACKEND])
    out.mkdir(parents=True)
    for n,b in files.items():(out/n).write_bytes(b)
    r.update(status='PREPARED_SELECTED_READ_CAPTURE_COMPONENT_UNQUALIFIED',scope=__doc__,capture_read_word=True,
             generated_hashes={n:hashlib.sha256(b).hexdigest()for n,b in files.items()},
             immediate_parent_provenance_sha256=hashlib.sha256((parent/'provenance.json').read_bytes()).hexdigest(),
             response_valid_timing_unchanged=True,invalid_response_data_may_change_earlier=True,
             native_fit_or_firmware_qualified=False,hardware_changed=False)
    (out/'provenance.json').write_text(json.dumps(r,indent=2)+'\n')
    return r


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();r=prepare(a.parent.resolve(),a.output.resolve());print(r['status'])
