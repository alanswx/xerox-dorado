#!/usr/bin/env python3
"""Reject changed bound evidence before any Quartus assembler operation."""
import argparse,hashlib,json,os,shutil,tempfile
from pathlib import Path
from assemble_storage_cache_native import validate,GUARD_CASES
ROOT=Path(__file__).resolve().parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def run(root,counter,qualified,controls,out):
 assert not out.exists();out.mkdir(parents=True);fixture=out/'fixture';shutil.copytree(root,fixture,copy_function=os.link)
 shutil.copytree(counter,fixture/'cache-hit-audit-v3',copy_function=os.link)
 shutil.copyfile(qualified,fixture/'cache-preassembly-qualification.json');shutil.copyfile(controls,fixture/'cache-fit-review-controls.json')
 c=json.loads(controls.read_text())
 for n,h in c['reviewer_dependencies'].items():
  f=ROOT/n;assert sha(f)==h;p=fixture/'review-dependencies'/n;p.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(f,p)
 validate(fixture)
 def hashes(p):return {str(f.relative_to(p)):sha(f)for f in p.rglob('*')if f.is_file()}
 before=hashes(root);counter_before=hashes(counter)
 def write(p,b):
  t=p.with_name(p.name+'.mutation');t.write_bytes(b);os.replace(t,p)
 def changed(p,n):f=p/n;write(f,f.read_bytes()+b'\n')
 def nonrejecting(p):
  f=p/'cache-fit-review-controls.json';r=json.loads(f.read_text());r['cases'][0]['returncode']=0;write(f,json.dumps(r).encode())
 corner='7_slow_1100mv_100c'
 mutations={
  'changed-source':lambda p:changed(p,'src/fpga/mister/dorado_storage_group.sv'),
  'changed-report':lambda p:changed(p,f'core/storage-sample-input-setup-{corner}.rpt'),
  'changed-qualification':lambda p:changed(p,'cache-preassembly-qualification.json'),
  'changed-reviewer':lambda p:changed(p,'review-dependencies/tools/review_storage_cache_native.py'),
  'nonrejecting-control':nonrejecting,
  'missing-phase-report':lambda p:(p/f'core/storage-sample-output-hold-{corner}.rpt').unlink(),
  'changed-manifest':lambda p:changed(p,'storage-cache-native.json'),
  'changed-counter-report':lambda p:changed(p,f'cache-hit-audit-v3/cache-hit-event-setup-{corner}.rpt'),
  'changed-counter-script':lambda p:changed(p,'cache-hit-audit-v3/audit.tcl')}
 assert set(mutations)==GUARD_CASES;cases=[]
 for name,mutate in mutations.items():
  with tempfile.TemporaryDirectory(prefix='dorado-cache-assembly-bad-')as tmp:
   p=Path(tmp)/'fixture';shutil.copytree(fixture,p,copy_function=os.link);mutate(p)
   try:validate(p)
   except(AssertionError,FileNotFoundError)as e:cases.append(dict(name=name,rejected=True,error=type(e).__name__,detail=str(e)))
   else:raise AssertionError('Accepted '+name)
   assert not(p/'cache-assembly-state.json').exists();print(name,'rejected',flush=True)
 validate(fixture);assert hashes(root)==before and hashes(counter)==counter_before
 r=dict(status='PASS_NATIVE_STORAGE_CACHE_ASSEMBLY_GUARD_CONTROLS',cases=cases,assembler_sha256=sha(ROOT/'tools/assemble_storage_cache_native.py'),qualification_sha256=sha(qualified),reviewer_controls_sha256=sha(controls),original_evidence_unchanged=True,Quartus_executed=False)
 (out/'guard-controls.json').write_text(json.dumps(r,indent=2)+'\n');print(r['status'],len(cases))
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['root','counter','qualified','controls','output']:p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();run(a.root.resolve(),a.counter.resolve(),a.qualified.resolve(),a.controls.resolve(),a.output.resolve())
