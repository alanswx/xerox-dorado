"""Native49: frozen48 plus frozen44 raw physical group/word interleaving.
Run on misterubuntu; isolated build, no hardware programming.
"""
from pathlib import Path
import hashlib, json, shutil, subprocess

old = Path('/home/alans/mister/Dorado_Native48_20261019')
selection = Path('/home/alans/mister/Dorado_Native44_20261018')
new = Path('/home/alans/mister/Dorado_Native49_20261019')
assert old.is_dir() and selection.is_dir() and not new.exists()
new.mkdir()
shutil.copytree(old/'src', new/'src')
shutil.copytree(old/'core', new/'core', ignore=shutil.ignore_patterns(
    'db', 'incremental_db', 'output_files', '*.rpt', '*.summary', '*.log', 'c5_pin_model_dump.txt'))
for p in (new/'core').rglob('*'):
    if p.is_file() and p.suffix in ('.qsf', '.qip', '.sdc', '.tcl', '.qpf'):
        p.write_text(p.read_text().replace(str(old), str(new)))
arb = Path('fpga/mister/dorado_storage_arb.sv')
shutil.copyfile(selection/'src'/arb, new/'src'/arb)
cp = new/'src/build/rtl-executive/native16-tx-cable'
p = cp/'dorado_native_machine.sv'
before = p.read_text()
marker = '.RAW_ADDR_BITS(RAW_BITS),.PREFETCH_NEXT(1)) u_arb('
assert before.count(marker) == 1
after = before.replace(marker, '.RAW_ADDR_BITS(RAW_BITS),.PREFETCH_NEXT(1),.INTERLEAVE_GROUPS(1)) u_arb(')
assert after.replace(',.INTERLEAVE_GROUPS(1)', '') == before
p.write_text(after)
p = cp/'manifest.json'
m = json.loads(p.read_text())
assert m['storage_backend'] == 'sdram' and not m['storage_line_cache_bits']
assert m.get('storage_prefetch') and not m.get('storage_interleave') and not m.get('storage_defer_reads')
m['storage_interleave'] = True
m['raw_layout'] = 'group-interleaved-v1'
p.write_text(json.dumps(m, indent=2)+'\n')
changes = []
for p in (new/'src').rglob('*'):
    if p.is_file() and p.read_bytes() != (old/'src'/p.relative_to(new/'src')).read_bytes():
        changes.append(str(p.relative_to(new/'src')))
expected = [str(arb), 'build/rtl-executive/native16-tx-cable/dorado_native_machine.sv',
            'build/rtl-executive/native16-tx-cable/manifest.json']
assert set(changes) == set(expected), changes
for name in ('native_timing.sdc', 'sdram_timing.sdc', 'checkpoint_timing.sdc', 'Dorado.sdc', 'Dorado.qsf', 'Dorado.sv'):
    assert (new/'core'/name).read_text().replace(str(new), str(old)) == (old/'core'/name).read_text(), name
p = new/'core/device-corners.tcl'
p.write_text(p.read_text().replace('native48-', 'native49-'))
(new/'interleave-experiment.json').write_text(json.dumps(dict(
    baseline=str(old), arbiter_source=str(selection/'src'/arb),
    change='Only frozen Native44 physical raw group/word address permutation on Native48. Same original data/ECC, chip pipeline, SDRAM, no raw cache, successor selection, physical DDR network and clock/timing constraints.',
    source_changes=changes,
    source_hashes={str(p.relative_to(new)): hashlib.sha256(p.read_bytes()).hexdigest()
                   for p in [new/'src'/name for name in changes]},
    extra_timing_exceptions='None'), indent=2)+'\n')
q = Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for exe, args, name in [
    ('quartus_map', ['Dorado'], 'map'), ('quartus_fit', ['Dorado'], 'fit'),
    ('quartus_asm', ['Dorado'], 'asm'), ('quartus_sta', ['Dorado'], 'sta'),
    ('quartus_sta', ['-t', 'audit_native_clocks.tcl'], 'audit'),
    ('quartus_sta', ['-t', 'device-corners.tcl'], 'device-corners')]:
    print('Native49', name, flush=True)
    with (new/(name+'.log')).open('w') as f:
        subprocess.run([str(q/exe), *args], cwd=new/'core', stdout=f, stderr=subprocess.STDOUT, check=True)
print('Native49 build complete; collect/review all-device timing before deployment', flush=True)
