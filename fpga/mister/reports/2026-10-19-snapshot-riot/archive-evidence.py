from pathlib import Path
import gzip,hashlib,json,shutil,tarfile
root=Path('/Users/alans/dev2/Dorado');report=root/'fpga/mister/reports/2026-10-19-snapshot-riot';report.mkdir(exist_ok=False)
def put(raw,p):
 p.parent.mkdir(parents=True,exist_ok=True)
 if p.suffix in('.v','.sv','.xml','.log','.txt','.rpt','.summary','.sdc','.qsf','.qpf','.tcl','.pin','.smsg')or(p.suffix=='.json'and len(raw)>102400):p=Path(str(p)+'.gz');p.write_bytes(gzip.compress(raw,mtime=0));assert gzip.decompress(p.read_bytes())==raw
 else:p.write_bytes(raw)
 return p
for tag,name in [('first-fixture-rejected','snapshot-riot-v1-20261019'),('simulation','snapshot-riot-v2-20261019'),('classification-first-rejected','snapshot-riot-classify-v1-20261019'),('classification','snapshot-riot-classify-v2-20261019')]:
 d=root/'build/rtl-executive'/name
 for p in sorted(d.rglob('*')):
  if p.is_file()and not any(x in('obj','__pycache__')for x in p.relative_to(d).parts):put(p.read_bytes(),report/tag/p.relative_to(d))
with tarfile.open(root/'build/rtl-executive/snapshot-riot-fit-evidence-20261019.tar.gz')as tar:
 for m in tar.getmembers():
  if not m.isfile():continue
  p=Path(m.name);assert not p.is_absolute()and '..'not in p.parts;put(tar.extractfile(m).read(),report/'fit'/p)
for n in ('generate_riot.py','test_riot.py','test_riot.cpp','classify_riot.py','fit_riot.py','audit_riot.py','fit_f10415.py','fit_f10415_enable.py'):
 p=root/'fpga/mister/snapshot'/n;put(p.read_bytes(),report/'tools'/n)
put((root/'tools/sil_state_inventory.py').read_bytes(),report/'tools/sil_state_inventory.py')
put((root/'verilog/vendor/riot/M6532.sv').read_bytes(),report/'vendor/M6532.sv')
for n in ('snapshot-riot-v1-20261019.log','snapshot-riot-v2-20261019.log','snapshot-riot-fit-20261019.log','snapshot-riot-audit-v1-20261019.log'):
 put((root/'build/rtl-executive'/n).read_bytes(),report/'local-review'/n)
def read(p):return p.read_bytes()if p.exists()else gzip.decompress(Path(str(p)+'.gz').read_bytes())
fit=report/'fit/SnapshotRiot_20261019';provenance=json.loads(read(fit/'provenance.json'))
for n,h in provenance['compiled_files'].items():
 if not n.endswith('.qsf'):assert hashlib.sha256(read(fit/n)).hexdigest()==h,n
sim=json.loads(read(report/'simulation/results.json'));assert sim['status']=='PASS'and len(sim['cases'])==10
assert hashlib.sha256(read(report/'vendor/M6532.sv')).hexdigest()==sim['manifest']['original_sha256']
for n,h in sim['source_hashes'].items():
 p=report/'vendor/M6532.sv'if n.endswith('/M6532.sv')else report/'simulation'/Path(n).name
 assert hashlib.sha256(read(p)).hexdigest()==h,n
qual=json.loads(read(fit/'qualification.json'));assert qual['status']=='PASS'and len(qual['failure_controls'])==8
assert qual['state_retention']['snapshot']['synthesis_registers']==1118
shutil.copyfile(__file__,report/'archive-evidence.py')
print('RIOT evidence readback verified:',len(provenance['compiled_files']),'fitinputs,10simcases,8fitcontrols,slack',qual['minimum_snapshot_slack_ns'])
