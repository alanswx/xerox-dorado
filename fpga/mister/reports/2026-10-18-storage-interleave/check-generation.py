from pathlib import Path
import hashlib,json,subprocess,sys,os
root=Path.cwd();base=root/'build/rtl-executive/storage-interleave'
old=base/'sil_fpga_native-before.py';old.write_bytes(subprocess.check_output(['git','show','c83d2261:tools/sil_fpga_native.py']))
env=os.environ.copy();env['PYTHONPATH']=str(root/'tools');results={}
for backend in ('ddr','sdram'):
 out=base/('generated-'+backend)
 args=['-o',str(out),'--storage-backend',backend,'--storage-modules','4','--storage-chip-kbits','64','--map-kwords','64','--receive-monitor','--instruction-monitor','--pilot-disk']
 with (base/('generation-'+backend+'.log')).open('w')as log:
  def run(script,flags=[]):subprocess.run([sys.executable,str(script),*args,*flags],stdout=log,stderr=subprocess.STDOUT,env=env,check=True)
  def contents():return {str(p.relative_to(out)):p.read_bytes()for p in out.rglob('*')if p.is_file()}
  run(old);previous=contents();run(root/'tools/sil_fpga_native.py');default=contents();assert previous==default
  cases=[]
  for prefetch in (False,True):
   flags=['--storage-prefetch']if prefetch else[]
   run(old,flags);control=contents()
   run(root/'tools/sil_fpga_native.py',flags+['--storage-interleave']);candidate=contents()
   changed=[n for n in candidate if candidate[n]!=control.get(n)];assert sorted(changed)==['dorado_native_machine.sv','manifest.json'],changed
   assert candidate['dorado_native_machine.sv'].replace(b',.INTERLEAVE_GROUPS(1)',b'')==control['dorado_native_machine.sv']
   m=json.loads(candidate['manifest.json']);assert m.pop('storage_interleave')is True and m.pop('raw_layout')=='group-interleaved-v1' and m==json.loads(control['manifest.json'])
   subprocess.run(['verilator','--lint-only','--top-module','dorado_native_machine','-Wno-fatal','-Wno-TIMESCALEMOD','-Wno-PINMISSING','-Iverilog/cells','-Iverilog/generated','+define+SYSPER_OVERRIDE=4','+define+DORADO_RX_MONITOR','+define+DORADO_F10145_LOGIC','+define+DORADO_F10415_M10K','+define+DORADO_HOST_CLOCK_HZ=50000000','-f',str(out/'sources.f')],stdout=log,stderr=subprocess.STDOUT,check=True)
   cases.append(dict(prefetch=prefetch,changed=changed,lint='PASS',wrapper_sha256=hashlib.sha256(candidate['dorado_native_machine.sv']).hexdigest()))
  results[backend]=dict(default_byte_identical_files=len(default),interleave=cases)
(base/'generation-results.json').write_text(json.dumps(results,indent=2)+'\n');print(json.dumps(results,indent=2))
