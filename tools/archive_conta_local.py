#!/usr/bin/env python3
"""Preserve original local tasking/branch producer qualification and attempts.

This is component evidence only. No firmware/FPGA timing or speed claim, build
database, bitstream, snapshot or disk bytes. Existing receipts remain unchanged.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path

from archive_r_front_ready_followup import verify
from test_conta_local import MUTATIONS,bench

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
PRIMARY='conta-local-full-v12-20261023'
LOW=[f'conta-local-low-cp-{p}-v{v}-20261023'for p,v in [('balanced',13),('fast_gates',14),('later_control',15)]]
ATTEMPTS=['conta-local-probe-v1-20261023','conta-local-probe-verilator-v2-20261023',
 'conta-local-probe-v3-20261023','conta-local-full-v4-20261023','conta-local-low-cp-v5-20261023',
 *[f'conta-local-low-cp-{p}-v{v}-20261023'for p,v in [('balanced',6),('fast_gates',7),('later_control',8),('balanced',9),('fast_gates',10),('later_control',11)]]]


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def checked(name):
 d=BUILD/name;s=json.loads((d/'run-state.json').read_text())
 assert s['status']!='RUNNING'and s.get('finished'),name
 assert all('returncode'in a for a in s['actions']),name
 result=d/'results.json'
 if result.exists():
  r=json.loads(result.read_text())
  for n,h in r['inputs'].items():assert sha(d/'inputs'/n)==h,(name,n)
 else:r=None
 return d,s,r


def archive(out):
 assert not out.exists()
 d,s,r=checked(PRIMARY)
 assert r['status']=='PASS_CONTA_LOCAL_COMPONENT'and len(r['actions'])==56 and len(r['primary'])==6
 assert len({(x['engine'],x['profile'])for x in r['primary']})==6
 assert all(x['summary']=='checks=23512 cp=518 mir=2724 task_changes=19 tasks=ffff'for x in r['primary'])
 assert set(r['rejected_mutations'])==set(MUTATIONS)and len(r['negative_cases'])==22
 assert {(x['engine'],x['case'])for x in r['negative_cases']}=={(e,n)for e in ['icarus','verilator']for n in MUTATIONS}
 assert (d/'inputs/tb_conta_local.sv').read_text()==bench()
 candidate=sha(ROOT/'verilog/rtl/dorado_conta_local_step.sv')
 assert candidate==r['inputs']['verilog/rtl/dorado_conta_local_step.sv']
 for row in r['primary']:
  log=(d/row['profile']/(row['engine']+'-original')/'test.log').read_text()
  assert 'CONTA_LOCAL_CADENCE ordinary=199 extended=66'in log
  assert 'CONTA_LOCAL_TASKING changed=3f edges=3985'in log
 for name in LOW:
  run,state,record=checked(name)
  assert record['status']=='PASS_CONTA_LOCAL_PROBE'and len(record['actions'])==8 and len(record['primary'])==2,name
  assert record['inputs']['verilog/rtl/dorado_conta_local_step.sv']==candidate,name
  assert len(record['negative_cases'])==2 and {x['case']for x in record['negative_cases']}=={'new_StopTasks_data_epoch'}
  assert all(x['summary']=='checks=1496 cp=257 mir=165 task_changes=4 tasks=0003'for x in record['primary']),name
  for row in record['primary']:
   log=(run/row['profile']/(row['engine']+'-original')/'test.log').read_text()
   assert 'CONTA_LOCAL_ASYNC root=0 tasking_n=0 switch=1 StopTasks=1'in log,name
   bad=(run/row['profile']/(row['engine']+'-new_StopTasks_data_epoch')/'test.log').read_text()
   assert 'CONTA_LOCAL_FAIL check=215'in bad and 'Control-A coupled mismatch'in bad,name
 boundaries=BUILD/'conta-local-unused-boundaries-v3-20261023'
 q=json.loads((boundaries/'results.json').read_text())
 assert q['status']=='PASS_UNUSED_CONTA_LOCAL_GATE_BOUNDARIES'and len(q['boundaries'])==13 and len(q['rejected_controls'])==3
 paths={};suffixes={'.py','.sv','.v','.json','.log','.wl','.lc','.analyze','.txt','.masm'}
 def tree(prefix,directory):
  for p in directory.rglob('*'):
   if p.is_file()and 'obj'not in p.relative_to(directory).parts and p.suffix in suffixes:
    paths[prefix+'/'+str(p.relative_to(directory))]=p
 for name in [PRIMARY,*LOW,*ATTEMPTS]:
  run,_,_=checked(name);tree('runs/'+name,run)
 tree('unused-boundary-audit',boundaries)
 for n in ['conta-local-unused-probe-v1-20261023.json','conta-local-bench-reuse-v1-20261023.json',
           'native-firmware-limit-extension-v1-20261023.json']:
  paths['followup/'+n]=BUILD/n
 paths['followup/native-watch-state.json']=BUILD/'native-firmware-window-watch-v1-20261023/state.json'
 for n in ['test_conta_local.py','test_conta_local_low_cp.py','generate_conta_local_reference.py',
           'audit_conta_local_boundaries.py','archive_conta_local.py']:
  paths['current/tools/'+n]=ROOT/'tools'/n
 paths['current/verilog/rtl/dorado_conta_local_step.sv']=ROOT/'verilog/rtl/dorado_conta_local_step.sv'
 rom='chm/dorado/expanded/doradobaserom.dm!12_/doradoreset.masm';paths['original/'+rom]=ROOT/rom
 for n in ['tb_native_cedar.sv','host.cpp']:
  paths['running-source/'+n]=BUILD/'native-cedar-queue-windows-v2-20261022/inputs/harness'/n
 out.mkdir(parents=True)
 receipt=dict(scope=__doc__,primary=PRIMARY,LOW_probes=LOW,attempts=ATTEMPTS,members=[],blobs={},
  candidate_sha256=candidate,native_integrated=False,physical_timing_qualified=False,hardware_speedup_claimed=False)
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
 archive(p.parse_args().output.resolve())
