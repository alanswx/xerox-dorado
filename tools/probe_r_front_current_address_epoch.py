#!/usr/bin/env python3
"""Probe current pre-event stack-address data against original Sil.

HM Processor Section pp.10-11 requires unadjusted StkP for reads and normally
writes; ModStkPBeforeW selects the adjusted write address. Test the existing
BEFORE cone's six address D pins at their capture, preserving all other
resolved LOW-window inputs. This is an experiment, not native qualification.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import time

from test_r_front_forward_view import sha
from test_r_front_task_event import replace


def front(raw, mutation=None):
    old = 'wire [201:0] core_after=clock_after ? core_pre : effective_pins(low_source,low_fa);'
    new = '''wire [201:0] window_after=clock_after ? core_pre : effective_pins(low_source,low_fa);
 // Experimental current BEFORE data for the stack-address FF packages.
 // Every other consumer retains its existing resolved LOW-window view.
 wire [201:0] core_after={window_after[201:172],before_source[171:166],window_after[165:0]};'''
    if mutation == 'zero_address':
        new = new.replace('before_source[171:166]', "6'b0")
    return replace(raw, old, new)


def run(source, out, engine, from_forwarding=False, mutation=None):
    parent = json.loads((source / 'run-state.json').read_text())
    if from_forwarding:
        assert parent['status'] == 'PASS_R_FRONT_PHYSICAL_FORWARD' and not parent['mutation']
    else:
        assert parent['status'] == 'PASS_R_FRONT_CONTROL_INPUT_PROJECTION' and parent['complete_control_projection']
    assert parent['engine'] == engine and parent['actions'][-1]['returncode'] == 0
    for row in parent['source_files']:
        assert sha(source / row['path']) == row['output_sha256']
    build = next(a for a in parent['actions'] if a['command'][0] in ['iverilog', 'verilator'])
    paths = [Path(n) for n in build['command'] if n.endswith(('.sv', '.v'))]
    out.mkdir(parents=True, exist_ok=False)
    records, copied = [], []
    for p in paths:
        rel = p.relative_to(source)
        q = out / rel
        q.parent.mkdir(parents=True, exist_ok=True)
        old = p.read_text()
        raw = front(old, mutation) if p.name == 'dorado_local_r_front_step.sv' else old
        if from_forwarding and p.name == 'candidate.sv':
            from test_r_front_control_inputs import candidate
            raw = candidate(old)[0]
        if from_forwarding and p.name == 'dorado_local_r_memory_step.sv':
            from test_r_front_memory_inputs import memory
            raw = memory(old)[0]
        q.write_text(raw)
        copied.append(q)
        records.append(dict(path=str(rel), parent_sha256=sha(p), output_sha256=sha(q), changed=old != raw))
    assert sum(r['changed'] for r in records) == (3 if from_forwarding else 1)
    (out / 'source-run-state.json').write_bytes((source / 'run-state.json').read_bytes())
    (out / Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state = dict(status='RUNNING', pid=os.getpid(), started=time.time(), engine=engine,
                 parent=str(source), parent_run_state_sha256=sha(source / 'run-state.json'),
                 source_files=records, actions=[], original_reference_unchanged=True,
                 from_forwarding=from_forwarding, mutation=mutation, address_data_view="current_before_source",
                 rejected_solver_or_state_checks_deferred=False, native_timing_qualified=False, assembly_permitted=False)
    def save():
        (out / 'run-state.json').write_text(json.dumps(state, indent=2) + '\n')
    defines = [x for x in build['command'] if x.startswith('-D')]
    commands = [
        ['iverilog', '-g2012', '-s', 'tb_r_front_local', *defines, '-o', str(out / 'test.vvp'), *map(str, copied)],
        ['vvp', '-i', str(out / 'test.vvp')]
    ] if engine == 'icarus' else [
        ['verilator', '--binary', '--timing', '-j', '2', '-Wno-fatal', '--top-module', 'tb_r_front_local',
         *defines, '--Mdir', str(out / 'obj'), '-o', 'test', *map(str, copied)],
        [str(out / 'obj/test')]]
    save()
    for i, cmd in enumerate(commands):
        with (out / ('compile.log' if i == 0 else 'test.log')).open('w') as log:
            p = subprocess.Popen(cmd, stdout=log, stderr=subprocess.STDOUT)
            row = dict(command=cmd, pid=p.pid, started=time.time())
            state['actions'].append(row)
            save()
            rc = p.wait()
            row.update(returncode=rc, finished=time.time())
            save()
        if rc:
            break
    text = (out / 'test.log').read_text() if (out / 'test.log').exists() else ''
    m = re.search(r'R_FRONT_LOCAL_PROBE PASS ([^\n]+)', text)
    state.update(status='PASS_R_FRONT_CURRENT_ADDRESS_EPOCH_PROBE' if not rc and m else 'R_FRONT_CURRENT_ADDRESS_EPOCH_PROBE_FAILED',
                 summary=m[1] if m else None, finished=time.time())
    save()
    print(state['status'], flush=True)
    return rc if rc else (0 if m else 1)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--engine', choices=['icarus', 'verilator'], required=True)
    p.add_argument('--from-forwarding', action='store_true')
    p.add_argument('--mutation', choices=['zero_address'])
    a = p.parse_args()
    sys.exit(run(a.source.resolve(), a.output.resolve(), a.engine, a.from_forwarding, a.mutation))
