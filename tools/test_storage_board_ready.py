#!/usr/bin/env python3
"""Compare board readiness reduction against the original144-chip MSA.

Check all stored bits and exact raw requests including host timestamps.
Bad board reductions must compile and fail at runtime, not just fail to build.
"""
import argparse,hashlib,json,os,subprocess,time
from pathlib import Path

def run(root):
 inputs=root/'inputs';p=json.loads((root/'provenance.json').read_text())
 for n,h in p['inputs'].items():assert hashlib.sha256((inputs/n).read_bytes()).hexdigest()==h,n
 assert not(root/'run-state.json').exists(),'Observe existing run'
 (root/'runner-executed.py').write_bytes(Path(__file__).read_bytes())
 s=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),actions=[],native_qualified=False,hardware_qualified=False)
 def save():(root/'run-state.json').write_text(json.dumps(s,indent=2)+'\n')
 def execute(cmd,label,negative=False):
  with(root/(label+'.log')).open('x')as log:
   proc=subprocess.Popen(cmd,cwd=inputs,stdout=log,stderr=subprocess.STDOUT)
   a=dict(stage=label,pid=proc.pid,command=cmd,started=time.time());s['actions'].append(a);save();rc=proc.wait();a.update(returncode=rc,finished=time.time());save()
  t=(root/(label+'.log')).read_text()
  assert (rc!=0 and any(d in t for d in ['STORAGE_BOARD_READY_REDUCTION','STORAGE_READY_PREDICTION','Dorado clock advanced while storage group was stalled']))if negative else rc==0,(label,rc,t[-3000:])
 def case(label,board,negative=False):
  obj=root/('obj-'+label)
  files=[board]+[inputs/n for n in ['dorado_storage_group.sv','msa.v','dorado_storage_arb.sv','dorado_storage_sdram.sv','dorado_storage_sdram_burst.sv','xsds_model.sv','tb.sv','burst-fixture.sv','addr_preview.sv']]
  cmd=['verilator','--binary','--timing','--autoflush','-j','2','--top-module','tb_storage_board','-GADDR_BITS=16','-GPREFETCH_NEXT=1','-GINTERLEAVE_GROUPS=1','-DSTORAGE_USE_SDRAM','-Wno-fatal','-Wno-TIMESCALEMOD','-Wno-WIDTHTRUNC','-Wno-WIDTHEXPAND','-Wno-REALCVT','-I'+str(inputs/'cells'),'--Mdir',str(obj),'-o','board-test',*map(str,files)]
  execute(cmd,label+'-compile');execute([str(obj/'board-test'),'+raw_trace='+str(root/(label+'-raw.csv'))],label+'-test',negative)
  if not negative:
   assert 'all 9437184 stored bits identical'in(root/(label+'-test.log')).read_text()
   assert(root/(label+'-raw.csv')).read_bytes()==Path(p['trace_reference']).read_bytes(),'Raw data or host epochs changed'
  print(label,'runtime rejected'if negative else'all bits/exact host trace PASS',flush=True)
 save()
 try:
  case('positive',inputs/'msa_ddram.v')
  source=(inputs/'msa_ddram.v').read_text()
  for label,old,new in [('stale-child','storage_step ? ready_advance_all : ready_hold_all','&storage_group_ready'),
                       ('ignore-step','storage_step ? ready_advance_all : ready_hold_all','ready_hold_all'),
                       ('omit-group15','&ready_hold_next','&ready_hold_next[14:0]')]:
   assert source.count(old)==1,(label,source.count(old));f=root/(label+'.v');f.write_text(source.replace(old,new));case(label,f,True)
  s.update(status='PASS_MSA_SAME_EDGE_BOARD_READY_REDUCTION',finished=time.time());save()
 except BaseException:s.update(status='TERMINAL_FAILED',finished=time.time());save();raise
 print(s['status'],flush=True)

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,required=True);a=p.parse_args();run(a.root.resolve())
