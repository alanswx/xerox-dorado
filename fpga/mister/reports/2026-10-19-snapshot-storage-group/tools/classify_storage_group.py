#!/usr/bin/env python3
"""Fresh native8/12 frontend state classification; DRAM oracle is outside this XML.

Uses the tested frontend/reference source bytes and parameter bindings. Only
the simulation-only DRAM oracle block is removed from the test fixture: its
hierarchical initial RAM writes are deliberately rejected by the generic
inventory reader. All frontend and fixture address state remains accounted.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
import generate_storage_group as gen
import test_storage_group_snapshot as test

sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def frontend_fixture(source):
    assert source==test.fixture(),'tested fixture differs'
    pattern=r' for\(genvar b=0;b<(8|12);b\+\+\)begin:chip\n.*?\n end\n'
    matches=list(re.finditer(pattern,source,re.S))
    assert len(matches)==2 and [m[1]for m in matches]==['8','12']
    for i,m in reversed(list(enumerate(matches))):
        source=source[:m.start()]+f' assign chip_data_out[{i}]=0;assign chip_word[{i}]=0;\n'+source[m.end():]
    assert 'cell_MK4096P_6'not in source and 'original.mem'not in source
    return source


def classify(data):
    expected={}
    for i,width in enumerate((8,12)):
        base=f'storage_snapshot_fixture.case{i}.'
        for side in ('dut','reference'):
            for name,bits in gen.fields(width):expected[base+side+'.'+name]=(bits,'PHYSICAL '+side.upper())
        expected[base+'dut.state_frozen']=(1,'PRIVATE TRANSPORT')
        for name in ('addr','reference_addr'):expected[base+name]=(16,'FIXTURE ADDRESS')
    decisions=[]
    for row in data['candidates']:
        if row['category']!='sequential':continue
        wanted=expected.pop(row['path'],None)
        assert wanted is not None,'extra/duplicate frontend state: '+row['path']
        bits,scope=wanted
        assert (row['width'],row['bits'],row['unpacked_dimensions'])==(bits,bits,[]),'frontend geometry differs: '+row['path']
        decisions.append(dict(path=row['path'],bits=bits,scope=scope))
    assert not expected,'missing frontend state: '+str(list(expected)[:3])
    totals={scope:sum(r['bits']for r in decisions if r['scope']==scope)for scope in ('PHYSICAL DUT','PHYSICAL REFERENCE','PRIVATE TRANSPORT','FIXTURE ADDRESS')}
    assert totals=={'PHYSICAL DUT':234,'PHYSICAL REFERENCE':234,'PRIVATE TRANSPORT':2,'FIXTURE ADDRESS':64}
    return dict(status='PASS',scope=__doc__,physical_bits_per_side=234,private_bits=2,fixture_bits=64,
                sequential_exclusions=[],decisions=decisions,xml_sha256=data['xml_sha256'])


def controls(data):
    results=[]
    for case in ('old_data_missing','waiting_missing','extra','duplicate','width','dimensions','history_not_sequential','fixed_pipeline_becomes_state','frozen_missing'):
        broken=copy.deepcopy(data);rows=broken['candidates'];base='storage_snapshot_fixture.case1.dut.'
        target=next(r for r in rows if r['path']==base+'old_data')
        if case.endswith('_missing'):
            name={'old_data_missing':'old_data','waiting_missing':'waiting','frozen_missing':'state_frozen'}[case]
            rows.remove(next(r for r in rows if r['path']==base+name))
        elif case in ('extra','duplicate'):
            added=copy.deepcopy(target)
            if case=='extra':added['path']+='_hidden'
            rows.append(added)
        elif case=='width':target['width']+=1
        elif case=='dimensions':target['unpacked_dimensions']=[[0,1]]
        elif case=='history_not_sequential':next(r for r in rows if r['path']==base+'cas_d')['category']='initial_only_candidate'
        else:
            added=copy.deepcopy(target);added.update(path=base+'pipe_known',name='pipe_known',width=1,bits=1)
            rows.append(added)
        try:classify(broken)
        except AssertionError as error:results.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged frontend inventory accepted: '+case)
    return results


def run(tests,out):
    result=json.loads((tests/'results.json').read_text());assert result['status']=='PASS'
    candidate,manifest=gen.generate();assert json.loads(json.dumps(manifest))==result['manifest']
    source=tests/'candidate';inputs=tests/'executed-inputs'
    assert (source/'storage_group_snapshot.sv').read_text()==candidate
    for name,digest in result['executed_inputs'].items():assert hashlib.sha256((inputs/name).read_bytes()).hexdigest()==digest,'tested input differs'
    out.mkdir(parents=True,exist_ok=False)
    (out/'frontend-fixture.sv').write_text(frontend_fixture((source/'fixture.sv').read_text()))
    command=['verilator','--xml-only','--assert','--top-module','storage_snapshot_fixture','-Wno-fatal',
             '--xml-output',str(out/'state.xml'),str(out/'frontend-fixture.sv'),str(source/'storage_group_snapshot.sv'),str(inputs/'dorado_storage_group.sv')]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    data=inventory((out/'state.xml').read_bytes());classified=classify(data);classified['failure_controls']=controls(data)
    for name,value in [('inventory',data),('classification',classified),('manifest',manifest)]:
        (out/(name+'.json')).write_text(json.dumps(value,indent=2)+'\n')
    print(json.dumps(dict(status='PASS',physical_bits_per_side=234,private_bits=2,fixture_bits=64,failure_controls=len(classified['failure_controls']))),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('tests',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.tests.resolve(),a.output.resolve())
