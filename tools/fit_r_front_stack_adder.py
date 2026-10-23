#!/usr/bin/env python3
"""Fit the binary-equivalent native stack adder in the unchanged 15 ns shell.

Qualification uses the existing six-profile MIR-mask gate, exhaustive generic
and actual-wiring equivalence, and two complete new processor-graph runs.
These are explicitly separate evidence classes, not twelve new profile runs.
No native/IO/ROM timing, assembly, deployment or hardware speed is qualified.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil

import fit_control_clock as fixture
from fit_r_front import quartus_cone
from fit_r_front_mir_mask import ROOT, BUILD, RTL, EVALUATOR, CANDIDATE_SHA, top_source
from rewrite_stack_adder import rewrite, PARENT_SHA

CONE_SHA = '6db485f25d397639fe381c4b3390bc0b7338a2e659ed17a654517b4f9c3c7ea6'
PARENT = BUILD/'r-front-mir-mask-budget-gate-v1-20261023'


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def qualify(out):
    assert not out.exists()
    parent = json.loads((PARENT/'results.json').read_text())
    assert parent['status'] == 'PASS_BOUNDED_R_FRONT_MIR_MASK_BUDGET_GATE'
    assert len(parent['arrivals']) == 12 and len(parent['negative_cases']) == 2
    unit = BUILD/'stack-adder-equivalence-v2-20261023'
    proof = json.loads((unit/'results.json').read_text())
    assert proof['status'] == 'PASS_STACK_ADDER_BINARY_EQUIVALENCE'
    assert len(proof['actions']) == 12 and all('returncode' in a for a in proof['actions'])
    assert proof['cone_sha256'] == CONE_SHA
    assert {r['engine'] for r in proof['cases']} == {'icarus', 'verilator'}
    for r in proof['cases']:
        if r['mutation'] is None:
            assert r['checks'] == 4096 and r['physical_wiring_checks'] == 512 and r['result'] == 'PASS'
        else:
            assert r['result'] == 'REJECTED_CARRY_ERROR'
    assert proof['source_sha256'] == parent['inputs']['verilog/cells/cell_MC10182.v']
    rows = []
    last = None
    last_state = None
    for engine in ['icarus', 'verilator']:
        d = BUILD/f'r-front-stack-adder-{engine}-v1-20261023'
        s = json.loads((d/'run-state.json').read_text())
        assert s['status'] == 'PASS_R_FRONT_LOCAL_PROBE'
        assert len(s['actions']) == 2 and all(r['returncode'] == 0 for r in s['actions'])
        assert s['summary'] == 'checks=13515 writes=328 reads=5608 epochs=797'
        assert s['executed_candidate_sha256'] == CANDIDATE_SHA
        assert s['inherited_cone_sha256'] == PARENT_SHA and s['executed_cone_sha256'] == CONE_SHA
        assert s['cone_rewrite']['connected_outputs_preserved'] and s['cone_rewrite']['non_arithmetic_source_unchanged']
        for n, digest in s['inputs'].items():
            assert sha(d/'inputs'/n) == digest, (d, n)
        log = (d/'test.log').read_text()
        assert 'MIR_MASK_LATE_NODISPATCH root=0 MIRcaptures=1 SwitchUp_n=1' in log
        assert 'R_FRONT_COMMANDS_DIRECTED commands=8 FF=12 JCN=4 intervals=120 WriteIM=16' in log
        rows.append(dict(engine=engine, run=d.name, summary=s['summary'], run_state_sha256=sha(d/'run-state.json')))
        last = d
        last_state = s
    out.mkdir(parents=True)
    shutil.copytree(PARENT/'executed-inputs', out/'executed-inputs')
    # Keep the inherited profile ledger separate from the sources actually
    # executed by the two new graph comparisons. RTL bytes are unchanged;
    # the new driver and cone transformation must also be reviewable.
    hashes = dict(parent['inputs'])
    for n, digest in last_state['inputs'].items():
        p = out/'executed-inputs'/n
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes((last/'inputs'/n).read_bytes())
        assert sha(p) == digest, n
        hashes[n] = digest
    shutil.copytree(PARENT/'mask-properties', out/'mask-properties')
    shutil.copytree(unit, out/'adder-equivalence', ignore=shutil.ignore_patterns('obj', 'test.vvp'))
    (out/'inherited-mask-gate.json').write_bytes((PARENT/'results.json').read_bytes())
    (out/'original-cone.sv.txt').write_bytes((PARENT/'dorado_r_front_cone.sv').read_bytes())
    cone = out/'dorado_r_front_cone.sv'
    cone.write_bytes((PARENT/'dorado_r_front_cone.sv').read_bytes())
    transform = rewrite(cone)
    assert transform == json.loads((unit/'transform.json').read_text())
    assert sha(cone) == CONE_SHA == sha(last/'dorado_r_front_cone.sv')
    (out/'adder-transform.json').write_text(json.dumps(transform, indent=2)+'\n')
    meta = json.loads((PARENT/'dorado_r_front_cone.json').read_text())
    assert meta['output_sha256'] == PARENT_SHA
    meta.update(output_sha256=CONE_SHA, parent_output_sha256=PARENT_SHA,
                shared_combinational_logic=False,
                scope='Original cone except binary-equivalent native six-bit stack adder; original reference unchanged.')
    (out/'dorado_r_front_cone.json').write_text(json.dumps(meta, indent=2)+'\n')
    record = dict(parent)
    record.update(status='PASS_BOUNDED_STACK_ADDER_BUDGET_GATE', scope=__doc__,
                  inputs=hashes,
                  arrival_qualification_method='binary_combinational_equivalence_bridge',
                  arrivals=rows, inherited_arrivals=parent['arrivals'],
                  inherited_negative_cases=parent['negative_cases'], negative_cases=[],
                  candidate_cone_sha256=CONE_SHA, parent_cone_sha256=PARENT_SHA,
                  adder_equivalence_sha256=sha(out/'adder-equivalence/results.json'),
                  adder_transform_sha256=sha(out/'adder-transform.json'),
                  inherited_mask_gate_sha256=sha(out/'inherited-mask-gate.json'))
    (out/'results.json').write_text(json.dumps(record, indent=2)+'\n')
    print(record['status'], '2 new full runs; 12 inherited profiles; exhaustive arithmetic/wiring', flush=True)


def prepare(qualified, out):
    gate = json.loads((qualified/'results.json').read_text())
    assert gate['status'] == 'PASS_BOUNDED_STACK_ADDER_BUDGET_GATE'
    assert gate['arrival_qualification_method'] == 'binary_combinational_equivalence_bridge'
    assert len(gate['arrivals']) == 2 and len(gate['inherited_arrivals']) == 12
    assert gate['candidate_cone_sha256'] == CONE_SHA
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
    for n in ['fit_control_clock.py', 'fit_r_front.py', 'fit_r_front_ready.py',
              'fit_r_front_mir_mask.py', 'fit_r_front_stack_adder.py']:
        (out/n).write_bytes((ROOT/'tools'/n).read_bytes())
    (out/'functional-gate.json').write_bytes((qualified/'results.json').read_bytes())
    record = dict(scope=__doc__, component='r_front_stack_adder', clock_period_ns=15,
                  ram_bits=9216, input_staging_bits=1429, launch_bits=1429, result_bits=1184,
                  observation_outputs_registered=False, observation_only_outputs=['source_pins', 'fa_n'],
                  files={p.name: sha(p) for p in out.iterdir() if p.is_file()},
                  assembly_permitted=False, native_qualified=False,
                  full_producer_arrival_contract_qualified=False)
    (out/'provenance.json').write_text(json.dumps(record, indent=2)+'\n')
    print('PREPARED_STACK_ADDER_15NS_BUDGET', sha(out/'top.sv'), flush=True)


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
