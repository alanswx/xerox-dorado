#!/usr/bin/env python3
"""Reviewed exact-advance/state clones of the remaining processor FF families.

Preserves the sampled transparent MC10175 latch and MC1668 S/R precedence
and both clock histories. This is an experiment, not production RTL.
"""
from pathlib import Path
import re

import generate_snapshot_ff as ff
import generate_contb_full as contb

ROOT=Path(__file__).resolve().parents[3]
FIELDS={
    'cell_MC10175':[('q',5)],
    'cell_MC1668':[('qa',1),('qb',1),('cka_d',1),('ckb_d',1)],
}
PROCESSES={
    'cell_MC10175':['''always @(posedge sys_clk) begin
    if (clk) begin
      if (p11) q <= 5'd0;
    end else
      q <= {p5, p9, p13, p12, p10};
  end'''],
    'cell_MC1668':[
        'always @(posedge sys_clk) begin cka_d <= p7; ckb_d <= p9; end',
        '''always @(posedge sys_clk) begin
    if (p5) qa <= 1'b1;
    else if (p4) qa <= 1'b0;
    else if (cka_en) qa <= p6;
  end''',
        '''always @(posedge sys_clk) begin
    if (p12) qb <= 1'b1;
    else if (p13) qb <= 1'b0;
    else if (ckb_en) qb <= p11;
  end'''],
}


def generate_one(kind,original=None):
    source=(ROOT/'verilog/cells'/f'{kind}.v').read_text() if original is None else original
    text=source;normal=[]
    for fragment in PROCESSES[kind]:
        text,process=ff.extract_once(text,fragment)
        normal.append(re.sub(r'^always\s*@\s*\(\s*posedge\s+sys_clk\s*\)\s*','',process,count=1))
    clean=ff.masked_comments(text)
    if re.search(r'\b(always\w*|initial|final)\b',clean):
        raise ValueError('extra processor state process: '+kind)
    registers={}
    for match in re.finditer(r'\breg\s+(?:\[(\d+):(\d+)\]\s*)?([^;]+);',clean):
        width=1 if match[1] is None else abs(int(match[1])-int(match[2]))+1
        for name in match[3].split(','):
            name=name.strip()
            if not re.fullmatch(r'\w+',name) or name in registers:
                raise ValueError('unreviewed processor register declaration')
            registers[name]=width
    if registers!=dict(FIELDS[kind]):
        raise ValueError('unexported/different processor registers: '+kind)
    text=contb.replace_once(text,'module '+kind+' ','module '+kind+'_snapshot ')
    text=contb.replace_once(text,'    input  wire sys_clk,','''    input wire state_host_clk,state_advance,state_hold,state_write,
    input wire [17:0] state_data,
    output wire [17:0] state_result,
    input  wire sys_clk,''')
    width=sum(w for _,w in FIELDS[kind]);packed='{'+','.join(n for n,_ in reversed(FIELDS[kind]))+'}'
    process=f'''
  assign state_result={{{18-width}'b0,{packed}}};
  always @(posedge state_host_clk) begin
    if (state_hold) begin
      if (state_write) {packed}<=state_data[{width-1}:0];
    end else if (state_advance) begin
'''+ '\n'.join(normal)+'''
    end
  end
'''
    return contb.replace_once(text,'endmodule',process+'endmodule')


def generate():
    return {kind+'_snapshot.v':generate_one(kind) for kind in FIELDS}
