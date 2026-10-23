#!/usr/bin/env python3
"""Bind folded STK address registers to existing address-owner FF inputs.

Compare physical data/enable/clock drivers, not merely fanin sets or inferred
RAM parameters. Shared inputs establish parallel capture, without another
pipeline stage. This does not qualify power-up state, memory collisions,
physical setup/hold, native boot, snapshots or a faster clock.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import time


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def review(out):
    state = json.loads((out / 'run-state.json').read_text())
    assert state['actions'][-1]['returncode'] == 0
    assert sha(out / 'query.tcl') == state['script_sha256']
    if 'query_log_sha256' in state:
        assert sha(out / 'query.log') == state['query_log_sha256']
    log = (out / 'query.log').read_text()
    assert 'Quartus Prime TimeQuest Timing Analyzer was successful' in log
    rows, current = {}, None
    for line in log.splitlines():
        if line.startswith('STK_QUERY_REGISTER '):
            current = line.removeprefix('STK_QUERY_REGISTER ')
            assert current not in rows
            rows[current] = dict(drivers={}, asynchronous_edges=[])
        match = re.match(r'STK_QUERY_DRIVER (\S+) synch (.+) -> (.+)', line)
        if match:
            role, source, sink = match.groups()
            assert role not in rows[current]['drivers'] and sink.endswith('|' + role)
            rows[current]['drivers'][role] = source
        if line.startswith('STK_QUERY_EDGE asynch '):
            rows[current]['asynchronous_edges'].append(line)
        if line.startswith('STK_QUERY_CELL '):
            rows[current]['cell_type'] = line.rsplit(' ', 1)[1]
    assert len(rows) == int(re.search(r'^STK_QUERY_COUNT (\d+)$', log, re.M)[1])
    owners, memories = {}, []
    for name, row in rows.items():
        assert row['cell_type'] == 'cyclonev_ff' and not row['asynchronous_edges']
        # The fitted FF can use either its D input or synchronous-load data
        # input as the sole data path. Retain the actual pin kind; compare its
        # driver rather than assuming a particular physical packing mode.
        assert set(row['drivers']) in [{'clk', 'ena', 'asdata'}, {'clk', 'ena', 'd'}]
        owner = re.search(r'\|r_front_upper_q\[(\d+)\]$', name)
        if owner:
            owners[int(owner[1])] = dict(name=name, **row)
        else:
            memory = re.search(r'\|altdpram:(stk_[hl])_rtl_0\|.*\|rdaddr_reg\[([0-7])\](~DUPLICATE[0-9]*)?$', name)
            assert memory, name
            memories.append(dict(bank=memory[1], address_bit=int(memory[2]), duplicate=bool(memory[3]), name=name, **row))
    # The two task-bank bits and six stack-address bits are the same for H/L
    # in this inactive native mapping. Source aliases and data epochs are
    # independently compared against the original in the component tests.
    expected = {0: 35, 1: 34, 2: 33, 3: 32, 4: 31, 5: 30, 6: 26, 7: 24}
    assert set(owners) == set(expected.values())
    def signature(row):
        d = row['drivers']
        return dict(clk=d['clk'], ena=d['ena'], data=d.get('asdata', d.get('d')))
    seen, pairs = set(), []
    for memory in memories:
        owner = owners[expected[memory['address_bit']]]
        assert signature(memory) == signature(owner), ('Register inputs differ', memory, owner)
        key = (memory['bank'], memory['address_bit'])
        if not memory['duplicate']:
            assert key not in seen
            seen.add(key)
        pairs.append(dict(bank=memory['bank'], address_bit=memory['address_bit'],
                          register=memory['name'], owner=owner['name'], duplicate=memory['duplicate'],
                          identical_physical_drivers=signature(owner),
                          memory_data_pin='asdata' if 'asdata' in memory['drivers'] else 'd',
                          owner_data_pin='asdata' if 'asdata' in owner['drivers'] else 'd'))
    assert seen == {(bank, bit) for bank in ['stk_h', 'stk_l'] for bit in range(8)}
    result = dict(status='PASS_FITTED_STK_PARALLEL_ADDRESS_REGISTERS', scope=__doc__,
                  compiled_manifest_sha256=state['manifest_sha256'], query_sha256=state['script_sha256'],
                  query_log_sha256=sha(out / 'query.log'),
                  pairs=pairs, primary_address_bits=16, physical_copies=len(memories), owner_FFs=len(owners),
                  extra_pipeline_stage=False, power_up_qualified=False, collision_behavior_qualified=False,
                  physical_setup_hold_qualified=False, native_boot_qualified=False, speedup_qualified=False)
    (out / 'results.json').write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], len(pairs), 'physical copies', flush=True)
    return result


def run(project, quartus, out):
    assert not out.exists()
    build = json.loads((project / 'run-state.json').read_text())
    follow = json.loads((project / 'physical-native-reports-v1/run-state.json').read_text())
    assert build['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    assert follow['status'] == 'TERMINAL_PHYSICAL_NATIVE_REPORTS_REVIEW_REQUIRED'
    pids = [build['builder_pid'], follow['pid']] + [a['pid'] for a in build['stages'] + follow['actions']]
    assert not [pid for pid in pids if (Path('/proc') / str(pid)).exists()], 'Observe the existing jobs until exit'
    manifest = project / 'r-front-control-native.json'
    r = json.loads(manifest.read_text())
    for n, h in r['source_hashes'].items():
        assert sha(project / n) == h, n
    out.mkdir(parents=True)
    script = out / 'query.tcl'
    script.write_bytes(Path(__file__).with_suffix('.tcl').read_bytes())
    state = dict(status='RUNNING', pid=os.getpid(), started=time.time(), actions=[], project=str(project),
                 manifest_sha256=sha(manifest), script_sha256=sha(script), absent_build_pids=pids,
                 assembly_permitted=False, constraints_changed=False)
    def save():
        (out / 'run-state.json').write_text(json.dumps(state, indent=2) + '\n')
    save()
    with (out / 'query.log').open('x') as log:
        cmd = [str(quartus / 'quartus_sta'), '-t', str(script)]
        p = subprocess.Popen(cmd, cwd=project / 'core', stdout=log, stderr=subprocess.STDOUT)
        row = dict(command=cmd, pid=p.pid, started=time.time())
        state['actions'].append(row)
        save()
        rc = p.wait()
        row.update(returncode=rc, finished=time.time())
        save()
    state['query_log_sha256'] = sha(out / 'query.log'); save()
    try:
        assert rc == 0
        for n, h in r['source_hashes'].items():
            assert sha(project / n) == h, n
        review(out)
    except BaseException as error:
        state.update(status='TERMINAL_FAILED', error=repr(error), finished=time.time()); save()
        raise
    state.update(status='TERMINAL_STK_ADDRESS_INPUTS_REVIEWED', finished=time.time())
    save()


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--project', type=Path)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--review', action='store_true')
    p.add_argument('--quartus', type=Path, default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a = p.parse_args()
    if a.review:
        review(a.output.resolve())
    else:
        run(a.project.resolve(), a.quartus.resolve(), a.output.resolve())
