#!/usr/bin/env python3
"""Compare original T parity storage/capture/checks and R boundary checks.

540 original bits/25 clocks, explicit illustrative arrival profiles plus PARC
min/max parity-arc controls. These are not board setup/hold qualification or a
native 60ns proof. Inherited normal/extended producer/setup contract applies.
R operand/RAM-parity producers and other retained cuts remain external.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

from generate_t_parity_reference import specs
from sil_netlist import load_board

ROOT = Path(__file__).resolve().parents[1]
CANDIDATE = 'verilog/rtl/dorado_t_parity_step.sv'
T_PATH = 'verilog/rtl/dorado_t_file_step.sv'
CORE = 'verilog/rtl/dorado_task_capture_step.sv'
TEST = 'verilog/verilator/tb_t_parity_step.sv'
PROFILES = [('balanced', 1000, 1000, 1000, None),
            ('fast_gates', 500, 1000, 500, None),
            ('later_control', 1000, 2000, 1000, None),
            ('parc_parity_min', 1000, 1000, 1000, 'min'),
            ('parc_parity_max', 1000, 1000, 1000, 'max')]
MUTATIONS = {
    'parity_from_new_address': [('state_q[404+4*old_h_address]', 'state_q[404+4*h_address]')],
    'parity_even_store': [('~^t_next[107:100]', '^t_next[107:100]')],
    'lose_parity_capture': [('if(clock_events[13])', "if(1'b0)")],
    'reverse_parity_address': [('{state_q[80],state_q[81],state_q[82],state_q[83]}','state_q[83:80]')],
    'lose_T_bypass_mask': [('!(bh|p[117]|ok[0])','!(p[117]|ok[0])')],
    'lose_T_ifu_mask': [('!(bh|p[117]|ok[0])','!(bh|ok[0])')],
    'lose_R_bypass_mask': [('!(p[148]|p[149]|ok[1])','!(p[149]|ok[1])')],
    'lose_R_ifu_mask': [('!(p[148]|p[149]|ok[1])','!(p[148]|ok[1])')],
    'suppress_T_fault': [('!(bh|p[117]|ok[0])', "1'b0")],
    'suppress_R_fault': [('!(p[148]|p[149]|ok[1])', "1'b0")],
    'wrong_R_check_sense': [('(^p[134:127])^q[535]', '(^p[134:127])^!q[535]')],
    'lose_low_fault_capture': [('source_faults_after=fault_requests(state_q,pins_after)', "source_faults_after=4'b0")],
}
EQUIVALENCES = {}


def execute(command, cwd, log):
    with log.open('w') as stream:
        code = subprocess.run([str(x) for x in command], cwd=cwd, stdout=stream,
                              stderr=subprocess.STDOUT).returncode
    return dict(command=[str(x) for x in command], cwd=str(cwd),
                returncode=code, log=str(log))


def run(out, mutations):
    out.mkdir(parents=True, exist_ok=False)
    inputs = out / 'executed-inputs'
    files = [CANDIDATE, T_PATH, CORE, TEST,
             'tools/generate_t_parity_reference.py','tools/stage_t_parity_timing.py',
             'verilog/verilator/t_parity_original_ff.sv','chm/sil/msa-Rev-Bg.dm!1_/ecldict.analyze', 'verilog/verilator/t_file_original_storage.sv',
             'tools/generate_t_file_reference.py','tools/stage_t_file_timing.py',
             'verilog/verilator/run_control_original_ff.sv',
             'verilog/verilator/processor_capture_original_ff.sv',
             'tools/generate_task_capture_reference.py',
             'tools/generate_processor_capture_reference.py',
             'tools/generate_control_clock_reference.py', 'tools/generate_run_control_reference.py',
             'tools/sil_netlist.py', 'tools/stage_task_capture_timing.py', 'tools/test_t_parity.py']
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
    actions = []

    def checked(command, log):
        row = execute(command, inputs, log)
        actions.append(row)
        if row['returncode']:
            raise RuntimeError('Command failed: ' + str(log))

    graph = out / 'reference'
    checked([sys.executable, inputs / 'tools/generate_t_parity_reference.py',
             '--output', graph], out / 'generation.log')
    manifest = json.loads((graph / 'manifest.json').read_text())
    assert (manifest['physical_bits'], manifest['packages'], manifest['original_pins']) == (540, 150, 1506)
    results = []
    baseline = None
    arrivals = []
    for profile, gates, ffs, phases, parc in PROFILES:
        directory = out / profile
        directory.mkdir()
        timed = directory / 'timed-reference'
        command=[sys.executable, inputs / 'tools/stage_t_parity_timing.py', '--inputs', inputs,
                 '--reference', graph, '--output', timed, '--gate-ps', gates,
                 '--ff-ps', ffs, '--phase-ps', phases]
        if parc:command+=['--parc',parc]
        checked(command,directory/'staging.log')
        support = [inputs / TEST, inputs / T_PATH, inputs / CORE, *sorted(timed.glob('*.sv')), *sorted(timed.glob('*.v'))]
        defines=['-DPARC_PARITY_TIMING']if parc else[]
        cases = [('original', source, False)]
        if mutations and profile == 'balanced':
            for name, edits in {**MUTATIONS, **EQUIVALENCES}.items():
                raw = source
                for before, after in edits:
                    assert raw.count(before) == 1, (name, before)
                    raw = raw.replace(before, after)
                cases.append((name, raw, name in MUTATIONS))
        for name, raw, expected_failure in cases:
            case = directory / name
            case.mkdir()
            rtl = case / 'dorado_t_parity_step.sv'
            rtl.write_text(raw)
            binary = case / 'test.vvp'
            checked(['iverilog', '-g2012', '-s', 'tb_t_parity_step', *defines, '-o', binary, rtl, *support], case / 'compile.log')
            row = execute(['vvp', binary], inputs, case / 'test.log')
            actions.append(row)
            text = (case / 'test.log').read_text()
            if not expected_failure:
                assert not row['returncode'] and 'T_PARITY_STEP PASS ' in text, (profile, text)
                summary = text.split('T_PARITY_STEP PASS ')[1].splitlines()[0]
                if baseline is None:
                    baseline = summary
                assert summary == baseline, (profile, summary, baseline)
                print('T_PARITY', profile, 'Icarus', summary, flush=True)
            else:
                assert row['returncode'] and re.search(r'FATAL:.*(?:T-file state mismatch|parity/fault output mismatch|T-file window mismatch|T-file read gate mismatch|T-file .*changed|task solver unsettled/repeated|manual task data isolation failed|task clock mismatch)', text), (name, text)
            results.append(dict(profile=profile, case=name, returncode=row['returncode'],
                                expected_failure=expected_failure))
        verilator = directory / 'verilator'
        verilator.mkdir()
        checked(['verilator', '--binary', '--timing', '--top-module', 'tb_t_parity_step',
                 *defines, '-Wno-fatal', '-Wno-PINMISSING', '-Wno-TIMESCALEMOD', '-Wno-MULTIDRIVEN',
                 '--Mdir', verilator / 'obj', '-o', 'test', inputs / CANDIDATE, *support], verilator / 'compile.log')
        checked([verilator / 'obj/test'], verilator / 'test.log')
        assert 'T_PARITY_STEP PASS ' + baseline in (verilator / 'test.log').read_text()
        vtext=(verilator/'test.log').read_text()
        itext=(directory/'original/test.log').read_text()
        timing=re.search(r'T_PARITY_TIMING ([^\n]+)',itext)[1]
        assert 'T_PARITY_TIMING '+timing in vtext,(profile,timing,vtext)
        arrivals.append(dict(profile=profile,measured_arrivals=timing,source_limits_enforced=bool(parc)))
        print('T_PARITY', profile, 'Verilator agrees;',timing, flush=True)
    for name, digest in hashes.items():
        assert hashlib.sha256((inputs / name).read_bytes()).hexdigest() == digest
    record = dict(status='PASS_T_PARITY_ARRIVAL_CONTRACT', inputs=hashes,
                  reference=manifest, profiles=PROFILES, arrivals=arrivals, cases=results, actions=actions,
                  equivalence_controls=list(EQUIVALENCES),
                  summary=baseline, physical_bits=540, native_integrated=False,
                  vendor_delays_qualified=False, analog_timing_qualified=False,
                  t_file_write_windows_checked=True, t_parity_ram_checked=True, R_storage_producer_qualified=False, architectural_alu_writeback_qualified=False, producer_timing_qualified=False, scope=__doc__)
    (out / 'results.json').write_text(json.dumps(record, indent=2) + '\n')
    print(record['status'], '540 original bits/25 clocks;', len(MUTATIONS) if mutations else 0, 'bad variants rejected')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--mutations', action='store_true')
    args = parser.parse_args()
    run(args.output.resolve(), args.mutations)
