#!/usr/bin/env python3
"""Compare an unmodified Micron-authored model with legacy/corrected local CAS timing.

The public mirror changes capacity parameters; this test overrides capacity to
1024 words/bank and tests row0 only. Vendor command/data scheduling is unchanged.
The vendor body is a downloaded private input, not redistributed by the archive.
"""
import argparse,hashlib,json,os,subprocess,time,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
URL='https://raw.githubusercontent.com/juliusbaxter/mor1kx-dev-env/master/bench/verilog/mt48lc16m16a2.v'
def sha(data):return hashlib.sha256(data).hexdigest()
def run(out,vendor,legacy):
 assert not out.exists();inputs=out/'inputs';inputs.mkdir(parents=True)
 files={'vendor/micron-reference.v':vendor.read_bytes(),'legacy-xsds-model.sv':legacy.read_bytes(),
  'corrected-xsds-model.sv':(ROOT/'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv').read_bytes(),
  'tb.sv':(ROOT/'verilog/verilator/tb_sdram_cas_reference.sv').read_bytes(),
  'timescale.v':b'`timescale 1ns/1ps\n','test-defines.v':b'',Path(__file__).name:Path(__file__).read_bytes()}
 assert re.search(rb'Company:\s+Micron Technology',files['vendor/micron-reference.v'])
 assert b'burst.latency        = cas_latency - 1;' in files['legacy-xsds-model.sv']
 assert b'burst.latency        = cas_latency - 2;' in files['corrected-xsds-model.sv']
 for n,data in files.items():
  p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(data)
 provenance=dict(scope=__doc__,vendor_url=URL,inputs={n:sha(b)for n,b in files.items()},vendor_body_redistributed=False)
 (out/'provenance.json').write_text(json.dumps(provenance,indent=2)+'\n')
 state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),actions=[])
 def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
 def execute(command,label,negative=False):
  with (out/(label+'.log')).open('x') as log:
   p=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT,cwd=out);r=dict(stage=label,pid=p.pid,command=command,started=time.time());state['actions'].append(r);save();code=p.wait();r.update(returncode=code,finished=time.time());save()
  text=(out/(label+'.log')).read_text()
  assert (code!=0 and 'CAS_REFERENCE mismatch' in text)if negative else code==0,(label,code,text[-2500:])
  if label.endswith('-test') and not negative:assert 'CAS_REFERENCE PASS CAS2_AND_CAS3' in text and 'delay_ns=26.000' in text and 'delay_ns=46.000' in text
 save()
 try:
  execute(['iverilog','-g2012','-DMICRON_VENDOR','-I',str(inputs),'-s','tb_sdram_cas_reference','-o',str(out/'vendor.vvp'),str(inputs/'vendor/micron-reference.v'),str(inputs/'tb.sv')],'vendor-compile')
  execute(['vvp',str(out/'vendor.vvp')],'vendor-test')
  for label,name,negative in [('legacy-local','legacy-xsds-model.sv',True),('corrected-local','corrected-xsds-model.sv',False)]:
   obj=out/('obj-'+label)
   execute(['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_sdram_cas_reference','--Mdir',str(obj),'-o','test-latency',str(inputs/name),str(inputs/'tb.sv')],label+'-compile')
   execute([str(obj/'test-latency')],label+'-test',negative)
  state.update(status='PASS_CAS_REFERENCE_VENDOR_AND_CORRECTED_LOCAL_LEGACY_REJECTED',finished=time.time());save();print(state['status'],flush=True)
 except BaseException:
  state.update(status='TERMINAL_FAILED',finished=time.time());save();raise
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['output','vendor-model','legacy-model']:p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();run(a.output.resolve(),a.vendor_model.resolve(),a.legacy_model.resolve())
