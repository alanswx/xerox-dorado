#!/usr/bin/env python3
"""Couple original CP maintenance storage and actual Ready asynchronous clear.

Reuse frozen passing CP/Ready inputs and workload. The independent reference is
one extracted original wiring graph: ClrReady is now internal, not a fixture
input. Phase/task/MIR sources and their arrivals remain explicit. This does
not qualify the full task arbiter, native integration or whole-board 60ns timing.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess

from generate_processor_capture_reference import graph

ROOT=Path(__file__).resolve().parents[1]
CANDIDATE='verilog/rtl/dorado_task_producers_step.sv'
MUTATIONS={
 'external_clear_stub':('cp_next[9],pins_after[14:0]',"1'b0,pins_after[14:0]"),
 'old_clear_epoch':('cp_next[9],pins_after[14:0]','state_q[25],pins_after[14:0]'),
 'wrong_cp_bit':('cp_next[9],pins_after[14:0]','cp_next[8],pins_after[14:0]'),
}
CP_TRANSACTIONS=r'''
  for(integer bank=0;bank<2;bank=bank+1)
   for(integer data=0;data<128;data=data+1)begin
    selected=pins;selected[34]=1;selected[33:31]=~3'(bank);selected[30:24]=7'(data);event_core(selected);
    selected[34]=0;event_core(selected);selected[34]=1;event_core(selected);
   end
'''
WRAPPER=r'''
 task event_pins(input [34:0] value);
  reg [10:0] bus;
  begin
   if(value[15]!=state[25])begin
    bus={1'b1,3'b110,7'b0};bus[6]=value[15];
    event_core({bus,pins[23:0]});
    bus[10]=0;event_core({bus,pins[23:0]});
    bus[10]=1;event_core({bus,pins[23:0]});
   end
   event_core({pins[34:24],value[23:0]});
  end
 endtask
'''


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def run(ready,cp,out,mutations):
 records=[json.loads((p/'results.json').read_text())for p in [ready,cp]]
 assert [d['status']for d in records]==['PASS_TASK_READY_PRODUCERS','PASS_CP_CONTROL_PRODUCERS']
 out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';hashes={}
 for source,record in zip([ready,cp],records):
  for n,h in record['inputs'].items():
   p=source/'executed-inputs'/n;assert sha(p)==h,n
   if n in hashes:assert hashes[n]==h,n
   target=inputs/n;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes());hashes[n]=h
 for n in [CANDIDATE,'tools/test_task_producers.py']:
  target=inputs/n;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes((ROOT/n).read_bytes());hashes[n]=sha(target)
 rr,cr=[d['reference']for d in records]
 bindings={k:v for k,v in rr['boundary_bindings'].items()if k!='ClrReady'}
 for n,v in cr['boundary_bindings'].items():bindings[n]=re.sub(r'pins\[(\d+)\]',lambda m:f'pins[{int(m[1])+24}]',v)
 comb=[p for p in rr['packages']if p not in rr['sequential']]+['a01']
 seq={**rr['sequential'],'j02':(16,6),'i02':(22,6)}
 ref=out/'reference';ref.mkdir()
 record=graph(inputs,'ContA-Rev-Cd',comb,seq,'task_producers_reference',bindings,
              [*rr['clock_taps'],*cr['clock_taps']],rr['flag_taps'],ref)
 p=ref/'task_producers_reference.sv';raw=p.read_text();assert raw.count('input wire [27:0] pins')==1
 p.write_text(raw.replace('input wire [27:0] pins','input wire [34:0] pins'))
 assert len(record['packages'])==18 and record['physical_bits']==28
 (ref/'manifest.json').write_text(json.dumps(record,indent=2)+'\n')
 bench=(ready/'executed-inputs/tb_task_ready.sv').read_text()
 bench=bench.replace('tb_task_ready','tb_task_producers').replace('task_ready_reference','task_producers_reference')
 bench=bench.replace('dorado_task_ready_step','dorado_task_producers_step')
 bench=bench.replace('[15:0] fixture','[27:0] fixture').replace('[15:0] original_q','[27:0] original_q').replace('[15:0] old','[27:0] old').replace('[15:0] changed','[27:0] changed')
 bench=bench.replace('[23:0] pins','[34:0] pins').replace('[23:0] value','[34:0] value').replace('[23:0] selected','[34:0] selected')
 bench=bench.replace("22'h047000","{11'h780,24'h047000}").replace("{4'b0,pins}",'pins')
 bench=bench.replace('[1:0] clocks','[3:0] clocks').replace('[1:0] observed','[3:0] observed')
 bench=bench.replace('integer checks=0,','integer cp0=0,cp1=0;\n integer checks=0,')
 bench=bench.replace(' always @(posedge clocks[1]) observed[1]=1;',' always @(posedge clocks[1]) observed[1]=1;\n always @(posedge clocks[2]) observed[2]=1;\n always @(posedge clocks[3]) observed[3]=1;')
 bench=bench.replace('task event_pins(input','task event_core(input',1)
 bench=bench.replace('clears=clears+pins[15];','clears=clears+next_q[25];cp0=cp0+events[2];cp1=cp1+events[3];')
 bench=bench.replace(' reg [34:0] selected;',WRAPPER+'\n reg [34:0] selected;')
 bench=bench.replace("fixture=16'hffff","fixture=28'hdffffff")
 bench=bench.replace("if(changed!=16'hffff",CP_TRANSACTIONS+"\n  if(cp0<128 || cp1<128 || changed!=28'hfffffff")
 bench=bench.replace('TASK_READY PASS','TASK_PRODUCERS PASS').replace('checks=%0d captures=%0d','checks=%0d cp0=%0d cp1=%0d captures=%0d').replace('checks,captures,clears,held,','checks,cp0,cp1,captures,clears,held,')
 p=inputs/'tb_task_producers.sv';p.write_text(bench);hashes['tb_task_producers.sv']=sha(p)
 actions=[]
 def execute(cmd,log,required=True):
  with log.open('w')as stream:code=subprocess.run(list(map(str,cmd)),stdout=stream,stderr=subprocess.STDOUT,cwd=inputs).returncode
  actions.append(dict(command=list(map(str,cmd)),returncode=code,log=str(log)))
  if required:assert code==0,log
  return code
 profiles=[];baseline=None
 for row in records[0]['profiles']:
  name=row['profile'];d=out/name;d.mkdir();timed=d/'reference'
  shutil.copytree(ready/name/'reference',timed)
  raw=(cp/name/'reference/run_control_original_ff.sv').read_bytes();(timed/'run_control_original_ff.sv').write_bytes(raw)
  support=[ref/'task_producers_reference.sv',*timed.glob('*.v'),*timed.glob('*.sv'),inputs/'tb_task_producers.sv',
           inputs/'verilog/rtl/dorado_cp_control_step.sv',inputs/'verilog/rtl/dorado_task_ready_step.sv']
  source=inputs/CANDIDATE
  execute(['iverilog','-g2012','-s','tb_task_producers','-o',d/'test.vvp',source,*support],d/'compile.log')
  execute(['vvp',d/'test.vvp'],d/'test.log')
  summary=re.search(r'^TASK_PRODUCERS PASS ([^\n]+)$',(d/'test.log').read_text(),re.M)[1]
  if baseline is None:baseline=summary
  assert summary==baseline
  print('TASK_PRODUCERS',name,'Icarus',summary,flush=True)
  execute(['verilator','--binary','--timing','--top-module','tb_task_producers','-Wno-fatal','--Mdir',d/'obj','-o','test',source,*support],d/'verilator-compile.log')
  execute([d/'obj/test'],d/'verilator-test.log')
  assert re.search(r'^TASK_PRODUCERS PASS ([^\n]+)$',(d/'verilator-test.log').read_text(),re.M)[1]==summary
  profiles.append(dict(profile=name,summary=summary,timed_source_hashes={p.name:sha(p)for p in timed.iterdir()}))
  print('TASK_PRODUCERS',name,'Verilator agrees',flush=True)
  if mutations and name=='balanced':
   for label,(a,b)in MUTATIONS.items():
    md=d/label;md.mkdir();raw=source.read_text();assert raw.count(a)==1,(label,a)
    p=md/'candidate.sv';p.write_text(raw.replace(a,b))
    execute(['iverilog','-g2012','-s','tb_task_producers','-o',md/'test.vvp',p,*support],md/'compile.log')
    assert execute(['vvp',md/'test.vvp'],md/'test.log',False)!=0,label
 for n,h in hashes.items():assert sha(inputs/n)==h,n
 result=dict(status='PASS_COUPLED_TASK_PRODUCERS',scope=__doc__,inputs=hashes,reference=record,
             profiles=profiles,summary=baseline,actions=actions,rejected_mutations=list(MUTATIONS)if mutations else[],
             native_integrated=False,task_arbiter_coupled=False,ClrReady_internally_connected=True)
 (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--ready',type=Path,required=True)
 p.add_argument('--cp',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
 p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.ready.resolve(),a.cp.resolve(),a.output.resolve(),a.mutations)
