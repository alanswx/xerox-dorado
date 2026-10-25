#!/usr/bin/env python3
"""Extract ProcH's original branch-condition file, latch and shared TIOA stages.

HM §3 ALUF/ALU Operations and ProcH sheet11 keep visible flags, their per-task
save file and g18's arithmetic copies distinct. Preserve all80 package bits.
ALU results, existing g18/CurrLast and phase-enable producers stay explicit
physical boundaries; this extraction does not qualify their native arrivals.
"""
import argparse
import json
from pathlib import Path
import re

from generate_processor_capture_reference import graph,ROOT
from generate_r_front_cone import PIN_Q
from generate_run_control_reference import model_ports
from sil_netlist import load_board
from test_external_r_packet import sha

SEQ={'e13':(0,64),'f17':(64,4),'g11':(68,6),'h11':(74,6)}
COMB=['l01','f13','g13','c18','d18','i18','e14','e19','f19','g20']


def generate(root,out,gate=1750,ff=400,latch=1000):
    assert not out.exists();out.mkdir(parents=True)
    spec=dict(bn='ProcH-Rev-Ce',seq=SEQ,comb=COMB,module='branch_condition_reference',
              bindings={},clock_taps=['@g11.p9','@h11.p9','@f17.p7','@e13.p13'],
              flag_taps=["ResEqZero'","ResLtZero'",'ALUCarry',"Overflow'"])
    external=graph(root,out=None,inspect=True,**spec)
    # These two pins are complementary outputs of the same FF decoder bit.
    names=sorted(set(external)-{"CLK.ph'","FF.3'a"})
    pins={name:i for i,name in enumerate(names)}
    spec['bindings']={name:('base' if name=="CLK.ph'" else
                           '~pins['+str(pins['FF.3a'])+']' if name=="FF.3'a" else
                           'pins['+str(pins[name])+']') for name in external}
    board=load_board(str(root/'chm/sil/ProcH-Rev-Ce.dm!1_/ProcH-Rev-Ce.wl'))
    selected=set(SEQ)|set(COMB)
    for net,row in board.nets.items():
        drivers={p['pkg'] for p in row['pins'] if p['dir']=='out'}
        if any(p['pkg'] in selected and p['dir']=='in' for p in row['pins']) and drivers&selected:
            assert drivers<=selected,(net,drivers-selected)
    record=graph(root,out=out,**spec)
    path=out/'branch_condition_reference.sv';raw=path.read_text()
    assert raw.count('input wire [27:0] pins')==1
    raw=raw.replace('input wire [27:0] pins',f'input wire [{len(pins)-1}:0] pins',1)
    assert raw.count('output wire [3:0] flags);')==1
    raw=raw.replace('output wire [3:0] flags);','output wire [3:0] flags,output wire [23:0] capture_data);',1)
    extra=''' assign capture_data[5:0]={g11.p12,g11.p11,g11.p10,g11.p7,g11.p6,g11.p5};
 assign capture_data[11:6]={h11.p12,h11.p11,h11.p10,h11.p7,h11.p6,h11.p5};
 assign capture_data[15:12]=f17.p9 ? {f17.p11,f17.p13,f17.p4,f17.p6}:{f17.p10,f17.p12,f17.p3,f17.p5};
 assign capture_data[19:16]={e13.p10,e13.p9,e13.p7,e13.p6};
 assign capture_data[23:20]={e13.p12,e13.p11,e13.p4,e13.p5};
'''
    raw=raw.replace('endmodule\n',extra+'endmodule\n',1);path.write_text(raw)
    combinational=set(record['packages'][p] for p in COMB)
    cone=[];source_hashes={};timed=out/'timed';timed.mkdir()
    for kind in sorted(combinational):
        path=root/'verilog/cells'/('cell_'+kind+'.v');text=path.read_text()
        source_hashes[str(path.relative_to(root))]=sha(path)
        cone.append(text.replace('cell_'+kind,'branch_comb_'+kind))
        delayed=re.sub(r'(?m)^(\s*assign\s+)',r'\g<1>#('+str(gate)+') ',text)
        (timed/path.name).write_text('`timescale 1ps/1ps\n'+delayed)
    for kind in sorted({record['packages'][p] for p in SEQ}):
        width,outputs=PIN_Q[kind];ports=model_ports(root,kind)
        assert {n for n,d in ports.items() if d=='output'}==set(outputs)
        args=[direction+' wire p'+str(pin) for pin,direction in sorted(ports.items())]
        args+=['input wire fixture_load',f'input wire [{width-1}:0] fixture_state',f'output wire [{width-1}:0] state_q']
        sampled='module branch_sampled_'+kind+'('+','.join(args)+');\n assign state_q=fixture_state;\n'
        if kind=='F10145A':sampled+=' wire [3:0] address={p10,p9,p7,p6};\n'
        for pin,expression in outputs.items():sampled+=f' assign p{pin}={expression};\n'
        cone.append(sampled+'endmodule\n')
    # Keep hierarchical package-pin observations in the timed oracle. The
    # synthesizable cone must use the exact connected local wires: Yosys
    # treats hierarchical input-port reads as undriven implicit identifiers.
    # Reject missing bindings; never replace them with constants or waive check.
    connections={}
    for package in SEQ:
        instance=re.findall(r' original_\w+ '+package+r'\((.*?)\);',raw)
        assert len(instance)==1,package
        for pin,wire in re.findall(r'\.p(\d+)\(([^()]*)\)',instance[0]):
            connections[package+'.p'+pin]=wire
    def lower(text):
        return re.sub(r'\b(?:e13|f17|g11|h11)\.p\d+\b',lambda m:connections[m[0]],text)
    cone_raw=lower(raw)
    cone.append(cone_raw.replace('branch_condition_reference','branch_condition_cone')
                .replace('original_','branch_sampled_').replace('cell_','branch_comb_'))
    (out/'cone.sv').write_text('\n'.join(cone))
    oracle=root/'build/rtl-executive/j19-full-icarus-v2-20261025/timed'
    for name in ['run_control_original_ff.sv','t_file_original_storage.sv']:
        path=oracle/name;text=path.read_text();source_hashes[str(path.relative_to(root))]=sha(path)
        if name.startswith('run_control'):
            # Keep the independently timed original primitive. Only its
            # explicit diagnostic delay changes between declared profiles.
            text=re.sub(r'<=#\(\d+\)', '<=#('+str(ff)+')',text)
        else:
            assert 'DELAY_PS=1000' in text
            text=text.replace('DELAY_PS=1000','DELAY_PS='+str(latch))
        (timed/name).write_text(text)
    manifest=dict(scope=__doc__,physical_bits=80,per_task_RAM_bits=64,
                  packages=record,pin_index=pins,clock_channels=4,
                  data_layout=dict(g11='5:0',h11='11:6',visible_latch='15:12',
                                   file_address='19:16',file_data='23:20'),
                  compiled_profile_ps=dict(gate=gate,FF=ff,latch=latch),
                  primary_sources=source_hashes,shared_combinational_wiring=True,
                  lowered_package_pin_connections=connections,
                  visible_mux_expression=lower('f17.p9 ? {f17.p11,f17.p13,f17.p4,f17.p6}:{f17.p10,f17.p12,f17.p3,f17.p5}'),
                  independent_timed_storage=True,native_integrated=False,
                  physical_timing_qualified=False,ALU_and_phase_producer_arrivals_qualified=False)
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    return manifest


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();r=generate(ROOT,a.output.resolve());print('BRANCH_REFERENCE',r['physical_bits'],len(r['pin_index']))
