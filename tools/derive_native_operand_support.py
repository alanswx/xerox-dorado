#!/usr/bin/env python3
"""Derive a conservative bit support for the actual native88-bit operand view.

Start from the source-bound epoch cofactor and traverse its optimized Yosys
netlist. Bitwise gates/muxes retain bit precision; other combinational cells
use all input bits conservatively. This is a proposed dependency mask, not a
theorem: the buffered native-function proof must pass with this mask afterward.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from qualify_native_operand_buffer import instance
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def support(module):
    drivers={}
    for name,c in module['cells'].items():
        for port,direction in c['port_directions'].items():
            if direction=='output':
                for index,bit in enumerate(c['connections'][port]):
                    if isinstance(bit,int):
                        assert bit not in drivers,(bit,name)
                        drivers[bit]=(c,port,index)
    found=set();seen=set();inputs={b:i for i,b in enumerate(module['ports']['descriptor']['bits'])}
    def visit(bit):
        if not isinstance(bit,int)or bit in seen:return
        seen.add(bit)
        if bit in inputs:found.add(inputs[bit]);return
        if bit not in drivers:return
        c,port,index=drivers[bit];kind=c['type'];ports=c['connections']
        assert not any(x in kind.lower()for x in ['dff','latch','mem']),kind
        if kind in ['$and','$or','$xor','$xnor','$not','$pos']:
            selected=[]
            for p in ['A','B']:
                if p in ports:
                    bits=ports[p]
                    if index<len(bits):selected.append(bits[index])
                    elif int(c['parameters'].get(p+'_SIGNED','0'),2):selected.append(bits[-1])
        elif kind=='$mux':selected=[ports['A'][index],ports['B'][index],*ports['S']]
        elif kind=='$pmux':
            width=len(ports['Y']);selected=[ports['A'][index],*ports['B'][index::width],*ports['S']]
        else:selected=[b for p,d in c['port_directions'].items()if d=='input'for b in ports[p]]
        for b in selected:visit(b)
    for b in module['ports']['fields']['bits']:visit(b)
    return found,len(seen)


def run(epoch,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    e=json.loads((epoch/'run-state.json').read_text());verify(epoch,e['files'])
    assert e['status']=='PASS_ACTUAL_NATIVE_PRE_EVENT_OPERAND_VIEW'and e['host_clock_varies_independently']
    for f in(epoch/'pre-event-view').glob('*.sv'):
        if f.name!='top.sv':shutil.copy2(f,out/f.name)
    top='''module operand_support(input wire [1263:0] descriptor,output wire [87:0] fields);
 wire [843:0] raw;
 INSTANCE
 assign fields={raw[595:588],raw[571:540],raw[403:372],raw[115:100]};
endmodule
'''.replace('INSTANCE',instance('actual','descriptor[1263:420]','descriptor[419]','descriptor[418:18]','descriptor[17:0]','raw'))
    (out/'top.sv').write_text(top);shutil.copy2(Path(__file__),out/Path(__file__).name)
    script=f'''read_verilog -sv -defer {' '.join('/work/'+f.name for f in sorted(out.glob('*.sv')))}
hierarchy -check -top operand_support
proc
flatten
opt
check -assert
write_json /work/netlist.json
'''
    (out/'support.ys').write_text(script)
    env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
    cmd=[str(yosys),'-Q','-T','-s','/work/support.ys']
    state=dict(status='RUNNING',started=time.time(),epoch_state_sha256=sha(epoch/'run-state.json'),command=cmd)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    with(out/'support.log').open('x')as log:
        p=subprocess.Popen(cmd,stdout=log,stderr=log,env=env);state['pid']=p.pid;save();code=p.wait()
    state.update(returncode=code,finished=time.time());save();assert code==0
    mod=json.loads((out/'netlist.json').read_text())['modules']['operand_support']
    bits,nodes=support(mod);mask=sum(1<<b for b in bits)
    state.update(status='DERIVED_NATIVE_OPERAND_SUPPORT_REQUIRES_PROOF',support_bits=len(bits),visited_nodes=nodes,
        descriptor_bits=1264,mask_hex=f'{mask:x}',combinational_cells=len(mod['cells']),
        files={p.name:sha(p)for p in out.iterdir()if p.name!='run-state.json'})
    save();print(state['status'],len(bits),'of1264 tag bits',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['epoch','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();run(a.epoch.resolve(),a.output.resolve(),a.yosys.resolve())
