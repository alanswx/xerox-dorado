#!/usr/bin/env python3
"""Fresh original64K MemX XML completeness and nonaliased state-map audit."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

import generate_memx_full as gen
sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(report,manifest,root):
    _,reference,_=gen.generate(root)
    if manifest!=reference:raise ValueError('complete MemX schema/source differs')
    prefix=gen.ORIGINAL_MODULE+'.';expected={};excluded={};intervals={i:[]for i in range(18)};covered_bits=0
    def key(package,name):return prefix+(package.replace('__','___05F')+'.'if package else'')+name
    def add(package,name,width,dimensions,word,lsb=0):
        nonlocal covered_bits
        path=key(package,name);depth=dimensions[0][1]-dimensions[0][0]+1 if dimensions else 1
        if path in expected or lsb+width>18:raise ValueError('duplicate/split MemX field')
        # Exact interval coverage avoids allocating millions of Python tuples.
        for bit in range(lsb,lsb+width):
            if any(not(word+depth-1<lo or hi<word)for lo,hi in intervals[bit]):raise ValueError('aliased MemX word/bit')
            intervals[bit].append((word,word+depth-1))
        covered_bits+=width*depth
        expected[path]=dict(width=width,bits=width*depth,dimensions=dimensions,
            access=dict(word=word,lsb=lsb,width=width,rows=dimensions))
    for row in manifest['ff']:
        for f in row['fields']:add(row['package'],f['name'],f['width'],[],row['word'],f['lsb'])
    diagnostics={'a_first':4,'d_first':4,'in_we':1,'n_amoved':32,'n_dmoved':32,'n_writes':32,'ramwe_dbg':1}
    for row in manifest['files']:
        add(row['package'],'mem',4,[[0,15]],row['word'])
        for f in row['write_window']['fields']:add(row['package'],f['name'],f['width'],[],row['word']|16,f['lsb'])
        for n,w in diagnostics.items():excluded[key(row['package'],n)]=w
    for row in manifest['rams']:
        add(row['package'],'mem',1,[[0,65535]],row['word'])
        for f in row['history']['fields']:
            add(row['package'],f['name'],f['width'],[],row['word']+65536+f['lsb']//18,f['lsb']%18)
    for f in manifest['phase']['fields']:add('',f['name'],f['width'],[],manifest['phase']['word'],f['lsb'])
    modules={gen.ORIGINAL_MODULE:(root/'verilog/generated/MemX-Rev-Ch.v').read_text(),
        **{Path(n).stem:(root/n).read_text()for n in manifest['original_sources']}}
    if hashlib.sha256(modules['cell_SG10139'].encode()).hexdigest()!='bcf5d25bac812a4143cb6e7fc893ab746837711d7038d3b4a496e7f228b9189d':
        raise ValueError('unreviewed immutable PROM source')
    roms={key(p['package'],'mem'):p for p in manifest['immutable_proms']}
    procedural={r['kind']for r in manifest['ff']}|{'cell_F10145A','cell_MosRam',gen.ORIGINAL_MODULE}
    derived={'ck_en','cka_en','ckb_en','clka','clkb','clk','d_a','d_b','d','a','addr','q','pe_n','ce_n',
             'load_data','d_use','s_use','dr_use','dl_use','sel','we','ja','ka','jb','kb'}
    decisions=[];seen=set()
    for row in report['candidates']:
        path=row['path'];category=row['category'];name=row['name']
        if path in seen:raise ValueError('duplicate original MemX candidate')
        seen.add(path)
        if path in expected:
            wanted=expected.pop(path)
            if (category,row['width'],row['bits'],row['unpacked_dimensions'])!=('sequential',wanted['width'],wanted['bits'],wanted['dimensions']):
                raise ValueError('MemX state geometry differs:'+path)
            decisions.append(dict(path=path,decision='SAVE',bits=row['bits'],access=wanted['access']));continue
        if path in excluded:
            width=excluded.pop(path);wanted='initial_only_candidate'if name=='ramwe_dbg'else'sequential'
            if (category,row['bits'],row['unpacked_dimensions'])!=(wanted,width,[]):raise ValueError('diagnostic exclusion differs')
            decisions.append(dict(path=path,decision='EXCLUDE_SIMULATION_ONLY',bits=width,reason='Pinned original translate_off block; no reference escaping exclusion'));continue
        module=row['module'].split('__')[0]
        if module not in modules:raise ValueError('unreviewed MemX module:'+module)
        if module=='cell_SG10139':
            if path in roms:
                prom=roms.pop(path)
                if (category,row['width'],row['bits'],row['unpacked_dimensions'])!=('initial_only_candidate',8,256,[[0,31]]):raise ValueError('PROM became mutable or changed shape')
                decisions.append(dict(path=path,decision='IMMUTABLE_BUILD_ASSET',bits=256,asset=prom));continue
            if category!='combinational_review':raise ValueError('unclassified immutable PROM state')
            decisions.append(dict(path=path,decision='RECOMPUTE',bits=row['bits'],reason='Pinned immutable PROM read/select/address'));continue
        text=gen.ff.masked_comments(modules[module])
        if module not in procedural and re.search(r'\b(always\w*|initial|reg|logic)\b',text):raise ValueError('unreviewed procedural MemX cell:'+module)
        if category=='initial_only_candidate':
            if not re.search(r'\bwire\s+(?:\[[^\]]+\]\s*)?'+re.escape(name)+r'\s*(?:[;=])',text):raise ValueError('unclassified initial MemX state:'+path)
        elif category=='combinational_review':
            if module in procedural and module!=gen.ORIGINAL_MODULE and not re.fullmatch('p[0-9]+',name)and name not in derived:raise ValueError('unreviewed derived MemX field:'+path)
        else:raise ValueError('unexported original MemX state:'+path)
        decisions.append(dict(path=path,decision='RECOMPUTE',bits=row['bits'],reason='Reviewed original continuous combinational/constant wire'))
    if expected or excluded or roms:raise ValueError('modeled MemX state missing from XML:'+str(list(expected)[:3]))
    bits=sum(x['bits']for x in decisions if x['decision']=='SAVE')
    if bits!=1378312 or covered_bits!=bits:raise ValueError('incomplete MemX state bit coverage')
    return dict(status='PASS',modeled_state_bits=bits,excluded_simulator_bits=1060,immutable_prom_bits=2048,
        xml_sha256=report['xml_sha256'],decisions=decisions,scope=manifest['scope'])


def controls(report,manifest,root):
    result=[]
    for name in ('lost_row','lost_col','lost_pipeline','lost_pending','lost_output','lost_phase','new_state','duplicate','wrong_width',
                 'ram_depth','history_alias','socket_alias','small_window','wrong_prom','mutable_prom','lost_prom','wrong_density','wrong_source'):
        r,m=copy.deepcopy(report),copy.deepcopy(manifest)
        lost={'lost_row':'row','lost_col':'col','lost_pipeline':'dout_r','lost_pending':'write_pending','lost_output':'dout'}
        if name in lost:
            target=gen.ORIGINAL_MODULE+'.'+manifest['rams'][0]['package']+'.'+lost[name]
            r['candidates']=[x for x in r['candidates']if x['path']!=target]
        elif name=='lost_phase':m['phase']['fields'].pop()
        elif name=='new_state':
            x=copy.deepcopy(next(x for x in r['candidates']if x['category']=='sequential'));x['path']+='_new';r['candidates'].append(x)
        elif name=='duplicate':r['candidates'].append(copy.deepcopy(r['candidates'][0]))
        elif name=='wrong_width':next(x for x in r['candidates']if x['category']=='sequential')['bits']+=1
        elif name=='ram_depth':next(x for x in r['candidates']if x['bits']==65536)['unpacked_dimensions']=[[0,16383]]
        elif name=='history_alias':m['rams'][0]['history']['first_word']=0
        elif name=='socket_alias':m['rams'][1]['word']=m['rams'][0]['word']
        elif name=='small_window':m['files'][0]['write_window']['fields'].pop()
        elif name=='wrong_prom':m['immutable_proms'][0]['sha256']='0'*64
        elif name=='mutable_prom':next(x for x in r['candidates']if x['path'].endswith('.u_g15.mem'))['category']='sequential'
        elif name=='lost_prom':m['immutable_proms'].pop()
        elif name=='wrong_density':m['rams'][0]['address_bits']=14
        else:m['original_sources']['verilog/cells/cell_MosRam.v']='0'*64
        try:classify(r,m,root)
        except ValueError as e:result.append(dict(case=name,rejected=True,diagnostic=str(e)))
        else:raise AssertionError('damaged MemX coverage accepted:'+name)
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    root=a.source_root.resolve();out=a.output.resolve();out.mkdir(parents=True,exist_ok=False);_,m,_=gen.generate(root)
    sources=[root/'verilog/generated/MemX-Rev-Ch.v',*[root/n for n in m['original_sources']]]
    command=['verilator','--xml-only','--top-module',gen.ORIGINAL_MODULE,'-Wno-fatal','-DDORADO_F10145_LOGIC','--xml-output',str(out/'memx.xml'),*map(str,sources)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    report=inventory((out/'memx.xml').read_bytes());(out/'inventory.json').write_text(json.dumps(report,indent=2)+'\n')
    q=classify(report,m,root);q['failure_controls']=controls(report,m,root)
    for name,data in [('manifest.json',m),('classification.json',q)]:(out/name).write_text(json.dumps(data,indent=2)+'\n')
    print('FULL MemX CLASSIFICATION PASS:1378312 mutable bits/2048immutable PROM bits,18damaged schemas/inventories rejected')
