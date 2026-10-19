from pathlib import Path
import gzip,hashlib,json,shutil,tarfile
root=Path('/Users/alans/dev2/Dorado');base=root/'build/rtl-executive/cedar59-launch';report=root/'fpga/mister/reports/2026-10-19-native59-field/desktop';report.mkdir(exist_ok=False)
def put(raw,p):
 p.parent.mkdir(parents=True,exist_ok=True)
 if p.suffix in('.log','.jsonl')or(p.suffix=='.json'and len(raw)>102400):p=Path(str(p)+'.gz');p.write_bytes(gzip.compress(raw,mtime=0))
 else:p.write_bytes(raw)
with tarfile.open(base/'desktop-collection.tar.gz')as tar:
 for m in tar.getmembers():
  if not m.isfile():continue
  p=Path(m.name);assert not p.is_absolute()and '..'not in p.parts;put(tar.extractfile(m).read(),report/p)
for name in ('collect-desktop.py','collect-desktop.log','desktop-qualification.json','mouse-to-cmd.log','click-cmd.log','focus-second-cmd.log','keyboard-short.log','keyboard-and-marker.log','tmpfs-retired58-trace-cleanup.json'):
 put((base/name).read_bytes(),report/name)
put(Path('/tmp/clean-native58-retired-traces-20261019.py').read_bytes(),report/'clean-retired58-traces.py')
for name in ('dorado59-desktop-latest-20261019.png','dorado59-mouse-on-cmd-20261019.png','dorado59-second-cmd-check-20261019.png','dorado59-keyboard-short-20261019.png','dorado59-marker-check-20261019.png','dorado59-marker-settled-20261019.png'):
 put((Path('/tmp')/name).read_bytes(),report/'captures'/name)
for name in ('fpga/mister/reports/2026-10-18-native34/input-mouse.py','fpga/mister/reports/2026-10-18-native34/input-keyboard.py','build/rtl-executive/cedar36-launch/type-keys-punct.py'):
 put((root/name).read_bytes(),report/'input-tools'/Path(name).name)
q=json.loads((report/'desktop-collection.json').read_text())
for name,r in q['files'].items():
 p=report/name;b=p.read_bytes()if p.exists()else gzip.decompress(Path(str(p)+'.gz').read_bytes());assert hashlib.sha256(b).hexdigest()==r['sha256'],name
shutil.copyfile(__file__,report/'archive-evidence.py')
print('DESKTOP collection verified',q['completed_disk_trace_records'],'complete records; partial',q['partial_final_record'])
