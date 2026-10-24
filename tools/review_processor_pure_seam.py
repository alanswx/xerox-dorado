#!/usr/bin/env python3
"""Retain the single-owner memory seam and stateless processor qualification.

Preserve original assertions, all closed-owner cache tests and failed early
preflight/declaration attempts. The fresh pure control function has no cache
history assumption. Two-event memory forwarding, timestamps and native timing
remain open; this receipt never selects a core or releases reserved hardware.
"""
import argparse
import gzip
import json
from pathlib import Path
import re

from inspect_r_front_pure_structure import inspect
from review_exact_pixel_phase import retain_text
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha
from test_r_front_memory_seam import transforms,EXTERNAL_OWNER
from test_r_front_pure_evaluator import pure_top

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def read(p):return json.loads(p.read_text())


def verify(d,files):
    for n,h in files.items():assert sha(d/n)==h,(d,n)


def subsequence(old,new):
    lines=iter(new.splitlines())
    return all(any(x==y for y in lines) for x in old.splitlines())


def review(out):
    assert not out.exists();dirs=[];records=[]
    for engine,version in [('icarus',3),('verilator',2)]:
        d=BUILD/f'processor-memory-seam-{engine}-v{version}-20261024';s=read(d/'run-state.json')
        assert s['status']=='PASS_FULL_PROCESSOR_SINGLE_OWNER_MEMORY_SEAM'
        assert all(v['returncode']==0 for v in s['actions']) and s['all100_clock_counts_match'] and s['sparse_captures']==4975
        parent=Path(s['source']);assert sha(parent/'run-state.json')==s['source_state_sha256']
        assert sum(row['changed'] for row in s['source_files'])==5
        for row in s['source_files']:
            p=d/row['path'];old=parent/row['path'];assert sha(p)==row['output_sha256'] and sha(old)==row['parent_sha256']
            if p.name=='tb_r_front_local.sv':
                text=p.read_text();assert text.count(EXTERNAL_OWNER)==1
                text=text.replace(EXTERNAL_OWNER,'',1).replace('files.','dut.child.child.child.files.')
                assert text.replace('control_flags,q,cp_reset,cp_ready,external_file_read,file_write_data);',
                                    'control_flags,q,cp_reset,cp_ready);',1)==old.read_text()
            else:assert p.read_text()==transforms(old.read_text(),p.name,None)
        log=(d/'test.log').read_text();assert 'checks=14884 writes=238 reads=6928 epochs=392' in log
        assert s['cache_coverage']==read(parent/'run-state.json')['cache_coverage']
        # The elaborated Icarus artifact confirms exactly one live four-bank
        # owner; an unused legacy declaration in the source list is not RAM.
        if engine=='icarus':assert len(re.findall(r'\.scope module, "files" "dorado_r_file_ports"', (d/'test.vvp').read_text()))==1
        dirs.append(d);records.append(dict(run=d.name,state_sha256=sha(d/'run-state.json')))
    for mutation in ['drop-write','invert-write-data']:
        d=BUILD/f'processor-memory-seam-{mutation}-icarus-v3-20261024';s=read(d/'run-state.json')
        assert s['status']=='PASS_SINGLE_OWNER_MEMORY_SEAM_COUNTEREXAMPLE'
        assert s['actions'][0]['returncode']==0 and s['actions'][-1]['returncode']!=0
        assert 'R front storage mismatch check=203 root=1->0' in (d/'test.log').read_text()
        for row in s['source_files']:assert sha(d/row['path'])==row['output_sha256']
        dirs.append(d)
    for engine in ['icarus','verilator']:
        d=BUILD/f'processor-memory-seam-{engine}-v1-20261024';s=read(d/'preflight-failure.json')
        assert s['status']=='TERMINAL_PREFLIGHT_COUNT_REJECTED' and sha(d/'test_r_front_memory_seam.py')==s['tool_sha256']
        assert not (d/'compile.log').exists();dirs.append(d)
    d=BUILD/'processor-memory-seam-icarus-v2-20261024';s=read(d/'run-state.json')
    assert s['status']=='TERMINAL_PROCESSOR_MEMORY_SEAM_NOT_ESTABLISHED' and s['actions'][0]['returncode']!=0
    assert 'declaration after use' in (d/'compile.log').read_text();dirs.append(d)
    proof=BUILD/'pure-control-prefix-v1-20261024';s=read(proof/'run-state.json');verify(proof,s['files'])
    assert s['status']=='PASS_STATELESS_CONTROL_PREFIX_ALL_OUTPUTS' and s['result_bits']==475 and len(s['cases'])==4
    for row in s['cases']:
        d=proof/f"{row['variant']}-w{row['waves']}";verify(d,{n:h for n,h in row['files'].items() if n!='proof.log'})
        # The original driver also captured the newly opened log as an input.
        # Preserve that launch snapshot; the completed hash below is binding.
        assert set(row['files'])=={'control.sv','actual.sv','top.sv','proof.ys','proof.log'}
        assert row['passed'] and row['returncode']==0 and sha(d/'proof.log')==row['log_sha256']
        assert ('-verify' if row['variant']=='positive' else '-falsify') in (d/'proof.ys').read_text()
    dirs.append(proof)
    for name,bad,status in [('four-state',False,'PASS_PURE_PREFIX_FULL_FOUR_STATE'),('bad-unknown',True,'PASS_PURE_PREFIX_UNKNOWN_COUNTEREXAMPLE')]:
        d=BUILD/f'pure-prefix-{name}-v1-20261024';s=read(d/'run-state.json');verify(d,s['files'])
        assert s['status']==status and s['bad_unknown']==bad and s['actions'][0]['returncode']==0
        assert sha(d/'test.log')==s['test_log_sha256']
        if bad:assert s['actions'][-1]['returncode']!=0 and 'PURE_PREFIX unknown fallback' in (d/'test.log').read_text()
        else:assert 'known=2 unknown=3820 inputs=955 outputs=475 waves=5/24' in (d/'test.log').read_text()
        dirs.append(d)
    d=BUILD/'pure-processor-verilator-v1-20261024';s=read(d/'run-state.json')
    assert s['status']=='PASS_FULL_PROCESSOR_STATELESS_EVALUATOR' and s['compared_output_ports']==30
    assert s['all_original_assertions_retained'] and s['pure_instance_auxiliary_state_bits']==0 and all(v['returncode']==0 for v in s['actions'])
    parent=Path(s['source']);assert sha(parent/'run-state.json')==s['source_state_sha256']
    for row in s['source_files']:
        p=d/row['path'];old=parent/row['path'];assert sha(p)==row['output_sha256'] and sha(old)==row['parent_sha256']
        if p.name=='candidate.sv':assert p.read_text()==old.read_text()+'\n'+pure_top(old.read_text(),(proof/'pure.sv').read_text())+'\n'
        elif p.name=='tb_r_front_local.sv':assert subsequence(old.read_text(),p.read_text())
        else:assert p.read_bytes()==old.read_bytes()
    assert sha(d/'test.log')==s['test_log_sha256'];dirs.append(d)
    structure=BUILD/'pure-processor-structure-v2-20261024';s=read(structure/'run-state.json')
    assert s['status']=='PASS_PURE_PROCESSOR_NO_EMBEDDED_STATE' and len(s['cases'])==2
    for row in s['cases']:
        d=structure/('clocked-bit-control' if row['bad'] else 'positive')
        assert row['returncode']==0 and row['passed'] and sha(d/'hierarchy.xml')==row['XML_sha256']
        assert inspect(d/'hierarchy.xml')==row['structure'] and sha(d/'elaborate.log')==row['log_sha256']
    assert s['cases'][0]['structure']==dict(module_types=67,clocked_items=[],delayed_assignments=0,delays=0,unpacked_arrays=0,forbidden_owners=[])
    assert s['cases'][1]['structure']['delayed_assignments']==1
    dirs.append(structure)
    # Retain the first uncalibrated XML observation as well.
    dirs.append(BUILD/'pure-processor-structure-v1-20261024')
    out.mkdir(parents=True)
    for d in dirs:retain_text(d,out/d.name)
    result=dict(status='REVIEWED_SINGLE_OWNER_MEMORY_AND_STATELESS_PROCESSOR',memory_reference_checks_per_engine=14884,
        physical_Qs=844,RAM_bits=9216,clock_channels=100,sparse_control_captures=4975,
        actual_writes=238,reads=6928,distinct_capture_epochs=392,closed_owner_cache_coverage_unchanged=True,
        memory_corruptions_rejected_at_original_check=203,early_preflight_and_declaration_failures_retained=True,
        pure_control_bits=475,pure_control_wave_parameters=[5,24],arbitrary_control_owners_and_pins=True,
        cache_history_assumed_by_pure_function=False,original_launch_log_snapshot_preserved_terminal_hash_bound=True,stale_prefix_and_forced_direct_corruptions_rejected=True,
        pure_control_unknown_cases=3820,pure_control_unknown_corruption_rejected=True,
        full_pure_processor_output_ports=30,full_pure_processor_original_reference_checks=14884,
        pure_processor_embedded_state_or_memory_arrays=0,structural_clocked_bit_calibration_rejected=True,
        two_event_memory_forwarding_qualified=False,actual_CP_and_device_timestamps_qualified=False,
        full_native_15ns_or_60ns_qualified=False,SDRAM66MHz_qualified=False,Cedar_Rollback_persistence_qualified=False,
        hardware_speed_qualified=False,hardware_changed=False,hardware_requires_user_release=True,full_goal_active=True,
        references=records)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path,required=True)
    a=p.parse_args();r=review(a.output.resolve());append_archive(a.archive.resolve(),a.output.resolve(),[],label='pure-processor-seam',extra_tools=[
        'test_r_front_memory_seam.py','prove_control_pure_prefix.py','test_control_pure_prefix.py','test_r_front_pure_evaluator.py',
        'inspect_r_front_pure_structure.py',Path(__file__).name]);print(r['status'],flush=True)
