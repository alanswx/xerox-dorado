#!/usr/bin/env python3
"""Retain the complete-prefix Ready comparison and its single native fit.

Every original Ready update and comparison result remains authoritative.
Pending Icarus/Quartus jobs are live observations, never passing results.
No hardware speed, SDRAM protocol,60ns cadence or firmware pass is implied.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess
import time

from qualify_native_ready_compare import CONTROL,READY,HELPER,control_transform,child_transform
from review_baseboard_clock_consumer import verify
from review_cp_paired_read import observed
from review_native_alu_capture import retain
from review_processor_branch_composition import checked
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=[Path(__file__).name,'qualify_native_ready_compare.py','test_native_ready_compare.py',
       'stage_native_ready_compare.py','replay_native_phase_fall_guard.py',
       'start_native_phase_guard_timing.py','lint_r_front_control_native.py','review_native_cp_read_timing.py']


def read(p):return json.loads(p.read_text())


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    proof=BUILD/'native-ready-compare-proof-v1-20261025';p=read(proof/'run-state.json');verify(proof,p['files'])
    assert p['status']=='PASS_ACTUAL_NATIVE_READY_COMPARISON'and p['arbitrary_binary_input_bits']==1982 and p['compared_outputs']==2969
    assert p['assumptions']==0 and p['all_original_prefix_outputs_and_ready_update_retained']
    for a in p['actions']:
        d=proof/a['label'];verify(d,a['files']);assert a['passed']and a['returncode']==0
        assert sha(d/'proof.log')==a['log_sha256']and '-set 'not in(d/'proof.ys').read_text()
    sig={s['name']:s for s in read(proof/'wrong-preempt-selector/counterexample.json')['signal']}
    def value(n):return int(sig[n]['data'][0]if'data'in sig[n]else sig[n]['wave'][0],2)
    different=value('actual')^value('expected');offset=0;changed=[]
    for direction,width,name in p['ports']:
        if direction=='output':
            if different>>offset & ((1<<width)-1):changed.append(name)
            offset+=width
    assert changed==['shared_state_mismatch']
    # The negative establishes the real guard output, not a stored-Q failure.
    retain(proof,out/proof.name)
    assert sha(ROOT/HELPER)==p['helper_sha256']
    dest=out/'current-RTL'/Path(HELPER).name;dest.parent.mkdir();dest.write_bytes((ROOT/HELPER).read_bytes())
    assert(proof/'native-control-after.sv').read_text()==control_transform((proof/'native-control-before.sv').read_text())
    assert(proof/'native-ready-after.sv').read_text()==child_transform((proof/'native-ready-before.sv').read_text())
    xz=BUILD/'native-ready-compare-xz-v1-20261025';x=checked(xz)
    assert x['status']=='PASS_ACTUAL_NATIVE_READY_COMPARE_XZ'and x['proof_state_sha256']==sha(proof/'run-state.json')
    assert 'known=3 unknown=11892 checks=11895 outputs=2969'in(xz/'test.log').read_text();retain(xz,out/xz.name)
    replays=[];pending=[]
    for mode in ['full','words']:
        for engine in ['icarus','verilator']:
            d=BUILD/f'native-ready-compare-{mode}-{engine}-v1-20261025';s,row=observed(d)
            assert s['ready_compare_proof_state_sha256']==sha(proof/'run-state.json')
            assert s['original_assertions_and_reference_unchanged']and s['native_cells_j19_single_SRAM_and_commit_barriers_retained']
            assert sha(d/'candidate.sv')==sha(proof/'native-control-after.sv')
            assert sha(d/'adapters/dorado_local_r_front_ready_step.sv')==sha(proof/'native-ready-after.sv')
            assert sha(d/'adapters/dorado_ready_compare_step.sv')==sha(ROOT/HELPER)
            if s['status']=='RUNNING':
                assert engine=='icarus'and row['verified_live'];pending.append(row)
            else:
                assert s['status']=='PASS_ACTUAL_NATIVE_PHASE_GUARD_PROCESSOR_REPLAY'
                assert len(re.findall(r'^PACKET_CLOCK channel=',(d/'test.log').read_text(),re.M))==100
                expected=re.findall(r'^(?:PACKET_CLOCK|FULL_PROCESSOR_PACKET_PASS|CP_PACKET_GEOMETRY_REJECTIONS|CP_PACKET_STAGED_READS|CACHED_PACKET_COVERAGE|NATIVE_R_PACKET_COVERAGE) .*$',(Path(s['source'])/'test.log').read_text(),re.M)
                assert s['outcomes']==expected
            retain(d,out/d.name);replays.append(row)
    stage=BUILD/'native-ready-compare-full15-stage-v1-20261025';s=read(stage/'r-front-control-native.json');verify(stage,s['source_hashes'])
    parent=Path(s['parent_stage']);old=read(parent/'r-front-control-native.json')
    assert len(s['source_hashes'])==575 and s['frequency_hz']==66666667 and s['sysper']==4 and s['nominal_ordinary_ns']==120
    changed_files={CONTROL:(proof/'native-control-after.sv').read_bytes(),READY:(proof/'native-ready-after.sv').read_bytes(),'src/'+HELPER:(ROOT/HELPER).read_bytes()}
    assert s['incremental_functional_HDL_changes']==sorted(changed_files)
    assert s['shared_state_mismatch_guard_retained']and s['all2969_prefix_outputs_retained']and not s['assembly_permitted']
    for n in s['source_hashes']:
        raw=changed_files[n]if n in changed_files else(parent/n).read_bytes()
        if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:raw=raw.replace(old['project'].encode(),s['project'].encode())
        if n=='core/Dorado.qsf':raw+=('\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(Path(s['project'])/('src/'+HELPER))+'\n').encode()
        assert(stage/n).read_bytes()==raw,n
    retain(stage,out/stage.name)
    lint=BUILD/'native-ready-compare-full15-lint-v1-20261025';l=read(lint/'run-state.json')
    assert l['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION'and l['source_files']==197 and l['structural_warnings_visible']
    assert l['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    warnings=len(re.findall(r'^%Warning-UNOPTFLAT:',(lint/'lint.log').read_text(),re.M));assert warnings==10
    retain(lint,out/lint.name)
    launch=BUILD/'native-ready-compare-full15-launch-v1-20261025';la=read(launch/'launch-observation.json')
    assert la['builder_pid']==4042975 and la['verified_source_count']==575 and la['manifest_sha256']==sha(stage/'r-front-control-native.json')
    query='''from pathlib import Path
import hashlib,json,subprocess,time
p=Path(PROJECT);r=json.loads((p/'run-state.json').read_text());m=json.loads((p/'r-front-control-native.json').read_text())
assert r['builder_pid']==4042975 and not r['assembly_permitted']
actual={n:hashlib.sha256((p/n).read_bytes()).hexdigest()for n in m['source_hashes']};assert actual==m['source_hashes']and len(actual)==575
assert not list((p/'core/output_files').glob('*.rbf'))and not list((p/'core/output_files').glob('*.sof'))
row=dict(observed=time.time(),state=r,source_readback=actual)
if r['status']=='RUNNING':
 a=r['stages'][-1];s=subprocess.run(['ps','-p',str(a['pid']),'-o','pid=,etime=,time=,stat=,args='],text=True,capture_output=True)
 assert s.returncode==0 and 'quartus_'in s.stdout;row.update(verified_live=True,process=s.stdout.strip())
else:assert r['status']=='TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
print(json.dumps(row))
'''.replace('Path(PROJECT)','Path('+repr(s['project'])+')',1)
    response=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=query,text=True,capture_output=True)
    assert response.returncode==0,response.stderr;fit=json.loads(response.stdout)
    (out/'same-ready-comparison-fit-observation.json').write_text(json.dumps(fit,indent=2)+'\n');retain(launch,out/launch.name)
    previous=BUILD/'native-task-parallel-full15-timing-review-v1-20261025';t=read(previous/'results.json')
    assert t['worst_setup_path']['slack_ns']==-12.971 and t['status']=='REVIEWED_FULL_CORE_15NS_TIMING_REJECTION'
    c=read(previous/'terminal/collection.json');verify(previous/'terminal',c['collected_hashes']);assert len(c['source_readback'])==574
    assert sha(previous/'terminal.tgz')==t['bundle_sha256'];retain(previous,out/previous.name)
    native=BUILD/'native-cp-read-j19-boot-v1-20261025';n=read(native/'run-state.json')
    assert int((native/'host-cycle-limit.txt').read_text())==5000000000
    native_observation=dict(state=n,observed=time.time())
    if n['status']=='RUNNING':
        assert n['actions'][-1]['pid']==62048
        ps=subprocess.run(['ps','-p','62048','-o','pid=,etime=,time=,stat=,command='],text=True,capture_output=True)
        assert ps.returncode==0 and str(native/'obj/native-cedar')in ps.stdout
        native_observation.update(verified_live=True,process=ps.stdout.strip())
    medium=BUILD/'cedar-work-native-hints.pack';assert medium.stat().st_size==63578624
    assert sha(medium)=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    result=dict(status='REVIEWED_COMPLETE_PREFIX_PARALLEL_READY_COMPARISON',scope=__doc__,
        complete_prefix_input_bits=1982,all_compared_outputs=2969,assumptions=0,unknown_checks=11895,
        negative_preempt_witness_changed_outputs=changed,negative_witness_is_guard_output_not_stored_state=True,
        original_Ready_physical_update_and_all_guards_retained=True,processor_replays=replays,pending_verified_live_Icarus=pending,
        native_input_count=575,incremental_HDL_files=sorted(changed_files),native_structural_warnings=warnings,
        previous_rejected_task_fit=t,current_single_ready_compare_fit=fit,original_corrected_read_boot=native_observation,
        nominal_diagnostic_ordinary_ns=120,target_ordinary_ns=60,
        hardware_or_media_changed=False,measured_speed_gain=False,full_goal_active=True,
        next_action='Follow SAME ReadyCompare job4042975 and Icarus peers17225/17226; collect all physical paths/corners before selection. Use explicit R-read/control capture phases to remove remaining serial dependencies and half-cycle gate; finish actual60ns chronology, SDRAM/timers and original Cedar/Rollback qualification')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-ready-compare',extra_tools=TOOLS)
    print(r['status'],flush=True)
