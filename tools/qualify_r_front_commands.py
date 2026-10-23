#!/usr/bin/env python3
"""Qualify original IM/TPC/count-branch producers in the combined capture graph.

Six illustrative delay profiles and both engines retain the complete state,
RAM and clock comparison. Damaged command sources must fail after compilation.
This does not qualify native producers, physical timing, ROM boot or speed.
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

from generate_r_front_commands_reference import CANDIDATE, ROOT
from test_r_front_local import PROFILES

MUTATIONS = ['count_external', 'read_external', 'wim_external', 'read_new_epoch']


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def qualify(base, out):
    assert not out.exists()
    out.mkdir(parents=True)
    state = dict(status='RUNNING', pid=os.getpid(), started=time.time(), cases=[])
    lock = threading.Lock()
    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')
    save()
    cases = [(p, e, None) for p in PROFILES for e in ['icarus', 'verilator']]
    cases += [('balanced', e, m) for e in ['icarus', 'verilator'] for m in MUTATIONS]

    def execute(case):
        profile, engine, mutation = case
        d = out/(profile+'-'+engine+'-'+(mutation or 'original'))
        command = [sys.executable, str(ROOT/'tools/test_r_front_commands.py'), '--base', str(base),
                   '--output', str(d), '--profile', profile, '--engine', engine, '--directed',
                   '--iterations', '0' if mutation else '1024']
        if mutation:
            command += ['--mutation', mutation]
        with (out/(d.name+'.log')).open('w') as log:
            process = subprocess.Popen(command, cwd=ROOT, stdout=log, stderr=subprocess.STDOUT)
            row = dict(profile=profile, engine=engine, mutation=mutation, directory=d.name,
                       command=command, pid=process.pid, started=time.time())
            with lock:
                state['cases'].append(row)
                save()
            rc = process.wait()
        with lock:
            row.update(returncode=rc, finished=time.time())
            save()
        s = json.loads((d/'run-state.json').read_text())
        assert s['status'] != 'RUNNING' and len(s['actions']) == 2, d
        assert s['actions'][0]['returncode'] == 0, ('compile failure', d)
        for n, h in s['inputs'].items():
            assert sha(d/'inputs'/n) == h, (d, n)
        log = (d/'test.log').read_text()
        if mutation:
            assert rc != 0 and s['status'] == 'R_FRONT_LOCAL_PROBE_FAILED', d
            if mutation in ['read_external', 'read_new_epoch']:
                check = 234 if mutation == 'read_external' else 222
                assert 'R_FRONT_DIFF bit=786' in log and f'R front state mismatch check={check}' in log, d
            elif mutation == 'count_external':
                assert 'Local front Control-A board crossing mismatch' in log, d
            else:
                assert 'shared=1' in log and 'R front solver unsettled/repeated' in log, d
            row['qualification'] = 'REJECTED_DAMAGED_COMMAND_PRODUCER'
        else:
            assert rc == 0 and s['status'] == 'PASS_R_FRONT_LOCAL_PROBE', d
            assert s['executed_candidate_sha256'] == sha(ROOT/CANDIDATE), d
            assert s['summary'] == 'checks=13449 writes=318 reads=5604 epochs=801', d
            assert 'R_FRONT_COMMANDS_DIRECTED commands=8 FF=12 JCN=4 intervals=120 WriteIM=16' in log, d
            assert 'R_FRONT_LOCAL_LATE_TASKING old_data=1 root=0 switch=1 tasking_n=0' in log, d
            assert 'R_FRONT_LOCAL_LATE_BRANCH old_stretch=1 root=0 StartCycle_c=0' in log, d
            assert 'R_FRONT_LOCAL_CHANGED front=fffffffffffffffffffffffff Ready=ffff' in log, d
            clocks = re.findall(r'^R_FRONT_LOCAL_CLOCK channel=(\d+) edges=(\d+)$', log, re.M)
            assert len(clocks) == 68 and {int(c) for c, n in clocks} == set(range(68)), d
            assert all(int(n) > 0 for c, n in clocks), d
            row.update(qualification='PASS_BOUNDED_COMMAND_ORDERING', summary=s['summary'], clocks=clocks)
        row['run_state_sha256'] = sha(d/'run-state.json')
        with lock:
            save()
        print(row['qualification'], d.name, flush=True)

    failures = []
    with ThreadPoolExecutor(max_workers=3) as pool:
        futures = {pool.submit(execute, c): c for c in cases}
        for future in as_completed(futures):
            try:
                future.result()
            except Exception as error:
                failures.append(dict(case=futures[future], error=repr(error)))
                print('QUALIFICATION_FAILURE', futures[future], repr(error), flush=True)
    primary = [r for r in state['cases'] if r.get('qualification') == 'PASS_BOUNDED_COMMAND_ORDERING']
    negative = [r for r in state['cases'] if r.get('qualification') == 'REJECTED_DAMAGED_COMMAND_PRODUCER']
    for p in PROFILES:
        pair = [r for r in primary if r['profile'] == p]
        if len(pair) != 2 or pair[0]['clocks'] != pair[1]['clocks']:
            failures.append(dict(profile=p, error='Engine event counts disagree or result missing'))
    state.update(status='COMMAND_ORDERING_FAILED' if failures else 'PASS_BOUNDED_R_FRONT_COMMAND_ORDERING',
                 finished=time.time(), failures=failures, primary=primary, negative_cases=negative,
                 candidate_sha256=sha(ROOT/CANDIDATE), physical_bits=792, ram_bits=9216, clocks=68,
                 full_producer_arrival_contract_qualified=False, physical_timing_qualified=False,
                 native_integrated=False, hardware_speedup_claimed=False)
    save()
    (out/'results.json').write_text(json.dumps(state, indent=2)+'\n')
    print(state['status'], len(primary), len(negative), flush=True)
    return 1 if failures else 0


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--base', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    sys.exit(qualify(a.base.resolve(), a.output.resolve()))
