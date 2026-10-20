#!/usr/bin/env python3
"""Lossless eleven-board namespace/global-hook qualification and source receipts."""
import gzip,hashlib,json,sys
from pathlib import Path
ROOT=Path('/Users/alans/dev2/Dorado');B=ROOT/'build/rtl-executive';T=ROOT/'fpga/mister/snapshot'
sys.path.insert(0,str(T))
import generate_serial_boards as gen
import audit_serial_boards,classify_serial_boards,test_serial_boards,test_serial_boards_mutations
for p in ('serial-boards-global-test-v2-20261019/results.json','serial-boards-global-mutants-v1-20261019/results.json','serial-boards-global-classification-v5-20261019/results.json','serial-boards-global-source-audit-v1-20261019.json'):
 assert json.loads((B/p).read_text())['status']=='PASS',p
O=ROOT/'fpga/mister/reports/2026-10-19-snapshot-serial-board-global';O.mkdir(exist_ok=False);rows=[]
def add(p,name):
 raw=p.read_bytes();zipped=p.suffix not in('.py','.json','.md')or len(raw)>100*1024
 q=O/(name+('.gz'if zipped else''));q.parent.mkdir(parents=True,exist_ok=True);assert not q.exists(),name
 q.write_bytes(gzip.compress(raw,mtime=0)if zipped else raw)
 rows.append(dict(source=str(p.relative_to(ROOT)),raw_name=name,stored=str(q.relative_to(O)),raw_bytes=len(raw),raw_sha256=hashlib.sha256(raw).hexdigest()))
def tree(p,name):
 assert p.is_dir(),p
 for q in sorted(p.rglob('*')):
  if q.is_file()and not set(q.relative_to(p).parts)&{'obj','obj_xml','db','incremental_db','__pycache__'}and not q.name.startswith('._'):add(q,name+'/'+str(q.relative_to(p)))
for v in (1,2,3):tree(B/f'serial-boards-global-v{v}-20261019',f'generation-v{v}')
for v in (1,2):tree(B/f'serial-boards-global-test-v{v}-20261019',f'simulation-v{v}')
tree(B/'serial-boards-global-mutants-v1-20261019','mutations')
for v in range(1,6):tree(B/f'serial-boards-global-classification-v{v}-20261019',f'classification-v{v}')
add(B/'serial-boards-global-source-audit-v1-20261019.json','source-audit.json')
add(B/'native-hold-domain-review-20261019.json','next-native-domain-review.json')
add(B/'review-native-hold-domains-20261019.py','next-native-domain-review.py')
# Exact original support review and its provenance remain review-only; capture
# all named source files after independent receipt revalidation.
I=B/'enabled60-support-inventory-v3-20261019'
for n in ('machine-support-review.json','provenance.json'):add(I/n,'native-support-review/'+n)
p=json.loads((I/'provenance.json').read_text());support=Path(p['build'])
for n,h in p['sources'].items():
 f=support/n;assert hashlib.sha256(f.read_bytes()).hexdigest()==h
 add(f,'native-support-source/'+n)
# Rebuild namespace conversion/catalog directly from extracted receipt bytes.
needed={}
for name,_,_,folder,version in gen.fmt.SPECS:
 q=gen.qualified(ROOT/'fpga/mister/reports',name);root=q['root'];prefix=q['prefix']
 names=needed.setdefault(root,set());names.add(prefix+'provenance.json');names.update(q['source_paths'].values())
 for n in (gen.CLASSIFICATION[name]+'/inventory.json',gen.CLASSIFICATION[name]+'/classification.json',version+'/candidate/command.json',version+'/manifest.json',version+'/results.json'):
  names.add(n)
 names.add(version+('/ports.json'if name=='ContB'else'/fixture-metadata.json'))
for root,names in needed.items():
 p=root/'archive-manifest.json';p=p if p.exists()else p.with_suffix('.json.gz');add(p,'receipt-root/'+root.name+'/'+p.name)
 for name in sorted(names):
  row=gen.fmt.receipt_index(root)[name];p=root/row['stored'];add(p,'receipt-root/'+root.name+'/'+row['stored'])
paths={Path(__file__),T/'ff_scan_controller.sv'}
for module in list(sys.modules.values()):
 filename=getattr(module,'__file__',None)
 if filename:
  p=Path(filename).resolve()
  if p.is_relative_to(ROOT)and p.suffix=='.py':paths.add(p)
for p in sorted(paths):add(p,'tools/'+str(p.relative_to(ROOT)))
history=dict(status='PASS',generation='V1 corrects ContA reset-fit receipt path. V2 corrects scalar ContB port metadata. V3 generates all11boards/148unique modules.',
 simulation='ContA smoke V1 and all11V2 pass. All9actual-board mutations on ContA/MemX/BaseBd reject, independently checking force hold, scan abort and post-reset auxiliary access.',
 classification='V1 encounters Verilator escaped module cell aliases. V2 adds alias resolution and then fixes an import. V3 exposes shared origName parameter aliases. V4 uses exact decoded specialization; V5 removes fallback entirely and adds unknown/missing-specialization controls. Raw XML remains unchanged; normalized XML and complete alias tables are separately retained.',
 coverage='1944744physical+3930private bits; no sequential exclusions; source inverse66controls and fresh inventory77controls pass.',
 limits='Component functional/source/state qualification only; no new Quartus fit, combined capacity/timing, native pause barriers, whole-machine snapshot, hardware deployment or speed improvement.')
(O/'qualification-history.json').write_text(json.dumps(history,indent=2)+'\n')
for row in rows:
 p=O/row['stored'];raw=gzip.decompress(p.read_bytes())if p.suffix=='.gz'else p.read_bytes()
 assert len(raw)==row['raw_bytes']and hashlib.sha256(raw).hexdigest()==row['raw_sha256']
(O/'archive-manifest.json.gz').write_bytes(gzip.compress((json.dumps(rows,indent=2)+'\n').encode(),mtime=0))
r=dict(status='PASS',raw_files=len(rows),raw_bytes=sum(x['raw_bytes']for x in rows),stored_bytes=sum((O/x['stored']).stat().st_size for x in rows))
(O/'archive-readback.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r))
