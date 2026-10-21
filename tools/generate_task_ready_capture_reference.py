#!/usr/bin/env python3
"""Extract actual Ready/IsNext in the task-selection/processor reference graph.

All three SetReady drivers and original clock fanout are included in one ContA
circuit. No fixture supplies Ready or IsNext to arbitration. Notify/preemption,
CP/MIR control and memory Hold producer boundaries remain explicit.
"""
import argparse
import hashlib
import json
from pathlib import Path

from generate_processor_capture_reference import graph
from generate_task_capture_reference import specs as task_specs,generate as task_generate
from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]
READY_COMB=['a08','b08','c08','d08','b06','b07','c06','c07','f08','f09','d07']


def specs():
 result=task_specs();c=result[0];c['module']='task_ready_conta'
 c['comb']=sorted(set(c['comb']+READY_COMB))
 c['seq']={**c['seq'],**{p:(48+4*i,4)for i,p in enumerate(['a09','b09','c09','d09'])}}
 for i in range(1,16):
  del c['bindings'][f'Ready.{i:02}'];del c['bindings'][f"IsNext.{i:02}'"]
 c['bindings'].update({**{f'bFF.{i}':f'pins[{72+i}]'for i in range(4,8)},
                       "bFF.4'":'~pins[76]',"PreEmpting'":'pins[80]',"FF=Notify'":'pins[81]',
                       'ClrReady':'pins[82]','ContA18.sil+3':'pins[83]','ContA18.sil+4':'pins[84]'})
 c['clock_taps'] += ["h*fclk0'Ab","h*fclk0'Ac"]
 c['flag_taps'] += [f'Ready.{i:02}'for i in range(1,16)]+[f"IsNext.{i:02}'"for i in range(1,16)]
 result[1]['module']='task_ready_proch';result[2]['module']='task_ready_procl'
 return result


def generate(root,out):
 out.mkdir(parents=True,exist_ok=False);plans=specs();records=[]
 for s in plans:
  records.append(graph(root,out=out,**s));p=out/(s['module']+'.sv');raw=p.read_text()
  assert raw.count('input wire [27:0] pins')==1
  raw=raw.replace('input wire [27:0] pins','input wire [85:0] pins,input wire [3:0] next_raw,input wire priority_n')
  p.write_text(raw)
 b=load_board(str(root/'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl'));chosen=set(plans[0]['comb'])|set(plans[0]['seq'])
 for i in range(1,16):
  drivers=[p for p in b.nets[f'SetReady.{i:02}']['pins']if p['dir']=='out']
  assert len(drivers)==3 and all(p['pkg']in chosen for p in drivers)
 task_generate(root,out/'task-source-audit')
 raw=(out/'task-source-audit/task_capture_reference.sv').read_text()
 edits=[('module task_capture_reference','module task_ready_capture_reference',1),('[87:0]','[103:0]',2),
        ('[75:0] pins','[85:0] pins',1),('[24:0] clocks','[26:0] clocks',1),
        ('wire [12:0] cc','wire [14:0] cc',1),('wire [12:0] fc','wire [42:0] fc',1),
        ('task_conta c(','task_ready_conta c(',1),('task_proch h(','task_ready_proch h(',1),('task_procl l(','task_ready_procl l(',1),
        ('{fixture_state[79:64],fixture_state[31:0]}','{fixture_state[103:88],fixture_state[79:64],fixture_state[31:0]}',1),
        ('{state_q[79:64],state_q[31:0]}','{state_q[103:88],state_q[79:64],state_q[31:0]}',1),
        ('assign clocks={cl[5]','assign clocks={cc[14:13],cl[5]',1),
        ('output wire [3:0] next_raw);','output wire [3:0] next_raw,output wire [29:0] ready_flags);',1),
        ('assign flags={fl,fh,fc[2:0]};','assign flags={fl,fh,fc[2:0]};\n assign ready_flags=fc[42:13];',1)]
 for a,b,count in edits:
  assert raw.count(a)==count,(a,raw.count(a));raw=raw.replace(a,b)
 (out/'task_ready_capture_reference.sv').write_text(raw)
 record=dict(boards=records,physical_bits=104,packages=sum(len(r['packages'])for r in records),
             original_pins=sum(r['original_pins']for r in records),clock_channels=27,
             Ready_and_IsNext_internal=True,SetReady_driver_count=3,
             generator_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest())
 (out/'manifest.json').write_text(json.dumps(record,indent=2)+'\n');print('TASK_READY_CAPTURE_REFERENCE',record['physical_bits'],record['packages'],record['original_pins'])
 return record


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,default=ROOT);p.add_argument('--output',type=Path,required=True)
 a=p.parse_args();generate(a.root.resolve(),a.output.resolve())
