import gzip,hashlib,json
from pathlib import Path
ROOT=Path('/Users/alans/dev2/Dorado');B=ROOT/'build/rtl-executive';T=ROOT/'fpga/mister/snapshot'
O=ROOT/'fpga/mister/reports/2026-10-19-snapshot-riot-row-decode-rejected';O.mkdir(exist_ok=False)
rows=[]
def add(p,name):
 raw=p.read_bytes();zipped=p.suffix not in('.py','.json','.md')or len(raw)>100*1024;q=O/(name+('.gz'if zipped else''));q.parent.mkdir(parents=True,exist_ok=True);assert not q.exists()
 q.write_bytes(gzip.compress(raw,mtime=0)if zipped else raw);rows.append(dict(source=str(p.relative_to(ROOT)),raw_name=name,stored=str(q.relative_to(O)),raw_bytes=len(raw),raw_sha256=hashlib.sha256(raw).hexdigest()))
def tree(p,name):
 for q in sorted(p.rglob('*')):
  if q.is_file()and not set(q.relative_to(p).parts)&{'obj','obj_xml','db','incremental_db','__pycache__'}and not q.name.startswith('._'):add(q,name+'/'+str(q.relative_to(p)))
a=json.loads((B/'riot-row-decode-audit-20261019.json').read_text());assert a['status']=='PASS'and a['delta']['alms']==0
for d,name in [('riot-row-decode-v1-20261019','simulation'),('riot-row-decode-v1-20261019-driver-inputs','simulation-driver-inputs'),('riot-row-decode-classification-v1-20261019','classification'),('riot-row-decode-fit-result-20261019/SnapshotRIOTRowDecode_20261019','fit')]:tree(B/d,name)
add(B/'riot-row-decode-v1-20261019.log','simulation/driver.log')
add(B/'riot-row-decode-audit-20261019.json','fit-audit.json')
for name in ['generate_riot_row_decode.py','test_riot_row_decode.py','fit_riot_row_decode.py','audit_riot_shared_port.py','classify_riot_shared_port.py']:add(T/name,'tools/'+name)
add(Path(__file__),'tools/'+Path(__file__).name)
(O/'rejection.json').write_text(json.dumps(dict(status='REJECTED_NO_AREA_BENEFIT',baseline_alms=782,candidate_alms=782,delta_alms=0,minimum_candidate_slack_ns=a['minimum_snapshot_slack_ns'],scope='Original/independent behavior, full state classification and isolated timing/retention pass. Area unchanged, so no full BaseBoard rerun or core integration.'),indent=2)+'\n')
for row in rows:
 p=O/row['stored'];raw=gzip.decompress(p.read_bytes())if p.suffix=='.gz'else p.read_bytes();assert len(raw)==row['raw_bytes']and hashlib.sha256(raw).hexdigest()==row['raw_sha256']
raw=(json.dumps(rows,indent=2)+'\n').encode();zipped=len(raw)>100*1024
(O/('archive-manifest.json'+('.gz'if zipped else''))).write_bytes(gzip.compress(raw,mtime=0)if zipped else raw)
r=dict(status='PASS',raw_files=len(rows),raw_bytes=sum(x['raw_bytes']for x in rows),stored_bytes=sum((O/x['stored']).stat().st_size for x in rows));(O/'archive-readback.json').write_text(json.dumps(r,indent=2)+'\n');print(r)
