#!/usr/bin/env python3
"""Complete modeled actual native DskEth board state, including passive monitor.

Preserves actual SYSPER4 binding, CRC PRECAP0/1, SN74123 TW400 and oscillator
23530/SYSPER8. Caller holds all consumers; no global barrier/timer correction.
"""
import argparse,hashlib,json,re
from collections import Counter
from pathlib import Path
import generate_contb_full as contb
import generate_snapshot_ff as ff
import generate_processor_ff as proc
import generate_dsketh_ff as dskff
import generate_jk as jk
import generate_f10145 as small
import generate_crc9401 as crc
import generate_sn74123 as timer
import generate_oscillator as osc
import generate_receive_monitor as monitor
ROOT=Path(__file__).resolve().parents[3]
SOURCE='verilog/generated/DskEth-Rev-Cf.v'
BOARD_SHA='14f3a2c55450231c0bd0feafa22b5e0c08f09eddad8358fec2797baf46ec5f81'
ORIGINAL_MODULE='DskEth_m_Rev_m_Cf_rxmonitor';MODULE='DskEth_full_snapshot_experiment'
FIELDS={**contb.FIELDS,**ff.FIELDS,**proc.FIELDS,**dskff.FIELDS,'cell_MC10135':jk.FIELDS}
COUNTS={'cell_SN74LS169':4,'cell_SN74123':2,'cell_SN74LS174':5,'cell_F10016':12,
 'cell_MC10176':27,'cell_MC10231':19,'cell_MC1650':4,'cell_MC10135':10,
 'cell_MC10136':6,'cell_F10145A':18,'cell_MC10173':13,'cell_MC10175':2,
 'cell_F10000':6,'cell_F9401':2,'cell_MC10141':8,'cell_K1115A':1,'cell_SE10231':1}
INSTANCES=re.compile(r'\b(cell_\w+)(\s*(?:#\(.*?\))?\s+)(u_\w+)\s*\((.*?)\)\s*;',re.S)
PHASE_FIELDS=[('Clk1',1)]+[('Clock1_p_'+n,1)for n in ('Bb','Bd','Ca','Cb','Cc','Da','Db','Dd')]
PROCESS='''always @(posedge sys_clk) begin
    Clk1 <= Clk1__lead;
    Clock1_p_Bb <= Clock1_p_Bb__lead;
    Clock1_p_Bd <= Clock1_p_Bd__lead;
    Clock1_p_Ca <= Clock1_p_Ca__lead;
    Clock1_p_Cb <= Clock1_p_Cb__lead;
    Clock1_p_Cc <= Clock1_p_Cc__lead;
    Clock1_p_Da <= Clock1_p_Da__lead;
    Clock1_p_Db <= Clock1_p_Db__lead;
    Clock1_p_Dd <= Clock1_p_Dd__lead;
  end'''
PROM_HASHES={
 'a20':'75af10fa2e3eb07e2bcfee1ea7aca6ee686dfdca2b59b97c83059faf97d39aed',
 'a21':'a2f364d6dccec3a274e6c5346dfcb148fd660a6c47afdbb9469e6788388fc593',
 'b14':'8cf2f170f0c89827f7129a3f97b4faa85a7448874f78b1d4eb8c288f25e686e0',
 'd05':'58bdddb8227ab1bd4e9b96c800c5b1a48103cbca361ae5ba89dff53cc555cc32',
 'd21':'467ceef97c456d061bd9131198d0b5562ca83ae469ace1307d8a79cc7c7c7ba1',
 'h09':'cded4bda41485e092bf9c7ddb6c69abb479dbaae2eea007f2140eba4e7a78898',
 'h10':'ddcaa5f5605cb71190e00707b07e2e95c399e684e5ca34e3b71eba4e4b69bd30',
 'h11':'1e9eb132b0a77c890f8fbf5b4e9a373926c2a10742a9526f6501a1181ebe17ba',
 'h14':'db8698c539cf0deeb5000ccd9f99e933927baa3b4e494be502c0047ef21fb390',
 'h15':'7756a8ad3065078c523b452b1daf07c754303a00e55964d80fe101751bcb922e',
 'h16':'a3fa91d53574edac6387ae8aac9c281640f43f07e43e998822a1d1abf661121d',
 'h22':'f6a3cf5059882919c6ff29da207cf42534eea646d41b5550c15d9e9500a60962',
 'l10':'82cfb1141fb5549f22811a65615bf3afb4c30c66a2299c79f7972032b42d5908',
 'l15':'aa633214d8718043294a237a3695696a501eebaaaf3f30818ae471409a780d6e'}


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def generate(root):
 original=(root/SOURCE).read_text()
 if digest(root/SOURCE)!=BOARD_SHA:raise ValueError('unreviewed actual native DskEth board')
 backplane=(root/'build/rtl-executive/native16-tx-cable/dorado_native_backplane.v').read_text()
 if backplane.count(ORIGINAL_MODULE+' #(.SYSPER(SYSPER)) b_DskEth (')!=1:raise ValueError('actual DskEth binding differs')
 text=contb.replace_once(original,'module '+ORIGINAL_MODULE+' ','module '+MODULE+' ')
 text=contb.replace_once(text,'    input  wire sys_clk,','''    input wire state_host_clk,state_guest_advance,state_freeze,state_guest_held,state_request,state_write,
    input wire [12:0] state_index,input wire [17:0] state_data,
    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,
    output wire [17:0] state_result,
    input  wire sys_clk,''')
 text,process=ff.extract_once(text,PROCESS)
 if re.search(r'\b(always\w*|initial|final)\b',ff.masked_comments(text)):raise ValueError('unreviewed DskEth board process')
 normal=re.sub(r'^always\s*@\s*\(\s*posedge\s+sys_clk\s*\)\s*','',process,count=1)
 packages=[(m[1],m[3])for m in INSTANCES.finditer(original)if m[1]in COUNTS]
 if dict(Counter(k for k,_ in packages))!=COUNTS:raise ValueError('DskEth sequential socket inventory differs')
 all_clones={**contb.ff_sources(),**ff.generate(),**proc.generate(),**{k+'_snapshot.sv':s for k,(s,_)in dskff.generate().items()},'cell_MC10135_snapshot.v':jk.generate()[0]}
 rows=[];clones={};memories=[]
 for kind,name in packages:
  if kind not in FIELDS:continue
  fields=[];offset=0
  for field,width in FIELDS[kind]:fields.append(dict(name=field,width=width,lsb=offset));offset+=width
  i=len(rows);rows.append(dict(index=i,word=i<<5,package=name,kind=kind,width=offset,fields=fields))
  n=kind+'_snapshot'+('.sv'if kind in dskff.FIELDS else'.v');clones[n]=all_clones[n]
 for kind,source,schema in [('cell_F10145A',*small.generate()),('cell_SN74123',*timer.generate()),('cell_F9401',*crc.generate()),('cell_K1115A',*osc.generate())]:
  clones[kind+'_snapshot.sv']=source
  for k,name in packages:
   if k!=kind:continue
   i=len(rows)+len(memories);r=dict(index=i,word=i<<5,memory_index=len(memories),package=name,kind=kind)
   if k=='cell_F10145A':r.update(words=16,width=4,write_window=schema['write_window'],logical_state_bits=74)
   else:
    r.update(words=4 if k=='cell_SN74123'else 2,width=18,logical_state_bits=schema['logical_state_bits'],fields=schema['fields'],transport_words=schema['words'])
    if k=='cell_K1115A':r['parameters']=dict(FREQ_KHZ=23530,SYSPER=8)
    if k=='cell_SN74123':r['parameters']=dict(TW_NS=3000,SYSPER=4,TW=400)
    if k=='cell_F9401':r['parameters']=dict(PRECAP=1 if name=='u_j05'else 0)
   memories.append(r)
 mon_source,mon_schema=monitor.generate();clones['dorado_receive_monitor_snapshot.sv']=mon_source
 i=len(rows)+len(memories);memories.append(dict(index=i,word=i<<5,memory_index=len(memories),package='u_receive_monitor',kind='dorado_receive_monitor',words=2,width=18,logical_state_bits=34,fields=mon_schema['fields'],transport_words=mon_schema['words']))
 nf,nm=len(rows),len(memories);phase_index=nf+nm;by_name={r['package']:r for r in rows+memories}
 def replace(m):
  kind,gap,name,body=m.groups()
  if name not in by_name:return m[0]
  r=by_name[name];i=r['index']
  if kind in FIELDS:
   return kind+'_snapshot'+gap+name+f''' (
    .state_host_clk(state_host_clk),.state_advance(state_guest_advance),.state_hold(state_hold),
    .state_write(state_accept && state_write && state_is_ff && state_package==8'd{i}),
    .state_data(state_data),.state_result(state_ff_result[{i}]),'''+body+');'
  mi=r['memory_index'];request=f"state_accept && state_valid && state_package==8'd{i}"
  limits={'cell_SN74123':'#(.SYSPER(SYSPER))','cell_F9401':'#(.PRECAP(1))'if name=='u_j05'else'','cell_K1115A':'#(.FREQ_KHZ(23530), .SYSPER(2*SYSPER))'}
  if kind in limits and gap.strip()!=limits[kind]:raise ValueError('native parameter binding differs:'+name)
  if kind=='cell_F10145A':index='state_row';data='state_data[9:0]';result=f'state_narrow_result[{mi}]'
  else:index='state_row[2:0]'if kind=='cell_SN74123'else'state_row[1:0]';data='state_data';result=f'state_memory_result[{mi}]'
  return kind+'_snapshot'+gap+name+f''' (
    .state_host_clk(state_host_clk),.state_advance(state_guest_advance),.state_freeze(state_freeze),.state_guest_held(state_guest_held),
    .state_request({request}),.state_write(state_write),.state_index({index}),.state_data({data}),
    .state_frozen(),.state_done(),.state_error(),.state_result({result}),'''+body+');'
 text=INSTANCES.sub(replace,text)
 mon_rx=re.compile(r'\bdorado_receive_monitor\s+(u_receive_monitor)\s*\((.*?)\)\s*;',re.S)
 matches=list(mon_rx.finditer(text))
 if len(matches)!=1:raise ValueError('native receive monitor differs')
 r=by_name['u_receive_monitor'];mi=r['memory_index']
 text=mon_rx.sub(lambda m:f'''dorado_receive_monitor_snapshot {m[1]} (
    .state_host_clk(state_host_clk),.state_advance(state_guest_advance),.state_freeze(state_freeze),.state_guest_held(state_guest_held),
    .state_request(state_accept && state_valid && state_package==8'd{r['index']}),.state_write(state_write),.state_index(state_row[1:0]),.state_data(state_data),
    .state_frozen(),.state_done(),.state_error(),.state_result(state_memory_result[{mi}]),'''+m[2]+');',text)
 phase_fields=[dict(name=n,width=w,lsb=i)for i,(n,w)in enumerate(PHASE_FIELDS)];packed='{'+','.join(n for n,_ in reversed(PHASE_FIELDS))+'}'
 valid=[f"(state_package==8'd{r['index']} && state_row<=5'd{16 if r['kind']=='cell_F10145A'else r['words']-1})"for r in memories]
 fabric=f'''
 wire state_hold=state_freeze && state_guest_held;
 wire state_accept=state_hold && state_frozen && state_request;
 wire [7:0] state_package=state_index[12:5];wire [4:0] state_row=state_index[4:0];
 wire state_is_ff=state_package<8'd{nf} && state_row==0;
 wire state_is_memory=state_package>=8'd{nf} && state_package<8'd{phase_index};
 wire state_is_phase=state_package==8'd{phase_index} && state_row==0;
 wire state_valid=state_is_ff || state_is_phase ||
 '''+' ||\n '.join(valid)+f''';
 wire [17:0] state_ff_result[0:{nf-1}];wire [9:0] state_narrow_result[0:{nm-1}];
 wire [17:0] state_memory_result[0:{nm-1}];
 reg state_response_memory;reg [4:0] state_response_memory_index;reg [17:0] state_aux_result;
 assign state_result=state_response_memory?state_memory_result[state_response_memory_index]:state_aux_result;
'''
 for r in memories:
  if r['kind']=='cell_F10145A':fabric+=f" assign state_memory_result[{r['memory_index']}]={{8'b0,state_narrow_result[{r['memory_index']}]}};\n"
 fabric+=f''' always @(posedge state_host_clk)begin
  state_frozen<=state_hold;state_done<=state_accept;state_error<=state_accept && !state_valid;
  if(state_accept)begin
   state_response_memory<=state_valid && state_is_memory && !state_write;
   state_response_memory_index<=state_is_memory?5'(state_package-8'd{nf}):5'd0;
   state_aux_result<=state_write?18'b0:state_is_ff?state_ff_result[state_package]:(state_is_phase?{{9'b0,{packed}}}:18'b0);
  end
  if(state_hold)begin
   if(state_accept && state_write && state_is_phase){packed}<=state_data[8:0];
  end else if(state_guest_advance)begin
{normal}
  end
 end
'''
 text=contb.replace_once(text,'endmodule',fabric+'endmodule');kinds=sorted({m[1]for m in INSTANCES.finditer(original)});proms=[]
 comment_only=[]
 for k in kinds:
  a=root/'verilog/cells'/(k+'.v');b=ROOT/'verilog/cells'/(k+'.v')
  if a.read_bytes()==b.read_bytes():continue
  # Actual Native58 still carries the earlier MC10170 explanatory comment.
  # Preserve its exact compiled file; permit only the reviewed non-code delta.
  if k!='cell_MC10170' or [t[0]for t in ff.tokens(a.read_text())]!=[t[0]for t in ff.tokens(b.read_text())]:
   raise ValueError('qualified original cell differs:'+k)
  comment_only.append(dict(file='verilog/cells/'+k+'.v',compiled_sha256=digest(a),current_sha256=digest(b),review='Comment-only, exact Verilog token stream unchanged; compiled original is retained.'))
 if digest(root/monitor.SOURCE)!=monitor.SHA:raise ValueError('actual receive monitor source differs')
 for m in INSTANCES.finditer(original):
  if m[1]not in ('cell_MCM10149','cell_SG10139','cell_SN74S288'):continue
  name='verilog/proms/packages/DskEth-'+m[3][2:]+'.mem'
  if '"'+name+'"'not in m[2]or m[3][2:]not in PROM_HASHES:raise ValueError('unknown DskEth PROM')
  wanted=PROM_HASHES[m[3][2:]]
  if digest(root/name)!=wanted or digest(ROOT/name)!=wanted:raise ValueError('actual PROM differs')
  proms.append(dict(package=m[3],file=name,sha256=wanted,words=256 if m[1]=='cell_MCM10149'else 32,width=4 if m[1]=='cell_MCM10149'else 8))
 bits=sum(r['width']for r in rows)+sum(r['logical_state_bits']for r in memories)+9
 if(nf,nm,phase_index,bits,len(proms))!=(117,24,141,2656,14):raise ValueError('unexpected complete DskEth geometry:'+str((nf,nm,phase_index,bits,len(proms))))
 manifest=dict(module=MODULE,original_module=ORIGINAL_MODULE,source=SOURCE,original_sha256=BOARD_SHA,ff=rows,memories=memories,
  parameters=dict(SYSPER=4),phase=dict(package_index=phase_index,word=phase_index<<5,width=9,fields=phase_fields),logical_state_bits=bits,index_bits=13,row_bits=5,
  immutable_proms=proms,ff_bits=sum(r['width']for r in rows),ram_bits=18*64,
  original_sources={**{'verilog/cells/'+k+'.v':digest(root/'verilog/cells'/(k+'.v'))for k in kinds},monitor.SOURCE:monitor.SHA},
  cloned_sources={n:hashlib.sha256(s.encode()).hexdigest()for n,s in clones.items()},generated_sha256=hashlib.sha256(text.encode()).hexdigest(),comment_only_source_deltas=comment_only,scope=__doc__)
 return text,manifest,clones

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False)
 s,m,clones=generate(a.source_root.resolve());(a.output/'DskEth_full_snapshot.sv').write_text(s);(a.output/'manifest.json').write_text(json.dumps(m,indent=2)+'\n')
 for n,s in clones.items():(a.output/n).write_text(s)
