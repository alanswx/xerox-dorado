#!/usr/bin/env python3
"""Retain exact root geometry, sparse/fused control gates and native counters.

Preserve the stale-Q30 theorem failure, passive-counter compiler failure and
initially unrecognized corruption rejections. Scope the passing15ns budgets
to the certified control function, never complete844-Q/SRAM packet ownership.
The completed5B queue diagnostic is attribution, not a paired speed/boot pass.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re

from review_baseboard_clock_consumer import collection,CORNERS
from review_exact_pixel_phase import retain_text
from review_late_hold_and_input_reset import first_path
from review_r_front_capture_followup import append_archive
from test_control_fall_capture import transform,WRAPPER
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def read(p):return json.loads(p.read_text())


def verify(d,files):
    for name,digest in files.items():assert sha(d/name)==digest,(d,name)


def proof(d,status,count):
    s=read(d/'run-state.json');assert s['status']==status and len(s['cases'])==count
    verify(d,s['files'])
    for row in s['cases']:
        p=d/f"{row['mode']}-{row['variant']}-w{row['waves']}"
        verify(p,row['files']);assert sha(p/'proof.log')==row['log_sha256']
        if status.startswith('PASS_'):assert row['passed'] and row['returncode']==0
    return s


def review(out):
    assert not out.exists();dirs=[];envelopes=[]
    audit=BUILD/'control-root-packet-audit-v1-20261024';a=read(audit/'results.json');verify(audit/'inputs',a['files'])
    assert a['status']=='PASS_NATIVE_ROOT_BINDING_AND_PACKET_GEOMETRY' and a['intervals']==160000
    assert a['debug_schedules_checked']==1905792 and a['CPU_rotations']==8
    assert a['constant_debug_zero_edges']=={'0':20000,'1':120000,'2':20000}
    assert not a['actual_CP_or_debug_transition_rate_assumed'] and a['arbitrary_debug_requires_generic_fallback']
    assert a['source_tests_state_sha256']==sha(Path(a['source_tests'])/'run-state.json')
    dirs.append(audit)
    failed=BUILD/'control-fall-fusion-proof-v1-20261024'
    f=proof(failed,'TERMINAL_CONTROL_FALL_FUSION_NOT_ESTABLISHED',3)
    assert [r['passed'] for r in f['cases']]==[True,True,False]
    c=read(failed/'classification.json');assert c['different_Q_bits']==[30] and c['different_result_bits']==[237]
    assert c['incorrect_fused_Q30']==0 and c['serial_Q30']==1
    dirs.append(failed)
    proofs=[]
    for version in [2,3]:
        d=BUILD/f'control-fall-fusion-proof-v{version}-20261024'
        s=proof(d,'PASS_CERTIFIED_FALL_AND_PARALLEL_RISE_FULL_CONTROL',11)
        assert s['compared_bits_per_case']==363 and not s['state_or_input_bits_masked']
        assert s['extra_saved_state_or_guest_cycles']==0
        proofs.append((d,s));dirs.append(d)
    assert not proofs[0][1].get('defer_unused_parameter_elaboration',False) and proofs[1][1]['defer_unused_parameter_elaboration']
    # The faster frontend changes only when defaults are elaborated; check
    # every miter/source and requested parameter against the completed run.
    for x,y in zip(proofs[0][1]['cases'],proofs[1][1]['cases']):
        assert (x['mode'],x['variant'],x['waves'])==(y['mode'],y['variant'],y['waves'])
        name=f"{x['mode']}-{x['variant']}-w{x['waves']}"
        left=proofs[0][0]/name;right=proofs[1][0]/name
        for file in ['actual.sv','atomic.sv']:assert sha(left/file)==sha(right/file)
        assert '-defer' not in (left/'proof.ys').read_text() and '-defer' in (right/'proof.ys').read_text()
        assert (right/'top.sv').read_text().replace(f"parameter integer WAVES={x['waves']}",'parameter integer WAVES=5',1)==(left/'top.sv').read_text()
    helper=proofs[1][0]/'dorado_control_fall_step.sv'
    for engine in ['icarus','verilator']:
        for variant,mutation in [('capture',None),('bad-cram','wrong-cram'),('bad-clock','drop-clock')]:
            d=BUILD/f'control-fall-{variant}-{engine}-v2-20261024';s=read(d/'run-state.json')
            assert s['engine']==engine and s['mutation']==mutation and s['actions'][0]['returncode']==0
            assert s['proof_state_sha256']==sha(proofs[1][0]/'run-state.json')
            for row in s['source_files']:assert sha(d/row['path'])==row['output_sha256']
            source=Path(s['source']);raw=(source/'candidate.sv').read_text()
            assert (d/'candidate.sv').read_text()==transform(raw,helper.read_text(),mutation)
            tb=(d/'inputs/tb_r_front_local.sv').read_text()
            addition='''
 integer sparse_falls=0;
 always @(posedge host_clk)if(advance && cp_ready && dut.control.direct_arrival.using_sparse)sparse_falls=sparse_falls+1;
 final $display("CONTROL_FALL_COVERAGE captures=%0d",sparse_falls);
'''
            assert tb.replace(addition,'',1)==(source/'inputs/tb_r_front_local.sv').read_text()
            if mutation:
                assert s['actions'][-1]['returncode']!=0
                c=read(d/'negative-classification.json')
                assert c['status']=='PASS_CONTROL_FALL_ORIGINAL_REJECTION_CLASSIFICATION'
                assert c['run_state_sha256']==sha(d/'run-state.json') and c['test_log_sha256']==sha(d/'test.log')
                assert c['original_check']==(2 if mutation=='wrong-cram' else 14)
            else:
                assert s['status']=='PASS_CONTROL_FALL_COMPLETE_ORIGINAL_CAPTURE' and s['actions'][-1]['returncode']==0
                assert s['sparse_falls']==4975 and s['all100_clock_counts_match']
                log=(d/'test.log').read_text();old=(source/'test.log').read_text()
                assert 'R_FRONT_LOCAL_PROBE PASS checks=14884 writes=238 reads=6928 epochs=392' in log
                pattern=r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$'
                assert len(re.findall(pattern,log,re.M))==100 and re.findall(pattern,log,re.M)==re.findall(pattern,old,re.M)
                assert re.search(r'^CP_PREFIX_CACHE (.+)$',log,re.M)[1]==s['cache_coverage']==read(source/'run-state.json')['cache_coverage']
            dirs.append(d)
    parser=BUILD/'control-fall-capture-icarus-v1-20261024';s=read(parser/'run-state.json')
    assert s['actions'][0]['returncode']!=0 and 'declaration after use' in (parser/'compile.log').read_text()
    dirs.append(parser)
    old=BUILD/'control-fall-capture-verilator-v1-20261024';s=read(old/'run-state.json')
    assert s['status']=='PASS_CONTROL_FALL_COMPLETE_ORIGINAL_CAPTURE' and s['sparse_falls']==4975
    assert sha(old/'candidate.sv')==sha(BUILD/'control-fall-capture-verilator-v2-20261024/candidate.sv');dirs.append(old)
    for name,status in [('four-state','PASS_CONTROL_FALL_FOUR_STATE'),('bad-unknown','PASS_CONTROL_FALL_UNKNOWN_COUNTEREXAMPLE')]:
        d=BUILD/f'control-fall-{name}-v1-20261024';s=read(d/'run-state.json');verify(d,s['files'])
        assert s['status']==status and s['actions'][0]['returncode']==0
        assert s['test_log_sha256']==sha(d/'test.log')
        if name=='four-state':assert 'known=2 unknown=3824 inputs=956 outputs=363 waves=5/24' in (d/'test.log').read_text()
        else:assert s['actions'][-1]['returncode']!=0
        dirs.append(d)
    corners=[]
    for variant in ['serial','parallel']:
        stage=BUILD/f'control-fall-fusion-{variant}-stage-v1-20261024';p=read(stage/'provenance.json');verify(stage,p['files'])
        assert p['result_bits']==726 and p['intermediate_bits']==p['final_bits']==363 and p['input_bits']==555
        d,envelope,r=collection(f'control-fall-fusion-{variant}-terminal-v1-20261024')
        assert len(r['members'])==58 and read(d/'provenance.json')==p
        s=read(d/'run-state.json');assert s['status']=='TERMINAL_REVIEW_REQUIRED' and all(row['returncode']==0 for row in s['stages'])
        readback=read(d/'collected-input-check.json')
        assert readback['job_pids_absent']==[s['builder_pid']]+[row['pid'] for row in s['stages']]
        assert readback['changed_inputs']==['clock_fit.qsf']
        assert re.findall(r'CONTROL_FUSION_OUTPUT_BITS (\d+)',(d/'corners.log').read_text())==['726']*4
        results=[]
        for corner in CORNERS:
            row=dict(corner=corner)
            for mode in ['setup','hold']:row[mode]=first_path(d/('clock-'+mode+'-'+corner+'.rpt'))
            counts=re.findall(r'; (?:Illegal Clocks|Unconstrained [^;]+)\s*;\s*(\d+)\s*;\s*(\d*)\s*;',
                (d/('clock-ucp-'+corner+'.rpt')).read_text())
            assert len(counts)==6 and all(a=='0' and b in ['','0'] for a,b in counts)
            assert all(row[m]['slack_ns']>=0 for m in ['setup','hold']);results.append(row)
        corners.append(dict(variant=variant,results=results))
        dirs.extend([stage,d]);envelopes.append(envelope)
    for file in ['top.sv','clock.sdc','clock_fit.qsf','audit.tcl','atomic.sv','dorado_control_fall_step.sv']:
        assert sha(BUILD/'control-fall-fusion-serial-stage-v1-20261024'/file)==sha(BUILD/'control-fall-fusion-parallel-stage-v1-20261024'/file)
    queue=BUILD/'native-queue-bound-terminal-v1-20261024';q=read(queue/'results.json')
    assert q['status']=='PRESERVED_TERMINAL_NATIVE_128_GROUP_DIAGNOSTIC' and q['original_bound']==5000000000
    inv=json.loads(gzip.decompress((queue/'provenance.json.gz').read_bytes()));verify(queue/'inputs',inv['inputs'])
    assert len(inv['inputs'])==537 and q['totals']['accepted']==q['totals']['completed']==429085040
    assert q['enabled_machine_edges']==3050833907 and q['totals']['issues']==303561422
    assert q['unchanged_medium_sha256']=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    assert q['receive_packet_total_not_recorded'] and not q['pinned_workload_comparison_qualified']
    dirs.append(queue)
    out.mkdir(parents=True)
    for d in dirs:retain_text(d,out/d.name)
    for p in envelopes:(out/p.name).write_bytes(p.read_bytes())
    result=dict(status='REVIEWED_CONTROL_FALL_FUSION_AND_TERMINAL_QUEUE_ATTRIBUTION',
        source_intervals=160000,CPU_rotations=8,debug_schedules=1905792,arbitrary_debug_fallback_required=True,
        exact_control_bits=363,fresh_pure_endpoint_certificates_proven=True,stale_Q30_counterexample_preserved=True,
        binary_cases_per_frontend=11,frontend_elaboration_only_change_inverse_checked=True,
        full_capture_checks_per_engine=14884,all844Q_9216RAM_100clocks_retained=True,sparse_captures_per_engine=4975,
        original_cache_coverage_unchanged=True,corruptions_reject_at_original_checks=[2,14],
        failed_negative_result_recognition_preserved_and_classified=True,passive_counter_parser_failure_preserved=True,
        unknown_input_checks=3824,unknown_fallback_corruption_rejected=True,
        control_component_bits=726,component_corners=corners,terminal_native_queue=q,
        broader_CP_arrival_profiles_or_analog_pulse_timing_qualified=False,
        uncommitted_packet_cache_context_qualified=False,complete844Q_SRAM_packet_fusion_qualified=False,
        native_15ns_or_60ns_qualified=False,Cedar_Rollback_persistence_qualified=False,hardware_speed_qualified=False,
        hardware_changed=False,hardware_requires_user_release=True,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['output','archive']:p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();r=review(a.output.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),[],label='control-fall-fusion',extra_tools=[
        'prove_control_fall_fusion.py','audit_control_root_packets.py','test_control_fall_capture.py',
        'fit_control_fall_fusion.py','collect_native_storage_terminal.py',Path(__file__).name])
    print(r['status'],flush=True)
