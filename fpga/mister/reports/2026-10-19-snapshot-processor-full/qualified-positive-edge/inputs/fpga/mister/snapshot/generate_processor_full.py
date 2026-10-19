#!/usr/bin/env python3
"""Complete modeled state adapters for the exact frozen49 processor boards.

No production changes. All consumers and external inputs/phase need a global
hold; async RAM scan outputs may change. No machine-wide barrier is provided.
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
import generate_async256 as asynchronous

ROOT=Path(__file__).resolve().parents[3]
FIELDS={**contb.FIELDS,**ff.FIELDS,**extra.FIELDS}
INSTANCES=re.compile(r'\b(cell_\w+)(\s*(?:#\(.*?\))?\s+)(u_\w+)\s*\((.*?)\)\s*;',re.S)
SPECS={
    'ProcH':dict(module='ProcH_m_Rev_m_Ce',sha256='f1d46a0d77645fb1afb91dbe7612255233489ea4e05e74d33a11e8e3f9e90c9f',
        bits=5712,counts={'cell_MC10176':14,'cell_MC10231':7,'cell_MC10173':24,
        'cell_MC10141':6,'cell_F10016':4,'cell_F10000':4,'cell_SE10231':1,
        'cell_MC10175':2,'cell_MC1668':2,'cell_F10145A':8,'cell_MB7071H':4,'cell_F10414':2}),
    'ProcL':dict(module='ProcL_m_Rev_m_Ci',sha256='504237f530c596851faeefececebb38ee39d947f62cbc53679de32c98c168cc0',
        bits=5581,counts={'cell_MC10176':14,'cell_MC10231':10,'cell_MC10173':27,
        'cell_MC10141':6,'cell_F10016':2,'cell_F10000':5,'cell_SE10231':1,
        'cell_MC10175':2,'cell_MC1668':2,'cell_F10145A':6,'cell_MB7071H':4,'cell_F10414':2}),
}

PROM_HASHES={
 'ProcH':{'verilog/proms/packages/ProcH-b07.mem':'cda44d8e98fe97617a52289320ad283a75052cef1a990daf1a0037b0dd1db053',
          'verilog/proms/packages/ProcH-b08.mem':'43a8753c23056582d8e6da74303000807d427142cdee227a3e7a6baa5b4ffa7c'},
 'ProcL':{'verilog/proms/packages/ProcL-b07.mem':'683b4932393f280506051c4f57058829356b56fbac2fc36ba06e1db3b8ea4e57',
          'verilog/proms/packages/ProcL-b08.mem':'89149faaebdcda39d58e0ec5f807f17d4fcf809362c77473da7387bc1960aee0'},
}


def generate(kind,source):
    spec=SPECS[kind];original=source.read_text()
    if hashlib.sha256(source.read_bytes()).hexdigest()!=spec['sha256']:
        raise ValueError('processor board differs from exact frozen49: '+kind)
    module=kind+'_full_snapshot_experiment'
    text=contb.replace_once(original,'module '+spec['module']+' ','module '+module+' ')
    text=contb.replace_once(text,'    input  wire sys_clk,','''    input wire state_host_clk,state_guest_advance,state_freeze,state_guest_held,
    input wire state_request,state_write,
    input wire [15:0] state_index,input wire [17:0] state_data,
    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,
    output wire [17:0] state_result,
    input  wire sys_clk,''')
    phases=re.findall(r'^\s*reg\s+(\w+)\s*=\s*1\'b[01];',original,re.M)
    if len(phases)!=10 or len(set(phases))!=10:
        raise ValueError('unreviewed processor phase registers')
    phase='always @(posedge sys_clk) begin\n'+''.join(f'{n} <= {n}__lead;\n' for n in phases)+'end'
    text,block=ff.extract_once(text,phase)
    if re.search(r'\b(always\w*|initial|final)\b',ff.masked_comments(text)):
        raise ValueError('extra processor board state process')
    packages=[(m[1],m[3]) for m in INSTANCES.finditer(original) if m[1] in spec['counts']]
    if dict(Counter(k for k,_ in packages))!=spec['counts']:
        raise ValueError('processor sequential package inventory differs')
    rows=[];files=[];rams=[]
    for cell,name in packages:
        if cell in FIELDS:
            fields=[];offset=0
            for field,width in FIELDS[cell]:
                fields.append(dict(name=field,width=width,lsb=offset));offset+=width
            index=len(rows)
            rows.append(dict(index=index,word=index<<9,package=name,kind=cell,width=offset,fields=fields))
    small_source,small_manifest=small.generate()
    for cell,name in packages:
        if cell=='cell_F10145A':
            index=len(rows)+len(files)
            files.append(dict(index=index,word=index<<9,package=name,kind=cell,
                ram=small_manifest['ram'],write_window=small_manifest['write_window'],memory_index=len(files)))
    for cell,name in packages:
        if cell in ('cell_MB7071H','cell_F10414'):
            index=len(rows)+len(files)+len(rams);width=4 if cell=='cell_MB7071H' else 1
            rams.append(dict(index=index,word=index<<9,package=name,kind=cell,width=width,
                words=256,memory_index=len(files)+len(rams)))
    phase_index=len(rows)+len(files)+len(rams)
    if phase_index>=127:
        raise ValueError('processor package index exhausted')
    by_name={r['package']:r for r in rows+files+rams}
    clones={**contb.ff_sources(),**ff.generate(),**extra.generate(),
            'cell_F10145A_snapshot.sv':small_source}
    for family in asynchronous.SPECS:
        clones['cell_'+family+'_snapshot.sv']=asynchronous.generate(family)[0]
    def replace(match):
        cell,gap,name,body=match.groups()
        if name not in by_name:return match[0]
        row=by_name[name];index=row['index']
        if cell in FIELDS:
            ports=f'''
    .state_host_clk(state_host_clk),.state_advance(state_guest_advance),
    .state_hold(state_hold),.state_write(state_accept && state_write && state_package==7'd{index} && state_row==0),
    .state_data(state_data),.state_result(state_ff_result[{index}]),'''
        else:
            mi=row['memory_index'];shape='9:0' if cell=='cell_F10145A' else '3:0'
            row_select='state_row[4:0]' if cell=='cell_F10145A' else 'state_row'
            ports=f'''
    .state_host_clk(state_host_clk),.state_advance(state_guest_advance),
    .state_freeze(state_freeze),.state_guest_held(state_guest_held),
    .state_request(state_accept && state_valid && state_package==7'd{index}),.state_write(state_write),
    .state_index({row_select}),.state_data(state_data[{shape}]),
    .state_frozen(),.state_done(),.state_error(),.state_result(state_memory_{mi}),'''
        return cell+'_snapshot'+gap+name+' ('+ports+body+');'
    text=INSTANCES.sub(replace,text)
    nf,nsmall,nram=len(rows),len(files),len(rams);nm=nsmall+nram
    packed='{'+','.join(reversed(phases))+'}'
    body=re.sub(r'^always\s*@\s*\(\s*posedge\s+sys_clk\s*\)\s*','',block,count=1)
    fabric=f'''
  // 16-bit address:7-bit package/9-bit row. FF/phase row0; small file0..16;
  // async RAM0..255. Every other index errors without an aliased write.
  wire state_hold=state_freeze && state_guest_held;
  wire state_accept=state_hold && state_frozen && state_request;
  wire [6:0] state_package=state_index[15:9];
  wire [8:0] state_row=state_index[8:0];
  wire state_is_ff=state_package<7'd{nf} && state_row==0;
  wire state_is_small=state_package>=7'd{nf} && state_package<7'd{nf+nsmall};
  wire state_is_ram=state_package>=7'd{nf+nsmall} && state_package<7'd{nf+nm};
  wire state_is_phase=state_package==7'd{phase_index} && state_row==0;
  wire state_valid=state_is_ff || (state_is_small && state_row<=16) ||
                   (state_is_ram && !state_row[8]) || state_is_phase;
  wire [17:0] state_ff_result[0:{nf-1}];
  wire [17:0] state_memory_result[0:{nm-1}];
  reg state_response_memory;
  reg [3:0] state_response_memory_index;
  reg [17:0] state_aux_result;
  assign state_result=state_response_memory ? state_memory_result[state_response_memory_index] : state_aux_result;
'''
    for row in files+rams:
        mi=row['memory_index'];width=10 if row['kind']=='cell_F10145A' else 4
        fabric+=f'  wire [{width-1}:0] state_memory_{mi};\n'
        fabric+=f"  assign state_memory_result[{mi}]={{{18-width}'b0,state_memory_{mi}}};\n"
    fabric+=f'''
  always @(posedge state_host_clk) begin
    state_frozen<=state_hold;
    state_done<=state_accept;
    state_error<=state_accept && !state_valid;
    if (state_accept) begin
      state_response_memory<=state_valid && (state_is_small || state_is_ram);
      state_response_memory_index<=(state_is_small || state_is_ram) ? 4'(state_package-7'd{nf}) : 4'd0;
      state_aux_result<=state_is_ff ? state_ff_result[state_package] :
                        (state_is_phase ? {{8'b0,{packed}}} : 18'b0);
    end
    if (state_hold) begin
      if (state_accept && state_write && state_is_phase) {packed}<=state_data[9:0];
    end else if (state_guest_advance) {body}
  end
'''
    text=contb.replace_once(text,'endmodule',fabric+'\nendmodule')
    bits=sum(r['width'] for r in rows)+nsmall*74+4*1024+2*256+10
    if bits!=spec['bits']:
        raise ValueError('unexpected complete modeled processor state size')
    kinds=sorted({m[1] for m in INSTANCES.finditer(original)})
    proms=[]
    for m in INSTANCES.finditer(original):
        if m[1]!='cell_SG10139':continue
        match=re.search(r'INIT_FILE\("([^\"]+)"\)',m[2])
        if match is None or match[1]not in PROM_HASHES[kind]:
            raise ValueError('unreviewed processor PROM binding')
        name=match[1];digest=hashlib.sha256((ROOT/name).read_bytes()).hexdigest()
        if digest!=PROM_HASHES[kind][name]:raise ValueError('processor PROM differs from frozen49')
        proms.append(dict(package=m[3],file=name,sha256=digest,words=32,width=8))
    if len(proms)!=2 or {p['file']for p in proms}!=set(PROM_HASHES[kind]):
        raise ValueError('incomplete immutable processor PROM set')
    manifest=dict(module=module,original_module=spec['module'],original_sha256=spec['sha256'],
        ff=rows,files=files,rams=rams,phase=dict(package_index=phase_index,word=phase_index<<9,fields=phases,width=10),
        logical_state_bits=bits,index_bits=16,row_bits=9,immutable_proms=proms,ff_bits=sum(r['width'] for r in rows),
        ram_bits=nsmall*64+4608,write_window_bits=nsmall*10,phase_bits=10,
        original_sources={cell:hashlib.sha256((ROOT/'verilog/cells'/f'{cell}.v').read_bytes()).hexdigest() for cell in kinds},
        generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
        cloned_sources={name:hashlib.sha256(s.encode()).hexdigest() for name,s in clones.items()},
        scope='Complete modeled frozen49 '+kind+' only; external consumers/inputs/phase/global freeze/ARM/device state excluded')
    return text,manifest,clones


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--kind',choices=SPECS,required=True)
    parser.add_argument('--source',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();args.output.mkdir(parents=True,exist_ok=False)
    source,manifest,clones=generate(args.kind,args.source.resolve())
    (args.output/(args.kind+'_full_snapshot.sv')).write_text(source)
    (args.output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    for name,text in clones.items():(args.output/name).write_text(text)
