#!/usr/bin/env python3
"""Cross-check native group/row/column temporal equivalence in two simulators."""
import argparse,hashlib,json,os,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def run(component,out):
 assert not out.exists();out.mkdir(parents=True);inputs=out/'inputs';inputs.mkdir()
 p=json.loads((component/'provenance.json').read_text())
 mapping={'group.sv':'dorado_storage_group.sv','reference.sv':'original_storage_group.sv','addr.v':'cells/cell_MK4096P_addr.v','preview.sv':'addr_preview.sv'}
 for dst,src in mapping.items():
  b=(component/'inputs'/src).read_bytes();assert hashlib.sha256(b).hexdigest()==p['inputs'][src]
  if dst=='reference.sv':b=b.replace(b'module dorado_storage_group #',b'module reference_storage_group #')
  (inputs/dst).write_bytes(b)
 (inputs/'tb.sv').write_bytes((ROOT/'verilog/verilator/tb_storage_ready_prediction.sv').read_bytes())
 (inputs/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
 proof=dict(scope=__doc__,component_provenance_sha256=hashlib.sha256((component/'provenance.json').read_bytes()).hexdigest(),inputs={f.name:hashlib.sha256(f.read_bytes()).hexdigest()for f in inputs.iterdir()},power_on='Explicit zero row/column/delayed-strobe fixture, same original and preview storage',native_qualified=False)
 (out/'provenance.json').write_text(json.dumps(proof,indent=2)+'\n')
 state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),actions=[])
 def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
 def execute(cmd,label,negative=False):
  with(out/(label+'.log')).open('x')as log:
   c=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);r=dict(stage=label,pid=c.pid,command=cmd,started=time.time());state['actions'].append(r);save();rc=c.wait();r.update(returncode=rc,finished=time.time());save()
  t=(out/(label+'.log')).read_text()
  assert (rc!=0 and 'STORAGE_READY_PREDICTION' in t)if negative else rc==0,(label,rc,t[-2500:])
  if label.endswith('-test') and not negative:assert 'STORAGE_READY_GROUP PASS' in t
 def case(engine,width,negative=False):
  label=engine+'-w'+str(width)+('-stale'if negative else'');group=inputs/'group.sv'
  if negative:
   b=group.read_bytes();old=b'machine_step ? ready_advance_next : ready_hold_next';assert b.count(old)==1
   group=out/(label+'.sv');group.write_bytes(b.replace(old,b'ready_reference'))
  files=[str(group)]+[str(inputs/n)for n in ['reference.sv','addr.v','preview.sv','tb.sv']]
  if engine=='icarus':
   binary=out/(label+'.vvp');execute(['iverilog','-g2012','-s','tb_storage_ready_prediction','-Ptb_storage_ready_prediction.WIDTH='+str(width),'-o',str(binary),*files],label+'-compile');execute(['vvp',str(binary)],label+'-test',negative)
  else:
   obj=out/('obj-'+label);execute(['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_storage_ready_prediction','-GWIDTH='+str(width),'--Mdir',str(obj),'-o','group-test',*files],label+'-compile');execute([str(obj/'group-test')],label+'-test',negative)
  print(label,'runtime rejected'if negative else'PASS',flush=True)
 save()
 try:
  for engine in ['icarus','verilator']:
   for width in [8,12]:case(engine,width)
   case(engine,12,True)
  state.update(status='PASS_READY_GROUP_TWO_SIMULATORS',finished=time.time());save()
 except BaseException:state.update(status='TERMINAL_FAILED',finished=time.time());save();raise
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['component','output']:p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();run(a.component.resolve(),a.output.resolve())
