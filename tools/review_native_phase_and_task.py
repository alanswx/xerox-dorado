#!/usr/bin/env python3
"""Retain actual native phase/task proofs, complete replays and measured timing.

The85-pin guard fit is a terminal rejection, not a speed gain. The new shared
task closure removes a proved terminal-settle dependency without adding guest
cycles. Pending Icarus/native/Quartus work requires current process evidence;
no pending run is counted as passing and no FPGA image is assembled or loaded.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from qualify_native_phase_fall_guard import CONTROL,transform
from qualify_native_task_terminal import TASK,transform as task_transform
from review_baseboard_clock_consumer import verify
from review_cp_paired_read import observed
from review_native_alu_capture import retain
from review_processor_branch_composition import checked
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=[Path(__file__).name,'qualify_native_phase_fall_guard.py','replay_native_phase_fall_guard.py',
       'test_native_phase_guard_four_state.py','stage_native_phase_guard_timing.py','start_native_phase_guard_timing.py',
       'review_native_cp_read_timing.py','qualify_native_task_terminal.py','test_native_task_terminal_four_state.py',
       'lint_r_front_control_native.py']


def read(p):return json.loads(p.read_text())


def proof(d,count,status):
    r=read(d/'run-state.json');verify(d,r['files'])
    assert r['status']==status and len(r['actions'])==count and r['assumptions']==0
    for a in r['actions']:
        p=d/a['label'];verify(p,a['files']);assert sha(p/'proof.log')==a['log_sha256']
        assert a['passed']and a['returncode']==0
        script=(p/'proof.ys').read_text()
        assert '-prove 'in script and '-show-ports'in script and '-set 'not in script
    return r


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    guard=BUILD/'native-phase-fall-guard-proof-v3-20261025'
    sparse=BUILD/'native-sparse-fall-frames-proof-v1-20261025'
    task=BUILD/'native-task-terminal-proof-v1-20261025'
    for d in [guard,sparse]:
        r=proof(d,6,'PASS_ACTUAL_NATIVE_PHASE_FALL_GUARD')
        assert r['symbolic_input_bits']==956 and r['compared_result_bits']==795
        assert r['removed_input_bits']==[*range(20,35),333,334]
        assert (d/'native-control-after.sv').read_text()==transform((d/'native-control-before.sv').read_text(),sparse_frames=d==sparse)
        assert {a['mode']for a in r['actions']}=={'positive','wrong-selector','without-certificate','cover-clock-and-data','post-HIGH-identity'}
        retain(d,out/d.name)
    t=proof(task,3,'PASS_ACTUAL_NATIVE_TASK_TERMINAL_CLOSURE')
    assert t['arbitrary_binary_input_bits']==678 and t['compared_outputs']==139
    assert (task/'native-task-after.sv').read_text()==task_transform((task/'native-task-before.sv').read_text())
    retain(task,out/task.name)
    # Historical guard proofs retain their original supported-parameter scope.
    for n in ['native-phase-fall-guard-proof-v1-20261025','native-phase-fall-guard-proof-v2-20261025']:
        d=BUILD/n;r=read(d/'run-state.json');assert r['status']=='PASS_ACTUAL_NATIVE_PHASE_FALL_GUARD'
        retain(d,out/n)
    replays=[];pending=[]
    for prefix in ['native-phase-guard','native-sparse-fall-frames','native-task-terminal']:
        for mode in ['full','words']:
            for engine in ['icarus','verilator']:
                d=BUILD/f'{prefix}-{mode}-{engine}-v1-20261025';s,row=observed(d)
                assert s['original_assertions_and_reference_unchanged']and s['native_cells_j19_single_SRAM_and_commit_barriers_retained']
                expected_proof=guard if prefix=='native-phase-guard'else sparse
                assert s['proof_state_sha256']==sha(expected_proof/'run-state.json')
                assert sha(d/'candidate.sv')==sha(expected_proof/'native-control-after.sv')
                if prefix=='native-task-terminal':
                    assert sha(d/'adapters/dorado_local_task_capture_step.sv')==sha(task/'native-task-after.sv')
                    assert s['task_terminal_proof_state_sha256']==sha(task/'run-state.json')
                if s['status']=='RUNNING':
                    assert engine=='icarus'and row['verified_live'];pending.append(row)
                    target=out/'pending-source-observations'/d.name;target.mkdir(parents=True)
                    for source in s['source_files']:
                        f=target/source['path'];f.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(d/source['path'],f)
                    (target/'run-state-at-observation.json').write_text(json.dumps(s,indent=2)+'\n')
                    (target/'process-observation.json').write_text(json.dumps(row,indent=2)+'\n')
                else:
                    assert s['status']=='PASS_ACTUAL_NATIVE_PHASE_GUARD_PROCESSOR_REPLAY'
                    assert len(re.findall(r'^PACKET_CLOCK channel=',(d/'test.log').read_text(),re.M))==100
                    pattern=r'^(?:PACKET_CLOCK|FULL_PROCESSOR_PACKET_PASS|CP_PACKET_GEOMETRY_REJECTIONS|CP_PACKET_STAGED_READS|CACHED_PACKET_COVERAGE|NATIVE_R_PACKET_COVERAGE) .*$'
                    assert s['outcomes']==re.findall(pattern,(Path(s['source'])/'test.log').read_text(),re.M)
                    retain(d,out/d.name)
                replays.append(row)
    four_state=[]
    for prefix in ['native-phase-guard','native-sparse-fall-frames','native-task-terminal']:
        for bad in [False,True]:
            d=BUILD/(prefix+('-bad'if bad else'')+'-xz-v1-20261025');s=checked(d)
            expected=('PASS_NATIVE_TASK_TERMINAL_'if prefix=='native-task-terminal'else'PASS_NATIVE_PHASE_GUARD_')+('UNKNOWN_COUNTEREXAMPLE'if bad else'XZ')
            assert s['status']==expected
            log=(d/'test.log').read_text()
            if bad:
                diffs=[int(x)for x in re.findall(r'output=(\d+) ',log)]
                assert any((0<k<139)if prefix=='native-task-terminal'else k<363 for k in diffs)
            else:
                assert ('known=3 unknown=4068 checks=4071 outputs=139'if prefix=='native-task-terminal'else
                        'known=4 unknown=7648 checks=7652 outputs=795')in log
            four_state.append(dict(directory=d.name,status=s['status'],state_sha256=sha(d/'run-state.json')));retain(d,out/d.name)
    d=BUILD/'native-sparse-fall-frames-bad-full-verilator-v1-20261025';s=checked(d)
    log=(d/'test.log').read_text();assert s['status']=='PASS_NATIVE_PHASE_FRAME_COUNTEREXAMPLE'
    assert 'record=6732'in log and 'bit=1538 actual=1 expected=0'in log and 'bit=1543 actual=1 expected=0'in log
    retain(d,out/d.name)
    timing=BUILD/'native-phase-guard-full15-timing-review-v1-20261025';r=read(timing/'results.json')
    assert r['status']=='REVIEWED_FULL_CORE_15NS_TIMING_REJECTION'and r['worst_setup_path']['slack_ns']==-12.480
    assert len(r['terminal_state']['stages'])==9 and all(a['returncode']==0 for a in r['terminal_state']['stages'])
    collection=read(timing/'terminal/collection.json');verify(timing/'terminal',collection['collected_hashes'])
    assert sha(timing/'terminal.tgz')==r['bundle_sha256']
    old=read(BUILD/'native-cp-read-full15-timing-review-v1-20261025/results.json')
    assert old['worst_setup_path']['slack_ns']==-11.669
    retain(timing,out/timing.name)
    parent=BUILD/'native-cp-read-full15-stage-v2-20261025'
    for name in ['native-phase-guard-full15-stage-v1-20261025','native-task-terminal-full15-stage-v1-20261025']:
        d=BUILD/name;m=read(d/'r-front-control-native.json');verify(d,m['source_hashes'])
        assert len(m['source_hashes'])==574 and m['frequency_hz']==66666667 and m['sysper']==4 and not m['assembly_permitted']
        is_task=name.startswith('native-task-terminal')
        changes={CONTROL:transform((parent/CONTROL).read_text(),sparse_frames=is_task).encode()}
        if is_task:changes[TASK]=task_transform((parent/TASK).read_text()).encode()
        original=read(parent/'r-front-control-native.json')
        for n in original['source_hashes']:
            expected=changes.get(n,(parent/n).read_bytes())
            if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:
                expected=expected.replace(original['project'].encode(),m['project'].encode())
            assert(d/n).read_bytes()==expected,n
        retain(d,out/name)
    lint=BUILD/'native-task-terminal-full15-lint-v2-20261025';l=read(lint/'run-state.json')
    assert l['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION'and l['structural_warnings_visible']
    assert l['stage_manifest_sha256']==sha(BUILD/'native-task-terminal-full15-stage-v1-20261025/r-front-control-native.json')
    warnings=len(re.findall(r'^%Warning-UNOPTFLAT:',(lint/'lint.log').read_text(),re.M));assert warnings==10
    retain(lint,out/lint.name)
    launch=BUILD/'native-task-terminal-full15-launch-v1-20261025';la=read(launch/'launch-observation.json')
    project=la['project'];assert la['builder_pid']==3440305 and la['verified_source_count']==574 and not la['assembly_permitted']
    query='''from pathlib import Path
import hashlib,json,subprocess,time
p=Path(PROJECT);s=json.loads((p/'run-state.json').read_text());m=json.loads((p/'r-front-control-native.json').read_text())
assert s['builder_pid']==3440305 and not s['assembly_permitted']
actual={n:hashlib.sha256((p/n).read_bytes()).hexdigest()for n in m['source_hashes']};assert actual==m['source_hashes']and len(actual)==574
row=dict(observed=time.time(),run_state=s,source_readback=actual)
if s['status']=='RUNNING':
 a=s['stages'][-1];ps=subprocess.run(['ps','-p',str(a['pid']),'-o','pid=,etime=,time=,stat=,args='],capture_output=True,text=True)
 assert ps.returncode==0 and 'quartus_'in ps.stdout
 row.update(verified_live=True,process=ps.stdout.strip())
else:assert s['status']=='TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
print(json.dumps(row))
'''.replace('Path(PROJECT)','Path('+repr(project)+')',1)
    response=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=query,text=True,capture_output=True)
    assert response.returncode==0,response.stderr
    fit_observation=json.loads(response.stdout)
    (out/'task-timing-process-observation.json').write_text(json.dumps(fit_observation,indent=2)+'\n')
    retain(launch,out/launch.name)
    retain(BUILD/'native-phase-guard-full15-launch-v1-20261025',out/'native-phase-guard-full15-launch-v1-20261025')
    native=[]
    for name,limit in [('native-cp-read-j19-boot-v1-20261025',5000000000),
                       ('control-short-native-jumper-boot-v2-20261024',5000000000),
                       ('native-alu-source-boot-prefix-v1-20261025',600000000)]:
        d=BUILD/name;s=read(d/'run-state.json');assert s['status']=='RUNNING'
        a=s['actions'][-1];assert a.get('stage',a.get('label'))=='test'and'returncode'not in a
        process=subprocess.check_output(['ps','-p',str(a['pid']),'-o','pid=,etime=,time=,stat=,command='],text=True).strip()
        assert str(d)in process and int((d/'host-cycle-limit.txt').read_text())==limit
        progress=[x for x in(d/'test.log').read_text().splitlines()if x.startswith('NATIVE_PROGRESS')][-1]
        native.append(dict(directory=name,pid=a['pid'],process=process,HOST_limit=limit,progress=progress,
                           observed=time.time(),state_sha256=sha(d/'run-state.json')))
    assert sha(BUILD/'cedar-work-native-hints.pack')=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    # Retain the two tooling reader rejections; they changed no staged sources.
    (out/'preflight-rejections.json').write_text(json.dumps([
        dict(action='first task candidate preparation',error='Flat checked() log reader looked for seven-wave-complete-result.log; actual proof.log is nested',
             correction='Verify each nested proof source/log hash explicitly before staging',stage_created=False),
        dict(action='native-task-terminal-full15-lint-v1-20261025',error='Lint tool accepted only older stage status',
             correction='Explicitly support frozen574-input diagnostic status with frequency/proof/no-exception checks',simulation_started=False)],indent=2)+'\n')
    result=dict(status='REVIEWED_NATIVE_PHASE_GUARD_REJECTION_AND_TASK_TERMINAL_CLOSURE',scope=__doc__,
        guard_proof_input_bits=956,guard_proof_output_bits=795,task_proof_input_bits=678,task_proof_output_bits=139,
        all_arbitrary_frame_bits_included=True,no_formal_assumptions=True,
        guard_replays=replays,pending_verified_live_replays=pending,four_state=four_state,
        wrong_shared_StartCycle_frame_counterexample=dict(record=6732,result_bits=[1538,1543],actual_clock_events=[12,17],stored_Q_counterexample=False),
        phase_guard_only_full15=r,previous_worst_setup_ns=-11.669,new_guard_only_worst_setup_ns=-12.480,
        task_closure_waves=7,original_task_waves=16,known_shared_terminal_unsettled_proved_zero=True,
        task_candidate_structural_warnings=warnings,task_candidate_timing=fit_observation,native_jobs=native,
        original_firmware_and_media_retained=True,native_phase_arrival_contract_qualified=False,
        full_core_timing_qualified=False,SDRAM66_7_protocol_and_timing_qualified=False,
        nominal_diagnostic_ordinary_ns=120,target_ordinary_ns=60,assembly_permitted=False,
        hardware_changed=False,selected_speed_gain=False,full_goal_active=True,
        next_action='Follow the SAME task-terminal full-core fit and native phase diagnostic; inspect actual timing, close native producer arrivals, then integrate four15ns CPU phases and qualify memory/device timing and original Cedar')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-phase-task',extra_tools=TOOLS)
    print(r['status'],flush=True)
