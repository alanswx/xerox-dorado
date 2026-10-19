#!/usr/bin/env python3
"""Complete modeled state access for the frozen native64K-map MemX board.

Every MosRam socket retains its own row/column/history/pending/output state.
All consumers and external inputs require the caller's global hold. No global
barrier, ARM coherence or complete physical machine snapshot is provided.
"""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re

import generate_contb_full as contb
import generate_snapshot_ff as ff
import generate_processor_ff as extra
import generate_f10145 as small
import generate_jk as jk
import generate_mosram as mos

ROOT=Path(__file__).resolve().parents[3]
MODULE='MemX_full_snapshot_experiment'
ORIGINAL_MODULE='MemX_m_Rev_m_Ch'
BOARD_SHA='709e14c98631a0923251f7a93ba5075fbecf27b6dac3347f52086c2b513c1816'
FIELDS={**contb.FIELDS,**ff.FIELDS,**extra.FIELDS,'cell_MC10135':jk.FIELDS}
COUNTS={'cell_F10016':31,'cell_MC10141':10,'cell_MC10175':4,'cell_MC10231':2,
        'cell_MC10176':22,'cell_MC10173':2,'cell_F10000':1,'cell_MC10135':1,
        'cell_SE10231':1,'cell_F10145A':10,'cell_MosRam':21}
INSTANCES=re.compile(r'\b(cell_\w+)(\s*(?:#\(.*?\))?\s+)(u_\w+)\s*\((.*?)\)\s*;',re.S)
PHASE_FIELDS=[('Clk1_p_Aa',1),('Clk1_p_Ab',1),('Clk1_p_Ab__lag1',1),('Clk1_p_Ab__lag2',1),
    ('Clk1_p_Ba',1),('Clk1_p_Bb',1),('Clk1_p_Bc',1),('Clk1_p_Ca',1),('Clk1_p_Cb',1),('Clk1_p_Cc',1),
    ('mapbuf_fh_delay',3),('sin_select_clock',1)]
PROCESSES=[
 'always @(posedge sys_clk) mapbuf_fh_delay <= {mapbuf_fh_delay[1:0],preFH_p_A};',
 'always @(posedge sys_clk) sin_select_clock <= Clk0_p_Bc;',
 '''always @(posedge sys_clk) begin
    Clk1_p_Aa <= Clk1_p_Aa__lead;
    Clk1_p_Ab__lag1 <= Clk1_p_Ab__lead;
    Clk1_p_Ab__lag2 <= Clk1_p_Ab__lag1;
    Clk1_p_Ab <= Clk1_p_Ab__lag2;
    Clk1_p_Ba <= Clk1_p_Ba__lead;
    Clk1_p_Bb <= Clk1_p_Bb__lead;
    Clk1_p_Bc <= Clk1_p_Bc__lead;
    Clk1_p_Ca <= Clk1_p_Ca__lead;
    Clk1_p_Cb <= Clk1_p_Cb__lead;
    Clk1_p_Cc <= Clk1_p_Cc__lead;
  end''',
]
PROM_HASHES={
 'verilog/proms/packages/MemX-g15.mem':'be3f5aabe0a069e51869bedd09f14c48ede0880762abbeee5c3d4f5a958d68cc',
 'verilog/proms/packages/MemX-h11.mem':'a7ae2f43e9c89d6adaf3949de1512ff7534b623644e25cc70acd987b2a47bb1c',
 'verilog/proms/packages/MemX-i12.mem':'67590dbb76115b3afb247f963bf32122ef9828d1dd653ac65b8623677f0c1492',
 'verilog/proms/packages/MemX-i14.mem':'706d0dd034f063d5807a1186f7009e4b01e90884be2fe799affedf25465f6b61',
 'verilog/proms/packages/MemX-j13.mem':'747d9c2c37cf9962d326cab4d2009a0230e6de2f724ce99f2679880c2945291e',
 'verilog/proms/packages/MemX-j14.mem':'f2243a5dc078d5112b59df26c52e8498f6b796bae2dc98aab49e10cd05021e16',
 'verilog/proms/packages/MemX-l11.mem':'d0b1ed0773111d57ebd37c797feac5068561a35731e9bad35f273883ee36f274',
 'verilog/proms/packages/MemX-l12.mem':'c55a27f683adf4ca4d994fb77e9cfbcf3c343a513a6abdc3f3b4b8a4431cec22',
}


def generate(root):
    source=root/'verilog/generated/MemX-Rev-Ch.v';original=source.read_text()
    if hashlib.sha256(source.read_bytes()).hexdigest()!=BOARD_SHA:raise ValueError('unreviewed frozen native MemX')
    text=contb.replace_once(original,'module '+ORIGINAL_MODULE+' ','module '+MODULE+' ')
    text=contb.replace_once(text,'    input  wire sys_clk,','''    input wire state_host_clk,state_guest_advance,state_freeze,state_guest_held,
    input wire state_request,state_write,
    input wire [23:0] state_index,input wire [17:0] state_data,
    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,
    output wire [17:0] state_result,
    input  wire sys_clk,''')
    normal=[]
    for fragment in PROCESSES:
        text,block=ff.extract_once(text,fragment)
        normal.append(re.sub(r'^always\s*@\s*\(\s*posedge\s+sys_clk\s*\)\s*','',block,count=1))
    if re.search(r'\b(always\w*|initial|final)\b',ff.masked_comments(text)):
        raise ValueError('extra MemX board-owned process')
    packages=[(m[1],m[3])for m in INSTANCES.finditer(original)if m[1]in COUNTS]
    if dict(Counter(k for k,_ in packages))!=COUNTS:raise ValueError('MemX sequential inventory differs')
    all_clones={**contb.ff_sources(),**ff.generate(),**extra.generate(),'cell_MC10135_snapshot.v':jk.generate()[0]}
    rows=[];files=[];rams=[];clones={}
    for kind,name in packages:
        if kind not in FIELDS:continue
        fields=[];offset=0
        for field,width in FIELDS[kind]:fields.append(dict(name=field,width=width,lsb=offset));offset+=width
        index=len(rows);rows.append(dict(index=index,word=index<<17,package=name,kind=kind,width=offset,fields=fields))
        clones[kind+'_snapshot.v']=all_clones[kind+'_snapshot.v']
    small_source,small_schema=small.generate();clones['cell_F10145A_snapshot.sv']=small_source
    for kind,name in packages:
        if kind=='cell_F10145A':
            index=len(rows)+len(files);files.append(dict(index=index,word=index<<17,package=name,kind=kind,
                ram=small_schema['ram'],write_window=small_schema['write_window'],memory_index=len(files)))
    mos_schema=mos.manifest(16);clones['cell_MosRam_snapshot.sv']=mos.generate()
    for m in INSTANCES.finditer(original):
        if m[1]!='cell_MosRam':continue
        if not re.fullmatch(r'\s*#\(\.ADDR_BITS\(16\)\)\s+',m[2]):raise ValueError('native64K map socket binding differs')
        index=len(rows)+len(files)+len(rams);rams.append(dict(index=index,word=index<<17,package=m[3],kind=m[1],
            address_bits=16,words=65536,width=1,history=mos_schema['history'],memory_index=len(files)+len(rams)))
    nf,ns,nr=len(rows),len(files),len(rams);nm=ns+nr;phase_index=nf+nm
    by_name={r['package']:r for r in rows+files+rams}
    def replace(m):
        kind,gap,name,body=m.groups()
        if name not in by_name:return m[0]
        row=by_name[name];i=row['index']
        if kind in FIELDS:
            ports=f'''
    .state_host_clk(state_host_clk),.state_advance(state_guest_advance),.state_hold(state_hold),
    .state_write(state_accept && state_write && state_is_ff && state_package==7'd{i}),
    .state_data(state_data),.state_result(state_ff_result[{i}]),'''
        else:
            mi=row['memory_index'];small_file=kind=='cell_F10145A'
            ports=f'''
    .state_host_clk(state_host_clk),.state_advance(state_guest_advance),
    .state_freeze(state_freeze),.state_guest_held(state_guest_held),
    .state_request(state_accept && state_valid && state_package==7'd{i}),.state_write(state_write),
    .state_index({'state_row[4:0]'if small_file else'state_row'}),.state_data({'state_data[9:0]'if small_file else'state_data'}),
    .state_frozen(),.state_done(),.state_error(),.state_result({'state_small_result['+str(mi)+']'if small_file else'state_memory_result['+str(mi)+']'}),'''
        return kind+'_snapshot'+gap+name+' ('+ports+body+');'
    text=INSTANCES.sub(replace,text)
    fields=[];offset=0
    for name,width in PHASE_FIELDS:fields.append(dict(name=name,width=width,lsb=offset));offset+=width
    packed='{'+','.join(n for n,_ in reversed(PHASE_FIELDS))+'}'
    fabric=f'''
  // 24-bit address:7package/17row. FF/phase0,small files0..16,
  // each independent64K RAM0..65535, history words65536/65537. Others error.
  wire state_hold=state_freeze && state_guest_held;
  wire state_accept=state_hold && state_frozen && state_request;
  wire [6:0] state_package=state_index[23:17];
  wire [16:0] state_row=state_index[16:0];
  wire state_is_ff=state_package<7'd{nf} && state_row==0;
  wire state_is_small=state_package>=7'd{nf} && state_package<7'd{nf+ns};
  wire state_is_ram=state_package>=7'd{nf+ns} && state_package<7'd{phase_index};
  wire state_is_phase=state_package==7'd{phase_index} && state_row==0;
  wire state_valid=state_is_ff || (state_is_small && state_row<=16) ||
    (state_is_ram && state_row<=17'd65537) || state_is_phase;
  wire [17:0] state_ff_result[0:{nf-1}];
  wire [9:0] state_small_result[0:{ns-1}];
  wire [17:0] state_memory_result[0:{nm-1}];
  reg state_response_memory;
  reg [4:0] state_response_memory_index;
  reg [17:0] state_aux_result;
  assign state_result=state_response_memory?state_memory_result[state_response_memory_index]:state_aux_result;
'''+''.join(f"  assign state_memory_result[{i}]={{8'b0,state_small_result[{i}]}};\n"for i in range(ns))+f'''
  always @(posedge state_host_clk)begin
    state_frozen<=state_hold;state_done<=state_accept;
    state_error<=state_accept && !state_valid;
    if(state_accept)begin
      state_response_memory<=state_valid && (state_is_small || state_is_ram);
      state_response_memory_index<=(state_is_small || state_is_ram)?5'(state_package-7'd{nf}):5'd0;
      state_aux_result<=state_is_ff?state_ff_result[state_package]:(state_is_phase?{{4'b0,{packed}}}:18'b0);
    end
    if(state_hold)begin
      if(state_accept && state_write && state_is_phase){packed}<=state_data[13:0];
    end else if(state_guest_advance)begin
'''+ '\n'.join(normal)+'''
    end
  end
'''
    text=contb.replace_once(text,'endmodule',fabric+'\nendmodule')
    kinds=sorted({m[1]for m in INSTANCES.finditer(original)});proms=[]
    for kind in {r['kind']for r in rows}|{'cell_F10145A','cell_MosRam','cell_SG10139'}:
        if (root/'verilog/cells'/f'{kind}.v').read_bytes()!=(ROOT/'verilog/cells'/f'{kind}.v').read_bytes():
            raise ValueError('qualified cell differs from frozen MemX:'+kind)
    for m in INSTANCES.finditer(original):
        if m[1]!='cell_SG10139':continue
        matches=re.findall(r'"(verilog/proms/[^\"]+)"',m[2])
        if len(matches)!=1 or matches[0]not in PROM_HASHES:raise ValueError('unreviewed MemX PROM binding')
        name=matches[0];sha=PROM_HASHES[name]
        if hashlib.sha256((root/name).read_bytes()).hexdigest()!=sha or hashlib.sha256((ROOT/name).read_bytes()).hexdigest()!=sha:
            raise ValueError('frozen immutable MemX PROM differs')
        proms.append(dict(package=m[3],file=name,sha256=sha,words=32,width=8))
    bits=sum(r['width']for r in rows)+ns*74+nr*(65536+23)+14
    if (nf,ns,nr,offset,bits,len(proms),phase_index)!=(74,10,21,14,1378312,8,105):raise ValueError('unexpected complete MemX state geometry')
    manifest=dict(module=MODULE,original_module=ORIGINAL_MODULE,original_sha256=BOARD_SHA,
        ff=rows,files=files,rams=rams,phase=dict(package_index=phase_index,word=phase_index<<17,width=14,fields=fields),
        logical_state_bits=bits,index_bits=24,row_bits=17,ff_bits=819,ram_bits=nr*65536+ns*64,
        map_history_bits=nr*23,write_window_bits=ns*10,phase_bits=14,immutable_proms=proms,
        original_sources={'verilog/cells/'+k+'.v':hashlib.sha256((root/'verilog/cells'/f'{k}.v').read_bytes()).hexdigest()for k in kinds},
        generated_sha256=hashlib.sha256(text.encode()).hexdigest(),cloned_sources={n:hashlib.sha256(s.encode()).hexdigest()for n,s in clones.items()},
        scope='Complete modeled frozen native64K-map MemX; every socket history independent,2-state. No external/global barrier,device/ARM state or physical machine snapshot')
    return text,manifest,clones


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    a.output.mkdir(parents=True,exist_ok=False);text,m,clones=generate(a.source_root.resolve());(a.output/'MemX_full_snapshot.sv').write_text(text)
    (a.output/'manifest.json').write_text(json.dumps(m,indent=2)+'\n')
    for n,s in clones.items():(a.output/n).write_text(s)
