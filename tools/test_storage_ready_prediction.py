#!/usr/bin/env python3
"""Compare post-edge ready prediction with the original144-chip MSA board.

Every original output and stored bit remains checked. A local assertion also
compares predicted ready with the unchanged combinational equation after each
host edge. No native fit, clock acceleration or hardware qualification here.
"""
import argparse,hashlib,json,os,subprocess,time
from pathlib import Path

def run(root,controls):
 inputs=root/'inputs';proof=json.loads((root/'provenance.json').read_text())
 for n,h in proof['inputs'].items():assert hashlib.sha256((inputs/n).read_bytes()).hexdigest()==h,n
 assert not(root/'run-state.json').exists()
 (root/'runner-executed.py').write_bytes(Path(__file__).read_bytes())
 state=dict(status='RUNNING',scope=__doc__,driver_pid=os.getpid(),started=time.time(),actions=[],runner_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),native_qualified=False,hardware_qualified=False)
 def save():(root/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
 def execute(command,label,negative=False):
  with(root/(label+'.log')).open('x')as log:
   p=subprocess.Popen(command,cwd=inputs,stdout=log,stderr=subprocess.STDOUT);r=dict(stage=label,pid=p.pid,command=command,started=time.time());state['actions'].append(r);save();code=p.wait();r.update(returncode=code,finished=time.time());save()
  text=(root/(label+'.log')).read_text()
  if negative:assert code!=0 and 'STORAGE_READY_PREDICTION' in text,(label,code,text[-2000:])
  else:assert code==0,(label,code,text[-2000:])
 def case(label,group,negative=False):
  obj=root/('obj-'+label)
  names=['msa_ddram.v','msa.v','dorado_storage_arb.sv','dorado_storage_sdram.sv','dorado_storage_sdram_burst.sv','xsds_model.sv','tb.sv','burst-fixture.sv','addr_preview.sv']
  cmd=['verilator','--binary','--timing','--autoflush','-j','2','--top-module','tb_storage_board','-GADDR_BITS=16','-GPREFETCH_NEXT=1','-GINTERLEAVE_GROUPS=1','-DSTORAGE_USE_SDRAM','-Wno-fatal','-Wno-TIMESCALEMOD','-Wno-WIDTHTRUNC','-Wno-WIDTHEXPAND','-Wno-REALCVT','-I'+str(inputs/'cells'),'--Mdir',str(obj),'-o','ready-test',str(group),*[str(inputs/n)for n in names]]
  execute(cmd,label+'-compile');execute([str(obj/'ready-test'),'+raw_trace='+str(root/(label+'-raw.csv'))],label+'-test',negative)
  if not negative:
   text=(root/(label+'-test.log')).read_text();assert 'all 9437184 stored bits identical' in text
   reference=Path(proof['parent'])/'burst-raw.csv'
   assert (root/(label+'-raw.csv')).read_bytes()==reference.read_bytes(),'Ready prediction changed raw transaction data or host epochs'
  print(label,'runtime rejected'if negative else 'original-board/complete-storage/exact-host-trace PASS',flush=True)
 save()
 try:
  case('positive',inputs/'dorado_storage_group.sv')
  if controls:
   source=(inputs/'dorado_storage_group.sv').read_text()
   mutations=[('stale-ready','machine_step ? ready_advance_next : ready_hold_next','ready_reference'),('old-address','pred_cache_addr==addr_after_edge','pred_cache_addr==addr'),('ignore-write','(!advance_write_pending || pred_write_done)',"1'b1")]
   for label,old,new in mutations:
    assert source.count(old)==1,(label,source.count(old));p=root/(label+'.sv');p.write_text(source.replace(old,new));case(label,p,True)
  state.update(status='PASS_POST_EDGE_READY_ORIGINAL_MSA_COMPONENT',finished=time.time());save()
 except BaseException:
  state.update(status='TERMINAL_FAILED',finished=time.time());save();raise
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,required=True);p.add_argument('--controls',action='store_true');a=p.parse_args();run(a.root.resolve(),a.controls)
