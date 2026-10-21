#!/usr/bin/env python3
"""Test the power-on raw-memory diagnostic through BL8 and independent SDRAM pins.

A bounded 14-bit sweep covers all burst lanes, physical row boundaries and both
address slices; only actual hardware will qualify the full 23-bit native sweep.
"""
import argparse,hashlib,json,os,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
FILES=['fpga/mister/dorado_storage_selftest.sv','fpga/mister/dorado_storage_sdram_burst.sv',
 'verilog/verilator/tb_storage_selftest.sv','verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv',
 'tools/test_storage_selftest.py']
def run(out,full_address=False):
 assert not out.exists(), 'Inspect the existing run'
 inputs=out/'inputs';inputs.mkdir(parents=True)
 for name in FILES:
  p=inputs/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/name).read_bytes())
 state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),actions=[],hardware_qualified=False,address_bits=23 if full_address else 14)
 def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
 def execute(command,label,negative=False):
  with (out/(label+'.log')).open('x') as log:
   p=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT,cwd=out)
   row=dict(stage=label,pid=p.pid,command=command,started=time.time());state['actions'].append(row);save()
   code=p.wait();row.update(returncode=code,finished=time.time());save()
  text=(out/(label+'.log')).read_text()
  assert (code!=0 and 'SELFTEST:' in text) if negative else code==0,(label,code,text[-3000:])
 def compile_case(label,source,hz=50000000):
  obj=out/('obj-'+label)
  command=['verilator','--binary','--timing','--autoflush','-j','2','--top-module','tb_storage_selftest',
   '-Wno-fatal','-Wno-TIMESCALEMOD','-Wno-WIDTHTRUNC','-Wno-WIDTHEXPAND','-Wno-REALCVT',
   '-Wno-PINMISSING','-Wno-DEFPARAM','-GADDR_BITS='+str(state['address_bits']),'-GCLOCK_HZ='+str(hz),'--Mdir',str(obj),'-o','test-selftest',
   str(source),*[str(inputs/n) for n in FILES[1:4]]]
  execute(command,label+'-compile');return obj/'test-selftest'
 save()
 try:
  binary=compile_case('positive',inputs/FILES[0])
  for fault in range(1 if full_address else 5):execute([str(binary),'+fault='+str(fault)],'fault-'+str(fault))
  if not full_address:
   # A comparator removed from the synthesized controller must be caught by
   # actual corrupted-return traffic, not by a source-string check.
   source=(inputs/FILES[0]).read_text();old='rsp_data[11:0]!=expected';assert source.count(old)==1
   mutant=inputs/'compare-removed.sv';mutant.write_text(source.replace(old,"1'b0"))
   binary=compile_case('compare-removed',mutant)
   execute([str(binary),'+fault=1'],'compare-removed-test',True)
  state.update(status='PASS_RAW_MEMORY_SELFTEST_PIN_COMPONENT',finished=time.time());save()
 except BaseException:
  state.update(status='TERMINAL_FAILED',finished=time.time());save();raise
 (out/'provenance.json').write_text(json.dumps(dict(scope=__doc__,inputs={str(p.relative_to(inputs)):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs.rglob('*') if p.is_file()}),indent=2)+'\n')
 print(state['status'],flush=True)
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--full-address',action='store_true')
 a=p.parse_args();run(a.output.resolve(),a.full_address)
