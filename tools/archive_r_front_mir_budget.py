#!/usr/bin/env python3
"""Preserve extended arrival checks and the rejected combined timing budget.

Reports and frozen sources only. No build databases, FPGA images or disk bytes.
The rejected fit remains rejected; this archive cannot qualify native speed.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path

from archive_r_front_ready_followup import verify
from audit_control_clock_fit import audit
from fit_r_front_mir import ARRIVALS
from review_r_front_mir import checked

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def archive(out):
 assert not out.exists()
 gate=BUILD/'r-front-mir-budget-gate-v1-20261023'
 terminal=BUILD/'r-front-mir-timing-terminal-v1-20261023'
 reviewed=audit(terminal,gate,False)
 assert reviewed['status']=='REJECTED_COMPONENT_TIMING'and reviewed['minimum_slack_ns']['setup']==-2.961
 assert json.loads((BUILD/'r-front-mir-timing-review-v1-20261023.json').read_text())==reviewed
 controls=BUILD/'r-front-mir-rejected-reader-controls-v1-20261023'
 c=json.loads((controls/'results.json').read_text())
 assert c['status']=='PASS_REJECTED_FIT_READER_CONTROLS'and len(c['cases'])==19 and all(x['rejected']for x in c['cases'])
 parent=BUILD/'r-front-mir-parent-ready-reader-controls-v2-20261023'
 p=json.loads((parent/'results.json').read_text())
 assert p['status']=='PASS'and len(p['cases'])==20 and all(x['rejected']for x in p['cases'])
 paths={};suffixes={'.py','.sv','.v','.json','.log','.wl','.lc','.analyze','.inc','.txt',
                   '.rpt','.summary','.sdc','.qsf','.qpf','.tcl'}
 def tree(prefix,directory):
  for p in directory.rglob('*'):
   if p.is_file()and 'obj'not in p.relative_to(directory).parts and p.suffix in suffixes:
    paths[prefix+'/'+str(p.relative_to(directory))]=p
 for name in ARRIVALS:
  d,s,log=checked(name)
  assert s['status']=='PASS_COMPOSITION_PROBE'and s['summary']=='checks=11507 writes=182 reads=4406 epochs=584'
  tree('arrivals/'+name,d)
 for prefix,d in [('qualification',gate),('timing',terminal),('reader-controls',controls),('parent-reader-controls',parent)]:tree(prefix,d)
 for n in ['r-front-mir-timing-review-v1-20261023.json','r-front-mir-rejected-path-v1-20261023.json',
           'r-front-mir-parent-ready-timing-regression-v1-20261023.json']:
  paths['reviews/'+n]=BUILD/n
 for n in ['test_r_front_mir.py','fit_r_front_mir.py','inspect_r_front_mir_timing.py',
           'audit_control_clock_fit.py','test_control_clock_fit_reader.py','archive_r_front_mir_budget.py',
           'collect_control_clock_fit.py']:
  paths['current/tools/'+n]=ROOT/'tools'/n
 # Freeze the read-only watcher prefix, without altering its controls or jobs.
 watcher=BUILD/'native-firmware-window-watch-v1-20261023'
 for n in ['state.json']:
  paths['running-prefix/'+n]=watcher/n
 out.mkdir(parents=True)
 record=dict(scope=__doc__,status='ARRIVALS_PASS_COMBINED_TIMING_REJECTED',
  members=[],blobs={},timing_review=reviewed,native_qualified=False,hardware_speedup_claimed=False)
 for name,p in sorted(paths.items()):
  raw=p.read_bytes();digest=hashlib.sha256(raw).hexdigest()
  if digest not in record['blobs']:
   stored='blobs/'+digest+'.gz';target=out/stored;target.parent.mkdir(parents=True,exist_ok=True)
   target.write_bytes(gzip.compress(raw,mtime=0))
   record['blobs'][digest]=dict(stored=stored,raw_bytes=len(raw),stored_bytes=target.stat().st_size,stored_sha256=sha(target))
  record['members'].append(dict(name=name,blob=digest))
 (out/'manifest.json.gz').write_bytes(gzip.compress((json.dumps(record,indent=2)+'\n').encode(),mtime=0))
 verify(out)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
 archive(p.parse_args().output.resolve())
