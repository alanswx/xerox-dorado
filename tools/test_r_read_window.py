#!/usr/bin/env python3
"""Compare closed-state/live-view R latches against original MC10173 models.

Data can settle while open between sampling events. At close the next API
data differs, while the physical old data holds15ns across closure. This is
digital latch behavior only; actual RAM port/phase/STA integration is pending.
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
module tb_r_read_window;
 reg [31:0] q=32'h12345678,fixture=32'h12345678;
 reg load=0;reg [1:0] gb=3,ga=3,physical_gate=3;
 reg [31:0] db=0,da=0,physical_data=0;
 wire [31:0] n,vb,va,reference;wire [1:0] closes;
 integer checks=0,closing_halves=0,unclocked_updates=0,closed_changes=0;
 dorado_r_read_window_step dut(q,gb,ga,db,da,n,vb,va,closes);
 for(genvar nibble=0;nibble<8;nibble++)begin: original
   wire [3:0] d=physical_data[4*nibble+:4];
   original_MC10173 cell_latch(.p5(d[0]),.p3(d[1]),.p12(d[2]),.p10(d[3]),
     .p6(d[0]),.p4(d[1]),.p13(d[2]),.p11(d[3]),.p9(1'b0),.p7(physical_gate[(nibble/2)%2]),
     .fixture_load(load),.fixture_state(fixture[4*nibble+:4]),.state_q(reference[4*nibble+:4]));
 end
 task automatic compare_before;
   begin #2;if(vb!==reference)$fatal(1,"R_READ_WINDOW before view mismatch check=%0d",checks);end
 endtask
 task automatic apply_event;
   reg [31:0] held;reg [31:0] observed;
   begin
     #2;held=n;observed=va;
     physical_gate=ga;#15;physical_data=da;#15;
     if(reference!==observed)$fatal(1,"R_READ_WINDOW visible mismatch check=%0d gates=%b->%b expected=%h actual=%h",checks,gb,ga,observed,reference);
     q=held;gb=ga;db=da;#2;
     if(vb!==reference||va!==reference)$fatal(1,"R_READ_WINDOW commit view mismatch check=%0d",checks);
     checks++;
   end
 endtask
 initial begin
   #15;load=1;#15;load=0;#15;
   for(integer mode=0;mode<3;mode++)for(integer value=0;value<256;value++)begin
     // Open H, L, or both, with distinct bytes; no held-Q update is needed
     // for the combinational latch outputs to follow their real inputs.
     ga=mode==0?2'b10:mode==1?2'b01:2'b00;
     da={8'(value^8'h55),8'(value^8'haa),8'(value),8'(~value)};apply_event();
     // Update actual D without an API commit, exercising true transparency.
     physical_data=physical_data^32'ha55ac33c;#15;db=physical_data;da=db;#2;
     compare_before();unclocked_updates++;
     // A final value arrives after the previous sampling event, with ample
     // model setup. The next data/address epoch is different at the API.
     physical_data=physical_data^32'h39c6f00f;#15;db=physical_data;compare_before();
     ga=3;da=physical_data^32'hffffffff;#2;
     if(closes!==(~gb & ga))$fatal(1,"R_READ_WINDOW closing flags mismatch");
     closing_halves+=int'(closes[0])+int'(closes[1]);apply_event();
     // Changing D while closed must affect neither saved nor visible Q.
     da=da^32'h1234abcd;apply_event();closed_changes++;
   end
   if(closing_halves!=1024||unclocked_updates!=768||closed_changes!=768)$fatal(1,"R_READ_WINDOW coverage incomplete");
   $display("R_READ_WINDOW PASS checks=%0d closing_halves=%0d unclocked_updates=%0d closed_changes=%0d",checks,closing_halves,unclocked_updates,closed_changes);$finish;
 end
endmodule
'''


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(out,engine,mutation):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'inputs';hashes={}
    for n in ['verilog/rtl/dorado_r_read_window_step.sv','verilog/verilator/t_file_original_storage.sv','tools/test_r_read_window.py']:
        p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=sha(p)
    candidate=out/'candidate.sv';raw=(inputs/'verilog/rtl/dorado_r_read_window_step.sv').read_text()
    edits={
      'new_data_at_close':('closing_events[HALF] ? data_before[8*byte_no+:8]','closing_events[HALF] ? data_after[8*byte_no+:8]'),
      'omit_close':('closing_events[HALF] ? data_before[8*byte_no+:8]','closing_events[HALF] ? held_q[8*byte_no+:8]'),
      'hide_open_view':('gate_before[HALF] ? held_q[8*byte_no+:8] : data_before[8*byte_no+:8]','held_q[8*byte_no+:8]')}
    if mutation:
        a,b=edits[mutation];assert raw.count(a)==1;raw=raw.replace(a,b,1)
    candidate.write_text(raw);tb=out/'tb.sv';tb.write_text(TB)
    support=[candidate,tb,inputs/'verilog/verilator/t_file_original_storage.sv']
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),engine=engine,mutation=mutation,inputs=hashes,actions=[],candidate_sha256=sha(candidate),testbench_sha256=sha(tb),native_integrated=False,physical_timing_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    commands=[['iverilog','-g2012','-s','tb_r_read_window','-o',str(out/'test.vvp'),*map(str,support)],['vvp',str(out/'test.vvp')]]if engine=='icarus'else[
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_read_window','--Mdir',str(out/'obj'),'-o','test',*map(str,support)], [str(out/'obj/test')]]
    save();rc=0
    for i,cmd in enumerate(commands):
        with(out/('compile.log'if i==0 else'test.log')).open('w')as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);row=dict(command=cmd,pid=p.pid,started=time.time());state['actions'].append(row);save();rc=p.wait();row.update(returncode=rc,finished=time.time());save()
        if rc:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else'';m=re.search(r'R_READ_WINDOW PASS ([^\n]+)',log)
    state.update(status='PASS_R_READ_WINDOW'if not rc and m else'R_READ_WINDOW_FAILED',summary=m[1]if m else None,finished=time.time());save();print(state['status'],flush=True);return rc if rc else(0 if m else 1)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--engine',choices=['icarus','verilator'],default='icarus')
    p.add_argument('--mutation',choices=['new_data_at_close','omit_close','hide_open_view']);a=p.parse_args();sys.exit(run(a.output.resolve(),a.engine,a.mutation))
