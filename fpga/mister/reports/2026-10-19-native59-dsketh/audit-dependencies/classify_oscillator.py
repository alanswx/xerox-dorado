#!/usr/bin/env python3
"""Classify the exact K1115A original's signed accumulator and output."""
import argparse,copy,hashlib,json,subprocess,sys
from pathlib import Path
import generate_oscillator as gen
sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(r,m):
    if m!=gen.generate()[1]:raise ValueError('unreviewed oscillator schema/source')
    expected={'acc':('sequential',32),'osc':('sequential',1),'p8':('combinational_review',1),'_unused':('initial_only_candidate',1)}
    decisions=[]
    for row in r['candidates']:
        name=row['name']
        if name not in expected or row['path']!='cell_K1115A.'+name:raise ValueError('extra/duplicate oscillator state')
        category,width=expected.pop(name)
        if (row['category'],row['width'],row['bits'],row['unpacked_dimensions'])!=(category,width,width,[]):raise ValueError('oscillator field differs:'+name)
        decisions.append(dict(path=row['path'],decision='SAVE'if category=='sequential'else'RECOMPUTE',bits=width,access=[dict(word=0,lsb=0,width=18,field_lsb=0),dict(word=1,lsb=0,width=14,field_lsb=18)]if name=='acc'else dict(word=1,lsb=14)if name=='osc'else'Original pinned continuous expression'))
    if expected:raise ValueError('missing oscillator field')
    return dict(status='PASS',modeled_state_bits=33,xml_sha256=r['xml_sha256'],decisions=decisions,scope='Original modeled K1115A only, not calibrated peripheral time or complete display/machine snapshots')


def run(out):
    out.mkdir(parents=True,exist_ok=False);source=gen.ROOT/'verilog/cells/cell_K1115A.v'
    cmd=['verilator','--xml-only','--top-module','cell_K1115A','-Wno-fatal','--xml-output',str(out/'cell.xml'),str(source)]
    (out/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as log:subprocess.run(cmd,check=True,stdout=log,stderr=subprocess.STDOUT)
    r=inventory((out/'cell.xml').read_bytes());_,m=gen.generate();q=classify(r,m);controls=[]
    for name in ('lost_acc','lost_output','wrong_width','new_state','duplicate','derived_state','schema_alias','source_hash'):
        report,manifest=copy.deepcopy(r),copy.deepcopy(m)
        if name in ('lost_acc','lost_output'):report['candidates']=[x for x in report['candidates']if x['name']!=('acc'if name=='lost_acc'else'osc')]
        elif name=='wrong_width':next(x for x in report['candidates']if x['name']=='acc')['width']=31
        elif name=='new_state':
            x=copy.deepcopy(report['candidates'][0]);x['name']='hidden';x['path']='cell_K1115A.hidden';report['candidates'].append(x)
        elif name=='duplicate':report['candidates'].append(copy.deepcopy(report['candidates'][0]))
        elif name=='derived_state':next(x for x in report['candidates']if x['name']=='p8')['category']='sequential'
        elif name=='schema_alias':manifest['words'][1]['lsb']=0
        else:manifest['original_sha256']='0'*64
        try:classify(report,manifest)
        except ValueError as e:controls.append(dict(case=name,rejected=True,diagnostic=str(e)))
        else:raise AssertionError('damaged oscillator schema accepted:'+name)
    q['failure_controls']=controls
    for name,value in [('inventory.json',r),('manifest.json',m),('classification.json',q)]:(out/name).write_text(json.dumps(value,indent=2)+'\n')
    print('OSCILLATOR CLASSIFICATION PASS:33modeled bits/8damaged schemas rejected')

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.output.resolve())
