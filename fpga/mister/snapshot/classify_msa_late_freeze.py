#!/usr/bin/env python3
"""Fresh XML accounting for the late-freeze MSA and two-bit pause coordinator.

Every old physical/private field is checked by the independent chip-width
classifier. Coordinator phase is REQUIRED WRAPPER ACCESS PENDING. No state is
excluded and no native/full-snapshot qualification is inferred.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import classify_msa_chipwidth as prior
import generate_msa_late_freeze as gen
from classify_serial_boards import read_inventory,alias_controls
from generate_serial_boards import raw_receipt

PHASE='msa_board_fixture.pause_barrier.phase'


def classify(data,manifest):
    phases=[r for r in data['candidates']if r['category']=='sequential'and r['path']==PHASE]
    assert len(phases)==1,'missing/duplicate pause phase'
    phase=phases[0]
    assert(phase['width'],phase['bits'],phase['unpacked_dimensions'])==(2,2,[]),'pause phase geometry differs'
    old=copy.deepcopy(data);old['candidates']=[r for r in old['candidates']if r['path']!=PHASE]
    result=prior.classify(old,manifest)
    result['status']='PASS_ALL_MSA_STATE_WRAPPER_PHASE_ACCESS_PENDING'
    result['scope']=__doc__;result['totals']['WRAPPER ACCESS PENDING']=2
    result['decisions'].append(dict(path=PHASE,bits=2,scope='WRAPPER ACCESS PENDING'))
    return result,old


def source_review(tests):
    report=gen.parent.base.ROOT/'fpga/mister/reports/2026-10-19-snapshot-msa-chipwidth'
    prefix='msa-v1/'
    frozen=raw_receipt(report,prefix+'candidate/msa_board.sv').decode()
    board=(tests/'candidate/msa_board.sv').read_text()
    assert gen.inverse(board)==frozen,'inverse differs from qualified board'
    old=json.loads(raw_receipt(report,prefix+'manifest.json'))
    manifest=json.loads((tests/'manifest.json').read_text())
    for key in ('layout','scan','frontends','logical_state_bits','frontend_bits'):
        assert manifest[key]==old[key],'original state schema differs: '+key
    helpers=gen.parent.generate()[2]
    for name in helpers:
        assert(tests/'candidate'/name).read_bytes()==raw_receipt(report,prefix+'candidate/'+name),'helper differs: '+name
    cases=[('wrong_hold','wire frontend_hold=state_frontend_hold || state_scan_hold_required;','wire frontend_hold=state_global_hold;'),
        ('lost_quarantine','state_frontend_hold || state_scan_hold_required','state_frontend_hold'),
        ('lost_ff_hold','wire state_hold=state_global_hold ||','wire state_hold='),
        ('early_group_hold','.state_hold(frontend_hold),','.state_hold(state_hold),'),
        ('new_state','  reg state_boundary_ready=',"  reg hidden_phase=1'b0;\n  reg state_boundary_ready=")]
    results=[]
    for label,before,after in cases:
        assert before in board
        try:assert gen.inverse(board.replace(before,after,1))==frozen
        except(AssertionError,ValueError):results.append(dict(case=label,rejected=True))
        else:raise AssertionError('damaged source accepted: '+label)
    return dict(status='PASS_EXACT_INVERSE_AND_UNCHANGED_HELPERS_SCHEMA',board_new_state_bits=0,
        new_input_ports=['state_frontend_hold'],frontend_hold_connections=16,
        frozen_board_sha256=hashlib.sha256(frozen.encode()).hexdigest(),failure_controls=results)


def run(tests,out):
    assert json.loads((tests/'results.json').read_text())['status']=='PASS'
    source=source_review(tests);manifest=json.loads((tests/'manifest.json').read_text())
    case=tests/'candidate';inputs=tests/'executed-inputs';out.mkdir(parents=True,exist_ok=False)
    files=[case/'fixture.sv',case/'msa_board.sv',*[case/n for n in gen.parent.generate()[2]],case/'memory_pause_barrier.sv',
        inputs/'msa_ddram.v',inputs/'dorado_storage_group.sv',*[inputs/n for n in gen.parent.base.original_cells()]]
    command=['verilator','--xml-only','--assert','--top-module','msa_board_fixture','-Wno-fatal','--xml-output',str(out/'state.xml'),*map(str,files)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    raw=(out/'state.xml').read_bytes();data,normalized=read_inventory(raw);result,old=classify(data,manifest)
    controls=prior.controls(old,manifest)+alias_controls(raw)
    for label in ('missing_phase','duplicate_phase','phase_width','extra_wrapper_state'):
        broken=copy.deepcopy(data);rows=broken['candidates'];target=next(r for r in rows if r['path']==PHASE)
        if label=='missing_phase':rows.remove(target)
        elif label=='duplicate_phase':rows.append(copy.deepcopy(target))
        elif label=='phase_width':target['width']=3
        else:
            extra=copy.deepcopy(target);extra['path']='msa_board_fixture.pause_barrier.hidden';rows.append(extra)
        try:classify(broken,manifest)
        except AssertionError as error:controls.append(dict(case=label,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged inventory accepted: '+label)
    result['failure_controls']=controls;result['source_review']=source
    result['executed_sources']={str(p):hashlib.sha256(p.read_bytes()).hexdigest()for p in files}
    (out/'state-normalized.xml').write_bytes(normalized)
    for name,value in [('inventory',data),('classification',result)]:
        (out/(name+'.json')).write_text(json.dumps(value,indent=2)+'\n')
    print(json.dumps(dict(status=result['status'],totals=result['totals'],failure_controls=len(controls),source_controls=len(source['failure_controls']))))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('tests',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.tests.resolve(),a.output.resolve())
