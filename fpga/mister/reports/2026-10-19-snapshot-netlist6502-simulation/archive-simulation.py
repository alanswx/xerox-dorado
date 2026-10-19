from pathlib import Path
import gzip,hashlib,json,shutil
root=Path('/Users/alans/dev2/Dorado');report=root/'fpga/mister/reports/2026-10-19-snapshot-netlist6502-simulation';report.mkdir(parents=True,exist_ok=False)
def put(src,dst):
 dst.parent.mkdir(parents=True,exist_ok=True);raw=src.read_bytes()
 if src.suffix in('.v','.sv','.xml','.log','.txt')or(src.suffix=='.json'and len(raw)>102400):
  dst=Path(str(dst)+'.gz');dst.write_bytes(gzip.compress(raw,mtime=0));assert gzip.decompress(dst.read_bytes())==raw
 else:dst.write_bytes(raw)
 return dst
for tag,name in [('simulation','snapshot-netlist6502-v1-20261019'),('exploratory-xml','snapshot-netlist6502-classify-v1-20261019'),('classification-first-rejected','snapshot-netlist6502-classify-v2-20261019'),('classification','snapshot-netlist6502-classify-v3-20261019')]:
 d=root/'build/rtl-executive'/name
 for p in sorted(d.rglob('*')):
  if p.is_file()and not any(s in ('obj','__pycache__')for s in p.relative_to(d).parts):put(p,report/tag/p.relative_to(d))
for name in ('generate_6502.py','test_6502.py','test_6502.cpp','classify_6502.py'):
 put(root/'fpga/mister/snapshot'/name,report/'tools'/name)
put(root/'verilog/vendor/6502/chip_6502.v',report/'vendor/chip_6502.v')
put(root/'tools/sil_state_inventory.py',report/'tools/sil_state_inventory.py')
q=json.loads((report/'simulation/results.json').read_text());assert q['status']=='PASS'and len(q['cases'])==9
c=json.loads((report/'classification/classification.json').read_text());assert c['status']=='PASS'and len(c['failure_controls'])==8
hashes={}
for p in sorted(report.rglob('*')):
 if p.is_file():
  b=gzip.decompress(p.read_bytes())if p.suffix=='.gz'else p.read_bytes();hashes[str(p.relative_to(report))]=hashlib.sha256(b).hexdigest()
(report/'archive-inputs.json').write_text(json.dumps(hashes,indent=2)+'\n')
shutil.copyfile(__file__,report/'archive-simulation.py')
print('NETLIST6502 simulation/classification archived, no physical timing claim',len(hashes),'files')
