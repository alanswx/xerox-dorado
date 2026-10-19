#!/usr/bin/env python3
"""Fresh complete original-wrapper inventory, phase plus actual MOS RIOT."""
import argparse,copy,hashlib,json,subprocess,sys,xml.etree.ElementTree as ET
from pathlib import Path
import generate_mcs6532 as gen
from classify_riot import classify as classify_child
sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory,constant


def classify(report,manifest):
 if manifest!=gen.generate()[1]:raise ValueError('unreviewed MCS6532 source/schema')
 expected={'p'+str(i):('combinational_review',1)for i in list(range(8,20))+list(range(21,34))}
 expected.update(phase_d=('sequential',1),phase_rise=('combinational_review',1),_unused=('initial_only_candidate',1))
 decisions=[];child=[];mapping=[]
 for row in report['candidates']:
  if row['module']=='cell_MCS6532':
   name=row['name']
   if row['path']!='cell_MCS6532.'+name or name not in expected:raise ValueError('extra/duplicate wrapper field')
   cat,width=expected.pop(name)
   if(row['category'],row['width'],row['bits'],row['unpacked_dimensions'])!=(cat,width,width,[]):raise ValueError('wrapper field differs:'+name)
   if Path(row['source']).resolve()!=gen.SOURCE.resolve():raise ValueError('wrapper declaration source differs')
   decision='SAVE'if name=='phase_d'else'IMMUTABLE_ZERO'if name=='_unused'else'RECOMPUTE'
   decisions.append(dict(path=row['path'],decision=decision,bits=width,access=dict(index=133,lsb=0,width=1)if name=='phase_d'else None))
  elif row['module']=='M6532'and row['path'].startswith('cell_MCS6532.u_riot.'):
   r=copy.deepcopy(row);r['path']='M6532.'+r['name'];child.append(r);mapping.append(dict(original=row['path'],primitive=r['path']))
  else:raise ValueError('unreviewed wrapper child/scope')
 if expected:raise ValueError('missing wrapper fields:'+','.join(expected))
 result=classify_child(dict(candidates=child,xml_sha256=report['xml_sha256']),manifest['child_manifest'])
 assert result['modeled_state_bits']==1097
 # Source-pinned original expressions establish the constant and init,
 # rather than treating arbitrary initial-only fields as immutable.
 source=gen.SOURCE.read_text()
 assert "wire _unused = &{1'b0, p1, p20, 1'b0};"in source and 'reg phase_d=0;'in source
 return dict(status='PASS',modeled_state_bits=1098,xml_sha256=report['xml_sha256'],wrapper=decisions,
  child=result,child_paths=mapping,scope='Actual wrapper phase_d plus all1097RIOT bits. Phase133, vendor0..132, invalid134..255; no complete BaseBoard/global save/load.')


def run(out):
 out.mkdir(parents=True,exist_ok=False)
 cmd=['verilator','--xml-only','--top-module','cell_MCS6532','-Wno-fatal','--xml-output',str(out/'original.xml'),str(gen.SOURCE),str(gen.riot.SOURCE)]
 (out/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
 with(out/'elaborate.log').open('w')as f:subprocess.run(cmd,check=True,stdout=f,stderr=subprocess.STDOUT)
 raw=(out/'original.xml').read_bytes();xml=ET.fromstring(raw)
 params=[constant(v.find('const').get('name'))for v in xml.findall('.//module/var')if v.get('name')=='init_7800'and v.get('param')=='true'and v.find('const')is not None]
 assert params==[0],params
 r=inventory(raw);_,m,_=gen.generate();q=classify(r,m);controls=[]
 for name in ('lost_phase','phase_width','phase_category','phase_alias','lost_ram','lost_history','hidden_root','constant_state','duplicate','child_state','source','child_param'):
  report,manifest=copy.deepcopy(r),copy.deepcopy(m)
  phase=next(x for x in report['candidates']if x['path']=='cell_MCS6532.phase_d')
  if name=='lost_phase':report['candidates'].remove(phase)
  elif name=='phase_width':phase['width']=2
  elif name=='phase_category':phase['category']='combinational_review'
  elif name=='phase_alias':manifest['phase']['word']=132
  elif name=='lost_ram':report['candidates']=[x for x in report['candidates']if x['name']!='riot_ram']
  elif name=='lost_history':report['candidates']=[x for x in report['candidates']if not x['name'].endswith('.old_pa7')]
  elif name=='hidden_root':
   x=copy.deepcopy(phase);x['path']='cell_MCS6532.hidden';x['name']='hidden';report['candidates'].append(x)
  elif name=='constant_state':next(x for x in report['candidates']if x['name']=='_unused')['category']='sequential'
  elif name=='duplicate':report['candidates'].append(copy.deepcopy(phase))
  elif name=='child_state':
   x=copy.deepcopy(next(x for x in report['candidates']if x['name']=='riot_ram'));x['path']='cell_MCS6532.u_riot.hidden';x['name']='hidden';report['candidates'].append(x)
  elif name=='source':manifest['original_sha256']='0'*64
  else:manifest['child_manifest']['parameters']['init_7800']=1
  try:classify(report,manifest)
  except ValueError as e:controls.append(dict(case=name,rejected=True,diagnostic=str(e)))
  else:raise AssertionError('damaged MCS6532 schema accepted:'+name)
 q['failure_controls']=controls
 paths=[Path(__file__),gen.SOURCE,gen.riot.SOURCE,Path(__file__).parent/'classify_riot.py',Path(__file__).parent/'generate_mcs6532.py',Path(__file__).parent/'generate_riot.py']
 for p in paths:(out/p.name).write_bytes(p.read_bytes())
 q['source_hashes']={str(p.relative_to(gen.ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths}
 for name,value in [('inventory.json',r),('manifest.json',m),('classification.json',q)]:(out/name).write_text(json.dumps(value,indent=2)+'\n')
 print('MCS6532 CLASSIFICATION PASS:1098bits/134words;12damaged schemas rejected')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.output.resolve())
