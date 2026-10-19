#!/usr/bin/env python3
"""Complete modeled MOS RIOT state for the actual BaseBoard init_7800=0 binding.

Retain ce-gated reads, unconditional reset priority, timer/interrupt ordering,
RAM and the block-local PA7 history. Caller supplies a coherent external hold.
The enclosing MCS6532 phase-history bit is separate board/wrapper state.
"""
import hashlib
from pathlib import Path

ROOT=Path(__file__).resolve().parents[3]
SOURCE=ROOT/'verilog/vendor/riot/M6532.sv'
SHA='43c9d1670fcbb98ad4681883981930817e2882a5ab32790f8285961c3ae0c1b1'
FIELDS=[('d_out',8),('out_a',8),('out_b',8),('dir_a',8),('dir_b',8),
        ('interrupt',8),('timer',8),('prescaler',10),('incr',2),('irq_en',2),
        ('edge_detect',1),('rollover',1),('old_pa7',1)]


def generate(original=None):
    raw=SOURCE.read_bytes()if original is None else original
    if hashlib.sha256(raw).hexdigest()!=SHA:raise ValueError('unreviewed original RIOT source')
    text=raw.decode();assert text.count('always_ff')==2
    a=text.index('always_ff @(posedge clk) begin');b=text.index('\nwire pa7',a)
    read=text[a:b];text=text[:a]+text[b:]
    a=text.index('always_ff @(posedge clk) if (~res_n) begin');b=text.index('\nendmodule',a)
    state=text[a:b];text=text[:a]+text[b:]
    assert state.count('\t\treg old_pa7;')==1
    # A procedural-block reg has static lifetime. Hoist only its declaration
    # so a distinct host access branch can restore it, with no reset added.
    state=state.replace('\t\treg old_pa7;','',1)
    read=read.removeprefix('always_ff @(posedge clk) ')
    state=state.removeprefix('always_ff @(posedge clk) ')
    anchor='module M6532\n(\n';assert text.count(anchor)==1
    text=text.replace(anchor,'''module M6532_snapshot
(
    input state_host_clk,state_advance,state_freeze,state_guest_held,state_request,state_write,
    input [7:0] state_index,input [17:0] state_data,
    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,
    output reg [17:0] state_result,
''',1)
    fields=[];lsb=0
    for name,width in FIELDS:
        fields.append(dict(name=name,width=width,lsb=lsb));lsb+=width
    assert lsb==73
    words=[dict(index=i,lsb=8*i,width=8,kind='RAM')for i in range(128)]
    cases=[]
    for i in range(5):
        lo=i*18;hi=min(72,lo+17);width=hi-lo+1;parts=[]
        for f in reversed(fields):
            left=max(lo,f['lsb']);right=min(hi,f['lsb']+f['width']-1)
            if left>right:continue
            part=f['name']
            if f['width']>1:part+='['+str(right-f['lsb'])+':'+str(left-f['lsb'])+']'
            parts.append(part)
        target=parts[0]if len(parts)==1 else'{'+','.join(parts)+'}'
        result=target if width==18 else'{'+str(18-width)+"'b0,"+target+'}'
        cases.append(f"                8'd{128+i}:if(state_write){target}<=state_data[{width-1}:0];else state_result<={result};")
        words.append(dict(index=128+i,lsb=1024+lo,width=width,kind='CONTROL'))
    fabric='''
reg old_pa7;
wire state_hold=state_freeze && state_guest_held;
wire state_accept=state_hold && state_frozen && state_request;
wire state_valid=state_index<8'd133;
always @(posedge state_host_clk)begin
    state_frozen<=state_hold;state_done<=state_accept;state_error<=state_accept && !state_valid;
    if(state_accept)begin
        state_result<=18'b0;
        if(state_valid)begin
            if(!state_index[7])begin
                if(state_write)riot_ram[state_index[6:0]]<=state_data[7:0];
                else state_result<={10'b0,riot_ram[state_index[6:0]]};
            end else case(state_index)
CASES
                default:;
            endcase
        end
    end
    if(!state_hold && state_advance)begin
READ
STATE
    end
end
'''.replace('CASES','\n'.join(cases)).replace('READ',read).replace('STATE',state)
    text=text.replace('\nendmodule',fabric+'\nendmodule',1)
    return text,dict(module='M6532_snapshot',logical_state_bits=1097,ram_bits=1024,
        control_bits=73,fields=fields,words=words,index_bits=8,parameters=dict(init_7800=0),
        valid_indices=list(range(133)),invalid_indices=list(range(133,256)),
        original_sha256=SHA,generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
        original_pa7_path='riot_stuff.old_pa7 (static procedural reg, declaration hoisted without initialization/reset)',
        scope=__doc__)
