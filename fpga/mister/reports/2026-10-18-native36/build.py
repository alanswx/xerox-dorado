from pathlib import Path
import shutil,subprocess,hashlib,json
old=Path('/home/alans/mister/Dorado_Native35_20261018');new=Path('/home/alans/mister/Dorado_Native36_20261018');payload=Path('/home/alans/mister/native35-input')
assert not new.exists();new.mkdir()
shutil.copytree(old/'src',new/'src')
shutil.copytree(old/'core',new/'core',ignore=shutil.ignore_patterns('db','incremental_db','output_files','*.rpt','*.summary','*.log','c5_pin_model_dump.txt'))
for p in (new/'core').rglob('*'):
 if p.is_file() and p.suffix in ('.qsf','.qip','.sdc','.tcl','.qpf'):p.write_text(p.read_text().replace(str(old),str(new)))
shutil.copyfile(payload/'cell_F10415A.v',new/'src/verilog/cells/cell_F10415A.v')
p=new/'core/Dorado.qsf';p.write_text(p.read_text()+'\nset_global_assignment -name VERILOG_MACRO DORADO_F10415_M10K\n')
(new/'placement-experiment.json').write_text(json.dumps(dict(baseline=str(old),change='F10415A ramstyle only, MLAB -> M10K; same registered read/write logic',cell_sha256=hashlib.sha256((payload/'cell_F10415A.v').read_bytes()).hexdigest(),frequency_mhz=50,sysper=4),indent=2))
q=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe,args,name in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),('quartus_sta',['-t','audit_native_clocks.tcl'],'audit')]:
 print('Native36',name,flush=True)
 with (new/(name+'.log')).open('w') as f:subprocess.run([str(q/exe),*args],cwd=new/'core',stdout=f,stderr=subprocess.STDOUT,check=True)
print('Native36 placement fit complete; not timing/hardware-qualified',flush=True)
