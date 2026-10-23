#!/usr/bin/env python3
"""Expose all eight original l06/l07 HIGH-transparent R flag latch outputs.

Thread actual existing after-clock levels out of their owner adapters; retain
closed values in the same eight bits and expose physical open Q. Preserve
the full 844-Q/RAM/clock/functional oracle and add original pin checks. No
native integration, timing exception, extra state/reset or guest cycle.
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

ROOT = Path(__file__).resolve().parents[1]


def candidate(raw, mutation=None):
    old = ' assign state_q={held_q[843:572],current_read_q,held_q[539:404],current_t_visible,'
    new = ''' // MC1668 l07 and l06: HIGH-transparent data, closed Set (R tied LOW).
 // Motorola1978 PDF235/4-30; boundary pins remain frozen during owner pause.
 wire [1:0] physical_forward;
 wire [7:0] current_r_flag_clocks,r_flag_clocks_after,r_flag_sets_after,current_r_flag_data,current_r_flag_sets;
 wire [7:0] current_r_flags,closed_r_flags_next;
 wire [1:0] current_r_md={held_q[51],held_q[34]};
 assign current_r_flag_clocks={
  (!current_read_n[1] && physical_forward[0] && !current_r_md[1]),
  (!current_read_n[1] && physical_forward[0] && current_r_md[1]),
  (!current_read_n[0] && physical_forward[0] && !current_r_md[0]),
  (!current_read_n[0] && physical_forward[0] && current_r_md[0]),
  ~current_read_n[1],!(current_read_n[1]|physical_forward[0]),
  ~current_read_n[0],!(current_read_n[0]|physical_forward[0])};
 assign current_r_flag_sets={
  (!current_read_n[1] && (!physical_forward[0] || current_r_md[1])),
  (!current_read_n[1] && (!physical_forward[0] || !current_r_md[1])),
  (!current_read_n[0] && (!physical_forward[0] || current_r_md[0])),
  (!current_read_n[0] && (!physical_forward[0] || !current_r_md[0])),
  1'b0,(!current_read_n[1] && physical_forward[0]),
  1'b0,(!current_read_n[0] && physical_forward[0])};
 assign current_r_flag_data={pins_before[91],pins_before[107],pins_before[76],pins_before[92],
  (physical_forward[0] && !pins_before[151]),file_read_data[15],
  (physical_forward[0] && !pins_before[149]),file_read_data[0]};
 dorado_r_flag_window_step l07_windows(held_q[591:588],current_r_flag_clocks[3:0],r_flag_clocks_after[3:0],
  current_r_flag_data[3:0],current_r_flag_sets[3:0],r_flag_sets_after[3:0],closed_r_flags_next[3:0],current_r_flags[3:0],);
 dorado_r_flag_window_step l06_windows(held_q[595:592],current_r_flag_clocks[7:4],r_flag_clocks_after[7:4],
  current_r_flag_data[7:4],current_r_flag_sets[7:4],r_flag_sets_after[7:4],closed_r_flags_next[7:4],current_r_flags[7:4],);
 assign state_q={held_q[843:596],current_r_flags,held_q[587:572],current_read_q,held_q[539:404],current_t_visible,'''
    raw = replace(raw, old, new)
    raw = replace(raw, 'wire [1:0] child_forward,physical_forward;', 'wire [1:0] child_forward;')
    raw = replace(raw, 'assign held_next={state_next[843:572],closed_read_next,state_next[539:404],closed_t_visible_next,',
                  'assign held_next={state_next[843:596],closed_r_flags_next,state_next[587:572],closed_read_next,state_next[539:404],closed_t_visible_next,')
    raw = replace(raw, 'child_source,child_fa,child_ready,physical_forward);',
                  'child_source,child_fa,child_ready,physical_forward,r_flag_clocks_after,r_flag_sets_after);')
    if mutation == 'omit_flag_close':
        for field, held in [('3:0', '591:588'), ('7:4', '595:592')]:
            old = f'r_flag_sets_after[{field}],closed_r_flags_next[{field}],current_r_flags[{field}],);'
            new = f'r_flag_sets_after[{field}],,current_r_flags[{field}],);\n assign closed_r_flags_next[{field}]=held_q[{held}];'
            raw = replace(raw, old, new)
    elif mutation == 'hide_open_flag':
        raw = replace(raw, 'held_q[843:596],current_r_flags,held_q[587:572]',
                      'held_q[843:596],held_q[595:588],held_q[587:572]')
    return raw


def adapter(raw, name):
    end = raw.index(');', raw.index('module '))
    width = 4 if name == 'dorado_local_r_operand_step.sv' else 8
    raw = raw[:end] + f',output wire [{width-1}:0] r_flag_clocks_after,r_flag_sets_after' + raw[end:]
    if name == 'dorado_local_r_operand_step.sv':
        return replace(raw, 'assign clock_events[28:25]=new_r_clocks & ~old_r_clocks;',
                       '''assign r_flag_clocks_after=new_r_clocks;
 assign r_flag_sets_after={1'b0,(!r_read_n[1] && pins_after[150]),1'b0,(!r_read_n[0] && pins_after[148])};
 assign clock_events[28:25]=new_r_clocks & ~old_r_clocks;''')
    if name == 'dorado_local_r_flags_step.sv':
        raw = replace(raw, 'wire [591:0] child_next;', 'wire [591:0] child_next;wire [3:0] child_flag_clocks,child_flag_sets;')
        raw = replace(raw, 'r_read_n,r_visible_n,r_flags);', 'r_read_n,r_visible_n,r_flags,child_flag_clocks,child_flag_sets);')
        return replace(raw, 'assign clock_events[32:29]=new_clocks & ~old_clocks;',
                       '''assign r_flag_clocks_after={new_clocks,child_flag_clocks};
 assign r_flag_sets_after={(!r_read_n[1] && (!new_bypass[1] || new_md[1])),
  (!r_read_n[1] && (!new_bypass[1] || !new_md[1])),
  (!r_read_n[0] && (!new_bypass[0] || new_md[0])),
  (!r_read_n[0] && (!new_bypass[0] || !new_md[0])),child_flag_sets};
 assign clock_events[32:29]=new_clocks & ~old_clocks;''')
    endings = {'dorado_local_r_address_step.sv': 'r_read_n,r_visible_n,r_flags,rm_flags_n);',
               'dorado_local_r_memory_step.sv': 'rm_flags_n,address,select_n,forwarded_bypass);',
               'dorado_local_r_front_step.sv': 'port_select_n,file_read_data,file_read_parity);',
               'dorado_local_r_front_ready_step.sv': 'child_source,fa_n,current_forwarded_bypass);'}
    old = endings[name]
    return replace(raw, old, old[:-2] + ',r_flag_clocks_after,r_flag_sets_after);')


def bench(raw):
    # Additional pin checks use the MC1668's actual truth-table care: D while
    # open, Set while closed. All existing complete state/oracle checks stay.
    data = '{original.l.l06.p11,original.l.l06.p6,original.h.l06.p11,original.h.l06.p6,original.l.l07.p11,original.l.l07.p6,original.h.l07.p11,original.h.l07.p6}'
    sets = "{original.l.l06.p12,original.l.l06.p5,original.h.l06.p12,original.h.l06.p5,1'b0,original.l.l07.p5,1'b0,original.h.l07.p5}"
    for epoch, marker in [('BEFORE', '     expected=n;predicted=events;'), ('AFTER', '     if(reference!==q) begin')]:
        extra = f'''     if(dut.current_r_flag_clocks!==clocks[32:25])$fatal(1,"R flag C mismatch {epoch} commit check=%0d",checks);
     if(((dut.current_r_flag_data^{data}) & dut.current_r_flag_clocks)!==8'b0)$fatal(1,"R flag D mismatch {epoch} commit check=%0d",checks);
     if(((dut.current_r_flag_sets^{sets}) & ~dut.current_r_flag_clocks)!==8'b0)$fatal(1,"R flag Set mismatch {epoch} commit check=%0d",checks);
'''
        raw = replace(raw, marker, extra + marker)
    return raw


def run(source, out, engine, mutation=None):
    parent = json.loads((source / 'run-state.json').read_text())
    assert parent['status'] == 'PASS_R_FRONT_ADDRESS_EPOCH_PROBE' and not parent.get('mutation')
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
            raw = candidate(old, mutation)
        if p.name in ['dorado_local_r_operand_step.sv', 'dorado_local_r_flags_step.sv', 'dorado_local_r_address_step.sv',
                      'dorado_local_r_memory_step.sv', 'dorado_local_r_front_step.sv', 'dorado_local_r_front_ready_step.sv']:
            raw = adapter(old, p.name)
        if p.name == 'tb_r_front_local.sv':
            raw = bench(old)
        q.write_text(raw); copied.append(q)
        records.append(dict(path=str(rel), parent_sha256=sha(p), output_sha256=sha(q), changed=old != raw))
    assert sum(r['changed'] for r in records) == 8
    helper = out / 'dorado_r_flag_window_step.sv'
    helper.write_bytes((ROOT / 'verilog/rtl/dorado_r_flag_window_step.sv').read_bytes()); copied.append(helper)
    (out / Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state = dict(status='RUNNING', pid=os.getpid(), started=time.time(), engine=engine, mutation=mutation, source_files=records,
                 helper_sha256=sha(helper), parent=str(source), parent_run_state_sha256=sha(source / 'run-state.json'), actions=[],
                 closed_encoding_bits=88, physical_Q_bits=844, RAM_bits=9216, clocks=100,
                 original_reference_unchanged=True, all_original_checks_preserved=True, pin_checks_added=True,
                 actual_after_clock_levels_exported=True, native_qualified=False, assembly_permitted=False)
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
    state.update(status='PASS_R_FRONT_FLAG_WINDOWS' if not rc and m else 'R_FRONT_FLAG_WINDOWS_FAILED', summary=m[1] if m else None, finished=time.time());save()
    print(state['status'], flush=True)
    return rc if rc else (0 if m else 1)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--engine', choices=['icarus', 'verilator'], required=True)
    p.add_argument('--mutation', choices=['omit_flag_close', 'hide_open_flag'])
    a = p.parse_args()
    sys.exit(run(a.source.resolve(), a.output.resolve(), a.engine, a.mutation))
