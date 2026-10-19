#!/usr/bin/env python3
"""Classify fresh original-cell XML against the bounded async-RAM state schema."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import sys

import generate_async256 as gen

sys.path.insert(0, str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(kind, report, manifest):
    width = gen.SPECS[kind]['width']
    expected = dict(a=('combinational_review',8),sel=('combinational_review',1))
    if kind=='MB7071H':
        expected.update(_unused_pins=('initial_only_candidate',1),q=('combinational_review',4),
                        we=('combinational_review',1))
        expected.update({p:('combinational_review',1) for p in ('p20','p21','p22','p23')})
    else:
        expected['p15']=('combinational_review',1)
    if any(manifest[k]!=v for k,v in dict(words=256,width=width,logical_state_bits=256*width,
            valid_indices=[0,255],invalid_indices=[256,511]).items()):
        raise ValueError('unreviewed access geometry')
    seen=set();decisions=[];saved=0
    for row in report['candidates']:
        name=row['name']
        if name in seen or row['path']!='cell_'+kind+'.'+name:
            raise ValueError('duplicate or unexpected instance')
        seen.add(name)
        if name=='mem':
            if (row['category'],row['width'],row['bits'],row['unpacked_dimensions'])!=('sequential',width,256*width,[[0,255]]):
                raise ValueError('unreviewed RAM state geometry')
            decisions.append(dict(path=row['path'],decision='SAVE',bits=256*width,
                access=dict(indices=[0,255],lsb=0,width=width)))
            saved+=256*width
        else:
            if name not in expected or (row['category'],row['width'])!=expected.pop(name) or row['unpacked_dimensions']:
                raise ValueError('unclassified state: '+name)
            decisions.append(dict(path=row['path'],decision='RECOMPUTE',bits=row['bits'],
                reason='Reviewed original continuous combinational/constant wire'))
    if expected or 'mem'not in seen or saved!=256*width:
        raise ValueError('missing original state or reviewed wire')
    return dict(status='PASS',physical_bits=saved,xml_sha256=report['xml_sha256'],
        decisions=decisions,scope='One pinned original async RAM primitive; not board/global snapshot completeness')


def controls(kind, report, manifest):
    results=[]
    for case in ('missing_ram','width','depth','new_history','duplicate','access_alias'):
        r,m=copy.deepcopy(report),copy.deepcopy(manifest)
        mem=next(x for x in r['candidates'] if x['name']=='mem')
        if case=='missing_ram':r['candidates'].remove(mem)
        elif case=='width':mem['width']+=1
        elif case=='depth':mem['unpacked_dimensions']=[[0,127]]
        elif case=='new_history':r['candidates'].append(dict(mem,name='lost',path='cell_'+kind+'.lost'))
        elif case=='duplicate':r['candidates'].append(copy.deepcopy(mem))
        else:m['valid_indices']=[0,511]
        try:
            classify(kind,r,m)
        except ValueError as error:
            results.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:
            raise AssertionError('damaged state coverage accepted: '+case)
    return results


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--negative-controls',action='store_true')
    args=parser.parse_args();out=args.output.resolve();out.mkdir(parents=True,exist_ok=False)
    summary={}
    for kind in gen.SPECS:
        case=out/kind;case.mkdir()
        source=gen.ROOT/'verilog/cells'/('cell_'+kind+'.v')
        command=['verilator','--xml-only','--top-module','cell_'+kind,'-Wno-fatal',
                 '--xml-output',str(case/'cell.xml'),str(source)]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with (case/'elaborate.log').open('w') as log:
            subprocess.run(command,check=True,stdout=log,stderr=subprocess.STDOUT)
        report=inventory((case/'cell.xml').read_bytes());_,manifest=gen.generate(kind)
        result=classify(kind,report,manifest)
        if args.negative_controls:
            result['failure_controls']=controls(kind,report,manifest)
        summary[kind]=result
        (case/'classification.json').write_text(json.dumps(result,indent=2)+'\n')
        (case/'inventory.json').write_text(json.dumps(report,indent=2)+'\n')
    paths=[Path(__file__).resolve(),gen.HERE/'generate_async256.py',gen.ROOT/'tools/sil_state_inventory.py']
    paths += [gen.ROOT/'verilog/cells'/('cell_'+k+'.v') for k in gen.SPECS]
    (out/'classification.json').write_text(json.dumps(summary,indent=2)+'\n')
    (out/'provenance.json').write_text(json.dumps({str(p.relative_to(gen.ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},indent=2)+'\n')
    print('ASYNC256 CLASSIFICATION PASS:1280 physical bits, both original primitives')
