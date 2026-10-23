#!/usr/bin/env python3
"""Compare explicit MIR captures with original Control-A pin-level registers.

The reference wiring is extracted from the original .wl, independent of the
candidate's manual bit equations. Event models are shared with prior original
run/control tests. Stable setup is exercised; analog timing, upstream IM/CP
arrivals, phase coupling and native/60 ns operation remain unqualified.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import sys

from generate_processor_capture_reference import graph
from inventory_mir_clock_boundaries import PACKAGES,BOARD,ROOT

CANDIDATE='verilog/rtl/dorado_mir_capture_step.sv'
FF='verilog/verilator/run_control_original_ff.sv'
BINDINGS={**{f'dFF.{i}':f'pins[{i}]'for i in range(8)},
 **{f'dJCN.{i}':f'pins[{8+i}]'for i in range(8)},'dIMRH':'pins[16]',"dBlock'":'pins[17]',
 **{f'sFF.{i}':f'pins[{18+i}]'for i in range(8)},
 **{f'sJCN.{i}':f'pins[{26+i}]'for i in range(8)},'sIMRH':'pins[34]','sBLOCK':'pins[35]',
 **{f'brMIR{letter}':f'pins[{36+i}]'for i,letter in enumerate('abc')},
 **{f"h*clk0'C{letter}":f'pins[{39+i}]'for i,letter in enumerate('abcdef')}}
TB=r'''
`timescale 1ns/1ps
module tb_mir_capture;
 reg [44:0] before_pins=0,after_pins=0,reference_pins=0;
 reg [21:0] state_q=0,fixture_state=0;reg fixture_load=0;
 wire [21:0] state_next,reference_q;wire [5:0] events;
 wire [0:0] unused_clock,unused_flag;
 integer checks=0,transfers=0,asyncs=0,late_changes=0,separate_blocks=0;
 integer edge_counts[0:5];reg [21:0] seen_zero=0,seen_one=0;
 reg [31:0] random_state=32'h318a4279;
 function automatic [31:0] random_word;
  begin random_state=random_state^(random_state<<13);random_state=random_state^(random_state>>17);
   random_state=random_state^(random_state<<5);random_word=random_state;end
 endfunction
 dorado_mir_capture_step dut(state_q,before_pins,after_pins,state_next,events);
 original_mir_capture reference(.fixture_load(fixture_load),.fixture_state(fixture_state),
  .base(1'b0),.start_a(1'b0),.en(1'b0),.pins(reference_pins),.state_q(reference_q),
  .clocks(unused_clock),.flags(unused_flag));
 task automatic apply(input [44:0] p);
  reg [21:0] expected;reg [5:0] captured;
  begin
   before_pins=reference_pins;after_pins=p;state_q=reference_q;#2;
   expected=state_next;captured=events;
   reference_pins=p;#3;
   if(reference_q!==expected)$fatal(1,"MIR physical state mismatch check=%0d actual=%h expected=%h before=%h after=%h",checks,reference_q,expected,before_pins,p);
   if(captured!==(p[44:39]&~before_pins[44:39]))$fatal(1,"MIR capture events mismatch");
   seen_one|=reference_q;seen_zero|=~reference_q;checks++;
   for(integer i=0;i<6;i++)if(captured[i])begin edge_counts[i]++;transfers++;end
   if(reference_q[21]!=reference_q[17])separate_blocks++;
  end
 endtask
 task automatic seed(input [21:0] value);
  begin apply(0);fixture_state=value;fixture_load=1;#2;fixture_load=0;#2;end
 endtask
 reg [44:0] p;reg [21:0] saved;
 initial begin
  for(integer i=0;i<6;i++)edge_counts[i]=0;
  // Independent physical package states, including the unobserved g16.A half.
  seed(22'h3fffff);apply(0);seed(0);
  // Each real MIR clock independently captures both data extremes.
  for(integer bank=0;bank<6;bank++)begin
   p=0;p[17:0]='1;apply(p);p[39+bank]=1;apply(p);
   saved=reference_q;p[17:0]=0;apply(p);
   if(reference_q!==saved)$fatal(1,"MIR data changed after held clock");late_changes++;
   p[39+bank]=0;apply(p);p[39+bank]=1;apply(p);p[39+bank]=0;apply(p);
  end
  // Old e23 Block and g16 PrBlock are distinct captures.
  seed(0);p=0;p[17]=1;apply(p);p[43]=1;apply(p);
  if(reference_q[17]!==1 || reference_q[21]!==0)$fatal(1,"MIR merged Block/PrBlock clocks");
  // d23 is separate physical storage from d24, even if normal writes agree.
  seed(22'(1<<12));apply(0);
  if(reference_q[12]!==1 || reference_q[14]!==0)$fatal(1,"MIR merged physical JCN storage");
  for(integer bit_no=18;bit_no<39;bit_no++)begin
   seed(0);p=0;p[bit_no]=1;apply(p);asyncs++;p=0;apply(p);
  end
  seed(22'h2aaaaa);
  for(integer trial=0;trial<4096;trial++)begin
   p=reference_pins;p[17:0]=18'(random_word());p[38:18]=21'(random_word());
   // Exclude asynchronous set/reset overlap; this is not analog recovery proof.
   if(p[36])p[25:18]=0;
   if(p[37])begin p[33:26]=0;p[34]=0;end
   if(p[35])begin p[36]=0;p[38]=0;end
   apply(p);asyncs++;
   p[44:39]=6'(random_word());apply(p);
   saved=reference_q;p[17:0]=18'(random_word());apply(p);late_changes++;
   if(reference_q!==saved)$fatal(1,"MIR late data passed held clock");
  end
  if(seen_zero!==22'h3fffff || seen_one!==22'h3fffff || separate_blocks<16 || transfers<1000 || asyncs<4096 || late_changes<4096)
   $fatal(1,"MIR coverage incomplete");
  for(integer i=0;i<6;i++)if(edge_counts[i]<128)$fatal(1,"MIR clock coverage incomplete");
  $display("MIR_CAPTURE PASS checks=%0d transfers=%0d asyncs=%0d late=%0d split_block=%0d",checks,transfers,asyncs,late_changes,separate_blocks);$finish;
 end
endmodule
'''
MUTATIONS={
 'merge_Block_PrBlock':('clock_events[5]', 'clock_events[4]'),
 'forget_JCN_or_half':('data[15:8]={pins_after[10],pins_after[11],pins_after[10],pins_after[11],','data[15:8]={pins_after[10],pins_after[11],pins_after[10],1\'b0,'),
 'wrong_Block_maintenance':('pins_after[33],pins_after[36],pins_after[34]','pins_after[33],pins_after[35],pins_after[34]'),
 'late_data_passthrough':('else if(capture[i])','else if(pins_after[39])'),
}


def run(out):
 assert not out.exists();out.mkdir(parents=True);inputs=out/'inputs';inputs.mkdir()
 sources=[CANDIDATE,FF,BOARD,BOARD.replace('.wl','.lc'),'verilog/cells/cell_MC10231.v']
 for module in list(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name and Path(name).resolve().parent==ROOT/'tools':sources.append(str(Path(name).resolve().relative_to(ROOT)))
 hashes={}
 for name in sorted(set(sources)):
  p=inputs/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/name).read_bytes());hashes[name]=hashlib.sha256(p.read_bytes()).hexdigest()
 ref=out/'reference';ref.mkdir()
 record=graph(inputs,'ContA-Rev-Cd',[],{p:(2*i,2)for i,p in enumerate(PACKAGES)},'original_mir_capture',BINDINGS,
   ["h*clk0'Ca"],["Block'"],ref)
 p=ref/'original_mir_capture.sv';raw=p.read_text();assert raw.count('input wire [27:0] pins')==1
 p.write_text(raw.replace('input wire [27:0] pins','input wire [44:0] pins'))
 (inputs/'tb_mir_capture.sv').write_text(TB);hashes['tb_mir_capture.sv']=hashlib.sha256(TB.encode()).hexdigest()
 actions=[];summaries=[]
 def execute(cmd,cwd,log):
  with log.open('w')as f:rc=subprocess.run(list(map(str,cmd)),cwd=cwd,stdout=f,stderr=subprocess.STDOUT).returncode
  actions.append(dict(command=list(map(str,cmd)),returncode=rc,log=str(log)));return rc
 source=(inputs/CANDIDATE).read_text()
 cases=[('original',source)]
 for label,(a,b)in MUTATIONS.items():
  assert source.count(a)==1,(label,a);cases.append((label,source.replace(a,b)))
 for engine in ['icarus','verilator']:
  for label,rtl in cases:
   d=out/(engine+'-'+label);d.mkdir();candidate=d/'candidate.sv';candidate.write_text(rtl)
   support=[candidate,inputs/'tb_mir_capture.sv',inputs/FF,p]
   cmd=['iverilog','-g2012','-s','tb_mir_capture','-o',d/'test.vvp',*support]if engine=='icarus'else['verilator','--binary','--timing','--top-module','tb_mir_capture','-Wno-fatal','--Mdir',d/'obj','-o','test',*support]
   assert execute(cmd,inputs,d/'compile.log')==0,d/'compile.log'
   cmd=['vvp',d/'test.vvp']if engine=='icarus'else[d/'obj/test']
   rc=execute(cmd,inputs,d/'test.log');text=(d/'test.log').read_text()
   if label=='original':
    assert rc==0 and 'MIR_CAPTURE PASS'in text,(engine,text)
    summaries.append(re.search(r'MIR_CAPTURE PASS ([^\n]+)',text)[1])
   else:assert rc!=0 and re.search(r'MIR (physical state mismatch|data changed|merged|late data)',text),(engine,label,text)
 assert summaries[0]==summaries[1],summaries
 for name,digest in hashes.items():assert hashlib.sha256((inputs/name).read_bytes()).hexdigest()==digest,name
 result=dict(status='PASS_MIR_CAPTURE_TWO_SIMULATORS',scope=__doc__,inputs=hashes,reference=record,actions=actions,
  summary=summaries[0],rejected_mutations=list(MUTATIONS),native_integrated=False,upstream_arrival_or_timing_qualified=False)
 (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['summary'])


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
 run(p.parse_args().output.resolve())
