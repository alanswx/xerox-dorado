#!/usr/bin/env python3
"""Review actual shared task ownership and the preceding full-core rejection.

Full interface proofs preserve every guard, state bit, RAM and capture epoch.
Pending processes are observations, not passes; this candidate still has120ns
nominal ordinary instructions and is not hardware or memory qualification.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess
import time

from qualify_native_address_task_owner import SOURCE,transform
from review_baseboard_clock_consumer import verify
from review_cp_paired_read import observed
from review_native_alu_capture import retain
from review_native_processor_terminal import review as processor_review,TOOLS as PROCESSOR_TOOLS
from review_processor_branch_composition import checked
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=[*PROCESSOR_TOOLS,Path(__file__).name,'prove_native_address_task_owner.py',
       'qualify_native_address_task_owner.py','test_native_address_task_owner_four_state.py']


def read(p):return json.loads(p.read_text())


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    previous=processor_review(out/'processor-terminal')
    assert not previous['previous_phase_task_review']['pending_verified_live_replays']
    invariant=BUILD/'native-address-task-owner-proof-v1-20261025';iv=read(invariant/'run-state.json')
    verify(invariant,iv['files']);assert iv['status']=='PROVED_ACTUAL_NATIVE_ADDRESS_TASK_OWNER_EQUIVALENCE'
    assert iv['compared_outputs']==139 and iv['assumptions']==0 and sha(invariant/'proof.log')==iv['log_sha256']
    assert 'no model found: SUCCESS!'in(invariant/'proof.log').read_text()
    retain(invariant,out/invariant.name)
    proofs=[]
    for version in [1,2]:
        d=BUILD/f'native-address-task-owner-qualification-v{version}-20261025';r=read(d/'run-state.json');verify(d,r['files'])
        assert r['status']=='PASS_ACTUAL_NATIVE_SHARED_ADDRESS_TASK_OWNER'and r['assumptions']==0
        assert r['actual_arbitrary_binary_input_bits']==2114 and r['compared_outputs']==807
        assert r['invariant_original_reported_input_bits']==2113
        for a in r['actions']:
            p=d/a['label'];verify(p,a['files']);assert a['passed']and a['returncode']==0
            assert sha(p/'proof.log')==a['log_sha256']and '-set 'not in(p/'proof.ys').read_text()
        negative=read(d/'wrong-StartCycle-alias/counterexample.json')
        signals={s['name']:s for s in negative['signal']}
        def value(name):
            s=signals[name];return int(s['data'][0],2)if'data'in s else int(s['wave'][0],2)
        diff=value('actual')^value('expected')
        # Output[784:753] is the real32-bit address;726 is unsettled.
        assert (diff>>753)&((1<<32)-1)
        proofs.append(dict(version=version,actual_address_counterexample_bits=[k-753 for k in range(753,785)if diff>>k&1],
                           complete_interface_outputs=807,input_bits=2114,state_sha256=sha(d/'run-state.json')))
        retain(d,out/d.name)
    d=BUILD/'native-address-task-owner-xz-v1-20261025';r=read(d/'run-state.json');verify(d,r['files'])
    assert r['status']=='TERMINAL_NATIVE_ADDRESS_TASK_OWNER_XZ_NOT_ESTABLISHED'and r['actions'][0]['returncode']!=0
    assert sha(d/'compile.log')==r['actions'][0]['log_sha256']
    assert 'Check for declaration after use'in(d/'compile.log').read_text()and not(d/'test.log').exists()
    retain(d,out/d.name)
    d=BUILD/'native-address-task-owner-xz-v2-20261025';r=checked(d)
    assert r['status']=='PASS_ACTUAL_NATIVE_ADDRESS_TASK_OWNER_XZ'and r['compared_outputs']==807
    assert 'known=3 unknown=12684 checks=12687 outputs=807'in(d/'test.log').read_text()
    retain(d,out/d.name)
    proof=BUILD/'native-address-task-owner-qualification-v2-20261025'
    before=(proof/'native-address-before.sv').read_text();after=(proof/'native-address-after.sv').read_text()
    assert after==transform(before)
    assert 'assign unsettled=core_unsettled|child_unsettled|(child_next[87:0]!=core_next);'in before
    assert 'assign unsettled=core_unsettled|child_unsettled|(child_next[87:0]!=core_next);'in after
    replays=[];pending=[]
    for version,engines in [(1,['verilator']),(2,['icarus','verilator'])]:
        for mode in ['full','words']:
            for engine in engines:
                d=BUILD/f'native-address-task-owner-{mode}-{engine}-v{version}-20261025';s,row=observed(d)
                expected_proof=BUILD/f'native-address-task-owner-qualification-v{version}-20261025'
                assert s['address_task_owner_proof_state_sha256']==sha(expected_proof/'run-state.json')
                assert sha(d/'adapters/dorado_local_r_address_step.sv')==sha(expected_proof/'native-address-after.sv')
                assert s['original_assertions_and_reference_unchanged']and s['native_cells_j19_single_SRAM_and_commit_barriers_retained']
                if s['status']=='RUNNING':
                    assert engine=='icarus'and row['verified_live'];pending.append(row)
                    target=out/'pending-source-observations'/d.name;target.mkdir(parents=True)
                    for source in s['source_files']:
                        f=target/source['path'];f.parent.mkdir(parents=True,exist_ok=True);f.write_bytes((d/source['path']).read_bytes())
                    (target/'run-state-at-observation.json').write_text(json.dumps(s,indent=2)+'\n')
                    (target/'process-observation.json').write_text(json.dumps(row,indent=2)+'\n')
                else:
                    assert s['status']=='PASS_ACTUAL_NATIVE_PHASE_GUARD_PROCESSOR_REPLAY'
                    pattern=r'^(?:PACKET_CLOCK|FULL_PROCESSOR_PACKET_PASS|CP_PACKET_GEOMETRY_REJECTIONS|CP_PACKET_STAGED_READS|CACHED_PACKET_COVERAGE|NATIVE_R_PACKET_COVERAGE) .*$'
                    assert s['outcomes']==re.findall(pattern,(Path(s['source'])/'test.log').read_text(),re.M)
                    retain(d,out/d.name)
                replays.append(row)
    timing=BUILD/'native-task-terminal-full15-timing-review-v1-20261025';t=read(timing/'results.json')
    assert t['status']=='REVIEWED_FULL_CORE_15NS_TIMING_REJECTION'and t['worst_setup_path']['slack_ns']==-13.075
    c=read(timing/'terminal/collection.json');verify(timing/'terminal',c['collected_hashes'])
    assert len(c['collected_hashes'])==327 and len(c['source_readback'])==574
    assert sha(timing/'terminal.tgz')==t['bundle_sha256']and all(s['returncode']==0 for s in t['terminal_state']['stages'])
    retain(timing,out/timing.name)
    stage=BUILD/'native-shared-task-full15-stage-v1-20261025';s=read(stage/'r-front-control-native.json');verify(stage,s['source_hashes'])
    assert len(s['source_hashes'])==574 and s['frequency_hz']==66666667 and s['sysper']==4 and not s['assembly_permitted']
    expected={
        'src/verilog/rtl/dorado_r_front_control_step.sv':BUILD/'native-sparse-fall-frames-proof-v1-20261025/native-control-after.sv',
        'src/verilog/rtl/dorado_local_task_capture_step.sv':BUILD/'native-task-terminal-proof-v1-20261025/native-task-after.sv',
        'src/verilog/rtl/dorado_processor_capture_step.sv':BUILD/'native-processor-terminal-proof-v2-20261025/native-processor-after.sv',
        SOURCE:proof/'native-address-after.sv'}
    assert s['functional_HDL_changes']==sorted(expected)
    parent=BUILD/'native-cp-read-full15-stage-v2-20261025';p=read(parent/'r-front-control-native.json')
    for n in p['source_hashes']:
        raw=expected[n].read_bytes()if n in expected else(parent/n).read_bytes()
        if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:raw=raw.replace(p['project'].encode(),s['project'].encode())
        assert(stage/n).read_bytes()==raw,n
    retain(stage,out/stage.name)
    lint=BUILD/'native-shared-task-full15-lint-v1-20261025';l=read(lint/'run-state.json')
    assert l['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION'and l['structural_warnings_visible']and l['source_files']==196
    assert l['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    warnings=len(re.findall(r'^%Warning-UNOPTFLAT:',(lint/'lint.log').read_text(),re.M));assert warnings==7
    retain(lint,out/lint.name)
    launch=BUILD/'native-shared-task-full15-launch-v1-20261025';la=read(launch/'launch-observation.json')
    assert la['builder_pid']==3571235 and la['verified_source_count']==574 and not la['assembly_permitted']
    query='''from pathlib import Path
import hashlib,json,subprocess,time
p=Path(PROJECT);s=json.loads((p/'run-state.json').read_text());m=json.loads((p/'r-front-control-native.json').read_text())
assert s['builder_pid']==3571235 and not s['assembly_permitted']
actual={n:hashlib.sha256((p/n).read_bytes()).hexdigest()for n in m['source_hashes']};assert actual==m['source_hashes']and len(actual)==574
row=dict(observed=time.time(),run_state=s,source_readback=actual)
if s['status']=='RUNNING':
 a=s['stages'][-1];ps=subprocess.run(['ps','-p',str(a['pid']),'-o','pid=,etime=,time=,stat=,args='],capture_output=True,text=True)
 assert ps.returncode==0 and 'quartus_'in ps.stdout
 row.update(verified_live=True,process=ps.stdout.strip())
else:assert s['status']=='TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
print(json.dumps(row))
'''.replace('Path(PROJECT)','Path('+repr(la['project'])+')',1)
    response=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=query,text=True,capture_output=True)
    assert response.returncode==0,response.stderr;fit=json.loads(response.stdout)
    retain(launch,out/launch.name)
    (out/'shared-task-timing-process-observation.json').write_text(json.dumps(fit,indent=2)+'\n')
    rejected=read(BUILD/'native-shared-task-owner-review-v1-20261025/failure.json')
    assert rejected['status']=='REJECTED_COMPILE_FAILURE_REVIEW_READER'
    (out/'preflight-review-rejection.json').write_text(json.dumps(rejected,indent=2)+'\n')
    result=dict(status='REVIEWED_ACTUAL_NATIVE_SHARED_TASK_OWNER_AND_FULL15_REJECTION',scope=__doc__,
        processor_terminal_review=previous,shared_task_invariant_outputs=139,actual_shared_task_input_bits=2114,
        invariant_v1_reported_input_bits=2113,invariant_metadata_count_corrected_from_actual_interface=True,
        full_interface_proofs=proofs,all807_outputs_and_all_guards_retained=True,
        unknown_checks=12687,unknown_outputs_compared=807,first_Icarus_forward_declaration_rejection_retained=True,
        shared_owner_replays=replays,pending_verified_live_shared_owner_replays=pending,
        pending_verified_live_replay_count=len(pending)+len(previous['pending_verified_live_replays']),
        completed_task_full15=t,shared_owner_stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        shared_owner_timing=fit,native_structural_warnings_before=10,native_structural_warnings_now=warnings,
        nominal_diagnostic_ordinary_ns=120,target_ordinary_ns=60,
        original_media_firmware_and_machine_barriers_retained=True,physical_native_arrival_contract_qualified=False,
        hardware_or_SDRAM_or_60ns_qualified=False,hardware_changed=False,measured_speed_gain=False,full_goal_active=True,
        next_action='Follow SAME shared-task full-core job3571235 and pending processor/shared-owner Icarus peers; inspect actual timing before selection, then close native phase arrivals and four15ns chronology/memory/timers/full Cedar qualification')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-shared-task-owner',extra_tools=TOOLS)
    print(r['status'],flush=True)
