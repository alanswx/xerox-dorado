#!/usr/bin/env python3
"""Classify actual F9401 sequential state in both DskEth PRECAP modes."""
import argparse,copy,hashlib,json,subprocess,sys,xml.etree.ElementTree as ET
from pathlib import Path
import generate_crc9401 as gen
sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory,constant


def classify(r,m):
    if m!=gen.generate()[1]:raise ValueError('unreviewed CRC9401 schema/source')
    expected={'r':('sequential',16),'cp_d':('sequential',1),
        'd_pre':('sequential',1),'cwe_pre':('sequential',1),
        'p12':('combinational_review',1),'p13':('combinational_review',1),
        'sel':('combinational_review',3),'shifted':('combinational_review',16)}
    access={'r':dict(word=0,lsb=0,width=16),'cp_d':dict(word=0,lsb=16,width=1),
            'd_pre':dict(word=0,lsb=17,width=1),'cwe_pre':dict(word=1,lsb=0,width=1)}
    decisions=[]
    for row in r['candidates']:
        name=row['name']
        if name not in expected or row['path']!='cell_F9401.'+name:raise ValueError('extra/duplicate CRC9401 field')
        category,width=expected.pop(name)
        if (row['category'],row['width'],row['bits'],row['unpacked_dimensions'])!=(category,width,width,[]):
            raise ValueError('CRC9401 field differs:'+name)
        decisions.append(dict(path=row['path'],decision='SAVE'if category=='sequential'else'RECOMPUTE',
            bits=width,access=access.get(name,'Original pinned continuous expression')))
    if expected:raise ValueError('missing CRC9401 field')
    return dict(status='PASS',modeled_state_bits=19,xml_sha256=r['xml_sha256'],decisions=decisions,
        scope='Original modeled F9401 only; every sequential field saved including PRECAP0 unused histories. No full board/global snapshot claim.')


def run(out):
    out.mkdir(parents=True,exist_ok=False);source=gen.ROOT/'verilog/cells/cell_F9401.v'
    for name in ('generate_crc9401.py','classify_crc9401.py'):(out/name).write_bytes((Path(__file__).parent/name).read_bytes())
    results=[]
    for mode in (0,1):
        d=out/('precap'+str(mode));d.mkdir()
        cmd=['verilator','--xml-only','--top-module','cell_F9401','-GPRECAP='+str(mode),'-Wno-fatal','--xml-output',str(d/'cell.xml'),str(source)]
        (d/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
        with(d/'elaborate.log').open('w')as log:subprocess.run(cmd,check=True,stdout=log,stderr=subprocess.STDOUT)
        xml=ET.fromstring((d/'cell.xml').read_bytes())
        values=[constant(v.find('const').get('name'))for v in xml.findall('.//module/var')
                if v.get('name')=='PRECAP'and v.get('param')=='true'and v.find('const')is not None]
        assert values==[mode],('actual CRC PRECAP binding differs',values,mode)
        r=inventory((d/'cell.xml').read_bytes());_,m=gen.generate();q=classify(r,m);controls=[]
        for name in ('lost_r','lost_cp_d','lost_d_pre','lost_cwe_pre','wrong_width','new_state','duplicate','derived_state','schema_alias','source_hash'):
            report,manifest=copy.deepcopy(r),copy.deepcopy(m)
            if name.startswith('lost_'):report['candidates']=[x for x in report['candidates']if x['name']!=name[5:]]
            elif name=='wrong_width':next(x for x in report['candidates']if x['name']=='r')['width']=15
            elif name=='new_state':
                x=copy.deepcopy(report['candidates'][0]);x['name']='hidden';x['path']='cell_F9401.hidden';report['candidates'].append(x)
            elif name=='duplicate':report['candidates'].append(copy.deepcopy(report['candidates'][0]))
            elif name=='derived_state':next(x for x in report['candidates']if x['name']=='p12')['category']='sequential'
            elif name=='schema_alias':manifest['words'][1]['index']=0
            else:manifest['original_sha256']='0'*64
            try:classify(report,manifest)
            except ValueError as e:controls.append(dict(case=name,rejected=True,diagnostic=str(e)))
            else:raise AssertionError('damaged CRC9401 schema accepted:'+name)
        q.update(precap=mode,failure_controls=controls)
        for name,value in [('inventory.json',r),('manifest.json',m),('classification.json',q)]:(d/name).write_text(json.dumps(value,indent=2)+'\n')
        results.append(q)
    (out/'results.json').write_text(json.dumps(dict(status='PASS',modes=results,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest()),indent=2)+'\n')
    print('CRC9401 CLASSIFICATION PASS:19bits in each PRECAP mode,10damaged schemas rejected per mode')

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.output.resolve())
