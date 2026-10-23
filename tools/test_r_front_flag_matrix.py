#!/usr/bin/env python3
"""Compare physical R flag windows with original Sil across six profiles.

Reuse the two complete balanced passes. Other profiles apply the same proven
flag-window change directly to their qualified address cases, then
run the full original oracle. Freeze all drivers while this matrix is live.
"""
import argparse
from concurrent.futures import ThreadPoolExecutor
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import threading
import time

from test_r_front_task_event_matrix import terminal, PROFILES

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build/rtl-executive'


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def check(d):
    r = json.loads((d / 'run-state.json').read_text())
    assert r['status'] == 'PASS_R_FRONT_FLAG_WINDOWS' and not r.get('mutation')
    assert r['actions'][-1]['returncode'] == 0 and r['original_reference_unchanged']
    assert r['all_original_checks_preserved'] and r['pin_checks_added']
    assert r['actual_after_clock_levels_exported'] and r['closed_encoding_bits']==88
    assert sha(d/'dorado_r_flag_window_step.sv')==r['helper_sha256']
    assert r['summary'] == 'checks=11300 writes=238 reads=4368 epochs=392'
    for row in r['source_files']:
        assert sha(d / row['path']) == row['output_sha256']
    clocks = re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$', (d / 'test.log').read_text(), re.M)
    assert len(clocks) == 100 and all(int(n) > 0 for c, n in clocks)
    return dict(directory=str(d.relative_to(ROOT)), engine=r['engine'], summary=r['summary'], clocks=clocks,
                candidate_sha256=sha(d / 'candidate.sv'), front_sha256=sha(d / 'adapters/dorado_local_r_front_step.sv'),
                run_state_sha256=sha(d / 'run-state.json'))


def run(out):
    parent = BUILD / 'r-front-address-epoch-matrix-v1-20261023'
    assert json.loads((parent / 'run-state.json').read_text())['status'] == 'PASS_R_FRONT_ADDRESS_EPOCH_PROFILE_MATRIX'
    names = ['tools/probe_r_front_flag_windows.py', 'tools/test_r_front_flag_matrix.py',
             'verilog/rtl/dorado_r_flag_window_step.sv', 'tools/test_r_front_task_event.py']
    hashes = {n: sha(ROOT / n) for n in names}
    out.mkdir(parents=True, exist_ok=False)
    state = dict(status='RUNNING', pid=os.getpid(), started=time.time(), source_hashes=hashes,
                 actions=[], cases=[], errors=[], native_integrated=False, physical_timing_qualified=False)
    lock = threading.Lock()
    def save():
        (out / 'run-state.json').write_text(json.dumps(state, indent=2) + '\n')
    def unchanged():
        for n, h in hashes.items():
            assert sha(ROOT / n) == h, ('Frozen driver changed', n)
    save()
    def worker(engine):
        try:
            d = BUILD / f'r-front-flag-windows-{engine}-v3-20261023'
            terminal(d)
            row = check(d)
            row.update(profile='balanced', reused_terminal_pass=True)
            with lock:
                state['cases'].append(row)
                save()
            for profile in PROFILES[1:]:
                unchanged()
                source = parent / f'{profile}-{engine}'
                assert terminal(source)['status'] == 'PASS_R_FRONT_ADDRESS_EPOCH_PROBE'
                d = out / f'{profile}-{engine}'
                cmd = [sys.executable, str(ROOT / 'tools/probe_r_front_flag_windows.py'), '--source', str(source),
                       '--output', str(d), '--engine', engine]
                with (out / (d.name + '.driver.log')).open('w') as log:
                    p = subprocess.Popen(cmd, stdout=log, stderr=subprocess.STDOUT)
                    action = dict(command=cmd, pid=p.pid, started=time.time(), profile=profile, engine=engine)
                    with lock:
                        state['actions'].append(action)
                        save()
                    rc = p.wait()
                with lock:
                    action.update(returncode=rc, finished=time.time())
                    save()
                assert rc == 0, (engine, profile, rc)
                row = check(d)
                row['profile'] = profile
                with lock:
                    state['cases'].append(row)
                    save()
            return True
        except Exception as e:
            with lock:
                state['errors'].append(dict(engine=engine, error=repr(e)))
                save()
            return False
    with ThreadPoolExecutor(max_workers=2) as pool:
        passed = list(pool.map(worker, ['icarus', 'verilator']))
    unchanged()
    if all(passed):
        assert len(state['cases']) == 12
        for profile in PROFILES:
            pair = [r for r in state['cases'] if r['profile'] == profile]
            assert len(pair) == 2 and pair[0]['clocks'] == pair[1]['clocks']
        assert len({r['candidate_sha256'] for r in state['cases']}) == 1
        assert len({r['front_sha256'] for r in state['cases']}) == 1
    state.update(status='PASS_R_FRONT_FLAG_WINDOW_PROFILE_MATRIX' if all(passed) else 'R_FRONT_FLAG_WINDOW_PROFILE_MATRIX_FAILED', finished=time.time())
    save()
    print(state['status'], flush=True)
    return 0 if all(passed) else 1


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    sys.exit(run(a.output.resolve()))
