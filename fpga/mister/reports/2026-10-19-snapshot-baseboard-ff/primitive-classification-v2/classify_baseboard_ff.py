#!/usr/bin/env python3
"""Fresh source-pinned XML classification for nine BaseBoard clock/reset families."""
import argparse,copy,hashlib,json,subprocess,sys,xml.etree.ElementTree as ET
from pathlib import Path
import generate_baseboard_ff as gen
sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory,constant
COMB={
 'cell_SN74LS259':dict(sel=3,p5=1,p7=1),
 'cell_SN74LS163':dict(ck_en=1,d=4,p15=1),
 'cell_SN74LS175':dict(ck_en=1,p2=1,p7=1),
 'cell_SN74LS74':dict(cka_en=1,ckb_en=1,p5=1,p6=1,p8=1),
 'cell_MC1690':dict(ck_en=1,p2=1,p3=1),
 'cell_MC12061':dict(p10=1),
 'cell_MC14521B':dict(fall=1,p13=1),
 'cell_MPQ3303':dict(p1=1,p7=1,p14=1,osc=1),
 'cell_CLOCKGEN':dict(EndClockPulse=1,StartClockPulse_n=1)}
CONSTANT={k:['_unused_pins']for k in ('cell_SN74LS259','cell_SN74LS163','cell_SN74LS175','cell_SN74LS74','cell_MC14521B')}
CONSTANT.update(cell_MC12061=['_unused','p2','p3'],cell_MPQ3303=['_unused','p3','p5','p8','p10','p12'])


def classify(kind,report,manifest):
 if manifest!=gen.generate_one(kind)[1]:raise ValueError('unreviewed source/schema')
 expected={f['name']:('sequential',f['width'])for f in manifest['fields']}
 expected.update({n:('combinational_review',w)for n,w in COMB[kind].items()})
 expected.update({n:('initial_only_candidate',1)for n in CONSTANT.get(kind,[])})
 decisions=[]
 for r in report['candidates']:
  name=r['name']
  if name not in expected or r['path']!=kind+'.'+name:raise ValueError('extra/duplicate primitive field')
  cat,w=expected.pop(name)
  if(r['category'],r['width'],r['bits'],r['unpacked_dimensions'])!=(cat,w,w,[]):raise ValueError('field shape/category differs:'+name)
  access=[]
  if cat=='sequential':
   f=next(x for x in manifest['fields']if x['name']==name)
   for word in manifest['words']:
    lo=max(f['lsb'],word['lsb']);hi=min(f['lsb']+w,word['lsb']+word['width'])
    if lo<hi:access.append(dict(word=word['index'],lsb=lo-word['lsb'],width=hi-lo,field_lsb=lo-f['lsb']))
  decisions.append(dict(path=r['path'],decision='SAVE'if access else'IMMUTABLE_ZERO'if name in CONSTANT.get(kind,[])else'RECOMPUTE',bits=w,access=access))
 if expected:raise ValueError('missing primitive field')
 return dict(status='PASS',modeled_state_bits=manifest['logical_state_bits'],xml_sha256=report['xml_sha256'],decisions=decisions,parameters=manifest['parameters'])


def run(out):
 out.mkdir(parents=True,exist_ok=False);results=[]
 for kind in gen.FIELDS:
  d=out/kind;d.mkdir();p=d/(kind+'.v');p.write_bytes(gen.original_source(kind));assert hashlib.sha256(p.read_bytes()).hexdigest()==gen.SHA[kind]
  cmd=['verilator','--xml-only','--top-module',kind,'-Wno-fatal','--xml-output',str(d/'cell.xml')]+(['-GN=4']if kind=='cell_CLOCKGEN'else[])+[str(p)]
  (d/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
  with(d/'elaborate.log').open('w')as f:subprocess.run(cmd,check=True,stdout=f,stderr=subprocess.STDOUT)
  raw=(d/'cell.xml').read_bytes()
  if kind=='cell_CLOCKGEN':
   xml=ET.fromstring(raw);params={v.get('name'):constant(v.find('const').get('name'))for v in xml.findall('.//module/var')if v.get('name')in('N','W','HALF','ECP_LO','SCP_LO')and v.find('const')is not None}
   assert params==dict(N=4,W=2,HALF=2,ECP_LO=0,SCP_LO=1),'actual CLOCKGEN parameter binding differs'
  r=inventory(raw);_,m=gen.generate_one(kind);q=classify(kind,r,m);controls=[]
  for case in ['lost_'+f['name']for f in m['fields']]+['width','extra','duplicate','derived_state','constant_state','schema_alias','source']:
   report,manifest=copy.deepcopy(r),copy.deepcopy(m)
   field=next(x for x in report['candidates']if x['category']=='sequential')
   if case.startswith('lost_'):report['candidates']=[x for x in report['candidates']if x['name']!=case[5:]]
   elif case=='width':field['width']+=1
   elif case=='extra':x=copy.deepcopy(field);x.update(name='hidden',path=kind+'.hidden');report['candidates'].append(x)
   elif case=='duplicate':report['candidates'].append(copy.deepcopy(field))
   elif case=='derived_state':next(x for x in report['candidates']if x['category']=='combinational_review')['category']='sequential'
   elif case=='constant_state':
    if not CONSTANT.get(kind):continue
    next(x for x in report['candidates']if x['name']==CONSTANT[kind][0])['category']='sequential'
   elif case=='schema_alias':manifest['fields'][0]['lsb']+=1
   else:manifest['original_sha256']='0'*64
   try:classify(kind,report,manifest)
   except ValueError as e:controls.append(dict(case=case,rejected=True,diagnostic=str(e)))
   else:raise AssertionError('damaged primitive schema accepted:'+kind+'/'+case)
  q.update(kind=kind,failure_controls=controls);results.append(q)
  for name,value in [('inventory.json',r),('manifest.json',m),('classification.json',q)]:(d/name).write_text(json.dumps(value,indent=2)+'\n')
 for n in ('classify_baseboard_ff.py','generate_baseboard_ff.py'):(out/n).write_bytes((Path(__file__).parent/n).read_bytes())
 (out/'results.json').write_text(json.dumps(dict(status='PASS',families=results,representative_state_bits=70,scope=__doc__),indent=2)+'\n')
 print('BASEBOARD_FF CLASSIFICATION PASS:9families/70representative bits/N4;all fields mapped, damaged schemas reject')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.output.resolve())
