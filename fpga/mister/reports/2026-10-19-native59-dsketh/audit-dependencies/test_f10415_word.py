#!/usr/bin/env python3
"""Width-correct old-data RAM against untouched cells and independent oracles."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
FIXTURE='''module word_bank_fixture(
 input wire host_clk,ref_clk,advance,freeze,held,request,write,
 input wire [9:0] address,input wire [17:0] data,
 input wire ce_n,we_n,input wire [10:0] word_index,
 input wire [17:0] state_data[10],
 output wire [9:0] frozen,done,error,
 output wire [17:0] q[10],original_q[10],result[10]);
 function integer width_for(input integer n);
  case(n%5)0:width_for=1;1:width_for=2;2:width_for=4;3:width_for=5;default:width_for=18;endcase
 endfunction
 genvar k,b;
 generate for(k=0;k<10;k=k+1)begin:banks
  localparam integer W=width_for(k);
  wire [W-1:0] q_w,original_w;
  if(k<5)begin:connected
   dorado_snapshot_f10415_word_bank #(.WIDTH(W)) candidate(
    .host_clk(host_clk),.guest_advance(advance),.address(address),.data(data[W-1:0]),
    .ce_n(ce_n),.we_n(we_n),.q(q_w),.freeze(freeze),.guest_held(held),.frozen(frozen[k]),
    .request(request),.write(write),.word_index(word_index),.state_data(state_data[k]),
    .done(done[k]),.error(error[k]),.result(result[k]));
   for(b=0;b<W;b=b+1)begin:bits
    cell_F10415A original(ref_clk,address[9],address[8],address[7],address[6],address[5],
      address[4],address[3],address[2],address[1],address[0],data[b],ce_n,we_n,original_w[b]);
   end
  end else begin:omitted
   // Preserve the original IFUM omission in BOTH interfaces. No explicit
   // zero tie-off or four-state/X equivalence is claimed by Verilator.
   dorado_snapshot_f10415_word_bank #(.WIDTH(W)) candidate(
    .host_clk(host_clk),.guest_advance(advance),.address(address),.data(data[W-1:0]),
    .we_n(we_n),.q(q_w),.freeze(freeze),.guest_held(held),.frozen(frozen[k]),
    .request(request),.write(write),.word_index(word_index),.state_data(state_data[k]),
    .done(done[k]),.error(error[k]),.result(result[k]));
   for(b=0;b<W;b=b+1)begin:bits
    cell_F10415A original(.sys_clk(ref_clk),.p2(address[9]),.p3(address[8]),.p4(address[7]),
      .p5(address[6]),.p6(address[5]),.p7(address[4]),.p9(address[3]),.p10(address[2]),
      .p11(address[1]),.p12(address[0]),.p15(data[b]),.p13(we_n),.p1(original_w[b]));
   end
  end
  assign q[k]={{(18-W){1'b0}},q_w};
  assign original_q[k]={{(18-W){1'b0}},original_w};
 end endgenerate
endmodule
'''


def run(output,mutations):
    output.mkdir(parents=True,exist_ok=False);source=(HERE/'f10415_word_bank.sv').read_text()
    cases=[('candidate',None,None,None)]
    if mutations:
        cases+=[
            ('ungated_read','scan_active || (!freeze_active && guest_advance)',"1'b1",'backend hold changed read pipeline'),
            ('ungated_write','(!freeze_active && guest_advance && !ce_n && !we_n)','(!freeze_active && !ce_n && !we_n)','ordinary old-data oracle differs'),
            ('premature_resume','if(!freeze_active && guest_advance)pending','if(!freeze_active)pending','backend hold changed read pipeline'),
            ('lost_pipeline','saved_q<=state_data[WIDTH-1:0];',"saved_q<='0;",'bit/padding write readback differs'),
            ('running_access','wire accepted=scan_active && request;','wire accepted=request;','running state request accepted'),
            ('invalid_alias',"wire valid_index=word_index<=11'd1024;","wire valid_index=1'b1;",'invalid index accepted'),
            ('wrong_data','state_data[WIDTH-1:0]:data',"{WIDTH{1'b0}}:data",'bit/padding write readback differs'),
            ('wrong_read_address','ram_q<=mem[ram_address];',"ram_q<=mem[10'b0];",'ordinary old-data oracle differs'),
        ]
    rows=[]
    for name,before,after,diagnostic in cases:
        case=output/name;case.mkdir();text=source
        if before:
            assert text.count(before)==1,(name,before);text=text.replace(before,after)
        (case/'fixture.sv').write_text(FIXTURE);(case/'bank.sv').write_text(text)
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','word_bank_fixture','-Wno-fatal',
            '-DDORADO_F10415_M10K','--Mdir',str(case/'obj'),str(case/'fixture.sv'),str(case/'bank.sv'),
            str(ROOT/'verilog/cells/cell_F10415A.v'),str(HERE/'test_f10415_word.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with (case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vword_bank_fixture')],capture_output=True,text=True,timeout=120)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text)
        if diagnostic:assert result.returncode and diagnostic in text,(name,text)
        else:assert result.returncode==0 and 'F10415 WORD PASS'in text,text
        rows.append(dict(case=name,returncode=result.returncode,expected_failure=diagnostic));print(name+': '+text.strip(),flush=True)
    paths=[Path(__file__).resolve(),HERE/'test_f10415_word.cpp',HERE/'f10415_word_bank.sv',ROOT/'verilog/cells/cell_F10415A.v']
    (output/'results.json').write_text(json.dumps(dict(status='PASS',widths=[1,2,4,5,18],cases=rows,
        scope='Primitive2-state model simulation: connected CE and original omitted CE preserved; no explicit tie-off/four-state claim, IFU integration/global barrier or physical snapshot.',
        source_hashes={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths}),indent=2)+'\n')


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--mutations',action='store_true');args=parser.parse_args();run(args.output.resolve(),args.mutations)
