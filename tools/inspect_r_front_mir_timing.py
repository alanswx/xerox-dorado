#!/usr/bin/env python3
"""Attribute the rejected combined fit to actual fixture fields and board pins.

The reports measure an isolated evaluator. Producer dependencies listed here
are source evidence and next work, not completed arrival/timing qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

from audit_control_clock_fit import audit
from audit_task_control_stubs import pin_dependencies
from fit_r_front_mir import FIELDS,top_source
from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]


def field_at(fields,index):
 offset=0
 for name,width in reversed(fields):
  if offset<=index<offset+width:return dict(field=name,bit=index-offset)
  offset+=width
 raise AssertionError(('outside fixture',index,offset))


def inspect(directory,qualified,out):
 assert not out.exists()
 reviewed=audit(directory,qualified,False)
 assert reviewed['component']=='r_front_mir'and reviewed['status']=='REJECTED_COMPONENT_TIMING'
 assert (directory/'top.sv').read_text()==top_source()
 corner=min(reviewed['conditions'],key=lambda c:c['evaluator_setup'])
 report=directory/('clock-evaluator-'+corner['corner']+'.rpt')
 raw=report.read_text();first=raw.split('Path #1:',1)[1].split('Path #2:',1)[0]
 launch=int(re.search(r'; From Node\s*; launch\[(\d+)\]',first)[1])
 result=int(re.search(r'; To Node\s*; result\[(\d+)\]',first)[1])
 inputs=[('q',786),('before_clock',1),('after_clock',1),('before_controls',310),('after_controls',310),('advance',1)]
 outputs=[(n,w)for n,w in FIELDS if n not in ['source_pins','fa_n']]
 source=field_at(inputs,launch);sink=field_at(outputs,result)
 assert source==dict(field='q',bit=70)and sink==dict(field='parity_ok',bit=1),(source,sink)
 assert 'evaluator|conta|task_no'in first and 'evaluator|after_child[14]'in first
 assert 'files|stk_h_rtl_0'in first and 'parity_ok[1]'in first
 levels=int(re.search(r'Number of Logic Levels\s*;\s*;\s*(\d+)',first)[1])
 delay=float(re.search(r'; Data Delay\s*;\s*([\d.]+)',first)[1])
 board_path=ROOT/'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl'
 board=load_board(str(board_path))
 pins={(p['pkg'],p['pin']):n for n,r in board.nets.items()for p in r['pins']}
 assert pins[('h09',2)]=='BNT.2'
 producers=[]
 for net,external_pin in [('StopTasks',65),('RWTPCorRWIM',1)]:
  drivers=[p for p in board.nets[net]['pins']if p['dir']=='out'];assert len(drivers)==1
  d=drivers[0];kind=board.packages[d['pkg']]['type']
  deps=pin_dependencies(ROOT,kind)[d['pin']]
  producers.append(dict(net=net,external_fixture_pin=external_pin,board='ContA-Rev-Cd',
   driver=dict(package=d['pkg'],pin=d['pin'],kind=kind),
   direct_inputs=[dict(pin=p,net=pins[(d['pkg'],p)])for p in sorted(deps)],
   arrival_qualified=False))
 record=dict(status='REJECTED_15NS_PATH_ATTRIBUTED',scope=__doc__,
  timing_status=reviewed['status'],corner=corner['corner'],slack_ns=corner['evaluator_setup'],
  data_delay_ns=delay,logic_levels=levels,launch=dict(index=launch,**source,
   original='ContA h09 physical Q2 / BNT.2'),result=dict(index=result,**sink),
  path=['old BNT/task priority','Control-A clock/capture resolution','MIR FF control mux',
        'stack-pointer/address cone','functional stk_h MLAB asynchronous read','visible R/parity'],
  reports_sha256={report.name:hashlib.sha256(report.read_bytes()).hexdigest()},
  original_sources_sha256={str(board_path.relative_to(ROOT)):hashlib.sha256(board_path.read_bytes()).hexdigest()},
  remaining_local_producers=producers,
  next_action='Close actual MIR/IM/hold and local tasking/branch producers; derive the original capture-stage boundary before shortening this functional path. Keep parity and all functional ports in the budget.',
  timing_exceptions_added=False,full_producer_arrival_contract_qualified=False,native_speedup_qualified=False)
 out.write_text(json.dumps(record,indent=2)+'\n')
 print(record['status'],source,sink,levels,'levels',delay,'ns',flush=True)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path)
 p.add_argument('--qualified',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
 a=p.parse_args();inspect(a.directory.resolve(),a.qualified.resolve(),a.output.resolve())
