from pathlib import Path
import hashlib,json,shutil,subprocess
old=Path('/home/alans/mister/Dorado_Native41_20261018')
new=Path('/home/alans/mister/Dorado_Native46_20261018')
payload=Path('/home/alans/mister/native46-input')
assert old.is_dir() and not new.exists(),'Inspect existing build before retrying'
new.mkdir();shutil.copytree(old/'src',new/'src')
shutil.copytree(old/'core',new/'core',ignore=shutil.ignore_patterns('db','incremental_db','output_files','*.rpt','*.summary','*.log','c5_pin_model_dump.txt'))
for p in (new/'core').rglob('*'):
 if p.is_file() and p.suffix in ('.qsf','.qip','.sdc','.tcl','.qpf'):
  p.write_text(p.read_text().replace(str(old),str(new)))
for name in ['Dorado.sv','dorado_ethernet_uio.sv','dorado_ethernet_ddr.sv']:
 shutil.copyfile(payload/name,new/'src/fpga/mister'/name)
shutil.copyfile(payload/'Dorado.sv',new/'core/Dorado.sv')
cp=new/'src/build/rtl-executive/native16-tx-cable'
m=json.loads((cp/'manifest.json').read_text())
assert m['native'] and m['storage_backend']=='sdram' and not m['storage_line_cache_bits'] and m['ethernet_wire']
qsf=new/'core/Dorado.qsf'
with qsf.open('a')as f:
 f.write('\nset_global_assignment -name VERILOG_MACRO DORADO_ETHERNET_DDR\n')
 f.write('set_global_assignment -name SYSTEMVERILOG_FILE ../src/fpga/mister/dorado_ethernet_ddr.sv\n')
# All architectural board/cell/raw-storage inputs stay frozen at41. Only
# wrapper/UIO and the added frame engine may differ in the copied source tree.
changes=[];added=[]
for p in sorted((new/'src').rglob('*')):
 if not p.is_file():continue
 rel=p.relative_to(new/'src');before=old/'src'/rel
 if not before.exists():added.append(str(rel))
 elif p.read_bytes()!=before.read_bytes():changes.append(str(rel))
assert sorted(changes)==['fpga/mister/Dorado.sv','fpga/mister/dorado_ethernet_uio.sv'],changes
assert added==['fpga/mister/dorado_ethernet_ddr.sv'],added
for name in ['native_timing.sdc','sdram_timing.sdc','checkpoint_timing.sdc','Dorado.sdc']:
 assert (new/'core'/name).read_text().replace(str(new),str(old))==(old/'core'/name).read_text(),name
files=[new/'core/Dorado.sv',qsf,new/'core/native_timing.sdc',new/'core/sdram_timing.sdc']+[new/'src/fpga/mister'/n for n in ['dorado_ethernet_uio.sv','dorado_ethernet_ddr.sv']]
(new/'network-experiment.json').write_text(json.dumps(dict(baseline=str(old),change='Native41 SDRAM baseline plus opt-in direct-DDR frame engine, atomic SPI capability/epoch control and packet-port ownership mux; engine power-up disabled. No host activation or deployment.',frequency_mhz=50,sysper=4,raw_line_cache_entries=0,storage_backend='sdram',cpu_memory_cell_changes=changes,added_sources=added,extra_timing_exceptions='None',source_hashes={str(p.relative_to(new)):hashlib.sha256(p.read_bytes()).hexdigest()for p in files}),indent=2)+'\n')
q=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe,args,name in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),('quartus_sta',['-t','audit_native_clocks.tcl'],'audit')]:
 print('Native46',name,flush=True)
 with (new/(name+'.log')).open('w')as f:subprocess.run([str(q/exe),*args],cwd=new/'core',stdout=f,stderr=subprocess.STDOUT,check=True)
print('Native46 build complete; all-device timing and software lifecycle/hardware gates pending',flush=True)
