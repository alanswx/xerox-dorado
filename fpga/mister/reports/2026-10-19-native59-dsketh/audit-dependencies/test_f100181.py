#!/usr/bin/env python3
"""Original/oracle and complete four-bit restore gate for the MemC ALU latch."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

import generate_f100181 as gen

HERE=Path(__file__).resolve().parent
PORTS='''.p1(pins[0]),.p24(pins[1]),.p23(pins[2]),.p22(pins[3]),
 .p12(pins[4]),.p13(pins[5]),.p14(pins[6]),.p15(pins[7]),
 .p16(pins[8]),.p17(pins[9]),.p20(pins[10]),.p21(pins[11]),
 .p11(pins[12]),.p19(pins[13]),.p6(1'b0),.p7(1'b0),.p18(1'b0),
 .p2(outputs[0]),.p3(outputs[1]),.p4(outputs[2]),.p5(outputs[3]),
 .p8(outputs[4]),.p9(outputs[5]),.p10(outputs[6])'''
FIXTURE='''module f100181_fixture(
 input wire host_clk,ref_clk,advance,hold,write,input wire [13:0] pins,
 input wire [17:0] data,output wire [17:0] result,
 output wire [6:0] outputs,original_outputs,output wire [3:0] original_state);
 cell_F100181_snapshot candidate(.state_host_clk(host_clk),.state_advance(advance),
 .state_hold(hold),.state_write(write),.state_data(data),.state_result(result),
 .sys_clk(ref_clk),PORTS);
 cell_F100181 original(.sys_clk(ref_clk),REFPORTS);
 assign original_state=original.q;
endmodule
'''.replace('REFPORTS',PORTS.replace('outputs','original_outputs')).replace('PORTS',PORTS)


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);source,manifest=gen.generate()
    cases=[('candidate',None,None,None)]
    if mutations:cases += [
        ('ungated','else if (state_advance) begin','else begin','state oracle differs'),
        ('ignore_hold','if (state_hold) begin',"if (1'b0) begin",'state oracle differs'),
        ('lost_restore','q<=state_data[3:0];',"q<=4'b0;",'state oracle differs'),
        ('wrong_latch','if (!p19) q <= f;','if (p19) q <= f;','state oracle differs'),
        ('wrong_carry','assign p8  = ~(grp_g | (grp_p & cin));','assign p8 = grp_g | (grp_p & cin);','pin oracle differs'),
    ]
    results=[]
    for name,before,after,diagnostic in cases:
        case=out/name;case.mkdir();text=source
        if before:
            assert text.count(before)==1,(name,before);text=text.replace(before,after)
        (case/'fixture.sv').write_text(FIXTURE);(case/'cell_F100181_snapshot.sv').write_text(text)
        cmd=['verilator','--cc','--exe','--build','-j','4','--top-module','f100181_fixture',
             '-Wno-fatal','--Mdir',str(case/'obj'),str(case/'fixture.sv'),
             str(case/'cell_F100181_snapshot.sv'),str(gen.ROOT/'verilog/cells/cell_F100181.v'),
             str(HERE/'test_f100181.cpp')]
        (case/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
        with (case/'build.log').open('w')as log:subprocess.run(cmd,check=True,stdout=log,stderr=subprocess.STDOUT)
        r=subprocess.run([str(case/'obj/Vf100181_fixture')],capture_output=True,text=True,timeout=60)
        result=r.stdout+r.stderr;(case/'test.txt').write_text(result)
        if diagnostic:assert r.returncode and diagnostic in result,(name,result)
        else:assert r.returncode==0 and 'F100181 PASS'in result,result
        results.append(dict(case=name,returncode=r.returncode,expected_failure=diagnostic))
        print(name+': '+result.strip(),flush=True)
    controls=[];original=(gen.ROOT/'verilog/cells/cell_F100181.v').read_text()
    for name,text in [('extra_state',original.replace('endmodule','reg lost;\nendmodule')),
                      ('wrong_latch',original.replace('if (!p19) q <= f;','if (p19) q <= f;')),
                      ('wrong_logic',original.replace('lg =  a ^  b;','lg =  a |  b;'))]:
        assert text!=original
        try:gen.generate(text)
        except ValueError as e:controls.append(dict(case=name,rejected=True,diagnostic=str(e)))
        else:raise AssertionError('changed original accepted')
    paths=[Path(__file__).resolve(),HERE/'test_f100181.cpp',HERE/'generate_f100181.py',
           HERE/'generate_snapshot_ff.py',HERE/'generate_contb_full.py',gen.ROOT/'verilog/cells/cell_F100181.v']
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    (out/'results.json').write_text(json.dumps(dict(cases=results,source_controls=controls,
        source_hashes={str(p):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths}),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
