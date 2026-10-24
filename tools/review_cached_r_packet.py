#!/usr/bin/env python3
"""Retain source-bound entry-cache safety, complete replay and full timing.

Keep the weaker stale-root calibration that found no bounded counterexample,
its stronger committed-context replacement and the unrecognized early-write
rejection. Retain the terminal original baseline with a source-derived final
counter boundary. Neither timing improvement nor unlike native workloads
establishes Dorado speed or closes any remaining full-goal requirement.
"""
import argparse
import gzip
import json
from pathlib import Path
import re

from generate_cached_r_packet import packet,stage,cache
from review_baseboard_clock_consumer import collection,CORNERS
from review_exact_pixel_phase import retain_text
from review_late_hold_and_input_reset import first_path
from review_r_front_capture_followup import append_archive
from test_cached_r_packet import bench
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def read(p):return json.loads(p.read_text())


def verify(d,files):
    for n,h in files.items():assert sha(d/n)==h,(d,n)


def review(out):
    assert not out.exists();dirs=[];models=[]
    for version in [1,2]:
        d=BUILD/f'cached-packet-proof-v{version}-20261024';s=read(d/'run-state.json');verify(d,s['files'])
        assert s['physical_projection_bits']==156 and s['processor_owner_bits']==844 and s['compared_control_bits_per_subevent']==475
        if version==1:
            assert s['status']=='TERMINAL_CACHED_PACKET_PROOF_NOT_ESTABLISHED' and len(s['cases'])==4
            assert [v['passed'] for v in s['cases']]==[True,True,True,False]
            assert s['cases'][-1]['returncode']!=0 and 'no model found: SUCCESS!' in (d/'stale-root/proof.log').read_text()
        else:
            assert s['status']=='PASS_ENTRY_CACHED_PACKET_INDUCTION_AND_OWNER_PROJECTION' and len(s['cases'])==5
            assert s['committed_root_history_checked'] and all(v['passed'] and v['returncode']==0 for v in s['cases'])
        for row in s['cases']:
            p=d/row['mode'];verify(p,row['files']);assert sha(p/'proof.log')==row['log_sha256']
            if row['mode']=='induction':assert 'Induction step proven: SUCCESS!' in (p/'proof.log').read_text()
        dirs.append(d)
    positive=[]
    expected='transactions=14884 commits=10423 pairs=4461 candidates=4461 fallbacks=0 paired_writes=119 forwarded_bytes=238 Qs=844 RAM=9216 results=2470 clocks=100'
    tag_checks='''
   if(dut.entry_cache.canonical_root!==ca)$fatal(1,"CACHED_PACKET committed root tag mismatch record=%0d",idx);
   if(dut.entry_cache.canonical_pins!==pa[397:0])$fatal(1,"CACHED_PACKET committed pin tag mismatch record=%0d",idx);
'''
    for name in ['cached-packet-icarus-v1-20261024','cached-packet-verilator-v1-20261024','cached-packet-verilator-v2-20261024']:
        d=BUILD/name;s=read(d/'run-state.json');verify(d,s['files'])
        assert s['status']=='PASS_ENTRY_CACHED_FULL_PROCESSOR_PACKETS' and s['summary']==expected and all(v['returncode']==0 for v in s['actions'])
        assert s['cache_coverage']=='fast=8885 staged=1538 prepare_edges=1538 resets=1' and s['all100_clock_counts_match']
        parent=Path(s['source']);assert sha(parent/'run-state.json')==s['source_state_sha256']
        raw=(parent/'candidate.sv').read_text()
        assert (d/'packet.sv').read_text()==packet(raw)+'\n'+stage(raw)+'\n'+cache(raw)
        text=bench((parent/'tb.sv').read_text())
        if '-v1-' in name:text=text.replace(tag_checks,'',1)
        assert (d/'tb.sv').read_text()==text
        for row in s['source_files']:assert sha(d/row['path'])==row['output_sha256']==sha(parent/row['path'])
        log=(d/'test.log').read_text();assert sha(d/'test.log')==s['test_log_sha256'] and 'PACKET_GEOMETRY_REJECTIONS cases=403' in log
        positive.append(re.findall(r'^PACKET_CLOCK channel=(\d+) edges=(\d+)$',log,re.M));dirs.append(d)
    assert len(positive[0])==100 and positive[0]==positive[1]==positive[2]
    for mutation,version,check,message in [('drop-forward',1,203,'PACKET second result mismatch'),('stale-intermediate',1,16,'PACKET second result mismatch'),
                                         ('stale-root',2,2,'CACHED_PACKET committed root tag mismatch'),('early-write',1,None,'PACKET invalid geometry committed bit=0')]:
        d=BUILD/f'cached-packet-{mutation}-icarus-v{version}-20261024';s=read(d/'run-state.json');verify(d,s['files'])
        assert s['actions'][0]['returncode']==0 and s['actions'][-1]['returncode']!=0
        log=(d/'test.log').read_text();assert message+(f' record={check}' if check is not None else '') in log
        if mutation=='early-write':
            assert s['status']=='TERMINAL_CACHED_PACKET_NOT_ESTABLISHED'
            c=read(d/'negative-classification.json');assert c['status']=='PASS_CACHED_PACKET_ORIGINAL_GEOMETRY_REJECTION'
            assert c['run_state_sha256']==sha(d/'run-state.json') and c['test_log_sha256']==sha(d/'test.log')
        else:assert s['status']=='PASS_CACHED_PACKET_COUNTEREXAMPLE'
        raw=(d/'candidate.sv').read_text()
        assert (d/'packet.sv').read_text()==packet(raw,mutation)+'\n'+stage(raw)+'\n'+cache(raw,mutation=='stale-root');dirs.append(d)
    stage_dir=BUILD/'cached-packet-stage-v1-20261024';p=read(stage_dir/'provenance.json');verify(stage_dir,p['files'])
    assert p['input_bits']==1652 and p['result_bits']==4943 and p['actual_entry_cache_and_auxiliary_state_retained']
    fit,envelope,receipt=collection('cached-packet-terminal-v1-20261024');assert len(receipt['members'])==86
    assert read(fit/'provenance.json')==p
    state=read(fit/'run-state.json');assert state['status']=='TERMINAL_REVIEW_REQUIRED' and all(v['returncode']==0 for v in state['stages'])
    readback=read(fit/'collected-input-check.json');assert readback['job_pids_absent']==[state['builder_pid']]+[v['pid'] for v in state['stages']]
    annotation=b'\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
    for name,digest in p['files'].items():
        assert readback['actual'][name]==sha(fit/name)
        if sha(fit/name)!=digest:
            assert name=='clock_fit.qsf' and (fit/name).read_bytes()==(stage_dir/name).read_bytes()+annotation
    assert re.findall(r'CACHED_PACKET_OUTPUT_BITS (\d+)',(fit/'corners.log').read_text())==['4943']*4
    corners=[]
    for corner in CORNERS:
        row=dict(corner=corner,setup=first_path(fit/f'clock-setup-{corner}.rpt'),hold=first_path(fit/f'clock-hold-{corner}.rpt'),
            evaluator=first_path(fit/f'clock-evaluator-{corner}.rpt'))
        row['pulse_width_ns']=float(re.search(r'Worst case slack is (-?[0-9.]+)',(fit/f'clock-mpw-{corner}.rpt').read_text())[1])
        for mode in ['recovery','removal']:assert 'Nothing to report.' in (fit/f'clock-{mode}-{corner}.rpt').read_text()
        ucp=re.findall(r'; (?:Illegal Clocks|Unconstrained [^;]+)\s*;\s*(\d+)\s*;\s*(\d*)\s*;', (fit/f'clock-ucp-{corner}.rpt').read_text())
        assert len(ucp)==6 and all(a=='0' and b in ['','0'] for a,b in ucp);corners.append(row)
    summary=(fit/'clock_fit.fit.summary').read_text();area=dict(ALMs=int(re.search(r'Logic utilization \(in ALMs\) : ([0-9,]+)',summary)[1].replace(',','')),
        registers=int(re.search(r'Total registers : ([0-9,]+)',summary)[1].replace(',','')),
        MLAB_bits=int(re.search(r'Total MLAB memory bits\s*;\s*([0-9,]+)',(fit/'clock_fit.fit.rpt').read_text(encoding='latin1'))[1].replace(',','')))
    assert area==dict(ALMs=8856,registers=8416,MLAB_bits=9216)
    dirs.extend([stage_dir,fit])
    baseline=BUILD/'native-baseline-bound-terminal-v1-20261024';b=read(baseline/'results.json')
    assert b['status']=='PRESERVED_TERMINAL_NATIVE_128_GROUP_DIAGNOSTIC' and b['original_bound']==5000000000
    inputs=json.loads(gzip.decompress((baseline/'provenance.json.gz').read_bytes()));verify(baseline/'inputs',inputs['inputs'])
    assert len(inputs['inputs'])==535 and b['original_handles_absent']==[57967,58600]
    assert b['totals']['accepted']==b['totals']['completed']==388029729 and b['totals']['issues']==283270639
    assert b['enabled_machine_edges']==2888495264 and b['final_enabled_step_at_stop']==0
    assert b['unchanged_medium_sha256']=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    boundary=BUILD/'native-baseline-counter-boundary-audit-v1-20261024';c=read(boundary/'classification.json')
    assert c['final_enabled_step']==0 and c['original_queue_case_final_enabled_step']==1 and sha(boundary/'original-reader.py')==c['original_reader_sha256']
    dirs.extend([baseline,boundary])
    out.mkdir(parents=True)
    for d in dirs:retain_text(d,out/d.name)
    (out/envelope.name).write_bytes(envelope.read_bytes())
    result=dict(status='REVIEWED_ENTRY_CACHED_PROCESSOR_PACKETS',processor_Qs=844,control_projection_bits=156,inducted_control_bits_per_subevent=475,
        unbounded_safety_and_committed_context_proven=True,weaker_stale_root_five_step_calibration_preserved=True,
        replay_transactions=14884,commits=10423,pairs=4461,all2470_bits_both_subevents_and9216_RAM_verified=True,
        cache_coverage='fast=8885 staged=1538 prepare_edges=1538 resets=1',invalid_geometries=403,
        corrupted_intermediate_and_forwarding_rejected=True,corrupted_root_tag_rejected=True,
        early_write_original_rejection_preserved_and_classified=True,component_corners=corners,component_area=area,
        component_15ns_pass=all(v[k]['slack_ns']>=0 for v in corners for k in ['setup','hold']) and all(v['pulse_width_ns']>=0 for v in corners),
        original_terminal_baseline=b,final_counter_boundary_source_derived=True,
        source_device_timestamps_or_native60ns_qualified=False,pinned_storage_speed_gain_qualified=False,
        hardware_speed_qualified=False,hardware_changed=False,hardware_requires_user_release=True,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path,required=True)
    a=p.parse_args();r=review(a.output.resolve());append_archive(a.archive.resolve(),a.output.resolve(),[],label='cached-processor-packet',extra_tools=[
        'generate_cached_r_packet.py','prove_cached_r_packet.py','test_cached_r_packet.py','fit_cached_r_packet.py',
        'collect_native_storage_terminal.py',Path(__file__).name]);print(r['status'],flush=True)
