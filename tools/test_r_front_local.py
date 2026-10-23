#!/usr/bin/env python3
"""Compare local Control-A producers in the full processor/R-front graph.

All792 physical Qs,9216 functional RAM bits and68 clocks compare per event.
Generated adapters separate StopTasks data from its clock view. This experiment
is not native integration, a complete arrival contract or physical timing proof.
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

from generate_r_front_local_reference import generate
from generate_r_front_local_adapters import generate as adapters_generate
from stage_r_operand_timing import stage
from test_r_front_mir import bench_source as parent_bench,PROFILES
from test_task_ready import ORACLE

ROOT=Path(__file__).resolve().parents[1]
CANDIDATE='verilog/rtl/dorado_r_front_local_step.sv'


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def bench(path,iterations,random,cp,low_cp,late_tasking=False,front_directed=False):
 raw=parent_bench(path,iterations,random,cp,False,low_cp)
 for a,b in [('tb_r_front_mir','tb_r_front_local'),('dorado_r_front_mir_step','dorado_r_front_local_step'),
  ('r_front_mir_reference','r_front_local_reference'),('[785:0]','[791:0]'),('[309:0]','[314:0]'),
  ('rp[309:145]','rp[314:145]'),('[66:0]','[67:0]'),('0:66','0:67'),('i<67','i<68'),
  ('i<786','i<792'),('[97:0] conta_next','[103:0] conta_next'),('channel<67','channel<68'),
  ("SEED=(310'h3", "SEED=(315'h1<<311)|(315'h1<<313)|(310'h3")]:raw=raw.replace(a,b)
 raw=raw.replace("fixture[751:736]=16'hffff;", "fixture[787]=1;fixture[751:736]=16'hffff;")
 raw=raw.replace('p[263]=trial[0];p[294]=trial[1];',
  'p[263]=trial[0];p[294]=trial[1];p[310]=trial[2];p[297]=trial[3];p[311]=trial[4];p[313]=trial[5];')
 raw=raw.replace('   for(integer channel=0;', '''
   for(integer half=0;half<2;half=half+1)
     if(write_address_changes[half]||write_select_changes[half])$fatal(1,"Local front write-window address/select movement");
   for(integer channel=0;''')
 raw=raw.replace('R_FRONT_MIR','R_FRONT_LOCAL')
 # Both d21 outputs cross the board boundary. jcnt currently reaches a spare
 # processor gate half, so state-only comparison cannot qualify that wire.
 marker='     if(ready_flags!==ref_ready_flags)'
 assert raw.count(marker)==1
 raw=raw.replace(marker,'''     if({dut.settled_pins[212],dut.settled_pins[235]} !==
        {original.cpu_pins[212],original.cpu_pins[235]})
       $fatal(1,"Local front Control-A board crossing mismatch check=%0d",checks);
'''+marker)
 if late_tasking or front_directed:
  marker=' reg [314:0] p;';assert raw.count(marker)==1
  raw=raw.replace(marker,marker+r'''
 reg found;
 function automatic [7:0] im_ff(input [7:0] ff);im_ff={ff[0],ff[1],ff[2],ff[3],ff[4],ff[5],ff[6],ff[7]};endfunction
 task automatic cp_write(input integer bank,data);
  begin
   if(!cb)event_step(1,p);
   p[292]=1;p[291:289]=~3'(bank);p[288:282]=7'(data);event_step(1,p);
   p[292]=0;event_step(1,p);p[292]=1;event_step(1,p);
  end
 endtask
''')
 if late_tasking:
  marker='   // IM data settles';assert raw.count(marker)==1
  directed=r'''
   // Clear Ready and select emulator task through real CP transactions.
   if(!cb)event_step(1,p);
   p=SEED;p[8:0]=9'h178;p[293]=0;p[294]=0;p[205]=1;p[254]=1;p[204:202]=3'b110;event_step(1,p);
   cp_write(0,40);cp_write(1,64);cp_write(0,8);cp_write(1,0);cp_write(0,0);
   for(integer n=0;n<32;n++)event_step(!cb,p);
   event_step(1,p);p[253:246]=im_ff(8'h62);event_step(1,p);
   for(integer n=0;n<32;n++)event_step(!cb,p);
   event_step(1,p);p[253:246]=0;event_step(1,p);
   for(integer n=0;n<32;n++)event_step(!cb,p);
   if(q[787])$fatal(1,"Local front did not store TaskingOff");
   event_step(1,p);p[20]=1;event_step(1,p);
   for(integer n=0;n<16;n++)event_step(!cb,p);
   if(q[73])$fatal(1,"Local front setup switched while tasking stopped");
   event_step(1,p);p[253:246]=im_ff(8'h63);event_step(1,p);found=0;
   for(integer n=0;n<32&&!found;n++)begin
    event_step(!cb,p);if(q[771:764]==8'h36&&!q[787])found=1;
   end
   if(!found||!cb)$fatal(1,"Local front setup missed pending TaskingOn");
   found=0;
   for(integer n=0;n<32&&!found;n++)begin
    event_step(!cb,p);if(!cb&&!clocks[67]&&!q[787])found=1;
   end
   if(!found)$fatal(1,"Local front setup did not open LOW window");
   p[292]=1;p[291:289]=7;p[288:282]=16;event_step(0,p);
   p[292]=0;event_step(0,p);p[292]=1;event_step(0,p);
   if(q[787]||!q[73])$fatal(1,"Local front lost old task data on late StopTasks clock");
   $display("R_FRONT_LOCAL_LATE_TASKING old_data=1 root=%b switch=%b tasking_n=%b",cb,q[73],q[787]);
   event_step(1,p);p[20]=0;cp_write(0,40);cp_write(0,8);cp_write(0,0);
   p[253:246]=0;p[261:254]=8'he6;event_step(1,p);
   for(integer n=0;n<32;n++)event_step(!cb,p);
   found=0;
   for(integer n=0;n<96&&!found;n++)begin
    event_step(!cb,p);
    if(!cb&&q[29]&&!q[31]&&!q[6]&&q[7]&&q[777]&&!q[773]&&q[772]&&q[774]&&q[782]&&q[783])found=1;
   end
   if(!found)$fatal(1,"Local front did not reach extended LOW branch window");
   p[292]=1;p[291:289]=7;p[288:282]=24;event_step(0,p);
   p[292]=0;event_step(0,p);p[292]=1;event_step(0,p);
   if(q[24])$fatal(1,"Local front lost old branch data on late NoDispatch clock");
   $display("R_FRONT_LOCAL_LATE_BRANCH old_stretch=1 root=%b StartCycle_c=%b",cb,q[24]);
   event_step(1,p);cp_write(0,0);
'''
  raw=raw.replace(marker,directed+'\n'+marker)
 if front_directed:
  marker='   // IM data settles';assert raw.count(marker)==1
  directed=r'''
   // Store TaskingOff through actual MIR before maintenance task selection.
   // Raw pin65 is no longer a producer; no forced StopTasks or planted Qs.
   if(!cb)event_step(1,p);
   p=SEED;p[8:0]=9'h178;p[293]=0;p[294]=0;p[205]=1;p[254]=1;p[204:202]=3'b110;event_step(1,p);
   cp_write(0,40);cp_write(1,64);cp_write(0,8);cp_write(1,0);cp_write(0,0);
   for(integer n=0;n<32;n++)event_step(!cb,p);
   event_step(1,p);p[253:246]=im_ff(8'h62);event_step(1,p);
   for(integer n=0;n<32;n++)event_step(!cb,p);
   event_step(1,p);p[253:246]=0;event_step(1,p);
   for(integer n=0;n<32;n++)event_step(!cb,p);
   if(q[787])$fatal(1,"Directed RBase/StkP setup did not store TaskingOff");
   for(integer pass_no=0;pass_no<2;pass_no=pass_no+1)
     for(integer task_no=0;task_no<16;task_no=task_no+1)begin
       p=SEED;p[8:0]=9'h178;p[293]=0;
       cp_write(0,40);p[74:71]=0;event_step(1,p);
       cp_write(0,8);p[74:71]=4'(task_no);event_step(1,p);
       p[74:71]=0;event_step(1,p);cp_write(0,0);
       p[204:202]=3'b110;p[205]=1;p[254]=1;
       p[253:246]=im_ff(8'h80|(pass_no ? 8'h0f : 8'h00));
       event_step(1,p);
       for(integer n=0;n<32;n++)event_step(!cb,p);
       if(q[787])$fatal(1,"Directed RBase unexpectedly enabled tasking");
       if(task_no==15 && q[703:640] !== (pass_no ? 64'b0 : ~64'b0))
         $fatal(1,"Local-directed RBase fill mismatch pass=%0d actual=%h",pass_no,q[703:640]);
     end
   p=SEED;p[8:0]=9'h178;p[293]=0;
   cp_write(0,40);p[74:71]=0;event_step(1,p);cp_write(0,8);cp_write(0,0);
   p[204:202]=3'b110;p[205]=1;p[254]=1;p[263]=1;p[253:246]=im_ff(8'o144);
   for(integer value=0;value<256;value=value+1)begin
     for(integer bit_no=0;bit_no<8;bit_no=bit_no+1)p[227+bit_no]=(value>>(7-bit_no))&1;
     event_step(1,p);
     for(integer n=0;n<16;n++)event_step(!cb,p);
     if({q[710],q[711],q[712],q[713],q[714],q[715],q[717],q[716]} !== 8'(value))
       $fatal(1,"Local-directed StkP load mismatch value=%h actual=%b",value,{q[710],q[711],q[712],q[713],q[714],q[715],q[717],q[716]});
   end
   $display("R_FRONT_LOCAL_DIRECTED RBase=32 StkP=256 changed=%h",front_changed);
   // Restore actual TaskingOn for the subsequent randomized producers.
   event_step(1,p);p[294]=0;p[253:246]=im_ff(8'h63);event_step(1,p);
   for(integer n=0;n<32;n++)event_step(!cb,p);
   if(!q[787])$fatal(1,"Directed RBase/StkP cleanup did not enable tasking");
   p=SEED;event_step(1,p);
'''
  raw=raw.replace(marker,directed+'\n'+marker)
 return raw


def run(base,out,iterations,engine,profile,random,cp,low_cp,late_tasking,mutation,front_directed,cone_rewrite=None,extra_sources=()):
 assert not out.exists();out.mkdir(parents=True);inputs=out/'inputs'
 parent=json.loads((base/'results.json').read_text());assert parent['status']=='PASS_R_FRONT_READY_ARRIVAL_CONTRACT'
 names=set(parent['inputs'])-{'tb_r_front_ready.sv'}
 names.update([CANDIDATE,'verilog/rtl/dorado_conta_local_step.sv'])
 names.update(extra_sources)
 for m in list(sys.modules.values()):
  n=getattr(m,'__file__',None)
  if n and Path(n).resolve().parent==ROOT/'tools':names.add(str(Path(n).resolve().relative_to(ROOT)))
 hashes={}
 for n in sorted(names):
  source=ROOT/n
  if n in parent['inputs']and n.startswith(('verilog/','chm/')):assert sha(source)==parent['inputs'][n],n
  p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(source.read_bytes());hashes[n]=sha(p)
 tb=inputs/'tb_r_front_local.sv';tb.write_text(bench(base/'executed-inputs/tb_r_front_ready.sv',iterations,random,cp,low_cp,late_tasking,front_directed));hashes[tb.name]=sha(tb)
 state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),actions=[],engine=engine,profile=profile,native_integrated=False,late_tasking=late_tasking,mutation=mutation,directed_RBase_StkP=front_directed)
 def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
 def execute(cmd,log):
  with log.open('w')as f:
   process=subprocess.Popen(list(map(str,cmd)),cwd=inputs,stdout=f,stderr=subprocess.STDOUT)
   row=dict(command=list(map(str,cmd)),pid=process.pid,started=time.time(),log=str(log));state['actions'].append(row);save()
   rc=process.wait();row.update(returncode=rc,finished=time.time());save()
  return rc
 save();ref=out/'reference';manifest=generate(inputs,ref)
 adapters=out/'adapters';adapted=adapters_generate(inputs,adapters)
 cone=out/'dorado_r_front_cone.sv';cone.write_bytes((base/cone.name).read_bytes());assert sha(cone)==parent['inherited_cone_sha256']
 inherited_cone=sha(cone);cone_change=None
 if cone_rewrite is not None:
  (out/'parent-cone.sv.txt').write_bytes(cone.read_bytes());cone_change=cone_rewrite(cone)
  assert sha(cone)==cone_change['output_sha256']
 gates,ffs,phases,parc,read_ps=PROFILES[profile];timed=out/'timed';stage(inputs,ref,timed,gates,ffs,phases,parc)
 for p in (base/'ram-reference').glob('*.sv'):(timed/p.name).write_bytes(p.read_bytes())
 for n in ['r_file_original_storage.sv','r_front_original_storage.sv']:
  raw=(inputs/'verilog/verilator'/n).read_text().replace('READ_PS=1000',f'READ_PS={read_ps}').replace('DELAY_PS=1000',f'DELAY_PS={ffs}')
  (timed/n).write_text(raw)
 (timed/'ready_ff.sv').write_text(ORACLE.replace('module original_F10000','module original_F10000_ready').replace('DELAY_PS=1000',f'DELAY_PS={ffs}'))
 candidate=out/'candidate.sv';raw=(inputs/CANDIDATE).read_text()
 if mutation:
  if mutation in ['next_macro','jcnt']:
   crossing={'next_macro':'r[212]=!p[313]','jcnt':'r[235]=!p[311]'}[mutation]
   assert raw.count(crossing)==1;raw=raw.replace(crossing,crossing.split('=')[0]+"=1'b0")
  else:
   parameter={'stop_data':'STOP_CAPTURE_OLD','stop_clock':'STOP_CLOCK_NEW','branch_data':'BRANCH_CAPTURE_OLD'}[mutation]
   assert raw.count(parameter+'=1')==1;raw=raw.replace(parameter+'=1',parameter+'=0')
 candidate.write_text(raw)
 support=[candidate,tb,cone,*[inputs/n for n in sorted(hashes)if n.startswith('verilog/rtl/')and n!=CANDIDATE],
          *sorted(adapters.glob('*.sv')),*sorted(timed.glob('*.sv')),*sorted(timed.glob('*.v'))]
 defines=['-DPARC_PARITY_TIMING']if parc else[]
 command=['iverilog','-g2012','-s','tb_r_front_local',*defines,'-o',out/'test.vvp',*support]if engine=='icarus'else[
  'verilator','--binary','--timing','-j','2','--top-module','tb_r_front_local','-Wno-fatal',*defines,'--Mdir',out/'obj','-o','test',*support]
 rc=execute(command,out/'compile.log')
 if rc==0:rc=execute(['vvp','-i',out/'test.vvp']if engine=='icarus'else[out/'obj/test'],out/'test.log')
 for n,h in hashes.items():assert sha(inputs/n)==h,n
 log=(out/'test.log').read_text()if(out/'test.log').exists()else''
 summary=re.search(r'^R_FRONT_LOCAL_PROBE PASS ([^\n]+)$',log,re.M);passed=rc==0 and summary is not None
 state.update(status='PASS_R_FRONT_LOCAL_PROBE'if passed else'R_FRONT_LOCAL_PROBE_FAILED',finished=time.time(),
  summary=summary[1]if summary else None,inputs=hashes,reference=manifest,adapters=adapted,
  timed_sources={p.name:sha(p)for p in timed.iterdir()if p.suffix in ['.v','.sv']},
  full_arrival_contract_qualified=False,physical_timing_qualified=False,executed_candidate_sha256=sha(candidate))
 state.update(random_external_producers=random,CP_bus_sweep=cp,LOW_root_CP=low_cp,
  inherited_cone_sha256=inherited_cone,executed_cone_sha256=sha(cone),cone_rewrite=cone_change,
  delay_profile=dict(gates_ps=gates,ffs_ps=ffs,phases_ps=phases,
   parc_parity_control=parc,R_read_ps=read_ps))
 save();print(state['status'],out,flush=True);return 0 if passed else(rc or 1)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--base',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
 p.add_argument('--iterations',type=int,default=64);p.add_argument('--engine',choices=['icarus','verilator'],default='icarus')
 p.add_argument('--profile',choices=PROFILES,default='balanced');p.add_argument('--producer-random',action='store_true')
 p.add_argument('--cp',action='store_true');p.add_argument('--low-cp',action='store_true')
 p.add_argument('--late-tasking',action='store_true');p.add_argument('--front-directed',action='store_true');p.add_argument('--mutation',choices=['stop_data','stop_clock','branch_data','next_macro','jcnt']);a=p.parse_args()
 sys.exit(run(a.base.resolve(),a.output.resolve(),a.iterations,a.engine,a.profile,a.producer_random,a.cp,a.low_cp,a.late_tasking,a.mutation,a.front_directed))
