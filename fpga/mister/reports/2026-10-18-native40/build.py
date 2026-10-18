"""Place tiny F10145 files in logic; preserve Native38's SDRAM interface."""
from pathlib import Path
import hashlib
import json
import shutil
import subprocess

old = Path('/home/alans/mister/Dorado_Native38_20261018')
new = Path('/home/alans/mister/Dorado_Native40_20261018')
payload = Path('/home/alans/mister/native40-input')
assert not new.exists(), 'Inspect existing build before retrying'
new.mkdir()
shutil.copytree(old/'src', new/'src')
shutil.copytree(old/'core', new/'core', ignore=shutil.ignore_patterns(
    'db', 'incremental_db', 'output_files', '*.rpt', '*.summary', '*.log',
    'c5_pin_model_dump.txt'))
for p in (new/'core').rglob('*'):
    if p.is_file() and p.suffix in ('.qsf', '.qip', '.sdc', '.tcl', '.qpf'):
        p.write_text(p.read_text().replace(str(old), str(new)))
cell = new/'src/verilog/cells/cell_F10145A.v'
shutil.copyfile(payload/cell.name, cell)
p = new/'core/Dorado.qsf'
p.write_text(p.read_text()+'\nset_global_assignment -name VERILOG_MACRO DORADO_F10145_LOGIC\n')
(new/'placement-experiment.json').write_text(json.dumps(dict(
    baseline=str(old), change='F10145A tiny files: MLAB -> logic, same read/write RTL',
    cell_sha256=hashlib.sha256(cell.read_bytes()).hexdigest(),
    frequency_mhz=50, sysper=4, extra_timing_exceptions='None'), indent=2)+'\n')
q = Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe, args, name in [
    ('quartus_map', ['Dorado'], 'map'), ('quartus_fit', ['Dorado'], 'fit'),
    ('quartus_asm', ['Dorado'], 'asm'), ('quartus_sta', ['Dorado'], 'sta'),
    ('quartus_sta', ['-t', 'audit_native_clocks.tcl'], 'audit'),
]:
    print('Native40', name, flush=True)
    with (new/(name+'.log')).open('w') as f:
        subprocess.run([str(q/exe), *args], cwd=new/'core',
                       stdout=f, stderr=subprocess.STDOUT, check=True)
print('Native40 build complete; all-condition timing and hardware gates remain', flush=True)
