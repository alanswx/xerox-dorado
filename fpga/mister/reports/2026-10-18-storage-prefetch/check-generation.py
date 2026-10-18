from pathlib import Path
import hashlib,json,subprocess,sys,os
root=Path.cwd();base=root/'build/rtl-executive/storage-prefetch'
old=base/'sil_fpga_native-before.py';old.write_bytes(subprocess.check_output(['git','show','ffd7e70f:tools/sil_fpga_native.py']))
env=os.environ.copy();env['PYTHONPATH']=str(root/'tools')
results={}
for backend in ('ddr','sdram'):
 out=base/('generated-'+backend)
 args=['-o',str(out),'--storage-backend',backend,'--storage-modules','4','--storage-chip-kbits','64','--map-kwords','64','--receive-monitor','--instruction-monitor','--pilot-disk']
 with (base/('generation-'+backend+'.log')).open('w')as log:
  def run(script,flags=[]):subprocess.run([sys.executable,str(script),*args,*flags],stdout=log,stderr=subprocess.STDOUT,env=env,check=True)
  def contents():return {str(p.relative_to(out)):p.read_bytes()for p in out.rglob('*')if p.is_file()}
  run(old);previous=contents()
  run(root/'tools/sil_fpga_native.py');default=contents()
  assert previous==default,(backend,[n for n in previous if previous[n]!=default.get(n)])
  run(root/'tools/sil_fpga_native.py',['--storage-prefetch']);candidate=contents()
  changed=[n for n in candidate if candidate[n]!=default.get(n)]
  assert sorted(changed)==['dorado_native_machine.sv','manifest.json'],changed
  assert candidate['dorado_native_machine.sv'].replace(b',.PREFETCH_NEXT(1)',b'')==default['dorado_native_machine.sv']
  manifest=json.loads(candidate['manifest.json']);assert manifest.pop('storage_prefetch')is True and manifest==json.loads(default['manifest.json'])
  subprocess.run(['verilator','--lint-only','--top-module','dorado_native_machine','-Wno-fatal','-Wno-TIMESCALEMOD','-Wno-PINMISSING','-Iverilog/cells','-Iverilog/generated','+define+SYSPER_OVERRIDE=4','+define+DORADO_RX_MONITOR','+define+DORADO_F10145_LOGIC','+define+DORADO_F10415_M10K','+define+DORADO_HOST_CLOCK_HZ=50000000','-f',str(out/'sources.f')],stdout=log,stderr=subprocess.STDOUT,check=True)
  results[backend]=dict(default_byte_identical_files=len(default),opt_in_changed_files=changed,native_wrapper_lint='PASS',wrapper_sha256=hashlib.sha256(candidate['dorado_native_machine.sv']).hexdigest())
print(json.dumps(results,indent=2));(base/'generation-results.json').write_text(json.dumps(results,indent=2)+'\n')
