#!/usr/bin/env python3
"""Fresh native inventory: preserve every old field, require nineteen phase bits.

Match all 479 physical helper PRECAP bindings to frozen Serial73 and require
PHASE_QUALIFIED=1. Keep added registers REQUIRED/ACCESS PENDING; XML cannot
prove fitted duplication, native export/import, complete snapshots or timing.
"""
import argparse
from collections import Counter
import copy
import hashlib
import json
from pathlib import Path
import shlex
import subprocess
import xml.etree.ElementTree as ET
import inventory_native_ready_prediction as prior
from classify_serial_boards import read_inventory,alias_controls
from generate_serial_boards import raw_receipt
from sil_state_inventory import constant

ROOT=prior.ROOT;P=prior.P
FIELDS={P+f'm.u_board_phase.boards[{n}].local_step'for n in range(19)}


def sha(raw):return hashlib.sha256(raw).hexdigest()


def payload_bindings(normalized):
    tree=ET.fromstring(normalized);modules={m.get('name'):m for m in tree.findall('.//module')};values={}
    for cell in tree.findall('.//cells//cell'):
        module=modules[cell.get('submodname')]
        if not any(s in module.get('origName','')for s in ('cell_MC10176_snapshot','cell_MC10231_snapshot')):continue
        values[cell.get('hier')]={v.get('name'):constant(v.find('const').get('name'))for v in module.findall('./var')
            if v.get('param')=='true'and v.find('const')is not None}
    return values


def phase_binding(normalized):
    tree=ET.fromstring(normalized);modules={m.get('name'):m for m in tree.findall('.//module')}
    selected=[c for c in tree.findall('.//cells//cell')if c.get('hier')==P+'m.u_board_phase']
    assert len(selected)==1,'missing/duplicate phase distributor'
    module=modules[selected[0].get('submodname')]
    return {v.get('name'):constant(v.find('const').get('name'))for v in module.findall('./var')
        if v.get('param')=='true'and v.find('const')is not None}


def classify(data,expected,old_payloads):
    base=copy.deepcopy(data);base['candidates']=[];remaining=set(FIELDS);added=[]
    for row in data['candidates']:
        if row['path']in FIELDS:
            assert row['path']in remaining,'duplicate phase bit';remaining.remove(row['path'])
            assert row['category']=='sequential','phase bit hidden from sequential inventory'
            assert(row['width'],row['bits'],row['unpacked_dimensions'])==(1,1,[]),'phase bit geometry differs'
            added.append(dict(path=row['path'],bits=1,scope='WRAPPER ACCESS PENDING'))
        else:base['candidates'].append(copy.deepcopy(row))
    assert not remaining,'missing required phase bits: '+str(sorted(remaining))
    assert data['phase_binding']=={'BOARDS':19},'phase distributor geometry differs'
    assert set(data['payload_bindings'])==set(old_payloads)and len(old_payloads)==479,'physical helper set differs'
    for path,values in old_payloads.items():
        assert data['payload_bindings'][path]==dict(**values,PHASE_QUALIFIED=1),'physical helper phase/PRECAP binding differs: '+path
    result=prior.classify(base,expected);decisions=result['decisions']+added;totals=Counter()
    for row in decisions:totals[row['scope']]+=row['bits']
    assert dict(totals)=={'PHYSICAL BOARD':1957048,'PRIVATE BOARD':4578,'PRIVATE TRANSPORT':69,'WRAPPER ACCESS PENDING':132638}
    result.update(status='PASS_NATIVE_PHASE_DISTRIBUTION_STATE_ACCESS_PENDING',scope=__doc__,totals=dict(totals),
        decisions=decisions,new_required_wrapper_bits=19,sequential_exclusions=[],
        phase_binding=data['phase_binding'],payload_bindings=data['payload_bindings'])
    return result


def controls(data,expected,old_payloads):
    result=[]
    for label in('missing_phase','duplicate_phase','wide_phase','dimensions_phase','hidden_phase','extra_phase',
                 'wrong_phase_count','lost_payload_optin','changed_precap','missing_payload'):
        bad=copy.deepcopy(data);rows=bad['candidates'];target=next(r for r in rows if r['path']in FIELDS)
        if label=='missing_phase':rows.remove(target)
        elif label in('duplicate_phase','extra_phase'):
            row=copy.deepcopy(target)
            if label=='extra_phase':row['path']+='_hidden'
            rows.append(row)
        elif label=='wide_phase':target['width']=2
        elif label=='dimensions_phase':target['unpacked_dimensions']=[[0,1]]
        elif label=='hidden_phase':target['category']='initial_only_candidate'
        elif label=='wrong_phase_count':bad['phase_binding']['BOARDS']=18
        else:
            path=next(iter(bad['payload_bindings']))
            if label=='missing_payload':del bad['payload_bindings'][path]
            else:bad['payload_bindings'][path]['PHASE_QUALIFIED'if label=='lost_payload_optin'else'PRECAP']^=1
        try:classify(bad,expected,old_payloads)
        except AssertionError as error:result.append(dict(case=label,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged native phase inventory accepted: '+label)
    base=copy.deepcopy(data);base['candidates']=[r for r in base['candidates']if r['path']not in FIELDS]
    return result+prior.controls(base,expected)


def run(project,reports,out):
    record=json.loads((project/'phase-distribution-integration.json').read_text());sources=[];defines=[]
    for name,digest in record['source_hashes'].items():assert sha((project/name).read_bytes())==digest,name
    for line in(project/'core/Dorado.qsf').read_text().splitlines():
        words=shlex.split(line,comments=True)
        if words[:3]==['set_global_assignment','-name','VERILOG_MACRO']:defines.append(words[3])
        if words[:3]in(['set_global_assignment','-name','VERILOG_FILE'],['set_global_assignment','-name','SYSTEMVERILOG_FILE']):
            path=Path(words[3]);remote=Path(record['project'])
            path=project/path.relative_to(remote)if path.is_absolute()and path.is_relative_to(remote)else(project/'core'/path).resolve()
            if path.is_relative_to(project/'src'):sources.append(path)
    sources=list(dict.fromkeys(sources));defines=sorted(set(defines));assert all(p.is_file()for p in sources)
    assert'DORADO_STORAGE_INPUT_DELAY=2'in defines and'DORADO_HOST_CLOCK_HZ=50000000'in defines
    out.mkdir(parents=True,exist_ok=False)
    cmd=['verilator','--xml-only','--timing','--assert','--top-module','dorado_native_machine','-Wno-fatal',
        *[f'-I{project/"src"/p}'for p in('verilog/cells','verilog/generated','verilog/vendor/6502','verilog/vendor/riot','fpga/mister/snapshot_serial')],
        *['+define+'+d for d in defines],'--xml-output',str(out/'native.xml'),*map(str,sources)]
    (out/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as log:subprocess.run(cmd,cwd=project/'src',stdout=log,stderr=subprocess.STDOUT,check=True)
    raw=(out/'native.xml').read_bytes();data,normalized=read_inventory(raw);data['bindings']=prior.prior.prior.prior.bindings(normalized)
    data,review=prior.prior.prior.review_subroutines(data,normalized);expected=prior.prior.prior.expected_fields(reports)
    data['phase_binding']=phase_binding(normalized);data['payload_bindings']=payload_bindings(normalized)
    original=raw_receipt(reports/'2026-10-20-snapshot-native-ready-prediction-rejected','qualification/classification/native-normalized.xml')
    old_payloads=payload_bindings(original)
    (out/'inventory.json').write_text(json.dumps(data,indent=2)+'\n')
    (out/'native-normalized.xml').write_bytes(normalized)
    result=classify(data,expected,old_payloads);result['subroutine_review']=review
    result['failure_controls']=controls(data,expected,old_payloads)+alias_controls(raw);result['source_hashes']=record['source_hashes']
    provenance=dict(macros=defines,source_hashes={str(p.relative_to(project)):sha(p.read_bytes())for p in sources},
        qsf_sha256=sha((project/'core/Dorado.qsf').read_bytes()))
    for name,value in(('classification',result),('expected-fields',expected),('provenance',provenance),('old-payload-bindings',old_payloads)):
        (out/(name+'.json')).write_text(json.dumps(value,indent=2)+'\n')
    print(json.dumps(dict(status=result['status'],totals=result['totals'],required_bits=sum(result['totals'].values()),
        subroutine_required_bits=review['required_bits'],function_scratch_bits=review['function_scratch_bits'],
        phase_bits=19,qualified_helpers=len(old_payloads),failure_controls=len(result['failure_controls']))))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('project',type=Path)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.project.resolve(),a.reports.resolve(),a.output.resolve())
