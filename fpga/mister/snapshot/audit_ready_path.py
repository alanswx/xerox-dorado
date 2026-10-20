#!/usr/bin/env python3
"""Independent byte/pin/constraint review of the readiness-only full-core change.

Reverse the two exact combinational edits without importing the generator, then
apply the existing native source reviewer. Check every selected parent byte and
the real readiness/global-hold wiring. No timing or full-pause claim is made.
"""
import argparse
from functools import lru_cache
import hashlib
import json
from pathlib import Path
from generate_serial_boards import raw_receipt,ROOT
import audit_serial_chipwidth as prior


def sha(raw):return hashlib.sha256(raw).hexdigest()


def check(ok,why):
    if not ok:raise ValueError(why)


@lru_cache(maxsize=1)
def parent(reports):
    report=reports/'2026-10-19-snapshot-serial-chipwidth'
    record=json.loads(raw_receipt(report,'terminal-fit/chipwidth-integration.json'))
    files={n:raw_receipt(report,'terminal-fit/'+n)for n in record['source_hashes']}
    check(len(files)==659,'wrong parent count')
    for n,raw in files.items():check(sha(raw)==record['source_hashes'][n],'damaged parent: '+n)
    return record,files


def audit(project,reports,overrides=None):
    overrides=overrides or {}
    def read(n):
        value=overrides.get(n,(project/n).read_bytes());return value.encode()if isinstance(value,str)else value
    record=json.loads(read('ready-path-integration.json'));old,files=parent(reports)
    check(record['verified_parent_inputs']==old['source_hashes'],'parent hashes differ')
    check(record['capacity_only']and not record['assembly_permitted']and not record['real_pause_barriers'],'experiment scope differs')
    newcore=read('core/Dorado.sv').decode()
    before='wire machine_enable = ~reset && pack_ready && storage_ready && !board_state_hold;'
    after='wire snapshot_frontends_ready;\nassign CAPACITY_MEMORY_DRAINED=snapshot_frontends_ready;\nwire machine_enable = ~reset && pack_ready && storage_initialized && snapshot_frontends_ready && !board_state_hold;'
    check(newcore.count(after)==1,'CPU readiness guard differs')
    restored=newcore.replace(after,before,1)
    needle='.snapshot_memory_drained(snapshot_frontends_ready)'
    check(restored.count(needle)==1,'actual frontend aggregate not connected')
    restored=restored.replace(needle,'.snapshot_memory_drained(CAPACITY_MEMORY_DRAINED)',1)
    for name,before_bytes in files.items():
        expected=before_bytes
        if Path(name).suffix in ('.qsf','.qpf','.qip','.sdc','.tcl'):
            expected=expected.decode().replace(old['project'],record['project']).encode()
        actual=restored.encode()if name=='core/Dorado.sv'else read(name)
        check(actual==expected,'unapproved source/constraint edit: '+name)
        check(record['source_hashes'].get(name)==sha(read(name)),'staged hash differs: '+name)
    # These actual native edges connect the independently tested predicates.
    wrapper=read('src/build/rtl-executive/native10-im-parity/dorado_native_machine.sv').decode()
    bp=read(prior.BP).decode();msa=read(prior.PREFIX+'MSA__msa_board.sv').decode()
    check('assign storage_ready=storage_initialized && board_ready;'in wrapper,'initialization relation differs')
    check('assign storage_ready = &storage_board_ready;'in bp,'ready aggregate differs')
    check('assign snapshot_memory_drained=&snapshot_msa_drained;'in bp,'normal aggregate differs')
    for i in range(8):
        check(bp.count('.state_memory_drained(snapshot_msa_drained['+str(i)+'])')==1,'missing member drained')
        check(bp.count('.storage_ready(storage_board_ready['+str(i)+'])')==1,'missing member ready')
    check('wire state_hold=state_global_hold || (state_freeze && state_guest_held) || state_scan_hold_required;'in msa,'local hold relation differs')
    check('assign state_memory_drained=&state_storage_drained;'in msa,'member normal aggregate differs')
    upstream=prior.audit(project,reports,{'core/Dorado.sv':restored})
    return dict(status='PASS_READY_PATH_SOURCE_ONLY',scope=__doc__,verified_parent_inputs=659,
                changed_logic_files=['core/Dorado.sv'],schema_id=upstream['schema_id'],
                original_pin_connections=upstream['original_pin_connections'],constraints_unchanged=True,
                new_state_bits=0,new_clocks=0,new_ports=0,real_pause_barriers=False,
                limits='Frontend readiness is not whole SDRAM/arbiter drainage; timing, complete snapshots and hardware remain separate.')


def controls(project,reports):
    core=(project/'core/Dorado.sv').read_text();cases=[]
    for name,before,after in [
        ('lost_init',' && storage_initialized && snapshot_frontends_ready',' && snapshot_frontends_ready'),
        ('lost_hold','snapshot_frontends_ready && !board_state_hold','snapshot_frontends_ready'),
        ('constant_ready',' && snapshot_frontends_ready &&',' && 1\'b1 &&'),
        ('wrong_aggregate','.snapshot_memory_drained(snapshot_frontends_ready)','.snapshot_memory_drained(CAPACITY_MEMORY_DRAINED)'),
    ]:cases.append((name,{'core/Dorado.sv':core.replace(before,after,1)}))
    bp=(project/prior.BP).read_text();needle='.state_global_hold(snapshot_hold)'
    check(needle in bp,'control anchor missing');cases.append(('lost_member_global_hold',{prior.BP:bp.replace(needle,".state_global_hold(1'b0)",1)}))
    qsf=(project/'core/Dorado.qsf').read_text();cases.append(('new_timing_cut',{'core/Dorado.qsf':qsf+'\nset_global_assignment -name SDC_FILE hidden.sdc\n'}))
    out=[]
    for name,changes in cases:
        record=json.loads((project/'ready-path-integration.json').read_text())
        for path,value in changes.items():record['source_hashes'][path]=sha(value.encode())
        changes['ready-path-integration.json']=json.dumps(record)
        try:audit(project,reports,changes)
        except ValueError as e:out.append(dict(case=name,rejected=True,reason=str(e)))
        else:raise AssertionError('damaged rehashed source accepted: '+name)
    return out


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--project',type=Path,required=True)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args()
    result=audit(a.project.resolve(),a.reports.resolve())
    if a.mutations:result['failure_controls']=controls(a.project.resolve(),a.reports.resolve())
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
