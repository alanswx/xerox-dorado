#!/usr/bin/env python3
"""Test exact native fixed-lane data/diagnostic capture through physical pins.

Use the qualified parent's actual backend and unchanged memory diagnostic,
independent chip model and transaction-order oracle. Test50/66.7MHz, faults,
reversed diagnostic lanes and the rejected late capture. No hardware claim.
"""
import argparse,hashlib,json,os,subprocess,time
from pathlib import Path
from storage_capture_decode import decode_capture,allow_66mhz
ROOT=Path(__file__).resolve().parents[1]
BACKEND='src/fpga/mister/dorado_storage_sdram_burst.sv'
def sha(b):return hashlib.sha256(b).hexdigest()

def run(parent,out,full=False,full_hz=66666667):
 assert not out.exists();out.mkdir(parents=True);inp=out/'inputs';inp.mkdir()
 p=json.loads((parent/'sdram-burst-native.json').read_text())
 b=(parent/BACKEND).read_bytes();assert sha(b)==p['source_hashes'][BACKEND]and p['corrected_sdram_read_capture']
 selftest=(ROOT/'fpga/mister/dorado_storage_selftest.sv').read_bytes();assert sha(selftest)==p['source_hashes']['src/fpga/mister/dorado_storage_selftest.sv']
 (inp/'backend.sv').write_bytes(allow_66mhz(decode_capture(b)));(inp/'selftest.sv').write_bytes(selftest)
 (inp/'tb.sv').write_bytes((ROOT/'verilog/verilator/tb_storage_selftest.sv').read_bytes())
 (inp/'model.sv').write_bytes((ROOT/'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv').read_bytes())
 for n in ['storage_capture_decode.py',Path(__file__).name]:(inp/n).write_bytes((ROOT/'tools'/n).read_bytes())
 proof=dict(scope=__doc__,parent_manifest_sha256=sha((parent/'sdram-burst-native.json').read_bytes()),
  parent_backend_sha256=sha(b),address_bits=23 if full else 14,
  inputs={f.name:sha(f.read_bytes())for f in inp.iterdir()},native_qualified=False,hardware_qualified=False)
 (out/'provenance.json').write_text(json.dumps(proof,indent=2)+'\n')
 s=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),actions=[])
 def save():(out/'run-state.json').write_text(json.dumps(s,indent=2)+'\n')
 def execute(cmd,label,negative=False):
  with(out/(label+'.log')).open('x')as log:
   c=subprocess.Popen(cmd,cwd=inp,stdout=log,stderr=subprocess.STDOUT);a=dict(stage=label,pid=c.pid,command=cmd,started=time.time());s['actions'].append(a);save();rc=c.wait();a.update(returncode=rc,finished=time.time());save()
  t=(out/(label+'.log')).read_text();assert(rc!=0 and 'SELFTEST:'in t)if negative else rc==0,(label,rc,t[-3000:])
 def compile(label,hz,backend):
  obj=out/('obj-'+label);execute(['verilator','+define+DORADO_BURST_OBSERVER','--binary','--timing','--autoflush','-j','2','--top-module','tb_storage_selftest','-Wno-fatal','-Wno-TIMESCALEMOD','-Wno-WIDTHTRUNC','-Wno-WIDTHEXPAND','-Wno-REALCVT','-Wno-PINMISSING','-Wno-DEFPARAM','-GADDR_BITS='+str(proof['address_bits']),'-GCLOCK_HZ='+str(hz),'--Mdir',str(obj),'-o','trace-test',str(inp/'selftest.sv'),str(backend),str(inp/'tb.sv'),str(inp/'model.sv')],label+'-compile');return obj/'trace-test'
 save()
 try:
  for hz in ([full_hz]if full else[50000000,66666667]):
   label='hz'+str(hz);binary=compile(label,hz,inp/'backend.sv')
   for fault in range(1 if full else 5):execute([str(binary),'+fault='+str(fault)],label+'-fault'+str(fault))
   print(label,'all phases/first burst/fault cases PASS',flush=True)
  if not full:
   source=(inp/'backend.sv').read_bytes()
   for label,old,new in [('reversed-trace',b'first_burst_words[12*trace_beat+:12]',b'first_burst_words[12*(7-trace_beat)+:12]'),('late-capture',b'delay_count<=2;state<=READ_WAIT;',b'delay_count<=3;state<=READ_WAIT;')]:
    assert source.count(old)==1;f=out/(label+'.sv');f.write_bytes(source.replace(old,new));binary=compile(label,50000000,f);execute([str(binary),'+fault=0'],label+'-test',True);print(label,'runtime rejected',flush=True)
  s.update(status='PASS_NATIVE_FIXED_CAPTURE_MEMORY_TRACE_PINS',finished=time.time());save()
 except BaseException:s.update(status='TERMINAL_FAILED',finished=time.time());save();raise
 print(s['status'],flush=True)

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--parent',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--full-address',action='store_true');p.add_argument('--full-hz',type=int,choices=[57142857,66666667],default=66666667);a=p.parse_args();run(a.parent.resolve(),a.output.resolve(),a.full_address,a.full_hz)
