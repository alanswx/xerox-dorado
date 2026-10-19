from pathlib import Path
import gzip,hashlib,json,shutil
root=Path('/Users/alans/dev2/Dorado');source=root/'build/rtl-executive/native58-dsketh-baseline-src';report=root/'fpga/mister/reports/2026-10-19-snapshot-baseboard-inventory'
report.mkdir(parents=True,exist_ok=False)
def put(src,dst):
 dst.parent.mkdir(parents=True,exist_ok=True);raw=src.read_bytes()
 if src.suffix in ('.v','.sv','.xml','.log','.mem')or(src.suffix=='.json'and len(raw)>102400):
  dst=Path(str(dst)+'.gz');dst.write_bytes(gzip.compress(raw,mtime=0));assert gzip.decompress(dst.read_bytes())==raw
 else:dst.write_bytes(raw)
 return dst
for tag,folder in [('initial','snapshot-baseboard-inventory-20261019'),('reproducible','snapshot-baseboard-inventory-v2-20261019')]:
 d=root/'build/rtl-executive'/folder
 for p in sorted(d.iterdir()):
  if p.is_file():put(p,report/tag/p.name)
p=root/'build/rtl-executive/snapshot-baseboard-inventory-v2-20261019/provenance.json';meta=json.loads(p.read_text());stored={}
for name,wanted in meta['source_hashes'].items():
 p=source/name;assert hashlib.sha256(p.read_bytes()).hexdigest()==wanted
 dest=put(p,report/'compiled-source'/name);stored[name]=dict(file=str(dest.relative_to(report)),sha256=wanted)
for name in ('tools/sil_state_inventory.py','fpga/mister/snapshot/inventory_baseboard.py'):
 p=root/name;dest=put(p,report/'tools'/p.name);stored[name]=dict(file=str(dest.relative_to(report)),sha256=hashlib.sha256(p.read_bytes()).hexdigest())
(report/'compiled-inputs.json').write_text(json.dumps(stored,indent=2)+'\n')
for name,row in stored.items():
 p=report/row['file'];b=gzip.decompress(p.read_bytes())if p.suffix=='.gz'else p.read_bytes();assert hashlib.sha256(b).hexdigest()==row['sha256']
shutil.copyfile(__file__,report/'archive-inventory.py')
print('ARCHIVE VERIFIED',len(stored),'compiled/tool inputs')
