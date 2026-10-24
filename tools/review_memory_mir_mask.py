#!/usr/bin/env python3
"""Review formal/simulator phase proofs and the rejected15ns component fit.

Keep every original reference/check, rejected four-wave case and malformed
prototype. A faster combinational implementation is not a hardware speed
measurement. Original CP-arrival/native/clock/firmware gates remain open.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil

from fit_memory_mir_mask import gate
from probe_memory_mir_mask import candidate
from review_late_hold_and_input_reset import first_path,retain
from review_r_front_capture_followup import append_archive
from review_r_front_cp_subevents import observation
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def read(path):return json.loads(path.read_text())


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    final=[];history=[];formal=[];replays=[];negative=[]
    for engine in ['icarus','verilator']:
        source=BUILD/f'memory-hold-all-eight-{engine}-v1-20261023';r,g=gate(source,engine)
        assert r['settling_waves']==8 and r['all_hold_clocks']
        assert g['coverage']==dict(wave_checks=476288,high_distinct=63,low_clear_epochs=0)
        final.append(g);retain(source,out/source.name)
        for name in [f'memory-mir-mask-{engine}-v3-20261023',f'memory-mir-mask-eight-{engine}-v1-20261023']:
            source=BUILD/name;r=read(source/'run-state.json')
            assert r['status']=='PASS_MEMORY_MIR_MASK_ORIGINAL_COMPARISON'and r['all100_original_clock_counts_match']
            assert r['summary']=='checks=14884 writes=238 reads=6928 epochs=392'
            for row in r['source_files']:assert sha(source/row['path'])==row['output_sha256']
            history.append(dict(directory=name,status=r['status'],coverage=r['mask_coverage']));retain(source,out/name)
        for name in [f'memory-mir-mask-invert-{engine}-v3-20261023',f'memory-mir-mask-invert-original-{engine}-v4-20261023']:
            source=BUILD/name;r=read(source/'run-state.json')
            assert r['status']=='MEMORY_MIR_MASK_FAILED'and r['actions'][0]['returncode']==0 and r['actions'][-1]['returncode']!=0
            log=(source/'test.log').read_text()
            independent='invert-original'in name
            if independent:
                assert 'R front state commit changed settled Q check=16'in log
                assert sha(source/'inputs/tb_r_front_local.sv')==sha(Path(r['parent'])/'inputs/tb_r_front_local.sv')
            else:assert 'MIR Hold clock substitution mismatch check=16'in log
            negative.append(dict(directory=name,independent_original_bench=independent,rejected_at=16));retain(source,out/name)
        for version in [1,2]:
            source=BUILD/f'memory-mir-mask-{engine}-v{version}-20261023'
            if version==1:assert read(source/'preparation-failure.json')['status']=='PREPARATION_FAILED_BEFORE_COMPILE'
            else:
                r=read(source/'run-state.json');assert r['status']=='MEMORY_MIR_MASK_FAILED'and r['actions'][0]['returncode']!=0
            retain(source,out/source.name)
    assert final[0]['candidate_sha256']==final[1]['candidate_sha256']
    assert final[0]['clocks']==final[1]['clocks']
    for name,expected in [
        ('memory-mir-rtl-equivalence-v1-20261023','PASS_ALL_BINARY_CONTROL_MIR_EQUIVALENCE'),
        ('memory-mir-rtl-eight-waves-v1-20261023','PASS_ALL_BINARY_CONTROL_MIR_EQUIVALENCE'),
        ('memory-hold-all-eight-rtl-proof-v1-20261023','PASS_ALL_BINARY_CONTROL_MIR_EQUIVALENCE'),
        ('memory-mir-rtl-invert-counterexample-v1-20261023','PASS_RTL_COUNTEREXAMPLE_CONTROL'),
        ('memory-mir-rtl-four-waves-v1-20261023','TERMINAL_RTL_PROOF_NOT_ESTABLISHED')]:
        source=BUILD/name;r=read(source/'run-state.json');assert r['status']==expected
        for path,h in r['source_files'].items():assert sha(source/path)==h,path
        assert r['compared_output_bits']==363 and r['state_and_input_bits']==955 and not r['assumptions_added']
        if 'four-waves'in name:assert r['counterexample_found']and r['returncode']!=0
        formal.append(dict(directory=name,status=r['status'],candidate_waves=r.get('candidate_atomic_waves',24),
                           state_sha256=sha(source/'run-state.json')));retain(source,out/name)
    for name in ['memory-mir-counterexample-replay-v1-20261023','memory-mir-four-wave-replay-v1-20261023']:
        source=BUILD/name;r=read(source/'run-state.json')
        assert r['status']=='PASS_DUAL_SIMULATOR_FORMAL_COUNTEREXAMPLE_REPLAY'
        assert all(a['returncode']==0 for a in r['actions'])
        replays.append(dict(directory=name,mismatch_bits=r['mismatch_bits']));retain(source,out/name)
    proof=BUILD/'memory-mir-mask-proof-v2-20261023';r=read(proof/'proof.json')
    assert r['status']=='PASS_SOURCE_BOUND_MEMORY_MIR_MASK'
    assert [v['result']for v in r['cases']]==['unsat']*4+['sat']*4
    for name in ['memory-mir-mask-proof-v1-20261023',proof.name,'memory-hold-all-eight-prepared-v1-20261023']:
        retain(BUILD/name,out/name)
    fit=BUILD/'memory-hold-all-eight-fit-terminal-v1-20261023';r=read(fit/'run-state.json')
    assert r['status']=='TERMINAL_REVIEW_REQUIRED'and all(s['returncode']==0 for s in r['stages'])
    p=read(fit/'provenance.json')
    for name,h in p['files'].items():
        raw=(fit/name).read_bytes()
        if name=='clock_fit.qsf':raw=re.sub(rb'\nset_global_assignment -name LAST_QUARTUS_VERSION "17\.0\.2 Lite Edition"\n?$',b'',raw)
        import hashlib
        assert hashlib.sha256(raw).hexdigest()==h,name
    assert p['result_bits']==1400 and p['physical_SRAM_banks']==4
    reports=fit/'memory-hold-reports';corners=[]
    for condition in ['7_slow_1100mv_100c','7_slow_1100mv_-40c','MIN_fast_1100mv_100c','MIN_fast_1100mv_-40c']:
        row=dict(condition=condition)
        for key,prefix in [('global','clock-setup'),('owners','clock-owner-results'),('late_owners','clock-late-hold-owners'),('hold','clock-hold')]:
            row[key]=first_path(reports/(prefix+'-'+condition+'.rpt'))
        counts=re.findall(r'; Unconstrained (?:Clocks|Input Ports|Input Port Paths|Output Ports|Output Port Paths)\s*; (\d+)\s*; (\d+)',
                          (reports/('clock-ucp-'+condition+'.rpt')).read_text())
        assert len(counts)==5 and all(v==('0','0')for v in counts)
        corners.append(row)
    assert min(r['global']['slack_ns']for r in corners)==-7.699
    assert min(r['owners']['slack_ns']for r in corners)==-7.567
    assert all(r['hold']['slack_ns']>0 and r['late_owners']['slack_ns']>0 for r in corners)
    assert corners[1]['global']['source']=='launch[293]'and corners[1]['owners']['sink']=='result[1146]~reg0'
    retain(fit,out/fit.name)
    fit_summary=(fit/'clock_fit.fit.summary').read_text()
    assert '4,169 / 41,910'in fit_summary and '1,526 / 41,910'in fit_summary and 'Total registers : 3511'in fit_summary
    rejected=BUILD/'r-front-cp-subevents-matrix-v1-20261023/fast_gates-icarus'
    assert read(rejected/'run-state.json')['status']=='CP_BEFORE_ROOT_SUBEVENTS_FAILED'
    assert 'R front state mismatch check=12839'in(rejected/'test.log').read_text()
    native=[]
    for kind in ['5c65','queue']:
        for suffix in ['skip-wait-v1','input-reset-v3']:
            source=BUILD/f'native-cedar-{kind}-{suffix}-20261023';dest=out/('observed-'+source.name)
            row=observation(source,dest)
            assert int((source/'host-cycle-limit.txt').read_text())==5000000000
            for name in ['provenance.json.gz','host-cycle-limit.txt','window-control.txt']:shutil.copy2(source/name,dest/name)
            native.append(row)
    identities=[]
    for name in ['control-canonical-cp-identity-v1-20261023','control-bad-cp-before-identity-v1-20261023']:
        source=BUILD/name;r=read(source/'run-state.json')
        if r['status']=='RUNNING':
            identities.append(observation(source,out/('observed-'+name)))
        else:
            assert r['conditional_settled_identity']
            identities.append(dict(directory=name,status=r['status'],proof_succeeded=r['proof_succeeded'],
                                   counterexample_found=r['counterexample_found'],native_saved_pin_contract_qualified=False))
            retain(source,out/name)
    result=dict(status='REVIEWED_EXACT_PHASE_HOLD_REWRITE_AND_REJECTED_15NS_FIT',scope=__doc__,
        functional_gates=final,earlier_passes=history,negative_controls=negative,formal_proofs=formal,
        counterexample_replays=replays,fit_corners=corners,component_ALMs=4169,virtual_pin_ALMs=1526,component_registers=3511,
        fit_rejected=True,fields_removed=False,timing_exceptions_added=False,added_host_or_guest_cycles=0,
        clock_phase_counts_unchanged=True,eight_combinational_waves_proven=True,four_waves_rejected=True,
        derivative_simulation_coverage=dict(high_distinct_epochs=63,LOW_DisHold_clear_epochs=0),
        direct_RTL_proof_covers_arbitrary_binary_DisHold=True,original_CP_profile_rejection_retained=True,
        native_observations=native,next_identity_evidence=identities,native_identity_fast_path_implemented=False,
        hardware_changed=False,firmware_qualified=False,physical_timing_qualified=False,speedup_qualified=False,
        full_goal_active=True,full_FPGA_snapshots_deferred=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['output','archive']:parser.add_argument('--'+name,type=Path,required=True)
    args=parser.parse_args();result=review(args.output.resolve())
    append_archive(args.archive.resolve(),args.output.resolve(),[],label='phase-hold-formal',extra_tools=[
        'prove_memory_mir_mask.py','probe_memory_mir_mask.py','prove_memory_mir_rtl.py',
        'replay_memory_mir_counterexample.py','fit_memory_mir_mask.py',Path(__file__).name])
    print(result['status'],flush=True)
