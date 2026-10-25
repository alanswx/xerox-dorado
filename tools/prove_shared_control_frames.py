#!/usr/bin/env python3
"""Prove passive control frames preserve results and their binary chronology.

Compare all363 original outputs at5/24 waves, without assumptions on955 input
bits. Check each frame's Q linkage, physical event projection, repeated events
and final actual owner. This does not prove the separate processor consumer's
arrival epochs or any native timing/boot property.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from generate_shared_control_capture import control
from prove_control_fall_fusion import sources
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_r_front_forward_view import sha

TOP='''module equivalence(input wire [155:0] q,input wire before_root,after_root,
 input wire [397:0] before_pins,after_pins,input wire refresh,
 output wire [362:0] actual,expected,output wire equivalent,frames_consistent);
 wire [431:0] frames;
 dorado_control_hold_atomic_step #(.WAVES(WAVES_VALUE)) original(q,before_root,after_root,before_pins,after_pins,
  expected[362:207],expected[206:151],expected[150:95],expected[94],refresh,expected[93:0]);
 dorado_capture_hold_atomic_step #(.WAVES(WAVES_VALUE)) candidate(q,before_root,after_root,before_pins,after_pins,
  actual[362:207],actual[206:151],actual[150:95],actual[94],refresh,actual[93:0],frames);
 reg [11:0] events,repeated,now;reg consistent;reg [47:0] prior;
 always @*begin
  events=0;repeated=0;consistent=1;prior=q[47:0];now=0;
  for(integer i=0;i<4;i++)begin
   consistent &= frames[108*i+12+:48]==prior;
   now=frames[108*i+:12];repeated|=now&events;events|=now;
   prior=frames[108*i+60+:48];
  end
  consistent &= prior==actual[207+:48];
  consistent &= events=={actual[151+12],actual[151+:11]};
  consistent &= repeated=={actual[95+12],actual[95+:11]};
 end
 assign frames_consistent=consistent;
 assign equivalent=actual==expected && frames_consistent;
endmodule
'''


def run(source,out,yosys):
    assert not out.exists()
    s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
    assert s['status']=='PASS_OWNED_FULL_PROCESSOR_PACKETS' and s['mutation'] is None
    raw=(source/'candidate.sv').read_text();original,_=sources(raw);shared=control(raw)
    actual=module(shared,'dorado_capture_hold_atomic_step')+'\n'+module(shared,'dorado_capture_terminal_body')
    out.mkdir(parents=True)
    (out/'original.sv').write_text(original);(out/'actual.sv').write_text(actual)
    for n in [Path(__file__).name,'generate_shared_control_capture.py']:shutil.copy2(Path(__file__).with_name(n),out/n)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),
               source_state_sha256=sha(source/'run-state.json'),arbitrary_binary_input_bits=955,compared_outputs=363,
               frame_count=4,frame_bits=108,no_assumptions=True,native_integrated=False,
               files={p.name:sha(p) for p in out.iterdir()},cases=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for waves in [5,24]:
        d=out/f'w{waves}';d.mkdir();(d/'top.sv').write_text(TOP.replace('WAVES_VALUE',str(waves)))
        (d/'proof.ys').write_text(f'''read_verilog -sv -defer /work/original.sv /work/actual.sv /work/w{waves}/top.sv
hierarchy -check -top equivalence
proc
flatten
memory_map
opt
check -assert
sat -timeout 120 -prove equivalent 1 -verify -show-ports -dump_json /work/w{waves}/counterexample.json
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
        cmd=[str(yosys),'-Q','-T','-s',f'/work/w{waves}/proof.ys']
        row=dict(waves=waves,command=cmd,started=time.time(),files={p.name:sha(p) for p in d.iterdir()})
        with (d/'proof.log').open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT,env=env);row['pid']=child.pid;state['cases'].append(row);save();code=child.wait()
        row.update(returncode=code,passed=code==0 and 'no model found: SUCCESS!' in (d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(waves,'PASS' if row['passed'] else 'FAIL',flush=True)
        if not row['passed']:break
    passed=len(state['cases'])==2 and all(r['passed'] for r in state['cases'])
    state.update(status='PASS_SHARED_CONTROL_FRAME_RESULTS_AND_CHRONOLOGY' if passed else 'TERMINAL_CONTROL_FRAMES_NOT_ESTABLISHED',finished=time.time());save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.yosys.resolve()))
