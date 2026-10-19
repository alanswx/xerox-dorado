#!/usr/bin/env python3
"""Independent actual DskEth XML completeness and nonaliased transport map."""
import argparse,copy,hashlib,json,re,subprocess,sys
from pathlib import Path
import generate_dsketh_full as gen
sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(report,m,root):
 if m!=gen.generate(root)[1]:raise ValueError('unreviewed DskEth schema/source')
 prefix=gen.ORIGINAL_MODULE+'.';expected={};excluded={};allocated=set()
 def key(pkg,name):return prefix+(pkg+'.'if pkg else'')+name
 def add(pkg,name,width,dimensions,word,lsb=0,access=None):
  depth=dimensions[0][1]-dimensions[0][0]+1 if dimensions else 1
  access=access or[dict(word=word,lsb=lsb,width=width,field_lsb=0,depth=depth)]
  if key(pkg,name)in expected:raise ValueError('duplicate DskEth field')
  fields=set()
  for c in access:
   if c['lsb']+c['width']>18 or c['field_lsb']+c['width']>width or c['depth']!=depth:raise ValueError('invalid DskEth transport')
   for a in range(depth):
    for bit in range(c['width']):
     target=(c['word']+a,c['lsb']+bit);source=(a,c['field_lsb']+bit)
     if target in allocated or source in fields:raise ValueError('aliased DskEth field')
     allocated.add(target);fields.add(source)
  if len(fields)!=width*depth:raise ValueError('missing DskEth transport bits')
  expected[key(pkg,name)]=dict(width=width,bits=width*depth,dimensions=dimensions,access=access)
 for r in m['ff']:
  for f in r['fields']:add(r['package'],f['name'],f['width'],[],r['word'],f['lsb'])
 for r in m['memories']:
  p=r['package'];word=r['word']
  if r['kind']=='cell_F10145A':
   add(p,'mem',4,[[0,15]],word)
   for f in r['write_window']['fields']:add(p,f['name'],f['width'],[],word+16,f['lsb'])
   for n,w in dict(a_first=4,d_first=4,in_we=1,n_amoved=32,n_dmoved=32,n_writes=32,ramwe_dbg=1).items():excluded[key(p,n)]=w
  else:
   pos=0
   for f in r['fields']:
    access=[]
    for w in r['transport_words']:
     a=max(pos,w['index']*18);b=min(pos+f['width'],w['index']*18+w['width'])
     if b>a:access.append(dict(word=word+w['index'],lsb=a-w['index']*18,width=b-a,field_lsb=a-pos,depth=1))
    add(p,f['name'],f['width'],[],word,access=access);pos+=f['width']
 for f in m['phase']['fields']:add('',f['name'],f['width'],[],m['phase']['word'],f['lsb'])
 roms={key(p['package'],'mem'):p for p in m['immutable_proms']}
 modules={gen.ORIGINAL_MODULE:(root/gen.SOURCE).read_text(),**{Path(n).stem:(root/n).read_text()for n in m['original_sources']}}
 procedural={r['kind']for r in m['ff']+m['memories']}|{gen.ORIGINAL_MODULE}
 derived=set('ck_en cka_en ckb_en clka clkb clk d_a d_b d a addr q pe_n ce_n load_data d_use s_use dr_use dl_use sel we ja ka jb kb _unused setup_pre data_use cwe_use fb shifted taps su_n sd_n cin_n preset up down trig1 trig2 observation'.split())
 seen=set();decisions=[];immutable=0;diagnostics=0
 for r in report['candidates']:
  path=r['path'];category=r['category'];name=r['name']
  if path in seen:raise ValueError('duplicate DskEth XML candidate')
  seen.add(path)
  if path in expected:
   w=expected.pop(path)
   if(category,r['width'],r['bits'],r['unpacked_dimensions'])!=('sequential',w['width'],w['bits'],w['dimensions']):raise ValueError('DskEth state differs:'+path)
   decisions.append(dict(path=path,decision='SAVE',bits=r['bits'],access=w['access']));continue
  if path in excluded:
   w=excluded.pop(path);wanted='initial_only_candidate'if name=='ramwe_dbg'else'sequential'
   if(category,r['bits'],r['unpacked_dimensions'])!=(wanted,w,[]):raise ValueError('DskEth diagnostic differs')
   diagnostics+=w;decisions.append(dict(path=path,decision='EXCLUDE_SIMULATION_ONLY',bits=w,reason='Checked translate_off block, no references outside it'));continue
  module=r['module'].split('__')[0]
  if module not in modules:raise ValueError('unreviewed DskEth module:'+module)
  if module in ('cell_MCM10149','cell_SG10139','cell_SN74S288'):
   if path in roms:
    prom=roms.pop(path);bits=prom['width']*prom['words']
    if(category,r['width'],r['bits'],r['unpacked_dimensions'])!=('initial_only_candidate',prom['width'],bits,[[0,prom['words']-1]]):raise ValueError('DskEth PROM shape/mutability differs')
    immutable+=bits;decisions.append(dict(path=path,decision='IMMUTABLE_BUILD_ASSET',bits=bits,asset=prom));continue
   if category!='combinational_review':raise ValueError('unclassified PROM state')
  else:
   source=gen.ff.masked_comments(modules[module])
   if module not in procedural and re.search(r'\b(always\w*|initial|reg|logic)\b',source):raise ValueError('unreviewed procedural DskEth cell')
   if category=='initial_only_candidate':
    tied_output=(module==gen.ORIGINAL_MODULE and name.endswith('__drv')
      and re.search(r'\boutput\s+wire\s+'+re.escape(name)+r'\b',source)
      and re.search(r'\bassign\s+'+re.escape(name)+r"\s*=\s*1'b[01]\s*;",source))
    if not tied_output and not re.search(r'\bwire\s+(?:\[[^\]]+\]\s*)?'+re.escape(name)+r'\s*(?:[;=])',source):raise ValueError('unclassified initial state:'+path)
   elif category=='combinational_review':
    if module in procedural and module!=gen.ORIGINAL_MODULE and not re.fullmatch('p[0-9]+',name)and name not in derived and not re.fullmatch(r'__Vdfg(?:Extracted|Regularize)_\w+',name):raise ValueError('unreviewed derived field:'+path)
   else:raise ValueError('unexported DskEth state:'+path)
  decisions.append(dict(path=path,decision='RECOMPUTE',bits=r['bits'],reason='Pinned continuous combinational/constant expression'))
 if expected or excluded or roms:raise ValueError('missing DskEth state/assets:'+str(list(expected)[:3]))
 bits=sum(x['bits']for x in decisions if x['decision']=='SAVE')
 if(bits,len(allocated),diagnostics,immutable)!=(2656,2656,1908,12032):raise ValueError('incomplete DskEth state coverage')
 return dict(status='PASS',modeled_state_bits=bits,excluded_simulator_bits=diagnostics,immutable_prom_bits=immutable,xml_sha256=report['xml_sha256'],decisions=decisions,scope='Complete modeled actual native DskEth including monitor; no physical/global snapshot or timer calibration')


def controls(report,m,root):
 rows=[]
 target_small=next(r for r in m['memories']if r['kind']=='cell_F10145A')['package']
 suffixes=dict(lost_phase='.Clk1',lost_timer='.u_a03.t1',lost_timer_history='.u_a05.a1_d',lost_crc='.u_j05.cwe_pre',lost_monitor='.u_receive_monitor.write_d',lost_window='.'+target_small+'.we_d',lost_ram='.'+target_small+'.mem',lost_osc='.u_j20.osc')
 for name in [*suffixes,'ram_depth','wrong_width','new_state','duplicate','socket_alias','prom_hash','mutable_prom','source_hash']:
  r,s=copy.deepcopy(report),copy.deepcopy(m)
  if name in suffixes:
   before=len(r['candidates']);r['candidates']=[x for x in r['candidates']if not x['path'].endswith(suffixes[name])];assert len(r['candidates'])<before,name
  elif name=='ram_depth':next(x for x in r['candidates']if x['path'].endswith('.'+target_small+'.mem'))['unpacked_dimensions']=[[0,7]]
  elif name=='wrong_width':next(x for x in r['candidates']if x['path'].endswith('.u_a03.t1'))['width']=31
  elif name=='new_state':
   x=copy.deepcopy(next(x for x in r['candidates']if x['category']=='sequential'));x['path']+='_hidden';r['candidates'].append(x)
  elif name=='duplicate':r['candidates'].append(copy.deepcopy(r['candidates'][0]))
  elif name=='socket_alias':s['memories'][1]['word']=s['memories'][0]['word']
  elif name=='prom_hash':s['immutable_proms'][0]['sha256']='0'*64
  elif name=='mutable_prom':next(x for x in r['candidates']if x['path'].endswith('.'+m['immutable_proms'][0]['package']+'.mem'))['category']='sequential'
  else:s['original_sources']['verilog/cells/cell_SN74123.v']='0'*64
  try:classify(r,s,root)
  except ValueError as e:rows.append(dict(case=name,rejected=True,diagnostic=str(e)))
  else:raise AssertionError('damaged DskEth schema accepted:'+name)
 return rows

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();out=a.output.resolve();out.mkdir(parents=True,exist_ok=False);root=a.source_root.resolve()
 _,m,_=gen.generate(root);cmd=['verilator','--xml-only','--top-module',gen.ORIGINAL_MODULE,'-GSYSPER=4','-Wno-fatal','-DDORADO_F10145_LOGIC','--xml-output',str(out/'dsketh.xml'),str(root/gen.SOURCE),*[str(root/n)for n in m['original_sources']]]
 for p in [Path(__file__),Path(__file__).with_name('generate_dsketh_full.py'),Path(__file__).with_name('generate_receive_monitor.py')]:
  (out/p.name).write_bytes(p.read_bytes())
 (out/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
 with(out/'elaborate.log').open('w')as f:subprocess.run(cmd,cwd=root,stdout=f,stderr=subprocess.STDOUT,check=True)
 r=inventory((out/'dsketh.xml').read_bytes());q=classify(r,m,root);q['failure_controls']=controls(r,m,root)
 for name,x in [('inventory.json',r),('manifest.json',m),('classification.json',q)]:(out/name).write_text(json.dumps(x,indent=2)+'\n')
 print('DSKETH CLASSIFICATION PASS:2656mutable/12032immutable bits,16damaged schemas rejected')
