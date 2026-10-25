#!/usr/bin/env python3
"""Retain parallel task capture, actual native flag history and full-fit evidence.

Terminal proofs/replays and physical timing are distinct evidence. Keep failed
frontends, actual prefix mismatches, inferred-history scope and uncovered clock
arrivals visible. Follow existing jobs only, without restart or FPGA changes.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time

from qualify_native_task_parallel import TASK,transform
from review_baseboard_clock_consumer import verify
from review_cp_paired_read import observed
from review_native_alu_capture import retain
from review_processor_branch_composition import checked
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=[Path(__file__).name,'qualify_native_task_parallel.py','stage_native_task_parallel.py',
       'replay_native_phase_fall_guard.py','test_native_task_terminal_four_state.py',
       'start_native_phase_guard_timing.py','review_native_cp_read_timing.py',
       'analyze_native_alu_phases.py','qualify_native_flag_file_window.py',
       'replay_native_flag_file_phases.py','qualify_native_branch_storage.py']


def read(p):return json.loads(p.read_text())


def value(signals,name):
    s=signals[name];return int(s['data'][0]if'data'in s else s['wave'][0],2)


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    proofs=[]
    for name,expected in [
        ('native-task-parallel-proof-v1-20261025','PASS_ACTUAL_NATIVE_TASK_PARALLEL_CAPTURE'),
        ('native-flag-file-window-proof-v1-20261025','TERMINAL_NATIVE_FLAG_FILE_WINDOW_NOT_ESTABLISHED'),
        ('native-flag-file-window-proof-v2-20261025','PASS_ACTUAL_NATIVE_FLAG_FILE_WINDOW_TRANSFER'),
        ('native-branch-storage-proof-v1-20261025','PASS_ACTUAL_NATIVE_BRANCH_STORAGE_TRANSFER')]:
        d=BUILD/name;r=read(d/'run-state.json');verify(d,r['files']);assert r['status']==expected
        for a in r['actions']:
            p=d/a['label'];verify(p,a['files']);assert sha(p/'proof.log')==a['log_sha256']
            assert '-set 'not in(p/'proof.ys').read_text()
            if 'v1' in name and name.startswith('native-flag-file-window'):
                assert a['returncode']!=0 and not a['passed']
                assert 'ERROR:'in(p/'proof.log').read_text()
            else:assert a['returncode']==0 and a['passed']
        if name.startswith('native-task-parallel'):
            assert r['compared_outputs']==139 and r['arbitrary_binary_input_bits']==678
            assert r['assumptions']==0
            sig={s['name']:s for s in read(d/'wrong-data-epoch/counterexample.json')['signal']}
            diff=value(sig,'actual')^value(sig,'expected');assert diff>>51
            task_wrong_epoch_real_state_bits=[k-51 for k in range(51,139)if diff>>k&1]
        retain(d,out/d.name);proofs.append(dict(directory=name,status=expected,state_sha256=sha(d/'run-state.json')))
    for n in ['dorado_flag_file_window_step.sv','dorado_native_branch_capture_step.sv']:
        assert sha(ROOT/'verilog/rtl'/n)==sha(BUILD/'native-branch-storage-proof-v1-20261025'/n)
        dest=out/'current-RTL'/n;dest.parent.mkdir(exist_ok=True);dest.write_bytes((ROOT/'verilog/rtl'/n).read_bytes())
    replays=[];pending=[]
    for mode in ['full','words']:
        for engine in ['verilator','icarus']:
            d=BUILD/f'native-task-parallel-{mode}-{engine}-v1-20261025';s,row=observed(d)
            assert s['task_parallel_proof_state_sha256']==sha(BUILD/'native-task-parallel-proof-v1-20261025/run-state.json')
            assert s['original_assertions_and_reference_unchanged']and s['native_cells_j19_single_SRAM_and_commit_barriers_retained']
            if s['status']=='RUNNING':
                assert engine=='icarus'and row['verified_live'];pending.append(row)
            else:
                assert s['status']=='PASS_ACTUAL_NATIVE_PHASE_GUARD_PROCESSOR_REPLAY'
                assert len(re.findall(r'^PACKET_CLOCK channel=',(d/'test.log').read_text(),re.M))==100
            retain(d,out/d.name);replays.append(row)
    for bad in [False,True]:
        d=BUILD/('native-task-parallel-'+('bad-'if bad else'')+'xz-v1-20261025');r=checked(d)
        assert r['status']=='PASS_NATIVE_TASK_PARALLEL_'+('UNKNOWN_COUNTEREXAMPLE'if bad else'XZ')
        if not bad:assert 'known=3 unknown=4068 checks=4071 outputs=139'in(d/'test.log').read_text()
        retain(d,out/d.name)
    previous_completed=[]
    for name in ['native-processor-terminal-full-icarus-v1-20261025',
                 'native-address-task-owner-full-icarus-v2-20261025','native-address-task-owner-words-icarus-v2-20261025']:
        d=BUILD/name;r=checked(d);assert r['status']=='PASS_ACTUAL_NATIVE_PHASE_GUARD_PROCESSOR_REPLAY'
        retain(d,out/d.name);previous_completed.append(dict(directory=name,status=r['status'],state_sha256=sha(d/'run-state.json')))
    for suffix,expected in [('icarus','PASS_NATIVE_FLAG_FILE_OBSERVED_PREFIX_COMPATIBILITY'),
                           ('verilator','PASS_NATIVE_FLAG_FILE_OBSERVED_PREFIX_COMPATIBILITY'),
                           ('missing-icarus','PASS_NATIVE_FLAG_FILE_MISSING_HISTORY_COUNTEREXAMPLE')]:
        d=BUILD/f'native-flag-file-prefix-{suffix}-v1-20261025';r=checked(d);assert r['status']==expected
        assert r['initial_history_is_inferred_not_directly_sampled']and r['all74_native_storage_bits_compared']
        retain(d,out/d.name)
    for version in [1,2]:
        d=BUILD/f'native-alu-phase-analysis-v{version}-20261025';r=read(d/'results.json')
        if version==1:assert r['observed_records']==0
        else:
            assert r['observed_records']==128 and r['matching_records']==127 and len(r['mismatches'])==1
            assert r['candidate_event_counts']==[0,0,0,0]and not r['native_source_arrival_contract_qualified']
            obs=r['observation'];run=Path(r['run'])
            with(run/'test.log').open('rb')as f:prefix=f.read(obs['log_prefix_bytes'])
            assert hashlib.sha256(prefix).hexdigest()==obs['log_prefix_sha256']
            (out/'native-ALU-exact-analyzed-log-prefix.log.gz').write_bytes(gzip.compress(prefix,mtime=0))
            native_prefix=r
        retain(d,out/d.name)
    d=BUILD/'native-branch-window-live-v1-20261025';r=read(d/'result.json')
    assert r['read_only_guest_inspection']and r['pid']==69736
    assert 'Process 69736 detached'in(d/'lldb.log').read_text();retain(d,out/d.name)
    timing=BUILD/'native-shared-task-full15-timing-review-v1-20261025';t=read(timing/'results.json')
    assert t['worst_setup_path']['slack_ns']==-11.281 and t['status']=='REVIEWED_FULL_CORE_15NS_TIMING_REJECTION'
    c=read(timing/'terminal/collection.json');verify(timing/'terminal',c['collected_hashes'])
    assert len(c['collected_hashes'])==327 and len(c['source_readback'])==574
    assert all(s['returncode']==0 for s in t['terminal_state']['stages'])and sha(timing/'terminal.tgz')==t['bundle_sha256']
    retain(timing,out/timing.name)
    stage=BUILD/'native-task-parallel-full15-stage-v1-20261025';s=read(stage/'r-front-control-native.json');verify(stage,s['source_hashes'])
    parent=Path(s['parent_stage']);p=read(parent/'r-front-control-native.json')
    assert s['incremental_functional_HDL_changes']==[TASK]and len(s['source_hashes'])==574
    assert (stage/TASK).read_text()==transform((parent/TASK).read_text())
    for n in s['source_hashes']:
        raw=(BUILD/'native-task-parallel-proof-v1-20261025/native-task-after.sv').read_bytes()if n==TASK else(parent/n).read_bytes()
        if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:raw=raw.replace(p['project'].encode(),s['project'].encode())
        assert(stage/n).read_bytes()==raw,n
    retain(stage,out/stage.name)
    d=BUILD/'native-task-parallel-full15-lint-v1-20261025';r=read(d/'run-state.json')
    assert r['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION'and r['source_files']==196
    assert r['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    warnings=len(re.findall(r'^%Warning-UNOPTFLAT:',(d/'lint.log').read_text(),re.M));assert warnings==10
    retain(d,out/d.name)
    launch=BUILD/'native-task-parallel-full15-launch-v1-20261025';la=read(launch/'launch-observation.json')
    assert la['builder_pid']==3827091 and la['manifest_sha256']==sha(stage/'r-front-control-native.json')
    query='''from pathlib import Path
import hashlib,json,subprocess,time
p=Path(PROJECT);r=json.loads((p/'run-state.json').read_text());m=json.loads((p/'r-front-control-native.json').read_text())
assert r['builder_pid']==3827091 and not r['assembly_permitted']
actual={n:hashlib.sha256((p/n).read_bytes()).hexdigest()for n in m['source_hashes']};assert actual==m['source_hashes']and len(actual)==574
row=dict(observed=time.time(),run_state=r,source_readback=actual)
if r['status']=='RUNNING':
 a=r['stages'][-1];ps=subprocess.run(['ps','-p',str(a['pid']),'-o','pid=,etime=,time=,stat=,args='],capture_output=True,text=True)
 assert ps.returncode==0 and 'quartus_'in ps.stdout;row.update(verified_live=True,process=ps.stdout.strip())
else:assert r['status']=='TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
print(json.dumps(row))
'''.replace('Path(PROJECT)','Path('+repr(s['project'])+')',1)
    response=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=query,text=True,capture_output=True)
    assert response.returncode==0,response.stderr;fit=json.loads(response.stdout)
    (out/'same-parallel-task-fit-observation.json').write_text(json.dumps(fit,indent=2)+'\n');retain(launch,out/launch.name)
    native=[]
    for name,pid,bound in [('native-cp-read-j19-boot-v1-20261025',62048,5000000000),
                           ('control-short-native-jumper-boot-v2-20261024',69736,5000000000),
                           ('native-alu-source-boot-prefix-v1-20261025',87487,600000000)]:
        d=BUILD/name;r=read(d/'run-state.json');assert int((d/'host-cycle-limit.txt').read_text())==bound
        if r['status']=='RUNNING':
            a=r['actions'][-1];assert a['pid']==pid and'returncode'not in a
            ps=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,time=,stat=,command='],capture_output=True,text=True)
            assert ps.returncode==0 and str(d/'obj/native-cedar')in ps.stdout
            native.append(dict(run=name,pid=pid,bound=bound,verified_live=True,process=ps.stdout.strip()))
        else:native.append(dict(run=name,pid=pid,bound=bound,terminal_state=r))
    medium=BUILD/'cedar-work-native-hints.pack'
    assert medium.stat().st_size==63578624 and sha(medium)=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    result=dict(status='REVIEWED_PARALLEL_TASK_CAPTURE_AND_NATIVE_STORAGE_HISTORY',scope=__doc__,
        task_parallel_proofs=proofs,task_wrong_epoch_real_state_bits=task_wrong_epoch_real_state_bits,
        task_parallel_replays=replays,pending_verified_live_Icarus=pending,task_unknown_checks=4071,
        previous_pending_replays_now_passed=previous_completed,completed_shared_owner_timing=t,
        actual_native_prefix=native_prefix,existing_native_flag_history_bits=24,native_flag_storage_transfer_bits=104,
        native_flag_history_initially_inferred_not_directly_sampled=True,
        flag_source_arrivals_and_native_owner_migration_qualified=False,
        new_task_parallel_fit=fit,native_structural_warnings_previous=7,native_structural_warnings_current=warnings,
        native_simulations=native,nominal_diagnostic_ordinary_ns=120,target_ordinary_ns=60,
        hardware_changed=False,media_changed=False,measured_speed_gain=False,full_goal_active=True,
        next_action='Follow SAME task-parallel full-core builder3827091 and Icarus peers; collect all terminal corners and measured path before selection; continue actual flag producer chronology, four15ns phases, SDRAM/timers and original Cedar/Rollback qualification')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-task-parallel',extra_tools=TOOLS)
    print(r['status'],flush=True)
