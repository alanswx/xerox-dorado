#!/usr/bin/env python3
"""Bind Control-A MIR capture pins to the qualified 5c65 native source.

Inventory only: the original clock nets remain distinct, including the separate
PrBlock capture. This does not qualify arrival timing, a replacement evaluator,
firmware, or 60 ns operation. No RTL, clock, device or guest state is changed.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import sys

from sil_netlist import load_board
from sil_to_verilog import vname

ROOT=Path(__file__).resolve().parents[1]
BOARD='chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl'
ACTIVE='src/fpga/mister/snapshot_native_conta/ContA_full_snapshot.sv'
MONITOR='src/build/rtl-executive/native16-tx-cable/ContA_monitor.v'
BACKPLANE='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
PACKAGES=['a23','a24','b23','b24','c23','c24','d23','d24','e23','e24','g16']
HALVES=[dict(half='A',q=2,q_n=3,d=7,s=5,r=4,clocks=[6,9]),
        dict(half='B',q=15,q_n=14,d=10,s=12,r=13,clocks=[11,9])]


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def connections(text,pkg,kind):
 matches=list(re.finditer(r'^  '+re.escape(kind)+r' u_'+pkg+r'\s*\((.*?)^  \);',text,re.M|re.S))
 assert len(matches)==1,('package binding',pkg,kind)
 pairs=re.findall(r'\.p(\d+)\(([^()]*)\)',matches[0][1])
 ports={int(k):v.strip()for k,v in pairs};assert len(ports)==len(pairs)
 return ports


def audit(root,native,qualification,verify_sources=True):
 q=json.loads(qualification.read_text())
 assert q['status']=='PASS_NATIVE_OLD_WORD_EARLY_PREASSEMBLY_REVIEW'
 assert q['frequency_hz']==57142857 and q['sysper']==4 and q['state_admin']=='INACTIVE'
 assert len(q['source_hashes'])==503
 if verify_sources:
  for name,digest in q['source_hashes'].items():assert sha(native/name)==digest,('source identity',name)
 active=(native/ACTIVE).read_text();monitor=(native/MONITOR).read_text();backplane=(native/BACKPLANE).read_text()
 assert len(re.findall(r'ContA_full_snapshot_experiment\s+b_ContA\s*\(',backplane))==1
 for signal in ['state_freeze','state_guest_held','state_request','state_write']:
  assert re.search(r'\.'+signal+r"\(1'b0\)",backplane),('state administration',signal)
 board=load_board(str(root/BOARD));pin_net={(p['pkg'],p['pin']):n for n,row in board.nets.items()for p in row['pins']}
 rows=[];clock_groups={}
 for index,pkg in enumerate(PACKAGES):
  assert board.packages[pkg]['type']=='MC10231',pkg
  old=connections(monitor,pkg,'cell_MC10231');live=connections(active,pkg,'cell_MC10231_snapshot')
  assert old==live,('active/monitor pin mismatch',pkg)
  physical={pin:n for (p,pin),n in pin_net.items()if p==pkg}
  assert set(live)==set(physical),('original pin coverage',pkg)
  for pin,net in physical.items():
   base=vname(net);actual=live[pin]
   # Mixed and exported outputs keep the emitter's separate contributions.
   assert actual in (base,base+'__drv',base+f'__{pkg}_{pin}'),('original pin binding',pkg,pin,net,actual)
  halves=[]
  for j,h in enumerate(HALVES):
   clocks=[physical[p]for p in h['clocks']if p in physical]
   key=' | '.join(clocks)if clocks else 'unwired LOW'
   clock_groups.setdefault(key,[]).append(pkg+'.'+h['half'])
   halves.append(dict(half=h['half'],physical_bit=2*index+j,
      q=physical.get(h['q']),q_n=physical.get(h['q_n']),
      data=physical.get(h['d']),async_set=physical.get(h['s']),async_reset=physical.get(h['r']),
      clock_inputs=clocks,unused_outputs=h['q']not in physical and h['q_n']not in physical))
  rows.append(dict(package=pkg,kind='MC10231',native_module=ACTIVE,pins={str(k):dict(net=n,native=live[k])for k,n in physical.items()},halves=halves))
 # Two physical outputs drive JCN.2or3. They are not one stored state bit.
 drivers={(p['pkg'],p['pin'])for p in board.nets['JCN.2or3']['pins']if p['dir']=='out'}
 assert drivers=={('d23',2),('d23',15)},drivers
 assert rows[-1]['halves'][0]['unused_outputs'] and rows[-1]['halves'][1]['clock_inputs']==["h*clk0'Cf"]
 return dict(status='PASS_ORIGINAL_MIR_NATIVE_PIN_INVENTORY',scope=__doc__,
   qualification_sha256=sha(qualification),checked_native_sources=503 if verify_sources else 0,
   physical_Q_bits=22,wired_Q_halves=sum(not h['unused_outputs']for r in rows for h in r['halves']),
   package_rows=rows,clock_groups=clock_groups,
   wired_OR=dict(net='JCN.2or3',drivers=[dict(package=p,pin=i)for p,i in sorted(drivers)]),
   source_hashes={n:sha(native/n)for n in (ACTIVE,MONITOR,BACKPLANE)},
   firmware_or_timing_qualified=False,native_replacement_generated=False)


def run(native,qualification,out):
 assert not out.exists();record=audit(ROOT,native,qualification);out.mkdir(parents=True)
 inputs=out/'inputs';sources={BOARD,BOARD.replace('.wl','.lc')}
 for module in list(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   path=Path(name).resolve()
   if path.parent==ROOT/'tools'and path.suffix=='.py':sources.add(str(path.relative_to(ROOT)))
 hashes={}
 for name in sorted(sources):
  p=inputs/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/name).read_bytes());hashes[name]=sha(p)
 for name in record['source_hashes']:
  p=inputs/'native'/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((native/name).read_bytes());hashes['native/'+name]=sha(p)
 p=inputs/'qualification.json';p.write_bytes(qualification.read_bytes());hashes['qualification.json']=sha(p)
 controls=[]
 original=(native/ACTIVE).read_text()
 for label,pkg,pin in [('wrong_PrBlock_clock','g16',11),('collapsed_JCN_or_driver','d23',15),('wrong_Block_set','e23',12)]:
  row=connections(original,pkg,'cell_MC10231_snapshot');target=row[pin]
  pattern=r'(cell_MC10231_snapshot u_'+pkg+r'\s*\(.*?\.p'+str(pin)+r'\()'+re.escape(target)+r'(\))'
  changed,n=re.subn(pattern,r'\g<1>wrong_producer\g<2>',original,count=1,flags=re.S);assert n==1
  # Bind the real copied board, bypassing ONLY the initial whole-file digest
  # check so this control must reject at the physical pin comparison.
  from unittest.mock import patch
  original_read=Path.read_text
  def read(path,*args,**kwargs):return changed if path==native/ACTIVE else original_read(path,*args,**kwargs)
  with patch.object(Path,'read_text',read):
   try:audit(ROOT,native,qualification,verify_sources=False)
   except AssertionError as error:
    assert 'active/monitor pin mismatch'in str(error),error
    controls.append(dict(case=label,rejected=True,diagnostic=str(error)))
   else:raise AssertionError('wrong physical binding accepted: '+label)
 for name,digest in hashes.items():assert sha(inputs/name)==digest,name
 record.update(inputs=hashes,rejected_controls=controls)
 (out/'inventory.json').write_text(json.dumps(record,indent=2)+'\n')
 print(record['status'],record['physical_Q_bits'],'physical Qs;',record['wired_Q_halves'],'wired halves;',len(controls),'bad bindings rejected')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for name in ('native','qualification','output'):p.add_argument('--'+name,type=Path,required=True)
 a=p.parse_args();run(a.native.resolve(),a.qualification.resolve(),a.output.resolve())
