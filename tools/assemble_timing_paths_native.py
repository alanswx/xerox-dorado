#!/usr/bin/env python3
"""Assemble one fully reviewed split-selection timing-path fit; never fit or program it."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time

GUARD_CASES = {'changed-source', 'changed-report', 'changed-qualification',
               'changed-reviewer', 'nonrejecting-control', 'missing-response-report',
               'changed-manifest', 'changed-counter-report',
               'changed-lookahead-inventory', 'changed-crystal-report'}
OLD_WORD_GUARD_CASES = GUARD_CASES | {
    'changed-oldword-path', 'changed-oldword-inventory',
    'changed-oldword-audit-source', 'changed-early-source'}


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def checked(root, name):
    path = Path(name)
    assert not path.is_absolute() and '..' not in path.parts
    return root / path


def validate(project, old_word=False):
    qpath, cpath = project / 'timing-preassembly-qualification.json', project / 'timing-fit-review-controls.json'
    q, c = json.loads(qpath.read_text()), json.loads(cpath.read_text())
    r = json.loads((project / 'timing-paths-native.json').read_text())
    if old_word:
        from test_review_storage_old_word_native import CASES as expected_cases
    else:
        from test_review_timing_paths_native import CASES as expected_cases
    label = 'OLD_WORD_EARLY' if old_word else 'TIMING_PATHS'
    assert q['status'] == f'PASS_NATIVE_{label}_PREASSEMBLY_REVIEW'
    assert c['status'] == f'PASS_NATIVE_{label}_REVIEW_REJECTION_CONTROLS'
    assert c['positive_qualification_sha256'] == sha(qpath) and c['original_evidence_unchanged']
    assert {a['name'] for a in c['cases']} == set(expected_cases) and len(c['cases']) == len(expected_cases)
    assert all(a['rejected'] and a['returncode'] != 0 for a in c['cases'])
    review_module = 'tools/review_storage_old_word_native.py' if old_word else 'tools/review_timing_paths_native.py'
    assert {review_module, 'tools/review_storage_response_native.py'} <= set(c['reviewer_dependencies'])
    for name, digest in c['reviewer_dependencies'].items():
        assert sha(checked(project / 'review-dependencies', name)) == digest, name
    assert c['recipe_config_sha256'] == q['recipe_config_sha256']
    assert q['source_hashes'] == r['source_hashes'] and len(q['source_hashes']) == 503
    assert q['frequency_hz'] == r['frequency_hz'] == 57142857
    assert q['sysper'] == r['sysper'] == 4 and q['state_admin'] == r['state_admin'] == 'INACTIVE'
    assert q['raw_buffer_lines'] == r['raw_buffer_lines'] == 4 and q['response_overlap'] == r['response_overlap'] == 1
    assert q['parallel_lookup'] == r.get('parallel_lookup', False)
    assert not q['firmware_qualified'] and not q['hardware_speedup_qualified']
    assert len(q['timing_checks']) == 31 and all(float(s) > 0 for _, s in q['timing_checks'])
    assert len(q['detailed_paths']) == (468 if old_word else 444)
    assert all(a['paths'] > 0 and a['slack_ns'] > 0 for a in q['detailed_paths'])
    assert q['minimum_setup_ns'] > 0 and q['minimum_hold_ns'] > 0
    assert q['resources']['ram_blocks'] == 332 and q['resources']['dsp_blocks'] == 35
    assert q['memory']['cache']['packing'] == 'independent40'
    assert q['memory']['cache']['unique_m10ks'] == 40 and q['memory']['cache']['logical_bits'] == 294912
    assert q['buffer_registers'] == {'line_words': {'bits': 384, 'bits_per_line': 96},
                                     'line_tag': {'bits': 80, 'bits_per_line': 20}}
    assert q['hit_counter_bits'] == 32
    for feature in ['split_selection','parallel_ready_compare','ALU_carry_select']:
        assert q[feature] is True and r[feature] is True
    assert q['request_lookahead'] == r['request_lookahead'] == 1
    assert q['crystal_SYSPER'] == r['baseboard_crystal_samples_per_instruction'] == 8
    assert q['crystal_FREQ_KHZ'] == 10000 and q['lookahead_state_bits'] == 165
    assert r['nominal_ordinary_ns'] == 140
    if old_word:
        for feature in ['old_word_falling', 'early_read_response', 'decoded_mux', 'ALU_direct_carry']:
            assert q[feature] is True and r[feature] is True
        assert q['capture_read_word'] is True and q['transport_control_bits'] == 128
        assert q['old_word_register_families'] == {
            corner: {family: dict(physical=bits, canonical=bits)
                     for family, bits in [('word', 1152), ('pipe', 1152), ('qualifier', 128)]}
            for corner in ['7_slow_1100mv_-40c', '7_slow_1100mv_100c',
                           'MIN_fast_1100mv_-40c', 'MIN_fast_1100mv_100c']}
    for name, digest in {**q['source_hashes'], **q['reviewed_evidence_hashes']}.items():
        assert sha(checked(project, name)) == digest, ('Reviewed input changed', name)
    for name, pidkey, key in [('run-state.json', 'builder_pid', 'stages'),
                              ('storage-ready-reports-v1/run-state.json', 'driver_pid', 'actions')]:
        state = json.loads((project / name).read_text())
        assert state['status'].startswith('TERMINAL_') and all(a['returncode'] == 0 for a in state[key])
        for pid in [state[pidkey]] + [a['pid'] for a in state[key]]:
            assert not (Path('/proc') / str(pid)).exists(), ('Recorded process still present', pid)
    if old_word:
        state = json.loads((project / 'extra-old-word-v1/run-state.json').read_text())
        assert state['status'] == 'TERMINAL_OLD_WORD_REPORTS_REVIEW_REQUIRED' and state['returncode'] == 0
        for pid in [state['pid'], state['child_pid']]:
            assert not (Path('/proc') / str(pid)).exists(), ('Old-word audit still present', pid)
    return q, c


def run(project, quartus, old_word=False):
    q, c = validate(project, old_word=old_word)
    gpath = project / 'timing-assembly-guard-controls.json'
    g = json.loads(gpath.read_text())
    label = 'OLD_WORD_EARLY' if old_word else 'TIMING_PATHS'
    assert g['status'] == f'PASS_NATIVE_{label}_ASSEMBLY_GUARD_CONTROLS'
    assembler = Path(__file__).with_name('assemble_storage_old_word_native.py') if old_word else Path(__file__)
    assert g['assembler_sha256'] == sha(assembler) and g['original_evidence_unchanged']
    if old_word:
        assert g['shared_assembler_sha256'] == sha(Path(__file__))
    assert not g['Quartus_executed'] and g['qualification_sha256'] == sha(project / 'timing-preassembly-qualification.json')
    assert g['reviewer_controls_sha256'] == sha(project / 'timing-fit-review-controls.json')
    guard_cases = OLD_WORD_GUARD_CASES if old_word else GUARD_CASES
    assert {a['name'] for a in g['cases']} == guard_cases and len(g['cases']) == len(guard_cases)
    assert all(a['rejected'] for a in g['cases'])
    statepath = project / 'timing-assembly-state.json'
    rbf = project / 'core/output_files/Dorado.rbf'
    assert not statepath.exists() and not rbf.exists(), 'Inspect the existing assembly; never restart it'
    state = dict(status='RUNNING_ASSEMBLY', driver_pid=os.getpid(), started=time.time(),
                 qualification_sha256=sha(project / 'timing-preassembly-qualification.json'),
                 controls_sha256=sha(project / 'timing-fit-review-controls.json'),
                 hardware_deployed=False, firmware_qualified=False)

    def save():
        temporary = statepath.with_suffix('.tmp')
        temporary.write_text(json.dumps(state, indent=2) + '\n')
        os.replace(temporary, statepath)

    save()
    try:
        command = [str(quartus / 'quartus_asm'), '--read_settings_files=on', '--write_settings_files=off', 'Dorado']
        with (project / 'timing-assembly.log').open('x') as log:
            process = subprocess.Popen(command, cwd=project / 'core', stdout=log, stderr=subprocess.STDOUT)
            state.update(child_pid=process.pid, command=command)
            save()
            state['returncode'] = process.wait()
            save()
            assert state['returncode'] == 0
        assert 'Assembler was successful' in (project / 'timing-assembly.log').read_text()
        assert rbf.stat().st_size > 1000000
        validate(project, old_word=old_word)
        state.update(status=f'TERMINAL_ASSEMBLED_REVIEWED_NATIVE_{label}', finished=time.time(),
                     rbf_sha256=sha(rbf), rbf_bytes=rbf.stat().st_size)
        save()
        print(json.dumps(state), flush=True)
    except BaseException:
        state.update(status='TERMINAL_FAILED', finished=time.time())
        save()
        raise


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--project', type=Path, required=True)
    parser.add_argument('--quartus', type=Path, default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    parser.add_argument('--check-only', action='store_true')
    args = parser.parse_args()
    if args.check_only:
        validate(args.project.resolve())
        print('PASS_NATIVE_TIMING_PATHS_ASSEMBLY_INPUT_GUARD; no Quartus call')
    else:
        run(args.project.resolve(), args.quartus.resolve())
