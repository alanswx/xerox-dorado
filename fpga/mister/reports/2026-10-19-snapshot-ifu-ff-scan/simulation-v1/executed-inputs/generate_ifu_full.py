#!/usr/bin/env python3
"""Complete modeled frozen IFU state access, retaining independent RAM groups.

Experiments only. Caller holds every consumer/external input and captures
global phase; no complete machine barrier or physical snapshot is provided.
"""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re

import audit_ifu_im as im
import generate_contb_full as contb
import generate_snapshot_ff as ff
import generate_jk as jk

ROOT=Path(__file__).resolve().parents[3]
MODULE='IFU_full_snapshot_experiment'
ORIGINAL_MODULE='IFU_m_Rev_m_Ch'
PHASES=['clk1_p_Aa','clk1_p_Ab','clk1_p_Ac','clk1_p_Da']
FIELDS={**contb.FIELDS,**ff.FIELDS,'cell_MC10135':jk.FIELDS}
COUNTS={'cell_F10016':38,'cell_MC10176':30,'cell_MC10231':16,'cell_MC10173':4,
        'cell_MC10141':2,'cell_MC10135':4,'cell_F10415A':27}
PROM='verilog/proms/packages/IFU-a06.mem'
PROM_SHA='691f366c2aeaf7241f9b30868f225b0a49508a0954c23e561db73bcf9ac0fc0c'
INSTANCES=re.compile(r'\b(cell_\w+)(\s*(?:#\(.*?\))?\s+)(u_\w+)\s*\((.*?)\)\s*;',re.S)


def generate(source_root):
    source=source_root/'verilog/generated/IFU-Rev-Ch.v';original=source.read_text()
    grouping=im.audit(source_root)
    text=contb.replace_once(original,'module '+ORIGINAL_MODULE+' ','module '+MODULE+' ')
    text=contb.replace_once(text,'    input  wire sys_clk,','''    input wire state_host_clk,state_guest_advance,state_freeze,state_guest_held,
    input wire state_request,state_write,
    input wire [17:0] state_index,state_data,
    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,
    output wire [17:0] state_result,
    input  wire sys_clk,''')
    phase='always @(posedge sys_clk) begin\n'+''.join(n+' <= '+n+'__lead;\n'for n in PHASES)+'end'
    text,block=ff.extract_once(text,phase)
    if re.search(r'\b(always\w*|initial|final)\b',ff.masked_comments(text)):
        raise ValueError('extra IFU board-owned state process')
    packages=[(m[1],m[3])for m in INSTANCES.finditer(original)if m[1]in COUNTS]
    if dict(Counter(k for k,_ in packages))!=COUNTS:raise ValueError('IFU sequential inventory differs')
    all_clones={**contb.ff_sources(),**ff.generate(),'cell_MC10135_snapshot.v':jk.generate()[0]}
    clones={};rows=[]
    for kind,name in packages:
        if kind not in FIELDS:continue
        fields=[];offset=0
        for field,width in FIELDS[kind]:fields.append(dict(name=field,width=width,lsb=offset));offset+=width
        index=len(rows);rows.append(dict(index=index,word=index<<11,package=name,kind=kind,width=offset,fields=fields))
        filename=kind+'_snapshot.v';clones[filename]=all_clones[filename]
        if (ROOT/'verilog/cells'/f'{kind}.v').read_bytes()!=(source_root/'verilog/cells'/f'{kind}.v').read_bytes():
            raise ValueError('qualified FF source differs from frozen IFU: '+kind)
    by_name={r['package']:r for r in rows};nf=len(rows)
    def replace(match):
        kind,gap,name,body=match.groups()
        if kind=='cell_F10415A':return ''
        if name not in by_name:return match[0]
        index=by_name[name]['index']
        ports=f'''
    .state_host_clk(state_host_clk),.state_advance(state_guest_advance),.state_hold(state_hold),
    .state_write(state_accept && state_write && state_is_ff && state_package==7'd{index}),
    .state_data(state_data),.state_result(state_ff_result[{index}]),'''
        return kind+'_snapshot'+gap+name+' ('+ports+body+');'
    text=INSTANCES.sub(replace,text)
    pins={name:p for kind,name,p in im.logic.instances(original)if kind=='cell_F10415A'}
    banks=[];code=[]
    for i,group in enumerate(grouping['groups']):
        members=group['members'];p=pins['u_'+members[0]['package']];width=group['width'];package=nf+i
        # This audit proved exact literal fanout groups for the frozen IFU;
        # do not infer equivalence between independently registered replicas.
        for m in members:
            other=pins['u_'+m['package']]
            if any(other['p'+str(n)]!=p['p'+str(n)]for n in (*im.logic.ADDRESS,13)):
                raise ValueError('review nonliteral IFUM group before integration')
            if 'p14'in other:raise ValueError('original omitted CE changed')
        address=[p['p'+str(n)]for n in im.logic.ADDRESS]
        data=[pins['u_'+m['package']]['p15']for m in members]
        output=[pins['u_'+m['package']]['p1']for m in members]
        code+=[f'  wire [{width-1}:0] state_bank_q_{i};',
            f'  dorado_snapshot_f10415_word_bank #(.WIDTH({width})) snapshot_bank_{i}(',
            '    .host_clk(state_host_clk),.guest_advance(state_guest_advance),',
            '    .address({'+','.join(address)+'}),.data({'+','.join(reversed(data))+'}),',
            f'    .we_n({p["p13"]}),.q(state_bank_q_{i}),',
            '    // CE remains OMITTED, as in every original IFUM bit-cell instance.',
            '    .freeze(state_freeze),.guest_held(state_guest_held),.frozen(),',
            f"    .request(state_accept && state_valid && state_package==7'd{package}),",
            '    .write(state_write),.word_index(state_row),.state_data(state_data),',
            f'    .done(),.error(),.result(state_memory_result[{i}]));']
        code+=[f'  assign {name}=state_bank_q_{i}[{bit}];'for bit,name in enumerate(output)]
        banks.append(dict(index=package,word=package<<11,bank=i,width=width,members=members,
            address=address,data=data,outputs=output,we=p['p13'],ce='OMITTED',ram_words=1024,pipeline_word=1024))
    phase_index=nf+len(banks);packed='{'+','.join(reversed(PHASES))+'}'
    body=re.sub(r'^always\s*@\s*\(\s*posedge\s+sys_clk\s*\)\s*','',block,count=1)
    fabric=f'''
  // 18-bit address:7-bit package/11-bit row. FF/phase row0; RAM0..1023/raw
  // pipeline1024. Every other address acknowledges error without state writes.
  wire state_hold=state_freeze && state_guest_held;
  wire state_accept=state_hold && state_frozen && state_request;
  wire [6:0] state_package=state_index[17:11];
  wire [10:0] state_row=state_index[10:0];
  wire state_is_ff=state_package<7'd{nf} && state_row==0;
  wire state_is_ram=state_package>=7'd{nf} && state_package<7'd{phase_index};
  wire state_is_phase=state_package==7'd{phase_index} && state_row==0;
  wire state_valid=state_is_ff || (state_is_ram && state_row<=11'd1024) || state_is_phase;
  wire [17:0] state_ff_result[0:{nf-1}];
  wire [17:0] state_memory_result[0:{len(banks)-1}];
  reg state_response_memory;
  reg [3:0] state_response_memory_index;
  reg [17:0] state_aux_result;
  assign state_result=state_response_memory?state_memory_result[state_response_memory_index]:state_aux_result;
'''+ '\n'.join(code)+f'''
  always @(posedge state_host_clk)begin
    state_frozen<=state_hold;state_done<=state_accept;
    state_error<=state_accept && !state_valid;
    if(state_accept)begin
      state_response_memory<=state_valid && state_is_ram;
      state_response_memory_index<=state_is_ram?4'(state_package-7'd{nf}):4'd0;
      state_aux_result<=state_is_ff?state_ff_result[state_package]:
        (state_is_phase?{{14'b0,{packed}}}:18'b0);
    end
    if(state_hold)begin
      if(state_accept && state_write && state_is_phase){packed}<=state_data[3:0];
    end else if(state_guest_advance){body}
  end
'''
    text=contb.replace_once(text,'endmodule',fabric+'\nendmodule')
    clones['f10415_word_bank.sv']=(ROOT/'fpga/mister/snapshot/f10415_word_bank.sv').read_text()
    if hashlib.sha256((source_root/PROM).read_bytes()).hexdigest()!=PROM_SHA or \
            hashlib.sha256((ROOT/PROM).read_bytes()).hexdigest()!=PROM_SHA:
        raise ValueError('IFU immutable PROM differs from frozen board')
    prom_instances=[m for m in INSTANCES.finditer(original)if m[1]=='cell_SG10139']
    if len(prom_instances)!=1 or PROM not in prom_instances[0][2]:raise ValueError('IFU PROM binding changed')
    bits=sum(r['width']for r in rows)+27648+27+4
    if nf!=94 or bits!=28653:raise ValueError('unexpected IFU modeled state size')
    manifest=dict(module=MODULE,original_module=ORIGINAL_MODULE,original_sha256=im.BOARD_SHA,
        ff=rows,banks=banks,phase=dict(package_index=phase_index,word=phase_index<<11,fields=PHASES,width=4),
        logical_state_bits=bits,ff_bits=974,ram_bits=27648,read_pipeline_bits=27,phase_bits=4,index_bits=18,row_bits=11,
        immutable_proms=[dict(package=prom_instances[0][3],file=PROM,sha256=PROM_SHA,words=32,width=8)],
        original_sources={'verilog/cells/'+kind+'.v':hashlib.sha256((source_root/'verilog/cells'/f'{kind}.v').read_bytes()).hexdigest()
            for kind in sorted({m[1]for m in INSTANCES.finditer(original)})},grouping=grouping,
        generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
        cloned_sources={name:hashlib.sha256(s.encode()).hexdigest()for name,s in clones.items()},
        scope='Complete modeled frozen IFU only;2-state model with original CE omission preserved. No external/global phase/barrier/device/ARM state or physical machine snapshot.')
    return text,manifest,clones


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--source-root',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
    args.output.mkdir(parents=True,exist_ok=False);board,manifest,clones=generate(args.source_root.resolve())
    (args.output/'IFU_full_snapshot.sv').write_text(board)
    (args.output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    for name,text in clones.items():(args.output/name).write_text(text)
