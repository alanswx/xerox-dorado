#!/usr/bin/env python3
"""Retain the Ready-target qualification, failed witnesses and same live jobs."""
import argparse
import json
from pathlib import Path
import re
import subprocess
import time

from replay_r_read_buffer import outcomes
from review_baseboard_clock_consumer import verify
from review_native_alu_capture import retain
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'


def review(out):
    assert not out.exists()
    stage=BUILD/'native-ready-mode-stage-v1-20261025'
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    audit=BUILD/'native-ready-mode-audit-v1-20261025';a=json.loads((audit/'results.json').read_text())
    assert a['status']=='PASS_NATIVE_READY_MODE_STRUCTURAL_AUDIT' and a['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    assert a['unchanged_original_sequential_processes']==3814 and a['added_sequential_processes']==0
    dirs=[stage,audit,BUILD/'native-ready-mode-lint-v1-20261025'];receipts={}
    for name,row in m['parallel_Ready_target_receipts'].items():
        d=Path(row['directory']);f=d/('results.json'if name=='component'else'run-state.json')
        assert sha(f)==row['state_sha256'];r=json.loads(f.read_text());receipts[name]=r['status'];dirs.append(d)
        if 'files'in r:verify(d,r['files'])
        if name!='component':assert all(v['passed']for v in r['actions'])
        if name in ['proof','native']:
            for v in r['actions']:verify(d/v['label'],v['files']);assert sha(d/v['label']/'proof.log')==v['log_sha256']
    # Retain the failed generic sparse case and both early fixture-startup
    # failures alongside the successful full-vector dual-engine replay.
    historical=['native-ready-mode-component-v1-20261025','native-ready-mode-proof-v1-20261025',
        'native-ready-mode-tests-v1-20261025','native-ready-mode-native-proof-v1-20261025',
        'native-ready-mode-witness-replay-v1-20261025','native-ready-mode-witness-replay-v2-20261025']
    dirs.extend(BUILD/n for n in historical)
    failed=json.loads((BUILD/'native-ready-mode-native-proof-v1-20261025/run-state.json').read_text())
    assert failed['status']=='TERMINAL_NATIVE_READY_MODE_NOT_ESTABLISHED'
    replays=[]
    for mode in ['full','words']:
        for engine in ['verilator','icarus']:
            d=BUILD/f'native-ready-mode-{mode}-{engine}-v1-20261025';r=json.loads((d/'run-state.json').read_text());verify(d,r['files'])
            assert r['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
            source=Path(r['source']);s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
            assert r['source_state_sha256']==sha(source/'run-state.json')
            assert sha(d/'tb.sv')==sha(source/'tb.sv') and sha(d/'caller.sv')==sha(source/'caller.sv')
            assert sha(d/'candidate.sv')==sha(stage/'src/verilog/rtl/dorado_r_front_control_step.sv')
            for name in s['files']:
                if name!='candidate.sv':assert sha(d/name)==sha(source/name),name
            row=dict(mode=mode,engine=engine,directory=str(d),state_sha256=sha(d/'run-state.json'),status=r['status'])
            if r['status']=='RUNNING':
                pid=r['actions'][-1]['pid'];ps=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,time=,stat=,args='],text=True,capture_output=True)
                assert ps.returncode==0 and str(d/'test.vvp')in ps.stdout
                row.update(status='VERIFIED_LIVE_ICARUS_REPLAY',pid=pid,process=ps.stdout.strip(),observed=time.time())
            else:
                assert r['status']=='PASS_READY_MODE_COMPLETE_CALLER_REPLAY' and len(r['actions'])==2
                assert all(v['returncode']==0 for v in r['actions'])
                for v in r['actions']:assert sha(d/(v['label']+'.log'))==v['log_sha256']
                assert outcomes((d/'test.log').read_text())==outcomes((source/'test.log').read_text())
                assert r['metrics']==s['metrics'] and r['original_results_match']
                row.update(original_results_and_hold_preparation_metrics_identical=True,metrics=r['metrics'])
            replays.append(row);dirs.append(d)
    launch=BUILD/'native-ready-mode-launch-v1-20261025';l=json.loads((launch/'launch-observation.json').read_text());dirs.append(launch)
    assert l['manifest_sha256']==sha(stage/'r-front-control-native.json') and l['verified_source_count']==582
    query='''import json,subprocess,time
from pathlib import Path
p=Path(PROJECT);s=json.loads((p/'run-state.json').read_text())
assert s['builder_pid']==BUILDER
pids=[s['builder_pid']]+[a['pid']for a in s['stages']if 'returncode'not in a]
ps=subprocess.run(['ps','-p',','.join(map(str,pids)),'-o','pid=,etime=,time=,stat=,args='],text=True,capture_output=True)
if s['status']=='RUNNING':assert ps.returncode==0 and str(s['builder_pid'])in ps.stdout
print(json.dumps({'observed':time.time(),'run_state':s,'processes':ps.stdout.strip(),'no_restart_or_assembly':True}))
'''.replace('PROJECT',repr(m['project'])).replace('BUILDER',str(l['builder_pid']))
    p=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=query,text=True,capture_output=True,check=True)
    job=json.loads(p.stdout)
    out.mkdir(parents=True)
    for d in dirs:retain(d,out/'retained'/d.name)
    (out/'same-job-observation.json').write_text(json.dumps(job,indent=2)+'\n')
    result=dict(status='REVIEWED_NATIVE_READY_MODE_QUALIFICATION_AND_SAME_JOB',
        previous_goal_turn='no progress: status response only; this turn integrates/proves/audits the candidate and launches one timing job',
        stage_manifest_sha256=sha(stage/'r-front-control-native.json'),source_files_verified=582,
        qualification_statuses=receipts,unrestricted_binary_input_bits=1982,public_outputs_proved_identical=2970,assumptions=0,
        unchanged_sequential_processes=3814,added_sequential_processes=0,added_combinational_instances=len(a['added_combinational_instances']),
        additional_host_or_guest_cycles=0,original_all_readiness_and_storage_barriers_retained=True,
        actual_native_capture_and_caller_history_unchanged=True,
        lint_warning_explanation=a['warning_explanation'],lint_warning_delta=a['lint_warning_delta'],
        sparse_counterexample='Generic body clears Ready where the existing certified sparse falling path retains it. Preserve exact sparse eligibility and Ready identity; no arbitrary state excluded.',
        witness_fixture_failures='First two Icarus fixtures initialized stimulus at declaration time and left a combinational known-input observer X. Explicit X-to-witness transition fixes fixture startup; all2970 reference bits still checked.',
        retained_historical_runs=historical,replays=replays,same_timing_job=job,
        assembly_or_hardware_changed=False,nominal_ordinary_ns=120,target_ordinary_ns=60,new_hardware_speed_gain_measured=False,
        full_goal_active=True,next_action='Observe same Quartus and Icarus processes without restart. Review all CPU/SDRAM corners and routing; reject unless physically qualified. Complete original60ns phase integration and hardware/Cedar/Checkpoint/Rollback/persistence/three-boots/two-hour gates; FPGA snapshots remain deferred.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-ready-mode',extra_tools=[
        Path(__file__).name,'prepare_ready_mode_compare.py','prove_ready_mode_compare.py','qualify_ready_mode_native.py',
        'test_ready_mode_compare.py','replay_ready_mode_counterexample.py','stage_native_ready_mode.py','audit_native_ready_mode.py',
        'replay_native_ready_mode.py','start_native_ready_mode_timing.py','start_native_phase_guard_timing.py',
        'review_native_cp_read_timing.py','collect_native_clock_enable_fit.py'])
    print(r['status'],flush=True)
