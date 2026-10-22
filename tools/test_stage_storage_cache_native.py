#!/usr/bin/env python3
"""Reject wrong native sources, clocks, mutation gates and board evidence.

Use isolated symlink/copy views of already-qualified inputs. Never alter the
positive source stage or rerun its simulations/fit. No Quartus or FPGA action.
"""
import argparse,hashlib,json
from pathlib import Path
from stage_storage_cache_native import stage,WRAPPER
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def run(a):
 out=a.output.resolve();assert not out.exists();out.mkdir(parents=True)
 parent=a.parent.resolve();q=json.loads(a.qualification.read_text())
 baseline={n:sha(parent/n)for n in q['source_hashes']}
 cases=[]
 for name in ['changed-native-wrapper','wrong-host-frequency','nonrejecting-control','changed-board-trace']:
  p=out/name;p.mkdir();args=[parent,a.qualification.resolve(),a.capacity.resolve(),a.controls.resolve(),a.board.resolve(),p/'candidate',Path('/home/alans/mister/REJECTED_StorageCache57')]
  if name=='changed-native-wrapper':
   view=p/'parent';view.mkdir()
   (view/'pack-window-native.json').symlink_to(parent/'pack-window-native.json')
   for n in baseline:
    target=view/n;target.parent.mkdir(parents=True,exist_ok=True)
    if n==WRAPPER:target.write_bytes((parent/n).read_bytes()+b'\n// deliberately changed source\n')
    else:target.symlink_to(parent/n)
   args[0]=view
  elif name=='wrong-host-frequency':
   bad=dict(q,frequency_hz=66666667);f=p/'qualification.json';f.write_text(json.dumps(bad));args[1]=f
  elif name=='nonrejecting-control':
   s=json.loads((a.controls/'run-state.json').read_text());s['cases'][1]['rejected']=False
   d=p/'controls';d.mkdir();(d/'run-state.json').write_text(json.dumps(s));args[3]=d
  else:
   d=p/'board';d.mkdir()
   for n in ['inputs','provenance.json.gz','run-state.json','lines4-raw.csv']:(d/n).symlink_to((a.board/n).resolve())
   (d/'lines16-raw.csv').write_bytes((a.board/'lines16-raw.csv').read_bytes()+b'CORRUPT\n');args[4]=d
  try:stage(*args)
  except AssertionError as error:
   assert not args[5].exists(),'Rejected input produced a candidate'
   if name=='changed-native-wrapper':assert error.args==(WRAPPER,),error.args
   cases.append(dict(name=name,rejected=True,diagnostic=repr(error),candidate_created=False))
  else:raise AssertionError(('Invalid evidence accepted',name))
 assert all(sha(parent/n)==h for n,h in baseline.items())
 result=dict(status='PASS_STORAGE_CACHE_NATIVE_STAGING_REJECTION_CONTROLS',cases=cases,
  qualified_parent_unchanged=True,Quartus_or_FPGA_executed=False,
  positive_manifest_sha256=sha(a.positive/'storage-cache-native.json'),
  stager_sha256=sha(Path(__file__).with_name('stage_storage_cache_native.py')),
  controls_sha256=sha(Path(__file__)))
 (out/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(cases),'rejections')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['parent','qualification','capacity','controls','board','positive','output']:p.add_argument('--'+n,type=Path,required=True)
 run(p.parse_args())
