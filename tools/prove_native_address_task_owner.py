#!/usr/bin/env python3
"""Check the two actual task evaluators inside the native R-address consumer.

Compare all139 Q/event/repeat/settle outputs for2114 arbitrary binary inputs,
including every shared capture-frame bit. This diagnoses whether sharing the
real task result is sound; it changes no native owner, guard or FPGA input.
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

NAMES=['dorado_local_r_address_step','dorado_local_r_flags_step','dorado_local_r_operand_step',
       'dorado_local_t_parity_step','dorado_local_t_file_step','dorado_local_task_capture_step']
TOP='''module address_task_owner_contract(
 input wire stop_tasks,input wire [3:0] canonical,input wire [635:0] q,
 input wire cb,ca,input wire [201:0] pb,pa,capture_pins,input wire [864:0] frames,
 output wire [138:0] actual,expected,output wire equivalent);
 dorado_local_r_address_step dut(.stop_tasks_capture(stop_tasks),.canonical_task_clock(canonical),
  .state_q(q),.clock_before(cb),.clock_after(ca),.pins_before(pb),.pins_after(pa),
  .capture_pins(capture_pins),.shared_capture(frames),.task_actual(actual),.task_expected(expected));
 assign equivalent=actual==expected;
endmodule
'''


def run(stage,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    sources={}
    for n in NAMES:
        name='src/verilog/rtl/'+n+'.sv';sources[name]=sha(stage/name)
        text=(stage/name).read_text()
        if n==NAMES[0]:
            token='input wire [201:0] capture_pins,input wire [864:0] shared_capture);'
            assert text.count(token)==1
            text=text.replace(token,'input wire [201:0] capture_pins,input wire [864:0] shared_capture,\n output wire [138:0] task_actual,task_expected);',1)
            marker=' assign unsettled=core_unsettled|child_unsettled|(child_next[87:0]!=core_next);'
            assert text.count(marker)==1
            extra=''' assign task_actual={child_next[87:0],clock_events[24:0],repeated_events[24:0],child_unsettled};
 assign task_expected={core_next,core_events,core_repeated,core_unsettled};
'''
            text=text.replace(marker,extra+marker,1)
        (out/(n+'.sv')).write_text(text)
    (out/'top.sv').write_text(TOP);shutil.copy2(Path(__file__),out/Path(__file__).name)
    script='''read_verilog -sv -defer SOURCES /work/top.sv
hierarchy -check -top address_task_owner_contract
proc
flatten
opt
check -assert
sat -timeout 120 -prove equivalent 1 -show-ports -dump_json /work/counterexample.json
'''.replace('SOURCES',' '.join('/work/'+n+'.sv'for n in NAMES))
    (out/'proof.ys').write_text(script)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),stage=str(stage),
        stage_manifest_sha256=sha(stage/'r-front-control-native.json'),source_hashes=sources,
        arbitrary_binary_input_bits=2114,compared_outputs=139,assumptions=0,
        files={p.name:sha(p)for p in out.iterdir()},native_changed=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
    command=[str(yosys),'-Q','-T','-s','/work/proof.ys']
    with(out/'proof.log').open('x')as log:
        child=subprocess.Popen(command,stdout=log,stderr=log,env=env);state.update(pid=child.pid,command=command);save();code=child.wait()
    log=(out/'proof.log').read_text();proved=code==0 and'no model found: SUCCESS!'in log
    counterexample=code==0 and'model found: FAIL!'in log
    state.update(status='PROVED_ACTUAL_NATIVE_ADDRESS_TASK_OWNER_EQUIVALENCE'if proved else
                 'COUNTEREXAMPLE_TO_NATIVE_ADDRESS_TASK_OWNER_EQUIVALENCE'if counterexample else'TERMINAL_NATIVE_ADDRESS_TASK_OWNER_NOT_ESTABLISHED',
        returncode=code,proved=proved,counterexample=counterexample,log_sha256=sha(out/'proof.log'),finished=time.time())
    save();print(state['status'],flush=True);return 0 if proved or counterexample else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.output.resolve(),a.yosys.resolve()))
