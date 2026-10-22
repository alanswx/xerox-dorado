#!/usr/bin/env python3
"""Assemble the reviewed disk-window fit; do not program hardware."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time

from test_review_pack_window_native import CASES

GUARD_CASES = {'changed-source', 'changed-report', 'changed-qualification',
               'changed-reviewer', 'nonrejecting-control', 'missing-phase-report',
               'changed-window-manifest'}


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def checked(project, name):
    p = Path(name)
    assert not p.is_absolute() and '..' not in p.parts
    return project / p


def validate(project):
    q = json.loads((project / 'pack-preassembly-qualification.json').read_text())
    c = json.loads((project / 'pack-fit-review-controls.json').read_text())
    r = json.loads((project / 'pack-window-native.json').read_text())
    assert q['status'] == 'PASS_NATIVE_PACK_WINDOW_PREASSEMBLY_REVIEW'
    assert c['status'] == 'PASS_NATIVE_PACK_WINDOW_REVIEW_REJECTION_CONTROLS'
    assert c['positive_qualification_sha256'] == sha(project / 'pack-preassembly-qualification.json')
    assert c['original_evidence_unchanged']
    assert {x['name'] for x in c['cases']} == set(CASES) and len(c['cases']) == len(CASES)
    assert all(x['rejected'] and x['returncode'] != 0 for x in c['cases'])
    assert 'tools/review_pack_window_native.py' in c['reviewer_dependencies']
    for n, h in c['reviewer_dependencies'].items():
        assert sha(checked(project / 'review-dependencies', n)) == h, n
    assert c['recipe_config_sha256'] == q['recipe_config_sha256']
    assert q['frequency_hz'] == r['frequency_hz'] == 57142857
    assert q['sysper'] == r['sysper'] == 4 and q['state_admin'] == r['state_admin'] == 'INACTIVE'
    assert not q['firmware_qualified'] and not q['hardware_speedup_qualified']
    assert q['source_hashes'] == r['source_hashes'] and len(q['source_hashes']) == 503
    assert q['sample_registers'] == 2048 and q['ready_registers'] == 8
    assert len(q['timing_checks']) == 31 and all(float(s) > 0 for _, s in q['timing_checks'])
    assert len(q['detailed_paths']) == 332 and all(x['paths'] > 0 and x['slack_ns'] >= 0 for x in q['detailed_paths'])
    assert q['minimum_setup_ns'] > 0 and q['minimum_hold_ns'] > 0
    assert q['resources']['ram_blocks'] == 332 and q['resources']['dsp_blocks'] == 35
    assert q['memory']['cache']['packing'] == 'independent40'
    assert q['memory']['cache']['unique_m10ks'] == 40 and q['memory']['cache']['logical_bits'] == 294912
    assert q['command_encoding'] == dict(MODE='000', REFRESH='001', PRECHARGE='010', ACTIVE='011', WRITE='100', READ='101', NOP='111')
    for n, h in {**q['source_hashes'], **q['reviewed_evidence_hashes']}.items():
        assert sha(checked(project, n)) == h, ('Reviewed source/report changed', n)
    for n, pid_key, rows in [('run-state.json', 'builder_pid', 'stages'), ('storage-ready-reports-v1/run-state.json', 'driver_pid', 'actions')]:
        s = json.loads((project / n).read_text())
        assert s['status'].startswith('TERMINAL_') and all(x['returncode'] == 0 for x in s[rows])
        for pid in [s[pid_key]] + [x['pid'] for x in s[rows]]:
            try:
                os.kill(pid, 0)
            except ProcessLookupError:
                continue
            raise RuntimeError('Recorded process still present; inspect ' + str(pid))
    return q, c


def run(project, quartus):
    q, c = validate(project)
    t = json.loads((project / 'pack-assembly-guard-controls.json').read_text())
    assert t['status'] == 'PASS_NATIVE_PACK_WINDOW_ASSEMBLY_GUARD_CONTROLS'
    assert t['assembler_sha256'] == sha(Path(__file__))
    assert t['qualification_sha256'] == sha(project / 'pack-preassembly-qualification.json')
    assert t['reviewer_controls_sha256'] == sha(project / 'pack-fit-review-controls.json')
    assert {x['name'] for x in t['cases']} == GUARD_CASES and len(t['cases']) == len(GUARD_CASES)
    assert all(x['rejected'] for x in t['cases']) and t['original_evidence_unchanged'] and not t['Quartus_executed']
    path, rbf = project / 'pack-window-assembly-state.json', project / 'core/output_files/Dorado.rbf'
    assert not path.exists() and not rbf.exists(), 'Observe existing assembly; never restart'
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(),
                 qualification_sha256=sha(project / 'pack-preassembly-qualification.json'),
                 controls_sha256=sha(project / 'pack-fit-review-controls.json'), firmware_qualified=False, hardware_deployed=False)

    def save():
        temp = path.with_suffix('.json.tmp')
        temp.write_text(json.dumps(state, indent=2) + '\n')
        os.replace(temp, path)

    save()
    try:
        with (project / 'pack-window-asm.log').open('x') as log:
            command = [str(quartus / 'quartus_asm'), '--read_settings_files=on', '--write_settings_files=off', 'Dorado']
            p = subprocess.Popen(command, cwd=project / 'core', stdout=log, stderr=subprocess.STDOUT)
            state.update(assembler_pid=p.pid, command=command);save()
            state['returncode'] = p.wait();save()
            assert state['returncode'] == 0
        assert 'Assembler was successful' in (project / 'pack-window-asm.log').read_text()
        assert rbf.stat().st_size > 1000000
        assert all(sha(project / n) == h for n, h in q['source_hashes'].items())
        state.update(status='TERMINAL_ASSEMBLED_REVIEWED_PACK_WINDOW', finished=time.time(), rbf_sha256=sha(rbf), rbf_bytes=rbf.stat().st_size)
        save();print(json.dumps(state), flush=True)
    except BaseException:
        state.update(status='TERMINAL_FAILED', finished=time.time());save();raise


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--project', type=Path, required=True)
    p.add_argument('--quartus', type=Path, default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    p.add_argument('--check-only', action='store_true')
    a = p.parse_args()
    if a.check_only:
        validate(a.project.resolve());print('PASS_NATIVE_PACK_WINDOW_ASSEMBLY_GUARD; no assembly requested')
    else:
        run(a.project.resolve(), a.quartus.resolve())
