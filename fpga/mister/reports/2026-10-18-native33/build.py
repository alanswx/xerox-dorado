from pathlib import Path
import shutil,subprocess,hashlib
old=Path('/home/alans/mister/Dorado_Native32_20261018')
new=Path('/home/alans/mister/Dorado_Native33_20261018')
payload=Path('/home/alans/mister/native33-input')
assert not new.exists(),'Do not replace an existing candidate'
assert hashlib.sha256((old/'core/output_files/Dorado.rbf').read_bytes()).hexdigest()=='5747eb2698ea0d8ebbed933b714da6ad29db79730f9eecd16595724aa9c040ea'
new.mkdir()
shutil.copytree(old/'src',new/'src')
shutil.copytree(old/'core',new/'core',ignore=shutil.ignore_patterns('db','incremental_db','output_files','*.rpt','*.summary','*.log','c5_pin_model_dump.txt'))
for p in (new/'core').rglob('*'):
 if p.is_file() and p.suffix in ('.qsf','.qip','.sdc','.tcl','.qpf'):
  p.write_text(p.read_text().replace(str(old),str(new)))
shutil.copy2(payload/'dorado_storage_ddram.sv',new/'src/fpga/mister/dorado_storage_ddram.sv')
assert 'output_clock_frequency0("50.000000 MHz")' in (new/'core/rtl/pll/pll_0002.v').read_text()
print('Native33 EXPERIMENT: Native32 50MHz/SYSPER4 cache, issue cold reads without added lookup latency. Requires fresh STA and full original-firmware/Cedar qualification.',flush=True)
q=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe,args,name in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),('quartus_sta',['-t','audit_native_clocks.tcl'],'audit')]:
 print('Native33',name,flush=True)
 with (new/(name+'.log')).open('w') as f:
  subprocess.run([str(q/exe),*args],cwd=new/'core',stdout=f,stderr=subprocess.STDOUT,check=True)
print('Native33 complete; timing and hardware review required before promotion',flush=True)
