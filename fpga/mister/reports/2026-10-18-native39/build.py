"""Instrument frozen Native36 DDR baseline with passive instruction counts."""
from pathlib import Path
import hashlib
import json
import shutil
import subprocess

old = Path('/home/alans/mister/Dorado_Native36_20261018')
new = Path('/home/alans/mister/Dorado_Native39_20261018')
payload = Path('/home/alans/mister/native39-input')
assert not new.exists(), 'Inspect existing build before retrying'
new.mkdir()
shutil.copytree(old/'src', new/'src')
shutil.copytree(old/'core', new/'core', ignore=shutil.ignore_patterns(
    'db', 'incremental_db', 'output_files', '*.rpt', '*.summary', '*.log',
    'c5_pin_model_dump.txt'))
for p in (new/'core').rglob('*'):
    if p.is_file() and p.suffix in ('.qsf', '.qip', '.sdc', '.tcl', '.qpf'):
        p.write_text(p.read_text().replace(str(old), str(new)))
for filename in ['Dorado.sv', 'dorado_instruction_monitor.sv']:
    shutil.copyfile(payload/filename, new/'src/fpga/mister'/filename)
shutil.copyfile(payload/'Dorado.sv', new/'core/Dorado.sv')
checkpoint = new/'src/build/rtl-executive/native16-tx-cable'


def replace(path, old_text, new_text, count=1):
    text = path.read_text()
    assert text.count(old_text) == count, (path, old_text, text.count(old_text))
    path.write_text(text.replace(old_text, new_text))


# Only passive ports and one assignment are added to the frozen board.
# Do not regenerate unrelated clock/cell specializations in this baseline.
p = checkpoint/'ContA_monitor.v'
replace(p, '    output wire [31:0] processor_debug,',
        '    output wire [4:0] instruction_signals,\n'
        '    output wire [31:0] processor_debug,')
replace(p, '  // ---- packages',
        "  assign instruction_signals = {StartCycle, RepeatCur, clk2_p_Be, ~Run_p_, Jam};\n"
        '  // ---- packages')
p = checkpoint/'dorado_native_backplane.v'
replace(p, '    output wire [31:0] processor_debug,',
        '    output wire [4:0] instruction_signals,\n'
        '    output wire [31:0] processor_debug,', count=2)
replace(p, '    .processor_debug(processor_debug),',
        '    .instruction_signals(instruction_signals),\n'
        '    .processor_debug(processor_debug),', count=2)
p = checkpoint/'dorado_native_machine.sv'
replace(p, '    output wire [31:0] baseboard_debug,processor_debug,',
        '    output wire [4:0] instruction_signals,\n'
        '    output wire [31:0] baseboard_debug,processor_debug,')
replace(p, '    .processor_debug(processor_debug),',
        '    .instruction_signals(instruction_signals),\n'
        '    .processor_debug(processor_debug),')
p = checkpoint/'manifest.json'
manifest = json.loads(p.read_text())
manifest.update(storage_backend='ddr', storage_line_cache_bits=7,
                instruction_monitor=True)
p.write_text(json.dumps(manifest, indent=2)+'\n')
p = new/'core/Dorado.qsf'
with p.open('a') as f:
    f.write('\nset_global_assignment -name VERILOG_MACRO DORADO_INSTRUCTION_MONITOR\n')
    f.write('set_global_assignment -name SYSTEMVERILOG_FILE '
            '../src/fpga/mister/dorado_instruction_monitor.sv\n')
files = [new/'core/Dorado.sv', new/'core/Dorado.qsf',
         new/'src/fpga/mister/dorado_instruction_monitor.sv',
         checkpoint/'ContA_monitor.v', checkpoint/'dorado_native_backplane.v',
         checkpoint/'dorado_native_machine.sv']
(new/'instruction-experiment.json').write_text(json.dumps(dict(
    baseline=str(old), change='Passive instruction issue/held/clock monitor and Main command70 views',
    frequency_mhz=50, sysper=4, raw_line_cache_entries=128,
    extra_timing_exceptions='None',
    source_hashes={str(p.relative_to(new)):hashlib.sha256(p.read_bytes()).hexdigest()
                   for p in files}), indent=2)+'\n')
q = Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe, args, name in [
    ('quartus_map', ['Dorado'], 'map'), ('quartus_fit', ['Dorado'], 'fit'),
    ('quartus_asm', ['Dorado'], 'asm'), ('quartus_sta', ['Dorado'], 'sta'),
    ('quartus_sta', ['-t', 'audit_native_clocks.tcl'], 'audit'),
]:
    print('Native39', name, flush=True)
    with (new/(name+'.log')).open('w') as f:
        subprocess.run([str(q/exe), *args], cwd=new/'core',
                       stdout=f, stderr=subprocess.STDOUT, check=True)
print('Native39 build complete; all-condition timing and hardware gates remain', flush=True)
