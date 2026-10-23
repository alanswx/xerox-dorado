#!/usr/bin/env python3
"""Check physical RbBypass at both actual operand-capture inputs.

Reuse the full original comparison unchanged except for two additional seam
assertions. Positive RTL is byte-identical to the tested forwarding component;
compare its physical export with both consumer BEFORE inputs at both epochs.
Original clocks and all844Qs include the captured l07 values. Native consumer
setup/hold and original60ns qualification remain separate requirements.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import time

from test_r_front_task_event import replace


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def bench(raw):
    expression='{dut.child.child.child.child.child_before[150],dut.child.child.child.child.child_before[148]}'
    for epoch,marker in [('BEFORE','     expected=n;predicted=events;'),('AFTER','     if({raw_parity,raw_data}!=={ref_raw_parity,ref_raw_data})')]:
        assert raw.count(marker)==1
        check=f'     if({expression}!=={{2{{forwarded[0]}}}})$fatal(1,"Physical RbBypass consumer mismatch {epoch} commit check=%0d",checks);\n'
        raw=raw.replace(marker,check+marker,1)
    return raw


def run(source,out,engine,mutation):
    parent=json.loads((source/'run-state.json').read_text());assert parent['status']=='PASS_R_FRONT_PHYSICAL_FORWARD'and not parent['mutation']
    assert parent['actions'][-1]['returncode']==0 and parent['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
    for row in parent['source_files']:assert sha(source/row['path'])==row['output_sha256']
    build=next(a for a in parent['actions']if a['command'][0]in ['iverilog','verilator']);paths=[Path(n)for n in build['command']if n.endswith(('.sv','.v'))]
    out.mkdir(parents=True,exist_ok=False);records=[];copied=[]
    for p in paths:
        rel=p.relative_to(source);q=out/rel;q.parent.mkdir(parents=True,exist_ok=True);original=p.read_text();raw=original
        if p.name=='tb_r_front_local.sv':raw=bench(raw)
        if mutation and p.name=='dorado_local_r_address_step.sv':
            old='pins_before[149],rb_forward(state_q,pins_before),pins_before[147:113],'
            raw=replace(raw,old,'pins_before[149],!rb_forward(state_q,pins_before),pins_before[147:113],')
        q.write_text(raw);copied.append(q);records.append(dict(path=str(rel),parent_sha256=sha(p),output_sha256=sha(q),changed=original!=raw))
    assert sum(r['changed']for r in records)==(2 if mutation else 1)
    (out/'source-run-state.json').write_bytes((source/'run-state.json').read_bytes());(out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),engine=engine,mutation=mutation,parent=str(source),source_files=records,
               parent_run_state_sha256=sha(source/'run-state.json'),actions=[],positive_RTL_byte_identical=not bool(mutation),
               both_capture_inputs_checked=True,captured_Qs_and_clocks_still_original_checked=True,
               native_consumer_timing_qualified=False,physical_timing_qualified=False,assembly_permitted=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    defines=[x for x in build['command']if x.startswith('-D')]
    commands=[['iverilog','-g2012','-s','tb_r_front_local',*defines,'-o',str(out/'test.vvp'),*map(str,copied)],['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else[
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,'--Mdir',str(out/'obj'),'-o','test',*map(str,copied)],[str(out/'obj/test')]]
    save();rc=0
    for i,cmd in enumerate(commands):
        with(out/('compile.log'if i==0 else'test.log')).open('w')as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);row=dict(command=cmd,pid=p.pid,started=time.time());state['actions'].append(row);save()
            rc=p.wait();row.update(returncode=rc,finished=time.time());save()
        if rc:break
    text=(out/'test.log').read_text()if(out/'test.log').exists()else'';m=re.search(r'R_FRONT_LOCAL_PROBE PASS ([^\n]+)',text)
    state.update(status='PASS_R_FORWARD_CONSUMER'if not rc and m else'R_FORWARD_CONSUMER_FAILED',summary=m[1]if m else None,finished=time.time());save()
    print(state['status'],flush=True);return rc if rc else(0 if m else 1)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],default='icarus');p.add_argument('--mutation',choices=['invert_H_capture']);a=p.parse_args()
    sys.exit(run(a.source.resolve(),a.output.resolve(),a.engine,a.mutation))
