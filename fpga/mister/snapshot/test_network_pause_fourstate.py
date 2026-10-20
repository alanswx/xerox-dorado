#!/usr/bin/env python3
"""Four-state network pause/release/quarantine controls, with compiled faults."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

HERE=Path(__file__).resolve().parent


def run(out):
    out.mkdir(parents=True,exist_ok=False)
    sources={n:(HERE/n).read_text()for n in('network_pause_barrier.sv','test_network_pause_barrier.sv')}
    cases=[('candidate',None,None),
        ('ignore_parser_latch',"(parser_held===1'b1);","1'b1;"),
        ('early_ready','phase==FROZEN && (owner_hold===1\'b1)',"(phase==SETTLE || phase==FROZEN) && (owner_hold===1'b1)"),
        ('early_release','assign hold=requested || phase!=RUN;','assign hold=requested || (phase!=RUN && phase!=RELEASE);'),
        ('release_on_fault','phase<=QUARANTINE;','phase<=RUN;'),
        ('unknown_owner',"wire requested=(owner_hold!==1'b0);",'wire requested=owner_hold;')]
    results=[]
    for label,before,after in cases:
        case=out/label;case.mkdir();files=dict(sources)
        if before:
            assert before in files['network_pause_barrier.sv']
            files['network_pause_barrier.sv']=files['network_pause_barrier.sv'].replace(before,after)
        for name,text in files.items():(case/name).write_text(text)
        cmd=['iverilog','-g2012','-s','tb','-o',str(case/'test.vvp'),*[str(case/n)for n in files]]
        with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=40)
        text=result.stdout+result.stderr;(case/'test.log').write_text(text)
        if before:assert result.returncode and'FATAL'in text,text
        else:assert not result.returncode and'NETWORK PAUSE FOURSTATE PASS'in text,text
        (case/'commands.json').write_text(json.dumps([cmd,['vvp',str(case/'test.vvp')]],indent=2)+'\n')
        print(label+': '+text.strip(),flush=True)
        results.append(dict(case=label,returncode=result.returncode,inputs={n:hashlib.sha256(s.encode()).hexdigest()for n,s in files.items()}))
    chain_sources={n:(HERE/n).read_text()for n in('network_pause_barrier.sv','memory_pause_barrier.sv','test_network_memory_pause.sv')}
    for label,bad in(('memory_chain',False),('memory_chain_early_release',True)):
        case=out/label;case.mkdir();files=dict(chain_sources)
        if bad:
            before='.owner_hold(owner_hold || network_hold)';assert files['test_network_memory_pause.sv'].count(before)==1
            files['test_network_memory_pause.sv']=files['test_network_memory_pause.sv'].replace(before,'.owner_hold(owner_hold)')
        for name,text in files.items():(case/name).write_text(text)
        cmd=['iverilog','-g2012','-s','tb','-o',str(case/'test.vvp'),*[str(case/n)for n in files]]
        with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=40)
        text=result.stdout+result.stderr;(case/'test.log').write_text(text)
        if bad:assert result.returncode and'memory released before parser latch'in text,text
        else:assert not result.returncode and'NETWORK MEMORY CHAIN PASS'in text,text
        (case/'commands.json').write_text(json.dumps([cmd,['vvp',str(case/'test.vvp')]],indent=2)+'\n')
        print(label+': '+text.strip(),flush=True)
        results.append(dict(case=label,returncode=result.returncode,inputs={n:hashlib.sha256(s.encode()).hexdigest()for n,s in files.items()}))
    (out/'results.json').write_text(json.dumps(dict(status='PASS',cases=results,required_top_state_bits=5,
        inputs={n:hashlib.sha256(s.encode()).hexdigest()for n,s in {**sources,**chain_sources}.items()},
        limits='Controller only; physical DDR visibility, full Main coordinator and complete restore remain required.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.output.resolve())
