#!/usr/bin/env python3
"""Freeze original-ROM native startup with the new shared control barrier.

Use the real complete staged machine, physical SDRAM model, original C/button
input and read-only medium. Add passive commit/closed-owner/SRAM/global-board
pause checks and held128-group request/response checks. No guest state import,
firmware patch, artificial CP stimulus, timing waiver or hardware deployment.
"""
import argparse
import gzip
import json
from pathlib import Path
import re

from run_r_front_control_native import prepare as prepare_base
from test_r_front_forward_view import sha
from test_r_front_task_event import replace

MONITOR='''
// Real native consumers, not replacement test registers. Observe the exact
// pre-commit interface, because forecasts are invalid while control_ready=0.
longint unsigned cp_native_commits=0,cp_native_pauses=0,cp_native_snapshots=0;
longint unsigned cp_native_pending=0,cp_native_replies=0,cp_native_held_requests=0,cp_native_held_responses=0;
reg [1229:0] cp_owner_snapshot;
reg [9215:0] cp_ram_snapshot;
reg [15:0] cp_bb_pc;
reg [127:0] cp_prev_valid=0,cp_prev_accept=0,cp_prev_write=0,cp_prev_response=0,cp_prev_response_ready=0;
reg [2047:0] cp_prev_addresses=0,cp_prev_wdata=0,cp_prev_rdata=0;
always @(posedge host_clk)begin
 if(!reset)begin
  if(step && !control_ready)$fatal(1,"NATIVE_CP late readiness change after falling-edge gate host=%0d",host_ticks);
  if(dut.m.r_front_evaluator.child.child.child.files.advance !== (step && control_ready))
   $fatal(1,"NATIVE_CP SRAM permission disagrees with whole-machine commit host=%0d",host_ticks);
  if(step)begin
   cp_native_commits++;
   if(cp_native_commits>16 && (dut.m.r_front_unsettled || |dut.m.r_front_repeated))
    $fatal(1,"NATIVE_CP invalid committed control forecast host=%0d",host_ticks);
  end
  if(|(cp_prev_valid & ~cp_prev_accept) || |(cp_prev_response & ~cp_prev_response_ready))
  for(integer g=0;g<128;g++)begin
   if(cp_prev_valid[g] && !cp_prev_accept[g])begin
    cp_native_held_requests++;
    if(!dut.valid[g] || dut.addresses[16*g+:16]!==cp_prev_addresses[16*g+:16] ||
       dut.wr[g]!==cp_prev_write[g] ||
       (cp_prev_write[g] && dut.wdata[16*g+:16]!==cp_prev_wdata[16*g+:16]))
      $fatal(1,"NATIVE_CP held request changed group=%0d host=%0d",g,host_ticks);
   end
   if(cp_prev_response[g] && !cp_prev_response_ready[g])begin
    cp_native_held_responses++;
    if(!dut.response[g] || dut.rdata[16*g+:16]!==cp_prev_rdata[16*g+:16])
      $fatal(1,"NATIVE_CP held response changed group=%0d host=%0d",g,host_ticks);
   end
  end
 end
 cp_prev_valid=reset?0:dut.valid;cp_prev_accept=dut.accept;cp_prev_write=dut.wr;
 cp_prev_addresses=dut.addresses;cp_prev_wdata=dut.wdata;
 cp_prev_response=reset?0:dut.response;cp_prev_response_ready=dut.response_ready;cp_prev_rdata=dut.rdata;
 if(!reset && storage_initialized && !control_ready)begin
  cp_native_pauses++;
  if(dut.valid!=0)cp_native_pending++;
  if(dut.sv && dut.sr)cp_native_replies++;
  if(step || machine_clk || {dut.m.b_ProcH.state_guest_advance,dut.m.b_ProcL.state_guest_advance,
       dut.m.b_ContA.state_guest_advance,dut.m.b_ContB.state_guest_advance,dut.m.b_IFU.state_guest_advance,
       dut.m.b_MemC.state_guest_advance,dut.m.b_MemD.state_guest_advance,dut.m.b_MemX.state_guest_advance,
       dut.m.b_DskEth.state_guest_advance,dut.m.b_DispY.state_guest_advance}!==10'b0)
   $fatal(1,"NATIVE_CP board advanced while control paused host=%0d",host_ticks);
  cp_owner_snapshot={dut.m.r_front_upper_q,dut.m.r_front_lower_q,dut.m.r_front_before,dut.m.r_front_clock_before};
  cp_ram_snapshot=native_cp_memory_image();
  cp_bb_pc=dut.m.b_BaseBd.u_f61.u_cpu.logic_00.pc;
  #1;
  if({dut.m.r_front_upper_q,dut.m.r_front_lower_q,dut.m.r_front_before,dut.m.r_front_clock_before}!==cp_owner_snapshot)
   $fatal(1,"NATIVE_CP closed owner changed during control pause host=%0d",host_ticks);
  if(native_cp_memory_image()!==cp_ram_snapshot)
   $fatal(1,"NATIVE_CP SRAM changed during control pause host=%0d",host_ticks);
  if(dut.m.b_BaseBd.u_f61.u_cpu.logic_00.pc!==cp_bb_pc)
   $fatal(1,"NATIVE_CP BaseBoard PC advanced during control pause host=%0d",host_ticks);
  cp_native_snapshots++;
 end
end
always @(negedge host_clk)if(!reset && (host_ticks%10000000==0 || host_ticks+2>=limit))
 $display("NATIVE_CP_PAUSE host=%0d guest=%0d ready=%b commits=%0d pauses=%0d snapshots=%0d pending=%0d replies=%0d held_req=%0d held_rsp=%0d",
  host_ticks,guest_ticks,control_ready,cp_native_commits,cp_native_pauses,cp_native_snapshots,
  cp_native_pending,cp_native_replies,cp_native_held_requests,cp_native_held_responses);
'''


def prepare(stage,baseline,component,out,limit):
    assert not out.exists()
    staged=json.loads((stage/'r-front-control-native.json').read_text())
    assert staged['global_control_ready_gate_wired']and staged['storage_ready_kept_separate']
    prepare_base(stage,baseline,out,limit,monitor_baseboard=True)
    provenance=out/'provenance.json.gz';r=json.loads(gzip.decompress(provenance.read_bytes()))
    tb=out/'inputs/harness/tb_native_cedar.sv';raw=tb.read_text()
    raw=replace(raw,'  wire storage_ready,storage_initialized,pack_ready,pack_fault;',
                '  wire storage_ready,storage_initialized,pack_ready,pack_fault;wire control_ready;')
    raw=replace(raw,'  wire source_enable=!reset && storage_ready && pack_ready;',
                '  wire source_enable=!reset && storage_ready && pack_ready && control_ready;')
    raw=replace(raw,'.storage_ready(storage_ready),.storage_initialized(storage_initialized),',
                '.storage_ready(storage_ready),.storage_initialized(storage_initialized),.control_ready(control_ready),')
    # The central owner intentionally replaces package j02. Preserve the
    # passive observer by binding its seven INPUT pins to the exact original
    # nets from the frozen production package, rather than removing it.
    original_ca=baseline/'inputs/src/fpga/mister/snapshot_native_conta/ContA_full_snapshot.sv'
    ca=original_ca.read_text()
    package=re.search(r'cell_MC10176_snapshot u_j02 \(.*?\n\s*\);',ca,re.S)[0]
    observer_map={}
    for pin in [9,12,11,10,7,6,5]:
        net=re.search(r'\.p'+str(pin)+r'\((\w+)\)',package)[1]
        old=f'dut.m.b_ContA.u_j02.p{pin}'
        new='dut.m.b_ContA.'+net
        raw=replace(raw,old,new);observer_map[str(pin)]=net
    # Keep the legacy passive checks, but only inspect valid forecasts. Add
    # separate pre-commit assertions above to catch readiness/gate races.
    raw=replace(raw,'always @(negedge host_clk)if(!reset && step)begin',
                'always @(negedge host_clk)if(!reset && step && control_ready)begin')
    assert raw.count('else traffic_classes[6]++;')==1 and raw.count('else window_gate[6]++;')==1
    raw=replace(raw,'else traffic_classes[6]++;','else if(!control_ready)traffic_classes[7]++;\n    else traffic_classes[6]++;')
    raw=replace(raw,'else window_gate[6]++;','else if(!control_ready)window_gate[7]++;\n      else window_gate[6]++;')
    ref=(component/'inputs/tb_r_front_local.sv').read_text()
    function=re.search(r' function automatic \[9215:0\] memory_image\(\);.*? endfunction',ref,re.S)[0]
    function=function.replace('memory_image','native_cp_memory_image').replace('dut.child.', 'dut.m.r_front_evaluator.child.')
    assert raw.count('endmodule')==1
    raw=raw.replace('endmodule',function+'\n'+MONITOR+'\nendmodule')
    tb.write_text(raw);r['inputs']['harness/tb_native_cedar.sv']=sha(tb)
    source=out/'inputs/prepare_cp_native_pause.py';source.write_bytes(Path(__file__).read_bytes())
    r['inputs'][source.name]=sha(source)
    r.update(scope=__doc__,control_barrier_gate_in_native_harness=True,
             original_baseline_inputs_or_limits_changed=False,framework_reset_and_boot_stimulus_unchanged=True,
             passive_commit_and_all_ten_board_pause_checks=True,passive_closed_owner_and_SRAM_pause_checks=True,
             passive_held128_request_response_checks=True,traffic_gate_class7_is_control_wait=True,
             passive_CP_package_input_net_mapping=observer_map,
             passive_CP_observer_source_sha256=sha(original_ca),
             passive_forecast_monitor_requires_ready=True,full_goal_active=True)
    provenance.write_bytes(gzip.compress((json.dumps(r,indent=2)+'\n').encode(),mtime=0))
    print('PREPARED_SHARED_CONTROL_NATIVE_PAUSE_TEST',limit,'host clocks',flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['stage','baseline','component','output']:parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--limit',type=int,default=300000000)
    a=parser.parse_args();prepare(a.stage.resolve(),a.baseline.resolve(),a.component.resolve(),a.output.resolve(),a.limit)
