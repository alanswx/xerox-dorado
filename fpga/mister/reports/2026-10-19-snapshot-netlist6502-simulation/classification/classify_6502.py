#!/usr/bin/env python3
"""Fresh original-vendor XML classification of all netlist6502 node state."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import generate_6502 as gen

sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(report,manifest):
    if manifest!=gen.generate()[1]:raise ValueError('unreviewed node-state schema/source')
    seen=set();saved=[];derived=[]
    for row in report['candidates']:
        path=row['path']
        if path in seen or not path.startswith('chip_6502.'):raise ValueError('duplicate/foreign node field')
        seen.add(path)
        if path=='chip_6502.q':
            if (row['module'],row['category'],row['width'],row['bits'],row['unpacked_dimensions'])!=('chip_6502','sequential',1725,1725,[]):
                raise ValueError('node vector shape/category differs')
            saved.append(dict(path=path,bits=1725,words=manifest['words']))
        else:
            if row['category']!='combinational_review'or row['module']not in('chip_6502','LOGIC','MUX6502__N2','MUX6502__N3','MUX6502__N5','MUX6502__N6','MUX6502__N7','MUX6502__N8'):
                raise ValueError('extra state or unreviewed node field:'+path)
            derived.append(dict(path=path,bits=row['bits'],decision='RECOMPUTE from pinned vendor combinational expressions'))
    if len(saved)!=1:raise ValueError('missing node vector')
    expected={'chip_6502.ni','chip_6502.logic_00.o','chip_6502.ab','chip_6502.dbo','chip_6502.rw','chip_6502.sync'}
    if not expected<=seen:raise ValueError('missing vendor combinational interface')
    return dict(status='PASS',modeled_state_bits=1725,xml_sha256=report['xml_sha256'],save=saved,recompute=derived,
        scope='Actual pinned iterative vendor core: all1725 q nodes saved in96nonaliased words; combinational netlist recomputes without a guest advance. No behavioral6502 substitution or full machine restore claim.')


def run(out):
    out.mkdir(parents=True,exist_ok=False)
    cmd=['verilator','--xml-only','--top-module','chip_6502','-Wno-fatal','--xml-output',str(out/'original.xml'),str(gen.SOURCE)]
    (out/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as f:subprocess.run(cmd,check=True,stdout=f,stderr=subprocess.STDOUT)
    report=inventory((out/'original.xml').read_bytes());_,manifest=gen.generate();result=classify(report,manifest);controls=[]
    for name in ('lost_nodes','narrow_nodes','array_nodes','extra_state','duplicate','derived_state','schema_alias','source_hash'):
        r,m=copy.deepcopy(report),copy.deepcopy(manifest)
        q=next(x for x in r['candidates']if x['path']=='chip_6502.q')
        if name=='lost_nodes':r['candidates'].remove(q)
        elif name=='narrow_nodes':q['width']=1724
        elif name=='array_nodes':q['unpacked_dimensions']=[[0,1724]]
        elif name=='extra_state':
            q=copy.deepcopy(q);q['path']='chip_6502.hidden';r['candidates'].append(q)
        elif name=='duplicate':r['candidates'].append(copy.deepcopy(q))
        elif name=='derived_state':next(x for x in r['candidates']if x['path']=='chip_6502.ni')['category']='sequential'
        elif name=='schema_alias':m['words'][1]['lsb']=0
        else:m['original_sha256']='0'*64
        try:classify(r,m)
        except ValueError as e:controls.append(dict(case=name,rejected=True,diagnostic=str(e)))
        else:raise AssertionError('damaged netlist6502 schema accepted:'+name)
    result['failure_controls']=controls
    result['source_sha256']=hashlib.sha256(gen.SOURCE.read_bytes()).hexdigest()
    for n,v in [('inventory.json',report),('manifest.json',manifest),('classification.json',result)]:
        (out/n).write_text(json.dumps(v,indent=2)+'\n')
    for n in ('generate_6502.py','classify_6502.py'):(out/n).write_bytes((Path(__file__).parent/n).read_bytes())
    print('NETLIST6502 CLASSIFICATION PASS:1725nodes/96words;8damaged schemas rejected')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.output.resolve())
