#!/usr/bin/env python3
"""Complete modeled actual native DispY_field board state-access experiment.

Preserves actual display-board and oscillator defaults SYSPER16; fabric SYSPER4.
All consumers/inputs require caller-held freeze. No global barrier/timer fix.
"""
import argparse
from collections import Counter
import hashlib,json,re
from pathlib import Path
import generate_contb_full as contb
import generate_snapshot_ff as ff
import generate_jk as jk
import generate_f10145 as small
import generate_async256 as asynchronous
import generate_oscillator as osc

ROOT=Path(__file__).resolve().parents[3]
SOURCE='build/rtl-executive/native16-tx-cable/DispY_field.v'
BOARD_SHA='48ae129bd7523b5efb9db87d8f43cc314186f10fc0a4fe52f681b5c345836b4d'
MODULE='DispY_full_snapshot_experiment';ORIGINAL_MODULE='DispY_m_Rev_m_Cl_field'
FIELDS={**contb.FIELDS,**ff.FIELDS,'cell_MC10135':jk.FIELDS}
COUNTS={'cell_F10016':44,'cell_MC10176':35,'cell_F10000':33,'cell_MC10231':16,
        'cell_MC10135':7,'cell_MC10141':4,'cell_SE10231':1,
        'cell_MB7071H':9,'cell_F10145A':3,'cell_F10415A':3,'cell_K1115A':1}
INSTANCES=re.compile(r'\b(cell_\w+)(\s*(?:#\(.*?\))?\s+)(u_\w+)\s*\((.*?)\)\s*;',re.S)
PHASE_FIELDS=[('clk1_p_'+n,1)for n in ('Aa','Ab','Ca','Cb','Cc','Da','Db','Dc')]
PROCESS='''always @(posedge sys_clk) begin
    clk1_p_Aa <= clk1_p_Aa__lead;
    clk1_p_Ab <= clk1_p_Ab__lead;
    clk1_p_Ca <= clk1_p_Ca__lead;
    clk1_p_Cb <= clk1_p_Cb__lead;
    clk1_p_Cc <= clk1_p_Cc__lead;
    clk1_p_Da <= clk1_p_Da__lead;
    clk1_p_Db <= clk1_p_Db__lead;
    clk1_p_Dc <= clk1_p_Dc__lead;
  end'''
PROM_HASHES={
 'verilog/proms/packages/DispY-i15.mem':'080400e0d0f96ca17bc440c40a8f697bed9938332fa857fc29c565592ffbb182',
 'verilog/proms/packages/DispY-l15.mem':'dde88c8f705492b6b9329b83f5458650166910cc3bf877c7cf5027407e8fa36f'}


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def generate(root):
    original=(root/SOURCE).read_text()
    if digest(root/SOURCE)!=BOARD_SHA:raise ValueError('unreviewed actual native field board')
    if len(re.findall(r'\bSYSPER\b',ff.masked_comments(original)))!=1:raise ValueError('display SYSPER is no longer unused')
    backplane=(root/'build/rtl-executive/native16-tx-cable/dorado_native_backplane.v').read_text()
    if backplane.count(ORIGINAL_MODULE+' b_DispY (')!=1:raise ValueError('actual display default binding changed')
    text=contb.replace_once(original,'module '+ORIGINAL_MODULE+' ','module '+MODULE+' ')
    text=contb.replace_once(text,'    input  wire sys_clk,','''    input wire state_host_clk,state_guest_advance,state_freeze,state_guest_held,
    input wire state_request,state_write,
    input wire [18:0] state_index,input wire [17:0] state_data,
    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,
    output wire [17:0] state_result,
    input  wire sys_clk,''')
    text,process=ff.extract_once(text,PROCESS)
    if re.search(r'\b(always\w*|initial|final)\b',ff.masked_comments(text)):raise ValueError('unreviewed board process')
    normal=re.sub(r'^always\s*@\s*\(\s*posedge\s+sys_clk\s*\)\s*','',process,count=1)
    packages=[(m[1],m[3])for m in INSTANCES.finditer(original)if m[1]in COUNTS]
    if dict(Counter(k for k,_ in packages))!=COUNTS:raise ValueError('display sequential package inventory differs')
    all_clones={**contb.ff_sources(),**ff.generate(),'cell_MC10135_snapshot.v':jk.generate()[0]}
    rows=[];clones={};memories=[]
    for kind,name in packages:
        if kind not in FIELDS:continue
        fields=[];offset=0
        for field,width in FIELDS[kind]:fields.append(dict(name=field,width=width,lsb=offset));offset+=width
        index=len(rows);rows.append(dict(index=index,word=index<<11,package=name,kind=kind,width=offset,fields=fields))
        clones[kind+'_snapshot.v']=all_clones[kind+'_snapshot.v']
    small_source,small_schema=small.generate();clones['cell_F10145A_snapshot.sv']=small_source
    mb_source,mb_schema=asynchronous.generate('MB7071H');clones['cell_MB7071H_snapshot.sv']=mb_source
    clones['f10415_word_bank.sv']=(ROOT/'fpga/mister/snapshot/f10415_word_bank.sv').read_text()
    osc_source,osc_schema=osc.generate();clones['cell_K1115A_snapshot.sv']=osc_source
    for kind in ('cell_MB7071H','cell_F10145A','cell_F10415A','cell_K1115A'):
        for k,name in packages:
            if k!=kind:continue
            index=len(rows)+len(memories);r=dict(index=index,word=index<<11,memory_index=len(memories),package=name,kind=kind)
            if k=='cell_MB7071H':r.update(words=256,width=4,logical_state_bits=1024)
            elif k=='cell_F10145A':r.update(words=16,width=4,write_window=small_schema['write_window'],logical_state_bits=74)
            elif k=='cell_F10415A':r.update(words=1024,width=1,raw_pipeline_word=1024,logical_state_bits=1025)
            else:r.update(words=2,width=18,logical_state_bits=33,fields=osc_schema['fields'],parameters=dict(FREQ_KHZ=50000,SYSPER=16))
            memories.append(r)
    nf,nm=len(rows),len(memories);phase_index=nf+nm;by_name={r['package']:r for r in rows+memories}
    def replace(m):
        kind,gap,name,body=m.groups()
        if name not in by_name:return m[0]
        r=by_name[name];i=r['index']
        if kind in FIELDS:
            return kind+'_snapshot'+gap+name+''' (
    .state_host_clk(state_host_clk),.state_advance(state_guest_advance),.state_hold(state_hold),
    .state_write(state_accept && state_write && state_is_ff && state_package==8'd'''+str(i)+f'''),
    .state_data(state_data),.state_result(state_ff_result[{i}]),'''+body+');'
        mi=r['memory_index'];request=f"state_accept && state_valid && state_package==8'd{i}"
        if kind=='cell_F10415A':
            pins=dict(re.findall(r'\.(\w+)\s*\(([^()]*)\)',body));expected={'sys_clk','p2','p3','p4','p5','p6','p7','p9','p10','p11','p12','p15','p13','p1'}
            if set(pins)!=expected or pins['sys_clk']!='sys_clk' or gap.strip():
                raise ValueError('unreviewed display word-memory pins/omitted CE')
            address=','.join(pins['p'+str(n)]for n in (2,3,4,5,6,7,9,10,11,12))
            r['pins']=pins;r['instance']='display_word_'+str(mi);r['ce']='OMITTED, as in original instance'
            return f'''dorado_snapshot_f10415_word_bank #(.WIDTH(1)) {r['instance']}(
    .host_clk(state_host_clk),.guest_advance(state_guest_advance),
    .address({{{address}}}),.data({pins['p15']}),.we_n({pins['p13']}),.q({pins['p1']}),
    // CE remains omitted, as in the original display memory socket.
    .freeze(state_freeze),.guest_held(state_guest_held),.frozen(),.request({request}),.write(state_write),
    .word_index(state_row),.state_data(state_data),.done(),.error(),.result(state_memory_result[{mi}]));'''
        if kind=='cell_K1115A'and gap.strip()!='#(.FREQ_KHZ(50000))':raise ValueError('actual oscillator binding differs')
        index='state_row[8:0]'if kind=='cell_MB7071H'else'state_row[4:0]'if kind=='cell_F10145A'else'state_row[1:0]'
        data='state_data[3:0]'if kind=='cell_MB7071H'else'state_data[9:0]'if kind=='cell_F10145A'else'state_data'
        result=f'state_narrow_result[{mi}][3:0]'if kind=='cell_MB7071H'else f'state_narrow_result[{mi}]'if kind=='cell_F10145A'else f'state_memory_result[{mi}]'
        return kind+'_snapshot'+gap+name+f''' (
    .state_host_clk(state_host_clk),.state_advance(state_guest_advance),.state_freeze(state_freeze),.state_guest_held(state_guest_held),
    .state_request({request}),.state_write(state_write),.state_index({index}),.state_data({data}),
    .state_frozen(),.state_done(),.state_error(),.state_result({result}),'''+body+');'
    text=INSTANCES.sub(replace,text)
    fields=[dict(name=n,width=w,lsb=i)for i,(n,w)in enumerate(PHASE_FIELDS)]
    packed='{'+','.join(n for n,_ in reversed(PHASE_FIELDS))+'}'
    ranges={k:[r['index']for r in memories if r['kind']==k]for k in ('cell_MB7071H','cell_F10145A','cell_F10415A','cell_K1115A')}
    valid=[f"(state_package==8'd{r['index']} && state_row<=11'd{255 if r['kind']=='cell_MB7071H'else 16 if r['kind']=='cell_F10145A'else 1024 if r['kind']=='cell_F10415A'else 1})"for r in memories]
    fabric=f'''
  // 19-bit index:8package/11row. All valid state is explicit; no row alias.
  wire state_hold=state_freeze && state_guest_held;
  wire state_accept=state_hold && state_frozen && state_request;
  wire [7:0] state_package=state_index[18:11];wire [10:0] state_row=state_index[10:0];
  wire state_is_ff=state_package<8'd{nf} && state_row==0;
  wire state_is_memory=state_package>=8'd{nf} && state_package<8'd{phase_index};
  wire state_is_phase=state_package==8'd{phase_index} && state_row==0;
  wire state_valid=state_is_ff || state_is_phase ||
    '''+' ||\n    '.join(valid)+f''';
  wire [17:0] state_ff_result[0:{nf-1}];
  wire [9:0] state_narrow_result[0:{nm-1}];
  wire [17:0] state_memory_result[0:{nm-1}];
  reg state_response_memory;reg [3:0] state_response_memory_index;reg [17:0] state_aux_result;
  assign state_result=state_response_memory?state_memory_result[state_response_memory_index]:state_aux_result;
'''
    for r in memories:
        i=r['memory_index']
        if r['kind']=='cell_MB7071H':fabric+=f"  assign state_memory_result[{i}]={{14'b0,state_narrow_result[{i}][3:0]}};\n"
        elif r['kind']=='cell_F10145A':fabric+=f"  assign state_memory_result[{i}]={{8'b0,state_narrow_result[{i}]}};\n"
    fabric+=f'''  always @(posedge state_host_clk)begin
    state_frozen<=state_hold;state_done<=state_accept;state_error<=state_accept && !state_valid;
    if(state_accept)begin
      state_response_memory<=state_valid && state_is_memory && !state_write;
      state_response_memory_index<=state_is_memory?4'(state_package-8'd{nf}):4'd0;
      state_aux_result<=state_write?18'b0:state_is_ff?state_ff_result[state_package]:(state_is_phase?{{10'b0,{packed}}}:18'b0);
    end
    if(state_hold)begin
      if(state_accept && state_write && state_is_phase){packed}<=state_data[7:0];
    end else if(state_guest_advance)begin
{normal}
    end
  end
'''
    text=contb.replace_once(text,'endmodule',fabric+'endmodule')
    kinds=sorted({m[1]for m in INSTANCES.finditer(original)});proms=[]
    for kind in set(FIELDS)&set(COUNTS)|{'cell_MB7071H','cell_F10145A','cell_F10415A','cell_K1115A','cell_MCM10149'}:
        if (root/'verilog/cells'/f'{kind}.v').read_bytes()!=(ROOT/'verilog/cells'/f'{kind}.v').read_bytes():raise ValueError('qualified cell differs:'+kind)
    for m in INSTANCES.finditer(original):
        if m[1]!='cell_MCM10149':continue
        names=re.findall(r'"(verilog/proms/[^\"]+)"',m[2])
        if len(names)!=1 or names[0]not in PROM_HASHES:raise ValueError('unreviewed display PROM binding')
        name=names[0]
        if digest(root/name)!=PROM_HASHES[name]or digest(ROOT/name)!=PROM_HASHES[name]:raise ValueError('display PROM differs')
        proms.append(dict(package=m[3],file=name,sha256=PROM_HASHES[name],words=256,width=4))
    bits=sum(r['width']for r in rows)+sum(r['logical_state_bits']for r in memories)+8
    if (nf,nm,phase_index,bits,len(proms))!=(140,16,156,13993,2):raise ValueError('unexpected complete display geometry')
    manifest=dict(module=MODULE,original_module=ORIGINAL_MODULE,source=SOURCE,original_sha256=BOARD_SHA,ff=rows,memories=memories,
        parameters=dict(SYSPER=16),phase=dict(package_index=phase_index,word=phase_index<<11,width=8,fields=fields),logical_state_bits=bits,index_bits=19,row_bits=11,
        immutable_proms=proms,ff_bits=sum(r['width']for r in rows),ram_bits=9*1024+3*64+3*1024,
        original_sources={'verilog/cells/'+k+'.v':digest(root/'verilog/cells'/f'{k}.v')for k in kinds},
        cloned_sources={n:hashlib.sha256(v.encode()).hexdigest()for n,v in clones.items()},generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
        scope=__doc__)
    return text,manifest,clones

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False)
    s,m,clones=generate(a.source_root.resolve());(a.output/'DispY_full_snapshot.sv').write_text(s);(a.output/'manifest.json').write_text(json.dumps(m,indent=2)+'\n')
    for n,v in clones.items():(a.output/n).write_text(v)
