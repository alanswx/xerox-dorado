#!/usr/bin/env python3
"""Compare explicit RBase closing capture with the original legal RAM waveform.

Every word/nibble is written with address held15ns after close, minimum30ns
write pulses and data stable15ns before close. The next address may already
be present at the transfer API, while the physical mux switches after close.
The original zero-delay RAM model is unchanged; no invalid overlap is filtered.
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
module tb_rbase_window;
 reg [63:0] q=64'h0123456789abcdef,fixture=64'h0123456789abcdef;
 reg fixture_load=0,wb=1,wa=1,physical_we=1;
 reg [3:0] ab=0,aa=0,db=0,da=0,physical_address=0,physical_data=0;
 wire [63:0] n,reference;wire write_event,closing_event;wire [3:0] rd;
 integer checks=0,closes=0,changed_close_addresses=0,open_updates=0;
 reg monitor=0;realtime address_changed=0,data_changed=0,opened=0,closed=-1000;
 integer valid_write_pulses=0;
 // Independent waveform checks against the specified F10145A limits. The
 // reference storage model itself remains unchanged and unfiltered.
 always @(physical_address)begin
  if(monitor && (!physical_we || $realtime-closed<1.0))$fatal(1,"F10145A address changed inside write/hold interval");
  address_changed=$realtime;
 end
 always @(physical_data)data_changed=$realtime;
 always @(negedge physical_we)if(monitor)begin
  if($realtime-address_changed<3.5)$fatal(1,"F10145A address setup before write assertion");
  opened=$realtime;
 end
 always @(posedge physical_we)if(monitor)begin
  if($realtime-opened<4.0 || $realtime-data_changed<4.5)$fatal(1,"F10145A write width/data setup");
  closed=$realtime;valid_write_pulses++;
 end
 dorado_rbase_window_step dut(q,wb,wa,ab,aa,db,da,n,write_event,closing_event);
 original_F10145A original(physical_address[3],physical_address[2],physical_address[1],physical_address[0],
  physical_data[0],physical_data[1],physical_data[2],physical_data[3],1'b0,physical_we,1'b0,
  rd[0],rd[1],rd[2],rd[3],fixture_load,fixture,reference);
 task automatic compare(input [63:0] expected);
  begin
   if(reference!==expected)$fatal(1,"RBASE_WINDOW mismatch check=%0d close=%b old_address=%h arrived_address=%h actual=%h expected=%h",checks,closing_event,ab,aa,reference,expected);
   q=expected;checks++;
  end
 endtask
 initial begin
  #15;fixture_load=1;#15;fixture_load=0;#15;
  if(reference!==q)$fatal(1,"RBase fixture mismatch");monitor=1;
  for(integer word_no=0;word_no<16;word_no++)for(integer nibble=0;nibble<16;nibble++)begin
   // Establish source address/data while Write Enable remains closed.
   physical_address=4'(word_no);physical_data=4'(nibble);#15;
   wb=1;wa=0;ab=physical_address;aa=physical_address;db=physical_data;da=physical_data;#1;
   if(!write_event||closing_event)$fatal(1,"RBase opening event flags");
   begin reg [63:0] expected;expected=n;physical_we=0;#15;compare(expected);end
   // g14's LOW-transparent data may settle during the open window; retain
   // the same address and allow15ns data setup before closing.
   wb=0;wa=0;db=physical_data;da=4'(nibble^15);#1;
   begin reg [63:0] expected;expected=n;physical_data=da;#15;compare(expected);end
   open_updates++;
   // A final g14 data value can settle between native sampling events.
   // Do not pre-commit this value to q: closing must capture it itself.
   physical_data=4'(nibble^10);#15;
   // Model one native capture: close the old write, then expose Next.
   // Giving the API a different next address catches an incorrect close
   // that writes the next task's entry rather than the just-finished one.
   wb=0;wa=1;ab=physical_address;db=physical_data;
   aa=4'((word_no+1)%16);da=4'(nibble^5);#1;
   if(!write_event||!closing_event)$fatal(1,"RBase closing event flags");
   begin reg [63:0] expected;expected=n;physical_we=1;#15;
    physical_address=aa;physical_data=da;#15;compare(expected);
   end
   closes++;if(ab!=aa)changed_close_addresses++;
   wb=1;wa=1;ab=aa;db=da;#1;
   if(write_event||closing_event||n!==q)$fatal(1,"RBase closed window changed storage");
  end
  if(closes!=256||changed_close_addresses!=256||open_updates!=256||valid_write_pulses!=256)$fatal(1,"RBase write coverage incomplete");
  $display("RBASE_WINDOW_LEGAL_WRITES pulses=%0d address_setup_min=3.5 address_hold_min=1 write_width_min=4 data_setup_min=4.5",valid_write_pulses);
  $display("RBASE_WINDOW PASS checks=%0d closes=%0d different_next_address=%0d open_data_updates=%0d",checks,closes,changed_close_addresses,open_updates);$finish;
 end
endmodule
'''


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(out,engine,wrong_close,omit_close=False):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'inputs';inputs.mkdir()
    sources=['verilog/rtl/dorado_rbase_window_step.sv','verilog/verilator/t_file_original_storage.sv','tools/test_rbase_window.py']
    hashes={}
    for name in sources:
        p=inputs/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/name).read_bytes());hashes[name]=sha(p)
    candidate=out/'candidate.sv';raw=(inputs/sources[0]).read_text()
    if wrong_close:
        old='else if(closing_event)state_next[4*address_before+:4]=data_before;';assert raw.count(old)==1
        raw=raw.replace(old,'else if(closing_event)state_next[4*address_after+:4]=data_before;')
    if omit_close:
        old='else if(closing_event)state_next[4*address_before+:4]=data_before;';assert raw.count(old)==1
        raw=raw.replace(old,'')
    candidate.write_text(raw);tb=out/'tb.sv';tb.write_text(TB)
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),actions=[],engine=engine,wrong_close=wrong_close,omit_close=omit_close,
               inputs=hashes,candidate_sha256=sha(candidate),testbench_sha256=sha(tb),native_integrated=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    def execute(cmd,log):
        with log.open('w')as f:
            p=subprocess.Popen(list(map(str,cmd)),stdout=f,stderr=subprocess.STDOUT)
            row=dict(command=list(map(str,cmd)),pid=p.pid,started=time.time());state['actions'].append(row);save();rc=p.wait()
            row.update(returncode=rc,finished=time.time());save();return rc
    support=[candidate,tb,inputs/sources[1]]
    cmd=['iverilog','-g2012','-s','tb_rbase_window','-o',out/'test.vvp',*support]if engine=='icarus'else[
        'verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_rbase_window','--Mdir',out/'obj','-o','test',*support]
    rc=execute(cmd,out/'compile.log')
    if not rc:rc=execute(['vvp','-i',out/'test.vvp']if engine=='icarus'else[out/'obj/test'],out/'test.log')
    log=(out/'test.log').read_text()if(out/'test.log').exists()else'';summary=re.search(r'^RBASE_WINDOW PASS (.*)$',log,re.M)
    state.update(status='PASS_RBASE_WINDOW'if not rc and summary else'RBASE_WINDOW_FAILED',summary=summary[1]if summary else None,finished=time.time())
    save();print(state['status']);return rc or(0 if summary else 1)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],default='icarus');p.add_argument('--wrong-close',action='store_true')
    p.add_argument('--omit-close',action='store_true')
    a=p.parse_args();assert not(a.wrong_close and a.omit_close)
    sys.exit(run(a.output.resolve(),a.engine,a.wrong_close,a.omit_close))
