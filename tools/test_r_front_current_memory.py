#!/usr/bin/env python3
"""Use the actual current RM/STK address for the single physical SRAM port.

Original SRAM sees current address/selection and physical dRm Q throughout its
write window. Drop the next-address preview and its duplicate-clock checks;
retain the actual controller, ALL independent100 clock/844-Q/9216-RAM/port
checks and source epochs. No new state, RAM copy, guest cycle or timing waiver.
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


def memory(raw,mutation):
    original=raw
    start=raw.index(' // Preview only the clock/address cone;')
    end=raw.index(' wire [201:0] source_before=',start)
    preview=raw[start:end]
    assert 'dorado_local_r_port_preview preview('in preview and'parity_events'in preview
    current=''' // One physical SRAM address epoch. Read and write see current
 // address/selection; the closing clock must not forecast the next task.
 wire [31:0] previous_address=old_address(state_q,pins_before);
 wire [3:0] previous_select=old_select(state_q,pins_before);
 assign port_address=previous_address;
 assign port_select_n=previous_select;
'''
    raw=raw[:start]+current+raw[end:]
    old=''' assign unsettled=child_unsettled | preview_unsettled | (resolved_address!=address) | (resolved_select!=select_n) |
   (parity_events!={clock_events[18],clock_events[13]});'''
    raw=replace(raw,old,' assign unsettled=child_unsettled;')
    before='state_next[555:540],previous_address,previous_select,file_read_data,file_read_parity);'
    after='state_q[555:540],previous_address,previous_select,file_read_data,file_read_parity);'
    raw=replace(raw,before,after)
    inverse=raw.replace(after,before,1).replace(' assign unsettled=child_unsettled;',old,1).replace(current,preview,1)
    assert inverse==original
    if mutation=='wrong_write_address':
        old='files(host_clk,advance,r_write_n,port_address,port_select_n,'
        raw=replace(raw,old,"files(host_clk,advance,r_write_n,port_address^32'h00000001,port_select_n,")
    return raw


def run(source,out,engine,mutation):
    parent=json.loads((source/'run-state.json').read_text());assert parent['status']=='PASS_R_FRONT_T_WINDOWS' and not parent['mutation']
    assert parent['actions'][-1]['returncode']==0 and parent['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
    for row in parent['source_files']:assert sha(source/row['path'])==row['output_sha256']
    build=next(a for a in parent['actions']if a['command'][0]in ['iverilog','verilator'])
    paths=[Path(n)for n in build['command']if n.endswith(('.sv','.v'))]
    out.mkdir(parents=True,exist_ok=False);records=[];copied=[]
    for p in paths:
        rel=p.relative_to(source);q=out/rel;q.parent.mkdir(parents=True,exist_ok=True);original=p.read_text()
        raw=memory(original,mutation)if p.name=='dorado_local_r_memory_step.sv'else original
        q.write_text(raw);copied.append(q);records.append(dict(path=str(rel),parent_sha256=sha(p),output_sha256=sha(q),changed=original!=raw))
    assert sum(r['changed']for r in records)==1
    assert sha(out/'inputs/tb_r_front_local.sv')==sha(source/'inputs/tb_r_front_local.sv')
    (out/'source-run-state.json').write_bytes((source/'run-state.json').read_bytes())
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),engine=engine,mutation=mutation,parent=str(source),source_files=records,
               parent_run_state_sha256=sha(source/'run-state.json'),actions=[],closed_latch_encoding_bits=80,
               full_state_comparison='all844 physical Qs after owner commit',independent_reference_and_bench_unchanged=True,
               actual_clocks_and_controller_preserved=True,deleted_preview_was_not_an_architectural_owner=True,
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
    state.update(status='PASS_R_FRONT_CURRENT_MEMORY'if not rc and m else'R_FRONT_CURRENT_MEMORY_FAILED',summary=m[1]if m else None,finished=time.time());save()
    print(state['status'],flush=True);return rc if rc else(0 if m else 1)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],default='icarus');p.add_argument('--mutation',choices=['wrong_write_address']);a=p.parse_args()
    sys.exit(run(a.source.resolve(),a.output.resolve(),a.engine,a.mutation))
