#!/usr/bin/env python3
"""Fresh source-pinned XML classification for DskEth's small state words."""
import argparse,copy,json,re,subprocess,sys,xml.etree.ElementTree as ET
from pathlib import Path
import generate_dsketh_ff as gen
sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory,constant
COMB={
 'cell_MC1650':dict(p2=1,p14=1),
 'cell_MC10136':dict(ck_en=1,p14=1,p15=1,p2=1,p3=1,p4=1,__VdfgRegularize_hf026c66c_0_0=1,__VdfgRegularize_hf026c66c_0_1=1),
 'cell_SN74LS169':dict(ck_en=1,d=4,p15=1),
 'cell_SN74LS174':dict(ck_en=1,p2=1,p5=1,p7=1,p10=1,p12=1,p15=1),
}
MODES=[('cell_MC1650',None),('cell_MC10136',None),('cell_SN74LS169',None),('cell_SN74LS174',0),('cell_SN74LS174',1)]


def classify(kind,r,m):
 if m!=gen.generate_one(kind)[1]:raise ValueError('unreviewed DskEth FF schema/source')
 expected={f['name']:('sequential',f['width'])for f in m['fields']}
 expected.update({n:('combinational_review',w)for n,w in COMB[kind].items()})
 if kind.startswith('cell_SN'):expected['_unused_pins']=('initial_only_candidate',1)
 decisions=[]
 for row in r['candidates']:
  name=row['name']
  if name not in expected or row['path']!=kind+'.'+name:raise ValueError('extra/duplicate DskEth FF field')
  category,width=expected.pop(name)
  if(row['category'],row['width'],row['bits'],row['unpacked_dimensions'])!=(category,width,width,[]):raise ValueError('DskEth FF field differs:'+name)
  access=next((dict(word=0,lsb=f['lsb'],width=f['width'])for f in m['fields']if f['name']==name),'Pinned combinational expression or zero AND constant')
  decisions.append(dict(path=row['path'],decision='SAVE'if category=='sequential'else'RECOMPUTE',bits=width,access=access))
 if expected:raise ValueError('missing DskEth FF field')
 return dict(status='PASS',modeled_state_bits=m['logical_state_bits'],xml_sha256=r['xml_sha256'],decisions=decisions,scope='Local state word only; no board/global handshake or machine snapshot')


def run(out):
 out.mkdir(parents=True,exist_ok=False);results=[]
 for kind,mode in MODES:
  d=out/(kind+(''if mode is None else'-precap'+str(mode)));d.mkdir()
  cmd=['verilator','--xml-only','--top-module',kind,'-Wno-fatal','--xml-output',str(d/'cell.xml')]+([]if mode is None else['-GPRECAP='+str(mode)])+[str(gen.ROOT/'verilog/cells'/(kind+'.v'))]
  (d/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
  with(d/'elaborate.log').open('w')as f:subprocess.run(cmd,stdout=f,stderr=subprocess.STDOUT,check=True)
  if mode is not None:
   x=ET.fromstring((d/'cell.xml').read_bytes());values=[constant(v.find('const').get('name'))for v in x.findall('.//module/var')if v.get('param')=='true'and v.get('name')=='PRECAP'and v.find('const')is not None]
   assert values==[mode],('actual FF PRECAP binding differs',values,mode)
  r=inventory((d/'cell.xml').read_bytes());_,m=gen.generate_one(kind);q=classify(kind,r,m);controls=[]
  names=['lost_'+f['name']for f in m['fields']]+['wrong_width','new_state','duplicate','derived_state','new_combin','schema_alias','source_hash']
  for name in names:
   report,manifest=copy.deepcopy(r),copy.deepcopy(m)
   if name.startswith('lost_'):report['candidates']=[x for x in report['candidates']if x['name']!=name[5:]]
   elif name=='wrong_width':report['candidates'][0]['width']+=1
   elif name in ('new_state','new_combin'):
    x=copy.deepcopy(report['candidates'][0]);x.update(name='hidden',path=kind+'.hidden',category='sequential'if name=='new_state'else'combinational_review');report['candidates'].append(x)
   elif name=='duplicate':report['candidates'].append(copy.deepcopy(report['candidates'][0]))
   elif name=='derived_state':next(x for x in report['candidates']if x['category']=='combinational_review')['category']='sequential'
   elif name=='schema_alias':manifest['fields'][0]['lsb']+=1
   else:manifest['original_sha256']='0'*64
   try:classify(kind,report,manifest)
   except ValueError as e:controls.append(dict(case=name,rejected=True,diagnostic=str(e)))
   else:raise AssertionError('damaged DskEth FF schema accepted:'+name)
  q.update(kind=kind,precap=mode,failure_controls=controls);results.append(q)
  for name,value in [('inventory.json',r),('manifest.json',m),('classification.json',q)]:(d/name).write_text(json.dumps(value,indent=2)+'\n')
 for n in ('classify_dsketh_ff.py','generate_dsketh_ff.py'):(out/n).write_bytes((Path(__file__).parent/n).read_bytes())
 (out/'results.json').write_text(json.dumps(dict(status='PASS',modes=results),indent=2)+'\n')
 print('DSKETH_FF CLASSIFICATION PASS:4families/5modes, all state fields and padding mapped')

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.output.resolve())
