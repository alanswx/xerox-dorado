#!/usr/bin/env python3
"""Preserve the frozen MemC ALU's four sampled output-latch bits.

Arithmetic and carry/lookahead logic remain byte-identical to the original
model, including its documented lack of BCD correction. No clock migration
or correction of the original cell is performed here.
"""
import hashlib
from pathlib import Path
import re

import generate_contb_full as contb
import generate_snapshot_ff as ff

ROOT=Path(__file__).resolve().parents[3]
ORIGINAL_SHA='70e30781ed4372459bb63580fd1b955bb54568e70bc722e0b73ce9e46546cc27'
FIELDS=[('q',4)]


def generate(original=None):
    source=(ROOT/'verilog/cells/cell_F100181.v').read_text()if original is None else original
    if hashlib.sha256(source.encode()).hexdigest()!=ORIGINAL_SHA:
        raise ValueError('unreviewed F100181 source')
    text,process=ff.extract_once(source,'always @(posedge sys_clk) if (!p19) q <= f;')
    # The other process is the original combinational ALU function table.
    clean=ff.masked_comments(text)
    if len(re.findall(r'\balways\s*@\s*\*',clean))!=1 or \
            re.search(r'\b(initial|final|always_ff|always_latch)\b',clean)or \
            len(re.findall(r'\balways\b',clean))!=1:
        raise ValueError('extra F100181 state process')
    text=contb.replace_once(text,'module cell_F100181 ','module cell_F100181_snapshot ')
    text=contb.replace_once(text,'    input  wire sys_clk,','''    input wire state_host_clk,state_advance,state_hold,state_write,
    input wire [17:0] state_data,output wire [17:0] state_result,
    input  wire sys_clk,''')
    body=re.sub(r'^always\s*@\s*\(\s*posedge\s+sys_clk\s*\)\s*','',process,count=1)
    fabric='''
  assign state_result={14'b0,q};
  always @(posedge state_host_clk) begin
    if (state_hold) begin
      if (state_write) q<=state_data[3:0];
    end else if (state_advance) begin
'''+body+'''
    end
  end
'''
    result=contb.replace_once(text,'endmodule',fabric+'endmodule')
    return result,dict(kind='cell_F100181',fields=FIELDS,modeled_state_bits=4,
        original_sha256=ORIGINAL_SHA,generated_sha256=hashlib.sha256(result.encode()).hexdigest(),
        scope='One sampled ALU output latch; carry/lookahead remain combinational. No complete MemC/global snapshot or original BCD accuracy claim.')
