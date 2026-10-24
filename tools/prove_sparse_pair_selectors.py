#!/usr/bin/env python3
"""Bind sparse-first selectors to the actual current cofactor implementation.

For arbitrary binary owner/pins/prefix inputs with the real HIGH identity
certificate, the actual selector preview at a stable-pin HIGH->LOW fall must
return precisely the unchanged three owners. Prove at5 and24 waves. Preserve
all current ordered/certified-fall functions; the prior cofactor proof alone
used an older ordered wrapper. A wrong-selector control must fail. Unknown
inputs remain excluded by the packet's original full-input binary ready guard.
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

TOP='''module sparse_selector_equivalence #(parameter WAVES=5)(
 input wire [155:0] q,input wire [397:0] pins,input wire refresh,
 input wire [268:0] saved,input wire [10:0] saved_cp,saved_after,
 input wire [2:0] original_selector,output wire valid,equal,output wire [2:0] actual,expected);
 wire [155:0] identity_q;wire [55:0] identity_events,identity_repeated;wire identity_unsettled;
 dorado_control_hold_atomic_step #(.WAVES(WAVES)) identity(q,1'b1,1'b1,pins,pins,
  identity_q,identity_events,identity_repeated,identity_unsettled,refresh,);
 assign valid=(identity_q==q) && !(|identity_events) && !(|identity_repeated) && !identity_unsettled;
 dorado_hold_selector_preview #(.WAVES(WAVES)) selector(q,1'b1,1'b0,pins,pins,refresh,
  1'b1,saved,saved_cp,saved_after,original_selector,actual);
 assign expected={q[118],q[121],q[120]};
 assign equal=!valid || actual==expected;
endmodule
'''


def run(source,out,yosys):
    assert not out.exists();s=json.loads((source/'run-state.json').read_text())
    assert s['status']=='PASS_ENTRY_CACHED_FULL_PROCESSOR_PACKETS' and s['sparse_pair']
    for n,h in s['files'].items():assert sha(source/n)==h
    raw=(source/'candidate.sv').read_text()
    names=['dorado_hold_selector_preview','dorado_control_hold_ordered_step','dorado_control_hold_atomic_step',
           'dorado_control_terminal_body','dorado_control_certified_fall_step','dorado_control_fall_step']
    atoms='\n'.join(module(raw,n) for n in names)
    actual_stage=(source/'packet.sv').read_text()
    assert 'pair_fall ? {joint_q[118],joint_q[121],joint_q[120]}:ordinary_selector;' in actual_stage
    assert 'clock_before && clock_after && pins_before==pins_after' in actual_stage
    out.mkdir(parents=True);(out/'actual-source.sv').write_text(atoms)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),
        source_state_sha256=sha(source/'run-state.json'),cases=[],files={p.name:sha(p) for p in out.iterdir()},
        all_binary_inputs_unconstrained_except_actual_identity_certificate=True,actual_current_ordered_wrapper=True,
        native_integrated=False,hardware_changed=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for mode,waves in [('positive',5),('positive',24),('wrong-selector',5)]:
        d=out/f'{mode}-w{waves}';d.mkdir();top=TOP.replace('parameter WAVES=5',f'parameter WAVES={waves}',1)
        if mode=='wrong-selector':top=top.replace('{q[118],q[121],q[120]}','{q[118],q[121],!q[120]}',1)
        (d/'actual.sv').write_text(atoms);(d/'top.sv').write_text(top)
        flag='verify' if mode=='positive' else 'falsify'
        (d/'proof.ys').write_text(f'''read_verilog -sv -defer /work/actual.sv /work/top.sv
hierarchy -check -top sparse_selector_equivalence
proc
flatten
opt
check -assert
sat -timeout 120 -prove equal 1 -{flag} -show-ports -dump_json /work/counterexample.json
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        row=dict(mode=mode,waves=waves,command=cmd,files={p.name:sha(p) for p in d.iterdir()},started=time.time())
        with (d/'proof.log').open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT,env=env);row['pid']=child.pid;state['cases'].append(row);save();code=child.wait()
        text=(d/'proof.log').read_text();passed=code==0 and ('no model found: SUCCESS!' if mode=='positive' else 'model found: FAIL!') in text
        row.update(returncode=code,passed=passed,finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        if not passed:break
    passed=len(state['cases'])==3 and all(v['passed'] for v in state['cases'])
    state.update(status='PASS_CURRENT_SPARSE_FIRST_SELECTOR_PROJECTION' if passed else 'TERMINAL_SPARSE_SELECTOR_NOT_ESTABLISHED',finished=time.time());save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.yosys.resolve()))
