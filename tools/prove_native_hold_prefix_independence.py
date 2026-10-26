#!/usr/bin/env python3
"""Prove host prefix preparation cannot be polluted by an unready Hold buffer.

The native packet may save live_prefix on a host edge without publishing guest
state. Establish this complete269-bit output is independent of after-Hold386
for all inputs with actual native constants. Current owner/key are source-only
held/before-context; canonical/settled updates require qualified publication.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from qualify_native_ready_compare import SINGLE,ports
from probe_native_late_hold_permission import NATIVE_CONSTANTS
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def run(probe,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    r=json.loads((probe/'run-state.json').read_text());assert r['native_constant_inputs']==NATIVE_CONSTANTS
    source=probe/'validation';verify(source,r['actions'][0]['files'])
    raw=(source/(SINGLE+'.sv')).read_text();decl=[];args=[[],[]];offset=0
    for direction,width,name in ports(raw):
        if direction!='input':continue
        if name in NATIVE_CONSTANTS:values=["1'b0"]*2
        else:
            decl.append(f' wire [{width-1}:0] {name}=stimulus[{offset}+:{width}];');offset+=width
            values=[name,name]
            if name=='pins_after':values[1]="(pins_after & ~(401'b1<<386))"
        for i in range(2):args[i].append('.'+name+'('+values[i]+')')
    for i,n in enumerate(['expected','actual']):args[i].append('.live_prefix('+n+')')
    top=f'''module prefix_hold_contract(input wire [{offset-1}:0] stimulus,
 output wire [268:0] expected,actual,output wire equivalent);
{chr(10).join(decl)}
 {SINGLE} first({','.join(args[0])});
 {SINGLE} second({','.join(args[1])});
 assign equivalent=expected==actual;
endmodule
'''
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),probe_state_sha256=sha(probe/'run-state.json'),
               native_constants=NATIVE_CONSTANTS,arbitrary_binary_inputs=offset,compared_prefix_bits=269,assumptions=0,actions=[])
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,bad in [('contracts',False),('wrong-before-CP',True)]:
        d=out/label;d.mkdir()
        for f in source.glob('*.sv'):
            if f.name!='top.sv':shutil.copy2(f,d/f.name)
        current=top
        if bad:
            pos=current.index(SINGLE+' second(')
            before=current[:pos];after=current[pos:]
            assert after.count('.pins_before(pins_before)')==1
            current=before+after.replace('.pins_before(pins_before)',".pins_before(pins_before ^ (401'b1<<286))",1)
        (d/'top.sv').write_text(current)
        (d/'proof.ys').write_text(f'''read_verilog -sv -defer {' '.join('/work/'+f.name for f in sorted(d.glob('*.sv')))}
hierarchy -check -top prefix_hold_contract
proc
flatten
opt
check -assert
sat -timeout 120 -prove equivalent 1 -{'falsify'if bad else'verify'} -show-ports -dump_json /work/counterexample.json
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with(d/'proof.log').open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log,env=env);a=dict(label=label,pid=child.pid,command=cmd,started=time.time(),negative=bad,
                files={f.name:sha(f)for f in d.iterdir()if f.name!='proof.log'});state['actions'].append(a);save();code=child.wait()
        marker='model found: FAIL!'if bad else'no model found: SUCCESS!'
        a.update(returncode=code,passed=code==0 and marker in(d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(label,a['passed'],flush=True)
        if not a['passed']:break
    passed=len(state['actions'])==2 and all(a['passed']for a in state['actions'])
    state.update(status='PASS_NATIVE_HOST_PREFIX_AFTER_HOLD_INDEPENDENCE'if passed else'TERMINAL_HOST_PREFIX_INDEPENDENCE_NOT_ESTABLISHED',finished=time.time());save()
    return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['probe','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.probe.resolve(),a.output.resolve(),a.yosys.resolve()))
