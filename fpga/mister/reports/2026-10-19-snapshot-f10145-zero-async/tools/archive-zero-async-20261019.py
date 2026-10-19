#!/usr/bin/env python3
"""Archive zero-private-write-reply logic gates and physical RAM experiments."""
import gzip
import hashlib
import json
from pathlib import Path
import re
import sys
ROOT=Path('/Users/alans/dev2/Dorado')
BUILD=ROOT/'build/rtl-executive'
TOOLS=ROOT/'fpga/mister/snapshot'
sys.path.insert(0,str(TOOLS))
import fit_riot
import generate_f10145_zero_write as gen

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

boards=json.loads((BUILD/'boards-zero-write-v1-20261019/results.json').read_text())
assert boards['status']=='PASS' and len(boards['boards'])==8
cell,manifest=gen.generate()
rows=[]
counts=dict(ContA=1353,ProcH=5712,ProcL=5581,MemC=23718,MemD=296591,MemX=1378312,DskEth=2656,DispY=13993)
for board in boards['boards']:
 d=BUILD/'boards-zero-write-v1-20261019'/board
 r=json.loads((d/'results.json').read_text())
 assert len(r['cases'])==1 and r['cases'][0]['returncode']==0
 files=list(d.rglob('cell_F10145A_snapshot.sv'))
 assert len(files)==1 and files[0].read_bytes()==cell.encode()
 rows.append(dict(board=board,physical_bits=counts[board],evidence=str(d.relative_to(ROOT)),
                  cell_sha256=sha(files[0]),results_sha256=sha(d/'results.json')))
aggregate=dict(status='PASS',scope='Eight unchanged positive full-board restore fixtures with zero private F10145 write replies; no physical/global machine snapshot or rerun of older broken-board suites.',
               boards=rows,total_modeled_bits=sum(r['physical_bits']for r in rows))
assert aggregate['total_modeled_bits']==1727916
(BUILD/'boards-zero-write-aggregate-20261019.json').write_text(json.dumps(aggregate,indent=2)+'\n')

strict=BUILD/'f10145-zero-strict-mlab-result-20261019/SnapshotF10145ZeroWriteMLAB_20261019'
review=fit_riot.audit(strict)
fit=(strict/'snapshot/snapshot_fit.fit.rpt').read_text(encoding='latin1')
log=(strict/'snapshot/map.log').read_text()
assert re.search(r'; Total MLAB memory bits\s*; 0\s*;',fit)
assert log.count('Info (276009):')==2 and log.count('is uninferred due to unsupported read-during-write behavior')==2
assert review['delta']['alms']==11
rejection=dict(status='REJECTED_MAPPING_AND_AREA',scope='First zero-reply strict MLAB trial. Timing passes but no RAM is inferred; not selected in a core. Exact executed sources are preserved.',
 logic_alms=review['results']['baseline']['resources']['alms'],candidate_alms=review['results']['snapshot']['resources']['alms'],
 delta_alms=review['delta']['alms'],mlab_memory_bits=0,uninferred_ram_instances=2,source_and_timing_review=review)
(BUILD/'f10145-zero-strict-mlab-rejection-20261019.json').write_text(json.dumps(rejection,indent=2)+'\n')

OUT=ROOT/'fpga/mister/reports/2026-10-19-snapshot-f10145-zero-async'
OUT.mkdir(exist_ok=False)
archive=[]
def add(p,name):
 raw=p.read_bytes()
 zipped=p.suffix not in('.py','.json','.md') or len(raw)>100*1024
 target=OUT/(name+('.gz'if zipped else''))
 target.parent.mkdir(parents=True,exist_ok=True)
 assert not target.exists(),name
 target.write_bytes(gzip.compress(raw,mtime=0)if zipped else raw)
 archive.append(dict(source=str(p.relative_to(ROOT)),raw_name=name,stored=str(target.relative_to(OUT)),raw_bytes=len(raw),raw_sha256=hashlib.sha256(raw).hexdigest()))
def tree(p,name):
 assert p.is_dir(),p
 for q in sorted(p.rglob('*')):
  if q.is_file() and not set(q.relative_to(p).parts)&{'obj','obj_xml','db','incremental_db','__pycache__'} and not q.name.startswith('._'):
   add(q,name+'/'+str(q.relative_to(p)))
for name in ['generate_f10145_zero_write.py','test_f10145_zero_write.py','classify_f10145_zero_write.py','classify_f10145_shared_write.py',
             'fit_f10145_mlab.py','audit_f10145_zero_async.py','test_f10145_shared_write.py','test_boards_shared_f10145.py',
             'build_enabled_core.py','audit_enabled_core.py','audit_enabled_capacity.py','fit_riot.py']:
 add(TOOLS/name,'tools/'+name)
add(Path(__file__),'tools/'+Path(__file__).name)
for v in (1,2):
 stem=f'f10145-zero-write-v{v}-20261019'
 tree(BUILD/stem,f'simulation-v{v}')
 tree(BUILD/(stem+'-driver-inputs'),f'simulation-v{v}-driver-inputs')
 add(BUILD/(stem+'.log'),f'simulation-v{v}/driver.log')
for v in(2,3):tree(BUILD/f'f10145-zero-write-classification-v{v}-20261019',f'classification-v{v}')
tree(BUILD/'boards-zero-write-v1-20261019','full-boards')
add(BUILD/'boards-zero-write-v1-20261019.log','full-boards/driver.log')
add(BUILD/'boards-zero-write-aggregate-20261019.json','eight-board-aggregate.json')
external=set()
for p in(BUILD/'boards-zero-write-v1-20261019').rglob('command.json'):
 for arg in json.loads(p.read_text()):
  q=Path(arg)
  if q.is_absolute()and q.is_file()and q.is_relative_to(ROOT)and not q.is_relative_to(BUILD/'boards-zero-write-v1-20261019'):external.add(q)
for p in(BUILD/'boards-zero-write-v1-20261019').glob('*/manifest.json'):
 for image in json.loads(p.read_text()).get('immutable_proms',[]):external.add(ROOT/image['file'])
for p in sorted(external):add(p,'executed-external/'+str(p.relative_to(ROOT)))
for dirname in('native57-dispy-baseline-src','native58-dsketh-baseline-src'):
 add(BUILD/dirname/'build/rtl-executive/native16-tx-cable/dorado_native_backplane.v','parent-backplanes/'+dirname+'.v')
tree(strict,'strict-mlab-rejected')
add(BUILD/'f10145-zero-strict-mlab-rejection-20261019.json','strict-mlab-rejection.json')
tree(BUILD/'f10145-zero-async-mlab-result-20261019/SnapshotF10145ZeroAsyncMLAB_20261019','async-fit')
add(BUILD/'f10145-zero-async-audit-v2-20261019.json','async-fit-audit.json')
add(BUILD/'f10145-zero-async-mlab-fit-review-20261019.json','async-source-and-timing-review.json')
tree(BUILD/'enabled60-zero-async-assets-20261019','prepared-full-core-assets')
add(BUILD/'enabled60-zero-async-source-audit-20261019.json','full-core-source-audit.json')
for row in archive:
 p=OUT/row['stored'];raw=gzip.decompress(p.read_bytes())if p.suffix=='.gz'else p.read_bytes()
 assert len(raw)==row['raw_bytes']and hashlib.sha256(raw).hexdigest()==row['raw_sha256']
raw=json.dumps(archive,indent=2).encode()+b'\n'
assert len(raw)>100*1024
(OUT/'archive-manifest.json.gz').write_bytes(gzip.compress(raw,mtime=0))
result=dict(status='PASS',raw_files=len(archive),raw_bytes=sum(r['raw_bytes']for r in archive),stored_bytes=sum((OUT/r['stored']).stat().st_size for r in archive))
(OUT/'archive-readback.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result))
