#!/usr/bin/env python3
"""Four actual-width 2125 byte banks against untouched slices and RAM oracles."""
import argparse,hashlib,json,subprocess
from pathlib import Path
import audit_baseboard_ram as grouping

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
FIXTURE='''module i2125_fixture(
 input host_clk,ref_clk,advance,freeze,held,request,write,
 input [9:0] address,input [7:0] data,input [3:0] ce_n,we_n,
 input [12:0] index,input [17:0] state_data,
 output [3:0] frozen,done,error,
 output [7:0] q[4],original_q[4],raw_q[4],original_raw[4],
 output [17:0] result[4],output [7:0] physical[4][1024]);
 genvar k,b,a;
 generate for(k=0;k<4;k=k+1)begin:banks
  dorado_snapshot_i2125_byte_bank candidate(
   .host_clk(host_clk),.guest_advance(advance),.address(address),.data(data),
   .ce_n(ce_n[k]),.we_n(we_n[k]),.q(q[k]),.freeze(freeze),.guest_held(held),.frozen(frozen[k]),
   .request(request && index[12:11]==k),.write(write),.word_index(index[10:0]),
   .state_data(state_data),.done(done[k]),.error(error[k]),.result(result[k]));
  assign raw_q[k]=candidate.memory.logical_q;
  for(a=0;a<1024;a=a+1)begin:words
   assign physical[k][a]=candidate.memory.mem[a];
  end
  for(b=0;b<8;b=b+1)begin:bits
   cell_i2125 original(.sys_clk(ref_clk),.p13(address[9]),.p12(address[8]),
    .p11(address[7]),.p10(address[6]),.p9(address[5]),.p6(address[4]),
    .p5(address[3]),.p4(address[2]),.p3(address[1]),.p2(address[0]),
    .p1(ce_n[k]),.p14(we_n[k]),.p15(data[b]),.p7(original_q[k][b]),.p8(1'b0),.p16(1'b1));
   assign original_raw[k][b]=original.dout_r;
  end
 end endgenerate
endmodule
'''


def run(out,mutations):
 out.mkdir(parents=True,exist_ok=False)
 paths=[Path(__file__),HERE/'test_i2125.cpp',HERE/'i2125_byte_bank.sv',HERE/'f10415_word_bank.sv',ROOT/'verilog/cells/cell_i2125.v']
 inputs=out/'executed-inputs';inputs.mkdir()
 for p in paths:(inputs/p.name).write_bytes(p.read_bytes())
 assert hashlib.sha256((inputs/'cell_i2125.v').read_bytes()).hexdigest()==grouping.CELL_SHA
 assert hashlib.sha256((inputs/'f10415_word_bank.sv').read_bytes()).hexdigest()==grouping.CORE_SHA
 wrapper=(inputs/'i2125_byte_bank.sv').read_text();core=(inputs/'f10415_word_bank.sv').read_text()
 cases=[('candidate','core',None,None,None)]
 if mutations:cases +=[
  ('unmasked_write','wrapper',"we_n ? bank_q : 8'b0",'bank_q','2125 output differs'),
  ('ungated_read','core','scan_active || (!freeze_active && guest_advance)',"1'b1",'2125 raw pipeline differs'),
  ('ungated_write','core','(!freeze_active && guest_advance && !ce_n && !we_n)','(!freeze_active && !ce_n && !we_n)','2125 RAM differs'),
  ('lost_pipeline','core','saved_q<=state_data[WIDTH-1:0];',"saved_q<='0;",'2125 raw pipeline differs'),
  ('premature_resume','core','if(!freeze_active && guest_advance)pending','if(!freeze_active)pending','2125 raw pipeline differs'),
  ('running_access','core','wire accepted=scan_active && request;','wire accepted=request;','2125 handshake differs'),
  ('invalid_alias','core',"wire valid_index=word_index<=11'd1024;","wire valid_index=1'b1;",'2125 handshake differs'),
  ('new_data','core','if(read_enable)ram_q<=mem[ram_address];','if(read_enable)ram_q<=ram_write?ram_data:mem[ram_address];','2125 raw pipeline differs'),
  ('scan_address_alias','core','scan_active?word_index[9:0]:address',"scan_active?10'b0:address",'2125 RAM differs')]
 results=[]
 for name,target,before,after,why in cases:
  d=out/name;d.mkdir();w,c=wrapper,core
  if before:
   text=w if target=='wrapper'else c;assert text.count(before)==1,(name,before);text=text.replace(before,after,1)
   if target=='wrapper':w=text
   else:c=text
  (d/'fixture.sv').write_text(FIXTURE);(d/'i2125_byte_bank.sv').write_text(w);(d/'f10415_word_bank.sv').write_text(c)
  cmd=['verilator','--cc','--exe','--build','-j','4','--top-module','i2125_fixture','-Wno-fatal','--Mdir',str(d/'obj'),
       str(d/'fixture.sv'),str(d/'i2125_byte_bank.sv'),str(d/'f10415_word_bank.sv'),str(inputs/'cell_i2125.v'),str(inputs/'test_i2125.cpp')]
  (d/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
  with(d/'build.log').open('w')as f:subprocess.run(cmd,check=True,stdout=f,stderr=subprocess.STDOUT)
  p=subprocess.run([str(d/'obj/Vi2125_fixture')],capture_output=True,text=True,timeout=60)
  text=p.stdout+p.stderr;(d/'test.txt').write_text(text);print(name+': '+text.strip(),flush=True)
  if why:assert p.returncode and why in text,(name,text)
  else:assert not p.returncode and 'I2125 PASS'in text,text
  results.append(dict(case=name,returncode=p.returncode,expected_failure=why))
 (out/'results.json').write_text(json.dumps(dict(status='PASS',cases=results,logical_state_bits=32800,
  scope='Four1024x8groups and32read bits, two-state original model/oracle equivalence; no board/global freeze or physical machine snapshot',
  source_hashes={str(p.relative_to(ROOT)):hashlib.sha256((inputs/p.name).read_bytes()).hexdigest()for p in paths}),indent=2)+'\n')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true')
 a=p.parse_args();run(a.output.resolve(),a.mutations)
