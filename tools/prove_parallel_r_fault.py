#!/usr/bin/env python3
"""Prove the actual pure child fault bit under parallel decode cofactors.

All1517 binary input bits are arbitrary, with no protocol/state assumptions.
Cofactor selection changes one output only; source inversion retains every
other processor byte. Preserve the exact compiled child/header/source graph.
Wrong low polarity and stale low-cofactor pins must produce counterexamples.
Four-state equivalence is a separate compiled gate, required before selection.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from generate_parallel_r_fault import INPUTS,input_declarations,child_call,helper,validate_child_signature
from test_r_front_forward_view import sha

TOP='''module parallel_fault_equivalence(
 DECLARATIONS,output wire equal,actual,output wire [3:0] expected);
 REFERENCE
 dorado_parallel_r_fault_step candidate(
 BINDINGS,
 .original_fault(expected[1]),.fault(actual));
 assign equal=(actual==expected[1]);
endmodule
'''


def run(source,out,yosys):
    assert not out.exists();s=json.loads((source/'run-state.json').read_text())
    assert s['status']=='PASS_ENTRY_CACHED_FULL_PROCESSOR_PACKETS' and s['packet_variant']=='split-pair'
    for n,h in s['files'].items():assert sha(source/n)==h
    validate_child_signature((source/'adapters/dorado_local_r_front_ready_step.sv').read_text())
    out.mkdir(parents=True);inputs=out/'inputs';inputs.mkdir();paths=[]
    for row in s['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256'];d=inputs/row['path'];d.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,d);paths.append(row['path'])
    for n in [Path(__file__).name,'generate_parallel_r_fault.py']:shutil.copy2(Path(__file__).with_name(n),out/n)
    top=TOP.replace('DECLARATIONS',input_declarations()).replace('REFERENCE',child_call('reference','pins_after','expected')).replace(
        'BINDINGS',',\n '.join('.'+n+'('+n+')' for n,w in INPUTS))
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),source_state_sha256=sha(source/'run-state.json'),
        arbitrary_binary_input_bits=sum(w for _,w in INPUTS),input_or_state_assumptions_added=False,
        unused_packet_controller_module_file_excluded=True,
        files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file()},cases=[],hardware_changed=False,native_integrated=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for mode in ['positive','invert-low','stale-low']:
        d=out/mode;d.mkdir();(d/'helper.sv').write_text(helper(None if mode=='positive' else mode));(d/'top.sv').write_text(top)
        # candidate.sv defines the enclosing packet/controller/cache, none of
        # which is instantiated by this pure child. Keep it frozen as source
        # evidence, but do not parse its unrelated initialized cache ports.
        # hierarchy -check still requires the complete real child graph.
        names=['/work/inputs/'+n for n in paths if n!='candidate.sv']+['/work/'+mode+'/helper.sv','/work/'+mode+'/top.sv']
        flag='verify' if mode=='positive' else 'falsify'
        (d/'proof.ys').write_text('read_verilog -sv -defer '+' '.join(names)+f'''
hierarchy -check -top parallel_fault_equivalence
proc
flatten
memory_map
opt
check -assert
sat -timeout 120 -prove equal 1 -{flag} -show-ports -dump_json /work/{mode}/counterexample.json
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
        cmd=[str(yosys),'-Q','-T','-s','/work/'+mode+'/proof.ys'];row=dict(mode=mode,command=cmd,files={p.name:sha(p) for p in d.iterdir()},started=time.time())
        with (d/'proof.log').open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT,env=env);row['pid']=child.pid;state['cases'].append(row);save();code=child.wait()
        text=(d/'proof.log').read_text();marker='no model found: SUCCESS!' if mode=='positive' else 'model found: FAIL!'
        row.update(passed=code==0 and marker in text,returncode=code,finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        if not row['passed']:break
    passed=len(state['cases'])==3 and all(v['passed'] for v in state['cases'])
    state.update(status='PASS_PARALLEL_R_FAULT_ALL_BINARY_INPUTS' if passed else 'TERMINAL_PARALLEL_FAULT_NOT_ESTABLISHED',finished=time.time());save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.yosys.resolve()))
