#!/usr/bin/env python3
"""Extract one original ContA graph for coupled MIR/CP/Notify/preemption/Ready.

No candidate equations. Keep independent BNT/BNT' boundary storage and every
wired-OR driver. Unused gates sharing physical packages retain explicit boundary
inputs; their original decoded producers are outside this component.
"""
import argparse,json
from pathlib import Path
from generate_processor_capture_reference import graph
from generate_task_ready_capture_reference import READY_COMB
from inventory_mir_clock_boundaries import PACKAGES,ROOT
from test_mir_capture import BINDINGS as MIR_BINDINGS

COMB=sorted(set(READY_COMB+['a01','a02','a18','a20','b16','b20','b22','c19','c22','f24']))
SEQ={**{p:(4*i,4)for i,p in enumerate(['a09','b09','c09','d09'])},'j02':(16,6),'i02':(22,6),**{p:(28+2*i,2)for i,p in enumerate(PACKAGES)}}
CLOCKS=["h*fclk0'Ab","h*fclk0'Ac","CPStrb0'","CPStrb1'",*[f"h*clk0'C{x}"for x in 'abcdef']]
FLAGS=[*[f"IsNext.{i:02}'"for i in range(1,16)],*[f'Ready.{i:02}'for i in range(1,16)],
 "FF=Notify'","PreEmpting'","FFok'c",*[f'bFF.{i}'for i in range(4,8)],"Block'","PrBlock'"]


def plan(root):
 bindings={**{f'BNT.{i}':f'pins[{i}]'for i in range(4)},**{f"BNT.{i}'":f'pins[{87+i}]'for i in range(4)},
  **{f'CTask.{i}':f'pins[{4+i}]'for i in range(4)},"CTask.0'":'~pins[4]',
  "Switch'b":'pins[12]','CAHold':'pins[16]','FreezeAC':'pins[17]',"preclk0'Aa":'pins[18]',
  "preclk2'Aa":'pins[19]','RepeatCur':'pins[20]','SCorFreezeb':'pins[21]',
  'ContA18.sil+3':'pins[22]','ContA18.sil+4':'pins[23]',
  **{n:__import__('re').sub(r'pins\[(\d+)\]',lambda m:f'pins[{24+int(m[1])}]',v)for n,v in MIR_BINDINGS.items()},
  **{f'CPOut.{i}':f'pins[{68+i}]'for i in range(1,8)},
  **{f"CPAddr.{i}'":f'pins[{78-i}]'for i in range(3)},"CPStrb'":'pins[79]',
  "ASEL.0'a":'pins[80]',"BSEL.0'":'pins[81]','Hold':'pins[82]','SwitchUp':'pins[83]',
  'RepeatCurC':'pins[84]','StopMIRClk':'pins[85]',"preclk0'Cb":'pins[86]'}
 args=dict(bn='ContA-Rev-Cd',comb=COMB,seq=SEQ,module='mir_ready_reference',bindings={},clock_taps=CLOCKS,flag_taps=FLAGS)
 external=graph(root,out=None,inspect=True,**args)
 extras={n:91+i for i,n in enumerate(sorted(set(external)-set(bindings)))}
 args['bindings']={n:bindings[n]if n in bindings else f'pins[{extras[n]}]'for n in external}
 return args,extras


def generate(root,out):
 out.mkdir(parents=True,exist_ok=False);args,extras=plan(root);record=graph(root,out=out,**args)
 p=out/'mir_ready_reference.sv';s=p.read_text();assert s.count('input wire [27:0] pins')==1
 width=91+len(extras);p.write_text(s.replace('input wire [27:0] pins',f'input wire [{width-1}:0] pins'))
 record.update(pin_count=width,unused_package_gate_boundaries=extras,native_or_timing_qualified=False)
 (out/'manifest.json').write_text(json.dumps(record,indent=2)+'\n');return record


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,default=ROOT);p.add_argument('--output',type=Path,required=True)
 a=p.parse_args();r=generate(a.root.resolve(),a.output.resolve());print('MIR_READY_REFERENCE',r['physical_bits'],r['pin_count'],r['unused_package_gate_boundaries'])
