#!/usr/bin/env python3
"""Synthesize exact private native sources to inspect structural feedback.

This runs Analysis & Synthesis only on misterubuntu. It cannot fit, assemble,
deploy or qualify physical timing. Preserve all loop/RAM/multiple-driver
diagnostics; successful mapping alone does not classify a warning as safe.
"""
import argparse
import hashlib
import json
from pathlib import Path
import os
import subprocess
import time


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(root, expected, quartus):
    manifest = root / 'r-front-control-native.json'
    assert sha(manifest) == expected
    p = json.loads(manifest.read_text())
    assert p['project'] == str(root)
    assert p['owned_context_specialization_requested'] and not p['assembly_permitted']
    assert p['frequency_hz'] == 57142857 and p['sysper'] == 4
    for name, digest in p['source_hashes'].items():
        assert sha(root / name) == digest, name
    target = root / 'native-map-state.json'
    assert not target.exists(), 'Observe the existing job; do not restart'
    command = [str(quartus), '--read_settings_files=on', '--write_settings_files=off', 'Dorado']
    state = dict(status='RUNNING', scope=__doc__, driver_pid=os.getpid(), started=time.time(),
                 source_manifest_sha256=expected, source_count=len(p['source_hashes']),
                 command=command, native_clock_hz=57142857, sysper=4,
                 fitting_permitted=False, assembly_permitted=False, hardware_changed=False,
                 physical_timing_qualified=False, feedback_safety_qualified=False)
    def save():
        target.write_text(json.dumps(state, indent=2) + '\n')
    save()
    with (root / 'native-map.log').open('x') as log:
        process = subprocess.Popen(command, cwd=root / 'core', stdout=log, stderr=subprocess.STDOUT)
        state['pid'] = process.pid
        save()
        code = process.wait()
    changed = [name for name, digest in p['source_hashes'].items() if sha(root / name) != digest]
    state.update(status='TERMINAL_NATIVE_SYNTHESIS_REVIEW_REQUIRED' if code == 0 and not changed else
                 'TERMINAL_NATIVE_SYNTHESIS_FAILED', returncode=code, source_changes=changed,
                 log_sha256=sha(root / 'native-map.log'), finished=time.time())
    save()
    print(state['status'], flush=True)
    return 0 if code == 0 and not changed else 1


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--run', type=Path, required=True)
    p.add_argument('--expected-manifest', required=True)
    p.add_argument('--quartus-map', type=Path,
                   default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin/quartus_map'))
    a = p.parse_args()
    raise SystemExit(run(a.run.resolve(), a.expected_manifest, a.quartus_map.resolve()))
