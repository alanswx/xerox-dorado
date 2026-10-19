#!/usr/bin/env python3
"""Independent complete actual BaseBoard XML/transport coverage and immutable ROMs."""
import argparse,copy,hashlib,json,re
from pathlib import Path
import generate_baseboard_full as gen
import inventory_baseboard as inv


def classify(report,m,root):
 if m!=gen.generate(root)[1]:raise ValueError('unreviewed BaseBoard source/schema')
 prefix=gen.ORIGINAL_MODULE+'.';expected={};allocated=set();roms={}
 def key(p,n):return prefix+p+'.'+n
 def add(p,n,width,dims,access):
  depth=dims[0][1]-dims[0][0]+1 if dims else 1;path=key(p,n)
  if path in expected:raise ValueError('duplicate physical state field')
  covered=set()
  for a in access:
   if a['lsb']+a['width']>18 or a['field_lsb']+a['width']>width:raise ValueError('invalid transport slice')
   for row in range(depth):
    for b in range(a['width']):
     target=(a['word']+row,a['lsb']+b);source=(row,a['field_lsb']+b)
     if target in allocated or source in covered:raise ValueError('aliased transport/source state')
     allocated.add(target);covered.add(source)
  if len(covered)!=width*depth:raise ValueError('uncovered field bits')
  expected[path]=dict(width=width,bits=width*depth,dimensions=dims,access=access)
 def fields(p,fs,base,start=0):
  for f in fs:
   access=[];lo=f['lsb'];hi=lo+f['width']
   for row in range((hi+17)//18):
    a=max(lo,row*18);b=min(hi,row*18+18)
    if a<b:access.append(dict(word=base+start+row,lsb=a-row*18,width=b-a,field_lsb=a-lo))
   add(p,f['name'],f['width'],[],access)
 for r in m['packages']:
  p=r['package'];base=r['word'];kind=r['type']
  if kind=='FF':fields(p,r['fields'],base)
  elif kind=='CPU':fields(p+'.u_cpu',r['schema']['fields'],base)
  elif kind=='EPROM':
   fields(p,r['schema']['fields'],base)
   if r['init_file']:roms[key(p,'mem')]=r
  elif kind=='SRAM':
   for member in r['group']['members']:
    for n,dims,word in [('mem',[[0,1023]],base),('dout_r',[],base+1024)]:add(member['instance'],n,1,dims,[dict(word=word,lsb=member['bit'],width=1,field_lsb=0)])
  else:
   add(p,'phase_d',1,[],[dict(word=base+133,lsb=0,width=1,field_lsb=0)])
   add(p+'.u_riot','riot_ram',8,[[0,127]],[dict(word=base,lsb=0,width=8,field_lsb=0)])
   fields(p+'.u_riot',r['schema']['child_manifest']['fields'],base,128)
 modules={gen.ORIGINAL_MODULE:(root/gen.SOURCE).read_text()}
 for name in m['original_sources']:
  text=(root/name).read_text();stem=Path(name).stem
  # Vendor6502 contains three modules in the same CRLF source file.
  for k in re.findall(r'(?m)^\s*module\s+(\w+)\b',text):modules[k]=text
 procedural=set(gen.misc.PACKAGES)|{'cell_MCS6502','cell_MCS6532','cell_i2125','cell_i2716','chip_6502','LOGIC','MUX6502','M6532',gen.ORIGINAL_MODULE}
 seen=set();decisions=[];immutable=0
 for row in report['candidates']:
  path=row['path'];name=row['name'];module=row['module'].split('__')[0];category=row['category']
  if path in seen:raise ValueError('duplicate original XML field')
  seen.add(path);normalized=path
  if module=='M6532'and name.endswith('.old_pa7'):
   if row['line']!=129 or not re.fullmatch(r'@[A-Za-z0-9_]+,101,1,101,10\.old_pa7',name):raise ValueError('PA7 declaration/procedure differs')
   normalized=path[:-(len(name))]+'old_pa7'
  if normalized in expected:
   w=expected.pop(normalized)
   if(category,row['width'],row['bits'],row['unpacked_dimensions'])!=('sequential',w['width'],w['bits'],w['dimensions']):raise ValueError('physical state shape/category differs:'+path)
   decisions.append(dict(path=path,decision='SAVE',bits=row['bits'],access=w['access']));continue
  if path in roms:
   rom=roms.pop(path)
   if(category,row['width'],row['bits'],row['unpacked_dimensions'])!=('initial_only_candidate',8,16384,[[0,2047]]):raise ValueError('immutable EPROM shape/mutability differs')
   if gen.digest(root/rom['init_file'])!=rom['sha256']:raise ValueError('immutable image bytes differ')
   immutable+=16384;decisions.append(dict(path=path,decision='IMMUTABLE_BUILD_ASSET',bits=16384,asset=rom['init_file'],sha256=rom['sha256']));continue
  if module not in modules:raise ValueError('unreviewed original module:'+module)
  source=gen.newff.ff.masked_comments(modules[module])
  if module not in procedural and re.search(r'\b(always\w*|initial|reg|logic)\b',source):raise ValueError('unreviewed procedural cell')
  if category=='initial_only_candidate':
   # Source-pinned wire declarations and constant output assignments only;
   # never treat arbitrary initialized registers/RAM as reconstructible.
   declared_wire=re.search(r'\bwire\s+(?:\[[^\]]+\]\s*)?'+re.escape(name)+r'\s*(?:[;=])',source)
   constant_output=re.search(r'\boutput\s+wire\s+'+re.escape(name)+r'\b',source)and re.search(r'\bassign\b[^;]*\b'+re.escape(name)+r"\s*=\s*1'b[01]",source)
   if not(declared_wire or constant_output):raise ValueError('unclassified initial state:'+path)
  elif category!='combinational_review':raise ValueError('unexported BaseBoard state:'+path)
  decisions.append(dict(path=path,decision='RECOMPUTE',bits=row['bits'],reason='Pinned continuous combinational/constant expression'))
 if expected or roms:raise ValueError('missing physical state/assets:'+str(list(expected)[:4]))
 bits=sum(x['bits']for x in decisions if x['decision']=='SAVE')
 if(bits,len(allocated),immutable)!=(40366,40366,65536):raise ValueError('incomplete board state coverage')
 return dict(status='PASS',modeled_state_bits=bits,immutable_image_bits=immutable,empty_eproms=[r['package']for r in m['packages']if r['type']=='EPROM'and not r['init_file']],
  xml_sha256=report['xml_sha256'],decisions=decisions,scope='Complete actual nominal BaseBoard at SYSPER4. Empty EPROM arrays retain original uninitialized semantics; no excluded sequential bits or machine/global snapshot claim.')


def controls(report,m,root):
 rows=[]
 tests={'lost_cpu':'.u_f61.u_cpu.q','lost_riot_ram':'.u_c62.u_riot.riot_ram','lost_phase':'.u_c62.phase_d','lost_sram':'.u_b11.mem','lost_sram_pipeline':'.u_b11.dout_r','lost_eprom_pipeline':'.u_b61.d','lost_counter_history':'.u_g21.ck_d','lost_clock':'.u_clockgen.phase'}
 for name in [*tests,'lost_pa7','ram_depth','wrong_width','extra_state','duplicate','transport_alias','image_hash','mutable_rom','source_hash']:
  r,s=copy.deepcopy(report),copy.deepcopy(m)
  if name in tests:r['candidates']=[x for x in r['candidates']if not x['path'].endswith(tests[name])]
  elif name=='lost_pa7':r['candidates']=[x for x in r['candidates']if not x['path'].endswith('.old_pa7')]
  elif name=='ram_depth':next(x for x in r['candidates']if x['path'].endswith('.u_b11.mem'))['unpacked_dimensions']=[[0,1022]]
  elif name=='wrong_width':next(x for x in r['candidates']if x['path'].endswith('.u_g21.cnt'))['width']=23
  elif name=='extra_state':x=copy.deepcopy(next(x for x in r['candidates']if x['category']=='sequential'));x['path']+='_hidden';r['candidates'].append(x)
  elif name=='duplicate':r['candidates'].append(copy.deepcopy(r['candidates'][0]))
  elif name=='transport_alias':s['packages'][1]['word']=s['packages'][0]['word']
  elif name=='image_hash':next(x for x in s['packages']if x['type']=='EPROM'and x['init_file'])['sha256']='0'*64
  elif name=='mutable_rom':next(x for x in r['candidates']if x['path'].endswith('.u_b61.mem'))['category']='sequential'
  else:s['original_sources']['verilog/cells/cell_MCS6502.v']='0'*64
  try:classify(r,s,root)
  except ValueError as e:rows.append(dict(case=name,rejected=True,diagnostic=str(e)))
  else:raise AssertionError('damaged complete BaseBoard schema accepted:'+name)
 return rows


def run(root,out):
 inv.run(root,out);report=json.loads((out/'inventory.json').read_text());_,m,_=gen.generate(root);q=classify(report,m,root);q['failure_controls']=controls(report,m,root)
 for name,value in [('manifest.json',m),('classification.json',q)]:(out/name).write_text(json.dumps(value,indent=2)+'\n')
 for n in('classify_baseboard_full.py','generate_baseboard_full.py'):(out/n).write_bytes((Path(__file__).parent/n).read_bytes())
 print('COMPLETE BASEBOARD CLASSIFICATION PASS:40366mutable/65536immutable bits,17bad schemas reject')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--frozen-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.frozen_root.resolve(),a.output.resolve())
