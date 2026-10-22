#!/usr/bin/env python3
"""Assemble only the exact reviewed57.1MHz native build; never program hardware."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time

CASES = {'missing-ready-corner', 'negative-SDRAM-output-setup', 'missing-original-IFU',
         'rehashed-weakened-clock', 'rehashed-late-capture', 'preview-not-used',
         'wrong-derived-clock', 'wrong-actual-timeout', 'wrong-command-encoding',
         'wrong-cache-old-data', 'cache-pair-alias', 'cache-wrong-block-count',
         'missing-ready-register', 'missing-real-trace-bit', 'failed-fit-state',
         'new-unreviewed-warning', 'negative-first-fault', 'rehashed-disconnected-step'}


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def checked_file(project, name):
    p = Path(name)
    assert not p.is_absolute() and '..' not in p.parts
    return project / p


def validate(project):
    q = json.loads((project / 'preassembly-qualification.json').read_text())
    c = json.loads((project / 'board-fit-review-controls.json').read_text())
    r = json.loads((project / 'storage-board-native.json').read_text())
    assert q['status'] == 'PASS_NATIVE_STORAGE_BOARD_PREASSEMBLY_REVIEW'
    assert c['status'] == 'PASS_NATIVE_STORAGE_BOARD_REVIEW_REJECTION_CONTROLS'
    assert c['positive_qualification_sha256'] == sha(project / 'preassembly-qualification.json')
    assert c['original_evidence_unchanged']
    assert len(c['cases']) == len(CASES) and {x['name'] for x in c['cases']} == CASES
    assert all(x['rejected'] and x['returncode'] != 0 for x in c['cases'])
    assert 'tools/review_storage_board_native.py' in c['reviewer_dependencies']
    for n, h in c['reviewer_dependencies'].items():
        assert sha(checked_file(project / 'review-dependencies', n)) == h, n
    assert c['recipe_config_sha256'] == q['recipe_config_sha256']
    assert q['frequency_hz'] == r['frequency_hz'] == 57142857
    assert q['frequency_mhz'] == r['frequency_mhz'] == 57.142857
    assert q['sysper'] == r['sysper'] == 4 and q['state_admin'] == r['state_admin'] == 'INACTIVE'
    assert r['CPU_cache_map_ECC_peripheral_logic_unchanged']
    assert not q['firmware_qualified'] and not q['hardware_speedup_qualified']
    assert q['source_hashes'] == r['source_hashes'] and len(q['source_hashes']) == 503
    assert q['ready_registers'] == 8 and len(q['timing_checks']) == 31
    assert all(float(slack) > 0 for _, slack in q['timing_checks'])
    assert len(q['detailed_paths']) == 316
    assert all(x['paths'] > 0 and x['slack_ns'] >= 0 for x in q['detailed_paths'])
    assert q['minimum_setup_ns'] > 0 and q['minimum_hold_ns'] > 0
    assert q['resources']['ram_blocks'] == 330 and q['resources']['dsp_blocks'] == 35
    assert q['memory']['fitting']['cache_repacking']['current_unique_m10ks'] == 38
    assert q['memory']['fitting']['cache_repacking']['physical']['logical_bits'] == 294912
    assert q['command_encoding'] == dict(MODE='000', REFRESH='001', PRECHARGE='010', ACTIVE='011', WRITE='100', READ='101', NOP='111')
    for name, h in {**q['source_hashes'], **q['reviewed_evidence_hashes']}.items():
        assert sha(checked_file(project, name)) == h, ('Reviewed source/report changed', name)
    for n, pid_key, rows in [('run-state.json', 'builder_pid', 'stages'),
                             ('storage-ready-reports-v2/run-state.json', 'driver_pid', 'actions')]:
        state = json.loads((project / n).read_text())
        assert state['status'].startswith('TERMINAL_') and all(x['returncode'] == 0 for x in state[rows])
        for pid in [state[pid_key]] + [x['pid'] for x in state[rows]]:
            try:
                os.kill(pid, 0)
            except ProcessLookupError:
                continue
            raise RuntimeError('Recorded process still present; inspect ' + str(pid))
    return q, c


def run(project, quartus):
    q, c = validate(project)
    tested = json.loads((project / 'board-assembly-guard-controls.json').read_text())
    assert tested['status'] == 'PASS_NATIVE_STORAGE_BOARD_ASSEMBLY_GUARD_CONTROLS'
    assert tested['assembler_sha256'] == sha(Path(__file__))
    assert tested['qualification_sha256'] == sha(project / 'preassembly-qualification.json')
    assert tested['reviewer_controls_sha256'] == sha(project / 'board-fit-review-controls.json')
    assert len(tested['cases']) == 6 and all(x['rejected'] for x in tested['cases'])
    assert tested['original_evidence_unchanged'] and not tested['Quartus_executed']
    path = project / 'board-assembly-state.json'
    rbf = project / 'core/output_files/Dorado.rbf'
    assert not path.exists() and not rbf.exists(), 'Observe an existing assembly; do not restart'
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(),
                 qualification_sha256=sha(project / 'preassembly-qualification.json'),
                 controls_sha256=sha(project / 'board-fit-review-controls.json'),
                 firmware_qualified=False, hardware_deployed=False)

    def save():
        temp = path.with_suffix('.json.tmp')
        temp.write_text(json.dumps(state, indent=2) + '\n')
        os.replace(temp, path)

    save()
    try:
        with (project / 'board-asm.log').open('x') as log:
            command = [str(quartus / 'quartus_asm'), '--read_settings_files=on',
                       '--write_settings_files=off', 'Dorado']
            child = subprocess.Popen(command, cwd=project / 'core', stdout=log, stderr=subprocess.STDOUT)
            state.update(assembler_pid=child.pid, command=command)
            save()
            rc = child.wait()
            state['returncode'] = rc
            save()
            assert rc == 0
        assert 'Assembler was successful' in (project / 'board-asm.log').read_text()
        assert rbf.stat().st_size > 1000000
        for name, h in q['source_hashes'].items():
            assert sha(project / name) == h, name
        state.update(status='TERMINAL_ASSEMBLED_REVIEWED_STORAGE_BOARD', finished=time.time(),
                     rbf_sha256=sha(rbf), rbf_bytes=rbf.stat().st_size)
        save()
        print(json.dumps(state), flush=True)
    except BaseException:
        state.update(status='TERMINAL_FAILED', finished=time.time())
        save()
        raise


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--project', type=Path, required=True)
    p.add_argument('--quartus', type=Path, default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    p.add_argument('--check-only', action='store_true')
    a = p.parse_args()
    if a.check_only:
        validate(a.project.resolve())
        print('PASS_NATIVE_STORAGE_BOARD_ASSEMBLY_GUARD; no assembly requested')
    else:
        run(a.project.resolve(), a.quartus.resolve())
