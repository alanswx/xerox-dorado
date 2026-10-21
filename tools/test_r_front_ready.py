#!/usr/bin/env python3
"""Qualify original Ready coupled to full RBase/stack/RM/STK storage.

All752 processor and9216 RAM bits after every event; all59 clocks; both
simulators/six profiles. Existing nonoverlap, old-parity source epochs and
write-window monitors remain. This is not native integration or60ns proof.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

from generate_r_front_ready_reference import generate
from test_r_front import MUTATIONS as FRONT_MUTATIONS,PROFILES
from stage_r_operand_timing import stage

ROOT=Path(__file__).resolve().parents[1]
CANDIDATE='verilog/rtl/dorado_r_front_ready_step.sv'
WRAPPER_MUTATIONS={
 'ignore_ready': [('child_after[49:35]=pre_ready','child_after[49:35]=0')],
 'lose_ready_capture':[("28'(clock_events[20])","28'(1'b0)")],
}


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(base,out,mutations,smoke):
 parent=json.loads((base/'results.json').read_text());assert parent['status']=='PASS_R_FRONT_ARRIVAL_CONTRACT'
 out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';hashes={}
 for n,h in parent['inputs'].items():
  assert sha(base/'executed-inputs'/n)==h and sha(ROOT/n)==h,n
  p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=h
 extra=[CANDIDATE,'verilog/rtl/dorado_task_ready_step.sv','verilog/rtl/dorado_task_ready_capture_step.sv',
        'verilog/cells/cell_MC10162.v','tools/generate_task_ready_capture_reference.py','tools/test_task_ready.py',
        'tools/generate_r_front_ready_reference.py','tools/test_r_front_ready.py']
 for n in extra:
  p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=sha(p)
 # Keep both instances of the shared pin mapper identical by inspection.
 def function(text,name):
  found=[m for m in re.findall(r' function automatic .*? endfunction',text,re.S)if re.search(r'\b'+name+r'\(',m.split('\n')[0])]
  assert len(found)==1,name
  return found[0]
 for fn in ['current_task','ready_pins']:
  assert function((inputs/CANDIDATE).read_text(),fn)==function((inputs/'verilog/rtl/dorado_task_ready_capture_step.sv').read_text(),fn)
 ref=out/'reference';manifest=generate(inputs,ref)
 for name in ['dorado_r_front_cone.sv','dorado_r_front_cone.json']:(out/name).write_bytes((base/name).read_bytes())
 cone=out/'dorado_r_front_cone.sv';assert sha(cone)==json.loads((out/'dorado_r_front_cone.json').read_text())['output_sha256']
 ram=out/'ram-reference';ram.mkdir()
 for p in (base/'ram-reference').glob('*'):
  if p.is_file():(ram/p.name).write_bytes(p.read_bytes())
 bench=(inputs/'verilog/verilator/tb_r_front_step.sv').read_text()
 bench=bench.replace('tb_r_front_step','tb_r_front_ready').replace('dorado_r_front_step','dorado_r_front_ready_step').replace('r_front_reference original','r_front_ready_reference original')
 bench=bench.replace('[235:0]','[245:0]').replace('[735:0]','[751:0]').replace('[56:0]','[58:0]')
 bench=bench.replace('{rp[235:145],','{rp[245:145],')
 bench=bench.replace('0:56','0:58').replace('i<57','i<59').replace('ch<57','ch<59').replace('i<736','i<752')
 bench=bench.replace("SEED=(236'h1", "SEED=(246'h3<<240)|(236'h1")
 bench=bench.replace('wire unsettled;','wire unsettled;wire [29:0] ready_flags,ref_ready_flags;')
 bench=bench.replace('raw_parity,source_pins,fa_n);','raw_parity,source_pins,fa_n,ready_flags);')
 bench=bench.replace('rforwarded,ref_source_pins,ref_fa_n,);','rforwarded,ref_source_pins,ref_fa_n,,ref_ready_flags);')
 bench=bench.replace('dut.child.files','dut.child.child.files')
 bench=bench.replace('if(raddress!==ea||', 'if(ready_flags!==ref_ready_flags) $fatal(1,"R front Ready flag mismatch");\n     if(raddress!==ea||')
 bench=bench.replace('word_no*32+bit_no<236','word_no*32+bit_no<246')
 directed=r'''
   // Notify-only wakeups traverse Ready->priority with no direct wake wires.
   for(integer task_no=1;task_no<16;task_no=task_no+1) begin
     if(!cb)event_step(1,p);
     p=SEED;p[8:0]=9'h178;p[34:20]=0;p[240]=1;p[241]=0;
     p[239:236]={task_no[0],task_no[1],task_no[2],task_no[3]};
     event_step(1,p);
     for(integer edge_no=0;edge_no<128;edge_no=edge_no+1)event_step(!cb,p);
     if({q[77],q[76],q[79],q[78]}!=4'(task_no))
       $fatal(1,"R front Notify-only task wakeup failed task=%0d actual=%0d",task_no,{q[77],q[76],q[79],q[78]});
     ready_wakeups=ready_wakeups+1;
   end
'''
 bench=bench.replace('integer iterations=2048;','integer iterations=2048,ready_wakeups=0;')
 bench=bench.replace('integer writes=0,reads=0,capture_epoch_changes=0;',"reg [15:0] ready_changed=0;integer writes=0,reads=0,capture_epoch_changes=0;")
 bench=bench.replace('front_changed=front_changed|', 'ready_changed=ready_changed|(q[751:736]^expected[751:736]);front_changed=front_changed|')
 bench=bench.replace('fixture[83:80]=15;',"fixture[751:736]=16'hffff;fixture[83:80]=15;")
 bench=bench.replace('   for(integer i=0;i<64;i=i+1) event_step(!cb,p);', '   p[242]=1;event_step(1,p);p[242]=0;event_step(1,p);\n   for(integer i=0;i<64;i=i+1) event_step(!cb,p);')
 bench=bench.replace('   // Drive original maintenance inputs',directed+'\n   // Drive original maintenance inputs',1)
 bench=bench.replace('   $display("R_FRONT_COVERAGE', '   if(ready_wakeups!=15||ready_changed!=65535)$fatal(1,"R front Ready wakeup coverage incomplete");\n   $display("R_FRONT_READY_COVERAGE ready_wakeups=%0d ready_changed=%h",ready_wakeups,ready_changed);\n   $display("R_FRONT_COVERAGE')
 bench=bench.replace('R_FRONT_STEP PASS','R_FRONT_READY_STEP PASS').replace('R_FRONT_SMOKE PASS','R_FRONT_READY_SMOKE PASS')
 p=inputs/'tb_r_front_ready.sv';p.write_text(bench);hashes[p.name]=sha(p)
 actions=[]
 def execute(cmd,log,required=True):
  with log.open('w')as stream:code=subprocess.run(list(map(str,cmd)),stdout=stream,stderr=subprocess.STDOUT,cwd=inputs).returncode
  actions.append(dict(command=list(map(str,cmd)),returncode=code,log=str(log)))
  if required:assert code==0,log
  return code
 from test_task_ready import ORACLE
 profiles=[(*p,1000)for p in PROFILES]+[('later_r_memory',1000,1000,1000,None,9900)]
 if smoke:profiles=profiles[:1]
 records=[];baseline=None
 for name,gates,ffs,phases,parc,read_ps in profiles:
  d=out/name;d.mkdir();timed=d/'timed-reference';stage(inputs,ref,timed,gates,ffs,phases,parc)
  for p in ram.glob('*.sv'):(timed/p.name).write_bytes(p.read_bytes())
  raw=(inputs/'verilog/verilator/r_file_original_storage.sv').read_text().replace('READ_PS=1000',f'READ_PS={read_ps}')
  (timed/'r_file_original_storage.sv').write_text(raw)
  raw=(inputs/'verilog/verilator/r_front_original_storage.sv').read_text().replace('DELAY_PS=1000',f'DELAY_PS={ffs}')
  (timed/'r_front_original_storage.sv').write_text(raw)
  raw=ORACLE.replace('module original_F10000','module original_F10000_ready').replace('DELAY_PS=1000',f'DELAY_PS={ffs}')
  (timed/'original_ready_ff.sv').write_text(raw)
  rtl=[inputs/n for n in hashes if n.startswith('verilog/rtl/')and n not in [CANDIDATE,'verilog/rtl/dorado_r_front_step.sv','verilog/rtl/dorado_task_ready_capture_step.sv']]
  support=[cone,inputs/'tb_r_front_ready.sv',*rtl,*sorted(timed.glob('*.sv')),*sorted(timed.glob('*.v'))]
  defines=['-DPARC_PARITY_TIMING']if parc else[]
  variants=[('original',[],[])]
  if mutations and name=='balanced':variants += [(k,v,[])for k,v in WRAPPER_MUTATIONS.items()]+[(k,[],v)for k,v in FRONT_MUTATIONS.items()]
  rows=[]
  for label,edits,child_edits in variants:
   case=d/label;case.mkdir();raw=(inputs/CANDIDATE).read_text();child=(inputs/'verilog/rtl/dorado_r_front_step.sv').read_text()
   for a,b in edits:assert raw.count(a)==1;raw=raw.replace(a,b)
   for a,b in child_edits:assert child.count(a)==1;child=child.replace(a,b)
   p=case/'dorado_r_front_ready_step.sv';p.write_text(raw);c=case/'dorado_r_front_step.sv';c.write_text(child)
   execute(['iverilog','-g2012','-s','tb_r_front_ready',*defines,'-o',case/'test.vvp',p,c,*support],case/'compile.log')
   code=execute(['vvp',case/'test.vvp',*(['+iterations=16']if smoke else[])],case/'test.log',False)
   text=(case/'test.log').read_text()
   if label=='original':
    assert code==0,case/'test.log';marker='R_FRONT_READY_SMOKE PASS'if smoke else'R_FRONT_READY_STEP PASS'
    summary=re.search('^'+marker+r' ([^\n]+)$',text,re.M)[1]
    if baseline is None:baseline=summary
    assert summary==baseline
    print('R_FRONT_READY',name,'Icarus',summary,flush=True)
   else:assert code!=0 and 'FATAL:'in text,(label,text)
   rows.append(dict(case=label,returncode=code,expected_failure=label!='original'))
  if not smoke:
   v=d/'verilator';v.mkdir()
   execute(['verilator','--binary','--timing','--top-module','tb_r_front_ready',*defines,'-Wno-fatal','-Wno-PINMISSING','-Wno-TIMESCALEMOD','-Wno-MULTIDRIVEN','--Mdir',v/'obj','-o','test',inputs/CANDIDATE,inputs/'verilog/rtl/dorado_r_front_step.sv',*support],v/'compile.log')
   execute([v/'obj/test'],v/'test.log')
   text=(v/'test.log').read_text();assert re.search(r'^R_FRONT_READY_STEP PASS ([^\n]+)$',text,re.M)[1]==summary
   itext=(d/'original/test.log').read_text()
   for marker,expected in [('R_FRONT_READY_COVERAGE',1),('R_FRONT_COVERAGE',1),('R_FRONT_STORAGE',1),('R_FRONT_TIMING',1),('R_FRONT_CLOCK',59),('R_FRONT_WRITE_WINDOW',2)]:
    x=re.findall('^'+marker+r' ([^\n]+)$',itext,re.M);assert len(x)==expected and x==re.findall('^'+marker+r' ([^\n]+)$',text,re.M),(name,marker)
   print('R_FRONT_READY',name,'Verilator agrees',flush=True)
  records.append(dict(profile=name,cases=rows,timed_source_hashes={p.name:sha(p)for p in timed.iterdir()if p.suffix in ['.sv','.v']}))
 for n,h in hashes.items():assert sha(inputs/n)==h,n
 result=dict(status='PASS_R_FRONT_READY_SMOKE'if smoke else'PASS_R_FRONT_READY_ARRIVAL_CONTRACT',scope=__doc__,inputs=hashes,
             reference=manifest,physical_bits=752,ram_bits=9216,total_physical_bits=9968,profiles=records,actions=actions,summary=baseline,
             inherited_cone_sha256=sha(cone),Ready_and_IsNext_internal=True,Ready_only_wakeups=15,
             all_R_memory_bits_compared=True,r_read_write_nonoverlap_checked=True,post_commit_source_epoch_checked=True,
             R_final_address_stage_qualified=True,ram_arrays_flattened=False,native_integrated=False,
             producer_timing_qualified=False,independent_gate_functions_qualified=False)
 (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--base',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
 p.add_argument('--mutations',action='store_true');p.add_argument('--smoke',action='store_true');a=p.parse_args();run(a.base.resolve(),a.output.resolve(),a.mutations,a.smoke)
