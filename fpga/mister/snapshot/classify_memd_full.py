#!/usr/bin/env python3
"""Fail-closed state classification of freshly elaborated native MemD."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

import generate_memd_full as gen
sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(report,manifest,root):
    _,reference,_=gen.generate(root)
    if manifest!=reference:raise ValueError('complete MemD schema/source differs')
    prefix=gen.ORIGINAL_MODULE+'.';expected={};excluded={};locations=set()
    def path(package,name):return prefix+(package.replace('__','___05F')+'.'if package else'')+name
    def add(package,name,width,dimensions,word,lsb=0):
        key=path(package,name);depth=dimensions[0][1]-dimensions[0][0]+1 if dimensions else 1
        where={(word+a,b)for a in range(depth)for b in range(lsb,lsb+width)}
        if key in expected or where&locations:raise ValueError('aliased/duplicate MemD field')
        locations.update(where)
        expected[key]=dict(width=width,bits=width*depth,dimensions=dimensions,
            access=dict(word=word,lsb=lsb,width=width,rows=dimensions))
    for row in manifest['ff']:
        for f in row['fields']:add(row['package'],f['name'],f['width'],[],row['word'],f['lsb'])
    diagnostics={'a_first':4,'d_first':4,'in_we':1,'n_amoved':32,'n_dmoved':32,'n_writes':32,'ramwe_dbg':1}
    for row in manifest['files']:
        add(row['package'],'mem',4,[[0,15]],row['word'])
        for f in row['write_window']['fields']:add(row['package'],f['name'],f['width'],[],row['word']|16,f['lsb'])
        for n,w in diagnostics.items():excluded[path(row['package'],n)]=w
    for row in manifest['banks']:
        for bit,m in enumerate(row['members']):
            add(m['instance'],'mem',1,[[0,4095]],row['word'],bit)
            add(m['instance'],'dout_r',1,[],row['word']|4096,bit)
    for bit,n in enumerate(manifest['phase']['fields']):add('',n,1,[],manifest['phase']['word'],bit)
    add('storage_rx_pipeline','words',18,[[0,1]],manifest['storage_input']['word'])
    add('storage_rx_pipeline','clock_d',1,[],manifest['storage_input']['word']|2)
    modules={gen.ORIGINAL_MODULE:(root/'verilog/generated/MemD-Rev-Ca.v').read_text(),
        **{Path(n).stem:(root/n).read_text()for n in manifest['original_sources']}}
    procedural={r['kind']for r in manifest['ff']}|{'cell_F10145A','cell_F10470',gen.ORIGINAL_MODULE}
    derived={'ck_en','cka_en','ckb_en','clka','clkb','clk','d_a','d_b','d','a','q','pe_n','ce_n',
             'load_data','d_use','s_use','dr_use','dl_use','sel','we'}
    seen=set();decisions=[];loop_controls=set()
    for row in report['candidates']:
        key=row['path'];category=row['category'];name=row['name']
        if key in seen:raise ValueError('duplicate original MemD candidate')
        seen.add(key)
        if key in expected:
            wanted=expected.pop(key)
            if (category,row['width'],row['bits'],row['unpacked_dimensions'])!=('sequential',wanted['width'],wanted['bits'],wanted['dimensions']):
                raise ValueError('MemD state geometry differs: '+key)
            decisions.append(dict(path=key,decision='SAVE',bits=row['bits'],access=wanted['access']));continue
        if key in excluded:
            width=excluded.pop(key);wanted='initial_only_candidate'if name=='ramwe_dbg'else'sequential'
            if (category,row['bits'],row['unpacked_dimensions'])!=(wanted,width,[]):raise ValueError('diagnostic exclusion differs')
            decisions.append(dict(path=key,decision='EXCLUDE_SIMULATION_ONLY',bits=width,
                reason='Pinned original translate_off diagnostics; small-file generator rejects any reference escaping the excluded region'));continue
        # Both indices are overwritten before use on every entry to their
        # exact source-pinned for loops. Their previous host-simulation values
        # cannot affect a future word/edge; they are not persistent datapath.
        loop=None
        if key==prefix+'storage_rx_pipeline.j':loop='j'
        elif re.fullmatch(re.escape(prefix)+r'storage_rx_pipeline\.@[^.]+\.k',key):loop='k'
        if loop:
            wanted='sequential'if loop=='j'else'initial_only_candidate'
            if (category,row['width'],row['bits'],row['unpacked_dimensions'])!=(wanted,32,32,[]):raise ValueError('loop control geometry differs')
            if loop in loop_controls:raise ValueError('duplicate loop control')
            loop_controls.add(loop)
            decisions.append(dict(path=key,decision='RECOMPUTE_LOOP_CONTROL',bits=32,
                reason='Pinned original loop starts by assigning index before any read; no index reference outside its loop'));continue
        module=row['module'].split('__')[0]
        if module not in modules:raise ValueError('unreviewed MemD module: '+module)
        text=gen.ff.masked_comments(modules[module])
        if module not in procedural and re.search(r'\b(always\w*|initial|reg|logic)\b',text):
            raise ValueError('unreviewed procedural MemD cell: '+module)
        if category=='initial_only_candidate':
            if not re.search(r'\bwire\s+(?:\[[^\]]+\]\s*)?'+re.escape(name)+r'\s*(?:[;=])',text):
                raise ValueError('unclassified initial MemD state: '+key)
        elif category=='combinational_review':
            if module in procedural and module!=gen.ORIGINAL_MODULE and not re.fullmatch('p[0-9]+',name)and name not in derived:
                raise ValueError('unreviewed derived MemD state: '+key)
        else:raise ValueError('unexported original MemD state: '+key)
        decisions.append(dict(path=key,decision='RECOMPUTE',bits=row['bits'],reason='Reviewed original continuous combinational/constant wire'))
    if expected or excluded or loop_controls!={'j','k'}:raise ValueError('modeled MemD state absent from XML: '+str(list(expected)[:3]))
    bits=sum(d['bits']for d in decisions if d['decision']=='SAVE')
    if bits!=296591 or len(locations)!=bits:raise ValueError('incomplete MemD state coverage')
    return dict(status='PASS',modeled_state_bits=bits,excluded_simulator_bits=8*106,recomputed_loop_index_bits=64,
        xml_sha256=report['xml_sha256'],decisions=decisions,scope=manifest['scope'])


def controls(report,manifest,root):
    results=[]
    for name in ('missing_history','lost_pipeline','ram_depth','new_state','duplicate','wrong_width',
                 'bank_alias','lane_alias','phase_alias','small_window','ce_omitted','wrong_source','unsupported_delay','lost_phase',
                 'lost_storage_word','lost_storage_history'):
        r,m=copy.deepcopy(report),copy.deepcopy(manifest)
        if name=='missing_history':m['ff'][0]['fields'].pop()
        elif name=='lost_pipeline':r['candidates']=[x for x in r['candidates']if not x['path'].endswith('.dout_r')]
        elif name=='ram_depth':next(x for x in r['candidates']if x['bits']==4096)['unpacked_dimensions']=[[0,2047]]
        elif name=='new_state':
            x=copy.deepcopy(next(x for x in r['candidates']if x['category']=='sequential'));x['path']+='_new';r['candidates'].append(x)
        elif name=='duplicate':r['candidates'].append(copy.deepcopy(r['candidates'][0]))
        elif name=='wrong_width':next(x for x in r['candidates']if x['category']=='sequential')['bits']+=1
        elif name=='bank_alias':m['banks'][1]['word']=m['banks'][0]['word']
        elif name=='lane_alias':m['banks'][0]['members'][1]=m['banks'][0]['members'][0]
        elif name=='phase_alias':m['phase']['word']=m['ff'][0]['word']
        elif name=='small_window':m['files'][0]['write_window']['fields'].pop()
        elif name=='ce_omitted':m['banks'][0]['ce']='OMITTED'
        elif name=='wrong_source':m['original_sources']['verilog/cells/cell_F10470.v']='0'*64
        elif name=='unsupported_delay':m['parameters']['STORAGE_INPUT_DELAY']=1
        elif name=='lost_storage_word':r['candidates']=[x for x in r['candidates']if not x['path'].endswith('.storage_rx_pipeline.words')]
        elif name=='lost_storage_history':r['candidates']=[x for x in r['candidates']if not x['path'].endswith('.storage_rx_pipeline.clock_d')]
        else:m['phase']['fields'].pop()
        try:classify(r,m,root)
        except ValueError as e:results.append(dict(case=name,rejected=True,diagnostic=str(e)))
        else:raise AssertionError('damaged MemD coverage accepted: '+name)
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source-root',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();root=a.source_root.resolve();out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
    _,manifest,_=gen.generate(root)
    sources=[root/'verilog/generated/MemD-Rev-Ca.v',*[root/n for n in manifest['original_sources']]]
    command=['verilator','--xml-only','--top-module',gen.ORIGINAL_MODULE,'-Wno-fatal','-DDORADO_F10145_LOGIC',
        '-GSYSPER=4','-GSTORAGE_INPUT_DELAY=2','--xml-output',str(out/'memd.xml'),*map(str,sources)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    report=inventory((out/'memd.xml').read_bytes())
    (out/'inventory.json').write_text(json.dumps(report,indent=2)+'\n')
    result=classify(report,manifest,root);result['failure_controls']=controls(report,manifest,root)
    for name,data in [('manifest.json',manifest),('classification.json',result)]:
        (out/name).write_text(json.dumps(data,indent=2)+'\n')
    print('FULL MemD CLASSIFICATION PASS:296591 modeled mutable bits;16 damaged schemas/inventories rejected')
