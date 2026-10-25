#!/usr/bin/env python3
"""Review native publication, complete replays and both equal-domain fits.

This qualifies an owned processor packet component, not native source/device
epochs, sixty-nanosecond cadence, SDRAM, Cedar or hardware speed. Retain failed
testbench attempts and distinguish the old racing false negative calibration.
"""
import argparse
import json
from pathlib import Path
import re

from generate_owned_r_packet import cache,caller
from generate_cached_r_packet import stage
from generate_parallel_r_fault import packet
from test_owned_r_packet import bench,trace_links
from fit_owned_r_packet import TOP,checked
from review_baseboard_clock_consumer import collection,CORNERS,verify
from review_exact_pixel_phase import retain_text
from review_late_hold_and_input_reset import first_path
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

BUILD=Path(__file__).resolve().parents[1]/'build/rtl-executive'


def read(p):return json.loads(p.read_text())


def review(out,digital_only=False):
    assert not out.exists();dirs=[];envelopes=[]
    proof=BUILD/'owned-packet-context-proof-v1-20261024';p=read(proof/'run-state.json');verify(proof,p['files'])
    assert p['status']=='PASS_SOURCE_BOUND_OWNED_PACKET_CANONICAL_CONTEXT' and len(p['cases'])==6
    assert p['cache_state_bits_compared']==1416 and p['physical_control_projection_bits']==156
    assert p['stored_owner_bits']==828 and p['original_external_R_bits']==16
    assert p['architectural_power_up_state']=='unconstrained' and p['external_refresh_histories_unconstrained']
    for row in p['cases']:
        d=proof/row['mode'];verify(d,row['files']);assert row['passed'] and row['returncode']==0 and sha(d/'proof.log')==row['log_sha256']
        if row['mode']=='induction':assert 'Induction step proven: SUCCESS!' in (d/'proof.log').read_text() and '-set-init-zero' not in (d/'proof.ys').read_text()
        elif row['mode']!='projection':assert 'model found: FAIL!' in (d/'proof.log').read_text() and '-prove interfaces_equal 1 -falsify' in (d/'proof.ys').read_text()
    source=Path(p['source']);assert sha(source/'run-state.json')==p['source_state_sha256']
    raw=(source/'candidate.sv').read_text();bp=(proof/'native-backplane.v').read_text()
    assert (proof/'caller.sv').read_text()==caller(raw,bp)
    assert (proof/'generic-cache.sv').read_text()==cache(raw) and (proof/'specialized-cache.sv').read_text()==cache(raw,True)
    dirs.append(proof)
    for name,status,needle in [('owned-context-four-state-v3-20261024','PASS_OWNED_CONTEXT_FOUR_STATE','context_cases=4984 external_cases=68 known_cases=2 trials=5054 checks=40292'),
                              ('owned-context-bad-unknown-v3-20261024','PASS_OWNED_CONTEXT_UNKNOWN_COUNTEREXAMPLE','unknown_phase=1')]:
        d=BUILD/name;s=read(d/'run-state.json');verify(d,s['files'])
        assert s['status']==status and s['proof_state_sha256']==sha(proof/'run-state.json')
        assert s['actions'][0]['returncode']==0 and sha(d/'test.log')==s['test_log_sha256'] and needle in (d/'test.log').read_text();dirs.append(d)
    # These are evidence of failed fixture attempts, not qualified controls.
    for name in ['owned-context-four-state-v1-20261024','owned-context-four-state-v2-20261024','owned-context-bad-unknown-v1-20261024']:
        d=BUILD/name;s=read(d/'run-state.json');verify(d,s['files'])
        assert s['actions'][0]['returncode']==0 and s['actions'][-1]['returncode']!=0
        assert sha(d/'test.log')==s['test_log_sha256'] and 'interface mismatch trial=0 root=1' in (d/'test.log').read_text();dirs.append(d)
    failed=BUILD/'owned-generic-icarus-v1-20261024';s=read(failed/'run-state.json');verify(failed,s['files'])
    assert s['status']=='TERMINAL_OWNED_PACKET_NOT_ESTABLISHED' and s['actions'][0]['returncode']!=0
    assert 'declaration after use' in (failed/'compile.log').read_text();dirs.append(failed)
    counts=[]
    for specialized in [False,True]:
        label='specialized' if specialized else 'generic'
        for engine in (['verilator'] if digital_only else ['icarus','verilator']):
            d=BUILD/f'owned-{label}-{engine}-v2-20261024';s=checked(d)
            assert s['specialized']==specialized and s['proof_state_sha256']==sha(proof/'run-state.json')
            assert s['trace_links']==trace_links((source/'tb.sv').read_text())
            assert (d/'tb.sv').read_text()==bench((source/'tb.sv').read_text())
            assert (d/'caller.sv').read_text()==caller(raw,bp)
            assert (d/'packet.sv').read_text()==packet(raw)+'\n'+stage(raw,True)+'\n'+cache(raw,specialized)
            for row in s['source_files']:assert sha(d/row['path'])==row['output_sha256']==sha(source/row['path'])
            counts.append(re.findall(r'^PACKET_CLOCK channel=(\d+) edges=(\d+)$',(d/'test.log').read_text(),re.M));dirs.append(d)
    assert all(len(c)==100 and c==counts[0] for c in counts)
    negatives=[]
    for mutation,needle in [('early-write','PACKET invalid geometry committed bit=0'),('drop-forward','PACKET second result mismatch record=203'),('stale-intermediate','PACKET second result mismatch record=16')]:
        d=BUILD/f'owned-{mutation}-icarus-v2-20261024';s=read(d/'run-state.json');verify(d,s['files'])
        assert s['status']=='PASS_OWNED_PACKET_COUNTEREXAMPLE' and s['specialized'] and s['mutation']==mutation
        assert s['actions'][0]['returncode']==0 and s['actions'][-1]['returncode']!=0
        assert sha(d/'test.log')==s['test_log_sha256'] and needle in (d/'test.log').read_text()
        assert (d/'packet.sv').read_text()==packet(raw,mutation)+'\n'+stage(raw,True,mutation)+'\n'+cache(raw,True)
        negatives.append(dict(mutation=mutation,original_assertion=needle));dirs.append(d)
    if digital_only:
        out.mkdir(parents=True)
        for d in dirs:retain_text(d,out/d.name)
        result=dict(status='REVIEWED_OWNED_CONTEXT_AND_VERILATOR_PACKETS',scope=__doc__,cache_bits_inductively_compared=1416,
            control_projection_bits=156,source_bound_stored_owners=828,external_R_aliases=16,powerup_owners_unconstrained=True,
            prefix_key_unchanged=True,refresh_comparison_retained=True,whole_final_root_preserved=True,
            binary_proof_cases=6,XZ_context_cases=4984,XZ_external_cases=68,four_state_comparisons=40292,
            historical_racing_false_control_excluded=True,failed_fixture_attempts_preserved=True,
            verilator_full_replays=2,original_transactions_per_replay=14884,commits=10423,pairs=4461,paired_writes=119,forwarded_bytes=238,
            all_original_assertions_retained=True,invalid_geometries=403,before_root_negative_is_explicit_contract_violation=True,
            compiled_negative_controls=negatives,Icarus_full_replays_qualification_pending=True,
            equal_domain_422_input_4943_output_fits_pending=True,variant_selected=False,hardware_changed=False,
            source_CP_device_interior_epochs_qualified=False,native60ns_or_hardware_speed_qualified=False,
            hardware_requires_user_release=True,full_goal_active=True)
        (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result
    fits=[];stages=[]
    for label in ['generic','specialized']:
        st=BUILD/f'owned-{label}-stage-v1-20261024';p=read(st/'provenance.json');verify(st,p['files'])
        assert checked(Path(p['tests']))['specialized']==p['specialized']
        assert sha(Path(p['tests'])/'run-state.json')==p['tests_state_sha256']
        peer=checked(Path(p['peer']));assert peer['specialized']==p['specialized']
        if p.get('pending_peer_at_staging') is not None:
            assert peer['files']==p['pending_peer_at_staging']['frozen_input_hashes']
            assert p['pending_peer_at_staging']['qualification_still_requires_terminal_pass']
        else:assert sha(Path(p['peer'])/'run-state.json')==p['peer_state_sha256']
        assert p['input_bits']==422 and p['result_bits']==4943 and not p['assembly_permitted']
        assert p['specialized']==(label=='specialized') and (st/'top.sv').read_text()==TOP
        fit,envelope,receipt=collection(f'owned-{label}-terminal-v1-20261024')
        assert read(fit/'provenance.json')==p
        state=read(fit/'run-state.json');assert state['status']=='TERMINAL_REVIEW_REQUIRED' and all(r['returncode']==0 for r in state['stages'])
        back=read(fit/'collected-input-check.json');assert back['job_pids_absent']==[state['builder_pid']]+[r['pid'] for r in state['stages']]
        for n,h in p['files'].items():
            assert back['actual'][n]==sha(fit/n)
            if sha(fit/n)!=h:assert n=='clock_fit.qsf' and (fit/n).read_bytes()==(st/n).read_bytes()+b'\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
        assert re.findall(r'OWNED_PACKET_OUTPUT_BITS (\d+)',(fit/'corners.log').read_text())==['4943']*4
        assert re.findall(r'OWNED_PACKET_INPUT_BITS (\d+)',(fit/'corners.log').read_text())==['422']*4
        corners=[]
        for c in CORNERS:
            row=dict(corner=c,setup=first_path(fit/f'clock-setup-{c}.rpt'),hold=first_path(fit/f'clock-hold-{c}.rpt'),evaluator=first_path(fit/f'clock-evaluator-{c}.rpt'))
            row['pulse_width_ns']=float(re.search(r'Worst case slack is (-?[0-9.]+)',(fit/f'clock-mpw-{c}.rpt').read_text())[1])
            for mode in ['recovery','removal']:assert 'Nothing to report.' in (fit/f'clock-{mode}-{c}.rpt').read_text()
            ucp=re.findall(r'; (?:Illegal Clocks|Unconstrained [^;]+)\s*;\s*(\d+)\s*;\s*(\d*)\s*;', (fit/f'clock-ucp-{c}.rpt').read_text())
            assert len(ucp)==6 and all(a=='0' and b in ['','0'] for a,b in ucp);corners.append(row)
        summary=(fit/'clock_fit.fit.summary').read_text();rpt=(fit/'clock_fit.fit.rpt').read_text(encoding='latin1')
        area=dict(ALMs=int(re.search(r'Logic utilization \(in ALMs\) : ([0-9,]+)',summary)[1].replace(',','')),
                  registers=int(re.search(r'Total registers : ([0-9,]+)',summary)[1].replace(',','')),
                  MLAB_bits=int(re.search(r'Total MLAB memory bits\s*;\s*([0-9,]+)',rpt)[1].replace(',','')))
        assert area['MLAB_bits']==9216
        fits.append(dict(variant=label,corners=corners,area=area,archive_sha256=receipt['archive_sha256'],members=len(receipt['members']),
                         pass_15ns=all(r['setup']['slack_ns']>=0 and r['hold']['slack_ns']>=0 and r['pulse_width_ns']>=0 for r in corners)))
        dirs.extend([st,fit]);envelopes.append(envelope);stages.append(st)
    # Prove these really are an equal-domain comparison. Apart from provenance
    # and the specialized cache module, all actual fitted sources are identical.
    a=read(stages[0]/'provenance.json')['files'];b=read(stages[1]/'provenance.json')['files'];assert a.keys()==b.keys()
    assert [n for n in a if a[n]!=b[n]]==['packet.sv']
    out.mkdir(parents=True)
    for d in dirs:retain_text(d,out/d.name)
    for p in envelopes:(out/p.name).write_bytes(p.read_bytes())
    result=dict(status='REVIEWED_SOURCE_BOUND_OWNED_PROCESSOR_PACKET',scope=__doc__,cache_bits_inductively_compared=1416,control_projection_bits=156,
        stored_closed_owners=828,external_R_aliases=16,powerup_owners_unconstrained=True,prefix_key_unchanged=True,refresh_check_retained=True,
        whole_final_root_preserved=True,XZ_context_cases=4984,XZ_external_cases=68,four_state_comparisons=40292,
        historical_racing_false_control_excluded=True,testbench_clock_race_and_declaration_failures_preserved=True,
        full_replays=4,original_transactions_per_replay=14884,commits=10423,pairs=4461,paired_writes=119,forwarded_bytes=238,
        all_original_result_state_RAM_clock_assertions_retained=True,invalid_geometries=403,before_root_negative_is_explicit_contract_violation=True,
        negative_controls=negatives,fit_input_bits=422,fit_result_bits=4943,equal_domain_fits=fits,
        same_fitted_sources_except_cache_specialization=True,variant_selected=False,native60ns_or_hardware_speed_qualified=False,
        source_CP_device_interior_epochs_qualified=False,hardware_changed=False,hardware_requires_user_release=True,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['output','archive']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--digital-only',action='store_true',help='Save the completed proof/four-state/Verilator milestone; Icarus and fits remain pending')
    a=p.parse_args();r=review(a.output.resolve(),a.digital_only)
    append_archive(a.archive.resolve(),a.output.resolve(),[],label='owned-packet-context-digital' if a.digital_only else 'owned-processor-packet',extra_tools=[
        'generate_owned_r_packet.py','prove_owned_r_packet.py','test_owned_r_context_four_state.py','test_owned_r_packet.py','fit_owned_r_packet.py',Path(__file__).name])
    print(r['status'],flush=True)
