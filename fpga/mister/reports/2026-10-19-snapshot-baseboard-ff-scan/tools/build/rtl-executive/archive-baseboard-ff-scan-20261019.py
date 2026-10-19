#!/usr/bin/env python3
"""Lossless BaseBoard serial FF/CPU qualification evidence and failed attempts."""
import gzip,hashlib,json,sys
from pathlib import Path
ROOT=Path('/Users/alans/dev2/Dorado');B=ROOT/'build/rtl-executive';T=ROOT/'fpga/mister/snapshot'
sys.path.insert(0,str(T))
import generate_baseboard_ff_scan as gen
import test_baseboard_ff_scan,classify_baseboard_ff_scan,fit_baseboard_ff_scan,audit_baseboard_ff_scan
assert json.loads((B/'baseboard-ff-scan-v4-20261019/results.json').read_text())['status']=='PASS'
assert json.loads((B/'baseboard-ff-scan-fit-audit-v2-20261019.json').read_text())['status']=='PASS_ISOLATED_FIT'
final=json.loads((B/'baseboard-ff-scan-v4-20261019/results.json').read_text())
assert len(final['cases'])==13 and final['cases'][0]['returncode']==0
assert all(r['returncode']!=0 for r in final['cases'][1:])
for path,digest in final['source_hashes'].items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==digest
cpp='executed-inputs/test_baseboard_ff_scan.cpp'
assert (B/'baseboard-ff-scan-v2-20261019'/cpp).read_bytes()==(B/'baseboard-ff-scan-v3-20261019'/cpp).read_bytes()==(B/'baseboard-ff-scan-v4-20261019'/cpp).read_bytes()
for name in ('board.sv','fixture.sv','chip_6502_snapshot.sv'):
 assert (B/'baseboard-ff-scan-v2-20261019/candidate'/name).read_bytes()==(B/'baseboard-ff-scan-v3-20261019/candidate'/name).read_bytes()==(B/'baseboard-ff-scan-v4-20261019/candidate'/name).read_bytes()
history=dict(status='PASS_FINAL_V4',attempts=[
 dict(version=1,status='REJECTED',finding='Original-board physical comparison failed at edge2: generator removed the CPU input/output combinational suffix along with the old admin process. Corrected process boundary and added byte-exact original-suffix guard.'),
 dict(version=2,status='POSITIVE_PASS_CONTROL_TARGET_CORRECTED',finding='Positive full restore passes. The proposed lost-chain-bit mutation changed an unused internal wire slice, so it had no physical effect. Version3 targets a consumed helper serial-input pin instead.'),
 dict(version=3,status='POSITIVE_PASS_DIAGNOSTIC_ORDER_CORRECTED',finding='Positive passes; chain/FF/CPU/ownership/hold/reset controls reject. Reset reentry corruption is caught by full physical-state comparison before the expected later response check. Version4 accepts both correct diagnostics.'),
 dict(version=4,status='PASS',finding='All13 real executions complete: positive and12 damaged variants. Existing compiled binaries are reused only after every original/candidate/helper/fixture/C++/header/oracle byte matches; each is actually executed again with exit status and binary hash recorded.')],
 positive_cpp_and_candidate_rtl_v2_v3_v4_byte_identical=True,
 fit=dict(status='PASS_ISOLATED_FIT',alms_before=6403,alms_after=5564,minimum_slack_ns=0.124,damaged_controls=14,
  audit_packaging='Export named synthesized firmware MIFs separately from Quartus DBs. All four synthesized images in each variant match original assets. Distinguish four RAM OLD_DATA macro entries from four ROM DONT_CARE entries.'),
 limits='No complete-core integration, FPGA deployment, machine snapshot or speed improvement claimed.')
(B/'baseboard-ff-scan-qualification-history-20261019.json').write_text(json.dumps(history,indent=2)+'\n')
O=ROOT/'fpga/mister/reports/2026-10-19-snapshot-baseboard-ff-scan';O.mkdir(exist_ok=False)
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
for v in (1,2,3,4):tree(B/f'baseboard-ff-scan-v{v}-20261019',f'simulation-v{v}')
for v in (1,2):tree(B/f'baseboard-ff-scan-classification-v{v}-20261019',f'classification-v{v}')
tree(B/'baseboard-ff-scan-fit-result-20261019/SnapshotBaseBoardFFScan_20261019','fit')
add(B/'baseboard-ff-scan-fit-audit-v2-20261019.json','fit-audit.json')
add(B/'baseboard-ff-scan-qualification-history-20261019.json','qualification-history.json')
add(B/'serial-helper-conflicts-20261019.json','next-integration-review/helper-conflicts.json')
add(B/'review-serial-helper-conflicts-20261019.py','next-integration-review/review-serial-helper-conflicts.py')
source=B/'native59-snapshot-budget-baseline/src';_,m,_=gen.generate(source)
for n in sorted({gen.SOURCE,'build/rtl-executive/native16-tx-cable/dorado_native_backplane.v',*m['original_sources'],*[r['init_file']for r in m['packages']if r['type']=='EPROM'and r['init_file']]}):add(source/n,'generator-root/'+n)
for n in ('chm/sil/BaseBd-Rev-Am.dm!1_/BaseBd-Rev-Am.wl','chm/sil/BaseBd-Rev-Am.dm!1_/BaseBd-Rev-Am.lc',
 'fpga/mister/reports/2026-10-19-native59-dsketh/compiled-inputs.json',
 'fpga/mister/reports/2026-10-19-native59-dsketh/src/verilog/cells/cell_CLOCKGEN.v.gz'):
 add(ROOT/n,'dependencies/'+n)
paths={Path(__file__),T/'test_baseboard_full.cpp',T/'riot_oracle.hpp',T/'ff_scan_controller.sv',T/'f10415_word_bank.sv',T/'i2125_byte_bank.sv'}
for module in list(sys.modules.values()):
 filename=getattr(module,'__file__',None)
 if filename:
  p=Path(filename).resolve()
  if p.is_relative_to(ROOT)and p.suffix=='.py':paths.add(p)
for p in sorted(paths):add(p,'tools/'+str(p.relative_to(ROOT)))
for row in rows:
 p=O/row['stored'];raw=gzip.decompress(p.read_bytes())if p.suffix=='.gz'else p.read_bytes()
 assert len(raw)==row['raw_bytes']and hashlib.sha256(raw).hexdigest()==row['raw_sha256']
raw=(json.dumps(rows,indent=2)+'\n').encode();(O/'archive-manifest.json.gz').write_bytes(gzip.compress(raw,mtime=0))
r=dict(status='PASS',raw_files=len(rows),raw_bytes=sum(x['raw_bytes']for x in rows),stored_bytes=sum((O/x['stored']).stat().st_size for x in rows))
(O/'archive-readback.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r))
