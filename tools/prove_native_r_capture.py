#!/usr/bin/env python3
"""Prove two original mux/latch capture epochs compose into the native16 bits.

Use the exact four native MC10173 cells, with uninitialized Qs and arbitrary
frames/commit. This proves the capture interface/primitive composition, not
unbounded full-processor or original analog clock equivalence. Source-bind the
frame to the actual corrected operand input, without using predicted R outputs.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from generate_native_r_capture import generate,FOLD
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
TOP='''module contract(input wire clk,commit,use_pair,
 input wire [35:0] single_frame,first_frame,last_frame,
 output wire [15:0] visible,output reg [15:0] expected,
 output wire native_match);
 FOLD
 wire [35:0] folded=use_pair ? native_r_fold(first_frame,last_frame):single_frame;
 dorado_native_r_capture cells(clk,commit,folded,visible);
 function automatic [15:0] original_transfer(input [15:0] q,input [35:0] f);
  reg [15:0] n;
  begin
   n=q;
   for(integer half=0;half<2;half++)
    if(!f[34+half])n[8*half+:8]=f[32+half] ? f[16+8*half+:8]:f[8*half+:8];
   original_transfer=n;
  end
 endfunction
 reg past_valid=0;
 always @(posedge clk)begin
  past_valid<=1;
  expected<=visible;
  if(commit)expected<=use_pair ? original_transfer(original_transfer(visible,first_frame),last_frame):
                               original_transfer(visible,single_frame);
 end
 assign native_match=!past_valid || visible==expected;
endmodule
'''


def run(out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    bridge,cell,manifest=generate(ROOT)
    # Exact equations from the actual corrected operand used by both packet
    # variants. Its only visible-R assignments have this clock/mux contract.
    source=ROOT/'build/rtl-executive/cp-paired-read-replay-verilator-v2-20261025/adapters/dorado_local_r_operand_step.sv'
    raw=source.read_text();assert 'state_next=state_q;'in raw
    for h,flag,a,b in [(0,589,'547:540','563:556'),(1,591,'555:548','571:564')]:
        statement=f'if(!r_visible_n[{h}]) state_next[{579+8*h}:{572+8*h}]=state_q[{flag}] ? state_q[{a}] : state_q[{b}];'
        assert raw.count(statement)==1
    (out/'operand-source.sv').write_text(raw)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    shutil.copy2(Path(__file__).with_name('generate_native_r_capture.py'),out/'generate_native_r_capture.py')
    (out/'native_R_manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    s=dict(status='RUNNING',scope=__doc__,started=time.time(),driver_pid=os.getpid(),
           no_protocol_assumptions=True,initial_native_Q_bits_unconstrained=16,
           monitor_valid_only_initializer=True,source_operand_sha256=sha(source),
           original_native_cell_sha256=manifest['native_cell_sha256'],cases=[],
           unbounded_full_processor_proof=False,analog_clock_equivalence=False)
    def save():(out/'run-state.json').write_text(json.dumps(s,indent=2)+'\n')
    save()
    for mutation in [None,'final-clock','last-data','invert-select']:
        d=out/(mutation or'contracts');d.mkdir();f=FOLD;b=bridge
        if mutation=='final-clock':
            old='result[34+half]=first[34+half] & last[34+half];';new='result[34+half]=last[34+half];'
            assert f.count(old)==1;f=f.replace(old,new,1)
        elif mutation=='last-data':
            assert f.count('result=first;')==1;f=f.replace('result=first;','result=last;',1)
        elif mutation=='invert-select':
            old='wire RbBypassDly=select_b;';assert b.count(old)==2;b=b.replace(old,'wire RbBypassDly=!select_b;')
        for name,text in [('bridge.sv',b),('cell.v',cell),('contract.sv',TOP.replace('FOLD',f))]:
            (d/name).write_text(text)
        sat='sat -timeout 60 '
        sat+=('-seq 4 -prove native_match 1 -falsify -show-ports -dump_json /work/'+d.name+'/counterexample.json'if mutation else
              '-seq 2 -tempinduct -verify -prove native_match 1')
        (d/'proof.ys').write_text(f'''read_verilog -formal -sv /work/{d.name}/bridge.sv /work/{d.name}/cell.v /work/{d.name}/contract.sv
hierarchy -check -top contract
proc
flatten
opt
check -assert
{sat}
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
        cmd=[str(yosys),'-Q','-T','-s',f'/work/{d.name}/proof.ys']
        row=dict(mutation=mutation,command=cmd,files={p.name:sha(p)for p in d.iterdir()},started=time.time())
        with(d/'proof.log').open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT,env=env);row['pid']=child.pid;s['cases'].append(row);save();code=child.wait()
        log=(d/'proof.log').read_text();passed=code==0 and ('model found: FAIL!'in log if mutation else'Induction step proven: SUCCESS!'in log)
        row.update(returncode=code,passed=passed,log_sha256=sha(d/'proof.log'),finished=time.time());save()
        print(mutation or'contracts','PASS'if passed else'FAIL',flush=True)
        if not passed:break
    passed=len(s['cases'])==4 and all(r['passed']for r in s['cases'])
    s.update(status='PASS_ORIGINAL_NATIVE_R_CAPTURE_AND_NEGATIVES'if passed else'TERMINAL_NATIVE_R_CAPTURE_NOT_ESTABLISHED',finished=time.time())
    s['files']={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()and p.name!='run-state.json'}
    save();return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--yosys',type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.output.resolve(),a.yosys.resolve()))
