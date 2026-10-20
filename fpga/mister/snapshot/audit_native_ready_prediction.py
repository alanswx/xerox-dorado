#!/usr/bin/env python3
"""Independently invert Serial73 wiring and replay frozen Serial72 native review.

Only the exact component-qualified combinational predictor, eight rising-edge
registers and their guarded Core enable are allowed. Retain clocks, board pins,
raw drain, reset/permission/release behavior and all unrelated source bytes.
"""
import argparse
from functools import lru_cache
import hashlib
import json
from pathlib import Path
from generate_serial_boards import ROOT,raw_receipt
import audit_native_disk_pause as prior

WRAPPER='src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
BACKPLANE='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
PREFIX='src/fpga/mister/snapshot_serial/'


def sha(raw):return hashlib.sha256(raw).hexdigest()


def check(ok,why):
    if not ok:raise ValueError(why)


def once(text,before,after):
    check(text.count(before)==1,'unexpected inverse anchor: '+before[:90]);return text.replace(before,after,1)


def inverse_core(text):
    text=once(text,'wire snapshot_frontends_ready,snapshot_frontends_cached;','wire snapshot_frontends_ready;')
    text=once(text,'storage_initialized && snapshot_frontends_cached && !board_state_hold;',
        'storage_initialized && snapshot_frontends_ready && !board_state_hold;')
    return once(text,'\n    .snapshot_frontends_cached(snapshot_frontends_cached),','')


def inverse_wrapper(text):
    text=once(text,'    output wire snapshot_frontends_ready,snapshot_frontends_cached,',
        '    output wire snapshot_frontends_ready,')
    return once(text,'\n    .snapshot_ready_cached(snapshot_frontends_cached),','')


def inverse_backplane(text):
    text=once(text,'    output wire snapshot_memory_drained,snapshot_ready_cached,',
        '    output wire snapshot_memory_drained,')
    block='''  // Required implementation state outside the physical MSA scan images.
  // Capture exactly each board's post-edge reset-free readiness. Live reset,
  // initialization, disk readiness and global hold still gate the Core enable.
  wire [7:0] snapshot_msa_flags_next;
  reg [7:0] snapshot_msa_flags_ready=8'b0;
  always @(posedge storage_clk)snapshot_msa_flags_ready<=snapshot_msa_flags_next;
  assign snapshot_ready_cached=&snapshot_msa_flags_ready;
'''
    text=once(text,block,'')
    for i in range(8):text=once(text,f'\n    .state_memory_flags_next(snapshot_msa_flags_next[{i}]),','')
    return text


@lru_cache(maxsize=1)
def parent(reports):
    report=reports/'2026-10-20-snapshot-native-disk-pause-source'
    record=json.loads(raw_receipt(report,'staged-source/disk-pause-integration.json'))
    files={n:raw_receipt(report,'staged-source/'+n)for n in record['source_hashes']}
    check(len(files)==662,'parent source count differs')
    for n,raw in files.items():check(sha(raw)==record['source_hashes'][n],'damaged frozen parent '+n)
    return record,files


def audit(project,reports,overrides=None):
    overrides=overrides or{}
    def read(n):
        raw=overrides.get(n,(project/n).read_bytes());return raw.encode()if isinstance(raw,str)else raw
    record=json.loads(read('ready-prediction-integration.json'));old,files=parent(reports)
    check(record['verified_parent_inputs']==old['source_hashes'],'parent hashes differ')
    check(set(record['source_hashes'])==set(files),'native source set differs')
    check(record['capacity_only']and not record['assembly_permitted']and not record['real_pause_barriers'],'qualification scope differs')
    check(record['new_required_wrapper_bits']==8 and record['new_clocks']==record['new_external_ports']==0,'new state/clock/port count differs')
    check(record['schema_id']=='af0b647cb43a3a62','physical board-only schema differs')
    report=reports/'2026-10-20-snapshot-ready-prediction'
    proofs={label:json.loads(raw_receipt(report,label+'/results.json'))for label in('fourstate','msa','eight-msa')}
    check(record['qualified_proofs']==proofs,'component qualifications differ')
    check(all(p['status']=='PASS'for p in proofs.values())and[len(proofs[x]['cases'])for x in proofs]==[8,3,3],'unqualified predictor')
    check(json.loads(raw_receipt(report,'notes/verification.json'))['status']=='PASS','extracted-source rebuild missing')
    assets={PREFIX+'MSA__msa_board.sv':'msa_board.sv',PREFIX+'MSA__storage_group.sv':'storage_group.sv'}
    inverse={};changed=[]
    for name,before in files.items():
        expected=before
        if Path(name).suffix in('.qsf','.qpf','.qip','.sdc','.tcl'):expected=before.decode().replace(old['project'],record['project']).encode()
        raw=read(name);restored=raw
        if name in assets:
            member=assets[name];qualified=raw_receipt(report,'eight-msa/executed-inputs/'+member)
            check(raw==qualified and sha(raw)==proofs['eight-msa']['executed_inputs'][member],'unqualified predictor asset: '+name)
            check(raw==raw_receipt(report,'msa/candidate/'+member),'single/eight-MSA predictor differs')
            restored=expected
        elif name=='core/Dorado.sv':restored=inverse_core(raw.decode()).encode()
        elif name==WRAPPER:restored=inverse_wrapper(raw.decode()).encode()
        elif name==BACKPLANE:restored=inverse_backplane(raw.decode()).encode()
        elif name==PREFIX+'assets.json':
            actual=json.loads(raw);original=json.loads(before)
            for path in assets:
                member=Path(path).name
                check(actual['assets'][member]==sha(read(path)),'MSA asset hash differs')
                actual['assets'][member]=original['assets'][member]
            check(actual==original,'unapproved asset metadata edit');restored=expected
        check(restored==expected,'unapproved native wiring/pin/clock/source edit: '+name)
        check(sha(raw)==record['source_hashes'][name],'actual source hash differs: '+name)
        inverse[name]=restored
        if raw!=expected:changed.append(name)
    rolled=dict(old);rolled.update(project=record['project'],source_hashes={n:sha(r)for n,r in inverse.items()})
    inverse['disk-pause-integration.json']=json.dumps(rolled).encode()
    upstream=prior.audit(project,reports,inverse)
    return dict(status='PASS_NATIVE_READY_PREDICTION_SOURCE',scope=__doc__,source_hashes=record['source_hashes'],
        verified_parent_inputs=662,actual_inputs=662,changed_parent_inputs=changed,
        original_pin_connections=upstream['original_pin_connections'],schema_id=upstream['schema_id'],
        new_required_wrapper_bits=8,new_clocks=0,new_external_ports=0,constraints_unchanged=True,
        ordinary_advance_phase_unchanged=True,raw_drain_unchanged=True,
        limits='Exact native source integration; state access, actual routed capacity/timing, complete snapshots and faster clocks remain open.')


def controls(project,reports):
    cases=[('wrong_register_phase',BACKPLANE,'always @(posedge storage_clk)snapshot_msa_flags_ready',
            'always @(negedge storage_clk)snapshot_msa_flags_ready'),
        ('gated_prediction_clock',BACKPLANE,'always @(posedge storage_clk)snapshot_msa_flags_ready',
            'always @(posedge sys_clk)snapshot_msa_flags_ready'),
        ('missing_member',BACKPLANE,'.state_memory_flags_next(snapshot_msa_flags_next[7])',
            '.state_memory_flags_next(snapshot_msa_flags_next[6])'),
        ('wrong_aggregate',BACKPLANE,'assign snapshot_ready_cached=&snapshot_msa_flags_ready;',
            'assign snapshot_ready_cached=&snapshot_msa_flags_ready[6:0];'),
        ('cached_instead_of_drain',BACKPLANE,'assign snapshot_memory_drained=&snapshot_msa_drained;',
            'assign snapshot_memory_drained=&snapshot_msa_flags_ready;'),
        ('wrong_wrapper_link',WRAPPER,'.snapshot_ready_cached(snapshot_frontends_cached)',
            '.snapshot_ready_cached(snapshot_frontends_ready)'),
        ('lost_initialization','core/Dorado.sv',' && storage_initialized && snapshot_frontends_cached',
            ' && snapshot_frontends_cached'),
        ('lost_disk_guard','core/Dorado.sv','~reset && pack_ready && storage_initialized',
            '~reset && storage_initialized'),
        ('lost_owner_hold','core/Dorado.sv','snapshot_frontends_cached && !board_state_hold;',
            'snapshot_frontends_cached;'),
        ('wrong_core_input','core/Dorado.sv','.snapshot_frontends_cached(snapshot_frontends_cached)',
            '.snapshot_frontends_cached(snapshot_frontends_ready)'),
        ('predictor_changed',PREFIX+'MSA__storage_group.sv','waiting && rsp_valid,req_write',
            'waiting && rsp_valid,write_pending'),
        ('board_clock_changed',BACKPLANE,'dorado_serial_MSA #(.SYSPER(SYSPER)) b_msa (',
            'dorado_serial_MSA #(.SYSPER(2)) b_msa ('),
        ('timing_relaxed','core/Dorado.sdc','derive_pll_clocks',
            'create_clock -period 1000 [get_ports FAKE_CLOCK]\nderive_pll_clocks')]
    results=[]
    for label,name,before,after in cases:
        damaged=once((project/name).read_text(),before,after)
        record=json.loads((project/'ready-prediction-integration.json').read_text());record['source_hashes'][name]=sha(damaged.encode())
        try:audit(project,reports,{name:damaged,'ready-prediction-integration.json':json.dumps(record)})
        except(ValueError,AssertionError)as e:results.append(dict(case=label,rejected=True,diagnostic=str(e),metadata_rehashed=True))
        else:raise ValueError('damaged readiness integration accepted: '+label)
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--project',type=Path,required=True)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    result=audit(a.project.resolve(),a.reports.resolve());result['failure_controls']=controls(a.project.resolve(),a.reports.resolve())
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k!='source_hashes'}))
