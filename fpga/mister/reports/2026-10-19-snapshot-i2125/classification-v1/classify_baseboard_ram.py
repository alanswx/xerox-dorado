#!/usr/bin/env python3
"""Fresh actual BaseBoard XML: map all32 original SRAMs into audited byte words.

RAM/read-pipeline scope only. Other board/CPU/RIOT/clock state remains outside
this classification; the full inventory is retained without dropping it.
"""
import argparse,copy,hashlib,json
from pathlib import Path
import audit_baseboard_ram as grouping
import inventory_baseboard as inv

BOARD_SHA='387f588aed70ff2870c662a51ffecb731ca83e549787bd130e30cd23f83a376e'


def classify(report,manifest):
 if manifest['source_sha256']!=BOARD_SHA or manifest['cell_sha256']!=grouping.CELL_SHA or manifest['wirelist_sha256']!=grouping.SIL_SHA:
  raise ValueError('unreviewed BaseBoard SRAM source/schema')
 groups=manifest['groups'];members={m['instance']:(g,m)for g in groups for m in g['members']}
 if len(groups)!=4 or len(members)!=32:raise ValueError('RAM group membership differs')
 # The generator's independently grounded pin proof is the schema oracle.
 for g in groups:
  if g['index']not in range(4)or [m['bit']for m in g['members']]!=list(range(8)):
   raise ValueError('nonunique/missing SRAM plane')
 expected={name:{'mem':('sequential',1,1024,[[0,1023]]),'dout_r':('sequential',1,1,[]),
  'a':('combinational_review',10,10,[]),'p7':('combinational_review',1,1,[]),'_unused_pins':('initial_only_candidate',1,1,[])}for name in members}
 decisions=[];outside=0
 for r in report['candidates']:
  if r['module']!='cell_i2125':outside+=1;continue
  parts=r['path'].split('.')
  if len(parts)!=3 or parts[0]!='BaseBd_m_Rev_m_Am_nominal'or parts[1]not in members or parts[2]!=r['name']:
   raise ValueError('unreviewed SRAM instance/path')
  package=parts[1];name=r['name']
  if name not in expected[package]:raise ValueError('extra/duplicate SRAM field')
  wanted=expected[package].pop(name)
  if(r['category'],r['width'],r['bits'],r['unpacked_dimensions'])!=wanted:raise ValueError('SRAM field shape/category differs:'+r['path'])
  g,m=members[package];access=None
  if name=='mem':access=dict(bank=g['index'],word_range=[0,1023],word_bit=m['bit'],width=1)
  elif name=='dout_r':access=dict(bank=g['index'],word=1024,word_bit=m['bit'],width=1)
  decisions.append(dict(path=r['path'],bits=r['bits'],decision='SAVE'if access else'IMMUTABLE_ZERO'if name=='_unused_pins'else'RECOMPUTE',access=access))
 if any(v for v in expected.values()):raise ValueError('missing SRAM fields')
 if sum(x['bits']for x in decisions if x['decision']=='SAVE')!=32800:raise ValueError('saved SRAM bits differ')
 # Check plane slots independently of path names to reject a schema alias.
 slots=[(g['index'],m['bit'])for g in groups for m in g['members']]
 if len(set(slots))!=32 or set(slots)!={(i,b)for i in range(4)for b in range(8)}:raise ValueError('aliased SRAM word map')
 return dict(status='PASS',scope=__doc__,xml_sha256=report['xml_sha256'],
  modeled_ram_bits=32768,registered_read_bits=32,modeled_state_bits=32800,
  decisions=decisions,other_candidates_retained_but_unclassified=outside,
  access='Four banks; local words0..1023RAM,1024raw pipeline; each local1025..2047 rejects. Word bit i is MCD_i.')


def controls(report,manifest):
 rows=[]
 for name in ('missing_ram','missing_pipeline','wrong_depth','wrong_width','derived_state','constant_state','duplicate','new_state','plane_alias','bank_alias','source'):
  r,m=copy.deepcopy(report),copy.deepcopy(manifest)
  target=next(x for x in r['candidates']if x['path']=='BaseBd_m_Rev_m_Am_nominal.u_b11.mem')
  if name=='missing_ram':r['candidates'].remove(target)
  elif name=='missing_pipeline':r['candidates']=[x for x in r['candidates']if x['path']!='BaseBd_m_Rev_m_Am_nominal.u_b11.dout_r']
  elif name=='wrong_depth':target['unpacked_dimensions']=[[0,1022]]
  elif name=='wrong_width':target['width']=2
  elif name in ('derived_state','constant_state'):
   field='a'if name=='derived_state'else'_unused_pins'
   next(x for x in r['candidates']if x['path']=='BaseBd_m_Rev_m_Am_nominal.u_b11.'+field)['category']='sequential'
  elif name=='duplicate':r['candidates'].append(copy.deepcopy(target))
  elif name=='new_state':
   x=copy.deepcopy(target);x['name']='hidden';x['path']='BaseBd_m_Rev_m_Am_nominal.u_b11.hidden';r['candidates'].append(x)
  elif name=='plane_alias':m['groups'][0]['members'][1]['bit']=0
  elif name=='bank_alias':m['groups'][1]['index']=0
  else:m['cell_sha256']='0'*64
  try:classify(r,m)
  except ValueError as e:rows.append(dict(case=name,rejected=True,diagnostic=str(e)))
  else:raise AssertionError('damaged SRAM classification accepted:'+name)
 return rows


def run(source,out):
 inv.run(source,out)
 report=json.loads((out/'inventory.json').read_text());m=grouping.audit(source)
 # Source/cell evidence is checked again against the exact compiled files.
 p=json.loads((out/'provenance.json').read_text())
 assert p['source_hashes'][m['source']]==BOARD_SHA
 assert p['source_hashes']['verilog/cells/cell_i2125.v']==grouping.CELL_SHA
 q=classify(report,m);q['failure_controls']=controls(report,m)
 m['failure_controls']=grouping.controls(source)
 for name,data in [('ram-grouping.json',m),('ram-classification.json',q)]:
  (out/name).write_text(json.dumps(data,indent=2)+'\n')
 for name in ('audit_baseboard_ram.py','classify_baseboard_ram.py'):
  (out/name).write_bytes((Path(__file__).parent/name).read_bytes())
 print('BASEBOARD SRAM CLASSIFICATION PASS:all32memories/32raw reads/32800bits;11bad schemas+11bad grouping controls reject')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--frozen-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
 a=p.parse_args();run(a.frozen_root.resolve(),a.output.resolve())
