#!/usr/bin/env python3
"""Account for original F10415 RAM/read state in width-correct bank layouts."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import sys

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
WIDTHS=(1,2,4,5,18)
ORIGINAL_SHA='44cb1c6eee73b8507f7a66e5781073f916d0c02a221618dedf4d09879a9a7227'
sys.path.insert(0,str(ROOT/'tools'))
from sil_state_inventory import inventory


def manifest(width):
    if width not in WIDTHS:raise ValueError('unqualified word width')
    return dict(width=width,words=1024,transport_bits=18,padding_bits=18-width,
        word_index_bits=11,valid_indices=[0,1024],invalid_indices=[1025,2047],
        ram=dict(first_word=0,last_word=1023,original_field='mem',original_cell_bits=1),
        pipeline=dict(word=1024,original_field='dout_r',original_cell_bits=1),
        modeled_state_bits=width*1025,original_sha256=ORIGINAL_SHA,
        cells=[dict(index=i,data_bit=i)for i in range(width)])


def classify(report,schema):
    width=schema['width']
    if schema!=manifest(width):raise ValueError('unreviewed RAM/pipeline access schema')
    original=ROOT/'verilog/cells/cell_F10415A.v'
    if hashlib.sha256(original.read_bytes()).hexdigest()!=ORIGINAL_SHA:
        raise ValueError('unreviewed original F10415 source')
    expected={'mem':('sequential',1,1024,[[0,1023]]),'dout_r':('sequential',1,1,[]),
              'a':('combinational_review',10,10,[]),'p1':('combinational_review',1,1,[])}
    seen=set();decisions=[]
    for row in report['candidates']:
        name=row['name']
        if name in seen or row['path']!='cell_F10415A.'+name:raise ValueError('unexpected/duplicate original field')
        seen.add(name)
        if name not in expected or (row['category'],row['width'],row['bits'],row['unpacked_dimensions'])!=expected.pop(name):
            raise ValueError('unclassified/different original state: '+name)
        item=dict(path=row['path'],bits=row['bits'],decision='SAVE'if name in ('mem','dout_r')else'RECOMPUTE')
        if name=='mem':item['access']=dict(words=[0,1023],bit='original cell index0..width-1')
        elif name=='dout_r':item['access']=dict(word=1024,bit='original cell index0..width-1')
        else:item['reason']='Pinned original continuous address/output expression; CE not rewritten'
        decisions.append(item)
    if expected:raise ValueError('missing original RAM/read-pipeline state')
    locations=set()
    for cell in schema['cells']:
        locations.update((a,cell['data_bit'])for a in range(1025))
    if len(locations)!=width*1025:raise ValueError('aliased/missing original cell plane')
    return dict(status='PASS',modeled_state_bits=width*1025,original_cell_bits=1025,
        xml_sha256=report['xml_sha256'],decisions=decisions,
        scope='Pinned original one-bit RAM+read pipeline repeated across distinct width planes; not fitted state count, full IFU coverage, global barrier or physical snapshot')


def controls(report):
    rows=[]
    for name in ('missing_pipeline','wrong_depth','wrong_width','new_state','duplicate','derived_state','pipeline_alias','valid_alias','wrong_padding','unqualified_width','wrong_source_hash'):
        r,s=copy.deepcopy(report),manifest(5)
        if name=='missing_pipeline':r['candidates']=[x for x in r['candidates']if x['name']!='dout_r']
        elif name=='wrong_depth':next(x for x in r['candidates']if x['name']=='mem')['unpacked_dimensions']=[[0,511]]
        elif name=='wrong_width':next(x for x in r['candidates']if x['name']=='mem')['width']=2
        elif name=='new_state':
            x=copy.deepcopy(r['candidates'][0]);x['name']='lost';x['path']='cell_F10415A.lost';r['candidates'].append(x)
        elif name=='duplicate':r['candidates'].append(copy.deepcopy(r['candidates'][0]))
        elif name=='derived_state':next(x for x in r['candidates']if x['name']=='a')['category']='sequential'
        elif name=='pipeline_alias':s['pipeline']['word']=0
        elif name=='valid_alias':s['valid_indices']=[0,2047]
        elif name=='wrong_padding':s['padding_bits']=0
        elif name=='unqualified_width':s['width']=19
        else:s['original_sha256']='0'*64
        try:classify(r,s)
        except ValueError as error:rows.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged original-state coverage accepted: '+name)
    return rows


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();out=args.output.resolve();out.mkdir(parents=True,exist_ok=False)
    source=ROOT/'verilog/cells/cell_F10415A.v'
    command=['verilator','--xml-only','--top-module','cell_F10415A','-Wno-fatal','-DDORADO_F10415_M10K',
             '--xml-output',str(out/'cell.xml'),str(source)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with (out/'elaborate.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    report=inventory((out/'cell.xml').read_bytes());schemas={str(w):manifest(w)for w in WIDTHS}
    result=dict(status='PASS',widths={str(w):classify(report,manifest(w))for w in WIDTHS},failure_controls=controls(report))
    (out/'inventory.json').write_text(json.dumps(report,indent=2)+'\n')
    (out/'schemas.json').write_text(json.dumps(schemas,indent=2)+'\n')
    (out/'classification.json').write_text(json.dumps(result,indent=2)+'\n')
    paths=[Path(__file__).resolve(),HERE/'f10415_word_bank.sv',source,ROOT/'tools/sil_state_inventory.py']
    (out/'provenance.json').write_text(json.dumps({str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths},indent=2)+'\n')
    print('F10415 WORD CLASSIFICATION PASS:1025 original modeled bits per cell, widths1/2/4/5/18,11 damaged schemas/inventories rejected')
