#!/usr/bin/env python3
"""Account for every original/adapter MSA primitive sequential field in fresh XML.

Only three local families are covered. No native MSA board, storage frontend,
external RAM, global barrier, complete-machine snapshot or timing claim.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import generate_msa_ff as gen
import test_msa_ff as test

sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(data):
    expected={}
    for i,(kind,bits) in enumerate(test.CASES):
        for side in ('cell_i','ref_i'):
            for name,width in gen.fields(kind,bits or 8):
                path=f'msa_ff_fixture.case{i}.{side}.{name}'
                expected[path]=(width,width,[])
    decisions=[]
    for row in data['candidates']:
        if row['category']!='sequential':continue
        wanted=expected.pop(row['path'],None)
        assert wanted is not None,'extra/duplicate state: '+row['path']
        assert (row['width'],row['bits'],row['unpacked_dimensions'])==wanted,'state geometry differs: '+row['path']
        decisions.append(dict(path=row['path'],bits=row['bits'],scope='PHYSICAL'))
    assert not expected,'missing state: '+str(list(expected)[:3])
    totals={side:sum(r['bits']for r in decisions if '.'+side+'.' in r['path'])for side in ('cell_i','ref_i')}
    assert totals==dict(cell_i=50,ref_i=50)
    return dict(status='PASS',scope=__doc__,physical_bits_per_side=50,private_bits=0,
                sequential_exclusions=[],decisions=decisions,xml_sha256=data['xml_sha256'])


def controls(data):
    results=[]
    for case in ('missing','extra','duplicate','width','dimensions','history_not_sequential','missing_native_8bit'):
        broken=copy.deepcopy(data)
        rows=broken['candidates']
        target=next(r for r in rows if r['category']=='sequential'and '.cell_i.'in r['path'])
        if case=='missing':rows.remove(target)
        elif case in ('extra','duplicate'):
            added=copy.deepcopy(target)
            if case=='extra':added['path']+='_hidden'
            rows.append(added)
        elif case=='width':target['width']+=1
        elif case=='dimensions':target['unpacked_dimensions']=[[0,1]]
        elif case=='history_not_sequential':
            next(r for r in rows if r['path']=='msa_ff_fixture.case3.cell_i.ras_d')['category']='initial_only_candidate'
        else:
            broken['candidates']=[r for r in rows if not r['path'].startswith('msa_ff_fixture.case3.cell_i.')]
        try:classify(broken)
        except AssertionError as error:results.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged primitive inventory accepted: '+case)
    return results


def run(tests,out):
    result=json.loads((tests/'results.json').read_text())
    assert result['status']=='PASS'
    case=tests/'candidate';inputs=tests/'executed-inputs'
    for name,digest in result['executed_inputs'].items():
        assert hashlib.sha256((inputs/name).read_bytes()).hexdigest()==digest,'tested input changed'
    for name,digest in result['generated_sha256'].items():
        assert hashlib.sha256((case/name).read_bytes()).hexdigest()==digest,'tested helper changed'
    out.mkdir(parents=True,exist_ok=False)
    paths=[case/'fixture.sv',*[case/n for n in result['generated_sha256']],*[inputs/(k+'.v')for k in gen.FIELDS]]
    command=['verilator','--xml-only','--top-module','msa_ff_fixture','-Wno-fatal','--xml-output',str(out/'state.xml'),*map(str,paths)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with (out/'elaborate.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    data=inventory((out/'state.xml').read_bytes());result=classify(data);result['failure_controls']=controls(data)
    (out/'inventory.json').write_text(json.dumps(data,indent=2)+'\n')
    (out/'classification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(dict(status='PASS',physical_bits_per_side=50,private_bits=0,failure_controls=len(result['failure_controls']))),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('tests',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.tests.resolve(),a.output.resolve())
