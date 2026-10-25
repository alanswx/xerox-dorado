#!/usr/bin/env python3
"""Review the six-data-bit fall guard theorem and exact unknown fallback.

Bind all363 results to the real HIGH-identity certificate already maintained
by packet cache induction. Preserve all required selector bits, both waveform
depths, nonvacuous changed-data/clock witness and real bad-default output failure.
The helper is not yet integrated into the processor packet or fitted natively.
"""
import argparse
import json
from pathlib import Path

from prove_control_fall_data_guard import candidate,MASK
from prove_control_fall_fusion import sources
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from review_exact_pixel_phase import retain_text
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

BUILD=Path(__file__).resolve().parents[1]/'build/rtl-executive'


def read(p):return json.loads(p.read_text())


def review(out):
    assert not out.exists();d=BUILD/'fall-data-guard-proof-v1-20261024';s=read(d/'run-state.json');verify(d,s['files'])
    assert s['status']=='PASS_SOURCE_BOUND_FALL_DATA_GUARD' and len(s['cases'])==5
    assert s['omitted_guard_inputs']==list(range(348,354)) and s['adjacent_selector_inputs_retained']==[345,346,347]
    assert [i for i in range(398) if not (MASK>>i)&1]==list(range(348,354))
    raw=(Path(s['source'])/'candidate.sv').read_text();atomic,fall=sources(raw)
    assert (d/'atomic.sv').read_text()==atomic and (d/'fall.sv').read_text()==fall
    assert (d/'original.sv').read_text()==module(raw,'dorado_control_certified_fall_step')
    assert (d/'actual.sv').read_text()==candidate(raw).replace('module dorado_control_certified_fall_step #','module dorado_control_data_guard_step #',1)
    for r in s['cases']:
        p=d/(r['mode']+'-w'+str(r['waves']));verify(p,r['files'])
        assert r['passed'] and r['returncode']==0 and sha(p/'proof.log')==r['log_sha256']
        assert ('no model found: SUCCESS!' if r['mode']=='positive' else 'model found: FAIL!') in (p/'proof.log').read_text()
    cover=read(d/'cover-data-clock-w5/counterexample.json');signals={v['name']:v for v in cover['signal']}
    assert all(signals[n]['wave'][0]=='1' for n in ['certificate','entry','new_sparse','witnessed_data_change','equivalent'])
    assert signals['old_sparse']['wave'][0]=='0' and signals['actual']['data'][0]==signals['expected']['data'][0]
    dirs=[d]
    for name,status in [('fall-data-guard-four-state-v1-20261024','PASS_FALL_DATA_FOUR_STATE'),('fall-data-guard-bad-unknown-v1-20261024','PASS_FALL_DATA_UNKNOWN_COUNTEREXAMPLE')]:
        p=BUILD/name;r=read(p/'run-state.json');verify(p,r['files']);assert r['status']==status and r['proof_state_sha256']==sha(d/'run-state.json')
        assert r['actions'][0]['returncode']==0 and sha(p/'test.log')==r['test_log_sha256']
        log=(p/'test.log').read_text()
        assert ('FALL_DATA_XZ result mismatch trial=0 bit=283 mode=2' if r['mutation'] else 'FALL_DATA_FOUR_STATE_PASS known=4 unknown=4520 checks=4524') in log
        dirs.append(p)
    # This is the same independent identity predicate the existing cache
    # proof checks, not a new assumption that every arbitrary state is ready.
    history=BUILD/'sparse-pair-control-proof-v2-20261024';h=read(history/'run-state.json');verify(history,h['files'])
    assert h['status']=='PASS_ENTRY_CACHED_PACKET_INDUCTION_AND_OWNER_PROJECTION'
    r=next(r for r in h['cases'] if r['mode']=='induction');verify(history/'induction',r['files'])
    assert r['passed'] and sha(history/'induction/proof.log')==r['log_sha256']
    control=(history/'induction/control.sv').read_text()
    for name in ['dorado_control_hold_atomic_step','dorado_control_terminal_body']:
        assert module(control,name)==module(raw,name)
    cache=(history/'induction/cache.sv').read_text()
    assert 'dorado_control_hold_atomic_step #(.WAVES(5)) canonical_function(canonical_q,canonical_root,canonical_root,' in cache
    assert '({canonical_check_q,canonical_events,canonical_repeated,canonical_unsettled}=={canonical_q,113\'b0})' in cache
    out.mkdir(parents=True)
    for p in dirs:retain_text(p,out/p.name)
    for name in ['cache.sv','control.sv','top.sv','proof.ys','proof.log']:
        (out/('existing-history-'+name)).write_bytes((history/'induction'/name).read_bytes())
    result=dict(status='REVIEWED_SOURCE_BOUND_FALL_DATA_GUARD',scope=__doc__,compared_bits=363,physical_owners=156,clock_channels=56,
        omitted_input_bits=list(range(348,354)),native_data_bits=['DMD_'+str(i).zfill(2) for i in range(6,12)],retained_selector_bits=[345,346,347],
        checked_wave_depths=[5,24],actual_HIGH_identity_certificate_required=True,existing_cache_identity_invariant_source_bound=True,
        wrong_selector_and_missing_certificate_rejected=True,changed_data_with_real_clock_event_witnessed=True,
        four_state_known=4,four_state_XZ=4520,unknown_fallback_bad_control_changes_actual_results=True,
        no_result_or_state_bit_masked=True,no_saved_state_or_guest_cycles_added=True,
        processor_packet_integration_pending=True,full_replays_and_timing_fit_pending=True,native60ns_or_speed_qualified=False,
        hardware_changed=False,hardware_requires_user_release=True,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['output','archive']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();r=review(a.output.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),[],label='fall-data-guard',extra_tools=[
        'prove_control_fall_data_guard.py','test_control_fall_data_guard_four_state.py',Path(__file__).name])
    print(r['status'],flush=True)
