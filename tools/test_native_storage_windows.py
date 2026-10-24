#!/usr/bin/env python3
"""Exercise exact passive observer with independent timed raw/boot stimuli.

This test validates observation and accounting, not the native machine. Both
simulators must capture three complete windows and one explicit reset abort,
including final capped ACK, held first reply, trigger delay and same-edge work.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import time

from report_native_storage_windows import parse_windows

ROOT=Path(__file__).resolve().parents[1]
TB=r'''
`timescale 1ns/1ps
module window_backend_stub;
 reg [3:0] state=2;reg refresh_due=0,lookup_hit=0;
 reg [25:0] address=0;reg [2:0] fill_beat=0;
 reg [7:0] open=0;reg [12:0] rows[0:7];
 initial for(integer i=0;i<8;i++)rows[i]=0;
endmodule
module window_dut_stub;
 reg rv=0,rr=0,rw=0,sv=0,sr=0;reg [22:0] ra=0;reg [15:0] wd=0,sd=0;
 reg [127:0] valid=0,accept=0;
 window_backend_stub u_sdram();
endmodule
module tb_native_windows;
 reg host_clk=0;always #5 host_clk=~host_clk;
 reg reset=0,storage_initialized=1,step=1,pack_ready=1,storage_ready=1;
 reg [4:0] instruction_signals=0;reg [31:0] cpu_debug=32'h30221000;
 longint unsigned host_ticks=0;always @(negedge host_clk)host_ticks++;
 integer bfn=0;
 function automatic integer native_boot_request();native_boot_request=bfn;endfunction
 window_dut_stub dut();
 OBSERVER
 task automatic control(input integer epoch,trigger);
  integer f;
  begin @(negedge host_clk);f=$fopen(window_control_path,"w");$fwrite(f,"%0d %0d\n",epoch,trigger);$fclose(f);@(negedge host_clk);end
 endtask
 task automatic offer(input integer address,writing,value,kind);
  begin
   @(negedge host_clk);dut.ra=23'(address);dut.rw=1'(writing);dut.wd=16'(value);dut.rv=1;dut.rr=1;dut.valid=1;
   dut.u_sdram.lookup_hit=kind==0;dut.u_sdram.state=kind>=1 && kind<=3 ? 6 : 2;
   dut.u_sdram.address={3'b0,23'(address&~7)};dut.u_sdram.fill_beat=kind==1 ? 7 : kind==2 ? 3'(address&7) : 0;
   @(negedge host_clk);dut.rv=0;dut.rr=0;
  end
 endtask
 task automatic reply(input integer value,hold);
  begin
   @(negedge host_clk);dut.sd=16'(value);dut.sv=1;dut.sr=0;
   repeat(hold)@(negedge host_clk);
   dut.sr=1;@(negedge host_clk);dut.sv=0;dut.sr=0;
  end
 endtask
 initial begin
  #1;control(1,2);
  // Power-up work must be invisible;113 arrives while an older read is active.
  bfn=113;offer(0,0,0,4);
  if(window_open)$fatal(1,"WINDOW_TEST decimal113 incorrectly treated as113B");
  // Independent packet oracle: the original TX's sixth word is decimal75.
  bfn=75;
  if(window_open)$fatal(1,"WINDOW_TEST opened before next accepted request");
  reply(0,0);
  offer(129,0,0,0);
  // Coherent host categories and actual raw stall categories are distinct.
  step=0;pack_ready=0;storage_ready=0;@(negedge host_clk);
  storage_ready=1;@(negedge host_clk);pack_ready=1;storage_ready=0;@(negedge host_clk);
  storage_ready=1;@(negedge host_clk);step=1;
  dut.rv=1;dut.u_sdram.state=6;@(negedge host_clk);
  dut.u_sdram.state=8;@(negedge host_clk);
  dut.u_sdram.state=7;@(negedge host_clk);
  dut.u_sdram.state=3;@(negedge host_clk);dut.rv=0;
  reply(16'h456,2);
  offer(258,1,16'h789,5);reply(0,0);
  offer(387,0,0,3);reply(16'hdef,0);
  if(window_open || window_replies!=3)$fatal(1,"WINDOW_TEST final capped ACK missing");
  offer(400,0,0,4);reply(0,0);if(window_count!=3)$fatal(1,"WINDOW_TEST cap overflow");
  control(2,1);bfn=0;
  offer(8,0,0,1);reply(0,0);if(window_id!=1)$fatal(1,"WINDOW_TEST first-issue trigger ignored");
  instruction_signals=16;@(negedge host_clk);instruction_signals=0;
  offer(8,0,0,1);reply(1,0);
  offer(11,0,0,2);reply(2,0);
  dut.u_sdram.open=1;dut.u_sdram.rows[0]=0;
  offer(16,0,0,4);reply(3,0);
  if(window_open || window_id!=2)$fatal(1,"WINDOW_TEST second epoch incomplete");
  control(3,0);dut.u_sdram.open=0;offer(19,1,16'habc,5);
  @(negedge host_clk);reset=1;storage_initialized=0;@(negedge host_clk);reset=0;storage_initialized=1;
  if(window_open)$fatal(1,"WINDOW_TEST reset did not abort observation");
  control(4,0);dut.u_sdram.open=1;dut.u_sdram.rows[0]=1;
  offer(23,1,16'hcc0,5);reply(0,0);
  dut.u_sdram.rows[0]=0;offer(24,1,16'hcc1,5);
  // Consume and replace ownership on the SAME edge.
  @(negedge host_clk);dut.sv=1;dut.sr=1;dut.ra=25;dut.rw=0;dut.rv=1;dut.rr=1;dut.wd=0;
  dut.u_sdram.open=0;dut.u_sdram.state=2;dut.u_sdram.lookup_hit=0;
  @(negedge host_clk);dut.sv=0;dut.sr=0;dut.rv=0;dut.rr=0;
  reply(16'hcca,0);
  if(window_open || window_id!=4 || window_replies!=3)$fatal(1,"WINDOW_TEST final epoch incomplete");
  repeat(3)@(negedge host_clk);
  $display("WINDOW_TEST PASS epochs=4 complete=3 aborted=1");$finish;
 end
 initial begin #20000;$fatal(1,"WINDOW_TEST deadline");end
endmodule
'''


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(out,control_pauses=False):
    assert not out.exists(),'Preserve existing observation test'
    out.mkdir(parents=True);inputs=out/'inputs';inputs.mkdir()
    observer=ROOT/'verilog/verilator/native_cedar/storage_windows.sv.inc'
    harness=ROOT/'verilog/verilator/native_cedar/tb_native_cedar.sv'
    for p in [observer,harness,Path(__file__),ROOT/'tools/report_native_storage_windows.py',ROOT/'tools/report_native_storage_traffic.py']:
        shutil.copyfile(p,inputs/p.name)
    body=observer.read_text();tb=TB
    if control_pauses:
        from prepare_cp_native_pause import extend_window_gate
        body=extend_window_gate(body)
        tb=tb.replace('integer bfn=0;', 'reg control_ready=1;integer bfn=0;',1)
        tb=tb.replace('storage_ready=1;@(negedge host_clk);step=1;',
            'storage_ready=1;@(negedge host_clk);control_ready=0;repeat(3)@(negedge host_clk);control_ready=1;@(negedge host_clk);step=1;',1)
        shutil.copyfile(ROOT/'tools/prepare_cp_native_pause.py',inputs/'prepare_cp_native_pause.py')
    (inputs/'tb.sv').write_text(tb.replace('OBSERVER',body))
    text=harness.read_text();start=text.index('    wait(!reset);repeat(3)@(negedge host_clk);')
    end=text.index('\n  end\n  wire [12:0] a;',start);release=text[start:end]
    assert release.count("while(native_boot_request()!=32'o113)")==1
    release_tb='''`timescale 1ns/1ps
module tb_c_release;
 reg host_clk=0;always #5 host_clk=~host_clk;
 reg reset=1;reg [10:0] ps2_key=0;integer bfn=0;
 integer host_ticks=0,guest_ticks=0;
 always @(negedge host_clk)begin host_ticks++;guest_ticks++;end
 function automatic integer native_boot_request();native_boot_request=bfn;endfunction
 initial begin repeat(3)@(negedge host_clk);reset=0;end
 initial begin\n'''+release+'''
 end
 initial begin
  wait(ps2_key==11'h621);bfn=113;
  repeat(1100)@(negedge host_clk);
  if(ps2_key!=11'h621)$fatal(1,"RELEASE_TEST C released for decimal113");
  bfn=75; // captured original113B packet, independently expressed in decimal
  repeat(1100)@(negedge host_clk);
  if(ps2_key!=11'h021)$fatal(1,"RELEASE_TEST real113B failed to release C");
  $display("RELEASE_TEST PASS decimal113 rejected real113B releases C");$finish;
 end
endmodule
'''
    (inputs/'release.sv').write_text(release_tb)
    (inputs/'old-decimal-release.sv').write_text(release_tb.replace("while(native_boot_request()!=32'o113)","while(native_boot_request()!=0113)"))
    frozen={p.name:sha(p)for p in inputs.iterdir()};actions=[];results={};release_results={}
    def execute(command,label):
        command=list(map(str,command))
        with(out/(label+'.log')).open('x')as f:
            p=subprocess.Popen(command,cwd=inputs,stdout=f,stderr=subprocess.STDOUT)
            row=dict(command=command,pid=p.pid,started=time.time());actions.append(row);rc=p.wait();row.update(returncode=rc,finished=time.time())
        return rc
    try:
        for engine in ['icarus','verilator']:
            if engine=='icarus':
                obj=out/'test.vvp';cmd=['iverilog','-g2012','-s','tb_native_windows','-o',obj,inputs/'tb.sv'];exe=['vvp',obj]
            else:
                obj=out/'obj';cmd=['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_native_windows','--Mdir',obj,'-o','test',inputs/'tb.sv'];exe=[obj/'test']
            assert execute(cmd,engine+'-compile')==0,engine
            assert execute([*exe,'+window-control='+str(out/(engine+'-window.txt')),'+window-limit=3','+window-poll=1'],engine+'-test')==0,engine
            log=(out/(engine+'-test.log')).read_bytes();r=parse_windows(log)
            assert [w['complete']for w in r['windows'].values()]==[True,True,False,True]
            assert r['windows'][1]['firmware_trigger_observed']
            if control_pauses:
                assert all(set(w['gate'])==set(range(8)) for w in r['windows'].values())
                assert int(r['windows'][1]['gate'][7]['ticks'])==3
            assert r['windows'][3]['footer']['status']=='ABORT' and r['windows'][3]['trace']['unpaired_request']
            assert sum(int(w['read_class'][5]['admitted'])for w in r['windows'].values())==4
            results[engine]=r;print(engine,'WINDOW_TEST PASS',flush=True)
            for label,bad in [('release',False),('old-decimal-release',True)]:
                source=inputs/(label+'.sv');target=out/(engine+'-'+label)
                if engine=='icarus':
                    cmd=['iverilog','-g2012','-s','tb_c_release','-o',target,source];exe=['vvp',target]
                else:
                    cmd=['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_c_release','--Mdir',target,'-o','test',source];exe=[target/'test']
                assert execute(cmd,engine+'-'+label+'-compile')==0
                rc=execute(exe,engine+'-'+label+'-test');log=(out/(engine+'-'+label+'-test.log')).read_text()
                if bad:assert rc!=0 and 'RELEASE_TEST C released for decimal113'in log
                else:assert rc==0 and 'RELEASE_TEST PASS'in log
                release_results[engine+'-'+label]=dict(returncode=rc,old_decimal_error_rejected=bad)
        assert results['icarus']==results['verilator'],'Observation differs between simulators'
        assert all(sha(inputs/n)==h for n,h in frozen.items())
        state=dict(status='PASS_PASSIVE_NATIVE_WINDOW_OBSERVER_TWO_SIMULATORS',inputs=frozen,actions=actions,results=results,release_results=release_results,
                   control_pause_schema_tested=control_pauses,native_firmware_or_speed_qualified=False)
    except BaseException as e:
        state=dict(status='TERMINAL_FAILED',error=repr(e),actions=actions,results=results);raise
    finally:(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--control-pauses',action='store_true')
    a=p.parse_args();run(a.output.resolve(),a.control_pauses)
