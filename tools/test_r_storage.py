#!/usr/bin/env python3
"""Compare the coupled 596-bit processor and 9216-bit physical R RAM storage.

All RAM data/parity now come from actual ports; j05's original write gates
are observed independently. Physical address/select producers remain external
and change only in a closed high-root window. Not native integration or 60ns.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

from generate_r_flags_reference import specs
from sil_netlist import load_board

ROOT = Path(__file__).resolve().parents[1]
CANDIDATE = 'verilog/rtl/dorado_r_storage_step.sv'
FLAGS = 'verilog/rtl/dorado_r_flags_step.sv'
PORTS = 'verilog/rtl/dorado_r_file_ports.sv'
OPERAND = 'verilog/rtl/dorado_r_operand_step.sv'
PARITY = 'verilog/rtl/dorado_t_parity_step.sv'
T_PATH = 'verilog/rtl/dorado_t_file_step.sv'
CORE = 'verilog/rtl/dorado_task_capture_step.sv'
TEST = 'verilog/verilator/tb_r_storage_step.sv'
PROFILES = [('balanced', 1000, 1000, 1000, None),
            ('fast_gates', 500, 1000, 500, None),
            ('later_control', 1000, 2000, 1000, None),
            ('parc_parity_min', 1000, 1000, 1000, 'min'),
            ('parc_parity_max', 1000, 1000, 1000, 'max')]
MUTATIONS = {
    'wrong_R_write_decode': [('state_next[37]', 'state_next[38]')],
    'lose_delayed_hold': [('state_next[93]', "1'b0")],
    'wrong_write_data': [('state_next[555:540]', 'pins_after[91:76]')],
    'lose_read_parity': [('file_read_parity,file_read_data,pins_after[126:0]', "2'b00,file_read_data,pins_after[126:0]")],
    'swap_read_halves': [('file_read_parity,file_read_data,pins_after[126:0]', 'file_read_parity,{file_read_data[7:0],file_read_data[15:8]},pins_after[126:0]')],
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
    files = [CANDIDATE, FLAGS, PORTS, OPERAND, 'tools/generate_r_file_reference.py',
             'verilog/verilator/r_file_original_storage.sv', 'verilog/cells/cell_MB7071H.v', 'verilog/cells/cell_F10414.v', 'tools/generate_r_operand_reference.py', PARITY, T_PATH, CORE, TEST,
             'tools/generate_t_parity_reference.py','tools/stage_t_parity_timing.py',
             'verilog/verilator/t_parity_original_ff.sv',
             'tools/generate_r_flags_reference.py','tools/stage_r_operand_timing.py',
             'verilog/verilator/r_operand_original_ff.sv','chm/sil/msa-Rev-Bg.dm!1_/ecldict.analyze', 'verilog/verilator/t_file_original_storage.sv',
             'tools/generate_t_file_reference.py','tools/stage_t_file_timing.py',
             'verilog/verilator/run_control_original_ff.sv',
             'verilog/verilator/processor_capture_original_ff.sv',
             'tools/generate_task_capture_reference.py',
             'tools/generate_processor_capture_reference.py',
             'tools/generate_control_clock_reference.py', 'tools/generate_run_control_reference.py',
             'tools/sil_netlist.py', 'tools/stage_task_capture_timing.py', 'tools/test_r_storage.py']
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
    checked([sys.executable, inputs / 'tools/generate_r_flags_reference.py',
             '--output', graph], out / 'generation.log')
    manifest = json.loads((graph / 'manifest.json').read_text())
    assert (manifest['physical_bits'], manifest['packages'], manifest['original_pins']) == (596, 170, 1748)
    ram_graph=out/'ram-reference'
    checked([sys.executable, inputs/'tools/generate_r_file_reference.py', '--output', ram_graph], out/'ram-generation.log')
    ram_manifest=json.loads((ram_graph/'manifest.json').read_text())
    assert (ram_manifest['physical_bits'],ram_manifest['packages'],ram_manifest['original_pins'])==(9216,14,244)
    results = []
    baseline = None
    arrivals = []
    for profile, gates, ffs, phases, parc, read_ps in [(*x,1000)for x in PROFILES]+[('later_r_memory',1000,1000,1000,None,9900)]:
        directory = out / profile
        directory.mkdir()
        timed = directory / 'timed-reference'
        command=[sys.executable, inputs / 'tools/stage_r_operand_timing.py', '--inputs', inputs,
                 '--reference', graph, '--output', timed, '--gate-ps', gates,
                 '--ff-ps', ffs, '--phase-ps', phases]
        if parc:command+=['--parc',parc]
        checked(command,directory/'staging.log')
        for p in ram_graph.glob('*.sv'):(timed/p.name).write_bytes(p.read_bytes())
        raw=(inputs/'verilog/verilator/r_file_original_storage.sv').read_text().replace('READ_PS=1000','READ_PS='+str(read_ps))
        (timed/'r_file_original_storage.sv').write_text(raw)
        support = [inputs / FLAGS, inputs / PORTS, inputs / OPERAND, inputs / TEST, inputs / PARITY, inputs / T_PATH, inputs / CORE, *sorted(timed.glob('*.sv')), *sorted(timed.glob('*.v'))]
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
            rtl = case / 'dorado_r_storage_step.sv'
            rtl.write_text(raw)
            binary = case / 'test.vvp'
            checked(['iverilog', '-g2012', '-s', 'tb_r_storage_step', *defines, '-o', binary, rtl, *support], case / 'compile.log')
            row = execute(['vvp', binary], inputs, case / 'test.log')
            actions.append(row)
            text = (case / 'test.log').read_text()
            if not expected_failure:
                assert not row['returncode'] and 'R_STORAGE_STEP PASS ' in text, (profile, text)
                summary = text.split('R_STORAGE_STEP PASS ')[1].splitlines()[0]
                if baseline is None:
                    baseline = summary
                assert summary == baseline, (profile, summary, baseline)
                print('R_STORAGE', profile, 'Icarus', summary, flush=True)
            else:
                assert row['returncode'] and re.search(r'FATAL:.*R storage (?:write gate|state|RAM output|flag/parity|commit)',text), (name, text)
            results.append(dict(profile=profile, case=name, returncode=row['returncode'],
                                expected_failure=expected_failure))
        verilator = directory / 'verilator'
        verilator.mkdir()
        checked(['verilator', '--binary', '--timing', '--top-module', 'tb_r_storage_step',
                 *defines, '-Wno-fatal', '-Wno-PINMISSING', '-Wno-TIMESCALEMOD', '-Wno-MULTIDRIVEN',
                 '--Mdir', verilator / 'obj', '-o', 'test', inputs / CANDIDATE, *support], verilator / 'compile.log')
        checked([verilator / 'obj/test'], verilator / 'test.log')
        assert 'R_STORAGE_STEP PASS ' + baseline in (verilator / 'test.log').read_text()
        vtext=(verilator/'test.log').read_text()
        itext=(directory/'original/test.log').read_text()
        timing=itext.split("R_STORAGE_TIMING ",1)[1].splitlines()[0]
        assert 'R_STORAGE_TIMING '+timing in vtext,(profile,timing,vtext)
        arrivals.append(dict(profile=profile,measured_arrivals=timing,source_limits_enforced=bool(parc)))
        print('R_STORAGE',profile,'Verilator agrees;',timing,flush=True)
    for name, digest in hashes.items():
        assert hashlib.sha256((inputs / name).read_bytes()).hexdigest() == digest
    record = dict(status='PASS_R_STORAGE_ARRIVAL_CONTRACT', inputs=hashes,
                  reference=manifest, ram_reference=ram_manifest, profiles=[(*x,1000)for x in PROFILES]+[('later_r_memory',1000,1000,1000,None,9900)], arrivals=arrivals, cases=results, actions=actions,
                  equivalence_controls=list(EQUIVALENCES),
                  summary=baseline, physical_bits=596, ram_bits=9216, total_physical_bits=9812, native_integrated=False,
                  vendor_delays_qualified=False, analog_timing_qualified=False,
                  functional_R_ports_coupled=True, r_read_write_nonoverlap_checked=True, post_commit_source_epoch_checked=True, ram_arrays_flattened=False, address_select_producers_qualified=False, producer_timing_qualified=False, scope=__doc__)
    (out / 'results.json').write_text(json.dumps(record, indent=2) + '\n')
    print(record['status'], '596 original bits/33 clocks;', len(MUTATIONS) if mutations else 0, 'bad variants rejected')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--mutations', action='store_true')
    args = parser.parse_args()
    run(args.output.resolve(), args.mutations)
