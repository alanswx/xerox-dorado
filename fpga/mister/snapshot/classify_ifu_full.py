#!/usr/bin/env python3
"""Classify fresh original IFU XML against its complete modeled access map."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

import generate_ifu_full as gen

sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(report,manifest,source_root):
    _,reference,_=gen.generate(source_root)
    if manifest!=reference:raise ValueError('complete IFU schema/source differs')
    prefix=gen.ORIGINAL_MODULE+'.';expected={};locations=set()
    def add(package,name,width,dimensions,word,lsb=0):
        path=prefix+(package+'.'if package else'')+name
        if path in expected:raise ValueError('duplicate modeled IFU field')
        depth=dimensions[0][1]-dimensions[0][0]+1 if dimensions else 1
        used={(word+a,bit)for a in range(depth)for bit in range(lsb,lsb+width)}
        if used&locations:raise ValueError('aliased IFU bit/word')
        locations.update(used)
        expected[path]=dict(width=width,bits=width*depth,dimensions=dimensions,
            access=dict(word=word,lsb=lsb,width=width,rows=dimensions))
    for row in manifest['ff']:
        for f in row['fields']:add(row['package'],f['name'],f['width'],[],row['word'],f['lsb'])
    for row in manifest['banks']:
        for bit,m in enumerate(row['members']):
            add('u_'+m['package'],'mem',1,[[0,1023]],row['word'],bit)
            add('u_'+m['package'],'dout_r',1,[],row['word']|1024,bit)
    for bit,name in enumerate(manifest['phase']['fields']):add('',name,1,[],manifest['phase']['word'],bit)
    source=source_root/'verilog/generated/IFU-Rev-Ch.v'
    modules={gen.ORIGINAL_MODULE:source.read_text(),**{Path(name).stem:(source_root/name).read_text()for name in manifest['original_sources']}}
    combinational={'cell_MC10181':'6d12fa3385eec395899363453743d0665bbdc2ee6a455739001e6f4fc7e0eee0'}
    immutable={'cell_SG10139':'bcf5d25bac812a4143cb6e7fc893ab746837711d7038d3b4a496e7f228b9189d'}
    for kind,sha in {**combinational,**immutable}.items():
        if hashlib.sha256(modules[kind].encode()).hexdigest()!=sha:raise ValueError('unreviewed ALU/PROM source: '+kind)
    roms={prefix+p['package']+'.mem':p for p in manifest['immutable_proms']}
    procedural={r['kind']for r in manifest['ff']}|{'cell_F10415A',gen.ORIGINAL_MODULE}
    derived={'ck_en','cka_en','ckb_en','clka','clkb','clk','d_a','d_b','d','a','q','pe_n','ce_n',
             'load_data','d_use','s_use','dr_use','dl_use','sel','we','ja','ka','jb','kb'}
    decisions=[];seen=set()
    for row in report['candidates']:
        path=row['path'];name=row['name'];category=row['category']
        if path in seen:raise ValueError('duplicate original IFU candidate')
        seen.add(path)
        if path in expected:
            wanted=expected.pop(path)
            if (category,row['width'],row['bits'],row['unpacked_dimensions'])!=('sequential',wanted['width'],wanted['bits'],wanted['dimensions']):
                raise ValueError('modeled IFU state geometry differs: '+path)
            decisions.append(dict(path=path,decision='SAVE',bits=row['bits'],access=wanted['access']));continue
        module=row['module'].split('__')[0]
        if module not in modules:raise ValueError('unreviewed original module: '+module)
        text=gen.ff.masked_comments(modules[module])
        if module in immutable:
            if path in roms:
                prom=roms.pop(path)
                if (category,row['width'],row['bits'],row['unpacked_dimensions'])!=('initial_only_candidate',8,256,[[0,31]]):
                    raise ValueError('immutable PROM became mutable or changed shape')
                decisions.append(dict(path=path,decision='IMMUTABLE_BUILD_ASSET',bits=256,asset=prom));continue
            if category!='combinational_review':raise ValueError('unclassified PROM state')
            decisions.append(dict(path=path,decision='RECOMPUTE',bits=row['bits'],reason='Pinned immutable PROM address/read/select'));continue
        if module in combinational:
            if category!='combinational_review':raise ValueError('unreviewed ALU state')
            decisions.append(dict(path=path,decision='RECOMPUTE',bits=row['bits'],reason='Pinned complete combinational ALU table/default assigns all outputs'));continue
        if module not in procedural and re.search(r'\b(always\w*|initial|reg|logic)\b',text):
            raise ValueError('unreviewed procedural IFU cell: '+module)
        if category=='initial_only_candidate':
            if not re.search(r'\bwire\s+(?:\[[^\]]+\]\s*)?'+re.escape(name)+r'\s*(?:[;=])',text):
                raise ValueError('unclassified initial IFU state: '+path)
        elif category=='combinational_review':
            if module in procedural and module!=gen.ORIGINAL_MODULE:
                if not re.fullmatch('p[0-9]+',name)and name not in derived:raise ValueError('unreviewed derived IFU state: '+path)
        else:raise ValueError('unexported original IFU state: '+path)
        decisions.append(dict(path=path,decision='RECOMPUTE',bits=row['bits'],reason='Reviewed original continuous combinational/constant wire'))
    if expected or roms:raise ValueError('modeled IFU state absent from XML')
    bits=sum(x['bits']for x in decisions if x['decision']=='SAVE')
    if bits!=28653 or len(locations)!=28653:raise ValueError('incomplete IFU bit coverage')
    return dict(status='PASS',modeled_state_bits=bits,immutable_prom_bits=256,xml_sha256=report['xml_sha256'],
        decisions=decisions,scope=manifest['scope'],
        limits='Complete original IFU model classification only; not fitted state count, whole-core/global barrier, enabled resource budget, physical snapshots or original60ns clocks')


def controls(report,manifest,source_root):
    rows=[]
    for name in ('lost_history','lost_phase','lost_pipeline','ram_plane_alias','ram_depth','new_state','duplicate','wrong_width','prom_missing','prom_hash','prom_mutable','address_alias','ce_tie','new_source'):
        r,m=copy.deepcopy(report),copy.deepcopy(manifest)
        if name=='lost_history':m['ff'][0]['fields'].pop()
        elif name=='lost_phase':m['phase']['fields'].pop()
        elif name=='lost_pipeline':r['candidates']=[x for x in r['candidates']if not x['path'].endswith('.u_g09.dout_r')]
        elif name=='ram_plane_alias':m['banks'][1]['members'][1]=m['banks'][1]['members'][0]
        elif name=='ram_depth':next(x for x in r['candidates']if x['path'].endswith('.u_g09.mem'))['unpacked_dimensions']=[[0,511]]
        elif name=='new_state':
            x=copy.deepcopy(next(x for x in r['candidates']if x['category']=='sequential'));x['path']+='_lost';r['candidates'].append(x)
        elif name=='duplicate':r['candidates'].append(copy.deepcopy(r['candidates'][0]))
        elif name=='wrong_width':next(x for x in r['candidates']if x['category']=='sequential')['bits']+=1
        elif name=='prom_missing':m['immutable_proms']=[]
        elif name=='prom_hash':m['immutable_proms'][0]['sha256']='0'*64
        elif name=='prom_mutable':next(x for x in r['candidates']if x['path'].endswith('.u_a06.mem'))['category']='sequential'
        elif name=='address_alias':m['banks'][1]['word']=m['banks'][0]['word']
        elif name=='ce_tie':m['banks'][0]['ce']="1'b0"
        else:m['original_sources']['verilog/cells/cell_SG10139.v']='0'*64
        try:classify(r,m,source_root)
        except ValueError as error:rows.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged IFU coverage accepted: '+name)
    return rows


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--source-root',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
    root=args.source_root.resolve();out=args.output.resolve();out.mkdir(parents=True,exist_ok=False)
    _,manifest,_=gen.generate(root)
    sources=[root/'verilog/generated/IFU-Rev-Ch.v',*[root/name for name in manifest['original_sources']]]
    command=['verilator','--xml-only','--top-module',gen.ORIGINAL_MODULE,'-Wno-fatal','-DDORADO_F10415_M10K',
        '--xml-output',str(out/'ifu.xml'),*map(str,sources)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with (out/'elaborate.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    report=inventory((out/'ifu.xml').read_bytes());result=classify(report,manifest,root)
    result['failure_controls']=controls(report,manifest,root)
    for name,data in [('inventory.json',report),('manifest.json',manifest),('classification.json',result)]:
        (out/name).write_text(json.dumps(data,indent=2)+'\n')
    paths=[Path(__file__).resolve(),gen.ROOT/'tools/sil_state_inventory.py',*sources]
    (out/'provenance.json').write_text(json.dumps({str(p):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths},indent=2)+'\n')
    print('FULL IFU CLASSIFICATION PASS:28653 modeled mutable bits/256immutable PROM bits,14 damaged schemas/inventories rejected')
