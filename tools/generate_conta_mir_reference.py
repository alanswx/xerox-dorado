#!/usr/bin/env python3
"""Extract actual Control-A clocks/tasks plus MIR, CP and Ready in one graph.

Original packages/pins and independent register references; no candidate
next-state functions are consulted. Memory Hold, IFU/RWTPC, StopTasks and
remaining original producers stay explicit; full native clock proof is open.
"""
import argparse,json,re
from pathlib import Path
from generate_processor_capture_reference import graph
from generate_task_ready_capture_reference import specs
from generate_mir_ready_reference import COMB as MIR_COMB
from inventory_mir_clock_boundaries import ROOT,PACKAGES
from test_mir_capture import BINDINGS as MIR_BINDINGS

CLOCKS=["prepreclk'd",'StopRunClock',"preRunClk'Bc","RunClk'a","preclk2'Bb","clk2'Bc",'@l06.ca',"@k04.ca",'preCRamClock',
 "clk0'Ba","h*fclk0'Aa","@f14.ca",*[f"h*clk0'C{x}"for x in 'abcdef'],"h*fclk0'Ab","h*fclk0'Ac","CPStrb0'","CPStrb1'"]
FLAGS=[*[f"IsNext.{i:02}'"for i in range(1,16)],*[f'Ready.{i:02}'for i in range(1,16)],
 "FF=Notify'","PreEmpting'","FFok'c",*[f'bFF.{i}'for i in range(4,8)],"Block'","PrBlock'",*[f'Next.{i}'for i in range(4)]]


def plan(root):
 spec=specs()[0];spec['module']='conta_mir_reference';spec['comb']=sorted(set(spec['comb']+MIR_COMB))
 spec['seq'].update({'j02':(64,6),'i02':(70,6),**{p:(76+2*i,2)for i,p in enumerate(PACKAGES)}})
 bindings={n:v for n,v in spec['bindings'].items()if v not in ("1'b0","1'b1")}
 bindings.update({n:re.sub(r'pins\[(\d+)\]',lambda m:f'pins[{76+int(m[1])}]',v)for n,v in MIR_BINDINGS.items()})
 # MIR clock/brMIR roots are now actual internal fanout, not those old slots.
 for n in [*['brMIR'+x for x in 'abc'],*[f"h*clk0'C{x}"for x in 'abcdef']]:bindings.pop(n)
 bindings.update({**{f'CPOut.{i}':f'pins[{111+i}]'for i in range(1,8)},
  **{f"CPAddr.{i}'":f'pins[{121-i}]'for i in range(3)},"CPStrb'":'pins[122]',
  "ASEL.0'a":'pins[123]',"BSEL.0'":'pins[124]','StopMIRClk':'pins[125]',
  'ContA18.sil+3':'pins[126]','ContA18.sil+4':'pins[127]',"dBlock'":'pins[93]'})
 spec['clock_taps']=CLOCKS;spec['flag_taps']=FLAGS;spec['bindings']={}
 external=graph(root,out=None,inspect=True,**spec)
 extras={n:128+i for i,n in enumerate(sorted(set(external)-set(bindings)))}
 spec['bindings']={n:bindings[n]if n in bindings else f'pins[{extras[n]}]'for n in external}
 return spec,extras


def generate(root,out):
 out.mkdir(parents=True,exist_ok=False);spec,extras=plan(root);record=graph(root,out=out,**spec)
 p=out/'conta_mir_reference.sv';s=p.read_text();assert s.count('input wire [27:0] pins')==1
 width=128+len(extras);p.write_text(s.replace('input wire [27:0] pins',f'input wire [{width-1}:0] pins'))
 record.update(pin_count=width,extra_boundaries=extras,native_or_timing_qualified=False)
 (out/'manifest.json').write_text(json.dumps(record,indent=2)+'\n');return record


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,default=ROOT);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 r=generate(a.root.resolve(),a.output.resolve());print('CONTA_MIR_REFERENCE',r['physical_bits'],r['pin_count'],r['extra_boundaries'])
