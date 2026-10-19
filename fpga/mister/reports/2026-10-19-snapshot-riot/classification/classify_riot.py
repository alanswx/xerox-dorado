#!/usr/bin/env python3
"""Classify fresh actual MOS RIOT XML including its procedural PA7 history."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
import xml.etree.ElementTree as ET
import generate_riot as gen

sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory,constant


def classify(report,manifest):
    if manifest!=gen.generate()[1]:raise ValueError('unreviewed RIOT source/schema')
    expected={name:('sequential',width,width,[])for name,width in gen.FIELDS}
    expected['riot_ram']=('sequential',8,1024,[[0,127]])
    expected.update({name:('combinational_review',width,width,[])for name,width in
        [('IRQ_n',1),('PA_out',8),('PB_out',8),('oe',1),('pa7',1),('tick_inc',1)]})
    decisions=[]
    for row in report['candidates']:
        name=row['name']
        if re.fullmatch(r'@(?:e|fg),\d+,\d+,\d+,\d+\.old_pa7',name):name='old_pa7'
        if row['module']!='M6532'or row['path']!='M6532.'+row['name']or name not in expected:
            raise ValueError('extra/duplicate/unreviewed RIOT field:'+row['path'])
        wanted=expected.pop(name)
        if(row['category'],row['width'],row['bits'],row['unpacked_dimensions'])!=wanted:
            raise ValueError('RIOT field geometry/category differs:'+name)
        if wanted[0]=='sequential':
            if name=='riot_ram':access=dict(kind='RAM',indices=list(range(128)),width=8)
            else:
                f=next(f for f in manifest['fields']if f['name']==name);access=[]
                for i in range(5):
                    lo=max(i*18,f['lsb']);hi=min(i*18+17,f['lsb']+f['width']-1)
                    if lo<=hi:access.append(dict(index=128+i,word_lsb=lo-i*18,field_lsb=lo-f['lsb'],width=hi-lo+1))
            decisions.append(dict(path=row['path'],decision='SAVE',bits=row['bits'],access=access))
        else:decisions.append(dict(path=row['path'],decision='RECOMPUTE',bits=row['bits']))
    if expected:raise ValueError('missing RIOT fields:'+','.join(expected))
    if sum(r['bits']for r in decisions if r['decision']=='SAVE')!=1097:raise ValueError('RIOT saved bits differ')
    return dict(status='PASS',modeled_state_bits=1097,xml_sha256=report['xml_sha256'],decisions=decisions,
        parameters=dict(init_7800=0),scope='Actual MOS RIOT only:128x8RAM+73control/history bits in133words; enclosing cell_MCS6532 phase_d is separate and remains required. No complete BaseBoard/machine snapshot claim.')


def run(out):
    out.mkdir(parents=True,exist_ok=False)
    cmd=['verilator','--xml-only','--top-module','M6532','-Ginit_7800=0','-Wno-fatal',
         '--xml-output',str(out/'original.xml'),str(gen.SOURCE)]
    (out/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as f:subprocess.run(cmd,check=True,stdout=f,stderr=subprocess.STDOUT)
    raw=(out/'original.xml').read_bytes();xml=ET.fromstring(raw)
    params=[constant(v.find('const').get('name'))for v in xml.findall('.//module/var')
            if v.get('name')=='init_7800'and v.get('param')=='true'and v.find('const')is not None]
    assert params==[0],('actual RIOT binding differs',params)
    r=inventory(raw);_,m=gen.generate();q=classify(r,m);controls=[]
    for name in ('lost_ram','lost_pa7','lost_read_output','wrong_depth','wrong_width','new_state','duplicate','derived_state','schema_alias','source_hash'):
        report,manifest=copy.deepcopy(r),copy.deepcopy(m)
        if name.startswith('lost_'):
            field={'lost_ram':'riot_ram','lost_pa7':'old_pa7','lost_read_output':'d_out'}[name]
            report['candidates']=[v for v in report['candidates']if v['name'].split('.')[-1]!=field]
        elif name=='wrong_depth':next(v for v in report['candidates']if v['name']=='riot_ram')['unpacked_dimensions']=[[0,126]]
        elif name=='wrong_width':next(v for v in report['candidates']if v['name']=='prescaler')['width']=9
        elif name=='new_state':
            v=copy.deepcopy(report['candidates'][0]);v['name']='hidden';v['path']='M6532.hidden';v['category']='sequential';report['candidates'].append(v)
        elif name=='duplicate':report['candidates'].append(copy.deepcopy(report['candidates'][0]))
        elif name=='derived_state':next(v for v in report['candidates']if v['name']=='pa7')['category']='sequential'
        elif name=='schema_alias':manifest['words'][129]['lsb']=1024
        else:manifest['original_sha256']='0'*64
        try:classify(report,manifest)
        except ValueError as e:controls.append(dict(case=name,rejected=True,diagnostic=str(e)))
        else:raise AssertionError('damaged RIOT inventory/schema accepted:'+name)
    q['failure_controls']=controls;q['source_sha256']=hashlib.sha256(gen.SOURCE.read_bytes()).hexdigest()
    for name,value in [('inventory.json',r),('manifest.json',m),('classification.json',q)]:
        (out/name).write_text(json.dumps(value,indent=2)+'\n')
    for name in ('classify_riot.py','generate_riot.py'):(out/name).write_bytes((Path(__file__).parent/name).read_bytes())
    print('RIOT CLASSIFICATION PASS:1097bits/133words/init_7800=0;10damaged schemas rejected')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.output.resolve())
