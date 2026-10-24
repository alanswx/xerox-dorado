#!/usr/bin/env python3
"""Observe an exact frozen native BaseBoard through its zero-page clear.

This isolates the board from the rest of the machine, with self-returned
backplane outputs and idle external inputs. It is a startup diagnostic, not
native firmware/CPU/storage qualification. No live run or source is changed.
"""
import argparse
import gzip
import json
import os
from pathlib import Path
import re
import subprocess
import time

from test_r_front_forward_view import sha


BOARD = 'src/build/rtl-executive/native16-tx-cable/BaseBd_nominal.v'


def fixture(board, sysper, gated, through_midas, through_cp, through_watchdog, finish_cp):
    header = board[:board.index(');')]
    ports = re.findall(r'^\s*(input|output)\s+(?:wire\s+)?(\[[^]]+\]\s*)?(\w+)\s*[,)]?', header, re.M)
    outputs = {n for d, w, n in ports if d == 'output'}
    wires = [f'wire {w or ""}{n};' for d, w, n in ports if d == 'output']
    connections = []
    environment = {}
    for direction, width, name in ports:
        if direction == 'output': value = name
        elif name == 'sys_clk': value = 'sys_clk'
        elif name == 'baseboard_boot_n': value = "1'b1"
        elif name + '__drv' in outputs: value = name + '__drv'
        elif name == 'CLK_InBase': value = 'CLK_OutBase_p___drv'
        elif name.startswith('ACP') and 'Gnd' not in name: value = "1'b1"
        elif name in ['OISData_p_', 'XmtData_p_']: value = "1'b1"
        else: value = "1'b0"
        connections.append(f'.{name}({value})')
        if direction == 'input': environment[name] = value
    return '''`timescale 1ns/1ps
`default_nettype none
module tb_native_baseboard_reset;
''' + ('''reg host_clk=0; always #8.75 host_clk=~host_clk;
reg [15:0] gate_pattern=16'h317a;
reg step=0;
always @(negedge host_clk)begin
 gate_pattern<={gate_pattern[14:0],gate_pattern[15]^gate_pattern[13]^gate_pattern[12]^gate_pattern[10]};
 step<=gate_pattern[0]|gate_pattern[3];
end
wire sys_clk=host_clk && step;
''' if gated else 'reg sys_clk=0; always #8.75 sys_clk=~sys_clk;\n') + '\n'.join(wires) + f'''
BaseBd_m_Rev_m_Am_nominal #(.SYSPER({sysper})) bb (
''' + ',\n'.join(connections) + ''');
longint unsigned ticks=0, phases=0, fetches=0, stores=0, dex=0;
longint unsigned last_rise=0, shortest_period=64'hffffffffffffffff, longest_period=0;
reg previous_phase=0;
integer age=0;
wire [15:0] address=bb.u_f61.ab;
wire [7:0] data_in=bb.u_f61.dbi;
wire [7:0] x=bb.u_f61.u_cpu.logic_00.x;
wire [15:0] pc=bb.u_f61.u_cpu.logic_00.pc;
reg saw_reset=0,clear_done=0,midas_called=0,midas_returned=0,cp_previous=1;
reg reset_previous=0,watchdog_previous=0,watchdog_released=0;
longint unsigned reset_pulses=0,boot_stores=0,boot_dex=0,boots=0;
localparam THROUGH_MIDAS=THROUGH_MIDAS_VALUE;
localparam THROUGH_CP=THROUGH_CP_VALUE;
localparam THROUGH_WATCHDOG=THROUGH_WATCHDOG_VALUE;
localparam FINISH_CP=FINISH_CP_VALUE;
localparam longint unsigned TICK_BOUND=THROUGH_WATCHDOG ? 160000000 : 3000000;
always @(negedge sys_clk) begin
 ticks=ticks+1;
 if(bb.MCClk && !previous_phase)begin
  if(phases>0)begin
   if(ticks-last_rise<shortest_period)shortest_period=ticks-last_rise;
   if(ticks-last_rise>longest_period)longest_period=ticks-last_rise;
  end
  phases=phases+1;last_rise=ticks;age=0;
 end else age=age+1;
 previous_phase=bb.MCClk;
 if(bb.u_g22.qa && !watchdog_previous)begin
  watchdog_released=1;
  $display("BB_WATCHDOG_FIRST_PHASE ticks=%0d phases=%0d counter=%0d resets=%0d in=%b out=%b",ticks,phases,bb.u_g21.cnt,reset_pulses,bb.WatchdogIn,bb.WatchdogOut);
  if(bb.u_g21.cnt!=1048576)$fatal(1,"watchdog phase did not follow original Q21 boundary");
 end
 watchdog_previous=bb.u_g22.qa;
 if(!bb.MCReset_p_ && reset_previous)begin
  reset_pulses=reset_pulses+1;
  if(reset_pulses<=2)$display("BB_WATCHDOG_RESET ticks=%0d pc=%h counter=%0d in=%b out=%b gate=%b boot_n=%b pwr_good=%b",ticks,pc,bb.u_g21.cnt,bb.WatchdogIn,bb.WatchdogOut,bb.u_g22.p6,BootMC_p___drv,bb.PwrGood);
  if(THROUGH_WATCHDOG && (watchdog_released || BootMC_p___drv || !bb.PwrGood || !(bb.WatchdogIn^bb.WatchdogOut) || !bb.u_g22.p6))
   $fatal(1,"reset has a cause other than initial watchdog mismatch");
 end
 reset_previous=bb.MCReset_p_;
 if(THROUGH_CP && midas_returned && CPStrb_p___drv && !cp_previous && (!THROUGH_WATCHDOG || watchdog_released) &&
    (!FINISH_CP || (pc>=16'hf8a5 && pc<=16'hf8ab)))begin
  $display("BB_RESET_FIRST_CP_WRITE ticks=%0d phases=%0d fetches=%0d pc=%h bank_n=%b resets=%0d",ticks,phases,fetches,pc,{CPAddr_0_p___drv,CPAddr_1_p___drv,CPAddr_2_p___drv},reset_pulses);
  if(THROUGH_WATCHDOG && (!watchdog_released || reset_pulses==0))$fatal(1,"CP progress did not follow the observed watchdog startup phase");
  if(FINISH_CP)$display("BB_RESET_ROM_FINISH_CP ticks=%0d pc=%h data=%b",ticks,pc,
   {CPOut_0__drv,CPOut_1__drv,CPOut_2__drv,CPOut_3__drv,CPOut_4__drv,CPOut_5__drv,CPOut_6__drv,CPOut_7__drv,CPOut_8__drv});
  $finish;
 end
 cp_previous=CPStrb_p___drv;
 if(bb.MCClk && age==20)begin
  if(!bb.u_f61.rw)stores=stores+1;
  if(bb.u_f61.sync)begin
   fetches=fetches+1;
   if(address==16'hf3a7)begin
    saw_reset=1;boot_stores=stores;boot_dex=dex;boots=boots+1;
    if(THROUGH_WATCHDOG)begin clear_done=0;midas_called=0;midas_returned=0;end
   end
   if(address==16'hf3ae)dex=dex+1;
   if(fetches<=32 || (!THROUGH_WATCHDOG && ((address==16'hf3ae && (dex%64)==0) || (clear_done && (fetches<900 || THROUGH_CP)))) ||
      (THROUGH_WATCHDOG && boots==1 && (address==16'hf3eb || address==16'hf3c3)))
    $display("BB_RESET_FETCH tick=%0d phase=%0d address=%h data=%h pc=%h x=%h a=%h reset_n=%b stores=%0d dex=%0d",
     ticks,phases,address,data_in,pc,x,bb.u_f61.u_cpu.logic_00.a,bb.MCReset_p_,stores,dex);
   if(saw_reset && address==16'hf3b1)begin
    if(!THROUGH_WATCHDOG || boots==1)
     $display("BB_RESET_CLEAR_EXIT ticks=%0d phases=%0d fetches=%0d stores=%0d dex=%0d x=%h period_min=%0d period_max=%0d",
      ticks,phases,fetches,stores,dex,x,shortest_period,longest_period);
    if(stores-boot_stores!=256 || dex-boot_dex!=256 || x!=0)$fatal(1,"incorrect reset clear loop");
    clear_done=1;
    if(!THROUGH_MIDAS)$finish;
   end
   if(clear_done && address==16'hf248)midas_called=1;
   if(clear_done && address==16'hf3c3)begin
    if(!midas_called)$fatal(1,"no MidasSetup call");
    if(!THROUGH_WATCHDOG || boots==1)
     $display("BB_RESET_MIDAS_RETURN ticks=%0d phases=%0d fetches=%0d stores=%0d x=%h",ticks,phases,fetches,stores,x);
    midas_returned=1;
    if(!THROUGH_CP)$finish;
   end
   if(!THROUGH_WATCHDOG && clear_done && address==16'hf3a7)begin
    $fatal(1,"unexpected second Reset before MidasSetup returned");
   end
  end
 end
 if(ticks>=TICK_BOUND)begin
  $display("BB_RESET_BOUND ticks=%0d phases=%0d fetches=%0d stores=%0d dex=%0d pc=%h x=%h period_min=%0d period_max=%0d",
   ticks,phases,fetches,stores,dex,pc,x,shortest_period,longest_period);
  $fatal(1,"BaseBoard did not exit zero-page clear within diagnostic bound");
 end
end
endmodule
`default_nettype wire
'''.replace('THROUGH_MIDAS_VALUE',str(int(through_midas))).replace('THROUGH_CP_VALUE',str(int(through_cp))).replace('THROUGH_WATCHDOG_VALUE',str(int(through_watchdog))).replace('FINISH_CP_VALUE',str(int(finish_cp))), environment


def run(source, out, sysper, gated, through_midas, through_cp, through_watchdog, finish_cp):
    assert not out.exists()
    r = json.loads(gzip.decompress((source/'provenance.json.gz').read_bytes()))
    out.mkdir(parents=True)
    inputs = out/'inputs'
    hashes = {}
    for name, h in r['inputs'].items():
        p = source/'inputs'/name
        assert sha(p) == h, name
        dest = inputs/name
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_bytes(p.read_bytes())
        hashes[name] = h
    bench, environment = fixture((inputs/BOARD).read_text(), sysper, gated, through_midas, through_cp, through_watchdog, finish_cp)
    tb = out/'tb_native_baseboard_reset.sv'
    tb.write_text(bench)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    command = ['verilator','--binary','--timing','--autoflush','--top-module','tb_native_baseboard_reset',
               '-j','2','-Wno-fatal','-Wno-PINMISSING','-Wno-TIMESCALEMOD',
               '-Iverilog/cells','-Iverilog/generated','-Iverilog/vendor/6502','-Iverilog/vendor/riot',
               *['-D'+d for d in r['defines']], '--Mdir',str(out/'obj'),'-o','test',
               *[str(inputs/n) for n in r['used_hdl']], str(tb)]
    state = dict(status='RUNNING',pid=os.getpid(),started=time.time(),source=str(source),sysper=sysper,
                 pseudo_random_clock_barrier=gated,
                 through_first_midas_return=through_midas,
                 through_first_CP_write=through_cp,
                 through_original_watchdog_startup=through_watchdog,
                 require_original_FinishCP_instructions=finish_cp,
                 source_provenance_sha256=sha(source/'provenance.json.gz'),inputs=hashes,
                 fixture_sha256=sha(tb),external_inputs=environment,actions=[],
                 original_board_and_cells_unchanged=True,original_ROM_unchanged=True,
                 live_runs_changed=False,native_machine_qualified=False,hardware_changed=False)
    def save(): (out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for name, cmd in [('compile',command),('test',[str(out/'obj/test')])]:
        with (out/(name+'.log')).open('x') as log:
            child = subprocess.Popen(cmd,cwd=inputs/'src',stdout=log,stderr=subprocess.STDOUT)
            a = dict(stage=name,command=cmd,pid=child.pid,started=time.time())
            state['actions'].append(a);save()
            rc = child.wait();a.update(returncode=rc,finished=time.time());save()
        if rc: break
    log = (out/'test.log').read_text() if (out/'test.log').exists() else ''
    marker='BB_RESET_ROM_FINISH_CP' if finish_cp else ('BB_RESET_FIRST_CP_WRITE' if through_cp else ('BB_RESET_MIDAS_RETURN' if through_midas else 'BB_RESET_CLEAR_EXIT'))
    state.update(status='PASS_ISOLATED_BASEBOARD_RESET_CLEAR' if not rc and marker in log
                 else 'ISOLATED_BASEBOARD_RESET_CLEAR_FAILED',finished=time.time())
    save();print(state['status'],flush=True)
    return rc


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--sysper',type=int,choices=[4,16],default=4)
    p.add_argument('--gated',action='store_true',help='Pause board clock with a deterministic host-domain pattern')
    p.add_argument('--through-midas',action='store_true',help='Also require the first JSR/RTS through original MidasSetup')
    p.add_argument('--through-cp',action='store_true',help='Trace original startup through its first CP write')
    p.add_argument('--through-watchdog',action='store_true',help='Observe full original watchdog startup and require subsequent CP progress')
    p.add_argument('--finish-cp',action='store_true',help='Require the strobe to occur in original FinishCP at F8A5..F8AB')
    a = p.parse_args()
    assert not a.through_watchdog or a.sysper==4,'This bound is for the actual native SYSPER4 ratio'
    assert not a.finish_cp or a.through_watchdog,'Use full unchanged watchdog startup for the FinishCP observation'
    raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.sysper,a.gated,
                        a.through_midas or a.through_cp or a.through_watchdog,
                        a.through_cp or a.through_watchdog,a.through_watchdog,a.finish_cp))
