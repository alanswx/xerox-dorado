import gzip,hashlib,json
from pathlib import Path
ROOT=Path('/Users/alans/dev2/Dorado');B=ROOT/'build/rtl-executive';O=ROOT/'fpga/mister/reports/2026-10-19-snapshot-ff-scan-controller';O.mkdir(exist_ok=False)
rows=[]
def add(p,name):
 raw=p.read_bytes();zipped=p.suffix not in('.py','.json','.md')or len(raw)>100*1024;q=O/(name+('.gz'if zipped else''));q.parent.mkdir(parents=True,exist_ok=True);assert not q.exists()
 q.write_bytes(gzip.compress(raw,mtime=0)if zipped else raw);rows.append(dict(source=str(p.relative_to(ROOT)),raw_name=name,stored=str(q.relative_to(O)),raw_bytes=len(raw),raw_sha256=hashlib.sha256(raw).hexdigest()))
for v in(1,2):
 d=B/f'ff-scan-controller-v{v}-20261019';r=json.loads((d/'results.json').read_text());assert r['status']=='PASS'
 for p in sorted(d.rglob('*')):
  if p.is_file()and p.name!='sim':add(p,f'simulation-v{v}/'+str(p.relative_to(d)))
 for row in r['cases']:
  log=(d/(row['case']+'-'+str(row['bits']))/'test.log').read_text()
  if row['case']=='candidate':assert row['returncode']==0 and 'SCAN PASS:'in log
  else:
   assert row['returncode']!=0
   anchor={'lost_feedback':'physical word oracle','early_completion':'rotation length/status','ignored_write_window':'physical word oracle','lost_quarantine':'fault escaped'}[row['case']]
   assert anchor in log
 add(d.with_name(d.name+'.log'),f'simulation-v{v}/driver.log')
for name in ['ff_scan_controller.sv','test_ff_scan_controller.py']:add(ROOT/'fpga/mister/snapshot'/name,'tools/'+name)
add(Path(__file__),'tools/'+Path(__file__).name)
for row in rows:
 p=O/row['stored'];raw=gzip.decompress(p.read_bytes())if p.suffix=='.gz'else p.read_bytes();assert len(raw)==row['raw_bytes']and hashlib.sha256(raw).hexdigest()==row['raw_sha256']
(O/'archive-manifest.json').write_text(json.dumps(rows,indent=2)+'\n')
r=dict(status='PASS',raw_files=len(rows),raw_bytes=sum(x['raw_bytes']for x in rows),stored_bytes=sum((O/x['stored']).stat().st_size for x in rows));(O/'archive-readback.json').write_text(json.dumps(r,indent=2)+'\n');print(r)
