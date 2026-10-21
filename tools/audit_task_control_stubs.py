#!/usr/bin/env python3
"""Prove inactive Control-A fixture bindings enter only unconsumed gate outputs.

Physical packages include unrelated gates. A constant boundary is harmless to
this component only when every output affected by its exact pin function is
unwired or has no selected consumer/observation. This does not prove the signal
inactive in the complete machine: original logic outside the component remains
required. Reject a boundary reaching any selected sequential or logic input.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import sys

from generate_task_capture_reference import specs as task_specs
from generate_task_ready_capture_reference import specs as ready_specs
from generate_r_front_reference import specs as front_specs
from generate_run_control_reference import model_ports
from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def pin_dependencies(root,kind):
 raw=(root/'verilog/cells'/('cell_'+kind+'.v')).read_text()
 ports=model_ports(root,kind);definitions={}
 for name,expr in re.findall(r'\b(?:assign|wire)\s+([A-Za-z_]\w*)\s*=\s*([^;]+);',raw):
  assert name not in definitions,(kind,name);definitions[name]=expr
 def expand(name,seen):
  if re.fullmatch('p[0-9]+',name)and ports.get(int(name[1:]))=='input':return {int(name[1:])}
  assert name not in seen and name in definitions,(kind,'unresolved/cyclic expression',name)
  expr=re.sub(r"\b[0-9]+'[sS]?[bBoOdDhH][0-9a-fA-FxXzZ_]+",'',definitions[name])
  result=set()
  for n in re.findall(r'\b[A-Za-z_]\w*\b',expr):result |= expand(n,seen|{name})
  return result
 return {pin:expand('p'+str(pin),set())for pin,direction in ports.items()if direction=='output'}


def audit(root,plans):
 board_path='chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl';b=load_board(str(root/board_path))
 pin_net={(p['pkg'],p['pin']):n for n,row in b.nets.items()for p in row['pins']}
 dependencies={};rows=[];cells=set()
 for label,plan in plans:
  chosen=set(plan['comb'])|set(plan['seq']);observed=set(plan['clock_taps'])|set(plan['flag_taps'])
  constants={n:v for n,v in plan['bindings'].items()if v in ("1'b0","1'b1")}
  assert len(constants)==12,(label,'unexpected constant boundary coverage',constants)
  entries=[]
  for net,value in sorted(constants.items()):
   consumers=[p for p in b.nets[net]['pins']if p['dir']=='in'and p['pkg']in chosen]
   assert consumers,(label,'empty boundary',net)
   effects=[]
   for p in consumers:
    assert p['pkg']in plan['comb'],(label,net,'direct sequential consumer',p)
    kind=b.packages[p['pkg']]['type'];cells.add('verilog/cells/cell_'+kind+'.v')
    if kind not in dependencies:dependencies[kind]=pin_dependencies(root,kind)
    affected=[pin for pin,inputs in dependencies[kind].items()if p['pin']in inputs]
    assert affected,(kind,'missing input dependency',p)
    for pin in affected:
     output_net=pin_net.get((p['pkg'],pin))
     sinks=[]if output_net is None else [x for x in b.nets[output_net]['pins']if x['dir']=='in'and x['pkg']in chosen]
     assert not sinks,(label,net,'LIVE SELECTED CONSUMER',output_net,sinks)
     assert output_net not in observed and f"@{p['pkg']}.p{pin}"not in observed,(label,net,'OBSERVED OUTPUT',output_net)
     effects.append(dict(package=p['pkg'],kind=kind,input_pin=p['pin'],output_pin=pin,
                         output_net=output_net,selected_sinks=0,observed=False))
   entries.append(dict(boundary=net,binding=value,affected_outputs=effects))
  rows.append(dict(profile=label,boundaries=entries,selected_packages=len(chosen)))
 return dict(status='PASS_UNCONSUMED_CONTROL_STUBS',scope=__doc__,profiles=rows,
             physical_pin_functions_expanded=True,complete_machine_signals_inactive=False),[board_path,*sorted(cells)]


def plans(root):
 front,_,_=front_specs(root)
 return [('task_capture',task_specs()[0]),('task_ready_capture',ready_specs()[0]),('R_front',front[0])]


def run(out):
 record,sources=audit(ROOT,plans(ROOT));out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs'
 for bn in ['ContA-Rev-Cd','ProcH-Rev-Ce','ProcL-Rev-Ci']:
  prefix='chm/sil/'+bn+'.dm!1_/'+bn;sources += [prefix+'.wl',prefix+'.lc']
 # Freeze the imported extraction chain as well as exact wiring/pin functions.
 for module in list(sys.modules.values()):
  file=getattr(module,'__file__',None)
  if file:
   p=Path(file).resolve()
   if p.parent==ROOT/'tools'and p.suffix=='.py':sources.append(str(p.relative_to(ROOT)))
 hashes={}
 for n in sorted(set(sources)):
  p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=sha(p)
 # Meaningful bad-source controls must fail on an actual live sink, not hashes.
 controls=[]
 variants=[('live_cp_to_memory_run','MC10103','assign p2 = (p4 | p5);','assign p2 = (p4 | p5 | p10);'),
           ('getlink_into_stop','MC10195','assign p2  = ~(p9 ^ p5);','assign p2  = ~(p9 ^ p5 ^ p12);'),
           ('midas_into_switch','MC1660','assign p3  = ~a;','assign p3  = ~(a | p10);')]
 for label,kind,a,c in variants:
  d=out/label;shutil.copytree(inputs,d);p=d/'verilog/cells'/('cell_'+kind+'.v');raw=p.read_text();assert raw.count(a)==1
  p.write_text(raw.replace(a,c))
  try:audit(d,plans(ROOT))
  except AssertionError as error:
   assert 'LIVE SELECTED CONSUMER'in str(error),(label,error)
   controls.append(dict(case=label,rejected=True,diagnostic=str(error)))
  else:raise AssertionError('live dependency accepted: '+label)
 observed=plans(ROOT);observed[0][1]['flag_taps'].append('ContA02.sil+1')
 try:audit(ROOT,observed)
 except AssertionError as error:
  assert 'OBSERVED OUTPUT'in str(error),error
  controls.append(dict(case='exported_output',rejected=True,diagnostic=str(error)))
 else:raise AssertionError('observable output accepted')
 for n,h in hashes.items():assert sha(inputs/n)==h,n
 record.update(inputs=hashes,rejected_controls=controls,native_integrated=False)
 (out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
 print(record['status'],'12 boundaries x3 profiles;4 bad controls reject;',len(hashes),'frozen inputs')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
 p.add_argument('--replay',type=Path);a=p.parse_args()
 if a.replay:
  record,_=audit(a.replay.resolve(),plans(a.replay.resolve()));a.output.write_text(json.dumps(record,indent=2)+'\n');print(record['status'])
 else:run(a.output.resolve())
