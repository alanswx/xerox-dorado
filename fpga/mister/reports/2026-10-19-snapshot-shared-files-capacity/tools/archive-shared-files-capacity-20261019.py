#!/usr/bin/env python3
"""Preserve the terminal combined state-preserving area experiment."""
import gzip
import hashlib
import json
from pathlib import Path
ROOT=Path('/Users/alans/dev2/Dorado')
BUILD=ROOT/'build/rtl-executive'
OUT=ROOT/'fpga/mister/reports/2026-10-19-snapshot-shared-files-capacity'
OUT.mkdir(exist_ok=False)
rows=[]
sources=[(p,'failed-fit/'+p.name)for p in(BUILD/'enabled60-shared-files-capacity-20261019').iterdir()if p.is_file()]
sources += [(BUILD/'enabled60-shared-files-capacity-review-20261019.json','capacity-review.json'),
            (BUILD/'enabled60-shared-files-actual-source-audit-20261019.json','actual-source-audit.json'),
            (ROOT/'fpga/mister/snapshot/audit_enabled_capacity.py','executed-audit.py'),
            (Path(__file__),'tools/'+Path(__file__).name)]
review=json.loads((BUILD/'enabled60-shared-files-capacity-review-20261019.json').read_text())
expected=next(h for p,h in review['input_hashes'].items()if p.endswith('/audit_enabled_capacity.py'))
assert hashlib.sha256((ROOT/'fpga/mister/snapshot/audit_enabled_capacity.py').read_bytes()).hexdigest()==expected
for source,name in sources:
 raw=source.read_bytes();zipped=source.suffix not in('.py','.json')or len(raw)>100*1024
 target=OUT/(name+('.gz'if zipped else''));target.parent.mkdir(parents=True,exist_ok=True)
 target.write_bytes(gzip.compress(raw,mtime=0)if zipped else raw)
 rows.append(dict(source=str(source.relative_to(ROOT)),raw_name=name,stored=str(target.relative_to(OUT)),
                  raw_bytes=len(raw),raw_sha256=hashlib.sha256(raw).hexdigest()))
for row in rows:
 p=OUT/row['stored'];raw=gzip.decompress(p.read_bytes())if p.suffix=='.gz'else p.read_bytes()
 assert len(raw)==row['raw_bytes']and hashlib.sha256(raw).hexdigest()==row['raw_sha256']
(OUT/'archive-manifest.json').write_text(json.dumps(rows,indent=2)+'\n')
r=dict(status='PASS',raw_files=len(rows),raw_bytes=sum(x['raw_bytes']for x in rows),stored_bytes=sum((OUT/x['stored']).stat().st_size for x in rows))
(OUT/'archive-readback.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r))
