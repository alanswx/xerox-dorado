#!/usr/bin/env python3
"""Observe unknown actual single-function inputs in the old Icarus replay.

Run an immutable source set with passive prints and a32-record diagnostic cap.
No original fixture, architectural state, assertion or RTL is changed. The
early stop is an investigation only and is never a complete replay pass.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from prove_cp_prefix_cache import module
from qualify_native_ready_compare import SINGLE,ports
from test_external_r_packet import sha


def run(source,out):
    assert not out.exists();out.mkdir(parents=True)
    s=json.loads((source/'run-state.json').read_text())
    assert s['actions'][-1]['returncode']==0
    inputs=[];paths=[]
    for row in s['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256'];paths.append(p)
        inputs.append(dict(path=str(p),sha256=sha(p)))
    function=module((source/'candidate.sv').read_text(),SINGLE)
    names=[n for d,w,n in ports(function)if d=='input']
    prints='\n'.join(f'''    if((^dut.packet.single.{n})===1'bx)$display("OPERAND_UNKNOWN_INPUT record=%0d name={n} value=%h",idx,dut.packet.single.{n});'''for n in names)
    tb=(source/'tb.sv').read_text()
    marker=' task automatic prepare_operand_view;'
    assert tb.count(marker)==1
    tb=tb.replace(marker,' integer operand_context_observations=0;\n'+marker,1)
    marker='   edges=0;saved_memory=memory_image();saved_owner=held_q;'
    # This marker occurs in both preparation tasks; target operand task only.
    start=tb.index(' task automatic prepare_operand_view;');pos=tb.index(marker,start)
    text='''   if(operand_context_observations<6)begin
    $display("OPERAND_CONTEXT record=%0d known=%b pair=%b",idx,dut.operand_context_known,use_pair);
PRINTS
    operand_context_observations++;
   end
'''.replace('PRINTS',prints)
    tb=tb[:pos]+text+tb[pos:]
    marker='  while(idx<COUNT)begin';assert tb.count(marker)==1
    tb=tb.replace(marker,marker+'\n   if(idx>=32)begin $display("OPERAND_CONTEXT_DIAGNOSTIC_BOUND records=%0d",idx);$finish;end',1)
    target=out/'tb.sv';target.write_text(tb);paths=[target if p.name=='tb.sv'else p for p in paths]
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,source_state_sha256=sha(source/'run-state.json'),inputs=inputs,
        tb_sha256=sha(target),started=time.time(),actions=[],complete_replay_pass=False,hardware_changed=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    for label,command in [('compile',['iverilog','-g2012','-s','tb_r_front_packet','-o',str(out/'test.vvp'),*map(str,paths)]),
                          ('test',['vvp','-i',str(out/'test.vvp')])]:
        with(out/(label+'.log')).open('x')as log:
            p=subprocess.Popen(command,stdout=log,stderr=log)
            action=dict(label=label,pid=p.pid,command=command,started=time.time());state['actions'].append(action);save();code=p.wait()
        action.update(returncode=code,finished=time.time());save()
        if code:break
    state.update(status='OBSERVED_BOUNDED_NATIVE_OPERAND_CONTEXT'if code==0 else'TERMINAL_OPERAND_CONTEXT_DIAGNOSTIC_FAILED',returncode=code,finished=time.time(),
        files={p.name:sha(p)for p in out.iterdir()if p.is_file()and p.name!='run-state.json'and p.suffix!='.vvp'})
    save();print(state['status'],flush=True);return code


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve()))
