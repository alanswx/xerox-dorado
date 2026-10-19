#!/usr/bin/env python3
"""Lossless terminal capacity rejection and RIOT shared-port evidence archives."""
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
        zip_ = p.suffix not in ('.py', '.json', '.md') or (p.suffix == '.json' and len(raw) > 100*1024)
        target = self.root/(name+('.gz' if zip_ else ''))
        target.parent.mkdir(parents=True, exist_ok=True)
        assert not target.exists(), name
        target.write_bytes(gzip.compress(raw, mtime=0) if zip_ else raw)
        self.rows.append(dict(source=str(p.relative_to(ROOT)), raw_name=name,
                              stored=str(target.relative_to(self.root)), raw_bytes=len(raw),
                              raw_sha256=hashlib.sha256(raw).hexdigest()))

    def tree(self, p, name, skip=()):
        for q in p.rglob('*'):
            relative = q.relative_to(p)
            if (q.is_file() and not set(relative.parts)&{'obj', 'obj_xml', 'db', 'incremental_db', '__pycache__'}
                    and not q.name.startswith('._') and str(relative) not in skip):
                self.add(q, name+'/'+str(relative))

    def finish(self):
        for row in self.rows:
            p = self.root/row['stored']
            raw = gzip.decompress(p.read_bytes()) if p.suffix == '.gz' else p.read_bytes()
            assert len(raw) == row['raw_bytes'] and hashlib.sha256(raw).hexdigest() == row['raw_sha256']
        raw = json.dumps(self.rows, indent=2).encode()+b'\n'
        zip_ = len(raw) > 100*1024
        (self.root/('archive-manifest.json'+('.gz' if zip_ else ''))).write_bytes(gzip.compress(raw, mtime=0) if zip_ else raw)
        result = dict(status='PASS', raw_files=len(self.rows), raw_bytes=sum(r['raw_bytes'] for r in self.rows),
                      stored_bytes=sum((self.root/r['stored']).stat().st_size for r in self.rows),
                      scope='Lossless readback of raw evidence, excluding derived objects/databases; not global snapshot qualification.')
        (self.root/'archive-readback.json').write_text(json.dumps(result, indent=2)+'\n')
        print(self.root.name, json.dumps(result))


capacity = Archive('2026-10-19-snapshot-enabled-capacity')
capacity.tree(BUILD/'enabled60-capacity-20261019', 'failed-fit')
capacity.add(BUILD/'enabled60-capacity-review-20261019.json', 'capacity-review.json')
capacity.add(BUILD/'enabled60-remote-review-20261019/actual-build-audit.json', 'actual-source-audit.json')
for name in ('audit_enabled_capacity.py', 'inventory_machine_support.py'):
    capacity.add(TOOLS/name, 'tools/'+name)
capacity.add(Path(__file__), 'tools/'+Path(__file__).name)
for version in (1, 2, 3):
    d = BUILD/f'enabled60-support-inventory-v{version}-20261019'
    # V2 and V3 XML/full inventories are byte-identical. Keep only the V3
    # copies; retain V2's distinct provenance/summary and elaboration log.
    skip = ('native-machine.xml', 'complete-review-inventory.json') if version == 2 else ()
    capacity.tree(d, f'machine-support-v{version}', skip)
    capacity.add(d.with_name(d.name+'.log'), f'machine-support-v{version}/driver.log')
capacity.add(ROOT/'tools/sil_state_inventory.py', 'tools/sil_state_inventory.py')
capacity.finish()

riot = Archive('2026-10-19-snapshot-riot-shared-port')
for name in ('generate_riot_shared_port.py', 'test_riot_shared_port.py', 'classify_riot_shared_port.py',
             'fit_riot_shared_port.py', 'audit_riot_shared_port.py', 'generate_baseboard_shared_riot.py',
             'test_baseboard_shared_riot.py', 'build_enabled_core.py', 'generate_baseboard_full.py',
             'generate_riot.py', 'fit_riot.py'):
    riot.add(TOOLS/name, 'tools/'+name)
riot.add(Path(__file__), 'tools/'+Path(__file__).name)
riot.tree(BUILD/'riot-shared-port-v1-20261019', 'simulation')
riot.add(BUILD/'riot-shared-port-v1-20261019.log', 'simulation/driver.log')
riot.tree(BUILD/'riot-shared-port-classification-v1-20261019', 'classification')
riot.tree(BUILD/'riot-shared-port-fit-result-20261019/SnapshotRIOTSharedPort_20261019', 'fit')
riot.add(BUILD/'riot-shared-port-audited-20261019.json', 'fit-audit.json')
riot.tree(BUILD/'baseboard-shared-riot-v1-20261019', 'full-baseboard')
riot.add(BUILD/'baseboard-shared-riot-v1-20261019.log', 'full-baseboard/driver.log')
riot.tree(BUILD/'enabled60-shared-riot-assets-20261019', 'prepared-full-core-assets')
riot.add(BUILD/'enabled60-shared-riot-source-audit-20261019.json', 'full-core-source-audit.json')
riot.finish()
