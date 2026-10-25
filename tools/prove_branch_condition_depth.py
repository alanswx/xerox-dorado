#!/usr/bin/env python3
"""Prove the branch-family fixed point needs three passes, without assumptions.

All80 old state bits, both38-pin frames and both root levels are arbitrary
binary inputs. With --frame, the additional38 early inputs give196 inputs.
Compare every next-state/event/settling bit with eight passes.
A one-pass counterexample must change the actual visible flags. This proves
combinational depth equivalence, not arbitrary-arrival or physical timing.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from test_external_r_packet import sha
from review_baseboard_clock_consumer import verify

ROOT=Path(__file__).resolve().parents[1]
TOP='''module contract(input wire [79:0] q,input wire cb,ca,
 input wire [37:0] pb,pa,output wire [84:0] actual,expected,
 output wire equal,visible_equal);
 dorado_branch_condition_step #(.PINS(38),.WAVES(DEPTH)) candidate(q,cb,ca,pb,pa,actual[84:5],actual[4:1],actual[0]);
 dorado_branch_condition_step #(.PINS(38),.WAVES(8)) baseline(q,cb,ca,pb,pa,expected[84:5],expected[4:1],expected[0]);
 assign equal=actual==expected;
 assign visible_equal=actual[72:69]==expected[72:69];
endmodule
'''


def run(source,out,yosys,frame=False):
    assert not out.exists();out.mkdir(parents=True)
    parent=json.loads((source/'run-state.json').read_text());verify(source,parent['files'])
    if frame:
        assert parent['status']=='PASS_ORIGINAL_PROCESSOR_BRANCH_COMPOSITION' and parent.get('mutation') is None
        assert parent['explicit_early_frame'] and parent['optimized_branch_transfer_checked']
        candidate=source/'branch-step.sv';cone=source/'reference/branch/cone.sv'
        top=TOP.replace('input wire [37:0] pb,pa','input wire [37:0] pb,pe,pa')
        top=top.replace('dorado_branch_condition_step','dorado_branch_condition_frame_step').replace('q,cb,ca,pb,pa,','q,cb,ca,pb,pe,pa,')
    else:
        assert parent['status']=='PASS_BRANCH_CONDITION_CAPTURE' and parent['mutation'] is None
        assert parent['reference_manifest']['physical_bits']==80
        candidate=source/'candidate.sv';cone=source/'reference/cone.sv';top=TOP
    for name,path in [('candidate.sv',candidate),('cone.sv',cone)]:
        shutil.copy2(path,out/name)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    result=dict(status='RUNNING',scope=__doc__,source=str(source),source_state_sha256=sha(source/'run-state.json'),
                arbitrary_binary_input_bits=196 if frame else 158,compared_result_bits=85,protocol_assumptions=0,
                explicit_early_frame=frame,
                started=time.time(),actions=[],files={p.name:sha(p) for p in out.iterdir()})
    def save():(out/'run-state.json').write_text(json.dumps(result,indent=2)+'\n')
    save()
    for depth in [3,1]:
        positive=depth==3;label='depth'+str(depth);(out/(label+'.sv')).write_text(top.replace('DEPTH',str(depth)))
        flag='verify' if positive else'falsify';signal='equal' if positive else'visible_equal'
        script=f'''read_verilog -formal -sv /work/candidate.sv /work/cone.sv /work/{label}.sv
hierarchy -check -top contract
proc
flatten
opt
check -assert
sat -timeout 120 -prove {signal} 1 -{flag} -show-ports -dump_json /work/{label}-counterexample.json
'''
        (out/(label+'.ys')).write_text(script)
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
        command=[str(yosys),'-Q','-T','-s','/work/'+label+'.ys']
        with (out/(label+'.log')).open('x') as log:
            child=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT,env=env)
            row=dict(depth=depth,command=command,pid=child.pid,started=time.time());result['actions'].append(row);save()
            code=child.wait()
        text=(out/(label+'.log')).read_text()
        passed=code==0 and ('no model found: SUCCESS!' in text if positive else'model found: FAIL!' in text)
        row.update(returncode=code,passed=passed,log_sha256=sha(out/(label+'.log')),finished=time.time());save()
        if not passed:break
    passed=len(result['actions'])==2 and all(r['passed'] for r in result['actions'])
    result.update(status='PASS_BRANCH_THREE_PASS_DEPTH_AND_VISIBLE_NEGATIVE' if passed else'TERMINAL_BRANCH_DEPTH_NOT_ESTABLISHED',
                  finished=time.time(),files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file() and p.name!='run-state.json'})
    save();print(result['status']);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['source','output','yosys']:p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--frame',action='store_true',help='Prove the explicit early/late input-frame transfer with196 arbitrary binary inputs')
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.yosys.resolve(),a.frame))
