#!/usr/bin/env python3
"""Preserve exact-state CP timing replay and its initialization failures.

Eight original-g18 valid-margin cases agreeing between engines do not qualify
the candidate, native producer setup, other sinks, a bitstream or CPU speed.
The original failing full comparison remains rejected and unchanged.
"""
import argparse
import json
from pathlib import Path
import re

from review_r_front_capture_followup import append_archive
from review_r_front_cp_subevents import observation
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def read(p):return json.loads(p.read_text())


def verify(d):
    r=read(d/'run-state.json')
    assert r['status']!='RUNNING' and all('returncode'in a for a in r['actions'])
    assert r['original_timed_cells_unchanged'] and r['original_parent_rejection_retained']
    assert r['component_state_replay_only'] and not r['native_state_import'] and not r['qualifying_gate']
    assert sha(d/'tb_cp_flag_replay.sv')==r['fixture_sha256']
    parent=Path(r['parent']);p=read(parent/'run-state.json')
    originals={row['path']:row['output_sha256']for row in p['source_files']}
    for row in r['source_files']:
        assert sha(d/row['path'])==row['sha256']==originals[row['path']]
    return r


def review(out):
    assert not out.exists();out.mkdir(parents=True);dirs=[];passes=[];history=[]
    for engine in ['icarus','verilator']:
        for version in [1,2,3]:
            d=BUILD/f'r-front-cp-flag-replay-{engine}-v{version}-20261023';r=verify(d)
            assert r['actions'][0]['returncode']==0
            if version==3:
                assert r['status']=='PASS_ORIGINAL_CP_FLAG_MARGIN_REPLAY' and r['actions'][-1]['returncode']==0
                assert len(r['results'])==11 and sum(v['valid']for v in r['results'])==8
                assert (d/'test.log').read_text().count('CP_REPLAY_SEED_EXACT ')==11
                assert r['minimum_setup_ps']==2500 and r['minimum_hold_ps']==1500
                passes.append(dict(engine=engine,results=r['results'],snapshot=read(d/'snapshot.json'),
                                   directory=d.name,state_sha256=sha(d/'run-state.json')))
            elif engine=='icarus' and version==2:
                assert r['status']=='PASS_ORIGINAL_CP_FLAG_MARGIN_REPLAY'
            else:
                assert r['status']=='ORIGINAL_CP_FLAG_MARGIN_REPLAY_FAILED' and r['actions'][-1]['returncode']!=0
                assert 'CP_REPLAY_SEED_DIFF' in (d/'test.log').read_text()
            history.append(dict(directory=d.name,status=r['status'],state_sha256=sha(d/'run-state.json')));dirs.append(d)
    assert passes[0]['snapshot']==passes[1]['snapshot']
    left={v['offset_ps']:v for v in passes[0]['results']};right={v['offset_ps']:v for v in passes[1]['results']}
    assert set(left)==set(right)=={-16000,-8000,-4000,-2500,-1000,0,1000,1500,4000,8000,16000}
    comparisons=[]
    for offset in sorted(left):
        a,b=left[offset],right[offset]
        assert a['margin_ps']==b['margin_ps'] and a['valid']==b['valid']
        assert a['expected']==b['expected']
        if a['valid']:assert a['Q']==b['Q']==a['expected']
        if offset==0:assert a['Q']==1 and b['Q']==0 and a['margin_ps']==0
        else:assert a['Q']==b['Q']
        comparisons.append(dict(offset_ps=offset,margin_ps=a['margin_ps'],part_setup_hold_met=bool(a['valid']),
                                Icarus_Q=a['Q'],Verilator_Q=b['Q']))
    assert [v['offset_ps']for v in comparisons if not v['part_setup_hold_met']]==[-1000,0,1000]
    failed=BUILD/'r-front-cp-subevents-matrix-v1-20261023/fast_gates-icarus'
    assert read(failed/'run-state.json')['status']=='CP_BEFORE_ROOT_SUBEVENTS_FAILED'
    assert 'R front state mismatch check=12839'in(failed/'test.log').read_text()
    original=ROOT/'fpga/mister/reports/2026-10-23-native-flag-windows/cp-flag-arrival-summary.json'
    spec=read(original)['original_part_timing'];assert spec['minimum_setup_ns']==2.5 and spec['minimum_hold_ns']==1.5
    native=[]
    for kind in ['5c65','queue']:
        d=BUILD/f'native-cedar-{kind}-skip-wait-v1-20261023'
        observed=observation(d,out/('observed-native-'+kind))
        assert int((d/'host-cycle-limit.txt').read_text())==5000000000
        for n in ['provenance.json.gz','host-cycle-limit.txt','window-control.txt']:
            (out/('observed-native-'+kind)/n).write_bytes((d/n).read_bytes())
        native.append(observed)
    result=dict(status='REVIEWED_EXACT_ORIGINAL_CP_FLAG_MARGIN_REPLAY',scope=__doc__,
                original_timing_source_sha256=sha(original),original_part_timing=spec,
                snapshots_identical_between_engines=True,exact_restore_trials_per_engine=11,
                physical_state_bits_per_restore=844,RAM_bits_per_restore=9216,
                valid_g18_trials_per_engine=8,timing_violation_trials_per_engine=3,comparison=comparisons,
                original_failed_miter_retained=True,initialization_history=history,native_observations=native,
                candidate_margin_equivalence_qualified=False,other_sink_timing_qualified=False,
                native_producer_setup_qualified=False,physical_timing_qualified=False,
                assembly_permitted=False,hardware_changed=False,speedup_qualified=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result,dirs


def clock7(out):
    assert not out.exists();out.mkdir(parents=True);dirs=[];rows=[]
    for engine in ['icarus','verilator']:
        d=BUILD/f'r-front-cp-flag-replay-{engine}-v4-20261023';r=verify(d)
        assert r['status']=='PASS_ORIGINAL_CP_FLAG_MARGIN_REPLAY' and r['actions'][-1]['returncode']==0
        assert len(r['results'])==11 and sum(v['valid']for v in r['results'])==8
        previous=read(BUILD/f'r-front-cp-flag-replay-{engine}-v3-20261023/run-state.json')
        for a,b in zip(r['results'],previous['results']):
            assert all(a[k]==v for k,v in b.items()),('Flag observer changed g18 replay',engine,a,b)
        rows.append(dict(engine=engine,directory=d.name,results=r['results'],state_sha256=sha(d/'run-state.json')));dirs.append(d)
    widths=[(v['offset_ps'],v['clock7_edges'],v['clock7_low_ps'])for v in rows[0]['results']]
    assert widths==[(v['offset_ps'],v['clock7_edges'],v['clock7_low_ps'])for v in rows[1]['results']]
    assert widths==[(-16000,1,16500),(-8000,1,8500),(-4000,1,4500),(-2500,1,3000),
                    (-1000,1,1500),(0,1,500),(1000,0,-1),(1500,0,-1),(4000,0,-1),(8000,0,-1),(16000,0,-1)]
    result=dict(status='REVIEWED_ORIGINAL_CP_CLOCK7_WIDTH_REPLAY',scope=__doc__,engine_results=rows,
                clock7_edges_and_widths_match_both_engines=True,zero_skew_low_pulse_ps=500,
                g18_observed_results_unchanged_from_v3=True,original_failed_miter_retained=True,
                minimum_clock7_pulse_spec_established=False,physical_runt_capture_qualified=False,
                native_producer_setup_qualified=False,candidate_equivalence_qualified=False,
                physical_timing_qualified=False,assembly_permitted=False,hardware_changed=False,speedup_qualified=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result,dirs


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path,required=True)
    p.add_argument('--clock7',action='store_true',help='Append the original clock7 pulse-width observations')
    a=p.parse_args();r,dirs=(clock7 if a.clock7 else review)(a.output.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),dirs,label='cp-clock7-width'if a.clock7 else'cp-flag-replay',extra_tools=[
        'replay_r_front_cp_flag.py',Path(__file__).name])
    print(r['status'])
