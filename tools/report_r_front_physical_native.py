#!/usr/bin/env python3
"""Follow a live native builder, then query central/RAM/visible-R timing.

No refit, assembly, constraint change or deployment. Verify that every recorded
builder/stage process has exited before reopening its database. Keep failures.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def run(project, quartus, output):
    assert not output.exists()
    manifest = project / 'r-front-control-native.json'
    original = json.loads(manifest.read_text())
    assert original['physical_Q_export_separate_from_closed_storage']
    assert not original['assembly_permitted'] and original['canonical_visible_R_owners'] == 16
    source = Path(__file__).with_name('audit_r_front_physical_native.tcl')
    output.mkdir(parents=True)
    script = output / source.name
    script.write_bytes(source.read_bytes())
    state = dict(status='WAITING_EXISTING_NATIVE_BUILD', pid=os.getpid(), started=time.time(),
                 project=str(project), script_sha256=sha(script), manifest_sha256=sha(manifest),
                 actions=[], assembly_permitted=False, constraints_changed=False)
    def save():
        temp = output / 'run-state.json.tmp'
        temp.write_text(json.dumps(state, indent=2) + '\n')
        os.replace(temp, output / 'run-state.json')
    save()
    try:
        while True:
            build = json.loads((project / 'run-state.json').read_text())
            pid = build['builder_pid']
            live = Path('/proc') / str(pid)
            if build['status'] != 'RUNNING' and not live.exists():
                break
            assert live.exists(), 'Recorded builder disappeared; inspect, do not restart'
            assert str(project) in (live / 'cmdline').read_bytes().replace(b'\0', b' ').decode()
            time.sleep(10)
        state['original_build_state'] = build
        save()
        for pid in [build['builder_pid']] + [a['pid'] for a in build['stages']]:
            assert not (Path('/proc') / str(pid)).exists(), ('Build stage remains live', pid)
        assert build['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
        assert all(a['returncode'] == 0 for a in build['stages'])
        for n, h in original['source_hashes'].items():
            assert sha(project / n) == h, n
        command = [str(quartus / 'quartus_sta'), '-t', str(script)]
        with (output / 'audit.log').open('x') as log:
            process = subprocess.Popen(command, cwd=project / 'core', stdout=log, stderr=subprocess.STDOUT)
            row = dict(command=command, pid=process.pid, started=time.time())
            state['actions'].append(row)
            state['status'] = 'RUNNING_READ_ONLY_NATIVE_REPORTS'
            save()
            rc = process.wait()
            row.update(returncode=rc, finished=time.time())
            save()
        assert rc == 0, ('Native timing query failed', rc)
        for n, h in original['source_hashes'].items():
            assert sha(project / n) == h, n
        state.update(status='TERMINAL_PHYSICAL_NATIVE_REPORTS_REVIEW_REQUIRED', finished=time.time())
        save()
    except BaseException:
        state.update(status='TERMINAL_FAILED', finished=time.time())
        save()
        raise
    print(state['status'], flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--project', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--quartus', type=Path, default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a = p.parse_args()
    run(a.project.resolve(), a.quartus.resolve(), a.output.resolve())
