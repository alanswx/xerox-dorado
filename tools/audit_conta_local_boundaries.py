#!/usr/bin/env python3
"""Prove remaining tasking/branch package-half extras are unused in this slice.

The two other d17 data inputs are explicit physical register boundaries, not
stubs. These13 extras only feed unconsumed halves of selected combinational
packages. Full-machine producers remain required.
"""
import argparse
import hashlib
import json
from pathlib import Path
import sys
from unittest.mock import patch

from audit_mir_ready_boundaries import audit_selected
from generate_conta_local_reference import plan,ROOT
from inventory_mir_clock_boundaries import BOARD


def audit(root):
 spec,extras=plan(root)
 return audit_selected(root,spec,extras,13)


def run(out):
 assert not out.exists();record,cells=audit(ROOT);out.mkdir(parents=True);inputs=out/'inputs'
 sources={BOARD,BOARD.replace('.wl','.lc'),*cells}
 for m in list(sys.modules.values()):
  name=getattr(m,'__file__',None)
  if name and Path(name).resolve().parent==ROOT/'tools':sources.add(str(Path(name).resolve().relative_to(ROOT)))
 hashes={}
 for n in sorted(sources):
  p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=hashlib.sha256(p.read_bytes()).hexdigest()
 controls=[]
 for label,kind,a,b in [
  ('SWm_into_preemption','MC1662','assign p14 = ~(p10 | p11);','assign p14 = ~(p10 | p11 | p5);'),
  ('Call13_into_branch','MC10102','assign p2 = ~(p4 | p5);','assign p2 = ~(p4 | p5 | p6);'),
  ('WantIfuHold_into_tasking','MC10109','assign p2  =  a;','assign p2  =  a | p12;')]:
  file=ROOT/'verilog/cells'/('cell_'+kind+'.v');source=file.read_text();assert source.count(a)==1;changed=source.replace(a,b)
  original_read=Path.read_text
  def read(path,*args,**kwargs):return changed if path==file else original_read(path,*args,**kwargs)
  with patch.object(Path,'read_text',read):
   try:audit(ROOT)
   except AssertionError as error:
    assert any(t in str(error)for t in ['LIVE SELECTED CONSUMER','OBSERVED OUTPUT']),error
    controls.append(dict(case=label,rejected=True,diagnostic=str(error)))
   else:raise AssertionError('Live dependency accepted: '+label)
 for n,h in hashes.items():assert hashlib.sha256((inputs/n).read_bytes()).hexdigest()==h,n
 record.update(status='PASS_UNUSED_CONTA_LOCAL_GATE_BOUNDARIES',scope=__doc__,inputs=hashes,rejected_controls=controls)
 (out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
 print(record['status'],len(record['boundaries']),len(controls))


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
 run(p.parse_args().output.resolve())
