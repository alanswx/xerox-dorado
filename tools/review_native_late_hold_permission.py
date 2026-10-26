#!/usr/bin/env python3
"""Retain native-bound Hold dependence, dual-engine witnesses and producer map."""
import argparse
import json
from pathlib import Path

from review_baseboard_clock_consumer import verify
from review_native_alu_capture import retain
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT/'build/rtl-executive'


def review(out):
    assert not out.exists()
    stage = BUILD/'native-riot-blockram-stage-v1-20261025'
    m = json.loads((stage/'r-front-control-native.json').read_text())
    verify(stage, m['source_hashes'])
    probe = BUILD/'native-late-hold-permission-v2-20261025'
    replay = BUILD/'native-late-hold-permission-replay-v2-20261025'
    cone = BUILD/'native-memc-hold-producer-v3-20261025'
    p = json.loads((probe/'run-state.json').read_text())
    r = json.loads((replay/'run-state.json').read_text())
    c = json.loads((cone/'results.json').read_text())
    assert p['status'] == 'TERMINAL_DEPENDENCE_PROBE'
    assert p['native_constant_inputs'] == dict(advance=0, pair_fall=0, cp_reset=0, refresh_before_run_capture=0)
    assert p['arbitrary_binary_input_bits'] == 1978 and p['assumptions'] == 0
    assert p['manifest_sha256'] == c['stage_manifest_sha256'] == sha(stage/'r-front-control-native.json')
    verify(stage, p['source_hashes'])
    verify(stage, c['source_hashes'])
    assert len(p['actions']) == 2
    for a in p['actions']:
        assert a['returncode'] == 0 and a['result'] == 'DEPENDENCE_COUNTEREXAMPLE'
        d = probe/a['label']
        verify(d, a['files'])
        assert sha(d/'proof.log') == a['log_sha256']
        assert sha(d/'counterexample.json') == a['counterexample_sha256']
        assert 'model found: FAIL!' in (d/'proof.log').read_text()
    assert r['status'] == 'PASS_DUAL_ENGINE_NATIVE_LATE_HOLD_DEPENDENCE_REPLAY'
    assert r['source_state_sha256'] == sha(probe/'run-state.json')
    verify(replay, r['files'])
    assert len(r['actions']) == 4 and all(a['returncode'] == 0 for a in r['actions'])
    for a in r['actions']:
        assert sha(replay/(a['label']+'.log')) == a['log_sha256']
    for engine in ['icarus', 'verilator']:
        log = (replay/(engine+'-test.log')).read_text()
        assert 'NATIVE_LATE_HOLD_COUNTEREXAMPLES_PASS cases=2' in log
        assert 'LATE_HOLD_REPLAY_CASE_PASS permission' in log
    cases = {v['label']: v for v in r['cases']}
    assert cases['validation']['validation_changed_bits'] == [69]
    assert cases['permission']['validation_changed_bits'] == [100, 101]
    assert (cases['permission']['permission_0'], cases['permission']['permission_1']) == ('0', '1')
    assert c['status'] == 'AUDITED_ACTUAL_MEMC_HOLD386_COMBINATIONAL_BOUNDARIES'
    assert len(c['visited_nets']) == 179 and len(c['boundary_nets']) == 132
    assert c['native_feedback_aliases']['Hold']['output_port'] == 'Hold__drv'
    assert {v.get('expression') for v in c['boundary_nets'].values() if v['kind'] == 'vector_expression'} == {
        '~r_front_q[840]', '~r_front_q[841]', 'r_front_q[842]'}
    assert set(c['residual_board_input_drivers']) == {'EcWantsA', 'STfree_p_', 'CLKEnable_p_b', 'ASEL_0', '_u_MD', '_u_MDI'}
    out.mkdir(parents=True)
    # Preserve the broader first run too; its permission witness used refresh=1.
    # It must never be presented as a native-bound witness.
    dirs = [BUILD/'native-late-hold-permission-v1-20261025',
            BUILD/'native-late-hold-permission-replay-v1-20261025',
            BUILD/'native-memc-hold-producer-v1-20261025',
            BUILD/'native-memc-hold-producer-v2-20261025', probe, replay, cone]
    for d in dirs:
        retain(d, out/'retained'/d.name)
    manual = ROOT/'DoradoDocs/dorado-hardware-manual/Dorado_Hardware_Manual.md'
    text = manual.read_text()
    hold = text.split('### 4.9. Hold', 1)[1].split('### 4.10.', 1)[0]
    assert 'clocked at t 1' in hold and 'previous instruction' in hold
    assert 'Higher priority tasks are not prevented' in hold
    assert 'disables clocks to CIA, TPC, and MIR' in hold
    (out/'original-hold-contract.txt').write_text(hold)
    result = dict(status='REVIEWED_NATIVE_BOUND_LATE_HOLD_DEPENDENCE_AND_PRODUCER_BOUNDARIES',
                  previous_goal_turn='no progress: status explanation and existing handoff reads only; resumed with new native-bound dependence evidence',
                  stage_manifest_sha256=sha(stage/'r-front-control-native.json'), source_files_verified=len(m['source_hashes']),
                  native_constant_inputs=p['native_constant_inputs'], arbitrary_binary_input_bits=1978, assumptions=0,
                  native_bound_validation_and_permission_counterexamples_replayed_in_both_engines=True,
                  cases=r['cases'], first_broader_run_refresh_caveat_retained=True,
                  firmware_reachability_or_valid_prefix_context_proved=False,
                  conclusion='Hold386 cannot be removed from the complete validation or aggregate guard for unrestricted inputs, including the actual native constant bindings. The witnesses are component inputs, not proof that firmware reaches those states. Do not infer architectural Hold may be bypassed or impose a new reachability assumption.',
                  producer_combinational_nets=179, producer_boundary_nets=132,
                  residual_board_input_drivers=c['residual_board_input_drivers'],
                  own_Hold_feedback_expanded_to_held_bits=[840, 841, 842],
                  producer_boundaries_are_state_or_complex_outputs_not_a_minimal_or_stable_descriptor=True,
                  original_Hold_contract_source_sha256=sha(manual), original_Hold_captures_at_t1_ns=30,
                  older_instruction_completion_higher_priority_tasks_and_Next_Lies_must_be_preserved=True,
                  next_action='Expand the six external producers and complex cache-output gating to stable owner/read-word sources. Prove which descriptor is available before the original t1 edge and how it is invalidated by writes/address changes. Then integrate a producer/capture phase cut that preserves original60ns overlap, all storage/readiness barriers and four-state fallback; do not add an unqualified stale-Ready register or a serialized120ns replacement.',
                  RTL_or_native_clock_guard_or_firmware_changed=False, new_Quartus_or_FPGA_load=False,
                  new_hardware_speed_gain_measured=False, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--archive', type=Path)
    p.add_argument('--archive-label', default='native-late-hold-permission')
    a = p.parse_args()
    r = review(a.output.resolve())
    if a.archive:
        append_archive(a.archive.resolve(), a.output.resolve(), [], label=a.archive_label,
                       extra_tools=[Path(__file__).name, 'probe_native_late_hold_permission.py',
                                    'replay_native_late_hold_permission.py', 'audit_native_memc_hold_producer.py'])
    print(r['status'], flush=True)
