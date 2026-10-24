#!/usr/bin/env python3
"""Preserve terminal native failures, capture proofs and open CP ordering.

The ordinary capture matrices pass; CP/root coincidences remain rejected.
Archive the rejected fixes and an original-reference diagnostic, without
selecting a core or weakening physical-state/clock assertions. Observe new
original-ROM jumper experiments separately from completed qualification.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shlex
import subprocess
import time

from prepare_native_skip_disk_wait import BASEBOARD,TB,OBSERVER
from probe_r_front_all_address_capture import adapter
from review_r_front_capture_followup import append_archive,sources
from test_r_front_all_address_matrix import check as check_all
from test_r_front_rm_address_matrix import check as check_rm
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def read(p):return json.loads(p.read_text())


def native_observation(run,out):
    r=read(run/'run-state.json');p=json.loads(gzip.decompress((run/'provenance.json.gz').read_bytes()))
    assert r['status']=='RUNNING' and r['actions'][-1]['stage']=='test'
    assert all(a['returncode']==0 for a in r['actions'][:-1])
    pid=r['actions'][-1]['pid']
    ps=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,stat=,command='],capture_output=True,text=True,check=True)
    assert str(run/'obj/native-cedar') in ps.stdout
    assert p['skip_disk_wait_jumper_fitted'] and p['host_limit']==5000000000
    assert int((run/'host-cycle-limit.txt').read_text())==5000000000
    original=Path(p['source_run']);old=json.loads(gzip.decompress((original/'provenance.json.gz').read_bytes()))
    assert int((original/'host-cycle-limit.txt').read_text())==5000000000
    assert p['used_hdl']==old['used_hdl'] and p['defines']==old['defines'] and p['artifacts']==old['artifacts']
    assert p['normal_baseboard_boot_presses']==old['normal_baseboard_boot_presses']==3
    changed=[]
    for n,h in old['inputs'].items():
        a,b=original/'inputs'/n,run/'inputs'/n;assert sha(a)==h
        expected=a.read_bytes()
        if n==BASEBOARD:
            expected=expected.replace(b"1'b0, TSetRun, SkipWait_p___drv, MCPStrb})",b"1'b0, TSetRun, 1'b0, MCPStrb})")
        if n==TB:expected=expected.replace(b'endmodule',OBSERVER.encode()+b'\nendmodule')
        assert b.read_bytes()==expected,n
        if sha(a)!=sha(b):changed.append(n)
    assert set(changed)=={BASEBOARD,TB}
    assert not out.exists();out.mkdir()
    for n,h in p['inputs'].items():
        src=run/'inputs'/n;assert sha(src)==h,n
        dst=out/'inputs'/n;dst.parent.mkdir(parents=True,exist_ok=True);dst.write_bytes(src.read_bytes())
    (out/'provenance.json.gz').write_bytes((run/'provenance.json.gz').read_bytes())
    (out/'run-state-at-observation.json').write_text(json.dumps(r,indent=2)+'\n')
    log=run/'test.log';size=log.stat().st_size
    with log.open('rb') as f:prefix=f.read(size)
    assert len(prefix)==size
    (out/'test-prefix.log').write_bytes(prefix)
    observation=dict(status='VERIFIED_LIVE_ORIGINAL_ROM_JUMPER_EXPERIMENT',source_run=str(run),pid=pid,
                     ps=ps.stdout.strip(),observed=time.time(),bounded_log_bytes=size,
                     compiled=True,host_limit=5000000000,original_limits_changed=False,
                     architectural_inputs_changed=['MCS6532 PB1 grounded: period SkipDiskWait jumper'],
                     original_firmware_changed=False,firmware_or_speed_qualified=False)
    (out/'observation.json').write_text(json.dumps(observation,indent=2)+'\n');return observation


def review(out):
    assert not out.exists();dirs=[];fits=[];matrices=[];probes=[];failures=[]
    for v,worst in [(7,-5.081),(8,-4.638)]:
        native=BUILD/f'r-front-physical-native-fit-terminal-v{v}-20261023'
        envelope=BUILD/f'r-front-physical-native-v{v}-terminal-20261023.tgz.json';e=read(envelope)
        assert sha(Path(str(envelope)[:-5]))==e['archive_sha256']
        for row in e['members']:
            p=native/row['path'];assert sha(p)==row['sha256'] and p.stat().st_size==row['bytes']
        manifest=read(native/'r-front-control-native.json');back=read(native/'physical-native-input-readback.json')
        assert back['actual']==manifest['source_hashes'] and len(e['members'])==953
        assert back['build_status']=='TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED' and back['report_status']=='TERMINAL_PHYSICAL_NATIVE_REPORTS_REVIEW_REQUIRED'
        d=BUILD/f'r-front-physical-native-review-v{v}-20261023';r=read(d/'results.json')
        assert r['status']=='REVIEWED_NATIVE_PHYSICAL_FIT_REJECTED' and r['setup_min_ns']==worst
        fits.append(dict(version=v,review=r,collection_sha256=sha(envelope)));dirs.extend([native,d])
    for name,status,checker in [
        ('r-front-rm-address-matrix-v2-20261023','PASS_R_FRONT_RM_ADDRESS_PROFILE_MATRIX',check_rm),
        ('r-front-all-address-matrix-v1-20261023','PASS_R_FRONT_ALL_ADDRESS_PROFILE_MATRIX',check_all)]:
        d=BUILD/name;r=read(d/'run-state.json');assert r['status']==status and len(r['cases'])==12 and not r['errors']
        for row in r['cases']:checker(ROOT/row['directory'])
        matrices.append(dict(run=name,status=status,cases=12,run_state_sha256=sha(d/'run-state.json')));dirs.append(d)
    expected=[2180,4393,2180,4393,4393,4393,1616,32]
    for engine in ['icarus','verilator']:
        for bad in [False,True]:
            n=f'r-front-all-address-{"invert-bank" if bad else "capture"}-{engine}-v1-20261023'
            d=BUILD/n;r=read(d/'run-state.json');sources(d,r);parent=Path(r['parent'])
            assert r['original_reference_and_checks_unchanged'] and r['actual_clock_functions_unchanged']
            for row in r['source_files']:
                a,b=parent/row['path'],d/row['path'];assert sha(a)==row['parent_sha256']
                expected_text=adapter(a.read_text(),a.name,r['mutation']) if row['changed'] else a.read_text()
                assert b.read_text()==expected_text
            assert r['actions'][0]['returncode']==0
            if bad:
                assert r['status']=='R_FRONT_ALL_ADDRESS_CAPTURE_FAILED' and r['actions'][-1]['returncode']!=0
                assert 'R front output mismatch check=15' in (d/'test.log').read_text()
            else:check_all(d)
            probes.append(dict(run=n,status=r['status'],run_state_sha256=sha(d/'run-state.json')));dirs.append(d)
        d=BUILD/f'r-front-all-address-pin-audit-{engine}-v1-20261023';r=read(d/'run-state.json');sources(d,r)
        assert r['status']=='PASS_POINTER_PACKAGE_D_AT_CLOCK_AUDIT' and r['all_R_address_D_at_clock_checked']
        assert [x['edges'] for x in r['all_address_pin_coverage']]==expected
        assert all(x['distinct']==0 for x in r['all_address_pin_coverage'])
        probes.append(dict(run=d.name,status=r['status'],pin_groups=r['all_address_pin_coverage']));dirs.append(d)
        d=BUILD/f'r-front-all-address-coincident-cp-{engine}-v1-20261023';r=read(d/'run-state.json');sources(d,r)
        assert r['status']=='POINTER_PACKAGE_D_AT_CLOCK_AUDIT_FAILED' and r['actions'][0]['returncode']==0 and r['actions'][-1]['returncode']!=0
        assert 'R_FRONT_LOCAL_SOLVER check=11327 shared=1' in (d/'test.log').read_text()
        failures.append(dict(run=d.name,status=r['status'],first_check=11327,source_sha256=sha(d/'run-state.json')));dirs.append(d)
    d=BUILD/'r-front-rm-address-coincident-cp-verilator-v1-20261023';r=read(d/'run-state.json');sources(d,r)
    assert r['status']=='POINTER_PACKAGE_D_AT_CLOCK_AUDIT_FAILED' and 'R_FRONT_LOCAL_SOLVER check=11327 shared=1' in (d/'test.log').read_text();dirs.append(d)
    d=BUILD/'r-front-cp-epoch-diagnostic-v1-20261023';r=read(d/'run-state.json');sources(d,r)
    assert r['status']=='DIAGNOSTIC_OBSERVED_ORIGINAL_AND_RETAINED_REJECTION' and not r['qualifying_gate']
    text=(d/'test.log').read_text()
    m=re.search(r'child_shared=([0-9a-f]+) joint_shared=([0-9a-f]+)',text)
    golden=int(re.search(r'CP_EPOCH_ORIGINAL shared=([0-9a-f]+)',text)[1],16)
    child,joint=map(lambda x:int(x,16),m.groups());assert child==golden and joint^golden==1<<24
    diagnostic=dict(original_shared=hex(golden),child_shared=hex(child),joint_shared=hex(joint),wrong_joint_bit=24,
                    original_fatal_retained=True,qualifying_gate=False);dirs.append(d)
    for v,check,message in [(1,6735,'R_FRONT_LOCAL_SOLVER check=6735'),(2,12839,'R front clock mismatch check=12839 channel=7 actual=1 expected=0')]:
        d=BUILD/f'r-front-cp-clock-epoch-verilator-v{v}-20261023';r=read(d/'run-state.json');sources(d,r)
        assert r['status']=='CP_NODISPATCH_CLOCK_EPOCH_FAILED' and r['actions'][0]['returncode']==0 and r['actions'][-1]['returncode']!=0
        assert message in (d/'test.log').read_text();failures.append(dict(run=d.name,status=r['status'],first_check=check));dirs.append(d)
    stage=BUILD/'r-front-physical-native-stage-v9-20261023';s=read(stage/'r-front-control-native.json')
    for n,h in s['source_hashes'].items():assert sha(stage/n)==h,n
    assert not s['assembly_permitted'] and not s['firmware_qualified']
    lint=BUILD/'r-front-physical-native-lint-v9-20261023';l=read(lint/'run-state.json')
    assert l['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION' and l['structural_warnings_visible'];dirs.extend([stage,lint])
    code='from pathlib import Path; import json; p=Path('+repr(s['project'])+'); print(json.dumps({"project_exists":p.exists(),"build_exists":(p/"run-state.json").exists()}))'
    remote=subprocess.run(['ssh','misterubuntu','python3 -c '+shlex.quote(code)],capture_output=True,text=True,check=True)
    absent=json.loads(remote.stdout);assert not absent['project_exists'] and not absent['build_exists']
    observations=[]
    for label in ['5c65','queue']:
        run=BUILD/f'native-cedar-{label}-skip-wait-v1-20261023';snapshot=BUILD/(out.name+'-native-'+label)
        observations.append(native_observation(run,snapshot));dirs.append(snapshot)
    out.mkdir(parents=True)
    result=dict(status='REVIEWED_NATIVE_REJECTIONS_AND_OPEN_CP_ORDERING',scope=__doc__,terminal_native_fits=fits,
                complete_matrices=matrices,bounded_capture_proofs=probes,retained_CP_rejections=failures,
                original_reference_diagnostic=diagnostic,v9_stage_sha256=sha(stage/'r-front-control-native.json'),
                v9_fit_started=False,v9_withheld_reason='CP/root-coincidence gate still rejects; resolve input timing contract/ordered subevents first',
                native_original_ROM_jumper_observations=observations,native_timing_qualified=False,
                original_firmware_boot_qualified=False,speedup_qualified=False,hardware_changed=False,assembly_permitted=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result,dirs


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path,required=True)
    a=p.parse_args();r,dirs=review(a.output.resolve());append_archive(a.archive.resolve(),a.output.resolve(),dirs,'clock-order-followup',
        ['probe_r_front_all_address_capture.py','test_r_front_all_address_matrix.py','stage_r_front_all_address_native.py',
         'inspect_r_front_pointer_epoch.py','diagnose_r_front_cp_epoch.py','probe_r_front_cp_clock_epoch.py',
         'prepare_native_skip_disk_wait.py','review_r_front_cp_ordering.py']);print(r['status'])
