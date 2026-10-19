#!/usr/bin/env python3
"""Pinned local state words for DskEth comparator/counter/TTL FF families.

Caller freezes the whole board before accessing these local words. This does
not implement the board handshake, global barrier or calibrated timers.
"""
import hashlib,re
from pathlib import Path
import generate_snapshot_ff as ff
ROOT=Path(__file__).resolve().parents[3]
FIELDS={
 'cell_MC1650':[('qa',1),('qb',1)],
 'cell_MC10136':[('q',4),('d_pre',4),('su_pre',1),('sd_pre',1),('cin_pre',1),('ck_d',1)],
 'cell_SN74LS169':[('q',4),('p7_d',1),('p9_d',1),('p10_d',1),('ck_d',1)],
 'cell_SN74LS174':[('q',6),('d_pre',6),('ck_d',1)],
}
SHA={
 'cell_MC1650':'8650f11b09555a812e510469d01576cb8af8c23c31e412e64d7f2059f7cde799',
 'cell_MC10136':'b4314b6a838492c95fd72423e92a883e8ea92c660c55fdc1f266b034b68e8017',
 'cell_SN74LS169':'abc49703a29ef790c4bf503c329204e20868c000d88ae3d8ee3ae1d1b7cf801b',
 'cell_SN74LS174':'05a45e9f4788af98180a92f3ef5987f54a5e1c495fc9c006f5e61b5761d14b59',
}
PROCESSES={
 'cell_MC1650':['''always @(posedge sys_clk) begin
    if (p4) begin
      if (p6 & ~p5) qa <= 1'b1;
      else if (~p6 & p5) qa <= 1'b0;
    end
    if (p13) begin
      if (p12 & ~p11) qb <= 1'b1;
      else if (~p12 & p11) qb <= 1'b0;
    end
  end'''],
 'cell_MC10136':['''always @(posedge sys_clk) begin
    d_pre <= {p5,p6,p11,p12}; su_pre <= p9; sd_pre <= p7; cin_pre <= p10;
  end''','always @(posedge sys_clk) ck_d <= p13;',
 '''always @(posedge sys_clk) if (ck_en) begin
      if (!su_pre && !sd_pre) q <= d_pre;
      else if (!su_pre && sd_pre && !cin_pre) q <= q + 4'd1;
      else if (su_pre && !sd_pre && !cin_pre) q <= q - 4'd1;
    end'''],
 'cell_SN74LS169':['always @(posedge sys_clk) ck_d <= p2;',
 'always @(posedge sys_clk) begin p7_d <= p7; p9_d <= p9; p10_d <= p10; end',
 '''always @(posedge sys_clk) if (ck_en) begin
      if (!p9_d) q <= d;
      else if (!p7_d && !p10_d) q <= p1 ? q + 4'd1 : q - 4'd1;
    end'''],
 'cell_SN74LS174':['always @(posedge sys_clk) ck_d <= p9;',
 'always @(posedge sys_clk) d_pre <= {p14,p13,p11,p6,p4,p3};',
 '''always @(posedge sys_clk) begin
    if (!p1) q <= 6'd0;
    else if (ck_en) q <= PRECAP ? d_pre : {p14,p13,p11,p6,p4,p3};
  end'''],
}


def generate_one(kind,original=None):
 source=(ROOT/'verilog/cells'/(kind+'.v')).read_text()if original is None else original
 if hashlib.sha256(source.encode()).hexdigest()!=SHA[kind]:raise ValueError('unreviewed original '+kind)
 text=source;normal=[]
 for fragment in PROCESSES[kind]:
  text,process=ff.extract_once(text,fragment)
  normal.append(re.sub(r'^always\s*@\s*\(\s*posedge\s+sys_clk\s*\)\s*','',process,count=1))
 if re.search(r'\b(always\w*|initial|final)\b',ff.masked_comments(text)):raise ValueError('extra state process '+kind)
 assert text.count('module '+kind+' ')==1 and text.count('    input  wire sys_clk,')==1
 text=text.replace('module '+kind+' ','module '+kind+'_snapshot ',1)
 text=text.replace('    input  wire sys_clk,','''    input wire state_host_clk,state_advance,state_hold,state_write,
    input wire [17:0] state_data,output wire [17:0] state_result,
    input  wire sys_clk,''',1)
 width=sum(w for _,w in FIELDS[kind]);packed='{'+','.join(n for n,_ in reversed(FIELDS[kind]))+'}'
 process=f'''
 assign state_result={{{18-width}'b0,{packed}}};
 always @(posedge state_host_clk)begin
  if(state_hold)begin
   if(state_write){packed}<=state_data[{width-1}:0];
  end else if(state_advance)begin
'''+ '\n'.join(normal)+'''
  end
 end
'''
 text=text.replace('endmodule',process+'endmodule',1)
 fields=[];lsb=0
 for name,w in FIELDS[kind]:fields.append(dict(name=name,width=w,lsb=lsb));lsb+=w
 return text,dict(module=kind+'_snapshot',original_sha256=SHA[kind],generated_sha256=hashlib.sha256(text.encode()).hexdigest(),fields=fields,logical_state_bits=width,transport_bits=18,scope=__doc__)


def generate():return {kind:generate_one(kind)for kind in FIELDS}

if __name__=='__main__':
 import argparse,json
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False)
 manifest={}
 for k,(s,m)in generate().items():(a.output/(k+'_snapshot.sv')).write_text(s);manifest[k]=m
 (a.output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
