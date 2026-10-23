#!/usr/bin/env python3
"""Review the rejected v5 fit, SRAM folding and bounded flag-window gates.

Keep initial compile/integration/reader failures alongside corrected results.
No FPGA image, native selection, faster cadence or boot qualification follows.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import re

from probe_r_front_flag_windows import candidate, adapter, bench
from review_r_front_control_native import archive
from test_r_front_flag_matrix import check
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def read(p):return json.loads(p.read_text())


def verify_sources(d,r):
    for row in r.get('source_files',r.get('inputs',[])):
        assert sha(d/row['path'])==row['output_sha256'],(d,row['path'])


def review(out):
    assert not out.exists()
    dirs=[];units=[];flags=[];folding=[];failures=[]
    native=BUILD/'r-front-physical-native-fit-terminal-v5-20261023'
    collection=read(BUILD/'r-front-physical-native-v5-terminal-20261023.tgz.json')
    assert sha(BUILD/'r-front-physical-native-v5-terminal-20261023.tgz')==collection['archive_sha256']
    for row in collection['members']:
        p=native/row['path'];assert sha(p)==row['sha256'] and p.stat().st_size==row['bytes']
    manifest=read(native/'r-front-control-native.json')
    assert read(native/'physical-native-input-readback.json')['actual']==manifest['source_hashes']
    for n,h in manifest['source_hashes'].items():assert sha(native/n)==h,n
    result_dir=BUILD/'r-front-physical-native-review-v5-20261023'
    fit=read(result_dir/'results.json')
    assert fit['status']=='REVIEWED_NATIVE_PHYSICAL_FIT_REJECTED'
    assert [c['setup_ns'] for c in fit['corners']]==[-6.235,-5.868,3.960,4.663]
    assert fit['hold_min_ns']==0.109 and fit['mpw_min_ns']==1.122
    dirs.extend([native,result_dir])
    for name,copies in [('stk-address-registers-v1',17),('stk-address-registers-v5-query1',16)]:
        d=BUILD/name;r=read(d/'results.json');s=read(d/'run-state.json')
        assert r['status']=='PASS_FITTED_STK_PARALLEL_ADDRESS_REGISTERS' and r['physical_copies']==copies
        assert r['primary_address_bits']==16 and r['owner_FFs']==8 and not r['extra_pipeline_stage']
        assert sha(d/'query.log')==r['query_log_sha256'] and sha(d/'query.tcl')==s['script_sha256']
        assert s['actions'][-1]['returncode']==0
        if copies==16:
            repair=read(d/'review-repair.json')
            assert repair['status']=='TERMINAL_STK_REVIEW_RECOVERED_WITHOUT_QUERY_RESTART'
            assert (d/'initial-review-failure.json').exists() and (d/'initial-reader.py').exists()
        folding.append(dict(directory=name,result_sha256=sha(d/'results.json'),physical_copies=copies));dirs.append(d)
    for engine in ['icarus','verilator']:
        d=BUILD/f'r-flag-window-{engine}-v2-20261023';r=read(d/'run-state.json');verify_sources(d,r)
        assert r['status']=='PASS_R_FLAG_WINDOW' and r['actions'][-1]['returncode']==0
        assert 'R_FLAG_WINDOW PASS checks=327680 transitions=65536 per_bit_states=128' in (d/'test.log').read_text()
        assert sha(d/'dorado_r_flag_window_step.sv')==sha(ROOT/'verilog/rtl/dorado_r_flag_window_step.sv')
        units.append(dict(directory=d.name,status=r['status'],run_state_sha256=sha(d/'run-state.json')));dirs.append(d)
        for mutation in ['omit_close','ignore_set','set_over_clock','omit_new_set']:
            d=BUILD/f'r-flag-window-{mutation}-{engine}-v2-20261023';r=read(d/'run-state.json');verify_sources(d,r)
            assert r['status']=='R_FLAG_WINDOW_FAILED' and r['actions'][0]['returncode']==0 and r['actions'][-1]['returncode']!=0
            assert 'R flag ' in (d/'test.log').read_text()
            units.append(dict(directory=d.name,status=r['status'],mutation=mutation,run_state_sha256=sha(d/'run-state.json')));dirs.append(d)
        for mutation in [None,'omit_flag_close','hide_open_flag']:
            suffix=(mutation+'-') if mutation else ''
            d=BUILD/f'r-front-flag-windows-{suffix}{engine}-v3-20261023';r=read(d/'run-state.json');verify_sources(d,r)
            parent=Path(r['parent']);assert sha(parent/'run-state.json')==r['parent_run_state_sha256']
            for row in r['source_files']:
                p=d/row['path'];q=parent/row['path'];assert sha(q)==row['parent_sha256']
                raw=q.read_text();expected=raw
                if p.name=='candidate.sv':expected=candidate(raw,mutation)
                elif p.name=='tb_r_front_local.sv':expected=bench(raw)
                elif p.name in ['dorado_local_r_operand_step.sv','dorado_local_r_flags_step.sv',
                                'dorado_local_r_address_step.sv','dorado_local_r_memory_step.sv',
                                'dorado_local_r_front_step.sv','dorado_local_r_front_ready_step.sv']:
                    expected=adapter(raw,p.name)
                assert p.read_text()==expected,(d,row['path'])
            assert r['all_original_checks_preserved'] and r['pin_checks_added'] and r['closed_encoding_bits']==88
            if mutation:
                assert r['status']=='R_FRONT_FLAG_WINDOWS_FAILED' and r['actions'][-1]['returncode']!=0
                rejected=15 if mutation=='omit_flag_close' else 14
                assert f'R front state mismatch check={rejected}' in (d/'test.log').read_text()
            else:check(d)
            flags.append(dict(directory=d.name,status=r['status'],mutation=mutation,run_state_sha256=sha(d/'run-state.json')));dirs.append(d)
        for version in [1,2]:
            d=BUILD/f'r-front-flag-windows-{engine}-v{version}-20261023';r=read(d/'run-state.json')
            assert r['status']=='R_FRONT_FLAG_WINDOWS_FAILED' and r['actions'][-1]['returncode']!=0
            verify_sources(d,r);failures.append(dict(directory=d.name,status=r['status'],run_state_sha256=sha(d/'run-state.json')));dirs.append(d)
        d=BUILD/f'r-front-current-address-{engine}-v1-20261023';r=read(d/'run-state.json');verify_sources(d,r)
        assert r['status']=='PASS_R_FRONT_CURRENT_ADDRESS_EPOCH_PROBE' and r['actions'][-1]['returncode']==0
        dirs.append(d)
    matrices=[]
    for kind,name,status in [
        ('address','r-front-address-epoch-matrix-v1-20261023','PASS_R_FRONT_ADDRESS_EPOCH_PROFILE_MATRIX'),
        ('flags','r-front-flag-windows-matrix-v1-20261023','PASS_R_FRONT_FLAG_WINDOW_PROFILE_MATRIX')]:
        d=BUILD/name;r=read(d/'run-state.json');assert r['status']==status and len(r['cases'])==12 and not r['errors']
        for row in r['cases']:
            case=ROOT/row['directory'];s=read(case/'run-state.json');verify_sources(case,s)
            assert s['actions'][-1]['returncode']==0 and s['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
            if kind=='flags':check(case)
        matrices.append(dict(directory=name,status=status,cases=12,run_state_sha256=sha(d/'run-state.json')));dirs.append(d)
    stage=BUILD/'r-front-physical-native-stage-v6-20261023';s=read(stage/'r-front-control-native.json')
    for n,h in s['source_hashes'].items():assert sha(stage/n)==h,n
    assert s['closed_latch_encoding_bits']==88 and s['canonical_visible_R_owners']==16 and not s['assembly_permitted']
    lint=BUILD/'r-front-physical-native-lint-v6-20261023';l=read(lint/'run-state.json')
    assert l['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION' and l['structural_warnings_visible']
    warnings=dict(Counter(re.findall(r'^%Warning-([A-Z0-9_]+):',(lint/'lint.log').read_text(),re.M)))
    assert warnings['UNOPTFLAT']==3 and warnings['MULTIDRIVEN']==1
    dirs.extend([stage,lint])
    # Preserve the earlier exhaustive unit gate and its rejected variants too.
    for d in BUILD.glob('r-flag-window-*-v1-20261023'):
        r=read(d/'run-state.json');assert r['status']!='RUNNING';verify_sources(d,r);dirs.append(d)
    out.mkdir(parents=True)
    result=dict(status='REVIEWED_NATIVE_V5_REJECTION_AND_R_FLAG_WINDOWS',scope=__doc__,native_v5_fit=fit,
                collection_sha256=sha(BUILD/'r-front-physical-native-v5-terminal-20261023.tgz.json'),
                STK_parallel_capture=folding,unit_gates=units,integration_gates=flags,retained_failures=failures,
                profile_matrices=matrices,native_v6_stage_sha256=sha(stage/'r-front-control-native.json'),
                native_v6_structural_warnings=warnings,native_timing_qualified=False,firmware_qualified=False,
                cadence_changed=False,speedup_qualified=False,assembly_permitted=False,
                next='Review v6 actual full-core timing; audit pointer capture epochs; retain full 60ns/SDRAM and Cedar/Rollback/persistence/boot/stress goals.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    return result,dirs


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r,dirs=review(a.output.resolve());print(r['status'])
    if a.archive:archive(a.archive.resolve(),a.output.resolve(),dirs,additional_tools=[
        'inspect_stk_address_registers.py','inspect_stk_address_registers.tcl','probe_r_front_current_address_epoch.py',
        'probe_r_front_flag_windows.py','test_r_flag_window.py','test_r_front_flag_matrix.py','stage_r_front_flag_native.py',
        'review_r_front_physical_native.py','review_r_front_flag_native.py'])
