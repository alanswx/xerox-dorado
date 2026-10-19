#!/usr/bin/env python3
"""MC10135 original/oracle/state restore tests across all PRECAP combinations."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

import generate_jk as gen

HERE=Path(__file__).resolve().parent
FIXTURE='''module jk_fixture(
 input wire host_clk,ref_clk,advance,hold,write,restore_individual,
 input wire [8:0] pins,input wire [17:0] data,
 input wire [6:0] restore_data[4],
 output wire [17:0] result[4],output wire [6:0] candidate_state[4],original_state[4],
 output wire [15:0] outputs,original_outputs);
 genvar i;
 generate for(i=0;i<4;i=i+1)begin:cases
 cell_MC10135_snapshot #(.PRECAP_A(i&1),.PRECAP_B((i>>1)&1)) candidate(
 .state_host_clk(host_clk),.state_advance(advance),.state_hold(hold),.state_write(write),
 .state_data(restore_individual?{11'b0,restore_data[i]}:data),.state_result(result[i]),
 .sys_clk(ref_clk),PORTS);
 cell_MC10135 #(.PRECAP_A(i&1),.PRECAP_B((i>>1)&1)) original(
 .sys_clk(ref_clk),REFPORTS);
 assign candidate_state[i]={candidate.ck_d,candidate.jk_pre,candidate.qb,candidate.qa};
 assign original_state[i]={original.ck_d,original.jk_pre,original.qb,original.qa};
 end endgenerate
endmodule
'''
PORTS='''.p7(pins[0]),.p6(pins[1]),.p10(pins[2]),.p11(pins[3]),
 .p4(pins[4]),.p5(pins[5]),.p13(pins[6]),.p12(pins[7]),.p9(pins[8]),
 .p2(outputs[4*i]),.p3(outputs[4*i+1]),.p15(outputs[4*i+2]),.p14(outputs[4*i+3])'''
FIXTURE=FIXTURE.replace('REFPORTS',PORTS.replace('outputs','original_outputs')).replace('PORTS',PORTS)


def run(output,mutations):
    output.mkdir(parents=True,exist_ok=False);source,manifest=gen.generate()
    cases=[('candidate',None,None,None)]
    if mutations:
        cases+=[
            ('lost_setup','jk_pre <= {p11,p10,p6,p7};',"jk_pre <= 4'b0;",'state oracle differs'),
            ('lost_clock','ck_d <= p9;',"ck_d <= 1'b0;",'state oracle differs'),
            ('ungated','else if (state_advance) begin','else begin','state oracle differs'),
            ('ignore_hold','if (state_hold) begin',"if (1'b0) begin",'state oracle differs'),
            ('lost_restore','<=state_data[6:0];',"<=7'b0;",'state oracle differs'),
            ('ignore_precap','PRECAP_A ? jk_pre[0] : p7','p7','state oracle differs'),
            ('wrong_jk','(ja & ~qa) | (~ka & qa)','(ja & ~qa) | (ka & qa)','state oracle differs'),
        ]
    results=[]
    for name,before,after,diagnostic in cases:
        case=output/name;case.mkdir();text=source
        if before:
            assert text.count(before)==1,(name,before);text=text.replace(before,after)
        (case/'fixture.sv').write_text(FIXTURE);(case/'cell_MC10135_snapshot.sv').write_text(text)
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','jk_fixture','-Wno-fatal',
            '--Mdir',str(case/'obj'),str(case/'fixture.sv'),str(case/'cell_MC10135_snapshot.sv'),
            str(gen.ROOT/'verilog/cells/cell_MC10135.v'),str(HERE/'test_jk.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with (case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vjk_fixture')],capture_output=True,text=True,timeout=60)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text)
        if diagnostic:assert result.returncode and diagnostic in text,(name,text)
        else:assert result.returncode==0 and 'JK PASS'in text,text
        results.append(dict(case=name,returncode=result.returncode,expected_failure=diagnostic))
        print(name+': '+text.strip(),flush=True)
    controls=[];original=(gen.ROOT/'verilog/cells/cell_MC10135.v').read_text()
    for name,text in [('extra_state',original.replace('endmodule','reg lost;\nendmodule')),
                      ('extra_process',original.replace('endmodule','always @(posedge sys_clk) qa <= 0;\nendmodule')),
                      ('changed_priority',original.replace("if (p5)         qa <= 1'b1;","if (p4)         qa <= 1'b1;"))]:
        assert text!=original
        try:gen.generate(text)
        except ValueError:controls.append(dict(case=name,rejected=True))
        else:raise AssertionError('unreviewed original accepted: '+name)
    paths=[Path(__file__),HERE/'test_jk.cpp',HERE/'generate_jk.py',gen.ROOT/'verilog/cells/cell_MC10135.v',
           HERE/'generate_snapshot_ff.py',HERE/'generate_contb_full.py']
    (output/'results.json').write_text(json.dumps(dict(status='PASS',manifest=manifest,cases=results,
        source_controls=controls,source_hashes={str(p.relative_to(gen.ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths}),indent=2)+'\n')


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--mutations',action='store_true');args=parser.parse_args();run(args.output.resolve(),args.mutations)
