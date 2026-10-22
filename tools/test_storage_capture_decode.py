#!/usr/bin/env python3
"""Exercise fixed-lane BL8 capture with the canonical physical-pin workload.

Run the unmodified canonical matrix in a frozen source tree, with only its
candidate backend transformed. Add66.7MHz delayed-clock cases and an actual
bad cache-lane runtime control. No native timing/firmware/speed claim follows.
"""
import argparse,gzip,hashlib,json,os,subprocess,time
from pathlib import Path
from storage_capture_decode import decode_capture,allow_66mhz
ROOT=Path(__file__).resolve().parents[1]
FILES=['fpga/mister/dorado_storage_sdram.sv','fpga/mister/dorado_storage_sdram_burst.sv',
 'verilog/verilator/tb_storage_sdram_burst.sv','verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv',
 'tools/test_storage_sdram_burst.py','tools/storage_capture_source.py','tools/storage_capture_decode.py','tools/test_storage_capture_decode.py']
def sha(b):return hashlib.sha256(b).hexdigest()

def run(out):
 assert not out.exists(),'Observe existing run';out.mkdir(parents=True);fixture=out/'fixture'
 for n in FILES:
  f=fixture/n;f.parent.mkdir(parents=True,exist_ok=True);b=(ROOT/n).read_bytes()
  if n=='fpga/mister/dorado_storage_sdram_burst.sv':b=allow_66mhz(decode_capture(b))
  f.write_bytes(b)
 proof=dict(scope=__doc__,inputs={str(f.relative_to(fixture)):sha(f.read_bytes())for f in fixture.rglob('*')if f.is_file()},native_qualified=False,hardware_qualified=False)
 (out/'provenance.json').write_text(json.dumps(proof,indent=2)+'\n')
 s=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),actions=[],native_qualified=False,hardware_qualified=False)
 def save():(out/'run-state.json').write_text(json.dumps(s,indent=2)+'\n')
 def execute(cmd,label,negative=False):
  with(out/(label+'.log')).open('x')as log:
   c=subprocess.Popen(cmd,cwd=fixture,stdout=log,stderr=subprocess.STDOUT);a=dict(stage=label,pid=c.pid,command=cmd,started=time.time());s['actions'].append(a);save();rc=c.wait();a.update(returncode=rc,finished=time.time());save()
  t=(out/(label+'.log')).read_text()
  assert(rc!=0 and 'read 'in t.lower())if negative else rc==0,(label,rc,t[-3000:])
 def case(label,hz,delay=0,source=None,negative=False):
  inp=out/'matrix/inputs';obj=out/('obj-'+label)
  cmd=['verilator','+define+DORADO_CORRECT_READ_CAPTURE','--binary','--timing','--autoflush','-j','2','--top-module','tb_storage_sdram_burst','-Wno-fatal','-Wno-TIMESCALEMOD','-Wno-WIDTHTRUNC','-Wno-WIDTHEXPAND','-Wno-REALCVT','-Wno-PINMISSING','-Wno-DEFPARAM','-GCLOCK_HZ='+str(hz),'-GCLOCK_DELAY_NS='+str(delay),'--Mdir',str(obj),'-o','pin-test',str(inp/FILES[0]),str(source or inp/FILES[1]),str(inp/FILES[2]),str(inp/FILES[3])]
  execute(cmd,label+'-compile');execute([str(obj/'pin-test')],label+'-test',negative)
  if not negative:assert 'STORAGE-SDRAM-BURST PASS'in(out/(label+'-test.log')).read_text()
  print(label,'runtime rejected'if negative else'PASS',flush=True)
 save()
 try:
  execute(['python3',str(fixture/'tools/test_storage_sdram_burst.py'),'--output',str(out/'matrix'),'--controls'],'canonical-matrix')
  q=json.loads((out/'matrix/run-state.json').read_text());assert q['status']=='PASS_ISOLATED_SDRAM_BURST_PIN_MODEL'and len(q['actions'])==26
  b=(out/'matrix/inputs'/FILES[1]).read_bytes();assert b==(fixture/FILES[1]).read_bytes()
  case('clock66666667',66666667)
  case('clock66666667-delay6',66666667,6)
  old=b'line_words[capture_line][16*capture_beat+:16]';assert b.count(old)==1
  f=out/'reversed-cache-lanes.sv';f.write_bytes(b.replace(old,b'line_words[capture_line][16*(7-capture_beat)+:16]'))
  case('reversed-cache-lanes',50000000,source=f,negative=True)
  s.update(status='PASS_FIXED_LANE_CAPTURE_PIN_MATRIX_AND_66MHZ',finished=time.time());save()
 except BaseException:s.update(status='TERMINAL_FAILED',finished=time.time());save();raise
 print(s['status'],flush=True)

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.output.resolve())
