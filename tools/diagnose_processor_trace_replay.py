#!/usr/bin/env python3
"""Passively record raw original-reference inputs without altering its checks."""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from test_external_r_packet import sha


def run(source, out):
    assert not out.exists()
    state=json.loads((source/'run-state.json').read_text())
    assert state['status']=='PASS_ORIGINAL_FULL_PROGRAM_WITH_CP_SOURCE_CLOSURE'
    out.mkdir(parents=True)
    for row in state['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256']
        q=out/row['path'];q.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,q)
    tb=out/'inputs/tb_r_front_local.sv';raw=tb.read_text()
    raw=raw.replace(str(source),str(out))
    declaration='''
 integer raw_trace;reg [400:0] raw_before;reg raw_before_root;
 initial begin raw_trace=$fopen("RAW_TRACE","w");if(!raw_trace)$fatal(1,"raw trace open failed");end
'''.replace('RAW_TRACE',str(out/'raw-inputs.hex'))
    raw=raw.replace(' integer trace_file,',declaration+' integer trace_file,',1)
    token='     ca=next_root;pa=pins;pa[144:127]=0;'
    assert raw.count(token)==1
    raw=raw.replace(token,'     raw_before=rp;raw_before_root=root;\n'+token,1)
    token='     trace_record={trace_owner,trace_before,trace_after,trace_pb,trace_pa,trace_result,actual_memory};'
    assert raw.count(token)==1
    raw=raw.replace(token,'''     $fdisplay(raw_trace,"%h",{raw_before_root,raw_before,next_root,pins,reference});
     if(checks<20)$display("RAW_TRACE event=%0d data=%h dR=%h expected=%h h05C=%b h05S=%b",checks,ref_raw_data,reference[571:556],expected[571:556],original.h.h05.p7,original.h.h05.p9);
'''+token,1)
    tb.write_text(raw);shutil.copy2(Path(__file__),out/Path(__file__).name)
    report=dict(status='RUNNING',scope=__doc__,source=str(source),source_state_sha256=sha(source/'run-state.json'),
                actions=[],started=time.time(),raw_record_bits=1648,
                files={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()})
    def save():(out/'run-state.json').write_text(json.dumps(report,indent=2)+'\n')
    save()
    for action in state['actions']:
        command=[v.replace(str(source),str(out))for v in action['command']]
        with(out/(action['label']+'.log')).open('x')as log:
            child=subprocess.Popen(command,stdout=log,stderr=log)
            row=dict(label=action['label'],command=command,pid=child.pid,started=time.time());report['actions'].append(row);save()
            code=child.wait()
        row.update(returncode=code,finished=time.time());save()
        if code:break
    passed=code==0 and 'FULL_PROCESSOR_CP_SOURCE_PASS checks=14884' in (out/'test.log').read_text()
    if passed:
        assert len((out/'raw-inputs.hex').read_text().splitlines())==14884
    report.update(status='PASS_PASSIVE_RAW_INPUT_RECORDING'if passed else'TERMINAL_RAW_RECORDING_FAILED',
                  finished=time.time(),returncode=code)
    save();print(report['status']);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve()))
