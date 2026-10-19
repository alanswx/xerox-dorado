#!/usr/bin/env python3
"""Lossless complete MemD serial-access qualification evidence."""
import gzip,hashlib,json
from pathlib import Path
ROOT=Path('/Users/alans/dev2/Dorado');B=ROOT/'build/rtl-executive';T=ROOT/'fpga/mister/snapshot'
O=ROOT/'fpga/mister/reports/2026-10-19-snapshot-memd-ff-scan';O.mkdir(exist_ok=False)
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
tree(B/'memd-ff-scan-v1-20261019','simulation-v1')
tree(B/'memd-ff-scan-v2-20261019','simulation-v2')
assert json.loads((B/'memd-ff-scan-v2-20261019/results.json').read_text())['status']=='PASS'
tree(B/'memd-ff-scan-classification-v1-20261019','classification')
tree(B/'memd-ff-scan-fit-result-20261019/SnapshotMemDFFScan_20261019','fit')
add(B/'memd-ff-scan-fit-audit-v1-20261019.json','fit-audit.json')
source_root=B/'native59-snapshot-budget-baseline/src'
m=json.loads((B/'memd-ff-scan-classification-v1-20261019/manifest.json').read_text())
needed={'verilog/generated/MemD-Rev-Ca.v',*m['original_sources'],*[p['file']for p in m.get('immutable_proms',[])]}
for n in sorted(needed):add(source_root/n,'generator-root/'+n)
for suffix,key in [('wl','wirelist_sha256'),('lc','locations_sha256')]:
 name='chm/sil/MemD-Rev-Ca.dm!1_/MemD-Rev-Ca.'+suffix
 assert hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==m['grouping'][key]
 add(ROOT/name,'generator-root/'+name)
for n in ['generate_memd_ff_scan.py','test_memd_ff_scan.py','fit_memd_ff_scan.py','classify_memd_ff_scan.py','audit_memd_ff_scan.py',
 'generate_memd_full.py','test_memd_full.py','test_memd_full.cpp','fit_memd_full.py','audit_memd_full.py','audit_memd_im.py',
 'ff_scan_controller.sv','generate_conta_ff_scan.py','generate_conta_full.py','generate_contb_full.py','generate_contb.py','generate_snapshot_ff.py',
 'generate_processor_ff.py','generate_f10145.py','generate_f10145_shared_write.py','generate_f10145_zero_write.py',
 'audit_ifu_im.py','f10470_word_bank.sv','audit_contb_im.py','fit_conta_full.py','fit_f10415.py','fit_f10415_enable.py',
 'audit_baseboard_full.py','audit_enabled_core.py']:
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
