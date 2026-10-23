#!/usr/bin/env python3
"""Separate physical RbBypass from next-capture lookahead.

Export the original existing BEFORE cone's current comparison, through two
adapters, without another state bit or RAM/cone copy. Preserve the prediction
assertion and add independent physical-value checks BEFORE and AFTER owner
commit. All844Qs/9216RAM/100clocks/functional checks remain. No native timing,
original60ns, real consumer/capture qualification or hardware speed claim.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import time

from test_r_front_task_event import replace

ROOT=Path(__file__).resolve().parents[1]


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def candidate(raw,mutation):
    raw=replace(raw,'wire [31:0] child_address;wire [3:0] child_select;wire [1:0] child_forward;',
                'wire [31:0] child_address;wire [3:0] child_select;wire [1:0] child_forward,physical_forward;')
    raw=replace(raw,'child_source,child_fa,child_ready);','child_source,child_fa,child_ready,physical_forward);')
    old='assign forwarded_bypass=POST_MIR_PINS==2 ? settled_forward : child_forward;'
    new='''// This export is the physical current wire. settled_forward remains
 // the next-capture forecast and is independently checked in simulation.
 assign forwarded_bypass=physical_forward;'''
    if mutation=='invert_current':new=new.replace('=physical_forward;','=physical_forward^2\'b11;')
    return replace(raw,old,new)


def front(raw):
    raw=replace(raw,'output wire [201:0] source_pins,output wire [7:0] fa_n);',
                'output wire [201:0] source_pins,output wire [7:0] fa_n,output wire [1:0] current_forwarded_bypass);')
    return replace(raw,'.clocks(before_clocks),.front_data(before_front_data));',
                   '.clocks(before_clocks),.front_data(before_front_data),.forwarded_bypass(current_forwarded_bypass));')


def ready(raw):
    raw=replace(raw,'output wire [29:0] ready_flags);','output wire [29:0] ready_flags,output wire [1:0] current_forwarded_bypass);')
    return replace(raw,'child_source,fa_n);','child_source,fa_n,current_forwarded_bypass);')


def bench(raw):
    # Keep the existing forecast assertion against the reference after its
    # event. The newly exported physical signal is checked independently at
    # BOTH epochs; remapping the export cannot hide a wrong forecast.
    raw=replace(raw,'ea=address;es=select_n;ef=forwarded;','ea=address;es=select_n;ef=dut.settled_forward;')
    marker='     expected=n;predicted=events;'
    assert raw.count(marker)==1
    raw=raw.replace(marker,'     if(forwarded!==rforwarded)$fatal(1,"Physical RbBypass mismatch BEFORE commit check=%0d actual=%b original=%b",checks,forwarded,rforwarded);\n'+marker,1)
    marker='     if({raw_parity,raw_data}!=={ref_raw_parity,ref_raw_data})'
    assert raw.count(marker)==1
    return raw.replace(marker,'     if(forwarded!==rforwarded)$fatal(1,"Physical RbBypass mismatch AFTER commit check=%0d actual=%b original=%b",checks,forwarded,rforwarded);\n'+marker,1)


def run(source,out,engine,mutation):
    parent=json.loads((source/'run-state.json').read_text());assert parent['status']=='PASS_R_FRONT_CURRENT_MEMORY'and not parent['mutation']
    assert parent['actions'][-1]['returncode']==0 and parent['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
    for row in parent['source_files']:assert sha(source/row['path'])==row['output_sha256']
    build=next(a for a in parent['actions']if a['command'][0]in ['iverilog','verilator'])
    paths=[Path(n)for n in build['command']if n.endswith(('.sv','.v'))]
    out.mkdir(parents=True,exist_ok=False);records=[];copied=[]
    transforms={'candidate.sv':lambda raw:candidate(raw,mutation),'dorado_local_r_front_step.sv':front,
                'dorado_local_r_front_ready_step.sv':ready,'tb_r_front_local.sv':bench}
    for p in paths:
        rel=p.relative_to(source);q=out/rel;q.parent.mkdir(parents=True,exist_ok=True);original=p.read_text()
        raw=transforms[p.name](original)if p.name in transforms else original;q.write_text(raw);copied.append(q)
        records.append(dict(path=str(rel),parent_sha256=sha(p),output_sha256=sha(q),changed=original!=raw))
    assert sum(r['changed']for r in records)==4
    (out/'source-run-state.json').write_bytes((source/'run-state.json').read_bytes())
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),engine=engine,mutation=mutation,parent=str(source),source_files=records,
               parent_run_state_sha256=sha(source/'run-state.json'),actions=[],closed_latch_encoding_bits=80,
               full_state_comparison='all844 physical Qs after owner commit',physical_forwarding_before_and_after_commit_checked=True,
               next_capture_forecast_still_checked=True,original_reference_unchanged=True,
               RAM_bits=9216,clock_channels=100,native_integrated=False,physical_timing_qualified=False,assembly_permitted=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    defines=[x for x in build['command']if x.startswith('-D')]
    commands=[['iverilog','-g2012','-s','tb_r_front_local',*defines,'-o',str(out/'test.vvp'),*map(str,copied)],['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else[
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,'--Mdir',str(out/'obj'),'-o','test',*map(str,copied)],[str(out/'obj/test')]]
    save();rc=0
    for i,cmd in enumerate(commands):
        with(out/('compile.log'if i==0 else'test.log')).open('w')as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);row=dict(command=cmd,pid=p.pid,started=time.time());state['actions'].append(row);save()
            rc=p.wait();row.update(returncode=rc,finished=time.time());save()
        if rc:break
    text=(out/'test.log').read_text()if(out/'test.log').exists()else'';m=re.search(r'R_FRONT_LOCAL_PROBE PASS ([^\n]+)',text)
    state.update(status='PASS_R_FRONT_PHYSICAL_FORWARD'if not rc and m else'R_FRONT_PHYSICAL_FORWARD_FAILED',summary=m[1]if m else None,finished=time.time());save()
    print(state['status'],flush=True);return rc if rc else(0 if m else 1)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],default='icarus');p.add_argument('--mutation',choices=['invert_current']);a=p.parse_args()
    sys.exit(run(a.source.resolve(),a.output.resolve(),a.engine,a.mutation))
