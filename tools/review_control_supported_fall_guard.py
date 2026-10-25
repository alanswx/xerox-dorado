#!/usr/bin/env python3
"""Review a supported fall guard, complete results and four-state fallback.

Bind the independently derived HIGH certificate to the existing cache history
invariant. This only qualifies a pure helper; complete processor replays,
timing, native capture epochs and hardware speed remain separate gates.
"""
import argparse
import json
from pathlib import Path

from prove_control_fall_data_guard import candidate
from prove_control_supported_fall_guard import TOP
from prove_control_fall_fusion import sources
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from review_exact_pixel_phase import retain_text
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

BUILD=Path(__file__).resolve().parents[1]/'build/rtl-executive'


def read(p):
    return json.loads(p.read_text())


def checked(proof,four,bad):
    s=read(proof/'run-state.json');verify(proof,s['files'])
    assert s['status']=='PASS_SOURCE_BOUND_SUPPORTED_FALL_GUARD' and len(s['cases'])==5
    assert s['arbitrary_change_bits']==398 and s['compared_bits']==363
    assert s['arbitrary_binary_miter_input_bits']==954
    if s.get('phase_audit'):
        phase=Path(s['phase_audit']);a=read(phase/'run-state.json');verify(phase,a['files'])
        assert a['status']=='PASS_CONTROL_FALL_PHASE_SUPPORT' and sha(phase/'run-state.json')==s['phase_audit_state_sha256']
        for row in a['actions']:
            assert row['returncode']==0 and sha(phase/(row['label']+'.log'))==row['log_sha256']
    raw=(Path(s['source'])/'candidate.sv').read_text()
    assert s['source_state_sha256']==sha(Path(s['source'])/'run-state.json')
    atomic,fall=sources(raw)
    assert (proof/'atomic.sv').read_text()==atomic and (proof/'fall.sv').read_text()==fall
    assert (proof/'original.sv').read_text()==module(raw,'dorado_control_certified_fall_step')
    mask=int(s['mask_hex'],16)
    assert mask.bit_count()==s['guard_pin_count'] and 0<s['guard_pin_count']<=102 and 119 in s['omitted_guard_inputs']
    assert s['omitted_guard_inputs']==[i for i in range(398) if not (mask>>i)&1]
    assert (proof/'actual.sv').read_text()==candidate(raw,mask=mask).replace('module dorado_control_certified_fall_step #','module dorado_control_data_guard_step #',1)
    assert [(r['mode'],r['waves']) for r in s['cases']]==[('positive',5),('positive',24),('wrong-selector',5),('without-certificate',5),('cover-data-clock',5)]
    for row in s['cases']:
        d=proof/(row['mode']+'-w'+str(row['waves']));verify(d,row['files'])
        assert row['passed'] and row['returncode']==0 and sha(d/'proof.log')==row['log_sha256']
        expected=candidate(raw,row['mode']=='wrong-selector',mask).replace('module dorado_control_certified_fall_step #','module dorado_control_data_guard_step #',1)
        assert (d/'actual.sv').read_text()==expected
        top=TOP.replace('WAVES_VALUE',str(row['waves'])).replace('CANDIDATE_ENTRY','entry' if row['mode']=='without-certificate' else 'certified_entry')
        if s.get('phase_audit'):
            top=top.replace('changes[119] &&','changes[119] && changes[26] &&',1)
        assert (d/'top.sv').read_text()==top
        script=(d/'proof.ys').read_text()
        assert '-set' not in script
        assert ('no model found: SUCCESS!' if row['mode']=='positive' else 'model found: FAIL!') in (d/'proof.log').read_text()
    sig={v['name']:v for v in read(proof/'cover-data-clock-w5/counterexample.json')['signal']}
    assert all(sig[n]['wave'][0]=='1' for n in ['certificate','entry','new_sparse','witnessed_data_change','equivalent'])
    assert sig['old_sparse']['wave'][0]=='0' and sig['actual']['data'][0]==sig['expected']['data'][0]
    for d,status,mutation in [(four,'PASS_SUPPORTED_FALL_FOUR_STATE',None),(bad,'PASS_SUPPORTED_FALL_UNKNOWN_COUNTEREXAMPLE','bad-unknown')]:
        r=read(d/'run-state.json');verify(d,r['files'])
        assert r['status']==status and r['mutation']==mutation
        assert r['proof_state_sha256']==sha(proof/'run-state.json') and r['variable_input_bits']==954
        assert r['actions'][0]['returncode']==0 and sha(d/'test.log')==r['test_log_sha256']
        assert len(r['actions'])==2
        log=(d/'test.log').read_text()
        assert ('FALL_DATA_XZ result mismatch' if mutation else 'FALL_DATA_FOUR_STATE_PASS known=4 unknown=7632 checks=7636') in log
        assert (r['actions'][-1]['returncode']!=0)==bool(mutation)
    return s,raw


def apply_source(raw,review):
    r=read(review/'results.json')
    assert r['status']=='REVIEWED_SOURCE_BOUND_SUPPORTED_FALL_GUARD'
    proof=review/r['proof_directory'];four=review/r['four_state_directory'];bad=review/r['bad_unknown_directory']
    s,original=checked(proof,four,bad)
    assert r['proof_state_sha256']==sha(proof/'run-state.json') and r['mask_hex']==s['mask_hex']
    assert raw==original
    old=module(raw,'dorado_control_certified_fall_step')
    new=candidate(raw,mask=int(s['mask_hex'],16))
    assert raw.count(old)==1
    actual=raw.replace(old,new,1)
    assert actual.replace(new,old,1)==raw
    return actual


def review(out,direct_pins=False,phase=False):
    assert not out.exists()
    prefix='phase-supported-fall-guard' if phase else 'supported-fall-guard'
    proof=BUILD/(prefix+'-proof-v1-20261024')
    version=2 if direct_pins else 1
    four=BUILD/f'{prefix}-four-state-v{version}-20261024'
    bad=BUILD/f'{prefix}-bad-unknown-v{version}-20261024'
    s,raw=checked(proof,four,bad)
    if direct_pins:
        for d in [four,bad]:
            assert read(d/'run-state.json')['direct_before_after_pins']
            top=(d/'top.sv').read_text();tb=(d/'tb.sv').read_text()
            original=(proof/'positive-w5/top.sv').read_text()
            assert top.replace('before_pins,after_pins,','before_pins,changes,',1).replace(
                'wire [397:0] changes=before_pins ^ after_pins;',
                'wire [397:0] after_pins=before_pins ^ changes;',1)==original
            assert '{q,before_pins,after_pins,refresh,entry}=stimulus;' in tb
            assert 'dut(q,before_pins,after_pins,refresh,entry' in tb
    # Reuse only the already established identity invariant of the exact same
    # atomic and terminal functions. Architectural state is not assumed ready.
    history=BUILD/'sparse-pair-control-proof-v2-20261024';h=read(history/'run-state.json');verify(history,h['files'])
    assert h['status']=='PASS_ENTRY_CACHED_PACKET_INDUCTION_AND_OWNER_PROJECTION'
    row=next(r for r in h['cases'] if r['mode']=='induction');verify(history/'induction',row['files'])
    assert row['passed'] and sha(history/'induction/proof.log')==row['log_sha256']
    for name in ['dorado_control_hold_atomic_step','dorado_control_terminal_body']:
        assert module((history/'induction/control.sv').read_text(),name)==module(raw,name)
    cache=(history/'induction/cache.sv').read_text()
    assert 'dorado_control_hold_atomic_step #(.WAVES(5)) canonical_function(canonical_q,canonical_root,canonical_root,' in cache
    assert '({canonical_check_q,canonical_events,canonical_repeated,canonical_unsettled}=={canonical_q,113\'b0})' in cache
    out.mkdir(parents=True)
    for d in [proof,four,bad]:
        retain_text(d,out/d.name)
    if phase:
        audit_review=BUILD/'control-fall-phase-support-review-v1-20261024'
        assert read(audit_review/'results.json')['status']=='REVIEWED_CONTROL_FALL_PHASE_SUPPORT'
        retain_text(audit_review,out/audit_review.name)
    for name in ['cache.sv','control.sv','top.sv','proof.ys','proof.log']:
        (out/('existing-history-'+name)).write_bytes((history/'induction'/name).read_bytes())
    result=dict(status='REVIEWED_SOURCE_BOUND_SUPPORTED_FALL_GUARD',scope=__doc__,guard_pin_count=s['guard_pin_count'],
                omitted_pin_count=398-s['guard_pin_count'],mask_hex=s['mask_hex'],compared_bits=363,physical_owners=156,clock_channels=56,
                checked_wave_depths=[5,24],arbitrary_binary_miter_input_bits=954,arbitrary_change_bits=398,
                actual_HIGH_identity_certificate_required=True,existing_cache_identity_invariant_source_bound=True,
                required_selector_and_missing_certificate_counterexamples=True,changed_IfuData119_with_real_Q30_event_witness=True,
                changed_TWReq07_with_real_Q30_event_witness=bool(s.get('phase_audit')),
                four_state_known=4,four_state_XZ=7632,four_state_direct_before_after_pins=direct_pins,
                unknown_bad_control_changes_actual_results=True,
                no_result_or_state_bit_masked=True,unknown_and_nonfalling_fallback_unchanged=True,
                proof_directory=proof.name,four_state_directory=four.name,bad_unknown_directory=bad.name,
                proof_state_sha256=sha(proof/'run-state.json'),processor_packet_integration_pending=True,
                full_replays_and_timing_fit_pending=True,native60ns_or_speed_qualified=False,hardware_changed=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    assert apply_source(raw,out)!=raw
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['output','archive']:
        p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--direct-pins',action='store_true')
    p.add_argument('--phase',action='store_true')
    a=p.parse_args();r=review(a.output.resolve(),a.direct_pins,a.phase)
    label='supported-fall-guard-direct-pins' if a.direct_pins else 'supported-fall-guard'
    if a.phase:label='phase-'+label
    append_archive(a.archive.resolve(),a.output.resolve(),[],label=label,extra_tools=[
        'prove_control_supported_fall_guard.py','test_control_supported_fall_guard.py',Path(__file__).name])
    print(r['status'],flush=True)
