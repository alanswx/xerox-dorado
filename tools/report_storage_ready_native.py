#!/usr/bin/env python3
"""Follow the existing native build with read-only storage reports/inventory.

Never refit, assemble, change constraints or restart the builder. Freeze report
inputs and retain any original report-stage failure separately. A repaired
report filter is evidence collection only, not a passed native qualification.
"""
import argparse,hashlib,json,os,subprocess,time
from pathlib import Path
def sha(b):return hashlib.sha256(b).hexdigest()

def run(project,quartus):
 out=project/'storage-ready-reports-v1';assert not(out/'run-state.json').exists()
 proof=json.loads((out/'inputs.json').read_text())
 for n,h in proof.items():assert sha((out/n).read_bytes())==h,n
 manifest=json.loads((project/'storage-ready-native.json').read_text())
 state=dict(status='WAITING_EXISTING_NATIVE_BUILD',driver_pid=os.getpid(),started=time.time(),actions=[],report_inputs=proof,assembly_permitted=False)
 def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
 save()
 try:
  while True:
   build=json.loads((project/'run-state.json').read_text())
   alive=(Path('/proc')/str(build['builder_pid'])).exists()
   if build['status']!='RUNNING'and not alive:break
   assert alive,'Builder disappeared without terminal state; inspect, do not restart'
   time.sleep(10)
  state['original_build_state']=build;save()
  original=['map','fit','sta','clocks','corners','control-crossings','contb-crossings','adapter-crossings']
  assert [s['stage']for s in build['stages'][:8]]==original and all(s['returncode']==0 for s in build['stages'][:8])
  assert len(build['stages'])==9 and build['stages'][8]['stage']=='storage-ready'
  for s in build['stages']:assert not(Path('/proc')/str(s['pid'])).exists(),s
  if build['status']=='TERMINAL_FAILED':
   assert build['stages'][-1]['returncode']!=0
   text=(project/'storage-ready.log').read_text()
   assert 'Missing native ready/step state'in text,'Unexpected original report failure'
   state['original_report_failure_retained']=True
  else:assert build['status']=='TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'and build['stages'][-1]['returncode']==0
  for n,h in manifest['source_hashes'].items():assert sha((project/n).read_bytes())==h,n
  for script,label in [('audit-storage-ready2.tcl','ready'),('audit_storage_burst_observer_native.tcl','backend'),('inspect_storage_burst_observer_fit.tcl','inventory')]:
   command=[str(quartus/'quartus_sta'),'-t',str(out/script)]
   with(out/(label+'.log')).open('x')as log:
    p=subprocess.Popen(command,cwd=project/'core',stdout=log,stderr=subprocess.STDOUT)
    row=dict(stage=label,pid=p.pid,command=command,started=time.time());state['actions'].append(row);state['status']='RUNNING_READ_ONLY_REPORTS';save();rc=p.wait();row.update(returncode=rc,finished=time.time());save()
   assert rc==0,(label,rc)
  for n,h in manifest['source_hashes'].items():assert sha((project/n).read_bytes())==h,n
  state.update(status='TERMINAL_STORAGE_READY_REPORTS_REVIEW_REQUIRED',finished=time.time());save()
 except BaseException:state.update(status='TERMINAL_FAILED',finished=time.time());save();raise
 print(state['status'],flush=True)

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--project',type=Path,required=True);p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args();run(a.project.resolve(),a.quartus.resolve())
