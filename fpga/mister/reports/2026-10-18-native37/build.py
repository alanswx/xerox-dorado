"""Run on misterubuntu after staging the four reviewed candidate inputs.

Clone the exact Native36 inputs so the raw backend is the only functional
change. Payload: Dorado.sv, dorado_native_machine.sv,
dorado_storage_sdram.sv, sdram_timing.sdc. No guest state is imported.
"""
from pathlib import Path
import hashlib
import json
import shutil
import subprocess

old = Path('/home/alans/mister/Dorado_Native36_20261018')
new = Path('/home/alans/mister/Dorado_Native37_20261018')
payload = Path('/home/alans/mister/native37-input')
assert not new.exists(), 'Preserve existing build; inspect before retrying'
new.mkdir()
shutil.copytree(old / 'src', new / 'src')
shutil.copytree(old / 'core', new / 'core', ignore=shutil.ignore_patterns(
    'db', 'incremental_db', 'output_files', '*.rpt', '*.summary', '*.log',
    'c5_pin_model_dump.txt'))
for p in (new / 'core').rglob('*'):
    if p.is_file() and p.suffix in ('.qsf', '.qip', '.sdc', '.tcl', '.qpf'):
        p.write_text(p.read_text().replace(str(old), str(new)))
targets = {
    'Dorado.sv': ['core/Dorado.sv', 'src/fpga/mister/Dorado.sv'],
    'dorado_native_machine.sv': [
        'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'],
    'dorado_storage_sdram.sv': ['src/fpga/mister/dorado_storage_sdram.sv'],
    'sdram_timing.sdc': ['core/sdram_timing.sdc',
                         'src/fpga/mister/sdram_timing.sdc'],
}
for filename, destinations in targets.items():
    for destination in destinations:
        shutil.copyfile(payload / filename, new / destination)
p = new / 'core/Dorado.qsf'
with p.open('a') as f:
    f.write('\nset_global_assignment -name VERILOG_MACRO DORADO_STORAGE_SDRAM\n')
    f.write('set_global_assignment -name SYSTEMVERILOG_FILE '
            '../src/fpga/mister/dorado_storage_sdram.sv\n')
    f.write('set_global_assignment -name SDC_FILE sdram_timing.sdc\n')
    # Framework already requests fast SDRAM input/output/OE registers.
recipe = dict(baseline=str(old), change='Raw DDR backend -> dedicated SDRAM',
              frequency_mhz=50, sysper=4, required_sdram_bytes=128*1024*1024,
              raw_backing_bytes=16*1024*1024, guest_bytes=8*1024*1024,
              raw_line_cache_entries=0, baseline_raw_line_cache_entries=128,
              extra_timing_exceptions='None',
              candidate_hashes={name: hashlib.sha256(
                  (payload / name).read_bytes()).hexdigest() for name in targets})
(new / 'backend-experiment.json').write_text(json.dumps(recipe, indent=2)+'\n')
q = Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe, args, name in [
    ('quartus_map', ['Dorado'], 'map'),
    ('quartus_fit', ['Dorado'], 'fit'),
    ('quartus_asm', ['Dorado'], 'asm'),
    ('quartus_sta', ['Dorado'], 'sta'),
    ('quartus_sta', ['-t', 'audit_native_clocks.tcl'], 'audit'),
]:
    print('Native37', name, flush=True)
    with (new / (name+'.log')).open('w') as f:
        subprocess.run([str(q / exe), *args], cwd=new / 'core',
                       stdout=f, stderr=subprocess.STDOUT, check=True)
print('Native37 build complete; all-condition timing and hardware gates remain',
      flush=True)
