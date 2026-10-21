#!/usr/bin/env python3
"""Qualify original ContA Ready/IsNext and all wired-OR SetReady drivers.

The extracted reference retains both notify/preemption decoders, feedback ANDs,
actual clock fanout and all four F10000 packages, including one unused Q.
Illustrative delays establish digital ordering only. MIR/phase/CP timing and
coupling to the task arbiter remain open; no native/60ns qualification is claimed.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

from generate_processor_capture_reference import graph

ROOT=Path(__file__).resolve().parents[1]
CANDIDATE='verilog/rtl/dorado_task_ready_step.sv'
PROFILES=[('balanced',1000,1000),('fast_decode',500,1000),('late_capture',1000,2000)]
MUTATIONS={
 'ignore_preemption':("!pins_after[13] &&","1'b0 &&"),
 'ignore_notify':("!pins_after[14] &&","1'b0 &&"),
 'keep_selected_ready':('state_q[bit_no] && is_next_n[task_no-1]','state_q[bit_no]'),
 'wrong_switch_polarity':('pins_after[12] ||','!pins_after[12] ||'),
 'ignore_repeat_gate':('pins[17]|pins[18]|pins[20]','pins[17]|pins[18]'),
 'synchronous_clear':('if(pins_after[15])','if(pins_after[15] && clock_events[0])'),
 'wrong_bit_order':('task_number={raw[0],raw[1],raw[2],raw[3]}','task_number=raw'),
 'invented_unused_bit':('capture_data=0;',"capture_data=16'h1000;"),
}
ORACLE=r'''`timescale 1ps/1ps
module original_F10000 #(parameter integer DELAY_PS=1000)(
 input wire p4,p13,p11,p10,p9,p7,p6,p12,p5,
 output wire p14,p15,p2,p3,input wire fixture_load,
 input wire [3:0] fixture_state,output reg [3:0] state_q);
 wire clk=p4|p13;
 always @(posedge clk or posedge p12 or posedge fixture_load) begin
  if(fixture_load)state_q<=fixture_state;
  else if(p12)state_q<=#(DELAY_PS) 0;
  else begin
   if(p4 || p5 || p6)$fatal(1,"Ready reference used outside original parallel-load wiring");
   state_q<=#(DELAY_PS) {p11,p10,p9,p7};
  end
 end
 assign {p14,p15,p2,p3}=state_q;
endmodule
'''
BENCH=r'''`timescale 1ps/1ps
module tb_task_ready;
 reg load=0;reg [15:0] fixture=0,state=0;
 reg [23:0] pins=22'h047000,before_pins=22'h047000;
 wire [15:0] original_q,next_q;wire [1:0] clocks,events,after_clocks;
 wire [29:0] flags;wire [14:0] next_n,ready;
 task_ready_reference original(load,fixture,1'b0,1'b0,1'b0,{4'b0,pins},original_q,clocks,flags);
 dorado_task_ready_step dut(state,before_pins,pins,next_q,events,after_clocks,next_n,ready);
 integer checks=0,captures=0,clears=0,held=0;reg [1:0] observed=0;
 reg [15:0] changed=0;reg [14:0] seen_ready=0,seen_selected=0;
 always @(posedge clocks[0]) observed[0]=1;
 always @(posedge clocks[1]) observed[1]=1;
 function automatic [3:0] raw_task(input integer n);raw_task={n[0],n[1],n[2],n[3]};endfunction
 task event_pins(input [23:0] value);
  reg [15:0] old;
  begin
   old=state;observed=0;before_pins=pins;pins=value;#20000;
   if(original_q!==next_q || clocks!==after_clocks || observed!==events || flags!=={ready,next_n})begin
    $display("READY_FAIL check=%0d before=%h after=%h old=%h expected=%h actual=%h flags=%h/%h events=%b/%b",checks,before_pins,pins,state,original_q,next_q,flags,{ready,next_n},observed,events);
    $fatal(1,"Ready producer mismatch");
   end
   captures=captures+events[0];clears=clears+pins[15];held=held+((pins[17]||pins[20])&&!events[0]);
   changed=changed|(old^next_q);seen_ready=seen_ready|ready;seen_selected=seen_selected|~next_n;
   state=next_q;checks=checks+1;#100;
  end
 endtask
 reg [23:0] selected;
 initial begin
  #20000;fixture=16'hffff;load=1;#20000;load=0;state=fixture;#20000;
  selected=pins;
  // Clear with no capture edge. Then exercise every combination of BNT,
  // current task, notify operand and the three decode enable bits.
  selected[15]=1;event_pins(selected);selected[15]=0;event_pins(selected);
  for(integer enable=0;enable<8;enable=enable+1)
   for(integer nt=0;nt<16;nt=nt+1)
    for(integer ct=0;ct<16;ct=ct+1)
     for(integer ff=0;ff<16;ff=ff+1)begin
      selected[18]=1;selected[14:12]=3'(enable);
      selected[23:22]=2'(ct);selected[3:0]=raw_task(nt);selected[7:4]=raw_task(ct);selected[11:8]=raw_task(ff);event_pins(selected);
      selected[18]=0;event_pins(selected);selected[18]=1;event_pins(selected);
     end
  // Independent Freeze/Repeat gating and asynchronous reset while held.
  for(integer gate_no=0;gate_no<2;gate_no=gate_no+1)begin
   for(integer task_no=1;task_no<16;task_no=task_no+1)begin
    selected[18]=1;selected[14]=0;selected[11:8]=raw_task(task_no);event_pins(selected);
    selected[gate_no==0?17:20]=1;event_pins(selected);
    selected[18]=0;event_pins(selected);
    selected[15]=1;event_pins(selected);selected[15]=0;event_pins(selected);
    selected[gate_no==0?17:20]=0;event_pins(selected);
    selected[18]=1;event_pins(selected);
   end
  end
  if(changed!=16'hffff||seen_ready!=15'h7fff||seen_selected!=15'h7fff||clears<31||held<60)
   $fatal(1,"Ready coverage missing changed=%h ready=%h selected=%h clear=%0d held=%0d",changed,seen_ready,seen_selected,clears,held);
  $display("TASK_READY PASS checks=%0d captures=%0d clear_checks=%0d held=%0d changed=%h ready=%h selected=%h",checks,captures,clears,held,changed,seen_ready,seen_selected);
  $finish;
 end
endmodule
'''


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def run(out,mutations):
 out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs'
 names=[CANDIDATE,'tools/test_task_ready.py','tools/generate_processor_capture_reference.py',
        'tools/generate_control_clock_reference.py','tools/generate_run_control_reference.py','tools/sil_netlist.py',
        'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl','chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.lc']
 kinds=['MC10161','MC10162','MC10104','SE10210','F10000'];names += ['verilog/cells/cell_'+k+'.v'for k in kinds]
 hashes={}
 for n in names:
  p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=sha(p)
 for name,raw in [('tb_task_ready.sv',BENCH),('original_ready_ff.sv',ORACLE)]:
  p=inputs/name;p.write_text(raw);hashes[name]=sha(p)
 bindings={'ContA18.sil+3':'pins[22]','ContA18.sil+4':'pins[23]',**{f'BNT.{i}':f'pins[{i}]'for i in range(4)},
           **{f"BNT.{i}'":f'~pins[{i}]'for i in range(4)},
           **{f'CTask.{i}':f'pins[{4+i}]'for i in range(4)},"CTask.0'":'~pins[4]',
           **{f'bFF.{i}':f'pins[{4+i}]'for i in range(4,8)},"bFF.4'":'~pins[8]',
           **dict(zip(["Switch'b","PreEmpting'","FF=Notify'",'ClrReady','CAHold','FreezeAC',"preclk0'Aa","preclk2'Aa",'RepeatCur','SCorFreezeb'],[f'pins[{i}]'for i in range(12,22)]))}
 comb=['a08','b08','c08','d08','b06','b07','c06','c07','f08','f09','d07']
 seq={p:(4*i,4)for i,p in enumerate(['a09','b09','c09','d09'])}
 ref=out/'reference';ref.mkdir()
 record=graph(inputs,'ContA-Rev-Cd',comb,seq,'task_ready_reference',bindings,
              ["h*fclk0'Ab","h*fclk0'Ac"],
              [f"IsNext.{i:02}'"for i in range(1,16)]+[f'Ready.{i:02}'for i in range(1,16)],ref)
 assert record['physical_bits']==16 and len(record['packages'])==15
 # graph() keeps every physical driver on each SetReady wired-OR net.
 from sil_netlist import load_board
 board=load_board(str(inputs/'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl'))
 for task in range(1,16):
  drivers=[p for p in board.nets[f'SetReady.{task:02}']['pins']if p['dir']=='out']
  assert len(drivers)==3 and all(p['pkg']in comb for p in drivers),(task,drivers)
 (ref/'manifest.json').write_text(json.dumps(record,indent=2)+'\n')
 actions=[]
 def execute(cmd,log,required=True):
  with log.open('w')as stream:code=subprocess.run(list(map(str,cmd)),stdout=stream,stderr=subprocess.STDOUT,cwd=inputs).returncode
  actions.append(dict(command=list(map(str,cmd)),returncode=code,log=str(log)))
  if required:assert code==0,log
  return code
 profiles=[];baseline=None
 for name,gate,ff in PROFILES:
  d=out/name;d.mkdir();timed=d/'reference';timed.mkdir()
  for kind in kinds[:-1]:
   raw=(inputs/f'verilog/cells/cell_{kind}.v').read_text()
   raw=re.sub(r'\bassign\s+(p\d+)\b',lambda m:f'assign #({gate}) '+m[1],raw)
   (timed/f'cell_{kind}.v').write_text('`timescale 1ps/1ps\n'+raw)
  (timed/'original_ready_ff.sv').write_text(ORACLE.replace('DELAY_PS=1000',f'DELAY_PS={ff}'))
  support=[ref/'task_ready_reference.sv',*timed.glob('*.v'),*timed.glob('*.sv'),inputs/'tb_task_ready.sv']
  source=inputs/CANDIDATE
  execute(['iverilog','-g2012','-s','tb_task_ready','-o',d/'test.vvp',source,*support],d/'compile.log')
  execute(['vvp',d/'test.vvp'],d/'test.log')
  summary=re.search(r'^TASK_READY PASS ([^\n]+)$',(d/'test.log').read_text(),re.M)[1]
  if baseline is None:baseline=summary
  assert summary==baseline
  print('TASK_READY',name,'Icarus',summary,flush=True)
  execute(['verilator','--binary','--timing','--top-module','tb_task_ready','-Wno-fatal','--Mdir',d/'obj','-o','test',source,*support],d/'verilator-compile.log')
  execute([d/'obj/test'],d/'verilator-test.log')
  assert re.search(r'^TASK_READY PASS ([^\n]+)$',(d/'verilator-test.log').read_text(),re.M)[1]==summary
  profiles.append(dict(profile=name,gate_delay_ps=gate,ff_delay_ps=ff,summary=summary,
                       timed_source_hashes={p.name:sha(p)for p in timed.iterdir()}))
  print('TASK_READY',name,'Verilator agrees',flush=True)
  if mutations and name=='balanced':
   for label,(a,b)in MUTATIONS.items():
    md=d/label;md.mkdir();raw=source.read_text();assert raw.count(a)==1,(label,a)
    mutated=md/'candidate.sv';mutated.write_text(raw.replace(a,b))
    execute(['iverilog','-g2012','-s','tb_task_ready','-o',md/'test.vvp',mutated,*support],md/'compile.log')
    assert execute(['vvp',md/'test.vvp'],md/'test.log',False)!=0,label
 for n,h in hashes.items():assert sha(inputs/n)==h,n
 result=dict(status='PASS_TASK_READY_PRODUCERS',scope=__doc__,inputs=hashes,reference=record,
             profiles=profiles,summary=baseline,actions=actions,rejected_mutations=list(MUTATIONS)if mutations else[],
             native_integrated=False,task_arbiter_coupled=False,all_SetReady_drivers_checked=True)
 (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
 p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
