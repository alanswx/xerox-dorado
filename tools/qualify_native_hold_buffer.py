#!/usr/bin/env python3
"""Induct on prepared Hold using the source-bound native memory producer.

For arbitrary descriptors and reset history, every ready prepared decision must
equal the actual producer. Saved descriptors are re-evaluated by a second full
producer instance to strengthen induction. Real wrong-tag/payload mutations
must produce wrong ready decisions. No input assumptions or new guest edge.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

TOP='''module hold_buffer_contract(input wire clk,reset,input wire [131:0] descriptor,
 output wire ready,safe_decision,output wire actual,expected);
 wire valid,saved_hold,known;wire [131:0] saved_context;wire saved_function;
 dorado_memc_hold_function current(descriptor,expected);
 dorado_memc_hold_function saved(saved_context,saved_function);
 dorado_memc_hold_buffer buffered(clk,reset,descriptor,expected,ready,actual,valid,saved_context,saved_hold,known);
 assign safe_decision=!ready || actual==expected;
 always @*begin
  if(valid)assert(saved_hold==saved_function);
  assert(safe_decision);
 end
endmodule
'''


def run(source,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    r=json.loads((source/'results.json').read_text());verify(source,r['files'])
    assert r['status']=='PREPARED_SOURCE_BOUND_NATIVE_HOLD_FUNCTION'and r['context_bits']==132
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),function_receipt_sha256=sha(source/'results.json'),
               helper_sha256=r['buffer_sha256'],context_bits=132,assumptions=0,actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,mutation in [('contracts',None),('ignore-context',('valid && saved_context==source_context','valid')),
                           ('wrong-payload',('saved_hold<=raw_hold;','saved_hold<=~raw_hold;'))]:
        d=out/label;d.mkdir()
        for p in source.iterdir():
            if p.suffix in ['.v','.sv']:shutil.copy2(p,d/p.name)
        if mutation:
            raw=(d/'buffer.sv').read_text();assert raw.count(mutation[0])==1
            (d/'buffer.sv').write_text(raw.replace(*mutation,1))
        (d/'top.sv').write_text(TOP)
        sat=('sat -timeout 120 -seq 3 -tempinduct -verify -prove-asserts'if mutation is None else
             'sat -timeout 120 -seq 4 -prove safe_decision 1 -falsify -show-ports -dump_json /work/counterexample.json')
        script=f'''read_verilog -formal -sv -defer {' '.join('/work/'+p.name for p in sorted(d.iterdir())if p.suffix in ['.v','.sv'])}
hierarchy -check -top hold_buffer_contract
proc
flatten
chformal -lower
opt
check -assert
{sat}
'''
        (d/'proof.ys').write_text(script);env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with(d/'proof.log').open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log,env=env)
            a=dict(label=label,pid=child.pid,command=cmd,started=time.time(),negative=mutation is not None,
                   files={p.name:sha(p)for p in d.iterdir()if p.name!='proof.log'})
            state['actions'].append(a);save();code=child.wait()
        marker='Induction step proven: SUCCESS!'if mutation is None else'model found: FAIL!'
        a.update(returncode=code,passed=code==0 and marker in(d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(label,a['passed'],flush=True)
        if not a['passed']:break
    passed=len(state['actions'])==3 and all(a['passed']for a in state['actions'])
    state.update(status='PASS_SOURCE_BOUND_NATIVE_HOLD_BUFFER_INDUCTION'if passed else'TERMINAL_HOLD_BUFFER_NOT_QUALIFIED',
                 finished=time.time(),full_native_integration_timing_60ns_or_speed_qualified=False)
    save();return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.yosys.resolve()))
