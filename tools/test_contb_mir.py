#!/usr/bin/env python3
"""Compare ContB MIR/maintenance captures with the original board's pin graph.

Includes independent physical ASEL copies, CBHold-driven RepeatCur, local and
remote rMIRa drivers, parity-gated StopMIRClk, CP set and manifold release edges.
Upstream IM/parity, DMD and CA/Memory producers are still explicit boundaries.
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
from generate_contb_mir_reference import generate,plan,ROOT
from generate_contb_mir_candidate import generate as candidate_generate
from stage_task_capture_timing import stage

PROFILES={'balanced':(1000,1000,1000),'fast_gates':(250,1500,1000),
          'later_control':(1750,400,1250)}
MUTATIONS={
    'clock_copy':('normal=p[0]|p[1]|p[2]|stop_mir(q,p)',
                  'normal=p[0]|p[2]|p[2]|stop_mir(q,p)'),
    'hold':('repeat_cur=p[3]&p[4]','repeat_cur=p[4]'),
    'reset_driver':('reset_mir=p[6]|q[34]','reset_mir=p[6]'),
    'parity_mask':('(p[7] | !q[23]) & (p[8] | !q[22])',
                   '(p[7] | !q[22]) & (p[8] | !q[23])'),
    'manifold_edge':("v[22]=!enable|(selector!=3'd0)",
                     "v[22]=enable&(selector==3'd0)"),
    'cp_selector':("selector={p[11],p[12]}","selector={p[12],p[11]}"),
}

TB=r'''`timescale 1ns/1ps
module tb_contb_mir;
 localparam [55:0] SAFE=(56'h1<<0)|(56'h1<<1)|(56'h1<<2)|(56'h1<<4)|(56'h1<<7)|(56'h1<<8)|(56'h1<<10)|(56'h1<<11)|(56'h1<<12)|(56'h1<<13)|(56'h1<<46);
 reg [55:0] before_pins=SAFE,after_pins=SAFE,reference_pins=SAFE;
 reg [39:0] q=0,fixture=0;reg load=0,monitor=0;
 wire [39:0] next_q,reference;wire [24:0] events,repeated,clocks;wire unsettled;wire [25:0] flags,ref_flags;
 integer checks=0,counts[0:24];reg [39:0] seen_zero=0,seen_one=0;
 reg [31:0] rng=32'hcb004042;
 function automatic [31:0] random_word;
  begin rng=rng^(rng<<13);rng=rng^(rng>>17);rng=rng^(rng<<5);random_word=rng;end
 endfunction
 dorado_contb_mir_step dut(q,before_pins,after_pins,next_q,events,repeated,unsettled,flags);
 contb_mir_reference original(load,fixture,1'b0,1'b0,1'b0,reference_pins,reference,clocks,ref_flags);
 for(genvar i=0;i<25;i++)begin
  initial counts[i]=0;
  always @(posedge clocks[i])if(monitor)counts[i]++;
 end
 task automatic event_pins(input [55:0] p);
  reg [39:0] expected;reg [24:0] predicted;reg [25:0] expected_flags;integer previous[0:24];
  begin
   before_pins=reference_pins;after_pins=p;q=reference;#100;
   expected=next_q;predicted=events;expected_flags=flags;
   if(unsettled||repeated)$fatal(1,"ContB MIR resolver unsettled/repeated check=%0d",checks);
   for(integer i=0;i<25;i++)previous[i]=counts[i];
   reference_pins=p;#100;
   for(integer i=0;i<25;i++)if(counts[i]-previous[i]!=int'(predicted[i]))
    $display("CONTB_CLOCK_DIFF check=%0d channel=%0d actual=%0d expected=%0d",checks,i,counts[i]-previous[i],predicted[i]);
   if(reference!==expected)begin
    for(integer i=0;i<40;i++)if(reference[i]!==expected[i])$display("CONTB_DIFF bit=%0d old=%b expected=%b original=%b",i,q[i],expected[i],reference[i]);
    $fatal(1,"ContB MIR state mismatch check=%0d before=%h after=%h old=%h expected=%h original=%h",checks,before_pins,p,q,expected,reference);
   end
   if(ref_flags!==expected_flags)$fatal(1,"ContB MIR flags mismatch check=%0d original=%h expected=%h",checks,ref_flags,expected_flags);
   for(integer i=0;i<25;i++)if(counts[i]-previous[i]!=int'(predicted[i]))
    $fatal(1,"ContB MIR clock mismatch check=%0d channel=%0d actual=%0d expected=%0d",checks,i,counts[i]-previous[i],predicted[i]);
   seen_zero|=~reference;seen_one|=reference;q=reference;before_pins=p;after_pins=p;#100;
   if(next_q!==q)$fatal(1,"ContB MIR commit changed settled state");checks++;
  end
 endtask
 task automatic seed(input [39:0] value);
  begin
   monitor=0;reference_pins=SAFE;before_pins=SAFE;after_pins=SAFE;#100;
   fixture=value;fixture[34]=0;
   load=1;#100;load=0;#100;load=1;#100;load=0;#100;
   if(reference!==fixture)$fatal(1,"ContB MIR fixture mismatch");q=fixture;monitor=1;
   seen_zero|=~reference;seen_one|=reference;
  end
 endtask
 task automatic manifold(input integer code,data);
  reg [55:0] p;
  begin
   p=reference_pins;p[0]=1;p[5]=1;p[46]=1;p[19]=1'((code>>2)&1);p[20]=1'((code>>1)&1);p[21]=1'(code&1);p[27:22]=6'(data);event_pins(p);
   p[46]=0;event_pins(p);p[46]=1;event_pins(p);
  end
 endtask
 reg [55:0] p;reg [39:0] saved;reg [21:0] cp_seen_one=0,cp_seen_zero=0;
 initial begin
  seed(40'hffffffffff);event_pins(SAFE);
  // Unused MIR halves and duplicate physical ASEL storage remain independent.
  seed(40'h0000000002);event_pins(SAFE);
  if(q[1]!==1||q[3]!==0)$fatal(1,"ContB MIR collapsed ASEL.0 copies");
  seed(0);
  // Distinguish the root, registered clock-enable copy and StartCycle input.
  // Changing any qualifier LOW must hold; each separate rise captures data.
  for(integer mask=0;mask<8;mask++)begin
   p=SAFE;p[2:0]=0;p[45:28]=18'h15555;event_pins(p);saved=q;
   p[45:28]=18'h2aaaa;event_pins(p);
   if(q!==saved)$fatal(1,"ContB MIR captured without clock edge");
   p[2:0]=3'(mask);event_pins(p);
   if(mask!=0 && q[1]!==p[28])$fatal(1,"ContB MIR qualifier failed capture mask=%0d",mask);
  end
  // Both halves of BNT/CBHold must be HIGH for RepeatCur's independent edge.
  for(integer mask=0;mask<4;mask++)begin
   p=SAFE;p[2:0]=0;p[4:3]=0;p[45:28]=18'h15555;event_pins(p);
   p[4:3]=2'(mask);event_pins(p);
  end
  // Return root HIGH before releasing RepeatCur. A simultaneous independent
  // root rise/repeat fall has gate-delay-dependent pulses, not a settled-level
  // contract. The rejected v2 run retains that uncovered arrival counterexample.
  p[0]=1;event_pins(p);
  for(integer trial=0;trial<512;trial++)begin
   p=SAFE;p[1]=0;p[2]=0;p[45:28]=18'(random_word());event_pins(p);
   p[0]=0;event_pins(p);p[0]=1;event_pins(p);
   saved=q;p[45:28]=~p[45:28];event_pins(p);
   if(q!==saved)$fatal(1,"ContB MIR changed on late data while clock held");
   // CBHold is its own input. RepeatCur can clock the MIR with root LOW.
   p[0]=0;event_pins(p);p[3]=1;event_pins(p);p[3]=0;event_pins(p);
  end
  seed(0);
  // Real active-low manifold decoder release edges capture all package Qs.
  for(integer code_no=0;code_no<3;code_no++)for(integer data=0;data<64;data++)begin
   manifold(code_no==0?0:code_no==1?7:1,data);
  end
  // Two original rMIRa drivers share one wired-OR backplane net.
  manifold(1,0);p=SAFE;p[6]=1;event_pins(p);manifold(1,1);
  p=reference_pins;p[6]=0;event_pins(p);p[1]=0;p[2]=0;p[45:28]='1;p[0]=0;event_pins(p);p[0]=1;event_pins(p);
  if(q[21:0]!==0)$fatal(1,"ContB MIR lost local reset while remote reset released");
  manifold(1,0);
  p=reference_pins;p[0]=0;event_pins(p);p[0]=1;event_pins(p);
  if(!q[1]||!q[3])$fatal(1,"ContB MIR reset release did not permit capture");
  // Fault masking stops the MIR clock HIGH through actual stored enables.
  manifold(0,3);manifold(7,1);
  p=reference_pins;p[0]=0;p[1]=0;p[2]=0;p[3]=0;p[7]=1;p[8]=1;event_pins(p);
  p[7]=0;event_pins(p);if(!flags[20])$fatal(1,"ContB MIR left parity did not stop");
  p[7]=1;event_pins(p);p[8]=0;event_pins(p);if(!flags[20])$fatal(1,"ContB MIR right parity did not stop");
  manifold(7,0);if(flags[20])$fatal(1,"ContB MIR ignored stored stop disable");
  manifold(0,0);p=reference_pins;p[7]=1;p[8]=1;event_pins(p);
  // All combinations of stored mask bits, stored stop enable, and raw parity.
  // Disabled and asymmetric masks must not inherit the other half's fault.
  for(integer enables=0;enables<4;enables++)for(integer stop=0;stop<2;stop++)begin
   manifold(0,enables);manifold(7,stop);
   p=reference_pins;p[2:0]=0;p[4:3]=0;p[8:7]=3;event_pins(p);
   for(integer parity=0;parity<4;parity++)begin
    p[8:7]=2'(parity);event_pins(p);
    if(flags[20]!==1'(stop && ((!parity[0] && enables[1])||(!parity[1] && enables[0]))))
     $fatal(1,"ContB MIR parity truth table mismatch");
   end
  end
  manifold(0,0);manifold(7,0);
  // CP decoder sets are asynchronous and distinct from manifold capture.
  for(integer selector=0;selector<4;selector++)for(integer data=0;data<32;data++)begin
   // Clear through the real shared reset with CP disabled. Otherwise earlier
   // captures leave every bit set and a swapped decoder can go undetected.
   p=SAFE;p[6]=1;event_pins(p);p[6]=0;event_pins(p);
   p=SAFE;p[10]=0;p[11]=1'(selector>>1);p[12]=selector[0];p[17:14]=4'(data);p[18]=data[4];event_pins(p);
   p[13]=0;event_pins(p);p[13]=1;event_pins(p);
   cp_seen_one|=q[21:0];cp_seen_zero|=~q[21:0];
  end
  if(cp_seen_one!==22'h3dfffe||cp_seen_zero!==22'h3fffff)$fatal(1,"ContB MIR CP set/clear coverage incomplete");
  $display("CONTB_MIR_CP seen_one=%h seen_zero=%h",cp_seen_one,cp_seen_zero);
  for(integer i=0;i<25;i++)if(counts[i]<16)$fatal(1,"ContB MIR clock coverage incomplete channel=%0d",i);
  if(seen_zero!==40'hffffffffff||seen_one!==40'hffffffffff)$fatal(1,"ContB MIR physical Q coverage incomplete");
  for(integer i=0;i<25;i++)$display("CONTB_MIR_CLOCK channel=%0d edges=%0d",i,counts[i]);
  $display("CONTB_MIR PASS checks=%0d physical=40 clocks=25",checks);$finish;
 end
endmodule
'''


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(out,engine,profile='balanced',mutation=None,hazard_case=False):
    out.mkdir(parents=True,exist_ok=False);ref=out/'reference';r=generate(ROOT,ref)
    inputs=out/'inputs';sources={'chm/sil/ContB-Rev-Cd.dm!1_/ContB-Rev-Cd.wl','chm/sil/ContB-Rev-Cd.dm!1_/ContB-Rev-Cd.lc',
                              'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl','chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.lc',
                              'verilog/verilator/run_control_original_ff.sv','verilog/verilator/processor_capture_original_ff.sv','verilog/cells/cell_MC10210.v',*r['combinational_sources']}
    for m in list(sys.modules.values()):
        n=getattr(m,'__file__',None)
        if n and Path(n).resolve().parent==ROOT/'tools':sources.add(str(Path(n).resolve().relative_to(ROOT)))
    hashes={}
    for n in sorted(sources):
        p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=sha(p)
    candidate=out/'candidate.sv';generated=candidate_generate(candidate)
    if mutation:
        before,after=MUTATIONS[mutation];raw=candidate.read_text();assert raw.count(before)==1
        candidate.write_text(raw.replace(before,after))
    timed=out/'timed';timing=stage(inputs,ref,timed,*PROFILES[profile])
    tb=out/'tb.sv';bench=TB
    if hazard_case:
        marker='  p[0]=1;event_pins(p);\n  for(integer trial=0;trial<512;trial++)begin'
        assert bench.count(marker)==1
        bench=bench.replace(marker,'  for(integer trial=0;trial<512;trial++)begin')
    tb.write_text(bench)
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),actions=[],engine=engine,profile=profile,mutation=mutation,
               inputs=hashes,reference=r,candidate=generated,timing=timing,native_integrated=False,hazard_case=hazard_case,
               executed_candidate_sha256=sha(candidate),testbench_sha256=sha(tb))
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    def execute(cmd,log):
        with log.open('w')as f:
            p=subprocess.Popen(list(map(str,cmd)),stdout=f,stderr=subprocess.STDOUT);row=dict(command=list(map(str,cmd)),pid=p.pid,started=time.time());state['actions'].append(row);save();rc=p.wait()
            row.update(returncode=rc,finished=time.time());save();return rc
    support=[candidate,tb,*sorted(timed.glob('*.v')),*sorted(timed.glob('*.sv'))]
    state['compiled_sources']={str(p.relative_to(out)):sha(p)for p in support};save()
    cmd=['iverilog','-g2012','-s','tb_contb_mir','-o',out/'test.vvp',*support]if engine=='icarus'else['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_contb_mir','--Mdir',out/'obj','-o','test',*support]
    rc=execute(cmd,out/'compile.log')
    if rc==0:rc=execute(['vvp','-i',out/'test.vvp']if engine=='icarus'else[out/'obj/test'],out/'test.log')
    text=(out/'test.log').read_text()if(out/'test.log').exists()else'';summary=re.search(r'^CONTB_MIR PASS ([^\n]+)$',text,re.M)
    state.update(status='PASS_CONTB_MIR_PROBE'if rc==0 and summary else'CONTB_MIR_PROBE_FAILED',summary=summary[1]if summary else None,finished=time.time())
    save();print(state['status'],flush=True);return rc if rc else(0 if summary else 1)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--engine',choices=['icarus','verilator'],default='verilator')
    p.add_argument('--profile',choices=PROFILES,default='balanced');p.add_argument('--mutation',choices=MUTATIONS)
    p.add_argument('--hazard-case',action='store_true',help='Retain the unresolved simultaneous root-rise/repeat-release counterexample')
    a=p.parse_args();sys.exit(run(a.output.resolve(),a.engine,a.profile,a.mutation,a.hazard_case))
