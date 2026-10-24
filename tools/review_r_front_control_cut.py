#!/usr/bin/env python3
"""Preserve a bounded registered-cut proof and its rejected native schedule.

Both original-reference component comparisons may pass while the proposed
nonoverlapped native schedule is still unsuitable: it needs 120 ns rather
than 60 ns at the intended host clock. Preserve that distinction and keep
the original CP profile rejection. Do not start a fit or select a core.
"""
import argparse
import json
from pathlib import Path
import re

from probe_r_front_control_cut import candidate,bench
from review_r_front_capture_followup import append_archive
from review_r_front_cp_subevents import observation
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def read(p):return json.loads(p.read_text())


def review(out):
    assert not out.exists();out.mkdir(parents=True);dirs=[];gates=[]
    for engine in ['icarus','verilator']:
        d=BUILD/f'r-front-control-cut-{engine}-v1-20261023';r=read(d/'run-state.json')
        assert r['status']=='PASS_REGISTERED_CONTROL_COMPONENT_CUT' and r['engine']==engine
        assert r['summary']=='checks=14884 writes=238 reads=6928 epochs=392'
        assert r['actions'][-1]['returncode']==0
        assert r['registered_control_bits']==475 and r['original_reference_and_checks_unchanged']
        assert not r['native_phase_schedule_qualified'] and not r['extra_guest_cycle_excluded']
        source=Path(r['parent']);parent=read(source/'run-state.json')
        assert sha(source/'run-state.json')==r['parent_state_sha256']
        assert (d/'candidate.sv').read_text()==candidate((source/'candidate.sv').read_text())
        assert (d/'inputs/tb_r_front_local.sv').read_text()==bench((source/'inputs/tb_r_front_local.sv').read_text())
        old={v['path']:v for v in parent['source_files']}
        for row in r['source_files']:
            assert sha(d/row['path'])==row['output_sha256']
            assert row['parent_sha256']==old[row['path']]['output_sha256']
            if row['changed']:assert Path(row['path']).name in ['candidate.sv','tb_r_front_local.sv']
        assert sum(v['changed']for v in r['source_files'])==2
        # All original assertion statements remain verbatim, including the
        # complete RAM/state comparisons and every clock/output check.
        previous=(source/'inputs/tb_r_front_local.sv').read_text()
        current=(d/'inputs/tb_r_front_local.sv').read_text()
        for message in re.findall(r'\$fatal\([^;]+;',previous):assert message in current,message
        clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',(d/'test.log').read_text(),re.M)
        parent_clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',(source/'test.log').read_text(),re.M)
        assert len(clocks)==100 and clocks==parent_clocks
        gates.append(dict(engine=engine,directory=d.name,state_sha256=sha(d/'run-state.json'),
                          candidate_sha256=sha(d/'candidate.sv'),summary=r['summary'],clocks=clocks))
        dirs.append(d)
    assert gates[0]['candidate_sha256']==gates[1]['candidate_sha256']
    assert gates[0]['clocks']==gates[1]['clocks']
    budget=BUILD/'r-front-control-cut-budget-v1-20261023';b=read(budget/'results.json')
    assert b['status']=='REJECT_NONOVERLAPPED_CONTROL_CUT_FOR_60NS'
    assert b['target_instruction_host_ticks']==4 and b['nonoverlapped_instruction_host_ticks']==8
    assert b['nonoverlapped_instruction_ns']==120 and not b['quartus_build_started']
    assert sha(budget/'inputs'/b['clock_source'])==b['clock_source_sha256']
    dirs.append(budget)
    rejected=BUILD/'r-front-cp-subevents-matrix-v1-20261023/fast_gates-icarus'
    assert read(rejected/'run-state.json')['status']=='CP_BEFORE_ROOT_SUBEVENTS_FAILED'
    assert 'R front state mismatch check=12839'in(rejected/'test.log').read_text()
    native=[]
    for kind in ['5c65','queue']:
        d=BUILD/f'native-cedar-{kind}-skip-wait-v1-20261023'
        obs=out/('observed-native-'+kind);native.append(observation(d,obs))
        assert int((d/'host-cycle-limit.txt').read_text())==5000000000
        for n in ['provenance.json.gz','host-cycle-limit.txt','window-control.txt']:(obs/n).write_bytes((d/n).read_bytes())
    result=dict(status='REVIEWED_CONTROL_CUT_COMPONENT_PASSES_NATIVE_SCHEDULE_REJECTED',scope=__doc__,
                component_gates=gates,original_assertions_retained_verbatim=True,
                all100_clock_counts_identical_to_parent=True,all_original844_Qs_and9216_RAM_bits_checked=True,
                saved_control_bits=475,component_transaction_host_edges=2,
                control_stage_RAM_advance=False,physical_SRAM_banks=4,
                old_owner_and_pin_epoch_held=True,post_commit_physical_views_use_new_owners=True,
                schedule_budget=b,original_failed_profile_miter_retained=True,
                native_observations=native,registered_cut_native_schedule_rejected=True,
                native_phase_schedule_qualified=False,quartus_build_started=False,
                assembly_permitted=False,hardware_changed=False,speedup_qualified=False,
                next='Use original capture edges, or prove overlapped state forwarding and all board consumers, to fit four target host ticks per instruction. Continue the same bounded native firmware runs.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result,dirs


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path,required=True)
    a=p.parse_args();r,dirs=review(a.output.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),dirs,label='control-cut-budget',extra_tools=[
        'probe_r_front_control_cut.py','audit_r_front_control_cut_budget.py',Path(__file__).name])
    print(r['status'],flush=True)
