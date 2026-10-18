from pathlib import Path
import shutil,subprocess
old=Path('/home/alans/mister/Dorado_Native29_20261018')
new=Path('/home/alans/mister/Dorado_Native30_20261018')
payload=Path('/home/alans/mister/native30-input')
assert not new.exists(),'Do not replace an existing candidate'
new.mkdir()
shutil.copytree(old/'src',new/'src')
shutil.copytree(old/'core',new/'core',ignore=shutil.ignore_patterns('db','incremental_db','output_files','*.rpt','*.summary','*.log','c5_pin_model_dump.txt'))
for p in (new/'core').rglob('*'):
 if p.is_file() and p.suffix in ('.qsf','.qip','.sdc','.tcl','.qpf'):
  p.write_text(p.read_text().replace(str(old),str(new)))
for name in ['Dorado.sv','dorado_ethernet_uio.sv','dorado_speed_monitor.sv']:
 shutil.copy2(payload/name,new/'src/fpga/mister'/name)
shutil.copy2(payload/'Dorado.sv',new/'core/Dorado.sv')
assert 'output_clock_frequency0("40.000000 MHz")' in (new/'core/rtl/pll/pll_0002.v').read_text()
shutil.copy2(new/'core/native26-before-review.sdc',new/'core/native_timing.sdc')
with (new/'core/Dorado.qsf').open('a') as f:
 f.write('\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(new/'src/fpga/mister/dorado_speed_monitor.sv')+'\n')
print('Native30: Native29 40MHz/SYSPER4 plus passive fabric-edge counts and coherent debug snapshots; requires timing/hardware qualification',flush=True)
q=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe,args,name in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),('quartus_sta',['-t','audit_native_clocks.tcl'],'audit')]:
 print('Native30',name,flush=True)
 with (new/(name+'.log')).open('w') as f:
  subprocess.run([str(q/exe),*args],cwd=new/'core',stdout=f,stderr=subprocess.STDOUT,check=True)
print('Native30 fit complete; review required before deployment',flush=True)
