#!/usr/bin/env python3
"""Prove a fresh, stateless CP prefix for uncommitted processor subevents.

Read the actual qualified control evaluator, without native canonical-owner
specialization. Derive the prefix from this call's state/pin epoch and use the
direct path only when that exact prefix is an identity. Compare all475 outputs
to always-ordered evaluation with the same fresh prefix, for arbitrary binary
owners and pins. This establishes a pure function, not physical packet timing.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from prove_cp_prefix_cache import module
from test_r_front_forward_view import sha

PURE='''module dorado_control_pure_prefix_step #(parameter integer WAVES=5)(
 input wire [155:0] state_q,input wire clock_before,clock_after,
 input wire [397:0] pins_before,pins_after,input wire refresh,
 output wire [474:0] result,output reg using_direct);
 wire split=(clock_before!=clock_after)&&(pins_before[292:282]!=pins_after[292:282]);
 wire [10:0] prefix_cp=split ? pins_after[292:282] : pins_before[292:282];
 wire [268:0] fresh_prefix;
 always @*begin
  using_direct=0;
  case(^{state_q,clock_before,clock_after,pins_before,pins_after,refresh})
   1'b0,1'b1:using_direct=!split && fresh_prefix=={state_q,56'b0,56'b0,1'b0};
   default:using_direct=0;
  endcase
 end
 dorado_control_hold_ordered_step #(.WAVES(WAVES)) actual(
  state_q,clock_before,clock_after,pins_before,pins_after,
  result[474:319],result[318:263],result[262:207],result[206],refresh,result[205:112],result[111:56],result[55:0],
  using_direct,fresh_prefix,prefix_cp,pins_after[292:282],fresh_prefix);
endmodule
'''
TOP='''module pure_prefix_equivalence(
 input wire [155:0] q,input wire before_root,after_root,
 input wire [397:0] before_pins,after_pins,input wire refresh,
 output wire equal,output wire [474:0] actual,expected,output wire direct);
 wire split=(before_root!=after_root)&&(before_pins[292:282]!=after_pins[292:282]);
 wire [10:0] prefix_cp=split ? after_pins[292:282] : before_pins[292:282];
 wire [268:0] reference_prefix;
 dorado_control_hold_ordered_step #(.WAVES(WAVES_VALUE)) reference_step(
  q,before_root,after_root,before_pins,after_pins,
  expected[474:319],expected[318:263],expected[262:207],expected[206],refresh,expected[205:112],expected[111:56],expected[55:0],
  1'b0,reference_prefix,prefix_cp,after_pins[292:282],reference_prefix);
 dorado_control_pure_prefix_step #(.WAVES(WAVES_VALUE)) actual_step(
  q,before_root,after_root,before_pins,after_pins,refresh,actual,direct);
 assign equal=actual==expected;
endmodule
'''


def run(source,proof,out,yosys):
    assert not out.exists()
    s=json.loads((source/'run-state.json').read_text())
    assert s['status']=='PASS_CONTROL_FALL_COMPLETE_ORIGINAL_CAPTURE'
    for row in s['source_files']:assert sha(source/row['path'])==row['output_sha256']
    r=json.loads((proof/'run-state.json').read_text())
    assert r['status']=='PASS_CERTIFIED_FALL_AND_PARALLEL_RISE_FULL_CONTROL'
    for name,h in r['files'].items():assert sha(proof/name)==h
    raw=(source/'candidate.sv').read_text()
    functions='\n'.join(module(raw,name) for name in ['dorado_control_hold_atomic_step','dorado_control_terminal_body',
        'dorado_control_hold_ordered_step','dorado_control_certified_fall_step','dorado_control_fall_step'])
    out.mkdir(parents=True)
    (out/'control.sv').write_text(functions);(out/'pure.sv').write_text(PURE)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),proof=str(proof),
        source_state_sha256=sha(source/'run-state.json'),proof_state_sha256=sha(proof/'run-state.json'),
        result_bits=475,physical_control_Qs=156,clocks=56,flags=94,CP_prefix_event_bits=112,
        input_state_and_pins_arbitrary=True,owner_or_cache_history_assumed=False,
        auxiliary_or_architectural_state_added=0,packet_memory_or_physical_timing_qualified=False,hardware_changed=False,
        files={p.name:sha(p) for p in out.iterdir() if p.is_file()},cases=[])

    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')

    save()
    for variant,waves in [('positive',5),('positive',24),('force-direct',5),('stale-prefix',5)]:
        d=out/f'{variant}-w{waves}';d.mkdir();actual=PURE
        if variant=='force-direct':actual=actual.replace("using_direct=!split && fresh_prefix=={state_q,56'b0,56'b0,1'b0};","using_direct=1;")
        if variant=='stale-prefix':actual=actual.replace('using_direct,fresh_prefix,prefix_cp,',"using_direct,fresh_prefix^(269'b1<<113),prefix_cp,",1)
        (d/'control.sv').write_text(functions);(d/'actual.sv').write_text(actual)
        (d/'top.sv').write_text(TOP.replace('WAVES_VALUE',str(waves)))
        flag='verify' if variant=='positive' else 'falsify'
        (d/'proof.ys').write_text(f'''read_verilog -sv -defer /work/control.sv /work/actual.sv /work/top.sv
hierarchy -check -top pure_prefix_equivalence
proc
flatten
memory_map
opt
check -assert
sat -timeout 120 -prove equal 1 -{flag} -show-ports -dump_json /work/counterexample.json
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with (d/'proof.log').open('x') as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT,env=env)
            row=dict(variant=variant,waves=waves,pid=p.pid,command=cmd,started=time.time(),
                files={p.name:sha(p) for p in d.iterdir() if p.is_file() and p.name!='proof.log'});state['cases'].append(row);save();code=p.wait()
        text=(d/'proof.log').read_text();passed=code==0 and ('no model found: SUCCESS!' if variant=='positive' else 'model found: FAIL!') in text
        row.update(returncode=code,passed=passed,finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        if not passed:break
    passed=len(state['cases'])==4 and all(r['passed'] for r in state['cases'])
    state.update(status='PASS_STATELESS_CONTROL_PREFIX_ALL_OUTPUTS' if passed else 'TERMINAL_PURE_CONTROL_PREFIX_NOT_ESTABLISHED',finished=time.time());save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['source','proof','output','yosys']:p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.proof.resolve(),a.output.resolve(),a.yosys.resolve()))
