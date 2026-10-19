#!/usr/bin/env python3
"""Complete state access for the frozen native MemD, STORAGE_INPUT_DELAY=2.

Experiments only. All consumers/external inputs must be held by the caller.
Both native storage input words and their edge history are exported. Other
delay configurations explicitly reject. No global machine barrier is added.
"""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re

import audit_memd_im as im
import generate_contb_full as contb
import generate_snapshot_ff as ff
import generate_processor_ff as extra
import generate_f10145 as small

ROOT=Path(__file__).resolve().parents[3]
MODULE='MemD_full_snapshot_experiment'
ORIGINAL_MODULE='MemD_m_Rev_m_Ca'
FIELDS={**contb.FIELDS,**ff.FIELDS,**extra.FIELDS}
COUNTS={'cell_MC10231':18,'cell_MC10176':44,'cell_F10016':18,
        'cell_F10000':5,'cell_MC10175':4,'cell_SE10231':2,'cell_MC10141':1,
        'cell_F10145A':8,'cell_F10470':72}
INSTANCES=im.INSTANCES
DELAY='''generate if (STORAGE_INPUT_DELAY == 0) begin : storage_rx_direct
    assign storage_rx_data = storage_rx_raw;
  end else begin : storage_rx_pipeline
    reg [17:0] words [0:STORAGE_INPUT_DELAY-1];
    reg clock_d = 0;
    integer j;
    initial for (integer k=0; k<STORAGE_INPUT_DELAY; k=k+1) words[k]=0;
    always @(posedge sys_clk) begin
      clock_d <= clk2_p_Aa;
      if (clk2_p_Aa && !clock_d) begin
        words[0] <= storage_rx_raw;
        for (j=1; j<STORAGE_INPUT_DELAY; j=j+1) words[j] <= words[j-1];
      end
    end
    assign storage_rx_data = words[STORAGE_INPUT_DELAY-1];
  end endgenerate'''


def generate(source_root):
    source=source_root/'verilog/generated/MemD-Rev-Ca.v';original=source.read_text()
    grouping=im.audit(source_root)
    if hashlib.sha256((source_root/'verilog/cells/cell_F10470.v').read_bytes()).hexdigest()!='90c6ae2805dac5ec07952fa9f0344154317ebfc602241e25350bf7bca1fe0d30':
        raise ValueError('qualified F10470 source differs from frozen MemD')
    text=contb.replace_once(original,'module '+ORIGINAL_MODULE+' ', 'module '+MODULE+' ')
    text=contb.replace_once(text,'    input  wire sys_clk,','''    input wire state_host_clk,state_guest_advance,state_freeze,state_guest_held,
    input wire state_request,state_write,
    input wire [19:0] state_index,input wire [17:0] state_data,
    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,
    output wire [17:0] state_result,
    input  wire sys_clk,''')
    # Remove the exact original branch before checking board-owned processes;
    # its native two-word state is reinstalled under the host/hold contract.
    text=contb.replace_once(text,DELAY,'')
    phases=re.findall(r'^\s*reg\s+(\w+)\s*=\s*1\'b1;',text,re.M)
    if len(phases)!=10 or len(set(phases))!=10:raise ValueError('unreviewed MemD phase registers')
    phase='always @(posedge sys_clk) begin\n'+''.join(f'{n} <= {n}__lead;\n'for n in phases)+'end'
    text,block=ff.extract_once(text,phase)
    if re.search(r'\b(always\w*|initial|final)\b',ff.masked_comments(text)):
        raise ValueError('extra MemD board-owned process')
    packages=[(m[1],m[3])for m in INSTANCES.finditer(original)if m[1]in COUNTS]
    if dict(Counter(k for k,_ in packages))!=COUNTS:raise ValueError('MemD sequential inventory differs')
    rows=[];files=[];banks=[]
    all_clones={**contb.ff_sources(),**ff.generate(),**extra.generate()};clones={}
    for kind,name in packages:
        if kind not in FIELDS:continue
        fields=[];offset=0
        for field,width in FIELDS[kind]:fields.append(dict(name=field,width=width,lsb=offset));offset+=width
        index=len(rows);rows.append(dict(index=index,word=index<<13,package=name,kind=kind,width=offset,fields=fields))
        clones[kind+'_snapshot.v']=all_clones[kind+'_snapshot.v']
    small_source,small_manifest=small.generate()
    if (source_root/'verilog/cells/cell_F10145A.v').read_bytes()!=(ROOT/'verilog/cells/cell_F10145A.v').read_bytes():
        raise ValueError('qualified small RAM differs from frozen MemD')
    clones['cell_F10145A_snapshot.sv']=small_source
    for kind,name in packages:
        if kind=='cell_F10145A':
            index=len(rows)+len(files)
            files.append(dict(index=index,word=index<<13,package=name,kind=kind,
                ram=small_manifest['ram'],write_window=small_manifest['write_window'],memory_index=len(files)))
    by_name={r['package']:r for r in rows+files};nf=len(rows);ns=len(files)
    def replace(m):
        kind,gap,name,body=m.groups()
        if kind=='cell_F10470':return ''
        if name not in by_name:return m[0]
        row=by_name[name];index=row['index']
        if kind in FIELDS:
            ports=f'''
    .state_host_clk(state_host_clk),.state_advance(state_guest_advance),.state_hold(state_hold),
    .state_write(state_accept && state_write && state_is_ff && state_package==7'd{index}),
    .state_data(state_data),.state_result(state_ff_result[{index}]),'''
        else:
            ports=f'''
    .state_host_clk(state_host_clk),.state_advance(state_guest_advance),
    .state_freeze(state_freeze),.state_guest_held(state_guest_held),
    .state_request(state_accept && state_valid && state_package==7'd{index}),.state_write(state_write),
    .state_index(state_row[4:0]),.state_data(state_data[9:0]),
    .state_frozen(),.state_done(),.state_error(),.state_result(state_small_result[{row['memory_index']}] ),'''
        return kind+'_snapshot'+gap+name+' ('+ports+body+');'
    text=INSTANCES.sub(replace,text)
    pins={name:p for kind,name,p in im.logic.instances(original)if kind=='cell_F10470'}
    resolve,_,_=im.review_network(original,source_root);code=[]
    for i,g in enumerate(grouping['groups']):
        members=g['members'];p=pins[members[0]['instance']];width=g['width'];package=nf+ns+i
        for member in members:
            other=pins[member['instance']]
            if any(resolve(other['p'+str(n)])!=resolve(p['p'+str(n)])for n in (*im.ADDRESS,15,16)):
                raise ValueError('cache canonical pin identity changed')
        address=[p['p'+str(n)]for n in im.ADDRESS]
        data=[pins[m['instance']]['p17']for m in members]
        outputs=[pins[m['instance']]['p1']for m in members]
        code += [f'  wire [{width-1}:0] state_bank_q_{i};',
          f'  dorado_snapshot_f10470_word_bank #(.WIDTH({width})) snapshot_bank_{i}(',
          '    .host_clk(state_host_clk),.guest_advance(state_guest_advance),',
          '    .address({'+','.join(address)+'}),.data({'+','.join(reversed(data))+'}),',
          f'    .we_n({p["p15"]}),.ce_n({p["p16"]}),.q(state_bank_q_{i}),',
          '    .freeze(state_freeze),.guest_held(state_guest_held),.frozen(),',
          f"    .request(state_accept && state_valid && state_package==7'd{package}),",
          '    .write(state_write),.word_index(state_row),.state_data(state_data),',
          f'    .done(),.error(),.result(state_memory_result[{ns+i}]));']
        code +=[f'  assign {name}=state_bank_q_{i}[{bit}];'for bit,name in enumerate(outputs)]
        banks.append(dict(index=package,word=package<<13,bank=i,memory_index=ns+i,width=width,
            members=members,address=address,data=data,outputs=outputs,we=p['p15'],ce=p['p16'],ram_words=4096,pipeline_word=4096))
    phase_index=nf+ns+len(banks);storage_index=phase_index+1
    nm=ns+len(banks);packed='{'+','.join(reversed(phases))+'}'
    if phase_index>=128 or nm!=22:raise ValueError('MemD package/response index review required')
    body=re.sub(r'^always\s*@\s*\(\s*posedge\s+sys_clk\s*\)\s*','',block,count=1)
    fabric=f'''
  // Native55's actual QSF binds two storage-input word-clock stages.
  generate if(STORAGE_INPUT_DELAY!=2)begin:unsupported_storage_delay
    initial $fatal(1,"MemD snapshot requires STORAGE_INPUT_DELAY=2");
  end endgenerate
  generate begin:storage_rx_pipeline
    reg [17:0] words[0:1];
    reg clock_d=0;
    initial begin words[0]=0;words[1]=0;end
    always @(posedge state_host_clk)begin
      if(state_hold)begin
        if(state_accept && state_write && state_is_storage)begin
          if(state_row<2)words[state_row[0]]<=state_data;
          else if(state_row==2)clock_d<=state_data[0];
        end
      end else if(state_guest_advance)begin
        clock_d<=clk2_p_Aa;
        if(clk2_p_Aa && !clock_d)begin
          words[0]<=storage_rx_raw;words[1]<=words[0];
        end
      end
    end
    assign storage_rx_data=words[1];
  end endgenerate
  // 20-bit address:7-bit package/13-bit row; FF/phase0,small file0..16,
  // cache0..4095 and raw read pipeline4096. All other indices error.
  wire state_hold=state_freeze && state_guest_held;
  wire state_accept=state_hold && state_frozen && state_request;
  wire [6:0] state_package=state_index[19:13];
  wire [12:0] state_row=state_index[12:0];
  wire state_is_ff=state_package<7'd{nf} && state_row==0;
  wire state_is_small=state_package>=7'd{nf} && state_package<7'd{nf+ns};
  wire state_is_ram=state_package>=7'd{nf+ns} && state_package<7'd{phase_index};
  wire state_is_phase=state_package==7'd{phase_index} && state_row==0;
  wire state_is_storage=state_package==7'd{storage_index} && state_row<=2;
  wire state_valid=state_is_ff || (state_is_small && state_row<=16) ||
                   (state_is_ram && state_row<=13'd4096) || state_is_phase || state_is_storage;
  wire [17:0] state_ff_result[0:{nf-1}];
  wire [9:0] state_small_result[0:{ns-1}];
  wire [17:0] state_memory_result[0:{nm-1}];
  reg state_response_memory;
  reg [4:0] state_response_memory_index;
  reg [17:0] state_aux_result;
  assign state_result=state_response_memory?state_memory_result[state_response_memory_index]:state_aux_result;
'''+''.join(f"  assign state_memory_result[{i}]={{8'b0,state_small_result[{i}]}};\n"for i in range(ns))+'\n'.join(code)+f'''
  always @(posedge state_host_clk)begin
    state_frozen<=state_hold;state_done<=state_accept;
    state_error<=state_accept && !state_valid;
    if(state_accept)begin
      state_response_memory<=state_valid && (state_is_small || state_is_ram);
      state_response_memory_index<=(state_is_small || state_is_ram)?5'(state_package-7'd{nf}):5'd0;
      state_aux_result<=state_is_ff?state_ff_result[state_package]:
        (state_is_phase?{{8'b0,{packed}}}:(state_is_storage?
          (state_row<2?storage_rx_pipeline.words[state_row[0]]:{{17'b0,storage_rx_pipeline.clock_d}}):18'b0));
    end
    if(state_hold)begin
      if(state_accept && state_write && state_is_phase){packed}<=state_data[9:0];
    end else if(state_guest_advance){body}
  end
'''
    text=contb.replace_once(text,'endmodule',fabric+'\nendmodule')
    clones['f10470_word_bank.sv']=(ROOT/'fpga/mister/snapshot/f10470_word_bank.sv').read_text()
    kinds=sorted({m[1]for m in INSTANCES.finditer(original)})
    for kind in FIELDS:
        if kind in kinds and (source_root/'verilog/cells'/f'{kind}.v').read_bytes()!=(ROOT/'verilog/cells'/f'{kind}.v').read_bytes():
            raise ValueError('qualified FF differs from frozen MemD: '+kind)
    bits=sum(r['width']for r in rows)+ns*74+72*4097+len(phases)+37
    if nf!=92 or bits!=296591:raise ValueError('unexpected MemD modeled state size')
    manifest=dict(module=MODULE,original_module=ORIGINAL_MODULE,original_sha256=im.BOARD_SHA,
        ff=rows,files=files,banks=banks,phase=dict(package_index=phase_index,word=phase_index<<13,fields=phases,width=10),
        logical_state_bits=bits,ff_bits=sum(r['width']for r in rows),ram_bits=ns*64+72*4096,
        write_window_bits=ns*10,read_pipeline_bits=72,phase_bits=10,index_bits=20,row_bits=13,
        parameters=dict(STORAGE_INPUT_DELAY=2),grouping=grouping,storage_input_bits=37,
        storage_input=dict(package_index=storage_index,word=storage_index<<13,words=2,width=18,clock_word=2),
        original_sources={'verilog/cells/'+k+'.v':hashlib.sha256((source_root/'verilog/cells'/f'{k}.v').read_bytes()).hexdigest()for k in kinds},
        generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
        cloned_sources={n:hashlib.sha256(s.encode()).hexdigest()for n,s in clones.items()},
        scope='Complete modeled frozen native MemD,STORAGE_INPUT_DELAY=2 only;2-state. External consumers/inputs,global barrier,device/ARM state and physical machine snapshot excluded.')
    return text,manifest,clones


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source-root',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False)
    text,manifest,clones=generate(a.source_root.resolve());(a.output/'MemD_full_snapshot.sv').write_text(text)
    (a.output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    for n,s in clones.items():(a.output/n).write_text(s)
