#!/usr/bin/env python3
"""Map qualified physical Qs and package pins to preserved MC1668 native source.

This is integration inventory, not a generated replacement, timing proof or
firmware qualification. No state transport, Main changes or hardware access.
All498 native source identities are checked before inspecting the actual board
instances. Histories/setup copies are distinguished from original physical Qs.
"""
import argparse
import ast
import hashlib
import json
from pathlib import Path
import re

ROOT=Path(__file__).resolve().parents[1]


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def integer(text):
 node=ast.parse(text,mode='eval').body
 def walk(n):
  if isinstance(n,ast.Constant)and type(n.value)is int:return n.value
  if isinstance(n,ast.BinOp)and isinstance(n.op,(ast.Add,ast.Sub,ast.Mult)):
   a,b=walk(n.left),walk(n.right)
   return a+b if isinstance(n.op,ast.Add)else a-b if isinstance(n.op,ast.Sub)else a*b
  raise AssertionError(('unsupported constant arithmetic',text))
 return walk(node)


def indices(text):
 if '+:'in text:
  base,width=map(integer,text.split('+:'));return list(range(base,base+width))
 if ':'in text:
  high,low=map(integer,text.split(':'));assert high>=low;return list(range(low,high+1))
 return [integer(text)]


def source_bits(text):
 found=re.findall(r'fixture_state\[([^\]]+)\]',text)
 assert found and not re.sub(r'fixture_state\[[^\]]+\]|[{},\s]','',text),text
 return [i for expression in reversed(found)for i in indices(expression)]


def global_mapping(reference):
 raw=reference.read_text();maps={'ContA':{},'ProcH':{},'ProcL':{}}
 m=re.search(r'c\(fixture_load,(\{[^}]+\}),bases\[2\]',raw);assert m
 maps['ContA']=dict(enumerate(source_bits(m[1])))
 pattern=r' for\(genvar i=0;i<16;i=i\+1\) begin: map_t_file\n(.*?)\n end\n'
 m=re.search(pattern,raw,re.S);assert m
 expanded='\n'.join(re.sub(r'\bi\b',str(i),m[1])for i in range(16))
 raw=raw[:m.start()]+expanded+raw[m.end():]
 for owner,left,right in re.findall(r'assign\s+(ih|il)\[([^\]]+)\]\s*=\s*([^;]+);',raw):
  board='ProcH'if owner=='ih'else'ProcL';a,b=indices(left),source_bits(right);assert len(a)==len(b)
  for local,global_bit in zip(a,b):assert local not in maps[board];maps[board][local]=global_bit
 assert [len(maps[k])for k in ['ContA','ProcH','ProcL']]==[64,280,408]
 flat=[v for m in maps.values()for v in m.values()];assert len(set(flat))==752 and sorted(flat)==list(range(752))
 return maps


def inventory(native,reference,out):
 candidate=json.loads((native/'mc1668-candidate.json').read_text());assert candidate['state_admin']=='INACTIVE'
 assert candidate['baseline_files']==498 and len(candidate['source_hashes'])==498
 for n,h in candidate['source_hashes'].items():assert sha(native/n)==h,('native source identity',n)
 manifest=json.loads((reference/'manifest.json').read_text());assert manifest['physical_bits']==752
 maps=global_mapping(reference/'r_front_ready_reference.sv')
 paths={k:'src/fpga/mister/'+('snapshot_native_conta'if k=='ContA'else'snapshot_native_processor')+'/'+k+'_full_snapshot.sv'for k in maps}
 assets={}
 for k in maps:
  d=json.loads((native/str(Path(paths[k]).parent/'assets.json')).read_text())
  assets[k]=d['manifest']if k=='ContA'else d['manifests'][k]
 rows=[];total=old_extra=0;identities={}
 for board in manifest['boards']:
  name=board['board'].split('-Rev-')[0];path=native/paths[name];text=path.read_text();identities[paths[name]]=sha(path)
  am=assets[name];metadata={x['package'].removeprefix('u_'):x for key in ['ff','files']for x in am[key]}
  local_map=maps[name]
  for pkg,(start,width)in board['sequential'].items():
   kind=board['packages'][pkg]
   m=re.search(r'^  (cell_\w+)([^\n]*) u_'+re.escape(pkg)+r'\s*\(\n(.*?)^  \);',text,re.M|re.S);assert m,(name,pkg)
   assert m[1]=='cell_'+kind+'_snapshot',(name,pkg,kind,m[1])
   pin_rows=re.findall(r'\.p(\d+)\(([^()]*)\)',m[3]);pins={int(k):v.strip()for k,v in pin_rows};assert len(pins)==len(pin_rows)
   item=metadata[pkg];assert item['kind']=='cell_'+kind
   if kind=='F10145A':
    assert item['ram']['words']*item['ram']['width']==width==64
    storage='16x4 RAM';extra=item['write_window']['width']
   else:
    fields=[f for f in item['fields']if f['name']in ['q','qa','qb']]
    assert sum(f['width']for f in fields)==width,(name,pkg,item,width)
    storage='physical FF/latch Q';extra=item['width']-width
   global_bits=[local_map[i]for i in range(start,start+width)]
   total+=width;old_extra+=extra
   rows.append(dict(board=name,package=pkg,kind=kind,physical_width=width,
                    global_Q_bits_lsb_first=global_bits,native_source=paths[name],native_instance='u_'+pkg,
                    native_model=m[1],native_parameters=m[2].strip(),physical_pin_connections=pins,
                    storage=storage,old_nonarchitectural_history_bits=extra,
                    existing_metadata=item))
 assert total==752
 ram_manifest_path=reference.parent/'ram-reference/manifest.json'
 ram_manifest=json.loads(ram_manifest_path.read_text());assert ram_manifest['physical_bits']==9216
 ram_rows=[];ram_total=0
 roles={0:('rm',0,4),1024:('rm',4,4),2048:('stk',0,4),3072:('stk',4,4),4096:('rm',8,1),4352:('stk',8,1)}
 for half,board in enumerate(ram_manifest['boards']):
  name=board['board'].split('-Rev-')[0];text=(native/paths[name]).read_text()
  metadata={x['package'].removeprefix('u_'):x for x in assets[name]['rams']}
  for pkg,(start,width)in board['sequential'].items():
   kind=board['packages'][pkg];item=metadata[pkg]
   m=re.search(r'^  (cell_\w+)([^\n]*) u_'+re.escape(pkg)+r'\s*\(\n(.*?)^  \);',text,re.M|re.S);assert m,(name,pkg)
   assert m[1]=='cell_'+kind+'_snapshot'
   prefix,bit_start,word_width=roles[start];assert width==256*word_width
   assert item['words']==256 and item['width']==word_width and item['kind']=='cell_'+kind
   pin_rows=re.findall(r'\.p(\d+)\(([^()]*)\)',m[3]);pins={int(k):v.strip()for k,v in pin_rows};assert len(pins)==len(pin_rows)
   ram_rows.append(dict(board=name,package=pkg,kind=kind,native_instance='u_'+pkg,native_source=paths[name],
                        physical_pin_connections=pins,reference_RAM_bit_range=[4608*half+start,4608*half+start+width-1],
                        component_array=prefix+('_h'if half==0 else'_l'),component_word_bits=list(range(bit_start,bit_start+word_width)),
                        words=256,physical_bits=width,parity=word_width==1,existing_metadata=item))
   ram_total+=width
 assert ram_total==9216 and len(ram_rows)==12
 inputs={}
 # Freeze only selected native sources/metadata here; all498 identities were
 # verified against the preserved candidate record above, not duplicated.
 out.mkdir(parents=True,exist_ok=False);frozen=out/'executed-inputs'
 files=[(str(Path('tools')/Path(__file__).name),Path(__file__)),('mc1668-candidate.json',native/'mc1668-candidate.json'),
        ('reference/manifest.json',reference/'manifest.json'),('reference/r_front_ready_reference.sv',reference/'r_front_ready_reference.sv'),('ram-reference/manifest.json',ram_manifest_path),('verilog/rtl/dorado_r_file_ports.sv',ROOT/'verilog/rtl/dorado_r_file_ports.sv')]
 files += [(n,native/n)for n in sorted(set(paths.values())|{str(Path(p).parent/'assets.json')for p in paths.values()})]
 for n,path in files:
  p=frozen/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(path.read_bytes());inputs[n]=sha(p)
 result=dict(status='PASS_R_FRONT_READY_NATIVE_INVENTORY',scope=__doc__,inputs=inputs,
             verified_native_sources=len(candidate['source_hashes']),native_state_admin='INACTIVE',
             physical_Q_bits=752,selected_packages=len(rows),old_history_setup_window_bits=old_extra,
             native_board_identities=identities,packages=rows,ready_Q_range=[736,751],
             RAM_packages=ram_rows,RAM_bits=ram_total,RAM_array_count=4,RAM_depth=256,RAM_word_width=9,
             native_replacement_generated=False,arrivals_qualified=False,hardware_changed=False)
 (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
 print(result['status'],len(rows),'packages/752 physical Qs;',old_extra,'existing metadata history/setup/window bits;12 RAM packages/9216 bits;all498 native identities verified')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--native',type=Path,required=True)
 p.add_argument('--reference',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
 a=p.parse_args();inventory(a.native.resolve(),a.reference.resolve(),a.output.resolve())
