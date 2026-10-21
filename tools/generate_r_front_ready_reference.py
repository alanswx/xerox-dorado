#!/usr/bin/env python3
"""Extract Ready, task/processor, RBase/stack and full R memory control together.

Ready/IsNext and all three SetReady drivers are internal to the same original
ContA circuit. The original processor wiring and independent storage models
are retained. Live MIR/notify/preemption/Hold/IFU/ALU producers remain external.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

from generate_r_front_reference import specs as front_specs,generate as front_generate
from generate_processor_capture_reference import graph
from generate_task_ready_capture_reference import READY_COMB
from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]


def generate(root,out):
 out.mkdir(parents=True,exist_ok=False);base=out/'front-source-audit';front_generate(root,base)
 plans,signals,count=front_specs(root);assert count==236
 c=plans[0];c['module']='r_front_ready_conta';c['comb']=sorted(set(c['comb']+READY_COMB))
 c['seq']={**c['seq'],**{p:(48+4*i,4)for i,p in enumerate(['a09','b09','c09','d09'])}}
 for i in range(1,16):
  del c['bindings'][f'Ready.{i:02}'];del c['bindings'][f"IsNext.{i:02}'"]
 c['bindings'].update({**{f'bFF.{i}':f'pins[{232+i}]'for i in range(4,8)},
                       "bFF.4'":'~pins[236]',"PreEmpting'":'pins[240]',"FF=Notify'":'pins[241]',
                       'ClrReady':'pins[242]','ContA18.sil+3':'pins[243]','ContA18.sil+4':'pins[244]'})
 assert len(c['clock_taps'])==13 and len(c['flag_taps'])==14
 c['clock_taps'] += ["h*fclk0'Ab","h*fclk0'Ac"]
 c['flag_taps'] += [f'Ready.{i:02}'for i in range(1,16)]+[f"IsNext.{i:02}'"for i in range(1,16)]
 record=graph(root,out=out,**{k:v for k,v in c.items()if not k.startswith('_')})
 p=out/'r_front_ready_conta.sv';raw=p.read_text().replace('input wire [27:0] pins',
   'input wire [245:0] pins,input wire [3:0] next_raw,input wire priority_n,freeze,rb_forward,rb_forward_n')
 for name in ['a09','b09','c09','d09']:
  a='original_F10000 '+name+'(';assert raw.count(a)==1;raw=raw.replace(a,'original_F10000_ready '+name+'(')
 p.write_text(raw)
 for name in ['proch','procl']:
  raw=(base/f'r_front_{name}.sv').read_text()
  raw=raw.replace(f'r_front_{name}',f'r_front_ready_{name}').replace('input wire [235:0] pins','input wire [245:0] pins')
  (out/f'r_front_ready_{name}.sv').write_text(raw)
 raw=(base/'r_front_reference.sv').read_text()
 edits=[('module r_front_reference','module r_front_ready_reference',1),('[735:0]','[751:0]',2),
        ('[235:0] pins','[245:0] pins',1),('[56:0] clocks','[58:0] clocks',1),
        ('wire [12:0] cc','wire [14:0] cc',1),('wire [13:0] fc','wire [43:0] fc',1),
        ('r_front_conta c(','r_front_ready_conta c(',1),('r_front_proch h(','r_front_ready_proch h(',1),('r_front_procl l(','r_front_ready_procl l(',1),
        ('{fixture_state[79:64],fixture_state[31:0]}','{fixture_state[751:736],fixture_state[79:64],fixture_state[31:0]}',1),
        ('{state_q[79:64],state_q[31:0]}','{state_q[751:736],state_q[79:64],state_q[31:0]}',1),
        ('assign clocks={cl[36:25]','assign clocks={cc[14:13],cl[36:25]',1),
        ('output wire [43:0] front_data);','output wire [43:0] front_data,output wire [29:0] ready_flags);',1)]
 for a,b,n in edits:
  assert raw.count(a)==n,(a,raw.count(a));raw=raw.replace(a,b)
 for i in range(30):
  a=f'assign source_pins[{35+i}]=pins[{35+i}];';assert raw.count(a)==1
  raw=raw.replace(a,f'assign source_pins[{35+i}]=fc[{14+i}];')
 raw=raw.replace('endmodule\n',' assign ready_flags=fc[43:14];\nendmodule\n')
 (out/'r_front_ready_reference.sv').write_text(raw)
 manifest=json.loads((base/'manifest.json').read_text());manifest['boards'][0]=record
 manifest.update(physical_bits=752,packages=274,pin_count=246,
                 Ready_and_IsNext_internal=True,clock_channels=59,
                 original_pins=sum(r['original_pins']for r in manifest['boards']),
                 generator_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest())
 assert sum(r['physical_bits']for r in manifest['boards'])==752
 b=load_board(str(root/'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl'));selected=set(c['comb'])|set(c['seq'])
 for i in range(1,16):assert len([p for p in b.nets[f'SetReady.{i:02}']['pins']if p['dir']=='out'and p['pkg']in selected])==3
 (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
 print('R_FRONT_READY_REFERENCE752Qs/59clocks',manifest['original_pins'],'pins')
 return manifest


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,default=ROOT);p.add_argument('--output',type=Path,required=True)
 a=p.parse_args();generate(a.root.resolve(),a.output.resolve())
