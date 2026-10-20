#!/usr/bin/env python3
"""Build the two-delta MC1668/Native59 candidate on misterubuntu only.

Stages are sequential. Keep original constraints and reporting scripts; their
historical report prefixes are not artifact identities. Assembly is allowed
only after the summary and all requested detailed paths pass. Deployment is
separate and requires original-firmware and full memory/timing review.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import time


def run(project,quartus):
    record=json.loads((project/'mc1668-candidate.json').read_text())
    assert record['project']==str(project)and record['state_admin']=='INACTIVE'
    assert record['frequency_mhz']==50 and record['sysper']==4
    for name,digest in record['source_hashes'].items():
        assert hashlib.sha256((project/name).read_bytes()).hexdigest()==digest,name
    path=project/'run-state.json';assert not path.exists(),'Observe the existing job; do not restart'
    state=dict(status='RUNNING',builder_pid=os.getpid(),stages=[],started=time.time())
    def write():path.write_text(json.dumps(state,indent=2)+'\n')
    def stage(tool,args,label):
        print('MC1668_NATIVE',label,flush=True)
        with(project/(label+'.log')).open('x')as log:
            process=subprocess.Popen([str(quartus/tool),*args],cwd=project/'core',stdout=log,stderr=subprocess.STDOUT)
            row=dict(stage=label,pid=process.pid,started=time.time());state['stages'].append(row);write()
            code=process.wait();row.update(returncode=code,finished=time.time());write()
            if code:raise RuntimeError(label+' failed')
    write()
    try:
        for tool,args,label in [
            ('quartus_map',['--read_settings_files=on','--write_settings_files=off','Dorado'],'map'),
            ('quartus_fit',['--read_settings_files=on','--write_settings_files=off','Dorado'],'fit'),
            ('quartus_sta',['Dorado'],'sta'),
            ('quartus_sta',['-t','audit_native_clocks.tcl'],'clocks'),
            ('quartus_sta',['-t','device-corners.tcl'],'corners'),
            ('quartus_sta',['-t','audit-control-crossings.tcl'],'control-crossings'),
            ('quartus_sta',['-t','audit-contb-crossings.tcl'],'contb-crossings'),
            ('quartus_sta',['-t','audit-adapter-crossings.tcl'],'adapter-crossings')]:stage(tool,args,label)
        checks=re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)',(project/'core/output_files/Dorado.sta.summary').read_text())
        assert checks and all(float(v)>=0 for _,v in checks),checks
        paths={}
        for report in(project/'core').glob('native*-*.rpt'):
            values=re.findall(r'Worst case slack is (-?\d+\.\d+)',report.read_text(),re.I)
            if values:
                paths[report.name]=[float(v)for v in values]
                assert min(paths[report.name])>=0,(report.name,values)
        assert len(paths)>=196,('incomplete detailed timing reports',len(paths))
        (project/'preassembly-timing.json').write_text(json.dumps(dict(summary_checks=checks,detailed_paths=paths),indent=2)+'\n')
        stage('quartus_asm',['Dorado'],'asm')
        state.update(status='TERMINAL_REVIEW_REQUIRED',finished=time.time());write()
        print('MC1668_NATIVE terminal; source/memory/timing/firmware review required',flush=True)
    except BaseException:
        state.update(status='TERMINAL_FAILED',finished=time.time());write();raise


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--project',type=Path,required=True)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
    run(a.project.resolve(),a.quartus.resolve())
