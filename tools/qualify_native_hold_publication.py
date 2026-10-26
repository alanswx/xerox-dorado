#!/usr/bin/env python3
"""Compare every complete native single-step output when prepared Hold is ready.

Compose the source-bound producer/buffer induction with full-interface
congruence: equal raw/prepared Hold inputs must produce every same public
output, for all other arbitrary/shared inputs. The implication is the theorem,
not an input assumption. A wrong native pin binding must be detected.
This is a publication theorem, not a preparation-wait/peripheral timing proof.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from qualify_native_ready_compare import SINGLE, ports
from probe_native_late_hold_permission import NATIVE_CONSTANTS
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def run(probe,function,induction,stage,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    p=json.loads((probe/'run-state.json').read_text());f=json.loads((function/'results.json').read_text())
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes']);verify(function,f['files'])
    proof=json.loads((induction/'run-state.json').read_text())
    assert proof['status']=='PASS_SOURCE_BOUND_NATIVE_HOLD_BUFFER_INDUCTION'and proof['assumptions']==0
    assert proof['function_receipt_sha256']==sha(function/'results.json')and all(a['passed']for a in proof['actions'])
    for a in proof['actions']:
        verify(induction/a['label'],a['files'])
        assert a['log_sha256']==sha(induction/a['label']/'proof.log')
    assert p['native_constant_inputs']==NATIVE_CONSTANTS and p['status']=='TERMINAL_DEPENDENCE_PROBE'
    verify(probe/'validation',p['actions'][0]['files'])
    for name,h in p['source_hashes'].items():assert sha(stage/name)==h
    raw=(probe/'validation'/(SINGLE+'.sv')).read_text();rows=ports(raw)
    inbits=sum(w for d,w,n in rows if d=='input'and n not in NATIVE_CONSTANTS)
    outbits=sum(w for d,w,n in rows if d=='output')
    assert inbits==1978
    decl=[];args=[[],[]];ino=outo=0
    for direction,width,name in rows:
        if direction=='input':
            if name in NATIVE_CONSTANTS:values=["1'b0"]*2
            else:
                decl.append(f' wire [{width-1}:0] {name}=stimulus[{ino}+:{width}];');ino+=width
                values=[name]*2
                if name=='pins_after':
                    values=["((pins_after & ~(401'b1<<386)) | ({400'b0,"+value+"}<<386))"for value in ['expected_hold','actual_hold']]
            for i in range(2):args[i].append('.'+name+'('+values[i]+')')
        else:
            for i,prefix in enumerate(['expected','actual']):args[i].append(f'.{name}({prefix}[{outo}+:{width}])')
            outo+=width
    top=f'''module native_hold_publication(input wire expected_hold,actual_hold,
 input wire [{inbits-1}:0] stimulus,output wire ready,safe_publication,
 output wire [{outbits-1}:0] expected,actual);
{chr(10).join(decl)}
 {SINGLE} original({','.join(args[0])});
 {SINGLE} candidate({','.join(args[1])});
 // The separate source-bound induction establishes this equality whenever
 // the prepared buffer is ready. Verify its consequence on the full function.
 assign ready=actual_hold==expected_hold;
 assign safe_publication=!ready || actual==expected;
endmodule
'''
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
               probe_state_sha256=sha(probe/'run-state.json'),function_receipt_sha256=sha(function/'results.json'),
               source_bound_buffer_induction_sha256=sha(induction/'run-state.json'),
               composed_from_induction_and_unrestricted_full_interface_congruence=True,
               arbitrary_native_function_input_bits=inbits,source_descriptor_bits=132,compared_public_outputs=outbits,
               assumptions=0,native_constants=NATIVE_CONSTANTS,actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,bad in [('contracts',False),('wrong-native-pin',True)]:
        d=out/label;d.mkdir()
        for source in [probe/'validation']:
            for path in source.iterdir():
                if path.suffix in ['.sv','.v']and path.name!='top.sv':shutil.copy2(path,d/path.name)
        actual=top
        if bad:
            a="((pins_after & ~(401'b1<<386)) | ({400'b0,actual_hold}<<386))"
            b="((pins_after & ~(401'b1<<385)) | ({400'b0,actual_hold}<<385))"
            assert actual.count(a)==1;actual=actual.replace(a,b,1)
        (d/'top.sv').write_text(actual)
        sat='sat -timeout 120 -prove safe_publication 1 -'+('falsify'if bad else'verify')+' -show-ports -dump_json /work/counterexample.json'
        (d/'proof.ys').write_text(f'''read_verilog -formal -sv -defer {' '.join('/work/'+p.name for p in sorted(d.iterdir())if p.suffix in ['.sv','.v'])}
hierarchy -check -top native_hold_publication
proc
flatten
chformal -lower
opt
check -assert
{sat}
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with(d/'proof.log').open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log,env=env)
            a=dict(label=label,pid=child.pid,command=cmd,started=time.time(),negative=bad,
                   files={p.name:sha(p)for p in d.iterdir()if p.name!='proof.log'})
            state['actions'].append(a);save();code=child.wait()
        marker='model found: FAIL!'if bad else'no model found: SUCCESS!'
        a.update(returncode=code,passed=code==0 and marker in(d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(label,a['passed'],flush=True)
        if not a['passed']:break
    passed=len(state['actions'])==2 and all(a['passed']for a in state['actions'])
    state.update(status='PASS_COMPLETE_NATIVE_PREPARED_HOLD_PUBLICATION'if passed else'TERMINAL_NATIVE_HOLD_PUBLICATION_NOT_QUALIFIED',finished=time.time(),
                 original60ns_schedule_preparation_waits_firmware_or_timing_qualified=False)
    save();return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['probe','function','induction','stage','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.probe.resolve(),a.function.resolve(),a.induction.resolve(),a.stage.resolve(),a.output.resolve(),a.yosys.resolve()))
