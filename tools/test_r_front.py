#!/usr/bin/env python3
"""Compare the real R-address/control graph coupled to functional RM/STK storage.

All 9216 SRAM bits and 736 processor bits are compared at every resolved event.
The old g16 capture epoch is kept without extra RAM ports/copies. The 100 new
bits cover actual per-task RBase RAM, stack pointers and saved/write addresses.
MIR/IFU/ALU/counter/I/O producer boundaries remain explicit. Combinational gate
wiring/functions are shared; delayed storage and capture models are independent.
This is not native integration or a complete 60ns proof.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

from generate_r_front_reference import specs
from sil_netlist import load_board

ROOT = Path(__file__).resolve().parents[1]
CANDIDATE = 'verilog/rtl/dorado_r_front_step.sv'
ADDRESS = 'verilog/rtl/dorado_r_address_step.sv'
PORTS = 'verilog/rtl/dorado_r_file_ports.sv'
OPERAND = 'verilog/rtl/dorado_r_operand_step.sv'
PARITY = 'verilog/rtl/dorado_t_parity_step.sv'
T_PATH = 'verilog/rtl/dorado_t_file_step.sv'
CORE = 'verilog/rtl/dorado_task_capture_step.sv'
TEST = 'verilog/verilator/tb_r_front_step.sv'
PROFILES = [('balanced', 1000, 1000, 1000, None),
            ('fast_gates', 500, 1000, 500, None),
            ('later_control', 1000, 2000, 1000, None),
            ('parc_parity_min', 1000, 1000, 1000, 'min'),
            ('parc_parity_max', 1000, 1000, 1000, 'max')]
MUTATIONS = {
    'lost_rbase_ram_write': [('if(!after_clocks[46]) front_next', "if(1'b0) front_next")],
    'wrong_g14_phase': [('if(!after_clocks[45]) front_next', 'if(after_clocks[45]) front_next')],
    'lost_stack_save_low': [('if(captures[8]) front_next', "if(1'b0) front_next")],
    'lost_stack_save_high': [('if(captures[9]) front_next', "if(1'b0) front_next")],
    'lost_pointer_high': [('if(captures[3]) front_next', "if(1'b0) front_next")],
    'lost_wadr_latch': [('if(!after_clocks[51]) front_next', "if(1'b0) front_next")],
    'raw_instead_of_decoded_fa': [("result[19:18]=fa[0] ? 2'b11 : 2'b00;", 'result[19:18]=p[19:18];')],
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
    files = [CANDIDATE, 'verilog/rtl/dorado_r_memory_step.sv', 'tools/generate_r_front_reference.py', 'tools/generate_r_front_cone.py', 'verilog/verilator/r_front_original_storage.sv', ADDRESS, PORTS, 'verilog/rtl/dorado_r_port_preview.sv', 'verilog/rtl/dorado_processor_capture_step.sv', 'tools/generate_r_file_reference.py', 'verilog/verilator/r_file_original_storage.sv', 'verilog/cells/cell_MB7071H.v', 'verilog/cells/cell_F10414.v', 'verilog/rtl/dorado_r_flags_step.sv', 'tools/generate_r_flags_reference.py', OPERAND, 'tools/generate_r_operand_reference.py', PARITY, T_PATH, CORE, TEST, 'verilog/verilator/tb_r_front_cone.sv',
             'tools/generate_t_parity_reference.py','tools/stage_t_parity_timing.py',
             'verilog/verilator/t_parity_original_ff.sv',
             'tools/generate_r_address_reference.py','tools/stage_r_operand_timing.py',
             'verilog/verilator/r_operand_original_ff.sv','chm/sil/msa-Rev-Bg.dm!1_/ecldict.analyze', 'verilog/verilator/t_file_original_storage.sv',
             'tools/generate_t_file_reference.py','tools/stage_t_file_timing.py',
             'verilog/verilator/run_control_original_ff.sv',
             'verilog/verilator/processor_capture_original_ff.sv',
             'tools/generate_task_capture_reference.py',
             'tools/generate_processor_capture_reference.py',
             'tools/generate_control_clock_reference.py', 'tools/generate_run_control_reference.py',
             'tools/sil_netlist.py', 'tools/stage_task_capture_timing.py', 'tools/test_r_front.py']
    kinds = {'MC10210'}
    boards = ['BaseBd-Rev-Am']
    for spec in specs()[0]:
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
    checked([sys.executable, inputs / 'tools/generate_r_front_reference.py',
             '--output', graph], out / 'generation.log')
    manifest = json.loads((graph / 'manifest.json').read_text())
    assert (manifest['physical_bits'],manifest['packages'],manifest['pin_count'])==(736,259,236)
    cone=out/'dorado_r_front_cone.sv'
    checked([sys.executable,inputs/'tools/generate_r_front_cone.py','--reference',graph,'--output',cone],out/'cone-generation.log')
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
        raw=(inputs/'verilog/verilator/r_front_original_storage.sv').read_text().replace('DELAY_PS=1000','DELAY_PS='+str(ffs))
        (timed/'r_front_original_storage.sv').write_text(raw)
        support = [cone, inputs/'verilog/rtl/dorado_r_memory_step.sv', inputs / ADDRESS, inputs / PORTS, inputs / 'verilog/rtl/dorado_r_port_preview.sv', inputs / 'verilog/rtl/dorado_processor_capture_step.sv', inputs / 'verilog/rtl/dorado_r_flags_step.sv', inputs / OPERAND, inputs / TEST, inputs / PARITY, inputs / T_PATH, inputs / CORE, *sorted(timed.glob('*.sv')), *sorted(timed.glob('*.v'))]
        defines=['-DPARC_PARITY_TIMING']if parc else[]
        if profile=='balanced':
            # Compare sampled pin adapters and raw FF/ASEL function truth
            # separately from the full state/capture ordering test.
            check=directory/'cone-truth';check.mkdir()
            cone_support=[cone,inputs/'verilog/verilator/tb_r_front_cone.sv',
                          *sorted(timed.glob('*.sv')),*sorted(timed.glob('*.v'))]
            checked(['iverilog','-g2012','-s','tb_r_front_cone','-o',check/'test.vvp',*cone_support],check/'compile.log')
            checked(['vvp',check/'test.vvp'],check/'test.log')
            expected='R_FRONT_CONE PASS checks=4352 raw_function_cases=4096 sampled_Q_cases=256'
            assert expected in (check/'test.log').read_text()
            checked(['verilator','--binary','--timing','--top-module','tb_r_front_cone',
                     '-Wno-fatal','-Wno-PINMISSING','-Wno-TIMESCALEMOD','-Wno-MULTIDRIVEN',
                     '--Mdir',check/'obj','-o','test',*cone_support],check/'verilator-compile.log')
            checked([check/'obj/test'],check/'verilator-test.log')
            assert expected in (check/'verilator-test.log').read_text()
            print('R_FRONT_CONE both simulators',expected,flush=True)

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
            rtl = case / 'dorado_r_front_step.sv'
            rtl.write_text(raw)
            binary = case / 'test.vvp'
            checked(['iverilog', '-g2012', '-s', 'tb_r_front_step', *defines, '-o', binary, rtl, *support], case / 'compile.log')
            row = execute(['vvp', binary], inputs, case / 'test.log')
            actions.append(row)
            text = (case / 'test.log').read_text()
            if not expected_failure:
                assert not row['returncode'] and 'R_FRONT_STEP PASS ' in text, (profile, text)
                summary = re.search(r'^R_FRONT_STEP PASS ([^\n]+)$', text, re.M)[1]
                if baseline is None:
                    baseline = summary
                assert summary == baseline, (profile, summary, baseline)
                print('R_FRONT', profile, 'Icarus', summary, flush=True)
            else:
                assert row['returncode'] and re.search(r'FATAL:.*R front (?:state mismatch|output mismatch|operand output mismatch|clock mismatch|storage mismatch|write gate mismatch|commit changed|write/read windows overlap|capture epoch overlaps write|capture port lost old parity epoch|solver unsettled/repeated|resolved source mismatch)', text), (name, text)
            results.append(dict(profile=profile, case=name, returncode=row['returncode'],
                                expected_failure=expected_failure))
        verilator = directory / 'verilator'
        verilator.mkdir()
        checked(['verilator', '--binary', '--timing', '--top-module', 'tb_r_front_step',
                 *defines, '-Wno-fatal', '-Wno-PINMISSING', '-Wno-TIMESCALEMOD', '-Wno-MULTIDRIVEN',
                 '--Mdir', verilator / 'obj', '-o', 'test', inputs / CANDIDATE, *support], verilator / 'compile.log')
        checked([verilator / 'obj/test'], verilator / 'test.log')
        assert re.search(r'^R_FRONT_STEP PASS ([^\n]+)$', (verilator / 'test.log').read_text(), re.M)[1] == baseline
        vtext=(verilator/'test.log').read_text()
        itext=(directory/'original/test.log').read_text()
        coverage=re.search(r'R_FRONT_COVERAGE ([^\n]+)',itext)[1]
        assert 'R_FRONT_COVERAGE '+coverage in vtext,(profile,coverage,vtext)
        timing=re.search(r'^R_FRONT_TIMING ([^\n]+)$',itext,re.M)[1]
        assert re.search(r'^R_FRONT_TIMING ([^\n]+)$',vtext,re.M)[1]==timing
        clocks=re.findall(r'^R_FRONT_CLOCK channel=(\d+) edges=(\d+)$',itext,re.M)
        assert len(clocks)==57 and clocks==re.findall(r'^R_FRONT_CLOCK channel=(\d+) edges=(\d+)$',vtext,re.M)
        storage=re.search(r'^R_FRONT_STORAGE ([^\n]+)$',itext,re.M)[1]
        assert re.search(r'^R_FRONT_STORAGE ([^\n]+)$',vtext,re.M)[1]==storage
        write_windows=re.findall(r'^R_FRONT_WRITE_WINDOW ([^\n]+)$',itext,re.M)
        assert len(write_windows)==2 and write_windows==re.findall(r'^R_FRONT_WRITE_WINDOW ([^\n]+)$',vtext,re.M)
        arrivals.append(dict(profile=profile,coverage=coverage,measured_arrivals=timing,
                             source_limits_enforced=bool(parc),clock_edge_counts=clocks,
                             storage=storage,write_windows=write_windows))
        print('R_FRONT', profile, 'Verilator agrees;',coverage, flush=True)
    for name, digest in hashes.items():
        assert hashlib.sha256((inputs / name).read_bytes()).hexdigest() == digest
    record = dict(status='PASS_R_FRONT_ARRIVAL_CONTRACT', inputs=hashes,
                  reference=manifest, ram_reference=ram_manifest, profiles=[(*x,1000)for x in PROFILES]+[('later_r_memory',1000,1000,1000,None,9900)], arrivals=arrivals, cases=results, actions=actions,
                  equivalence_controls=list(EQUIVALENCES),
                  summary=baseline, physical_bits=736, ram_bits=9216, total_physical_bits=9952, native_integrated=False,
                  vendor_delays_qualified=False, analog_timing_qualified=False,
                  t_file_state_and_gate_equivalence_checked=True, t_parity_ram_checked=True,
                  R_front_storage_component_checked=True, R_final_address_stage_qualified=True,
                  functional_R_ports_coupled=True, r_read_write_nonoverlap_checked=True,
                  post_commit_source_epoch_checked=True, all_R_memory_bits_compared=True,
                  ram_arrays_flattened=False,
                  R_read_latches_checked=True, architectural_alu_writeback_qualified=False,
                  producer_timing_qualified=False, shared_combinational_logic=True, raw_function_cases=4096, sampled_Q_cases=256, scope=__doc__)
    (out / 'results.json').write_text(json.dumps(record, indent=2) + '\n')
    print(record['status'], '736 original bits/57 clocks;', len(MUTATIONS) if mutations else 0, 'bad variants rejected')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--mutations', action='store_true')
    args = parser.parse_args()
    run(args.output.resolve(), args.mutations)
