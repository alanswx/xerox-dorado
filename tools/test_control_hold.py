#!/usr/bin/env python3
"""Compare the joint original Control-A/Control-B/MemC capture graph.

Every physical Q, clock count and selected functional flag compares after
each event. Actual upstream memory/IM/DMD data remain boundaries. This is a
capture-order experiment, not native boot, analog or FPGA timing qualification.
"""
import argparse
from contextlib import redirect_stdout
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import time
from generate_control_hold_reference import generate,ROOT
from generate_control_hold_candidate import generate as candidate_generate,PARENTS
from test_contb_mir import PROFILES
from test_task_ready import ORACLE
from stage_task_capture_timing import stage

# Retain the Control-A component's established later-FF arrival profile as a
# separate case. The earlier-phase stress profile remains required and fails;
# this additional comparison does not replace or waive that result.
PROFILES={**PROFILES,'ca_later_control':(1000,2000,1000)}

MUTATIONS={
    'clock_copy':('b[1]=!s[9]|s[11]','b[1]=s[6]|!s[7]'),
    'start_copy':('b[2]=!s[23]','b[2]=!s[25]'),
    'new_MemSH':('mp[58]=!state_q[22]','mp[58]=!n[22]'),
    'ASEL_copy':('a[123]=!s[111]','a[123]=!s[113]'),
    'ungated_UseDMD':('b[5]=s[104]&p[6]','b[5]=s[104]'),
    'local_reset':('ap,q[68]|q[144])','ap,q[68])'),
    'CP_bus':('b[15]=p[282]','b[15]=b[15]'),
}

TB=r'''`timescale 1ns/1ps
module tb_control_hold;
 localparam [397:0] IDLE=398'h128 | (398'h1<<292) | (398'h7<<289) | (398'h1<<263) |
   (398'h1<<(326+7)) | (398'h1<<(326+8)) | (398'h1<<(326+46)) |
   (398'h1<<(326+28)) | (398'h1<<(326+60)) | (398'h1<<(326+61));
 reg [397:0] before_pins=IDLE,after_pins=IDLE,reference_pins=IDLE;
 reg before_root=0,after_root=0,root=0;
 reg [155:0] q=0,fixture=0;reg load=0,monitor=0;
 wire [155:0] next_q,reference;wire [55:0] events,repeated,clocks;wire unsettled;wire [93:0] flags,ref_flags;
 integer checks=0,counts[0:55];reg [155:0] changed=0;reg [31:0] rng=32'h156ca046;
 function automatic [31:0] random_word;
  begin rng=rng^(rng<<13);rng=rng^(rng>>17);rng=rng^(rng<<5);random_word=rng;end
 endfunction
 dorado_control_hold_step dut(q,before_root,after_root,before_pins,after_pins,next_q,events,repeated,unsettled,flags);
 control_hold_reference original(load,fixture,root,reference_pins,reference,clocks,ref_flags);
 for(genvar i=0;i<56;i++)begin
  initial counts[i]=0;always @(posedge clocks[i])if(monitor)counts[i]++;
 end
 task automatic event_step(input next_root,input [397:0] p);
  reg [155:0] expected;reg [55:0] predicted;reg [93:0] expected_flags;integer previous[0:55];
  begin
   before_root=root;after_root=next_root;before_pins=reference_pins;after_pins=p;q=reference;#100;
   expected=next_q;predicted=events;expected_flags=flags;
   if(unsettled||repeated)$fatal(1,"Control Hold resolver unsettled/repeated check=%0d repeated=%h",checks,repeated);
   for(integer i=0;i<56;i++)previous[i]=counts[i];
   reference_pins=p;root=next_root;#100;
   for(integer i=0;i<56;i++)if(counts[i]-previous[i]!=int'(predicted[i]))
    $display("CONTROL_HOLD_CLOCK_DIFF check=%0d channel=%0d actual=%0d predicted=%0d",checks,i,counts[i]-previous[i],predicted[i]);
   if(reference!==expected)begin
    for(integer i=0;i<156;i++)if(reference[i]!==expected[i])$display("CONTROL_HOLD_DIFF bit=%0d old=%b expected=%b original=%b",i,q[i],expected[i],reference[i]);
    $fatal(1,"Control Hold state mismatch check=%0d root=%b->%b old=%h expected=%h original=%h",checks,before_root,next_root,q,expected,reference);
   end
   if(ref_flags!==expected_flags)$fatal(1,"Control Hold flags mismatch check=%0d actual=%h expected=%h",checks,ref_flags,expected_flags);
   for(integer i=0;i<56;i++)if(counts[i]-previous[i]!=int'(predicted[i]))$fatal(1,"Control Hold clock mismatch check=%0d channel=%0d",checks,i);
   changed|=q^reference;q=reference;before_root=root;after_root=root;before_pins=p;after_pins=p;#100;
   if(next_q!==q)$fatal(1,"Control Hold commit changed settled state check=%0d",checks);checks++;
  end
 endtask
 task automatic seed(input [155:0] value);
  begin
   monitor=0;root=0;reference_pins=IDLE;#100;
   // First settle the original asynchronous clock/phase resets, then load
   // the requested independent physical copies. Do not normalize them away.
   fixture=0;#100;load=1;#100;load=0;#100;
   fixture=value;fixture[75:64]=0;fixture[30]=1;fixture[99]=1;fixture[144]=0;
   fixture[150]=1;fixture[151]=0;fixture[153:152]=3;fixture[154]=0;
   if(fixture[21])begin fixture[15:14]=3;fixture[17:16]=0;end
   if(fixture[6]||!fixture[7])begin fixture[29:23]=7'b1000000;fixture[31]=0;end
   #100;load=1;#100;load=0;#100;load=1;#100;load=0;#100;
   if(reference!==fixture)begin
    for(integer i=0;i<156;i++)if(reference[i]!==fixture[i])$display("CONTROL_HOLD_FIXTURE bit=%0d expected=%b actual=%b",i,fixture[i],reference[i]);
    $fatal(1,"Control Hold fixture mismatch");
   end
   q=reference;before_root=0;after_root=0;before_pins=IDLE;after_pins=IDLE;monitor=1;
  end
 endtask
 task automatic cycle;begin event_step(1,reference_pins);event_step(0,reference_pins);end endtask
 task automatic cp_write(input integer bank,data);
  reg [397:0] p;
  begin
   if(!root)event_step(1,reference_pins);
   p=reference_pins;p[292]=1;p[291:289]=~3'(bank);p[288:282]=7'(data);event_step(1,p);
   p[292]=0;event_step(1,p);p[292]=1;event_step(1,p);
  end
 endtask
 function automatic [7:0] im_ff(input [7:0] f);im_ff={f[0],f[1],f[2],f[3],f[4],f[5],f[6],f[7]};endfunction
 task automatic start;
  reg [397:0] p;
  begin p=reference_pins;p[6]=1;event_step(1,p);repeat(64)cycle();end
 endtask
 task automatic manifold(input integer code,data);
  reg [397:0] p;
  begin
   if(!root)event_step(1,reference_pins);
   p=reference_pins;p[326+46]=1;p[326+19]=1'(code>>2);p[326+20]=1'(code>>1);p[326+21]=code[0];p[326+27:326+22]=6'(data);event_step(1,p);
   p[326+46]=0;event_step(1,p);p[326+46]=1;event_step(1,p);
  end
 endtask
 task automatic cp_cb(input integer bank,data);
  reg [397:0] p;
  begin
   p=reference_pins;p[292]=1;p[291:289]=~3'(bank);p[288:282]=7'((data>>1)&7);
   p[326+14]=data[0];p[326+18]=data[4];event_step(1,p);
   p[292]=0;event_step(1,p);p[292]=1;event_step(1,p);
  end
 endtask
 reg [397:0] p;reg [155:0] sample;reg found;integer previous_mir,late_captures=0;
 initial begin
  seed({156{1'b1}});event_step(0,reference_pins);seed(0);event_step(0,reference_pins);
  p=reference_pins;p[6]=1;event_step(1,p);repeat(128)cycle();
  $display("CONTROL_HOLD_STARTED cycles=128 state=%h",q);
  for(integer trial=0;trial<512;trial++)begin
   event_step(1,reference_pins);p=reference_pins;
   p[253:246]=8'(random_word());p[261:254]=8'(random_word());
   p[4]=trial[4];p[5]=!trial[5];p[6]=trial[6];p[3]=trial[7];
   p[326+45:326+28]=18'(random_word());p[326+60]=!trial[0];p[326+61]=!trial[1];p[326+62]=trial[2];p[326+63]=trial[3];
   event_step(1,p);cycle();
  end
  for(integer bank=0;bank<2;bank++)for(integer data=0;data<128;data++)cp_write(bank,data);
  cp_write(0,0);cp_write(1,0);repeat(32)cycle();
  // Original registered copies may disagree before their respective clocks.
  // Exercise both StartCycle stages and clock enables as real independent Qs.
  sample=0;sample[7]=1;sample[9]=1;sample[23]=0;sample[25]=1;
  seed(sample);event_step(0,reference_pins);cycle();
  sample=0;sample[7]=1;sample[23]=1;sample[25]=1;
  seed(sample);event_step(0,reference_pins);cycle();
  // ASEL.0'a is b23.B; b24.B is a distinct physical copy.
  sample=0;sample[111]=1;sample[113]=0;sample[78]=1;sample[85]=1;
  sample[77]=1;sample[80]=1;sample[81]=1;sample[99]=0;
  seed(sample);event_step(0,reference_pins);
  seed(0);start();p=reference_pins;p[253:246]=im_ff(8'h60);p[261:254]=1;
  p[326+28]=1;p[326+31]=0;event_step(1,p);repeat(64)cycle();
  if(!flags[55])$fatal(1,"Control Hold actual FF=UseDMD did not reach gated board output");
  // SetRun gates stored UseDMD. Releasing it closes an asserted manifold
  // clock while l17 itself remains held by the HIGH root.
  p=reference_pins;p[326+46]=0;p[326+21:326+19]=0;p[326+27:326+22]=3;event_step(1,p);
  p[6]=0;event_step(1,p);if(flags[55])$fatal(1,"Control Hold UseDMD ignored SetRun");
  p[6]=1;event_step(1,p);p[326+46]=1;event_step(1,p);
  for(integer code_no=0;code_no<3;code_no++)for(integer data=0;data<64;data++)
   manifold(code_no==0?0:code_no==1?7:1,data);
  manifold(1,0);manifold(0,3);manifold(7,1);
  p=reference_pins;p[253:246]=0;p[261:254]=1;event_step(1,p);repeat(32)cycle();
  found=0;
  for(integer step=0;step<128&&!found;step++)begin
   event_step(!root,reference_pins);if(!root&&!clocks[15]&&!clocks[26])found=1;
  end
  if(!found)$fatal(1,"Control Hold did not find actual LOW MIR window");
  previous_mir=counts[15];p=reference_pins;p[326+7]=0;event_step(0,p);
  if(!flags[60+20]||counts[15]!=previous_mir+1)$fatal(1,"Control Hold actual parity stop did not close MIR");
  // The raw IM-parity producer remains external. Restore the source HIGH
  // before clearing its fault; the simultaneous raw handoff is tested
  // separately as an explicitly unqualified upstream arrival.
  event_step(1,reference_pins);p=reference_pins;p[326+7]=1;event_step(1,p);
  // Real CP rMIRa clears the set fields before every independent CP selector.
  for(integer bank=4;bank<8;bank++)for(integer data=0;data<32;data++)begin
   cp_write(0,16);cp_write(0,0);cp_cb(bank,data);
  end
  cp_write(0,0);cp_write(1,0);repeat(32)cycle();
  // NoDispatch's asynchronous task reset can close MIR with a LOW source.
  // It must use the actual updated clock view and pre-event capture data.
  seed(0);start();p=reference_pins;p[4]=1;p[261:254]=1;event_step(1,p);
  cp_write(0,40);cp_write(1,64);cp_write(0,8);cp_write(1,0);cp_write(0,0);repeat(32)cycle();
  // Store Hold through the real memory phase before requesting a higher task.
  p=reference_pins;p[326+60]=0;event_step(1,p);found=0;
  for(integer step=0;step<128&&!found;step++)begin
   event_step(!root,reference_pins);if(root&&flags[86])found=1;
  end
  if(!found)$fatal(1,"Control Hold did not store memory Hold for NoDispatch");
  p=reference_pins;p[20]=1;event_step(1,p);found=0;
  for(integer step=0;step<128&&!found;step++)begin
   event_step(!root,reference_pins);if(root&&!q[43])found=1;
  end
  if(!found)$fatal(1,"Control Hold did not create pending higher-task SwitchUp");
  found=0;
  for(integer step=0;step<128&&!found;step++)begin
   event_step(!root,reference_pins);if(!root&&!clocks[15]&&!q[69]&&!q[43]&&flags[86])found=1;
  end
  if(!found)$fatal(1,"Control Hold did not find LOW NoDispatch window");
  p=reference_pins;p[292]=1;p[291:289]=7;p[288:282]=8;event_step(0,p);
  previous_mir=counts[15];p[292]=0;event_step(0,p);p[292]=1;event_step(0,p);
  if(counts[15]!=previous_mir+1)$fatal(1,"Control Hold LOW NoDispatch did not capture MIR once");
  late_captures++;
  $display("CONTROL_HOLD_DIRECTED actual_UseDMD=1 maintenance_codes=3 CP_selectors=4 LOW_NoDispatch=%0d",late_captures);
  cp_write(0,0);p=reference_pins;p[20]=0;event_step(1,p);
  // Repeated real Hold assertions/releases also exercise the otherwise
  // unused MIR halves' RepeatCur-only clocks; never force raw CBHold.
  for(integer trial=0;trial<16;trial++)for(integer wanted=0;wanted<2;wanted++)begin
   p=reference_pins;p[326+60]=!wanted[0];event_step(1,p);found=0;
   for(integer step=0;step<128&&!found;step++)begin
    event_step(!root,reference_pins);if(root&&flags[86]===wanted[0])found=1;
   end
   if(!found)$fatal(1,"Control Hold repeated actual Hold capture timeout wanted=%0d",wanted);
  end
  for(integer i=0;i<56;i++)$display("CONTROL_HOLD_CLOCK channel=%0d edges=%0d",i,counts[i]);
  for(integer i=0;i<56;i++)if(counts[i]<8)$fatal(1,"Control Hold clock coverage incomplete channel=%0d count=%0d",i,counts[i]);
  $display("CONTROL_HOLD PASS checks=%0d physical=156 clocks=56 changed=%h",checks,changed);$finish;
 end
endmodule
'''


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(out,engine,profile,mutation=None,parity_handoff=False):
    out.mkdir(parents=True,exist_ok=False);ref=out/'reference'
    with(out/'generation.log').open('w')as log,redirect_stdout(log):r=generate(ROOT,ref)
    inputs=out/'inputs';sources={'verilog/verilator/run_control_original_ff.sv','verilog/verilator/processor_capture_original_ff.sv','verilog/cells/cell_MC10210.v',*PARENTS}
    for bn in ['ContA-Rev-Cd','ContB-Rev-Cd','MemC-Rev-Be','BaseBd-Rev-Am']:
        for ext in ['wl','lc']:sources.add('chm/sil/'+bn+'.dm!1_/'+bn+'.'+ext)
    for b in r['boards']:sources.update(b['combinational_sources'])
    for m in list(sys.modules.values()):
        n=getattr(m,'__file__',None)
        if n and Path(n).resolve().parent==ROOT/'tools':sources.add(str(Path(n).resolve().relative_to(ROOT)))
    hashes={}
    for n in sorted(sources):
        p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=sha(p)
    candidate=out/'candidate.sv';metadata=candidate_generate(candidate)
    if mutation:
        before,after=MUTATIONS[mutation];raw=candidate.read_text();assert raw.count(before)==1
        candidate.write_text(raw.replace(before,after))
    timed=out/'timed';timing=stage(inputs,ref,timed,*PROFILES[profile])
    ready=timed/'ready_ff.sv';ready.write_text(ORACLE.replace('DELAY_PS=1000','DELAY_PS='+str(PROFILES[profile][1])));timing['files'][ready.name]=sha(ready)
    (timed/'timing-profile.json').write_text(json.dumps(timing,indent=2)+'\n')
    tb=out/'tb.sv';bench=TB
    if parity_handoff:
        before='  event_step(1,reference_pins);p=reference_pins;p[326+7]=1;event_step(1,p);'
        assert bench.count(before)==1
        bench=bench.replace(before,'  $display("CONTROL_HOLD_RAW_PARITY_HANDOFF old_root=%b new_root=1 stable_IM=1",root);\n  p[326+7]=1;event_step(1,p);')
    tb.write_text(bench);support=[candidate,tb,*sorted(timed.glob('*.v')),*sorted(timed.glob('*.sv'))]
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),actions=[],engine=engine,profile=profile,mutation=mutation,parity_handoff=parity_handoff,inputs=hashes,
               reference=r,candidate=metadata,timing=timing,compiled_sources={str(p.relative_to(out)):sha(p)for p in support},
               executed_candidate_sha256=sha(candidate),testbench_sha256=sha(tb),native_integrated=False,physical_timing_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    def execute(cmd,log):
        with log.open('w')as f:
            p=subprocess.Popen(list(map(str,cmd)),stdout=f,stderr=subprocess.STDOUT);row=dict(command=list(map(str,cmd)),pid=p.pid,started=time.time());state['actions'].append(row);save();rc=p.wait()
            row.update(returncode=rc,finished=time.time());save();return rc
    save()
    cmd=['iverilog','-g2012','-s','tb_control_hold','-o',out/'test.vvp',*support]if engine=='icarus'else[
        'verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_control_hold','--Mdir',out/'obj','-o','test',*support]
    rc=execute(cmd,out/'compile.log')
    if rc==0:rc=execute(['vvp','-i',out/'test.vvp']if engine=='icarus'else[out/'obj/test'],out/'test.log')
    text=(out/'test.log').read_text()if(out/'test.log').exists()else'';summary=re.search(r'^CONTROL_HOLD PASS ([^\n]+)$',text,re.M)
    state.update(status='PASS_CONTROL_HOLD_PROBE'if rc==0 and summary else'CONTROL_HOLD_PROBE_FAILED',summary=summary[1]if summary else None,finished=time.time())
    save();print(state['status'],flush=True);return rc if rc else(0 if summary else 1)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--engine',choices=['icarus','verilator'],default='verilator')
    p.add_argument('--profile',choices=PROFILES,default='balanced');p.add_argument('--mutation',choices=MUTATIONS);p.add_argument('--parity-handoff',action='store_true')
    a=p.parse_args();sys.exit(run(a.output.resolve(),a.engine,a.profile,a.mutation,a.parity_handoff))
