#!/usr/bin/env python3
"""Classify every original frozen ContA candidate against physical state access."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

import generate_conta_full as gen
import generate_snapshot_ff as ff

sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(report,manifest,source):
    # Re-run the original process/declaration guards before relying on source
    # categories. Neither an inventory nor a manifest alone is completeness.
    _,expected_manifest,_=gen.generate(source)
    expected={};excluded={};positions=set()
    def add(package,name,width,dimensions,word,lsb=0):
        path='ContA_m_Rev_m_Cd_monitor.'+(package+'.'if package else'')+name
        if path in expected:raise ValueError('duplicate physical field')
        bits=width
        if dimensions:bits*=dimensions[0][1]-dimensions[0][0]+1
        access=dict(word=word,lsb=lsb,width=width)
        if dimensions:access['rows']=dimensions
        locations={(word+row,bit)for row in range(16)for bit in range(width)}if dimensions else{(word,bit)for bit in range(lsb,lsb+width)}
        if positions&locations:raise ValueError('aliased physical access bits')
        positions.update(locations)
        expected[path]=dict(width=width,bits=bits,dimensions=dimensions,access=access)
    for row in manifest['ff']:
        if row['word']!=row['index']<<5:raise ValueError('wrong package word')
        offset=0
        for field,(name,width)in zip(row['fields'],gen.FIELDS[row['kind']]):
            if (field['name'],field['width'],field['lsb'])!=(name,width,offset):raise ValueError('FF field packing differs')
            add(row['package'],name,width,[],row['word'],offset);offset+=width
        if offset!=row['width']or len(row['fields'])!=len(gen.FIELDS[row['kind']]):raise ValueError('incomplete FF field layout')
    diagnostics={'a_first':4,'d_first':4,'in_we':1,'n_amoved':32,'n_dmoved':32,'n_writes':32,'ramwe_dbg':1}
    for row in manifest['files']:
        if row['ram']!=dict(words=16,width=4,indices=[0,15]):raise ValueError('changed RAM geometry')
        add(row['package'],'mem',4,[[0,15]],row['index']<<5)
        fields=row['write_window']['fields'];reference=expected_manifest['files'][0]['write_window']['fields']
        if fields!=reference:raise ValueError('RAM write-window packing differs')
        for field in fields:add(row['package'],field['name'],field['width'],[],(row['index']<<5)|16,field['lsb'])
        for name,width in diagnostics.items():excluded[f"ContA_m_Rev_m_Cd_monitor.{row['package']}.{name}"]=width
    for bit,name in enumerate(manifest['phase']['fields']):add('',name,1,[],manifest['phase']['word'],bit)
    decisions=[];seen=set()
    modules={'ContA_m_Rev_m_Cd_monitor':source.read_text(),**{name:(gen.ROOT/'verilog/cells'/f'{name}.v').read_text()for name in manifest['original_sources']}}
    for row in report['candidates']:
        path=row['path'];category=row['category'];name=row['name']
        if path in seen:raise ValueError('duplicate inventory state')
        seen.add(path)
        if path in expected:
            item=expected.pop(path)
            if category!='sequential'or(row['width'],row['bits'],row['unpacked_dimensions'])!=(item['width'],item['bits'],item['dimensions']):raise ValueError('physical state geometry differs: '+path)
            decisions.append(dict(path=path,decision='SAVE',bits=row['bits'],access=item['access']));continue
        if path in excluded:
            width=excluded.pop(path);expected_category='initial_only_candidate'if name=='ramwe_dbg'else'sequential'
            if(category,row['bits'],row['unpacked_dimensions'])!=(expected_category,width,[]):raise ValueError('diagnostic exclusion differs')
            decisions.append(dict(path=path,decision='EXCLUDE_SIMULATION_ONLY',bits=width,reason='Original synthesis translate_off region; exact file generator guards no reference outside it'));continue
        module=row['module'].split('__')[0]
        if module not in modules:raise ValueError('unreviewed source module: '+module)
        text=ff.masked_comments(modules[module])
        if module not in gen.FIELDS and module not in ('cell_F10145A','ContA_m_Rev_m_Cd_monitor'):
            if re.search(r'\b(always\w*|initial|reg|logic)\b',text):raise ValueError('unreviewed procedural cell')
        if category=='initial_only_candidate':
            if not re.search(r'\bwire\s+(?:\[[^\]]+\]\s*)?'+re.escape(name)+r'\s*(?:[;=])',text):raise ValueError('unclassified initial state: '+path)
        elif category=='combinational_review':
            if module in gen.FIELDS or module=='cell_F10145A':
                derived={'ck_en','cka_en','ckb_en','clka','clkb','clk','d_a','d_b','d','a','q','pe_n','ce_n','load_data','d_use','s_use','dr_use','dl_use'}
                if not re.fullmatch('p[0-9]+',name)and name not in derived:raise ValueError('unreviewed derived state node: '+path)
        else:raise ValueError('unexported state: '+path+' '+category)
        decisions.append(dict(path=path,decision='RECOMPUTE',bits=row['bits'],reason='Original continuous combinational/constant wire; guarded clocked destinations are all covered above'))
    if expected or excluded:raise ValueError('state/exclusion absent from original elaboration')
    if sum(row['bits']for row in decisions if row['decision']=='SAVE')!=1353:raise ValueError('incorrect physical state total')
    # This pins the exact remaining schema details too (including package and
    # phase order), independently of which details happen to appear in XML.
    if manifest!=expected_manifest:raise ValueError('manifest differs from generated access fabric')
    return dict(status='PASS',scope=manifest['scope'],physical_state_bits=1353,
        excluded_simulator_bits=954,xml_sha256=report['xml_sha256'],decisions=decisions,
        limits='Board only. Outer phase/input/global barrier/raw SDRAM/ARM services remain outside; no physical machine restore')


def controls(report,manifest,source):
    rows=[]
    for case in ('missing_history','wrong_width','new_state','duplicate','alias_package','lost_phase','window_swap'):
        r,m=copy.deepcopy(report),copy.deepcopy(manifest)
        if case=='missing_history':m['ff'][0]['fields'].pop()
        elif case=='wrong_width':next(x for x in r['candidates']if x['category']=='sequential')['bits']+=1
        elif case=='new_state':
            x=copy.deepcopy(next(x for x in r['candidates']if x['category']=='sequential'));x['path']+='_new';r['candidates'].append(x)
        elif case=='duplicate':r['candidates'].append(copy.deepcopy(r['candidates'][0]))
        elif case=='alias_package':m['ff'][1]['index']=m['ff'][0]['index'];m['ff'][1]['word']=m['ff'][0]['word']
        elif case=='lost_phase':m['phase']['fields']=m['phase']['fields'][:-1]
        else:m['files'][0]['write_window']['fields'][0]['lsb']=9
        try:classify(r,m,source)
        except ValueError as error:rows.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:raise ValueError('damaged physical coverage accepted: '+case)
    for kind in ff.FIELDS:
        original=(gen.ROOT/'verilog/cells'/f'{kind}.v').read_text()
        field=gen.FIELDS[kind][0][0]
        for tag,addition in [('extra_register','reg missing_history;'),('extra_process',f"always @(posedge sys_clk) {field} <= 1'b0;")]:
            changed=original.replace('endmodule',addition+'\nendmodule')
            try:ff.generate_one(kind,changed)
            except ValueError as error:rows.append(dict(case=kind+'_'+tag,rejected=True,diagnostic=str(error)))
            else:raise ValueError('unreviewed FF source accepted: '+kind+' '+tag)
    return rows


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--negative-controls',action='store_true')
    args=parser.parse_args();source=args.source.resolve();out=args.output.resolve();out.mkdir(parents=True,exist_ok=False)
    _,manifest,_=gen.generate(source)
    cells=[gen.ROOT/'verilog/cells'/f'{name}.v'for name in manifest['original_sources']]
    command=['verilator','--xml-only','--top-module','ContA_m_Rev_m_Cd_monitor','-GSYSPER=4','-DDORADO_F10145_LOGIC','-Wno-fatal','--xml-output',str(out/'board.xml'),str(source),*map(str,cells)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    report=inventory((out/'board.xml').read_bytes());result=classify(report,manifest,source)
    if args.negative_controls:result['failure_controls']=controls(report,manifest,source)
    (out/'classification.json').write_text(json.dumps(result,indent=2)+'\n')
    (out/'inventory.json').write_text(json.dumps(report,indent=2)+'\n')
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    paths=[Path(__file__).resolve(),gen.ROOT/'tools/sil_state_inventory.py',
           gen.ROOT/'fpga/mister/snapshot/generate_conta_full.py',gen.ROOT/'fpga/mister/snapshot/generate_snapshot_ff.py',source,*cells]
    (out/'provenance.json').write_text(json.dumps({str(p):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths},indent=2)+'\n')
    print(json.dumps({k:v for k,v in result.items()if k!='decisions'},indent=2))
