#!/usr/bin/env python3
"""Qualify parallel Ready comparisons on native single and paired paths.

The child retains its exact original Ready update and every old output. Add
only a comparison target/result, and use that equivalent result in the existing
masked shared-state check. Every public prefix output is compared for arbitrary
inputs, including invalid cache payloads, pair mode, memory data and flags.
No state, clocks, barriers, original comparisons or input assumptions are removed.
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
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
PREFIX='dorado_r_front_prefix_step';CHILD='dorado_local_r_front_ready_step'
SINGLE='dorado_r_front_parallel_fault_step';FAULT='dorado_parallel_r_fault_step'
CONTROL='src/verilog/rtl/dorado_r_front_control_step.sv'
READY='src/verilog/rtl/dorado_local_r_front_ready_step.sv'
HELPER='verilog/rtl/dorado_ready_compare_step.sv'


def child_transform(raw):
    token='output wire [15:0] file_write_data,input wire [864:0] shared_capture);'
    replacement='output wire [15:0] file_write_data,input wire [864:0] shared_capture,\n input wire [15:0] ready_compare_target,output wire ready_compare_mismatch);'
    assert raw.count(token)==1
    extra=''' // Compare the actual original capture, retaining it for X/Z fallback.
 dorado_ready_compare_step ready_comparison(state_q[751:736],ready_compare_target,
  state_next[751:736],capture_data,clock_events[20],ready_compare_mismatch);
'''
    after=raw.replace(token,replacement,1).replace('endmodule',extra+'endmodule',1)
    assert after.replace(replacement,token,1).replace(extra,'',1)==raw
    return after


def control_transform(raw,native_single=False):
    result=raw;calls=re.findall(r' dorado_local_r_front_ready_step child\(.*?shared_capture\);',raw,re.S)
    assert len(calls)==4
    for call in calls:
        result=result.replace(call,call[:-2]+",16'b0,);",1)
    # The named early-fault consumers do not use this added comparison.
    for name in ['early_zero','early_one']:
        pattern=r'  dorado_local_r_front_ready_step '+name+r'\(.*?\);'
        found=re.findall(pattern,result,re.S);assert len(found)==1
        replacement=found[0].replace('.shared_capture(shared_capture)',".ready_compare_target(16'b0),.ready_compare_mismatch(),\n  .shared_capture(shared_capture)",1)
        assert replacement!=found[0];result=result.replace(found[0],replacement,1)
    for name in [PREFIX,*([SINGLE]if native_single else[])]:
        before=module(result,name);after=before
        call=re.findall(r' dorado_local_r_front_ready_step child\(.*?shared_capture,16\'b0,\);',after,re.S)
        assert len(call)==1
        changed=' wire child_ready_comparison;\n'+call[0].replace("shared_capture,16'b0,);",'shared_capture,conta_next[63:48],child_ready_comparison);',1)
        after=after.replace(call[0],changed,1)
        old="assign shared_state_mismatch=|(({child_next[751:736],child_next[79:64],child_next[31:0]}^conta_next[63:0]) & 64'hffffffffffff3fff);"
        new="assign shared_state_mismatch=|(({child_next[79:64],child_next[31:0]}^conta_next[47:0]) & 48'hffffffff3fff) | child_ready_comparison;"
        assert after.count(old)==1;after=after.replace(old,new,1)
        assert after.replace(changed,call[0],1).replace(new,old,1)==before
        result=result.replace(before,after,1)
    return result


def ports(raw):
    header=raw.split(')(\n',1)[1].split(');',1)[0];rows=[];direction=None;width=1
    for item in header.split(','):
        item=re.sub(r'//[^\n]*','',item).strip()
        m=re.fullmatch(r'(input|output)\s+(?:wire|reg)\s*(?:\[(\d+):(\d+)\])?\s*(\w+)',item)
        if m:direction=m[1];width=int(m[2])-int(m[3])+1 if m[2]else 1;name=m[4]
        else:assert direction and re.fullmatch(r'\w+',item),item;name=item
        rows.append((direction,width,name))
    return rows


def run(stage,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    control=(stage/CONTROL).read_text();child=(stage/READY).read_text()
    transformed=control_transform(control,True)
    (out/'native-control-before.sv').write_text(control);(out/'native-control-after.sv').write_text(transformed)
    (out/'native-ready-before.sv').write_text(child);(out/'native-ready-after.sv').write_text(child_transform(child))
    shutil.copy2(ROOT/HELPER,out/Path(HELPER).name);shutil.copy2(Path(__file__),out/Path(__file__).name)
    bank={};owners={}
    for n in m['source_hashes']:
        if not n.startswith('src/verilog/rtl/')or Path(n).suffix not in ['.v','.sv']:continue
        for f in re.finditer(r'^module\s+(\w+)\b.*?^endmodule',(stage/n).read_text(),re.M|re.S):
            assert f[1]not in bank;bank[f[1]]=f[0]+'\n';owners[f[1]]=n
    original_prefix=bank[PREFIX];original_single=bank[SINGLE];original_child=bank[CHILD]
    packet=module(control,'dorado_r_front_packet_step')
    assert re.search(r'\b'+SINGLE+r'\s+single\(',packet)
    bank['reference_prefix']=original_prefix.replace('module '+PREFIX+' #','module reference_prefix #',1).replace(CHILD+' child(', 'reference_ready_child child(',1)
    bank['reference_single']=original_single.replace('module '+SINGLE+' #','module reference_single #',1).replace(CHILD+' child(','reference_ready_child child(',1).replace(FAULT+' early_fault(','reference_early_fault early_fault(',1)
    bank['reference_early_fault']=bank[FAULT].replace('module '+FAULT+'(','module reference_early_fault(',1).replace(CHILD+' early_', 'reference_ready_child early_')
    bank['reference_ready_child']=original_child.replace('module '+CHILD+'(','module reference_ready_child(',1)
    for name in [PREFIX,SINGLE,FAULT]:bank[name]=module(transformed,name)
    bank[CHILD]=module(child_transform(child),CHILD)
    bank['dorado_ready_compare_step']=(ROOT/HELPER).read_text()
    chosen=set()
    def collect(n):
        if n in chosen:return
        chosen.add(n)
        for other in re.findall(r'^\s*(\w+)\s+(?:#\([\s\S]*?\)\s+)?\w+\s*\(',bank[n],re.M):
            if other in bank:collect(other)
    for name in [PREFIX,SINGLE,'reference_prefix','reference_single']:collect(name)
    rows=ports(original_prefix);bits=sum(w for d,w,n in rows if d=='input');outputs=sum(w for d,w,n in rows if d=='output')
    declarations=[];args=[[],[]];ino=outo=0
    for direction,width,name in rows:
        if direction=='input':
            declarations.append(f' wire [{width-1}:0] {name}=stimulus[{ino}+:{width}];');ino+=width
            for a in args:a.append('.'+name+'('+name+')')
        else:
            for i,prefix in enumerate(['expected','actual']):args[i].append(f'.{name}({prefix}[{outo}+:{width}])')
            outo+=width
    top=f'''module ready_compare_equivalence(input wire [{bits-1}:0] stimulus,
 output wire [{outputs-1}:0] actual,expected,output wire equivalent);
{chr(10).join(declarations)}
 reference_prefix original({','.join(args[0])});
 {PREFIX} candidate({','.join(args[1])});
 assign equivalent=actual==expected;
endmodule
'''
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),stage=str(stage),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        source_control_sha256=sha(stage/CONTROL),source_ready_sha256=sha(stage/READY),helper_sha256=sha(ROOT/HELPER),
        arbitrary_binary_input_bits=bits,compared_outputs=outputs,assumptions=0,ports=rows,
        all_original_prefix_outputs_and_ready_update_retained=True,
        actual_native_single_path_qualified=True,native_single_type=SINGLE,
        proof_families={'single':dict(case='single-complete-interface',input_bits=bits,output_bits=outputs),
                        'prefix':dict(case='prefix-complete-interface',input_bits=bits,output_bits=outputs)},
        source_hashes={owners[n]:sha(stage/owners[n])for n in chosen if n in owners},
        files={p.name:sha(p)for p in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,bad,family in [('single-complete-interface',False,'single'),('prefix-complete-interface',False,'prefix'),('wrong-preempt-selector',True,'single')]:
        d=out/label;d.mkdir()
        for n in sorted(chosen):
            raw=bank[n]
            if bad and n=='dorado_ready_compare_step':
                token='capture_mismatch[data[14:13]]';assert raw.count(token)==1
                raw=raw.replace(token,"capture_mismatch[data[14:13]^2'b01]",1)
            (d/(n+'.sv')).write_text(raw)
        current_top=top if family=='prefix'else top.replace('reference_prefix original(', 'reference_single original(',1).replace(PREFIX+' candidate(',SINGLE+' candidate(',1)
        assert ports(original_single)==ports(original_prefix)
        (d/'top.sv').write_text(current_top)
        script=f'''read_verilog -sv -defer {' '.join('/work/'+n+'.sv'for n in sorted(chosen))} /work/top.sv
hierarchy -check -top ready_compare_equivalence
proc
flatten
opt
check -assert
sat -timeout 120 -prove equivalent 1 -{'falsify'if bad else'verify'} -show-ports -dump_json /work/counterexample.json
'''
        (d/'proof.ys').write_text(script);env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with(d/'proof.log').open('x')as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=log,env=env);a=dict(label=label,family=family,negative=bad,pid=p.pid,command=cmd,started=time.time(),files={p.name:sha(p)for p in d.iterdir()if p.name!='proof.log'})
            state['actions'].append(a);save();code=p.wait()
        marker='model found: FAIL!'if bad else'no model found: SUCCESS!'
        a.update(returncode=code,passed=code==0 and marker in(d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(label,a['passed'],flush=True)
        if not a['passed']:break
    passed=len(state['actions'])==3 and all(a['passed']for a in state['actions'])
    state.update(status='PASS_ACTUAL_NATIVE_READY_COMPARISON'if passed else'TERMINAL_NATIVE_READY_COMPARISON_NOT_ESTABLISHED',finished=time.time(),
        native_selected=False,physical_timing_or_firmware_or_speed_qualified=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.output.resolve(),a.yosys.resolve()))
