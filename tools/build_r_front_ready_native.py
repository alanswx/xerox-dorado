#!/usr/bin/env python3
"""Fit the experimental full native Ready/front replacement on misterubuntu.

Retain Native59 constraints, four-condition and crossing reports. Do not
assemble or deploy: detailed timing/memory and actual firmware review remain
required. All stages share one project/database and execute sequentially.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import time


def run(project, quartus, manifest='r-front-ready-native.json'):
    assert manifest in ('r-front-ready-native.json','sdram-burst-native.json','storage-ready-native.json','storage-board-native.json','pack-window-native.json')
    r=json.loads((project/manifest).read_text())
    assert r['project']==str(project) and r['state_admin']=='INACTIVE'
    if manifest in ('storage-board-native.json','pack-window-native.json'):
        assert r['frequency_mhz']=={57142857:57.142857,66666667:66.666667}[r['frequency_hz']]
    else:
        assert r['frequency_mhz']==50
    assert r['sysper']==4
    for n,h in r['source_hashes'].items():
        assert hashlib.sha256((project/n).read_bytes()).hexdigest()==h,n
    path=project/'run-state.json'
    assert not path.exists(),'Observe the existing job; never restart from a polling timeout'
    state=dict(status='RUNNING',builder_pid=os.getpid(),started=time.time(),stages=[],
               builder_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),assembly_permitted=False)
    def write():
        temp=path.with_suffix('.json.tmp')
        temp.write_text(json.dumps(state,indent=2)+'\n')
        os.replace(temp,path)
    write()
    try:
        stages=[('quartus_map',['--read_settings_files=on','--write_settings_files=off','Dorado'],'map'),
                ('quartus_fit',['--read_settings_files=on','--write_settings_files=off','Dorado'],'fit'),
                ('quartus_sta',['Dorado'],'sta'),
                ('quartus_sta',['-t','audit_native_clocks.tcl'],'clocks'),
                ('quartus_sta',['-t','device-corners.tcl'],'corners'),
                ('quartus_sta',['-t','audit-control-crossings.tcl'],'control-crossings'),
                ('quartus_sta',['-t','audit-contb-crossings.tcl'],'contb-crossings'),
                ('quartus_sta',['-t','audit-adapter-crossings.tcl'],'adapter-crossings')]
        if manifest in ('storage-ready-native.json','storage-board-native.json','pack-window-native.json'):
            stages.append(('quartus_sta',['-t','audit-storage-ready.tcl'],'storage-ready'))
        for tool,args,label in stages:
            print('R_FRONT_READY_NATIVE',label,flush=True)
            with(project/(label+'.log')).open('x')as log:
                p=subprocess.Popen([str(quartus/tool),*args],cwd=project/'core',stdout=log,stderr=subprocess.STDOUT)
                row=dict(stage=label,pid=p.pid,started=time.time());state['stages'].append(row);write()
                code=p.wait();row.update(returncode=code,finished=time.time());write()
                assert code==0,(label,code)
        checks=re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)',(project/'core/output_files/Dorado.sta.summary').read_text())
        assert checks,'No all-device summary checks'
        reports={}
        for report in(project/'core').glob('native*-*.rpt'):
            values=re.findall(r'Worst case slack is (-?\d+\.\d+)',report.read_text(),re.I)
            reports[report.name]=dict(slacks=list(map(float,values)),sha256=hashlib.sha256(report.read_bytes()).hexdigest())
        # Keep empty/missing targeted paths visible for migration review.
        # A changed hierarchy is not evidence that an old exception applies.
        result=dict(summary_checks=checks,detailed_reports=reports,
                    all_summary_slacks_nonnegative=all(float(v)>=0 for _,v in checks),
                    all_reported_path_slacks_nonnegative=all(v>=0 for r in reports.values()for v in r['slacks']),
                    detailed_scope_review_required=True,assembly_permitted=False)
        (project/'timing-observation.json').write_text(json.dumps(result,indent=2)+'\n')
        state.update(status='TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED',finished=time.time())
        write()
        print(state['status'],'no assembly/deployment',flush=True)
    except BaseException:
        state.update(status='TERMINAL_FAILED',finished=time.time());write();raise


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--project',type=Path,required=True)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    p.add_argument('--manifest',choices=['r-front-ready-native.json','sdram-burst-native.json','storage-ready-native.json','storage-board-native.json','pack-window-native.json'],default='r-front-ready-native.json')
    a=p.parse_args();run(a.project.resolve(),a.quartus.resolve(),a.manifest)
