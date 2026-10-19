#!/usr/bin/env python3
"""Complete modeled actual nominal BaseBoard state access, no production changes.

All40366modeled state bits are exported. Preserve SYSPER4/CLOCKGEN N4,
1725-node6502 relaxation, five complete RIOTs,32SRAM slices as four byte
banks and eight EPROM pipelines including four uninitialized empty sockets.
Caller holds every external consumer; no whole-machine/backend/ARM barrier.
"""
import argparse,hashlib,json,re
from pathlib import Path
import generate_contb_full as cb
import generate_baseboard_ff as newff
import generate_dsketh_ff as oldff
import classify_baseboard_misc as misc
import audit_baseboard_ram as ram
import generate_i2716 as eprom
import generate_mcs6532 as riot
import generate_6502 as cpu
ROOT=Path(__file__).resolve().parents[3]
SOURCE='build/rtl-executive/native16-tx-cable/BaseBd_nominal.v'
BOARD_SHA=misc.BOARD_SHA
ORIGINAL_MODULE='BaseBd_m_Rev_m_Am_nominal'
MODULE='BaseBd_full_snapshot_experiment'
CPU_WRAPPER_SHA='f07cb48d6d0ac2ffb71ce10bb2f8db86dbe2133f88c0baef408a2d691ea2641e'
INSTANCES=re.compile(r'\b(cell_\w+)(\s*(?:#\(.*?\))?\s+)(u_\w+)\s*\((.*?)\)\s*;',re.S)


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def cpu_wrapper(raw):
 if hashlib.sha256(raw).hexdigest()!=CPU_WRAPPER_SHA:raise ValueError('unreviewed actual MCS6502 wrapper')
 text=raw.decode()
 if re.search(r'\b(always\w*|initial|reg|logic)\b',newff.ff.masked_comments(text)):raise ValueError('extra CPU wrapper state')
 text=cb.replace_once(text,'module cell_MCS6502 (','module cell_MCS6502_snapshot (')
 text=cb.replace_once(text,'    input  wire sys_clk,','''    input wire state_host_clk,state_advance,state_freeze,state_guest_held,state_request,state_write,
    input wire [6:0] state_index,input wire [17:0] state_data,
    output wire state_frozen,state_done,state_error,output wire [17:0] state_result,
    input  wire sys_clk,''')
 return cb.replace_once(text,'  chip_6502 u_cpu (','''  chip_6502_snapshot u_cpu (
      .state_host_clk(state_host_clk),.state_advance(state_advance),
      .state_freeze(state_freeze),.state_guest_held(state_guest_held),
      .state_request(state_request),.state_write(state_write),.state_index(state_index),.state_data(state_data),
      .state_frozen(state_frozen),.state_done(state_done),.state_error(state_error),.state_result(state_result),''')


def generate(root):
 original=(root/SOURCE).read_text()
 if digest(root/SOURCE)!=BOARD_SHA:raise ValueError('unreviewed actual BaseBoard')
 backplane=(root/'build/rtl-executive/native16-tx-cable/dorado_native_backplane.v').read_text()
 if backplane.count(ORIGINAL_MODULE+' #(.SYSPER(SYSPER)) b_BaseBd (')!=1:raise ValueError('actual BaseBoard binding differs')
 if re.search(r'\b(always\w*|initial|reg|logic)\b',newff.ff.masked_comments(original)):raise ValueError('extra board-level state')
 grouping=ram.audit(root);ff=misc.schema()['packages'];clones={};rows=[]
 for k,(s,_)in newff.generate().items():clones[k+'_snapshot.sv']=s
 clones['cell_MC10176_snapshot.v']=cb.ff_sources()['cell_MC10176_snapshot.v']
 for k in ('cell_MC10136','cell_SN74LS174'):clones[k+'_snapshot.sv']=oldff.generate_one(k)[0]
 clones['f10415_word_bank.sv']=(ROOT/'fpga/mister/snapshot/f10415_word_bank.sv').read_text()
 clones['i2125_byte_bank.sv']=(ROOT/'fpga/mister/snapshot/i2125_byte_bank.sv').read_text()
 clones['cell_i2716_snapshot.sv'],eprom_schema=eprom.generate()
 clones['cell_MCS6532_snapshot.sv'],riot_schema,children=riot.generate();clones.update(children)
 cpu_bytes,cpu_schema=cpu.generate();clones['chip_6502_snapshot.sv']=cpu_bytes
 clones['cell_MCS6502_snapshot.sv']=cpu_wrapper((root/'verilog/cells/cell_MCS6502.v').read_bytes())
 for r in ff:rows.append(dict(**r,type='FF',word=r['index']<<11,words_count=len(r['words'])))
 for g in grouping['groups']:
  i=len(rows);rows.append(dict(index=i,type='SRAM',kind='cell_i2125',package='snapshot_sram_'+str(g['index']),word=i<<11,words_count=1025,logical_state_bits=8200,width=8,group=g))
 for p in eprom.SOCKETS:
  i=len(rows);rows.append(dict(index=i,type='EPROM',kind='cell_i2716',package='u_'+p,word=i<<11,words_count=1,logical_state_bits=9,width=9,
   init_file='verilog/proms/eprom/BaseBd-'+p+'.mem'if p in eprom.IMAGES else None,sha256=eprom.IMAGES.get(p),schema=eprom_schema))
 for p in ('c62','f62','f63','i62','l62'):
  i=len(rows);rows.append(dict(index=i,type='RIOT',kind='cell_MCS6532',package='u_'+p,word=i<<11,words_count=134,logical_state_bits=1098,schema=riot_schema))
 i=len(rows);rows.append(dict(index=i,type='CPU',kind='cell_MCS6502',package='u_f61',word=i<<11,words_count=96,logical_state_bits=1725,schema=cpu_schema))
 assert len(rows)==48 and sum(r['logical_state_bits']for r in rows)==40366
 by_name={r['package']:r for r in rows if r['type']!='SRAM'};ram_names={m['instance']for g in grouping['groups']for m in g['members']};replaced=set()
 text=cb.replace_once(original,'module '+ORIGINAL_MODULE+' ','module '+MODULE+' ')
 text=cb.replace_once(text,'    input  wire sys_clk,','''    input wire state_host_clk,state_guest_advance,state_freeze,state_guest_held,state_request,state_write,
    input wire [16:0] state_index,input wire [17:0] state_data,
    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,output wire [17:0] state_result,
    input  wire sys_clk,''')
 def replace(m):
  kind,gap,name,body=m.groups()
  if name in ram_names:replaced.add(name);return ''
  if name not in by_name:return m[0]
  r=by_name[name];i=r['index'];replaced.add(name)
  if kind!=r['kind']:raise ValueError('state package kind differs:'+name)
  common=f'''    .state_host_clk(state_host_clk),.state_advance(state_guest_advance),'''
  if r['type']=='FF':
   common+=f''' .state_hold(state_hold),.state_write(state_accept && state_write && state_valid && state_package==6'd{i}),
    .state_data(state_data),.state_result(state_child_result[{i}]),'''
   if kind in newff.FIELDS:common+='\n    .state_index(state_row[1:0]),'
  else:
   width={'EPROM':2,'RIOT':8,'CPU':7}[r['type']]
   common+=f'''
    .state_freeze(state_freeze),.state_guest_held(state_guest_held),
    .state_request(state_accept && state_valid && state_package==6'd{i}),.state_write(state_write),
    .state_index(state_row[{width-1}:0]),.state_data(state_data),
    .state_frozen(state_child_frozen[{i}]),.state_done(),.state_error(),.state_result(state_child_result[{i}]),'''
  return kind+'_snapshot'+gap+name+' (\n'+common+body+');'
 text=INSTANCES.sub(replace,text)
 if replaced!=set(by_name)|ram_names:raise ValueError('state socket replaced missing/duplicate')
 fabric='''
 generate if(SYSPER!=4)begin:unqualified_baseboard initial $fatal(1,"BaseBoard snapshot requires qualified SYSPER4");end endgenerate
 wire state_hold=state_freeze && state_guest_held;
 wire state_accept=state_hold && state_frozen && state_request;
 wire [5:0] state_package=state_index[16:11];wire [10:0] state_row=state_index[10:0];
 wire [17:0] state_child_result[0:47];wire [47:0] state_child_frozen;
 wire state_valid=
'''+ ' ||\n'.join(f" (state_package==6'd{r['index']} && state_row<11'd{r['words_count']})"for r in rows)+''';
 reg [5:0] state_response_package;reg state_response_child;reg [17:0] state_aux_result;
 assign state_result=state_response_child?state_child_result[state_response_package]:state_aux_result;
 always @(posedge state_host_clk)begin
  state_frozen<=state_hold;state_done<=state_accept;state_error<=state_accept && !state_valid;
  if(state_accept)begin
   state_response_child<=state_valid && !state_write && state_package>=6'd30;
   state_response_package<=state_package<6'd48 ? state_package :6'd0;
   state_aux_result<=(state_valid && !state_write && state_package<6'd30)?state_child_result[state_package]:18'b0;
  end
 end
'''
 for r in rows:
  i=r['index']
  if r['type']=='FF':fabric+=f" assign state_child_frozen[{i}]=state_hold;\n"
  if r['type']!='SRAM':continue
  g=r['group'];data='{'+','.join(m['data']for m in reversed(g['members']))+'}';address='{'+','.join(g['address'])+'}'
  fabric+=f''' wire [7:0] state_sram_q_{i};
 dorado_snapshot_i2125_byte_bank {r['package']}(
 .host_clk(state_host_clk),.guest_advance(state_guest_advance),.address({address}),.data({data}),
 .ce_n({g['ce']}),.we_n({g['we']}),.q(state_sram_q_{i}),.freeze(state_freeze),.guest_held(state_guest_held),
 .frozen(state_child_frozen[{i}]),.request(state_accept && state_valid && state_package==6'd{i}),
 .write(state_write),.word_index(state_row),.state_data(state_data),.done(),.error(),.result(state_child_result[{i}]));
'''
  for m in g['members']:fabric+=f" assign {m['output']}=state_sram_q_{i}[{m['bit']}];\n"
 text=cb.replace_once(text,'endmodule',fabric+'endmodule')
 kinds=sorted({m[1]for m in INSTANCES.finditer(original)})
 sources={f'verilog/cells/{k}.v':digest(root/'verilog/cells'/(k+'.v'))for k in kinds}
 sources.update({str(p.relative_to(root)):digest(p)for p in [root/'verilog/vendor/6502/chip_6502.v',root/'verilog/vendor/riot/M6532.sv']})
 compiled=json.loads((ROOT/'fpga/mister/reports/2026-10-19-native59-dsketh/compiled-inputs.json').read_text())
 for n,h in sources.items():
  if compiled['src/'+n]!=h:raise ValueError('actual compiled source differs:'+n)
 for p in eprom.images(root).values():
  if compiled['src/'+str(p.relative_to(root))]!=digest(p):raise ValueError('actual compiled firmware differs')
 # Preserve CRLF in the vendor clone; every shared helper is byte-identical.
 manifest=dict(module=MODULE,original_module=ORIGINAL_MODULE,original_sha256=BOARD_SHA,generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
  parameters=dict(SYSPER=4),index_bits=17,row_bits=11,logical_state_bits=40366,packages=rows,
  original_sources=sources,cloned_sources={n:hashlib.sha256(s if isinstance(s,bytes)else s.encode()).hexdigest()for n,s in clones.items()},
  grouping=grouping,scope=__doc__)
 return text,manifest,clones


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--frozen-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False)
 board,m,clones=generate(a.frozen_root.resolve());(a.output/'BaseBd_snapshot.sv').write_text(board)
 for n,s in clones.items():(a.output/n).write_bytes(s if isinstance(s,bytes)else s.encode())
 (a.output/'manifest.json').write_text(json.dumps(m,indent=2)+'\n');print('BaseBoard complete candidate generated:40366bits/48selectors/4905words;not qualified')
