#!/usr/bin/env python3
"""Review and preserve the bounded R-front/MIR ordering experiment.

No native integration, complete producer-arrival contract or physical timing
claim. Earlier compiler/bench failures and rejected uniform epochs stay visible.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
POSITIVES=[
 'r-front-mir-matrix-balanced-icarus-v16-20261023',
 'r-front-mir-matrix-balanced-verilator-v11-20261023',
 'r-front-mir-matrix-fast-icarus-v9-20261023',
 'r-front-mir-matrix-fast-verilator-v12-20261023',
 'r-front-mir-matrix-later-icarus-v10-20261023',
 'r-front-mir-matrix-later-verilator-v13-20261023',
]
NEGATIVES=[f'r-front-mir-negative-{mode}-{engine}-v{version}-20261023'
 for mode,engine,version in [('old','icarus',14),('new','icarus',15),('old','verilator',17),('new','verilator',18)]]
CP=['r-front-mir-cp-coverage-icarus-v19-20261023','r-front-mir-cp-coverage-verilator-v20-20261023']


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def checked(name):
 d=BUILD/name;s=json.loads((d/'run-state.json').read_text())
 assert s['status']!='RUNNING' and s.get('finished'),name
 for n,h in s['inputs'].items():assert sha(d/'inputs'/n)==h,(name,n)
 for n,h in s.get('timed_sources',{}).items():assert sha(d/'timed'/n)==h,(name,n)
 assert all('returncode'in a for a in s['actions']),name
 return d,s,(d/'test.log').read_text()


def review(out,directed,low_cp):
 assert not out.exists();summaries=[];rows=[];candidate=sha(ROOT/'verilog/rtl/dorado_r_front_mir_step.sv')
 fresh=BUILD/'r-front-mir-reference-metadata-v21-20261023'
 for name in POSITIVES:
  d,s,log=checked(name)
  assert s['status']=='PASS_COMPOSITION_PROBE'and s['summary']=='checks=4417 writes=182 reads=822 epochs=267',name
  assert len(s['actions'])==2 and all(a['returncode']==0 for a in s['actions']),name
  assert s['post_mir_pins']==2 and sha(d/'candidate.sv')==candidate,name
  assert s['reference']['physical_bits']==786 and s['reference']['clock_channels']==67
  for p in fresh.glob('*.sv'):assert p.read_bytes()==(d/'reference'/p.name).read_bytes(),(name,p.name)
  clocks=[(int(a),int(b))for a,b in re.findall(r'R_FRONT_MIR_CLOCK channel=(\d+) edges=(\d+)',log)]
  assert len(clocks)==67 and [i for i,n in clocks if n==0]==[65,66],name
  summaries.append(s['summary']);rows.append(dict(run=name,profile=s['profile'],engine=s['engine'],summary=s['summary']))
 assert len({(r['profile'],r['engine'])for r in rows})==6
 for name in NEGATIVES:
  d,s,log=checked(name);assert s['status']=='COMPOSITION_PROBE_FAILED',name
  assert s['actions'][0]['returncode']==0 and s['actions'][1]['returncode']!=0,name
  if s['post_mir_pins']==0:assert 'resolved source mismatch check=67'in log,name
  else:
   assert 'state mismatch check=91'in log and 'R_FRONT_DIFF bit=597'in log and 'R_FRONT_DIFF bit=607'in log,name
  mutant=(ROOT/'verilog/rtl/dorado_r_front_mir_step.sv').read_text().replace('POST_MIR_PINS=2',f"POST_MIR_PINS={s['post_mir_pins']}")
  assert (d/'candidate.sv').read_text()==mutant,name
 for name in CP:
  d,s,log=checked(name);assert s['status']=='PASS_COMPOSITION_PROBE'and s['summary']=='checks=1537 writes=0 reads=346 epochs=10',name
  assert all(a['returncode']==0 for a in s['actions']) and sha(d/'candidate.sv')==candidate,name
  assert 'R_FRONT_MIR_CLOCK channel=65 edges=128'in log and 'R_FRONT_MIR_CLOCK channel=66 edges=128'in log,name
  assert 'R_FRONT_MIR_CHANGED front=c000000f0000000000000000f Ready=ffff'in log,name
 for name in directed:
  d,s,log=checked(name);assert s['status']=='PASS_COMPOSITION_PROBE'and s['directed_RBase_StkP'],name
  assert 'R_FRONT_MIR_DIRECTED RBase=32 StkP=256'in log,name
  assert all(a['returncode']==0 for a in s['actions']) and sha(d/'candidate.sv')==candidate,name
 for name in low_cp:
  d,s,log=checked(name);assert s['status']=='PASS_COMPOSITION_PROBE'and s['LOW_root_CP'],name
  assert s['summary']=='checks=2818 writes=0 reads=1370 epochs=10',name
  assert all(a['returncode']==0 for a in s['actions']) and sha(d/'candidate.sv')==candidate,name
  for channel in [65,66]:assert f'R_FRONT_MIR_CLOCK channel={channel} edges=256'in log,name
 m=json.loads((fresh/'manifest.json').read_text())
 assert m['packages']==sum(len(b['packages'])for b in m['boards'])==296
 assert m['generator_sha256']==sha(ROOT/'tools/generate_r_front_mir_reference.py')
 r=dict(status='PASS_BOUNDED_R_FRONT_MIR_ORDERING_EXPERIMENT',scope=__doc__,candidate_sha256=candidate,
        primary=rows,negative_controls=NEGATIVES,CP_coverage=CP,directed=directed,LOW_root_CP=low_cp,
        physical_Qs_compared=786,RAM_bits_compared=9216,clock_channels_compared_per_event=67,
        reference_metadata_correction='Old parent package count274/generator hash inherited; corrected296/current generator; every executed reference SV byte identical.',
        coverage_report_correction='Primary clock totals printed before CP sweep; CP extensions print after sweep and require128 captures per bank.',
        native_integrated=False,physical_timing_qualified=False,full_producer_arrival_contract_qualified=False,
        native_speedup_qualified=False)
 out.write_text(json.dumps(r,indent=2)+'\n');print(r['status']);return r


def archive(out,review_path,extra_runs):
 from archive_r_front_ready_followup import verify
 r=json.loads(review_path.read_text());names=POSITIVES+NEGATIVES+CP+r['directed']+r['LOW_root_CP']+extra_runs
 assert len(names)==len(set(names));paths={}
 for name in names:
  d=BUILD/name;s=json.loads((d/'run-state.json').read_text())
  assert s['status']!='RUNNING',name
  for p in d.rglob('*'):
   rel=p.relative_to(d)
   if not p.is_file()or 'obj'in rel.parts:continue
   if p.suffix in ('.sv','.v','.py','.json','.log','.wl','.lc','.analyze','.inc','.txt'):
    paths['runs/'+name+'/'+str(rel)]=p
 fresh=BUILD/'r-front-mir-reference-metadata-v21-20261023'
 for p in fresh.rglob('*'):
  if p.is_file():paths['corrected-reference/'+str(p.relative_to(fresh))]=p
 for n in ['tools/review_r_front_mir.py','tools/test_r_front_mir.py','tools/generate_r_front_mir_reference.py','verilog/rtl/dorado_r_front_mir_step.sv']:
  paths['current/'+n]=ROOT/n
 paths['review.json']=review_path
 out.mkdir(parents=True,exist_ok=False);receipt=dict(scope=__doc__,runs=names,members=[],blobs={},native_speedup_qualified=False)
 for name,p in sorted(paths.items()):
  raw=p.read_bytes();digest=hashlib.sha256(raw).hexdigest()
  if digest not in receipt['blobs']:
   stored='blobs/'+digest+'.gz';target=out/stored;target.parent.mkdir(parents=True,exist_ok=True)
   target.write_bytes(gzip.compress(raw,mtime=0))
   receipt['blobs'][digest]=dict(stored=stored,raw_bytes=len(raw),stored_bytes=target.stat().st_size,stored_sha256=sha(target))
  receipt['members'].append(dict(name=name,blob=digest))
 (out/'manifest.json.gz').write_bytes(gzip.compress((json.dumps(receipt,indent=2)+'\n').encode(),mtime=0))
 verify(out)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
 p.add_argument('--directed',action='append',default=[]);p.add_argument('--archive',type=Path);p.add_argument('--extra-run',action='append',default=[])
 p.add_argument('--low-cp',action='append',default=[])
 a=p.parse_args();review(a.output.resolve(),a.directed,a.low_cp)
 if a.archive:archive(a.archive.resolve(),a.output.resolve(),a.extra_run)
