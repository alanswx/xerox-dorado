#!/usr/bin/env python3
"""Bind the native startup jumper defect and its source-only staging repair.

The old diagnostics inherited 'jumper fitted' metadata while their stage overlay
restored the ungrounded PB1. Preserve those runs/limits and the read-only RIOT
inspection. A separate fresh original-ROM run restores the same baseline jumper,
retains all assertions/medium/timers/buttons and adds an actual package-pin check.
This is an environment repair, not Cedar, storage-speed or FPGA qualification.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import subprocess
import time

from preserve_native_environment import BASEBOARD,DEFAULT,FITTED,preserve
from review_exact_pixel_phase import retain_text
from review_late_hold_and_input_reset import retain
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

BUILD=Path(__file__).resolve().parents[1]/'build/rtl-executive'


def frozen(name):
    d=BUILD/name;r=json.loads(gzip.decompress((d/'provenance.json.gz').read_bytes()))
    for n,h in r['inputs'].items():assert sha(d/'inputs'/n)==h
    assert int((d/'host-cycle-limit.txt').read_text())==5000000000
    return d,r


def review(out):
    assert not out.exists();baseline,b=frozen('native-cedar-5c65-input-reset-v3-20261023')
    source=baseline/'inputs'/BASEBOARD;assert source.read_bytes().count(FITTED)==1 and b['skip_disk_wait_jumper_fitted']
    original=[];rows=[]
    for name in ['control-short-native-boot-v1-20261024','cp-prefix-balanced-native-boot-v1-20261024']:
        d,r=frozen(name);raw=(d/'inputs'/BASEBOARD).read_bytes()
        assert r['skip_disk_wait_jumper_fitted'] and raw.count(DEFAULT)==1 and FITTED not in raw
        fixed,receipt=preserve(raw,source.read_bytes(),b)
        assert fixed==source.read_bytes() and receipt['only_external_PB1_input_changed']
        unchanged,again=preserve(fixed,source.read_bytes(),b);assert unchanged==fixed and not again['changed']
        try:preserve(raw,raw,b)
        except AssertionError:pass
        else:raise AssertionError('False baseline claim accepted')
        rows.append(dict(run=name,metadata_claim_disagrees_with_actual_source=True,repair=receipt,
            provenance_sha256=sha(d/'provenance.json.gz'),bound_sha256=sha(d/'host-cycle-limit.txt')))
        original.append((d,r))
    fresh,n=frozen('control-short-native-jumper-boot-v2-20261024');old,oldr=original[0]
    changed=[k for k in oldr['inputs'] if oldr['inputs'][k]!=n['inputs'].get(k)]
    assert set(changed)=={BASEBOARD,'harness/tb_native_cedar.sv','run_r_front_control_native.py'}
    assert set(n['inputs'])-set(oldr['inputs'])=={'preserve_native_environment.py'} and set(oldr['inputs'])<=set(n['inputs'])
    assert (fresh/'inputs'/BASEBOARD).read_bytes()==source.read_bytes()
    assert n['artifacts']==oldr['artifacts'] and n['defines']==oldr['defines'] and n['used_hdl']==oldr['used_hdl']
    assert n['normal_baseboard_boot_presses']==oldr['normal_baseboard_boot_presses']==3
    assert n['host_clock_hz']==oldr['host_clock_hz']==57142857
    assert len(n['source_verified_physical_environment_preservation'])==1
    tb=(fresh/'inputs/harness/tb_native_cedar.sv').read_text()
    assert 'if(dut.m.b_BaseBd.u_i62.pb_in[1]!==1\'b0)' in tb and 'NATIVE_BOOT_ENV_PASS skip_wait_PB1=0' in tb
    board=(fresh/'inputs'/BASEBOARD).read_text()
    package=re.search(r'cell_MCS6532 u_i62 \(.*?\n  \);',board,re.S)[0]
    assert '.p37(Ram3_p_)' in package and '.pb_in({MCPBus_08, MCPABus_0, MCPABus_1, MCPABus_2, '+FITTED.decode() in package
    defs=(fresh/'inputs/skip-wait-evidence/doradoio.mdefs').read_text()
    assert 'MCPBusL = 580+PB' in defs and "SkipDiskWait' = 2" in defs
    inspection=BUILD/'control-short-native-timer-inspection-v3-20261024';i=json.loads((inspection/'result.json').read_text())
    assert i['pid']==75056 and i['read_only_guest_inspection'] and i['phase_alias_binding']['inverted']
    assert sha(inspection/'baseboard-riot-ram.bin')==i['riot_ram']['image_sha256']
    assert i['riot_ram']['timer100ms_word']==74 and i['riot_ram']['EndOfDelay_word']==229
    assert 'Process 75056 detached' in (inspection/'lldb.log').read_text()
    state=json.loads((fresh/'run-state.json').read_text());assert state['status']=='RUNNING'
    assert all(v['returncode']==0 for v in state['actions'][:-1])
    child=state['actions'][-1];assert child['stage']=='test' and 'returncode' not in child
    actual=subprocess.check_output(['ps','-p',str(child['pid']),'-o','pid=,etime=,stat=,command='],text=True).strip()
    assert str(fresh/'obj/native-cedar') in actual
    out.mkdir(parents=True)
    # Freeze immutable compiled inputs and a timestamped observation, not a
    # later mutable run-state reference or an assertion that startup passed.
    retain_text(fresh/'inputs',out/'fresh-inputs')
    for name in ['provenance.json.gz','host-cycle-limit.txt','compile.log']:(out/('fresh-'+name)).write_bytes((fresh/name).read_bytes())
    (out/'fresh-run-state-observed.json').write_text(json.dumps(state,indent=2)+'\n')
    log=(fresh/'test.log').read_bytes();log=log[:log.rfind(b'\n')+1] if log and not log.endswith(b'\n') else log
    (out/'fresh-test-observed.log').write_bytes(log)
    for d,r in original:
        p=out/d.name;p.mkdir();(p/'provenance.json.gz').write_bytes((d/'provenance.json.gz').read_bytes())
        (p/'BaseBd_nominal.v').write_bytes((d/'inputs'/BASEBOARD).read_bytes());(p/'host-cycle-limit.txt').write_bytes((d/'host-cycle-limit.txt').read_bytes())
    for name in ['control-short-native-timer-inspection-v1-20261024','control-short-native-timer-inspection-v2-20261024','control-short-native-timer-inspection-v3-20261024',
                 'control-short-native-jumper-boot-v1-20261024']:
        retain(BUILD/name,out/name)
    result=dict(status='REVIEWED_SOURCE_VERIFIED_NATIVE_JUMPER_PRESERVATION',scope=__doc__,original_runs=rows,
        original_runs_or_bounds_changed=False,source_only_PB1_restored_to_baseline=True,wrong_baseline_metadata_rejected=True,
        idempotent_already_fitted_stage=True,fresh_changed_files=changed,fresh_added_file='preserve_native_environment.py',
        all_other_compiled_sources_ROMs_defines_media_buttons_and_clocks_unchanged=True,
        actual_package_pin_monitor_added=True,read_only_RIOT_inspection=dict(timer100ms=74,EndOfDelay=229,process_detached=True),
        failed_inspector_and_preparation_attempts_preserved=True,fresh_run=str(fresh),fresh_run_observation=dict(time=time.time(),test_pid=child['pid'],ps=actual),
        fresh_run_bound=5000000000,firmware_boot_qualified=False,storage_or_native_speed_qualified=False,hardware_changed=False,
        hardware_requires_user_release=True,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['output','archive']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();r=review(a.output.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-jumper-preservation',extra_tools=[
        'preserve_native_environment.py','run_r_front_control_native.py','prepare_cp_native_pause.py','inspect_native_baseboard_process.py',Path(__file__).name])
    print(r['status'],flush=True)
