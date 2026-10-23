#!/usr/bin/env python3
"""Extract original Control-A tasking state and branch stretch producers.

Add actual FF/tasking decode, d17 and e21/d21 to the qualified MIR/clock/task
graph. StopTasks and RWTPCorRWIM cease to be boundary inputs. Other unselected
producers and unused package-half inputs remain explicit; no native claim.
"""
import argparse
import json
from pathlib import Path

from generate_conta_mir_reference import plan as parent_plan
from generate_processor_capture_reference import graph
from inventory_mir_clock_boundaries import ROOT

COMB=['a13','a16','a17','a19','b18','b21','e21','d21']
LOCAL_FLAGS=['StopTasks','RWTPCorRWIM',"TaskingIsOff'",'FF=TaskingOff',
             'FF=TaskingOn','FF=MidasOn','FF=TOffIsOK',"StopTasks'"]


def plan(root):
 spec,_=parent_plan(root)
 spec['module']='conta_local_reference'
 spec['comb']=sorted(set(spec['comb']+COMB))
 spec['seq']['d17']=(98,6)
 spec['clock_taps']=spec['clock_taps']+["clk0'Cc"]
 spec['flag_taps']=spec['flag_taps']+LOCAL_FLAGS
 bindings={n:v for n,v in spec['bindings'].items()if n not in ['StopTasks','RWTPCorRWIM']
           and (not v.startswith('pins[')or int(v[5:-1])<128)}
 # These two d17 data pins are not in the tasking latch's feedback cone.
 # Retain them explicitly for the other physical package Qs.
 bindings.update({'ContA03.sil+2':'pins[128]','ContA28.sil+2':'pins[129]'})
 spec['bindings']={}
 external=graph(root,out=None,inspect=True,**spec)
 # Rebind all old unused package-half pins as well as newly exposed inputs.
 bound={n:v for n,v in bindings.items()if n in external}
 extras={n:130+i for i,n in enumerate(sorted(set(external)-set(bound)))}
 spec['bindings']={n:bound[n]if n in bound else f'pins[{extras[n]}]'for n in external}
 indexed=[v for v in spec['bindings'].values()if v.startswith('pins[')]
 assert len(indexed)==len(set(indexed)), 'Distinct external producer pins were aliased'
 return spec,extras


def generate(root,out):
 out.mkdir(parents=True,exist_ok=False)
 spec,extras=plan(root);record=graph(root,out=out,**spec)
 p=out/'conta_local_reference.sv';raw=p.read_text()
 assert raw.count('input wire [27:0] pins')==1
 width=130+len(extras)
 p.write_text(raw.replace('input wire [27:0] pins',f'input wire [{width-1}:0] pins'))
 record.update(pin_count=width,extra_boundaries=extras,local_flags=LOCAL_FLAGS,
  StopTasks_internal=True,RWTPCorRWIM_internal=True,native_or_timing_qualified=False)
 (out/'manifest.json').write_text(json.dumps(record,indent=2)+'\n')
 return record


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,default=ROOT);p.add_argument('--output',type=Path,required=True)
 a=p.parse_args();r=generate(a.root.resolve(),a.output.resolve())
 print('CONTA_LOCAL_REFERENCE',r['physical_bits'],r['pin_count'],r['extra_boundaries'])
