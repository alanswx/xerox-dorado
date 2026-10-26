#!/usr/bin/env python3
"""Review full prepared-Hold caller histories and completed synthesis evidence.

Words-only traces legitimately publish only Hold1. Check all original results
and the full publication count, with both Hold values required across the suite.
Retain the original overstrict driver verdict; never rewrite a running or
completed test. A live Icarus process remains pending, not a passing replay.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess
import time

from replay_native_prepared_hold import bench
from replay_r_read_buffer import outcomes
from review_baseboard_clock_consumer import verify
from review_native_alu_capture import retain
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'


def review(out):
    assert not out.exists()
    fixture=BUILD/'native-hold-replay-fixtures-v1-20261025';f=json.loads((fixture/'results.json').read_text());verify(fixture,f['files'])
    stage=BUILD/'native-prepared-hold-stage-v1-20261025';m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    mapping=BUILD/'native-prepared-hold-map-review-v1-20261025';mp=json.loads((mapping/'results.json').read_text());verify(mapping,mp['files'])
    assert mp['status']=='COLLECTED_PREPARED_HOLD_NATIVE_MAPPING_AND_SAME_JOB_STATE'
    assert mp['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')and mp['same_builder']['builder_pid']==1458513
    assert (mp['synthesis_registers_before'],mp['synthesis_registers_after'])==(50362,50494)
    assert mp['memory_bits_before']==mp['memory_bits_after']==2511686
    rows=[];dirs=[mapping,fixture,BUILD/'native-prepared-hold-full-verilator-v1-20261025']
    observed_values=set()
    for mode in ['full','words']:
        for engine in ['verilator','icarus']:
            run=BUILD/f'native-prepared-hold-{mode}-{engine}-v2-20261025';dirs.append(run)
            r=json.loads((run/'run-state.json').read_text());verify(run,r['files'])
            assert r['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')and r['fixtures_receipt_sha256']==sha(fixture/'results.json')
            source=Path(r['source']);s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
            assert r['source_state_sha256']==sha(source/'run-state.json')and r['original_assertions_reference_and_recorded_inputs_retained']
            assert bench((source/'tb.sv').read_text(),source,run,f['fixtures_hex'])==(run/'tb.sv').read_text()
            for item in r['source_files']:assert sha(run/item['path'])==item['output_sha256']
            assert sha(run/'caller.sv')==sha(source/'caller.sv')and sha(run/'candidate.sv')==sha(source/'candidate.sv')
            assert sha(run/'hold-fixture/buffer.sv')==sha(stage/'src/verilog/rtl/dorado_memc_hold_buffer.sv')
            row=dict(mode=mode,engine=engine,directory=str(run),driver_status=r['status'],driver_state_sha256=sha(run/'run-state.json'))
            if r['status']=='RUNNING':
                assert r['actions'][-1]['label']=='test'and r['actions'][0]['returncode']==0
                pid=r['actions'][-1]['pid'];ps=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,time=,stat=,args='],text=True,capture_output=True)
                assert ps.returncode==0 and str(run/'test.vvp')in ps.stdout
                row.update(status='VERIFIED_RUNNING_PENDING',pid=pid,process=ps.stdout.strip(),observed=time.time())
            else:
                assert len(r['actions'])==2 and all(a['returncode']==0 for a in r['actions'])
                for a in r['actions']:assert sha(run/(a['label']+'.log'))==a['log_sha256']
                original=(source/'test.log').read_text();log=(run/'test.log').read_text()
                assert outcomes(log)==outcomes(original)and not any(x in log for x in ['FATAL','%Error','Assertion failed'])
                expected=re.findall(r'^FULL_PROCESSOR_PACKET_PASS .*? commits=(\d+) ',original,re.M)
                metrics=re.findall(r'^HOLD_REPLAY_PREPARATION edges=(\d+) published0=(\d+) published1=(\d+)$',log,re.M)
                assert len(expected)==len(metrics)==1
                edges,pub0,pub1=map(int,metrics[0]);assert pub0+pub1==int(expected[0])
                observed_values|={i for i,n in enumerate([pub0,pub1])if n}
                row.update(status='REVIEWED_COMPLETE_CALLER_REPLAY_PASS',publications=pub0+pub1,hold0_publications=pub0,hold1_publications=pub1,
                           extra_hold_task_edges=edges,all_original_outcomes_and_assertions_pass=True,
                           corrected_overstrict_coverage_verdict=r['status']!='PASS_PREPARED_HOLD_COMPLETE_CALLER_REPLAY')
            rows.append(row)
    assert observed_values=={0,1}
    out.mkdir(parents=True)
    for d in dirs:retain(d,out/'retained'/d.name)
    complete=all(r['status']=='REVIEWED_COMPLETE_CALLER_REPLAY_PASS'for r in rows)
    result=dict(status='REVIEWED_ALL_FOUR_PREPARED_HOLD_CALLER_REPLAYS'if complete else'REVIEWED_VERILATOR_CALLER_REPLAYS_WITH_ICARUS_PENDING',
                previous_goal_turn='progress: implemented/proved/staged prepared Hold, launched exact jobs and pushedf38a55ac0',
                stage_manifest_sha256=sha(stage/'r-front-control-native.json'),replays=rows,
                both_Hold_values_exercised_across_completed_suite=True,
                synthetic_context_scope='Two actual-function SAT-selected contexts reproduce recorded Hold386. This qualifies caller/host preparation history, not native132-value timing, firmware boot or useful speed.',
                words_only_trace_contains_only_Hold1_not_missing_publications=True,
                old_overstrict_driver_verdict_and_all_original_inputs_and_assertions_retained=True,
                completed_synthesis=dict(builder_pid=1458513,registers_before=50362,registers_after=50494,additional_registers=132,
                    memory_bits_unchanged=2511686,five_original_RIOT_M10Ks_retained=True,placement_or_timing_not_established=True),
                synthesis_observation_sha256=sha(mapping/'results.json'),
                initialization_wait_counter_caveat='Native hold_prepare_waits counts !hold_ready even during storage_reset/memory initialization. Use post-initialization interval deltas; do not report the initialization total as CPU preparation overhead.',
                same_native_simulation_pid=65832,native_simulation_inputs_or_limits_changed=False,
                terminal_collectors_support_581_sources=True,full_goal_active=True,new_hardware_speed_gain_measured=False,
                next_action='Observe same1458513 fitter and65832 native simulation, plus the original Icarus replay handles. Collect terminal reports without restarting. Evaluate post-initialization preparation cost before selecting; actual60ns phases and all original hardware/Cedar/Rollback/persistence/stress requirements remain.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    p.add_argument('--archive-label',default='native-prepared-hold-replays')
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label=a.archive_label,
        extra_tools=[Path(__file__).name,'prepare_hold_replay_fixtures.py','replay_native_prepared_hold.py',
                     'collect_native_prepared_hold_map.py','review_native_cp_read_timing.py','collect_native_clock_enable_fit.py'])
    print(r['status'],flush=True)
