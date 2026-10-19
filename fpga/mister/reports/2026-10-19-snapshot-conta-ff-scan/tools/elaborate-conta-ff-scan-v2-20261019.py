import json,sys,subprocess
from pathlib import Path
sys.path.insert(0,'fpga/mister/snapshot');sys.path.insert(0,'tools')
import generate_conta_ff_scan as g
from sil_state_inventory import inventory
r=Path('build/rtl-executive/native59-snapshot-budget-baseline/src').resolve();s=r/'build/rtl-executive/native16-tx-cable/ContA_monitor.v'
o=Path('build/rtl-executive/conta-ff-scan-xml-v2-20261019').resolve();o.mkdir(exist_ok=False)
b,m,c=g.generate(s,async_files=True)
(o/'ContA_snapshot.sv').write_text(b)
for n,t in c.items():(o/n).write_text(t)
cmd=['verilator','--xml-only','--top-module',g.MODULE,'-GSYSPER=4','-DDORADO_F10145_LOGIC','-Wno-fatal','--xml-output',str(o/'board.xml'),str(o/'ContA_snapshot.sv'),*[str(o/n)for n in c],*[str(r/'verilog/cells'/(n+'.v'))for n in m['original_sources']]]
(o/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
with(o/'elaborate.log').open('w')as f:subprocess.run(cmd,stdout=f,stderr=subprocess.STDOUT,check=True)
v=inventory((o/'board.xml').read_bytes());(o/'inventory.json').write_text(json.dumps(v,indent=2)+'\n');(o/'manifest.json').write_text(json.dumps(m,indent=2)+'\n')
for x in v['candidates']:
 if x['category']=='sequential' and (x['path'].startswith(g.MODULE+'.u_ff_scan.')or x['path'].count('.')==1):print(x['path'],x['width'],x['bits'])
print('Sequential total',sum(x['bits']for x in v['candidates']if x['category']=='sequential'))
