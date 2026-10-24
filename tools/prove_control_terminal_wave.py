#!/usr/bin/env python3
"""Diagnose whether the fifth atomic wave transfers data or recognizes settling.

One property compares all362 state/event/flag outputs at four versus24waves,
explicitly excluding the unsettled bit. A separate property tests whether
the original24-wave unsettled output is always clear. Neither property alone
authorizes fewer waves: an actual replacement must still prove ALL363 outputs,
retain four-state behavior/diagnostics and pass the full original comparisons.
No RTL, assumptions, native timing, guest state or hardware is changed.
"""
import argparse
import json
import os
from pathlib import Path
import subprocess
import time

from prove_memory_mir_rtl import atomic,TOP
from test_r_front_forward_view import sha


def run(source,out,yosys,property_name):
    assert not out.exists();out.mkdir(parents=True)
    raw=source.read_text()
    (out/'original.sv').write_text(raw)
    (out/'reference.sv').write_text(atomic(raw,'original_atomic'))
    (out/'four.sv').write_text(atomic(raw,'candidate_atomic'))
    top=TOP.replace('original_atomic original(','original_atomic #(.WAVES(24)) original(')
    top=top.replace('candidate_atomic candidate(','candidate_atomic #(.WAVES(4)) candidate(')
    old='assign equal=~|different;'
    assert top.count(old)==1
    expression="~|(different & ~(363'b1 << 94))"if property_name=='four-transfer'else'!old_unsettled'
    top=top.replace(old,'assign equal='+expression+';',1)
    (out/'top.sv').write_text(top);(out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    script='''read_verilog -sv /work/reference.sv /work/four.sv /work/top.sv
hierarchy -check -top memory_mir_equivalence
proc
flatten
opt
check -assert
sat -timeout 120 -prove equal 1 -show-inputs -show-outputs -dump_json /work/counterexample.json
'''
    (out/'proof.ys').write_text(script)
    state=dict(status='RUNNING',scope=__doc__,driver_pid=os.getpid(),started=time.time(),
        property=property_name,source=str(source),source_sha256=sha(source),
        arbitrary_binary_state_input_bits=955,assumptions_added=False,
        outputs_compared=362 if property_name=='four-transfer'else 1,
        excluded_output='Unsettled bit94'if property_name=='four-transfer'else'All outputs except original unsettled',
        files={p.name:sha(p)for p in out.iterdir()if p.is_file()},
        authorizes_wave_count_change=False,hardware_changed=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    env=os.environ.copy();env['YOWASP_MOUNT']='/work='+str(out)
    env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');save()
    with(out/'proof.log').open('x')as log:
        command=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        p=subprocess.Popen(command,env=env,stdout=log,stderr=subprocess.STDOUT)
        state.update(pid=p.pid,command=command);save();code=p.wait()
    log=(out/'proof.log').read_text();proved=code==0 and'SAT proof finished - no model found: SUCCESS!'in log
    counterexample=code==0 and'SAT proof finished - model found: FAIL!'in log
    state.update(status='PROVED_TERMINAL_WAVE_PROPERTY'if proved else
                 'COUNTEREXAMPLE_TO_TERMINAL_WAVE_PROPERTY'if counterexample else'TERMINAL_WAVE_PROPERTY_NOT_ESTABLISHED',
                 returncode=code,proved=proved,counterexample_found=counterexample,
                 finished=time.time(),proof_log_sha256=sha(out/'proof.log'))
    save();print(state['status'],property_name,flush=True);return 0 if proved or counterexample else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['source','output','yosys']:p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--property',choices=['four-transfer','settled'],required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.yosys.resolve(),a.property))
