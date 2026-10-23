#!/usr/bin/env python3
"""Budget the source-bound MIR-clock mask in the original 15 ns fit shell.

All 792 Qs, events, functional ports, parity and invariants stay registered.
Only source_pins/fa_n remain test observations, as in the rejected parent.
This bounded component experiment cannot assemble, deploy or qualify native IO.
"""
import argparse
import hashlib
import json
from pathlib import Path

import fit_control_clock as fixture
from fit_r_front import quartus_cone
from fit_r_front_ready import RTL as PARENT_RTL, FIELDS as PARENT_FIELDS
from generate_r_front_local_adapters import NEW
from prove_mir_clock_mask import prove

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT/'build/rtl-executive'
SUITE = BUILD/'r-front-mir-mask-full-v4-20261023'
EVALUATOR = 'dorado_r_front_commands_step.sv'
ADAPTERS = [n+'.sv' for n in NEW.values()]
RTL = [EVALUATOR, *PARENT_RTL, *ADAPTERS]
FIELDS = [(n, {'next_q': 792, 'events': 68, 'repeated': 68}.get(n, w)) for n, w in PARENT_FIELDS]+[
    ('conta_next', 104), ('shared_state_mismatch', 1)]
CANDIDATE_SHA = 'e695318064d80a9e308ff5e76e3fd57d86bb55412a448b163da135856883dd0e'


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def qualify(out):
    assert not out.exists()
    suite = json.loads((SUITE/'results.json').read_text())
    assert suite['status'] == 'PASS_BOUNDED_R_FRONT_MIR_MASK_ORDERING' and not suite['failures']
    assert len(suite['primary']) == 12 and len(suite['negative_cases']) == 2
    assert len({(r['profile'], r['engine']) for r in suite['primary']}) == 12
    baseline = BUILD/'r-front-commands-full-v4-20261023'
    command = json.loads((baseline/'results.json').read_text())
    assert command['status'] == 'PASS_BOUNDED_R_FRONT_COMMAND_ORDERING'
    assert len(command['primary']) == 12 and len(command['negative_cases']) == 8
    boundaries = BUILD/'r-front-commands-boundaries-v2-20261023/results.json'
    audit = json.loads(boundaries.read_text())
    assert audit['status'] == 'PASS_UNUSED_R_FRONT_COMMAND_GATE_BOUNDARIES'
    assert len(audit['boundaries']) == 12 and len(audit['rejected_controls']) == 5
    last = None
    for row in suite['cases']:
        d = SUITE/row['directory']
        assert sha(d/'run-state.json') == row['run_state_sha256']
        s = json.loads((d/'run-state.json').read_text())
        assert s['status'] != 'RUNNING' and len(s['actions']) == 2
        assert s['actions'][0]['returncode'] == 0
        for n, digest in s['inputs'].items():
            assert sha(d/'inputs'/n) == digest, (d, n)
        log = (d/'test.log').read_text()
        if row['frozen']:
            assert s['actions'][1]['returncode'] != 0 and 'R front state mismatch check=6279' in log
        else:
            assert s['actions'][1]['returncode'] == 0 and sha(d/'candidate.sv') == CANDIDATE_SHA
            assert s['summary'] == 'checks=13515 writes=328 reads=5608 epochs=797'
            assert 'MIR_MASK_LATE_NODISPATCH root=0 MIRcaptures=1 SwitchUp_n=1' in log
            assert all(r['inverse_byte_identical'] for r in s['adapters']['files'])
            assert s['reference']['physical_bits'] == 792 and s['reference']['clock_channels'] == 68
            last = (d, s)
    out.mkdir(parents=True)
    d, s = last
    inputs = out/'executed-inputs'
    hashes = dict(s['inputs'])
    for n, digest in hashes.items():
        p = inputs/n
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes((d/'inputs'/n).read_bytes())
        assert sha(p) == digest
    for name in ADAPTERS:
        n = 'verilog/rtl/'+name
        p = inputs/n
        p.write_bytes((d/'adapters'/name).read_bytes())
        hashes[n] = sha(p)
    n = 'verilog/rtl/'+EVALUATOR
    (inputs/n).write_bytes((d/'candidate.sv').read_bytes())
    hashes[n] = CANDIDATE_SHA
    parent = BUILD/'r-front-ready-qualified-v1-20261020'
    for name in ['dorado_r_front_cone.sv', 'dorado_r_front_cone.json']:
        (out/name).write_bytes((parent/name).read_bytes())
    prove(inputs/n, out/'mask-properties')
    record = dict(status='PASS_BOUNDED_R_FRONT_MIR_MASK_BUDGET_GATE', scope=__doc__,
                  inputs=hashes, physical_bits=792, ram_bits=9216, total_physical_bits=10008,
                  bounded_original_graph_checks=True, all_R_memory_bits_compared=True,
                  r_read_write_nonoverlap_checked=True, post_commit_source_epoch_checked=True,
                  ram_arrays_flattened=False, full_producer_arrival_contract_qualified=False,
                  native_integrated=False, physical_timing_qualified=False, speedup_qualified=False,
                  compiled_rtl_files=RTL, candidate_sha256=CANDIDATE_SHA,
                  arrivals=suite['primary'], negative_cases=suite['negative_cases'],
                  source_bound_properties_sha256=sha(out/'mask-properties/proof.json'),
                  command_family_results_sha256=sha(baseline/'results.json'),
                  boundary_results_sha256=sha(boundaries))
    (out/'results.json').write_text(json.dumps(record, indent=2)+'\n')
    print(record['status'], flush=True)


def top_source():
    stored = [n for n, w in FIELDS if n not in ['source_pins', 'fa_n']]
    assert sum(w for n, w in FIELDS if n in stored) == 1184
    top = '''module clock_fit(input wire host_clk,input wire [1428:0] pins,output reg [1183:0] result);
 reg [1428:0] input_stage,launch;
 always @(negedge host_clk) input_stage<=pins;
 always @(posedge host_clk) launch<=input_stage;
 wire [791:0] q;
 wire [316:0] before_controls,after_controls;
 wire before_clock,after_clock,advance;
 assign {q,before_clock,after_clock,before_controls,after_controls,advance}=launch;
'''
    top += ''.join(' wire '+('' if w == 1 else f'[{w-1}:0] ')+n+';\n' for n, w in FIELDS)
    top += ' dorado_r_front_commands_step evaluator(host_clk,advance,q,before_clock,after_clock,before_controls,after_controls,\n  '+','.join(n for n, w in FIELDS)+');\n'
    top += ' always @(posedge host_clk) result<={'+','.join(stored)+'};\nendmodule\n'
    return top


def prepare(qualified, out):
    gate = json.loads((qualified/'results.json').read_text())
    assert gate['status'] == 'PASS_BOUNDED_R_FRONT_MIR_MASK_BUDGET_GATE'
    assert len(gate['arrivals']) == 12 and gate['compiled_rtl_files'] == RTL
    assert not gate['native_integrated'] and not gate['full_producer_arrival_contract_qualified']
    for n, digest in gate['inputs'].items():
        assert sha(qualified/'executed-inputs'/n) == digest, n
    out.mkdir(parents=True, exist_ok=False)
    qsf = fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
        ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+n+'\n' for n in [*RTL, 'dorado_r_front_cone.sv']))
    for n, raw in {'top.sv': top_source(), 'clock.sdc': fixture.SDC, 'clock_fit.qsf': qsf,
                   'clock_fit.qpf': 'PROJECT_REVISION = "clock_fit"\n', 'audit.tcl': fixture.AUDIT}.items():
        (out/n).write_text(raw)
    for n in RTL:
        (out/n).write_bytes((qualified/'executed-inputs/verilog/rtl'/n).read_bytes())
    original = (qualified/'dorado_r_front_cone.sv').read_text()
    (out/'dorado_r_front_cone.sv').write_text(quartus_cone(original))
    (out/'qualified-cone.sv.txt').write_text(original)
    for n in ['fit_control_clock.py', 'fit_r_front.py', 'fit_r_front_ready.py', 'fit_r_front_mir_mask.py']:
        (out/n).write_bytes((ROOT/'tools'/n).read_bytes())
    (out/'functional-gate.json').write_bytes((qualified/'results.json').read_bytes())
    record = dict(scope=__doc__, component='r_front_mir_mask', clock_period_ns=15,
                  ram_bits=9216, input_staging_bits=1429, launch_bits=1429, result_bits=1184,
                  observation_outputs_registered=False, observation_only_outputs=['source_pins', 'fa_n'],
                  files={p.name: sha(p) for p in out.iterdir() if p.is_file()},
                  assembly_permitted=False, native_qualified=False,
                  full_producer_arrival_contract_qualified=False)
    (out/'provenance.json').write_text(json.dumps(record, indent=2)+'\n')
    print('PREPARED_BOUNDED_R_FRONT_MIR_MASK_15NS_BUDGET', sha(out/'top.sv'), flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    g = p.add_mutually_exclusive_group(required=True)
    g.add_argument('--qualify', type=Path)
    g.add_argument('--prepare', type=Path)
    g.add_argument('--run', type=Path)
    p.add_argument('--qualified', type=Path)
    p.add_argument('--quartus', type=Path, default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a = p.parse_args()
    if a.qualify:
        qualify(a.qualify.resolve())
    elif a.prepare:
        assert a.qualified
        prepare(a.qualified.resolve(), a.prepare.resolve())
    else:
        fixture.run(a.run.resolve(), a.quartus.resolve())
