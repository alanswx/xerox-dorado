#!/usr/bin/env python3
"""Exact modeled F9401 CRC/PRECAP state in two bounded18-bit words.

Preserves all19bits, reset/preset/falling-edge order, setup history and the
original polynomial selection. Caller holds all consumers; no global barrier.
"""
import hashlib,re
from pathlib import Path
import generate_snapshot_ff as ff
ROOT=Path(__file__).resolve().parents[3]
SHA='5ce6f28490c85eaf51dfe2757d6df84773f8cfc6c8901265c3fd2ac467b7b17d'
PROCESSES=['always @(posedge sys_clk)begin d_pre<=p11;cwe_pre<=p10;end',
'''always @(posedge sys_clk) begin
    cp_d <= p1;
    if (p4)             r <= 16'd0;          // MR
    else if (!p2)       r <= 16'hFFFF;       // P'
    else if (cp_d && !p1) r <= shifted;      // CP high-to-low
  end''']


def generate(original=None):
    source=(ROOT/'verilog/cells/cell_F9401.v').read_text()if original is None else original
    if hashlib.sha256(source.encode()).hexdigest()!=SHA:raise ValueError('unreviewed original CRC9401')
    text=source;normal=[]
    for fragment in PROCESSES:
        text,process=ff.extract_once(text,fragment);normal.append(re.sub(r'^always\s*@\s*\(\s*posedge\s+sys_clk\s*\)\s*','',process,count=1))
    if re.search(r'\b(always\w*|initial|final)\b',ff.masked_comments(text)):raise ValueError('extra CRC process')
    assert text.count('module cell_F9401 #(')==1 and text.count('    input  wire sys_clk,')==1
    text=text.replace('module cell_F9401 #(','module cell_F9401_snapshot #(',1).replace('    input  wire sys_clk,','''    input wire state_host_clk,state_advance,state_freeze,state_guest_held,state_request,state_write,
    input wire [1:0] state_index,input wire [17:0] state_data,
    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,
    output reg [17:0] state_result,
    input  wire sys_clk,''',1)
    fabric='''
  wire state_hold=state_freeze && state_guest_held;
  wire state_accept=state_hold && state_frozen && state_request;
  wire state_valid=!state_index[1];
  always @(posedge state_host_clk)begin
    state_frozen<=state_hold;state_done<=state_accept;state_error<=state_accept && !state_valid;
    if(state_accept)begin
      state_result<=18'b0;
      if(state_valid)begin
        if(state_write)begin
          if(!state_index[0]){d_pre,cp_d,r}<=state_data;
          else cwe_pre<=state_data[0];
        end else if(!state_index[0])state_result<={d_pre,cp_d,r};
        else state_result<={17'b0,cwe_pre};
      end
    end
    if(!state_hold && state_advance)begin
'''+ '\n'.join(normal)+'''
    end
  end
'''
    text=text.replace('endmodule',fabric+'endmodule',1)
    return text,dict(module='cell_F9401_snapshot',logical_state_bits=19,fields=[dict(name='r',width=16,lsb=0),dict(name='cp_d',width=1,lsb=16),dict(name='d_pre',width=1,lsb=17),dict(name='cwe_pre',width=1,lsb=18)],
        words=[dict(index=0,width=18),dict(index=1,width=1)],valid_indices=[0,1],invalid_indices=[2,3],original_sha256=SHA,generated_sha256=hashlib.sha256(text.encode()).hexdigest(),scope=__doc__)
