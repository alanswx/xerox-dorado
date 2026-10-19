from pathlib import Path
from collections import defaultdict
import hashlib,json,subprocess,sys,xml.etree.ElementTree as ET
root=Path('/Users/alans/dev2/Dorado');sys.path.insert(0,str(root/'tools'))
from sil_state_inventory import inventory,constant
source=root/'build/rtl-executive/native58-dsketh-baseline-src';board=source/'build/rtl-executive/native16-tx-cable/BaseBd_nominal.v'
out=root/'build/rtl-executive/snapshot-baseboard-inventory-20261019';out.mkdir(exist_ok=False)
paths=[board,*sorted((source/'verilog/cells').glob('*.v')),source/'verilog/vendor/6502/chip_6502.v',source/'verilog/vendor/riot/M6532.sv']
cmd=['verilator','--xml-only','--top-module','BaseBd_m_Rev_m_Am_nominal','-GSYSPER=4','-Wno-fatal','-DDORADO_F10145_LOGIC','-DDORADO_F10415_M10K','--xml-output',str(out/'baseboard.xml'),*map(str,paths)]
(out/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
with(out/'elaborate.log').open('w')as f:subprocess.run(cmd,cwd=source,stdout=f,stderr=subprocess.STDOUT,check=True)
r=inventory((out/'baseboard.xml').read_bytes());(out/'inventory.json').write_text(json.dumps(r,indent=2)+'\n')
seq=defaultdict(lambda:dict(entries=0,bits=0,fields={},packages=set()))
for row in r['candidates']:
 if row['category']!='sequential':continue
 q=seq[row['module']];q['entries']+=1;q['bits']+=row['bits'];q['fields'][row['name']]=dict(bits=row['bits'],width=row['width'],dimensions=row['unpacked_dimensions']);q['packages'].add(row['path'].rsplit('.',1)[0])
for q in seq.values():q['packages']=sorted(q['packages'])
x=ET.fromstring((out/'baseboard.xml').read_bytes());bindings=[]
for mod in x.findall('.//module'):
 if mod.get('origName')in ('cell_CLOCKGEN','BaseBd_m_Rev_m_Am_nominal','cell_MCS6532','M6532'):
  params={v.get('name'):constant(v.find('const').get('name'))for v in mod.findall('var')if(v.get('param')=='true'or v.get('localparam')=='true')and v.find('const')is not None}
  bindings.append(dict(module=mod.get('name'),parameters=params))
meta=dict(status='REVIEW ONLY',scope='Exact actual Native58 BaseBd_nominal with explicit SYSPER4 and both vendor6502/RIOT dependencies; external wrapper/backends/ARM state excluded',sequential=seq,bindings=bindings)
(out/'sequential-review.json').write_text(json.dumps(meta,indent=2)+'\n')
(out/'provenance.json').write_text(json.dumps(dict(source_hashes={str(p.relative_to(source)):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths},tool_sha256=hashlib.sha256((root/'tools/sil_state_inventory.py').read_bytes()).hexdigest()),indent=2)+'\n')
(out/'inventory-driver.py').write_bytes(Path(__file__).read_bytes());print('BaseBoard sequential bits',sum(q['bits']for q in seq.values()));print(bindings)
