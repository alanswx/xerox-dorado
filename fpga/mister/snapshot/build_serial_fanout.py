#!/usr/bin/env python3
"""Target only network_step fanout in the terminal Serial65 nineteen-board core.

All RTL, clocks, periods, IO constraints, schema and virtual service contracts
are preserved. One register's MAX_FANOUT=64 is enforced. No assembly/deployment.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
from generate_serial_boards import ROOT,raw_receipt

POLICY='''
# Target the registered guest-advance source; retain all real timing checks.
set_instance_assignment -name MAX_FANOUT 64 -to "emu|network_step"
set_instance_assignment -name IGNORE_MAX_FANOUT_ASSIGNMENTS OFF -to "emu|network_step"
'''


def sha(raw):return hashlib.sha256(raw).hexdigest()


def stage(out,reports,destination):
    report=reports/'2026-10-19-snapshot-serial-ready'
    parent=json.loads(raw_receipt(report,'terminal-fit/ready-path-integration.json'))
    state=json.loads(raw_receipt(report,'terminal-fit/terminal-state.json'))
    assert not state['builder_live']and not state['project_processes']and state['prepared_inputs_changed']==[]
    review=json.loads(raw_receipt(report,'source-audit.json'));assert review['status']=='PASS_READY_PATH_SOURCE_ONLY'and len(review['failure_controls'])==6
    old=parent['project'];new=str(destination or out);out.mkdir(parents=True,exist_ok=False)
    for name,digest in parent['source_hashes'].items():
        raw=raw_receipt(report,'terminal-fit/'+name);assert sha(raw)==digest,name
        if Path(name).suffix in ('.qsf','.qpf','.qip','.sdc','.tcl'):raw=raw.decode().replace(old,new).encode()
        if name=='core/Dorado.qsf':raw+=POLICY.encode()
        p=out/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
    assert len(parent['source_hashes'])==659
    hashes={n:sha((out/n).read_bytes())for n in parent['source_hashes']}
    for metadata in ('chipwidth-integration.json','ready-path-integration.json'):
        record=json.loads(raw_receipt(report,'terminal-fit/'+metadata));record.update(project=new,source_hashes=hashes)
        (out/metadata).write_text(json.dumps(record,indent=2)+'\n')
    record=dict(scope=__doc__,project=new,parent_report=report.name,parent_project=old,
                verified_parent_inputs=parent['source_hashes'],source_hashes=hashes,builder_sha256=sha(Path(__file__).read_bytes()),
                target='emu|network_step',max_fanout=64,capacity_only=True,assembly_permitted=False,real_pause_barriers=False,
                limits='Physical optimization experiment; real service barriers and complete machine snapshots remain pending.')
    (out/'fanout-integration.json').write_text(json.dumps(record,indent=2)+'\n');print('Staged exact Serial65 RTL with targeted network_step fanout64;',new,flush=True)


def run(out,quartus):
    record=json.loads((out/'fanout-integration.json').read_text())
    assert record['capacity_only']and not record['assembly_permitted']and not record['real_pause_barriers']
    assert sha(Path(__file__).read_bytes())==record['builder_sha256']
    for name,digest in record['source_hashes'].items():assert sha((out/name).read_bytes())==digest,name
    assert not(out/'map.log').exists(),'observe existing run; do not restart'
    for tool,args,label in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_sta',['Dorado'],'sta'),
                            ('quartus_sta',['-t','audit_native_clocks.tcl'],'clocks'),('quartus_sta',['-t','device-corners.tcl'],'corners'),
                            ('quartus_sta',['-t','audit-adapter-crossings.tcl'],'crossings')]:
        print('Serial66Fanout',label,flush=True)
        with(out/(label+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=out/'core',stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Fanout experiment terminal; independent capacity/timing/actual-assignment review required; no assembly.',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--stage',type=Path);p.add_argument('--destination',type=Path);p.add_argument('--run',type=Path)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
    if a.stage:stage(a.stage.resolve(),a.reports.resolve(),a.destination)
    elif a.run:run(a.run.resolve(),a.quartus)
    else:p.error('Specify --stage or --run')
