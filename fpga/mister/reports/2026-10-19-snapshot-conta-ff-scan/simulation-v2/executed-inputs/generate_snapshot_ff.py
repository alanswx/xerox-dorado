#!/usr/bin/env python3
"""Reviewed host-advance/state adapters for the remaining ContA FF families.

Original pin/parameter/continuous logic remains intact. Only exact reviewed
nonblocking processes are merged under a host advance and frozen state load.
Any extra process or unexported register fails closed. Not production RTL.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

import generate_contb_full as contb

ROOT=Path(__file__).resolve().parents[3]
FIELDS={
    'cell_F10000':[('q',4),('d_pre',4),('ck_d',1)],
    'cell_F10016':[('q',4),('setup_pre',6),('ck_d',1)],
    'cell_MC10141':[('q',4),('d_pre',4),('s_pre',2),('dr_pre',1),('dl_pre',1),('ck_d',1)],
    'cell_MC10173':[('q',4)],
    'cell_SE10231':[('qb',1),('qa',1),('d_pre_b',1),('d_pre_a',1),('ckb_d',1),('cka_d',1)],
}
PROCESSES={
    'cell_F10000':[
        'always @(posedge sys_clk) ck_d <= clk;',
        'always @(posedge sys_clk) d_pre <= {p11,p10,p9,p7};',
        '''always @(posedge sys_clk) begin
          if (p12) q <= 4'd0;
          else if (ck_en) begin
            if (!p5) q <= PRECAP ? d_pre : {p11,p10,p9,p7};
            else q <= {q[2:0],p6};
          end
        end'''],
    'cell_F10016':[
        'always @(posedge sys_clk) ck_d <= p13;',
        'always @(posedge sys_clk) setup_pre <= {p6,p5,p11,p10,p9,p7};',
        '''always @(posedge sys_clk) begin
          if (p12) q <= 4'd0;
          else if (ck_en) begin
            if (!pe_n) q <= load_data;
            else if (!ce_n) q <= q + 4'd1;
          end
        end'''],
    'cell_MC10141':[
        'always @(posedge sys_clk) ck_d <= p4;',
        '''always @(posedge sys_clk) begin
          d_pre <= {p6,p9,p11,p12}; s_pre <= {p10,p7}; dr_pre <= p5; dl_pre <= p13;
        end''',
        '''always @(posedge sys_clk) if (ck_en) begin
          case (s_use)
            2'b00: q <= d_use;
            2'b01: q <= {dr_use,q[3:1]};
            2'b10: q <= {q[2:0],dl_use};
            default: q <= q;
          endcase
        end'''],
    'cell_MC10173':['always @(posedge sys_clk) if (!p7) q <= d;'],
    'cell_SE10231':[
        'always @(posedge sys_clk) begin cka_d <= clka; ckb_d <= clkb; end',
        'always @(posedge sys_clk) begin d_pre_a <= p7; d_pre_b <= p10; end',
        'always @(posedge sys_clk) if (cka_en) qa <= d_a;',
        '''always @(posedge sys_clk) begin
          if (p13) qb <= 1'b0;
          else if (ckb_en) qb <= d_b;
        end'''],
}


def masked_comments(text):
    return re.sub(r'/\*.*?\*/|//[^\n]*',lambda m:''.join('\n'if c=='\n'else' 'for c in m[0]),text,flags=re.S)


def tokens(text):
    # Token matching ignores only comments/whitespace; all code characters
    # must match a reviewed process. It is not a general Verilog parser.
    return list(re.finditer(r'\w+|[^\s]',masked_comments(text)))


def extract_once(source,fragment):
    full=tokens(source);part=[m[0]for m in tokens(fragment)];values=[m[0]for m in full]
    hits=[i for i in range(len(full)-len(part)+1)if values[i:i+len(part)]==part]
    if len(hits)!=1:raise ValueError('unreviewed/ambiguous state process: '+fragment[:80])
    i=hits[0];start,end=full[i].start(),full[i+len(part)-1].end()
    return source[:start]+source[end:],source[start:end]


def generate_one(kind,original=None):
    source=(ROOT/'verilog/cells'/f'{kind}.v').read_text()if original is None else original
    text=source;normal=[]
    for fragment in PROCESSES[kind]:
        text,process=extract_once(text,fragment)
        body=re.sub(r'^always\s*@\s*\(\s*posedge\s+sys_clk\s*\)\s*','',process,count=1)
        normal.append(body)
    clean=masked_comments(text)
    if re.search(r'\b(always\w*|initial|final)\b',clean):raise ValueError('extra state process: '+kind)
    registers={}
    for match in re.finditer(r'\breg\s+(?:\[(\d+):(\d+)\]\s*)?([^;]+);',clean):
        width=1 if match[1]is None else abs(int(match[1])-int(match[2]))+1
        for name in match[3].split(','):
            name=name.strip()
            if not re.fullmatch(r'\w+',name)or name in registers:raise ValueError('unreviewed register declaration')
            registers[name]=width
    if registers!=dict(FIELDS[kind]):raise ValueError('unexported/different registers: '+kind)
    text=contb.replace_once(text,'module '+kind+' ','module '+kind+'_snapshot ')
    text=contb.replace_once(text,'    input  wire sys_clk,','''    input wire state_host_clk,state_advance,state_hold,state_write,
    input wire [17:0] state_data,
    output wire [17:0] state_result,
    input  wire sys_clk,''')
    width=sum(w for _,w in FIELDS[kind])
    packed='{'+','.join(name for name,_ in reversed(FIELDS[kind]))+'}'
    process=f'''
  assign state_result = {{{18-width}'b0,{packed}}};
  always @(posedge state_host_clk) begin
    if (state_hold) begin
      if (state_write) {packed} <= state_data[{width-1}:0];
    end else if (state_advance) begin
'''+ '\n'.join(normal)+'''
    end
  end
'''
    text=contb.replace_once(text,'endmodule',process+'endmodule')
    return text


def generate():
    return {kind+'_snapshot.v':generate_one(kind)for kind in FIELDS}


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();args.output.mkdir(parents=True,exist_ok=False)
    generated=generate()
    for name,text in generated.items():(args.output/name).write_text(text)
    record=dict(scope=__doc__,fields=FIELDS,
        original_sources={kind:hashlib.sha256((ROOT/'verilog/cells'/f'{kind}.v').read_bytes()).hexdigest()for kind in FIELDS},
        generated={name:hashlib.sha256(text.encode()).hexdigest()for name,text in generated.items()})
    (args.output/'manifest.json').write_text(json.dumps(record,indent=2)+'\n')
