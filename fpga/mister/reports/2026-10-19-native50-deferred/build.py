"""Native50: frozen49 plus frozen45 deferred unobserved raw reads; no load."""
from pathlib import Path
import hashlib,json,re,shutil,subprocess
old=Path('/home/alans/mister/Dorado_Native49_20261019')
proven=Path('/home/alans/mister/Dorado_Native45_20261018')
new=Path('/home/alans/mister/Dorado_Native50_20261019')
assert old.is_dir() and proven.is_dir() and not new.exists()
new.mkdir();shutil.copytree(old/'src',new/'src')
shutil.copytree(old/'core',new/'core',ignore=shutil.ignore_patterns('db','incremental_db','output_files','*.rpt','*.summary','*.log','c5_pin_model_dump.txt'))
for p in (new/'core').rglob('*'):
 if p.is_file()and p.suffix in ('.qsf','.qip','.sdc','.tcl','.qpf'):
  p.write_text(p.read_text().replace(str(old),str(new)))
group=Path('fpga/mister/dorado_storage_group.sv')
shutil.copyfile(proven/'src'/group,new/'src'/group)
cp=new/'src/build/rtl-executive/native16-tx-cable'
p=cp/'msa_ddram.v';before=p.read_text()
assert before.count('dorado_storage_group #(')==16 and '.DEFER_READS'not in before
after,count=re.subn(r'(\.WIDTH\(\d+\))(\) u_storage_\d+)',r'\1, .DEFER_READS(1)\2',before)
assert count==16 and after.replace(', .DEFER_READS(1)','')==before
p.write_text(after)
p=cp/'manifest.json';m=json.loads(p.read_text())
assert m['storage_backend']=='sdram'and not m['storage_line_cache_bits']
assert m['storage_prefetch']and m['storage_interleave']and m['raw_layout']=='group-interleaved-v1'
assert not m.get('storage_defer_reads')
m['storage_defer_reads']=True;p.write_text(json.dumps(m,indent=2)+'\n')
changes=[]
for p in (new/'src').rglob('*'):
 if p.is_file()and p.read_bytes()!=(old/'src'/p.relative_to(new/'src')).read_bytes():changes.append(str(p.relative_to(new/'src')))
assert set(changes)=={str(group),'build/rtl-executive/native16-tx-cable/msa_ddram.v','build/rtl-executive/native16-tx-cable/manifest.json'},changes
for name in ('native_timing.sdc','sdram_timing.sdc','checkpoint_timing.sdc','Dorado.sdc','Dorado.qsf','Dorado.sv'):
 assert (new/'core'/name).read_text().replace(str(new),str(old))==(old/'core'/name).read_text(),name
p=new/'core/device-corners.tcl';p.write_text(p.read_text().replace('native49-','native50-'))
(new/'deferred-read-experiment.json').write_text(json.dumps(dict(baseline=str(old),group_source=str(proven/'src'/group),change='Only frozen Native45 deferred unobserved DRAM reads on Native49. Preserve previous-column/read-before-write state, original CPU/cache/map/ECC, write cadence, SDRAM layout, raw arbitration, physical DDR network and all clocks/constraints.',source_changes=changes,source_hashes={name:hashlib.sha256((new/'src'/name).read_bytes()).hexdigest()for name in changes},extra_timing_exceptions='None'),indent=2)+'\n')
q=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe,args,name in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),('quartus_sta',['-t','audit_native_clocks.tcl'],'audit'),('quartus_sta',['-t','device-corners.tcl'],'device-corners')]:
 print('Native50',name,flush=True)
 with(new/(name+'.log')).open('w')as f:subprocess.run([str(q/exe),*args],cwd=new/'core',stdout=f,stderr=subprocess.STDOUT,check=True)
print('Native50 build complete; all-device review and hardware qualification pending',flush=True)
