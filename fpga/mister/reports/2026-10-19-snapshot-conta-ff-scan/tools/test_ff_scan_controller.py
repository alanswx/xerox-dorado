#!/usr/bin/env python3
"""Exercise a serial FF transport against independent word/bit bank operations.

Controller and mock physical bank only; no real board, global barrier, area or
timing qualification. A partial rotation requires system reset after a fault.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

HERE = Path(__file__).resolve().parent
TB = r'''
module tb;
parameter N=56;
reg clk=0; always #5 clk=~clk;
reg reset=1,freeze=0,held=0,request=0,write=0;
reg [23:0] index=0;
reg [17:0] data=0;
wire sin,step,ready,hold_required,busy,done,error,fault;
wire [17:0] result;
reg [N-1:0] physical,expected,before_op;
dorado_ff_scan_controller #(.STATE_BITS(N)) d(
 .clk(clk),.reset(reset),.freeze(freeze),.guest_held(held),.request(request),
 .write(write),.index(index),.data(data),.serial_out(physical[0]),
 .serial_in(sin),.scan_step(step),.ready(ready),.hold_required(hold_required),
 .busy(busy),.done(done),.error(error),.fault(fault),.result(result));
generate if(N==1)begin
 always @(posedge clk)if(step)physical<=sin;
end else begin
 always @(posedge clk)if(step)physical<={sin,physical[N-1:1]};
end endgenerate
task tick; begin @(posedge clk);#1; end endtask
task idle; begin @(negedge clk);request=0;tick;end endtask
integer operations=0,w,b,k,waited;
reg [17:0] want;
task operation(input integer address,input bit wr,input [17:0] payload);
 begin
  before_op=physical;expected=physical;want=0;
  for(integer bitno=0;bitno<18;bitno=bitno+1)
   if(address*18+bitno<N)begin
    want[bitno]=physical[address*18+bitno];
    if(wr)expected[address*18+bitno]=payload[bitno];
   end
  @(negedge clk);request=1;write=wr;index=address;data=payload;tick;
  if(!busy||done||!hold_required)$fatal(1,"request handshake");
  // Every live request/data input changes while busy. A second request must
  // neither replace the owner nor affect the captured payload/address.
  waited=0;
  while(!done)begin
   @(negedge clk);request=1;write=!wr;index=24'hffffff;data=~payload;
   tick;waited=waited+1;
   if(waited>N+1)$fatal(1,"missing bounded completion");
  end
  if(waited!=N||busy||error||fault||hold_required)$fatal(1,"rotation length/status");
  if(physical!==expected)$fatal(1,"physical word oracle");
  if(result!==(wr?18'b0:want))$fatal(1,"reply word oracle");
  operations=operations+1;idle;
 end
endtask
initial begin
 physical=0;tick;idle;
 @(negedge clk);reset=0;freeze=1;held=1;request=1;tick;
 if(busy||done||ready!==1'b1)$fatal(1,"entry request accepted");
 idle;
 for(w=0;w<(N+17)/18;w=w+1)begin
  operation(w,1,18'h2a695^(w*97));operation(w,0,0);
 end
 for(b=0;b<18;b=b+1)begin
  w=b%((N+17)/18);operation(w,1,18'b1<<b);operation(w,0,0);
 end
 before_op=physical;
 @(negedge clk);request=1;index=(N+17)/18;write=1;data=18'h3ffff;tick;
 if(!done||!error||busy||result!==0||physical!==before_op)$fatal(1,"invalid request");idle;
 @(negedge clk);request=1;index=24'hffffff;tick;
 if(!done||!error||busy||physical!==before_op)$fatal(1,"high index truncation");idle;
 @(negedge clk);freeze=0;request=1;index=0;tick;
 if(busy||done||step||physical!==before_op)$fatal(1,"running request accepted");
 @(negedge clk);freeze=1;held=1;request=0;tick;idle;
 @(negedge clk);request=1;index=0;write=1;data=18'h12345;tick;
 if(N>3)begin
  repeat(2)begin @(negedge clk);request=0;tick;end
  if(!busy)$fatal(1,"partial rotation unexpectedly complete");
 end
 @(negedge clk);request=0;held=0;tick;
 if(!fault||!busy||!hold_required||!done||!error||step)$fatal(1,"lost boundary not quarantined");
 before_op=physical;
 @(negedge clk);freeze=1;held=1;request=1;index=0;
 repeat(4)begin tick;if(step||ready||physical!==before_op||!hold_required)$fatal(1,"fault escaped");end
 // Only whole-system reset is a valid recovery contract. Re-seed the mock
 // physical bank explicitly after resetting the controller.
 @(negedge clk);reset=1;tick;@(negedge clk);physical=0;reset=0;request=0;tick;idle;
 operation(0,1,18'h19a5);operation(0,0,0);
 $display("SCAN PASS: N=%0d operations=%0d;words/tail/busy/entry/invalid/high/running/fault/reset",N,operations);
 $finish;
end
endmodule
'''


def run(out):
    out.mkdir(parents=True, exist_ok=False)
    rtl = (HERE/'ff_scan_controller.sv').read_text()
    (out/'executed-driver.py').write_bytes(Path(__file__).read_bytes())
    (out/'reference-controller.sv').write_text(rtl)
    records = []
    variants = [('candidate', None, None),
                ('lost_feedback', ': serial_out;', ": 1'b0;"),
                ('early_completion', 'position == STATE_BITS-1', 'position == STATE_BITS-2'),
                ('ignored_write_window', 'writing && in_window ?', 'writing ?'),
                ('lost_quarantine', '&& !fault;', ';')]
    for name, before, after in variants:
        text = rtl
        if before:
            assert before in text
            text = text.replace(before, after)
        for bits in ((1, 17, 18, 19, 56, 1725) if name == 'candidate' else (56,)):
            case = out/(name+'-'+str(bits))
            case.mkdir()
            (case/'controller.sv').write_text(text)
            (case/'tb.sv').write_text(TB)
            command = ['iverilog', '-g2012', '-s', 'tb', '-Ptb.N='+str(bits),
                       '-o', str(case/'sim'), str(case/'controller.sv'), str(case/'tb.sv')]
            (case/'command.json').write_text(json.dumps(command, indent=2)+'\n')
            result = subprocess.run(command, capture_output=True, text=True)
            (case/'build.log').write_text(result.stdout+result.stderr)
            assert result.returncode == 0, result.stderr
            result = subprocess.run(['vvp', str(case/'sim')], capture_output=True, text=True, timeout=60)
            (case/'test.log').write_text(result.stdout+result.stderr)
            assert (result.returncode == 0) == (name == 'candidate'), result.stdout+result.stderr
            if name == 'candidate':
                assert 'SCAN PASS:' in result.stdout
            print(name, bits, result.stdout.strip(), flush=True)
            records.append(dict(case=name, bits=bits, returncode=result.returncode))
    (out/'results.json').write_text(json.dumps(dict(status='PASS', scope=__doc__,
        cases=records, controller_sha256=hashlib.sha256(rtl.encode()).hexdigest()), indent=2)+'\n')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    run(a.output.resolve())
