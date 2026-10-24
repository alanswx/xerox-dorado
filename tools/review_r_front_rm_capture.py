#!/usr/bin/env python3
"""Review bounded low RM capture tests and terminal pre-Cedar traffic.

Keep failed matrix-driver metadata checks, coverage gaps and live experiments
explicit. Original five-billion-tick native runs stay terminal at their bounds;
their power-up traffic is not a Cedar workload or a hardware speed measurement.
"""
import argparse
import json
from pathlib import Path
import subprocess
import time

from probe_r_front_rm_address_epoch import address
from report_native_storage_traffic import load,compare
from review_r_front_capture_followup import append_archive,sources
from test_r_front_forward_view import sha
from test_r_front_pointer_matrix import check

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def read(p):return json.loads(p.read_text())


def review(out):
    assert not out.exists();dirs=[];gates=[]
    matrix=BUILD/'r-front-pointer-capture-matrix-v1-20261023';m=read(matrix/'run-state.json')
    assert m['status']=='PASS_R_FRONT_POINTER_CAPTURE_PROFILE_MATRIX' and len(m['cases'])==12 and not m['errors']
    for row in m['cases']:check(ROOT/row['directory'])
    dirs.append(matrix)
    for engine in ['icarus','verilator']:
        for bad in [False,True]:
            d=BUILD/f'r-front-rm-address-{"zero-" if bad else ""}{engine}-v1-20261023';r=read(d/'run-state.json');sources(d,r)
            parent=Path(r['parent']);assert sha(parent/'run-state.json')==r['parent_run_state_sha256']
            for row in r['source_files']:
                p=d/row['path'];q=parent/row['path'];assert sha(q)==row['parent_sha256']
                expected=address(q.read_text(),r['mutation']) if p.name=='dorado_local_r_address_step.sv' else q.read_text()
                assert p.read_text()==expected
            assert r['original_reference_and_checks_unchanged'] and r['only_four_RBase_capture_inputs_changed']
            assert r['actions'][0]['returncode']==0
            if bad:
                assert r['status']=='R_FRONT_RM_ADDRESS_EPOCH_PROBE_FAILED' and r['actions'][-1]['returncode']!=0
                assert 'R front resolved source mismatch check=1434' in (d/'test.log').read_text()
            else:
                assert r['status']=='PASS_R_FRONT_RM_ADDRESS_EPOCH_PROBE' and r['actions'][-1]['returncode']==0
                assert r['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
            gates.append(dict(run=d.name,status=r['status'],run_state_sha256=sha(d/'run-state.json')));dirs.append(d)
        d=BUILD/f'r-front-rm-address-pin-audit-{engine}-v1-20261023';r=read(d/'run-state.json');sources(d,r)
        assert r['status']=='PASS_POINTER_PACKAGE_D_AT_CLOCK_AUDIT' and r['RM_capture_coverage']==[4393,0]
        assert r['low_RM_address_D_at_clock_checked'] and r['all_original_checks_retained'] and r['actions'][-1]['returncode']==0
        gates.append(dict(run=d.name,status=r['status'],actual_clock_edges=4393,distinct_epoch_samples=0));dirs.append(d)
    stage=BUILD/'r-front-physical-native-stage-v8-20261023';s=read(stage/'r-front-control-native.json')
    for n,h in s['source_hashes'].items():assert sha(stage/n)==h,n
    assert s['low_RM_RBase_capture_data_bits']==4 and not s['assembly_permitted']
    lint=BUILD/'r-front-physical-native-lint-v8-20261023';l=read(lint/'run-state.json')
    assert l['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION' and l['structural_warnings_visible']
    dirs.extend([stage,lint])
    failed=BUILD/'r-front-rm-address-matrix-v1-20261023';f=read(failed/'run-state.json')
    assert f['status']=='R_FRONT_RM_ADDRESS_PROFILE_MATRIX_FAILED' and not f['actions'] and not f['cases']
    assert all("KeyError('original_reference_unchanged')"==e['error'] for e in f['errors'])
    assert sha(failed/'initial-driver.py')==f['source_hashes']['tools/test_r_front_rm_address_matrix.py'];dirs.append(failed)
    running=BUILD/'r-front-rm-address-matrix-v2-20261023';r=read(running/'run-state.json')
    if r['status']=='RUNNING':
        ps=subprocess.run(['ps','-p',str(r['pid']),'-o','pid=,etime=,stat=,command='],capture_output=True,text=True,check=True)
        assert str(running.relative_to(ROOT)) in ps.stdout
        observation=dict(status='VERIFIED_LIVE_MATRIX',pid=r['pid'],cases_at_observation=len(r['cases']),ps=ps.stdout.strip(),observed=time.time())
    else:
        assert r['status']=='PASS_R_FRONT_RM_ADDRESS_PROFILE_MATRIX' and len(r['cases'])==12 and not r['errors']
        observation=dict(status=r['status'],cases=12);dirs.append(running)
    native=[]
    for name in ['native-cedar-5c65-windows-v2-20261022','native-cedar-queue-windows-v2-20261022']:
        d=BUILD/name;r=read(d/'run-state.json');p=load(d)
        assert r['status']=='TERMINAL_NATIVE_MEMORY_DIAGNOSTIC' and r['actions'][-1]['returncode']==0
        assert int((d/'host-cycle-limit.txt').read_text())==5000000000
        assert 'NATIVE_DIAGNOSTIC_STOP reason=  HOST_LIMIT host=5000000000' in (d/'test.log').read_text()
        assert not p['actual_Cedar113_key_release_observed'] and p['progress'][-1]['issues']==1 and p['progress'][-1]['BFN']==0
        native.append(p);dirs.append(d)
    comparison=compare(*native);assert comparison['compared_completed_prefix']==8191 and comparison['ordered_operation_data_agree']
    out.mkdir(parents=True)
    result=dict(status='REVIEWED_RM_CAPTURE_AND_TERMINAL_PRE_CEDAR_TRAFFIC',scope=__doc__,bounded_gates=gates,
                pointer_matrix_status=m['status'],pointer_matrix_cases=12,RM_profile_matrix=observation,
                native_v8_manifest_sha256=sha(stage/'r-front-control-native.json'),distinct_capture_epoch_samples=0,
                original_native_traffic_comparison=comparison,
                original_native_endpoints=[p['progress'][-1] for p in native],original_native_bounds_extended=False,
                full_firmware_traffic_qualified=False,native_timing_qualified=False,speedup_qualified=False,assembly_permitted=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result,dirs


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path,required=True)
    a=p.parse_args();r,dirs=review(a.output.resolve());append_archive(a.archive.resolve(),a.output.resolve(),dirs,'address-followup',
        ['probe_r_front_rm_address_epoch.py','test_r_front_rm_address_matrix.py','stage_r_front_rm_address_native.py',
         'review_r_front_rm_capture.py','report_native_storage_traffic.py']);print(r['status'])
