"""Correct Native37's failed SDRAM input capture, preserving other inputs."""
from pathlib import Path
import hashlib
import json
import shutil
import subprocess

old = Path('/home/alans/mister/Dorado_Native37_20261018')
new = Path('/home/alans/mister/Dorado_Native38_20261018')
payload = Path('/home/alans/mister/native38-input')
assert not new.exists(), 'Inspect existing build before retrying'
new.mkdir()
shutil.copytree(old/'src', new/'src')
shutil.copytree(old/'core', new/'core', ignore=shutil.ignore_patterns(
    'db', 'incremental_db', 'output_files', '*.rpt', '*.summary', '*.log',
    'c5_pin_model_dump.txt'))
for p in (new/'core').rglob('*'):
    if p.is_file() and p.suffix in ('.qsf', '.qip', '.sdc', '.tcl', '.qpf'):
        p.write_text(p.read_text().replace(str(old), str(new)))
source = new/'src/fpga/mister/dorado_storage_sdram.sv'
shutil.copyfile(payload/source.name, source)
checkpoint = new/'src/build/rtl-executive/native16-tx-cable'
p = checkpoint/'manifest.json'
manifest = json.loads(p.read_text())
manifest.update(storage_backend='sdram', storage_line_cache_bits=0,
                required_sdram_bytes=128*1024*1024)
p.write_text(json.dumps(manifest, indent=2)+'\n')
p = checkpoint/'sources.f'
line = 'fpga/mister/dorado_storage_sdram.sv'
if line not in p.read_text().splitlines():
    p.write_text(p.read_text().rstrip()+'\n'+line+'\n')
(new/'capture-experiment.json').write_text(json.dumps(dict(
    baseline=str(old), change='Falling-edge input capture, then rising-edge response; one extra read cycle',
    controller_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),
    frequency_mhz=50, sysper=4, extra_timing_exceptions='None'), indent=2)+'\n')
q = Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe, args, name in [
    ('quartus_map', ['Dorado'], 'map'), ('quartus_fit', ['Dorado'], 'fit'),
    ('quartus_asm', ['Dorado'], 'asm'), ('quartus_sta', ['Dorado'], 'sta'),
    ('quartus_sta', ['-t', 'audit_native_clocks.tcl'], 'audit'),
]:
    print('Native38', name, flush=True)
    with (new/(name+'.log')).open('w') as f:
        subprocess.run([str(q/exe), *args], cwd=new/'core',
                       stdout=f, stderr=subprocess.STDOUT, check=True)
print('Native38 build complete; all-condition timing and hardware gates remain', flush=True)
