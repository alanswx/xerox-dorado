#!/usr/bin/env python3
"""Predict actual address changes from real original helper edge behavior.

Random physical helper states include independently varied row/column/strobe
history. Equal-address captures are directed. Tests known binary state, both
ROW_BITS7 and8; no four-state or native clock-barrier equivalence claim.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import generate_msa_compact as gen
from generate_serial_boards import raw_receipt

TOP='''module address_fixture;
 reg clk=0,advance=0,hold=0,write=0,p4=0,p15=0;reg [17:0] state_data=0;reg [7:0] a=0;
 wire [15:0] addr8,ref8;wire [13:0] addr7,ref7;
 wire [17:0] state8,refstate8,state7,refstate7;wire change8,change7;
 integer seed=32'h1650866a,checks=0;reg [15:0] before8;reg [13:0] before7;reg expected8,expected7;
 NAME #(.ROW_BITS(8)) dut8(.state_host_clk(clk),.state_advance(advance),.state_hold(hold),.state_write(write),.state_data(state_data),
 .state_result(state8),.sys_clk(clk),.p4(p4),.p15(p15),.a(a),.addr(addr8),.addr_will_change(change8));
 REFERENCE #(.ROW_BITS(8)) ref_dut8(.state_host_clk(clk),.state_advance(advance),.state_hold(hold),.state_write(write),.state_data(state_data),
 .state_result(refstate8),.sys_clk(clk),.p4(p4),.p15(p15),.a(a),.addr(ref8));
 NAME #(.ROW_BITS(7)) dut7(.state_host_clk(clk),.state_advance(advance),.state_hold(hold),.state_write(write),.state_data(state_data),
 .state_result(state7),.sys_clk(clk),.p4(p4),.p15(p15),.a(a[6:0]),.addr(addr7),.addr_will_change(change7));
 REFERENCE #(.ROW_BITS(7)) ref_dut7(.state_host_clk(clk),.state_advance(advance),.state_hold(hold),.state_write(write),.state_data(state_data),
 .state_result(refstate7),.sys_clk(clk),.p4(p4),.p15(p15),.a(a[6:0]),.addr(ref7));
 task same;
  if({addr8,addr7,state8,state7}!=={ref8,ref7,refstate8,refstate7})$fatal(1,"original address state differs");
 endtask
 initial begin
  for(integer n=0;n<12000;n=n+1)begin
   clk=0;hold=1;write=1;state_data=$random(seed);advance=0;#1;clk=1;#1;same;
   clk=0;hold=0;write=0;advance=1;p4=$random(seed);p15=$random(seed);
   case(n%5)
    0:a=ref8[7:0];
    1:a=ref8[15:8];
    2:a={1'b0,ref7[6:0]};
    3:a={1'b0,ref7[13:7]};
    4:a=$random(seed);
   endcase
   #1;before8=ref8;before7=ref7;expected8=change8;expected7=change7;
   clk=1;#1;same;
   if((ref8!=before8)!==expected8 || (ref7!=before7)!==expected7)$fatal(1,"address change prediction differs");
   checks=checks+1;
  end
  $display("ADDRESS PREDICTION PASS independent_states=%0d geometries=7/8",checks);$finish;
 end
endmodule
'''


def run(out):
    out.mkdir(parents=True,exist_ok=False);_,_,helpers=gen.generate();source=helpers['cell_MK4096P_addr.sv']
    reference=raw_receipt(gen.base.ROOT/'fpga/mister/reports/2026-10-19-snapshot-msa-transport',
                          'simulation-v5/candidate/cell_MK4096P_addr.sv').decode()
    name=gen.base.MODULE+'__cell_MK4096P_addr_msa_snapshot';reference=gen.base.rename(reference,{name:'original_address_helper'})
    fixture=TOP.replace('REFERENCE','original_address_helper').replace('NAME',name)
    (out/'fixture.sv').write_text(fixture);(out/'reference.sv').write_text(reference)
    (out/'executed-runner.py').write_bytes(Path(__file__).read_bytes())
    cases=[('candidate',None,None),('missing_column','((~p15 & cas_d) && (col != a))',"1'b0"),
           ('strobe_not_change','((~p4 & ras_d) && (row != a)) || ((~p15 & cas_d) && (col != a))','(~p4 & ras_d) || (~p15 & cas_d)'),
           ('wrong_history','(~p4 & ras_d)','(~p4 & cas_d)'),('wrong_address_half','(row != a)','(col != a)')]
    results=[]
    for label,before,after in cases:
        case=out/label;case.mkdir();text=source
        if before:assert text.count(before)==1;text=text.replace(before,after,1)
        (case/'candidate.sv').write_text(text)
        command=['iverilog','-g2012','-s','address_fixture','-o',str(case/'test.vvp'),str(case/'candidate.sv'),str(out/'reference.sv'),str(out/'fixture.sv')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=30)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text.strip(),flush=True)
        if before:assert result.returncode and 'address change prediction differs'in text
        else:assert result.returncode==0 and 'ADDRESS PREDICTION PASS'in text
        results.append(dict(case=label,returncode=result.returncode))
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,cases=results,
        candidate_sha256=hashlib.sha256(source.encode()).hexdigest(),reference_sha256=hashlib.sha256(reference.encode()).hexdigest()),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.output.resolve())
