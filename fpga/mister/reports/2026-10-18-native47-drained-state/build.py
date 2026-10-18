from pathlib import Path
import hashlib,json,shutil,subprocess
old=Path('/home/alans/mister/Dorado_Native46_20261018');new=Path('/home/alans/mister/Dorado_Native47_20261018');payload=Path('/home/alans/mister/native47-input')
assert old.is_dir() and not new.exists(),'Inspect any existing47 build before retry'
new.mkdir();shutil.copytree(old/'src',new/'src')
shutil.copytree(old/'core',new/'core',ignore=shutil.ignore_patterns('db','incremental_db','output_files','*.rpt','*.summary','*.log','c5_pin_model_dump.txt'))
for p in (new/'core').rglob('*'):
 if p.is_file() and p.suffix in ('.qsf','.qip','.sdc','.tcl','.qpf'):p.write_text(p.read_text().replace(str(old),str(new)))
shutil.copyfile(payload/'dorado_ethernet_ddr.sv',new/'src/fpga/mister/dorado_ethernet_ddr.sv')
changes=[]
for p in (new/'src').rglob('*'):
 if p.is_file() and p.read_bytes()!=(old/'src'/p.relative_to(new/'src')).read_bytes():changes.append(str(p.relative_to(new/'src')))
assert changes==['fpga/mister/dorado_ethernet_ddr.sv'],changes
for name in ('native_timing.sdc','sdram_timing.sdc','checkpoint_timing.sdc','Dorado.sdc','Dorado.qsf'):
 assert (new/'core'/name).read_text().replace(str(new),str(old))==(old/'core'/name).read_text(),name
p=new/'core/device-corners.tcl';p.write_text(p.read_text().replace('native46-', 'native47-'))
(new/'drained-state-experiment.json').write_text(json.dumps(dict(baseline=str(old),change='Native46 plus stopped decoded from existing drained STOPPED state; remove separate initialized-high flag; no other functional source, clocks, placement or timing-constraint changes',source_changes=changes,extra_timing_exceptions='None'),indent=2)+'\n')
q=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe,args,name in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),('quartus_sta',['-t','audit_native_clocks.tcl'],'audit'),('quartus_sta',['-t','device-corners.tcl'],'device-corners')]:
 print('Native47',name,flush=True)
 with (new/(name+'.log')).open('w')as f:subprocess.run([str(q/exe),*args],cwd=new/'core',stdout=f,stderr=subprocess.STDOUT,check=True)
print('Native47 compilation/corner audit complete; collect/validate before deployment',flush=True)
