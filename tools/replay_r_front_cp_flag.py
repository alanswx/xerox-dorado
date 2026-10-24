#!/usr/bin/env python3
"""Replay saved CP/root capture in the unchanged full timed original graph.

Restore the exact component-only 844-Q/9216-RAM snapshot and sweep CP timing.
Validate that restoration is exact before every trial. Report setup/hold
margins and enforce the original MC10176 truth only where its documented
timing requirements are met. This does not waive or replace the parent miter,
change a reference cell, import a native-machine state or qualify a core.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import time

from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]


def snapshot(source):
    log=(source/'test.log').read_text()
    lines=re.findall(r'^CP_FLAG_PRE (.+)$',log,re.M);assert len(lines)==1
    row=dict(re.findall(r'(\w+)=([0-9a-f]+)',lines[0]))
    if 'memory'not in row:
        row['memory']=re.search(r'^CP_FLAG_RAM_HI (\w+)',log,re.M)[1]+re.search(r'^CP_FLAG_RAM_LO (\w+)',log,re.M)[1]
    assert row['root']=='0' and row['next_root']=='1'
    assert row['stored']==row['original']
    delta=int(row['pins_before'],16)^int(row['pins_after'],16)
    assert delta & ~((1<<292)|(((1<<18)-1)<<127))==0
    return row


def bench(s):
    template='''`timescale 1ps/1ps
`default_nettype none
module tb_cp_flag_replay;
localparam [843:0] SAVED=844'hSAVED_VALUE;
localparam [9215:0] MEMORY=9216'hMEMORY_VALUE;
localparam [400:0] BEFORE=401'hBEFORE_VALUE,AFTER=401'hAFTER_VALUE;
reg load=0,root=0,monitor=0;
reg [400:0] pins=BEFORE;
wire [843:0] q;
wire [9215:0] memory_q;
wire [15:0] raw_data;wire [1:0] raw_parity;
wire [31:0] address;wire [3:0] select_n;wire [99:0] clocks;
wire [1:0] write_n={original.l.j05.p13,original.h.j05.p13};
wire [400:0] reference_pins={pins[400:145],raw_parity,raw_data,pins[126:0]};
r_front_control_reference original(.fixture_load(load),.fixture_state(SAVED),.base(root),
 .pins(reference_pins),.state_q(q),.clocks(clocks),.address(address),.select_n(select_n));
r_file_ports_reference files(.fixture_load(load),.fixture_state(MEMORY),.advance(1'b1),
 .write_n(write_n),.address(address),.select_n(select_n),.write_data(q[555:540]),
 .read_data(raw_data),.read_parity(raw_parity),.state_q(memory_q));
integer trial=0,offset_ps=0,clock_count=0,data_falls=0;
time clock_at=0,fall_at=0;
reg captured_d=0;
integer clock7_edges=0,clock7_low_ps=-1;
time clock7_fall_at=0;
reg clock7_low_seen=0;
always @(negedge clocks[7])if(monitor)begin
 clock7_low_seen=1;clock7_fall_at=$time;
end
always @(posedge clocks[7])if(monitor)begin
 clock7_edges=clock7_edges+1;
 if(clock7_low_seen)clock7_low_ps=integer'($time-clock7_fall_at);
 $display("CP_REPLAY_CLOCK7 trial=%0d offset_ps=%0d at_ps=%0t low_ps=%0d D=%b",trial,offset_ps,$time,clock7_low_ps,original.c.k04.p7);
end
always @(negedge original.h.g18.p5)if(monitor)begin
 data_falls=data_falls+1;fall_at=$time;
 $display("CP_REPLAY_D trial=%0d offset_ps=%0d at_ps=%0t",trial,offset_ps,$time);
end
always @(posedge original.h.g18.p9)if(monitor)begin
 clock_count=clock_count+1;clock_at=$time;captured_d=original.h.g18.p5;
 $display("CP_REPLAY_C trial=%0d offset_ps=%0d at_ps=%0t D=%b",trial,offset_ps,$time,original.h.g18.p5);
end
task automatic replay(input integer skew);
 integer margin_ps,expected_q,timing_valid;
 begin
  // Establish a real load edge after initialization and input settling.
  // A time-zero assignment can be folded into initialization by an engine.
  monitor=0;load=0;#10000;root=0;pins=BEFORE;#10000;load=1;
  #100000;load=0;#100000;
  // First restore CP/MIR controls and let their asynchronous sets/resets
  // settle. A second load restores downstream Qs with those controls stable;
  // the original FF reset/set priority is deliberately not changed.
  load=1;#100000;load=0;#100000;
  if(q!==SAVED)begin
   for(integer b=0;b<844;b++)if(q[b]!==SAVED[b])$display("CP_REPLAY_SEED_DIFF bit=%0d actual=%b saved=%b",b,q[b],SAVED[b]);
   $fatal(1,"Component state restore does not reproduce saved boundary");
  end
  if(memory_q!==MEMORY)$fatal(1,"Component RAM restore does not reproduce saved boundary");
  $display("CP_REPLAY_SEED_EXACT trial=%0d physical_bits=844 RAM_bits=9216",trial);
  clock_count=0;data_falls=0;clock_at=0;fall_at=0;
  clock7_edges=0;clock7_low_ps=-1;clock7_low_seen=0;clock7_fall_at=0;offset_ps=skew;monitor=1;
  if(skew<0)begin #(20000+skew);pins=AFTER;#(-skew);root=1;end
  else begin #20000;root=1;#(skew);pins=AFTER;end
  #100000;monitor=0;
  if(clock_count!=1 || data_falls!=1)$fatal(1,"Replay did not isolate one flag capture and data fall");
  timing_valid=0;expected_q=-1;margin_ps=0;
  if(fall_at<clock_at)begin
   margin_ps=integer'(clock_at-fall_at);timing_valid=margin_ps>=2500;expected_q=0;
  end else if(fall_at>clock_at)begin
   margin_ps=integer'(fall_at-clock_at);timing_valid=margin_ps>=1500;expected_q=1;
  end
  $display("CP_REPLAY_RESULT trial=%0d offset_ps=%0d D_ps=%0t C_ps=%0t margin_ps=%0d valid=%0d expected=%0d sampled_D=%b Q=%b clock7_edges=%0d clock7_low_ps=%0d",
   trial,skew,fall_at,clock_at,margin_ps,timing_valid,expected_q,captured_d,q[88],clock7_edges,clock7_low_ps);
  if(timing_valid && q[88]!==expected_q[0])$fatal(1,"Original flag violates documented valid-margin capture truth");
  trial=trial+1;
 end
endtask
initial begin
 replay(-16000);replay(-8000);replay(-4000);replay(-2500);replay(-1000);
 replay(0);replay(1000);replay(1500);replay(4000);replay(8000);replay(16000);
 $display("CP_REPLAY_COMPLETE trials=%0d original_cells_unchanged=1 component_only=1",trial);$finish;
end
endmodule
`default_nettype wire
'''
    for key,value in [('SAVED_VALUE',s['original']),('MEMORY_VALUE',s['memory']),
                      ('BEFORE_VALUE',s['pins_before']),('AFTER_VALUE',s['pins_after'])]:
        template=template.replace(key,value)
    return template


def run(source,other,out):
    assert not out.exists();r=json.loads((source/'run-state.json').read_text());engine=r['engine']
    assert r['status']=='DIAGNOSTIC_CP_FLAG_ARRIVAL_OBSERVED'
    assert r['actions'][0]['returncode']==0 and r['qualifying_gate'] is False
    seed=snapshot(source);assert seed==snapshot(other),'Pre-event state/RAM/pins differ between engines'
    out.mkdir(parents=True);records=[];paths=[]
    for row in r['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256']
        if p.name=='tb_r_front_local.sv':continue
        q=out/row['path'];q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes());paths.append(str(q))
        records.append(dict(path=row['path'],sha256=sha(q),unchanged=True))
    tb=out/'tb_cp_flag_replay.sv';tb.write_text(bench(seed));paths.append(str(tb))
    (out/'snapshot.json').write_text(json.dumps(seed,indent=2)+'\n')
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    defines=[s for s in r['actions'][0]['command']if s.startswith('-D')]
    commands=[['iverilog','-g2012','-s','tb_cp_flag_replay',*defines,'-o',str(out/'test.vvp'),*paths],['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else[
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','-Wno-PINMISSING','--top-module','tb_cp_flag_replay',
         *defines,'--Mdir',str(out/'obj'),'-o','test',*paths],[str(out/'obj/test')]]
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),engine=engine,parent=str(source),
               parent_state_sha256=sha(source/'run-state.json'),other_engine_snapshot=str(other),
               snapshots_identical=True,source_files=records,fixture_sha256=sha(tb),actions=[],
               original_timed_cells_unchanged=True,original_parent_rejection_retained=True,
               component_state_replay_only=True,native_state_import=False,native_qualified=False,
               minimum_setup_ps=2500,minimum_hold_ps=1500,qualifying_gate=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for i,command in enumerate(commands):
        with(out/('compile.log'if i==0 else'test.log')).open('w')as log:
            p=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT)
            a=dict(command=command,pid=p.pid,started=time.time());state['actions'].append(a);save()
            rc=p.wait();a.update(returncode=rc,finished=time.time());save()
        if rc:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    rows=[]
    for line in re.findall(r'^CP_REPLAY_RESULT (.+)',log,re.M):
        rows.append({key:int(value)for key,value in re.findall(r'(\w+)=(-?\d+)',line)})
    passed=rc==0 and len(rows)==11 and log.count('CP_REPLAY_SEED_EXACT ')==11
    state.update(status='PASS_ORIGINAL_CP_FLAG_MARGIN_REPLAY'if passed else'ORIGINAL_CP_FLAG_MARGIN_REPLAY_FAILED',
                 results=rows,finished=time.time());save();print(state['status'],flush=True)
    return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['source','other','output']:p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.other.resolve(),a.output.resolve()))
