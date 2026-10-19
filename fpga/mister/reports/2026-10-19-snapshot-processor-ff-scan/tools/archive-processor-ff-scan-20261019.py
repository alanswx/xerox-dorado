#!/usr/bin/env python3
"""Preserve both physical processor-board scan qualifications unchanged."""
import gzip,hashlib,json
from pathlib import Path
ROOT=Path('/Users/alans/dev2/Dorado');B=ROOT/'build/rtl-executive';T=ROOT/'fpga/mister/snapshot'
O=ROOT/'fpga/mister/reports/2026-10-19-snapshot-processor-ff-scan';O.mkdir(exist_ok=False)
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
for short,kind in [('proch','ProcH'),('procl','ProcL')]:
 unit=B/f'{short}-ff-scan-v1-20261019'
 r=json.loads((unit/'results.json').read_text());assert r['status']=='PASS'and len(r['cases'])==7
 tree(unit,kind+'/simulation');add(unit.with_name(unit.name+'.log'),kind+'/simulation/driver.log')
 tree(B/f'{short}-ff-scan-classification-v1-20261019',kind+'/classification')
 tree(B/f'{short}-ff-scan-fit-result-20261019/Snapshot{kind}FFScan_20261019',kind+'/fit')
 add(B/f'{short}-ff-scan-fit-audit-v1-20261019.json',kind+'/fit-audit.json')
for name in ['generate_processor_ff_scan.py','test_processor_ff_scan.py','fit_processor_ff_scan.py','classify_processor_ff_scan.py','audit_processor_ff_scan.py',
             'ff_scan_controller.sv','generate_conta_ff_scan.py','test_conta_ff_scan.py','test_conta_full.cpp','generate_conta_full.py',
             'generate_processor_full.py','test_processor_full.py','test_processor_full.cpp','generate_processor_ff.py','generate_snapshot_ff.py',
             'generate_contb_full.py','generate_contb.py','audit_contb_im.py','generate_f10145.py','generate_f10145_shared_write.py',
             'generate_f10145_zero_write.py','generate_async256.py','fit_processor_full.py','fit_conta_full.py','fit_f10415.py','fit_f10415_enable.py',
             'audit_processor_full.py','audit_baseboard_full.py','audit_enabled_core.py']:
 add(T/name,'tools/'+name)
add(ROOT/'tools/sil_state_inventory.py','tools/sil_state_inventory.py')
add(Path(__file__),'tools/'+Path(__file__).name)
for row in rows:
 p=O/row['stored'];raw=gzip.decompress(p.read_bytes())if p.suffix=='.gz'else p.read_bytes()
 assert len(raw)==row['raw_bytes']and hashlib.sha256(raw).hexdigest()==row['raw_sha256']
raw=(json.dumps(rows,indent=2)+'\n').encode();zipped=len(raw)>100*1024
(O/('archive-manifest.json'+('.gz'if zipped else''))).write_bytes(gzip.compress(raw,mtime=0)if zipped else raw)
r=dict(status='PASS',raw_files=len(rows),raw_bytes=sum(x['raw_bytes']for x in rows),stored_bytes=sum((O/x['stored']).stat().st_size for x in rows))
(O/'archive-readback.json').write_text(json.dumps(r,indent=2)+'\n');print(r)
