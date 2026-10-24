#!/usr/bin/env python3
"""Retain actual event-driven BaseBoard firmware and full-state RIOT gates.

Keep failed cadence assumptions/compiler fixtures. The passing firmware gate
is isolated BaseBoard reset/Midas with a15ns clock and pauses, never a Cedar,
native CPU/CP timing, original60ns or hardware-speed qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

from review_exact_pixel_phase import retain_text
from review_late_hold_and_input_reset import first_path
from review_baseboard_clock_consumer import CORNERS
from review_r_front_capture_followup import append_archive
from test_riot_clock_event import portable_array_syntax
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def read(p):
    return json.loads(p.read_text())


def verify(d,files):
    for name,digest in files.items():
        assert sha(d/name)==digest,(d,name)


def riot(name,observed):
    d=BUILD/name;s=read(d/'run-state.json')
    assert s['status']=='PASS_RIOT_EXPLICIT_EVENT_ORIGINAL_STATE_AND_RAM'
    assert len(s['cases'])==10 and s['scalar_bits']==73 and s['RAM_bytes']==128
    verify(d/'inputs',s['inputs'])
    assert portable_array_syntax((d/'inputs/M6532.sv').read_text())==(d/'inputs/M6532-icarus.sv').read_text()
    if observed:
        original=(d/'inputs/M6532-reference.sv').read_text().replace('module M6532_original','module M6532',1)
        assert original==(BUILD/'riot-explicit-event-v1-20261024/inputs/M6532.sv').read_text()
        assert portable_array_syntax((d/'inputs/M6532-reference.sv').read_text())==(d/'inputs/M6532-reference-icarus.sv').read_text()
        c=read(d/'source-scope-classification.json')
        assert not c['actual_vendor_source_bytes_unchanged'] and c['original_reference_body_unchanged']
    for row in s['cases']:
        assert row['passed'] and row['actions'][0]['returncode']==0
        assert (row['actions'][-1]['returncode']!=0)==row['expected_rejection']
        p=d/(row['engine']+'-'+row['variant'])/'test.log'
        assert sha(p)==row['test_log_sha256']
        if row['variant']=='positive':
            assert 'host=82725 events=27075 holds=914 idles=54736 states=82725 RAM=10588800 irq_events=18133 reset_events=2 scalar_bits=73 RAM_bytes=128' in p.read_text()
    return d


def collection(name):
    d=BUILD/name;r=read(BUILD/(name+'.tgz.json'))
    assert sha(BUILD/(name+'.tgz'))==r['archive_sha256']
    for row in r['members']:
        assert sha(d/row['path'])==row['sha256'] and (d/row['path']).stat().st_size==row['bytes']
    p=read(d/'provenance.json');b=read(d/'collected-input-check.json');s=read(d/'run-state.json')
    assert b['job_pids_absent']==[s['builder_pid']]+[row['pid'] for row in s['stages']]
    for name,digest in p['files'].items():
        raw=(d/name).read_bytes();assert sha(d/name)==b['actual'][name]
        if name in b['changed_inputs']:
            assert name=='clock_fit.qsf'
            annotation=b'\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
            assert raw.endswith(annotation) and raw.count(annotation)==1
            raw=raw[:-len(annotation)]
        assert hashlib.sha256(raw).hexdigest()==digest
    return d,r


def review(out,fit):
    assert not out.exists()
    envelopes=[]
    dirs=[riot('riot-explicit-event-v1-20261024',False),riot('riot-observation-scope-v1-20261024',True)]
    traces=[]
    for name in ['baseboard-event-rom-v1-20261024','baseboard-event-rom-v2-20261024',
                 'baseboard-event-rom-gated-v2-20261024','baseboard-event-rom-v3-20261024',
                 'baseboard-event-rom-gated-v3-20261024','baseboard-event-rom-v4-20261024',
                 'baseboard-event-rom-gated-v4-20261024']:
        d=BUILD/name;s=read(d/'run-state.json');verify(d/'inputs',s['inputs'])
        assert sha(d/'tb_baseboard_event_boot.sv')==s['fixture_sha256']
        assert s['actions'][0]['returncode']==0 and s['require_6502_settled_before_phase']
        log=(d/'test.log').read_text()
        assert 'BB_RESET_CLEAR_EXIT' in log and 'BB_RESET_MIDAS_RETURN' in log
        if '-v2-' in name:
            assert s['status']=='TERMINAL_EVENT_BASEBOARD_NOT_ESTABLISHED' and s['actions'][-1]['returncode']!=0
            assert 'BB_EVENT expected66/67 host ticks per1MHz period' in log
        else:
            assert s['status']=='PASS_ISOLATED_EVENT_BASEBOARD_ROM_MIDAS' and s['actions'][-1]['returncode']==0
            assert 'BB_EVENT_CONVERGENCE checks=5178 unsettled=0' in log
        if '-v3-' in name or '-v4-' in name:
            assert 'BB_EVENT_PERIOD min=66 max=129 nominal_ns=1000 cold_initial_interval=129' in log
            assert 'BB_EVENT_COLD_REFERENCE intervals=176750 crystal_rises=26513 Q_bits=6' in log
            assert 'BB_RESET_MIDAS_RETURN ticks=176750 phases=2651 fetches=865 stores=289 x=ff' in log
            traces.append(re.findall(r'^BB_RESET_[^\n]+',log,re.M))
        if '-v4-' in name:
            assert 'BB_EVENT_SETTLING phases=5177 max_last_unsettled_age=14 shortest_phase_counter=32 longest_phase_counter=33' in log
        dirs.append(d)
    assert all(trace==traces[0] for trace in traces), 'Transport-paused firmware trace differs'
    for version in [1,2]:
        d,r=collection(f'baseboard-event-component-failed-v{version}-20261024')
        s=read(d/'run-state.json');assert s['status']=='TERMINAL_FAILED' and s['stages'][0]['returncode']==3
        marker='can\'t resolve reference' if version==1 else 'object "riot_stuff" is not declared'
        assert marker in (d/'map.log').read_text()
        dirs.append(d)
        envelopes.append(BUILD/(d.name+'.tgz.json'))
    for version in [1,2,3]:
        d=BUILD/f'baseboard-event-component-stage-v{version}-20261024';p=read(d/'provenance.json');verify(d,p['files'])
        assert p['result_bits']==7316 and p['clock_period_ns']==15 and p['RIOT_packages']==5
        if version>=2:
            for row in p['observation_port_changes']:
                raw=(d/'inputs'/row['path']).read_bytes().decode()
                for change in reversed(row['replacements']):
                    i=change['offset'];old=change['before'];new=change['after']
                    assert raw[i:i+len(new)]==new
                    raw=raw[:i]+old+raw[i+len(new):]
                assert hashlib.sha256(raw.encode()).hexdigest()==row['original_sha256'] and row['inverse_byte_identical']
        dirs.append(d)
    qualified=read(BUILD/'baseboard-event-component-stage-v3-20261024/observation-scope-qualification.json')
    assert qualified['tests_state_sha256']==sha(BUILD/'riot-observation-scope-v1-20261024/run-state.json')
    d,r=collection(fit.name);assert d==fit
    s=read(d/'run-state.json');assert s['status']=='TERMINAL_REVIEW_REQUIRED' and all(row['returncode']==0 for row in s['stages'])
    p=read(d/'provenance.json');assert p==read(BUILD/'baseboard-event-component-stage-v3-20261024/provenance.json')
    assert re.findall(r'BB_EVENT_OUTPUT_BITS (\d+)',(d/'corners.log').read_text())==['7316']*4
    corners=[]
    for c in CORNERS:
        row=dict(condition=c)
        for mode in ['setup','hold']:
            row[mode]=first_path(d/('clock-'+mode+'-'+c+'.rpt'))
        counts=re.findall(r'; (?:Illegal Clocks|Unconstrained [^;]+)\s*;\s*(\d+)\s*;\s*(\d*)\s*;',
            (d/('clock-ucp-'+c+'.rpt')).read_text())
        assert len(counts)==6 and all(a=='0' and b in ['','0'] for a,b in counts)
        corners.append(row)
    dirs.append(d)
    envelopes.append(BUILD/(d.name+'.tgz.json'))
    out.mkdir(parents=True)
    for d in dirs:
        retain_text(d,out/d.name)
    for p in envelopes:
        (out/p.name).write_bytes(p.read_bytes())
    result=dict(status='REVIEWED_BASEBOARD_EVENT_ROM_AND_FULL_STATE_COMPONENT',
        RIOT_host_checks_per_engine=82725,RIOT_events_per_engine=27075,RIOT_RAM_byte_checks_per_engine=10588800,
        RIOT_scalar_bits=73,RIOT_RAM_bytes=128,negative_controls_per_engine=4,
        cold_and_steady_clock_intervals_per_boot=176750,original_reset_stores=256,original_reset_DEX=256,
        MidasSetup_return=True,transport_paused_firmware_trace_identical=True,
        MCU_convergence_checks_per_boot=5178,MCU_unsettled_boundaries=0,
        MCU_max_last_unsettled_check_age=14,MCU_steady_half_period_host_edges=[33,34],
        MCU_settling_measurement_scope='Original reset and first MidasSetup return, not arbitrary firmware/peripheral histories',
        failed_startup_cadence_assumption_preserved=True,two_compiler_failures_preserved=True,
        observation_edits_inverse_byte_identical=True,static_PA7_scope_change_full_state_reference_qualified=True,
        component_output_bits=7316,component_corners=corners,
        component_15ns_setup_hold_pass=all(row[m]['slack_ns']>=0 for row in corners for m in ['setup','hold']),
        native_CPU_root_CP_subevents_integrated=False,native_timing_qualified=False,
        Cedar_qualified=False,original_60ns_qualified=False,hardware_changed=False,speedup_qualified=False,
        hardware_requires_user_release=True,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['output','fit','archive']:
        p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();result=review(a.output.resolve(),a.fit.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),[],label='baseboard-event-rom',extra_tools=[
        'test_riot_clock_event.py','test_baseboard_clock_event_boot.py','fit_baseboard_event.py',
        'collect_control_clock_fit.py',Path(__file__).name])
    print(json.dumps(result),flush=True)
