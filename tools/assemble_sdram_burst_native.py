#!/usr/bin/env python3
"""Assemble the reviewed raw-BL8 native candidate; never program hardware."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def reviewer_sources(project, controls):
    reviewer = project / 'review_sdram_burst_native.py'
    assert controls['reviewer_sha256'] == sha(reviewer)
    if 'from storage_capture_source import' in reviewer.read_text():
        assert controls.get('reviewer_dependencies') == {'storage_capture_source.py': sha(project / 'storage_capture_source.py')}, 'Reviewed capture-source helper changed'


def run(project, quartus):
    q = json.loads((project / 'preassembly-qualification.json').read_text())
    c = json.loads((project / 'burst-fit-review-controls.json').read_text())
    record = json.loads((project / 'sdram-burst-native.json').read_text())
    assert q['status'] == 'PASS_NATIVE_SDRAM_BURST_PREASSEMBLY_REVIEW'
    assert c['status'] == 'PASS_NATIVE_BURST_REVIEW_REJECTION_CONTROLS'
    reviewer_sources(project, c)
    has_observer = q.get('passive_boot_fault_observer', False)
    assert has_observer == record.get('passive_boot_fault_observer', False)
    has_selftest = q.get('raw_storage_selftest', False)
    assert has_selftest == record.get('raw_storage_selftest', False)
    has_trace = q.get('raw_burst_observer', False)
    assert has_trace == record.get('raw_burst_observer', False)
    has_correct_capture=q.get('corrected_sdram_read_capture',False)
    assert has_correct_capture==record.get('corrected_sdram_read_capture',False)
    assert len(c['cases']) == (18 if has_correct_capture else 16 if has_trace else 13 if has_selftest else 10 if has_observer else 7) and all(row['rejected'] and row['returncode'] != 0 for row in c['cases'])
    if has_observer:
        assert len(q['observer_paths']) == 8 and q['observer_component']
    if has_selftest:
        assert q['selftest_component'] and q['selftest_full_component'] and len(q['selftest_paths']) == 24 and q['selftest_registers']
    if has_trace:
        assert len(q['burst_observer_paths']) == 8 and q['burst_observer_component'] and q['burst_observer_full_component']
    if has_correct_capture:assert q['read_capture_qualification']
    assert len(q['timing_checks']) == 31 and len(q['detailed_paths']) == 196 and len(q['burst_paths']) == 56
    assert q['minimum_slack_ns'] > 0 and q['resources']['ram_blocks'] == 332
    assert q['frequency_mhz'] == 50 and q['sysper'] == 4 and q['state_admin'] == 'INACTIVE'
    assert not q['firmware_qualified'] and not q['hardware_speedup_qualified']
    assert q['source_hashes'] == record['source_hashes'] and len(q['source_hashes']) == 499 + int(has_observer) + int(has_selftest)
    for name, digest in {**q['source_hashes'], **q['reviewed_evidence_hashes']}.items():
        assert sha(project / name) == digest, ('Reviewed source/report changed', name)
    for name, pid_key, children in [('run-state.json', 'builder_pid', 'stages'),
                                     ('burst-review-v2-state.json', 'driver_pid', 'actions')]:
        state = json.loads((project / name).read_text())
        assert state['status'].startswith('TERMINAL_') and all(r['returncode'] == 0 for r in state[children])
        for pid in [state[pid_key]] + [r['pid'] for r in state[children]]:
            try:
                os.kill(pid, 0)
            except ProcessLookupError:
                continue
            raise RuntimeError('Recorded process still present; inspect ' + str(pid))
    path = project / 'burst-assembly-state.json'
    assert not path.exists(), 'Observe the existing assembly job'
    rbf = project / 'core/output_files/Dorado.rbf'
    assert not rbf.exists(), 'Inspect an existing image before assembly'
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(),
                 qualification_sha256=sha(project / 'preassembly-qualification.json'),
                 controls_sha256=sha(project / 'burst-fit-review-controls.json'))
    def save():
        path.write_text(json.dumps(state, indent=2) + '\n')
    save()
    try:
        with (project / 'burst-asm.log').open('x') as log:
            child = subprocess.Popen([str(quartus / 'quartus_asm'), 'Dorado'], cwd=project / 'core', stdout=log, stderr=subprocess.STDOUT)
            state['assembler_pid'] = child.pid
            save()
            code = child.wait()
            state['returncode'] = code
            save()
            assert code == 0
        assert 'Assembler was successful' in (project / 'burst-asm.log').read_text()
        assert rbf.stat().st_size > 1000000
        state.update(status='TERMINAL_ASSEMBLED_REVIEWED_SDRAM_BURST', finished=time.time(),
                     rbf_sha256=sha(rbf), rbf_bytes=rbf.stat().st_size,
                     firmware_qualified=False, hardware_deployed=False)
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
    args = p.parse_args()
    run(args.project.resolve(), args.quartus.resolve())
