#!/usr/bin/env python3
"""Test one-port read-latch closure and transparent visible-R handoff.

Compare48 actual Qs against12 original MC10173 models. A single asynchronous
ROM-fixture address changes only after the capture edge, with late data before
close and no extra host edge to propagate read-close/visible-open. This is a
component ownership prototype, not full RAM/controller/l07/native qualification.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import time

ROOT=Path(__file__).resolve().parents[1]
TB=r'''`timescale 1ns/1ps
module tb_r_operand_latches;
 reg host=0,advance=1,load=0;
 reg [1:0] read_gate=3,read_next=3,vis_gate=3,vis_next=3,bypass=0,bypass_next=0;
 reg [1:0] ref_read_gate=3,ref_vis_gate=3,ref_bypass=0;
 reg [7:0] address=0,address_next=0,ref_address=0;
 reg [15:0] md=0,md_next=0,ref_md=0;
 reg [15:0] rom_fixture[0:255];
 wire [31:0] data={rom_fixture[address],md},ref_data={rom_fixture[ref_address],ref_md};
 wire [31:0] read_q,ref_read_q;wire [15:0] visible_q,ref_visible_q;wire [47:0] held;
 reg [47:0] fixture=48'h13579bdf2468;
 integer checks=0,handoffs=0,late_updates=0,closed_holds=0;
 // Real upstream registers change AFTER the capture edge. Keep these NBAs
 // in a clocked process: Verilator treats initial/task NBAs as blocking.
 always @(posedge host)if(advance)begin
   read_gate<=read_next;vis_gate<=vis_next;address<=address_next;
   md<=md_next;bypass<=bypass_next;
   ref_read_gate<=read_next;ref_vis_gate<=vis_next;
 end
 dorado_r_operand_latches dut(host,advance,read_gate,read_next,vis_gate,vis_next,data,bypass,read_q,visible_q,held);
 for(genvar nibble=0;nibble<8;nibble++)begin: original_read
   wire [3:0] d=ref_data[4*nibble+:4];
   original_MC10173 latch(.p5(d[0]),.p3(d[1]),.p12(d[2]),.p10(d[3]),.p6(d[0]),.p4(d[1]),.p13(d[2]),.p11(d[3]),
     .p9(1'b0),.p7(ref_read_gate[(nibble/2)%2]),.fixture_load(load),.fixture_state(fixture[4*nibble+:4]),.state_q(ref_read_q[4*nibble+:4]));
 end
 for(genvar nibble=0;nibble<4;nibble++)begin: original_visible
   localparam integer H=nibble/2;
   wire [3:0] a=ref_read_q[16+4*nibble+:4],b=ref_read_q[4*nibble+:4];
   original_MC10173 latch(.p5(a[0]),.p3(a[1]),.p12(a[2]),.p10(a[3]),.p6(b[0]),.p4(b[1]),.p13(b[2]),.p11(b[3]),
     .p9(ref_bypass[H]),.p7(ref_vis_gate[H]),.fixture_load(load),.fixture_state(fixture[32+4*nibble+:4]),.state_q(ref_visible_q[4*nibble+:4]));
 end
 task automatic compare;
   begin
     if(read_q!==ref_read_q||visible_q!==ref_visible_q)
       $fatal(1,"R_OPERAND_LATCH mismatch check=%0d read=%h/%h visible=%h/%h",checks,read_q,ref_read_q,visible_q,ref_visible_q);
     if((read_gate | vis_gate)!==2'b11)$fatal(1,"Invalid overlapping read/visible latch gates");
     checks++;
   end
 endtask
 task automatic commit;
   begin
     #15;host=1;
     #15;host=0;
     // Old data holds15ns after gate closure. The new source address/data
     // can now arrive; transparent reference latches follow it naturally.
     if(advance)begin ref_address=address_next;ref_md=md_next;ref_bypass=bypass_next;end
     #15;compare();
   end
 endtask
 initial begin
   for(integer i=0;i<256;i++)rom_fixture[i]=16'(i*103+37);
   dut.held_data=fixture[31:0];dut.held_r=fixture[47:32];
   #15;load=1;#15;load=0;#15;compare();
   for(integer mode=0;mode<4;mode++)for(integer value=0;value<256;value++)begin
     // Open the read bank; visible R remains closed. There is one current
     // asynchronous memory address, without a second predicted RAM port.
     read_next=0;vis_next=3;address_next=8'(value);md_next=16'(value*71+13);bypass_next=2'(mode);commit();
     begin reg [47:0] saved;
       saved=held;rom_fixture[address]=rom_fixture[address]^16'ha35c;md=md^16'h789a;ref_md=md;#15;compare();
       if(held!==saved)$fatal(1,"R latch closed storage followed unclocked open input");
       late_updates++;
     end
     // Close read and open visible in ONE host edge, while the next RAM
     // address differs. Correct R appears without another sampling edge.
     read_next=3;vis_next=0;address_next=8'(value+1);md_next=md^16'hffff;commit();handoffs++;
     // Changing the visible latch selector while open must update Q even
     // without a host clock; both candidate and original read bank is closed.
     begin reg [47:0] saved;
       saved=held;bypass=~bypass;ref_bypass=bypass;#15;compare();
       if(held!==saved)$fatal(1,"R latch closed storage changed on open selector update");
     end
     // Close visible before the following read window. Its data survives
     // subsequent RAM changes and a held machine edge.
     read_next=0;vis_next=3;address_next=8'(value+2);md_next=md^16'h1234;bypass_next=bypass;commit();
     begin reg [47:0] saved;reg [15:0] r_saved;
       saved=held;r_saved=visible_q;advance=0;rom_fixture[address]=rom_fixture[address]^16'h4321;#15;commit();
       if(held!==saved||visible_q!==r_saved)$fatal(1,"R latch hold changed closed state");
       advance=1;closed_holds++;
     end
   end
   if(handoffs!=1024||late_updates!=1024||closed_holds!=1024)$fatal(1,"R latch handoff coverage incomplete");
   $display("R_OPERAND_LATCH PASS checks=%0d handoffs=%0d late_updates=%0d held_edges=%0d Qs=48 RAM_read_addresses=1",checks,handoffs,late_updates,closed_holds);$finish;
 end
endmodule
'''


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(out,engine,mutation):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'inputs';hashes={}
    for n in ['verilog/rtl/dorado_r_read_window_step.sv','verilog/rtl/dorado_r_operand_latches.sv','verilog/verilator/t_file_original_storage.sv','tools/test_r_operand_latches.py']:
        p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=sha(p)
    candidate=out/'candidate.sv';raw=(inputs/'verilog/rtl/dorado_r_operand_latches.sv').read_text()
    edits={
      'registered_visible':('visible_gate[half] ? held_r[8*half+:8] : selected[8*half+:8]','held_r[8*half+:8]'),
      'wrong_visible_close':('held_r[8*half+:8]<=visible_q[8*half+:8]','held_r[8*half+:8]<=~visible_q[8*half+:8]')}
    if mutation:
        a,b=edits[mutation];assert raw.count(a)==1;raw=raw.replace(a,b,1)
    candidate.write_text(raw);tb=out/'tb.sv';tb.write_text(TB)
    support=[candidate,tb,inputs/'verilog/rtl/dorado_r_read_window_step.sv',inputs/'verilog/verilator/t_file_original_storage.sv']
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),engine=engine,mutation=mutation,inputs=hashes,actions=[],candidate_sha256=sha(candidate),testbench_sha256=sha(tb),native_integrated=False,physical_timing_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    commands=[['iverilog','-g2012','-s','tb_r_operand_latches','-o',str(out/'test.vvp'),*map(str,support)],['vvp',str(out/'test.vvp')]]if engine=='icarus'else[
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_operand_latches','--Mdir',str(out/'obj'),'-o','test',*map(str,support)], [str(out/'obj/test')]]
    save();rc=0
    for i,cmd in enumerate(commands):
        with(out/('compile.log'if i==0 else'test.log')).open('w')as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);row=dict(command=cmd,pid=p.pid,started=time.time());state['actions'].append(row);save();rc=p.wait();row.update(returncode=rc,finished=time.time());save()
        if rc:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else'';m=re.search(r'R_OPERAND_LATCH PASS ([^\n]+)',log)
    state.update(status='PASS_R_OPERAND_LATCHES'if not rc and m else'R_OPERAND_LATCHES_FAILED',summary=m[1]if m else None,finished=time.time());save();print(state['status'],flush=True);return rc if rc else(0 if m else 1)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--engine',choices=['icarus','verilator'],default='icarus')
    p.add_argument('--mutation',choices=['registered_visible','wrong_visible_close']);a=p.parse_args();sys.exit(run(a.output.resolve(),a.engine,a.mutation))
