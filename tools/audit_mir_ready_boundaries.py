#!/usr/bin/env python3
"""Prove additional MIR/Ready fixture pins touch only unconsumed gate halves.

These signals remain required outside this selected graph. This is an exact
cell-function dependency check, not an assertion that the full machine may tie
them off. Native original logic is unchanged.
"""
import argparse,hashlib,json,sys
from pathlib import Path
from unittest.mock import patch
from audit_task_control_stubs import pin_dependencies
from generate_mir_ready_reference import plan,ROOT
from inventory_mir_clock_boundaries import BOARD
from sil_netlist import load_board


def audit_selected(root,spec,extras,expected):
 b=load_board(str(root/BOARD));chosen=set(spec['comb'])|set(spec['seq'])
 observed=set(spec['flag_taps'])|set(spec['clock_taps']);rows=[];cells=set()
 pin_net={(p['pkg'],p['pin']):n for n,row in b.nets.items()for p in row['pins']}
 for net,index in extras.items():
  consumers=[p for p in b.nets[net]['pins']if p['dir']=='in'and p['pkg']in chosen];assert consumers,net
  effects=[]
  for p in consumers:
   assert p['pkg']in spec['comb'],('Sequential extra boundary',net,p)
   kind=b.packages[p['pkg']]['type'];cells.add('verilog/cells/cell_'+kind+'.v')
   affected=[out for out,inputs in pin_dependencies(root,kind).items()if p['pin']in inputs];assert affected,(net,p)
   for out in affected:
    n=pin_net.get((p['pkg'],out));sinks=[]if n is None else [x for x in b.nets[n]['pins']if x['dir']=='in'and x['pkg']in chosen]
    assert not sinks,('LIVE SELECTED CONSUMER',net,n,sinks)
    assert n not in observed,('OBSERVED OUTPUT',net,n)
    effects.append(dict(package=p['pkg'],input_pin=p['pin'],output_pin=out,output_net=n))
  rows.append(dict(net=net,pin=index,effects=effects))
 assert len(rows)==expected
 return dict(status='PASS_UNUSED_MIR_READY_GATE_BOUNDARIES',scope=__doc__,boundaries=rows,full_machine_signals_inactive=False),sorted(cells)


def audit(root):
 spec,extras=plan(root);return audit_selected(root,spec,extras,8)


def run(out):
 assert not out.exists();r,cells=audit(ROOT);out.mkdir(parents=True);inputs=out/'inputs';sources=[BOARD,BOARD.replace('.wl','.lc'),*cells]
 for m in list(sys.modules.values()):
  file=getattr(m,'__file__',None)
  if file and Path(file).resolve().parent==ROOT/'tools':sources.append(str(Path(file).resolve().relative_to(ROOT)))
 hashes={}
 for name in sorted(set(sources)):
  p=inputs/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/name).read_bytes());hashes[name]=hashlib.sha256(p.read_bytes()).hexdigest()
 controls=[]
 for label,kind,a,b in [('SWm_into_preemption','MC1662','assign p14 = ~(p10 | p11);','assign p14 = ~(p10 | p11 | p5);'),
                         ('Link_decode_into_Notify','MC10103','assign p14 = (p10 | p11);','assign p14 = (p10 | p11 | p13);')]:
  file=ROOT/'verilog/cells'/('cell_'+kind+'.v');raw=file.read_text();assert raw.count(a)==1;changed=raw.replace(a,b)
  original_read=Path.read_text
  def read(path,*args,**kwargs):return changed if path==file else original_read(path,*args,**kwargs)
  with patch.object(Path,'read_text',read):
   try:audit(ROOT)
   except AssertionError as error:
    assert 'LIVE SELECTED CONSUMER'in str(error),error
    controls.append(dict(case=label,rejected=True,diagnostic=str(error)))
   else:raise AssertionError('Live gate dependency accepted: '+label)
 for name,digest in hashes.items():assert hashlib.sha256((inputs/name).read_bytes()).hexdigest()==digest,name
 r.update(inputs=hashes,rejected_controls=controls);(out/'results.json').write_text(json.dumps(r,indent=2)+'\n');print(r['status'],len(r['boundaries']),'boundaries;',len(controls),'live-dependency controls rejected')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);run(p.parse_args().output.resolve())
