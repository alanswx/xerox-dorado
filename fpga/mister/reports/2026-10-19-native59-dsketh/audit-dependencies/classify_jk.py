#!/usr/bin/env python3
"""Classify original MC10135 XML against its complete seven-bit state map."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import sys

import generate_jk as gen

sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(report,manifest):
    _,reference=gen.generate()
    if manifest!=reference:raise ValueError('unreviewed J-K state schema/source')
    saved={};offset=0
    for name,width in gen.FIELDS:
        saved[name]=(width,offset);offset+=width
    derived={'ck_en','ja','ka','jb','kb','p2','p3','p15','p14'}
    seen=set();decisions=[]
    for row in report['candidates']:
        name=row['name']
        if name in seen or row['path']!='cell_MC10135.'+name:raise ValueError('duplicate/unexpected instance')
        seen.add(name)
        if row['unpacked_dimensions']:raise ValueError('unexpected J-K array')
        if name in saved:
            width,lsb=saved.pop(name)
            if (row['category'],row['width'],row['bits'])!=('sequential',width,width):
                raise ValueError('J-K state geometry differs: '+name)
            decisions.append(dict(path=row['path'],decision='SAVE',bits=width,lsb=lsb))
        elif name in derived:
            derived.remove(name)
            if (row['category'],row['width'],row['bits'])!=('combinational_review',1,1):
                raise ValueError('derived J-K wire became state: '+name)
            decisions.append(dict(path=row['path'],decision='RECOMPUTE',bits=1,
                reason='Source-pinned continuous pin/clock/PRECAP expression'))
        else:raise ValueError('unclassified J-K state: '+name)
    if saved or derived:raise ValueError('missing original state/wire')
    return dict(status='PASS',modeled_state_bits=7,xml_sha256=report['xml_sha256'],decisions=decisions,
        scope='One modeled MC10135 primitive, not IFU/whole-machine snapshot coverage or fitted-state count')


def controls(report,manifest):
    rows=[]
    for name in ('lost_clock','lost_setup','wrong_width','new_state','duplicate','derived_state','schema_alias','source_hash'):
        r,m=copy.deepcopy(report),copy.deepcopy(manifest)
        if name in ('lost_clock','lost_setup'):
            r['candidates']=[x for x in r['candidates']if x['name']!=('ck_d'if name=='lost_clock'else'jk_pre')]
        elif name=='wrong_width':next(x for x in r['candidates']if x['name']=='jk_pre')['width']=3
        elif name=='new_state':
            x=copy.deepcopy(r['candidates'][0]);x['name']='lost';x['path']='cell_MC10135.lost';r['candidates'].append(x)
        elif name=='duplicate':r['candidates'].append(copy.deepcopy(r['candidates'][0]))
        elif name=='derived_state':next(x for x in r['candidates']if x['name']=='ck_en')['category']='sequential'
        elif name=='schema_alias':m['fields'][1]=m['fields'][0]
        else:m['original_sha256']='0'*64
        try:classify(r,m)
        except ValueError as error:rows.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged J-K classification accepted: '+name)
    return rows


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();out=args.output.resolve();out.mkdir(parents=True,exist_ok=False)
    source=gen.ROOT/'verilog/cells/cell_MC10135.v'
    command=['verilator','--xml-only','--top-module','cell_MC10135','-Wno-fatal','--xml-output',str(out/'cell.xml'),str(source)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with (out/'elaborate.log').open('w')as log:subprocess.run(command,check=True,stdout=log,stderr=subprocess.STDOUT)
    report=inventory((out/'cell.xml').read_bytes());_,manifest=gen.generate()
    result=classify(report,manifest);result['failure_controls']=controls(report,manifest)
    (out/'inventory.json').write_text(json.dumps(report,indent=2)+'\n')
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    (out/'classification.json').write_text(json.dumps(result,indent=2)+'\n')
    paths=[Path(__file__).resolve(),gen.ROOT/'fpga/mister/snapshot/generate_jk.py',source,gen.ROOT/'tools/sil_state_inventory.py']
    (out/'provenance.json').write_text(json.dumps({str(p.relative_to(gen.ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths},indent=2)+'\n')
    print('J-K CLASSIFICATION PASS:7 modeled bits,8 damaged schema/inventory controls rejected')
