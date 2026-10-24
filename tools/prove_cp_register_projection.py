#!/usr/bin/env python3
"""Prove direct CP-register results equal the ordered prefix/root transfer.

Compare all12 CP owner bits for955 arbitrary binary state/input bits, with
no state/pin assumptions. This isolates the proposed guard-to-CP dependency
removal; all other control/event/flag outputs still need the ordered path.
No candidate, native core, clock constraint or image is changed by this tool.
"""
import argparse
import json
import os
from pathlib import Path
import subprocess
import time

from fit_cp_prefix_cache import gate
from prove_cp_prefix_cache import module
from prove_memory_mir_rtl import atomic
from test_r_front_forward_view import sha

TOP='''module cp_register_projection(
 input wire [155:0] q,input wire before_root,after_root,
 input wire [397:0] before_pins,after_pins,input wire refresh,
 output wire equal,output wire [11:0] different);
 wire [155:0] ordered_q,direct_q;
 original_ordered #(.WAVES(WAVE_COUNT)) original(
  .state_q(q),.clock_before(before_root),.clock_after(after_root),
  .pins_before(before_pins),.pins_after(after_pins),.state_next(ordered_q),
  .clock_events(),.repeated_events(),.unsettled(),.refresh_before_run_capture(refresh),
  .flags(),.cp_prefix_events(),.cp_prefix_repeated());
 dorado_control_hold_atomic_step #(.WAVES(WAVE_COUNT)) direct(
  .state_q(q),.clock_before(before_root),.clock_after(after_root),
  .pins_before(before_pins),.pins_after(after_pins),.state_next(direct_q),
  .clock_events(),.repeated_events(),.unsettled(),.refresh_before_run_capture(refresh),.flags());
 assign different=ordered_q[75:64] ^ PROJECTION;
 assign equal=~|different;
endmodule
'''


def run(experiment,yosys,out,stale):
    assert not out.exists()
    state,_=gate(experiment,'verilator')
    parent=Path(state['parent'])
    raw=(experiment/'candidate.sv').read_text()
    waves=state.get('settling_waves',8)
    ordered=module((parent/'candidate.sv').read_text(),'dorado_control_hold_ordered_step')
    ordered=ordered.replace('module dorado_control_hold_ordered_step ', 'module original_ordered ',1)
    out.mkdir(parents=True)
    sources={'atomic.sv':atomic(raw,'dorado_control_hold_atomic_step')+'\n','ordered.sv':ordered,
             'top.sv':TOP.replace('WAVE_COUNT',str(waves)).replace('PROJECTION','q[75:64]'if stale else'direct_q[75:64]')}
    for name,text in sources.items():(out/name).write_text(text)
    script='''read_verilog -sv /work/atomic.sv /work/ordered.sv /work/top.sv
hierarchy -check -top cp_register_projection
proc
flatten
opt
check -assert
sat -timeout 120 -prove equal 1 -FLAG -show-inputs -show-outputs -dump_json /work/counterexample.json
'''.replace('FLAG','falsify'if stale else'verify')
    (out/'proof.ys').write_text(script)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
    (out/'version.txt').write_text(subprocess.check_output([str(yosys),'-V'],text=True,env=env))
    record=dict(status='RUNNING',scope=__doc__,driver_pid=os.getpid(),started=time.time(),
                experiment=str(experiment),experiment_state_sha256=sha(experiment/'run-state.json'),
                parent_source_sha256=sha(parent/'candidate.sv'),candidate_source_sha256=sha(experiment/'candidate.sv'),
                state_and_input_bits=955,compared_output_bits=12,compared_joint_Q_bits=[64,75],
                settling_waves=waves,stale_projection_control=stale,assumptions_added=False,
                source_files={p.name:sha(p)for p in out.iterdir()if p.is_file()},
                projection_implemented=False,native_qualified=False,physical_timing_qualified=False,speedup_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(record,indent=2)+'\n')
    save()
    command=[str(yosys),'-Q','-T','-s','/work/proof.ys']
    with(out/'proof.log').open('x')as log:
        process=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT,env=env)
        record.update(pid=process.pid,command=command);save();code=process.wait()
    text=(out/'proof.log').read_text();success='SAT proof finished - no model found: SUCCESS!'in text
    counterexample='SAT proof finished - model found: FAIL!'in text
    passed=code==0 and(counterexample if stale else success)
    record.update(status=('PASS_STALE_CP_PROJECTION_COUNTEREXAMPLE'if stale else'PASS_DIRECT_CP_REGISTER_PROJECTION')
                  if passed else'TERMINAL_CP_PROJECTION_NOT_ESTABLISHED',
                  returncode=code,finished=time.time(),proof_succeeded=success,counterexample_found=counterexample,
                  proof_log_sha256=sha(out/'proof.log'))
    save();print(record['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['experiment','yosys','output']:parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--stale-projection',action='store_true')
    a=parser.parse_args();raise SystemExit(run(a.experiment.resolve(),a.yosys.resolve(),a.output.resolve(),a.stale_projection))
