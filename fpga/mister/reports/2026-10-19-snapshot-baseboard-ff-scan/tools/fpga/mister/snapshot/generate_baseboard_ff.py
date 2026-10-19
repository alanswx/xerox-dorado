#!/usr/bin/env python3
"""Pinned BaseBoard clock/reset/TTL local state adapters, preserving source processes.

These nine families cover70representative bits and87actual board bits. The
other192remaining board FF bits reuse qualified adapters unchanged. Local
words are18bits, valid only while the caller holds every consumer. No board
handshake/global barrier/timer calibration or physical snapshot is provided.
"""
import gzip,hashlib,json,re
from pathlib import Path
import generate_snapshot_ff as ff
ROOT=Path(__file__).resolve().parents[3]
FIELDS={'cell_SN74LS259': [('q', 8)], 'cell_SN74LS163': [('q', 4), ('ck_d', 1)], 'cell_SN74LS175': [('q', 2), ('ck_d', 1)], 'cell_SN74LS74': [('qa', 1), ('qb', 1), ('cka_d', 1), ('ckb_d', 1)], 'cell_MC1690': [('q', 1), ('ck_d', 1)], 'cell_MC12061': [('osc_div', 3)], 'cell_MC14521B': [('cnt', 24), ('ck_d', 1)], 'cell_MPQ3303': [('vco', 2), ('por', 16)], 'cell_CLOCKGEN': [('phase', 2)]}
SHA={'cell_SN74LS259': 'e756e0efb317aff6c96a2ad2ae8e5ee381f447e08ee22b5ceeb0657a2eef843f', 'cell_SN74LS163': '6c6e4478fde05434115d552c17e40d3825ee8a34fc5b7801e9fc8b3e2c1ae3ad', 'cell_SN74LS175': '617c9c875496d26d48ca5f4555cb2951fc2c7c8f11dee3ea3a4bc8d91dfa7ff1', 'cell_SN74LS74': 'dc01bf5078d1eae612e82f55adebd7fa3c485cb08ee3d0f5f7eebe93f09a71b1', 'cell_MC1690': 'c46a086041ccff663cae5d3a71c91ac52d791760bcc0f26657cefd387172b753', 'cell_MC12061': '9480333422f9f6b846a7507bcd1d4676681b6840aafe55dc3825fad3ece744df', 'cell_MC14521B': '566a04d2ac2dff2b9cd695147d9c9a37d36fb4997dc81b32a57f7cf9d06ebf3d', 'cell_MPQ3303': '98ae705be80d123e4669c0c785d1e6fe601dada155302cc68301704816fdfba1', 'cell_CLOCKGEN': '6101cd5bf07c11fdb143ddeedb1532f2e8ebc19d5e8c45ee7f843b8b05b92c09'}


def original_source(kind):
 if kind=='cell_CLOCKGEN':
  return gzip.decompress((ROOT/'fpga/mister/reports/2026-10-19-native59-dsketh/src/verilog/cells/cell_CLOCKGEN.v.gz').read_bytes())
 return (ROOT/'verilog/cells'/(kind+'.v')).read_bytes()


def chunks(fields):
 rows=[];offset=0;positions=[]
 for name,width in fields:positions.append((name,offset,width));offset+=width
 for start in range(0,offset,18):
  width=min(18,offset-start);parts=[]
  for name,lsb,w in positions:
   lo=max(start,lsb);hi=min(start+width,lsb+w)
   if lo>=hi:continue
   a,b=lo-lsb,hi-lsb-1
   part=name if a==0 and b==w-1 else name+'['+str(a)+']'if a==b else name+'['+str(b)+':'+str(a)+']'
   parts.append(part)
  rows.append(dict(index=len(rows),width=width,lsb=start,expression='{'+','.join(reversed(parts))+'}'))
 return rows


def generate_one(kind,original=None):
 source=original_source(kind).decode()if original is None else original
 if hashlib.sha256(source.encode()).hexdigest()!=SHA[kind]:raise ValueError('unreviewed original '+kind)
 clean=ff.masked_comments(source)
 # These pinned files have a simple top-level declaration/process layout.
 # Slice whole original processes only; do not regenerate their equations.
 boundaries=list(re.finditer(r'(?m)^  (?:always|wire|reg|assign|localparam)\b|^endmodule\b',clean))
 spans=[]
 for i,b in enumerate(boundaries):
  if clean[b.start():b.end()].strip()!='always':continue
  end=boundaries[i+1].start();part=source[b.start():end].strip()
  if not re.match(r'always\s*@\(posedge sys_clk\)',part):raise ValueError('unreviewed clock process')
  spans.append((b.start(),end,part))
 if not spans:raise ValueError('missing sequential processes')
 text=source;normal=[]
 for start,end,part in reversed(spans):text=text[:start]+text[end:]
 for _,_,part in spans:normal.append(re.sub(r'^always\s*@\(posedge sys_clk\)\s*','',part,count=1))
 if re.search(r'\b(always\w*|initial|final)\b',ff.masked_comments(text)):raise ValueError('extra state process')
 assert text.count('module '+kind+' ')==1 and text.count('    input  wire sys_clk,')==1
 text=text.replace('module '+kind+' ','module '+kind+'_snapshot ',1).replace('    input  wire sys_clk,',"""    input wire state_host_clk,state_advance,state_hold,state_write,
    input wire [1:0] state_index,input wire [17:0] state_data,output reg [17:0] state_result,
    input  wire sys_clk,""",1)
 words=chunks(FIELDS[kind]);width=sum(w for _,w in FIELDS[kind])
 # N4 is the actual current nominal BaseBoard binding. Reject an unqualified
 # clock-divider parameter rather than silently serializing a wrong width.
 fabric="  generate if(N!=4)begin:unqualified_clockgen initial $fatal(1,\"Snapshot CLOCKGEN requires qualified N4\");end endgenerate\n"if kind=='cell_CLOCKGEN'else''
 fabric+=' always @* begin\n  state_result=18\'b0;\n  case(state_index)\n'
 for r in words:fabric+="   2'd%d:state_result={%d'b0,%s};\n"%(r['index'],18-r['width'],r['expression'])if r['width']<18 else"   2'd%d:state_result=%s;\n"%(r['index'],r['expression'])
 fabric+='   default:state_result=18\'b0;\n  endcase\n end\n always @(posedge state_host_clk)begin\n  if(state_hold)begin\n   if(state_write)case(state_index)\n'
 for r in words:fabric+="    2'd%d:%s<=state_data[%d:0];\n"%(r['index'],r['expression'],r['width']-1)
 fabric+='    default:;\n   endcase\n  end else if(state_advance)begin\n'+'\n'.join(normal)+'\n  end\n end\n'
 text=text.replace('endmodule',fabric+'endmodule',1)
 offset=0;fields=[]
 for name,w in FIELDS[kind]:fields.append(dict(name=name,width=w,lsb=offset));offset+=w
 return text,dict(module=kind+'_snapshot',logical_state_bits=width,fields=fields,words=words,original_sha256=SHA[kind],generated_sha256=hashlib.sha256(text.encode()).hexdigest(),parameters=dict(N=4)if kind=='cell_CLOCKGEN'else{},scope=__doc__)


def generate():return {kind:generate_one(kind)for kind in FIELDS}


if __name__=='__main__':
 import argparse
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False)
 records={}
 for k,(s,m)in generate().items():(a.output/(k+'_snapshot.sv')).write_text(s);records[k]=m
 (a.output/'manifest.json').write_text(json.dumps(records,indent=2)+'\n')
