#!/usr/bin/env python3
"""Fit the same-edge one-board reduction using the qualified15ns fixture.

Retain the original virtual-service top, constraints, cells and gate. Change
only the tested MSA/group and register inventory/report target. No assembly.
"""
import argparse,hashlib,json,os,shutil,subprocess,time
from pathlib import Path

def sha(b):return hashlib.sha256(b).hexdigest()

def prepare(component,parent_component,parent_groups,parent_fit,parent_qualification,out):
 from prepare_storage_board_ready import board_reduction,group_outputs
 from review_storage_ready_prediction import review
 assert not out.exists()
 q=json.loads(parent_qualification.read_text());assert json.loads(json.dumps(review(parent_fit,parent_component,parent_groups)))==q
 p=json.loads((component/'provenance.json').read_text());s=json.loads((component/'run-state.json').read_text())
 assert s['status']=='PASS_MSA_SAME_EDGE_BOARD_READY_REDUCTION'and len(s['actions'])==8
 assert p['parent_provenance_sha256']==sha((parent_component/'provenance.json').read_bytes())
 for n,h in p['inputs'].items():assert sha((component/'inputs'/n).read_bytes())==h,n
 assert(component/'positive-raw.csv').read_bytes()==(parent_component/'positive-raw.csv').read_bytes()
 for n,fn in [('msa_ddram.v',board_reduction),('dorado_storage_group.sv',group_outputs)]:
  assert(component/'inputs'/n).read_bytes()==fn((parent_component/'inputs'/n).read_bytes())
 old=json.loads((parent_fit/'provenance.json').read_text());out.mkdir(parents=True)
 tail=b'\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
 for n,h in old['files'].items():
  b=(parent_fit/n).read_bytes()
  if n=='ready_fit.qsf':assert b.endswith(tail);b=b[:-len(tail)]
  assert sha(b)==h,n
  f=out/n;f.parent.mkdir(parents=True,exist_ok=True);f.write_bytes(b)
 for n in ['msa_ddram.v','dorado_storage_group.sv']:(out/n).write_bytes((component/'inputs'/n).read_bytes())
 audit=(out/'audit.tcl').read_text();old_guard='[get_collection_size $predicted]<16';assert audit.count(old_guard)==1
 audit=audit.replace(old_guard,'[get_collection_size $predicted]!=1')
 needle=' puts "READY_REGISTERS $name [get_collection_size $predicted]"';assert audit.count(needle)==1
 audit=audit.replace(needle,needle+'''
 set child_ready [get_registers -nowarn {*|u_storage_*|ready_q*}]
 puts "REDUNDANT_GROUP_READY $name [get_collection_size $child_ready]"
 if {[get_collection_size $child_ready]!=0} {error "Redundant group readiness state survived"}''')
 (out/'audit.tcl').write_text(audit)
 (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
 record=dict(scope=__doc__,parent_qualification_sha256=sha(parent_qualification.read_bytes()),
  parent_fit_provenance_sha256=sha((parent_fit/'provenance.json').read_bytes()),
  component_provenance_sha256=sha((component/'provenance.json').read_bytes()),
  component_state_sha256=sha((component/'run-state.json').read_bytes()),group_count=16,
  board_ready_registers=1,group_ready_registers=0,period_ns=15,
  virtual_input_bits=old['virtual_input_bits'],virtual_output_bits=old['virtual_output_bits'],
  files={str(f.relative_to(out)):sha(f.read_bytes())for f in out.rglob('*')if f.is_file()},
  assembly_permitted=False,native_qualified=False)
 (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n');print('PREPARED_BOARD_REDUCTION_15NS_FIT',flush=True)

def run(project,quartus):
 p=json.loads((project/'provenance.json').read_text())
 for n,h in p['files'].items():assert sha((project/n).read_bytes())==h,n
 assert not(project/'run-state.json').exists(),'Observe existing fit'
 s=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),actions=[],assembly_permitted=False)
 def save():(project/'run-state.json').write_text(json.dumps(s,indent=2)+'\n')
 save()
 try:
  for tool,args,label in [('quartus_map',['--read_settings_files=on','--write_settings_files=off','ready_fit'],'map'),('quartus_fit',['--read_settings_files=on','--write_settings_files=off','ready_fit'],'fit'),('quartus_sta',['ready_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'audit')]:
   with(project/(label+'.log')).open('x')as log:
    c=subprocess.Popen([str(quartus/tool),*args],cwd=project,stdout=log,stderr=subprocess.STDOUT)
    a=dict(stage=label,pid=c.pid,started=time.time());s['actions'].append(a);save();rc=c.wait();a.update(returncode=rc,finished=time.time());save();assert rc==0,(label,rc)
  s.update(status='TERMINAL_BOARD_READY_TIMING_REVIEW_REQUIRED',finished=time.time());save()
 except BaseException:s.update(status='TERMINAL_FAILED',finished=time.time());save();raise
 print(s['status'],flush=True)

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);m=p.add_mutually_exclusive_group(required=True);m.add_argument('--prepare',type=Path);m.add_argument('--run',type=Path)
 for n in ['component','parent-component','parent-groups','parent-fit','parent-qualification']:p.add_argument('--'+n,type=Path)
 p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
 if a.prepare:prepare(a.component.resolve(),a.parent_component.resolve(),a.parent_groups.resolve(),a.parent_fit.resolve(),a.parent_qualification.resolve(),a.prepare.resolve())
 else:run(a.run.resolve(),a.quartus.resolve())
