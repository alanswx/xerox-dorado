#!/usr/bin/env python3
"""Four-state and abort/quarantine checks for the memory pause coordinator."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

HERE=Path(__file__).resolve().parent


def run(out):
    out.mkdir(parents=True,exist_ok=False)
    sources={n:(HERE/n).read_text()for n in ('memory_pause_barrier.sv','test_memory_pause_barrier.sv')}
    cases=[('candidate',None,None),
        ('ignore_raw_idle',"(raw_service_idle===1'b1);","1'b1;"),
        ('release_on_abort','phase<=QUARANTINE;','phase<=RUN;'),
        ('nonsticky_hold','assign frontend_hold=phase==FROZEN || phase==QUARANTINE;',
         'assign frontend_hold=(phase==FROZEN && frontends_drained) || phase==QUARANTINE;'),
        ('unsafe_ready',"(raw_service_idle===1'b1) && (abort===1'b0);","1'b1;")]
    results=[]
    for label,before,after in cases:
        case=out/label;case.mkdir();files=dict(sources)
        if before:assert before in files['memory_pause_barrier.sv'];files['memory_pause_barrier.sv']=files['memory_pause_barrier.sv'].replace(before,after)
        for n,s in files.items():(case/n).write_text(s)
        cmd=['iverilog','-g2012','-s','tb','-o',str(case/'test.vvp'),*[str(case/n)for n in files]]
        with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=30)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text)
        if before:assert result.returncode and 'FATAL'in text,text
        else:assert not result.returncode and 'MEMORY PAUSE FOURSTATE PASS'in text,text
        (case/'commands.json').write_text(json.dumps([cmd,['vvp',str(case/'test.vvp')]],indent=2)+'\n')
        print(label+': '+text.strip(),flush=True);results.append(dict(case=label,returncode=result.returncode))
    (out/'results.json').write_text(json.dumps(dict(status='PASS',cases=results,coordinator_required_wrapper_bits=2,
        inputs={n:hashlib.sha256(s.encode()).hexdigest()for n,s in sources.items()},
        limits='Coordinator control only; true machine reseed and real native device barriers are integration requirements.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.output.resolve())
