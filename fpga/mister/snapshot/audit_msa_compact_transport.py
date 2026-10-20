#!/usr/bin/env python3
"""Exact inverse of compact transport bounds/module edits; owner/UIO unchanged.

All five service domains, deadline, quarantine and parser-reset ownership are
retained. Actual eight-board routing is tested separately from native barriers.
"""
import argparse
import json
from pathlib import Path
from generate_serial_boards import raw_receipt
import audit_msa_transport as prior
import audit_msa_compact as member

ROOT=Path(__file__).resolve().parents[3]


def audit(case,manifest,reports,overrides=None):
    overrides=overrides or {};read=lambda n:overrides.get(n,(case/n).read_bytes()).decode()
    reference=reports/'2026-10-19-snapshot-msa-transport';ref=lambda n:raw_receipt(reference,'simulation-v5/candidate/'+n).decode()
    fabric=read('board_state_fabric_msa.sv');assert fabric.count("index[7:0]<8'd92")==1
    rolled=fabric.replace("index[7:0]<8'd92","index[7:0]<8'd150",1)
    assert rolled==ref('board_state_fabric_msa.sv'),'owner differs beyond compact word bound'
    uio=read('board_state_uio_msa.sv');assert uio==ref('board_state_uio_msa.sv'),'UIO differs'
    group=read('msa_state_group_compact.sv')
    for a,b in [('module dorado_msa_state_group_compact(','module dorado_msa_state_group('),
                ("index[7:0]<8'd92","index[7:0]<8'd150"),('word0..91.','word0..149.')]:
        assert group.count(a)==1;group=group.replace(a,b,1)
    assert group==ref('msa_state_group.sv'),'member router differs beyond compact bound/module name'
    prior.audit(rolled,uio)
    member.audit(case,manifest,reports,{k:v for k,v in overrides.items()if k in ['msa_board.sv',*manifest['cloned_sources']]})
    return dict(status='PASS_COMPACT_TRANSPORT_SOURCE',scope=__doc__,legacy_owner_uio_unchanged_after_inverse=True,
        member_router_unchanged_after_inverse=True,word_limit=92,deadline=4096,service_domains=5,
        limits='No native service barriers or complete snapshot qualification.')


def controls(case,manifest,reports):
    cases=[('outer_bounds','board_state_fabric_msa.sv',"index[7:0]<8'd92","index[7:0]<8'd150"),
           ('router_bounds','msa_state_group_compact.sv',"index[7:0]<8'd92","index[7:0]<8'd150"),
           ('router_live_owner','msa_state_group_compact.sv','unit_done[owner]','unit_done[index[10:8]]'),
           ('unknown_hold','msa_state_group_compact.sv',"children_holding=(unit_hold_required!==8'b0)",'children_holding=|unit_hold_required'),
           ('schema','board_state_uio_msa.sv',"6:if(din!=SCHEMA_ID[63:48])valid_packet<=0;",'6:begin end'),
           ('deadline','board_state_fabric_msa.sv','RESPONSE_LIMIT=4096','RESPONSE_LIMIT=2048')]
    result=[]
    for name,path,before,after in cases:
        raw=(case/path).read_bytes();assert raw.count(before.encode())==1;damaged=raw.replace(before.encode(),after.encode(),1)
        try:audit(case,manifest,reports,{path:damaged})
        except AssertionError as error:result.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged compact transport accepted: '+name)
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('tests',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');a=p.parse_args()
    manifest=json.loads((a.tests/'manifest.json').read_text());case=a.tests/'candidate'
    result=audit(case,manifest,a.reports);result['failure_controls']=controls(case,manifest,a.reports)
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(status=result['status'],failure_controls=len(result['failure_controls']))))
