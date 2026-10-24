#!/usr/bin/env python3
"""Qualify sparse falling-root transfer and parallel fall/rise composition.

Use the ACTUAL native156-Q/56-clock/94-flag control evaluator. The domain is
an independently checked identity prefix at rootHIGH, with stable external
pins through both source edges. This is the existing canonical certificate,
not an assumption that every guest state is canonical. Other transactions
must retain the generic/staged path. No native integration or speed follows.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from prove_cp_prefix_cache import module
from prove_native_cp_context import RTL
from test_r_front_forward_view import sha

HELPER = '''// Sparse physical falling-root candidate. Use ONLY with the proved
// rootHIGH identity certificate and unchanged pins. No added saved state.
module dorado_control_fall_step(
 input wire [155:0] state_q,input wire [397:0] pins,input wire refresh,
 output wire [155:0] state_next,output wire [55:0] events,repeated,
 output wire unsettled,output wire [93:0] flags);
 FUNCTIONS
 wire [2:0] commands=memory_commands(state_q[97:76]);
 wire capture_cram=!(state_q[6] || !state_q[7]);
 wire next_cram=commands[1] ? 1'b1 : (capture_cram ? !state_q[27] : state_q[30]);
 assign state_next={state_q[155:31],next_cram,state_q[29:0]};
 assign events=56'(capture_cram)<<8;
 assign repeated=0;assign unsettled=0;
 assign flags={{state_next[150],1'b1,{6{hold_at(state_next[155:150])}}},
  cb_flags_at(state_next[149:110],cb_pins(state_next,1'b0,pins)),ca_flags_at(state_next,pins)};
endmodule
'''
TOP = '''module fall_equivalence #(parameter integer WAVES=5)(
 input wire [155:0] q,input wire [397:0] pins,input wire refresh,
 output wire valid,equal,output wire [362:0] actual,expected,
 output wire [155:0] identity_q,fall_q);
 wire [55:0] identity_events,identity_repeated,fall_events,fall_repeated;
 wire identity_unsettled,fall_unsettled;
 wire [93:0] identity_flags,fall_flags;
 dorado_control_hold_atomic_step #(.WAVES(WAVES)) identity(q,1'b1,1'b1,pins,pins,
  identity_q,identity_events,identity_repeated,identity_unsettled,refresh,identity_flags);
 assign valid=(identity_q==q) && !(|identity_events) && !(|identity_repeated) && !identity_unsettled;
 dorado_control_hold_atomic_step #(.WAVES(WAVES)) falling(q,1'b1,1'b0,pins,pins,
  fall_q,fall_events,fall_repeated,fall_unsettled,refresh,fall_flags);
 wire [155:0] fast_q;
 wire [55:0] fast_events,fast_repeated;
 wire fast_unsettled;
 wire [93:0] fast_flags;
 dorado_control_fall_step actual_fall(q,pins,refresh,fast_q,fast_events,fast_repeated,fast_unsettled,fast_flags);
 FALL_OR_FUSION
 assign equal=!valid || actual==expected;
endmodule
'''
FALL = ''' assign actual={fast_q,fast_events,fast_repeated,fast_unsettled,fast_flags};
 assign expected={fall_q,fall_events,fall_repeated,fall_unsettled,fall_flags};
'''
FUSION = ''' wire [155:0] serial_q,parallel_q;
 wire [55:0] serial_events,serial_repeated,parallel_events,parallel_repeated;
 wire serial_unsettled,parallel_unsettled;
 wire [93:0] serial_flags,parallel_flags;
 dorado_control_hold_atomic_step #(.WAVES(WAVES)) serial_rise(fall_q,1'b0,1'b1,pins,pins,
  serial_q,serial_events,serial_repeated,serial_unsettled,refresh,serial_flags);
 dorado_control_hold_atomic_step #(.WAVES(WAVES)) parallel_rise(q,1'b0,1'b1,pins,pins,
  parallel_q,parallel_events,parallel_repeated,parallel_unsettled,refresh,parallel_flags);
 // A new MIR can asynchronously SET Q30 during the rise. Evaluate that one
 // bit's zero cofactor in parallel; q30 has no influence on other owners.
 wire [155:0] low_initial_q={q[155:31],1'b0,q[29:0]};
 wire [155:0] low_rise_q;
 dorado_control_hold_atomic_step #(.WAVES(WAVES)) low_cram_rise(low_initial_q,1'b0,1'b1,pins,pins,
  low_rise_q,,,,refresh,);
 wire fused_cram=low_rise_q[30] | fast_q[30];
 wire [155:0] fused_q={parallel_q[155:31],fused_cram,parallel_q[29:0]};
 assign actual={fused_q,fast_events|parallel_events,
  fast_repeated|parallel_repeated|(fast_events&parallel_events),fast_unsettled|parallel_unsettled,parallel_flags};
 assign expected={serial_q,fall_events|serial_events,
  fall_repeated|serial_repeated|(fall_events&serial_events),fall_unsettled|serial_unsettled,serial_flags};
'''


def sources(raw):
    terminal=module(raw,'dorado_control_terminal_body')
    start=terminal.index('    function automatic') if '    function automatic' in terminal else terminal.index(' function automatic')
    end=terminal.index(' // Pure diagnostic wave vectors',start)
    functions=terminal[start:end]
    assert len(re.findall(r'\bfunction\b',functions))==len(re.findall(r'\bendfunction\b',functions))
    return module(raw,'dorado_control_hold_atomic_step')+terminal,HELPER.replace('FUNCTIONS',functions)


def run(stage,out,yosys,defer_frontend=False):
    assert not out.exists()
    manifest=stage/'r-front-control-native.json';s=json.loads(manifest.read_text())
    for name,digest in s['source_hashes'].items():
        assert sha(stage/name)==digest
    raw=(stage/RTL).read_text()
    atomic,helper=sources(raw)
    out.mkdir(parents=True)
    (out/'native-control.sv').write_text(raw)
    (out/'atomic.sv').write_text(atomic)
    (out/'dorado_control_fall_step.sv').write_text(helper)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,
        stage=str(stage),manifest_sha256=sha(manifest),control_source_sha256=sha(stage/RTL),
        compared_bits_per_case=363,physical_Q_bits=156,clock_channels=56,flag_bits=94,
        certificate='actual HIGH-root identity prefix; identical before/after pins',
        state_or_input_bits_masked=False,extra_saved_state_or_guest_cycles=0,
        native_integrated=False,timing_qualified=False,firmware_qualified=False,speedup_qualified=False,
        defer_unused_parameter_elaboration=defer_frontend,
        files={p.name:sha(p) for p in out.iterdir() if p.is_file()},cases=[])

    def save():
        (out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')

    save()
    cases=[('fall','positive',w) for w in [5,24]]+[('fusion','positive',w) for w in [5,24]]
    cases += [('fall-canonical','positive',5),('fusion-canonical','positive',5)]
    cases += [('fall','wrong-cram',5),('fall','drop-clock',5),('fall','without-certificate',5),
              ('fall','stale-pins',5),('fusion','stale-fall-cram',5)]
    for mode,variant,waves in cases:
        d=out/f'{mode}-{variant}-w{waves}';d.mkdir()
        actual=helper;top=TOP.replace('FALL_OR_FUSION',FALL if mode.startswith('fall') else FUSION)
        if mode.endswith('-canonical'):
            endpoint='fast_q' if mode.startswith('fall') else 'fused_q'
            root="1'b0" if mode.startswith('fall') else "1'b1"
            extra=f''' wire [155:0] closed_q;
 wire [55:0] closed_events,closed_repeated;
 wire closed_unsettled;
 dorado_control_hold_atomic_step #(.WAVES(WAVES)) certify({endpoint},{root},{root},pins,pins,
  closed_q,closed_events,closed_repeated,closed_unsettled,refresh,);
 wire new_certificate=(closed_q=={endpoint}) && !(|closed_events) && !(|closed_repeated) && !closed_unsettled;
'''
            top=top.replace(' assign equal=!valid || actual==expected;',extra+' assign equal=!valid || ((actual==expected)&&new_certificate);')
        if variant=='wrong-cram':
            actual=actual.replace('next_cram,state_q[29:0]','!next_cram,state_q[29:0]',1)
        if variant=='drop-clock':
            actual=actual.replace("assign events=56'(capture_cram)<<8;","assign events=56'b0;",1)
        if variant=='without-certificate':
            top=top.replace('assign equal=!valid || actual==expected;','assign equal=actual==expected;',1)
        if variant=='stale-pins':
            top=top.replace("falling(q,1'b1,1'b0,pins,pins,", "falling(q,1'b1,1'b0,pins,pins^(398'b1<<292),",1)
            top=top.replace('actual_fall(q,pins,refresh,', "actual_fall(q,pins^(398'b1<<292),refresh,",1)
        if variant=='stale-fall-cram':
            top=top.replace('low_rise_q[30] | fast_q[30]','fast_q[30]',1)
        if defer_frontend:
            # Elaborate the actual requested top parameter once. The default
            # 24-wave body is otherwise expanded even for every5-wave case.
            assert top.count('parameter integer WAVES=5')==1
            top=top.replace('parameter integer WAVES=5',f'parameter integer WAVES={waves}',1)
        (d/'actual.sv').write_text(actual)
        (d/'atomic.sv').write_text(atomic)
        (d/'top.sv').write_text(top)
        flag='verify' if variant=='positive' else 'falsify'
        frontend='-defer ' if defer_frontend else ''
        parameter='' if defer_frontend else f'chparam -set WAVES {waves} fall_equivalence\n'
        (d/'proof.ys').write_text(f'''read_verilog -sv {frontend}/work/atomic.sv /work/actual.sv /work/top.sv
{parameter}\
hierarchy -check -top fall_equivalence
proc
flatten
memory_map
opt
check -assert
sat -timeout 120 -prove equal 1 -{flag} -show-ports -dump_json /work/counterexample.json
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        command=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with (d/'proof.log').open('x') as log:
            p=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT,env=env)
            row=dict(mode=mode,variant=variant,waves=waves,command=command,pid=p.pid,started=time.time(),
                     files={p.name:sha(p) for p in d.iterdir() if p.suffix!='.log'})
            state['cases'].append(row);save();code=p.wait()
        text=(d/'proof.log').read_text();marker='no model found: SUCCESS!' if variant=='positive' else 'model found: FAIL!'
        passed=code==0 and marker in text
        row.update(passed=passed,returncode=code,finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(mode,variant,waves,'PASS' if passed else 'FAIL',flush=True)
        if not passed:
            state.update(status='TERMINAL_CONTROL_FALL_FUSION_NOT_ESTABLISHED',finished=time.time());save();return 1
    state.update(status='PASS_CERTIFIED_FALL_AND_PARALLEL_RISE_FULL_CONTROL',finished=time.time());save()
    print(state['status'],flush=True);return 0


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['stage','output','yosys']:
        p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--defer-frontend',action='store_true',help='Defer unused parameter elaboration; preserve every proof and top parameter')
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.output.resolve(),a.yosys.resolve(),a.defer_frontend))
