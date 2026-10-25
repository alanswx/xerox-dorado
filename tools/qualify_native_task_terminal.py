#!/usr/bin/env python3
"""Shorten the actual shared task consumer and prove its terminal closure.

Compare all139 physical state/event/repeat/unsettled bits for678 arbitrary
binary inputs, including all four unconstrained capture frames. No source
arrival, firmware or physical timing is assumed qualified. Unknown contexts
retain the original evaluator and unsupported wave parameters are unchanged.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

TASK='src/verilog/rtl/dorado_local_task_capture_step.sv'
NAME='dorado_local_task_capture_atomic_step'
ORIGINAL='dorado_local_task_capture_atomic_original_step'
TOP='''module native_task_terminal_contract(
 input wire stop_tasks,input wire [1:0] canonical,input wire [87:0] q,
 input wire cb,ca,input wire [75:0] pb,pa,input wire [432:0] frames,
 output wire [138:0] actual,expected,output wire equivalent,terminal_closed);
 dorado_local_task_capture_atomic_step original(stop_tasks,canonical,q,cb,ca,pb,pa,
  expected[138:51],expected[50:26],expected[25:1],expected[0],frames);
 native_task_terminal changed(stop_tasks,canonical,q,cb,ca,pb,pa,
  actual[138:51],actual[50:26],actual[25:1],actual[0],frames);
 assign equivalent=actual==expected;
 assign terminal_closed=!frames[432] || !expected[0];
endmodule
'''


def transform(raw,waves=7):
    old=module(raw,NAME)
    token='for(integer wave=0;wave<WAVES;wave++)begin'
    assert old.count(token)==1
    candidate=old.replace(token,f'for(integer wave=0;wave<((WAVES==16) ? {waves}:WAVES);wave++)begin',1)
    token='{q,events,repeated,!settled};'
    assert candidate.count(token)==1
    candidate=candidate.replace(token,"{q,events,repeated,((WAVES==16) ? 1'b0:!settled)};",1)
    return raw.replace(old,candidate,1)


def run(stage,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    manifest=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,manifest['source_hashes'])
    raw=(stage/TASK).read_text();after=transform(raw)
    (out/'native-task-before.sv').write_text(raw);(out/'native-task-after.sv').write_text(after)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),stage=str(stage),
        stage_manifest_sha256=sha(stage/'r-front-control-native.json'),source_task_sha256=sha(stage/TASK),
        arbitrary_binary_input_bits=678,compared_outputs=139,assumptions=0,
        original_waves=16,shared_consumer_waves=7,terminal_unsettled=0,
        original_fallback_and_unsupported_parameters_retained=True,
        files={p.name:sha(p)for p in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,waves,signal,positive in [('seven-wave-complete-result',7,'equivalent',True),
                                      ('original-terminal-closure',7,'terminal_closed',True),
                                      ('four-wave-counterexample',4,'equivalent',False)]:
        d=out/label;d.mkdir()
        (d/'original.sv').write_text(module(raw,ORIGINAL)+'\n'+module(raw,NAME))
        (d/'actual.sv').write_text(module(transform(raw,waves),NAME).replace('module '+NAME+' #','module native_task_terminal #',1))
        (d/'top.sv').write_text(TOP)
        script=f'''read_verilog -sv -defer /work/original.sv /work/actual.sv /work/top.sv
hierarchy -check -top native_task_terminal_contract
proc
flatten
opt
check -assert
sat -timeout 120 -prove {signal} 1 -{'verify'if positive else'falsify'} -show-ports -dump_json /work/counterexample.json
'''
        (d/'proof.ys').write_text(script)
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        command=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with(d/'proof.log').open('x')as log:
            child=subprocess.Popen(command,stdout=log,stderr=log,env=env)
            row=dict(label=label,waves=waves,signal=signal,positive=positive,pid=child.pid,command=command,
                     started=time.time(),files={p.name:sha(p)for p in d.iterdir()if p.name!='proof.log'})
            state['actions'].append(row);save();code=child.wait()
        marker='no model found: SUCCESS!'if positive else'model found: FAIL!'
        row.update(returncode=code,passed=code==0 and marker in(d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(label,row['passed'],flush=True)
        if not row['passed']:break
    passed=len(state['actions'])==3 and all(r['passed']for r in state['actions'])
    state.update(status='PASS_ACTUAL_NATIVE_TASK_TERMINAL_CLOSURE'if passed else'TERMINAL_NATIVE_TASK_CLOSURE_NOT_ESTABLISHED',
        finished=time.time(),native_selected=False,physical_timing_qualified=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.output.resolve(),a.yosys.resolve()))
