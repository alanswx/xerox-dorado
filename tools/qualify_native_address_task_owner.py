#!/usr/bin/env python3
"""Share the actual child's task result with its native address consumer.

Retain every existing output/guard and compare the full interface against the
frozen module. This removes a duplicate task evaluation, not an architectural
capture or storage barrier. No native clock, state or timing constraint changes.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from prove_cp_prefix_cache import module
from prove_native_address_task_owner import NAMES
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

SOURCE='src/verilog/rtl/dorado_local_r_address_step.sv'
NAME=NAMES[0]


def ports(text):
    header=module(text,NAME).split('(',1)[1].split(');',1)[0]
    header=re.sub(r'//[^\n]*','',header)
    result=[];direction=None;width=None
    for field in header.split(','):
        token=field.strip()
        m=re.match(r'(input|output)\s+(?:wire|reg)\s+(?:\[(\d+):(\d+)\]\s+)?(\w+)$',token)
        if m:
            direction=m[1];width=int(m[2])-int(m[3])+1 if m[2]else 1;name=m[4]
        else:
            assert direction and re.fullmatch(r'\w+',token),token;name=token
        result.append((direction,width,name))
    assert len({name for _,_,name in result})==len(result)
    return result


def transform(raw,bad=False):
    old=module(raw,NAME)
    pattern=r' dorado_local_task_capture_step core\(.*?shared_capture\);'
    found=re.findall(pattern,old,re.S);assert len(found)==1
    comment=' // One actual task result supplies both address and operand consumers.'
    replacement='''
 // Keep the original four-state self-comparison below: unknown Q still
 // yields an unknown comparison, exactly as with two identical evaluators.
 assign core_next=child_next[87:0];
 assign core_events=clock_events[24:0];
 assign core_repeated=repeated_events[24:0];
 assign core_unsettled=child_unsettled;'''
    if bad:replacement=replacement.replace('core_next=child_next[87:0];',"core_next=child_next[87:0]^(88'b1<<32);")
    declaration=' wire child_unsettled;'
    assert old.count(declaration)==1
    actual=old.replace(found[0],comment,1).replace(declaration,declaration+replacement,1)
    assert actual.replace(declaration+replacement,declaration,1).replace(comment,found[0],1)==old
    return raw.replace(old,actual,1)


def top(raw):
    rows=ports(raw);inputs=[r for r in rows if r[0]=='input'];outputs=[r for r in rows if r[0]=='output']
    bits=sum(w for _,w,_ in inputs);result_bits=sum(w for _,w,_ in outputs)
    decl=[];actual=[];expected=[];offset=0
    for _,width,name in inputs:
        decl.append(f' wire [{width-1}:0] {name}=stimulus[{offset}+:{width}];');offset+=width
    offset=0
    for direction,width,name in rows:
        if direction=='input':a=b=name
        else:
            a=f'actual[{offset}+:{width}]';b=f'expected[{offset}+:{width}]';offset+=width
        actual.append('.'+name+'('+a+')');expected.append('.'+name+'('+b+')')
    return f'''module address_owner_equivalence(input wire [{bits-1}:0] stimulus,
 output wire [{result_bits-1}:0] actual,expected,output wire equivalent);
{chr(10).join(decl)}
 reference_address_step original({','.join(expected)});
 {NAME} candidate({','.join(actual)});
 assign equivalent=actual==expected;
endmodule
''',bits,result_bits,rows


def run(stage,invariant,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    p=json.loads((invariant/'run-state.json').read_text());verify(invariant,p['files'])
    assert p['status']=='PROVED_ACTUAL_NATIVE_ADDRESS_TASK_OWNER_EQUIVALENCE'and p['assumptions']==0
    assert sha(invariant/'proof.log')==p['log_sha256']
    # The v1 receipt miscounted one202-bit capture bus as201. The exact
    # frozen top/proof has2114 inputs; preserve its metadata and correct the
    # count here from the actual complete interface.
    for n,h in p['source_hashes'].items():assert sha(stage/n)==h,n
    raw=(stage/SOURCE).read_text();after=transform(raw)
    (out/'native-address-before.sv').write_text(raw);(out/'native-address-after.sv').write_text(after)
    wrapper,bits,result_bits,rows=top(raw);assert bits==2114
    support=[]
    for name in NAMES[1:]:
        f=out/(name+'.sv');shutil.copy2(stage/('src/verilog/rtl/'+name+'.sv'),f);support.append(f.name)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),stage=str(stage),
        invariant=str(invariant),invariant_state_sha256=sha(invariant/'run-state.json'),
        invariant_original_reported_input_bits=p['arbitrary_binary_input_bits'],
        actual_arbitrary_binary_input_bits=bits,compared_outputs=result_bits,ports=rows,assumptions=0,
        source_address_sha256=sha(stage/SOURCE),files={p.name:sha(p)for p in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,bad in [('complete-interface',False),('wrong-StartCycle-alias',True)]:
        d=out/label;d.mkdir()
        for name in support:shutil.copy2(out/name,d/name)
        (d/'original.sv').write_text(module(raw,NAME).replace('module '+NAME+'(','module reference_address_step(',1))
        (d/'actual.sv').write_text(transform(raw,bad));(d/'top.sv').write_text(wrapper)
        script=f'''read_verilog -sv -defer {' '.join('/work/'+n for n in support)} /work/original.sv /work/actual.sv /work/top.sv
hierarchy -check -top address_owner_equivalence
proc
flatten
opt
check -assert
sat -timeout 120 -prove equivalent 1 -{'falsify'if bad else'verify'} -show-ports -dump_json /work/counterexample.json
'''
        (d/'proof.ys').write_text(script)
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        command=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with(d/'proof.log').open('x')as log:
            child=subprocess.Popen(command,stdout=log,stderr=log,env=env)
            row=dict(label=label,negative=bad,pid=child.pid,command=command,started=time.time(),files={p.name:sha(p)for p in d.iterdir()if p.name!='proof.log'})
            state['actions'].append(row);save();code=child.wait()
        marker='model found: FAIL!'if bad else'no model found: SUCCESS!'
        row.update(returncode=code,passed=code==0 and marker in(d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(label,row['passed'],flush=True)
        if not row['passed']:break
    passed=len(state['actions'])==2 and all(a['passed']for a in state['actions'])
    state.update(status='PASS_ACTUAL_NATIVE_SHARED_ADDRESS_TASK_OWNER'if passed else'TERMINAL_NATIVE_SHARED_ADDRESS_TASK_OWNER_NOT_ESTABLISHED',
        finished=time.time(),native_selected=False,physical_timing_qualified=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','invariant','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.invariant.resolve(),a.output.resolve(),a.yosys.resolve()))
