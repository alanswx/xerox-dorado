#!/usr/bin/env python3
"""Review complete split processor functions, cache history and full timing.

Bind both old/new first-call input tuples, unchanged second/ready/one-RAM
routing, geometry-wide control theorem and actual selector theorem. Keep every
original processor result, memory, clock, geometry and reset check. Full15ns
reports include ALL4943 outputs and cache/SRAM enables, with no exceptions.
No native clock/source-device timestamp, firmware or speed qualification.
"""
import argparse
import json
from pathlib import Path
import re

from audit_split_r_packet import audit
from generate_split_r_packet import packet
from generate_cached_r_packet import stage,cache
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import collection,CORNERS
from review_exact_pixel_phase import retain_text
from review_late_hold_and_input_reset import first_path
from review_pair_direct_packet import read,verify,BUILD
from review_r_front_capture_followup import append_archive
from review_sparse_pair_packet import proof
from test_cached_r_packet import bench
from test_r_front_forward_view import sha


def review(out):
    assert not out.exists();dirs=[]
    control=BUILD/'sparse-pair-control-proof-v2-20261024'
    c=proof(control,'PASS_ENTRY_CACHED_PACKET_INDUCTION_AND_OWNER_PROJECTION',7)
    assert c['sparse_pair'] and c['pair_direct'] and c['compared_control_bits_per_subevent']==475 and c['committed_root_history_checked']
    top=(control/'induction/top.sv').read_text()
    assert "wire control_domain=entry_ready && (!use_pair ||\n  (fast && before_root && after_root && before_pins==after_pins));" in top
    assert "assign equivalent=invariant && (!control_domain || different==950'b0);" in top
    dirs.append(control)
    cache_proof=BUILD/'split-pair-cache-proof-v1-20261024'
    c=proof(cache_proof,'PASS_PAIR_DIRECT_CACHE_READY_AND_PERMISSION_INDUCTION',3)
    assert c['split_pair'] and c['sparse_pair'] and not c['source_bound_input_substitution_only']
    assert c['cache_state_bits']==1416 and c['all_unready_pair_result_differences_allowed']
    top=(cache_proof/'induction/top.sv').read_text()
    assert 'geometry=before_root && committed_root && !first_root && before_pins==after_pins' in top
    assert 'input wire [400:0] before_pins,after_pins' in top
    dirs.append(cache_proof)
    expected='transactions=14884 commits=10423 pairs=4461 candidates=4461 fallbacks=0 paired_writes=119 forwarded_bytes=238 Qs=844 RAM=9216 results=2470 clocks=100'
    counts=[];substitutions=[]
    for engine in ['icarus','verilator']:
        d=BUILD/f'split-pair-{engine}-v1-20261024';s=read(d/'run-state.json');verify(d,s['files'])
        assert s['status']=='PASS_ENTRY_CACHED_FULL_PROCESSOR_PACKETS' and s['packet_variant']=='split-pair' and s['sparse_pair'] and s['mutation'] is None
        assert s['summary']==expected and s['cache_coverage']=='fast=8885 staged=1538 prepare_edges=1538 resets=1'
        assert all(v['returncode']==0 for v in s['actions']) and s['all100_clock_counts_match']
        parent=Path(s['source']);assert sha(parent/'run-state.json')==s['source_state_sha256'];raw=(parent/'candidate.sv').read_text()
        actual=packet(raw)
        assert (d/'packet.sv').read_text()==actual+'\n'+stage(raw,True)+'\n'+cache(raw)
        assert actual==(cache_proof/'new-packet.sv').read_text()
        assert stage(raw,True)==(control/'controlled-processor.sv').read_text()==(cache_proof/'controlled-stage.sv').read_text()
        substitutions.append(audit(raw,actual))
        for old,new in [(".clock_before(1'b1)",".clock_before(1'b0)"),('.held_q(first_held_next)','.held_q(held_q)'),
                        ('owned_port_address=use_pair ? first_port_address:single_port_address','owned_port_address=use_pair ? single_port_address:first_port_address')]:
            assert actual.count(old)==1
            try:audit(raw,actual.replace(old,new,1))
            except AssertionError:pass
            else:raise AssertionError('input/owner audit corruption escaped')
        assert (d/'tb.sv').read_text()==bench((parent/'tb.sv').read_text())
        for row in s['source_files']:assert sha(d/row['path'])==row['output_sha256']==sha(parent/row['path'])
        assert 'PACKET_GEOMETRY_REJECTIONS cases=403' in (d/'test.log').read_text()
        counts.append(re.findall(r'^PACKET_CLOCK channel=(\d+) edges=(\d+)$',(d/'test.log').read_text(),re.M));dirs.append(d)
    assert len(counts[0])==100 and counts[0]==counts[1] and substitutions[0]==substitutions[1]
    selector=BUILD/'sparse-pair-selector-proof-v1-20261024';s=read(selector/'run-state.json');verify(selector,s['files'])
    assert s['status']=='PASS_CURRENT_SPARSE_FIRST_SELECTOR_PROJECTION'
    assert [(v['mode'],v['waves']) for v in s['cases']]==[('positive',5),('positive',24),('wrong-selector',5)]
    parent=Path(s['source']);assert sha(parent/'run-state.json')==s['source_state_sha256']
    raw=(parent/'candidate.sv').read_text();names=['dorado_hold_selector_preview','dorado_control_hold_ordered_step','dorado_control_hold_atomic_step',
        'dorado_control_terminal_body','dorado_control_certified_fall_step','dorado_control_fall_step']
    current=(BUILD/'split-pair-verilator-v1-20261024/candidate.sv').read_text()
    assert (selector/'actual-source.sv').read_text()=='\n'.join(module(current,n) for n in names)=='\n'.join(module(raw,n) for n in names)
    for row in s['cases']:
        d=selector/f"{row['mode']}-w{row['waves']}";verify(d,row['files'])
        assert row['passed'] and row['returncode']==0 and sha(d/'proof.log')==row['log_sha256']
    dirs.append(selector)
    negatives=[]
    for mutation,engine,needle in [('early-write','icarus','PACKET invalid geometry committed bit=0'),
                                  ('drop-forward','icarus','PACKET second result mismatch record=203'),
                                  ('stale-intermediate','icarus','PACKET second result mismatch record=16'),
                                  ('stale-root','icarus','CACHED_PACKET committed root tag mismatch record=2'),
                                  ('wrong-fall-cram','verilator','PACKET coverage incomplete pairs=0 writes=0 forward=0'),
                                  ('drop-fall-clock','verilator','PACKET coverage incomplete pairs=70 writes=0 forward=0')]:
        d=BUILD/f'split-pair-{mutation}-{engine}-v1-20261024';s=read(d/'run-state.json');verify(d,s['files'])
        assert s['status']=='PASS_CACHED_PACKET_COUNTEREXAMPLE' and s['packet_variant']=='split-pair'
        assert s['actions'][0]['returncode']==0 and s['actions'][-1]['returncode']!=0 and needle in (d/'test.log').read_text()
        raw=(d/'candidate.sv').read_text()
        assert (d/'packet.sv').read_text()==packet(raw,mutation)+'\n'+stage(raw,True,mutation)+'\n'+cache(raw,mutation=='stale-root')
        negatives.append(dict(mutation=mutation,engine=engine,original_assertion=needle));dirs.append(d)
    st=BUILD/'split-pair-stage-v1-20261024';p=read(st/'provenance.json');verify(st,p['files'])
    assert p['packet_variant']=='split-pair' and p['sparse_pair'] and p['equivalence_state_sha256']==sha(cache_proof/'run-state.json')
    assert p['input_bits']==1652 and p['result_bits']==4943 and not p['assembly_permitted']
    fit,envelope,receipt=collection('split-pair-terminal-v1-20261024');assert len(receipt['members'])==86 and read(fit/'provenance.json')==p
    state=read(fit/'run-state.json');assert state['status']=='TERMINAL_REVIEW_REQUIRED' and all(v['returncode']==0 for v in state['stages'])
    back=read(fit/'collected-input-check.json');assert back['job_pids_absent']==[state['builder_pid']]+[v['pid'] for v in state['stages']]
    suffix=b'\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
    for n,h in p['files'].items():
        assert back['actual'][n]==sha(fit/n)
        if sha(fit/n)!=h:assert n=='clock_fit.qsf' and (fit/n).read_bytes()==(st/n).read_bytes()+suffix
    assert re.findall(r'CACHED_PACKET_OUTPUT_BITS (\d+)',(fit/'corners.log').read_text())==['4943']*4
    corners=[]
    for c in CORNERS:
        row=dict(corner=c,setup=first_path(fit/f'clock-setup-{c}.rpt'),hold=first_path(fit/f'clock-hold-{c}.rpt'),evaluator=first_path(fit/f'clock-evaluator-{c}.rpt'))
        row['pulse_width_ns']=float(re.search(r'Worst case slack is (-?[0-9.]+)',(fit/f'clock-mpw-{c}.rpt').read_text())[1])
        for mode in ['recovery','removal']:assert 'Nothing to report.' in (fit/f'clock-{mode}-{c}.rpt').read_text()
        ucp=re.findall(r'; (?:Illegal Clocks|Unconstrained [^;]+)\s*;\s*(\d+)\s*;\s*(\d*)\s*;', (fit/f'clock-ucp-{c}.rpt').read_text())
        assert len(ucp)==6 and all(a=='0' and b in ['','0'] for a,b in ucp);corners.append(row)
    summary=(fit/'clock_fit.fit.summary').read_text()
    area=dict(ALMs=int(re.search(r'Logic utilization \(in ALMs\) : ([0-9,]+)',summary)[1].replace(',','')),
              registers=int(re.search(r'Total registers : ([0-9,]+)',summary)[1].replace(',','')),
              MLAB_bits=int(re.search(r'Total MLAB memory bits\s*;\s*([0-9,]+)',(fit/'clock_fit.fit.rpt').read_text(encoding='latin1'))[1].replace(',','')))
    assert area['MLAB_bits']==9216;dirs.extend([st,fit]);out.mkdir(parents=True)
    for d in dirs:retain_text(d,out/d.name)
    (out/envelope.name).write_bytes(envelope.read_bytes())
    result=dict(status='REVIEWED_SPLIT_FULL_PROCESSOR_PACKET',accepted_invocation_input_substitution=substitutions[0],
        input_owner_audit_corruptions_rejected=3,all950_control_bits_proven_throughout_geometry=True,
        actual_current_selector_bypass_proven_at5_and24waves=True,ready_permissions_and1416_cache_bits_inductively_equal=True,
        full_original_replay_transactions=14884,commits=10423,pairs=4461,all2470_subevent_bits_844_Qs_9216_RAM_100clocks_retained=True,
        invalid_geometries=403,reset_checks_retained=True,negative_controls=negatives,component_corners=corners,component_area=area,
        component_15ns_pass=all(v['setup']['slack_ns']>=0 and v['hold']['slack_ns']>=0 and v['pulse_width_ns']>=0 for v in corners),
        best_previous_setup_min_ns=-9.270,this_setup_min_ns=min(v['setup']['slack_ns'] for v in corners),
        variant_selected=False,source_and_device_timestamps_qualified=False,native60ns_or_hardware_speed_qualified=False,
        hardware_changed=False,hardware_requires_user_release=True,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path,required=True)
    a=p.parse_args();r=review(a.output.resolve());append_archive(a.archive.resolve(),a.output.resolve(),[],label='split-processor-packet',extra_tools=[
        'generate_cached_r_packet.py','generate_split_r_packet.py','audit_split_r_packet.py','prove_cached_r_packet.py','prove_pair_direct_cache.py',
        'prove_sparse_pair_selectors.py','test_cached_r_packet.py','test_split_r_packet.py','fit_cached_r_packet.py',Path(__file__).name]);print(r['status'],flush=True)
