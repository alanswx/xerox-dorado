#!/usr/bin/env python3
"""Native8/12 frontend serial state versus original RTL and original DRAM chips."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import generate_storage_group as gen

HERE=Path(__file__).resolve().parent


def fixture():
    text='''module storage_snapshot_fixture(
 input wire clk,ref_clk,advance,hold,scan_step,serial_in,
 input wire selected,reset,input wire [15:0] probe_addr,input wire [1:0] ras_n,cas_n,we_n,req_ready,rsp_valid,
 input wire [15:0] next_addr[2],data_in[2],rsp_data[2],
 output wire [1:0] ready,req_valid,req_write,rsp_ready,frozen,drained,serial_out,
 output wire [1:0] ref_ready,ref_req_valid,ref_req_write,ref_rsp_ready,
 output wire [15:0] req_addr[2],req_data[2],data_out[2],ref_req_addr[2],ref_req_data[2],ref_data_out[2],chip_data_out[2],
 output wire [15:0] chip_word[2],output wire [127:0] physical[2],original_state[2]);
 wire reference_machine_clk=ref_clk && advance;
'''
    for i,width in enumerate((8,12)):
        bits=sum(w for _,w in gen.fields(width));param=f'#(.ADDR_BITS(16),.WIDTH({width}),.DEFER_READS(0))'
        text+=f'''begin:case{i}
 reg [15:0] addr=0,reference_addr=0;
 always @(posedge clk)if(advance && !hold)addr<=next_addr[{i}];
 always @(posedge reference_machine_clk)reference_addr<=next_addr[{i}];
 dorado_storage_group_snapshot {param} dut(
 .state_guest_advance(advance),.state_hold(hold),.state_scan_step(scan_step && selected==1'd{i}),.state_serial_in(serial_in),
 .state_serial_out(serial_out[{i}]),.state_frozen(frozen[{i}]),.state_drain_ready(drained[{i}]),
 .host_clk(clk),.machine_clk(reference_machine_clk),.reset(reset),
 .ras_n(ras_n[{i}]),.cas_n(cas_n[{i}]),.we_n(we_n[{i}]),.addr(addr),.data_in(data_in[{i}][{width-1}:0]),
 .data_out(data_out[{i}][{width-1}:0]),.ready(ready[{i}]),.req_valid(req_valid[{i}]),.req_write(req_write[{i}]),
 .req_addr(req_addr[{i}]),.req_data(req_data[{i}]),.req_ready(req_ready[{i}]),
 .rsp_valid(rsp_valid[{i}]),.rsp_ready(rsp_ready[{i}]),.rsp_data(rsp_data[{i}]));
 dorado_storage_group {param} reference(
 .host_clk(ref_clk),.machine_clk(reference_machine_clk),.reset(reset),
 .ras_n(ras_n[{i}]),.cas_n(cas_n[{i}]),.we_n(we_n[{i}]),.addr(reference_addr),.data_in(data_in[{i}][{width-1}:0]),
 .data_out(ref_data_out[{i}][{width-1}:0]),.ready(ref_ready[{i}]),.req_valid(ref_req_valid[{i}]),.req_write(ref_req_write[{i}]),
 .req_addr(ref_req_addr[{i}]),.req_data(ref_req_data[{i}]),.req_ready(req_ready[{i}]),
 .rsp_valid(rsp_valid[{i}]),.rsp_ready(ref_rsp_ready[{i}]),.rsp_data(rsp_data[{i}]));
 for(genvar b=0;b<{width};b++)begin:chip
 cell_MK4096P_6 #(.ADDR_BITS(16)) original(.sys_clk(reference_machine_clk),.p2(data_in[{i}][b]),.p3(we_n[{i}]),
 .p4(ras_n[{i}]),.p15(cas_n[{i}]),.addr_i(reference_addr),.p14(chip_data_out[{i}][b]));
 initial for(integer a=0;a<65536;a++)original.mem[a]=0;
 assign chip_word[{i}][b]=original.mem[probe_addr];
 end
'''
        for out,prefix in [('physical','dut'),('original_state','reference')]:
            packed='{'+','.join(prefix+'.'+n for n,_ in reversed(gen.FIELDS))+'}'
            text+=f" assign {out}[{i}]={{{128-bits}'b0,{packed}}};\n"
        for out in ('data_out','ref_data_out','chip_data_out','chip_word'):
            text+=f' assign {out}[{i}][15:{width}]=0;\n'
        text+='end\n'
    return text+'endmodule\n'


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    source,manifest=gen.generate()
    for p in (Path(__file__).resolve(),HERE/'generate_storage_group.py',HERE/'generate_snapshot_ff.py',HERE/'test_storage_group_snapshot.cpp',
              gen.ROOT/'fpga/mister/dorado_storage_group.sv',gen.ROOT/'verilog/cells/cell_MK4096P_6.v'):
        (inputs/p.name).write_bytes(p.read_bytes())
    cases=[('candidate',None,None,None)]
    if mutations:
        cases += [
            ('lost_hold','if(state_hold)begin',"if(1'b0)begin",'physical state differs'),
            ('lost_guest_enable','if(state_guest_advance) begin',"if(1'b1) begin",'physical state differs'),
            ('host_frozen_with_guest','if(reset) begin cache_valid', 'if(!state_guest_advance)begin end else if(reset) begin cache_valid','physical state differs'),
            ('held_request_leak','assign req_valid=normal_req_valid && !state_hold;','assign req_valid=normal_req_valid;','held raw request/response leak'),
            ('held_response_leak','assign rsp_ready=waiting && !state_hold;','assign rsp_ready=waiting;','held raw request/response leak'),
            ('lost_old_word','read_pipe<=read_value[WIDTH-1:0];','read_pipe<=cache_data[WIDTH-1:0];','physical state differs'),
            ('lost_saved_address','saved_addr<=req_addr;','saved_addr<=0;','physical state differs'),
            ('serial_history_loss','wire [STATE_BITS-1:0] state_bits={old_data,saved_data,cache_data,saved_addr,cache_addr,saved_write,waiting,write_done,cache_valid,data_out,read_pipe,write_data,write_pending,we_d,cas_d};',
             "wire [STATE_BITS-1:0] state_bits={old_data,saved_data,cache_data,saved_addr,cache_addr,saved_write,waiting,write_done,cache_valid,data_out,read_pipe,write_data,write_pending,2'b0};",'serial physical oracle differs'),
            ('scan_when_running','if(state_hold)begin','if(state_hold || state_scan_step)begin','physical state differs'),
        ]
    results=[]
    for label,before,after,why in cases:
        case=out/label;case.mkdir();candidate=source
        if before:
            assert candidate.count(before)==1,(label,before)
            candidate=candidate.replace(before,after,1)
        (case/'storage_group_snapshot.sv').write_text(candidate);(case/'fixture.sv').write_text(fixture())
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','storage_snapshot_fixture','-Wno-fatal','--assert',
                 '--Mdir',str(case/'obj'),str(case/'fixture.sv'),str(case/'storage_group_snapshot.sv'),
                 str(inputs/'dorado_storage_group.sv'),str(inputs/'cell_MK4096P_6.v'),str(inputs/'test_storage_group_snapshot.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vstorage_snapshot_fixture')],capture_output=True,text=True,timeout=120)
        log=result.stdout+result.stderr;(case/'test.txt').write_text(log);print(label+': '+log.strip(),flush=True)
        if why:assert result.returncode and why in log,(label,log)
        else:assert result.returncode==0 and 'STORAGE SNAPSHOT PASS'in log,log
        results.append(dict(case=label,returncode=result.returncode,expected_failure=why))
    controls=[]
    for label,before,after in [('extra_state','endmodule','reg hidden;\nendmodule'),('changed_process','saved_addr<=req_addr;','saved_addr<=0;')]:
        original=(inputs/'dorado_storage_group.sv').read_text();assert original.count(before)==1
        try:gen.generate(original.replace(before,after,1))
        except ValueError as error:controls.append(dict(case=label,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('unreviewed source accepted')
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,manifest=manifest,cases=results,source_controls=controls,
        executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},
        limits='Two native frontend widths at address16/defer0; no MSA board, native/global barrier, SDRAM refresh or machine snapshot qualification.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true')
    a=p.parse_args();run(a.output.resolve(),a.mutations)
