#!/usr/bin/env python3
"""Stage the readiness-only experiment from immutable Serial64 terminal inputs.

Retain all original periods, IO/hold constraints, area options, board schema and
explicit virtual service contracts. No assembly or deployment is permitted.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
from generate_serial_boards import raw_receipt,ROOT
import generate_ready_path as change

REPORT='2026-10-19-snapshot-serial-chipwidth'


def sha(raw):return hashlib.sha256(raw).hexdigest()


def stage(out,reports,destination):
    parent=reports/REPORT;record=json.loads(raw_receipt(parent,'terminal-fit/chipwidth-integration.json'))
    oldpath=record['project'];newpath=str(destination or out);verified={}
    out.mkdir(parents=True,exist_ok=False)
    for name,digest in record['source_hashes'].items():
        raw=raw_receipt(parent,'terminal-fit/'+name);assert sha(raw)==digest,name
        verified[name]=digest
        if Path(name).suffix in ('.qsf','.qpf','.qip','.sdc','.tcl'):raw=raw.decode().replace(oldpath,newpath).encode()
        if name=='core/Dorado.sv':raw=change.transform(raw.decode()).encode()
        p=out/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
    assert len(verified)==659
    record.update(project=newpath,source_hashes={n:sha((out/n).read_bytes())for n in verified})
    (out/'chipwidth-integration.json').write_text(json.dumps(record,indent=2)+'\n')
    ready=dict(scope=__doc__,project=newpath,parent_report=REPORT,parent_project=oldpath,
               verified_parent_inputs=verified,source_hashes=record['source_hashes'],
               driver_sha256=sha(Path(__file__).read_bytes()),generator_sha256=sha(Path(change.__file__).read_bytes()),
               assembly_permitted=False,real_pause_barriers=False,capacity_only=True,
               limits='Readiness experiment only; wrapper capture, real service barriers, full snapshots and speed qualification pending.')
    (out/'ready-path-integration.json').write_text(json.dumps(ready,indent=2)+'\n')
    print('Staged readiness-only experiment:',len(verified),'exact parent inputs;',newpath,flush=True)


def run(out,quartus):
    record=json.loads((out/'ready-path-integration.json').read_text())
    assert record['capacity_only']and not record['assembly_permitted']and not record['real_pause_barriers']
    assert sha(Path(__file__).read_bytes())==record['driver_sha256']
    assert sha(Path(change.__file__).read_bytes())==record['generator_sha256']
    for name,digest in record['source_hashes'].items():assert sha((out/name).read_bytes())==digest,name
    assert not(out/'map.log').exists(),'observe the existing job; do not restart'
    for tool,args,label in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_sta',['Dorado'],'sta'),
                            ('quartus_sta',['-t','audit_native_clocks.tcl'],'clocks'),('quartus_sta',['-t','device-corners.tcl'],'corners'),
                            ('quartus_sta',['-t','audit-adapter-crossings.tcl'],'crossings')]:
        print('Serial65Ready',label,flush=True)
        with(out/(label+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=out/'core',stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Readiness experiment terminal; independent fit/timing review required; no assembly.',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--stage',type=Path);p.add_argument('--destination',type=Path)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--run',type=Path)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
    if a.stage:stage(a.stage.resolve(),a.reports.resolve(),a.destination)
    elif a.run:run(a.run.resolve(),a.quartus)
    else:p.error('Specify --stage or --run')
