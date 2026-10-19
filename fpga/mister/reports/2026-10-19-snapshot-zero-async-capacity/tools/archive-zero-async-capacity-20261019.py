import gzip,hashlib,json
from pathlib import Path
ROOT=Path('/Users/alans/dev2/Dorado');B=ROOT/'build/rtl-executive'
O=ROOT/'fpga/mister/reports/2026-10-19-snapshot-zero-async-capacity';O.mkdir(exist_ok=False)
rows=[]
def add(p,name):
 raw=p.read_bytes();zipped=p.suffix not in('.py','.json','.md')or len(raw)>100*1024
 q=O/(name+('.gz'if zipped else''));q.parent.mkdir(parents=True,exist_ok=True);assert not q.exists()
 q.write_bytes(gzip.compress(raw,mtime=0)if zipped else raw)
 rows.append(dict(source=str(p.relative_to(ROOT)),raw_name=name,stored=str(q.relative_to(O)),raw_bytes=len(raw),raw_sha256=hashlib.sha256(raw).hexdigest()))
for p in(B/'enabled60-zero-async-capacity-20261019').iterdir():add(p,'failed-fit/'+p.name)
add(B/'enabled60-zero-async-synthesis-20261019/map.log','failed-fit/map.log')
for name,label in [('enabled60-zero-async-capacity-review-20261019.json','capacity-review.json'),('enabled60-zero-async-mapping-audit-20261019.json','mapping-audit.json'),('enabled60-zero-async-actual-source-audit-20261019.json','actual-source-audit.json')]:add(B/name,label)
r=json.loads((B/'enabled60-zero-async-mapping-audit-20261019.json').read_text());src=(B/'enabled60-zero-async-source-check-20261019/src').resolve()
for name in r['input_hashes']:
 p=Path(name)
 assert hashlib.sha256(p.read_bytes()).hexdigest()==r['input_hashes'][name]
 if p.is_relative_to(src):add(p,'source-root/'+str(p.relative_to(src)))
for name in ['audit_enabled_capacity.py','audit_enabled_f10145_mapping.py','audit_enabled_core.py','build_enabled_core.py']:add(ROOT/'fpga/mister/snapshot'/name,'tools/'+name)
add(Path(__file__),'tools/'+Path(__file__).name)
for row in rows:
 p=O/row['stored'];raw=gzip.decompress(p.read_bytes())if p.suffix=='.gz'else p.read_bytes()
 assert len(raw)==row['raw_bytes']and hashlib.sha256(raw).hexdigest()==row['raw_sha256']
(O/'archive-manifest.json').write_text(json.dumps(rows,indent=2)+'\n')
res=dict(status='PASS',raw_files=len(rows),raw_bytes=sum(x['raw_bytes']for x in rows),stored_bytes=sum((O/x['stored']).stat().st_size for x in rows))
(O/'archive-readback.json').write_text(json.dumps(res,indent=2)+'\n');print(res)
