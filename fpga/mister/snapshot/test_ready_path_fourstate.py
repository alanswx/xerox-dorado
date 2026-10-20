#!/usr/bin/env python3
"""Exhaustive 0/1/X/Z readiness checks on actual WIDTH8/12 frontend RTL.

All 65,536 combinations of reset, waiting, cache-valid, pending-write,
write-done, local/global hold, initialization and pack-ready are evaluated.
This proves the local predicate; actual global-to-local hold is a separate test.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import generate_storage_chipwidth as gen
import generate_ready_path as change


def fixture(expression=None):
    old,new=change.expressions();new=expression or new
    old=old.replace('storage_ready','(storage_initialized && ready[u])')
    new=new.replace('snapshot_frontends_ready','drained[u]')
    return '''`default_nettype none
module ready_path_fourstate;
 reg reset=0,waiting=0,cache_valid=0,write_pending=0,write_done=0;
 reg board_state_hold=0,storage_initialized=0,pack_ready=0;
 wire [1:0] ready,drained,old_gate,new_gate;
 genvar u;
 generate for(u=0;u<2;u=u+1)begin:members
 localparam integer WIDTH=u==0?8:12;
 dorado_storage_group_chipwidth #(.WIDTH(WIDTH)) dut(
 .state_guest_advance(1'b0),.state_hold(board_state_hold),.state_scan_step(1'b0),.state_serial_in(1'b0),
 .state_serial_out(),.state_drain_ready(drained[u]),.state_frozen(),
 .host_clk(1'b0),.machine_clk(1'b0),.reset(reset),.ras_n(1'b1),.cas_n(1'b1),.we_n(1'b1),.addr_will_change(1'b0),
 .addr(16'b0),.data_in({WIDTH{1'b0}}),.data_out(),.ready(ready[u]),.req_valid(),.req_ready(1'b0),
 .req_write(),.req_addr(),.req_data(),.rsp_valid(1'b0),.rsp_ready(),.rsp_data(16'b0));
 always @* begin
  dut.waiting=waiting;dut.cache_valid=cache_valid;
  dut.write_pending=write_pending;dut.write_done=write_done;
 end
 assign old_gate[u]='''+old+''';
 assign new_gate[u]='''+new+''';
 end endgenerate
 function automatic reg symbol(input integer n);
  case(n)0:symbol=0;1:symbol=1;2:symbol=1'bx;3:symbol=1'bz;endcase
 endfunction
 integer i,known_ready=0,unknown_result=0;
 initial begin
  for(i=0;i<65536;i=i+1)begin
   reset=symbol(i&3);waiting=symbol((i>>2)&3);cache_valid=symbol((i>>4)&3);
   write_pending=symbol((i>>6)&3);write_done=symbol((i>>8)&3);
   board_state_hold=symbol((i>>10)&3);storage_initialized=symbol((i>>12)&3);pack_ready=symbol((i>>14)&3);
   #1;
   if(old_gate!==new_gate)$fatal(1,"four-state CPU readiness differs case=%0d old=%b new=%b",i,old_gate,new_gate);
   if(old_gate===2'b11)known_ready=known_ready+1;
   if((^old_gate)===1'bx)unknown_result=unknown_result+1;
  end
  if(known_ready<1 || unknown_result<1)$fatal(1,"four-state coverage missing");
  $display("READY PATH FOURSTATE PASS combinations=65536 widths=8/12 known-ready=%0d unknown-result=%0d",known_ready,unknown_result);$finish;
 end
endmodule
`default_nettype wire
'''


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);source,manifest=gen.generate();_,new=change.expressions()
    cases=[('candidate',None)]
    if mutations:cases += [('lost_init',new.replace(' && storage_initialized','')),
                           ('lost_hold',new.replace(' && !board_state_hold','')),
                           ('wrong_ready',new.replace('snapshot_frontends_ready',"1'b1"))]
    results=[]
    for label,expression in cases:
        case=out/label;case.mkdir();(case/'fixture.sv').write_text(fixture(expression));(case/'frontend.sv').write_text(source)
        command=['iverilog','-g2012','-s','ready_path_fourstate','-o',str(case/'test.vvp'),str(case/'fixture.sv'),str(case/'frontend.sv')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=60)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text.strip(),flush=True)
        if expression:assert result.returncode and 'four-state CPU readiness differs'in text
        else:assert not result.returncode and 'READY PATH FOURSTATE PASS'in text,text
        results.append(dict(case=label,returncode=result.returncode))
    inputs=out/'executed-inputs';inputs.mkdir()
    for p in (Path(__file__),Path(gen.__file__),Path(change.__file__)):(inputs/p.name).write_bytes(p.read_bytes())
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,cases=results,combinations=65536,widths=[8,12],
        frontend_sha256=hashlib.sha256(source.encode()).hexdigest(),executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()}),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true')
    a=p.parse_args();run(a.output.resolve(),a.mutations)
