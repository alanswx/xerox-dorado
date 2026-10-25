#!/usr/bin/env python3
"""Retain source-bound CP projection, publication and bounded capture evidence.

This qualifies the declared digital arrival experiments only. It does not
select a native core, prove arbitrary asynchronous arrival orders, change the
retained prefix key, or replace original R latch capture and shared ownership.
"""
import argparse
import json
from pathlib import Path
import re

from generate_control_cp_source_reference import generate
from generate_cp_block_capture import rewrite,route
from prove_control_fall_fusion import sources
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from review_exact_pixel_phase import retain_text
from review_r_front_capture_followup import append_archive
from test_control_cp_source import WRAPPER
from test_control_hold import PROFILES
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
TOOLS=['generate_native_cp_source.py','test_native_cp_source.py','prove_native_cp_publication.py',
       'generate_control_cp_source_reference.py','generate_cp_block_capture.py',
       'test_control_cp_source.py','prove_cp_block_capture.py','review_cp_source_capture.py']


def read(p):return json.loads(p.read_text())


def review(out):
    assert not out.exists()
    cp=BUILD/'native-cp-source-v1-20261024';m=read(cp/'manifest.json');verify(cp,m['files'])
    assert m['status']=='GENERATED_SOURCE_BOUND_NATIVE_CP_INPUTS'
    assert len(m['direct_aliases'])==20 and len(m['decoded_MIR_sets'])==18
    assert any(r['net']=='CPOut.8' and r['index']==344 for r in m['direct_aliases'])
    proof=BUILD/'native-cp-source-proof-v1-20261024';p=read(proof/'run-state.json');verify(proof,p['files'])
    assert p['status']=='PASS_ORIGINAL_NATIVE_CP_SOURCE_PROJECTION'
    assert p['source_manifest_sha256']==sha(cp/'manifest.json') and p['known_cases']==65536 and p['XZ_cases']==3336
    for a in p['actions']:
        assert a['returncode']==0 and sha(proof/(a['label']+'.log'))==a['log_sha256']
    differences={}
    for n in ['wrong-selector','wrong-enable','stale-data']:
        signals={v['name']:v for v in read(proof/(n+'-counterexample.json'))['signal']}
        delta=int(signals['actual']['data'][0],2)^int(signals['expected']['data'][0],2)
        bits=[i for i in range(401) if (delta>>i)&1]
        assert bits and all(264<=i<=281 for i in bits)
        differences[n]=bits
    pub=BUILD/'native-cp-publication-proof-v1-20261024';u=read(pub/'run-state.json');verify(pub,u['files'])
    assert u['status']=='PASS_NATIVE_CP_SOURCE_PUBLICATION_INDUCTION'
    assert u['CP_source_manifest_sha256']==sha(cp/'manifest.json') and u['CP_proof_state_sha256']==sha(proof/'run-state.json')
    assert u['architectural_owner_and_context_powerup_unconstrained'] and u['retained_prefix_key_unchanged']
    assert [r['mode'] for r in u['cases']]==['induction','raw-decoder-snapshot','wrong-CPOut8-snapshot']
    for r in u['cases']:
        d=pub/r['mode'];verify(d,r['files'])
        assert r['passed'] and r['returncode']==0 and sha(d/'proof.log')==r['log_sha256']
        script=(d/'proof.ys').read_text();assert '-set-init-zero' not in script
        assert ('-tempinduct' if r['mode']=='induction' else '-prove interfaces_equal 1 -falsify') in script
    disabled=BUILD/'cp-block-default-proof-v1-20261024';z=read(disabled/'run-state.json');verify(disabled,z['files'])
    assert z['status']=='PASS_DISABLED_CP_BLOCK_CAPTURE_ALL_RESULTS' and z['arbitrary_binary_input_bits']==955
    assert z['compared_bits']==363 and z['no_assumptions']
    for r in z['cases']:
        d=disabled/f"disabled-w{r['waves']}";verify(d,r['files'])
        assert r['passed'] and r['returncode']==0 and sha(d/'proof.log')==r['log_sha256']
    assert [r['waves'] for r in z['cases']]==[5,24]
    out.mkdir(parents=True)
    generate(ROOT,out/'review-reference')
    raw=(Path(z['source'])/'candidate.sv').read_text();fn=(cp/'functions.sv').read_text()
    _,fall=sources(raw)
    records=[];dirs=[cp,proof,pub,disabled]

    def checked(name,positive=True,full=False,repro=False,elaboration=False,normal=False,bad=False):
        d=BUILD/name;s=read(d/'run-state.json')
        verify(d/'inputs',s['inputs']);verify(d,s['compiled_sources'])
        assert s['status']==('PASS_CONTROL_HOLD_PROBE' if positive else 'CONTROL_HOLD_PROBE_FAILED')
        assert s['native_integrated'] is False and s['physical_timing_qualified'] is False
        assert s['reference']['physical_bits']==156 and s['reference']['clock_channels']==56 and s['reference']['flag_count']==94
        assert s['reference']['shared_CPOut8_slot']==344 and len(s['reference']['closed_CP_set_boundaries'])==18
        for f in (out/'review-reference').glob('*.sv'):
            assert (d/'reference'/f.name).read_bytes()==f.read_bytes(),(name,f.name)
        assert sha(d/'candidate.sv')==s['executed_candidate_sha256'] and sha(d/'tb.sv')==s['testbench_sha256']
        actions=s['actions'];assert all(a.get('returncode') is not None for a in actions)
        if elaboration:
            assert len(actions)==1 and actions[0]['returncode']!=0
            assert 'CP_TPC_BLOCK_EARLY' in (d/'compile.log').read_text()
        else:
            assert len(actions)==2 and actions[0]['returncode']==0 and (actions[-1]['returncode']==0)==positive
            log=(d/'test.log').read_text()
            if not positive:
                assert 'CONTROL_HOLD_DIFF bit=42' in log and 'Control Hold state mismatch' in log
            elif full:
                assert 'CONTROL_CP_SOURCE_PASS checks=29570 words=4096 collisions=128 Qs=156 clocks=56 flags=94 sets=3ffff' in log
                assert len(re.findall(r'^CONTROL_CP_SOURCE_CLOCK channel=\d+ edges=\d+$',log,re.M))==56
            elif normal:
                assert 'CONTROL_NORMAL_BLOCK_REPRO_PASS' in log
            elif repro:
                assert 'CONTROL_CP_BLOCK_REPRO_PASS' in log
            if positive:
                g,f,phase=PROFILES[s['profile']];descriptor=route(ROOT,g,f)
                assert descriptor['strict_order'] and descriptor==s['CP_TPC_block_capture_descriptor']
                atomic,_=sources(rewrite(raw,bool(descriptor['early'])))
                expected=atomic+'\n'+fall+'\n'+module(raw,'dorado_control_certified_fall_step')+'\n'+module(raw,'dorado_control_hold_ordered_step')+'\n'
                expected+=WRAPPER.replace('FUNCTIONS',fn).replace('ARRIVAL',str(int(phase<g)))
                assert (d/'candidate.sv').read_text()==expected
        records.append(dict(directory=name,status=s['status'],engine=s['engine'],profile=s['profile'],
                            full_CP_suite=full,normal_Block_case=normal,bad_normal_epoch=bad,
                            elaboration_failure=elaboration,state_sha256=sha(d/'run-state.json')))
        dirs.append(d)

    checked('control-cp-source-icarus-later-v1-20261024',False,full=True)
    for engine in ['icarus','verilator']:
        checked(f'control-cp-block-{engine}-later-v1-20261024',False,repro=True)
        checked(f'control-cp-block-{engine}-later-fixed-v1-20261024',False,elaboration=True)
        checked(f'control-cp-block-{engine}-later-fixed-v2-20261024',True,repro=True)
        checked(f'control-cp-source-{engine}-later-fixed-v1-20261024',True,full=True)
    for profile in ['balanced','fast']:
        checked(f'control-cp-source-verilator-{profile}-fixed-v1-20261024',True,full=True)
    checked('control-normal-block-icarus-later-fixed-v1-20261024',True,normal=True)
    checked('control-normal-block-icarus-later-bad-v1-20261024',False,normal=True,bad=True)
    result=dict(status='REVIEWED_CP_SOURCE_PUBLICATION_AND_BOUNDED_CAPTURE',scope=__doc__,
                physical_CP_wires=13,direct_CP_aliases=20,decoded_MIR_set_inputs=18,
                CPOut8_already_present_at_native_slot=344,all_pin_results_compared=401,
                formal_source_variables=417,additional_old_CP_counterexample_variables=13,formal_port_bits=430,
                formal_negative_real_MIR_differences=differences,known_projection_cases=65536,XZ_projection_cases=3336,
                actual_publication_induction_unbounded=True,architectural_powerup_unconstrained=True,
                retained_generic_prefix_key_unchanged=True,normal_memory_MIR_data_independent=True,
                default_off_equivalence_wave_depths=[5,24],default_off_compared_bits=363,
                full_suite_per_run=dict(checks=29570,stable_CP_words=4096,root_strobe_collisions=128,
                                       state_bits=156,clock_channels_compared=56,flags=94,physical_reset_seed=0),
                profiles=dict(later_control=route(ROOT,1750,400),balanced=route(ROOT,1000,1000),fast_gates=route(ROOT,250,1500)),
                tied_arrival_profile_qualified=False,arbitrary_owner_or_arrival_coverage_qualified=False,
                historical_metadata_correction='Earlier fixed runs incorrectly label atomic/terminal unchanged; exact compiled source is the rewritten descriptor version, checked here.',
                failed_attempts_preserved=True,runs=records,native_integrated=False,
                shared_R_IFU_Memory_capture_pending=True,original16_R_latch_ownership_required=True,
                physical_15ns_or_60ns_qualified=False,new_RBF_ready=False,hardware_changed=False,full_goal_active=True)
    for d in dirs:retain_text(d,out/d.name)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['output','archive']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();r=review(a.output.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),[],label='cp-source-capture',extra_tools=TOOLS)
    print(r['status'])
