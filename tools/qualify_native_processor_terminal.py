#!/usr/bin/env python3
"""Qualify the native front's separate64-Q processor phase evaluator.

Compare ALL109 outputs for122 arbitrary binary state/input bits, without
assumptions. A five-wave data transfer uses the proved terminal closure;
unknown contexts and nondefault wave parameters retain the original evaluator.
This is a source qualification, not native arrival or FPGA timing selection.
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

SOURCE='src/verilog/rtl/dorado_processor_capture_step.sv'
NAME='dorado_processor_capture_atomic_step'
BODY='dorado_processor_capture_original_atomic_step'
TOP='''module processor_terminal_contract(input wire [63:0] q,input wire cb,ca,
 input wire [27:0] pb,pa,output wire [108:0] actual,expected,
 output wire equivalent,original_terminal_closed);
 reference_processor_capture original(q,cb,ca,pb,pa,
  expected[108:45],expected[44:23],expected[22:1],expected[0]);
 dorado_processor_capture_atomic_step candidate(q,cb,ca,pb,pa,
  actual[108:45],actual[44:23],actual[22:1],actual[0]);
 assign equivalent=actual==expected;
 assign original_terminal_closed=!expected[0];
endmodule
'''


def transform(raw,waves=5):
    old=module(raw,NAME);header=old[:old.index('// Q0..31')]
    assert header.rstrip().endswith(');')
    body=old.replace('module '+NAME+' #','module '+BODY+' #',1)
    wrapper=header+f'''
 wire [108:0] original_result,short_result;
 {BODY} #(.WAVES(WAVES)) original(
  state_q,clock_before,clock_after,pins_before,pins_after,
  original_result[108:45],original_result[44:23],original_result[22:1],original_result[0]);
 {BODY} #(.WAVES({waves})) shortened(
  state_q,clock_before,clock_after,pins_before,pins_after,
  short_result[108:45],short_result[44:23],short_result[22:1],short_result[0]);
 always @*begin
  {{state_next,clock_events,repeated_events,unsettled}}=original_result;
  if(WAVES==16)case(^{{state_q,clock_before,clock_after,pins_before,pins_after}})
   1'b0,1'b1:{{state_next,clock_events,repeated_events,unsettled}}={{short_result[108:1],1'b0}};
   default:begin end
  endcase
 end
endmodule
'''
    return raw.replace(old,body+'\n'+wrapper,1)


def run(stage,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    raw=(stage/SOURCE).read_text();after=transform(raw)
    (out/'native-processor-before.sv').write_text(raw);(out/'native-processor-after.sv').write_text(after)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),stage=str(stage),
        source_sha256=sha(stage/SOURCE),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        arbitrary_binary_input_bits=122,compared_outputs=109,assumptions=0,
        old_waves=16,candidate_waves=5,unknown_fallback_retained=True,nondefault_parameters_unchanged=True,
        files={p.name:sha(p)for p in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,waves,signal,positive in [('five-wave-complete-result',5,'equivalent',True),
                                      ('original-terminal-closure',5,'original_terminal_closed',True),
                                      ('one-wave-counterexample',1,'equivalent',False)]:
        d=out/label;d.mkdir()
        (d/'original.sv').write_text(module(raw,NAME).replace('module '+NAME+' #','module reference_processor_capture #',1))
        (d/'actual.sv').write_text(transform(raw,waves));(d/'top.sv').write_text(TOP)
        script=f'''read_verilog -sv -defer /work/original.sv /work/actual.sv /work/top.sv
hierarchy -check -top processor_terminal_contract
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
    state.update(status='PASS_ACTUAL_NATIVE_PROCESSOR_TERMINAL_CLOSURE'if passed else'TERMINAL_NATIVE_PROCESSOR_CLOSURE_NOT_ESTABLISHED',
        finished=time.time(),native_selected=False,physical_timing_qualified=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.output.resolve(),a.yosys.resolve()))
