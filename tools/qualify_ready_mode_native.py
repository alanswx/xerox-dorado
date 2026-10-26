#!/usr/bin/env python3
"""Qualify the Ready-choice comparison against every actual native output.

Inputs, invalid prefix contents, resets, pair mode and SRAM data are arbitrary.
Only the comparison computation changes; architectural captures/guards retain
their values. No input assumptions, saved state or extra host/guest edge.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from prepare_ready_mode_compare import native_transform
from prove_cp_prefix_cache import module
from qualify_native_ready_compare import CONTROL,READY,SINGLE,ports
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def run(stage,component,proof,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    c=json.loads((component/'results.json').read_text());verify(component,c['files'])
    p=json.loads((proof/'run-state.json').read_text())
    assert p['status']=='PASS_SOURCE_BOUND_READY_MODE_AND_COMPARISON'and all(a['passed']for a in p['actions'])
    assert p['source_receipt_sha256']==sha(component/'results.json')and p['source_control_sha256']==sha(stage/CONTROL)
    for a in p['actions']:
        verify(proof/a['label'],a['files']);assert a['log_sha256']==sha(proof/a['label']/'proof.log')
    bank={};owners={}
    for name in m['source_hashes']:
        if not name.startswith('src/verilog/rtl/')or Path(name).suffix not in ['.v','.sv']:continue
        for found in re.finditer(r'^module\s+(\w+)\b.*?^endmodule',(stage/name).read_text(),re.M|re.S):
            assert found[1]not in bank;bank[found[1]]=found[0]+'\n';owners[found[1]]=name
    original=bank[SINGLE];candidate=native_transform((stage/CONTROL).read_text(),(stage/READY).read_text())
    (out/'native-control-after.txt').write_text(candidate)
    bank['reference_single']=original.replace('module '+SINGLE+' #','module reference_single #',1)
    bank[SINGLE]=module(candidate,SINGLE)
    for name,file in [('dorado_ready_mode_body','mode-body.sv'),('dorado_ready_target_compare','compare.sv')]:bank[name]=module((component/file).read_text(),name)
    chosen=set()
    def collect(name):
        if name in chosen:return
        chosen.add(name)
        for other in re.findall(r'^\s*(\w+)\s+(?:#\([\s\S]*?\)\s+)?\w+\s*\(',bank[name],re.M):
            if other in bank:collect(other)
    collect(SINGLE);collect('reference_single')
    rows=ports(original);args=[[],[]];decl=[];inbits=outbits=0
    for direction,width,name in rows:
        if direction=='input':
            decl.append(f' wire [{width-1}:0] {name}=stimulus[{inbits}+:{width}];');inbits+=width
            for a in args:a.append('.'+name+'('+name+')')
        else:
            for i,prefix in enumerate(['expected','actual']):args[i].append(f'.{name}({prefix}[{outbits}+:{width}])')
            outbits+=width
    assert inbits==1982 and outbits==2970
    top=f'''module native_ready_mode_contract(input wire [{inbits-1}:0] stimulus,
 output wire [{outbits-1}:0] expected,actual,output wire equivalent);
{chr(10).join(decl)}
 reference_single original({','.join(args[0])});
 {SINGLE} candidate({','.join(args[1])});
 assign equivalent=expected==actual;
endmodule
'''
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
               component_receipt_sha256=sha(component/'results.json'),component_proof_sha256=sha(proof/'run-state.json'),
               source_control_sha256=sha(stage/CONTROL),source_ready_sha256=sha(stage/READY),
               arbitrary_binary_input_bits=inbits,compared_public_outputs=outbits,assumptions=0,extra_host_or_guest_edges=0,
               source_hashes={n:sha(stage/n)for n in {owners[x]for x in chosen if x in owners}},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,bad in [('complete-native-interface',False),('wrong-outcome-selector',True)]:
        d=out/label;d.mkdir()
        for name in chosen:
            raw=bank[name]
            if bad and name=='dorado_ready_target_compare':
                a="2'd1:mismatch=comparisons[1];";assert raw.count(a)==1
                raw=raw.replace(a,"2'd1:mismatch=comparisons[2];",1)
            (d/(name+'.sv')).write_text(raw)
        (d/'top.sv').write_text(top)
        (d/'proof.ys').write_text(f'''read_verilog -sv -defer {' '.join('/work/'+p.name for p in sorted(d.glob('*.sv')))}
hierarchy -check -top native_ready_mode_contract
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
                 files={p.name:sha(p)for p in d.iterdir()if p.name!='proof.log'});state['actions'].append(a);save();code=child.wait()
        marker='model found: FAIL!'if bad else'no model found: SUCCESS!'
        a.update(returncode=code,passed=code==0 and marker in(d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(label,a['passed'],flush=True)
        if not a['passed']:break
    passed=len(state['actions'])==2 and all(a['passed']for a in state['actions'])
    state.update(status='PASS_COMPLETE_NATIVE_READY_MODE_COMPARISON'if passed else'TERMINAL_NATIVE_READY_MODE_NOT_ESTABLISHED',finished=time.time(),
                 FPGA_timing_or_speed_or60ns_schedule_qualified=False)
    save();return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','component','proof','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.component.resolve(),a.proof.resolve(),a.output.resolve(),a.yosys.resolve()))
