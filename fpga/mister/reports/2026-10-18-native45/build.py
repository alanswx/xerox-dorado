"""Isolated Native45: defer unobserved internal DRAM reads, retain original output pipeline.
Run only after component equivalence gates; no hardware load.
"""
from pathlib import Path
import hashlib,json,re,shutil,subprocess
old=Path('/home/alans/mister/Dorado_Native44_20261018')
new=Path('/home/alans/mister/Dorado_Native45_20261018')
payload=Path('/home/alans/mister/native45-input')
assert not new.exists(), 'Inspect existing build before retrying'
new.mkdir();shutil.copytree(old/'src',new/'src')
shutil.copytree(old/'core',new/'core',ignore=shutil.ignore_patterns('db','incremental_db','output_files','*.rpt','*.summary','*.log','c5_pin_model_dump.txt'))
for p in (new/'core').rglob('*'):
 if p.is_file() and p.suffix in ('.qsf','.qip','.sdc','.tcl','.qpf'):
  p.write_text(p.read_text().replace(str(old),str(new)))
for filename,target in [('dorado_storage_group.sv','fpga/mister'),('sil_storage_ddram.py','tools'),('sil_fpga_native.py','tools')]:
 shutil.copyfile(payload/filename,new/'src'/target/filename)
cp=new/'src/build/rtl-executive/native16-tx-cable'
p=cp/'msa_ddram.v';before=p.read_text()
assert before.count('dorado_storage_group #(')==16 and '.DEFER_READS' not in before
after,count=re.subn(r'(\.WIDTH\(\d+\))(\) u_storage_\d+)',r'\1, .DEFER_READS(1)\2',before)
assert count==16 and after.replace(', .DEFER_READS(1)','')==before
p.write_text(after)
p=cp/'manifest.json';m=json.loads(p.read_text())
assert m['storage_backend']=='sdram' and not m['storage_line_cache_bits']
assert m['storage_prefetch'] and m['storage_interleave'] and m['raw_layout']=='group-interleaved-v1'
m['storage_defer_reads']=True;p.write_text(json.dumps(m,indent=2)+'\n')
files=[new/'src/fpga/mister/dorado_storage_group.sv',cp/'msa_ddram.v',cp/'manifest.json',new/'core/Dorado.qsf',new/'core/native_timing.sdc',new/'core/sdram_timing.sdc']
(new/'deferred-read-experiment.json').write_text(json.dumps(dict(baseline=str(old),change='Defer only unobserved unconditional internal chip reads. Preserve previous-column output pipeline and old-before-write values; writes retain original capture/commit cadence. Same Native44 successor/interleaving, SDRAM, no line cache, clocks, architectural boards and timing constraints.',frequency_mhz=50,sysper=4,raw_line_cache_entries=0,extra_timing_exceptions='None',source_hashes={str(p.relative_to(new)):hashlib.sha256(p.read_bytes()).hexdigest() for p in files}),indent=2)+'\n')
q=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe,args,name in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),('quartus_sta',['-t','audit_native_clocks.tcl'],'audit')]:
 print('Native45',name,flush=True)
 with (new/(name+'.log')).open('w')as f:subprocess.run([str(q/exe),*args],cwd=new/'core',stdout=f,stderr=subprocess.STDOUT,check=True)
print('Native45 build complete; all-device timing and hardware gates pending',flush=True)
