#!/usr/bin/env python3
"""Fresh actual BaseBoard XML: classify eight EPROM pipelines and immutable images.

Empty INIT_FILE sockets retain the original uninitialized ROM semantics. Their
unassigned arrays are not emitted as inventory candidates; they are NOT zero
images. All other board candidates remain outside this partial classification.
"""
import argparse,copy,hashlib,json,re
from pathlib import Path
import generate_i2716 as gen
import inventory_baseboard as inv
BOARD_SHA='387f588aed70ff2870c662a51ffecb731ca83e549787bd130e30cd23f83a376e'


def sockets():
 return [dict(package='u_'+p,init_file='verilog/proms/eprom/BaseBd-'+p+'.mem'if p in gen.IMAGES else None,sha256=gen.IMAGES.get(p))for p in gen.SOCKETS]


def classify(report,review,provenance):
 if provenance['source_hashes']['build/rtl-executive/native16-tx-cable/BaseBd_nominal.v']!=BOARD_SHA:raise ValueError('unreviewed BaseBoard source')
 if provenance['source_hashes']['verilog/cells/cell_i2716.v']!=gen.SHA:raise ValueError('unreviewed 2716 source')
 expected_sockets=sockets()
 if review['eprom_sockets']!=expected_sockets:raise ValueError('EPROM socket/image map differs')
 for s in expected_sockets:
  if s['init_file']and provenance['source_hashes'][s['init_file']]!=s['sha256']:raise ValueError('EPROM image bytes differ')
 expected={s['package']:{'d':('sequential',8,8,[]),'en_q':('sequential',1,1,[]),
  'a':('combinational_review',11,11,[]),'_unused_pins':('initial_only_candidate',1,1,[]),
  **{p:('combinational_review',1,1,[])for p in ('p9','p10','p11','p13','p14','p15','p16','p17')},
  **({'mem':('initial_only_candidate',8,16384,[[0,2047]])}if s['init_file']else{})}for s in expected_sockets}
 decisions=[];outside=0
 for r in report['candidates']:
  if not r['module'].startswith('cell_i2716'):outside+=1;continue
  parts=r['path'].split('.')
  if len(parts)!=3 or parts[0]!='BaseBd_m_Rev_m_Am_nominal'or parts[1]not in expected or parts[2]!=r['name']:raise ValueError('unreviewed EPROM instance/path')
  pkg,name=parts[1:];fields=expected[pkg]
  if name not in fields:raise ValueError('extra/duplicate EPROM field')
  wanted=fields.pop(name)
  if(r['category'],r['width'],r['bits'],r['unpacked_dimensions'])!=wanted:raise ValueError('EPROM field shape/category differs')
  access=dict(socket=gen.SOCKETS.index(pkg[2:]),word=0,lsb=0 if name=='d'else 8,width=r['width'])if name in ('d','en_q')else None
  decisions.append(dict(path=r['path'],bits=r['bits'],decision='SAVE'if access else'IMMUTABLE_IMAGE'if name=='mem'else'IMMUTABLE_ZERO'if name=='_unused_pins'else'RECOMPUTE',access=access))
 if any(expected.values()):raise ValueError('missing EPROM fields')
 if sum(x['bits']for x in decisions if x['decision']=='SAVE')!=72:raise ValueError('EPROM saved bits differ')
 return dict(status='PASS',scope=__doc__,xml_sha256=report['xml_sha256'],modeled_state_bits=72,immutable_image_bits=65536,
  decisions=decisions,empty_sockets=[s['package']for s in expected_sockets if not s['init_file']],
  empty_memory='Original uninitialized readonly arrays, no zero assumption; source pinned; absent from writer-based inventory.',
  other_candidates_retained_but_unclassified=outside,sockets=expected_sockets)


def controls(report,review,provenance):
 rows=[]
 for case in ('missing_d','missing_enable','wrong_width','new_state','duplicate','derived_state','constant_state','rom_state','rom_missing','socket_image_swap','empty_zero','source','image_bytes'):
  r,v,p=copy.deepcopy(report),copy.deepcopy(review),copy.deepcopy(provenance)
  target=next(x for x in r['candidates']if x['path']=='BaseBd_m_Rev_m_Am_nominal.u_b61.d')
  if case=='missing_d':r['candidates'].remove(target)
  elif case=='missing_enable':r['candidates']=[x for x in r['candidates']if x['path']!='BaseBd_m_Rev_m_Am_nominal.u_b61.en_q']
  elif case=='wrong_width':target['width']=7
  elif case=='new_state':target['name']='hidden';target['path']='BaseBd_m_Rev_m_Am_nominal.u_b61.hidden'
  elif case=='duplicate':r['candidates'].append(copy.deepcopy(target))
  elif case in ('derived_state','constant_state','rom_state','rom_missing'):
   name={'derived_state':'a','constant_state':'_unused_pins','rom_state':'mem','rom_missing':'mem'}[case]
   t=next(x for x in r['candidates']if x['path']=='BaseBd_m_Rev_m_Am_nominal.u_b61.'+name)
   if case=='rom_missing':r['candidates'].remove(t)
   else:t['category']='sequential'
  elif case=='socket_image_swap':v['eprom_sockets'][1]['init_file']=v['eprom_sockets'][3]['init_file']
  elif case=='empty_zero':v['eprom_sockets'][0]['init_file']='zeros.mem'
  elif case=='source':p['source_hashes']['verilog/cells/cell_i2716.v']='0'*64
  elif case=='image_bytes':p['source_hashes']['verilog/proms/eprom/BaseBd-b61.mem']='0'*64
  try:classify(r,v,p)
  except ValueError as e:rows.append(dict(case=case,rejected=True,diagnostic=str(e)))
  else:raise AssertionError('damaged EPROM classification accepted:'+case)
 return rows


def run(source,out):
 inv.run(source,out);gen.images(source)
 r=json.loads((out/'inventory.json').read_text());v=json.loads((out/'sequential-review.json').read_text());p=json.loads((out/'provenance.json').read_text())
 q=classify(r,v,p);q['failure_controls']=controls(r,v,p)
 (out/'eprom-classification.json').write_text(json.dumps(q,indent=2)+'\n')
 for n in ('classify_baseboard_eprom.py','generate_i2716.py'):(out/n).write_bytes((Path(__file__).parent/n).read_bytes())
 print('EPROM CLASSIFICATION PASS:8pipelines/72bits/4images/4uninitialized;13bad schemas reject')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--frozen-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.frozen_root.resolve(),a.output.resolve())
