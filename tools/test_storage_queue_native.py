#!/usr/bin/env python3
"""Qualify the actual native storage fragment's queue/reset/selftest mux.

Compile the prepared wrapper fragment, real selftest, queue and backend against
independent XSDS pins. Keep all128 owners; use a bounded address geometry for
repeated complete selftests. This proves coordination, not full native firmware,
23-bit initialization duration, fitted IO timing or hardware performance.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from prepare_storage_response_pipeline import replace_once

ROOT=Path(__file__).resolve().parents[1]


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def fragment(wrapper):
    start=wrapper.index('  wire board_ready;')
    end=wrapper.index('  dorado_native_backplane #',start)
    body=wrapper[start:end]
    for old in ['  wire board_ready;\n',
        '  wire [GROUPS-1:0] valid,wr,accept,response,response_ready;\n',
        '  wire [GROUPS*16-1:0] addresses;\n',
        '  wire [GROUPS*16-1:0] wdata,rdata;\n']:
        body=replace_once(body,old,'')
    body=replace_once(body,'.GROUP_ADDR_BITS(16)','.GROUP_ADDR_BITS(GA)')
    return '''`default_nettype none
module native_storage_fragment #(parameter integer GA=3,GROUPS=128,RAW_BITS=GA+$clog2(GROUPS))(
 input wire clk_host,reset,board_ready,
 input wire [GROUPS-1:0] valid,wr,response_ready,
 output wire [GROUPS-1:0] accept,response,
 input wire [GROUPS*GA-1:0] addresses,
 input wire [GROUPS*16-1:0] wdata,
 output wire [GROUPS*16-1:0] rdata,
 output wire storage_ready,storage_initialized,
 output wire [12:0] sdram_a,output wire [1:0] sdram_ba,
 output wire sdram_ncs,sdram_nras,sdram_ncas,sdram_nwe,sdram_dqml,sdram_dqmh,sdram_cke,
 output wire [15:0] sdram_dq_out,output wire sdram_dq_oe,input wire [15:0] sdram_dq_in
);
 wire [8:0] storage_events;
 wire [127:0] storage_selftest_observation,storage_burst_observation;
 wire [7:0] ddram_burstcount,ddram_be;wire [28:0] ddram_addr;
 wire ddram_read,ddram_write;wire [63:0] ddram_wdata;
'''+body+'endmodule\n`default_nettype wire\n'


TB=r'''
`timescale 1ns/1ps
module tb_native_storage;
 localparam GROUPS=128,GA=3,RAW_BITS=10,WORDS=1<<RAW_BITS;
 reg clk=0;always #(500000000.0/57142857)clk=~clk;
 reg reset=1,board_ready=1;
 reg [GROUPS-1:0] valid=0,wr=0,rsp_ready=0;
 wire [GROUPS-1:0] accept,response;
 reg [GROUPS*GA-1:0] addresses=0;
 reg [GROUPS*16-1:0] wdata=0;wire [GROUPS*16-1:0] rdata;
 wire ready,initialized;wire [12:0] a;wire [1:0] ba;
 wire ncs,nras,ncas,nwe,dqml,dqmh,cke,dq_oe;
 wire [15:0] dq_out;tri [15:0] dq;assign dq=dq_oe ? dq_out : 16'hzzzz;
 native_storage_fragment dut(.clk_host(clk),.reset(reset),.board_ready(board_ready),
  .valid(valid),.wr(wr),.response_ready(rsp_ready),.accept(accept),.response(response),
  .addresses(addresses),.wdata(wdata),.rdata(rdata),.storage_ready(ready),.storage_initialized(initialized),
  .sdram_a(a),.sdram_ba(ba),.sdram_ncs(ncs),.sdram_nras(nras),.sdram_ncas(ncas),.sdram_nwe(nwe),
  .sdram_dqml(dqml),.sdram_dqmh(dqmh),.sdram_cke(cke),
  .sdram_dq_out(dq_out),.sdram_dq_oe(dq_oe),.sdram_dq_in(dq));
 wire chip_clk=~clk;
 as4c32m16sb_6tin_chip_model #(.CHIP_NAME("NATIVE_QUEUE_R0"),.INIT_UNWRITTEN_TO_X(0),
  .tRC_MIN(63.0),.tRFC_MIN(63.0),.tRCD_MIN(21.0),.tRP_MIN(21.0),
  .tRRD_MIN(14.0),.tMRD_MIN(14.0),.tWR_MIN(14.0),.tAC_MAX(6.0)) chip0(
  .Clk(chip_clk),.Cke(cke),.Cs_n(ncs),.Ras_n(nras),.Cas_n(ncas),.We_n(nwe),
  .Ba(ba),.Addr(a),.Ldqm(dqml),.Udqm(dqmh),.Dq(dq));
 as4c32m16sb_6tin_chip_model #(.CHIP_NAME("NATIVE_QUEUE_R1"),.INIT_UNWRITTEN_TO_X(0),
  .tRC_MIN(63.0),.tRFC_MIN(63.0),.tRCD_MIN(21.0),.tRP_MIN(21.0),
  .tRRD_MIN(14.0),.tMRD_MIN(14.0),.tWR_MIN(14.0),.tAC_MAX(6.0)) chip1(
  .Clk(chip_clk),.Cke(cke),.Cs_n(~ncs),.Ras_n(nras),.Cas_n(ncas),.We_n(nwe),
  .Ba(ba),.Addr(a),.Ldqm(dqml),.Udqm(dqmh),.Dq(dq));
 function automatic [15:0] physical(input integer address);
  physical=chip0.mem_read(0,address>>10,address&1023);
 endfunction
 integer ticks=0,admitted=0,issued=0,retired=0,completed=0,drain_ticks=0;
 integer epochs=0,init_resets=0,full_resets=0,raw_resets=0,physical_writes=0;
 reg prev_initialized=0;
 reg [RAW_BITS-1:0] descriptor_address[0:4095];
 reg [15:0] descriptor_data[0:4095];reg descriptor_write[0:4095];
 reg raw_active=0;
 reg [GROUPS-1:0] front_pending=0;
 reg [15:0] expected[0:GROUPS-1];reg expected_write[0:GROUPS-1];
 reg [RAW_BITS-1:0] expected_address[0:GROUPS-1];
 integer physical_at_issue=0;
 reg verify_drain=0;
 reg [127:0] drain_groups=0;
 integer max_owned=0;
 always @(posedge chip_clk)if({nras,ncas,nwe}==3'b100)physical_writes++;
 always @(posedge clk)begin
  ticks++;
  if(ticks>1000000)$fatal(1,"NATIVE_QUEUE deadline");
  if(dut.u_initialize.failed)$fatal(1,"NATIVE_QUEUE selftest failed");
  if(dut.u_arb.owned>max_owned)max_owned=dut.u_arb.owned;
  if(dut.u_sdram.reset && (dut.u_arb.owned!=0 || raw_active))
   $fatal(1,"NATIVE_QUEUE early backend reset");
  if(dut.storage_queue_draining)begin
   drain_ticks++;
   if(dut.init_valid || dut.init_accept || dut.init_response)
    $fatal(1,"NATIVE_QUEUE selftest active during drain");
   if(dut.u_arb.owned!=0 && !dut.storage_queue_grant)
    $fatal(1,"NATIVE_QUEUE drain disconnected");
  end
  if(verify_drain && dut.init_valid && dut.init_accept)begin
   if(issued!=admitted || retired!=admitted)$fatal(1,"NATIVE_QUEUE init before drain");
   for(integer g=0;g<GROUPS;g++)if(drain_groups[g])
    if(physical(int'(expected_address[g]))!==expected[g])
     $fatal(1,"NATIVE_QUEUE accepted write lost at reset group=%0d",g);
   verify_drain=0;
  end
  if(reset)front_pending=0;
  for(integer g=0;g<GROUPS;g++)begin
   if(response[g] && rsp_ready[g])begin
    if(!front_pending[g] || reset || !initialized)$fatal(1,"NATIVE_QUEUE stale owner response");
    if(expected_write[g])begin
     if(physical(int'(expected_address[g]))!==expected[g])$fatal(1,"NATIVE_QUEUE front ACK before write capture");
    end else if(rdata[16*g+:16]!==expected[g])$fatal(1,"NATIVE_QUEUE read data");
    completed++;front_pending[g]=0;
   end
   if(valid[g] && accept[g])begin
    if(front_pending[g] || reset || !ready)$fatal(1,"NATIVE_QUEUE illegal admission");
    descriptor_address[admitted]=(RAW_BITS'(addresses[GA*g+:GA])<<7)|RAW_BITS'(g);
    descriptor_data[admitted]=wdata[16*g+:16];descriptor_write[admitted]=wr[g];
    expected_write[g]=wr[g];expected_address[g]=descriptor_address[admitted];
    expected[g]=wr[g] ? wdata[16*g+:16] : physical(int'(expected_address[g]));
    admitted++;front_pending[g]=1;
   end
  end
  if(dut.sv && dut.sr)begin
   if(!raw_active)$fatal(1,"NATIVE_QUEUE orphan raw response");
   retired++;raw_active=0;
  end
  if(dut.rv && dut.rr)begin
   if(raw_active || issued>=admitted || dut.ra!==descriptor_address[issued] ||
      dut.rw!==descriptor_write[issued] || dut.wd!==descriptor_data[issued])
    $fatal(1,"NATIVE_QUEUE raw order/payload");
   issued++;raw_active=1;
  end
  if(initialized && !prev_initialized)begin
   for(integer j=0;j<WORDS;j++)if(physical(j)!==16'd0)$fatal(1,"NATIVE_QUEUE nonzero initialized memory");
   epochs++;
  end
  prev_initialized=initialized;
 end
 task automatic wait_ready;
  begin wait(ready);@(negedge clk);end
 endtask
 task automatic pulse_reset;
  begin reset=1;@(negedge clk);reset=0;valid=0;rsp_ready=0;end
 endtask
 task automatic batch(input integer write_mode);
  reg [127:0] sent,received;
  begin
   sent=0;received=0;valid='1;wr=write_mode ? '1 : '0;rsp_ready='1;
   for(integer g=0;g<GROUPS;g++)begin addresses[GA*g+:GA]=GA'(g%8);wdata[16*g+:16]=16'(g)^16'h0a50;end
   while(received!='1)begin
    @(posedge clk);
    for(integer g=0;g<GROUPS;g++)begin if(accept[g] && valid[g])sent[g]=1;if(response[g] && rsp_ready[g])received[g]=1;end
    @(negedge clk);valid=~sent;
   end
   valid=0;rsp_ready=0;
  end
 endtask
 task automatic reset_full(input integer read_first);
  integer count;
  begin
   count=0;valid=128'hf;wr=read_first ? 128'he : 128'hf;rsp_ready=0;
   for(integer g=0;g<4;g++)begin addresses[GA*g+:GA]=GA'(7);wdata[16*g+:16]=16'h0bc0+16'(g);end
   drain_groups=read_first ? 128'he : 128'hf;
   while(count<4)begin
    @(posedge clk);count+=$countones(valid & accept);@(negedge clk);valid &= ~front_pending;
   end
   if(read_first)begin wait(dut.u_sdram.state==6);@(negedge clk);raw_resets++;end
   else begin wait(response!=0);@(negedge clk);full_resets++;end
   verify_drain=1;pulse_reset();wait_ready();
   if(verify_drain)$fatal(1,"NATIVE_QUEUE missing drain verification");
  end
 endtask
 initial begin
  repeat(3)@(negedge clk);reset=0;
  // Interrupt actual power-on selftest while a request is outstanding.
  wait(dut.u_initialize.pending);@(negedge clk);pulse_reset();init_resets++;
  wait_ready();
  board_ready=0;@(negedge clk);if(ready)$fatal(1,"NATIVE_QUEUE board barrier bypass");board_ready=1;
  batch(1);batch(0);
  reset_full(0);batch(1);batch(0);reset_full(1);batch(0);
  wait(!raw_active && dut.u_arb.owned==0);repeat(20)@(negedge clk);
  if(chip0.error_count || chip1.error_count || chip0.warning_count || chip1.warning_count)
   $fatal(1,"NATIVE_QUEUE chip timing/protocol diagnostics");
  if(epochs!=3 || admitted!=issued || issued!=retired || max_owned!=4 || drain_ticks<4 ||
     full_resets!=1 || raw_resets!=1 || init_resets!=1 || completed!=admitted-8)
   $fatal(1,"NATIVE_QUEUE coverage epochs=%0d admitted=%0d issued=%0d retired=%0d completed=%0d",epochs,admitted,issued,retired,completed);
  $display("NATIVE_QUEUE PASS groups=128 address_bits=%0d epochs=%0d admitted=%0d issued=%0d retired=%0d completed=%0d max_owned=%0d drain_ticks=%0d ticks=%0d",GA,epochs,admitted,issued,retired,completed,max_owned,drain_ticks,ticks);
  $finish;
 end
endmodule
'''


def run(generated,output,controls):
    assert not output.exists(),'Observe existing work'
    proof=json.loads((generated/'provenance.json').read_text())
    for n,h in proof['generated_hashes'].items():assert sha(generated/n)==h
    output.mkdir(parents=True);inputs=output/'inputs';inputs.mkdir()
    for n in proof['generated_hashes']:shutil.copyfile(generated/n,inputs/n)
    for source in [Path(__file__),ROOT/'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv']:
        shutil.copyfile(source,inputs/source.name)
    (inputs/'fragment.sv').write_text(fragment((inputs/'dorado_native_machine.sv').read_text()))
    (inputs/'tb.sv').write_text(TB)
    frozen={p.name:sha(p)for p in inputs.iterdir()}
    (output/'provenance.json').write_text(json.dumps(dict(scope=__doc__,preparation=proof,inputs=frozen),indent=2)+'\n')
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),actions=[],results={})
    def save():
        temp=output/'state.tmp';temp.write_text(json.dumps(state,indent=2)+'\n');os.replace(temp,output/'run-state.json')
    def execute(label,command):
        command=list(map(str,command))
        with(output/(label+'.log')).open('x')as log:
            p=subprocess.Popen(command,cwd=inputs,stdout=log,stderr=subprocess.STDOUT)
            row=dict(label=label,pid=p.pid,command=command,started=time.time());state['actions'].append(row);save()
            row.update(returncode=p.wait(),finished=time.time());save()
        return row['returncode']
    save()
    try:
        cases=[('positive',None)]
        if controls:cases += [
            ('disconnected-drain',('storage_initialized || storage_queue_draining','storage_initialized','NATIVE_QUEUE drain disconnected')),
            ('early-backend-reset',('.reset(storage_queue_reset),','.reset(reset),','NATIVE_QUEUE early backend reset')),
            ('selftest-not-held',('.reset(reset || storage_queue_draining),','.reset(reset),','NATIVE_QUEUE selftest active during drain')),
        ]
        for label,mutation in cases:
            source=inputs/'fragment.sv'
            if mutation:
                old,new,diagnostic=mutation;source=output/(label+'.sv')
                source.write_text(replace_once((inputs/'fragment.sv').read_text(),old,new))
            obj=output/('obj-'+label)
            cmd=['verilator','--binary','--timing','--autoflush','-j','2','-Wno-fatal','-Werror-UNOPTFLAT',
                '-Wno-TIMESCALEMOD','--top-module','tb_native_storage','+define+DORADO_HOST_CLOCK_HZ=57142857',
                '--Mdir',obj,'-o','test',source,inputs/'dorado_storage_queue.sv',inputs/'dorado_storage_selftest.sv',
                inputs/'dorado_storage_sdram_burst.sv',inputs/'xsds_128mbyte_sdram_model.sv',inputs/'tb.sv']
            assert execute(label+'-compile',cmd)==0,label
            rc=execute(label+'-test',[obj/'test']);log=(output/(label+'-test.log')).read_text()
            if mutation:
                assert rc!=0 and diagnostic in log,(label,rc,log[-2000:]);state['results'][label]=dict(rejected=True,diagnostic=diagnostic)
            else:
                assert rc==0 and 'NATIVE_QUEUE PASS'in log,(label,rc,log[-3000:])
                state['results'][label]=dict(passed=True,output=log)
            save();print(label,state['results'][label],flush=True)
        assert all(sha(inputs/n)==h for n,h in frozen.items())
        state.update(status='PASS_NATIVE_QUEUE_RESET_INIT_BOUNDED_ADDRESS_PINS',finished=time.time(),inputs_unchanged=True,
            native_full_geometry_firmware_fit_or_speed_qualified=False);save()
    except BaseException as error:
        state.update(status='TERMINAL_FAILED',error=repr(error),finished=time.time());save();raise


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['generated','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--controls',action='store_true');a=p.parse_args()
    run(a.generated.resolve(),a.output.resolve(),a.controls)
