#!/usr/bin/env python3
"""Bounded full physical ContA state access on the exact frozen Native49 board.

No production source changes. Caller must capture external phase/inputs and
establish a machine-wide barrier; this adapter supplies neither by itself.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

import generate_contb_full as contb
import generate_f10145 as file_cell
import generate_snapshot_ff as ff

ROOT=Path(__file__).resolve().parents[3]
SOURCE_SHA='1eb77199656413cc740fd5c1081aad9e519adba2411fbd4d073e99f5a2987c5b'
MODULE='ContA_full_snapshot_experiment'
PHASES=['clk1_p_Aa','clk1_p_Ab','clk1_p_Ac','clk1_p_Ca','clk1_p_Cb','clk1_p_Da']
FIELDS={**contb.FIELDS,**ff.FIELDS}
EXPECTED_COUNTS={'cell_MC10176':24,'cell_MC10231':31,'cell_F10000':4,
                 'cell_F10016':9,'cell_MC10141':2,'cell_MC10173':4,'cell_SE10231':1,
                 'cell_F10145A':9}
INSTANCES=re.compile(r'\b(cell_\w+)(\s*(?:#\(.*?\))?\s+)(u_\w+)\s*\((.*?)\)\s*;',re.S)


def generate(source):
    original=source.read_text()
    if hashlib.sha256(source.read_bytes()).hexdigest()!=SOURCE_SHA:
        raise ValueError('ContA is not the exact qualified frozen49 board')
    text=contb.replace_once(original,'module ContA_m_Rev_m_Cd_monitor ','module '+MODULE+' ')
    text=contb.replace_once(text,'    input  wire sys_clk,','''    input wire state_host_clk,state_guest_advance,state_freeze,state_guest_held,
    input wire state_request,state_write,
    input wire [11:0] state_index,input wire [17:0] state_data,
    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,
    output wire [17:0] state_result,
    input  wire sys_clk,''')
    phase='always @(posedge sys_clk) begin\n'+''.join(f'{n} <= {n}__lead;\n'for n in PHASES)+'end'
    text,block=ff.extract_once(text,phase)
    if re.search(r'\b(always\w*|initial|final)\b',ff.masked_comments(text)):
        raise ValueError('unexpected board-owned state process')
    packages=[(m[1],m[3])for m in INSTANCES.finditer(original)if m[1]in EXPECTED_COUNTS]
    from collections import Counter
    if dict(Counter(k for k,_ in packages))!=EXPECTED_COUNTS:
        raise ValueError('different sequential package inventory')
    files=[(k,n)for k,n in packages if k=='cell_F10145A']
    sequential=[(k,n)for k,n in packages if k in FIELDS]
    clones={**contb.ff_sources(),**ff.generate()}
    file_source,file_manifest=file_cell.generate()
    clones['cell_F10145A_snapshot.sv']=file_source
    rows=[]
    for index,(kind,name)in enumerate(sequential):
        fields=[];offset=0
        for field,width in FIELDS[kind]:
            fields.append(dict(name=field,width=width,lsb=offset));offset+=width
        rows.append(dict(index=index,word=index<<5,package=name,kind=kind,width=offset,fields=fields))
    file_rows=[dict(index=75+i,package=n,kind=k,ram=file_manifest['ram'],write_window=file_manifest['write_window'])
               for i,(k,n)in enumerate(files)]
    by_name={r['package']:r for r in rows+file_rows}
    def replace(match):
        kind,gap,name,body=match.groups()
        if name not in by_name:return match[0]
        row=by_name[name];index=row['index']
        if kind in FIELDS:
            extra=f'''
    .state_host_clk(state_host_clk),.state_advance(state_guest_advance),
    .state_hold(state_hold),.state_write(state_accept && state_write && state_package==7'd{index} && state_row==0),
    .state_data(state_data),.state_result(state_ff_result[{index}]),'''
        else:
            extra=f'''
    .state_host_clk(state_host_clk),.state_advance(state_guest_advance),
    .state_freeze(state_freeze),.state_guest_held(state_guest_held),
    .state_request(state_accept && state_package==7'd{index}),.state_write(state_write),
    .state_index(state_row),.state_data(state_data[9:0]),
    .state_frozen(),.state_done(),.state_error(),.state_result(state_ram_result[{index-75}]),'''
        return kind+'_snapshot'+gap+name+' ('+extra+body+');'
    text=INSTANCES.sub(replace,text)
    packed='{'+','.join(reversed(PHASES))+'}'
    body=re.sub(r'^always\s*@\s*\(\s*posedge\s+sys_clk\s*\)\s*','',block,count=1)
    fabric=f'''
  // Package0..74: FF word0.75..83:16x4 RAM words0..15/window16.
  // Package84 word0: all six original board clock-history bits.
  wire state_hold=state_freeze && state_guest_held;
  wire state_accept=state_hold && state_frozen && state_request;
  wire [6:0] state_package=state_index[11:5];
  wire [4:0] state_row=state_index[4:0];
  wire state_is_ff=state_package<75 && state_row==0;
  wire state_is_ram=state_package>=75 && state_package<=83;
  wire state_is_phase=state_package==84 && state_row==0;
  wire state_valid=state_is_ff || (state_is_ram && state_row<=16) || state_is_phase;
  wire [17:0] state_ff_result[0:74];
  wire [9:0] state_ram_result[0:8];
  reg state_response_ram;
  reg [3:0] state_response_ram_index;
  reg [17:0] state_aux_result;
  assign state_result=state_response_ram ? {{8'b0,state_ram_result[state_response_ram_index]}} : state_aux_result;
  always @(posedge state_host_clk) begin
    state_frozen<=state_hold;
    state_done<=state_accept;
    state_error<=state_accept && !state_valid;
    if (state_accept) begin
      state_response_ram<=state_is_ram;
      state_response_ram_index<=state_is_ram ? state_package-7'd75 : 0;
      state_aux_result<=state_is_ff ? state_ff_result[state_package] :
                        (state_is_phase ? {{12'b0,{packed}}} : 18'b0);
    end
    if (state_hold) begin
      if (state_accept && state_write && state_is_phase) {packed}<=state_data[5:0];
    end else if (state_guest_advance) {body}
  end
'''
    text=contb.replace_once(text,'endmodule',fabric+'\nendmodule')
    physical=sum(r['width']for r in rows)+9*74+6
    if physical!=1353:raise ValueError('unexpected full physical ContA state size')
    kinds=sorted({m[1]for m in INSTANCES.finditer(original)})
    manifest=dict(module=MODULE,original_sha256=SOURCE_SHA,ff=rows,files=file_rows,
        phase=dict(package_index=84,word=84<<5,fields=PHASES,width=6),logical_state_bits=physical,
        ff_bits=681,ram_bits=576,write_window_bits=90,phase_bits=6,
        excluded_file_diagnostics=file_manifest['excluded_simulation_only'],
        original_sources={kind:hashlib.sha256((ROOT/'verilog/cells'/f'{kind}.v').read_bytes()).hexdigest()for kind in kinds},
        generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
        cloned_sources={name:hashlib.sha256(s.encode()).hexdigest()for name,s in clones.items()},
        scope='Complete physical modeled frozen49 ContA only; caller external inputs/phase/global hold and ARM/device state excluded')
    return text,manifest,clones


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();args.output.mkdir(parents=True,exist_ok=False)
    board,manifest,clones=generate(args.source.resolve())
    (args.output/'ContA_full_snapshot.sv').write_text(board)
    (args.output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    for name,text in clones.items():(args.output/name).write_text(text)
