#!/usr/bin/env python3
"""Compare task arbitration and processor captures with actual Ready producers.

One original circuit, all104 Qs/27 clock channels, both simulators and three
illustrative delay profiles. Ready/IsNext/phase/task feedback is internal.
CP/MIR/notify/preemption and Hold arrivals remain boundaries. No native or
whole-board60ns timing proof is claimed.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

from generate_task_ready_capture_reference import specs,generate
from sil_netlist import load_board
from stage_task_capture_timing import stage

ROOT=Path(__file__).resolve().parents[1]
CANDIDATE='verilog/rtl/dorado_task_ready_capture_step.sv'
PROFILES=[('balanced',1000,1000,1000),('fast_gates',500,1000,500),('later_control',1000,2000,1000)]
MUTATIONS={
 'ignore_ready':('core_after[49:35]=pre_ready',"core_after[49:35]=0"),
 'ignore_mask':('core_after[64:50]=pre_mask',"core_after[64:50]=15'h7fff"),
 'late_ready_clock':('28\'(clock_events[20])','28\'(clock_events[19])'),
 'use_new_task_data':('edge_data=capture_data |','edge_data=ready_pins(state_next[87:0],pins_after) |'),
 'external_ready_stub':('core_after[49:35]=pre_ready','core_after[49:35]=pins_after[49:35]'),
}


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def run(out,mutations):
 out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs'
 files=[CANDIDATE,'verilog/rtl/dorado_task_capture_step.sv','verilog/rtl/dorado_task_ready_step.sv',
        'verilog/rtl/dorado_processor_capture_step.sv','verilog/verilator/tb_task_capture_step.sv',
        'verilog/verilator/run_control_original_ff.sv','verilog/verilator/processor_capture_original_ff.sv',
        'tools/generate_task_ready_capture_reference.py','tools/generate_task_capture_reference.py',
        'tools/generate_processor_capture_reference.py','tools/generate_control_clock_reference.py',
        'tools/generate_run_control_reference.py','tools/sil_netlist.py','tools/stage_task_capture_timing.py',
        'tools/test_task_ready_capture.py','tools/test_task_ready.py']
 kinds={'MC10210'}
 for s in specs():
  board=load_board(str(ROOT/'chm/sil'/(s['bn']+'.dm!1_')/(s['bn']+'.wl')))
  kinds.update(board.packages[p]['type']for p in set(s['comb'])|set(s['seq']))
 for bn in ['BaseBd-Rev-Am',*[s['bn']for s in specs()]]:
  prefix='chm/sil/'+bn+'.dm!1_/'+bn;files += [prefix+'.wl',prefix+'.lc']
 files += ['verilog/cells/cell_'+k+'.v'for k in sorted(kinds)]
 hashes={}
 for n in sorted(set(files)):
  p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=sha(p)
 # Freeze the actual independent Ready oracle from the already qualified tool.
 from test_task_ready import ORACLE
 p=inputs/'original_ready_ff.sv';p.write_text(ORACLE);hashes[p.name]=sha(p)
 ref=out/'reference';record=generate(inputs,ref)
 bench=(inputs/'verilog/verilator/tb_task_capture_step.sv').read_text()
 bench=bench.replace('tb_task_capture_step','tb_task_ready_capture').replace('task_capture_reference','task_ready_capture_reference')
 bench=bench.replace('dorado_task_capture_step','dorado_task_ready_capture_step')
 bench=bench.replace('[75:0]','[85:0]').replace('[87:0]','[103:0]').replace('[24:0]','[26:0]')
 bench=bench.replace("SEED_PINS=76'h128", "SEED_PINS=(86'h3<<80)|76'h128")
 bench=bench.replace('i<25','i<27').replace('counts[0:24],totals[0:24]','counts[0:26],totals[0:26]').replace('previous_counts[0:24]','previous_counts[0:26]')
 bench=bench.replace('wire unsettled;','wire unsettled;wire [29:0] ready_flags,reference_ready_flags;')
 bench=bench.replace('state_next,events,repeated,unsettled);','state_next,events,repeated,unsettled,ready_flags);')
 bench=bench.replace('reference_flags,reference_next);','reference_flags,reference_next,reference_ready_flags);')
 bench=bench.replace("if(reference_next!==raw_next(expected))",'if(reference_ready_flags!==ready_flags) $fatal(1,"coupled Ready/IsNext flag mismatch ref=%h candidate=%h old=%h next=%h roots=%b->%b pins=%h",reference_ready_flags,ready_flags,state_q,expected,before_clocks,next_clocks,next_pins);\n        if(reference_next!==raw_next(expected))')
 bench=bench.replace('{next_pins[69],next_pins[65]}!={pins_before[69],pins_before[65]}','{next_pins[82],next_pins[69],next_pins[65]}!={pins_before[82],pins_before[69],pins_before[65]}')
 bench=bench.replace('setup_pins[69]=pins_before[69];','setup_pins[82]=pins_before[82];setup_pins[69]=pins_before[69];')
 bench=bench.replace('random_state[87:64]=rng[23:0];seed(random_state);','random_state[87:64]=rng[23:0];random_state[103:88]=rng[15:0];seed(random_state);')
 bench=bench.replace('if(selected[70]) selected[74:71]=0;','selected[85:76]=rng[9:0];\n                if(selected[70]) selected[74:71]=0;')
 bench=bench.replace('integer current_task;','integer current_task,ready_wakeups=0,bnt_pairs=0;')
 directed=r'''
        // Both BNT packages can contain unrelated power-up data. Exercise
        // all 256 pairs before any arbitration edge overwrites them.
        for(integer positive=0;positive<16;positive=positive+1)
         for(integer negative=0;negative<16;negative=negative+1)begin
          random_state=7;random_state[72]=1;random_state[73]=1;
          random_state[71:68]=4'(positive);random_state[67:64]=4'(negative);
          random_state[103:88]=16'hd1d1;seed(random_state);
          pin_event(0,SEED_PINS);bnt_pairs=bnt_pairs+1;
         end
        // Wake every task through the actual Notify->Ready->priority path,
        // with all direct wake wires zero. No fixture supplies Ready/IsNext.
        for(integer task_no=1;task_no<16;task_no=task_no+1)begin
          seed(7);selected=SEED_PINS;selected[19:0]=20'h178;
          selected[34:20]=0;selected[81]=0;selected[80]=1;
          selected[79:76]=task_number(4'(task_no));
          for(integer n=0;n<96;n=n+1)transfer(n[0],selected);
          if(task_number(raw_ct(state_q))!=task_no)
           $fatal(1,"Ready-only wake failed task=%0d actual=%0d",task_no,task_number(raw_ct(state_q)));
          ready_wakeups=ready_wakeups+1;
        end
'''
 bench=bench.replace('for(integer sample=0;sample<128;sample=sample+1) begin',directed+'\n        for(integer sample=0;sample<128;sample=sample+1) begin')
 bench=bench.replace('TASK_CAPTURE_STEP PASS','TASK_READY_CAPTURE PASS')
 bench=bench.replace('        $display("TASK_READY_CAPTURE PASS', '        if(ready_wakeups!=15||bnt_pairs!=256) $fatal(1,"Ready coupling coverage missing");\n        $display("TASK_READY_CAPTURE_COVERAGE ready_wakeups=%0d bnt_pairs=%0d",ready_wakeups,bnt_pairs);\n        $display("TASK_READY_CAPTURE PASS')
 p=inputs/'tb_task_ready_capture.sv';p.write_text(bench);hashes[p.name]=sha(p)
 actions=[]
 def execute(cmd,log,required=True):
  with log.open('w')as stream:code=subprocess.run(list(map(str,cmd)),stdout=stream,stderr=subprocess.STDOUT,cwd=inputs).returncode
  actions.append(dict(command=list(map(str,cmd)),returncode=code,log=str(log)))
  if required:assert code==0,log
  return code
 profiles=[];baseline=None
 for name,gates,ffs,phases in PROFILES:
  d=out/name;d.mkdir();timed=d/'timed-reference';stage(inputs,ref,timed,gates,ffs,phases)
  raw=(inputs/'original_ready_ff.sv').read_text().replace('DELAY_PS=1000',f'DELAY_PS={ffs}')
  (timed/'original_ready_ff.sv').write_text(raw)
  support=[inputs/'tb_task_ready_capture.sv',*sorted(timed.glob('*.sv')),*sorted(timed.glob('*.v')),
           inputs/'verilog/rtl/dorado_task_capture_step.sv',inputs/'verilog/rtl/dorado_task_ready_step.sv']
  source=inputs/CANDIDATE
  execute(['iverilog','-g2012','-s','tb_task_ready_capture','-o',d/'test.vvp',source,*support],d/'compile.log')
  execute(['vvp',d/'test.vvp'],d/'test.log')
  summary=re.search(r'^TASK_READY_CAPTURE PASS ([^\n]+)$',(d/'test.log').read_text(),re.M)[1]
  if baseline is None:baseline=summary
  assert summary==baseline
  print('TASK_READY_CAPTURE',name,'Icarus',summary,flush=True)
  execute(['verilator','--binary','--timing','--top-module','tb_task_ready_capture','-Wno-fatal','--Mdir',d/'obj','-o','test',source,*support],d/'verilator-compile.log')
  execute([d/'obj/test'],d/'verilator-test.log')
  assert re.search(r'^TASK_READY_CAPTURE PASS ([^\n]+)$',(d/'verilator-test.log').read_text(),re.M)[1]==summary
  print('TASK_READY_CAPTURE',name,'Verilator agrees',flush=True)
  profiles.append(dict(profile=name,summary=summary,timed_source_hashes={p.name:sha(p)for p in timed.iterdir()if p.is_file()}))
  if mutations and name=='balanced':
   for label,(a,b)in MUTATIONS.items():
    md=d/label;md.mkdir();raw=source.read_text();assert raw.count(a)==1,(label,a)
    p=md/'candidate.sv';p.write_text(raw.replace(a,b))
    execute(['iverilog','-g2012','-s','tb_task_ready_capture','-o',md/'test.vvp',p,*support],md/'compile.log')
    assert execute(['vvp',md/'test.vvp'],md/'test.log',False)!=0,label
 for n,h in hashes.items():assert sha(inputs/n)==h,n
 result=dict(status='PASS_TASK_READY_CAPTURE',scope=__doc__,inputs=hashes,reference=record,
             profiles=profiles,summary=baseline,actions=actions,rejected_mutations=list(MUTATIONS)if mutations else[],
             native_integrated=False,CP_MIR_Hold_producers_closed=False,Ready_and_IsNext_internal=True,Ready_only_wakeups=15,independent_BNT_pairs=256)
 (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
 p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
