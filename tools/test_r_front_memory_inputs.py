#!/usr/bin/env python3
"""Remove unused data dependencies from the current SRAM address interface.

Derive masks from every literal input reference in the two frozen functions.
Reject dynamic indexing or other function bodies. No address/select value,
capture epoch, RAM, observer, or state changes; run the complete original
844-Q/9216-bit/100-clock comparison after projecting the consumed inputs.
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


def memory(raw):
    functions = re.findall(r' function automatic .*?endfunction', raw, re.S)
    assert len(functions) == 2
    assert 'old_address(input [635:0] q,input [201:0] p)' in functions[0]
    assert 'old_select(input [635:0] q,input [201:0] p)' in functions[1]
    used = {'q': set(), 'p': set()}
    for function in functions:
        for name, index in re.findall(r'\b(q|p)\[([^]]+)\]', function):
            assert re.fullmatch(r'\d+', index), ('Nonliteral dependency', name, index)
            used[name].add(int(index))
    assert used['p'] == {172, 173, 174, 175, 176}
    assert used['q'].isdisjoint(set(range(540, 572)) | set(range(100, 116)) | set(range(372, 404)))
    qmask = sum(1 << i for i in used['q'])
    pmask = sum(1 << i for i in used['p'])
    old = 'wire [31:0] previous_address=old_address(state_q,pins_before);\n wire [3:0] previous_select=old_select(state_q,pins_before);'
    new = f'''// The current address functions consume only these original pins/Qs.
 // RAM data and open operand Q cannot feed the SRAM address by sharing a bus.
 wire [635:0] address_q=state_q & 636'h{qmask:x};
 wire [201:0] address_pins=pins_before & 202'h{pmask:x};
 wire [31:0] previous_address=old_address(address_q,address_pins);
 wire [3:0] previous_select=old_select(address_q,address_pins);'''
    return replace(raw, old, new), {n: sorted(v) for n, v in used.items()}


def run(source, out, engine):
    parent = json.loads((source / 'run-state.json').read_text())
    assert parent['status'] == 'PASS_R_FRONT_PHYSICAL_FORWARD' and not parent['mutation']
    assert parent['actions'][-1]['returncode'] == 0
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
        if p.name == 'dorado_local_r_memory_step.sv':
            raw, used = memory(old)
        q.write_text(raw)
        copied.append(q)
        records.append(dict(path=str(rel), parent_sha256=sha(p), output_sha256=sha(q), changed=old != raw))
    assert sum(r['changed'] for r in records) == 1
    (out / 'source-run-state.json').write_bytes((source / 'run-state.json').read_bytes())
    (out / Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state = dict(status='RUNNING', pid=os.getpid(), started=time.time(), engine=engine,
                 parent=str(source), parent_run_state_sha256=sha(source / 'run-state.json'),
                 source_files=records, literal_function_dependencies=used, actions=[],
                 original_reference_unchanged=True, native_timing_qualified=False, assembly_permitted=False)
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
    state.update(status='PASS_R_FRONT_MEMORY_INPUT_PROJECTION' if not rc and m else 'R_FRONT_MEMORY_INPUT_PROJECTION_FAILED',
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
