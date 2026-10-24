#!/usr/bin/env python3
"""Preserve the failed kept-buffer fit and current late-Hold qualification.

All original state/RAM/reference checks remain. Native timing and original
firmware progress are separate gates. Keep early probe versions and negative
diagnostic-reader failures; do not turn a running fit into a timing pass.
"""
import argparse
import json
from pathlib import Path
import re
import shutil

from cofactor_memory_hold import transform
from stage_memory_hold_cofactor import kept,CONTROL
from review_late_hold_and_input_reset import retain,first_path
from review_r_front_capture_followup import append_archive
from review_r_front_cp_subevents import observation
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def read(path):return json.loads(path.read_text())


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    failed=BUILD/'memc-tag-buffers-fit-terminal-v1-20261024'
    envelope=BUILD/'memc-tag-buffers-native-v1-20261024-terminal.tgz.json'
    e=read(envelope);assert sha(Path(str(envelope)[:-5]))==e['archive_sha256']
    for row in e['members']:
        path=failed/row['path'];assert sha(path)==row['sha256']and path.stat().st_size==row['bytes']
    assert read(failed/'run-state.json')['status']=='TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    assert all(s['returncode']==0 for s in read(failed/'run-state.json')['stages'])
    assert read(failed/'physical-report/run-state.json')['status']=='TERMINAL_PHYSICAL_NATIVE_REPORTS_REVIEW_REQUIRED'
    p=read(failed/'r-front-control-native.json')
    for name,digest in p['source_hashes'].items():assert sha(failed/name)==digest,name
    assert read(failed/'physical-native-input-readback.json')['actual']==p['source_hashes']
    original=BUILD/'balanced-native-fit-terminal-v1-20261024';corners=[]
    for c in ['7_slow_1100mv_100c','7_slow_1100mv_-40c','MIN_fast_1100mv_100c','MIN_fast_1100mv_-40c']:
        row=dict(condition=c)
        for label,prefix in [('setup','native59-setup'),('hold','native59-hold'),
                             ('input','physical-native-input-setup'),('internal','physical-native-internal-setup')]:
            name=prefix+'-'+c+'.rpt'
            assert(failed/'core'/name).read_bytes()==(original/'core'/name).read_bytes()
            row[label]=first_path(failed/'core'/name)
        corners.append(row)
    assert corners[0]['setup']['slack_ns']==-1.314
    critical=(failed/'core/native59-setup-7_slow_1100mv_100c.rpt').read_text()
    critical=critical[critical.index('Path #1:'):critical.index('Path #2:')]
    assert re.search(r';\s*322\s*;[^\n]*u_a15\|qa\|q\s*;',critical)
    retain(failed,out/failed.name);shutil.copy2(envelope,out/envelope.name)
    proofs=[]
    for version in [1,2]:
        for variant in ['positive','negative']:
            d=BUILD/f'atomic-memory-hold-{variant}-proof-v{version}-20261024';s=read(d/'run-state.json')
            assert s['status']==('PASS_LATE_MEMORY_HOLD_ATOMIC_EQUIVALENCE'if variant=='positive'else'PASS_LATE_MEMORY_HOLD_COUNTEREXAMPLES')
            assert s['compared_output_bits']==363 and s['arbitrary_binary_input_and_state_bits']==955
            assert not s['assumptions_added']and [(r['waves'],r['passed'])for r in s['results']]==[(5,True),(24,True)]
            for name,digest in s['files'].items():assert sha(d/name)==digest,name
            if version==2:assert(d/'candidate.sv').read_text()==transform((d/'original.sv').read_text(),variant=='negative')
            retain(d,out/d.name);proofs.append(dict(directory=d.name,status=s['status'],state_sha256=sha(d/'run-state.json')))
    tests=[];base=BUILD/'atomic-memory-hold-positive-proof-v2-20261024'
    for kind in ['four-state','bad-unknown','icarus','verilator','bad-selection-icarus','bad-selection-verilator']:
        d=BUILD/f'atomic-memory-hold-{kind}-v1-20261024';s=read(d/'run-state.json')
        for name,digest in s['files'].items():assert sha(d/name)==digest,name
        log=(d/'test.log').read_text();assert sha(d/'test.log')==s['test_log_sha256']
        if kind.startswith('bad-selection'):
            assert s['status']=='TERMINAL_MEMORY_HOLD_TEST_NOT_ESTABLISHED'
            assert s['actions'][0]['returncode']==0 and s['actions'][-1]['returncode']!=0
            assert 'R front resolved source mismatch check=15'in log
            assert (d/'candidate.sv').read_text()==transform((base/'original.sv').read_text(),True)
            interpretation='Original reference rejected wrong selection at check15; first diagnostic matcher incorrectly expected only state mismatch. No rerun.'
        else:
            expected={'four-state':'PASS_MEMORY_HOLD_FOUR_STATE','bad-unknown':'PASS_MEMORY_HOLD_SIMULATOR_COUNTEREXAMPLE'}.get(kind,
                'PASS_MEMORY_HOLD_COMPLETE_ORIGINAL_COMPARISON')
            assert s['status']==expected
            interpretation='Positive full comparison'if kind in ['icarus','verilator']else'Explicit four-state test/negative fallback control'
            if kind in ['icarus','verilator']:
                assert s['all100_clock_counts_match']and s['original_testbench_and_references_unchanged']
                assert 'checks=14884 writes=238 reads=6928 epochs=392'in log
                assert s['cache_coverage']=='fast=13347 staged=1537 wait_edges=1553 retries=7 resets=1'
                assert (d/'candidate.sv').read_bytes()==(base/'candidate.sv').read_bytes()
        retain(d,out/d.name);tests.append(dict(directory=d.name,status=s['status'],interpretation=interpretation,
            state_sha256=sha(d/'run-state.json')))
    stage=BUILD/'memory-hold-cofactor-native-stage-v1-20261024';n=read(stage/'r-front-control-native.json')
    parent=Path(n['parent_stage']);old=read(parent/'r-front-control-native.json')
    for name,digest in n['source_hashes'].items():assert sha(stage/name)==digest,name
    changed=[name for name in old['source_hashes']if(stage/name).read_bytes().replace(n['project'].encode(),old['project'].encode())!=(parent/name).read_bytes()]
    assert changed==[CONTROL]
    assert(stage/CONTROL).read_text()==kept(transform((parent/CONTROL).read_text()))
    assert sha(stage/'stage_memory_hold_cofactor.py')==sha(ROOT/'tools/stage_memory_hold_cofactor.py')
    assert n['frequency_hz']==57142857 and n['sysper']==4 and not n['assembly_permitted']
    retain(stage,out/stage.name)
    lint=BUILD/'memory-hold-cofactor-native-lint-v1-20261024';l=read(lint/'run-state.json')
    assert l['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION'and l['structural_warnings_visible']
    assert l['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    assert (lint/'lint.log').read_text().count('%Warning-UNOPTFLAT:')==3
    retain(lint,out/lint.name)
    remote=BUILD/'memory-hold-cofactor-native-live-v1-20261024.json';live=read(remote)
    assert live['run_state']['status']=='RUNNING'and 'quartus'in live['stage_command']
    assert live['manifest_sha256']==sha(stage/'r-front-control-native.json');shutil.copy2(remote,out/remote.name)
    rom=BUILD/'native-initial-rom-wait-v3-20261024';r=read(rom/'results.json')
    assert r['status']=='PASS_ORIGINAL_ROM_WAIT_BINDING'and r['global_Q']==r['sequence_number']==0
    assert r['RTC430_value']==r['emulator_task_T_threshold']==0o15 and r['timeout_subtraction_is_negative']
    earlier=read(BUILD/'native-initial-rom-wait-v2-20261024/results.json')
    assert r['host_tick']>earlier['host_tick']and r['RTC430_value']==earlier['RTC430_value']+1
    for name,digest in r['input_hashes'].items():assert sha(rom/'inputs'/name)==digest,name
    debug=BUILD/'native-5c65-network-read-v5-20261024'
    assert sha(debug/'result.json')==r['readback_sha256']
    for d in [rom,debug]:retain(d,out/d.name)
    vendor=BUILD/'lcell-vendor-model-v1-20261024.json';v=read(vendor)
    assert re.sub(r'\s+',' ',v['model']).strip()=='module lcell (in, out); input in; output out; assign out = in; endmodule'
    shutil.copy2(vendor,out/vendor.name)
    native=[]
    for name in ['cp-prefix-balanced-native-boot-v1-20261024','cp-prefix-native-owned-cold-v1-20261024',
                 'native-cedar-5c65-input-reset-v3-20261023','native-cedar-queue-input-reset-v3-20261023']:
        run=BUILD/name;dest=out/('observed-'+name);native.append(observation(run,dest))
        for file in ['provenance.json.gz','host-cycle-limit.txt','window-control.txt']:shutil.copy2(run/file,dest/file)
    result=dict(status='REVIEWED_MEMORY_HOLD_ATOMIC_COFACTORS',scope=__doc__,failed_buffer_corners=corners,
        failed_buffer_reported_paths_byte_identical_to_original=True,failed_buffer_source_loads=322,
        failed_buffer_accepted=False,atomic_proofs=proofs,comparisons_and_negative_controls=tests,
        all_original_physical_Q_bits=844,all_original_RAM_bits=9216,all_original_clock_counts=100,
        before_and_after_input_bits=955,atomic_output_bits=363,arbitrary_binary_equivalence=True,
        explicit_four_state_checks=128,unknown_selector_checks=64,
        native_cofactor=n['late_memory_hold_cofactor'],native_live=live,native_boot_observations=native,
        baseline_RTC_observation=r,clock_calibration_or_packet0_root_cause_qualified=False,
        LCELL_vendor_model_retained_as_unimplemented_option=True,
        native_timing_qualified=False,firmware_qualified=False,speedup_qualified=False,
        assembly_permitted=False,hardware_changed=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['output','archive']:p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();r=review(a.output.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),[],label='atomic-memory-hold',extra_tools=[
        'cofactor_memory_hold.py','test_memory_hold_cofactor.py','stage_memory_hold_cofactor.py',
        'inspect_native_baseboard_process.py','inspect_initial_rom_wait.py',Path(__file__).name])
    print(r['status'],flush=True)
