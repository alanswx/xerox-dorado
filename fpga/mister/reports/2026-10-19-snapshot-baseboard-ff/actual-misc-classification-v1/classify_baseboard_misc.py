#!/usr/bin/env python3
"""Fresh actual BaseBoard clock/reset/FF map: all279bits across30packages.

The other192bits reuse Native59's qualified adapters byte-identically; nine
new primitive families cover87actual bits. Partial classification only: CPU,
RIOT, SRAM, EPROM and nonsequential board glue remain in the full inventory.
"""
import argparse,copy,hashlib,json,re
from pathlib import Path
import generate_baseboard_ff as gen
import generate_contb_full as cb
import generate_dsketh_ff as df
import classify_baseboard_ff as classify_new
import classify_dsketh_ff as classify_old
import inventory_baseboard as inv
BOARD_SHA='387f588aed70ff2870c662a51ffecb731ca83e549787bd130e30cd23f83a376e'
PACKAGES={
 'cell_MC10176':['a01'],'cell_MC10136':['b02','k02','k03'],
 'cell_SN74LS174':['f19','f20','g20','h20','k07','k08','k09','l07','l08','l21','l22'],
 'cell_SN74LS259':['c20'],'cell_SN74LS163':['e07'],'cell_SN74LS175':['g07','l18'],
 'cell_SN74LS74':['g08','g22','j08'],'cell_MC1690':['g03','g04','h03','h04'],
 'cell_MC12061':['d08'],'cell_MC14521B':['g21'],'cell_MPQ3303':['h06'],'cell_CLOCKGEN':['clockgen']}
REUSED={
 'cell_MC10176':('src/fpga/mister/snapshot_native/cell_MC10176_snapshot.v','8ddfee8a71b8728d05b1d0abccb42c54fff8e7c0b70f7ff269ad55dcaae3651e'),
 'cell_MC10136':('src/fpga/mister/snapshot_native_dsketh/cell_MC10136_snapshot.sv','ff9d6691af62ffeead8320fbfe8da3c3a2b349166b6c1939086bf8b42ead62c6'),
 'cell_SN74LS174':('src/fpga/mister/snapshot_native_dsketh/cell_SN74LS174_snapshot.sv','96b1a6d13268c158c966d980a75d9aa810b4c1b645314ec7e44588f290e043f3')}


def schema():
 compiled=json.loads((gen.ROOT/'fpga/mister/reports/2026-10-19-native59-dsketh/compiled-inputs.json').read_text())
 fields={**gen.FIELDS,'cell_MC10176':cb.FIELDS['cell_MC10176'],'cell_MC10136':df.FIELDS['cell_MC10136'],'cell_SN74LS174':df.FIELDS['cell_SN74LS174']}
 clones={k:cb.ff_sources()[k+'_snapshot.v']if k=='cell_MC10176'else df.generate_one(k)[0]for k in REUSED}
 for k,(path,expected)in REUSED.items():
  if compiled[path]!=expected or hashlib.sha256(clones[k].encode()).hexdigest()!=expected:raise ValueError('qualified shared adapter changed:'+k)
 rows=[];sources={}
 for k in PACKAGES:
  expected=compiled['src/verilog/cells/'+k+'.v'];source=gen.original_source(k)if k in gen.FIELDS else(gen.ROOT/'verilog/cells'/(k+'.v')).read_bytes()
  if hashlib.sha256(source).hexdigest()!=expected:raise ValueError('actual original cell differs:'+k)
  sources['verilog/cells/'+k+'.v']=expected
  for p in PACKAGES[k]:
   width=sum(w for _,w in fields[k]);offset=0;f=[]
   for name,w in fields[k]:f.append(dict(name=name,width=w,lsb=offset));offset+=w
   rows.append(dict(package='u_'+p,kind=k,index=len(rows),fields=f,logical_state_bits=width,words=gen.chunks(fields[k]),
    parameters=dict(N=4)if k=='cell_CLOCKGEN'else dict(PRECAP=1 if p in('l21','l22')else 0)if k in('cell_SN74LS174','cell_MC10176')else{},
    adapter_sha256=hashlib.sha256(clones[k].encode()).hexdigest()if k in clones else gen.generate_one(k)[1]['generated_sha256']))
 assert len(rows)==30 and sum(r['logical_state_bits']for r in rows)==279
 return dict(board_sha256=BOARD_SHA,packages=rows,source_hashes=sources,reused_adapters={k:dict(native59_path=p,sha256=h)for k,(p,h)in REUSED.items()})


def classify(report,manifest,provenance,review):
 if manifest!=schema():raise ValueError('unreviewed state/socket/schema/source map')
 if provenance['source_hashes']['build/rtl-executive/native16-tx-cable/BaseBd_nominal.v']!=BOARD_SHA:raise ValueError('actual BaseBoard differs')
 for p,h in manifest['source_hashes'].items():
  if provenance['source_hashes'][p]!=h:raise ValueError('compiled original source differs')
 if review['sequential_bits_before_classification']!=40366:raise ValueError('full board inventory count differs')
 if next(x['parameters']for x in review['bindings']if x['module']=='BaseBd_m_Rev_m_Am_nominal')!=dict(SYSPER=4):raise ValueError('actual BaseBoard SYSPER differs')
 if next(x['parameters']for x in review['bindings']if x['module'].startswith('cell_CLOCKGEN'))!=dict(N=4,W=2,HALF=2,ECP_LO=0,SCP_LO=1):raise ValueError('actual CLOCKGEN binding differs')
 by_name={p['package']:p for p in manifest['packages']};expected={p:{f['name']:f for f in r['fields']}for p,r in by_name.items()};seen=set();decisions=[];outside=0
 comb={**classify_new.COMB,**{k:classify_old.COMB[k]for k in('cell_MC10136','cell_SN74LS174')},'cell_MC10176':dict(ck_en=1,p2=1,p3=1,p4=1,p13=1,p14=1,p15=1)}
 const={**classify_new.CONSTANT,'cell_MC10176':['_unused_pins'],'cell_SN74LS174':['_unused_pins']}
 for r in report['candidates']:
  parts=r['path'].split('.');kind=r['module'].split('__')[0]
  if kind not in PACKAGES and not(len(parts)>1 and parts[1]in by_name):outside+=1;continue
  if len(parts)!=3 or parts[0]!='BaseBd_m_Rev_m_Am_nominal'or parts[1]not in by_name or parts[2]!=r['name']:raise ValueError('unreviewed clock/reset scope')
  p,name=parts[1:];row=by_name[p]
  if kind!=row['kind']or r['path']in seen:raise ValueError('module/socket mismatch or duplicate field')
  seen.add(r['path']);access=[]
  if name in expected[p]:
   f=expected[p].pop(name);w=f['width']
   if(r['category'],r['width'],r['bits'],r['unpacked_dimensions'])!=('sequential',w,w,[]):raise ValueError('sequential clock/reset field shape differs')
   for word in row['words']:
    lo=max(f['lsb'],word['lsb']);hi=min(f['lsb']+w,word['lsb']+word['width'])
    if lo<hi:access.append(dict(package_index=row['index'],word=word['index'],lsb=lo-word['lsb'],width=hi-lo,field_lsb=lo-f['lsb']))
   decision='SAVE'
  elif name in comb[kind]:
   if(r['category'],r['width'],r['bits'],r['unpacked_dimensions'])!=('combinational_review',comb[kind][name],comb[kind][name],[]):raise ValueError('derived clock/reset field shape differs')
   decision='RECOMPUTE'
  elif name in const.get(kind,[]):
   if(r['category'],r['width'],r['bits'],r['unpacked_dimensions'])!=('initial_only_candidate',1,1,[]):raise ValueError('constant clock/reset field differs')
   decision='IMMUTABLE_ZERO'
  else:raise ValueError('unreviewed clock/reset field:'+r['path'])
  decisions.append(dict(path=r['path'],decision=decision,bits=r['bits'],access=access))
 if any(expected.values()):raise ValueError('missing clock/reset sequential state')
 if sum(x['bits']for x in decisions if x['decision']=='SAVE')!=279:raise ValueError('clock/reset saved bit count differs')
 return dict(status='PASS',modeled_state_bits=279,packages=30,xml_sha256=report['xml_sha256'],decisions=decisions,other_candidates_retained_but_unclassified=outside,scope=__doc__)


def run(source,out):
 inv.run(source,out);r=json.loads((out/'inventory.json').read_text());p=json.loads((out/'provenance.json').read_text());v=json.loads((out/'sequential-review.json').read_text());m=schema();q=classify(r,m,p,v);controls=[]
 for case in ('missing_counter','missing_history','duplicate','new_state','wrong_width','constant_state','derived_state','source','schema_alias','precap','clock_n','board_sysper'):
  report,manifest,provenance,review=map(copy.deepcopy,(r,m,p,v));target=next(x for x in report['candidates']if x['path']=='BaseBd_m_Rev_m_Am_nominal.u_g21.cnt')
  if case=='missing_counter':report['candidates'].remove(target)
  elif case=='missing_history':report['candidates']=[x for x in report['candidates']if x['path']!='BaseBd_m_Rev_m_Am_nominal.u_g21.ck_d']
  elif case=='duplicate':report['candidates'].append(copy.deepcopy(target))
  elif case=='new_state':target.update(name='hidden',path='BaseBd_m_Rev_m_Am_nominal.u_g21.hidden')
  elif case=='wrong_width':target['width']=23
  elif case in('constant_state','derived_state'):
   name='_unused_pins'if case=='constant_state'else'fall';next(x for x in report['candidates']if x['path']=='BaseBd_m_Rev_m_Am_nominal.u_g21.'+name)['category']='sequential'
  elif case=='source':provenance['source_hashes']['verilog/cells/cell_MC14521B.v']='0'*64
  elif case=='schema_alias':manifest['packages'][1]['index']=0
  elif case=='precap':next(x for x in manifest['packages']if x['package']=='u_l21')['parameters']['PRECAP']=0
  elif case=='clock_n':next(x for x in review['bindings']if x['module'].startswith('cell_CLOCKGEN'))['parameters']['N']=8
  else:next(x for x in review['bindings']if x['module']=='BaseBd_m_Rev_m_Am_nominal')['parameters']['SYSPER']=8
  try:classify(report,manifest,provenance,review)
  except ValueError as e:controls.append(dict(case=case,rejected=True,diagnostic=str(e)))
  else:raise AssertionError('damaged actual clock/reset schema accepted:'+case)
 q['failure_controls']=controls
 for name,value in [('misc-manifest.json',m),('misc-classification.json',q)]:(out/name).write_text(json.dumps(value,indent=2)+'\n')
 for n in('classify_baseboard_misc.py','generate_baseboard_ff.py','classify_baseboard_ff.py'):(out/n).write_bytes((Path(__file__).parent/n).read_bytes())
 print('BASEBOARD MISC CLASSIFICATION PASS:all279bits/30packages;192bits reuse exact Native59 adapters;12bad schemas reject')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--frozen-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.frozen_root.resolve(),a.output.resolve())
