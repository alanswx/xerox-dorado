#!/usr/bin/env python3
"""Check every physical F10145 bit, pending write and first-edge restore."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

import generate_f10145 as gen

HERE=Path(__file__).resolve().parent
FIXTURE='''module f10145_snapshot_fixture(
 input wire host_clk,ref_clk,advance,input wire [3:0] address,data,
 input wire ce_n,we_n,freeze,held,request,write,
 input wire [4:0] index,input wire [9:0] state_data,
 output wire frozen,done,error,output wire [9:0] result,
 output wire [3:0] q,ref_q,output wire [9:0] window,ref_window,
 output wire [63:0] memory,ref_memory);
 cell_F10145A_snapshot candidate(
 .state_host_clk(host_clk),.state_advance(advance),.state_freeze(freeze),
 .state_guest_held(held),.state_request(request),.state_write(write),
 .state_index(index),.state_data(state_data),.state_frozen(frozen),
 .state_done(done),.state_error(error),.state_result(result),
 .sys_clk(ref_clk),
 .p10(address[3]),.p9(address[2]),.p7(address[1]),.p6(address[0]),
 .p5(data[0]),.p4(data[1]),.p11(data[2]),.p12(data[3]),
 .p3(ce_n),.p13(we_n),.p2(q[0]),.p1(q[1]),.p15(q[2]),.p14(q[3]),.p8(1'b0));
 cell_F10145A original(.sys_clk(ref_clk),
 .p10(address[3]),.p9(address[2]),.p7(address[1]),.p6(address[0]),
 .p5(data[0]),.p4(data[1]),.p11(data[2]),.p12(data[3]),
 .p3(ce_n),.p13(we_n),.p2(ref_q[0]),.p1(ref_q[1]),.p15(ref_q[2]),.p14(ref_q[3]),.p8(1'b0));
 assign window={candidate.ce_lat,candidate.d_lat,candidate.a_lat,candidate.we_d};
 assign ref_window={original.ce_lat,original.d_lat,original.a_lat,original.we_d};
 genvar i;
 generate for(i=0;i<16;i=i+1)begin: observe
  assign memory[i*4+:4]=candidate.mem[i];
  assign ref_memory[i*4+:4]=original.mem[i];
 end endgenerate
endmodule
'''


def run(output,mutations):
    output.mkdir(parents=True,exist_ok=False)
    source,manifest=gen.generate()
    cases=[('candidate',source,None)]
    if mutations:
        for name,before,after,diagnostic in [
            ('ungated_guest','if (!state_hold && state_advance)', 'if (!state_hold)', 'held guest state changed'),
            ('lost_address','a_lat  <= a;', "a_lat  <= 4'b0;", 'ordinary oracle state differs'),
            ('lost_ce','ce_lat <= ~p3;', "ce_lat <= 1'b1;", 'ordinary oracle state differs'),
            ('current_address_write','mem[a_lat] <= d_lat;', 'mem[a] <= d_lat;', 'ordinary oracle state differs'),
            ('running_access','wire state_accept = state_hold && state_frozen && state_request;',
             'wire state_accept = state_request;', 'running request accepted'),
            ('entry_access','wire state_accept = state_hold && state_frozen && state_request;',
             'wire state_accept = state_hold && state_request;', 'entry request accepted'),
            ('invalid_alias',"wire state_valid = state_index <= 5'd16;", "wire state_valid = 1'b1;", 'invalid request accepted'),
            ('lost_window_restore','{ce_lat,d_lat,a_lat,we_d} <= state_data;',
             "{ce_lat,d_lat,a_lat,we_d} <= 10'b0;", 'write-window bit walk differs'),
        ]:
            if source.count(before)!=1:raise ValueError('ambiguous mutation: '+name)
            cases.append((name,source.replace(before,after),diagnostic))
    rows=[]
    for name,text,diagnostic in cases:
        case=output/name;case.mkdir()
        (case/'fixture.sv').write_text(FIXTURE)
        (case/'cell.sv').write_text(text)
        command=['verilator','--cc','--exe','--build','-j','4','--top-module',
            'f10145_snapshot_fixture','-Wno-fatal','-DDORADO_F10145_LOGIC',
            '--Mdir',str(case/'obj'),str(case/'fixture.sv'),str(case/'cell.sv'),
            str(gen.ORIGINAL),str(HERE/'test_f10145_snapshot.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with (case/'build.log').open('w')as log:
            subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vf10145_snapshot_fixture')],capture_output=True,text=True,timeout=60)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text)
        if diagnostic:assert result.returncode and diagnostic in text,(name,text)
        else:assert result.returncode==0 and 'F10145 SNAPSHOT PASS'in text,text
        rows.append(dict(case=name,returncode=result.returncode,expected_failure=diagnostic))
        print(name+': '+text.strip(),flush=True)
    (output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    paths=[Path(__file__).resolve(),HERE/'generate_f10145.py',HERE/'test_f10145_snapshot.cpp',gen.ORIGINAL]
    (output/'results.json').write_text(json.dumps(dict(scope=manifest['scope'],
        hashes={str(p.relative_to(gen.ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths},cases=rows),indent=2)+'\n')


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--mutations',action='store_true')
    args=parser.parse_args();run(args.output.resolve(),args.mutations)
