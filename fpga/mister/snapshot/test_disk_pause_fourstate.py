#!/usr/bin/env python3
"""Disk release/quarantine and the actual memory/network/disk coordinator chain."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

HERE=Path(__file__).resolve().parent


def run(out):
    out.mkdir(parents=True,exist_ok=False)
    unit={n:(HERE/n).read_text()for n in('disk_pause_barrier.sv','test_disk_pause_barrier.sv')}
    chain={n:(HERE/n).read_text()for n in('disk_pause_barrier.sv','network_pause_barrier.sv','memory_pause_barrier.sv','test_services_pause_chain.sv')}
    cases=[('candidate',unit,None,None,None,None),
        ('ignore_disk_latch',unit,'disk_pause_barrier.sv',"(device_held===1'b1);","1'b1;",'unknown/incomplete disk drain qualified'),
        ('early_ready',unit,'disk_pause_barrier.sv',"phase==FROZEN && (owner_hold===1'b1)","(phase==DRAIN || phase==FROZEN) && (owner_hold===1'b1)",'disk drain staging differs'),
        ('early_global_release',unit,'disk_pause_barrier.sv','assign hold=requested || phase!=RUN;','assign hold=requested || (phase!=RUN && phase!=RELEASE);','disk latch release lost guest hold'),
        ('release_on_fault',unit,'disk_pause_barrier.sv','phase<=QUARANTINE;','phase<=RUN;','disk revocation coincident with release escaped quarantine'),
        ('unknown_owner',unit,'disk_pause_barrier.sv',"wire requested=(owner_hold!==1'b0);",'wire requested=owner_hold;','unknown disk owner failed open'),
        ('chain',chain,None,None,None,None),
        ('chain_early_memory_release',chain,'test_services_pause_chain.sv','owner_hold || network_hold || disk_hold','owner_hold || network_hold','network release thawed held disk')]
    results=[]
    for label,source,target,before,after,diagnostic in cases:
        case=out/label;case.mkdir();files=dict(source)
        if target:assert before in files[target];files[target]=files[target].replace(before,after)
        for n,s in files.items():(case/n).write_text(s)
        cmd=['iverilog','-g2012','-s','tb','-o',str(case/'test.vvp'),*[str(case/n)for n in files]]
        (case/'commands.json').write_text(json.dumps([cmd,['vvp',str(case/'test.vvp')]],indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=40)
        text=result.stdout+result.stderr;(case/'test.log').write_text(text)
        if target:assert result.returncode and diagnostic in text,text
        else:assert not result.returncode and('SERVICES CHAIN PASS'if label=='chain'else'DISK COORDINATOR FOURSTATE PASS')in text,text
        print(label+': '+text.strip(),flush=True)
        results.append(dict(case=label,returncode=result.returncode,expected_failure=diagnostic,
            inputs={n:hashlib.sha256(s.encode()).hexdigest()for n,s in files.items()}))
    inputs=out/'executed-inputs';inputs.mkdir()
    for n,s in{**unit,**chain,Path(__file__).name:Path(__file__).read_text()}.items():(inputs/n).write_text(s)
    (out/'results.json').write_text(json.dumps(dict(status='PASS',cases=results,new_required_disk_coordinator_bits=3,
        inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},
        limits='Actual coordinators, independently controlled device ACKs. Disk/wire/physical-memory fixtures separately qualify devices; full Main coordination and restore remain required.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.output.resolve())
