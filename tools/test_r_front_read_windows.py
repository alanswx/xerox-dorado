#!/usr/bin/env python3
"""Integrate closed R-input storage and one current SRAM read port into844Qs.

Experimental full-graph integration. Keep the independent original reference,
stimulus, all100 clock checks,9216 RAM checks and functional-port assertions.
The32 latch storage bits now encode the last closed value; compare their LIVE
physical Qs after committing the owner state, rather than comparing the closed
encoding with an open original latch. No extra RAM port/copy or guest cycle.
This is not native integration, a phase-arrival proof or a timing qualification.
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

ROOT=Path(__file__).resolve().parents[1]


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def replace(raw,a,b):
    assert raw.count(a)==1,(a,raw.count(a))
    return raw.replace(a,b,1)


def transform_candidate(raw):
    raw=replace(raw,'input wire [843:0] state_q','input wire [843:0] held_q')
    raw=replace(raw,'output wire [843:0] state_next','output wire [843:0] held_next')
    raw=replace(raw,'output wire [93:0] control_flags','output wire [93:0] control_flags,output wire [843:0] physical_state')
    marker=' function automatic [2:0] memory_commands'
    assert raw.count(marker)==3  # top, retained Control-A and joint control
    declarations=''' // These32 bits retain the CLOSED input latch data. The actual Q of an
 // open MC10173 is its current D, including changes between host edges.
 wire [843:0] state_q,state_next;
 wire [31:0] current_read_q,closed_read_next;
 wire [1:0] current_read_n={clock_before|!held_q[8]|held_q[10]|held_q[48],
                          clock_before|!held_q[13]|held_q[12]|held_q[32]};
 wire [31:0] current_read_data={
   pins_before[151] ? pins_before[126:119] : file_read_data[15:8],
   pins_before[149] ? {8{pins_before[118]}} : file_read_data[7:0],
   held_q[51] ? pins_before[107:100] : pins_before[91:84],
   held_q[34] ? pins_before[99:92] : pins_before[83:76]};
 dorado_r_read_window_step read_windows(held_q[571:540],current_read_n,r_read_n,
   current_read_data,current_read_data,closed_read_next,current_read_q,,);
 assign state_q={held_q[843:572],current_read_q,held_q[539:0]};
 assign physical_state=state_q;
 assign held_next={state_next[843:572],closed_read_next,state_next[539:0]};
'''
    return raw.replace(marker,declarations+marker,1)


def transform_memory(raw):
    return replace(raw,'state_next[555:540],file_read_data,file_read_parity);',
                   'state_next[555:540],previous_address,previous_select,file_read_data,file_read_parity);')


def transform_ports(raw):
    # One asynchronous read per bank, independent of the synchronous write
    # address. Each existing bank still has exactly one physical array.
    raw=replace(raw,'input wire [15:0] write_data,','input wire [15:0] write_data,\n input wire [31:0] read_address,input wire [3:0] read_select_n,')
    start=raw.index(' wire [8:0] qh=')
    return raw[:start]+raw[start:].replace('select_n[','read_select_n[').replace('address[','read_address[')


def transform_bench(raw):
    raw=replace(raw,'reg [843:0] q=0,fixture=0;','reg [843:0] stored_q=0,fixture=0;wire [843:0] q;')
    raw=replace(raw,'host_clk,advance,q,cb,ca,pb,pa,','host_clk,advance,stored_q,cb,ca,pb,pa,')
    raw=replace(raw,'conta_next,shared_mismatch,control_flags);','conta_next,shared_mismatch,control_flags,q);')
    raw=replace(raw,'q=fixture;monitor=1;','stored_q=fixture;monitor=1;')
    raw=replace(raw,'q=expected;cb=next_root;','stored_q=expected;cb=next_root;')
    first=raw.index('     if(reference!==expected) begin')
    end=raw.index('     // Raw R memory data/parity',first)
    old=raw[first:end]
    assert 'i<844' in old and '$fatal' in old
    raw=raw[:first]+raw[end:]
    # Keep the ENTIRE original full-state comparison at the actual committed
    # physical view. The externally stored32-bit encoding is not physical Q
    # while open. No Q/RAM/clock channel is excluded from the oracle.
    post=old.replace('reference!==expected','reference!==q').replace('reference[i]!==expected[i]','reference[i]!==q[i]').replace('expected[i],reference[i]','q[i],reference[i]')
    marker='     if(n!==expected) $fatal(1,"R front state commit changed settled Q check=%0d",checks);'
    raw=replace(raw,marker,post+marker)
    return raw


def run(source,out,engine,mutation=None):
    parent=json.loads((source/'run-state.json').read_text())
    assert parent['status']=='PASS_R_FRONT_LOCAL_PROBE'
    assert parent['pre_event_control_capture'] and parent['masked_tasking_switch_clock']
    assert parent['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
    assert sha(source/'candidate.sv')=='ef0abf3eed4497350a1ada19ab8a00c0ddc5732ed81390722844eac1ddb0efea'
    compile_action=next(a for a in parent['actions'] if a['command'][0] in ('iverilog','verilator'))
    paths=[Path(n)for n in compile_action['command'] if n.endswith(('.sv','.v'))]
    out.mkdir(parents=True,exist_ok=False);records=[]
    transforms={'candidate.sv':transform_candidate,'tb_r_front_local.sv':transform_bench,
                'dorado_local_r_memory_step.sv':transform_memory,'dorado_r_file_ports.sv':transform_ports}
    copied=[]
    for p in paths:
        rel=p.relative_to(source);q=out/rel;q.parent.mkdir(parents=True,exist_ok=True)
        original=p.read_text();raw=transforms[p.name](original)if p.name in transforms else original
        q.write_text(raw);copied.append(q)
        records.append(dict(path=str(rel),parent_sha256=sha(p),output_sha256=sha(q),changed=raw!=original))
    assert sum(r['changed']for r in records)==4
    helper=out/'dorado_r_read_window_step.sv';helper.write_bytes((ROOT/'verilog/rtl'/helper.name).read_bytes());copied.append(helper)
    if mutation=='omit_close':
        helper.write_text(replace(helper.read_text(),'closing_events[HALF] ? data_before[8*byte_no+:8] : held_q[8*byte_no+:8]','held_q[8*byte_no+:8]'))
    (out/'source-run-state.json').write_bytes((source/'run-state.json').read_bytes())
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),engine=engine,parent=str(source),mutation=mutation,
               parent_run_state_sha256=sha(source/'run-state.json'),source_files=records,helper_sha256=sha(helper),
               actions=[],full_state_comparison='all844 physical Qs after owner commit',RAM_bits=9216,clock_channels=100,
               native_integrated=False,physical_timing_qualified=False,assembly_permitted=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    defines=[x for x in compile_action['command'] if x.startswith('-D')]
    commands=[['iverilog','-g2012','-s','tb_r_front_local',*defines,'-o',str(out/'test.vvp'),*map(str,copied)],['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else[
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,'--Mdir',str(out/'obj'),'-o','test',*map(str,copied)],[str(out/'obj/test')]]
    save();rc=0
    for i,cmd in enumerate(commands):
        with(out/('compile.log'if i==0 else'test.log')).open('w')as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);row=dict(command=cmd,pid=p.pid,started=time.time());state['actions'].append(row);save()
            rc=p.wait();row.update(returncode=rc,finished=time.time());save()
        if rc:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    m=re.search(r'R_FRONT_LOCAL_PROBE PASS ([^\n]+)',log)
    state.update(status='PASS_R_FRONT_READ_WINDOWS'if not rc and m else'R_FRONT_READ_WINDOWS_FAILED',summary=m[1]if m else None,finished=time.time());save()
    print(state['status'],flush=True);return rc if rc else(0 if m else 1)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],default='icarus');p.add_argument('--mutation',choices=['omit_close']);a=p.parse_args()
    sys.exit(run(a.source.resolve(),a.output.resolve(),a.engine,a.mutation))
