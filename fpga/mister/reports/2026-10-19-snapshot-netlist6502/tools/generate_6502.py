#!/usr/bin/env python3
"""Export every node of the actual netlist-derived BaseBoard 6502.

Preserve the vendor combinational netlist and its iterative relaxation.
Caller must hold all consumers at the same boundary; no global barrier here.
"""
import hashlib
from pathlib import Path
import re

ROOT=Path(__file__).resolve().parents[3]
SOURCE=ROOT/'verilog/vendor/6502/chip_6502.v'
SHA='8ae05dff81a1c2971e03d2e01c4c3cb09b4f111c40e54b7e9e437f1d8add46da'


def generate(original=None):
    raw=SOURCE.read_bytes()if original is None else original
    if hashlib.sha256(raw).hexdigest()!=SHA:raise ValueError('unreviewed original netlist6502 bytes')
    text=raw.decode();eol='\r\n'
    assert text.count(eol)==1811
    # Separate module names allow untouched original and candidate to coexist.
    for name in ('MUX6502','LOGIC','chip_6502'):
        text=re.sub(r'\b'+name+r'\b',name+'_snapshot',text)
    anchor='module chip_6502_snapshot ('+eol
    assert text.count(anchor)==1
    ports='''    input state_host_clk,state_advance,state_freeze,state_guest_held,state_request,state_write,
    input [6:0] state_index,input [17:0] state_data,
    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,
    output reg [17:0] state_result,
'''.replace('\n',eol)
    text=text.replace(anchor,anchor+ports,1)
    original_process='    always @ (posedge clk)'+eol+'        q <= no;'
    assert text.count(original_process)==1
    cases=[];words=[]
    for index in range(96):
        lo=index*18;width=min(18,1725-lo);hi=lo+width-1
        target=f'q[{hi}:{lo}]'
        result=target if width==18 else'{'+str(18-width)+"'b0,"+target+'}'
        cases.append(f"            7'd{index}:if(state_write){target}<=state_data[{width-1}:0];else state_result<={result};")
        words.append(dict(index=index,lsb=lo,width=width))
    process='''    wire state_hold=state_freeze && state_guest_held;
    wire state_accept=state_hold && state_frozen && state_request;
    wire state_valid=state_index<7'd96;
    always @(posedge state_host_clk) begin
        state_frozen<=state_hold;state_done<=state_accept;state_error<=state_accept && !state_valid;
        if(state_accept)begin
            state_result<=18'b0;
            if(state_valid)case(state_index)
CASES
            default:;
            endcase
        end
        if(!state_hold && state_advance)q<=no;
    end'''.replace('CASES','\n'.join(cases)).replace('\n',eol)
    text=text.replace(original_process,process,1)
    return text.encode(),dict(module='chip_6502_snapshot',logical_state_bits=1725,
        fields=[dict(name='q',width=1725,lsb=0)],words=words,index_bits=7,
        valid_indices=list(range(96)),invalid_indices=list(range(96,128)),
        original_sha256=SHA,generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
        source_line_endings='CRLF preserved',scope=__doc__)
