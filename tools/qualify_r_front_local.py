#!/usr/bin/env python3
"""Qualify bounded original Control-A/R-front digital ordering in two engines.

This retains all register/RAM/clock comparisons, maintenance/task commands and
late data/clock epochs. Passing this gate does not qualify native arrivals,
physical timing, original-ROM boot or an FPGA speedup.
"""
import argparse
from concurrent.futures import ThreadPoolExecutor, as_completed
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import threading
import time

from test_r_front_local import CANDIDATE, ROOT, PROFILES


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def qualify(base, out):
    assert not out.exists()
    out.mkdir(parents=True)
    state = dict(status='RUNNING', pid=os.getpid(), started=time.time(), cases=[],
                 native_integrated=False, physical_timing_qualified=False)
    lock = threading.Lock()
    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')
    save()
    cases = [(p, e, None) for p in PROFILES for e in ['icarus', 'verilator']]
    cases += [('balanced', e, m) for e in ['icarus', 'verilator']
              for m in ['stop_data', 'stop_clock', 'branch_data', 'next_macro', 'jcnt']]

    def execute(case):
        profile, engine, mutation = case
        target = out/(profile+'-'+engine+'-'+(mutation or 'original'))
        command = [sys.executable, str(ROOT/'tools/test_r_front_local.py'),
                   '--base', str(base), '--output', str(target), '--profile', profile,
                   '--engine', engine]
        if mutation in ['next_macro', 'jcnt']:
            command += ['--iterations', '128', '--producer-random', '--mutation', mutation]
        elif mutation:
            command += ['--iterations', '0', '--late-tasking', '--mutation', mutation]
        else:
            command += ['--iterations', '1024', '--producer-random', '--cp', '--low-cp',
                        '--late-tasking', '--front-directed']
        with (out/(target.name+'.log')).open('w') as log:
            process = subprocess.Popen(command, cwd=ROOT, stdout=log, stderr=subprocess.STDOUT)
            row = dict(profile=profile, engine=engine, mutation=mutation, directory=target.name,
                       command=command, pid=process.pid, started=time.time())
            with lock:
                state['cases'].append(row)
                save()
            rc = process.wait()
        with lock:
            row.update(returncode=rc, finished=time.time())
            save()
        record = json.loads((target/'run-state.json').read_text())
        assert record['status'] != 'RUNNING' and len(record['actions']) == 2, target
        assert record['actions'][0]['returncode'] == 0, ('compile failure', target)
        for n, h in record['inputs'].items():
            assert sha(target/'inputs'/n) == h, (target, n)
        log = (target/'test.log').read_text()
        if mutation:
            assert rc != 0 and record['status'] == 'R_FRONT_LOCAL_PROBE_FAILED', target
            if mutation in ['next_macro', 'jcnt']:
                assert any(t in log for t in ['R front resolved source mismatch check=65',
                                             'Local front Control-A board crossing mismatch check=65']), target
            else:
                check = {'stop_data': 201, 'stop_clock': 207, 'branch_data': 262}[mutation]
                assert f'R_FRONT_LOCAL_SOLVER check={check} shared=1' in log, target
                assert 'R front solver unsettled/repeated' in log, target
            row['qualification'] = 'REJECTED_DAMAGED_CANDIDATE'
        else:
            assert rc == 0 and record['status'] == 'PASS_R_FRONT_LOCAL_PROBE', target
            assert record['executed_candidate_sha256'] == sha(ROOT/CANDIDATE), target
            for marker in ['R_FRONT_LOCAL_LATE_TASKING old_data=1 root=0 switch=1 tasking_n=0',
                           'R_FRONT_LOCAL_LATE_BRANCH old_stretch=1 root=0 StartCycle_c=0',
                           'R_FRONT_LOCAL_DIRECTED RBase=32 StkP=256']:
                assert marker in log, (target, marker)
            clocks = re.findall(r'^R_FRONT_LOCAL_CLOCK channel=(\d+) edges=(\d+)$', log, re.M)
            assert len(clocks) == 68 and {int(c) for c, n in clocks} == set(range(68)), target
            assert all(int(n) > 0 for c, n in clocks), (target, clocks)
            coverage = re.fullmatch(r'checks=(\d+) writes=(\d+) reads=(\d+) epochs=(\d+)', record['summary'])
            assert coverage and all(int(n) > threshold for n, threshold in zip(coverage.groups(), [10000, 100, 1000, 100])), target
            assert 'Ready=ffff' in log, target
            row.update(qualification='PASS_BOUNDED_ORIGINAL_ORDERING', summary=record['summary'], clocks=clocks)
        row['run_state_sha256'] = sha(target/'run-state.json')
        with lock:
            save()
        print(row['qualification'], target.name, flush=True)
        return row

    failures = []
    with ThreadPoolExecutor(max_workers=3) as pool:
        futures = {pool.submit(execute, c): c for c in cases}
        for future in as_completed(futures):
            try:
                future.result()
            except Exception as error:
                failures.append(dict(case=futures[future], error=repr(error)))
                print('QUALIFICATION_FAILURE', futures[future], repr(error), flush=True)
    positives = [r for r in state['cases'] if r.get('qualification') == 'PASS_BOUNDED_ORIGINAL_ORDERING']
    negatives = [r for r in state['cases'] if r.get('qualification') == 'REJECTED_DAMAGED_CANDIDATE']
    for profile in PROFILES:
        pair = [r for r in positives if r['profile'] == profile]
        if len(pair) != 2 or pair[0]['summary'] != pair[1]['summary'] or pair[0]['clocks'] != pair[1]['clocks']:
            failures.append(dict(profile=profile, error='Engine coverage/capture counts disagree'))
    state.update(status='R_FRONT_LOCAL_ORDERING_FAILED' if failures else 'PASS_BOUNDED_R_FRONT_LOCAL_ORDERING',
                 finished=time.time(), failures=failures, primary=positives, negative_cases=negatives,
                 physical_bits=792, ram_bits=9216, clocks=68,
                 candidate_sha256=sha(ROOT/CANDIDATE),
                 full_producer_arrival_contract_qualified=False, hardware_speedup_claimed=False)
    save()
    (out/'results.json').write_text(json.dumps(state, indent=2)+'\n')
    print(state['status'], len(positives), len(negatives), flush=True)
    return 1 if failures else 0


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--base', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    sys.exit(qualify(a.base.resolve(), a.output.resolve()))
