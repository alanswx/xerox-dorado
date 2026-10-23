#!/usr/bin/env python3
"""Check unused package-half boundaries in the coupled Control-A graph.

Signals are unused only in this selected component. Original full-machine
producers remain required. Checks actual cell output-pin dependencies.
"""
import argparse,hashlib,json,sys
from pathlib import Path
from unittest.mock import patch
from audit_mir_ready_boundaries import audit_selected
from generate_conta_mir_reference import plan,ROOT
from inventory_mir_clock_boundaries import BOARD


def audit(root):
 spec,extras=plan(root);return audit_selected(root,spec,extras,15)


def run(out):
 assert not out.exists();r,cells=audit(ROOT);r['scope']=__doc__;out.mkdir(parents=True);inputs=out/'inputs'
 sources=[BOARD,BOARD.replace('.wl','.lc'),*cells]
 for m in list(sys.modules.values()):
  name=getattr(m,'__file__',None)
  if name and Path(name).resolve().parent==ROOT/'tools':sources.append(str(Path(name).resolve().relative_to(ROOT)))
 hashes={}
 for name in sorted(set(sources)):
  p=inputs/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/name).read_bytes());hashes[name]=hashlib.sha256(p.read_bytes()).hexdigest()
 controls=[]
 for label,kind,a,b in [('SWm_into_preemption','MC1662','assign p14 = ~(p10 | p11);','assign p14 = ~(p10 | p11 | p5);'),
 ('Link_into_Notify','MC10103','assign p14 = (p10 | p11);','assign p14 = (p10 | p11 | p13);')]:
  file=ROOT/'verilog/cells'/('cell_'+kind+'.v');source=file.read_text();assert source.count(a)==1;changed=source.replace(a,b)
  original_read=Path.read_text
  def read(path,*args,**kwargs):return changed if path==file else original_read(path,*args,**kwargs)
  with patch.object(Path,'read_text',read):
   try:audit(ROOT)
   except AssertionError as error:
    assert 'LIVE SELECTED CONSUMER'in str(error),error
    controls.append(dict(case=label,rejected=True,diagnostic=str(error)))
   else:raise AssertionError('Live boundary accepted: '+label)
 for name,digest in hashes.items():assert hashlib.sha256((inputs/name).read_bytes()).hexdigest()==digest,name
 r.update(status='PASS_UNUSED_CONTA_MIR_GATE_BOUNDARIES',inputs=hashes,rejected_controls=controls)
 (out/'results.json').write_text(json.dumps(r,indent=2)+'\n');print(r['status'],len(r['boundaries']),len(controls))


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);run(p.parse_args().output.resolve())
