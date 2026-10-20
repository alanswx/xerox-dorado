#!/usr/bin/env python3
"""Actual frozen disk RTL: cache-tag capture excludes a simultaneous guest edge.

Use the core's falling-edge ALTCLKCTRL contract, initially enabled, with PLL
reset held through startup. Exercise tags, illegal heads, remount, attachment,
reset, arbitrary additional guest stalls and delayed/failed host acknowledgments.
This is a functional clock-condition proof, not vendor timing simulation or disk
payload/CRC qualification. No timing exception is generated or approved here.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
from generate_serial_boards import ROOT,raw_receipt,once

SOURCE='terminal-fit/src/fpga/mister/dorado_pack_hps.sv'

TOP='''`timescale 1ns/1ps
`default_nettype none
module disk_cache_capture;
 reg clk=0,pll_reset=1,extra_reset=0,extra_ready=0;
 reg attached=1,remounted=0,sd_ack=0;
 reg [11:0] tagbus_n=12'hfff;
 reg cyltag_n=1,headtag_n=1,conttag_n=1;
 wire pack_ready,machine_clk;
 reg gate_open=1'b1;
 wire machine_enable=!(pll_reset||extra_reset) && pack_ready && extra_ready;
 always @(negedge clk)gate_open<=machine_enable;
 assign machine_clk=clk && gate_open;
 dorado_pack_hps #(.INITIAL_CYL(4),.INITIAL_HEAD(4),.HOST_TIMEOUT_CLOCKS(31)) dut(
 .clk_sys(clk),.machine_clk(machine_clk),.reset(pll_reset),.attached(attached),.readonly(1'b0),.remounted(remounted),
 .tagbus_n(tagbus_n),.cyltag_n(cyltag_n),.headtag_n(headtag_n),.drivetag_n(1'b1),.conttag_n(conttag_n),
 .selected(1'b0),.data_adv(1'b0),.pulse(16'b0),.write_data_p(1'b0),.write_data_m(1'b0),
 .data_bit(),.write_gate(),.endofcyl_n(),.ready(pack_ready),.dirty(),.fault(),.host_phase(),.fault_debug(),.fault_extra(),
 .sd_lba(),.sd_blk_cnt(),.sd_rd(),.sd_wr(),.sd_ack(sd_ack),.sd_buff_addr(13'b0),.sd_buff_dout(16'b0),.sd_buff_wr(1'b0),.sd_buff_din(),.reads(),.writes());
 integer ticks=0,captures=0,guest_edges=0,tag_changes=0,illegal_heads=0,failures=0;
 integer previous_captures;
 reg [11:0] before_cyl;
 reg [5:0] before_head;
 always @(posedge machine_clk)guest_edges=guest_edges+1;
 task high;begin
  previous_captures=dut.proof_captures;before_cyl=dut.cyl;before_head=dut.head;
  clk=1;#2;ticks=ticks+1;
  if(dut.proof_captures!=previous_captures)begin
   captures=captures+1;
   if(gate_open!==0)$fatal(1,"cache capture overlapped enabled guest edge");
   if(dut.cyl!==before_cyl || dut.head!==before_head)$fatal(1,"cache source changed on capture edge");
   if(dut.cached_cyl!==before_cyl || dut.cached_head!==before_head)$fatal(1,"cache capture did not preserve source");
  end
  if(dut.cyl!==before_cyl || dut.head!==before_head)tag_changes=tag_changes+1;
  if(dut.head>=5)illegal_heads=illegal_heads+1;
  if(dut.host_fault)failures=failures+1;
 end endtask
 integer seed=32'h674d13ac,i,r;
 initial begin
  // The PLL/reset guard is present before first useful cache capture even if
  // the clock-control enable register powers up high.
  repeat(4)begin high();clk=0;#2;end
  pll_reset=0;extra_ready=1;
  for(i=0;i<200000;i=i+1)begin
   // Drive before the falling edge, exactly as the core's raw enable contract.
   r=$random(seed);
   extra_ready=(r&7)!=0;extra_reset=(i%431)==0;
   pll_reset=(i%997)==0;remounted=(i%283)==0;
   attached=(i%509)>3;
   cyltag_n=(r&15)!=0;headtag_n=((r>>4)&15)!=0;conttag_n=((r>>8)&15)!=0;
   tagbus_n=~(r>>12);
   sd_ack=((r>>6)&7)==0;
   clk=0;#2;
   // A request changing AFTER the sampling edge cannot create a guest edge.
   if((i%5)==0)extra_ready=~extra_ready;
   high();
  end
  if(captures<100 || guest_edges<1000 || tag_changes<100 || illegal_heads<100 || failures<100)$fatal(1,"disk capture coverage missing captures=%0d guest=%0d tags=%0d illegal=%0d failures=%0d",captures,guest_edges,tag_changes,illegal_heads,failures);
  $display("DISK CACHE CAPTURE PASS ticks=%0d captures=%0d guest=%0d tags=%0d illegal=%0d failures=%0d",ticks,captures,guest_edges,tag_changes,illegal_heads,failures);$finish;
 end
endmodule
`default_nettype wire
'''


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False)
    original=raw_receipt(ROOT/'fpga/mister/reports/2026-10-19-snapshot-serial-chipwidth',SOURCE)
    core=raw_receipt(ROOT/'fpga/mister/reports/2026-10-19-snapshot-serial-chipwidth','terminal-fit/core/Dorado.sv').decode()
    assert '.ena_register_mode("falling edge")'in core and '.ena(machine_enable)'in core
    assert 'wire reset = RESET | status[0] | ~pll_locked;'in core
    instrumented=once(original.decode(),'  reg valid = 0;','  integer proof_captures=0;\n  reg valid = 0;')
    instrumented=once(instrumented,'          cached_cyl <= cyl; cached_head <= head;',
                      '          proof_captures<=proof_captures+1;\n          cached_cyl <= cyl; cached_head <= head;')
    # Icarus requires these module declarations before functions that read
    # them. Relocate declarations only; prove an exact inverse byte-for-byte.
    declarations=['  reg [11:0] cyl = INITIAL_CYL;\n','  reg [5:0] head = INITIAL_HEAD;\n',
                  '  reg [2:0] host_state = 0;\n','  reg [1:0] wr_state = 0, wr_block = 0;\n',
                  '  reg [5:0] wr_bits = 0;\n']
    normalized=instrumented
    for declaration in declarations:normalized=once(normalized,declaration,'')
    marker='  function automatic [31:0] fault_status'
    normalized=once(normalized,marker,''.join(declarations)+marker)
    inverse=once(normalized,''.join(declarations)+marker,marker)
    for declaration in declarations:
        at=instrumented.index(declaration)
        # Restore relative to the entire unchanged suffix following this line.
        suffix=instrumented[at+len(declaration):]
        for later in declarations:
            if later in suffix:suffix=suffix.replace(later,'',1)
        inverse=once(inverse,suffix,declaration+suffix)
    assert inverse==instrumented
    instrumented=normalized
    cases=[('candidate',None,None,None)]
    if mutations:cases += [('bypass_pack_gate','fixture',' && pack_ready',''),
                           ('capture_illegal_head','rtl','end else if (need_track && head < 5)','end else if (need_track)'),
                           ('always_guest_clock','fixture','assign machine_clk=clk && gate_open;','assign machine_clk=clk;')]
    results=[]
    for label,target,before,after in cases:
        case=out/label;case.mkdir();top=TOP;rtl=instrumented
        if target=='fixture':top=once(top,before,after)
        elif target=='rtl':rtl=once(rtl,before,after)
        (case/'fixture.sv').write_text(top);(case/'medium.sv').write_text(rtl)
        command=['iverilog','-g2012','-s','disk_cache_capture','-o',str(case/'test.vvp'),str(case/'fixture.sv'),str(case/'medium.sv')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=60)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text.strip(),flush=True)
        if target:assert result.returncode and ('cache capture overlapped'in text or 'cache source changed'in text)
        else:assert not result.returncode and 'DISK CACHE CAPTURE PASS'in text,text
        results.append(dict(case=label,returncode=result.returncode))
    inputs=out/'executed-inputs';inputs.mkdir();(inputs/'dorado_pack_hps.sv').write_bytes(original)
    (inputs/'Dorado.sv').write_text(core);(inputs/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,cases=results,original_sha256=hashlib.sha256(original).hexdigest(),
        executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},
        limits='Falling-edge clock contract, with PLL-reset startup; no vendor encrypted timing model, full disk payload test, new SDC exception or snapshot wrapper claim.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
