from pathlib import Path
import gzip,hashlib,json,shutil,tarfile
root=Path('/Users/alans/dev2/Dorado');report=root/'fpga/mister/reports/2026-10-19-snapshot-mcs6532';report.mkdir(exist_ok=False)
def put(raw,p):
 p.parent.mkdir(parents=True,exist_ok=True)
 if p.suffix in('.v','.sv','.hpp','.xml','.log','.txt','.rpt','.summary','.sdc','.qsf','.qpf','.tcl','.pin','.smsg')or(p.suffix=='.json'and len(raw)>102400):p=Path(str(p)+'.gz');p.write_bytes(gzip.compress(raw,mtime=0));assert gzip.decompress(p.read_bytes())==raw
 else:p.write_bytes(raw)
for tag,name in [('initial-simulation','snapshot-mcs6532-v1-20261019'),('simulation','snapshot-mcs6532-v2-20261019'),('initial-inventory','snapshot-mcs6532-inventory-20261019'),('classification','snapshot-mcs6532-classify-v1-20261019'),('vendor-source-location-classification','snapshot-riot-classify-v3-20261019')]:
 d=root/'build/rtl-executive'/name
 for p in sorted(d.rglob('*')):
  if p.is_file()and not any(x in('obj','__pycache__')for x in p.relative_to(d).parts):put(p.read_bytes(),report/tag/p.relative_to(d))
with tarfile.open(root/'build/rtl-executive/snapshot-mcs6532-fit-evidence-20261019.tar.gz')as tar:
 for m in tar.getmembers():
  if not m.isfile():continue
  p=Path(m.name);assert not p.is_absolute()and '..'not in p.parts;put(tar.extractfile(m).read(),report/'fit'/p)
for n in ('generate_mcs6532.py','generate_riot.py','test_mcs6532.py','test_mcs6532.cpp','riot_oracle.hpp','test_riot.py','classify_mcs6532.py','classify_riot.py','fit_mcs6532.py','audit_mcs6532.py','fit_f10415.py','fit_f10415_enable.py'):
 put((root/'fpga/mister/snapshot'/n).read_bytes(),report/'tools'/n)
put((root/'tools/sil_state_inventory.py').read_bytes(),report/'tools/sil_state_inventory.py')
for n in ('snapshot-mcs6532-v1-20261019.log','snapshot-mcs6532-v2-20261019.log','snapshot-mcs6532-fit-20261019.log','snapshot-mcs6532-fit-v2-20261019.log','snapshot-mcs6532-audit-v1-20261019.log'):
 put((root/'build/rtl-executive'/n).read_bytes(),report/'local-review'/n)
def read(p):return p.read_bytes()if p.exists()else gzip.decompress(Path(str(p)+'.gz').read_bytes())
fit=report/'fit/SnapshotMcs6532_v2_20261019';m=json.loads(read(fit/'provenance.json'))
for n,h in m['compiled_files'].items():
 if not n.endswith('.qsf'):assert hashlib.sha256(read(fit/n)).hexdigest()==h,n
sim=json.loads(read(report/'simulation/results.json'));assert sim['status']=='PASS'and len(sim['cases'])==9
for n,h in sim['source_hashes'].items():assert hashlib.sha256(read(report/'simulation/executed-inputs'/Path(n).name)).hexdigest()==h,n
q=json.loads(read(fit/'qualification.json'));assert q['status']=='PASS'and len(q['failure_controls'])==10
shutil.copyfile(__file__,report/'archive-evidence.py')
print('MCS6532 source/compiled/qualified evidence verified',len(m['compiled_files']),'inputs',q['minimum_snapshot_slack_ns'],'ns')
