#!/usr/bin/env python3
"""Fresh XML completeness/type/transport review of both SN74123 timers."""
import argparse,copy,hashlib,json,subprocess,sys,xml.etree.ElementTree as ET
from pathlib import Path
import generate_sn74123 as gen
from test_sn74123 import MODES
sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory,constant


def classify(r,m):
 if m!=gen.generate()[1]:raise ValueError('unreviewed SN74123 schema/source')
 expected={'t1':('sequential',32),'t2':('sequential',32)}
 expected.update({n:('sequential',1)for n in ('a1_d','b1_d','c1_d','a2_d','b2_d','c2_d')})
 expected.update({n:('combinational_review',1)for n in ('p13','p5','trig1','trig2')})
 wanted={n:dict(name='integer',left='31',right='0',signed='true')for n in ('t1','t2')}
 if r['timer_types']!=wanted:raise ValueError('timer signed integer type differs')
 decisions=[]
 for row in r['candidates']:
  n=row['name']
  if n not in expected or row['path']!='cell_SN74123.'+n:raise ValueError('extra/duplicate timer candidate')
  category,width=expected.pop(n)
  if(row['category'],row['width'],row['bits'],row['unpacked_dimensions'])!=(category,width,width,[]):raise ValueError('timer field differs:'+n)
  access='Pinned continuous expression'
  if category=='sequential':
   f=next(f for f in m['fields']if f['name']==n);access=[]
   for word in m['words']:
    a=max(f['lsb'],18*word['index']);b=min(f['lsb']+f['width'],18*word['index']+word['width'])
    if b>a:access.append(dict(word=word['index'],lsb=a-18*word['index'],field_lsb=a-f['lsb'],width=b-a))
   if sum(a['width']for a in access)!=width:raise ValueError('timer field transport gap')
  decisions.append(dict(path=row['path'],decision='SAVE'if category=='sequential'else'RECOMPUTE',bits=width,access=access))
 if expected:raise ValueError('missing timer field')
 return dict(status='PASS',modeled_state_bits=70,xml_sha256=r['xml_sha256'],timer_types=r['timer_types'],decisions=decisions,scope='Complete modeled cell only; no calibrated real time, board/global barrier or machine snapshot')


def run(out):
 out.mkdir(parents=True,exist_ok=False);source=gen.ROOT/'verilog/cells/cell_SN74123.v';results=[]
 for n in ('generate_sn74123.py','classify_sn74123.py','test_sn74123.py'):(out/n).write_bytes((Path(__file__).parent/n).read_bytes())
 for mode in MODES:
  d=out/('sp'+str(mode['sysper'])+'-ns'+str(mode['tw_ns']));d.mkdir()
  cmd=['verilator','--xml-only','--top-module','cell_SN74123','-GSYSPER='+str(mode['sysper']),'-GTW_NS='+str(mode['tw_ns']),'-Wno-fatal','--xml-output',str(d/'cell.xml'),str(source)]
  (d/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
  with(d/'elaborate.log').open('w')as f:subprocess.run(cmd,stdout=f,stderr=subprocess.STDOUT,check=True)
  x=ET.fromstring((d/'cell.xml').read_bytes());types={v.get('id'):v.attrib for v in x.findall('.//typetable/*')}
  bindings={v.get('name'):constant(v.find('const').get('name'))for v in x.findall('.//module/var')if v.get('name')in ('TW_NS','SYSPER','TW')and v.find('const')is not None}
  assert bindings==dict(TW_NS=mode['tw_ns'],SYSPER=mode['sysper'],TW=mode['tw_ns']*mode['sysper']//30),bindings
  r=inventory((d/'cell.xml').read_bytes());r['timer_types']={v.get('name'):{k:types[v.get('dtype_id')].get(k)for k in ('name','left','right','signed')}for v in x.findall('.//module/var')if v.get('name')in ('t1','t2')}
  _,m=gen.generate();q=classify(r,m);controls=[]
  names=['lost_'+n for n,_ in gen.FIELDS]+['wrong_width','new_state','duplicate','derived_state','unsigned_timer','short_timer','schema_alias','schema_signed','source_hash']
  for name in names:
   report,manifest=copy.deepcopy(r),copy.deepcopy(m)
   if name.startswith('lost_'):report['candidates']=[v for v in report['candidates']if v['name']!=name[5:]]
   elif name=='wrong_width':next(v for v in report['candidates']if v['name']=='t1')['width']=31
   elif name=='new_state':
    row=copy.deepcopy(report['candidates'][0]);row.update(name='hidden',path='cell_SN74123.hidden');report['candidates'].append(row)
   elif name=='duplicate':report['candidates'].append(copy.deepcopy(report['candidates'][0]))
   elif name=='derived_state':next(v for v in report['candidates']if v['name']=='trig1')['category']='sequential'
   elif name=='unsigned_timer':report['timer_types']['t1']['signed']='false'
   elif name=='short_timer':report['timer_types']['t2']['left']='30'
   elif name=='schema_alias':manifest['words'][3]['index']=2
   elif name=='schema_signed':manifest['fields'][0]['signed']=False
   else:manifest['original_sha256']='0'*64
   try:classify(report,manifest)
   except ValueError as e:controls.append(dict(case=name,rejected=True,diagnostic=str(e)))
   else:raise AssertionError('damaged timer schema accepted:'+name)
  q.update(parameters=bindings,failure_controls=controls);results.append(q)
  for name,value in [('inventory.json',r),('manifest.json',m),('classification.json',q)]:(d/name).write_text(json.dumps(value,indent=2)+'\n')
 (out/'results.json').write_text(json.dumps(dict(status='PASS',modes=results,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest()),indent=2)+'\n')
 print('SN74123 CLASSIFICATION PASS:70bits/6parameter modes/all signed types/17bad schemas per mode')

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.output.resolve())
