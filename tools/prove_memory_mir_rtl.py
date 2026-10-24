#!/usr/bin/env python3
"""Prove the MIR Hold rewrite directly from both complete Verilog functions.

Compare all156 control/memory Qs,56 events,56 repeated events,94 flags and
the unsettled result, for955 arbitrary binary state/input bits. No pin or
state assumptions, timing waivers, unknown-cell skipping or X masking.
This proves a digital transfer rewrite, not original analog timing, the
parent's rejected CP-arrival profile, native integration or FPGA frequency.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import time

from probe_memory_mir_mask import candidate
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
TOP='''module memory_mir_equivalence(
 input wire [155:0] q,input wire before_root,after_root,
 input wire [397:0] before_pins,after_pins,input wire refresh,
 output wire equal,output wire [362:0] different);
 wire [155:0] old_q,new_q;
 wire [55:0] old_events,new_events,old_repeated,new_repeated;
 wire old_unsettled,new_unsettled;
 wire [93:0] old_flags,new_flags;
 original_atomic original(q,before_root,after_root,before_pins,after_pins,
  old_q,old_events,old_repeated,old_unsettled,refresh,old_flags);
 candidate_atomic candidate(q,before_root,after_root,before_pins,after_pins,
  new_q,new_events,new_repeated,new_unsettled,refresh,new_flags);
 assign different={old_q,old_events,old_repeated,old_unsettled,old_flags}^
                  {new_q,new_events,new_repeated,new_unsettled,new_flags};
 assign equal=~|different;
endmodule
'''

IDENTITY='''module memory_mir_equivalence(
 input wire [155:0] q,input wire before_root,after_root,
 input wire [397:0] before_pins,after_pins,input wire refresh,
 output wire equal,output wire [362:0] different);
 wire [155:0] canonical_q,identity_q;
 wire [55:0] first_events,identity_events,first_repeated,identity_repeated;
 wire first_unsettled,identity_unsettled;
 wire [93:0] first_flags,identity_flags;
 wire [397:0] identity_before=IDENTITY_BEFORE;
 candidate_atomic #(.WAVES(WAVE_COUNT)) canonical(q,before_root,after_root,before_pins,after_pins,
  canonical_q,first_events,first_repeated,first_unsettled,refresh,first_flags);
 candidate_atomic #(.WAVES(WAVE_COUNT)) unchanged(canonical_q,after_root,after_root,identity_before,after_pins,
  identity_q,identity_events,identity_repeated,identity_unsettled,refresh,identity_flags);
 assign different={canonical_q^identity_q,identity_events,identity_repeated,identity_unsettled,first_flags^identity_flags};
 // The optimization requires the last transaction to have settled and the
 // next before pins/root to be the exact saved previous after pins/root.
 assign equal=first_unsettled || ~|different;
endmodule
'''


def atomic(raw,name):
    start=raw.index('module dorado_control_hold_atomic_step ')
    end=raw.index('\nendmodule',start)+len('\nendmodule')
    body=raw[start:end]
    return body.replace('module dorado_control_hold_atomic_step ', 'module '+name+' ',1)


def run(experiment,yosys,out,negative,candidate_waves=24,identity=False,bad_before=False):
    assert not out.exists()
    state=json.loads((experiment/'run-state.json').read_text())
    assert (bool(state['mutation'])or bad_before)==negative
    assert not bad_before or identity
    parent=Path(state['parent']);baseline=parent/'candidate.sv'
    actual=experiment/'candidate.sv'
    assert sha(parent/'run-state.json')==state['parent_state_sha256']
    assert actual.read_text()==candidate(baseline.read_text(),state['mutation'],state.get('settling_waves',24),state.get('all_hold_clocks',False))
    out.mkdir(parents=True)
    (out/'baseline.sv').write_text(atomic(baseline.read_text(),'original_atomic')+'\n')
    (out/'candidate.sv').write_text(atomic(actual.read_text(),'candidate_atomic')+'\n')
    assert candidate_waves in range(1,25)
    top=IDENTITY.replace('WAVE_COUNT',str(candidate_waves)).replace('IDENTITY_BEFORE',
        "after_pins ^ (398'b1 << 292)"if bad_before else'after_pins')if identity else TOP.replace(
            'candidate_atomic candidate(',f'candidate_atomic #(.WAVES({candidate_waves})) candidate(')
    (out/'top.sv').write_text(top)
    script='''read_verilog -sv /work/baseline.sv /work/candidate.sv /work/top.sv
hierarchy -check -top memory_mir_equivalence
proc
flatten
opt
check -assert
sat -timeout 120 -prove equal 1 -FLAG -show-inputs -show-outputs -dump_json /work/counterexample.json
'''.replace('FLAG','falsify'if negative else'verify')
    (out/'proof.ys').write_text(script)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(ROOT/'build/rtl-executive/formal-tool-cache')
    env['YOWASP_MOUNT']='/work='+str(out)
    version=subprocess.check_output([str(yosys),'-V'],text=True,env=env)
    (out/'version.txt').write_text(version)
    python=yosys.parent/'python'
    packages=subprocess.check_output([str(python),'-m','pip','--cache-dir',
                                     str(ROOT/'build/rtl-executive/formal-tool-download-cache'),'freeze'],text=True)
    (out/'packages.txt').write_text(packages)
    record=dict(status='RUNNING',scope=__doc__,driver_pid=os.getpid(),started=time.time(),
                experiment=str(experiment),experiment_state_sha256=sha(experiment/'run-state.json'),
                original_candidate_sha256=sha(baseline),rewritten_candidate_sha256=sha(actual),
                state_and_input_bits=955,compared_output_bits=363,extra_state_or_guest_cycles=0,
                reference_atomic_waves=24,candidate_atomic_waves=candidate_waves,
                conditional_settled_identity=identity,identity_before_pin_perturbation=bad_before,
                identity_guard='Last transaction settled; second root unchanged; second before/after pins equal exact saved previous after pins'if identity else None,
                native_saved_pin_contract_qualified=False,
                arbitrary_binary_inputs=True,assumptions_added=False,negative_control=negative,
                source_files={p.name:sha(p)for p in out.iterdir()if p.is_file()},
                native_qualified=False,physical_timing_qualified=False,speedup_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(record,indent=2)+'\n')
    save();command=[str(yosys),'-Q','-T','-s','/work/proof.ys']
    with(out/'proof.log').open('x')as log:
        process=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT,env=env)
        record.update(pid=process.pid,command=command);save()
        code=process.wait()
    log=(out/'proof.log').read_text()
    success='SAT proof finished - no model found: SUCCESS!'in log
    counterexample='SAT proof finished - model found: FAIL!'in log
    passed=code==0 and (counterexample if negative else success)
    positive_status='PASS_CONDITIONAL_SETTLED_CP_IDENTITY'if identity else'PASS_ALL_BINARY_CONTROL_MIR_EQUIVALENCE'
    record.update(status=('PASS_RTL_COUNTEREXAMPLE_CONTROL'if negative else positive_status)
                         if passed else'TERMINAL_RTL_PROOF_NOT_ESTABLISHED',
                  returncode=code,finished=time.time(),proof_succeeded=success,
                  counterexample_found=counterexample,proof_log_sha256=sha(out/'proof.log'))
    save();print(record['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['experiment','yosys','output']:parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--expected-counterexample',action='store_true')
    parser.add_argument('--candidate-waves',type=int,default=24,choices=range(1,25))
    parser.add_argument('--settled-identity',action='store_true')
    parser.add_argument('--bad-identity-before-pins',action='store_true')
    args=parser.parse_args();raise SystemExit(run(args.experiment.resolve(),args.yosys.resolve(),args.output.resolve(),args.expected_counterexample,args.candidate_waves,args.settled_identity,args.bad_identity_before_pins))
