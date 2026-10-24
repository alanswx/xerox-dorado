#!/usr/bin/env python3
"""Retain source-bound shared native gate, failures, lint and live observation.

This verifies wiring/source identity, not completed startup, physical timing,
feedback safety, faster clocks or software speed. A running prefix simulation
cannot satisfy the complete original-ROM/Cedar/pause/performance acceptance.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil

from review_late_hold_and_input_reset import retain
from review_r_front_capture_followup import append_archive
from review_r_front_cp_subevents import observation
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    stage=BUILD/'cp-prefix-native-stage-v2-20261024'
    s=json.loads((stage/'r-front-control-native.json').read_text())
    assert s['global_control_ready_gate_wired']and s['storage_ready_kept_separate']
    assert s['frequency_hz']==57142857 and s['sysper']==4 and not s['assembly_permitted']
    for name,h in s['source_hashes'].items():assert sha(stage/name)==h,name
    core=(stage/'core/Dorado.sv').read_text()
    for line in ['wire machine_enable = ~reset && pack_ready && storage_ready && control_ready;',
                 'always @(negedge clk_sys) network_step <= machine_enable;',
                 '.ena(machine_enable)', '.contb_advance(network_step),.control_ready(control_ready)']:
        assert core.count(line)==1,line
    bp=(stage/'src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v').read_text()
    assert len(re.findall(r'\.state_guest_advance\(contb_advance\)',bp))==10
    assert len(re.findall(r'\.storage_step\(contb_advance\)',bp))==8
    assert bp.count('.cp_reset(storage_reset),.cp_ready(control_ready)')==1
    assert bp.count('.host_clk(storage_clk),.advance(contb_advance),.held_q(r_front_held)')==1
    machine=(stage/'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv').read_text()
    assert 'assign storage_ready=storage_initialized && board_ready;'in machine
    assert '.storage_reset(reset || !storage_initialized)'in machine
    retain(stage,out/stage.name)
    failure=BUILD/'cp-prefix-native-preparation-failure-v1-20261024'
    assert json.loads((failure/'failure.json').read_text())['status']=='PREPARATION_FAILED_BEFORE_SOURCE_COPY'
    retain(failure,out/failure.name)
    lint=[]
    for n in [1,2]:
        d=BUILD/f'cp-prefix-native-lint-v{n}-20261024';r=json.loads((d/'run-state.json').read_text())
        text=(d/'lint.log').read_text()
        if n==1:
            assert r['returncode']!=0 and "Can't find definition of 'u_j02'"in text
        else:
            assert r['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION'and r['returncode']==0
            assert r['structural_warnings_visible']
            assert len(re.findall(r'^%Warning-UNOPTFLAT:',text,re.M))==3
            assert len(re.findall(r'^%Warning-MULTIDRIVEN:',text,re.M))==1
        lint.append(dict(directory=d.name,status=r['status'],state_sha256=sha(d/'run-state.json')))
        retain(d,out/d.name)
    first=BUILD/'cp-prefix-native-cold-v1-20261024';retain(first,out/first.name)
    old=BUILD/'cp-prefix-native-cold-v2-20261024';new=BUILD/'cp-prefix-native-cold-v3-20261024'
    stop=json.loads((old/'stop-reason.json').read_text());r=json.loads((old/'run-state.json').read_text())
    assert stop['status']=='INTENTIONAL_STOP_FOR_MONITOR_SCOPE_ERROR'and stop['pid']==36430
    assert r['status']=='TERMINAL_FAILED'and r['actions'][-1]['returncode']==-15
    assert int((old/'host-cycle-limit.txt').read_text())==int((new/'host-cycle-limit.txt').read_text())==300000000
    oldp=json.loads(gzip.decompress((old/'provenance.json.gz').read_bytes()))
    newp=json.loads(gzip.decompress((new/'provenance.json.gz').read_bytes()))
    for field in ['used_hdl','defines','artifacts','stage_manifest_sha256']:
        assert oldp[field]==newp[field],field
    changed=[n for n,h in newp['inputs'].items()if h!=oldp['inputs'][n]]
    assert set(changed)=={'harness/tb_native_cedar.sv','prepare_cp_native_pause.py'}
    oldtb=(old/'inputs/harness/tb_native_cedar.sv').read_text()
    newtb=(new/'inputs/harness/tb_native_cedar.sv').read_text()
    assert oldtb.replace('if(!reset && !control_ready)begin','if(!reset && storage_initialized && !control_ready)begin',1)==newtb
    assert len(newp['passive_CP_package_input_net_mapping'])==7
    for name,h in newp['inputs'].items():assert sha(new/'inputs'/name)==h,name
    retain(old,out/old.name)
    profile=Path('/private/tmp/cp-native-sample-20261024.txt')
    assert profile.exists();shutil.copy2(profile,out/'stopped-v2-simulator-sample.txt')
    live=observation(new,out/('observed-'+new.name))
    dest=out/('observed-'+new.name)
    retain(new/'inputs',dest/'inputs')
    for name in ['provenance.json.gz','host-cycle-limit.txt','window-control.txt']:
        shutil.copy2(new/name,dest/name)
    original=[]
    for kind in ['5c65','queue']:
        for suffix in ['skip-wait-v1','input-reset-v3']:
            source=BUILD/f'native-cedar-{kind}-{suffix}-20261023'
            assert int((source/'host-cycle-limit.txt').read_text())==5000000000
            original.append(observation(source,out/('observed-'+source.name)))
    result=dict(status='REVIEWED_SHARED_CONTROL_NATIVE_SOURCE_AND_LIVE_PREFIX',scope=__doc__,
        stage=str(stage),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        component_all_physical_owners_meet15ns=True,component_full15ns_timing_passed=False,
        shared_global_gate_source_bound=True,ten_snapshot_board_enables_share_commit=True,
        eight_MSA_groups_share_commit=True,storage_readiness_kept_separate=True,
        native_clock_hz=57142857,sysper=4,ordinary_host_ticks=8,
        stopped_reset_correct_adapters_source_bound=True,lint=lint,
        feedback_warnings_not_physical_qualification=True,
        stopped_v2_reason=stop,corrected_monitor_only_changes=changed,
        corrected_fixed_host_bound=300000000,live_native_prefix=live,original_four_native_observations=original,
        runtime_pause_contract_qualified=False,firmware_qualified=False,physical_timing_qualified=False,
        speedup_qualified=False,hardware_changed=False,assembly_permitted=False,
        original60ns_and_separate66MHz_qualification_still_required=True,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['output','archive']:parser.add_argument('--'+name,type=Path,required=True)
    a=parser.parse_args();r=review(a.output.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),[],label='cp-native-global',extra_tools=[
        'stage_cp_prefix_native.py','prepare_cp_native_pause.py','prove_cp_register_projection.py',Path(__file__).name])
    print(r['status'],flush=True)
