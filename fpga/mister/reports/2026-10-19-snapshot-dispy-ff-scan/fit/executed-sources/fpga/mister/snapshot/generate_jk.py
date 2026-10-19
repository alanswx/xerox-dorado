#!/usr/bin/env python3
"""Reviewed MC10135 state adapter for IFU/memory/display/network follow-up.

Preserve J/K setup, shared clock history and per-half PRECAP/S/R priority.
No production source or whole-machine freeze is changed here.
"""
import hashlib
from pathlib import Path
import re

import generate_snapshot_ff as ff
import generate_contb_full as contb

ROOT=Path(__file__).resolve().parents[3]
FIELDS=[('qa',1),('qb',1),('jk_pre',4),('ck_d',1)]
PROCESSES=[
    'always @(posedge sys_clk) jk_pre <= {p11,p10,p6,p7};',
    'always @(posedge sys_clk) ck_d <= p9;',
    '''always @(posedge sys_clk) begin
    if (p5)         qa <= 1'b1;
    else if (p4)    qa <= 1'b0;
    else if (ck_en) qa <= (ja & ~qa) | (~ka & qa);
  end''',
    '''always @(posedge sys_clk) begin
    if (p12)        qb <= 1'b1;
    else if (p13)   qb <= 1'b0;
    else if (ck_en) qb <= (jb & ~qb) | (~kb & qb);
  end''',
]


def generate(original=None):
    source=(ROOT/'verilog/cells/cell_MC10135.v').read_text()if original is None else original
    text=source;normal=[]
    for fragment in PROCESSES:
        text,process=ff.extract_once(text,fragment)
        normal.append(re.sub(r'^always\s*@\s*\(\s*posedge\s+sys_clk\s*\)\s*','',process,count=1))
    clean=ff.masked_comments(text)
    if re.search(r'\b(always\w*|initial|final)\b',clean):raise ValueError('extra J-K state process')
    registers={}
    for m in re.finditer(r'\breg\s+(?:\[(\d+):(\d+)\]\s*)?([^;]+);',clean):
        width=1 if m[1]is None else abs(int(m[1])-int(m[2]))+1
        for name in m[3].split(','):
            name=name.strip()
            if not re.fullmatch(r'\w+',name)or name in registers:raise ValueError('unreviewed J-K declaration')
            registers[name]=width
    if registers!=dict(FIELDS):raise ValueError('unexported J-K state')
    text=contb.replace_once(text,'module cell_MC10135 ','module cell_MC10135_snapshot ')
    text=contb.replace_once(text,'    input  wire sys_clk,','''    input wire state_host_clk,state_advance,state_hold,state_write,
    input wire [17:0] state_data,output wire [17:0] state_result,
    input  wire sys_clk,''')
    process='''
  assign state_result={11'b0,ck_d,jk_pre,qb,qa};
  always @(posedge state_host_clk) begin
    if (state_hold) begin
      if (state_write) {ck_d,jk_pre,qb,qa}<=state_data[6:0];
    end else if (state_advance) begin
'''+ '\n'.join(normal)+'''
    end
  end
'''
    result=contb.replace_once(text,'endmodule',process+'endmodule')
    return result,dict(kind='cell_MC10135',fields=FIELDS,modeled_state_bits=7,
        original_sha256=hashlib.sha256(source.encode()).hexdigest(),
        generated_sha256=hashlib.sha256(result.encode()).hexdigest(),
        scope='Primitive host-advance/state access; caller holds all consumers, no global machine barrier')
