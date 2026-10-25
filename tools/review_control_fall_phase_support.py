#!/usr/bin/env python3
"""Review phase support and retain the earlier conservative-cone attempt.

This establishes a property of HIGH identity plus initial clocks, not complete
falling-transfer equivalence. A new guard still needs its actual363-bit proof,
unknown fallback checks, full processor replays and complete timing review.
"""
import argparse
import json
from pathlib import Path

from audit_control_fall_phase_support import TOP,BINARY,FULL,MITER
from audit_control_pin_support import support
from prove_control_fall_fusion import sources
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from review_exact_pixel_phase import retain_text
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

BUILD=Path(__file__).resolve().parents[1]/'build/rtl-executive'


def read(p):
    return json.loads(p.read_text())


def review(out):
    assert not out.exists()
    audit=BUILD/'control-fall-phase-support-v2-20261024'
    s=read(audit/'run-state.json');verify(audit,s['files'])
    assert s['status']=='PASS_CONTROL_FALL_PHASE_SUPPORT' and s['compared_phase_bits']==57
    assert s['arbitrary_binary_miter_input_bits']==953
    assert sha(Path(s['source'])/'run-state.json')==s['source_state_sha256']
    raw=(Path(s['source'])/'candidate.sv').read_text();atomic,_=sources(raw)
    assert (audit/'atomic.sv').read_text()==atomic
    terminal=module(raw,'dorado_control_terminal_body')
    start=terminal.index('    function automatic') if '    function automatic' in terminal else terminal.index(' function automatic')
    functions=terminal[start:terminal.index(' // Pure diagnostic wave vectors',start)]
    assert terminal.count('pins_before')==2
    assert 'previous=joint_clocks(q,clock_before,pins_before,mir_switch_view,tasking_view);' in terminal
    assert (audit/'cone-top.sv').read_text()==TOP.replace('FUNCTIONS',functions).replace('INSTANCE',BINARY).replace('UNSETTLED',"1'b0")
    assert sha(audit/'cone.json')==s['cone_json_sha256']
    syntactic=support(read(audit/'cone.json')['modules']['phase_function'],['pins'])['pins']
    assert syntactic==s['syntactic_pin_support'] and len(syntactic)==106
    care=[i for i in syntactic if i not in range(20,35)]
    assert care==s['guard_pin_support'] and len(care)==s['guard_pin_count']==91
    mask=sum(1<<i for i in care)
    assert s['support_mask_hex']==format(mask,'x') and s['task_wake_pins_in_support']==[]
    assert len(s['actions'])==4
    for row in s['actions']:
        label=row['label']
        assert row['returncode']==0 and sha(audit/(label+'.log'))==row['log_sha256']
        assert sha(audit/(label+'.ys'))==row['script_sha256']
        if label=='cone':continue
        waves=24 if label=='positive-w24' else 5
        value=mask if label.startswith('positive') else mask & ~(1<<345)
        full=TOP.replace('FUNCTIONS',functions).replace('INSTANCE',FULL.replace('WAVES_VALUE',str(waves))).replace('UNSETTLED','unsettled')
        assert (audit/(label+'-source.sv')).read_text()==full
        assert (audit/(label+'-top.sv')).read_text()==MITER.replace('MASK',format(value,'x'))
        script=(audit/(label+'.ys')).read_text()
        assert '-set' not in script and '-prove equal 1' in script
        assert ('no model found: SUCCESS!' if label.startswith('positive') else 'model found: FAIL!') in (audit/(label+'.log')).read_text()
    sig={v['name']:v for v in read(audit/'missing-selector345-w5-counterexample.json')['signal']}
    assert sig['equal']['wave'][0]=='0' and sig['actual']['data'][0]!=sig['projected']['data'][0]
    failed=BUILD/'control-fall-phase-support-v1-20261024'
    old=read(failed/'run-state.json');verify(failed,old['files'])
    assert old['status']=='TERMINAL_FALL_PHASE_SUPPORT_NOT_ESTABLISHED'
    assert len(old['actions'])==1 and old['actions'][0]['returncode']==0
    assert old['actions'][0]['log_sha256']==sha(failed/'cone.log')
    assert support(read(failed/'cone.json')['modules']['phase_function'],['pins'])['pins']==syntactic
    out.mkdir(parents=True)
    for d in [audit,failed]:retain_text(d,out/d.name)
    result=dict(status='REVIEWED_CONTROL_FALL_PHASE_SUPPORT',scope=__doc__,phase_bits=57,
                complete_HIGH_identity_owners=156,initial_clock_channels=56,checked_waves=[5,24],
                arbitrary_binary_inputs=953,syntactic_support=106,semantically_proved_support=91,
                task_wake_group=list(range(20,35)),task_wake_group_semantically_irrelevant_to_these57_bits=True,
                required_selector345_counterexample=True,syntactic_support_not_a_minimality_claim=True,
                earlier_size_assertion_stopped_before_projection_and_is_not_a_failed_equivalence=True,
                actual_363_bit_narrowed_guard_proof_required=True,four_state_and_processor_replays_required=True,
                native_timing_or_speed_qualified=False,hardware_changed=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['output','archive']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();r=review(a.output.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),[],label='control-fall-phase-support',extra_tools=[
        'audit_control_fall_phase_support.py',Path(__file__).name])
    print(r['status'],flush=True)
