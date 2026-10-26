#!/usr/bin/env python3
"""Exercise the actual Hold producer/buffer under stalls, changes and X/Z inputs.

Compare every usable buffered decision to the live source-bound producer.
Explicitly change context between host edges, reset during a pending decision,
and exercise four-state raw fallback. Wrong tags/payloads must fail on data.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

TB='''`timescale 1ns/1ps
module tb_native_hold_buffer;
 reg clk=0,reset=1;reg [131:0] descriptor=0;reg override_raw=0;reg raw_override=0;
 wire producer,raw_hold,ready,actual,valid,known,saved_hold;wire [131:0] saved_context;
 integer checks=0,usable0=0,usable1=0,misses=0,fallbacks=0,i,j;reg [31:0] rng=32'h851327ad;
 reg [131:0] previous_descriptor;
 dorado_memc_hold_function function_under_test(descriptor,producer);
 assign raw_hold=override_raw ? raw_override:producer;
 dorado_memc_hold_buffer dut(clk,reset,descriptor,raw_hold,ready,actual,valid,saved_context,saved_hold,known);
 task check;begin
  #1;checks=checks+1;
  if(reset===1'b1)begin if(ready!==1'b0)$fatal(1,"ready during reset");end
  else if(known===1'b1)begin
   if(ready===1'b1)begin
    if(actual!==raw_hold)$fatal(1,"wrong prepared decision actual=%b raw=%b context=%h saved=%h",actual,raw_hold,descriptor,saved_context);
    if(actual)usable1=usable1+1;else usable0=usable0+1;
   end else if(ready!==1'b0)$fatal(1,"unknown binary preparation validity");
   if(valid && saved_context!==descriptor)begin
    if(ready!==1'b0)$fatal(1,"stale source context accepted");
    misses=misses+1;
   end
  end else begin
   if(ready!==1'b1 || actual!==raw_hold)$fatal(1,"four-state fallback differs ready=%b actual=%b raw=%b",ready,actual,raw_hold);
   fallbacks=fallbacks+1;
  end
 end endtask
 task edge_step;begin clk=0;#1;clk=1;#1;clk=0;check;end endtask
 task random_descriptor;begin
  for(j=0;j<132;j=j+1)begin
   rng=rng^(rng<<13);rng=rng^(rng>>17);rng=rng^(rng<<5);descriptor[j]=rng[0];
  end
 end endtask
 initial begin
  edge_step;reset=0;check;
  for(i=0;i<4096;i=i+1)begin
   if(i%3!=0)begin random_descriptor;check;end
   edge_step;
   if(i%31==0)begin
    previous_descriptor=descriptor;descriptor=~descriptor;check;descriptor=previous_descriptor;check;
   end
   if(i%127==0)begin reset=1;edge_step;reset=0;check;edge_step;end
  end
`ifndef VERILATOR
  // Context X/Z must retain raw behavior even when masking makes Hold known.
  for(i=0;i<132;i=i+1)begin
   random_descriptor;descriptor[i]=1'bx;check;edge_step;
   descriptor[i]=1'bz;check;edge_step;
  end
  random_descriptor;override_raw=1;
  raw_override=1'bx;check;edge_step;
  raw_override=1'bz;check;edge_step;
  override_raw=0;edge_step;
`endif
  if(usable0==0 || usable1==0 || misses==0)$fatal(1,"insufficient real producer coverage");
  $display("NATIVE_HOLD_BUFFER_PASS checks=%0d usable0=%0d usable1=%0d misses=%0d fallbacks=%0d",checks,usable0,usable1,misses,fallbacks);
  $finish;
 end
endmodule
'''


def run(source,out):
    assert not out.exists();out.mkdir(parents=True)
    r=json.loads((source/'results.json').read_text());verify(source,r['files'])
    assert r['status']=='PREPARED_SOURCE_BOUND_NATIVE_HOLD_FUNCTION'
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),function_receipt_sha256=sha(source/'results.json'),
               helper_sha256=r['buffer_sha256'],actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    cases=[('icarus',None),('verilator',None),('verilator',('valid && saved_context==source_context','valid')),
           ('verilator',('saved_hold<=raw_hold;','saved_hold<=~raw_hold;'))]
    for index,(engine,mutation)in enumerate(cases):
        d=out/(engine+'-'+str(index));d.mkdir()
        for f in source.iterdir():
            if f.suffix in ['.v','.sv']:shutil.copy2(f,d/f.name)
        if mutation:
            raw=(d/'buffer.sv').read_text();assert raw.count(mutation[0])==1
            (d/'buffer.sv').write_text(raw.replace(*mutation,1))
        (d/'tb.sv').write_text(TB)
        paths=[str(f)for f in sorted(d.iterdir())if f.suffix in ['.v','.sv']]
        commands=([['iverilog','-g2012','-s','tb_native_hold_buffer','-o',str(d/'test.vvp'),*paths],['vvp',str(d/'test.vvp')]]if engine=='icarus'else
                  [['verilator','--binary','--timing','--top-module','tb_native_hold_buffer','-Wno-fatal','--Mdir',str(d/'obj'),'-o','test','-j','2',*paths],[str(d/'obj/test')]])
        case=dict(engine=engine,mutation=mutation,actions=[],files={p.name:sha(p)for p in d.iterdir()if p.is_file()});state['actions'].append(case);save()
        for n,cmd in enumerate(commands):
            with(d/('compile.log'if n==0 else'test.log')).open('x')as log:
                child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT)
                a=dict(pid=child.pid,command=cmd,started=time.time());case['actions'].append(a);save();code=child.wait()
            a.update(returncode=code,finished=time.time());save()
            if n==0 and code:break
        log=(d/'test.log').read_text()if(d/'test.log').exists()else''
        case['passed']=(len(case['actions'])==2 and case['actions'][0]['returncode']==0 and
                        ((case['actions'][1]['returncode']==0 and'NATIVE_HOLD_BUFFER_PASS'in log)if mutation is None else
                         (case['actions'][1]['returncode']!=0 and any(s in log for s in ['wrong prepared decision','stale source context accepted']))))
        save();print(d.name,case['passed'],flush=True)
        if not case['passed']:break
    passed=len(state['actions'])==4 and all(c['passed']for c in state['actions'])
    state.update(status='PASS_DUAL_ENGINE_SOURCE_HOLD_BUFFER_AND_NEGATIVES'if passed else'TERMINAL_HOLD_BUFFER_TEST_FAILED',finished=time.time(),
                 hardware_speed_timing_or_60ns_qualified=False)
    save();return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve()))
