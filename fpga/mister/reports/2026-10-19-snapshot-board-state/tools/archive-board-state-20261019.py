#!/usr/bin/env python3
"""Archive board-only transport qualification without generated simulator objects."""
import gzip
import hashlib
import json
from pathlib import Path

ROOT = Path('/Users/alans/dev2/Dorado')
OUT = ROOT/'fpga/mister/reports/2026-10-19-snapshot-board-state'
OUT.mkdir(exist_ok=False)
records = []


def save(source, name):
    raw = source.read_bytes()
    zipped = source.suffix in {'.sv', '.v', '.vhd', '.vh', '.tcl', '.qsf', '.sdc', '.qip', '.qpf',
                               '.log', '.txt', '.rpt', '.mem', '.patch', '.pin'} or (
                                   source.suffix == '.json' and len(raw) > 100*1024)
    path = OUT/(name + ('.gz' if zipped else ''))
    path.parent.mkdir(parents=True, exist_ok=True)
    assert not path.exists(), name
    path.write_bytes(gzip.compress(raw, mtime=0) if zipped else raw)
    recovered = gzip.decompress(path.read_bytes()) if zipped else path.read_bytes()
    assert recovered == raw
    records.append(dict(source=str(source.relative_to(ROOT)), raw_name=name,
                        stored=str(path.relative_to(OUT)), raw_bytes=len(raw),
                        raw_sha256=hashlib.sha256(raw).hexdigest()))


for p in (ROOT/'build/rtl-executive').glob('snapshot-board-state-v*20261019*'):
    if p.is_dir():
        for q in p.rglob('*'):
            if q.is_file() and q.suffix != '.vvp':
                save(q, 'simulation/'+p.name+'/'+str(q.relative_to(p)))
    elif p.is_file():
        save(p, 'simulation/'+p.name)
for name in ('board_state_fabric.sv', 'board_state_uio.sv', 'test_board_state.py',
             'test_board_state.sv', 'build_enabled_core.py', 'audit_enabled_core.py'):
    save(ROOT/'fpga/mister/snapshot'/name, 'tools/'+name)
save(Path(__file__), 'tools/'+Path(__file__).name)
for p in (ROOT/'build/rtl-executive/enabled60-assets-20261019').iterdir():
    if p.is_file():
        save(p, 'prepared-assets/'+p.name)
baseline = ROOT/'build/rtl-executive/native59-snapshot-budget-baseline'
candidate = ROOT/'build/rtl-executive/enabled60-source-check-20261019'
for name, tree in [('baseline', baseline), ('candidate', candidate)]:
    for p in (tree/'core').rglob('*'):
        if p.is_file():
            save(p, 'local-integration/'+name+'/'+str(p.relative_to(tree)))
    for path in ('src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v',
                 'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'):
        save(tree/path, 'local-integration/'+name+'/'+path)
save(candidate/'enabled-integration.json', 'local-integration/enabled-integration.json')
for p in (ROOT/'build/rtl-executive').glob('enabled60-source-audit-v*-20261019.json'):
    save(p, 'local-integration/'+p.name)
for p in (ROOT/'build/rtl-executive/enabled60-remote-review-20261019').iterdir():
    if p.is_file():
        save(p, 'actual-build-review/'+p.name)
save(ROOT/'fpga/mister/reports/2026-10-19-native59-dsketh/compiled-inputs.json',
     'baseline-compiled-inputs.json')
manifest = json.dumps(records, indent=2).encode()+b'\n'
mp = OUT/('archive-manifest.json.gz' if len(manifest) > 100*1024 else 'archive-manifest.json')
mp.write_bytes(gzip.compress(manifest, mtime=0) if mp.suffix == '.gz' else manifest)
for row in records:
    p = OUT/row['stored']
    raw = gzip.decompress(p.read_bytes()) if p.suffix == '.gz' else p.read_bytes()
    assert len(raw) == row['raw_bytes'] and hashlib.sha256(raw).hexdigest() == row['raw_sha256']
readback = dict(status='PASS', raw_files=len(records),
                raw_bytes=sum(x['raw_bytes'] for x in records),
                stored_bytes=sum((OUT/x['stored']).stat().st_size for x in records),
                scope='Lossless readback of every archived raw file; no simulator objects or machine snapshot.')
(OUT/'archive-readback.json').write_text(json.dumps(readback, indent=2)+'\n')
print(json.dumps(readback))
