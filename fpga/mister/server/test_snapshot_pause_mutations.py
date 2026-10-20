#!/usr/bin/env python3
"""Isolated actual-server pause gate and broken lifecycle/timer/MMIO controls.

Stage exact executed sources and a minimal byte-exact protocol corpus. No
production fake mapper is linked, no FPGA is touched and no CPU is emulated.
"""
import argparse
import hashlib
import json
from pathlib import Path
import resource
import shutil
import signal
import subprocess


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def no_core():resource.setrlimit(resource.RLIMIT_CORE,(0,0))


def run(out,root):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs'
    files=[*sorted((root/'dorado/include').glob('*.h')),root/'dorado/src/ethernet.c',root/'dorado/src/io.c']
    files +=[p for p in sorted((root/'fpga/mister/server').iterdir())if p.is_file()and(p.suffix in ('.c','.h','.py')or p.name=='Makefile')]
    files +=[root/p for p in ('dorado/worlds/aemu.eb','chm/bootfiles/CRTTEST.BOOT!1','chm/cedar/stp-root/Cedar6.1/VersionMap/CedarSource.VersionMap')]
    for p in files:
        q=inputs/p.relative_to(root);q.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(p,q)
    server=inputs/'fpga/mister/server';results=[]
    cases=[('candidate',None,None,None),
           ('lifecycle_reset','dorado_server.c','if(control_events & (DORADO_CTRL_RESUMED_EVENT|DORADO_CTRL_STOPPED_EVENT))','if(control_events)'),
           ('wall_time_jump','dorado_service_clock.h','c->paused=!!paused','c->paused=0'),
           ('unguarded_paused_mmio','dorado_ddr_ring.c','    if (r->paused) return -EAGAIN;','')]
    for label,target,before,after in cases:
        case=out/label;case.mkdir()
        for name in ('dorado_server.c','dorado_ddr_ring.c','dorado_service_clock.h'):
            text=(server/name).read_text()
            if name==target:assert text.count(before)==1,(label,before);text=text.replace(before,after,1)
            (case/name).write_text(text)
        # Match the production server's GNU C environment (clock_gettime,
        # timegm and strdup); strict ISO C hides these declarations on glibc.
        compile=['cc','-O2','-Wall','-Wextra','-std=gnu11','-I'+str(server),'-I'+str(inputs/'dorado/include')]
        if label=='unguarded_paused_mmio':
            compile +=[str(case/'dorado_ddr_ring.c'),str(server/'dorado_ddr_control.c'),str(server/'test_ddr_control.c'),'-o',str(case/'test')]
            command=[str(case/'test')]
        else:
            compile +=['-DDORADO_TEST_PAUSE_MAPPER',str(case/'dorado_server.c'),str(case/'dorado_ddr_ring.c'),str(server/'dorado_ddr_control.c'),str(server/'test_pause_mapper.c'),
                       str(inputs/'dorado/src/ethernet.c'),str(inputs/'dorado/src/io.c'),'-o',str(case/'test')]
            command=['python3',str(server/'test_snapshot_pause.py'),str(case/'test'),str(inputs)]
        (case/'commands.json').write_text(json.dumps(dict(build=compile,test=command),indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(compile,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(command,capture_output=True,text=True,timeout=20,preexec_fn=no_core)
        text=result.stdout+result.stderr;(case/'test.log').write_text(text)
        if label=='candidate':assert not result.returncode and 'SNAPSHOT PAUSE BSP PASS'in text,text
        elif label=='lifecycle_reset':assert result.returncode and any(message in text for message in ('continue reset EFTP','real server response timeout')),text
        elif label=='wall_time_jump':assert result.returncode and 'retry timer advanced across pause'in text,text
        else:assert result.returncode in (-signal.SIGSEGV,-signal.SIGBUS),(label,result.returncode,text)
        print(label+': '+('PASS'if not result.returncode else'REJECTED')+' rc='+str(result.returncode),flush=True)
        results.append(dict(case=label,returncode=result.returncode))
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,cases=results,
        executed_inputs={str(p.relative_to(inputs)):sha(p)for p in inputs.rglob('*')if p.is_file()},
        limits='Host process/MMIO-control qualification only; no serialized server heap, real FPGA/ARM service barrier or complete snapshot restore.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--root',type=Path,default=Path(__file__).resolve().parents[3]);a=p.parse_args();run(a.output.resolve(),a.root.resolve())
