#!/usr/bin/env python3
"""Lossless serial transport contract and complete compact-board format evidence."""
import gzip,hashlib,json,sys
from pathlib import Path
ROOT=Path('/Users/alans/dev2/Dorado');B=ROOT/'build/rtl-executive';T=ROOT/'fpga/mister/snapshot'
sys.path.insert(0,str(T))
import serial_state_format as fmt
import audit_board_state_serial,classify_board_state_serial
assert json.loads((B/'board-state-serial-v6-20261019/results.json').read_text())['status']=='PASS'
assert json.loads((B/'serial-state-format-v1-20261019/results.json').read_text())['status']=='PASS'
assert json.loads((B/'board-state-serial-fit-audit-v1-20261019.json').read_text())['status']=='PASS_ISOLATED_FIT'
O=ROOT/'fpga/mister/reports/2026-10-19-snapshot-serial-transport';O.mkdir(exist_ok=False)
rows=[]
def add(p,name):
 raw=p.read_bytes();zipped=p.suffix not in('.py','.json','.md')or len(raw)>100*1024
 q=O/(name+('.gz'if zipped else''));q.parent.mkdir(parents=True,exist_ok=True);assert not q.exists(),name
 q.write_bytes(gzip.compress(raw,mtime=0)if zipped else raw)
 rows.append(dict(source=str(p.relative_to(ROOT)),raw_name=name,stored=str(q.relative_to(O)),raw_bytes=len(raw),raw_sha256=hashlib.sha256(raw).hexdigest()))
def tree(p,name):
 for q in sorted(p.rglob('*')):
  if q.is_file()and not set(q.relative_to(p).parts)&{'obj','obj_xml','db','incremental_db','__pycache__'}and not q.name.startswith('._'):add(q,name+'/'+str(q.relative_to(p)))
for v in range(1,7):tree(B/f'board-state-serial-v{v}-20261019',f'simulation-v{v}')
tree(B/'board-state-serial-classification-v1-20261019','classification')
tree(B/'serial-state-format-v1-20261019','format')
tree(B/'board-state-serial-fit-result-20261019/SnapshotSerialTransport_20261019','fit')
add(B/'board-state-serial-fit-audit-v1-20261019.json','fit-audit.json')
# Exact original receipt indexes plus selected byte-identical compressed members
# allow catalog regeneration from the extracted evidence, independent of build/.
added=set()
for name,_,_,folder,version in fmt.SPECS:
 root=ROOT/'fpga/mister/reports'/('2026-10-19-snapshot-'+folder+'-ff-scan')
 p=root/'archive-manifest.json';p=p if p.exists()else p.with_suffix('.json.gz')
 if p not in added:add(p,'receipt-root/'+root.name+'/'+p.name);added.add(p)
 for n in [version+'/manifest.json',version+'/results.json']+([]if name=='ContB'else[version+'/fixture-metadata.json']):
  row=fmt.receipt_index(root)[n];p=root/row['stored'];add(p,'receipt-root/'+root.name+'/'+row['stored'])
paths={Path(__file__),T/'board_state_fabric.sv',T/'board_state_uio.sv',T/'board_state_fabric_serial.sv',T/'board_state_uio_serial.sv',T/'ff_scan_controller.sv',T/'test_board_state_serial.sv',T/'test_board_state_serial.py',T/'test_serial_state_format.py'}
for module in list(sys.modules.values()):
 filename=getattr(module,'__file__',None)
 if filename:
  p=Path(filename).resolve()
  if p.is_relative_to(ROOT)and p.suffix=='.py':paths.add(p)
for p in sorted(paths):add(p,'tools/'+str(p.relative_to(ROOT)))
history=dict(status='PASS_FINAL_V6',scope='Real scan and stub-domain contract, no native/global snapshot implementation.',
 attempts=[
 dict(version=1,finding='Fixture declaration order rejected by Icarus before simulation; move long-scan wire declarations before initializers.'),
 dict(version=2,finding='Positive passes. Damaged transport-reset ownership is caught by a later commanded-hold assertion; widen the accepted correct diagnostic.'),
 dict(version=3,finding='Expanded positive passes. Busy-overwrite attempted an out-of-range address, so address validation independently rejected it; use a valid address to exercise ownership.'),
 dict(version=4,finding='Busy mutation now rejects. Bad schema acceptance is observed at a later control check because the fixture checked before owner propagation; add two observation cycles.'),
 dict(version=5,finding='Positive and preceding damaged controls reject. One generated early-control mutation lacked a token-separating space; fix mutation syntax.'),
 dict(version=6,finding='Positive12238ticks/141packets/44checks and all19damaged variants pass.')],
 catalog_attempts='V1 found IFU separate ff_rows metadata. V2 caught a duplicate phase append by exact total-bit coverage. V3 includes separately tested IFUM/cache/map RAM and pipelines and passes all1944744bits/385runs. Codec independently checked against cross-byte integer oracle; actual formatted packets drive RTL.',
 limits='No production builder/RBF/FPGA/speed change. Native wrappers/backends/input/ARM/disk barriers and full snapshots remain required.')
(O/'qualification-history.json').write_text(json.dumps(history,indent=2)+'\n')
for row in rows:
 p=O/row['stored'];raw=gzip.decompress(p.read_bytes())if p.suffix=='.gz'else p.read_bytes()
 assert len(raw)==row['raw_bytes']and hashlib.sha256(raw).hexdigest()==row['raw_sha256']
(O/'archive-manifest.json.gz').write_bytes(gzip.compress((json.dumps(rows,indent=2)+'\n').encode(),mtime=0))
r=dict(status='PASS',raw_files=len(rows),raw_bytes=sum(x['raw_bytes']for x in rows),stored_bytes=sum((O/x['stored']).stat().st_size for x in rows))
(O/'archive-readback.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r))
