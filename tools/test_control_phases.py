#!/usr/bin/env python3
"""Freeze and qualify the isolated original ContA phase next-state contract.

This does not qualify upstream Run/Hold/Freeze or native integration. Icarus
checks X/Z behavior; Verilator independently executes the two-state cases.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
CANDIDATE = 'verilog/rtl/dorado_control_phase_next.sv'
TEST = 'verilog/verilator/tb_control_phase_next.sv'
CELLS = ['verilog/cells/cell_MC10117.v', 'verilog/cells/cell_MC10104.v']
INPUTS = [CANDIDATE, TEST, *CELLS, 'tools/check_control_phase_wiring.py',
          'tools/sil_netlist.py', 'tools/test_control_phases.py',
          'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl',
          'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.lc']
MUTATIONS = {
    'ignore_extension': ('(im_tpc || !phase0)', "(1'b0 || !phase0)"),
    'cascade_new_state': ('state_q[3],state_q[2],state_q[1]',
                          'd_phase1,state_q[3],state_q[2]'),
    'merge_pre_clock': ('if (capture_pre)', 'if (capture_main)'),
    'ignore_initialization': ('if (phase0_init)', "if (1'b0)"),
    'unsafe_four_state_reduction': ('phase0 && phase_term',
                                    'phase0 && im_tpc && !phase4'),
    'toggle_phase0': ('{d_start_cycle,state_q[5],d_phase1,',
                      '{d_start_cycle,!state_q[4],d_phase1,'),
    'hold_pre_with_main': ('if (capture_pre)', 'if (capture_pre && capture_main)'),
    'gate_async_initialization': ('if (phase0_init)',
                                 'if (phase0_init && capture_main)'),
}


def execute(command, cwd, log):
    with log.open('w') as output:
        process = subprocess.run(command, cwd=cwd, stdout=output,
                                 stderr=subprocess.STDOUT)
    return dict(command=[str(x) for x in command], cwd=str(cwd),
                log=str(log), returncode=process.returncode)


def run(output, mutations):
    output.mkdir(parents=True, exist_ok=False)
    inputs = output / 'executed-inputs'
    hashes = {}
    for name in INPUTS:
        raw = (ROOT / name).read_bytes()
        target = inputs / name
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(raw)
        hashes[name] = hashlib.sha256(raw).hexdigest()
    actions = []

    def checked(command, cwd, log):
        action = execute(command, cwd, log)
        actions.append(action)
        if action['returncode']:
            raise RuntimeError('Command failed; inspect ' + str(log))

    checked([sys.executable, inputs/'tools/check_control_phase_wiring.py'],
            inputs, output/'wiring.log')
    cases = [('original', (inputs/CANDIDATE).read_text())]
    if mutations:
        source = cases[0][1]
        for name, (before, after) in MUTATIONS.items():
            if source.count(before) != 1:
                raise ValueError('Mutation target not unique: ' + name)
            cases.append((name, source.replace(before, after)))
    results = []
    for name, source in cases:
        directory = output / name
        directory.mkdir()
        rtl = directory/'dorado_control_phase_next.sv'
        rtl.write_text(source)
        binary = directory/'test.vvp'
        checked(['iverilog', '-g2012', '-s', 'tb_control_phase_next', '-o', binary,
                 rtl, inputs/TEST, *[inputs/path for path in CELLS]],
                inputs, directory/'compile.log')
        # Run the X/Z-sensitive bad reduction through the four-state suite;
        # all other bad variants must already fail the known-state cases.
        args = ['vvp', binary]
        if name in ('original', 'unsafe_four_state_reduction'):
            args.append('+four_state')
        action = execute(args, inputs, directory/'test.log')
        actions.append(action)
        text = (directory/'test.log').read_text()
        if name == 'original':
            if action['returncode'] or 'PASS checks=546739 known=2048 four_state=524288' not in text:
                raise RuntimeError('Original phase contract failed')
        elif not action['returncode'] or not re.search(r'FATAL:.*(?:phase mismatch|original j05 D differs)', text):
            raise RuntimeError('Bad variant did not fail for its behavior: ' + name)
        results.append(dict(case=name, returncode=action['returncode'],
                            expected_failure=name!='original'))
    directory=output/'verilator'
    directory.mkdir()
    checked(['verilator', '--binary', '--timing', '--top-module',
             'tb_control_phase_next', '-Wno-fatal', '-Wno-PINMISSING',
             '-Wno-TIMESCALEMOD', '--Mdir', directory/'obj', '-o', 'test',
             inputs/CANDIDATE, inputs/TEST, *[inputs/path for path in CELLS]],
            inputs, directory/'compile.log')
    checked([directory/'obj/test'], inputs, directory/'test.log')
    if 'PASS checks=22451 known=2048 four_state=0' not in (directory/'test.log').read_text():
        raise ValueError('Verilator coverage incomplete')
    for name, digest in hashes.items():
        if hashlib.sha256((inputs/name).read_bytes()).hexdigest()!=digest:
            raise ValueError('Executed source changed: ' + name)
    record = dict(status='PASS_ORIGINAL_CONTROL_PHASE_COMPONENT', inputs=hashes,
                  cases=results, actions=actions, four_state_checks=546739,
                  verilator_checks=22451, physical_phase_bits=7,
                  native_integrated=False, upstream_clock_controls_qualified=False,
                  scope=__doc__)
    (output/'results.json').write_text(json.dumps(record, indent=2)+'\n')
    print(record['status'], '7 original state bits; 546739 Icarus / 22451 Verilator checks;',
          len(cases)-1, 'bad variants rejected')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--mutations', action='store_true')
    args = parser.parse_args()
    run(args.output.resolve(), args.mutations)
