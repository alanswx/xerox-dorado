#!/usr/bin/env python3
"""Preserve both signed SN74123 timers and six trigger histories in70bits.

Four18bit words, valid0..3, invalid4..7; final word has16data bits. Caller
holds every consumer. Original timing arithmetic/parameters are unchanged.
"""
import hashlib,re
from pathlib import Path
import generate_snapshot_ff as ff
ROOT=Path(__file__).resolve().parents[3]
SHA='df8a9267e3af37fb8de2f4c3f5d83d73ba4315dd58bcad87d3c1c319b8ec880b'
PROCESS='''always @(posedge sys_clk) begin
    a1_d <= p1; b1_d <= p2; c1_d <= p3; a2_d <= p9; b2_d <= p10; c2_d <= p11;
    if (!p3) t1 <= 0;
    else if (trig1) t1 <= TW;
    else if (t1 > 0) t1 <= t1 - 1;
    if (!p11) t2 <= 0;
    else if (trig2) t2 <= TW;
    else if (t2 > 0) t2 <= t2 - 1;
  end'''
FIELDS=[('t1',32),('t2',32),('a1_d',1),('b1_d',1),('c1_d',1),('a2_d',1),('b2_d',1),('c2_d',1)]


def generate(original=None):
 source=(ROOT/'verilog/cells/cell_SN74123.v').read_text()if original is None else original
 if hashlib.sha256(source.encode()).hexdigest()!=SHA:raise ValueError('unreviewed original SN74123')
 text,process=ff.extract_once(source,PROCESS)
 if re.search(r'\b(always\w*|initial|final)\b',ff.masked_comments(text)):raise ValueError('extra timer process')
 normal=re.sub(r'^always\s*@\s*\(\s*posedge\s+sys_clk\s*\)\s*','',process,count=1)
 assert text.count('module cell_SN74123 #(')==1 and text.count('    input  wire sys_clk,')==1
 text=text.replace('module cell_SN74123 #(','module cell_SN74123_snapshot #(',1)
 text=text.replace('    input  wire sys_clk,','''    input wire state_host_clk,state_advance,state_freeze,state_guest_held,state_request,state_write,
    input wire [2:0] state_index,input wire [17:0] state_data,
    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,
    output reg [17:0] state_result,
    input  wire sys_clk,''',1)
 extra='''
 wire state_hold=state_freeze && state_guest_held;
 wire state_accept=state_hold && state_frozen && state_request;
 wire state_valid=!state_index[2];
 always @(posedge state_host_clk)begin
  state_frozen<=state_hold;state_done<=state_accept;state_error<=state_accept && !state_valid;
  if(state_accept)begin
   state_result<=18'b0;
   if(state_valid)begin
    if(state_write)begin
     case(state_index[1:0])
      0:t1[17:0]<=state_data;
      1:{t2[3:0],t1[31:18]}<=state_data;
      2:t2[21:4]<=state_data;
      3:{c2_d,b2_d,a2_d,c1_d,b1_d,a1_d,t2[31:22]}<=state_data[15:0];
     endcase
    end else begin
     case(state_index[1:0])
      0:state_result<=t1[17:0];
      1:state_result<={t2[3:0],t1[31:18]};
      2:state_result<=t2[21:4];
      3:state_result<={2'b0,c2_d,b2_d,a2_d,c1_d,b1_d,a1_d,t2[31:22]};
     endcase
    end
   end
  end
  if(!state_hold && state_advance)begin
'''+normal+'''
  end
 end
'''
 text=text.replace('endmodule',extra+'endmodule',1)
 fields=[];lsb=0
 for name,width in FIELDS:fields.append(dict(name=name,width=width,lsb=lsb,signed=name in ('t1','t2')));lsb+=width
 return text,dict(module='cell_SN74123_snapshot',logical_state_bits=70,fields=fields,
  words=[dict(index=i,width=18 if i<3 else 16)for i in range(4)],valid_indices=list(range(4)),invalid_indices=list(range(4,8)),
  original_sha256=SHA,generated_sha256=hashlib.sha256(text.encode()).hexdigest(),scope=__doc__)

if __name__=='__main__':
 import argparse,json
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False);s,m=generate();(a.output/'cell_SN74123_snapshot.sv').write_text(s);(a.output/'manifest.json').write_text(json.dumps(m,indent=2)+'\n')
