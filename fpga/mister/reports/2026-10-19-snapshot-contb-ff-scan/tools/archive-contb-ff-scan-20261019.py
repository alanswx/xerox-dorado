#!/usr/bin/env python3
"""Lossless complete ContB serial-access qualification evidence."""
import gzip,hashlib,json
from pathlib import Path
ROOT=Path('/Users/alans/dev2/Dorado');B=ROOT/'build/rtl-executive';T=ROOT/'fpga/mister/snapshot'
O=ROOT/'fpga/mister/reports/2026-10-19-snapshot-contb-ff-scan';O.mkdir(exist_ok=False)
rows=[]
def add(p,name):
 raw=p.read_bytes();zipped=p.suffix not in('.py','.json','.md')or len(raw)>100*1024
 q=O/(name+('.gz'if zipped else''));q.parent.mkdir(parents=True,exist_ok=True);assert not q.exists(),name
 q.write_bytes(gzip.compress(raw,mtime=0)if zipped else raw)
 rows.append(dict(source=str(p.relative_to(ROOT)),raw_name=name,stored=str(q.relative_to(O)),raw_bytes=len(raw),raw_sha256=hashlib.sha256(raw).hexdigest()))
def tree(p,name):
 assert p.is_dir(),p
 for q in sorted(p.rglob('*')):
  if q.is_file()and not set(q.relative_to(p).parts)&{'obj','obj_xml','db','incremental_db','__pycache__'}and not q.name.startswith('._'):add(q,name+'/'+str(q.relative_to(p)))
tree(B/'contb-ff-scan-v1-20261019','simulation-v1')
assert json.loads((B/'contb-ff-scan-v1-20261019/results.json').read_text())['status']=='PASS'
tree(B/'contb-ff-scan-classification-v2-20261019','classification')
tree(B/'contb-ff-scan-fit-result-20261019/SnapshotContBFFScan_20261019','fit')
add(B/'contb-ff-scan-fit-audit-v1-20261019.json','fit-audit.json')
source_root=B/'native59-snapshot-budget-baseline/src'
m=json.loads((B/'contb-ff-scan-classification-v2-20261019/manifest.json').read_text())
for n in sorted({'verilog/generated/ContB-Rev-Cd.v',*m['grouping_sources']}):add(source_root/n,'generator-root/'+n)
for suffix in ('wl','lc'):
 name='chm/sil/ContB-Rev-Cd.dm!1_/ContB-Rev-Cd.'+suffix
 add(ROOT/name,'generator-root/'+name)
for n in ['generate_contb_ff_scan.py','test_contb_ff_scan.py','fit_contb_ff_scan.py','classify_contb_ff_scan.py','audit_contb_ff_scan.py',
 'generate_contb_full.py','test_contb_full.py','test_contb_full.cpp','fit_contb_full.py','audit_contb_full.py',
 'generate_contb.py','test_contb_snapshot.py','fit_contb_snapshot.py','audit_contb_im.py','classify_contb_full.py',
 'f10415_enable_bank.sv','ff_scan_controller.sv','generate_conta_ff_scan.py','generate_conta_full.py','generate_snapshot_ff.py',
 'fit_f10415.py','fit_f10415_enable.py','audit_f10415.py','audit_f10415_enable.py','audit_baseboard_full.py']:
 add(T/n,'tools/'+n)
add(ROOT/'tools/sil_state_inventory.py','tools/sil_state_inventory.py')
add(Path(__file__),'tools/'+Path(__file__).name)
for row in rows:
 p=O/row['stored'];raw=gzip.decompress(p.read_bytes())if p.suffix=='.gz'else p.read_bytes()
 assert len(raw)==row['raw_bytes']and hashlib.sha256(raw).hexdigest()==row['raw_sha256']
raw=(json.dumps(rows,indent=2)+'\n').encode();zipped=len(raw)>100*1024
(O/('archive-manifest.json'+('.gz'if zipped else''))).write_bytes(gzip.compress(raw,mtime=0)if zipped else raw)
r=dict(status='PASS',raw_files=len(rows),raw_bytes=sum(x['raw_bytes']for x in rows),stored_bytes=sum((O/x['stored']).stat().st_size for x in rows))
(O/'archive-readback.json').write_text(json.dumps(r,indent=2)+'\n');print(r)
