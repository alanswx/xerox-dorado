#!/usr/bin/env python3
"""Retain corrected native Ready targeting and the registered read integration.

Formal, replay and elaboration results retain their actual scopes. Observe
the same pending simulators and source-bound timing jobs; never restart them.
No new60ns schedule, hardware speed gain or Cedar boot is implied.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from integrate_r_read_buffer import caller,memory
from qualify_native_ready_compare import control_transform,child_transform
from review_baseboard_clock_consumer import verify
from review_cp_paired_read import observed
from review_native_alu_capture import retain
from review_processor_branch_composition import checked
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=[Path(__file__).name,'qualify_native_ready_compare.py','test_native_ready_compare.py',
 'stage_native_ready_compare.py','replay_native_phase_fall_guard.py','review_native_cp_read_timing.py',
 'prove_r_read_buffer.py','integrate_r_read_buffer.py','replay_r_read_buffer.py',
 'stage_native_r_read_buffer.py','start_native_phase_guard_timing.py','lint_r_front_control_native.py']


def read(p):return json.loads(p.read_text())


def proof_check(d,status):
    p=read(d/'run-state.json');verify(d,p['files']);assert p['status']==status and p['assumptions']==0
    for a in p['actions']:
        q=d/a['label'];verify(q,a['files'])
        assert a['passed']and a['returncode']==0 and sha(q/'proof.log')==a['log_sha256']
    return p


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    retained=[]
    def keep(d):
        assert d.is_dir(),d
        retain(d,out/d.name);retained.append(d.name)
    ready=BUILD/'native-ready-compare-proof-v2-20261025'
    p=proof_check(ready,'PASS_ACTUAL_NATIVE_READY_COMPARISON')
    assert p['actual_native_single_path_qualified']and p['arbitrary_binary_input_bits']==1982 and p['compared_outputs']==2969
    assert {a['label']for a in p['actions']}=={'single-complete-interface','prefix-complete-interface','wrong-preempt-selector'}
    assert(ready/'native-control-after.sv').read_text()==control_transform((ready/'native-control-before.sv').read_text(),True)
    assert(ready/'native-ready-after.sv').read_text()==child_transform((ready/'native-ready-before.sv').read_text())
    keep(ready)
    for family in ['single','prefix']:
        d=BUILD/f'native-ready-compare-{family}-xz-v2-20261025';s=checked(d)
        assert s['status']=='PASS_ACTUAL_NATIVE_READY_COMPARE_XZ'and s['family']==family and s['proof_state_sha256']==sha(ready/'run-state.json')
        assert 'known=3 unknown=11892 checks=11895 outputs=2969'in(d/'test.log').read_text();keep(d)
    buffer=BUILD/'r-read-buffer-proof-v3-20261025';b=proof_check(buffer,'PASS_REGISTERED_R_READ_FRONTIER')
    assert b['initial_RAM_bits_unconstrained']==9216 and b['original_RAM_owner_update_unchanged']
    assert {a['label']for a in b['actions']}=={'contracts','stale-write','wrong-parity','ignore-address-tag','drop-STK-high'}
    assert b['buffer_sha256']==sha(ROOT/'verilog/rtl/dorado_r_read_buffer.sv');keep(buffer)
    replays=[];pending=[]
    for prefix,version in [('native-ready-compare','v2'),('r-read-buffer','v2')]:
        for mode in ['full','words']:
            for engine in ['verilator','icarus']:
                d=BUILD/f'{prefix}-{mode}-{engine}-{version}-20261025';s,row=observed(d)
                assert s['original_assertions_and_reference_unchanged']and s['native_cells_j19_single_SRAM_and_commit_barriers_retained']
                assert s['ready_compare_proof_state_sha256']==sha(ready/'run-state.json')if prefix=='native-ready-compare'else s['buffer_proof_state_sha256']==sha(buffer/'run-state.json')
                if s['status']=='RUNNING':
                    assert engine=='icarus'and row['verified_live'];pending.append(row)
                else:
                    expected='PASS_ACTUAL_NATIVE_PHASE_GUARD_PROCESSOR_REPLAY'if prefix=='native-ready-compare'else'PASS_REGISTERED_READ_COMPLETE_NATIVE_CALLER'
                    assert s['status']==expected
                    assert len(re.findall(r'^PACKET_CLOCK channel=',(d/'test.log').read_text(),re.M))==100
                    if prefix=='r-read-buffer':assert s['read_buffer_preparation_edges']>0
                keep(d);replays.append(row)
    # Preserve rejected frontend/transport/source-binding attempts verbatim.
    for name in ['native-ready-compare-binding-rejection-v1-20261025','native-ready-compare-full15-lint-v2-20261025',
                 'native-ready-compare-full15-launch-v2-20261025',
                 'r-read-buffer-proof-v1-20261025','r-read-buffer-proof-v2-20261025',
                 'r-read-buffer-full-icarus-v1-20261025','r-read-buffer-words-icarus-v1-20261025',
                 'r-read-buffer-full-verilator-v1-20261025','r-read-buffer-words-verilator-v1-20261025']:
        keep(BUILD/name)
    previous=BUILD/'native-ready-compare-full15-timing-review-v1-20261025';v=read(previous/'results.json')
    assert v['status']=='REVIEWED_FULL_CORE_15NS_TIMING_REJECTION'and not v['actual_native_single_comparison_connected']
    assert v['ready_v1_targeting_limitation']and v['worst_setup_path']['slack_ns']==-12.971
    verify(previous/'terminal',read(previous/'terminal/collection.json')['collected_hashes'])
    assert v['bundle_sha256']==sha(previous/'terminal.tgz');keep(previous)
    for mode in ['full','words']:
        d=BUILD/f'native-ready-compare-{mode}-icarus-v1-20261025';s,row=observed(d)
        assert s['status']=='PASS_ACTUAL_NATIVE_PHASE_GUARD_PROCESSOR_REPLAY';keep(d)
    builds=[]
    for stage_name,lint_name,launch_name,count,warnings,builder in [
        ('native-ready-compare-full15-stage-v2-20261025','native-ready-compare-full15-lint-v3-20261025','native-ready-compare-full15-launch-v3-20261025',575,10,4184852),
        ('native-r-read-buffer-full15-stage-v1-20261025','native-r-read-buffer-full15-lint-v1-20261025','native-r-read-buffer-full15-launch-v1-20261025',576,6,11306)]:
        d=BUILD/stage_name;s=read(d/'r-front-control-native.json');verify(d,s['source_hashes'])
        assert len(s['source_hashes'])==count and s['actual_native_single_comparison_connected']and not s['assembly_permitted']
        assert s['frequency_hz']==66666667 and s['sysper']==4 and s['nominal_ordinary_ns']==120
        parent=Path(s['parent_stage']);pm=read(parent/'r-front-control-native.json')
        if count==576:
            expected={'src/verilog/rtl/caller.sv':caller((parent/'src/verilog/rtl/caller.sv').read_text()).encode(),
                      'src/verilog/rtl/dorado_r_file_ports.sv':memory((parent/'src/verilog/rtl/dorado_r_file_ports.sv').read_text()).encode(),
                      'src/verilog/rtl/dorado_r_read_buffer.sv':(ROOT/'verilog/rtl/dorado_r_read_buffer.sv').read_bytes()}
            added='src/verilog/rtl/dorado_r_read_buffer.sv'
        else:
            expected={'src/verilog/rtl/dorado_r_front_control_step.sv':(ready/'native-control-after.sv').read_bytes(),
                      'src/verilog/rtl/dorado_local_r_front_ready_step.sv':(ready/'native-ready-after.sv').read_bytes(),
                      'src/verilog/rtl/dorado_ready_compare_step.sv':(ROOT/'verilog/rtl/dorado_ready_compare_step.sv').read_bytes()}
            added='src/verilog/rtl/dorado_ready_compare_step.sv'
        assert sorted(expected)==s['incremental_functional_HDL_changes']
        for n in s['source_hashes']:
            raw=expected[n]if n in expected else(parent/n).read_bytes()
            if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:raw=raw.replace(pm['project'].encode(),s['project'].encode())
            if n=='core/Dorado.qsf':raw+=('\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(Path(s['project'])/added)+'\n').encode()
            assert(d/n).read_bytes()==raw,n
        l=read(BUILD/lint_name/'run-state.json')
        assert l['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION'and l['structural_warnings_visible']and l['source_files']==count-378
        assert l['stage_manifest_sha256']==sha(d/'r-front-control-native.json')
        assert len(re.findall(r'^%Warning-UNOPTFLAT:',(BUILD/lint_name/'lint.log').read_text(),re.M))==warnings
        la=read(BUILD/launch_name/'launch-observation.json')
        assert la['builder_pid']==builder and la['manifest_sha256']==sha(d/'r-front-control-native.json')and la['verified_source_count']==count
        query='''from pathlib import Path
import hashlib,json,subprocess,time
p=Path(PROJECT);s=json.loads((p/'run-state.json').read_text());m=json.loads((p/'r-front-control-native.json').read_text())
assert s['builder_pid']==BUILDER and not s['assembly_permitted']
actual={n:hashlib.sha256((p/n).read_bytes()).hexdigest()for n in m['source_hashes']}
assert actual==m['source_hashes']and len(actual)==COUNT
assert not list((p/'core/output_files').glob('*.sof'))and not list((p/'core/output_files').glob('*.rbf'))
row=dict(observed=time.time(),state=s,source_readback=actual)
if s['status']=='RUNNING':
 a=s['stages'][-1];q=subprocess.run(['ps','-p',str(a['pid']),'-o','pid=,etime=,time=,stat=,args='],capture_output=True,text=True)
 assert q.returncode==0 and 'quartus_'in q.stdout
 row.update(verified_live=True,process=q.stdout.strip())
else:assert s['status']=='TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
print(json.dumps(row))
'''.replace('Path(PROJECT)','Path('+repr(s['project'])+')').replace('BUILDER',str(builder)).replace('COUNT',str(count))
        response=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=query,text=True,capture_output=True)
        (out/(stage_name+'-query.py')).write_text(query)
        (out/(stage_name+'-query.stderr')).write_text(response.stderr)
        assert response.returncode==0,response.stderr
        remote=json.loads(response.stdout);assert remote['source_readback']==s['source_hashes']
        (out/(stage_name+'-observation.json')).write_text(json.dumps(remote,indent=2)+'\n')
        builds.append(dict(stage=stage_name,builder_pid=builder,source_count=count,structural_warnings=warnings,
                           observation=stage_name+'-observation.json',state=remote['state'],verified_live=remote.get('verified_live',False)))
        keep(d);keep(BUILD/lint_name);keep(BUILD/launch_name)
    native=BUILD/'native-cp-read-j19-boot-v1-20261025';n=read(native/'run-state.json')
    assert int((native/'host-cycle-limit.txt').read_text())==5000000000
    native_observation=dict(observed=time.time(),state=n)
    if n['status']=='RUNNING':
        assert n['actions'][-1]['pid']==62048
        q=subprocess.run(['ps','-p','62048','-o','pid=,etime=,time=,stat=,command='],capture_output=True,text=True)
        assert q.returncode==0 and str(native/'obj/native-cedar')in q.stdout
        native_observation.update(verified_live=True,process=q.stdout.strip())
    medium=BUILD/'cedar-work-native-hints.pack'
    assert medium.stat().st_size==63578624 and sha(medium)=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    for name in ['dorado_r_read_buffer.sv','dorado_ready_compare_step.sv']:
        dest=out/'current-RTL'/name;dest.parent.mkdir(exist_ok=True);shutil.copy2(ROOT/'verilog/rtl'/name,dest)
    result=dict(status='REVIEWED_ACTUAL_NATIVE_READY_AND_REGISTERED_R_READ',scope=__doc__,
        ready_input_bits=1982,ready_compared_outputs=2969,ready_both_actual_single_and_paired_prefix_proved=True,
        ready_four_state_checks_per_family=11895,registered_read_unconstrained_RAM_bits=9216,assumptions=0,
        registered_read_positive_induction_and_four_wrong_data_negatives_pass=True,
        replays=replays,pending_verified_live_Icarus=pending,builds=builds,
        previous_ready_v1_fit_limitation=v['ready_v1_targeting_limitation'],previous_ready_v1_worst_slack_ns=-12.971,
        original_corrected_read_boot=native_observation,retained_directories=retained,
        nominal_diagnostic_ordinary_ns=120,target_ordinary_ns=60,original60ns_phase_schedule_qualified=False,
        hardware_or_media_changed=False,measured_hardware_speed_gain=False,full_goal_active=True,
        next_action='Observe same timing jobs and Icarus peers, collect all corners once terminal. Use the registered read boundary in the original t0/t1/t3 chronology, including flag/Ready/Hold producers; preserve all machine/storage barriers and finish60ns/SDRAM/timer/firmware qualification.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-registered-read',extra_tools=TOOLS)
    print(r['status'],flush=True)
