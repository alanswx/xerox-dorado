#!/usr/bin/env python3
"""Compare complete native RIOT behavior before connecting block RAM to a core.

Both engines retain every original port, timer/IRQ/direction/latch state and
all128 logical RAM bytes. Icarus gets a reversible, test-only lowering of its
unsupported unpacked-array reset assignments. Run that same lowering in both
engines; do not change production reset semantics or the guest clock cadence.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from prepare_riot_blockram import candidate,RIOT
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]


def lower(raw):
    original=raw;edits=[]
    for match in list(re.finditer(r"riot_ram <= '\{(.*?)\};",raw,re.S)):
        value=match[1];words=re.findall(r"8'h([0-9a-fA-F]{2})",value)
        if value=="128{8'h00}":words=['00']*128
        assert len(words)==128
        replacement='begin\n'+'\n'.join(f"riot_ram[{i}]<=8'h{word};"for i,word in enumerate(words))+'\nend'
        assert raw.count(match[0])==1;raw=raw.replace(match[0],replacement,1);edits.append((match[0],replacement))
    assert len(edits)==2
    restored=raw
    for before,after in reversed(edits):restored=restored.replace(after,before,1)
    assert restored==original
    return raw


def run(component,out):
    assert not out.exists()
    c=json.loads((component/'results.json').read_text());verify(component,c['files'])
    assert c['status']=='PREPARED_NATIVE_RIOT_BLOCK_RAM_COMPONENT'and not c['native_connected']
    stage=Path(c['stage']);m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    source=(stage/RIOT).read_text();old=module(source,'M6532_ce')
    prelude=source[:source.index('module M6532')]
    assert (component/'reference.sv').read_text()==prelude+old
    assert (component/'candidate.sv').read_text()==prelude+candidate(old)
    out.mkdir(parents=True);inputs=out/'inputs';inputs.mkdir()
    (inputs/'reference.sv').write_text(lower((component/'reference.sv').read_text()))
    for n in ['candidate.sv','ram.sv']:shutil.copy2(component/n,inputs/n)
    shutil.copy2(ROOT/'verilog/verilator/tb_riot_blockram.sv',inputs/'tb.sv')
    for n in [Path(__file__).name,'prepare_riot_blockram.py']:shutil.copy2(Path(__file__).with_name(n),out/n)
    state=dict(status='RUNNING',started=time.time(),scope=__doc__,component=str(component),
               component_state_sha256=sha(component/'results.json'),cases=[],
               reference_reset_lowering_test_only=True,all_original_non_RAM_state_and_all_logical_RAM_compared=True,
               files={str(q.relative_to(out)):sha(q)for q in out.rglob('*')if q.is_file()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for engine in ['verilator','icarus']:
        d=out/engine;d.mkdir();paths=[str(inputs/n)for n in ['reference.sv','candidate.sv','ram.sv','tb.sv']]
        commands=([['iverilog','-g2012','-s','tb_riot_blockram','-o',str(d/'test.vvp'),*paths],['vvp',str(d/'test.vvp')]]if engine=='icarus'else
                  [['verilator','--binary','--timing','--top-module','tb_riot_blockram','-Wno-fatal','-j','2','--Mdir',str(d/'obj'),'-o','test',*paths],[str(d/'obj/test')]])
        case=dict(engine=engine,actions=[]);state['cases'].append(case);save()
        for label,cmd in zip(['compile','test'],commands):
            with(d/(label+'.log')).open('x')as log:
                child=subprocess.Popen(cmd,stdout=log,stderr=log)
                action=dict(label=label,pid=child.pid,command=cmd,started=time.time());case['actions'].append(action);save();code=child.wait()
            action.update(returncode=code,finished=time.time(),log_sha256=sha(d/(label+'.log')));save()
            if code:break
        log=(d/'test.log').read_text()if(d/'test.log').exists()else''
        metrics=re.findall(r'^RIOT_COMPONENT_PASS .*$',log,re.M)
        passed=code==0 and len(metrics)==1 and ('checks=93155'if engine=='icarus'else'checks=60387')in metrics[0]
        case.update(passed=passed,metrics=metrics);save()
        assert passed,(engine,(d/('compile.log'if not log else'test.log')).read_text()[-1600:])
    state.update(status='PASS_COMPLETE_NATIVE_RIOT_BLOCK_RAM_COMPONENT',finished=time.time(),
                 native_BaseBoard_or_full_core_or_physical_timing_or_firmware_qualified=False)
    save();print(state['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['component','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();run(a.component.resolve(),a.output.resolve())
