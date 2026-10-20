#!/usr/bin/env python3
"""Independent exact parent/source review of targeted network_step global routing.

One routing assignment to the registered signal are the only permitted policy
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


def sha(raw):return hashlib.sha256(raw).hexdigest()


def check(ok,why):
    if not ok:raise ValueError(why)


@lru_cache(maxsize=1)
def parent(reports):
    report=reports/'2026-10-19-snapshot-serial-ready'
    record=json.loads(raw_receipt(report,'terminal-fit/ready-path-integration.json'))
    files={n:raw_receipt(report,'terminal-fit/'+n)for n in record['source_hashes']}
    check(len(files)==659,'parent count differs')
    for n,raw in files.items():check(sha(raw)==record['source_hashes'][n],'damaged parent '+n)
    return record,files


def audit(project,reports,overrides=None):
    overrides=overrides or {}
    def read(name):
        raw=overrides.get(name,(project/name).read_bytes());return raw.encode()if isinstance(raw,str)else raw
    record=json.loads(read('global-integration.json'));old,files=parent(reports)
    check(record['verified_parent_inputs']==old['source_hashes'],'parent hashes differ')
    check((record['target'],record['global_signal'])==('emu|network_step','Global Clock'),'target/routing differs')
    check(record['capacity_only']and not record['assembly_permitted']and not record['real_pause_barriers'],'capacity scope differs')
    suffix='''
# Route the registered guest-advance output globally; keep all real timing checks.
set_instance_assignment -name GLOBAL_SIGNAL "Global Clock" -to "emu|network_step"
'''
    qsf=read('core/Dorado.qsf');check(qsf.count(suffix.encode())==1,'targeted global-routing policy differs')
    inverse=qsf.replace(suffix.encode(),b'',1)
    for name,raw in files.items():
        expected=raw
        if Path(name).suffix in ('.qsf','.qpf','.qip','.sdc','.tcl'):expected=raw.decode().replace(old['project'],record['project']).encode()
        actual=inverse if name=='core/Dorado.qsf'else read(name)
        check(actual==expected,'unapproved source/clock/constraint edit: '+name)
        check(record['source_hashes'].get(name)==sha(read(name)),'actual source hash differs: '+name)
    rolled=json.loads(read('ready-path-integration.json'));rolled['source_hashes']['core/Dorado.qsf']=sha(inverse)
    upstream=prior.audit(project,reports,{'core/Dorado.qsf':inverse,'ready-path-integration.json':json.dumps(rolled)})
    return dict(status='PASS_GLOBAL_SIGNAL_SOURCE_ONLY',scope=__doc__,verified_parent_inputs=659,
                original_pin_connections=upstream['original_pin_connections'],schema_id=upstream['schema_id'],
                changed_logic_files=[],changed_policy_files=['core/Dorado.qsf'],target='emu|network_step',global_signal='Global Clock',
                new_state_bits=0,new_clocks=0,new_ports=0,constraints_unchanged=True,real_pause_barriers=False,
                limits='Source/policy proof only; check actual assignment application and full fit/timing before adoption.')


def controls(project,reports):
    cases=[('wrong_target','core/Dorado.qsf','-to "emu|network_step"','-to "emu|machine_enable"'),
           ('wrong_routing','core/Dorado.qsf','GLOBAL_SIGNAL "Global Clock"','GLOBAL_SIGNAL "Regional Clock"'),
           ('duplicate_policy','core/Dorado.qsf','set_instance_assignment -name GLOBAL_SIGNAL','set_instance_assignment -name GLOBAL_SIGNAL "Global Clock" -to "emu|network_step"\nset_instance_assignment -name GLOBAL_SIGNAL'),
           ('new_exception','core/Dorado.qsf','-name GLOBAL_SIGNAL','-name SDC_FILE hidden.sdc\nset_instance_assignment -name GLOBAL_SIGNAL'),
           ('lost_init','core/Dorado.sv',' && storage_initialized && snapshot_frontends_ready',' && snapshot_frontends_ready'),
           ('fake_service','core/Dorado.sv','.services_held(CAPACITY_SERVICES_HELD)',".services_held(5'h1f)")]

    out=[]
    for label,name,before,after in cases:
        raw=(project/name).read_text();check(before in raw,'missing control anchor');damaged=raw.replace(before,after,1)
        record=json.loads((project/'global-integration.json').read_text());record['source_hashes'][name]=sha(damaged.encode())
        try:audit(project,reports,{name:damaged,'global-integration.json':json.dumps(record)})
        except ValueError as e:out.append(dict(case=label,rejected=True,reason=str(e),damaged_metadata_rehashed=True))
        else:raise AssertionError('damaged global/source accepted: '+label)
    return out


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--project',type=Path,required=True);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    result=audit(a.project.resolve(),a.reports.resolve());result['failure_controls']=controls(a.project.resolve(),a.reports.resolve())
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
