#!/usr/bin/env python3
"""Derive and prove the entire actual control pin support, not one late bit.

Trace ALL363 outputs of the exact known-binary branch, then independently prove
arbitrary ignored-pin substitutions against the UNCHANGED whole atomic wrapper.
No q/root/refresh bit is projected. Both before/after pin cones contribute to
the guard support. Four-state fallback is outside this binary audit and must
remain unchanged. This audit does not modify a processor or select a native fit.
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

PORTS='''input wire [155:0] q,input wire before_root,after_root,
 input wire [397:0] before_pins,after_pins,input wire refresh'''
CALL='''q,before_root,after_root,before_pins,after_pins,
  result[362:207],result[206:151],result[150:95],result[94],refresh,result[93:0]'''
CONE='''module control_binary_cone(PORTS,output wire [362:0] result);
 wire [155:0] n;wire [55:0] events,repeated;wire [93:0] flags;
 // This tuple is the EXISTING wrapper's exact known-binary result, including
 // its explicit unsettled=0. The independent SAT miter uses the whole wrapper.
 dorado_control_terminal_body #(.WAVES(4)) actual(q,before_root,after_root,before_pins,after_pins,
  n,events,repeated,,refresh,flags,,,);
 assign result={n,events,repeated,1'b0,flags};
endmodule
'''.replace('PORTS',PORTS)
SOURCE='''module whole_control #(parameter integer WAVES=WAVES_VALUE)(PORTS,output wire [362:0] result);
 dorado_control_hold_atomic_step #(.WAVES(WAVES)) actual(CALL);
endmodule
'''.replace('PORTS',PORTS).replace('CALL',CALL)
MITER='''module control_support_equivalence(PORTS,
 input wire [397:0] alternate_before,alternate_after,output wire equal,output wire [362:0] different);
 localparam [397:0] CARE=398'hMASK;
 wire [397:0] projected_before=(before_pins & CARE)|(alternate_before & ~CARE);
 wire [397:0] projected_after=(after_pins & CARE)|(alternate_after & ~CARE);
 wire [362:0] actual,projected;
 whole_control original(q,before_root,after_root,before_pins,after_pins,refresh,actual);
 whole_control changed(q,before_root,after_root,projected_before,projected_after,refresh,projected);
 assign different=actual^projected;assign equal=~|different;
endmodule
'''.replace('PORTS',PORTS)


def support(net):
    drivers={}
    for cell in net['cells'].values():
        c=cell['connections'];kind=cell['type']
        ins={b for p,bits in c.items() if cell['port_directions'][p]=='input' for b in bits if isinstance(b,int)}
        for p,bits in c.items():
            if cell['port_directions'][p]!='output':continue
            for i,b in enumerate(bits):
                if not isinstance(b,int):continue
                deps=ins
                if p=='Y' and kind=='$mux' and len(c['A'])==len(c['B'])==len(bits):deps={c['A'][i],c['B'][i],*c['S']}
                elif p=='Y' and kind in ['$and','$or','$xor','$xnor'] and len(c['A'])==len(c['B'])==len(bits):deps={c['A'][i],c['B'][i]}
                elif p=='Y' and kind=='$not' and len(c['A'])==len(bits):deps={c['A'][i]}
                drivers.setdefault(b,set()).update(v for v in deps if isinstance(v,int))
    reached=set();pending=list(net['ports']['result']['bits'])
    while pending:
        b=pending.pop()
        if not isinstance(b,int) or b in reached:continue
        reached.add(b);pending.extend(drivers.get(b,()))
    return {name:[i for i,b in enumerate(net['ports'][name]['bits']) if b in reached] for name in ['before_pins','after_pins']}


def run(source,out,yosys):
    assert not out.exists();s=json.loads((source/'run-state.json').read_text())
    assert s['status']=='PASS_OWNED_FULL_PROCESSOR_PACKETS' and s['mutation'] is None
    for n,h in s['files'].items():assert sha(source/n)==h
    raw=(source/'candidate.sv').read_text();atomic=module(raw,'dorado_control_hold_atomic_step');terminal=module(raw,'dorado_control_terminal_body')
    assert 'generate if(WAVES==5 || WAVES==24)begin: shortened' in atomic
    assert 'dorado_control_terminal_body #(.WAVES(4)) short_transfer(' in atomic
    assert "1'b0,1'b1: {state_next,clock_events,repeated_events,unsettled,flags}=\n      {short_q,short_events,short_repeated,1'b0,short_flags};" in atomic
    out.mkdir(parents=True);(out/'atomic.sv').write_text(atomic+terminal);(out/'cone-top.sv').write_text(CONE)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),source_state_sha256=sha(source/'run-state.json'),
        complete_result_bits=363,reference_whole_wrapper_unchanged=True,no_q_root_or_refresh_bit_projected=True,
        cone_uses_exact_existing_binary_branch=True,four_state_guard_change_permitted=False,
        files={p.name:sha(p) for p in out.iterdir()},actions=[],hardware_changed=False,native_integrated=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    def action(label,script):
        (out/(label+'.ys')).write_text(script)
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
        cmd=[str(yosys),'-Q','-T','-s','/work/'+label+'.ys'];r=dict(label=label,command=cmd,started=time.time(),script_sha256=sha(out/(label+'.ys')))
        with (out/(label+'.log')).open('x') as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT,env=env);r['pid']=p.pid;state['actions'].append(r);save();code=p.wait()
        r.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        assert code==0,label
        return (out/(label+'.log')).read_text()
    save()
    try:
        action('cone','''read_verilog -sv -defer /work/atomic.sv /work/cone-top.sv
hierarchy -check -top control_binary_cone
proc
flatten
opt
wreduce
opt
check -assert
write_json /work/cone.json
''')
        cone=json.loads((out/'cone.json').read_text())['modules']['control_binary_cone'];ports=support(cone)
        care=sorted(set(ports['before_pins'])|set(ports['after_pins']));mask=sum(1<<b for b in care)
        assert care and len(care)<398 and 292 in care
        state.update(port_support=ports,guard_pin_support=care,unused_guard_pins=sorted(set(range(398))-set(care)),guard_pin_count=len(care),critical_before119_in_support=119 in care,
            support_mask_hex=format(mask,'x'),cone_json_sha256=sha(out/'cone.json'));save()
        for label,waves,value in [('positive-w5',5,mask),('positive-w24',24,mask),('missing-CP292-w5',5,mask & ~(1<<292))]:
            (out/(label+'-source.sv')).write_text(SOURCE.replace('WAVES_VALUE',str(waves)))
            (out/(label+'-top.sv')).write_text(MITER.replace('MASK',format(value,'x')))
            positive=label.startswith('positive');flag='verify' if positive else 'falsify'
            log=action(label,f'''read_verilog -sv -defer /work/atomic.sv /work/{label}-source.sv /work/{label}-top.sv
hierarchy -check -top control_support_equivalence
proc
flatten
opt
check -assert
sat -timeout 120 -prove equal 1 -{flag} -show-ports -dump_json /work/{label}-counterexample.json
''')
            assert ('no model found: SUCCESS!' if positive else 'model found: FAIL!') in log
        state.update(status='PASS_WHOLE_CONTROL_PIN_SUPPORT',finished=time.time(),arbitrary_binary_miter_input_bits=1751,
            ignored_pin_changes_proven_for_all363_results=True,required_CP_clock_bit_counterexample=True)
    except BaseException:
        state.update(status='TERMINAL_CONTROL_PIN_SUPPORT_NOT_ESTABLISHED',finished=time.time());save();raise
    save();print(state['status'],len(care),'pins; before119 cared =',119 in care,flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();run(a.source.resolve(),a.output.resolve(),a.yosys.resolve())
