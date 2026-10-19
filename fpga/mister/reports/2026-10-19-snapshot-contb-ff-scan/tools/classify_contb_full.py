#!/usr/bin/env python3
"""Classify freshly elaborated ORIGINAL ContB state against the export manifest.

This accounts for the modeled board only. Backplane inputs, external clock
phase, clock-gate state and machine/ARM/device state are outside this boundary.
Non-state candidates must be continuous-wire expressions in reviewed sources;
new procedural cells, variables, widths or array geometry fail closed.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

import audit_contb_im as im
import generate_contb_full as gen

ROOT = im.ROOT
sys.path.insert(0,str(ROOT/'tools'))
from sil_state_inventory import inventory


def classify(report,manifest):
    expected={}
    def add(package,name,width,bits,dimensions,access):
        path='ContB_m_Rev_m_Cd.u_'+package+'.'+name
        if path in expected:
            raise ValueError('duplicate exported state: '+path)
        expected[path]=dict(width=width,bits=bits,dimensions=dimensions,access=access)
    for g in manifest['groups']:
        for bit in g['bits']:
            add(bit['package'],'mem',1,1024,[[0,1023]],dict(group=g['index'],words=[0,1023],bit=bit['bit']))
            add(bit['package'],'dout_r',1,1,[],dict(group=g['index'],word=1024,bit=bit['bit']))
    for cell in manifest['ff']:
        for field in cell['fields']:
            add(cell['package'],field['name'],field['width'],field['width'],[],
                dict(word=cell['word'],lsb=field['lsb'],width=field['width']))
    original=im.GENERATED.read_text()
    modules={'ContB_m_Rev_m_Cd':original}
    for path in (ROOT/'verilog/cells').glob('cell_*.v'):
        modules[path.stem]=path.read_text()
    # Sequential cell shapes are independently guarded by exact reviewed
    # process extraction; every other used module must contain no procedural
    # state at all. Verilator's parameterized suffix is normalized below.
    gen.ff_sources()
    decisions=[]
    seen=set()
    for row in report['candidates']:
        path=row['path']
        if path in seen:
            raise ValueError('duplicate inventory state: '+path)
        seen.add(path)
        category=row['category']
        if category=='sequential':
            if path not in expected:
                raise ValueError('unexported sequential state: '+path)
            entry=expected.pop(path)
            if (row['width'],row['bits'],row['unpacked_dimensions'])!=(entry['width'],entry['bits'],entry['dimensions']):
                raise ValueError('state geometry differs: '+path)
            decisions.append(dict(path=path,decision='SAVE',bits=row['bits'],access=entry['access']))
            continue
        module=row['module'].split('__')[0]
        if module not in modules:
            raise ValueError('unreviewed source module: '+module)
        source=re.sub(r'/\*.*?\*/|//[^\n]*','',modules[module],flags=re.S)
        if module not in (*gen.FIELDS,'cell_F10415A'):
            if re.search(r'\b(always\w*|initial|reg|logic)\b',source):
                raise ValueError('unreviewed procedural source: '+module)
        if category=='initial_only_candidate':
            # These optimized initial nodes are wire constants, including
            # ground ties and constant-zero unused-power-pin expressions.
            declaration=r'\bwire\s+(?:\[[^\]]+\]\s*)?'+re.escape(row['name'])+r'\s*(?:[;=])'
            if not re.search(declaration,source):
                raise ValueError('unreviewed initial state: '+path)
            reason='Continuous wire constant folded into XML initialization; recomputed from identical RTL'
        elif category=='combinational_review':
            # Other modules contain only continuous assignments. Within the
            # reviewed sequential cells, these nodes are their output/enable
            # wires; the clocked destinations were all required above.
            if module in (*gen.FIELDS,'cell_F10415A') and not (
                re.fullmatch(r'p\d+',row['name']) or row['name'] in ('ck_en','cka_en','ckb_en','clka','clkb','d_a','d_b','a')):
                raise ValueError('unreviewed sequential-cell derived node: '+path)
            reason='Continuous combinational expression; recomputed from saved state and captured external inputs'
        else:
            raise ValueError('unreviewed candidate category: '+path+' '+category)
        decisions.append(dict(path=path,decision='RECOMPUTE',bits=row['bits'],reason=reason))
    if expected:
        raise ValueError('exported state absent from elaboration: '+next(iter(expected)))
    words=set()
    for cell in manifest['ff']:
        if cell['word'] in words:
            raise ValueError('aliased sequential state word')
        words.add(cell['word'])
        if cell['word'] != (8 << 11) | cell['index']:
            raise ValueError('incorrect sequential state word')
        occupied=set()
        for field in cell['fields']:
            positions=set(range(field['lsb'],field['lsb']+field['width']))
            if positions & occupied:
                raise ValueError('overlapping state field layout')
            occupied.update(positions)
        if occupied != set(range(cell['width'])):
            raise ValueError('incomplete state field layout')
    saved=sum(r['bits']for r in decisions if r['decision']=='SAVE')
    if saved!=147809:
        raise ValueError('unexpected complete board bit count')
    return dict(status='PASS',scope='All original modeled ContB candidates in fresh elaboration; external inputs/clocks/machine/ARM excluded',
                logical_state_bits=saved,ram_bits=147456,read_pipeline_bits=144,ff_history_bits=209,
                xml_sha256=report['xml_sha256'],manifest_sha256=hashlib.sha256(json.dumps(manifest,sort_keys=True).encode()).hexdigest(),
                decisions=decisions,
                adapter_state='Administrative request/result/frozen bookkeeping and physical scanned RAM output are reconstructed by completed freeze/load protocol; pending saved guest read state is included as the raw pipeline. No state-port request may remain outstanding at machine resume.')


def controls(report,manifest):
    rows=[]
    for case in ('missing_history','wrong_width','missing_parity','duplicate','new_state','overlap','alias_word'):
        r,m=copy.deepcopy(report),copy.deepcopy(manifest)
        if case=='missing_history':
            m['ff'][0]['fields']=[f for f in m['ff'][0]['fields'] if f['name']!='ck_d']
            expected='unexported sequential state'
        elif case=='wrong_width':
            m['ff'][0]['fields'][0]['width']+=1;expected='state geometry differs'
        elif case=='missing_parity':
            g=m['groups'][0];g['bits']=[b for b in g['bits'] if b['field']!='dIMLH']
            expected='unexported sequential state'
        elif case=='duplicate':
            r['candidates'].append(copy.deepcopy(r['candidates'][0]));expected='duplicate inventory state'
        elif case=='new_state':
            extra=copy.deepcopy(next(x for x in r['candidates'] if x['category']=='sequential'))
            extra['path']+='_new';r['candidates'].append(extra);expected='unexported sequential state'
        elif case=='overlap':
            m['ff'][0]['fields'][1]['lsb']=0;expected='overlapping state field layout'
        else:
            m['ff'][1]['word']=m['ff'][0]['word'];expected='aliased sequential state word'
        try:
            classify(r,m)
        except ValueError as e:
            if expected not in str(e):
                raise ValueError('wrong failure reason: '+case+' '+str(e))
            rows.append(dict(case=case,rejected=True,diagnostic=str(e)))
        else:
            raise ValueError('damaged state coverage accepted: '+case)
    return rows


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--negative-controls',action='store_true')
    args=parser.parse_args()
    out=args.output.resolve()
    out.mkdir(parents=True,exist_ok=False)
    cells=[ROOT/'verilog/cells'/f'{k}.v' for k in sorted({k for k,_,_ in im.instances(im.GENERATED.read_text())})]
    command=['verilator','--xml-only','--top-module','ContB_m_Rev_m_Cd','-Wno-fatal',
             '-DDORADO_F10415_M10K','--xml-output',str(out/'board.xml'),str(im.GENERATED),*map(str,cells)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with (out/'elaborate.log').open('w')as log:
        subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    report=inventory((out/'board.xml').read_bytes())
    _,manifest,_=gen.generate()
    result=classify(report,manifest)
    if args.negative_controls:
        result['failure_controls']=controls(report,manifest)
    (out/'classification.json').write_text(json.dumps(result,indent=2)+'\n')
    (out/'provenance.json').write_text(json.dumps({str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()
        for p in [Path(__file__),ROOT/'tools/sil_state_inventory.py',im.GENERATED,*cells]},indent=2)+'\n')
    print(json.dumps({k:v for k,v in result.items()if k!='decisions'},indent=2))
