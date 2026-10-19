#!/usr/bin/env python3
"""Lossless shared-write qualification, rejected MLAB trial and revised capacity."""
import gzip
import hashlib
import json
from pathlib import Path

ROOT = Path('/Users/alans/dev2/Dorado')
BUILD = ROOT/'build/rtl-executive'
TOOLS = ROOT/'fpga/mister/snapshot'


class Archive:
    def __init__(self, name):
        self.root = ROOT/'fpga/mister/reports'/name
        self.root.mkdir(exist_ok=False)
        self.rows = []

    def add(self, p, name):
        raw = p.read_bytes()
        zipped = p.suffix not in ('.py', '.json', '.md') or (p.suffix == '.json' and len(raw)>100*1024)
        target = self.root/(name+('.gz' if zipped else ''))
        target.parent.mkdir(parents=True, exist_ok=True)
        assert not target.exists(), name
        target.write_bytes(gzip.compress(raw, mtime=0) if zipped else raw)
        self.rows.append(dict(source=str(p.relative_to(ROOT)), raw_name=name,
            stored=str(target.relative_to(self.root)), raw_bytes=len(raw), raw_sha256=hashlib.sha256(raw).hexdigest()))

    def tree(self, p, name):
        for q in p.rglob('*'):
            if (q.is_file() and not set(q.relative_to(p).parts)&{'obj','obj_xml','db','incremental_db','__pycache__'}
                    and not q.name.startswith('._')):
                self.add(q, name+'/'+str(q.relative_to(p)))

    def finish(self):
        for row in self.rows:
            p = self.root/row['stored']
            raw = gzip.decompress(p.read_bytes()) if p.suffix == '.gz' else p.read_bytes()
            assert len(raw)==row['raw_bytes'] and hashlib.sha256(raw).hexdigest()==row['raw_sha256']
        raw = json.dumps(self.rows, indent=2).encode()+b'\n'
        zipped = len(raw)>100*1024
        (self.root/('archive-manifest.json'+('.gz' if zipped else ''))).write_bytes(gzip.compress(raw, mtime=0) if zipped else raw)
        result = dict(status='PASS', raw_files=len(self.rows), raw_bytes=sum(x['raw_bytes'] for x in self.rows),
                      stored_bytes=sum((self.root/x['stored']).stat().st_size for x in self.rows))
        (self.root/'archive-readback.json').write_text(json.dumps(result, indent=2)+'\n')
        print(self.root.name, json.dumps(result))


capacity = Archive('2026-10-19-snapshot-shared-riot-capacity')
capacity.tree(BUILD/'enabled60-shared-riot-capacity-20261019', 'failed-fit')
capacity.add(BUILD/'enabled60-shared-riot-capacity-review-20261019.json', 'capacity-review.json')
capacity.add(BUILD/'enabled60-shared-riot-actual-source-audit-20261019.json', 'actual-source-audit.json')
capacity.add(BUILD/'enabled60-shared-riot-capacity-executed-audit.py', 'executed-audit.py')
capacity.add(Path(__file__), 'tools/'+Path(__file__).name)
capacity.finish()

fold = Archive('2026-10-19-snapshot-f10145-shared-write')
for name in ('generate_f10145_shared_write.py', 'test_f10145_shared_write.py', 'classify_f10145_shared_write.py',
             'fit_f10145_shared_write.py', 'audit_f10145_shared_write.py', 'test_boards_shared_f10145.py',
             'build_enabled_core.py', 'audit_enabled_core.py', 'audit_enabled_capacity.py'):
    fold.add(TOOLS/name, 'tools/'+name)
fold.add(Path(__file__), 'tools/'+Path(__file__).name)
fold.tree(BUILD/'f10145-shared-write-v1-20261019', 'simulation')
fold.add(BUILD/'f10145-shared-write-v1-20261019.log', 'simulation/driver.log')
fold.tree(BUILD/'f10145-shared-write-classification-v1-20261019', 'classification')
fold.tree(BUILD/'f10145-shared-write-fit-result-20261019/SnapshotF10145SharedWrite_20261019', 'fit')
fold.add(BUILD/'f10145-shared-write-audited-20261019.json', 'fit-audit.json')
external = set()
for version in (1,2,3):
    directory = BUILD/f'boards-shared-f10145-v{version}-20261019'
    fold.tree(directory, f'full-board-v{version}')
    fold.add(directory.with_name(directory.name+'.log'), f'full-board-v{version}/driver.log')
    for p in directory.rglob('command.json'):
        for arg in json.loads(p.read_text()):
            q = Path(arg)
            if q.is_absolute() and q.is_file() and q.is_relative_to(ROOT) and not q.is_relative_to(directory):
                external.add(q)
    for p in directory.glob('*/manifest.json'):
        for image in json.loads(p.read_text()).get('immutable_proms', []):
            q = ROOT/image['file']
            assert q.is_file()
            external.add(q)
for p in sorted(external):
    fold.add(p, 'executed-external/'+str(p.relative_to(ROOT)))
for dirname in ('native57-dispy-baseline-src', 'native58-dsketh-baseline-src'):
    p=BUILD/dirname/'build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
    fold.add(p, 'parent-backplanes/'+dirname+'.v')
fold.add(BUILD/'boards-shared-f10145-aggregate-20261019.json', 'eight-board-aggregate.json')
fold.tree(BUILD/'enabled60-shared-files-v2-assets-20261019', 'prepared-full-core-assets')
for source, name in (
    ('enabled60-shared-files-source-audit-v2-20261019.json', 'first-staging-source-audit.json'),
    ('enabled60-shared-files-v2-source-audit-20261019.json', 'source-audit.json'),
    ('enabled60-shared-files-actual-source-audit-20261019.json', 'actual-source-audit.json'),
    ('enabled60-legacy-audit-after-shared-files-20261019.json', 'legacy-source-audit.json')):
    fold.add(BUILD/source, name)
fold.finish()

mlab = Archive('2026-10-19-snapshot-f10145-mlab-rejected')
for name in ('generate_f10145_mlab.py','fit_f10145_mlab.py','fit_riot.py'):
    mlab.add(TOOLS/name, 'tools/'+name)
mlab.add(Path(__file__), 'tools/'+Path(__file__).name)
mlab.tree(BUILD/'f10145-mlab-fit-result-20261019/SnapshotF10145SharedMLAB_20261019', 'fit')
mlab.add(BUILD/'f10145-mlab-rejection-20261019.json', 'rejection.json')
mlab.finish()
