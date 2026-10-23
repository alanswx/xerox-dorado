#!/usr/bin/env python3
"""Fit explicit stored read-latch sourcing for visible R in the 15 ns shell.

Qualification uses the existing six-profile MIR-mask gate, exhaustive generic
and source-bound nonoverlap equivalence, and two complete new processor-graph runs.
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
from rewrite_r_visible import rewrite, PARENT_SHA

CONE_SHA = '60d63614c927570d9fbed383e837955510fe10242a3ef74574ff485e0e6647c5'
OPERAND_SHA = '2e760e63e1a4332b4f0af567b22044fff5f12e8e6ee04e672030a15a2733724b'
PARENT = BUILD/'r-front-mir-mask-budget-gate-v1-20261023'


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def qualify(out):
    assert not out.exists()
    parent=json.loads((PARENT/'results.json').read_text())
    assert parent['status']=='PASS_BOUNDED_R_FRONT_MIR_MASK_BUDGET_GATE'
    proofdir=BUILD/'r-visible-proof-v3-20261023'
    proof=json.loads((proofdir/'results.json').read_text())
    assert proof['status']=='PASS_BINARY_R_VISIBLE_NONOVERLAP'
    assert [r['result']for r in proof['cases']]==['unsat','sat','sat']
    assert proof['transform']['parent_sha256']==PARENT_SHA
    assert proof['transform']['output_sha256']==OPERAND_SHA
    rows=[];last=None;last_state=None
    for engine in ['icarus','verilator']:
        d=BUILD/f'r-front-visible-{engine}-v1-20261023'
        r=json.loads((d/'run-state.json').read_text())
        assert r['status']=='PASS_R_FRONT_LOCAL_PROBE'
        assert len(r['actions'])==2 and all(x['returncode']==0 for x in r['actions'])
        assert r['summary']=='checks=13515 writes=328 reads=5608 epochs=797'
        assert r['executed_candidate_sha256']==CANDIDATE_SHA and r['executed_cone_sha256']==CONE_SHA
        assert sha(d/'adapters/dorado_local_r_operand_step.sv')==OPERAND_SHA
        assert r['adapters']['visible_rewrite']==proof['transform']
        for n,digest in r['inputs'].items():assert sha(d/'inputs'/n)==digest,n
        rows.append(dict(engine=engine,run=d.name,summary=r['summary'],run_state_sha256=sha(d/'run-state.json')))
        last=d;last_state=r
    out.mkdir(parents=True)
    shutil.copytree(PARENT/'executed-inputs',out/'executed-inputs')
    hashes=dict(parent['inputs'])
    for n,digest in last_state['inputs'].items():
        p=out/'executed-inputs'/n;p.parent.mkdir(parents=True,exist_ok=True)
        p.write_bytes((last/'inputs'/n).read_bytes());assert sha(p)==digest,n
        hashes[n]=digest
    changed=[]
    for n in RTL:
        source=last/'adapters'/n
        if not source.exists():continue
        digest=sha(source);old=hashes['verilog/rtl/'+n]
        if digest!=old:changed.append(n)
        (out/'executed-inputs/verilog/rtl'/n).write_bytes(source.read_bytes())
        hashes['verilog/rtl/'+n]=digest
    assert changed==['dorado_local_r_operand_step.sv'],changed
    for n in ['dorado_r_front_cone.sv','dorado_r_front_cone.json']:
        shutil.copyfile(PARENT/n,out/n)
    shutil.copytree(PARENT/'mask-properties',out/'mask-properties')
    shutil.copytree(proofdir,out/'visible-proof')
    shutil.copyfile(PARENT/'results.json',out/'inherited-mask-gate.json')
    shutil.copyfile(PARENT/'executed-inputs/verilog/rtl/dorado_local_r_operand_step.sv',out/'original-operand.sv.txt')
    record=dict(parent)
    record.update(status='PASS_BOUNDED_R_FRONT_VISIBLE_BUDGET_GATE',scope=__doc__,inputs=hashes,
                  arrival_qualification_method='binary_visible_nonoverlap_equivalence_bridge',
                  arrivals=rows,inherited_arrivals=parent['arrivals'],inherited_negative_cases=parent['negative_cases'],
                  negative_cases=[],visible_operand_sha256=OPERAND_SHA,parent_operand_sha256=PARENT_SHA,
                  visible_proof_sha256=sha(out/'visible-proof/results.json'),
                  inherited_mask_gate_sha256=sha(out/'inherited-mask-gate.json'))
    (out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    print(record['status'],'2 new full graph runs; 12 inherited; source-bound binary proof',flush=True)


def prepare(qualified, out):
    gate = json.loads((qualified/'results.json').read_text())
    assert gate['status'] == 'PASS_BOUNDED_R_FRONT_VISIBLE_BUDGET_GATE'
    assert gate['arrival_qualification_method'] == 'binary_visible_nonoverlap_equivalence_bridge'
    assert len(gate['arrivals']) == 2 and len(gate['inherited_arrivals']) == 12
    assert gate['visible_operand_sha256'] == OPERAND_SHA
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
              'fit_r_front_mir_mask.py', 'fit_r_front_visible.py']:
        (out/n).write_bytes((ROOT/'tools'/n).read_bytes())
    (out/'functional-gate.json').write_bytes((qualified/'results.json').read_bytes())
    record = dict(scope=__doc__, component='r_front_visible', clock_period_ns=15,
                  ram_bits=9216, input_staging_bits=1429, launch_bits=1429, result_bits=1184,
                  observation_outputs_registered=False, observation_only_outputs=['source_pins', 'fa_n'],
                  files={p.name: sha(p) for p in out.iterdir() if p.is_file()},
                  assembly_permitted=False, native_qualified=False,
                  full_producer_arrival_contract_qualified=False)
    (out/'provenance.json').write_text(json.dumps(record, indent=2)+'\n')
    print('PREPARED_VISIBLE_R_15NS_BUDGET', sha(out/'top.sv'), flush=True)


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
