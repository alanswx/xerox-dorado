#!/usr/bin/env python3
"""Review full parallel-fault processor, actual child proofs and complete fit.

Require all1517 binary inputs, every-input X/Z tests and bad controls. Bind
only SINGLE faults[1] substitution and every child argument; unchanged cache/
ready/owner/RAM cones reuse the actual split history theorem. Full original
replays and ALL4943 timing endpoints remain, with no exceptions or new cycles.
Preserve the unused-controller parser failure. No native/firmware/speed claim.
"""
import argparse
import json
from pathlib import Path
import re

from audit_parallel_r_fault import audit,evidence
from generate_parallel_r_fault import packet,helper
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
    failed=BUILD/'parallel-fault-proof-v1-20261024';s=read(failed/'run-state.json');verify(failed,s['files'])
    assert s['status']=='TERMINAL_PARALLEL_FAULT_NOT_ESTABLISHED' and len(s['cases'])==1
    r=s['cases'][0];verify(failed/'positive',r['files']);assert not r['passed'] and r['returncode']!=0
    assert sha(failed/'positive/proof.log')==r['log_sha256']
    assert 'SystemVerilog interface in module port list cannot have a default value' in (failed/'positive/proof.log').read_text()
    dirs.append(failed)
    fp=BUILD/'parallel-fault-proof-v2-20261024';fs=BUILD/'parallel-fault-four-state-v1-20261024';fu=BUILD/'parallel-fault-bad-unknown-v1-20261024'
    c=read(fp/'run-state.json');assert c['unused_packet_controller_module_file_excluded'] and not c['input_or_state_assumptions_added']
    # The frozen graph is identical; V2 excludes only the uninstantiated
    # enclosing packet/controller file from parsing. No child logic is changed.
    for n,h in s['files'].items():
        if n.startswith('inputs/'):assert c['files'][n]==h and sha(fp/n)==h
    ys=(fp/'positive/proof.ys').read_text();assert '/work/inputs/candidate.sv' not in ys and 'hierarchy -check -top parallel_fault_equivalence' in ys
    dirs.extend([fp,fs,fu])
    consumer=BUILD/'r-fault-consumer-audit-v1-20261024';s=read(consumer/'results.json');verify(consumer,s['files'])
    assert s['status']=='PASS_ORIGINAL_H_R_FAULT_CONSUMER_WIRING' and s['pins_checked']==20
    assert s['existing_architectural_k21_bits']==6 and s['new_state_added']==0 and not s['error_event_migration_qualified']
    assert s['first_capture']=='k21.D2(p7) -> Q2(p4)' and s['diagnostic_capture']=='k21.D3(p10)=old Q2 -> RmPerr(p13)'
    dirs.append(consumer)
    control=BUILD/'sparse-pair-control-proof-v2-20261024';p=proof(control,'PASS_ENTRY_CACHED_PACKET_INDUCTION_AND_OWNER_PROJECTION',7)
    assert p['sparse_pair'] and p['compared_control_bits_per_subevent']==475 and p['committed_root_history_checked']
    assert "assign equivalent=invariant && (!control_domain || different==950'b0);" in (control/'induction/top.sv').read_text();dirs.append(control)
    cp=BUILD/'split-pair-cache-proof-v1-20261024';p=proof(cp,'PASS_PAIR_DIRECT_CACHE_READY_AND_PERMISSION_INDUCTION',3)
    assert p['split_pair'] and p['cache_state_bits']==1416;dirs.append(cp)
    expected='transactions=14884 commits=10423 pairs=4461 candidates=4461 fallbacks=0 paired_writes=119 forwarded_bytes=238 Qs=844 RAM=9216 results=2470 clocks=100'
    counts=[];audits=[];qualifications=[]
    for engine in ['icarus','verilator']:
        d=BUILD/f'parallel-fault-packet-{engine}-v1-20261024';s=read(d/'run-state.json');verify(d,s['files'])
        assert s['status']=='PASS_ENTRY_CACHED_FULL_PROCESSOR_PACKETS' and s['packet_variant']=='split-parallel-fault' and s['mutation'] is None
        assert s['summary']==expected and s['cache_coverage']=='fast=8885 staged=1538 prepare_edges=1538 resets=1'
        assert all(v['returncode']==0 for v in s['actions']) and s['all100_clock_counts_match']
        parent=Path(s['source']);assert sha(parent/'run-state.json')==s['source_state_sha256'];raw=(parent/'candidate.sv').read_text()
        actual=(d/'packet.sv').read_text();assert actual==packet(raw)+'\n'+stage(raw,True)+'\n'+cache(raw)
        assert stage(raw,True)==(control/'controlled-processor.sv').read_text()==(cp/'controlled-stage.sv').read_text()
        audits.append(audit(raw,actual));qualifications.append(evidence(fp,fs,fu,d))
        assert module(actual,'dorado_r_front_packet_step').replace(' dorado_r_front_parallel_fault_step single(',
            ' dorado_r_front_prefix_step single(',1)==module((cp/'new-packet.sv').read_text(),'dorado_r_front_packet_step')
        assert (d/'tb.sv').read_text()==bench((parent/'tb.sv').read_text())
        for row in s['source_files']:assert sha(d/row['path'])==row['output_sha256']==sha(parent/row['path'])
        assert 'PACKET_GEOMETRY_REJECTIONS cases=403' in (d/'test.log').read_text()
        counts.append(re.findall(r'^PACKET_CLOCK channel=(\d+) edges=(\d+)$',(d/'test.log').read_text(),re.M));dirs.append(d)
    assert len(counts[0])==100 and counts[0]==counts[1] and audits[0]==audits[1] and qualifications[0]==qualifications[1]
    selector=BUILD/'sparse-pair-selector-proof-v1-20261024';s=read(selector/'run-state.json');verify(selector,s['files'])
    assert s['status']=='PASS_CURRENT_SPARSE_FIRST_SELECTOR_PROJECTION' and len(s['cases'])==3
    names=['dorado_hold_selector_preview','dorado_control_hold_ordered_step','dorado_control_hold_atomic_step',
           'dorado_control_terminal_body','dorado_control_certified_fall_step','dorado_control_fall_step']
    assert (selector/'actual-source.sv').read_text()=='\n'.join(module(raw,n) for n in names)
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
                                  ('drop-fall-clock','verilator','PACKET coverage incomplete pairs=70 writes=0 forward=0'),
                                  ('invert-low','verilator','PACKET first result mismatch record=129'),
                                  ('stale-low','verilator','PACKET first result mismatch record=7734')]:
        d=BUILD/f'parallel-fault-{mutation}-{engine}-v1-20261024';s=read(d/'run-state.json');verify(d,s['files'])
        assert s['status']=='PASS_CACHED_PACKET_COUNTEREXAMPLE' and s['packet_variant']=='split-parallel-fault'
        assert s['actions'][0]['returncode']==0 and s['actions'][-1]['returncode']!=0 and needle in (d/'test.log').read_text()
        raw=(d/'candidate.sv').read_text()
        assert (d/'packet.sv').read_text()==packet(raw,mutation)+'\n'+stage(raw,True,mutation)+'\n'+cache(raw,mutation=='stale-root')
        negatives.append(dict(mutation=mutation,engine=engine,original_assertion=needle));dirs.append(d)
    st=BUILD/'parallel-fault-stage-v1-20261024';p=read(st/'provenance.json');verify(st,p['files'])
    assert p['packet_variant']=='split-parallel-fault' and p['equivalence_state_sha256']==sha(cp/'run-state.json')
    assert p['fault_qualification']==qualifications[0] and p['fault_audit']==audits[0]
    assert p['input_bits']==1652 and p['result_bits']==4943 and not p['assembly_permitted']
    fit,envelope,receipt=collection('parallel-fault-terminal-v1-20261024');assert len(receipt['members'])==86 and read(fit/'provenance.json')==p
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
    worst=(fit/'clock-setup-7_slow_1100mv_-40c.rpt').read_text().split('Path #1:',1)[1].split('Path #2:',1)[0]
    assert all(n in worst for n in ['canonical_pins[151]','entry_cache|fast_path','single|control|direct_arrival',
                                   'single|shared_state_mismatch','last_result[954]'])
    assert re.search(r'; Data Delay\s*;\s*16.321\s*;',worst)
    assert area['MLAB_bits']==9216;dirs.extend([st,fit]);out.mkdir(parents=True)
    for d in dirs:retain_text(d,out/d.name)
    (out/envelope.name).write_bytes(envelope.read_bytes())
    result=dict(status='REVIEWED_PARALLEL_R_FAULT_FULL_PROCESSOR',field_only_source_audit=audits[0],child_qualification=qualifications[0],
        arbitrary_binary_input_bits=1517,protocol_or_state_assumptions_added=False,four_state_known_checks=8,four_state_XZ_checks=12136,unknown_selector_checks=8,
        stale_and_inverted_child_controls_have_counterexamples=True,unused_controller_parser_failure_preserved=True,
        original_R_fault_capture_wiring_audited=True,error_capture_event_kernel_still_unqualified=True,
        actual_control_selector_and1416_bit_cache_history_proofs_reused_by_unchanged_cones=True,
        full_original_replay_transactions=14884,commits=10423,pairs=4461,all2470_subevent_bits_844_Qs_9216_RAM_100clocks_retained=True,
        invalid_geometries=403,reset_checks_retained=True,negative_controls=negatives,component_corners=corners,component_area=area,
        component_15ns_pass=all(v['setup']['slack_ns']>=0 and v['hold']['slack_ns']>=0 and v['pulse_width_ns']>=0 for v in corners),
        best_previous_setup_min_ns=-2.538,this_setup_min_ns=min(v['setup']['slack_ns'] for v in corners),
        worst_path_data_delay_ns=16.321,remaining_worst_path='entry canonical-context equality through SINGLE shared-state consistency result',
        next_step='Source-bound common owner/pin/root publication and canonical-match specialization; retain generic prefix key and all checks',
        variant_selected=False,source_and_device_timestamps_qualified=False,native60ns_or_hardware_speed_qualified=False,
        hardware_changed=False,hardware_requires_user_release=True,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path,required=True)
    a=p.parse_args();r=review(a.output.resolve());append_archive(a.archive.resolve(),a.output.resolve(),[],label='parallel-r-fault',extra_tools=[
        'generate_parallel_r_fault.py','audit_parallel_r_fault.py','audit_r_fault_consumer.py','prove_parallel_r_fault.py','test_parallel_r_fault_four_state.py','test_parallel_r_packet.py',
        'generate_cached_r_packet.py','generate_split_r_packet.py','audit_split_r_packet.py','test_cached_r_packet.py','fit_cached_r_packet.py',Path(__file__).name]);print(r['status'],flush=True)
