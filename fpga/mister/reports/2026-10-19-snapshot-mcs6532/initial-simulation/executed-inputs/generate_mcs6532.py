#!/usr/bin/env python3
"""Actual MCS6532 wrapper state: complete RIOT plus original phase_d history.

Preserve the old-phase rising-edge enable and OE bus mask. Both wrapper and
RIOT freeze on the same externally confirmed boundary; no global barrier.
"""
import hashlib
from pathlib import Path
import generate_riot as riot

ROOT=Path(__file__).resolve().parents[3]
SOURCE=ROOT/'verilog/cells/cell_MCS6532.v'
SHA='5cc858c9465df03ee72bbb3a45e4b6ea6eab571872ae497285f3d32ed75337c0'


def generate(original=None):
    raw=SOURCE.read_bytes()if original is None else original
    if hashlib.sha256(raw).hexdigest()!=SHA:raise ValueError('unreviewed MCS6532 wrapper')
    text=raw.decode();child,manifest=riot.generate()
    assert text.count('module cell_MCS6532 (')==1 and text.count('    input  wire sys_clk,')==1
    text=text.replace('module cell_MCS6532 (','module cell_MCS6532_snapshot (',1)
    text=text.replace('    input  wire sys_clk,','''    input wire state_host_clk,state_advance,state_freeze,state_guest_held,state_request,state_write,
    input wire [7:0] state_index,input wire [17:0] state_data,
    output wire state_frozen,output reg state_done=1'b0,state_error=1'b0,
    output wire [17:0] state_result,
    input  wire sys_clk,''',1)
    normal='  always @(posedge sys_clk)phase_d<=p39;';assert text.count(normal)==1
    text=text.replace(normal,'''  reg phase_frozen=1'b0,response_vendor=1'b0;
  reg [17:0] phase_result;
  wire vendor_frozen,vendor_done,vendor_error;
  wire [17:0] vendor_result;
  wire state_hold=state_freeze && state_guest_held;
  assign state_frozen=phase_frozen && vendor_frozen;
  wire state_accept=state_hold && state_frozen && state_request;
  wire vendor_request=state_request && phase_frozen && state_index<8'd133;
  assign state_result=response_vendor ? vendor_result : phase_result;
  always @(posedge state_host_clk)begin
    phase_frozen<=state_hold;state_done<=state_accept;
    state_error<=state_accept && state_index>=8'd134;
    if(state_accept)begin
      response_vendor<=state_index<8'd133;
      phase_result<=18'b0;
      if(state_index==8'd133)begin
        if(state_write)phase_d<=state_data[0];
        else phase_result<={17'b0,phase_d};
      end
    end
    if(!state_hold && state_advance)phase_d<=p39;
  end''',1)
    anchor='  M6532 u_riot (';assert text.count(anchor)==1
    text=text.replace(anchor,'''  M6532_snapshot u_riot (
      .state_host_clk(state_host_clk),.state_advance(state_advance),
      .state_freeze(state_freeze),.state_guest_held(state_guest_held),
      .state_request(vendor_request),.state_write(state_write),
      .state_index(state_index),.state_data(state_data),
      .state_frozen(vendor_frozen),.state_done(vendor_done),
      .state_error(vendor_error),.state_result(vendor_result),''',1)
    return text,dict(module='cell_MCS6532_snapshot',logical_state_bits=1098,
        child_manifest=manifest,phase=dict(name='phase_d',width=1,word=133,initial=0),
        index_bits=8,valid_indices=list(range(134)),invalid_indices=list(range(134,256)),
        parameters=dict(child_init_7800=0),original_sha256=SHA,
        generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
        child_sha256=manifest['generated_sha256'],scope=__doc__),{'M6532_snapshot.sv':child}
