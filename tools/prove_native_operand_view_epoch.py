#!/usr/bin/env python3
"""Find the actual native operand view's pre-event preparation boundary.

Compare the physical844-bit view from two complete native single functions.
Only held state, before-root/pins and original SRAM word are shared. All late
root/pins/control-prefix/interior-read/host-clock inputs vary independently.
Native binds advance=0 and pair_fall=0; these are actual wiring, not a premise.
This establishes data-source chronology, not safe register integration alone.
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
from qualify_native_ready_compare import SINGLE,CONTROL,ports
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

SHARED={'held_q','clock_before','pins_before','external_file_read'}


def run(stage,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    bank={};owners={}
    for n in m['source_hashes']:
        if not n.startswith('src/verilog/rtl/')or Path(n).suffix not in ['.v','.sv']:continue
        for match in re.finditer(r'^module\s+(\w+)\b.*?^endmodule',(stage/n).read_text(),re.M|re.S):
            assert match[1]not in bank;bank[match[1]]=match[0]+'\n';owners[match[1]]=n
    chosen=set()
    def collect(n):
        if n in chosen:return
        chosen.add(n)
        for other in re.findall(r'^\s*(\w+)\s+(?:#\([\s\S]*?\)\s+)?\w+\s*\(',bank[n],re.M):
            if other in bank:collect(other)
    collect(SINGLE)
    rows=ports(bank[SINGLE]);inputs=[(w,n)for d,w,n in rows if d=='input']
    assert SHARED <= {n for _,n in inputs}
    save_files={n:sha(stage/n)for n in {owners[n]for n in chosen}}
    state=dict(status='RUNNING',started=time.time(),scope=__doc__,stage=str(stage),
        stage_manifest_sha256=sha(stage/'r-front-control-native.json'),source_control_sha256=sha(stage/CONTROL),
        source_hashes=save_files,shared_inputs=sorted(SHARED),native_constant_inputs={'advance':0,'pair_fall':0},
        host_clock_varies_independently=True,
        compared_physical_view_bits=844,assumptions=0,actions=[])
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,negative in [('pre-event-view',False),('wrong-original-read',True)]:
        d=out/label;d.mkdir();decl=[];args=[[],[]];offset=0
        for width,name in inputs:
            if name in ['advance','pair_fall']:values=["1'b0"]*2
            elif name in SHARED:
                decl.append(f' wire [{width-1}:0] {name}=stimulus[{offset}+:{width}];');offset+=width
                values=[name,name]
                if negative and name=='external_file_read':values[1]=f'({name} ^ 18\'h1)'
            else:
                values=[]
                for i in range(2):
                    decl.append(f' wire [{width-1}:0] {name}_{i}=stimulus[{offset}+:{width}];');offset+=width
                    values.append(name+'_'+str(i))
            for i in range(2):args[i].append(f'.{name}({values[i]})')
        for i in range(2):args[i].append('.physical_state('+['expected','actual'][i]+')')
        top=f'''module operand_epoch_contract(input wire [{offset-1}:0] stimulus,
 output wire [843:0] actual,expected,output wire equivalent);
{chr(10).join(decl)}
 {SINGLE} first({','.join(args[0])});
 {SINGLE} second({','.join(args[1])});
 assign equivalent=actual==expected;
endmodule
'''
        for n in chosen:(d/(n+'.sv')).write_text(bank[n])
        (d/'top.sv').write_text(top)
        script=f'''read_verilog -sv -defer {' '.join('/work/'+n+'.sv'for n in sorted(chosen))} /work/top.sv
hierarchy -check -top operand_epoch_contract
proc
flatten
opt
check -assert
sat -timeout 120 -prove equivalent 1 -{'falsify'if negative else'verify'} -show-ports -dump_json /work/counterexample.json
'''
        (d/'proof.ys').write_text(script);env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with(d/'proof.log').open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log,env=env)
            a=dict(label=label,negative=negative,pid=child.pid,command=cmd,started=time.time(),arbitrary_binary_input_bits=offset,
                   files={p.name:sha(p)for p in d.iterdir()if p.name!='proof.log'})
            state['actions'].append(a);save();code=child.wait()
        marker='model found: FAIL!'if negative else'no model found: SUCCESS!'
        a.update(returncode=code,passed=code==0 and marker in(d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(label,a['passed'],flush=True)
        if not a['passed']:break
    passed=len(state['actions'])==2 and all(a['passed']for a in state['actions'])
    state.update(status='PASS_ACTUAL_NATIVE_PRE_EVENT_OPERAND_VIEW'if passed else'TERMINAL_NATIVE_OPERAND_VIEW_EPOCH_NOT_ESTABLISHED',finished=time.time(),
        register_integration_or60ns_or_firmware_qualified=False)
    state['files']={p.name:sha(p)for p in out.iterdir()if p.is_file()and p.name!='run-state.json'}
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.output.resolve(),a.yosys.resolve()))
