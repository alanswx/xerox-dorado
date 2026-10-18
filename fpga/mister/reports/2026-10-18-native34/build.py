from pathlib import Path
import shutil,subprocess,hashlib
old=Path('/home/alans/mister/Dorado_Native33_20261018')
new=Path('/home/alans/mister/Dorado_Native34_20261018')
payload=Path('/home/alans/mister/native34-input')
assert not new.exists(),'Do not replace an existing candidate'
assert hashlib.sha256((old/'src/fpga/mister/dorado_storage_ddram.sv').read_bytes()).hexdigest()=='fde0fec9c5d8e2340c86e6fe19140531f1d6daa2701993c460ed8a68888b73ed'
new.mkdir()
shutil.copytree(old/'src',new/'src')
shutil.copytree(old/'core',new/'core',ignore=shutil.ignore_patterns('db','incremental_db','output_files','*.rpt','*.summary','*.log','c5_pin_model_dump.txt'))
for p in (new/'core').rglob('*'):
 if p.is_file() and p.suffix in ('.qsf','.qip','.sdc','.tcl','.qpf'):
  p.write_text(p.read_text().replace(str(old),str(new)))
shutil.copy2(payload/'dorado_storage_ddram.sv',new/'src/fpga/mister/dorado_storage_ddram.sv')
shutil.copy2(payload/'dorado_pack_hps.sv',new/'src/fpga/mister/dorado_pack_hps.sv')
qsf=new/'core/Dorado.qsf'
s=qsf.read_text();assert 'DORADO_HOST_CLOCK_HZ=' not in s
qsf.write_text(s+'\nset_global_assignment -name VERILOG_MACRO "DORADO_HOST_CLOCK_HZ=50000000"\n')
(new/'core/dorado_clock_config.json').write_text('{"sys_mhz":"50.000000","sys_hz":50000000,"hps_timeout_seconds":4}\n')
assert 'output_clock_frequency0("50.000000 MHz")' in (new/'core/rtl/pll/pll_0002.v').read_text()
print('Native34: Native33 cache/no-miss-bubble plus PLL-scaled four-second HPS watchdog and ACK-phase first-fault diagnostics. Requires fresh STA and full original-firmware/Cedar qualification.',flush=True)
q=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe,args,name in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),('quartus_sta',['-t','audit_native_clocks.tcl'],'audit')]:
 print('Native34',name,flush=True)
 with (new/(name+'.log')).open('w') as f:
  subprocess.run([str(q/exe),*args],cwd=new/'core',stdout=f,stderr=subprocess.STDOUT,check=True)
print('Native34 complete; timing and hardware review required before promotion',flush=True)
