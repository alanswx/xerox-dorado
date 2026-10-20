#!/usr/bin/env python3
"""Independent exact parent/source review of targeted network_step fanout.

Two assignments to the one registered signal are the only permitted policy
change. Reverse them and invoke the readiness/native pin/constraint review.
No logical state, clock, schema, IO delay or timing exception may change.
"""
import argparse
from functools import lru_cache
import hashlib
import json
from pathlib import Path
from generate_serial_boards import ROOT,raw_receipt
import audit_ready_path as prior
import audit_native_memory_pause as memory


def sha(raw):return hashlib.sha256(raw).hexdigest()


def check(ok,why):
    if not ok:raise ValueError(why)


@lru_cache(maxsize=1)
def parent(reports,native_memory=False):
    report=reports/('2026-10-20-snapshot-native-memory-pause-rejected'if native_memory else'2026-10-19-snapshot-serial-ready')
    record=json.loads(raw_receipt(report,'terminal-fit/'+('memory-pause-integration.json'if native_memory else'ready-path-integration.json')))
    files={n:raw_receipt(report,'terminal-fit/'+n)for n in record['source_hashes']}
    check(len(files)==(660 if native_memory else 659),'parent count differs')
    for n,raw in files.items():check(sha(raw)==record['source_hashes'][n],'damaged parent '+n)
    return record,files


def audit(project,reports,overrides=None):
    overrides=overrides or {}
    def read(name):
        raw=overrides.get(name,(project/name).read_bytes());return raw.encode()if isinstance(raw,str)else raw
    record=json.loads(read('fanout-integration.json'));native_memory=record.get('native_memory_pause',False)
    old,files=parent(reports,native_memory)
    check(record['verified_parent_inputs']==old['source_hashes'],'parent hashes differ')
    limit=256 if native_memory else 64
    check((record['target'],record['max_fanout'])==('emu|network_step',limit),'target/limit differs')
    check(record['capacity_only']and not record['assembly_permitted']and not record['real_pause_barriers'],'capacity scope differs')
    suffix='''
# Target the registered guest-advance source; retain all real timing checks.
set_instance_assignment -name MAX_FANOUT 64 -to "emu|network_step"
set_instance_assignment -name IGNORE_MAX_FANOUT_ASSIGNMENTS OFF -to "emu|network_step"
'''
    if native_memory:suffix=suffix.replace('MAX_FANOUT 64','MAX_FANOUT 256')
    qsf=read('core/Dorado.qsf');check(qsf.count(suffix.encode())==1,'targeted fanout policy differs')
    inverse=qsf.replace(suffix.encode(),b'',1)
    for name,raw in files.items():
        expected=raw
        if Path(name).suffix in ('.qsf','.qpf','.qip','.sdc','.tcl'):expected=raw.decode().replace(old['project'],record['project']).encode()
        actual=inverse if name=='core/Dorado.qsf'else read(name)
        check(actual==expected,'unapproved source/clock/constraint edit: '+name)
        check(record['source_hashes'].get(name)==sha(read(name)),'actual source hash differs: '+name)
    if native_memory:
        rolled=json.loads(read('memory-pause-integration.json'));rolled['source_hashes']['core/Dorado.qsf']=sha(inverse)
        upstream=memory.audit(project,reports,{'core/Dorado.qsf':inverse,'memory-pause-integration.json':json.dumps(rolled)})
        upstream.update(status='PASS_NATIVE_MEMORY_FANOUT_SOURCE_ONLY',verified_policy_parent_inputs=660,
            changed_policy_files=['core/Dorado.qsf'],target='emu|network_step',max_fanout=256,new_policy_state_bits=0,
            limits='Only fanout policy differs from Serial69. Actual application, capacity and all-condition timing pending; foreign service ACKs remain virtual.')
        return upstream
    rolled=json.loads(read('ready-path-integration.json'));rolled['source_hashes']['core/Dorado.qsf']=sha(inverse)
    upstream=prior.audit(project,reports,{'core/Dorado.qsf':inverse,'ready-path-integration.json':json.dumps(rolled)})
    return dict(status='PASS_TARGETED_FANOUT_SOURCE_ONLY',scope=__doc__,verified_parent_inputs=659,
                original_pin_connections=upstream['original_pin_connections'],schema_id=upstream['schema_id'],
                changed_logic_files=[],changed_policy_files=['core/Dorado.qsf'],target='emu|network_step',max_fanout=64,
                new_state_bits=0,new_clocks=0,new_ports=0,constraints_unchanged=True,real_pause_barriers=False,
                limits='Source/policy proof only; check actual assignment application and full fit/timing before adoption.')


def controls(project,reports):
    native_memory=json.loads((project/'fanout-integration.json').read_text()).get('native_memory_pause',False)
    limit=256 if native_memory else 64
    cases=[('wrong_target','core/Dorado.qsf','-to "emu|network_step"','-to "emu|machine_enable"'),
           ('wrong_limit','core/Dorado.qsf','MAX_FANOUT '+str(limit),'MAX_FANOUT 128'),
           ('ignored_limit','core/Dorado.qsf','IGNORE_MAX_FANOUT_ASSIGNMENTS OFF','IGNORE_MAX_FANOUT_ASSIGNMENTS ON'),
           ('new_exception','core/Dorado.qsf','-name MAX_FANOUT '+str(limit),'-name SDC_FILE hidden.sdc\nset_instance_assignment -name MAX_FANOUT '+str(limit)),
           ('lost_init','core/Dorado.sv',' && storage_initialized && snapshot_frontends_ready',' && snapshot_frontends_ready'),
           ('fake_service','core/Dorado.sv','.services_held(board_state_services_held)'if native_memory else'.services_held(CAPACITY_SERVICES_HELD)',".services_held(5'h1f)")]
    out=[]
    for label,name,before,after in cases:
        raw=(project/name).read_text();check(before in raw,'missing control anchor');damaged=raw.replace(before,after,1)
        record=json.loads((project/'fanout-integration.json').read_text());record['source_hashes'][name]=sha(damaged.encode())
        try:audit(project,reports,{name:damaged,'fanout-integration.json':json.dumps(record)})
        except ValueError as e:out.append(dict(case=label,rejected=True,reason=str(e),damaged_metadata_rehashed=True))
        else:raise AssertionError('damaged fanout/source accepted: '+label)
    return out


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--project',type=Path,required=True);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    result=audit(a.project.resolve(),a.reports.resolve());result['failure_controls']=controls(a.project.resolve(),a.reports.resolve())
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
