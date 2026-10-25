#!/usr/bin/env python3
"""Derive support of actual HIGH identity and initial HIGH clock levels.

The generic transfer reads before-pins only to obtain its initial56 clocks.
A falling guard therefore needs to preserve that clock vector and its HIGH
identity certificate, rather than all input data used on later rising edges.
Derive conservative support for these57 bits; independently prove projection
against the unchanged whole wrapper at5/24 waves. This audit alone does not
authorize a guard change or qualify a processor, native timing or speed.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from audit_control_pin_support import support
from prove_control_fall_fusion import sources
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_r_front_forward_view import sha

TOP='''module phase_function(input wire [155:0] q,input wire [397:0] pins,
 input wire refresh,output wire [56:0] result);
 FUNCTIONS
 wire [155:0] n;wire [55:0] events,repeated;wire unsettled;
 INSTANCE
 assign result={((n==q) && !(|events) && !(|repeated) && !UNSETTLED),joint_clocks(q,1'b1,pins,q[43],q[41])};
endmodule
'''
BINARY="""dorado_control_terminal_body #(.WAVES(4)) identity(q,1'b1,1'b1,pins,pins,n,events,repeated,,refresh,,,,);"""
FULL="""dorado_control_hold_atomic_step #(.WAVES(WAVES_VALUE)) identity(q,1'b1,1'b1,pins,pins,n,events,repeated,unsettled,refresh,);"""
MITER='''module phase_support_equivalence(input wire [155:0] q,
 input wire [397:0] pins,alternate,input wire refresh,
 output wire equal,output wire [56:0] actual,projected);
 localparam [397:0] CARE=398'hMASK;
 wire [397:0] changed=(pins & CARE)|(alternate & ~CARE);
 phase_function original(q,pins,refresh,actual);
 phase_function candidate(q,changed,refresh,projected);
 assign equal=actual==projected;
endmodule
'''


def run(source,out,yosys):
    assert not out.exists()
    s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
    assert s['status']=='PASS_OWNED_FULL_PROCESSOR_PACKETS' and s['mutation'] is None
    raw=(source/'candidate.sv').read_text();atomic,_=sources(raw)
    wrapper=module(raw,'dorado_control_hold_atomic_step')
    assert 'generate if(WAVES==5 || WAVES==24)begin: shortened' in wrapper
    assert "{short_q,short_events,short_repeated,1'b0,short_flags}" in wrapper
    terminal=module(raw,'dorado_control_terminal_body')
    start=terminal.index('    function automatic') if '    function automatic' in terminal else terminal.index(' function automatic')
    functions=terminal[start:terminal.index(' // Pure diagnostic wave vectors',start)]
    # Exact source binding for the only read of pins_before in the body.
    assert terminal.count('pins_before')==2
    assert 'previous=joint_clocks(q,clock_before,pins_before,mir_switch_view,tasking_view);' in terminal
    assert 'mir_switch_view=q[43];' in terminal and 'tasking_view=q[41];' in terminal
    out.mkdir(parents=True);(out/'atomic.sv').write_text(atomic)
    cone=TOP.replace('FUNCTIONS',functions).replace('INSTANCE',BINARY).replace('UNSETTLED',"1'b0")
    (out/'cone-top.sv').write_text(cone)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),
               source_state_sha256=sha(source/'run-state.json'),compared_phase_bits=57,arbitrary_binary_miter_input_bits=953,
               before_pins_used_only_for_initial_clocks_source_bound=True,no_state_result_or_clock_masking=True,
               files={p.name:sha(p) for p in out.iterdir()},actions=[],guard_change_authorized=False,native_integrated=False,hardware_changed=False)
    def save():
        (out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    def action(label,script):
        (out/(label+'.ys')).write_text(script)
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
        cmd=[str(yosys),'-Q','-T','-s','/work/'+label+'.ys']
        row=dict(label=label,command=cmd,started=time.time(),script_sha256=sha(out/(label+'.ys')))
        with (out/(label+'.log')).open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT,env=env)
            row['pid']=child.pid;state['actions'].append(row);save();code=child.wait()
        row.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        assert code==0,label
        return (out/(label+'.log')).read_text()
    save()
    try:
        action('cone','''read_verilog -sv -defer /work/atomic.sv /work/cone-top.sv
hierarchy -check -top phase_function
proc
flatten
opt
wreduce
opt
check -assert
write_json /work/cone.json
''')
        ports=support(json.loads((out/'cone.json').read_text())['modules']['phase_function'],['pins'])
        syntactic=ports['pins']
        # Syntactic cones can retain data whose effect is dominated by an
        # already-false identity predicate. Test the WHOLE task-wake group
        # semantically, without constraining q or assuming that predicate.
        care=[i for i in syntactic if i not in range(20,35)]
        mask=sum(1<<i for i in care)
        assert care and len(care)<398 and 345 in care
        state.update(syntactic_pin_support=syntactic,semantic_candidate_excluded_group=list(range(20,35)),
                     guard_pin_support=care,guard_pin_count=len(care),support_mask_hex=format(mask,'x'),
                     task_wake_pins_in_support=[i for i in range(20,35) if i in care],cone_json_sha256=sha(out/'cone.json'));save()
        for label,waves,value in [('positive-w5',5,mask),('positive-w24',24,mask),('missing-selector345-w5',5,mask & ~(1<<345))]:
            full=TOP.replace('FUNCTIONS',functions).replace('INSTANCE',FULL.replace('WAVES_VALUE',str(waves))).replace('UNSETTLED','unsettled')
            (out/(label+'-source.sv')).write_text(full)
            (out/(label+'-top.sv')).write_text(MITER.replace('MASK',format(value,'x')))
            positive=label.startswith('positive');flag='verify' if positive else 'falsify'
            log=action(label,f'''read_verilog -sv -defer /work/atomic.sv /work/{label}-source.sv /work/{label}-top.sv
hierarchy -check -top phase_support_equivalence
proc
flatten
opt
check -assert
sat -timeout 120 -prove equal 1 -{flag} -show-ports -dump_json /work/{label}-counterexample.json
''')
            assert ('no model found: SUCCESS!' if positive else 'model found: FAIL!') in log
        state.update(status='PASS_CONTROL_FALL_PHASE_SUPPORT',finished=time.time())
    except BaseException:
        state.update(status='TERMINAL_FALL_PHASE_SUPPORT_NOT_ESTABLISHED',finished=time.time());save();raise
    save();print(state['status'],len(care),'pins; task wake pins =',state['task_wake_pins_in_support'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['source','output','yosys']:
        p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();run(a.source.resolve(),a.output.resolve(),a.yosys.resolve())
