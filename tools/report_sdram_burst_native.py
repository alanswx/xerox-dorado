#!/usr/bin/env python3
"""Report actual fitted BL8 registers/paths after the native builder ends.

Read-only Quartus analysis; no new constraints, assembly or programming.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time


def run(project, quartus, attempt='burst-review'):
    assert attempt in ('burst-review', 'burst-review-v2')
    source = json.loads((project / 'sdram-burst-native.json').read_text())
    for name, digest in source['source_hashes'].items():
        assert hashlib.sha256((project / name).read_bytes()).hexdigest() == digest, name
    previous = json.loads((project / 'run-state.json').read_text())
    assert previous['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    assert len(previous['stages']) == 8 and all(r['returncode'] == 0 for r in previous['stages'])
    for pid in [previous['builder_pid']] + [r['pid'] for r in previous['stages']]:
        try:
            os.kill(pid, 0)
        except ProcessLookupError:
            continue
        raise RuntimeError('Existing builder/stage present; observe it: ' + str(pid))
    path = project / (attempt + '-state.json')
    assert not path.exists(), 'Observe the existing report job'
    tcl = project / ('audit_storage_selftest_native.tcl' if source.get('raw_storage_selftest') else 'audit_boot_fault_native.tcl' if source.get('passive_boot_fault_observer') else 'audit_sdram_burst_native.tcl')
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(),
                 tcl_sha256=hashlib.sha256(tcl.read_bytes()).hexdigest(), actions=[])
    def save():
        path.write_text(json.dumps(state, indent=2) + '\n')
    save()
    try:
        with (project / (attempt + '.log')).open('x') as log:
            command = [str(quartus / 'quartus_sta'), '-t', str(tcl)]
            child = subprocess.Popen(command, cwd=project / 'core', stdout=log, stderr=subprocess.STDOUT)
            row = dict(pid=child.pid, command=command, started=time.time())
            state['actions'].append(row)
            save()
            code = child.wait()
            row.update(returncode=code, finished=time.time())
            save()
            assert code == 0
        state.update(status='TERMINAL_BURST_REPORTS_REVIEW_REQUIRED', finished=time.time())
        save()
    except BaseException:
        state.update(status='TERMINAL_FAILED', finished=time.time())
        save()
        raise


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--project', type=Path, required=True)
    parser.add_argument('--quartus', type=Path, default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    parser.add_argument('--attempt', choices=['burst-review', 'burst-review-v2'], default='burst-review')
    args = parser.parse_args()
    run(args.project.resolve(), args.quartus.resolve(), args.attempt)
