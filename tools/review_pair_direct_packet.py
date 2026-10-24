#!/usr/bin/env python3
"""Preserve a safe pair-direct experiment without selecting a worse full fit.

Bind both950-bit control induction and1416-bit cache/ready/permission induction
to the single changed input expression. Preserve full original replay and all
corruption/geometry checks. The15ns result is rejected and does not demonstrate
improvement over the preceding entry-cache design. No native/hardware claim.
"""
import argparse
import json
from pathlib import Path
import re

from generate_cached_r_packet import packet,stage,cache
from review_baseboard_clock_consumer import collection,CORNERS
from review_exact_pixel_phase import retain_text
from review_late_hold_and_input_reset import first_path
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha
from test_cached_r_packet import bench

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def read(p):return json.loads(p.read_text())


def verify(d,files):
    for n,h in files.items():assert sha(d/n)==h,(d,n)


def review(out):
    assert not out.exists();dirs=[]
    for name,status,count in [('pair-direct-control-proof-v1-20261024','PASS_ENTRY_CACHED_PACKET_INDUCTION_AND_OWNER_PROJECTION',5),
                              ('pair-direct-cache-proof-v1-20261024','PASS_PAIR_DIRECT_CACHE_READY_AND_PERMISSION_INDUCTION',3)]:
        d=BUILD/name;s=read(d/'run-state.json');verify(d,s['files']);assert s['status']==status and len(s['cases'])==count
        for row in s['cases']:
            p=d/row['mode'];verify(p,row['files']);assert row['passed'] and row['returncode']==0 and sha(p/'proof.log')==row['log_sha256']
            if row['mode']=='induction':assert 'Induction step proven: SUCCESS!' in (p/'proof.log').read_text()
        if count==5:assert s['pair_direct'] and s['compared_control_bits_per_subevent']==475 and s['committed_root_history_checked']
        else:
            assert s['cache_state_bits']==1416 and s['all_unready_pair_result_differences_allowed']
            old=(d/'old-packet.sv').read_text();new=(d/'new-packet.sv').read_text()
            assert new.replace('.entry_fast((use_pair || entry_fast))','.entry_fast(entry_fast)',1)==old
        dirs.append(d)
    expected='transactions=14884 commits=10423 pairs=4461 candidates=4461 fallbacks=0 paired_writes=119 forwarded_bytes=238 Qs=844 RAM=9216 results=2470 clocks=100'
    counts=[]
    for engine in ['icarus','verilator']:
        d=BUILD/f'pair-direct-{engine}-v1-20261024';s=read(d/'run-state.json');verify(d,s['files'])
        assert s['status']=='PASS_ENTRY_CACHED_FULL_PROCESSOR_PACKETS' and s['pair_direct'] and s['mutation'] is None
        assert s['summary']==expected and s['cache_coverage']=='fast=8885 staged=1538 prepare_edges=1538 resets=1'
        assert all(v['returncode']==0 for v in s['actions']) and s['all100_clock_counts_match']
        parent=Path(s['source']);assert sha(parent/'run-state.json')==s['source_state_sha256']
        raw=(parent/'candidate.sv').read_text()
        assert (d/'packet.sv').read_text()==packet(raw,pair_direct=True)+'\n'+stage(raw)+'\n'+cache(raw)
        assert (d/'tb.sv').read_text()==bench((parent/'tb.sv').read_text())
        for row in s['source_files']:assert sha(d/row['path'])==row['output_sha256']==sha(parent/row['path'])
        assert 'PACKET_GEOMETRY_REJECTIONS cases=403' in (d/'test.log').read_text()
        counts.append(re.findall(r'^PACKET_CLOCK channel=(\d+) edges=(\d+)$',(d/'test.log').read_text(),re.M));dirs.append(d)
    assert len(counts[0])==100 and counts[0]==counts[1]
    for mutation,needle in [('early-write','PACKET invalid geometry committed bit=0'),('drop-forward','PACKET second result mismatch record=203'),
                            ('stale-intermediate','PACKET second result mismatch record=16'),('stale-root','CACHED_PACKET committed root tag mismatch record=2')]:
        d=BUILD/f'pair-direct-{mutation}-icarus-v1-20261024';s=read(d/'run-state.json');verify(d,s['files'])
        assert s['status']=='PASS_CACHED_PACKET_COUNTEREXAMPLE' and s['pair_direct'] and s['actions'][0]['returncode']==0 and s['actions'][-1]['returncode']!=0
        assert needle in (d/'test.log').read_text()
        raw=(d/'candidate.sv').read_text();assert (d/'packet.sv').read_text()==packet(raw,mutation,True)+'\n'+stage(raw)+'\n'+cache(raw,mutation=='stale-root');dirs.append(d)
    stage_dir=BUILD/'pair-direct-stage-v1-20261024';p=read(stage_dir/'provenance.json');verify(stage_dir,p['files'])
    assert p['pair_direct'] and p['equivalence_state_sha256']==sha(BUILD/'pair-direct-cache-proof-v1-20261024/run-state.json')
    assert p['input_bits']==1652 and p['result_bits']==4943 and not p['assembly_permitted']
    fit,envelope,receipt=collection('pair-direct-terminal-v1-20261024');assert len(receipt['members'])==86
    assert read(fit/'provenance.json')==p
    state=read(fit/'run-state.json');assert state['status']=='TERMINAL_REVIEW_REQUIRED' and all(v['returncode']==0 for v in state['stages'])
    back=read(fit/'collected-input-check.json');assert back['job_pids_absent']==[state['builder_pid']]+[v['pid'] for v in state['stages']]
    suffix=b'\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
    for n,h in p['files'].items():
        assert back['actual'][n]==sha(fit/n)
        if sha(fit/n)!=h:assert n=='clock_fit.qsf' and (fit/n).read_bytes()==(stage_dir/n).read_bytes()+suffix
    assert re.findall(r'CACHED_PACKET_OUTPUT_BITS (\d+)',(fit/'corners.log').read_text())==['4943']*4
    corners=[]
    for c in CORNERS:
        row=dict(corner=c,setup=first_path(fit/f'clock-setup-{c}.rpt'),hold=first_path(fit/f'clock-hold-{c}.rpt'),evaluator=first_path(fit/f'clock-evaluator-{c}.rpt'))
        row['pulse_width_ns']=float(re.search(r'Worst case slack is (-?[0-9.]+)',(fit/f'clock-mpw-{c}.rpt').read_text())[1])
        for mode in ['recovery','removal']:assert 'Nothing to report.' in (fit/f'clock-{mode}-{c}.rpt').read_text()
        ucp=re.findall(r'; (?:Illegal Clocks|Unconstrained [^;]+)\s*;\s*(\d+)\s*;\s*(\d*)\s*;', (fit/f'clock-ucp-{c}.rpt').read_text())
        assert len(ucp)==6 and all(a=='0' and b in ['','0'] for a,b in ucp);corners.append(row)
    summary=(fit/'clock_fit.fit.summary').read_text();area=dict(ALMs=int(re.search(r'Logic utilization \(in ALMs\) : ([0-9,]+)',summary)[1].replace(',','')),
        registers=int(re.search(r'Total registers : ([0-9,]+)',summary)[1].replace(',','')),
        MLAB_bits=int(re.search(r'Total MLAB memory bits\s*;\s*([0-9,]+)',(fit/'clock_fit.fit.rpt').read_text(encoding='latin1'))[1].replace(',','')))
    assert area==dict(ALMs=8675,registers=8434,MLAB_bits=9216)
    dirs.extend([stage_dir,fit]);out.mkdir(parents=True)
    for d in dirs:retain_text(d,out/d.name)
    (out/envelope.name).write_bytes(envelope.read_bytes())
    result=dict(status='REVIEWED_PAIR_DIRECT_SAFE_BUT_TIMING_NOT_IMPROVED',single_input_expression_change=True,ready_permission_and1416_cache_bits_inductively_equal=True,
        all950_control_bits_and156_owner_projection_proven=True,unready_pair_values_may_differ=True,prefix_capture_unchanged=True,
        original_replay_transactions=14884,commits=10423,pairs=4461,all2470_subevent_bits_844_Qs_9216_RAM_100clocks_retained=True,
        invalid_geometries=403,reset_checks_retained=True,all_four_processor_corruptions_rejected=True,
        component_corners=corners,component_area=area,component_15ns_pass=False,timing_gain_demonstrated=False,
        best_previous_setup_min_ns=-9.270,this_setup_min_ns=min(v['setup']['slack_ns'] for v in corners),
        variant_selected=False,source_and_device_timestamps_qualified=False,native60ns_or_hardware_speed_qualified=False,
        hardware_changed=False,hardware_requires_user_release=True,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path,required=True)
    a=p.parse_args();r=review(a.output.resolve());append_archive(a.archive.resolve(),a.output.resolve(),[],label='pair-direct-packet',extra_tools=[
        'generate_cached_r_packet.py','prove_cached_r_packet.py','prove_pair_direct_cache.py','test_cached_r_packet.py','fit_cached_r_packet.py',Path(__file__).name]);print(r['status'],flush=True)
