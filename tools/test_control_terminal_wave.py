#!/usr/bin/env python3
"""Compare shortened settling with unchanged complete references and X/Z cases.

Complete mode changes only candidate RTL in the original full graph test.
Four-state mode checks every one of955 inputs with X and Z at four depths,
including both optimized depths and unchanged parameter modes. It compares
all363 outputs and ALL original per-wave diagnostic vectors. Negative
controls corrupt one state bit or the unknown-input fallback independently.
No native clock, firmware, hardware or speed qualification follows.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from shorten_control_terminal_wave import transform, BODY
from prove_cp_prefix_cache import module
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]


def run(source, out, four_state=False, broken=False, bad_unknown=False):
    assert not out.exists() and not (broken and bad_unknown)
    parent = json.loads((source/'run-state.json').read_text())
    assert parent['status'] == 'PASS_CP_PREFIX_CACHE_ORIGINAL_COMPARISON'
    assert parent['balanced_task_priority'] and parent['settling_waves'] == 5
    raw = (source/'candidate.sv').read_text()
    assert not bad_unknown or four_state
    actual = transform(raw, broken, bad_unknown)
    out.mkdir(parents=True)
    paths, records = [], []
    if four_state:
        originals = module(raw, 'dorado_control_hold_atomic_step').replace(
            'module dorado_control_hold_atomic_step ', 'module original_atomic ', 1)
        candidates = module(actual, 'dorado_control_hold_atomic_step').replace(
            'module dorado_control_hold_atomic_step ', 'module candidate_atomic ', 1)
        candidates += '\n'+module(actual, BODY)
        (out/'original-atomic.sv').write_text(originals)
        (out/'candidate-atomic.sv').write_text(candidates)
        shutil.copy2(ROOT/'verilog/verilator/tb_control_terminal_wave.sv', out/'tb.sv')
        paths = [out/'original-atomic.sv', out/'candidate-atomic.sv', out/'tb.sv']
        engine, defines, top = 'icarus', [], 'tb_control_terminal_wave'
    else:
        for row in parent['source_files']:
            p = source/row['path']
            assert sha(p) == row['output_sha256'], row['path']
            target = out/row['path']
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(actual.encode() if p.name == 'candidate.sv' else p.read_bytes())
            paths.append(target)
            records.append(dict(path=row['path'], parent_sha256=sha(p), output_sha256=sha(target),
                                changed=sha(p) != sha(target)))
        assert sum(r['changed'] for r in records) == 1
        assert (out/'inputs/tb_r_front_local.sv').read_bytes() == (source/'inputs/tb_r_front_local.sv').read_bytes()
        engine = parent['engine']
        defines = [s for s in parent['actions'][0]['command'] if s.startswith('-D')]
        top = 'tb_r_front_local'
    (out/'candidate.sv').write_text(actual)
    for name in [Path(__file__).name, 'shorten_control_terminal_wave.py']:
        (out/name).write_bytes((ROOT/'tools'/name).read_bytes())
    if engine == 'icarus':
        commands = [['iverilog', '-g2012', '-s', top, *defines, '-o', str(out/'test.vvp'), *map(str, paths)],
                    ['vvp', '-i', str(out/'test.vvp')]]
    else:
        commands = [['verilator', '--binary', '--timing', '-j', '2', '-Wno-fatal', '--top-module', top,
                     *defines, '--Mdir', str(out/'obj'), '-o', 'test', *map(str, paths)], [str(out/'obj/test')]]
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), scope=__doc__, engine=engine,
        source=str(source), parent_state_sha256=sha(source/'run-state.json'), source_files=records,
        four_state=four_state, negative_control=broken, bad_unknown_fallback=bad_unknown,
        original_testbench_and_references_unchanged=not four_state,
        candidate_sha256=sha(out/'candidate.sv'), actions=[], native_timing_qualified=False,
        firmware_qualified=False, speedup_qualified=False, hardware_changed=False)

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    save()
    for label, command in zip(['compile', 'test'], commands):
        with (out/(label+'.log')).open('x') as log:
            p = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT)
            row = dict(label=label, command=command, pid=p.pid, started=time.time())
            state['actions'].append(row)
            save()
            row.update(returncode=p.wait(), finished=time.time())
            save()
        if row['returncode'] != 0:
            break
    log = (out/'test.log').read_text() if (out/'test.log').exists() else ''
    if broken or bad_unknown:
        diagnostic = r'CONTROL_TERMINAL four-state result mismatch' if four_state else r'R front (?:(?:state|resolved source) mismatch|state commit changed settled Q)'
        passed = state['actions'][0]['returncode'] == 0 and row['returncode'] != 0 and re.search(diagnostic, log) is not None
        status = 'PASS_SHORT_CONTROL_SIMULATOR_COUNTEREXAMPLE'
    else:
        expected = 'CONTROL_TERMINAL_FOUR_STATE_PASS checks=1974 unknown_checks=1910 input_bits=955 waves=4/5/6/24 outputs=363 diagnostics=12/15/18/72' if four_state else 'R_FRONT_LOCAL_PROBE PASS checks=14884 writes=238 reads=6928 epochs=392'
        passed = row['returncode'] == 0 and expected in log
        status = 'PASS_SHORT_CONTROL_FOUR_STATE' if four_state else 'PASS_SHORT_CONTROL_COMPLETE_ORIGINAL_COMPARISON'
        if passed and not four_state:
            old = (source/'test.log').read_text()
            clocks = re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$', log, re.M)
            assert len(clocks) == 100 and clocks == re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$', old, re.M)
            coverage = re.search(r'^CP_PREFIX_CACHE (.+)$', log, re.M)[1]
            assert coverage == parent['cache_coverage']
            state.update(all100_clock_counts_match=True, cache_coverage=coverage)
    state.update(status=status if passed else 'TERMINAL_SHORT_CONTROL_TEST_NOT_ESTABLISHED', finished=time.time(),
        test_log_sha256=sha(out/'test.log') if (out/'test.log').exists() else None,
        files={str(p.relative_to(out)): sha(p) for p in out.rglob('*') if p.is_file()
               and 'obj' not in p.relative_to(out).parts and p.suffix != '.vvp' and p.name != 'run-state.json'})
    save()
    print(state['status'], flush=True)
    return 0 if passed else 1


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['source', 'output']:
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--four-state', action='store_true')
    p.add_argument('--negative-control', action='store_true')
    p.add_argument('--bad-unknown', action='store_true')
    a = p.parse_args()
    raise SystemExit(run(a.source.resolve(), a.output.resolve(), a.four_state, a.negative_control, a.bad_unknown))
