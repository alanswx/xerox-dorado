from pathlib import Path
import gzip,json,tarfile,shutil
root=Path('/Users/alans/dev2/Dorado');base=root/'build/rtl-executive/cedar59-launch';report=root/'fpga/mister/reports/2026-10-19-native59-field';report.mkdir(exist_ok=False)
def put(raw,dst):
 dst.parent.mkdir(parents=True,exist_ok=True)
 if dst.suffix in('.log','.jsonl','.gdb')or(dst.suffix=='.json'and len(raw)>102400):dst=Path(str(dst)+'.gz');dst.write_bytes(gzip.compress(raw,mtime=0));assert gzip.decompress(dst.read_bytes())==raw
 else:dst.write_bytes(raw)
with tarfile.open(base/'deployment-evidence.tar.gz')as tar:
 for m in tar.getmembers():
  if not m.isfile():continue
  p=Path(m.name);assert not p.is_absolute()and '..'not in p.parts;put(tar.extractfile(m).read(),report/'startup'/p)
for name in ('deploy.py','deploy.log','collect-deployment.py','collect-deployment.log','setup-core.gdb','setup-core.log','physical-capability.json','c-until-request.py','cool-boot.py','cool-boot.log'):
 p=base/name;put(p.read_bytes(),report/'startup'/name)
for name in ('dorado58-before-native59-20261019.png','dorado58-normal-boot-20261019.png','dorado59-original-boot-20261019.png'):
 p=Path('/tmp')/name;put(p.read_bytes(),report/'captures'/name)
shutil.copyfile(__file__,report/'archive-field.py')
q=json.loads((report/'startup/deployment-collection.json').read_text());assert q['main_pid']==29511 and q['retired_processes_absent']
print('Native59 field startup archived; retired58diskrecords',q['retired_disk_records'],'pack',q['old_pack_hash_reverified'])
