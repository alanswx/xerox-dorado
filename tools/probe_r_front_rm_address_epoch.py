#!/usr/bin/env python3
"""Test old RBase mux data at the low RM address capture, keeping other data.

The v6 critical path runs through newly resolved MIR and the RBase file into
ProcL g10/h10. HM Processor pp.10-11 says an RBase load uses its old value
for that instruction's read and write. Probe only the four RBase mux inputs
to the existing edge capture; keep clocks, all LOW windows, all other address
inputs and the complete original reference/assertions unchanged.
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


def address(raw,mutation=None):
    old='captured_low=low_address_data(state_q,pins_after);'
    data="4'b0" if mutation=='zero_rbase' else 'pins_before[159:156]'
    return replace(raw,old,'captured_low=low_address_data(state_q,{pins_after[201:160],'+data+',pins_after[155:0]});')


def run(source,out,engine,mutation=None):
    parent=json.loads((source/'run-state.json').read_text())
    assert parent['status']=='PASS_R_FRONT_POINTER_CAPTURE_PROBE' and not parent.get('mutation')
    assert parent['engine']==engine and parent['actions'][-1]['returncode']==0
    out.mkdir(parents=True,exist_ok=False)
    records=[];paths=[]
    for row in parent['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256']
        raw=p.read_text()
        if p.name=='dorado_local_r_address_step.sv':raw=address(raw,mutation)
        q=out/row['path'];q.parent.mkdir(parents=True,exist_ok=True);q.write_text(raw);paths.append(str(q))
        records.append(dict(path=row['path'],parent_sha256=sha(p),output_sha256=sha(q),changed=sha(p)!=sha(q)))
    assert sum(x['changed'] for x in records)==1
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    defines=[x for x in parent['actions'][0]['command'] if x.startswith('-D')]
    commands=[['iverilog','-g2012','-s','tb_r_front_local',*defines,'-o',str(out/'test.vvp'),*paths],
              ['vvp','-i',str(out/'test.vvp')]] if engine=='icarus' else [
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,
         '--Mdir',str(out/'obj'),'-o','test',*paths],[str(out/'obj/test')]]
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),engine=engine,mutation=mutation,
               parent=str(source),parent_run_state_sha256=sha(source/'run-state.json'),source_files=records,actions=[],
               original_reference_and_checks_unchanged=True,only_four_RBase_capture_inputs_changed=True,
               transparent_window_data_unchanged=True,native_timing_qualified=False,assembly_permitted=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for i,cmd in enumerate(commands):
        with(out/('compile.log' if i==0 else 'test.log')).open('w') as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT)
            a=dict(command=cmd,pid=p.pid,started=time.time());state['actions'].append(a);save()
            rc=p.wait();a.update(returncode=rc,finished=time.time());save()
        if rc:break
    log=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    m=re.search(r'R_FRONT_LOCAL_PROBE PASS ([^\n]+)',log)
    state.update(status='PASS_R_FRONT_RM_ADDRESS_EPOCH_PROBE' if not rc and m else 'R_FRONT_RM_ADDRESS_EPOCH_PROBE_FAILED',
                 summary=m[1] if m else None,finished=time.time());save();print(state['status'],flush=True)
    return rc if rc else (0 if m else 1)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True);p.add_argument('--mutation',choices=['zero_rbase'])
    a=p.parse_args();sys.exit(run(a.source.resolve(),a.output.resolve(),a.engine,a.mutation))
