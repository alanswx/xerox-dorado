from pathlib import Path
import shutil,subprocess,hashlib,json
base=Path('/home/alans/mister/Dorado_Native35_20261018');payload=Path('/home/alans/mister/native35-input')
assert (base/'map.log').exists() and 'Error (10170)' in (base/'map.log').read_text(errors='replace')
(base/'map.log').rename(base/'map-failed-inline-genvar.log')
shutil.copyfile(payload/'dorado_speed_monitor.sv',base/'src/fpga/mister/dorado_speed_monitor.sv')
shutil.copyfile(payload/'Dorado.sv',base/'core/Dorado.sv')
(base/'inputs-after-syntax-fix.json').write_text(json.dumps({n:hashlib.sha256((payload/n).read_bytes()).hexdigest() for n in ['Dorado.sv','dorado_speed_monitor.sv','dorado_storage_ddram.sv','dorado_pack_hps.sv']},indent=2))
assert '.TRACK_CAUSES(1)' in (base/'core/Dorado.sv').read_text()
q=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe,args,name in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),('quartus_sta',['-t','audit_native_clocks.tcl'],'audit')]:
 print('Native35',name,flush=True)
 with (base/(name+'.log')).open('w') as f:subprocess.run([str(q/exe),*args],cwd=base/'core',stdout=f,stderr=subprocess.STDOUT,check=True)
print('Native35 fitted; not deployed or qualified yet',flush=True)
