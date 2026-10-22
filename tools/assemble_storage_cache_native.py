#!/usr/bin/env python3
"""Assemble the fully reviewed native16-line backing-buffer fit once.

Require source/report/counter/reviewer and negative-control bindings before
calling Quartus. No fit, constraint change or hardware deployment is allowed.
"""
import argparse,hashlib,json,os,subprocess,time
from pathlib import Path
from test_review_storage_cache_native import CASES
GUARD_CASES={'changed-source','changed-report','changed-qualification','changed-reviewer','nonrejecting-control','missing-phase-report','changed-manifest','changed-counter-report','changed-counter-script'}
def sha(p):
 h=hashlib.sha256()
 with p.open('rb')as f:
  for b in iter(lambda:f.read(1024*1024),b''):h.update(b)
 return h.hexdigest()
def checked(root,n):
 p=Path(n);assert not p.is_absolute()and'..'not in p.parts;return root/p
def validate(project):
 qpath=project/'cache-preassembly-qualification.json';cpath=project/'cache-fit-review-controls.json'
 q=json.loads(qpath.read_text());c=json.loads(cpath.read_text());r=json.loads((project/'storage-cache-native.json').read_text())
 assert q['status']=='PASS_NATIVE_STORAGE_CACHE_PREASSEMBLY_REVIEW'
 assert c['status']=='PASS_NATIVE_STORAGE_CACHE_REVIEW_REJECTION_CONTROLS'
 assert c['positive_qualification_sha256']==sha(qpath)and c['original_evidence_unchanged']
 assert {x['name']for x in c['cases']}==set(CASES)and len(c['cases'])==len(CASES)
 assert all(x['rejected']and x['returncode']!=0 for x in c['cases'])
 assert 'tools/review_storage_cache_native.py'in c['reviewer_dependencies']
 for n,h in c['reviewer_dependencies'].items():assert sha(checked(project/'review-dependencies',n))==h,n
 assert q['recipe_config_sha256']==c['recipe_config_sha256']
 assert q['frequency_hz']==r['frequency_hz']==57142857 and q['sysper']==r['sysper']==4
 assert q['state_admin']==r['state_admin']=='INACTIVE'and q['raw_buffer_lines']==r['raw_buffer_lines']==16
 assert not q['firmware_qualified']and not q['hardware_speedup_qualified']
 assert q['source_hashes']==r['source_hashes']and len(q['source_hashes'])==503
 assert len(q['timing_checks'])==31 and all(float(s)>0 for _,s in q['timing_checks'])
 assert len(q['detailed_paths'])==356 and all(x['paths']>0 and x['slack_ns']>0 for x in q['detailed_paths'])
 assert q['minimum_setup_ns']>0 and q['minimum_hold_ns']>0
 assert q['resources']['ram_blocks']==332 and q['resources']['dsp_blocks']==35
 assert q['memory']['cache']['packing']=='independent40'and q['hit_counter_bits']==32
 assert q['buffer_registers']=={'line_words':{'bits':1536,'bits_per_line':96},'line_tag':{'bits':320,'bits_per_line':20}}
 for n,h in {**q['source_hashes'],**q['reviewed_evidence_hashes']}.items():assert sha(checked(project,n))==h,n
 for n,h in q['counter_evidence_hashes'].items():assert sha(checked(project/'cache-hit-audit-v3',n))==h,n
 return q,c
def run(project,quartus):
 q,c=validate(project);g=json.loads((project/'cache-assembly-guard-controls.json').read_text())
 assert g['status']=='PASS_NATIVE_STORAGE_CACHE_ASSEMBLY_GUARD_CONTROLS'
 assert g['assembler_sha256']==sha(Path(__file__))and g['original_evidence_unchanged']and not g['Quartus_executed']
 assert g['qualification_sha256']==sha(project/'cache-preassembly-qualification.json')
 assert g['reviewer_controls_sha256']==sha(project/'cache-fit-review-controls.json')
 assert {x['name']for x in g['cases']}==GUARD_CASES and len(g['cases'])==len(GUARD_CASES)and all(x['rejected']for x in g['cases'])
 state_path=project/'cache-assembly-state.json';rbf=project/'core/output_files/Dorado.rbf'
 assert not state_path.exists()and not rbf.exists(),'Inspect the existing assembly, never restart'
 state=dict(status='RUNNING_ASSEMBLY',driver_pid=os.getpid(),started=time.time(),qualification_sha256=sha(project/'cache-preassembly-qualification.json'),controls_sha256=sha(project/'cache-fit-review-controls.json'),firmware_qualified=False,hardware_deployed=False)
 def save():
  t=state_path.with_suffix('.tmp');t.write_text(json.dumps(state,indent=2)+'\n');os.replace(t,state_path)
 save()
 try:
  command=[str(quartus/'quartus_asm'),'--read_settings_files=on','--write_settings_files=off','Dorado']
  with(project/'cache-assembly.log').open('x')as log:
   p=subprocess.Popen(command,cwd=project/'core',stdout=log,stderr=subprocess.STDOUT);state.update(child_pid=p.pid,command=command);save();code=p.wait();state['returncode']=code;save();assert code==0
  assert'Assembler was successful'in(project/'cache-assembly.log').read_text()
  assert rbf.stat().st_size>1000000
  validate(project)
  state.update(status='TERMINAL_ASSEMBLED_REVIEWED_NATIVE_STORAGE_CACHE',finished=time.time(),rbf_sha256=sha(rbf),rbf_bytes=rbf.stat().st_size);save();print(json.dumps(state),flush=True)
 except BaseException:
  state.update(status='TERMINAL_FAILED',finished=time.time());save();raise
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--project',type=Path,required=True);p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));p.add_argument('--check-only',action='store_true');a=p.parse_args()
 if a.check_only:validate(a.project.resolve());print('PASS_NATIVE_STORAGE_CACHE_ASSEMBLY_INPUT_GUARD; no Quartus call')
 else:run(a.project.resolve(),a.quartus.resolve())
