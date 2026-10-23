#!/usr/bin/env python3
"""Compare coupled original MemC phase/hold and ContB MIR capture ordering.

The original memory registers produce CBHold; raw pin3 is now obsolete. Test
actual held-data/release/new-capture epochs and clock enables, with upstream
CA/MCR/hold-data/IM/DMD boundaries explicit. Not native or analog qualification.
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
from generate_contb_hold_reference import generate,ROOT
from generate_contb_hold_candidate import generate as candidate_generate
from test_contb_mir import TB as PARENT,PROFILES
from stage_task_capture_timing import stage

MUTATIONS={
    'external_hold':('hold_at(q),pins_after[2:0]','pins_after[3],pins_after[2:0]'),
    'early_hold':('hold_at(q),pins_after[2:0]','(pins_after[62]|!pins_after[60]|!pins_after[61]),pins_after[2:0]'),
    'old_hold':('hold_at(q),pins_after[2:0]','hold_at(state_q[45:40]),pins_after[2:0]'),
    'drop_reference_hold':('q[4] | !q[2] | !q[3]','q[4] | !q[3]'),
    'phase_polarity':('phase1=p[0]|p[56]|!q[0]','phase1=p[0]|p[56]|q[0]'),
    'ignore_ignoreproc':('phase1|p[57]','phase1'),
    'retry_clock':('memory_clocks={phase1,','memory_clocks={phase1|p[57],'),
}


def bench():
    raw=PARENT[:PARENT.index('\n reg [55:0] p;')]
    edits=[('tb_contb_mir','tb_contb_hold'),('[55:0]','[71:0]'),("56'h","72'h"),
           ('[39:0]','[45:0]'),('[24:0]','[30:0]'),('[25:0]','[33:0]'),('0:24','0:30'),
           ('i<25','i<31'),('i<40','i<46'),('dorado_contb_mir_step','dorado_contb_hold_step'),
           ("(72'h1<<46);","(72'h1<<46)|(72'h1<<58)|(72'h1<<60)|(72'h1<<61);"),
           ('contb_mir_reference original(load,fixture,1\'b0,1\'b0,1\'b0,reference_pins,reference,clocks,ref_flags);',
            'contb_hold_reference original(load,fixture,reference_pins,reference,clocks,ref_flags);'),
           ('fixture[34]=0;','fixture[34]=0;fixture[40]=1;fixture[41]=0;fixture[43:42]=3;fixture[44]=0;')]
    for old,new in edits:
        assert old in raw,old;raw=raw.replace(old,new)
    raw=raw.replace('ContB MIR','ContB hold')
    raw+=r'''
 reg [71:0] p;reg [45:0] saved;integer before_counts[0:30],release_checks=0;
 initial begin
  seed(46'h3fffffffffff);event_pins(SAFE);seed(0);
  // Each of the three actual Hold drivers is asserted then released while
  // data for the next instruction is already stable. No raw CBHold forcing.
  for(integer source=0;source<3;source++)for(integer trial=0;trial<32;trial++)begin
   p=SAFE;p[2:1]=0;p[45:28]=18'h00000;event_pins(p);
   p[0]=0;event_pins(p);
   if(source==0)p[60]=0;else if(source==1)p[61]=0;else p[62]=1;
   event_pins(p);p[0]=1;event_pins(p);
   if(!flags[26]||!flags[27]||!flags[28]||q[1]!==0)$fatal(1,"ContB hold setup did not assert actual memory Hold");
   p[0]=0;event_pins(p);saved=q;
   p[45:28]=18'h3ffff;p[60]=1;p[61]=1;p[62]=0;p[3]=trial[0];event_pins(p);
   if(!flags[27]||q[39:0]!==saved[39:0])$fatal(1,"ContB hold changed before actual phase-1 capture");
   for(integer i=0;i<31;i++)before_counts[i]=counts[i];
   p[0]=1;event_pins(p);
   if(flags[27]||q[39:0]!==saved[39:0])$fatal(1,"ContB hold release captured new IM too early");
   for(integer i=0;i<22;i++)if(counts[i]!=before_counts[i])$fatal(1,"ContB hold release created a MIR clock edge");
   p[0]=0;event_pins(p);p[0]=1;event_pins(p);
   if(q[1]!==1)$fatal(1,"ContB hold next instruction did not capture released IM");
   release_checks++;
  end
  $display("CONTB_HOLD_RELEASE actual_sources=3 stable_data_cases=%0d no_early_MIR_edge=1",release_checks);
  // Actual phase register and all four hold-package halves remain live.
  for(integer trial=0;trial<1024;trial++)begin
   p[0]=1;event_pins(p);
   p[56]=trial[3];p[57]=trial[4];p[58]=(trial%3!=0);p[3]=trial[5];p[4]=trial[6];
   p[60]=!trial[0];p[61]=!trial[1];p[62]=trial[2];p[63]=trial[7];
   p[45:28]=18'(random_word());event_pins(p);
   p[0]=0;event_pins(p);
   p[45:28]=18'(random_word());event_pins(p);p[0]=1;event_pins(p);
  end
  // IgnoreProc can close the phase-1 window while the root remains LOW.
  p=SAFE;p[2:1]=0;p[56]=0;p[57]=0;p[58]=1;event_pins(p);
  p[0]=0;event_pins(p);
  for(integer data=0;data<16;data++)begin
   p[60]=data[0];p[61]=data[1];p[62]=data[2];p[63]=data[3];event_pins(p);
   p[57]=1;event_pins(p);p[57]=0;event_pins(p);
  end
  // DisHold reaches the real async set/reset pins, including the unused
  // e23 B-half BLretry state. It may release Hold at a LOW root.
  p[59]=1;event_pins(p);
  if(flags[27]||q[45:42]!==4'b1011)$fatal(1,"ContB hold DisHold polarity mismatch");
  p[59]=0;event_pins(p);p[0]=1;event_pins(p);
  // Maintenance captures and both shared-reset drivers still coexist with
  // the memory-produced Hold path. Parity inputs stay in their healthy state.
  p=SAFE;event_pins(p);
  for(integer code_no=0;code_no<3;code_no++)for(integer data=0;data<64;data++)
   manifold(code_no==0?0:code_no==1?7:1,data);
  manifold(1,0);p=SAFE;p[6]=1;event_pins(p);p[6]=0;event_pins(p);
  for(integer selector=0;selector<4;selector++)for(integer data=0;data<32;data++)begin
   p=SAFE;p[6]=1;event_pins(p);p[6]=0;event_pins(p);
   p[10]=0;p[11]=1'(selector>>1);p[12]=selector[0];p[17:14]=4'(data);p[18]=data[4];event_pins(p);
   p[13]=0;event_pins(p);p[13]=1;event_pins(p);
  end
  for(integer i=0;i<31;i++)if(counts[i]<16)$fatal(1,"ContB hold clock coverage incomplete channel=%0d edges=%0d",i,counts[i]);
  if(seen_zero!==46'h3fffffffffff||seen_one!==46'h3dffffffffff)$fatal(1,"ContB hold physical Q coverage incomplete zero=%h one=%h",seen_zero,seen_one);
  for(integer i=0;i<31;i++)$display("CONTB_HOLD_CLOCK channel=%0d edges=%0d",i,counts[i]);
  $display("CONTB_HOLD PASS checks=%0d physical=46 clocks=31 releases=%0d",checks,release_checks);$finish;
 end
endmodule
'''
    return raw


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(out,engine,profile,mutation=None):
    out.mkdir(parents=True,exist_ok=False);ref=out/'reference';r=generate(ROOT,ref);assert r['pin_count']==72
    inputs=out/'inputs';sources={'verilog/verilator/run_control_original_ff.sv','verilog/verilator/processor_capture_original_ff.sv','verilog/cells/cell_MC10210.v'}
    for bn in ['ContA-Rev-Cd','ContB-Rev-Cd','MemC-Rev-Be','BaseBd-Rev-Am']:
        for ext in ['wl','lc']:sources.add('chm/sil/'+bn+'.dm!1_/'+bn+'.'+ext)
    for b in r['boards']:sources.update(b['combinational_sources'])
    for m in list(sys.modules.values()):
        n=getattr(m,'__file__',None)
        if n and Path(n).resolve().parent==ROOT/'tools':sources.add(str(Path(n).resolve().relative_to(ROOT)))
    hashes={}
    for n in sorted(sources):
        p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=sha(p)
    candidate=out/'candidate.sv';metadata=candidate_generate(candidate,r['pin_count']);support_cb=out/'candidate-cb.sv'
    if mutation:
        before,after=MUTATIONS[mutation];raw=candidate.read_text();assert raw.count(before)==1
        candidate.write_text(raw.replace(before,after))
    timed=out/'timed';timing=stage(inputs,ref,timed,*PROFILES[profile]);tb=out/'tb.sv';tb.write_text(bench())
    # BaseBd source MC10210 is also used in the coupled reference.
    assert (timed/'cell_MC10210.v').exists()
    support=[candidate,support_cb,tb,*sorted(timed.glob('*.v')),*sorted(timed.glob('*.sv'))]
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),actions=[],engine=engine,profile=profile,mutation=mutation,inputs=hashes,
               reference=r,candidate=metadata,timing=timing,compiled_sources={str(p.relative_to(out)):sha(p)for p in support},
               executed_candidate_sha256=sha(candidate),testbench_sha256=sha(tb),native_integrated=False,physical_timing_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    def execute(cmd,log):
        with log.open('w')as f:
            p=subprocess.Popen(list(map(str,cmd)),stdout=f,stderr=subprocess.STDOUT);row=dict(command=list(map(str,cmd)),pid=p.pid,started=time.time());state['actions'].append(row);save();rc=p.wait()
            row.update(returncode=rc,finished=time.time());save();return rc
    save()
    cmd=['iverilog','-g2012','-s','tb_contb_hold','-o',out/'test.vvp',*support]if engine=='icarus'else[
        'verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_contb_hold','--Mdir',out/'obj','-o','test',*support]
    rc=execute(cmd,out/'compile.log')
    if rc==0:rc=execute(['vvp','-i',out/'test.vvp']if engine=='icarus'else[out/'obj/test'],out/'test.log')
    text=(out/'test.log').read_text()if(out/'test.log').exists()else'';summary=re.search(r'^CONTB_HOLD PASS ([^\n]+)$',text,re.M)
    state.update(status='PASS_CONTB_HOLD_PROBE'if rc==0 and summary else'CONTB_HOLD_PROBE_FAILED',summary=summary[1]if summary else None,finished=time.time())
    save();print(state['status'],flush=True);return rc if rc else(0 if summary else 1)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--engine',choices=['icarus','verilator'],default='verilator')
    p.add_argument('--profile',choices=PROFILES,default='balanced');p.add_argument('--mutation',choices=MUTATIONS)
    a=p.parse_args();sys.exit(run(a.output.resolve(),a.engine,a.profile,a.mutation))
