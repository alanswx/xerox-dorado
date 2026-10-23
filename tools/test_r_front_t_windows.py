#!/usr/bin/env python3
"""Expose original T latch windows, retaining closed storage/capture epochs.

The same MC10173 distinction used for R applies to T's32 input and16 visible
bits. The embedded original T RAM/parity arrays remain single owners. Compare
all844 physical Qs/9216 R RAM bits/100 clocks and ALL functional observations;
observe T parity from committed physical Q rather than a predicted open latch.
No added reset, guest cycle, state bit, native integration or timing waiver.
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
    marker=' assign state_q={held_q[843:572],current_read_q,held_q[539:0]};'
    added=''' // HM2.3/Table4 cadence; original k01/k02,j03/j04,i03/i04 MC10173s.
 // Store only the last closed T input/visible values. Current open Q is D.
 wire [31:0] current_t_inputs,closed_t_inputs_next;
 wire [15:0] current_t_visible,closed_t_visible_next;
 wire [3:0] current_t_address_h={held_q[80],held_q[81],held_q[82],held_q[83]};
 wire [3:0] current_t_address_l={held_q[84],held_q[85],held_q[86],held_q[87]};
 wire [31:0] current_t_data={
   pins_before[117] ? pins_before[126:119] : held_q[124+16*current_t_address_l+:8],
   pins_before[117] ? {8{pins_before[118]}} : held_q[116+16*current_t_address_h+:8],
   held_q[52] ? pins_before[107:100] : pins_before[91:84],
   held_q[36] ? pins_before[99:92] : pins_before[83:76]};
 wire [1:0] current_t_visible_n={~held_q[48],~held_q[32]};
 wire [1:0] current_t_bypass={!(held_q[99]|held_q[55]|held_q[54]|pins_before[117]),
                            !(held_q[93]|held_q[39]|held_q[38]|pins_before[117])};
 dorado_r_read_window_step t_windows({held_q[387:372],held_q[115:100]},current_read_n,latch_n,
   current_t_data,current_t_data,closed_t_inputs_next,current_t_inputs,,);
 assign current_t_visible[7:0]=current_t_visible_n[0] ? held_q[395:388] :
   (current_t_bypass[0] ? current_t_inputs[7:0] : current_t_inputs[23:16]);
 assign current_t_visible[15:8]=current_t_visible_n[1] ? held_q[403:396] :
   (current_t_bypass[1] ? current_t_inputs[15:8] : current_t_inputs[31:24]);
 assign closed_t_visible_next[7:0]=(!current_t_visible_n[0] && t_latch_n[0]) ? current_t_visible[7:0] : held_q[395:388];
 assign closed_t_visible_next[15:8]=(!current_t_visible_n[1] && t_latch_n[1]) ? current_t_visible[15:8] : held_q[403:396];
 assign state_q={held_q[843:572],current_read_q,held_q[539:404],current_t_visible,
   current_t_inputs[31:16],held_q[371:116],current_t_inputs[15:0],held_q[99:0]};'''
    raw=replace(raw,marker,added)
    raw=replace(raw,'assign held_next={state_next[843:572],closed_read_next,state_next[539:0]};',
        '''assign held_next={state_next[843:572],closed_read_next,state_next[539:404],closed_t_visible_next,
   closed_t_inputs_next[31:16],state_next[371:116],closed_t_inputs_next[15:0],state_next[99:0]};''')
    raw=replace(raw,'write_n,latch_n,t_latch_n,bypass,parity_ok,faults,r_read_n,r_visible_n,r_flags,rm_flags_n,',
                'write_n,latch_n,t_latch_n,bypass,child_parity_ok,child_faults,r_read_n,r_visible_n,r_flags,rm_flags_n,')
    marker=' wire [751:0] child_next;wire child_unsettled;'
    new=''' wire [3:0] child_parity_ok,child_faults;
 wire [1:0] physical_t_ok={(^state_q[403:396])^state_q[538],(^state_q[395:388])^state_q[534]};
 assign parity_ok={child_parity_ok[3],physical_t_ok[1],child_parity_ok[1],physical_t_ok[0]};
 assign faults={child_faults[3],!(current_t_bypass[1]|pins_before[117]|physical_t_ok[1]),
                child_faults[1],!(current_t_bypass[0]|pins_before[117]|physical_t_ok[0])};
'''
    raw=replace(raw,marker,new+marker)
    if mutation=='omit_t_close':
        raw=replace(raw,'current_t_data,current_t_data,closed_t_inputs_next,current_t_inputs,,);',
                    'current_t_data,current_t_data,,current_t_inputs,,);\n assign closed_t_inputs_next={held_q[387:372],held_q[115:100]};')
    if mutation=='hide_open_t':
        for half,out,held in [(0,'7:0','395:388'),(1,'15:8','403:396')]:
            expression=f'assign current_t_visible[{out}]=current_t_visible_n[{half}] ? held_q[{held}] :\n   (current_t_bypass[{half}] ? current_t_inputs[{"7:0"if half==0 else"15:8"}] : current_t_inputs[{"23:16"if half==0 else"31:24"}]);'
            raw=replace(raw,expression,f'assign current_t_visible[{out}]=held_q[{held}];')
    return raw


def t_file(raw):
    # Existing RAM write window consumes current physical k01/k02 Q. An
    # open-latch prediction at a new control epoch is not a second write port.
    raw=replace(raw,'state_next[116+16*address_h+:8]=state_next[107:100];','state_next[116+16*address_h+:8]=state_q[107:100];')
    return replace(raw,'state_next[124+16*address_l+:8]=state_next[115:108];','state_next[124+16*address_l+:8]=state_q[115:108];')


def t_parity(raw):
    raw=replace(raw,"{3'b0,~^t_next[107:100]}","{3'b0,~^state_q[107:100]}")
    return replace(raw,"{3'b0,~^t_next[115:108]}","{3'b0,~^state_q[115:108]}")


def bench(raw):
    old='''     if({rwn,rln,rtln,rbp,rok,rfault,rrr,rrv}!==eo || rrf!==rf || rrf_n!==rf_n)
       $fatal(1,"R front operand output mismatch check=%0d",checks);'''
    pre='''     // Capture/window controls still agree BEFORE committing owners.
     // T parity is a live physical observation, sampled after commit below.
     if({rwn,rln,rtln,rbp,rrr,rrv}!=={eo[19:12],eo[3:0]} || rrf!==rf || rrf_n!==rf_n)
       $fatal(1,"R front operand gate output mismatch check=%0d",checks);'''
    raw=replace(raw,old,pre)
    marker='     if({raw_parity,raw_data}!=={ref_raw_parity,ref_raw_data})'
    assert raw.count(marker)==1
    # Retain the COMPLETE20-bit functional assertion at actual physical Q.
    raw=raw.replace(marker,'     eo={wn,ln,tln,bp,ok,fault,rr,rv};\n'+old+'\n'+marker,1)
    return raw


def run(source,out,engine,mutation):
    parent=json.loads((source/'run-state.json').read_text());assert parent['status']=='PASS_R_FRONT_TASK_EVENT' and not parent['mutation']
    assert parent['actions'][-1]['returncode']==0 and parent['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
    for row in parent['source_files']:assert sha(source/row['path'])==row['output_sha256']
    build=next(a for a in parent['actions']if a['command'][0]in ['iverilog','verilator'])
    paths=[Path(n)for n in build['command']if n.endswith(('.sv','.v'))]
    out.mkdir(parents=True,exist_ok=False);records=[];copied=[]
    transforms={'candidate.sv':lambda raw:candidate(raw,mutation),'dorado_local_t_file_step.sv':t_file,
                'dorado_local_t_parity_step.sv':t_parity,'tb_r_front_local.sv':bench}
    for p in paths:
        rel=p.relative_to(source);q=out/rel;q.parent.mkdir(parents=True,exist_ok=True);original=p.read_text()
        raw=transforms[p.name](original)if p.name in transforms else original;q.write_text(raw);copied.append(q)
        records.append(dict(path=str(rel),parent_sha256=sha(p),output_sha256=sha(q),changed=original!=raw))
    assert sum(r['changed']for r in records)==4
    (out/'source-run-state.json').write_bytes((source/'run-state.json').read_bytes())
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),engine=engine,mutation=mutation,parent=str(source),source_files=records,
               parent_run_state_sha256=sha(source/'run-state.json'),actions=[],closed_latch_encoding_bits=80,
               full_state_comparison='all844 physical Qs after owner commit',all_functional_observations_preserved=True,
               functional_gates_compared_before_and_after_commit=True,independent_original_unchanged=True,RAM_bits=9216,clock_channels=100,
               native_integrated=False,physical_timing_qualified=False,assembly_permitted=False)
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
    state.update(status='PASS_R_FRONT_T_WINDOWS'if not rc and m else'R_FRONT_T_WINDOWS_FAILED',summary=m[1]if m else None,finished=time.time());save()
    print(state['status'],flush=True);return rc if rc else(0 if m else 1)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],default='icarus');p.add_argument('--mutation',choices=['omit_t_close','hide_open_t']);a=p.parse_args()
    sys.exit(run(a.source.resolve(),a.output.resolve(),a.engine,a.mutation))
