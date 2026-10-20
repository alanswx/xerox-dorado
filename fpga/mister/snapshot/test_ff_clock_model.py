#!/usr/bin/env python3
"""Check the low-latch simulation clock contract, including high-half toggles.

This does not stand in for the actual ALTCLKCTRL falling-edge implementation.
Known binary enables are driven; real integration must supply its raw request
before the hardware's falling-edge sampling boundary.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

HERE=Path(__file__).resolve().parent
TOP='''`timescale 1ns/1ps
module clock_fixture;
 parameter FALLING_ONLY=0;
 reg clk=1,enable=0;wire gated_clk,gate_open;
 dorado_ff_clock_model dut(.clk(clk),.enable(enable),.gated_clk(gated_clk),.gate_open(gate_open));
 integer ff_edges=0,source_edges=0,changes=0,seed=32'hc10c7e57;
 reg expected_level;integer before_edges;
 always @(posedge gated_clk)ff_edges=ff_edges+1;
 initial begin
  #2;
  for(integer cycle=0;cycle<5000;cycle=cycle+1)begin
   enable=$random(seed)&1;#1;clk=0;#1;
   if(gated_clk!==1'b0)$fatal(1,"clock high during source low");
   expected_level=enable;before_edges=ff_edges;
   enable=~enable;#1;
   if(!FALLING_ONLY)expected_level=enable;
   clk=1;source_edges=source_edges+1;#1;
   if(gated_clk!==expected_level || ff_edges!=before_edges+expected_level)$fatal(1,"source-edge clock contract differs");
   for(integer n=0;n<8;n=n+1)begin
    enable=~enable;changes=changes+1;#1;
    if(gated_clk!==expected_level || ff_edges!=before_edges+expected_level)$fatal(1,"high-half request caused clock glitch");
   end
  end
  $display("FF CLOCK MODEL PASS source_edges=%0d output_edges=%0d high_changes=%0d",source_edges,ff_edges,changes);$finish;
 end
endmodule
'''


def run(out,falling=False):
    out.mkdir(parents=True,exist_ok=False);source=(HERE/('ff_clock_falling_model.sv'if falling else'ff_clock_model.sv')).read_text();(out/'fixture.sv').write_text(TOP)
    results=[]
    for name,before,after in [('candidate',None,None),('runt','assign gated_clk=clk & gate_open;','assign gated_clk=clk & enable;'),
                              ('wrong_phase','always @(negedge clk)'if falling else'always_latch if(!clk)','always @(posedge clk)'if falling else'always_latch if(clk)'),('inverted','assign gated_clk=clk & gate_open;','assign gated_clk=clk & ~gate_open;')]:
        case=out/name;case.mkdir();text=source
        if before:assert text.count(before)==1;text=text.replace(before,after,1)
        (case/'clock.sv').write_text(text)
        command=['iverilog','-g2012','-s','clock_fixture','-Pclock_fixture.FALLING_ONLY='+str(int(falling)),'-o',str(case/'clock.vvp'),str(out/'fixture.sv'),str(case/'clock.sv')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'clock.vvp')],capture_output=True,text=True,timeout=20);log=result.stdout+result.stderr
        (case/'test.txt').write_text(log);print(name+': '+log.strip(),flush=True)
        if before:assert result.returncode and ('clock glitch'in log or 'clock contract differs'in log)
        else:assert not result.returncode and 'FF CLOCK MODEL PASS'in log
        results.append(dict(case=name,returncode=result.returncode))
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,falling_only=falling,cases=results,source_sha256=hashlib.sha256(source.encode()).hexdigest(),tool_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--falling',action='store_true');a=p.parse_args();run(a.output.resolve(),a.falling)
