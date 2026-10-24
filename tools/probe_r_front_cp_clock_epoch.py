#!/usr/bin/env python3
"""Test immediate CP NoDispatch gating at a coincident distributed root edge.

The existing joint solver first lowers the controller clock with old CP Q,
then raises it with newly captured NoDispatch, inventing a pulse absent from
the original delayed graph. Feed only that clock gate from the current CP
write's Q view. Keep CP owner state, controller data, all assertions and the
original reference unchanged. This is a timing-contract experiment.
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


def candidate(raw,mutation=None):
    a=raw.index('module dorado_control_hold_ordered_step')
    before,part=raw[:a],raw[a:]
    marker=' function automatic [55:0] joint_clocks('
    addition=''' // CP write clocks depend only on the external CP bus/decoder. On a
 // coincident falling root, NoDispatch reaches its clock gate before the
 // distributed instruction-clock fall. Avoid a synthetic LOW/HIGH pulse.
 wire [1:0] cp_clock_before={pins_before[292]|(pins_before[291:289]!=3'b110),
                            pins_before[292]|(pins_before[291:289]!=3'b111)};
 wire [1:0] cp_clock_after={pins_after[292]|(pins_after[291:289]!=3'b110),
                           pins_after[292]|(pins_after[291:289]!=3'b111)};
 wire [21:0] cp_direct_events={cp_clock_after & ~cp_clock_before,20'b0};
 wire [11:0] cp_direct_next=cp_transfer(state_q[75:64],cp_direct_events,ca_pins(state_q,pins_after));
 wire cp_no_dispatch_at_clock=cp_direct_next[5];
'''
    part=replace(part,marker,addition+marker)
    part=replace(part,'input switch_view,input tasking_view);',
                 'input switch_view,input tasking_view,input after_epoch);')
    level="s[69]" if mutation=='omit_early' else '(s[69]|(after_epoch & cp_no_dispatch_at_clock))'
    part=replace(part,'clock_s=s[109:0];clock_s[41]=tasking_view;',
                 f'clock_s=s[109:0];clock_s[69]={level};clock_s[41]=tasking_view;')
    part=replace(part,'previous=joint_clocks(q,clock_before,pins_before,mir_switch_view,tasking_view);',
                 "previous=joint_clocks(q,clock_before,pins_before,mir_switch_view,tasking_view,1'b0);")
    part=replace(part,'levels=joint_clocks(q,clock_after,pins_after,mir_switch_view,tasking_view);',
                 "levels=joint_clocks(q,clock_after,pins_after,mir_switch_view,tasking_view,1'b1);")
    return before+part


def run(source,out,engine,mutation=None):
    r=json.loads((source/'run-state.json').read_text())
    assert r['engine']==engine and r['actions'][-1]['returncode'] is not None
    assert r['status'] in ['PASS_R_FRONT_ALL_ADDRESS_CAPTURE','POINTER_PACKAGE_D_AT_CLOCK_AUDIT_FAILED']
    if r['status'].endswith('FAILED'):
        assert r['extra_coincident_CP_transactions'] and 'R front solver unsettled/repeated' in (source/'test.log').read_text()
    out.mkdir(parents=True,exist_ok=False);paths=[];records=[]
    for row in r['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256'];raw=p.read_text()
        if p.name=='candidate.sv':raw=candidate(raw,mutation)
        q=out/row['path'];q.parent.mkdir(parents=True,exist_ok=True);q.write_text(raw);paths.append(str(q))
        records.append(dict(path=row['path'],parent_sha256=sha(p),output_sha256=sha(q),changed=sha(p)!=sha(q)))
    assert sum(x['changed'] for x in records)==1
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    defines=[x for x in r['actions'][0]['command'] if x.startswith('-D')]
    commands=[['iverilog','-g2012','-s','tb_r_front_local',*defines,'-o',str(out/'test.vvp'),*paths],
              ['vvp','-i',str(out/'test.vvp')]] if engine=='icarus' else [
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,
         '--Mdir',str(out/'obj'),'-o','test',*paths],[str(out/'obj/test')]]
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),engine=engine,mutation=mutation,
               parent=str(source),parent_run_state_sha256=sha(source/'run-state.json'),source_files=records,actions=[],
               CP_owner_state_unchanged=True,controller_data_unchanged=True,original_reference_and_checks_unchanged=True,
               only_NoDispatch_clock_view_changed=True,qualifying_native_timing=False,assembly_permitted=False)
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
    state.update(status='PASS_CP_NODISPATCH_CLOCK_EPOCH' if not rc and m else 'CP_NODISPATCH_CLOCK_EPOCH_FAILED',
                 summary=m[1] if m else None,finished=time.time());save();print(state['status'],flush=True)
    return rc if rc else (0 if m else 1)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True);p.add_argument('--mutation',choices=['omit_early'])
    a=p.parse_args();sys.exit(run(a.source.resolve(),a.output.resolve(),a.engine,a.mutation))
