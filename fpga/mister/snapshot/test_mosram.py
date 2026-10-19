#!/usr/bin/env python3
"""Original/oracle/physical-probe/restore tests for14/16-bit MosRam sockets."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

import generate_mosram as gen

HERE=Path(__file__).resolve().parent
FIXTURE='''module mosram_fixture(
 input wire host_clk,ref_clk,advance,freeze,held,
 input wire [1:0] request,write,input wire [7:0] address,
 input wire ras_n,cas_n,we_n,data,
 input wire [16:0] state_index[2],input wire [17:0] state_data[2],
 input wire [15:0] probe_address,
 output wire [1:0] frozen,done,error,q,original_q,original_probe,candidate_probe,
 output wire [17:0] result[2],output wire [22:0] original_history[2],candidate_history[2]);
 genvar k;
 generate for(k=0;k<2;k=k+1)begin:banks
  localparam integer A=14+2*k;
  cell_MosRam original(.sys_clk(ref_clk),.p2(data),.p3(we_n),.p4(ras_n),
    .p5(address[1]),.p6(address[3]),.p7(address[2]),.p9(address[7]),
    .p10(address[6]),.p11(address[5]),.p12(address[4]),.p13(address[0]),
    .p15(cas_n),.p14(original_q[k]),.p1(1'b0),.p8(1'b0),.p16(1'b0));
  defparam original.ADDR_BITS=A;
  cell_MosRam_snapshot #(.ADDR_BITS(A)) candidate(.sys_clk(ref_clk),
    .state_host_clk(host_clk),.state_advance(advance),.state_freeze(freeze),
    .state_guest_held(held),.state_request(request[k]),.state_write(write[k]),
    .state_index(state_index[k][A:0]),.state_data(state_data[k]),
    .state_frozen(frozen[k]),.state_done(done[k]),.state_error(error[k]),.state_result(result[k]),
    .p2(data),.p3(we_n),.p4(ras_n),.p5(address[1]),.p6(address[3]),.p7(address[2]),
    .p9(address[7]),.p10(address[6]),.p11(address[5]),.p12(address[4]),.p13(address[0]),
    .p15(cas_n),.p14(q[k]),.p1(1'b0),.p8(1'b0),.p16(1'b0));
  assign original_history[k]={{(16-A){1'b0}},original.write_data,original.write_pending,
    original.we_d,original.dout_r,original.dout,original.cas_d,original.ras_d,original.col,original.row};
  assign candidate_history[k]={{(16-A){1'b0}},candidate.write_data,candidate.write_pending,
    candidate.we_d,candidate.logical_dout_r,candidate.dout,candidate.cas_d,candidate.ras_d,candidate.col,candidate.row};
  assign original_probe[k]=original.mem[probe_address[A-1:0]];
  assign candidate_probe[k]=candidate.mem[probe_address[A-1:0]];
  integer row;
  initial for(row=0;row<(1<<A);row=row+1)begin
    original.mem[row]=((row*32'd1103515245+32'd12345+32'd7919*k) ^ (row>>3) ^ (row>>7) ^ (row>>11) ^ (row>>15)) & 1;
    candidate.mem[row]=((row*32'd1103515245+32'd12345+32'd7919*k) ^ (row>>3) ^ (row>>7) ^ (row>>11) ^ (row>>15)) & 1;
  end
 end endgenerate
endmodule
'''


def run(output,mutations):
    output.mkdir(parents=True,exist_ok=False);source=gen.generate();cases=[('candidate',source,None)]
    for p in [Path(__file__).resolve(),HERE/'generate_mosram.py',HERE/'test_mosram.cpp']:
        (output/(p.stem+'-executed'+p.suffix)).write_bytes(p.read_bytes())
    if mutations:
        for name,before,after,diagnostic in [
            ('ungated_history','else if(state_advance)begin','else begin','ordinary/hold history differs'),
            ('ungated_read','state_scan || (!state_hold && state_advance)',"1'b1",'ordinary/hold history differs'),
            ('ungated_write','(!state_hold && state_advance && write_pending)','(!state_hold && write_pending)',('physical RAM differs','ordinary/hold history differs')),
            ('running_access','state_accept=state_scan && state_request','state_accept=state_request','running state request accepted'),
            ('freeze_without_held','state_hold=state_freeze && state_guest_held','state_hold=state_freeze','unheld freeze accepted'),
            ('invalid_alias','state_valid=state_is_ram || state_index<=DEPTH+1',"state_valid=1'b1",'invalid state index accepted'),
            ('row_alias','state_index[ADDR_BITS-1:0]:addr',"{1'b0,state_index[ADDR_BITS-2:0]}:addr",('RAM export differs','physical RAM differs')),
            ('lost_raw_pipeline','we_d,saved_dout_r,dout','we_d,dout_r,dout',('history bit write/readback differs','physical history write differs','ordinary/hold history differs')),
            ('wrong_write_data','state_scan?state_data[0]:write_data','state_scan?state_data[0]:p2',('physical RAM differs','ordinary/hold history differs')),
        ]:
            assert before in source,(name,before)
            cases.append((name,source.replace(before,after,1),diagnostic))
        before="    if(state_hold)begin\n      if(state_history_write)begin"
        assert before in source
        cases.append(('premature_resume',source.replace(before,"    if(!state_hold)restore_pending<=1'b0;\n"+before,1),'ordinary/hold history differs'))
        before='write_pending<=!p4 && !p15 && !p3 && (cas_d || we_d);'
        assert before in source
        cases.append(('lost_pending_write',source.replace(before,"write_pending<=1'b0;",1),'ordinary/hold history differs'))
        cases.append(('unsupported_density',source,'MosRam snapshot ADDR_BITS must be14 or16'))
    rows=[]
    for name,text,diagnostic in cases:
        case=output/name;case.mkdir();fixture=FIXTURE
        if name=='unsupported_density':fixture=fixture.replace('#(.ADDR_BITS(A)) candidate','#(.ADDR_BITS(12)) candidate')
        (case/'fixture.sv').write_text(fixture);(case/'cell.sv').write_text(text)
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','mosram_fixture','-Wno-fatal',
            '--Mdir',str(case/'obj'),str(case/'fixture.sv'),str(case/'cell.sv'),str(gen.ROOT/'verilog/cells/cell_MosRam.v'),str(HERE/'test_mosram.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vmosram_fixture')],capture_output=True,text=True,timeout=300)
        message=result.stdout+result.stderr;(case/'test.txt').write_text(message);print(name+': '+message.strip(),flush=True)
        if diagnostic:
            wanted=(diagnostic,)if isinstance(diagnostic,str)else diagnostic
            assert result.returncode and any(x in message for x in wanted),(name,message)
        else:assert result.returncode==0 and 'MOSRAM RESTORE PASS'in message,message
        rows.append(dict(case=name,returncode=result.returncode,expected_failure=diagnostic))
    (output/'manifest.json').write_text(json.dumps({str(a):gen.manifest(a)for a in (14,16)},indent=2)+'\n')
    paths=[Path(__file__).resolve(),HERE/'test_mosram.cpp',HERE/'generate_mosram.py',gen.ROOT/'verilog/cells/cell_MosRam.v']
    (output/'results.json').write_text(json.dumps(dict(status='PASS',cases=rows,
        scope='Primitive modeled socket state only; no full MemX/whole-machine snapshot or physical FPGA save/load',
        source_hashes={str(p):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths}),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
