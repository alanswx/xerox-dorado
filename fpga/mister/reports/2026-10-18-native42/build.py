"""Matched cache-off DDR control for Native41; run only on misterubuntu.

Keep Native41's generated boards, logic/M10K placement, clocks, diagnostics
and top level. Replace only raw storage backend wiring and its constraints.
The known Native39 DDR wrapper differs only in the reviewed backend/ports;
its raw line cache is disabled here. No guest state or wider regeneration.
"""
from pathlib import Path
import hashlib,json,shutil,subprocess
base=Path('/home/alans/mister')
old=base/'Dorado_Native41_20261018'
new=base/'Dorado_Native42_20261018'
ddr=base/'Dorado_Native39_20261018'
assert not new.exists(), 'Inspect existing build before retrying'
new.mkdir()
shutil.copytree(old/'src',new/'src')
shutil.copytree(old/'core',new/'core',ignore=shutil.ignore_patterns('db','incremental_db','output_files','*.rpt','*.summary','*.log','c5_pin_model_dump.txt'))
for p in (new/'core').rglob('*'):
 if p.is_file()and p.suffix in ('.qsf','.qip','.sdc','.tcl','.qpf'):
  p.write_text(p.read_text().replace(str(old),str(new)))
cp=Path('src/build/rtl-executive/native16-tx-cable')
a=(ddr/cp/'dorado_native_machine.sv').read_text()
b=(old/cp/'dorado_native_machine.sv').read_text()
# Validate unchanged upstream/frontend and CPU wiring, allowing only SDRAM
# physical ports and backend events/controller wiring to differ.
ports=b[b.index('    output wire [12:0] sdram_a,'):b.index('    input wire ddram_busy,ddram_rvalid,')]
b_noports=b.replace(ports,'')
start='  wire backend_ready,backend_response;'
end='  dorado_native_backplane #(.SYSPER(SYSPER)) m ('
assert a.split(start)[0]==b_noports.split(start)[0]
assert a.split(end)[1]==b.split(end)[1]
assert a.count('.LINE_CACHE_BITS(7)')==1
a=a.replace('.LINE_CACHE_BITS(7)','.LINE_CACHE_BITS(0)')
(new/cp/'dorado_native_machine.sv').write_text(a)
p=new/cp/'manifest.json';m=json.loads(p.read_text());m.update(storage_backend='ddr',storage_line_cache_bits=0);m.pop('required_sdram_bytes',None);p.write_text(json.dumps(m,indent=2)+'\n')
p=new/'core/Dorado.qsf'
removed=[];lines=[]
for line in p.read_text().splitlines():
 if any(t in line for t in ('VERILOG_MACRO DORADO_STORAGE_SDRAM','SDC_FILE sdram_timing.sdc','SYSTEMVERILOG_FILE ../src/fpga/mister/dorado_storage_sdram.sv')):removed.append(line)
 else:lines.append(line)
assert len(removed)==3,removed
p.write_text('\n'.join(lines)+'\n')
# Check fixed clocks, passive diagnostics and placement policies are retained.
assert 'DORADO_F10145_LOGIC' in p.read_text() and 'DORADO_F10415_M10K' in p.read_text()
assert 'DORADO_INSTRUCTION_MONITOR' in p.read_text() and 'DORADO_HOST_CLOCK_HZ=50000000' in p.read_text()
changed=['core/Dorado.qsf',str(cp/'dorado_native_machine.sv'),str(cp/'manifest.json')]
(new/'backend-experiment.json').write_text(json.dumps(dict(baseline=str(old),ddr_wrapper_reference=str(ddr),change='SDRAM -> DDR raw backend, both cache-off; same F10145 logic and F10415 M10K, 50MHz/SYSPER4, original generated CPU/memory boards unchanged',frequency_mhz=50,sysper=4,raw_line_cache_entries=0,extra_timing_exceptions='None',removed_assignments=removed,changed_inputs={n:hashlib.sha256((new/n).read_bytes()).hexdigest()for n in changed}),indent=2)+'\n')
q=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe,args,name in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),('quartus_sta',['-t','audit_native_clocks.tcl'],'audit')]:
 print('Native42',name,flush=True)
 with (new/(name+'.log')).open('w')as f:subprocess.run([str(q/exe),*args],cwd=new/'core',stdout=f,stderr=subprocess.STDOUT,check=True)
print('Native42 build complete; all-device and hardware gates pending',flush=True)
