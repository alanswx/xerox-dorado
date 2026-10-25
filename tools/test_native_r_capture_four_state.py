#!/usr/bin/env python3
"""Check native four-cell composition with X/Z clocks, selects, data and commit.

Fixture writes initialize the sixteen ORIGINAL cell Qs only in the testbench.
The production cells have no reset/init. Compare sequential original mux/latch
truth-table transfers, not a second synthesized folded implementation.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from generate_native_r_capture import generate,FOLD
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
TB='''`timescale 1ns/1ps
module tb;
 reg clk=0,commit=0,use_pair=0;reg [35:0] first_frame=0,last_frame=0;
 wire [35:0] frame=use_pair ? native_r_fold(first_frame,last_frame):first_frame;
 wire [15:0] visible;reg [15:0] expected,seed;
 dorado_native_r_capture cells(clk,commit,frame,visible);
 FOLD
 function automatic logic bit_value(input integer n);
  case(n%4)0:bit_value=0;1:bit_value=1;2:bit_value=1'bx;3:bit_value=1'bz;endcase
 endfunction
 function automatic [15:0] data_value(input integer n);
  case(n%8)
   0:data_value=16'h0000;1:data_value=16'hffff;2:data_value=16'h55aa;3:data_value=16'haa55;
   4:data_value=16'h8001;5:data_value=16'hx0fx;6:data_value=16'hz123;7:data_value=16'hzzzz;
  endcase
 endfunction
 function automatic [15:0] original_transfer(input [15:0] q,input [35:0] f);
  reg [15:0] n;
  begin
   n=q;
   for(integer half=0;half<2;half++)
    if(!f[34+half])n[8*half+:8]=f[32+half] ? f[16+8*half+:8]:f[8*half+:8];
   original_transfer=n;
  end
 endfunction
 integer checks=0;
 initial begin
  for(integer pair_mode=0;pair_mode<2;pair_mode++)
   for(integer clocks=0;clocks<16;clocks++)for(integer selects=0;selects<16;selects++)
    for(integer data=0;data<8;data++)for(integer enable_value=0;enable_value<4;enable_value++)begin
     clk=0;commit=0;use_pair=pair_mode[0];seed=16'(checks*71+16'h1265);
     cells.high.u_g06.q=seed[3:0];cells.high.u_g07.q=seed[7:4];
     cells.low.u_g06.q=seed[11:8];cells.low.u_g07.q=seed[15:12];
     first_frame={bit_value(clocks/4),bit_value(clocks),bit_value(selects/4),bit_value(selects),
                  data_value(data+1),data_value(data)};
     last_frame={bit_value(clocks/4),bit_value(clocks),bit_value(selects/4),bit_value(selects),
                 data_value(data+4),data_value(data+5)};
     // Each half sees EVERY ordered first/last clock and select pair. Swap
     // high/low assignments in last, so the2-bit fields vary independently.
     last_frame[35:34]={bit_value(clocks),bit_value(clocks/4)};
     last_frame[33:32]={bit_value(selects),bit_value(selects/4)};
     #2;commit=bit_value(enable_value);expected=seed;
     if(commit)expected=use_pair ? original_transfer(original_transfer(seed,first_frame),last_frame):
                                 original_transfer(seed,first_frame);
     #2;clk=1;#2;
     if(visible!==expected)$fatal(1,"NATIVE_R_FOUR_STATE check=%0d pair=%b commit=%b first=%h last=%h actual=%h expected=%h",checks,use_pair,commit,first_frame,last_frame,visible,expected);
     clk=0;#2;checks++;
    end
  if(checks!=16384)$fatal(1,"NATIVE_R_FOUR_STATE incomplete coverage");
  $display("NATIVE_R_FOUR_STATE_PASS checks=%0d clocks=16 select_pairs=16 data_pairs=8 commit_values=4 modes=2 native_bits=16",checks);
  $finish;
 end
endmodule
'''


def run(out,engine):
    assert engine=='icarus','Four-state qualification requires a four-state simulator.'
    assert not out.exists();out.mkdir(parents=True)
    bridge,cell,manifest=generate(ROOT)
    for n,text in [('bridge.sv',bridge),('cell.v',cell),('tb.sv',TB.replace('FOLD',FOLD))]:(out/n).write_text(text)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    s=dict(status='RUNNING',scope=__doc__,engine=engine,started=time.time(),actions=[],
           files={p.name:sha(p)for p in out.iterdir()},original_native_cell_sha256=manifest['native_cell_sha256'],
           actual_native_cell_bits=16,production_reset_or_init_added=False,analog_or_full_machine_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(s,indent=2)+'\n')
    paths=[str(out/n)for n in ['bridge.sv','cell.v','tb.sv']]
    commands=([['iverilog','-g2012','-s','tb','-o',str(out/'test.vvp'),*paths],['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else
              [['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb','--Mdir',str(out/'obj'),'-o','test',*paths],[str(out/'obj/test')]])
    save();code=1
    for label,cmd in zip(['compile','test'],commands):
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);r=dict(label=label,command=cmd,pid=child.pid,started=time.time());s['actions'].append(r);save();code=child.wait()
        r.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    passed=code==0 and 'NATIVE_R_FOUR_STATE_PASS checks=16384'in log
    s.update(status='PASS_NATIVE_R_FOUR_STATE_CAPTURE'if passed else'TERMINAL_NATIVE_R_FOUR_STATE_NOT_ESTABLISHED',finished=time.time())
    save();print(s['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus'],default='icarus')
    a=p.parse_args();raise SystemExit(run(a.output.resolve(),a.engine))
