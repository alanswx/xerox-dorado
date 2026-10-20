#!/usr/bin/env python3
"""Emit the original front-end combinational wiring with sampled Q outputs.

No clocks or storage are implemented here. The new clock-step evaluator owns
all Q updates. Gate wiring/functions are deliberately shared with the original
reference; its delayed sequential primitives remain independent. Verification
must establish capture ordering/state updates, not claim independent gate logic.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

from generate_run_control_reference import model_ports


PIN_Q={
 'MC10176':(6,{2:'state_q[0]',3:'state_q[1]',4:'state_q[2]',13:'state_q[3]',14:'state_q[4]',15:'state_q[5]'}),
 'MC10231':(2,{2:'state_q[0]',3:'~state_q[0]',15:'state_q[1]',14:'~state_q[1]'}),
 'SE10231':(2,{2:'state_q[0]',3:'~state_q[0]',15:'state_q[1]',14:'~state_q[1]'}),
 'MC10141':(4,{14:'state_q[0]',15:'state_q[1]',2:'state_q[2]',3:'state_q[3]'}),
 'MC10173':(4,{1:'state_q[0]',2:'state_q[1]',15:'state_q[2]',14:'state_q[3]'}),
 'MC1668':(2,{2:'state_q[0]',3:'~state_q[0]',15:'state_q[1]',14:'~state_q[1]'}),
 'F10000':(4,{14:'state_q[3]',15:'state_q[2]',2:'state_q[1]',3:'state_q[0]'}),
 'F10145A':(64,{2:'p3 ? 1\'b0 : state_q[4*address]',1:'p3 ? 1\'b0 : state_q[4*address+1]',
               15:'p3 ? 1\'b0 : state_q[4*address+2]',14:'p3 ? 1\'b0 : state_q[4*address+3]'})}


def generate(root,reference,output):
    manifest=json.loads((reference/'manifest.json').read_text())
    assert manifest['physical_bits']==736
    chunks=['// Generated physical combinational cone; no sequential clock emulation.\n']
    kinds=set();seqkinds=set()
    for board in manifest['boards']:
        kinds.update(Path(s).stem.removeprefix('cell_')for s in board['combinational_sources'])
        seqkinds.update(board['packages'][p]for p in board['sequential'])
    kinds.add('MC10210')
    hashes={}
    for kind in sorted(kinds):
        p=root/'verilog/cells'/('cell_'+kind+'.v');raw=p.read_text();hashes[str(p.relative_to(root))]=hashlib.sha256(p.read_bytes()).hexdigest()
        assert raw.count('module cell_'+kind)==1,kind
        chunks.append(raw.replace('cell_'+kind,'comb_'+kind))
    for kind in sorted(seqkinds|{'F10000_gated'}):
        source='F10000'if kind=='F10000_gated'else kind
        width,outputs=PIN_Q[source];ports=model_ports(root,source)
        assert {n for n,d in ports.items()if d=='output'}==set(outputs),kind
        args=[direction+' wire p'+str(pin)for pin,direction in sorted(ports.items())]
        args+=['input wire fixture_load',f'input wire [{width-1}:0] fixture_state',f'output wire [{width-1}:0] state_q']
        raw='module sampled_'+kind+'('+','.join(args)+');\n assign state_q=fixture_state;\n'
        if source in ('MC10231','SE10231'):raw+=' wire ca=p9|p6,cb=p9|p11;\n'
        if source=='F10000':raw+=' wire clk=p4|p13;\n'
        if source=='F10145A':raw+=' wire [3:0] address={p10,p9,p7,p6};\n'
        for pin,expression in outputs.items():raw+=f' assign p{pin}={expression};\n'
        chunks.append('`default_nettype none\n'+raw+'endmodule\n`default_nettype wire\n')
    for name in ('r_front_conta','r_front_proch','r_front_procl'):
        raw=(reference/(name+'.sv')).read_text()
        raw=raw.replace(name,name+'_sampled').replace('original_','sampled_').replace('cell_','comb_')
        chunks.append(raw)
    raw=(reference/'r_front_reference.sv').read_text()
    raw=raw.replace('state_q','sampled_q').replace('fixture_state','state_q')
    raw=raw.replace('module r_front_reference(input wire fixture_load,input wire [735:0] state_q,',
                    'module dorado_r_front_cone(input wire [735:0] state_q,')
    raw=raw.replace('front_data);','front_data);\n wire fixture_load=0;')
    for name in ('r_front_conta','r_front_proch','r_front_procl'):raw=raw.replace(name+' ',name+'_sampled ')
    raw=raw.replace('cell_','comb_');chunks.append(raw)
    output.parent.mkdir(parents=True,exist_ok=True);assert not output.exists()
    output.write_text('\n'.join(chunks))
    record=dict(scope=__doc__,source_cells=hashes,physical_bits=736,pin_count=manifest['pin_count'],
                output_sha256=hashlib.sha256(output.read_bytes()).hexdigest(),shared_combinational_logic=True,
                storage_updates_implemented=False,native_integrated=False)
    output.with_suffix('.json').write_text(json.dumps(record,indent=2)+'\n')
    print('R_FRONT_CONE',len(kinds),'combinational part types;',output.stat().st_size,'source bytes')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,default=Path(__file__).resolve().parents[1])
    p.add_argument('--reference',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();generate(a.root.resolve(),a.reference.resolve(),a.output.resolve())
