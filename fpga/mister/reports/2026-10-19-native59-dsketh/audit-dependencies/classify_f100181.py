#!/usr/bin/env python3
"""Fresh XML coverage of the source-pinned four-bit MemC ALU output latch."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

import generate_f100181 as gen
sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(report,manifest):
    _,expected=gen.generate()
    if manifest!=expected:raise ValueError('unreviewed ALU latch schema/source')
    widths={n:4 for n in ('a','b','s','f','gn','pn','lg','sum')}
    widths.update({n:1 for n in ('cin','arith','grp_g','grp_p','p2','p3','p4','p5','p8','p9','p10')})
    seen=set();decisions=[];saved=False
    for row in report['candidates']:
        n=row['name'];w=row['width']
        if n in seen or row['path']!='cell_F100181.'+n or row['unpacked_dimensions']:
            raise ValueError('duplicate/unexpected ALU candidate')
        seen.add(n)
        if n=='q':
            if (row['category'],w,row['bits'])!=('sequential',4,4):raise ValueError('latch geometry differs')
            saved=True;decisions.append(dict(path=row['path'],decision='SAVE',bits=4,lsb=0));continue
        if n=='_unused':
            if (row['category'],w,row['bits'])!=('initial_only_candidate',1,1):raise ValueError('constant became state')
            decisions.append(dict(path=row['path'],decision='RECOMPUTE',bits=1,reason='Source-pinned AND with literal zero'));continue
        if n in widths:expected_width=widths.pop(n)
        elif re.fullmatch(r'__Vdfg(?:Extracted|Regularize)_\w+',n)and w in (1,4):expected_width=w
        else:raise ValueError('unclassified ALU candidate: '+n)
        if (row['category'],w,row['bits'])!=('combinational_review',expected_width,expected_width):
            raise ValueError('derived ALU expression became state: '+n)
        decisions.append(dict(path=row['path'],decision='RECOMPUTE',bits=w,
            reason='Source-pinned continuous expression or complete original combinational function table'))
    if not saved or widths or '_unused'not in seen:raise ValueError('missing latch/derived source coverage')
    return dict(status='PASS',modeled_state_bits=4,xml_sha256=report['xml_sha256'],decisions=decisions,
        scope=manifest['scope'])


def controls(report,manifest):
    results=[]
    for name in ('missing_q','wrong_width','new_state','duplicate','combin_state',
                 'new_combin','constant_state','aliased_schema','source_hash'):
        r,m=copy.deepcopy(report),copy.deepcopy(manifest)
        q=next(x for x in r['candidates']if x['name']=='q')
        if name=='missing_q':r['candidates'].remove(q)
        elif name=='wrong_width':q['width']=3
        elif name=='new_state':
            row=copy.deepcopy(q);row.update(name='lost',path='cell_F100181.lost');r['candidates'].append(row)
        elif name=='duplicate':r['candidates'].append(copy.deepcopy(q))
        elif name=='combin_state':next(x for x in r['candidates']if x['name']=='sum')['category']='sequential'
        elif name=='new_combin':
            row=copy.deepcopy(q);row.update(name='new_comb',path='cell_F100181.new_comb',category='combinational_review');r['candidates'].append(row)
        elif name=='constant_state':next(x for x in r['candidates']if x['name']=='_unused')['category']='sequential'
        elif name=='aliased_schema':m['fields']=[('sum',4)]
        else:m['original_sha256']='0'*64
        try:classify(r,m)
        except ValueError as e:results.append(dict(case=name,rejected=True,diagnostic=str(e)))
        else:raise AssertionError('damaged ALU inventory accepted: '+name)
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
    source=gen.ROOT/'verilog/cells/cell_F100181.v'
    command=['verilator','--xml-only','--top-module','cell_F100181','-Wno-fatal','--xml-output',str(out/'cell.xml'),str(source)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with (out/'elaborate.log').open('w')as log:subprocess.run(command,check=True,stdout=log,stderr=subprocess.STDOUT)
    r=inventory((out/'cell.xml').read_bytes());_,m=gen.generate();q=classify(r,m);q['failure_controls']=controls(r,m)
    for name,value in [('inventory',r),('manifest',m),('classification',q)]:
        (out/(name+'.json')).write_text(json.dumps(value,indent=2)+'\n')
    paths=[Path(__file__).resolve(),gen.ROOT/'fpga/mister/snapshot/generate_f100181.py',
           gen.ROOT/'fpga/mister/snapshot/generate_snapshot_ff.py',gen.ROOT/'fpga/mister/snapshot/generate_contb_full.py',
           source,gen.ROOT/'tools/sil_state_inventory.py']
    (out/'provenance.json').write_text(json.dumps({str(p):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths},indent=2)+'\n')
    print('F100181 CLASSIFICATION PASS:4 modeled bits,9 bad inventories/schemas rejected')
