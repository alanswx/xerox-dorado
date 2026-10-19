#!/usr/bin/env python3
"""Exact modeled K1115A accumulator state; no oscillator-rate/timer correction.

The original signed 32-bit accumulator, output and parameter arithmetic are
preserved, including wrap behavior. Caller must hold every consumer/input.
"""
import argparse
import hashlib
import json
from pathlib import Path

ROOT=Path(__file__).resolve().parents[3]
SHA='7f187187b8b39d9ffa3070596bdeca43c84489f8f7b4b6c24245603aa6fcfe32'
PROCESS='''  always @(posedge sys_clk) begin
    if (acc + STEP >= SYS_KHZ) begin
      acc <= acc + STEP - SYS_KHZ;
      osc <= ~osc;
    end else begin
      acc <= acc + STEP;
    end
  end'''


def generate(original=None):
    if original is None:original=(ROOT/'verilog/cells/cell_K1115A.v').read_text()
    if hashlib.sha256(original.encode()).hexdigest()!=SHA:raise ValueError('unreviewed original K1115A')
    assert original.count(PROCESS)==1
    text=original.replace('module cell_K1115A #(','module cell_K1115A_snapshot #(',1)
    assert text.count('    input  wire sys_clk,')==1
    text=text.replace('    input  wire sys_clk,','''    input wire state_host_clk,state_advance,state_freeze,state_guest_held,
    input wire state_request,state_write,
    input wire [1:0] state_index,input wire [17:0] state_data,
    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,
    output reg [17:0] state_result,
    input  wire sys_clk,''',1)
    normal=PROCESS[len('  always @(posedge sys_clk) begin\n'):-len('\n  end')]
    replacement='''  wire state_hold=state_freeze && state_guest_held;
  wire state_accept=state_hold && state_frozen && state_request;
  wire state_valid=!state_index[1];
  always @(posedge state_host_clk) begin
    state_frozen<=state_hold;
    state_done<=state_accept;
    state_error<=state_accept && !state_valid;
    if(state_accept)begin
      state_result<=18'b0;
      if(state_valid)begin
        if(state_write)begin
          if(!state_index[0])acc[17:0]<=state_data;
          else {osc,acc[31:18]}<=state_data[14:0];
        end else if(!state_index[0])state_result<=acc[17:0];
        else state_result<={3'b0,osc,acc[31:18]};
      end
    end
    if(!state_hold && state_advance)begin
'''+normal+'''
    end
  end'''
    text=text.replace(PROCESS,replacement,1)
    return text,dict(module='cell_K1115A_snapshot',logical_state_bits=33,fields=[dict(name='acc',width=32,signed=True),dict(name='osc',width=1)],
        valid_indices=[0,1],invalid_indices=[2,3],words=[dict(index=0,lsb=0,width=18),dict(index=1,lsb=18,width=15)],
        original_sha256=SHA,generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
        scope=__doc__,parameters='Original FREQ_KHZ/SYSPER arithmetic retained verbatim, not a calibrated real-time oscillator')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False)
    text,m=generate();(a.output/'cell_K1115A_snapshot.sv').write_text(text);(a.output/'manifest.json').write_text(json.dumps(m,indent=2)+'\n')
