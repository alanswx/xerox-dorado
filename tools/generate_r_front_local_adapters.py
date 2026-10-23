#!/usr/bin/env python3
"""Thread a separate StopTasks capture-data view through the unchanged hierarchy.

Only task_transfer's data pins and the RAM preview's SwitchUp data use the
pre-event view. Clock levels keep the resolved producer. No state or RAM copy
is added. Every transformation is inversely checked against its parent source.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

ROOT=Path(__file__).resolve().parents[1]
NAMES=['task_capture','t_file','t_parity','r_operand','r_flags','r_address',
       'r_port_preview','r_memory','r_front','r_front_ready']
OLD={n:('dorado_'+n+('_step'if n!='r_port_preview'else''))for n in NAMES}
NEW={n:OLD[n].replace('dorado_','dorado_local_',1)for n in NAMES}


def generate(root,out):
 out.mkdir(parents=True,exist_ok=False);records=[]
 for n in NAMES:
  path=root/'verilog/rtl'/(OLD[n]+'.sv');original=path.read_text();raw=original
  if n=='task_capture':
   marker='    reg [87:0] q,n;';assert raw.count(marker)==1
   raw=raw.replace(marker,'    wire [75:0] task_data_pins={pins_after[75:66],stop_tasks_capture,pins_after[64:0]};\n'+marker)
   a='task_transfer(q,captures,pins_after,state_q[79:64])';b='task_transfer(q,captures,task_data_pins,state_q[79:64])'
   assert raw.count(a)==1;raw=raw.replace(a,b)
  if n=='r_port_preview':
   assert raw.count('pins_after[65]')==1;raw=raw.replace('pins_after[65]','stop_tasks_capture')
  for key in NAMES:raw=re.sub(r'\b'+OLD[key]+r'\b',NEW[key],raw)
  start=raw.index('\n input wire',raw.index('module '+NEW[n]));raw=raw[:start]+'\n input wire stop_tasks_capture,'+raw[start:]
  calls=[]
  for key in NAMES:
   pattern=r'(?m)^(\s*'+NEW[key]+r'\s+(\w+)\()'
   raw,count=re.subn(pattern,lambda m:m[1]+'stop_tasks_capture,',raw)
   if count:calls.append(dict(module=NEW[key],count=count))
  inverse=raw.replace('\n input wire stop_tasks_capture,','',1)
  for key in NAMES:
   inverse=re.sub(r'(?m)^(\s*'+NEW[key]+r'\s+\w+\()stop_tasks_capture,',r'\1',inverse)
   inverse=re.sub(r'\b'+NEW[key]+r'\b',OLD[key],inverse)
  if n=='task_capture':
   inverse=inverse.replace('    wire [75:0] task_data_pins={pins_after[75:66],stop_tasks_capture,pins_after[64:0]};\n','',1)
   inverse=inverse.replace('task_transfer(q,captures,task_data_pins,state_q[79:64])','task_transfer(q,captures,pins_after,state_q[79:64])',1)
  if n=='r_port_preview':inverse=inverse.replace('stop_tasks_capture','pins_after[65]')
  assert inverse==original,('unaccounted source change',n)
  p=out/(NEW[n]+'.sv');p.write_text(raw)
  records.append(dict(parent=str(path.relative_to(root)),parent_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),
   output=p.name,output_sha256=hashlib.sha256(p.read_bytes()).hexdigest(),calls=calls,inverse_byte_identical=True))
 result=dict(scope=__doc__,files=records,extra_architectural_state=False,native_integrated=False)
 (out/'manifest.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--root',type=Path,default=ROOT)
 a=p.parse_args();r=generate(a.root.resolve(),a.output.resolve());print('R_FRONT_LOCAL_ADAPTERS',len(r['files']))
