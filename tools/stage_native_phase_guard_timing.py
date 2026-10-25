#!/usr/bin/env python3
"""Stage the actual85-pin shared fall guard in the SAME full-core15ns fixture.

Select the proved guard, optional sparse frames and task terminal closure.
Preserve all remaining574 inputs,
constraints, PLL/SYSPER and device/firmware code. Pending simulator peers must
be observed live, never treated as a pass. Map/fit/STA only; no assembly/load.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from qualify_native_phase_fall_guard import CONTROL,transform
from review_baseboard_clock_consumer import verify
from review_cp_paired_read import observed
from review_processor_branch_composition import checked
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'


def prepare(parent,proof,out,project,task_proof=None):
    assert not out.exists()and project.parent==Path('/home/alans/mister')
    old=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,old['source_hashes'])
    assert old['frequency_hz']==66666667 and old['sysper']==4 and not old['assembly_permitted']
    p=json.loads((proof/'run-state.json').read_text());verify(proof,p['files'])
    assert p['status']=='PASS_ACTUAL_NATIVE_PHASE_FALL_GUARD'and len(p['actions'])==6
    assert all(a['passed']and a['returncode']==0 for a in p['actions'])
    assert p['source_control_sha256']==sha(parent/CONTROL)
    sparse=p.get('sparse_shared_capture_frames',False)
    assert (proof/'native-control-after.sv').read_text()==transform((parent/CONTROL).read_text(),sparse_frames=sparse)
    prefix='native-sparse-fall-frames'if sparse else'native-phase-guard'
    gate=BUILD/(prefix+'-xz-v1-20261025');xz=checked(gate)
    bad=BUILD/(prefix+'-bad-xz-v1-20261025');negative=checked(bad)
    assert xz['status']=='PASS_NATIVE_PHASE_GUARD_XZ'and negative['status']=='PASS_NATIVE_PHASE_GUARD_UNKNOWN_COUNTEREXAMPLE'
    assert xz['proof_state_sha256']==negative['proof_state_sha256']==sha(proof/'run-state.json')
    replays=[];pending=[]
    for mode in ['full','words']:
        for engine in ['verilator','icarus']:
            d=BUILD/f'{prefix}-{mode}-{engine}-v1-20261025';s,row=observed(d)
            assert (d/'candidate.sv').read_text()==(proof/'native-control-after.sv').read_text()
            assert s['original_assertions_and_reference_unchanged']
            if s['status']=='RUNNING':
                assert engine=='icarus'and row['verified_live']
                pending.append(row)
            else:
                assert s['status']=='PASS_ACTUAL_NATIVE_PHASE_GUARD_PROCESSOR_REPLAY'
            replays.append(row)
    changed={CONTROL:(proof/'native-control-after.sv').read_bytes()}
    task_replays=[];task_pending=[]
    if task_proof:
        from qualify_native_task_terminal import TASK,transform as task_transform
        t=json.loads((task_proof/'run-state.json').read_text());verify(task_proof,t['files'])
        assert t['status']=='PASS_ACTUAL_NATIVE_TASK_TERMINAL_CLOSURE'and len(t['actions'])==3
        assert all(a['passed']and a['returncode']==0 for a in t['actions'])
        for a in t['actions']:
            d=task_proof/a['label'];verify(d,a['files']);assert sha(d/'proof.log')==a['log_sha256']
        assert t['source_task_sha256']==sha(parent/TASK)
        assert (task_proof/'native-task-after.sv').read_text()==task_transform((parent/TASK).read_text())
        for bad,expected in [(False,'PASS_NATIVE_TASK_TERMINAL_XZ'),(True,'PASS_NATIVE_TASK_TERMINAL_UNKNOWN_COUNTEREXAMPLE')]:
            gate=BUILD/('native-task-terminal-'+('bad-'if bad else'')+'xz-v1-20261025');x=checked(gate)
            assert x['status']==expected and x['proof_state_sha256']==sha(task_proof/'run-state.json')
        for mode in ['full','words']:
            for engine in ['verilator','icarus']:
                d=BUILD/f'native-task-terminal-{mode}-{engine}-v1-20261025';s,row=observed(d)
                assert (d/'candidate.sv').read_text()==changed[CONTROL].decode()
                assert sha(d/'adapters/dorado_local_task_capture_step.sv')==sha(task_proof/'native-task-after.sv')
                assert s['original_assertions_and_reference_unchanged']
                assert s['task_terminal_proof_state_sha256']==sha(task_proof/'run-state.json')
                if s['status']=='RUNNING':
                    assert engine=='icarus'and row['verified_live'];task_pending.append(row)
                else:assert s['status']=='PASS_ACTUAL_NATIVE_PHASE_GUARD_PROCESSOR_REPLAY'
                task_replays.append(row)
        changed[TASK]=(task_proof/'native-task-after.sv').read_bytes()
    out.mkdir(parents=True)
    for n in old['source_hashes']:
        raw=(parent/n).read_bytes()
        if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:
            raw=raw.replace(old['project'].encode(),str(project).encode())
        if n in changed:raw=changed[n]
        path=out/n;path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(raw)
    new_hashes={n:sha(out/n)for n in old['source_hashes']}
    for n,h in old['source_hashes'].items():
        if n not in changed and Path(n).suffix not in ['.qsf','.qip','.qpf','.sdc','.tcl']:assert new_hashes[n]==h,n
        if n.endswith('.sdc'):
            assert (out/n).read_bytes()==(parent/n).read_bytes().replace(old['project'].encode(),str(project).encode()),n
    for n in ['build_r_front_ready_native.py',Path(__file__).name]:shutil.copy2(ROOT/'tools'/n,out/n)
    record={**old,'status':'PREPARED_FULL_NATIVE_15NS_PHASE_GUARD_DIAGNOSTIC','scope':__doc__,
        'project':str(project),'parent_project':old['project'],'parent_stage':str(parent),
        'parent_manifest_sha256':sha(parent/'r-front-control-native.json'),'source_hashes':new_hashes,
        'phase_guard_proof':str(proof),'phase_guard_proof_state_sha256':sha(proof/'run-state.json'),
        'phase_guard_old_pins':102,'phase_guard_new_pins':85,'all795_control_and_shared_frame_bits_proved':True,
        'actual_native_HIGH_identity_closure_proved':True,'unqualified_wave_parameters_keep_original_guard':True,
        'four_state_7652_checks_passed':True,'bad_unknown_default_functional_counterexample':True,
        'processor_replay_observations':replays,'pending_Icarus_peers_at_staging':pending,
        'sparse_shared_capture_frames':sparse,
        'task_terminal_proof':str(task_proof)if task_proof else None,
        'task_terminal_proof_state_sha256':sha(task_proof/'run-state.json')if task_proof else None,
        'task_terminal_replay_observations':task_replays,'pending_task_Icarus_peers_at_staging':task_pending,
        'assembly_permitted':False,'physical_timing_qualified':False,'firmware_qualified':False,
        'hardware_speedup_qualified':False,'no_new_timing_exceptions':True,
        'full60ns_event_batch_connected':False,'new_branch_condition_family_integrated':False,
        'nominal_ordinary_ns':120,'functional_HDL_changes':sorted(changed),'full_goal_active':True}
    assert len(new_hashes)==574
    (out/'r-front-control-native.json').write_text(json.dumps(record,indent=2)+'\n')
    print(record['status'],len(new_hashes),'inputs; pending peers',len(pending)+len(task_pending));return record


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','proof','output','project']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--task-proof',type=Path)
    a=p.parse_args();prepare(a.parent.resolve(),a.proof.resolve(),a.output.resolve(),a.project,
                            a.task_proof.resolve()if a.task_proof else None)
