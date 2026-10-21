#!/usr/bin/env python3
"""Test actual task-selection feedback and processor task/address captures.

88 Qs/25 clock channels, normal/extended running with 20ns boundary settling.
Original gates and event FFs use explicit illustrative arrival delays to avoid
zero-delay simulator races. This is a conditional digital-order contract, not
vendor delay characterization, analog setup/hold or a native 60ns proof.
Ready/IsNext, MIR decode, maintenance and memory-Hold producers remain external;
memory Hold/StopTasks change in the high window; simultaneous CT set/reset is
excluded. T data/RAM/writeback and native integration remain incomplete.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

from generate_task_capture_reference import specs
from sil_netlist import load_board

ROOT = Path(__file__).resolve().parents[1]
CANDIDATE = 'verilog/rtl/dorado_task_capture_step.sv'
TEST = 'verilog/verilator/tb_task_capture_step.sv'
PROFILES = [('balanced', 1000, 1000, 1000),
            ('fast_gates', 500, 1000, 500),
            ('later_control', 1000, 2000, 1000)]
MUTATIONS = {
    'live_task_source': [('captures[13:9],source_p,', 'captures[13:9],p,'),
                         ('captures[18:14],source_p,', 'captures[18:14],p,')],
    'wrong_bnt_hold': [('bnext_enable_n=repeat_cur &&', 'bnext_enable_n=!repeat_cur &&')],
    'ignore_ready': [('pins[34+task_no] && pins[49+task_no]', "1'b0")],
    'invert_ready_mask': [('pins[34+task_no] && pins[49+task_no]', 'pins[34+task_no] && !pins[49+task_no]')],
    'lowest_priority': [('task_no=1;task_no<16;task_no=task_no+1', 'task_no=15;task_no>0;task_no=task_no-1')],
    'swap_next_select': [('q[73] ? ~q[67:64] : current_raw(q)', 'q[73] ? current_raw(q) : ~q[67:64]')],
    'lose_ct_load': [('else if(captures[20])', "else if(1'b0)")],
    'ct_on_normal_clock': [('else if(captures[20])', 'else if(captures[19])')],
    'block_held_priority': [('p[24]=q[75]', "p[24]=1'b1")],
    'merge_hold_nets': [('repeat_a=pins[26] & !pins[27]', 'repeat_a=pins[25] & !pins[27]')],
    'invert_switchup': [('p[27]=!q[75]', 'p[27]=q[75]')],
    'address_on_clock1': [('levels[23]=pp_h;', 'levels[23]=pp_h | q[32];')],
    'address_phase_polarity': [('(!state_q[32] ? state_q[47:44] : ~source_p[23:20])', '(state_q[32] ? state_q[47:44] : ~source_p[23:20])')],
    'lose_last_address': [('(!state_q[32] ? state_q[47:44] : ~source_p[23:20])', '~source_p[23:20]')],
    'one_wave': [('parameter integer WAVES=16', 'parameter integer WAVES=1')],
}


def execute(command, cwd, log):
    with log.open('w') as stream:
        code = subprocess.run([str(x) for x in command], cwd=cwd, stdout=stream,
                              stderr=subprocess.STDOUT).returncode
    return dict(command=[str(x) for x in command], cwd=str(cwd),
                returncode=code, log=str(log))


def function(text, name):
    matches = re.findall(r'    function automatic .*?    endfunction', text, re.S)
    found = [x for x in matches if re.search(r'\b' + name + r'\(', x.split('\n')[0])]
    assert len(found) == 1, name
    return found[0]


def run(out, mutations):
    out.mkdir(parents=True, exist_ok=False)
    inputs = out / 'executed-inputs'
    files = [CANDIDATE, TEST, 'verilog/rtl/dorado_processor_capture_step.sv',
             'verilog/verilator/run_control_original_ff.sv',
             'verilog/verilator/processor_capture_original_ff.sv',
             'tools/generate_task_capture_reference.py',
             'tools/generate_processor_capture_reference.py',
             'tools/generate_control_clock_reference.py', 'tools/generate_run_control_reference.py',
             'tools/sil_netlist.py', 'tools/stage_task_capture_timing.py', 'tools/test_task_capture.py']
    kinds = {'MC10210'}
    boards = ['BaseBd-Rev-Am']
    for spec in specs():
        boards.append(spec['bn'])
        board = load_board(str(ROOT / 'chm/sil' / (spec['bn'] + '.dm!1_') / (spec['bn'] + '.wl')))
        kinds.update(board.packages[p]['type'] for p in set(spec['comb']) | set(spec['seq']))
    for board in boards:
        prefix = 'chm/sil/' + board + '.dm!1_/' + board
        files.extend([prefix + '.wl', prefix + '.lc'])
    files.extend('verilog/cells/cell_' + kind + '.v' for kind in sorted(kinds))
    hashes = {}
    for name in sorted(set(files)):
        raw = (ROOT / name).read_bytes()
        path = inputs / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(raw)
        hashes[name] = hashlib.sha256(raw).hexdigest()
    source = (inputs / CANDIDATE).read_text()
    old = (inputs / 'verilog/rtl/dorado_processor_capture_step.sv').read_text()
    for before, after in [('controller_clocks', 'controller_clocks'),
                          ('controller_transfer', 'controller_transfer'),
                          ('all_clocks', 'capture_clocks'), ('processor_transfer', 'processor_transfer')]:
        assert function(source, after).replace(after, before) == function(old, before), before
    actions = []

    def checked(command, log):
        row = execute(command, inputs, log)
        actions.append(row)
        if row['returncode']:
            raise RuntimeError('Command failed: ' + str(log))

    graph = out / 'reference'
    checked([sys.executable, inputs / 'tools/generate_task_capture_reference.py',
             '--output', graph], out / 'generation.log')
    manifest = json.loads((graph / 'manifest.json').read_text())
    assert (manifest['physical_bits'], manifest['packages'], manifest['original_pins']) == (88, 115, 1102)
    results = []
    baseline = None
    for profile, gates, ffs, phases in PROFILES:
        directory = out / profile
        directory.mkdir()
        timed = directory / 'timed-reference'
        checked([sys.executable, inputs / 'tools/stage_task_capture_timing.py', '--inputs', inputs,
                 '--reference', graph, '--output', timed, '--gate-ps', gates,
                 '--ff-ps', ffs, '--phase-ps', phases], directory / 'staging.log')
        support = [inputs / TEST, *sorted(timed.glob('*.sv')), *sorted(timed.glob('*.v'))]
        cases = [('original', source)]
        if mutations and profile == 'balanced':
            for name, edits in MUTATIONS.items():
                raw = source
                for before, after in edits:
                    assert raw.count(before) == 1, (name, before)
                    raw = raw.replace(before, after)
                cases.append((name, raw))
        for name, raw in cases:
            case = directory / name
            case.mkdir()
            rtl = case / 'dorado_task_capture_step.sv'
            rtl.write_text(raw)
            binary = case / 'test.vvp'
            checked(['iverilog', '-g2012', '-s', 'tb_task_capture_step', '-o', binary, rtl, *support], case / 'compile.log')
            row = execute(['vvp', binary], inputs, case / 'test.log')
            actions.append(row)
            text = (case / 'test.log').read_text()
            if name == 'original':
                assert not row['returncode'] and 'TASK_CAPTURE_STEP PASS ' in text, (profile, text)
                summary = text.split('TASK_CAPTURE_STEP PASS ')[1].splitlines()[0]
                if baseline is None:
                    baseline = summary
                assert summary == baseline, (profile, summary, baseline)
                print('TASK_CAPTURE', profile, 'Icarus', summary, flush=True)
            else:
                assert row['returncode'] and re.search(r'FATAL:.*(?:task state mismatch|task solver unsettled/repeated|task clock mismatch|manual hardware wakeup failed)', text), (name, text)
            results.append(dict(profile=profile, case=name, returncode=row['returncode'],
                                expected_failure=name != 'original'))
        verilator = directory / 'verilator'
        verilator.mkdir()
        checked(['verilator', '--binary', '--timing', '--top-module', 'tb_task_capture_step',
                 '-Wno-fatal', '-Wno-PINMISSING', '-Wno-TIMESCALEMOD', '-Wno-MULTIDRIVEN',
                 '--Mdir', verilator / 'obj', '-o', 'test', inputs / CANDIDATE, *support], verilator / 'compile.log')
        checked([verilator / 'obj/test'], verilator / 'test.log')
        assert 'TASK_CAPTURE_STEP PASS ' + baseline in (verilator / 'test.log').read_text()
        print('TASK_CAPTURE', profile, 'Verilator agrees', flush=True)
    for name, digest in hashes.items():
        assert hashlib.sha256((inputs / name).read_bytes()).hexdigest() == digest
    record = dict(status='PASS_TASK_CAPTURE_ARRIVAL_CONTRACT', inputs=hashes,
                  reference=manifest, profiles=PROFILES, cases=results, actions=actions,
                  summary=baseline, physical_bits=88, native_integrated=False,
                  vendor_delays_qualified=False, analog_timing_qualified=False,
                  data_writeback_qualified=False, producer_timing_qualified=False, scope=__doc__)
    (out / 'results.json').write_text(json.dumps(record, indent=2) + '\n')
    print(record['status'], '88 Qs/25 clocks;', len(MUTATIONS) if mutations else 0, 'bad variants rejected')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--mutations', action='store_true')
    args = parser.parse_args()
    run(args.output.resolve(), args.mutations)
