#!/usr/bin/env python3
"""Diagnose the measured native prefix Ready-state comparison, without edits.

Extract the actual combinational prefix caller and its dependency graph.
Every original input, including arbitrary cached prefix payload, is free.
Test whether the shared capture contract alone implies the masked comparison
is zero. A counterexample prohibits replacing the comparison unconditionally;
neither outcome changes a native guard, clock, RAM, or hardware source.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

NAME='dorado_r_front_prefix_step'
SOURCE='src/verilog/rtl/dorado_r_front_control_step.sv'


def run(stage,out,yosys,computed_prefix=False):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    bank={};owners={}
    for n in m['source_hashes']:
        if not n.startswith('src/verilog/rtl/')or Path(n).suffix not in ['.v','.sv']:continue
        for match in re.finditer(r'^module\s+(\w+)\b.*?^endmodule', (stage/n).read_text(),re.M|re.S):
            name=match[1];assert name not in bank,name;bank[name]=match[0]+'\n';owners[name]=n
    chosen=set()
    def collect(name):
        if name in chosen:return
        chosen.add(name)
        for other in re.findall(r'^\s*(\w+)\s+(?:#\([\s\S]*?\)\s+)?\w+\s*\(',bank[name],re.M):
            if other in bank:collect(other)
    collect(NAME)
    original=bank[NAME]
    token='input wire pair_fall,output wire [35:0] native_R_frame\n);'
    observed='input wire pair_fall,output wire [35:0] native_R_frame,\n output wire shared_context,output wire [63:0] child_control_projection\n);'
    assert original.count(token)==1
    extra=' assign shared_context=shared_capture[864];\n assign child_control_projection={child_next[751:736],child_next[79:64],child_next[31:0]};\n'
    bank[NAME]=original.replace(token,observed,1).replace('endmodule',extra+'endmodule',1)
    assert bank[NAME].replace(observed,token,1).replace(extra,'',1)==original
    sources=[]
    for name in sorted(chosen):
        assert not re.search(r'always\s*@\(\s*(?:pos|neg)edge',bank[name]),name
        p=out/(name+'.sv');p.write_text(bank[name]);sources.append('/work/'+p.name)
    header=original.split(')(\n',1)[1].split(');',1)[0];ports=[];direction=None;width=1
    for field in header.split(','):
        field=re.sub(r'//[^\n]*','',field).strip()
        match=re.fullmatch(r'(input|output)\s+(?:wire|reg)\s*(?:\[(\d+):(\d+)\])?\s*(\w+)',field)
        if match:direction=match[1];width=int(match[2])-int(match[3])+1 if match[2]else 1;name=match[4]
        else:assert direction and re.fullmatch(r'\w+',field),field;name=field
        ports.append((direction,width,name))
    bits=sum(w for d,w,n in ports if d=='input');offset=0;decl=[];connections=[]
    for direction,width,name in ports:
        if direction=='input':
            decl.append(f' wire [{width-1}:0] {name}=stimulus[{offset}+:{width}];');offset+=width
        else:decl.append(f' wire [{width-1}:0] {name};')
        connections.append('.'+name+'('+name+')')
    connections+=['.shared_context(shared_context)','.child_control_projection(child_control_projection)']
    cache_contract=''
    if computed_prefix:
        # Substitute the cache's proved payload function, not arbitrary saved
        # bits. The raw prefix output is independent of these cache inputs.
        cache_contract=''' wire split_prefix=(clock_before!=clock_after) && (pins_before[292:282]!=pins_after[292:282]);
 wire valid_fast=entry_fast && !split_prefix && (live_prefix=={control_owner,113'b0});
'''
        substitutions={
            '.entry_fast(entry_fast)':'.entry_fast(valid_fast)',
            '.entry_prefix(entry_prefix)':'.entry_prefix(live_prefix)',
            '.entry_prefix_cp(entry_prefix_cp)':'.entry_prefix_cp(split_prefix ? pins_after[292:282]:pins_before[292:282])',
            '.entry_after_cp(entry_after_cp)':'.entry_after_cp(pins_after[292:282])'}
        connections=[substitutions.get(c,c)for c in connections]
    top=f'''module native_shared_ready_contract(input wire [{bits-1}:0] stimulus,
 output wire redundant,shared_context,output wire [63:0] child_control_projection,
 output wire [63:0] joint_control_projection,output wire mismatch);
{chr(10).join(decl)}
{cache_contract}
 {NAME} dut({','.join(connections)});
 assign joint_control_projection=conta_next[63:0];assign mismatch=shared_state_mismatch;
 assign redundant=!shared_context || !shared_state_mismatch;
endmodule
'''
    (out/'top.sv').write_text(top);shutil.copy2(Path(__file__),out/Path(__file__).name)
    script=f'''read_verilog -sv -defer {' '.join(sources)} /work/top.sv
hierarchy -check -top native_shared_ready_contract
proc
flatten
opt
check -assert
sat -timeout 120 -prove redundant 1 -show-ports -dump_json /work/counterexample.json
'''
    (out/'proof.ys').write_text(script)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),stage=str(stage),
        stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        source_hashes={owners[n]:sha(stage/owners[n])for n in chosen},selected_modules=sorted(chosen),
        arbitrary_binary_input_bits=bits,assumptions=0,original_native_comparison_retained=True,
        cache_payload_replaced_by_actual_prefix_function=computed_prefix,
        free_saved_cache_payload_bits_unused_in_computed_mode=291 if computed_prefix else 0,
        files={p.name:sha(p)for p in out.iterdir()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
    cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
    with(out/'proof.log').open('x')as log:
        p=subprocess.Popen(cmd,stdout=log,stderr=log,env=env);state.update(pid=p.pid,command=cmd);save();code=p.wait()
    log=(out/'proof.log').read_text();proved=code==0 and 'no model found: SUCCESS!'in log
    counterexample=code==0 and 'model found: FAIL!'in log
    state.update(status='PROVED_NATIVE_SHARED_READY_COMPARISON_REDUNDANT'if proved else
        'COUNTEREXAMPLE_TO_NATIVE_SHARED_READY_REDUNDANCY'if counterexample else'TERMINAL_NATIVE_SHARED_READY_NOT_ESTABLISHED',
        returncode=code,proved=proved,counterexample=counterexample,finished=time.time(),log_sha256=sha(out/'proof.log'),
        native_changed=False,firmware_or_timing_or_speed_qualified=False)
    save();print(state['status'],flush=True);return 0 if proved or counterexample else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--computed-prefix',action='store_true')
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.output.resolve(),a.yosys.resolve(),a.computed_prefix))
