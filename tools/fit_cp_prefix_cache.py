#!/usr/bin/env python3
"""Freeze the guarded CP-prefix component in the original 15ns timing shell.

Retain every original launch/result bit and constraint. Add a staged reset
input and a registered readiness result, so neither interface is tied away.
Time all cache/tag/canonical registers as well as the original four SRAMs.
This cannot assemble or deploy a native core and does not qualify speed.
"""
import argparse
import json
from pathlib import Path
import re

import fit_control_clock as fixture
from fit_r_front import quartus_cone
from probe_cp_prefix_cache import candidate, bench
from prove_cp_prefix_cache import module
from report_r_front_late_hold_fit import TCL
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]


def gate(run, engine):
    state = json.loads((run / 'run-state.json').read_text())
    assert state['status'] == 'PASS_CP_PREFIX_CACHE_ORIGINAL_COMPARISON'
    assert state['engine'] == engine and state['mutation'] is None
    assert state['actions'][-1]['returncode'] == 0
    assert state['all100_original_clock_counts_match']
    assert state['summary'] == 'checks=14884 writes=238 reads=6928 epochs=392'
    parent = Path(state['parent'])
    assert sha(parent / 'run-state.json') == state['parent_state_sha256']
    assert (run / 'candidate.sv').read_text() == candidate((parent / 'candidate.sv').read_text(), None,
        state.get('parallel_control_results', False), state.get('control_only_settled_eligibility', False), state.get('settling_waves', 8), state.get('direct_cp_projection', False))
    tb = (run / 'inputs/tb_r_front_local.sv').read_text()
    assert tb == bench((parent / 'inputs/tb_r_front_local.sv').read_text())
    for row in state['source_files']:
        assert sha(run / row['path']) == row['output_sha256'], row['path']
    for fatal in re.findall(r'\$fatal\([^;]+;', (parent / 'inputs/tb_r_front_local.sv').read_text()):
        assert fatal in tb
    clocks = re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$', (run / 'test.log').read_text(), re.M)
    assert len(clocks) == 100
    assert clocks == re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$', (parent / 'test.log').read_text(), re.M)
    coverage = {k: int(v) for k, v in re.findall(r'(\w+)=(\d+)', state['cache_coverage'])}
    assert coverage['fast'] > 0 and coverage['staged'] > 0
    assert coverage['retries'] > 0 and coverage['resets'] == 1
    return state, dict(directory=str(run), engine=engine, state_sha256=sha(run / 'run-state.json'),
                       candidate_sha256=sha(run / 'candidate.sv'), clocks=clocks, coverage=coverage,
                       summary=state['summary'])


def prepare(parent, runs, formal, out):
    assert not out.exists()
    checked = [gate(run, engine) for run, engine in zip(runs, ['icarus', 'verilator'])]
    states, gates = zip(*checked)
    assert gates[0]['candidate_sha256'] == gates[1]['candidate_sha256']
    assert gates[0]['coverage'] == gates[1]['coverage'] and gates[0]['clocks'] == gates[1]['clocks']
    proof = json.loads((formal / 'run-state.json').read_text())
    assert proof['status'] in ['PASS_BOUNDED_CP_CACHE_READY_EQUIVALENCE', 'PASS_CP_CACHE_INDUCTIVE_READY_EQUIVALENCE']
    assert proof['returncode'] == 0 and proof['proof_succeeded']
    assert proof['compared_control_output_bits'] == 475
    assert proof['unbounded_induction_proven'] or proof['bounded_host_steps'] >= 4
    assert proof['source_bound_SRAM_permission'] == 'request && ready'
    assert proof['experiment_state_sha256'] == sha(runs[1] / 'run-state.json')
    for name, h in proof['source_files'].items():
        assert sha(formal / name) == h, name
    assert sha(formal / 'proof.log') == proof['proof_log_sha256']
    assert ('Induction step proven: SUCCESS!' if proof['unbounded_induction_proven'] else
            'SAT proof finished - no model found: SUCCESS!') in (formal / 'proof.log').read_text()
    fixture_proof = json.loads((parent / 'provenance.json').read_text())
    for name, h in fixture_proof['files'].items():
        assert sha(parent / name) == h, name
    assert (parent / 'clock.sdc').read_text() == fixture.SDC

    rtl = {}
    run = runs[0]
    for row in states[0]['source_files']:
        name = row['path']
        path = run / name
        if path.name == 'candidate.sv':
            target = 'dorado_r_front_control_step.sv'
        elif name.startswith(('adapters/', 'inputs/verilog/rtl/')) or path.name in [
                'dorado_r_front_cone.sv', 'dorado_r_read_window_step.sv', 'dorado_r_flag_window_step.sv']:
            target = path.name
        else:
            continue
        assert target not in rtl, target
        rtl[target] = quartus_cone(path.read_text()) if target == 'dorado_r_front_cone.sv' else path.read_text()
    assert len(rtl) == 30
    # Preserve the exact constant-zero power-up state with frontend-compatible
    # syntax. The formal proof uses the same inverse-checked lowering.
    name = 'dorado_r_front_control_step.sv'
    cache = module(rtl[name], 'dorado_cp_prefix_cache')
    lowered = (formal / 'cache.sv').read_text()
    if proof['unbounded_induction_proven']:
        from prove_cp_prefix_cache import INVARIANTS
        # Remove pure proof observations; the fitted scheduler must be exactly
        # the actual component, with no ghost logic or ghost output port.
        assert lowered.count(INVARIANTS) == 1
        lowered = lowered.replace(INVARIANTS, '', 1).replace(
            ' output reg [10:0] prefix_cp_saved,after_cp_saved,output wire invariant);',
            ' output reg [10:0] prefix_cp_saved,after_cp_saved);', 1)
    init_port = ' output reg [268:0] prefix_saved=0,\n output reg [10:0] prefix_cp_saved=0,after_cp_saved=0);'
    init_body = ' output reg [268:0] prefix_saved,\n output reg [10:0] prefix_cp_saved,after_cp_saved);\n initial begin prefix_saved=0;prefix_cp_saved=0;after_cp_saved=0;end'
    assert lowered.replace(init_body, init_port, 1) == cache
    assert rtl[name].count(cache.rstrip()) == 1
    rtl[name] = rtl[name].replace(cache.rstrip(), lowered.rstrip(), 1)

    name = 'dorado_r_read_window_step.sv'
    original, lowered = rtl[name], (parent / name).read_text()
    before, after = ' generate for(genvar byte_no=0;', ' genvar byte_no;\n generate for(byte_no=0;'
    assert original.count(before) == 1 and lowered.replace(after, before, 1) == original
    syntax = fixture_proof['quartus_genvar_syntax']
    assert syntax['inverse_byte_identical'] and syntax['lowered_sha256'] == sha(parent / name)
    rtl[name] = lowered

    top = (parent / 'top.sv').read_text()
    top = top.replace('input wire [1649:0] pins,output reg [1399:0] result',
                      'input wire [1650:0] pins,output reg [1400:0] result', 1)
    top = top.replace('reg [1649:0] input_stage,launch;', 'reg [1650:0] input_stage,launch;', 1)
    top = top.replace('}=launch;', '}=launch[1649:0];\n wire cp_reset=launch[1650];wire cp_ready;', 1)
    top = top.replace('control_flags,physical_state);', 'control_flags,physical_state,cp_reset,cp_ready);', 1)
    top = top.replace('result<={next_q,', 'result<={cp_ready,next_q,', 1)
    # Inverse comparison guarantees the original 1400 result indices and
    # 1650 launch indices retain exactly their previous meaning.
    inverse = top.replace('input wire [1650:0] pins,output reg [1400:0] result',
                          'input wire [1649:0] pins,output reg [1399:0] result', 1)
    inverse = inverse.replace('reg [1650:0] input_stage,launch;', 'reg [1649:0] input_stage,launch;', 1)
    inverse = inverse.replace('}=launch[1649:0];\n wire cp_reset=launch[1650];wire cp_ready;', '}=launch;', 1)
    inverse = inverse.replace('control_flags,physical_state,cp_reset,cp_ready);', 'control_flags,physical_state);', 1)
    inverse = inverse.replace('result<={cp_ready,next_q,', 'result<={next_q,', 1)
    assert inverse == (parent / 'top.sv').read_text()
    assert 'cp_reset=launch[1650]' in top and 'result<={cp_ready,next_q,' in top
    qsf = fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
                            ''.join('set_global_assignment -name SYSTEMVERILOG_FILE ' + n + '\n' for n in rtl))
    audit = TCL.replace('late-hold-flat-v5-reports', 'cp-prefix-cache-reports')
    # Global reports include every new auxiliary register and readiness bit.
    # Preserve the original owner-only queries, with the original indices.
    files = dict(rtl, **{'top.sv': top, 'clock.sdc': fixture.SDC, 'clock_fit.qsf': qsf,
                       'clock_fit.qpf': 'PROJECT_REVISION = "clock_fit"\n', 'audit.tcl': audit})
    out.mkdir(parents=True)
    for name, raw in files.items():
        (out / name).write_text(raw)
    for case in syntax['comparisons']:
        assert all(a['returncode'] == 0 for a in case['actions'])
        for action in case['actions']:
            log = parent / action['log']
            assert sha(log) == fixture_proof['files'][log.name]
            (out / log.name).write_bytes(log.read_bytes())
    for name in [Path(__file__).name, 'probe_cp_prefix_cache.py', 'prove_cp_prefix_cache.py',
                 'fit_control_clock.py', 'fit_r_front.py', 'report_r_front_late_hold_fit.py']:
        (out / name).write_bytes((ROOT / 'tools' / name).read_bytes())
    state = dict(status='PREPARED_CHECKED_CP_PREFIX_CACHE_FIT', scope=__doc__, component='guarded_cp_prefix_cache',
                 clock_period_ns=15, physical_Q_bits=844, ram_bits=9216, physical_SRAM_banks=4,
                 input_staging_bits=1651, launch_bits=1651, result_bits=1401,
                 original_launch_indices_retained=[0, 1649], original_result_indices_retained=[0, 1399],
                 auxiliary_reset_launch_bit=1650, readiness_result_bit=1400, original_shell_inverse_byte_identical=True,
                 functional_gates=gates, formal_directory=str(formal), formal_state_sha256=sha(formal / 'run-state.json'),
                 formal_bounded_host_steps=proof['bounded_host_steps'], unbounded_induction_proven=proof['unbounded_induction_proven'],
                 parallel_control_results=states[0].get('parallel_control_results', False),
                 control_only_settled_eligibility=states[0].get('control_only_settled_eligibility', False),
                 settling_waves=states[0].get('settling_waves', 8),
                 direct_cp_projection=states[0].get('direct_cp_projection', False),
                 quartus_genvar_syntax=syntax, output_initializer_syntax_lowering=proof['output_initializer_syntax_lowering'],
                 native_qualified=False, physical_timing_qualified=False, speedup_qualified=False, assembly_permitted=False,
                 files={p.name: sha(p) for p in out.iterdir() if p.is_file()})
    (out / 'provenance.json').write_text(json.dumps(state, indent=2) + '\n')
    print(state['status'], flush=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['parent', 'icarus', 'verilator', 'formal', 'output']:
        parser.add_argument('--' + name, type=Path, required=True)
    a = parser.parse_args()
    prepare(a.parent.resolve(), [a.icarus.resolve(), a.verilator.resolve()], a.formal.resolve(), a.output.resolve())
