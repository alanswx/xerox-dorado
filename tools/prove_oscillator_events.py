#!/usr/bin/env python3
"""Prove the actual50MHz/four-tick scheduler against an ideal5ns clock grid.

The independent reference advances THREE5ns steps per15ns fabric interval;
its50MHz level changes every TWO steps. Compare every ordered-event output
and strengthen induction with the exact reference-phase/remainder relation.
Reset and advance histories are arbitrary; actual power-up state is retained.
This proves this producer, not native display-consumer capture or clock timing.
"""
import argparse
import json
import os
from pathlib import Path
import subprocess
import time

from test_oscillator_events import sha

TOP='''module oscillator_equivalence(input wire clk,reset,advance,output wire equivalent);
 wire [1:0] edges;wire rise,fall,first,endpoint,before_level,after_level;
 wire [31:0] remainder;
 wire [31:0] first_offset,second_offset,interval_units;
 dorado_oscillator_events actual(clk,reset,advance,edges,rise,fall,first,endpoint,before_level,after_level,
  first_offset,second_offset,interval_units,remainder);
 // A50MHz crystal has a10ns half-period. Four5ns slots make its full period.
 reg [1:0] phase=0;
 reg [1:0] cursor,count;
 reg old_level,expected_rise,expected_fall,expected_first,expected_endpoint;
 reg [31:0] expected_first_offset,expected_second_offset;
 always @*begin
  cursor=phase;count=0;expected_rise=0;expected_fall=0;expected_first=0;expected_endpoint=0;
  expected_first_offset=0;expected_second_offset=0;
  if(advance && !reset)for(integer step=0;step<3;step++)begin
   old_level=cursor[1];cursor=cursor+2'd1;
   if(cursor[1]!=old_level)begin
    if(count==0)begin expected_first=cursor[1];expected_first_offset=32'(step+1)*2000000;end
    else expected_second_offset=32'(step+1)*2000000;
    count=count+2'd1;
    if(cursor[1])expected_rise=1;else expected_fall=1;
    if(step==2)expected_endpoint=1;
   end
  end
 end
 always @(posedge clk)begin
  if(reset)phase<=0;else if(advance)phase<=phase+2'd3;
 end
 assign equivalent=(before_level==phase[1]) &&
  (remainder==(phase[0]?32'd2000000:32'd0)) &&
  ({first_offset,second_offset,interval_units}=={expected_first_offset,expected_second_offset,32'd6000000}) &&
  ({edges,rise,fall,first,endpoint,after_level}==
   {count,expected_rise,expected_fall,expected_first,expected_endpoint,cursor[1]});
endmodule
'''


def run(tests,out,yosys,variant):
    assert not out.exists()
    t=json.loads((tests/'run-state.json').read_text())
    assert t['status']=='PASS_ORDERED_OSCILLATOR_EVENTS_AND_NATIVE_RATE_COUNTEREXAMPLE'
    for name,digest in t['inputs'].items():assert sha(tests/'inputs'/name)==digest,name
    original=(tests/'inputs'/(variant+'.sv')).read_text()
    old='    output wire [31:0] first_offset_units,second_offset_units,interval_units\n'
    new='    output wire [31:0] first_offset_units,second_offset_units,interval_units,observed_remainder\n'
    added='    assign observed_remainder=remainder;\n'
    assert original.count(old)==1
    exposed=original.replace(old,new,1).replace('    wire active=advance',added+'    wire active=advance',1)
    assert exposed.replace(new,old,1).replace(added,'',1)==original
    out.mkdir(parents=True)
    (out/'actual.sv').write_text(exposed);(out/'original.sv').write_text(original);(out/'reference.sv').write_text(TOP)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    positive=variant=='positive'
    strategy='-seq 2 -tempinduct -maxsteps 4'if positive else'-seq 4'
    script=f'''read_verilog -sv /work/actual.sv /work/reference.sv
hierarchy -check -top oscillator_equivalence
proc
flatten
opt
check -assert
sat {strategy} -timeout 120 -prove equivalent 1 -{'verify'if positive else'falsify'} -show-ports -dump_json /work/counterexample.json
'''
    (out/'proof.ys').write_text(script)
    state=dict(status='RUNNING',scope=__doc__,driver_pid=os.getpid(),started=time.time(),variant=variant,
               tests_state_sha256=sha(tests/'run-state.json'),actual_source_sha256=sha(tests/'inputs'/(variant+'.sv')),
               comparison='All event/level outputs and actual remainder against independent5ns reference',
               clock_interval_ns=15,crystal_half_period_ns=10,samples_per_instruction=4,
               assumptions_added=False,power_up_state='Actual explicit zero initializers',
               unbounded_induction_proven=False,native_consumer_qualified=False,physical_timing_qualified=False,
               files={p.name:sha(p)for p in out.iterdir()if p.is_file()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
    save()
    with(out/'proof.log').open('x')as log:
        p=subprocess.Popen([str(yosys),'-Q','-T','-s','/work/proof.ys'],env=env,stdout=log,stderr=subprocess.STDOUT)
        state['pid']=p.pid;save();code=p.wait()
    text=(out/'proof.log').read_text()
    expected='Induction step proven: SUCCESS!'if positive else'SAT proof finished - model found: FAIL!'
    passed=code==0 and expected in text
    state.update(status=('PASS_ORDERED_50MHZ_EVENTS_INDUCTION'if positive else'PASS_OSCILLATOR_EVENT_COUNTEREXAMPLE')
                 if passed else'TERMINAL_OSCILLATOR_EVENT_PROOF_NOT_ESTABLISHED',returncode=code,
                 unbounded_induction_proven=passed and positive,finished=time.time(),proof_log_sha256=sha(out/'proof.log'))
    save();print(state['status'],flush=True)
    return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['tests','output','yosys']:p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--variant',choices=['positive','drop-second','reverse-order','advance-while-held','late-offset'],default='positive')
    a=p.parse_args();raise SystemExit(run(a.tests.resolve(),a.output.resolve(),a.yosys.resolve(),a.variant))
