#!/usr/bin/env python3
"""Classify fresh processor-board XML against the complete modeled state map."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

import generate_processor_full as gen
import generate_snapshot_ff as ff

sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(kind,report,manifest,source):
    _,reference,_=gen.generate(kind,source)
    expected={};excluded={};locations=set();prefix=manifest['original_module']+'.'
    def add(package,name,width,dimensions,word,lsb=0):
        path=prefix+(package+'.' if package else '')+name
        if path in expected:raise ValueError('duplicate modeled field')
        depth=dimensions[0][1]-dimensions[0][0]+1 if dimensions else 1
        where={(word+a,b) for a in range(depth) for b in range(lsb,lsb+width)}
        if where&locations:raise ValueError('aliased state bits')
        locations.update(where)
        expected[path]=dict(width=width,bits=width*depth,dimensions=dimensions,
            access=dict(word=word,lsb=lsb,width=width,rows=dimensions))
    for row in manifest['ff']:
        if row['word']!=row['index']<<9:raise ValueError('wrong FF package word')
        fields=gen.FIELDS[row['kind']];offset=0
        if len(fields)!=len(row['fields']):raise ValueError('missing FF history')
        for field,(name,width) in zip(row['fields'],fields):
            if (field['name'],field['width'],field['lsb'])!=(name,width,offset):
                raise ValueError('FF field layout differs')
            add(row['package'],name,width,[],row['word'],offset);offset+=width
        if offset!=row['width']:raise ValueError('FF total width differs')
    diagnostics={'a_first':4,'d_first':4,'in_we':1,'n_amoved':32,'n_dmoved':32,'n_writes':32,'ramwe_dbg':1}
    for row,original in zip(manifest['files'],reference['files']):
        if row['ram']!=original['ram'] or row['write_window']!=original['write_window']:
            raise ValueError('small-file schema differs')
        add(row['package'],'mem',4,[[0,15]],row['word'])
        for f in row['write_window']['fields']:
            add(row['package'],f['name'],f['width'],[],row['word']|16,f['lsb'])
        for name,width in diagnostics.items():excluded[prefix+row['package']+'.'+name]=width
    for row in manifest['rams']:
        width=4 if row['kind']=='cell_MB7071H' else 1
        if row['width']!=width or row['words']!=256 or row['word']!=row['index']<<9:
            raise ValueError('async RAM geometry differs')
        add(row['package'],'mem',width,[[0,255]],row['word'])
    for i,name in enumerate(manifest['phase']['fields']):
        add('',name,1,[],manifest['phase']['word'],i)
    modules={manifest['original_module']:source.read_text(),
             **{cell:(gen.ROOT/'verilog/cells'/f'{cell}.v').read_text() for cell in manifest['original_sources']}}
    decisions=[];seen=set();roms={prefix+p['package']+'.mem':p for p in manifest['immutable_proms']}
    combinational_hashes={'cell_MC10181':'6d12fa3385eec395899363453743d0665bbdc2ee6a455739001e6f4fc7e0eee0'}
    immutable_hashes={'cell_SG10139':'bcf5d25bac812a4143cb6e7fc893ab746837711d7038d3b4a496e7f228b9189d'}
    for cell,digest in {**combinational_hashes,**immutable_hashes}.items():
        if hashlib.sha256(modules[cell].encode()).hexdigest()!=digest:
            raise ValueError('unreviewed combinational/PROM source: '+cell)
    procedural=set(gen.FIELDS)|{'cell_F10145A','cell_MB7071H','cell_F10414',manifest['original_module']}
    derived={'ck_en','cka_en','ckb_en','clka','clkb','clk','d_a','d_b','d','a','q','pe_n',
             'ce_n','load_data','d_use','s_use','dr_use','dl_use','sel','we'}
    for row in report['candidates']:
        path=row['path'];name=row['name'];category=row['category']
        if path in seen:raise ValueError('duplicate original candidate')
        seen.add(path)
        if path in expected:
            item=expected.pop(path)
            if category!='sequential' or (row['width'],row['bits'],row['unpacked_dimensions'])!=(item['width'],item['bits'],item['dimensions']):
                raise ValueError('modeled state geometry differs: '+path)
            decisions.append(dict(path=path,decision='SAVE',bits=row['bits'],access=item['access']));continue
        if path in excluded:
            width=excluded.pop(path);wanted='initial_only_candidate' if name=='ramwe_dbg' else 'sequential'
            if (category,row['bits'],row['unpacked_dimensions'])!=(wanted,width,[]):
                raise ValueError('diagnostic exclusion differs')
            decisions.append(dict(path=path,decision='EXCLUDE_SIMULATION_ONLY',bits=width,
                reason='Exact original synthesis translate_off diagnostics, no reference outside excluded region'));continue
        module=row['module'].split('__')[0]
        if module not in modules:raise ValueError('unreviewed original module')
        text=ff.masked_comments(modules[module])
        if module in immutable_hashes:
            if path in roms:
                prom=roms.pop(path)
                if (category,row['width'],row['bits'],row['unpacked_dimensions'])!=('initial_only_candidate',8,256,[[0,31]]):
                    raise ValueError('immutable PROM became mutable or changed geometry')
                decisions.append(dict(path=path,decision='IMMUTABLE_BUILD_ASSET',bits=256,asset=prom));continue
            if category!='combinational_review':raise ValueError('unclassified PROM state')
            decisions.append(dict(path=path,decision='RECOMPUTE',bits=row['bits'],reason='Pure read/select of pinned immutable PROM'));continue
        if module in combinational_hashes:
            if category!='combinational_review':raise ValueError('unreviewed ALU state')
            decisions.append(dict(path=path,decision='RECOMPUTE',bits=row['bits'],reason='Pinned complete combinational ALU table, all case/default branches assign lg/X/Y'));continue
        if module not in procedural and re.search(r'\b(always\w*|initial|reg|logic)\b',text):
            raise ValueError('unreviewed procedural cell')
        if category=='initial_only_candidate':
            if not re.search(r'\bwire\s+(?:\[[^\]]+\]\s*)?'+re.escape(name)+r'\s*(?:[;=])',text):
                raise ValueError('unclassified initial state: '+path)
        elif category=='combinational_review':
            if module in procedural and module!=manifest['original_module']:
                if not re.fullmatch('p[0-9]+',name) and name not in derived:
                    raise ValueError('unreviewed derived state: '+path)
        else:raise ValueError('unexported processor state: '+path)
        decisions.append(dict(path=path,decision='RECOMPUTE',bits=row['bits'],
            reason='Reviewed original continuous combinational/constant wire'))
    if expected or excluded or roms:raise ValueError('modeled state missing from original XML')
    bits=sum(d['bits'] for d in decisions if d['decision']=='SAVE')
    if bits!=manifest['logical_state_bits'] or manifest!=reference:
        raise ValueError('complete modeled schema differs')
    return dict(status='PASS',scope=manifest['scope'],modeled_state_bits=bits,
        excluded_simulator_bits=len(manifest['files'])*106,immutable_prom_bits=512,xml_sha256=report['xml_sha256'],decisions=decisions,
        limits='Board-model completeness only; no whole-core barrier, fitted state count, physical machine snapshot or external service state')


def controls(kind,report,manifest,source):
    results=[]
    for case in ('missing_history','wrong_width','new_state','duplicate','alias_package','lost_phase','window_swap','ram_depth','ram_alias','missing_prom','wrong_prom_hash','mutable_prom'):
        r,m=copy.deepcopy(report),copy.deepcopy(manifest)
        if case=='missing_history':m['ff'][0]['fields'].pop()
        elif case=='wrong_width':next(x for x in r['candidates'] if x['category']=='sequential')['bits']+=1
        elif case=='new_state':
            x=copy.deepcopy(next(x for x in r['candidates'] if x['category']=='sequential'));x['path']+='_new';r['candidates'].append(x)
        elif case=='duplicate':r['candidates'].append(copy.deepcopy(r['candidates'][0]))
        elif case=='alias_package':m['ff'][1]['index']=m['ff'][0]['index'];m['ff'][1]['word']=m['ff'][0]['word']
        elif case=='lost_phase':m['phase']['fields'].pop()
        elif case=='window_swap':m['files'][0]['write_window']['fields'][0]['lsb']=9
        elif case=='ram_depth':m['rams'][0]['words']=128
        elif case=='ram_alias':m['rams'][1]['word']=m['rams'][0]['word'];m['rams'][1]['index']=m['rams'][0]['index']
        elif case=='missing_prom':m['immutable_proms'].pop()
        elif case=='wrong_prom_hash':m['immutable_proms'][0]['sha256']='0'*64
        else:next(x for x in r['candidates']if x['module'].startswith('cell_SG10139')and x['name']=='mem')['category']='sequential'
        try:classify(kind,r,m,source)
        except ValueError as error:results.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:raise ValueError('damaged processor coverage accepted: '+case)
    for cell in gen.extra.FIELDS:
        original=(gen.ROOT/'verilog/cells'/f'{cell}.v').read_text()
        for case,addition in [('extra_register','reg lost_history;'),
                              ('extra_process',"always @(posedge sys_clk) lost_history<=1'b0;")]:
            changed=original.replace('endmodule',addition+'\nendmodule')
            try:gen.extra.generate_one(cell,changed)
            except ValueError as error:results.append(dict(case=cell+'_'+case,rejected=True,diagnostic=str(error)))
            else:raise ValueError('unreviewed processor FF source accepted')
    return results


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--kind',choices=gen.SPECS,required=True)
    parser.add_argument('--source',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--negative-controls',action='store_true')
    args=parser.parse_args();source=args.source.resolve();out=args.output.resolve();out.mkdir(parents=True,exist_ok=False)
    _,manifest,_=gen.generate(args.kind,source)
    cells=[gen.ROOT/'verilog/cells'/f'{cell}.v' for cell in manifest['original_sources']]
    command=['verilator','--xml-only','--top-module',manifest['original_module'],'-GSYSPER=4',
             '-DDORADO_F10145_LOGIC','-Wno-fatal','--xml-output',str(out/'board.xml'),str(source),*map(str,cells)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with (out/'elaborate.log').open('w') as log:
        subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    report=inventory((out/'board.xml').read_bytes());result=classify(args.kind,report,manifest,source)
    if args.negative_controls:result['failure_controls']=controls(args.kind,report,manifest,source)
    for name,value in [('classification.json',result),('inventory.json',report),('manifest.json',manifest)]:
        (out/name).write_text(json.dumps(value,indent=2)+'\n')
    paths=[Path(__file__).resolve(),gen.ROOT/'tools/sil_state_inventory.py',gen.ROOT/'fpga/mister/snapshot/generate_processor_full.py',
           gen.ROOT/'fpga/mister/snapshot/generate_processor_ff.py',source,*cells,*[gen.ROOT/p['file']for p in manifest['immutable_proms']]]
    (out/'provenance.json').write_text(json.dumps({str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},indent=2)+'\n')
    print(json.dumps({k:v for k,v in result.items() if k!='decisions'},indent=2))
