#!/usr/bin/env python3
"""Stage only parallel task capture over the frozen shared-task full core.

Keep all574 exact inputs except the proved task implementation and project
path relocation in build files. Keep every clock, timer, original firmware,
device, storage barrier and timing constraint. Full+words replay comparisons
remain mandatory; a pending Icarus job is recorded only after a live ps check.
This is a timing diagnostic, with no assembly or hardware programming.
"""
import argparse
import json
from pathlib import Path
import shutil

from qualify_native_task_parallel import TASK,transform
from review_baseboard_clock_consumer import verify
from review_cp_paired_read import observed
from review_processor_branch_composition import checked
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
CONFIG={'.qsf','.qip','.qpf','.sdc','.tcl'}


def prepare(parent,proof,out,project):
    assert not out.exists()and project.parent==Path('/home/alans/mister')
    p=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,p['source_hashes'])
    assert p['frequency_hz']==66666667 and p['sysper']==4 and not p['assembly_permitted']
    q=json.loads((proof/'run-state.json').read_text());verify(proof,q['files'])
    assert q['status']=='PASS_ACTUAL_NATIVE_TASK_PARALLEL_CAPTURE'and q['assumptions']==0
    assert q['compared_outputs']==139 and q['arbitrary_binary_input_bits']==678
    assert sha(parent/TASK)==q['source_task_sha256']
    after=(proof/'native-task-after.sv').read_bytes();assert after.decode()==transform((parent/TASK).read_text())
    for a in q['actions']:
        d=proof/a['label'];verify(d,a['files']);assert a['returncode']==0 and a['passed']
        assert sha(d/'proof.log')==a['log_sha256']
    for bad in [False,True]:
        d=BUILD/('native-task-parallel-'+('bad-'if bad else'')+'xz-v1-20261025');x=checked(d)
        assert x['status']=='PASS_NATIVE_TASK_PARALLEL_'+('UNKNOWN_COUNTEREXAMPLE'if bad else'XZ')
        assert x['proof_state_sha256']==sha(proof/'run-state.json')
    replays=[];pending=[]
    for mode in ['full','words']:
        for engine in ['verilator','icarus']:
            d=BUILD/f'native-task-parallel-{mode}-{engine}-v1-20261025';s,row=observed(d)
            assert sha(d/'adapters/dorado_local_task_capture_step.sv')==sha(proof/'native-task-after.sv')
            assert s['task_parallel_proof_state_sha256']==sha(proof/'run-state.json')
            assert s['original_assertions_and_reference_unchanged']and s['native_cells_j19_single_SRAM_and_commit_barriers_retained']
            if s['status']=='RUNNING':
                assert engine=='icarus'and row['verified_live'];pending.append(row)
            else:assert s['status']=='PASS_ACTUAL_NATIVE_PHASE_GUARD_PROCESSOR_REPLAY'
            replays.append(row)
    out.mkdir(parents=True)
    for n in p['source_hashes']:
        raw=after if n==TASK else(parent/n).read_bytes()
        if Path(n).suffix in CONFIG:raw=raw.replace(p['project'].encode(),str(project).encode())
        dest=out/n;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(raw)
    hashes={n:sha(out/n)for n in p['source_hashes']};assert len(hashes)==574
    for n in hashes:
        if n!=TASK and Path(n).suffix not in CONFIG:assert hashes[n]==p['source_hashes'][n]
        if n.endswith('.sdc'):assert(out/n).read_bytes()==(parent/n).read_bytes().replace(p['project'].encode(),str(project).encode())
    for n in ['build_r_front_ready_native.py',Path(__file__).name]:shutil.copy2(ROOT/'tools'/n,out/n)
    record={**p,'status':'PREPARED_FULL_NATIVE_15NS_PHASE_GUARD_DIAGNOSTIC','scope':__doc__,
        'project':str(project),'parent_project':p['project'],'parent_stage':str(parent),
        'parent_manifest_sha256':sha(parent/'r-front-control-native.json'),'source_hashes':hashes,
        'task_parallel_proof':str(proof),'task_parallel_proof_state_sha256':sha(proof/'run-state.json'),
        'task_parallel_replays':replays,'pending_task_parallel_Icarus_peers':pending,
        'incremental_functional_HDL_changes':[TASK],
        'functional_HDL_changes':p['functional_HDL_changes'],
        'nominal_ordinary_ns':120,'full60ns_event_batch_connected':False,
        'no_new_timing_exceptions':True,'assembly_permitted':False,'physical_timing_qualified':False,
        'firmware_qualified':False,'hardware_speedup_qualified':False,'full_goal_active':True}
    (out/'r-front-control-native.json').write_text(json.dumps(record,indent=2)+'\n')
    print('STAGED_NATIVE_TASK_PARALLEL',len(hashes),'exact inputs; incremental functional HDL changes1; pending',len(pending))
    return record


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','proof','output','project']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();prepare(a.parent.resolve(),a.proof.resolve(),a.output.resolve(),a.project)
