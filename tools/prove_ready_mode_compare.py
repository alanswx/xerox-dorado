#!/usr/bin/env python3
"""Prove the source Ready choice and full comparison for all binary inputs.

Retain all807 original four-wave state/event/flag/frame outputs. Check the
three-way reconstruction and compare fast/cached-prefix target selections to
the old comparator with arbitrary prefix payload and child capture inputs.
No clock/Ready/Hold input or protocol assumption is added.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

BODY='''module ready_mode_contract(input wire [155:0] state_q,input wire cb,ca,
 input wire [397:0] pb,pa,input wire refresh,output wire equivalent);
 wire [806:0] expected,actual;wire [1:0] choice;wire [15:0] data;
 dorado_capture_terminal_body #(.WAVES(4),.CP_TPC_BLOCK_EARLY(1)) original(
  state_q,cb,ca,pb,pa,expected[806:651],expected[650:595],expected[594:539],expected[538],refresh,
  expected[537:444],expected[443:440],expected[439:436],expected[435:432],expected[431:0]);
 dorado_ready_mode_body #(.WAVES(4),.CP_TPC_BLOCK_EARLY(1)) candidate(
  state_q,cb,ca,pb,pa,actual[806:651],actual[650:595],actual[594:539],actual[538],refresh,
  actual[537:444],actual[443:440],actual[439:436],actual[435:432],actual[431:0],choice,data);
 wire [15:0] rebuilt=choice==0 ? state_q[63:48]:choice==1 ? 16'b0:data;
 assign equivalent=(actual==expected) && (choice!=3) && (rebuilt==actual[714:699]);
endmodule
'''

COMPARE='''module ready_compare_contract(input wire [155:0] q,input wire cb,ca,
 input wire [397:0] pb,pa,input wire refresh,fast,enable,
 input wire [268:0] prefix,input wire [10:0] pre_cp,post_cp,
 input wire [15:0] current_ready,original_capture,input wire [27:0] data,input wire event_in,
 output wire equivalent,expected,actual);
 wire [155:0] selected_q=fast ? q:prefix[268:113];
 wire [397:0] selected_before=fast ? pb:{pb[397:293],pre_cp,pb[281:0]};
 wire [397:0] selected_after=fast ? pa:{pa[397:293],post_cp,pa[281:0]};
 wire [155:0] original_q;
 wire sparse_hold=fast && cb && !ca && ((pb & SPARSE_SUPPORT)==(pa & SPARSE_SUPPORT));
 dorado_capture_terminal_body #(.WAVES(4),.CP_TPC_BLOCK_EARLY(1)) root(
  .state_q(selected_q),.clock_before(cb),.clock_after(ca),.pins_before(selected_before),
  .pins_after(selected_after),.state_next(original_q),.refresh_before_run_capture(refresh));
 dorado_ready_compare_step original(current_ready,sparse_hold ? q[63:48]:original_q[63:48],original_capture,data,event_in,expected);
 dorado_ready_target_compare candidate(q,cb,ca,pb,pa,refresh,fast,prefix,pre_cp,post_cp,
  current_ready,original_capture,data,event_in,enable,expected,actual);
 assign equivalent=actual==expected;
endmodule
'''


def run(source,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    r=json.loads((source/'results.json').read_text());verify(source,r['files'])
    assert r['status']=='PREPARED_SOURCE_BOUND_READY_MODE_COMPARISON'
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),source_receipt_sha256=sha(source/'results.json'),
               source_control_sha256=r['source_control_sha256'],assumptions=0,original_body_outputs_compared=807,
               source_atomic_waves=4,additional_guest_or_host_cycles=0,actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,top,mutation in [('body-and-choice',BODY,None),('comparison',COMPARE,None),
        ('wrong-clear-choice',BODY,("if(q[73])ready_mode=2'd1","if(q[73])ready_mode=2'd2")),
        ('wrong-load-data',BODY,("22'b1<<18,ca_pins(state_q,pins_after)","22'b0,ca_pins(state_q,pins_after)"))]:
        d=out/label;d.mkdir()
        for path in source.glob('*.sv'):shutil.copy2(path,d/path.name)
        if mutation:
            raw=(d/'mode-body.sv').read_text();assert raw.count(mutation[0])==1
            (d/'mode-body.sv').write_text(raw.replace(*mutation,1))
        (d/'top.sv').write_text(top.replace('SPARSE_SUPPORT',r['fall_stable_pin_support']));name='ready_compare_contract'if top==COMPARE else'ready_mode_contract'
        (d/'proof.ys').write_text(f'''read_verilog -sv -defer {' '.join('/work/'+p.name for p in sorted(d.glob('*.sv')))}
hierarchy -check -top {name}
proc
flatten
opt
check -assert
sat -timeout 120 -prove equivalent 1 -{'falsify'if mutation else'verify'} -show-ports -dump_json /work/counterexample.json
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with(d/'proof.log').open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log,env=env);a=dict(label=label,pid=child.pid,command=cmd,started=time.time(),negative=bool(mutation),
                files={p.name:sha(p)for p in d.iterdir()if p.name!='proof.log'});state['actions'].append(a);save();code=child.wait()
        marker='model found: FAIL!'if mutation else'no model found: SUCCESS!'
        a.update(returncode=code,passed=code==0 and marker in(d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(label,a['passed'],flush=True)
        if not a['passed']:break
    passed=len(state['actions'])==4 and all(a['passed']for a in state['actions'])
    state.update(status='PASS_SOURCE_BOUND_READY_MODE_AND_COMPARISON'if passed else'TERMINAL_READY_MODE_NOT_ESTABLISHED',finished=time.time(),
                 native_equivalence_or_physical_timing_or_speed_qualified=False)
    save();return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.yosys.resolve()))
