#!/usr/bin/env python3
"""Fresh eleven-board converted inventories, compared to complete qualified state.

All physical and private sequential fields retain exact geometry. No excluded
state, native wrapper/ARM pause implementation, core fit or hardware claim.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
import xml.etree.ElementTree as ET
import generate_serial_boards as gen

sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def read_inventory(raw):
    # Verilator can emit a module's double underscore literally in module.name
    # but escaped as ___05F in cell.submodname. Resolve only missing, exact,
    # unambiguous name aliases; do not alter the archived raw XML or any field.
    tree=ET.fromstring(raw);declared=tree.findall('.//module');modules={m.get('name'):m for m in declared}
    if len(modules)!=len(declared):raise ValueError('duplicate XML module definition')
    aliases=[]
    for cell in tree.findall('.//cells//cell'):
        name=cell.get('submodname')
        if name in modules:continue
        decoded=re.sub(r'__0([0-9a-fA-F]{2})',lambda m:chr(int(m[1],16)),name or '')
        # Never fall back to origName: parameter variants share that string.
        # A missing exact decoded specialization is an error, not a guess.
        if decoded not in modules:raise ValueError('unresolved XML module alias: '+str(name))
        cell.set('submodname',decoded)
        aliases.append(dict(cell=cell.get('hier'),raw=name,resolved=decoded))
    normalized=ET.tostring(tree,encoding='utf-8');data=inventory(normalized)
    data['xml_sha256']=hashlib.sha256(raw).hexdigest()
    data['normalized_xml_sha256']=hashlib.sha256(normalized).hexdigest();data['resolved_module_aliases']=aliases
    return data,normalized


def alias_controls(raw):
    results=[]
    for case in ('unknown_module','missing_exact_specialization'):
        tree=ET.fromstring(raw);modules={m.get('name'):m for m in tree.findall('.//module')}
        target=next(c for c in tree.findall('.//cells//cell') if c.get('submodname') not in modules)
        if case=='unknown_module':target.set('submodname','unreviewed_missing_module')
        else:
            decoded=re.sub(r'__0([0-9a-fA-F]{2})',lambda m:chr(int(m[1],16)),target.get('submodname'))
            node=modules[decoded];parent=next(p for p in tree.iter() if node in list(p));parent.remove(node)
        try:read_inventory(ET.tostring(tree,encoding='utf-8'))
        except ValueError as error:results.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('unresolved module alias accepted: '+case)
    return results


def classify(report,original):
    expected={}
    prefix=original['manifest']['module']+'.'
    for row in original['inventory']['candidates']:
        if row['category']=='sequential':
            assert row['path'].startswith(prefix)
            expected[row['path'][len(prefix):]]=(row['width'],row['bits'],row['unpacked_dimensions'])
    physical={r['path'][len(prefix):] for r in original['classification']['decisions'] if r['scope']=='PHYSICAL'}
    decisions=[];remaining={side:dict(expected) for side in ('reference','dut')}
    for row in report['candidates']:
        if row['category']!='sequential':continue
        path=row['path'];parts=path.split('.',2)
        assert len(parts)==3 and parts[:2] in (['global_fixture','reference'],['global_fixture','dut']),'extra outside sequential state: '+path
        side,relative=parts[1:]
        wanted=remaining[side].pop(relative,None)
        assert wanted is not None,'extra/duplicate converted state: '+path
        assert (row['width'],row['bits'],row['unpacked_dimensions'])==wanted,'state geometry differs: '+path
        decisions.append(dict(path=path,bits=row['bits'],scope='PHYSICAL' if relative in physical else 'PRIVATE TRANSPORT'))
    assert not any(remaining.values()),'missing converted/reference state: '+str({k:list(v)[:3] for k,v in remaining.items() if v})
    results={}
    for side in ('reference','dut'):
        rows=[r for r in decisions if r['path'].startswith('global_fixture.'+side+'.')]
        results[side]=dict(physical_bits=sum(r['bits'] for r in rows if r['scope']=='PHYSICAL'),
                          private_transport_bits=sum(r['bits'] for r in rows if r['scope']=='PRIVATE TRANSPORT'))
    assert results['reference']==results['dut']
    assert results['dut']['physical_bits']==original['manifest']['logical_state_bits']
    return dict(status='PASS',results=results,sequential_exclusions=[],decisions=decisions,xml_sha256=report['xml_sha256'],
        normalized_xml_sha256=report['normalized_xml_sha256'],resolved_module_aliases=report['resolved_module_aliases'])


def controls(report,original):
    results=[];rows=[r for r in report['candidates'] if r['category']=='sequential' and r['path'].startswith('global_fixture.dut.')]
    for case in ('missing','extra','duplicate','width','dimensions'):
        data=copy.deepcopy(report);target=next(r for r in data['candidates'] if r['path']==rows[0]['path'])
        if case=='missing':data['candidates'].remove(target)
        elif case in ('extra','duplicate'):
            row=copy.deepcopy(target)
            if case=='extra':row['path']+='_hidden'
            data['candidates'].append(row)
        elif case=='width':target['width']+=1
        else:target['unpacked_dimensions']=[[0,1]]
        try:classify(data,original)
        except AssertionError as error:results.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged converted inventory accepted: '+case)
    return results


def run(reports,tests,out):
    out.mkdir(parents=True,exist_ok=False);results=[]
    tested=json.loads((tests/'results.json').read_text());assert tested['status']=='PASS' and len(tested['boards'])==11
    for name,*_ in gen.fmt.SPECS:
        case=out/name;case.mkdir();source=tests/name/'inputs';original=gen.qualified(reports,name)
        compiled=json.loads((tests/name/'command.json').read_text())
        paths=[Path(p) for p in compiled if p.endswith(('.v','.sv'))]
        expected=next(r for r in tested['boards'] if r['board']==name)['source_hashes']
        assert all(gen.sha((source/p).read_bytes())==h for p,h in expected.items()),'tested source changed'
        command=['verilator','--xml-only','--top-module','global_fixture','-Wno-fatal',
                 *original['simulation_defines'],'--xml-output',str(case/'board.xml'),*map(str,paths)]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'elaborate.log').open('w')as log:subprocess.run(command,cwd=source,stdout=log,stderr=subprocess.STDOUT,check=True)
        raw=(case/'board.xml').read_bytes();data,normalized=read_inventory(raw);(case/'board-normalized.xml').write_bytes(normalized)
        result=classify(data,original)
        result['failure_controls']=controls(data,original)+alias_controls(raw);result['board']=name
        for label,value in [('inventory',data),('classification',result)]:
            (case/(label+'.json')).write_text(json.dumps(value,indent=2)+'\n')
        results.append(result);print(name+': COMPLETE GLOBAL BOARD INVENTORY PASS',result['results']['dut'],flush=True)
    assert sum(r['results']['dut']['physical_bits'] for r in results)==1944744
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,boards=results,
        physical_bits=1944744,private_transport_bits=sum(r['results']['dut']['private_transport_bits'] for r in results),
        failure_controls=sum(len(r['failure_controls']) for r in results)),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('tests',type=Path)
    p.add_argument('--reports',type=Path,default=gen.ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.reports.resolve(),a.tests.resolve(),a.output.resolve())
