from pathlib import Path
import shutil,subprocess,hashlib
old=Path('/home/alans/mister/Dorado_Native31_20261018')
new=Path('/home/alans/mister/Dorado_Native32_20261018')
payload=Path('/home/alans/mister/native32-input')
assert not new.exists(),'Do not replace an existing candidate'
assert hashlib.sha256((old/'core/output_files/Dorado.rbf').read_bytes()).hexdigest()=='ca5572a080cf06655e83497c089a74e6c3c066d47bc9390d44b44b4ddc4d7705'
new.mkdir()
shutil.copytree(old/'src',new/'src')
shutil.copytree(old/'core',new/'core',ignore=shutil.ignore_patterns('db','incremental_db','output_files','*.rpt','*.summary','*.log','c5_pin_model_dump.txt'))
for p in (new/'core').rglob('*'):
 if p.is_file() and p.suffix in ('.qsf','.qip','.sdc','.tcl','.qpf'):
  p.write_text(p.read_text().replace(str(old),str(new)))
shutil.copy2(payload/'dorado_storage_ddram.sv',new/'src/fpga/mister/dorado_storage_ddram.sv')
wrapper=new/'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
s=wrapper.read_text();needle='dorado_storage_ddram #(.ADDR_BITS(RAW_BITS)) u_ddram('
assert s.count(needle)==1
s=s.replace(needle,'dorado_storage_ddram #(.ADDR_BITS(RAW_BITS),.LINE_CACHE_BITS(7),.GROUP_ADDR_BITS(16)) u_ddram(')
assert s== (payload/'dorado_native_machine.sv').read_text(),'Generated wrapper must differ ONLY by backing-port parameters'
wrapper.write_text(s)
assert 'output_clock_frequency0("50.000000 MHz")' in (new/'core/rtl/pll/pll_0002.v').read_text()
print('Native32 EXPERIMENT: Native31 50MHz/SYSPER4 plus128-line raw DDR buffer. Requires fresh STA and full original-firmware/Cedar qualification.',flush=True)
q=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe,args,name in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),('quartus_sta',['-t','audit_native_clocks.tcl'],'audit')]:
 print('Native32',name,flush=True)
 with (new/(name+'.log')).open('w') as f:
  subprocess.run([str(q/exe),*args],cwd=new/'core',stdout=f,stderr=subprocess.STDOUT,check=True)
print('Native32 complete; timing and hardware review required before promotion',flush=True)
