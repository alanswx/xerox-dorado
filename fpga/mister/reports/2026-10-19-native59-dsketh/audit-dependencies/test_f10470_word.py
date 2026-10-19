#!/usr/bin/env python3
"""Width-correct old-data RAM against untouched cells and independent oracles."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
FIXTURE='''module f10470_word_fixture(
 input wire host_clk,ref_clk,advance,freeze,held,request,write,
 input wire [11:0] address,input wire [17:0] data,
 input wire ce_n,we_n,input wire [12:0] word_index,
 input wire [17:0] state_data[6],output wire [5:0] frozen,done,error,
 output wire [17:0] q[6],original_q[6],result[6]);
 function integer width_for(input integer n);
  case(n)0:width_for=1;1:width_for=3;2:width_for=5;3:width_for=6;4:width_for=9;default:width_for=18;endcase
 endfunction
 genvar k,b;
 generate for(k=0;k<6;k=k+1)begin:banks
  localparam integer W=width_for(k);
  wire [W-1:0] q_w,original_w;
  dorado_snapshot_f10470_word_bank #(.WIDTH(W)) candidate(
    .host_clk(host_clk),.guest_advance(advance),.address(address),.data(data[W-1:0]),
    .ce_n(ce_n),.we_n(we_n),.q(q_w),.freeze(freeze),.guest_held(held),.frozen(frozen[k]),
    .request(request),.write(write),.word_index(word_index),.state_data(state_data[k]),
    .done(done[k]),.error(error[k]),.result(result[k]));
  for(b=0;b<W;b=b+1)begin:bits
    cell_F10470 original(.sys_clk(ref_clk),.p2(address[11]),.p3(address[10]),
      .p4(address[9]),.p5(address[8]),.p6(address[7]),.p7(address[6]),.p8(address[5]),
      .p10(address[4]),.p11(address[3]),.p12(address[2]),.p13(address[1]),.p14(address[0]),
      .p17(data[b]),.p16(ce_n),.p15(we_n),.p1(original_w[b]),.p9(1'b0),.p18(1'b0));
  end
  assign q[k]={{(18-W){1'b0}},q_w};
  assign original_q[k]={{(18-W){1'b0}},original_w};
 end endgenerate
endmodule
'''


def run(output,mutations):
    output.mkdir(parents=True,exist_ok=False);source=(HERE/'f10470_word_bank.sv').read_text()
    cases=[('candidate',None,None,None)]
    if mutations:
        cases+=[
            ('ungated_read','scan_active || (!freeze_active && guest_advance)',"1'b1",'backend hold changed read pipeline'),
            ('ungated_write','(!freeze_active && guest_advance && !ce_n && !we_n)','(!freeze_active && !ce_n && !we_n)','ordinary old-data oracle differs'),
            ('premature_resume','if(!freeze_active && guest_advance)pending','if(!freeze_active)pending','backend hold changed read pipeline'),
            ('lost_pipeline','saved_q<=state_data[WIDTH-1:0];',"saved_q<='0;",'bit/padding write readback differs'),
            ('running_access','wire accepted=scan_active && request;','wire accepted=request;','running state request accepted'),
            ('invalid_alias',"wire valid_index=word_index<=13'd4096;","wire valid_index=1'b1;",'invalid index accepted'),
            ('wrong_data','state_data[WIDTH-1:0]:data',"{WIDTH{1'b0}}:data",'bit/padding write readback differs'),
            ('wrong_read_address','ram_q<=mem[ram_address];',"ram_q<=mem[12'b0];",'ordinary old-data oracle differs'),
        ]
    rows=[]
    for name,before,after,diagnostic in cases:
        case=output/name;case.mkdir();text=source
        if before:
            assert text.count(before)==1,(name,before);text=text.replace(before,after)
        (case/'fixture.sv').write_text(FIXTURE);(case/'bank.sv').write_text(text)
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','f10470_word_fixture','-Wno-fatal',
            '--Mdir',str(case/'obj'),str(case/'fixture.sv'),str(case/'bank.sv'),
            str(ROOT/'verilog/cells/cell_F10470.v'),str(HERE/'test_f10470_word.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with (case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vf10470_word_fixture')],capture_output=True,text=True,timeout=120)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text)
        if diagnostic:assert result.returncode and diagnostic in text,(name,text)
        else:assert result.returncode==0 and 'F10470 WORD PASS'in text,text
        rows.append(dict(case=name,returncode=result.returncode,expected_failure=diagnostic));print(name+': '+text.strip(),flush=True)
    paths=[Path(__file__).resolve(),HERE/'test_f10470_word.cpp',HERE/'f10470_word_bank.sv',ROOT/'verilog/cells/cell_F10470.v']
    (output/'results.json').write_text(json.dumps(dict(status='PASS',widths=[1,3,5,6,9,18],cases=rows,
        scope='Primitive2-state model simulation with connected CE, as in all MemD cache cells. No full MemD classification/integration, four-state proof, global barrier or physical snapshot.',
        source_hashes={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths}),indent=2)+'\n')


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--mutations',action='store_true');args=parser.parse_args();run(args.output.resolve(),args.mutations)
