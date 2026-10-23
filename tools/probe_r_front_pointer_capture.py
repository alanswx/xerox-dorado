#!/usr/bin/env python3
"""Probe old control/current external data at the h14/l14 capture boundary.

The native failures contain a serial path through newly resolved MIR, pointer
next state and bypass flags. Test a separate data view for only the two
MC10176 pointer packages. Keep resolved clocks and all transparent windows.
The unmodified original pin/state/RAM/clock oracle decides whether this epoch
is correct. No diagnostic deferral or native qualification.
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


def candidate(raw):
    marker = ' wire [3:0] child_parity_ok,child_faults;'
    added = ''' // Experimental data-only epoch: old stored MIR/CP/selectors with
 // current external data. This does not drive clocks or transparent windows.
 wire [245:0] front_capture_pins=child_pins(producer_pins(joint_q,pins_after),cq[75:64],cq[97:76],cq);
'''
    raw = replace(raw, marker, added + marker)
    return replace(raw, 'child_source,child_fa,child_ready,physical_forward,r_flag_clocks_after,r_flag_sets_after);',
                   'child_source,child_fa,child_ready,physical_forward,r_flag_clocks_after,r_flag_sets_after,front_capture_pins);')


def adapter(raw, name, mutation):
    end = raw.index(');', raw.index('module '))
    width = 246 if name == 'dorado_local_r_front_ready_step.sv' else 236
    raw = raw[:end] + f',input wire [{width-1}:0] front_capture_pins' + raw[end:]
    if name == 'dorado_local_r_front_ready_step.sv':
        return replace(raw, 'child_source,fa_n,current_forwarded_bypass,r_flag_clocks_after,r_flag_sets_after);',
                       'child_source,fa_n,current_forwarded_bypass,r_flag_clocks_after,r_flag_sets_after,front_capture_pins[235:0]);')
    marker = ' wire [63:0] phase_next;'
    added = ''' wire [43:0] captured_front_data;
 dorado_r_front_cone capture_data_view(.state_q(state_q),.base(clock_before),.pins(front_capture_pins),
  .front_data(captured_front_data));
'''
    raw = replace(raw, marker, added + marker)
    for field, value in [('73:68', '17:12'), ('79:74', '23:18')]:
        expression = f'captured_front_data[{value}]'
        if mutation == 'zero_pointer':
            expression = "6'b0"
        elif mutation == 'invert_pointer':
            expression = '~' + expression
        raw = replace(raw, f'front_next[{field}]=pre_data[{value}];', f'front_next[{field}]={expression};')
    return raw


def run(source, out, engine, mutation=None):
    parent = json.loads((source / 'run-state.json').read_text())
    assert parent['status'] == 'PASS_R_FRONT_FLAG_WINDOWS' and not parent.get('mutation')
    assert parent['engine'] == engine and parent['actions'][-1]['returncode'] == 0
    for row in parent['source_files']:
        assert sha(source / row['path']) == row['output_sha256']
    assert sha(source / 'dorado_r_flag_window_step.sv') == parent['helper_sha256']
    build = next(a for a in parent['actions'] if a['command'][0] in ['iverilog', 'verilator'])
    paths = [Path(n) for n in build['command'] if n.endswith(('.sv', '.v'))]
    out.mkdir(parents=True, exist_ok=False)
    records, copied = [], []
    for p in paths:
        rel = p.relative_to(source);q = out / rel;q.parent.mkdir(parents=True, exist_ok=True)
        old = p.read_text();raw = old
        if p.name == 'candidate.sv':
            raw = candidate(old)
        if p.name in ['dorado_local_r_front_ready_step.sv', 'dorado_local_r_front_step.sv']:
            raw = adapter(old, p.name, mutation)
        q.write_text(raw);copied.append(q)
        records.append(dict(path=str(rel), parent_sha256=sha(p), output_sha256=sha(q), changed=old != raw))
    assert sum(r['changed'] for r in records) == 3
    (out / Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state = dict(status='RUNNING', pid=os.getpid(), started=time.time(), engine=engine, source_files=records,
                 parent=str(source), parent_run_state_sha256=sha(source / 'run-state.json'), actions=[], mutation=mutation,
                 original_reference_unchanged=True, all_checks_unchanged=True, twelve_capture_data_bits_changed=True,
                 transparent_window_data_unchanged=True, native_qualified=False, assembly_permitted=False)
    def save():
        (out / 'run-state.json').write_text(json.dumps(state, indent=2) + '\n')
    defines = [x for x in build['command'] if x.startswith('-D')]
    commands = [['iverilog', '-g2012', '-s', 'tb_r_front_local', *defines, '-o', str(out / 'test.vvp'), *map(str, copied)],
                ['vvp', '-i', str(out / 'test.vvp')]] if engine == 'icarus' else [
        ['verilator', '--binary', '--timing', '-j', '2', '-Wno-fatal', '--top-module', 'tb_r_front_local',
         *defines, '--Mdir', str(out / 'obj'), '-o', 'test', *map(str, copied)], [str(out / 'obj/test')]]
    save()
    for i, cmd in enumerate(commands):
        with (out / ('compile.log' if i == 0 else 'test.log')).open('w') as log:
            p = subprocess.Popen(cmd, stdout=log, stderr=subprocess.STDOUT)
            row = dict(command=cmd, pid=p.pid, started=time.time());state['actions'].append(row);save()
            rc = p.wait();row.update(returncode=rc, finished=time.time());save()
        if rc:
            break
    text = (out / 'test.log').read_text() if (out / 'test.log').exists() else ''
    m = re.search(r'R_FRONT_LOCAL_PROBE PASS ([^\n]+)', text)
    state.update(status='PASS_R_FRONT_POINTER_CAPTURE_PROBE' if not rc and m else 'R_FRONT_POINTER_CAPTURE_PROBE_FAILED',
                 summary=m[1] if m else None, finished=time.time());save()
    print(state['status'], flush=True)
    return rc if rc else (0 if m else 1)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--engine', choices=['icarus', 'verilator'], required=True)
    p.add_argument('--mutation', choices=['zero_pointer', 'invert_pointer'])
    a = p.parse_args()
    sys.exit(run(a.source.resolve(), a.output.resolve(), a.engine, a.mutation))
