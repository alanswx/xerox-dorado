#!/usr/bin/env python3
"""Compare actual Control-A tasking/branch producers with original circuitry.

All104 physical Qs,23 clocks and51 flags are compared after each event. The
reference retains actual FF decode and d17 feedback. This is component ordering,
not native IM/IFU/Hold arrival, physical timing, boot or60ns qualification.
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

from generate_conta_local_reference import generate,plan,ROOT
from sil_netlist import load_board
from test_conta_mir import TB as PARENT_TB,MUTATIONS as PARENT_MUTATIONS,PROFILES
from test_task_ready import ORACLE

CANDIDATE='verilog/rtl/dorado_conta_local_step.sv'
MUTATIONS={**PARENT_MUTATIONS,
 'external_branch_stretch':('c[1]=d[1];','c[1]=p[1];'),
 'external_StopTasks_clock':('c[11]=normal0 | d[0];','c[11]=normal0 | p[65];'),
 'wrong_JCN_copy':('rwtpc=q[89] &','rwtpc=q[91] &'),
 'lost_tasking_capture':('if(capture[22])n[103:98]',"if(1'b0)n[103:98]"),
}
MUTATION_PROFILES={'balanced'}


def bench():
 s=PARENT_TB
 for a,b in [('tb_conta_mir','tb_conta_local'),('conta_mir_reference','conta_local_reference'),
  ('dorado_conta_mir_step','dorado_conta_local_step'),
  ('<<142','<<141'),('[97:0]','[103:0]'),('[21:0]','[22:0]'),('[42:0]','[50:0]'),
  ('0:21','0:22'),('k<22','k<23'),('i<22','i<23'),('[127:0]','[129:0]'),('CONTA_MIR','CONTA_LOCAL')]:s=s.replace(a,b)
 s=s.replace('wire unsettled;', 'wire [7:0] local_flags;wire unsettled;')
 s=s.replace('unsettled,flags,next_task);','unsettled,flags,next_task,local_flags);')
 s=s.replace('{next_task,flags}','{local_flags,next_task,flags}')
 s=s.replace('integer checks=0,', "reg [5:0] tasking_changed=0;integer checks=0,")
 s=s.replace('committed_flags=flags;state=next_q;', 'tasking_changed|=state[103:98]^next_q[103:98];committed_flags=flags;state=next_q;')
 s=s.replace('state=next_q;checks++;','state=next_q;checks++;if(checks%4096==0)$display("CONTA_LOCAL_PROGRESS checks=%0d",checks);')
 # StopTasks starts in a stopped state; enable tasking via the actual MIR.
 marker='p[125]=0;event_pins(1,p);';assert s.count(marker)==1
 s=s.replace(marker,marker+r'''
   p=pins;p[83:76]=8'hc6;p[91:84]=8'h01;p[123]=0;p[124]=1;event_pins(1,p);
   repeat(16)cycle();
   p=pins;p[83:76]=0;event_pins(1,p);repeat(16)cycle();
   if(local_flags[0])$fatal(1,"TaskingOn did not release stored StopTasks");
''')
 # Add real tasking transitions and independent physical MIR-copy checks.
 marker='  // Independent BNT packages';assert s.count(marker)==1
 directed=r'''
  // FA=1,FB=4,FC=2/3: original TaskingOff/On, bit-reversed at dFF pins.
  seed(0);start();
  for(integer n=0;n<32;n++)begin
   event_pins(1,pins);p=pins;p[83:76]=8'h46;p[91:84]=8'h01;p[123]=0;p[124]=1;event_pins(1,p);
   repeat(16)cycle();event_pins(1,pins);p=pins;p[83:76]=0;event_pins(1,p);repeat(16)cycle();
   if(!local_flags[0]||state[99])$fatal(1,"TaskingOff did not persist after MIR changed");
   event_pins(1,pins);p=pins;p[83:76]=8'hc6;event_pins(1,p);repeat(16)cycle();
   event_pins(1,pins);p=pins;p[83:76]=0;event_pins(1,p);repeat(16)cycle();
   if(local_flags[0]||!state[99])$fatal(1,"TaskingOn did not persist after MIR changed");
  end
  // Midas and both values of otherwise unobserved d17 package Qs are kept.
  event_pins(1,pins);p=pins;p[83:76]=8'h86;event_pins(1,p);repeat(16)cycle();
  if(!state[100])$fatal(1,"MidasOn was not captured in d17");
  event_pins(1,pins);p=pins;p[83:76]=0;event_pins(1,p);repeat(16)cycle();
  if(state[100])$fatal(1,"d17 Midas state did not release");
  q=0;q[103:98]=6'b111111;seed(q);event_pins(0,pins);
  if(state[103:98]!==6'b111111)$fatal(1,"Unclocked d17 Qs were initialized");
  start();repeat(16)cycle();
  // d23 and d24 hold separate physical copies, including legal startup
  // disagreement. RWTPC reads d23.B, not the normal-data d24 copy.
  for(integer copy=0;copy<2;copy++)begin
   q=0;q[89]=copy[0];q[91]=!copy[0];q[85]=0;
   q[84]=1;q[86]=1;q[94]=1;q[95]=1;seed(q);event_pins(0,pins);
   if(local_flags[1]!==copy[0])$fatal(1,"RWTPC used wrong physical JCN.2 copy");
  end
'''
 s=s.replace(marker,directed+'\n'+marker)
 # Ordinary/extended cadence now comes from actual MIR JCN, not pin1.
 s=s.replace(' reg [5:0] tasking_changed=0;', ' integer last_start,intervals,ordinary_intervals=0,extended_intervals=0;reg old_start;\n reg [5:0] tasking_changed=0;')
 marker='  if(cp_edges<256 || mir_edges<100';assert s.count(marker)==1
 cadence=r'''
  for(integer ext=0;ext<2;ext++)begin
   seed(0);start();event_pins(1,pins);p=pins;
   p[4]=1;p[8]=1;p[1]=!ext[0];p[91:84]=ext ? 8'he6 : 8'h01;event_pins(1,p);repeat(16)cycle();
   if(local_flags[1]!==ext[0])$fatal(1,"Actual JCN branch-stretch decode mismatch");
   last_start=-1;intervals=0;
   for(integer n=0;n<800;n++)begin
    old_start=state[25];event_pins(n[0],pins);
    if(!old_start && state[25])begin
     if(last_start>=0)begin
      if(counts[0]-last_start!=(ext?6:2))$fatal(1,"Actual JCN instruction cadence mismatch ext=%0d clocks=%0d",ext,counts[0]-last_start);
      intervals++;
     end
     last_start=counts[0];
    end
   end
   if(intervals<(ext?50:150))$fatal(1,"Actual JCN cadence coverage incomplete");
   if(ext)extended_intervals=intervals;else ordinary_intervals=intervals;
  end
  $display("CONTA_LOCAL_CADENCE ordinary=%0d extended=%0d",ordinary_intervals,extended_intervals);
'''
 s=s.replace(marker,cadence+marker)
 s=s.replace('p[83:76]=8\'(random_word());', "p[128]=trial[0];p[129]=trial[1];p[83:76]=8'(random_word());")
 s=s.replace('  $display("CONTA_LOCAL PASS', '  if(tasking_changed!==6\'b111111)$fatal(1,"d17 physical-Q coverage incomplete");\n  $display("CONTA_LOCAL_TASKING changed=%h edges=%0d",tasking_changed,counts[22]);\n  $display("CONTA_LOCAL PASS')
 return s


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(out,engine,profile,mutations):
 assert not out.exists();out.mkdir(parents=True);inputs=out/'inputs'
 spec,extras=plan(ROOT);board=load_board(str(ROOT/'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl'))
 kinds=sorted({board.packages[p]['type']for p in set(spec['comb'])|set(spec['seq'])})
 sources={CANDIDATE,'verilog/rtl/dorado_conta_mir_step.sv',
  'verilog/verilator/run_control_original_ff.sv','verilog/verilator/processor_capture_original_ff.sv',
  'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl','chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.lc',
  *[f'verilog/cells/cell_{k}.v'for k in kinds]}
 for m in list(sys.modules.values()):
  name=getattr(m,'__file__',None)
  if name and Path(name).resolve().parent==ROOT/'tools':sources.add(str(Path(name).resolve().relative_to(ROOT)))
 hashes={}
 for n in sorted(sources):
  p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=sha(p)
 def functions(raw):
  found={}
  for body in re.findall(r'function automatic.*?endfunction',raw,re.S):
   name=re.match(r'function automatic\s+(?:\[[^]]+\]\s+)?(\w+)\(',body)[1]
   assert name not in found;found[name]=body
  return found
 old=functions((inputs/'verilog/rtl/dorado_conta_mir_step.sv').read_text())
 new=functions((inputs/CANDIDATE).read_text());reuse={}
 for n in ['controller_clocks','controller_transfer','reverse_task','current_task','next_raw','mir_decode',
           'is_next','priority_raw','task_transfer','ready_transfer','cp_transfer','mir_transfer']:
  body=new[n]
  if n not in ['controller_clocks','controller_transfer']:
   body=body.replace('[103:0]','[97:0]').replace('[129:0]','[127:0]')
  assert body==old[n],('changed inherited function',n)
  reuse[n]=dict(sha256=hashlib.sha256(body.encode()).hexdigest(),type_width_adaptation=n not in ['controller_clocks','controller_transfer'])
 tb=inputs/'tb_conta_local.sv';tb.write_text(bench());hashes[tb.name]=sha(tb)
 ref=out/'reference';record=generate(inputs,ref);assert(record['physical_bits'],record['pin_count'])==(104,143)
 state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),actions=[],engine=engine,profile=profile)
 def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
 def execute(cmd,log):
  with log.open('w')as f:
   process=subprocess.Popen(list(map(str,cmd)),cwd=inputs,stdout=f,stderr=subprocess.STDOUT)
   row=dict(command=list(map(str,cmd)),pid=process.pid,started=time.time(),log=str(log));state['actions'].append(row);save()
   rc=process.wait();row.update(returncode=rc,finished=time.time());save()
  return rc
 save();rows=[];negative_cases=[]
 for label,gate,ff,phase in [p for p in PROFILES if profile in ('all',p[0])]:
  timed=out/label/'reference';timed.mkdir(parents=True)
  for kind in kinds:
   if kind in ('F10000','MC10231','SE10231','MC10176','MC10141'):continue
   raw=(inputs/f'verilog/cells/cell_{kind}.v').read_text();raw=re.sub(r'`timescale[^\n]*\n','',raw)
   raw=re.sub(r'\bassign\s+(p\d+)\b',lambda m:f'assign #({gate}) '+m[1],raw)
   (timed/f'cell_{kind}.v').write_text('`timescale 1ps/1ps\n'+raw)
  for n in ['run_control_original_ff.sv','processor_capture_original_ff.sv']:
   raw=(inputs/'verilog/verilator'/n).read_text();raw=re.sub(r'`timescale[^\n]*\n','',raw)
   chunks=re.split(r'(module original_SE10231\b.*?endmodule)',raw,flags=re.S)
   raw=''.join(s.replace('<=',f'<=#({phase if s.startswith("module original_SE10231")else ff}) ')for s in chunks)
   (timed/n).write_text('`timescale 1ps/1ps\n'+raw)
  (timed/'ready_ff.sv').write_text(ORACLE.replace('DELAY_PS=1000',f'DELAY_PS={ff}'))
  variants=[('original',(inputs/CANDIDATE).read_text())]
  if mutations and label in MUTATION_PROFILES:
   for name,(a,b)in MUTATIONS.items():
    assert variants[0][1].count(a)==1,(name,a);variants.append((name,variants[0][1].replace(a,b)))
  for sim in ['icarus','verilator']if engine=='both'else[engine]:
   for name,source in variants:
    d=out/label/(sim+'-'+name);d.mkdir();candidate=d/'candidate.sv';candidate.write_text(source)
    support=[candidate,tb,ref/'conta_local_reference.sv',*sorted(timed.glob('*.v')),*sorted(timed.glob('*.sv'))]
    command=['iverilog','-g2012','-s','tb_conta_local','-o',d/'test.vvp',*support]if sim=='icarus'else[
     'verilator','--binary','--timing','--top-module','tb_conta_local','-Wno-fatal','--Mdir',d/'obj','-o','test',*support]
    assert execute(command,d/'compile.log')==0,d/'compile.log'
    rc=execute(['vvp','-i',d/'test.vvp']if sim=='icarus'else[d/'obj/test'],d/'test.log');log=(d/'test.log').read_text()
    if name=='original':
     assert rc==0 and 'CONTA_LOCAL PASS'in log,(label,sim,d/'test.log')
     summary=re.search(r'CONTA_LOCAL PASS ([^\n]+)',log)[1];print(label,sim,summary,flush=True)
     rows.append(dict(profile=label,engine=sim,summary=summary))
    else:
     assert rc!=0 and 'Control-A coupled mismatch'in log,(name,sim,d/'test.log')
     negative_cases.append(dict(profile=label,engine=sim,case=name,returncode=rc))
 assert len({r['summary']for r in rows})==1,rows
 for n,h in hashes.items():assert sha(inputs/n)==h,n
 result=dict(status='PASS_CONTA_LOCAL_COMPONENT'if engine=='both'and profile=='all'and mutations else'PASS_CONTA_LOCAL_PROBE',
  scope=__doc__,inputs=hashes,reference=record,primary=rows,
  rejected_mutations=sorted({r['case']for r in negative_cases}),negative_cases=negative_cases,
  actions=state['actions'],inherited_function_reuse=reuse,native_or_timing_qualified=False)
 (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');state.update(status=result['status'],finished=time.time());save()
 print(result['status'],flush=True)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
 p.add_argument('--engine',choices=['both','icarus','verilator'],default='both')
 p.add_argument('--profile',choices=['all',*[x[0]for x in PROFILES]],default='all');p.add_argument('--mutations',action='store_true')
 a=p.parse_args()
 try:run(a.output.resolve(),a.engine,a.profile,a.mutations)
 except BaseException as e:
  path=a.output/'run-state.json'
  if path.exists():
   s=json.loads(path.read_text());s.update(status='FAILED_REVIEW_REQUIRED',error=repr(e),finished=time.time());path.write_text(json.dumps(s,indent=2)+'\n')
  raise
