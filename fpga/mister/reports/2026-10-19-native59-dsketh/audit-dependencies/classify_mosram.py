#!/usr/bin/env python3
"""Classify every original MosRam field against its independent state map."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import sys

import generate_mosram as gen
sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(report,schema):
    a=schema['address_bits'];reference=gen.manifest(a)
    if schema!=reference:raise ValueError('unreviewed MosRam state schema')
    source=gen.ROOT/'verilog/cells/cell_MosRam.v'
    if hashlib.sha256(source.read_bytes()).hexdigest()!=gen.ORIGINAL_SHA:raise ValueError('unreviewed original MosRam')
    depth=1<<a;fields={f['name']:f for f in schema['history']['fields']}
    expected={'mem':('sequential',1,depth,[[0,depth-1]]),
        'a':('combinational_review',a//2,a//2,[]),'addr':('combinational_review',a,a,[]),
        'p14':('combinational_review',1,1,[]),'_unused_pins':('initial_only_candidate',1,1,[])}
    for n,f in fields.items():expected[n]=('sequential',f['width'],f['width'],[])
    seen=set();locations=set();decisions=[]
    for row in report['candidates']:
        n=row['name']
        if n in seen or row['path']!='cell_MosRam.'+n:raise ValueError('unexpected/duplicate original MosRam field')
        seen.add(n)
        if n not in expected or (row['category'],row['width'],row['bits'],row['unpacked_dimensions'])!=expected.pop(n):
            raise ValueError('unclassified/different original MosRam state: '+n)
        item=dict(path=row['path'],bits=row['bits'])
        if n=='mem':
            item.update(decision='SAVE',access=dict(words=[0,depth-1],bit=0));locations.update((i,0)for i in range(depth))
        elif n in fields:
            f=fields[n];word=depth+f['lsb']//18;bit=f['lsb']%18
            if bit+f['width']>18:raise ValueError('unreviewed split history field')
            where={(word,b)for b in range(bit,bit+f['width'])}
            if where&locations:raise ValueError('aliased RAM/history bit')
            locations.update(where);item.update(decision='SAVE',access=dict(word=word,lsb=bit,width=f['width']))
        else:item.update(decision='RECOMPUTE',reason='Pinned original continuous address/output expression or AND-with-literal-zero unused pins')
        decisions.append(item)
    if expected:raise ValueError('missing original MosRam state')
    bits=sum(x['bits']for x in decisions if x['decision']=='SAVE')
    if bits!=depth+a+7 or len(locations)!=bits:raise ValueError('incomplete/aliased modeled state')
    return dict(status='PASS',modeled_state_bits=bits,xml_sha256=report['xml_sha256'],decisions=decisions,
        scope='Pinned original14/16-bit MosRam socket, independent histories preserved; not full MemX/whole machine or physical FPGA restore')


def controls(report,a):
    result=[]
    for name in ('lost_row','lost_raw','lost_output','lost_pending','lost_write_data','new_state','duplicate',
                 'ram_depth','wrong_width','ram_history_alias','wrong_history_padding','wrong_density'):
        r,s=copy.deepcopy(report),gen.manifest(a)
        lost={'lost_row':'row','lost_raw':'dout_r','lost_output':'dout','lost_pending':'write_pending','lost_write_data':'write_data'}
        if name in lost:r['candidates']=[x for x in r['candidates']if x['name']!=lost[name]]
        elif name=='new_state':
            x=copy.deepcopy(r['candidates'][0]);x['name']='unexported';x['path']='cell_MosRam.unexported';r['candidates'].append(x)
        elif name=='duplicate':r['candidates'].append(copy.deepcopy(r['candidates'][0]))
        elif name=='ram_depth':next(x for x in r['candidates']if x['name']=='mem')['unpacked_dimensions']=[[0,255]]
        elif name=='wrong_width':next(x for x in r['candidates']if x['name']=='row')['width']+=1
        elif name=='ram_history_alias':s['history']['first_word']=0
        elif name=='wrong_history_padding':s['history']['width']+=1
        else:s['address_bits']=12
        try:classify(r,s)
        except ValueError as e:result.append(dict(case=name,rejected=True,diagnostic=str(e)))
        else:raise AssertionError('damaged MosRam coverage accepted:'+name)
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    out=a.output.resolve();out.mkdir(parents=True,exist_ok=False);summary={}
    for bits in (14,16):
        case=out/str(bits);case.mkdir();source=gen.ROOT/'verilog/cells/cell_MosRam.v'
        command=['verilator','--xml-only','--top-module','cell_MosRam','-Wno-fatal',f'-GADDR_BITS={bits}',
            '--xml-output',str(case/'cell.xml'),str(source)]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'elaborate.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        report=inventory((case/'cell.xml').read_bytes());(case/'inventory.json').write_text(json.dumps(report,indent=2)+'\n')
        schema=gen.manifest(bits);q=classify(report,schema);q['failure_controls']=controls(report,bits);summary[str(bits)]=q
        (case/'schema.json').write_text(json.dumps(schema,indent=2)+'\n')
    (out/'classification.json').write_text(json.dumps(summary,indent=2)+'\n')
    print('MOSRAM CLASSIFICATION PASS:16405/65559 modeled bits,21/23history bits for14/16address bits,12damaged controls per density reject')
