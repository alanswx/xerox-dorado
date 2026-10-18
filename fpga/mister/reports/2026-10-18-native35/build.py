from pathlib import Path
import shutil,subprocess,hashlib,json,difflib
old=Path('/home/alans/mister/Dorado_Native34_20261018')
new=Path('/home/alans/mister/Dorado_Native35_20261018')
payload=Path('/home/alans/mister/native35-input')
assert not new.exists(),'Do not replace an existing candidate'
new.mkdir()
shutil.copytree(old/'src',new/'src')
shutil.copytree(old/'core',new/'core',ignore=shutil.ignore_patterns('db','incremental_db','output_files','*.rpt','*.summary','*.log','c5_pin_model_dump.txt'))
for p in (new/'core').rglob('*'):
 if p.is_file() and p.suffix in ('.qsf','.qip','.sdc','.tcl','.qpf'):
  p.write_text(p.read_text().replace(str(old),str(new)))
for name in ['Dorado.sv','dorado_speed_monitor.sv','dorado_storage_ddram.sv','dorado_pack_hps.sv']:
 shutil.copyfile(payload/name,new/'src/fpga/mister'/name)
shutil.copyfile(payload/'Dorado.sv',new/'core/Dorado.sv')
p=new/'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
before=p.read_text();s=before
edits=[
('    output wire [31:0] pack_fault_debug,pack_fault_extra,',
 '    output wire [31:0] pack_fault_debug,pack_fault_extra,\n    output wire [2:0] pack_host_phase,\n    output wire [31:0] pack_reads,pack_writes,\n    output wire [8:0] storage_events,'),
('    .fault_debug(pack_fault_debug),.fault_extra(pack_fault_extra),',
 '    .fault_debug(pack_fault_debug),.fault_extra(pack_fault_extra),\n    .host_phase(pack_host_phase),.reads(pack_reads),.writes(pack_writes),'),
('  wire backend_ready,backend_response;',
 '  wire backend_ready,backend_response;\n  wire backing_cache_hit;\n  assign storage_events={backing_cache_hit,(sv && sr),(rv && !rr),(ddram_busy && (ddram_read || ddram_write)),ddram_rvalid,(ddram_write && !ddram_busy),(ddram_read && !ddram_busy),(rv && rr && rw),(rv && rr && !rw)};'),
('    .ddram_rdata(ddram_rdata),.ddram_rvalid(ddram_rvalid));',
 '    .cache_hit(backing_cache_hit),\n    .ddram_rdata(ddram_rdata),.ddram_rvalid(ddram_rvalid));')]
for a,b in edits:
 assert s.count(a)==1,(a,s.count(a))
 s=s.replace(a,b)
p.write_text(s)
(new/'wrapper-observation.diff').write_text(''.join(difflib.unified_diff(before.splitlines(True),s.splitlines(True),fromfile='Native34',tofile='Native35')))
assert '.LINE_CACHE_BITS(7)' in s and '.GROUP_ADDR_BITS(16)' in s
assert 'output_clock_frequency0("50.000000 MHz")' in (new/'core/rtl/pll/pll_0002.v').read_text()
(new/'inputs.json').write_text(json.dumps({name:hashlib.sha256((payload/name).read_bytes()).hexdigest() for name in ['Dorado.sv','dorado_speed_monitor.sv','dorado_storage_ddram.sv','dorado_pack_hps.sv']},indent=2))
q=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
print('Native35: Native34 data paths/PLL/watchdog/SDC, passive pause/transaction/ACK-phase counters only; full fresh timing audit required.',flush=True)
for exe,args,name in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),('quartus_sta',['-t','audit_native_clocks.tcl'],'audit')]:
 print('Native35',name,flush=True)
 with (new/(name+'.log')).open('w') as f:
  subprocess.run([str(q/exe),*args],cwd=new/'core',stdout=f,stderr=subprocess.STDOUT,check=True)
print('Native35 fitted; not deployed or timing-qualified yet.',flush=True)
