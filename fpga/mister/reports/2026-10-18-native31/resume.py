from pathlib import Path
import shutil,subprocess
new=Path('/home/alans/mister/Dorado_Native31_20261018')
assert 'generate for(leaf=0;' in (new/'src/fpga/mister/dorado_storage_arb.sv').read_text()
assert not (new/'core/output_files/Dorado.rbf').exists()
shutil.copy2(new/'map.log',new/'map-before-generate-fix.log')
q=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe,args,name in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),('quartus_sta',['-t','audit_native_clocks.tcl'],'audit')]:
 print('Native31 generate-fix',name,flush=True)
 with (new/(name+'.log')).open('w') as f:
  subprocess.run([str(q/exe),*args],cwd=new/'core',stdout=f,stderr=subprocess.STDOUT,check=True)
print('Native31 complete; full new timing review required',flush=True)
