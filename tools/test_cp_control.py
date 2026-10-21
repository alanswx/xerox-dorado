#!/usr/bin/env python3
"""Compare original ContA CP decoder/control storage with explicit event RTL.

All 12 physical Qs, including unwired-output stages, are compared after every
CP event. Original wiring and delayed event-driven FFs are independent of the
candidate equations. This is CP-bus component qualification, not native clock
integration, task-producer closure, metastability/skew or 60ns board proof.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

from generate_processor_capture_reference import graph
from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]
CANDIDATE='verilog/rtl/dorado_cp_control_step.sv'
PROFILES=[('balanced',1000,1000),('fast_decode',500,1000),('late_capture',1000,2000)]
MUTATIONS={
 'wrong_bank_select':("pins[9:7]!=3'b110","pins[9:7]!=3'b101"),
 'active_high_decode':("pins[9:7]!=3'b111","pins[9:7]!=3'b000"),
 'falling_capture':('clocks_after & ~clocks(pins_before)','~clocks_after & clocks(pins_before)'),
 'swap_banks':('if(clock_events[0])','if(clock_events[1])'),
 'ignore_strobe':("pins[10] | (pins[9:7]!=3'b111)","(pins[9:7]!=3'b111)"),
 'wrong_data':('pins_after[5],','pins_after[4],'),
 'invented_unused_data':("{2'b00,pins_after[6]","{2'b11,pins_after[6]"),
}
BENCH=r'''`timescale 1ps/1ps
module tb_cp_control;
 reg load=0;reg [11:0] fixture=0,state=0;
 reg [10:0] pins=11'h780,before_pins=11'h780;
 wire [11:0] original_q,next_q;wire [1:0] clocks,events,after_clocks;
 wire [11:0] unused_flags;
 cp_control_reference original(load,fixture,1'b0,1'b0,1'b0,{17'b0,pins},original_q,clocks,unused_flags);
 dorado_cp_control_step dut(state,before_pins,pins,next_q,events,after_clocks);
 integer checks=0,count0=0,count1=0;reg [1:0] observed=0;
 reg [11:0] changed=0;reg [7:0] addresses=0;
 always @(posedge clocks[0]) observed[0]=1;
 always @(posedge clocks[1]) observed[1]=1;
 task event_pins(input [10:0] value);
  reg [11:0] old;reg [1:0] expected_events;
  begin
   old=state;observed=0;before_pins=pins;pins=value;
   #20000;
   if(original_q!==next_q || clocks!==after_clocks || observed!==events) begin
    $display("CP_FAIL check=%0d before=%h after=%h old=%h expected=%h actual=%h clocks=%b/%b events=%b/%b",checks,before_pins,pins,state,original_q,next_q,clocks,after_clocks,observed,events);
    $fatal(1,"CP producer mismatch");
   end
   expected_events=events;changed=changed|(old^next_q);state=next_q;
   count0=count0+expected_events[0];count1=count1+expected_events[1];checks=checks+1;
   #100;
  end
 endtask
 reg [10:0] selected;
 initial begin
  #20000;fixture=12'hfff;load=1;#20000;load=0;state=fixture;#20000;
  for(integer address=0;address<8;address=address+1) begin
   for(integer data=0;data<128;data=data+1) begin
    selected={1'b1,~3'(address),7'(data)};event_pins(selected);
    selected[10]=0;event_pins(selected);
    // No clock occurs on changing data in the active strobe window.
    selected[6:0]=~7'(data);event_pins(selected);
    selected[10]=1;event_pins(selected);
    addresses[address]=1;
   end
  end
  // Direct address changes during an asserted strobe must also obey the
  // decoder's actual outgoing-clock edge, rather than a software write API.
  for(integer address=0;address<8;address=address+1) begin
   selected[10]=0;selected[6:0]=7'(address);event_pins(selected);
   selected[9:7]=~3'(address);event_pins(selected);
  end
  selected[10]=1;event_pins(selected);
  if(count0!=129 || count1!=129 || changed!=12'hfff || addresses!=8'hff)
   $fatal(1,"CP coverage incomplete clocks=%0d/%0d changed=%h addr=%h",count0,count1,changed,addresses);
  $display("CP_CONTROL PASS checks=%0d clock0=%0d clock1=%0d changed=%h addresses=%h",checks,count0,count1,changed,addresses);
  $finish;
 end
endmodule
'''


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def run(out,mutations):
 out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs'
 names=[CANDIDATE,'tools/test_cp_control.py','tools/generate_processor_capture_reference.py',
        'tools/generate_control_clock_reference.py','tools/generate_run_control_reference.py',
        'tools/sil_netlist.py','verilog/cells/cell_MC10161.v','verilog/cells/cell_MC10176.v',
        'verilog/verilator/run_control_original_ff.sv',
        'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl',
        'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.lc',
        'chm/sil/BaseBd-Rev-Am.dm!1_/BaseBd-Rev-Am.wl']
 hashes={}
 for n in names:
  target=inputs/n;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes((ROOT/n).read_bytes());hashes[n]=sha(target)
 bench=inputs/'tb_cp_control.sv';bench.write_text(BENCH);hashes['tb_cp_control.sv']=sha(bench)
 bindings={"CPStrb'":'pins[10]',**{f"CPAddr.{i}'":f'pins[{9-i}]'for i in range(3)},
           **{f'CPOut.{i}':f'pins[{i-1}]'for i in range(1,8)}}
 ref=out/'reference';ref.mkdir()
 record=graph(inputs,'ContA-Rev-Cd',['a01'],{'j02':(0,6),'i02':(6,6)},'cp_control_reference',bindings,
              ["CPStrb0'","CPStrb1'"],[f'@j02.state_q[{i}]'for i in range(6)]+[f'@i02.state_q[{i}]'for i in range(6)],ref)
 assert record['physical_bits']==12 and len(record['packages'])==3
 # Check both CP-bus endpoints by net name and actual connector pin. PARC's
 # backplane is not generally pin-for-pin, so record each endpoint separately.
 boards={n:load_board(str(inputs/'chm/sil'/(n+'.dm!1_')/(n+'.wl')))for n in ['ContA-Rev-Cd','BaseBd-Rev-Am']}
 seams=[]
 for n in bindings:
  a,b=[boards[k].nets[n]for k in boards]
  assert a.get('backplane')and b.get('backplane'),n
  assert not any(p['dir']=='out'for p in a['pins']),('unexpected local driver',n)
  drivers=[{**p,'type':boards['BaseBd-Rev-Am'].packages[p['pkg']]['type']}for p in b['pins']if p['dir']=='out']
  assert drivers,('missing BaseBoard CP producer',n)
  seams.append(dict(net=n,conta_connector=sorted(a['backplane']),baseboard_connector=sorted(b['backplane']),baseboard_drivers=drivers))
 (ref/'manifest.json').write_text(json.dumps(dict(graph=record,backplane_seams=seams),indent=2)+'\n')
 actions=[]
 def execute(cmd,log,required=True):
  with log.open('w')as stream:code=subprocess.run(list(map(str,cmd)),stdout=stream,stderr=subprocess.STDOUT,cwd=inputs).returncode
  actions.append(dict(command=list(map(str,cmd)),returncode=code,log=str(log)))
  if required:assert code==0,log
  return code
 profiles=[];baseline=None
 for name,gate_delay,ff_delay in PROFILES:
  d=out/name;d.mkdir();timed=d/'reference';timed.mkdir()
  raw=(inputs/'verilog/cells/cell_MC10161.v').read_text()
  raw=re.sub(r'\bassign\s+(p\d+)\b',lambda m:f'assign #({gate_delay}) '+m[1],raw)
  (timed/'cell_MC10161.v').write_text('`timescale 1ps/1ps\n'+raw)
  raw=(inputs/'verilog/verilator/run_control_original_ff.sv').read_text()
  raw=re.sub(r'`timescale[^\n]*\n','',raw).replace('<=',f'<=#({ff_delay}) ')
  (timed/'run_control_original_ff.sv').write_text('`timescale 1ps/1ps\n'+raw)
  reference=ref/'cp_control_reference.sv'
  support=[reference,*timed.glob('*.v'),*timed.glob('*.sv'),bench]
  source=inputs/CANDIDATE
  execute(['iverilog','-g2012','-s','tb_cp_control','-o',d/'test.vvp',source,*support],d/'compile.log')
  execute(['vvp',d/'test.vvp'],d/'test.log')
  summary=re.search(r'^CP_CONTROL PASS ([^\n]+)$',(d/'test.log').read_text(),re.M)[1]
  if baseline is None:baseline=summary
  assert summary==baseline
  print('CP_CONTROL',name,'Icarus',summary,flush=True)
  execute(['verilator','--binary','--timing','--top-module','tb_cp_control','-Wno-fatal','--Mdir',d/'obj','-o','test',source,*support],d/'verilator-compile.log')
  execute([d/'obj/test'],d/'verilator-test.log')
  assert re.search(r'^CP_CONTROL PASS ([^\n]+)$',(d/'verilator-test.log').read_text(),re.M)[1]==summary
  profiles.append(dict(profile=name,gate_delay_ps=gate_delay,ff_delay_ps=ff_delay,summary=summary,
                       timed_source_hashes={p.name:sha(p)for p in timed.iterdir()}))
  print('CP_CONTROL',name,'Verilator agrees',flush=True)
  if mutations and name=='balanced':
   for label,(a,b)in MUTATIONS.items():
    md=d/label;md.mkdir();raw=source.read_text();assert raw.count(a)==1,(label,a)
    mutated=md/'candidate.sv';mutated.write_text(raw.replace(a,b))
    execute(['iverilog','-g2012','-s','tb_cp_control','-o',md/'test.vvp',mutated,*support],md/'compile.log')
    assert execute(['vvp',md/'test.vvp'],md/'test.log',False)!=0,label
 for n,h in hashes.items():assert sha(inputs/n)==h,n
 result=dict(status='PASS_CP_CONTROL_PRODUCERS',scope=__doc__,inputs=hashes,reference=record,
             profiles=profiles,summary=baseline,actions=actions,backplane_seams=seams,
             rejected_mutations=list(MUTATIONS)if mutations else[],native_integrated=False,full_task_producers_closed=False)
 (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
 p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
