#!/usr/bin/env python3
"""Account every chip-width MSA bit and every unchanged original reference bit.

An independent frozen native inventory supplies the field list. Only the five
explicitly removed bridge registers per group are absent from the candidate;
all original FF/history, original reference and private controller state remain.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import classify_msa_board as original
import generate_msa_chipwidth as gen
import test_msa_board as oldtest
from classify_serial_boards import read_inventory,alias_controls

REMOVED={'saved_write','cache_addr','saved_addr','saved_data','old_data'}


def expected_fields():
    old=oldtest.physical_fields()
    rows=[dict(r)for r in old if not (r['path'].startswith('u_storage_')and r['path'].split('.')[-1]in REMOVED)]
    widths={r['path'].split('.')[0]:r['width']for r in old if r['path'].endswith('.write_data')}
    for row in rows:
        if row['path'].startswith('u_storage_')and row['path'].endswith('.cache_data'):row['width']=widths[row['path'].split('.')[0]]
    return old,rows


def classify(data,manifest):
    reference,candidate=expected_fields();assert len(candidate)==378 and sum(r['width']for r in candidate)==1538
    remaining={r['path']:r['width']for r in candidate};positions=[]
    for row in manifest['layout']:
        width=remaining.pop(row['path'],None);assert width==row['width'],'extra/duplicate/misshaped scan field'
        positions.extend(range(row['lsb'],row['lsb']+width))
    assert not remaining and positions==list(range(1538)),'missing/aliased scan state'
    assert manifest['scan']==dict(state_bits=1538,words=86,word_bits=18,last_word_bits=8,completion_edges=1538)
    expected={}
    for side,rows in [('reference',reference),('dut',candidate)]:
        for r in rows:expected[f'msa_board_fixture.{side}.'+r['path']]=(r['width'],'PHYSICAL '+side.upper())
    private=dict(original.PRIVATE);private.update({'u_ff_scan.position':11,'u_ff_scan.first_bit':11})
    for name,width in private.items():expected['msa_board_fixture.dut.'+name]=(width,'PRIVATE TRANSPORT')
    decisions=[]
    for row in data['candidates']:
        if row['category']!='sequential':continue
        wanted=expected.pop(row['path'],None);assert wanted is not None,'extra/duplicate MSA sequential state: '+row['path']
        width,scope=wanted
        assert (row['width'],row['bits'],row['unpacked_dimensions'])==(width,width,[]),'MSA state geometry differs'
        decisions.append(dict(path=row['path'],bits=width,scope=scope))
    assert not expected,'missing MSA state: '+str(list(expected)[:3])
    totals={s:sum(r['bits']for r in decisions if r['scope']==s)for s in ('PHYSICAL DUT','PHYSICAL REFERENCE','PRIVATE TRANSPORT')}
    assert totals=={'PHYSICAL DUT':1538,'PHYSICAL REFERENCE':2690,'PRIVATE TRANSPORT':81}
    return dict(status='PASS_ALL_CHIPWIDTH_AND_REFERENCE_STATE',scope=__doc__,totals=totals,
        sequential_exclusions=[],decisions=decisions,xml_sha256=data['xml_sha256'],normalized_xml_sha256=data['normalized_xml_sha256'],
        resolved_module_aliases=data['resolved_module_aliases'])


def controls(data,manifest):
    results=[]
    for name in ('missing','extra','duplicate','width','dimensions','not_sequential','reference_missing','controller_missing','schema_missing','schema_alias','schema_width','scan_bounds'):
        broken=copy.deepcopy(data);schema=copy.deepcopy(manifest);rows=broken['candidates']
        target=next(r for r in rows if r['path']=='msa_board_fixture.dut.u_storage_0.cache_data')
        if name=='missing':rows.remove(target)
        elif name in ('extra','duplicate'):
            added=copy.deepcopy(target)
            if name=='extra':added['path']='msa_board_fixture.dut.u_storage_0.saved_data'
            rows.append(added)
        elif name=='width':target['width']+=1
        elif name=='dimensions':target['unpacked_dimensions']=[[0,1]]
        elif name=='not_sequential':target['category']='initial_only_candidate'
        elif name=='reference_missing':rows.remove(next(r for r in rows if r['path']=='msa_board_fixture.reference.u_storage_0.saved_data'))
        elif name=='controller_missing':rows.remove(next(r for r in rows if r['path']=='msa_board_fixture.dut.u_ff_scan.first_bit'))
        elif name=='schema_missing':schema['layout'].pop()
        elif name=='schema_alias':schema['layout'][1]['lsb']-=1
        elif name=='schema_width':schema['layout'][1]['width']-=1
        else:schema['scan']['words']=150
        try:classify(broken,schema)
        except AssertionError as error:results.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged compact inventory accepted: '+name)
    return results


def run(tests,out):
    result=json.loads((tests/'results.json').read_text());assert result['status']=='PASS'
    board,manifest,helpers=gen.generate();case=tests/'candidate';inputs=tests/'executed-inputs'
    assert json.loads(json.dumps(manifest))==json.loads((tests/'manifest.json').read_text())
    for name,source in {'msa_board.sv':board,**helpers}.items():assert (case/name).read_text()==source,'tested source differs'
    for name,digest in result['executed_inputs'].items():assert hashlib.sha256((inputs/name).read_bytes()).hexdigest()==digest,'executed input differs'
    out.mkdir(parents=True,exist_ok=False)
    command=['verilator','--xml-only','--assert','--top-module','msa_board_fixture','-Wno-fatal','--xml-output',str(out/'state.xml'),
             str(case/'fixture.sv'),*[str(case/n)for n in ['msa_board.sv',*helpers]],str(inputs/'msa_ddram.v'),
             str(inputs/'dorado_storage_group.sv'),*[str(inputs/n)for n in gen.base.original_cells()]]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    raw=(out/'state.xml').read_bytes();data,normalized=read_inventory(raw);(out/'state-normalized.xml').write_bytes(normalized)
    result=classify(data,manifest);result['failure_controls']=controls(data,manifest)+alias_controls(raw)
    for name,value in [('inventory',data),('classification',result),('manifest',manifest)]:
        (out/(name+'.json')).write_text(json.dumps(value,indent=2)+'\n')
    print(json.dumps(dict(status=result['status'],totals=result['totals'],failure_controls=len(result['failure_controls']))),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('tests',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.tests.resolve(),a.output.resolve())
