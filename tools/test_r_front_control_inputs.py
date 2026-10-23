#!/usr/bin/env python3
"""Keep current control producers independent of unrelated open operand Q.

The selected control bits are identical in held_q and physical state_q. Use
their owner directly, so a packed physical-Q export cannot introduce a false
RAM-data dependency into the address/Ready control functions. Preserve every
physical operand consumer and all original-reference comparisons.
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


def candidate(raw, complete=True):
    projection = '''assign state_q={held_q[843:572],current_read_q,held_q[539:404],current_t_visible,
   current_t_inputs[31:16],held_q[371:116],current_t_inputs[15:0],held_q[99:0]};'''
    assert projection in raw
    old = 'wire [109:0] cq={state_q[797:792],state_q[791:786],state_q[785:764],state_q[763:752],state_q[751:736],state_q[79:64],state_q[31:0]};'
    indices = set()
    for hi, lo in re.findall(r'state_q\[(\d+):(\d+)\]', old):
        indices.update(range(int(lo), int(hi) + 1))
    indices.add(75)
    physical_diff = set(range(100, 116)) | set(range(372, 404)) | set(range(540, 572))
    assert len(physical_diff) == 80 and not (indices & physical_diff)
    new = '''// Control uses only owner bits that are unchanged by the open-Q
 // projection. Do not couple Ready/address functions to the operand view.
 ''' + old.replace('state_q[', 'held_q[')
    raw = replace(raw, old, new)
    raw = replace(raw, 'r[240]=!(p[69] | mir[17]) | state_q[75];',
                  'r[240]=!(p[69] | mir[17]) | held_q[75];')
    if complete:
        raw = replace(raw, 'wire [155:0] joint_q={state_q[843:798],cq};',
                      'wire [155:0] joint_q={held_q[843:798],cq};')
        indices.update(range(798, 844))
    assert not (indices & physical_diff)
    return raw, sorted(indices)


def run(source, out, engine):
    parent = json.loads((source / 'run-state.json').read_text())
    assert parent['status'] == 'PASS_R_FRONT_MEMORY_INPUT_PROJECTION'
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
        raw = old
        if p.name == 'candidate.sv':
            raw, bits = candidate(old)
        q.write_text(raw)
        copied.append(q)
        records.append(dict(path=str(rel), parent_sha256=sha(p), output_sha256=sha(q), changed=old != raw))
    assert sum(r['changed'] for r in records) == 1
    (out / 'source-run-state.json').write_bytes((source / 'run-state.json').read_bytes())
    (out / Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state = dict(status='RUNNING', pid=os.getpid(), started=time.time(), engine=engine,
                 parent=str(source), parent_run_state_sha256=sha(source / 'run-state.json'),
                 source_files=records, equivalent_held_control_bits=bits, actions=[],
                 complete_control_projection=True,
                 physical_operand_consumers_unchanged=True, original_reference_unchanged=True,
                 native_timing_qualified=False, assembly_permitted=False)
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
    state.update(status='PASS_R_FRONT_CONTROL_INPUT_PROJECTION' if not rc and m else 'R_FRONT_CONTROL_INPUT_PROJECTION_FAILED',
                 summary=m[1] if m else None, finished=time.time())
    save()
    print(state['status'], flush=True)
    return rc if rc else (0 if m else 1)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--engine', choices=['icarus', 'verilator'], required=True)
    a = p.parse_args()
    sys.exit(run(a.source.resolve(), a.output.resolve(), a.engine))
