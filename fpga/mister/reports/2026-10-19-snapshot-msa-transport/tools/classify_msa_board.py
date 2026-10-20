#!/usr/bin/env python3
"""Fresh complete MSA physical/private state, against the frozen native inventory.

All 458 original fields must retain exact geometry in reference and adapter.
No sequential exclusions, RAM regrouping or substitute shadow state. This
component classification does not qualify native barriers or full snapshots.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import generate_msa_board as gen
import test_msa_board as test
from classify_serial_boards import read_inventory,alias_controls


PRIVATE={'state_boundary_ready':1,'u_ff_scan.frozen':1,'u_ff_scan.writing':1,'u_ff_scan.payload':18,
         'u_ff_scan.position':12,'u_ff_scan.first_bit':12,'u_ff_scan.busy':1,'u_ff_scan.done':1,
         'u_ff_scan.error':1,'u_ff_scan.fault':1,'u_ff_scan.result':18,
         **{f'u_storage_{i}.state_frozen':1 for i in range(16)}}


def classify(data,manifest):
    physical={r['path']:r['width']for r in test.physical_fields()}
    layout=manifest['layout'];remaining=dict(physical);positions=[]
    for row in layout:
        width=remaining.pop(row['path'],None)
        assert width==row['width'],'extra/duplicate/misshaped scan field: '+row['path']
        positions.extend(range(row['lsb'],row['lsb']+width))
    assert not remaining and positions==list(range(2690)),'missing/aliased scan state'
    assert manifest['scan']==dict(state_bits=2690,words=150,word_bits=18,last_word_bits=8,completion_edges=2690)
    expected={}
    for side in ('reference','dut'):
        for name,width in physical.items():expected[f'msa_board_fixture.{side}.{name}']=(width,'PHYSICAL '+side.upper())
    for name,width in PRIVATE.items():expected['msa_board_fixture.dut.'+name]=(width,'PRIVATE TRANSPORT')
    decisions=[]
    for row in data['candidates']:
        if row['category']!='sequential':continue
        wanted=expected.pop(row['path'],None)
        assert wanted is not None,'extra/duplicate MSA sequential state: '+row['path']
        width,scope=wanted
        assert(row['width'],row['bits'],row['unpacked_dimensions'])==(width,width,[]),'MSA state geometry differs: '+row['path']
        decisions.append(dict(path=row['path'],bits=width,scope=scope))
    assert not expected,'missing MSA state: '+str(list(expected)[:3])
    totals={scope:sum(r['bits']for r in decisions if r['scope']==scope)for scope in ('PHYSICAL DUT','PHYSICAL REFERENCE','PRIVATE TRANSPORT')}
    assert totals=={'PHYSICAL DUT':2690,'PHYSICAL REFERENCE':2690,'PRIVATE TRANSPORT':83}
    return dict(status='PASS',scope=__doc__,physical_bits_per_side=2690,private_bits=83,
                sequential_exclusions=[],decisions=decisions,xml_sha256=data['xml_sha256'],
                normalized_xml_sha256=data['normalized_xml_sha256'],resolved_module_aliases=data['resolved_module_aliases'])


def controls(data,manifest):
    results=[]
    for case in ('missing','extra','duplicate','width','dimensions','old_word_not_sequential','controller_missing','schema_field_missing','schema_alias','schema_width'):
        broken=copy.deepcopy(data);schema=copy.deepcopy(manifest);rows=broken['candidates']
        target=next(r for r in rows if r['path']=='msa_board_fixture.dut.u_storage_0.old_data')
        if case=='missing':rows.remove(target)
        elif case in('extra','duplicate'):
            added=copy.deepcopy(target)
            if case=='extra':added['path']+='_hidden'
            rows.append(added)
        elif case=='width':target['width']+=1
        elif case=='dimensions':target['unpacked_dimensions']=[[0,1]]
        elif case=='old_word_not_sequential':target['category']='initial_only_candidate'
        elif case=='controller_missing':rows.remove(next(r for r in rows if r['path']=='msa_board_fixture.dut.u_ff_scan.first_bit'))
        elif case=='schema_field_missing':schema['layout'].pop()
        elif case=='schema_alias':schema['layout'][1]['lsb']-=1
        else:schema['layout'][1]['width']-=1
        try:classify(broken,schema)
        except AssertionError as error:results.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged full MSA inventory/schema accepted: '+case)
    return results


def run(tests,out):
    result=json.loads((tests/'results.json').read_text());assert result['status']=='PASS'
    board,manifest,clones=gen.generate();assets={'msa_board.sv':board,**clones};case=tests/'candidate';inputs=tests/'executed-inputs'
    assert json.loads(json.dumps(manifest))==json.loads((tests/'manifest.json').read_text())
    for name,source in assets.items():assert (case/name).read_bytes()==source.encode(),'tested MSA source differs: '+name
    for name,digest in result['executed_inputs'].items():assert hashlib.sha256((inputs/name).read_bytes()).hexdigest()==digest,'executed input differs: '+name
    out.mkdir(parents=True,exist_ok=False)
    command=['verilator','--xml-only','--assert','--top-module','msa_board_fixture','-Wno-fatal',
             '--xml-output',str(out/'state.xml'),str(case/'fixture.sv'),*[str(case/n)for n in assets],str(inputs/'msa_ddram.v'),
             str(inputs/'dorado_storage_group.sv'),*[str(inputs/n)for n in gen.original_cells()]]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    raw=(out/'state.xml').read_bytes();data,normalized=read_inventory(raw);(out/'state-normalized.xml').write_bytes(normalized)
    classified=classify(data,manifest);classified['failure_controls']=controls(data,manifest)+alias_controls(raw)
    for name,value in [('inventory',data),('classification',classified),('manifest',manifest)]:
        (out/(name+'.json')).write_text(json.dumps(value,indent=2)+'\n')
    print(json.dumps(dict(status='PASS',physical_bits_per_side=2690,private_bits=83,
                         failure_controls=len(classified['failure_controls']))),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('tests',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.tests.resolve(),a.output.resolve())
