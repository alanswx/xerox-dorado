#!/usr/bin/env python3
"""Account actual Serial73 QSF state, including eight required readiness bits.

Retain every Serial72 field and binding. Automatic predictor locals must have
function lifetime and no persistent writes. External aggregate state requires
native export/import; physical board scan images and schema are unchanged.
"""
import argparse
from collections import Counter
import copy
import hashlib
import json
from pathlib import Path
import shlex
import subprocess
import inventory_native_disk_pause as prior
from classify_serial_boards import read_inventory,alias_controls

ROOT=prior.ROOT;P=prior.P
FIELD=P+'m.snapshot_msa_flags_ready'


def sha(raw):return hashlib.sha256(raw).hexdigest()


def classify(data,expected):
    base=copy.deepcopy(data);base['candidates']=[];added=[]
    for row in data['candidates']:
        if row['path']==FIELD:
            assert not added,'duplicate readiness register'
            assert row['category']=='sequential','readiness register hidden from sequential inventory'
            assert(row['width'],row['bits'],row['unpacked_dimensions'])==(8,8,[]),'readiness register geometry differs'
            added.append(dict(path=FIELD,bits=8,scope='WRAPPER ACCESS PENDING'))
        else:base['candidates'].append(copy.deepcopy(row))
    assert len(added)==1,'missing required readiness state'
    result=prior.classify(base,expected);decisions=result['decisions']+added;totals=Counter()
    for row in decisions:totals[row['scope']]+=row['bits']
    assert dict(totals)=={'PHYSICAL BOARD':1957048,'PRIVATE BOARD':4578,'PRIVATE TRANSPORT':69,'WRAPPER ACCESS PENDING':132619}
    result.update(status='PASS_NATIVE_READY_PREDICTION_STATE_ACCESS_PENDING',scope=__doc__,totals=dict(totals),
        decisions=decisions,new_required_wrapper_bits=8,sequential_exclusions=[])
    return result


def controls(data,expected):
    cases=['missing_ready','duplicate_ready','width_ready','dimensions_ready','hidden_ready','extra_ready']
    result=[]
    for label in cases:
        bad=copy.deepcopy(data);rows=bad['candidates'];target=next(r for r in rows if r['path']==FIELD)
        if label=='missing_ready':rows.remove(target)
        elif label in('duplicate_ready','extra_ready'):
            row=copy.deepcopy(target)
            if label=='extra_ready':row['path']+='_hidden'
            rows.append(row)
        elif label=='width_ready':target['width']+=1
        elif label=='dimensions_ready':target['unpacked_dimensions']=[[0,1]]
        else:target['category']='initial_only_candidate'
        try:classify(bad,expected)
        except AssertionError as error:result.append(dict(case=label,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged readiness inventory accepted: '+label)
    # Independently retain all old disk/memory/board geometry and alias controls.
    base=copy.deepcopy(data);base['candidates']=[r for r in base['candidates']if r['path']!=FIELD]
    return result+prior.controls(base,expected)


def run(project,reports,out):
    record=json.loads((project/'ready-prediction-integration.json').read_text());sources=[];defines=[]
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
    raw=(out/'native.xml').read_bytes();data,normalized=read_inventory(raw);data['bindings']=prior.prior.prior.bindings(normalized)
    data,review=prior.prior.review_subroutines(data,normalized);expected=prior.prior.expected_fields(reports);result=classify(data,expected)
    result['subroutine_review']=review;result['failure_controls']=controls(data,expected)+alias_controls(raw)
    result['source_hashes']=record['source_hashes']
    provenance=dict(macros=defines,source_hashes={str(p.relative_to(project)):sha(p.read_bytes())for p in sources},
        qsf_sha256=sha((project/'core/Dorado.qsf').read_bytes()))
    (out/'native-normalized.xml').write_bytes(normalized)
    for name,value in(('inventory',data),('classification',result),('expected-fields',expected),('provenance',provenance)):
        (out/(name+'.json')).write_text(json.dumps(value,indent=2)+'\n')
    print(json.dumps(dict(status=result['status'],totals=result['totals'],subroutine_required_bits=review['required_bits'],
        function_scratch_bits=review['function_scratch_bits'],failure_controls=len(result['failure_controls']))))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('project',type=Path)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.project.resolve(),a.reports.resolve(),a.output.resolve())
