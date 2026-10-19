#!/usr/bin/env python3
"""Exact-advance/state access for a single original MosRam socket.

All independent row/column, edge, pending-write, read-pipeline and output
state is preserved. Supports the reviewed14/16-bit address configurations;
no address/history state is merged across sockets. No global barrier.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

import generate_contb_full as contb
import generate_snapshot_ff as ff

ROOT=Path(__file__).resolve().parents[3]
ORIGINAL_SHA='f6038d02e45ab7ba89054d6248b65ab2bd70b1b92e12c933cbfc589f7a60a26d'
PROCESSES=[
 'always @(posedge sys_clk) begin ras_d <= p4; cas_d <= p15; end',
 '''always @(posedge sys_clk) begin
    if (~p4  & ras_d) row <= a;
    if (~p15 & cas_d) col <= a;
  end''',
 '''always @(posedge sys_clk) begin
    we_d <= p3;
    write_pending <= !p4 && !p15 && !p3 && (cas_d || we_d);
    if (!p4 && !p15 && !p3 && (cas_d || we_d)) write_data <= p2;
  end''',
 '''always @(posedge sys_clk) begin
    dout_r <= mem[addr];
    if (write_pending) mem[addr] <= write_data;
  end''',
 '''always @(posedge sys_clk)
    if (!p4 && !p15 && p3) dout <= dout_r;''',
]


def manifest(address_bits):
    if address_bits not in (14,16):raise ValueError('unqualified MosRam address width')
    depth=1<<address_bits;offset=0;fields=[]
    for name,width in [('row',address_bits//2),('col',address_bits//2),('ras_d',1),('cas_d',1),
                       ('dout',1),('dout_r',1),('we_d',1),('write_pending',1),('write_data',1)]:
        fields.append(dict(name=name,width=width,lsb=offset));offset+=width
    return dict(module='cell_MosRam_snapshot',address_bits=address_bits,index_bits=address_bits+1,
        words=depth,width=1,transport_bits=18,ram=dict(words=depth,width=1,indices=[0,depth-1]),
        history=dict(first_word=depth,words=2,width=offset,fields=fields),
        modeled_state_bits=depth+offset,original_sha256=ORIGINAL_SHA,
        valid_indices=[0,depth+1],invalid_indices=[depth+2,2*depth-1],
        scope='Single original14/16-bit MosRam socket; complete modeled bit state, not global consumers/phase/backend or machine snapshots')


def generate(original=None):
    if original is None:original=(ROOT/'verilog/cells/cell_MosRam.v').read_text()
    if hashlib.sha256(original.encode()).hexdigest()!=ORIGINAL_SHA:raise ValueError('unreviewed original MosRam')
    text=original
    for fragment in PROCESSES:text,_=ff.extract_once(text,fragment)
    if re.search(r'\b(always\w*|initial|final)\b',ff.masked_comments(text)):
        raise ValueError('extra original MosRam state process')
    text=contb.replace_once(text,'module cell_MosRam #','module cell_MosRam_snapshot #')
    text=contb.replace_once(text,'    input  wire sys_clk,','''    input wire state_host_clk,state_advance,state_freeze,state_guest_held,
    input wire state_request,state_write,
    input wire [ADDR_BITS:0] state_index,input wire [17:0] state_data,
    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,
    output wire [17:0] state_result,
    input  wire sys_clk,''')
    fabric='''
  generate if(ADDR_BITS!=14 && ADDR_BITS!=16)begin:unsupported_density
    initial $fatal(1,"MosRam snapshot ADDR_BITS must be14 or16");
  end endgenerate
  localparam integer DEPTH=(1<<ADDR_BITS);
  localparam integer HISTORY_BITS=ADDR_BITS+7;
  wire state_hold=state_freeze && state_guest_held;
  wire state_scan=state_hold && state_frozen;
  wire state_accept=state_scan && state_request;
  wire state_is_ram=!state_index[ADDR_BITS];
  wire state_valid=state_is_ram || state_index<=DEPTH+1;
  wire state_history_write=state_accept && state_write && state_valid && !state_is_ram;
  wire state_read_enable=state_scan || (!state_hold && state_advance);
  wire state_ram_write=state_scan?(state_accept && state_write && state_is_ram):
    (!state_hold && state_advance && write_pending);
  wire [ADDR_BITS-1:0] state_ram_address=state_scan?state_index[ADDR_BITS-1:0]:addr;
  wire state_ram_data=state_scan?state_data[0]:write_data;
  reg saved_dout_r;
  reg restore_pending=1'b0;
  wire logical_dout_r=restore_pending?saved_dout_r:dout_r;
  wire [HISTORY_BITS-1:0] state_history={write_data,write_pending,we_d,
    logical_dout_r,dout,cas_d,ras_d,col,row};
  wire [HISTORY_BITS-1:0] state_restored_history=state_index[0]?
    {state_data[HISTORY_BITS-19:0],state_history[17:0]}:
    {state_history[HISTORY_BITS-1:18],state_data};
  reg state_response_ram;
  reg [17:0] state_aux_result;
  assign state_result=state_response_ram?{17'b0,dout_r}:state_aux_result;
  always @(posedge state_host_clk)begin
    // Same old-data RAM template as the original. Administrative reads borrow
    // this port; saved_dout_r keeps its separate logical guest read pipeline.
    if(state_read_enable)dout_r<=mem[state_ram_address];
    if(state_ram_write)mem[state_ram_address]<=state_ram_data;
    state_frozen<=state_hold;state_done<=state_accept;
    state_error<=state_accept && !state_valid;
    if(state_accept)begin
      state_response_ram<=state_is_ram;
      state_aux_result<=state_index[0]?{{(36-HISTORY_BITS){1'b0}},state_history[HISTORY_BITS-1:18]}:state_history[17:0];
    end
    if(state_hold && !state_frozen)begin
      saved_dout_r<=logical_dout_r;restore_pending<=1'b1;
    end
    if(state_hold)begin
      if(state_history_write)begin
        {write_data,write_pending,we_d,saved_dout_r,dout,cas_d,ras_d,col,row}<=state_restored_history;
      end
    end else if(state_advance)begin
      restore_pending<=1'b0;
      ras_d<=p4;cas_d<=p15;
      if(~p4 & ras_d)row<=a;
      if(~p15 & cas_d)col<=a;
      we_d<=p3;
      write_pending<=!p4 && !p15 && !p3 && (cas_d || we_d);
      if(!p4 && !p15 && !p3 && (cas_d || we_d))write_data<=p2;
      if(!p4 && !p15 && p3)dout<=logical_dout_r;
    end
  end
'''
    return contb.replace_once(text,'endmodule',fabric+'\nendmodule')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    a.output.mkdir(parents=True,exist_ok=False);s=generate();(a.output/'cell_MosRam_snapshot.sv').write_text(s)
    (a.output/'manifest.json').write_text(json.dumps({str(w):manifest(w)for w in (14,16)},indent=2)+'\n')
