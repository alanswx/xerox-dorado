#!/usr/bin/env python3
"""Check whether actual early flag data depends on the raw source-root level.

The pinned program did not reject rebinding this level. Compare the16 early
data bits actually consumed by the flag transfer for ALL binary physical Q,
producer inputs and root levels, without assumptions. Preserve a counterexample
if independence is false; neither outcome establishes physical arrival timing.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from review_processor_branch_composition import checked
from test_external_r_packet import sha

TOP='''module root_contract(
 input wire [1197:0] q,input wire [431:0] pins,input wire root,flag_root,
 output wire [15:0] first,second,output wire equal);
 wire [37:0] p0,p1;wire [23:0] d0,d1;
 alu_projection_processor_alu_reference a0(.fixture_load(1'b0),.fixture_state(q),.base(root),.pins(pins),.branch_pins(p0));
 alu_projection_processor_alu_reference a1(.fixture_load(1'b0),.fixture_state(q),.base(~root),.pins(pins),.branch_pins(p1));
 branch_condition_cone b0(.fixture_load(1'b0),.fixture_state(q[923:844]),.base(flag_root),.start_a(1'b0),.en(1'b0),.pins(p0),.capture_data(d0));
 branch_condition_cone b1(.fixture_load(1'b0),.fixture_state(q[923:844]),.base(flag_root),.start_a(1'b0),.en(1'b0),.pins(p1),.capture_data(d1));
 assign first=d0[15:0];assign second=d1[15:0];assign equal=first==second;
endmodule
'''


def run(source,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    s=checked(source);assert s['status']=='PASS_ALU_PROJECTION_NATIVE_LOWERING'and not s.get('mutation')
    files={'projection.sv':source/'lowered/projection.sv',
           'branch.sv':source/'replay/reference/base/branch/cone.sv'}
    for n,p in files.items():shutil.copy2(p,out/n)
    (out/'contract.sv').write_text(TOP)
    script='''read_verilog -formal -sv /work/projection.sv /work/branch.sv /work/contract.sv
hierarchy -check -top root_contract
proc
flatten
opt
check -assert
sat -timeout 120 -prove equal 1 -show-ports -dump_json /work/counterexample.json
'''
    (out/'check.ys').write_text(script);shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,source=str(source),started=time.time(),
        input_bits=1632,compared_data_bits=16,assumptions=0,
        source_state_sha256=sha(source/'run-state.json'),
        files={p.name:sha(p)for p in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
    cmd=[str(yosys),'-Q','-T','-s','/work/check.ys']
    with(out/'check.log').open('x')as log:
        child=subprocess.Popen(cmd,stdout=log,stderr=log,env=env)
        action=dict(label='check',command=cmd,pid=child.pid,started=time.time());state['actions'].append(action);save();code=child.wait()
    action.update(returncode=code,finished=time.time(),log_sha256=sha(out/'check.log'))
    text=(out/'check.log').read_text()
    if code==0 and 'no model found: SUCCESS!'in text:status='PROVED_EARLY_FLAG_DATA_SOURCE_ROOT_INDEPENDENT'
    elif code==0 and 'model found: FAIL!'in text:status='EARLY_FLAG_DATA_ROOT_DEPENDENCY_COUNTEREXAMPLE'
    else:status='TERMINAL_EARLY_FLAG_ROOT_DEPENDENCY_NOT_ESTABLISHED'
    state.update(status=status,finished=time.time(),physical_arrival_timing_qualified=False,
                 files={p.name:sha(p)for p in out.iterdir()if p.name!='run-state.json'})
    save();print(status);return 0 if code==0 and status!='TERMINAL_EARLY_FLAG_ROOT_DEPENDENCY_NOT_ESTABLISHED'else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['source','output','yosys']:p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.yosys.resolve()))
