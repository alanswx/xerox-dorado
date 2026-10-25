#!/usr/bin/env python3
"""Retain native common flag ownership and the completed parallel-task fit.

Preserve all rejected readers/frontends, the missing-PROM coverage gap and
the physical wrong-clock witness. Native enabled-fabric equivalence does not
qualify the original60ns chronology, software boot or FPGA timing.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import subprocess
import time

from review_baseboard_clock_consumer import verify
from review_native_alu_capture import retain
from review_processor_branch_composition import checked
from review_r_front_capture_followup import append_archive
from stage_native_branch_owner import BOARD,FILES,transform
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=[Path(__file__).name,'qualify_native_branch_owner.py','test_native_branch_owner.py',
       'stage_native_branch_owner.py','replay_native_branch_board.py','probe_native_shared_ready.py',
       'review_native_cp_read_timing.py']


def read(p):return json.loads(p.read_text())


def binary(signals,name,width,last=False):
    seq=signals[name]['data'];seq=reversed(seq)if last else seq
    return int(next(x for x in seq if len(x)==width and set(x)<={'0','1'}),2)


def terminal_native(out):
    assert not out.exists();out.mkdir(parents=True);runs=[]
    for name,pid,bound,reason in [
        ('control-short-native-jumper-boot-v2-20261024',69736,5000000000,'QUEUE_SEARCH'),
        ('native-alu-source-boot-prefix-v1-20261025',87487,600000000,'HOST_LIMIT')]:
        d=BUILD/name;r=read(d/'run-state.json');p=json.loads(gzip.decompress((d/'provenance.json.gz').read_bytes()))
        verify(d/'inputs',p['inputs'])
        assert r['status']=='TERMINAL_NATIVE_MEMORY_DIAGNOSTIC'and r['reason']==reason
        assert all(a['returncode']==0 for a in r['actions'])and r['actions'][-1]['pid']==pid
        assert int((d/'host-cycle-limit.txt').read_text())==bound
        log=(d/'test.log').read_text();stop=re.findall(r'^NATIVE_DIAGNOSTIC_STOP .*$',log,re.M);assert len(stop)==1
        fields=dict(re.findall(r'(\w+)=\s*(\w+)',stop[0]));assert fields['reason']==reason
        assert int(fields['host'])==bound if reason=='HOST_LIMIT'else int(fields['host'])<bound
        assert 'native device fault'not in log and 'independent SDRAM model diagnostic'not in log
        if reason=='QUEUE_SEARCH':
            harness=(d/'inputs/harness/tb_native_cedar.sv').read_text()
            assert 'if(queue_visits==128 || host_ticks>=limit)'in harness
            assert len(re.findall(r'^NATIVE_PIN_RING ',log,re.M))==4096
        else:assert len(re.findall(r'^NATIVE_ALU_PHASE_RECORD ',log,re.M))==128
        progress=re.findall(r'^NATIVE_PROGRESS .*$',log,re.M);assert progress
        retain(d,out/d.name)
        runs.append(dict(run=name,pid=pid,status=r['status'],reason=reason,original_bound=bound,
            stop=stop[0],last_progress=progress[-1],log_sha256=sha(d/'test.log'),
            state_sha256=sha(d/'run-state.json'),provenance_sha256=sha(d/'provenance.json.gz'),
            original_source_files_verified=len(p['inputs']),firmware_or_desktop_pass=False))
    medium=BUILD/'cedar-work-native-hints.pack'
    assert sha(medium)=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    result=dict(status='RETAINED_ORIGINAL_NATIVE_DIAGNOSTIC_TERMINALS',runs=runs,
        original_processes_sources_inputs_or_bounds_changed=False,hardware_or_media_changed=False,
        no_original_firmware_or_desktop_or_speed_qualification=True,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


def review(out):
    assert not out.exists();out.mkdir(parents=True);proofs=[]
    for version in range(1,5):
        d=BUILD/f'native-branch-owner-proof-v{version}-20261025';r=read(d/'run-state.json');verify(d,r['files'])
        expected='TERMINAL_NATIVE_BRANCH_OWNER_NOT_ESTABLISHED'if version==1 else'PASS_ACTUAL_NATIVE_BRANCH_COMMON_OWNER'
        assert r['status']==expected
        for a in r['actions']:
            p=d/a['label'];verify(p,a['files']);assert sha(p/'proof.log')==a['log_sha256']and '-set 'not in(p/'proof.ys').read_text()
            if version==1:assert a['returncode']!=0 and 'ERROR:'in(p/'proof.log').read_text()
            else:assert a['passed']and a['returncode']==0
        assert r['compared_outputs']==175 and r['assumptions']==0
        if version==4:
            assert r['arbitrary_binary_inputs']==190 and r['previous_file_response_independent_of_all104_storage_bits']
            negative=[]
            for label in ['wrong-g11-epoch','wrong-admin-row']:
                sig={s['name']:s for s in read(d/label/'counterexample.json')['signal']}
                diff=(binary(sig,'actual',175,True)^binary(sig,'expected',175,True))>>71
                bits=[k for k in range(80)if diff>>k&1];assert bits
                negative.append(dict(label=label,actual_physical_flag_bits=bits))
        proofs.append(dict(directory=d.name,status=r['status'],input_bits=r['arbitrary_binary_inputs'],state_sha256=sha(d/'run-state.json')))
        retain(d,out/d.name)
    good_proof=BUILD/'native-branch-owner-proof-v4-20261025'
    for n in FILES:
        assert sha(ROOT/n)==sha(good_proof/Path(n).name)
        target=out/'current-RTL'/Path(n).name;target.parent.mkdir(exist_ok=True);target.write_bytes((ROOT/n).read_bytes())
    interfaces=[]
    for engine in ['icarus','verilator']:
        for version in range(1,5):
            d=BUILD/f'native-branch-owner-{engine}-v{version}-20261025'
            if version==1:
                r=read(d/'failure.json');assert r['status']=='REJECTED_SEQUENTIAL_WITNESS_READER'
            else:
                r=read(d/'run-state.json');verify(d,r['files'])
                for a in r['actions']:assert sha(d/(a['label']+'.log'))==a['log_sha256']
                if version==2:
                    assert r['actions'][0]['returncode']!=0 and not(d/'test.log').exists()
                    log=(d/'compile.log').read_text()
                    assert ('top.sv:5: syntax error'in log)if engine=='icarus'else('unexpected before'in log)
                else:
                    assert r['status']=='PASS_ACTUAL_NATIVE_BRANCH_OWNER_INTERFACE'and all(a['returncode']==0 for a in r['actions'])
                    if version==4:
                        assert r['proof_state_sha256']==sha(good_proof/'run-state.json')
                        summary='binary=1024 unknown=1140 checks=2167 outputs=175'if engine=='icarus'else'binary=1024 unknown=0 checks=1024 outputs=175'
                        assert summary in(d/'test.log').read_text();interfaces.append(dict(engine=engine,summary=summary))
            retain(d,out/d.name)
    stages=[]
    for version in [1,2]:
        d=BUILD/f'native-branch-owner-stage-v{version}-20261025';r=read(d/'r-front-control-native.json');verify(d,r['source_hashes'])
        parent=Path(r['parent_stage']);p=read(parent/'r-front-control-native.json')
        after,binding=transform((parent/BOARD).read_text())
        assert after==(d/BOARD).read_text()and json.loads(json.dumps(binding))==r['actual_native_flag_owner_binding']
        assert len(r['source_hashes'])==581 and r['new_flag_owner_selected']and not r['assembly_permitted']
        for n in p['source_hashes']:
            raw=after.encode()if n==BOARD else(parent/n).read_bytes()
            if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:raw=raw.replace(p['project'].encode(),r['project'].encode())
            if n=='core/Dorado.qsf':
                for extra in FILES:raw+=('\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(Path(r['project'])/('src/'+extra))+'\n').encode()
            assert(d/n).read_bytes()==raw,n
        for n in FILES:assert sha(d/('src/'+n))==sha(ROOT/n)
        if version==2:
            assert r['native_branch_owner_proof_state_sha256']==sha(good_proof/'run-state.json')
            first=BUILD/'native-branch-owner-stage-v1-20261025';a=read(first/'r-front-control-native.json')
            for n in r['source_hashes']:
                raw=(first/n).read_bytes()
                if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:raw=raw.replace(a['project'].encode(),r['project'].encode())
                assert(d/n).read_bytes()==raw,n
        retain(d,out/d.name);stages.append(dict(directory=d.name,source_count=581,manifest_sha256=sha(d/'r-front-control-native.json')))
    d=BUILD/'native-branch-owner-lint-v1-20261025';r=read(d/'run-state.json')
    assert r['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION'and r['source_files']==203
    assert r['stage_manifest_sha256']==stages[0]['manifest_sha256']
    warnings=len(re.findall(r'^%Warning-UNOPTFLAT:',(d/'lint.log').read_text(),re.M));assert warnings==9
    retain(d,out/d.name)
    for version in [1,2,3]:
        d=BUILD/f'native-branch-board-verilator-v{version}-20261025'
        if version==1:assert read(d/'failure.json')['status']=='REJECTED_NATIVE_BINDING_READER'
        else:
            r=checked(d);assert r['status']=='PASS_ACTUAL_NATIVE_BRANCH_BOARD_REPLAY'
            assert r['input_bits']==1059 and r['output_bits']==929
            if version==2:assert '$readmem file not found'in(d/'test.log').read_text()
            else:
                assert '$readmem file not found'not in(d/'test.log').read_text()
                assert r['summary']=='NATIVE_BRANCH_BOARD_PASS inputs=1059 outputs=929 flags=104 checks=8215 guest_checks=8192 file=31 f17=251 g11=122 h11=127'
                board_summary=r['summary']
                stage=BUILD/'native-branch-owner-stage-v2-20261025';m=read(stage/'r-front-control-native.json')
                for n in m['source_hashes']:
                    if n.startswith('src/'):assert sha(d/n)==sha(stage/n),n
        retain(d,out/d.name)
    d=BUILD/'native-branch-board-wrong-clock-verilator-v1-20261025';r=checked(d)
    assert r['status']=='PASS_ACTUAL_NATIVE_BRANCH_BOARD_CLOCK_COUNTEREXAMPLE'and r['wrong_clock_buffer_bypass']
    assert 'BRANCH_BOARD_WRONG_CLOCK_PHYSICAL_Q check=87'in(d/'test.log').read_text()
    retain(d,out/d.name)
    probes=[]
    for name in ['native-shared-ready-probe-v1-20261025','native-shared-ready-computed-probe-v1-20261025']:
        d=BUILD/name;r=read(d/'run-state.json');verify(d,r['files'])
        assert r['status']=='COUNTEREXAMPLE_TO_NATIVE_SHARED_READY_REDUNDANCY'and r['returncode']==0
        assert sha(d/'proof.log')==r['log_sha256']and 'model found: FAIL!'in(d/'proof.log').read_text()
        sig={s['name']:s for s in read(d/'counterexample.json')['signal']}
        def v(n):
            s=sig[n];return int(s['data'][0]if'data'in s else s['wave'][0],2)
        assert v('shared_context')==v('mismatch')==1
        diff=v('child_control_projection')^v('joint_control_projection')
        bits=[k for k in range(64)if diff>>k&1];assert diff & 0xffffffffffff3fff
        probes.append(dict(directory=name,computed_prefix=r.get('cache_payload_replaced_by_actual_prefix_function',False),
            arbitrary_input_bits=r['arbitrary_binary_input_bits'],differing_control_projection_bits=bits,
            existing_comparison_must_be_retained=True));retain(d,out/d.name)
    timing=BUILD/'native-task-parallel-full15-timing-review-v1-20261025';t=read(timing/'results.json')
    c=read(timing/'terminal/collection.json');verify(timing/'terminal',c['collected_hashes'])
    assert len(c['collected_hashes'])==327 and len(c['source_readback'])==574
    assert t['worst_setup_path']['slack_ns']==-12.971 and t['status']=='REVIEWED_FULL_CORE_15NS_TIMING_REJECTION'
    assert sha(timing/'terminal.tgz')==t['bundle_sha256']and all(s['returncode']==0 for s in t['terminal_state']['stages'])
    retain(timing,out/timing.name)
    task_replays=[]
    for mode in ['full','words']:
        for engine in ['icarus','verilator']:
            d=BUILD/f'native-task-parallel-{mode}-{engine}-v1-20261025';r=checked(d)
            assert r['status']=='PASS_ACTUAL_NATIVE_PHASE_GUARD_PROCESSOR_REPLAY'
            assert r['original_assertions_and_reference_unchanged']and r['native_cells_j19_single_SRAM_and_commit_barriers_retained']
            assert len(re.findall(r'^PACKET_CLOCK channel=',(d/'test.log').read_text(),re.M))==100
            retain(d,out/d.name);task_replays.append(dict(directory=d.name,status=r['status'],state_sha256=sha(d/'run-state.json')))
    native=[]
    for name,pid,bound in [('native-cp-read-j19-boot-v1-20261025',62048,5000000000),
        ('control-short-native-jumper-boot-v2-20261024',69736,5000000000),('native-alu-source-boot-prefix-v1-20261025',87487,600000000)]:
        d=BUILD/name;r=read(d/'run-state.json');assert int((d/'host-cycle-limit.txt').read_text())==bound
        row=dict(run=name,pid=pid,bound=bound,state=r,observed=time.time())
        if r['status']=='RUNNING':
            assert r['actions'][-1]['pid']==pid
            ps=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,time=,stat=,command='],capture_output=True,text=True)
            assert ps.returncode==0 and str(d/'obj/native-cedar')in ps.stdout
            row.update(verified_live=True,process=ps.stdout.strip())
        native.append(row)
    medium=BUILD/'cedar-work-native-hints.pack'
    assert medium.stat().st_size==63578624 and sha(medium)=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    for version in [1,2]:
        rejected=BUILD/f'native-branch-owner-review-v{version}-20261025'
        if rejected!=out and(rejected/'failure.json').exists():
            dest=out/'rejected-review-readers'/rejected.name;dest.mkdir(parents=True)
            for n in ['failure.json','review_native_branch_owner.py']:(dest/n).write_bytes((rejected/n).read_bytes())
    result=dict(status='REVIEWED_NATIVE_BRANCH_COMMON_OWNER_AND_PARALLEL_TASK_TIMING',scope=__doc__,
        native_owner_proofs=proofs,independent_previous_file_response_qualified=True,negative_physical_Q_witnesses=negative,
        owner_interfaces=interfaces,native_stages=stages,board_replay=board_summary,
        missing_PROM_v2_run_not_a_complete_board_qualification=True,actual_clock_buffer_bypass_corrupts_physical_Q=True,
        native_structural_warnings=warnings,shared_ready_redundancy_probes=probes,all_existing_native_checks_retained=True,
        completed_task_parallel_timing=t,all_task_parallel_full_words_both_engines_passed=task_replays,
        native_simulations=native,nominal_diagnostic_ordinary_ns=120,target_ordinary_ns=60,
        hardware_or_protected_media_changed=False,measured_speed_gain=False,full_goal_active=True,
        next_action='Use the measured RM -> branch -> control -> Ready mismatch -> half-cycle enable path to integrate real common Ready/operand phases, retaining the guard; merge exact native flag ownership into that chronology; qualify four15ns phases, SDRAM/timers and full original Cedar/Rollback requirements')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    p.add_argument('--terminal-native',action='store_true')
    a=p.parse_args();r=terminal_native(a.output.resolve())if a.terminal_native else review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-branch-owner-native-terminal'if a.terminal_native else'native-branch-owner',extra_tools=TOOLS)
    print(r['status'],flush=True)
