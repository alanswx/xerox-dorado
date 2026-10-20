#!/usr/bin/env python3
"""Invert only cache padding/type and scan-length edits to qualified MSA bytes.

All other helper/board bytes remain exact. Every original guest field is kept.
The changed implementation representation requires its new physical inventory
and behavioral oracle; this is not compatibility with older snapshots.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
from generate_serial_boards import raw_receipt
from classify_msa_chipwidth import expected_fields
import audit_msa_compact as parent

ROOT=Path(__file__).resolve().parents[3]
EDITS=[('reg [15:0] cache_data=0;','reg [WIDTH-1:0] cache_data=0;'),
       ('STATE_BITS=22+3*WIDTH','STATE_BITS=6+4*WIDTH'),
       ('cache_data<=req_data;','cache_data<=req_data[WIDTH-1:0];'),
       ('cache_data<=rsp_data;','cache_data<=rsp_data[WIDTH-1:0];')]


def sha(raw):return hashlib.sha256(raw).hexdigest()


def audit(case,manifest,reports,overrides=None):
    overrides=overrides or {};read=lambda n:overrides[n]if n in overrides else(case/n).read_bytes()
    root=reports/'2026-10-19-snapshot-msa-compact';ref=lambda n:raw_receipt(root,'msa-v2/candidate/'+n)
    old=json.loads(raw_receipt(root,'msa-v2/manifest.json'))
    board=read('msa_board.sv');assert sha(board)==manifest['generated_sha256']
    text=board.decode();assert text.count('.STATE_BITS(1538)')==1
    rolled_board=text.replace('.STATE_BITS(1538)','.STATE_BITS(1650)',1).encode()
    assert rolled_board==ref('msa_board.sv'),'board/pin/equation differs beyond scan length'
    rolled={'msa_board.sv':rolled_board}
    for name,digest in manifest['cloned_sources'].items():
        raw=read(name);assert sha(raw)==digest,'helper digest differs'
        if name=='storage_group.sv':
            text=raw.decode();previous=ref(name).decode()
            for before,after in EDITS:
                assert text.count(after)==previous.count(before)>0,'cache width edit differs'
                text=text.replace(after,before)
            raw=text.encode()
        assert raw==ref(name),'helper differs beyond cache type/assignment/scan width: '+name
        rolled[name]=raw
    parent.audit(case,old,reports,rolled)
    assert (manifest['logical_state_bits'],manifest['ff_bits'],manifest['frontend_bits'])==(1538,866,672)
    _,fields=expected_fields();expected={r['path']:r['width']for r in fields};positions=[]
    for row in manifest['layout']:
        assert expected.pop(row['path'],None)==row['width'],'chip-width field geometry differs'
        positions.extend(range(row['lsb'],row['lsb']+row['width']))
    assert not expected and positions==list(range(1538)),'missing/aliased chip-width scan state'
    assert manifest['scan']==dict(state_bits=1538,words=86,word_bits=18,last_word_bits=8,completion_edges=1538)
    return dict(status='PASS_CHIPWIDTH_SOURCE_SCOPE',scope=__doc__,actual_total_bits=1538,original_ff_bits=866,
        actual_frontend_bits=672,fields=378,removed_implementation_padding_bits=112,
        limits='No native catalog/barrier, complete-core fit or hardware qualification.')


def controls(case,manifest,reports):
    cases=[('too_narrow','storage_group.sv','reg [WIDTH-1:0] cache_data=0;','reg [WIDTH-2:0] cache_data=0;'),
        ('guest_pipeline','storage_group.sv','read_pipe<=read_value[WIDTH-1:0];','read_pipe<=write_data;'),
        ('raw_payload','storage_group.sv','assign req_data={{(16-WIDTH){1\'b0}},write_data};','assign req_data=16\'b0;'),
        ('cross_group','msa_board.sv','.addr(dram_addr_0),.addr_will_change(dram_change_0),','.addr(dram_addr_0),.addr_will_change(dram_change_1),'),
        ('old_scan','msa_board.sv','.STATE_BITS(1538)','.STATE_BITS(1650)'),
        ('hidden_ff','cell_SN74166.sv','endmodule',"reg hidden;\nendmodule")]
    results=[]
    for name,target,before,after in cases:
        raw=(case/target).read_bytes();assert before.encode()in raw;damaged=raw.replace(before.encode(),after.encode(),1);record=copy.deepcopy(manifest)
        if target=='msa_board.sv':record['generated_sha256']=sha(damaged)
        else:record['cloned_sources'][target]=sha(damaged)
        try:audit(case,record,reports,{target:damaged})
        except AssertionError as error:results.append(dict(case=name,rejected=True,diagnostic=str(error),damaged_metadata_rehashed=True))
        else:raise AssertionError('damaged chip-width source accepted: '+name)
    for name in ('missing','alias','old_width','old_bounds'):
        broken=copy.deepcopy(manifest)
        if name=='missing':broken['layout'].pop()
        elif name=='alias':broken['layout'][1]['lsb']-=1
        elif name=='old_width':next(r for r in broken['layout']if r['path']=='u_storage_0.cache_data')['width']=16
        else:broken['scan']['words']=92
        try:audit(case,broken,reports)
        except AssertionError as error:results.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged chip-width schema accepted: '+name)
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('tests',type=Path);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    manifest=json.loads((a.tests/'manifest.json').read_text());case=a.tests/'candidate'
    result=audit(case,manifest,a.reports);result['failure_controls']=controls(case,manifest,a.reports)
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(status=result['status'],failure_controls=len(result['failure_controls']))))
