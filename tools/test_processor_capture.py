#!/usr/bin/env python3
"""Compare 64 original control/processor Qs and 22 capture-clock channels.

Common BaseBoard clock copies have zero modeled skew. External instruction,
task and priority data meets setup; memory Hold and SwitchUp change in the
clocked high window. Their producers, task arbitration, T data/writeback and
analog timing are outside this component. No native FPGA integration claimed.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

from generate_processor_capture_reference import specs
from sil_netlist import load_board

ROOT = Path(__file__).resolve().parents[1]
CANDIDATE = 'verilog/rtl/dorado_processor_capture_step.sv'
TEST = 'verilog/verilator/tb_processor_capture_step.sv'
FF = ['verilog/verilator/run_control_original_ff.sv',
      'verilog/verilator/processor_capture_original_ff.sv']
MUTATIONS = {
    'share_high_t_clock': [('low_half ? captures[2] : captures[3]', 'captures[3]')],
    'share_low_t_clock': [('low_half ? captures[2] : captures[3]', 'captures[2]')],
    'invert_clock1_phase': [('pp_h | q[32]', 'pp_h | !q[32]'),
                            ('pp_l | q[48]', 'pp_l | !q[48]')],
    'merge_hold_nets': [('repeat_a=pins[26] & !pins[27]', 'repeat_a=pins[25] & !pins[27]')],
    'block_priority_yield': [('repeat_curr=pins[25] & pins[24]', 'repeat_curr=pins[25]')],
    'lose_last_capture': [('if(captures[4]) n[15:12]=q[11:8];', 'if(1\'b0) n[15:12]=q[11:8];')],
    'curr_on_clock0': [('if(captures[2] && !repeat_curr)', 'if(captures[3] && !repeat_curr)')],
    'compare_last_task': [('q[11:8] ^ ~pins[23:20]', 'q[15:12] ^ ~pins[23:20]')],
    'lose_tgetsmd': [(" | (pins[19:12]==8'o075)", '')],
    'wrong_rwrite': [('rw_n=!(lc0 | (lc1 & !lc2))', 'rw_n=!(lc0 | (lc1 & lc2))')],
    'low_uses_high_enable': [('en_l=!q[8] | q[10]', 'en_l=!q[13] | q[12]')],
    'one_wave': [('parameter integer WAVES=16', 'parameter integer WAVES=1')],
}


def execute(command, cwd, log):
    with log.open('w') as stream:
        code = subprocess.run([str(x) for x in command], cwd=cwd,
                              stdout=stream, stderr=subprocess.STDOUT).returncode
    return dict(command=[str(x) for x in command], cwd=str(cwd),
                returncode=code, log=str(log))


def check_controller_reuse(source, qualified):
    """Only names may differ in the previously qualified 32-Q functions."""
    for old, new in [('clocks', 'controller_clocks'), ('transfer', 'controller_transfer')]:
        def function(text, name):
            return re.search(r'function automatic .*?\b' + name +
                             r'\(.*?endfunction', text, re.S).group(0).split('function automatic ')[-1]
        actual = function(source, new).replace(new, old)
        assert actual == function(qualified, old), old


def run(out, mutations):
    out.mkdir(parents=True, exist_ok=False)
    inputs = out / 'executed-inputs'
    files = [CANDIDATE, TEST, *FF, 'verilog/rtl/dorado_control_clock_step.sv',
             'tools/generate_processor_capture_reference.py',
             'tools/generate_control_clock_reference.py',
             'tools/generate_run_control_reference.py', 'tools/sil_netlist.py',
             'tools/test_processor_capture.py']
    kinds = {'MC10210'}
    boards = ['BaseBd-Rev-Am']
    for spec in specs():
        boards.append(spec['bn'])
        path = f"chm/sil/{spec['bn']}.dm!1_/{spec['bn']}.wl"
        board = load_board(str(ROOT / path))
        kinds.update(board.packages[p]['type'] for p in set(spec['comb']) | set(spec['seq']))
    for board in boards:
        prefix = f'chm/sil/{board}.dm!1_/{board}'
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
    check_controller_reuse(source, (inputs / 'verilog/rtl/dorado_control_clock_step.sv').read_text())
    actions = []

    def checked(command, log):
        row = execute(command, inputs, log)
        actions.append(row)
        if row['returncode']:
            raise RuntimeError('Command failed: ' + str(log))

    graph = out / 'reference'
    checked([sys.executable, inputs / 'tools/generate_processor_capture_reference.py',
             '--output', graph], out / 'generation.log')
    manifest = json.loads((graph / 'manifest.json').read_text())
    assert (manifest['physical_bits'], manifest['packages'], manifest['original_pins']) == (64, 70, 642)
    cells = {'verilog/cells/cell_MC10210.v'}
    for board in manifest['boards']:
        cells.update(board['combinational_sources'])
    support = [inputs / TEST, *[inputs / name for name in FF],
               *sorted(graph.glob('*.sv')), *[inputs / name for name in sorted(cells)]]
    cases = [('original', source)]
    if mutations:
        for name, edits in MUTATIONS.items():
            raw = source
            for before, after in edits:
                assert raw.count(before) == 1, (name, before)
                raw = raw.replace(before, after)
            cases.append((name, raw))
    results = []
    for name, raw in cases:
        directory = out / name
        directory.mkdir()
        rtl = directory / 'dorado_processor_capture_step.sv'
        rtl.write_text(raw)
        binary = directory / 'test.vvp'
        checked(['iverilog', '-g2012', '-s', 'tb_processor_capture_step', '-o',
                 binary, rtl, *support], directory / 'compile.log')
        row = execute(['vvp', binary], inputs, directory / 'test.log')
        actions.append(row)
        text = (directory / 'test.log').read_text()
        if name == 'original':
            assert not row['returncode'] and 'PROCESSOR_CAPTURE_STEP PASS ' in text, text
            baseline = text.split('PROCESSOR_CAPTURE_STEP PASS ')[1].splitlines()[0]
        else:
            assert row['returncode'] and re.search(
                r'FATAL:.*(?:processor state mismatch|processor solver unsettled/repeated|'
                r'processor clock mismatch|manual .*failed|manual held task changed)', text), (name, text)
        results.append(dict(case=name, returncode=row['returncode'], expected_failure=name != 'original'))
    verilator = out / 'verilator'
    verilator.mkdir()
    checked(['verilator', '--binary', '--timing', '--top-module', 'tb_processor_capture_step',
             '-Wno-fatal', '-Wno-PINMISSING', '-Wno-TIMESCALEMOD', '-Wno-MULTIDRIVEN',
             '--Mdir', verilator / 'obj', '-o', 'test', inputs / CANDIDATE, *support],
            verilator / 'compile.log')
    checked([verilator / 'obj/test'], verilator / 'test.log')
    assert 'PROCESSOR_CAPTURE_STEP PASS ' + baseline in (verilator / 'test.log').read_text()
    for name, digest in hashes.items():
        assert hashlib.sha256((inputs / name).read_bytes()).hexdigest() == digest
    record = dict(status='PASS_COUPLED_PROCESSOR_CAPTURE_COMPONENT', inputs=hashes,
                  reference=manifest, cases=results, actions=actions, summary=baseline,
                  physical_bits=64, native_integrated=False, analog_timing_qualified=False,
                  data_writeback_qualified=False, producer_timing_qualified=False, scope=__doc__)
    (out / 'results.json').write_text(json.dumps(record, indent=2) + '\n')
    print(record['status'], '64 Qs/22 clocks;', baseline, ';', len(cases) - 1, 'bad variants rejected')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--mutations', action='store_true')
    args = parser.parse_args()
    run(args.output.resolve(), args.mutations)
