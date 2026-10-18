"""Isolate response-boundary raw-arbiter selection on the Native41 baseline.

Run on misterubuntu after staging dorado_storage_arb.sv and sil_fpga_native.py
in native43-input. Preserve the qualified live boot; no hardware load here.
"""
from pathlib import Path
import hashlib,json,shutil,subprocess
old=Path('/home/alans/mister/Dorado_Native41_20261018')
new=Path('/home/alans/mister/Dorado_Native43_20261018')
payload=Path('/home/alans/mister/native43-input')
assert not new.exists(), 'Inspect existing build before retrying'
new.mkdir();shutil.copytree(old/'src',new/'src')
shutil.copytree(old/'core',new/'core',ignore=shutil.ignore_patterns('db','incremental_db','output_files','*.rpt','*.summary','*.log','c5_pin_model_dump.txt'))
for p in (new/'core').rglob('*'):
 if p.is_file()and p.suffix in ('.qsf','.qip','.sdc','.tcl','.qpf'):
  p.write_text(p.read_text().replace(str(old),str(new)))
for filename,target in [('dorado_storage_arb.sv','fpga/mister'),('sil_fpga_native.py','tools')]:
 shutil.copyfile(payload/filename,new/'src'/target/filename)
cp=new/'src/build/rtl-executive/native16-tx-cable'
p=cp/'dorado_native_machine.sv';before=p.read_text()
marker='.RAW_ADDR_BITS(RAW_BITS)) u_arb('
assert before.count(marker)==1
after=before.replace(marker,'.RAW_ADDR_BITS(RAW_BITS),.PREFETCH_NEXT(1)) u_arb(')
assert after.replace(',.PREFETCH_NEXT(1)','')==before
p.write_text(after)
p=cp/'manifest.json';m=json.loads(p.read_text());assert m['storage_backend']=='sdram'and not m['storage_line_cache_bits'];m['storage_prefetch']=True;p.write_text(json.dumps(m,indent=2)+'\n')
# No clock, placement, timing-exception or architectural-board edits.
files=[new/'src/fpga/mister/dorado_storage_arb.sv',cp/'dorado_native_machine.sv',cp/'manifest.json',new/'core/Dorado.qsf',new/'core/native_timing.sdc',new/'core/sdram_timing.sdc']
(new/'prefetch-experiment.json').write_text(json.dumps(dict(baseline=str(old),change='Raw arbiter response-boundary successor selection only; one outstanding operation, round robin, same address/data/ECC layout and original chip pipeline',frequency_mhz=50,sysper=4,raw_line_cache_entries=0,extra_timing_exceptions='None',source_hashes={str(p.relative_to(new)):hashlib.sha256(p.read_bytes()).hexdigest()for p in files}),indent=2)+'\n')
q=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe,args,name in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),('quartus_sta',['-t','audit_native_clocks.tcl'],'audit')]:
 print('Native43',name,flush=True)
 with (new/(name+'.log')).open('w')as f:subprocess.run([str(q/exe),*args],cwd=new/'core',stdout=f,stderr=subprocess.STDOUT,check=True)
print('Native43 build complete; all-device timing and hardware gates pending',flush=True)
