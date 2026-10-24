#!/usr/bin/env python3
"""Test a shared pre-control data view for all edge-triggered R address FFs.

Use the already existing old-control/current-external front capture cone.
Route its decoded data to the address packages independently of their current
resolved clocks. Preserve transparent h15 data, RAM ports and every original
state/clock/functional assertion. No extra storage, reset or guest cycle.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import time

from test_r_front_forward_view import sha
from test_r_front_task_event import replace


def adapter(raw,name,mutation=None):
    if name=='dorado_local_r_front_step.sv':
        raw=replace(raw,' wire [43:0] captured_front_data;',
                    ' wire [43:0] captured_front_data;wire [201:0] captured_source;wire [7:0] captured_fa;')
        raw=replace(raw,'.front_data(captured_front_data));',
                    '.front_data(captured_front_data),.source_pins(captured_source),.fa_n(captured_fa));')
        return replace(raw,'port_select_n,file_read_data,file_read_parity,r_flag_clocks_after,r_flag_sets_after);',
                       'port_select_n,file_read_data,file_read_parity,r_flag_clocks_after,r_flag_sets_after,effective_pins(captured_source,captured_fa));')
    end=raw.index(');',raw.index('module '))
    raw=raw[:end]+',input wire [201:0] capture_pins'+raw[end:]
    if name=='dorado_local_r_memory_step.sv':
        return replace(raw,'rm_flags_n,address,select_n,forwarded_bypass,r_flag_clocks_after,r_flag_sets_after);',
                       'rm_flags_n,address,select_n,forwarded_bypass,r_flag_clocks_after,r_flag_sets_after,capture_pins);')
    assert name=='dorado_local_r_address_step.sv'
    raw=replace(raw,'captured_low=low_address_data(state_q,{pins_after[201:160],pins_before[159:156],pins_after[155:0]});',
                'captured_low=low_address_data(state_q,capture_pins);')
    # Restrict the edit to edge D assignments; clocks, h15's LOW-transparent
    # mux, child processor/operand inputs and settled observations stay current.
    a=raw.index('   if(new_clocks[0] && !old_clocks[0])')
    b=raw.index('   // MC10173 is LOW-transparent.',a)
    portion=raw[a:b].replace('pins_after[','capture_pins[')
    if mutation=='invert_bank':
        portion=replace(portion,'{2{capture_pins[164]}}','{2{!capture_pins[164]}}')
    return raw[:a]+portion+raw[b:]


def run(source,out,engine,mutation=None):
    r=json.loads((source/'run-state.json').read_text())
    assert r['status']=='PASS_R_FRONT_RM_ADDRESS_EPOCH_PROBE' and not r['mutation']
    assert r['engine']==engine and r['actions'][-1]['returncode']==0
    out.mkdir(parents=True,exist_ok=False);records=[];paths=[]
    targets=['dorado_local_r_front_step.sv','dorado_local_r_memory_step.sv','dorado_local_r_address_step.sv']
    for row in r['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256'];raw=p.read_text()
        if p.name in targets:raw=adapter(raw,p.name,mutation)
        q=out/row['path'];q.parent.mkdir(parents=True,exist_ok=True);q.write_text(raw);paths.append(str(q))
        records.append(dict(path=row['path'],parent_sha256=sha(p),output_sha256=sha(q),changed=sha(p)!=sha(q)))
    assert sum(x['changed'] for x in records)==3
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    defines=[x for x in r['actions'][0]['command'] if x.startswith('-D')]
    commands=[['iverilog','-g2012','-s','tb_r_front_local',*defines,'-o',str(out/'test.vvp'),*paths],
              ['vvp','-i',str(out/'test.vvp')]] if engine=='icarus' else [
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,
         '--Mdir',str(out/'obj'),'-o','test',*paths],[str(out/'obj/test')]]
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),engine=engine,mutation=mutation,
               parent=str(source),parent_run_state_sha256=sha(source/'run-state.json'),source_files=records,actions=[],
               original_reference_and_checks_unchanged=True,all_R_address_edge_data_pre_control=True,
               actual_clock_functions_unchanged=True,transparent_window_data_unchanged=True,
               RAM_ports_unchanged=True,native_timing_qualified=False,assembly_permitted=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for i,cmd in enumerate(commands):
        with(out/('compile.log' if i==0 else 'test.log')).open('w') as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT)
            a=dict(command=cmd,pid=p.pid,started=time.time());state['actions'].append(a);save()
            rc=p.wait();a.update(returncode=rc,finished=time.time());save()
        if rc:break
    text=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    m=re.search(r'R_FRONT_LOCAL_PROBE PASS ([^\n]+)',text)
    state.update(status='PASS_R_FRONT_ALL_ADDRESS_CAPTURE' if not rc and m else 'R_FRONT_ALL_ADDRESS_CAPTURE_FAILED',
                 summary=m[1] if m else None,finished=time.time());save();print(state['status'],flush=True)
    return rc if rc else (0 if m else 1)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True);p.add_argument('--mutation',choices=['invert_bank'])
    a=p.parse_args();sys.exit(run(a.source.resolve(),a.output.resolve(),a.engine,a.mutation))
