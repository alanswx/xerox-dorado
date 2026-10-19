#!/usr/bin/env python3
"""Losslessly preserve real-board serial scan qualification and its history."""
import gzip,hashlib,json
from pathlib import Path
ROOT=Path('/Users/alans/dev2/Dorado');B=ROOT/'build/rtl-executive';T=ROOT/'fpga/mister/snapshot'
O=ROOT/'fpga/mister/reports/2026-10-19-snapshot-conta-ff-scan';O.mkdir(exist_ok=False)
rows=[]
def add(p,name):
 raw=p.read_bytes();zipped=p.suffix not in('.py','.json','.md')or len(raw)>100*1024
 q=O/(name+('.gz'if zipped else''));q.parent.mkdir(parents=True,exist_ok=True);assert not q.exists(),name
 q.write_bytes(gzip.compress(raw,mtime=0)if zipped else raw)
 rows.append(dict(source=str(p.relative_to(ROOT)),raw_name=name,stored=str(q.relative_to(O)),raw_bytes=len(raw),raw_sha256=hashlib.sha256(raw).hexdigest()))
def tree(p,name):
 assert p.is_dir(),p
 for q in sorted(p.rglob('*')):
  if q.is_file()and not set(q.relative_to(p).parts)&{'obj','obj_xml','db','incremental_db','__pycache__'}and q.name!='sim'and not q.name.startswith('._'):add(q,name+'/'+str(q.relative_to(p)))
for v in range(1,8):
 d=B/f'conta-ff-scan-v{v}-20261019';tree(d,f'simulation-v{v}');add(d.with_name(d.name+'.log'),f'simulation-v{v}/driver.log')
for v in(1,2):
 tree(B/f'conta-ff-scan-xml-v{v}-20261019',f'classification-v{v}')
 add(B/f'conta-ff-scan-classification-v{v}-20261019.json',f'classification-v{v}/classification.json')
tree(B/'ff-scan-controller-v3-20261019','controller-sized-offset-regression')
add(B/'ff-scan-controller-v3-20261019.log','controller-sized-offset-regression/driver.log')
tree(B/'conta-ff-scan-fit-result-20261019/SnapshotContAFFScanV2_20261019','first-fit')
add(B/'conta-ff-scan-fit-audit-v1-20261019.json','first-fit-audit.json')
tree(B/'conta-ff-scan-qualified-v1-tools-20261019','first-qualified-tools')
tree(B/'conta-ff-scan-reset-fit-result-20261019/SnapshotContAFFScanReset_20261019','reset-fit')
add(B/'conta-ff-scan-reset-fit-audit-v1-20261019.json','reset-fit-audit.json')
# Retain the prepared inputs for the first remote launch, which failed to
# import a source-generation dependency before any Quartus process started.
tree(B/'conta-ff-scan-fit-v1-20261019','first-launch-prepared-inputs')
for name in ['generate_conta_ff_scan.py','test_conta_ff_scan.py','fit_conta_ff_scan.py','classify_conta_ff_scan.py','audit_conta_ff_scan.py','ff_scan_controller.sv','test_ff_scan_controller.py',
 'generate_conta_full.py','test_conta_full.py','test_conta_full.cpp','generate_snapshot_ff.py','generate_contb_full.py','generate_contb.py','audit_contb_im.py','fit_conta_full.py','fit_f10415.py','fit_f10415_enable.py','audit_baseboard_full.py','audit_enabled_core.py','audit_f10415_enable.py','generate_f10145.py','generate_f10145_shared_write.py','generate_f10145_zero_write.py']:
 add(T/name,'tools/'+name)
for name in ['elaborate-conta-ff-scan-20261019.py','elaborate-conta-ff-scan-v2-20261019.py']:add(B/name,'tools/'+name)
add(ROOT/'tools/sil_state_inventory.py','tools/sil_state_inventory.py')
add(Path(__file__),'tools/'+Path(__file__).name)
for row in rows:
 p=O/row['stored'];raw=gzip.decompress(p.read_bytes())if p.suffix=='.gz'else p.read_bytes()
 assert len(raw)==row['raw_bytes']and hashlib.sha256(raw).hexdigest()==row['raw_sha256']
raw=(json.dumps(rows,indent=2)+'\n').encode();zipped=len(raw)>100*1024
(O/('archive-manifest.json'+('.gz'if zipped else''))).write_bytes(gzip.compress(raw,mtime=0)if zipped else raw)
r=dict(status='PASS',raw_files=len(rows),raw_bytes=sum(x['raw_bytes']for x in rows),stored_bytes=sum((O/x['stored']).stat().st_size for x in rows))
(O/'archive-readback.json').write_text(json.dumps(r,indent=2)+'\n');print(r)
