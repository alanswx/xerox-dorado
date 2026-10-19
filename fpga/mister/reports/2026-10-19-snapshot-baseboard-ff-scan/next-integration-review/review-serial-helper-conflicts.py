import hashlib,json,re
from collections import defaultdict
from pathlib import Path
B=Path('/Users/alans/dev2/Dorado/build/rtl-executive')
fits={
 'ContA':B/'conta-ff-scan-fit-result-20261019/SnapshotContAFFScanV2_20261019',
 'ContB':B/'contb-ff-scan-fit-result-20261019/SnapshotContBFFScan_20261019',
 'ProcH':B/'processor-ff-scan-archive-readback-20261019/ProcH/fit',
 'ProcL':B/'processor-ff-scan-archive-readback-20261019/ProcL/fit',
 'IFU':B/'ifu-ff-scan-fit-result-20261019/SnapshotIFUFFScan_20261019',
 'MemC':B/'memc-ff-scan-fit-result-20261019/SnapshotMemCFFScan_20261019',
 'MemD':B/'memd-ff-scan-fit-result-20261019/SnapshotMemDFFScan_20261019',
 'MemX':B/'memx-ff-scan-fit-result-20261019/SnapshotMemXFFScan_20261019',
 'DskEth':B/'dsketh-ff-scan-fit-result-20261019/SnapshotDskEthFFScan_20261019',
 'DispY':B/'dispy-ff-scan-fit-result-20261019/SnapshotDispYFFScan_20261019',
 'BaseBd':B/'baseboard-ff-scan-fit-result-20261019/SnapshotBaseBoardFFScan_20261019'}
mods=defaultdict(list)
for board,root in fits.items():
 record=json.loads((root/'provenance.json').read_text());manifest=record['manifest']
 for name in manifest['cloned_sources']:
  p=root/'snapshot'/name;raw=p.read_bytes();digest=hashlib.sha256(raw).hexdigest()
  assert record['compiled_files']['snapshot/'+name]==digest,(board,name)
  for module in re.findall(r'^\s*module\s+(\w+)\b',raw.decode(),re.M):mods[module].append(dict(board=board,file=name,sha256=digest))
conflicts={name:rows for name,rows in mods.items()if len({r['sha256']for r in rows})>1}
report=dict(scope='Actual compiled isolated serial helper definitions; no core integration or capacity prediction.',boards=list(fits),definitions=len(mods),conflicts=conflicts)
(B/'serial-helper-conflicts-20261019.json').write_text(json.dumps(report,indent=2)+'\n')
print('modules',len(mods),'conflicts',len(conflicts))
for name,rows in conflicts.items():print(name,[(r['board'],r['sha256'][:12])for r in rows])
