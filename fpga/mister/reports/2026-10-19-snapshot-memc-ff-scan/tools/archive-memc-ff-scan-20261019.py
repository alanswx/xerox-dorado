#!/usr/bin/env python3
"""Lossless complete MemC serial-access qualification evidence."""
import gzip,hashlib,json
from pathlib import Path
ROOT=Path('/Users/alans/dev2/Dorado');B=ROOT/'build/rtl-executive';T=ROOT/'fpga/mister/snapshot'
O=ROOT/'fpga/mister/reports/2026-10-19-snapshot-memc-ff-scan';O.mkdir(exist_ok=False)
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
for version in (1,2,3,4):
 tree(B/f'memc-ff-scan-v{version}-20261019',f'simulation-v{version}')
assert json.loads((B/'memc-ff-scan-v4-20261019/results.json').read_text())['status']=='PASS'
tree(B/'memc-ff-scan-classification-v4-20261019','classification')
tree(B/'memc-ff-scan-fit-result-20261019/SnapshotMemCFFScan_20261019','fit')
add(B/'memc-ff-scan-fit-audit-v1-20261019.json','fit-audit.json')
source_root=B/'native59-snapshot-budget-baseline/src'
m=json.loads((B/'memc-ff-scan-classification-v4-20261019/manifest.json').read_text())
needed={'verilog/generated/MemC-Rev-Be.v',*['verilog/cells/'+n+'.v' for n in m['original_sources']],*[p['file']for p in m.get('immutable_proms',[])]}
for n in sorted(needed):add(source_root/n,'generator-root/'+n)
for n in ['generate_memc_ff_scan.py','test_memc_ff_scan.py','fit_memc_ff_scan.py','classify_memc_ff_scan.py','audit_memc_ff_scan.py',
 'generate_memc_full.py','test_memc_full.py','test_memc_full.cpp','fit_memc_full.py','audit_memc_full.py','generate_f100181.py','generate_async256.py','generate_jk.py',
 'ff_scan_controller.sv','generate_conta_ff_scan.py','generate_conta_full.py','generate_contb_full.py','generate_contb.py','generate_snapshot_ff.py',
 'generate_processor_ff.py','generate_f10145.py','generate_f10145_shared_write.py','generate_f10145_zero_write.py',
 'audit_contb_im.py','fit_conta_full.py','fit_f10415.py','fit_f10415_enable.py',
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
