#!/usr/bin/env python3
"""Preserve actual 2716 ROM bytes/unknowns and export its nine read-pipeline bits.

ROM initialization and pin equations stay unchanged. Caller holds every consumer;
this is a primitive adapter, not a complete machine barrier or file format.
"""
import hashlib,re
from pathlib import Path
import generate_snapshot_ff as ff
ROOT=Path(__file__).resolve().parents[3]
SHA='05b1a05547819d302561e981a88a8a36bc4661a139dbc394eb0bc3769ca9cb57'
PROCESS='''always @(posedge sys_clk) begin
    d    <= mem[a];
    en_q <= ~p20 & ~p18;                  // CS' and PD' both low
  end'''
IMAGES={
 'b61':'2be73b7a1aa3c4331d00e6e66e154e93c6f155582e69fd23d6281a97beb9fcfa',
 'c61':'b6a3d3827f8ab2b087f587b9e4ee442aae90c91cd6955e036bbb7d29c2b7cb89',
 'e60':'c162cd3b6a11c488be5ffcbabec9065a322809a19c6ca8a9d4abd7384aa30d6b',
 'f60':'25033c349a69496bc82aaa09781733b2b7f3870369e20d54df1ec9a5f4ac0028'}
SOCKETS=('b60','b61','c60','c61','e60','f60','h61','i61')


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def images(root=ROOT):
 paths={pkg:root/'verilog/proms/eprom'/('BaseBd-'+pkg+'.mem')for pkg in IMAGES}
 for pkg,p in paths.items():
  if digest(p)!=IMAGES[pkg]:raise ValueError('unreviewed original EPROM image:'+pkg)
 return paths


def generate(original=None):
 source=(ROOT/'verilog/cells/cell_i2716.v').read_text()if original is None else original
 if hashlib.sha256(source.encode()).hexdigest()!=SHA:raise ValueError('unreviewed original 2716')
 text,process=ff.extract_once(source,PROCESS)
 init='initial if (INIT_FILE != "") $readmemh(INIT_FILE, mem);'
 if text.count(init)!=1:raise ValueError('2716 initialization differs')
 residue=text.replace(init,'',1)
 if re.search(r'\b(always\w*|initial|final)\b',ff.masked_comments(residue)):raise ValueError('extra 2716 process')
 normal=re.sub(r'^always\s*@\s*\(\s*posedge\s+sys_clk\s*\)\s*','',process,count=1)
 assert text.count('module cell_i2716 #(')==1 and text.count('    input  wire sys_clk,')==1
 text=text.replace('module cell_i2716 #(','module cell_i2716_snapshot #(',1).replace('    input  wire sys_clk,','''    input wire state_host_clk,state_advance,state_freeze,state_guest_held,state_request,state_write,
    input wire [1:0] state_index,input wire [17:0] state_data,
    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,
    output reg [17:0] state_result,
    input  wire sys_clk,''',1)
 text=text.replace('reg [7:0] d;',"reg [7:0] d;\n  reg [7:0] restored_d;reg restore_pending=1'b0;\n  wire [7:0] logical_d=restore_pending?restored_d:d;",1)
 text=text.replace("en_q ? d : 8'h00","en_q ? logical_d : 8'h00",1)
 fabric='''
  wire state_hold=state_freeze && state_guest_held;
  wire state_accept=state_hold && state_frozen && state_request;
  wire state_valid=state_index==2'b00;
  always @(posedge state_host_clk)begin
    state_frozen<=state_hold;state_done<=state_accept;state_error<=state_accept && !state_valid;
    if(state_accept)begin
      state_result<=18'b0;
      if(state_valid)begin
        if(state_write)begin en_q<=state_data[8];restored_d<=state_data[7:0];restore_pending<=1'b1;end
        else state_result<={9'b0,en_q,logical_d};
      end
    end
    if(!state_hold && state_advance)begin
'''+normal+'''
      restore_pending<=1'b0;
    end
  end
'''
 text=text.replace('endmodule',fabric+'endmodule',1)
 return text,dict(module='cell_i2716_snapshot',logical_state_bits=9,fields=[dict(name='d',width=8,lsb=0),dict(name='en_q',width=1,lsb=8)],
  words=[dict(index=0,width=9)],valid_indices=[0],invalid_indices=[1,2,3],original_sha256=SHA,
  generated_sha256=hashlib.sha256(text.encode()).hexdigest(),scope=__doc__,read_pipeline_view='logical_d',rom='Immutable initialization unchanged; empty INIT_FILE remains uninitialized, never forced to zero')
