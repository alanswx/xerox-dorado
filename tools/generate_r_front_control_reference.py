#!/usr/bin/env python3
"""Join original processor/R storage, ContA/ContB and MemC Hold circuits.

Keep the 798-Q processor map and append original ContB/MC state at843:798.
One original BaseBd j02/l01/k01 fanout supplies all five boards. Include
ProcH e19's real Holda path; PRhold, CA Hold and CBHold use distinct outputs.
IM/parity, MCR/hold data, ALU and B-bus remain explicit producer boundaries.
"""
import argparse
import json
from pathlib import Path

from generate_r_front_l17_reference import generate as processor_generate
from generate_contb_hold_reference import generate as memory_generate
from generate_control_hold_reference import SEAMS
from generate_conta_mir_reference import FLAGS as MIR_FLAGS
from generate_conta_local_reference import LOCAL_FLAGS
from generate_processor_capture_reference import graph,ROOT


def generate(root,out):
    out.mkdir(parents=True,exist_ok=False)
    parent=processor_generate(root,out/'processor-source-audit')
    memory=memory_generate(root,out/'memory-source-audit')
    ca_old,h_old,l_old=parent['boards']
    ca_spec=dict(bn=ca_old['board'],comb=sorted(set(ca_old['packages'])-set(ca_old['sequential'])),
                 seq=ca_old['sequential'],module='r_front_control_conta',bindings=ca_old['boundary_bindings'],
                 clock_taps=ca_old['clock_taps'],flag_taps=ca_old['flag_taps']+MIR_FLAGS+LOCAL_FLAGS+SEAMS)
    ca=graph(root,out=out,**ca_spec)
    p=out/'r_front_control_conta.sv';raw=p.read_text().replace('input wire [27:0] pins',
        'input wire [325:0] pins,input wire [3:0] next_raw,input wire priority_n,freeze,rb_forward,rb_forward_n,input wire cb_reset')
    for n in ['a09','b09','c09','d09']:raw=raw.replace('original_F10000 '+n+'(','original_F10000_ready '+n+'(')
    reset=next(line.split()[1].removesuffix(';')for line in raw.splitlines()if line.endswith('; // rMIRa'))
    old=' assign '+reset+'='+reset+'_j02_14;';assert raw.count(old)==1
    raw=raw.replace(old,old[:-1]+' | cb_reset;');p.write_text(raw)

    h_spec=dict(bn=h_old['board'],comb=sorted((set(h_old['packages'])-set(h_old['sequential']))|{'e19'}),
                seq=h_old['sequential'],module='r_front_control_proch',bindings={},
                clock_taps=h_old['clock_taps'],flag_taps=h_old['flag_taps'])
    external=set(graph(root,out=None,inspect=True,**h_spec))
    removed=set(h_old['boundary_bindings'])-external
    assert removed=={'Holda'},removed
    extras={n:398+i for i,n in enumerate(sorted(external-set(h_old['boundary_bindings'])))}
    assert set(extras)=={"MemBase.2'","MemBase.3'",'alu.00'},extras
    width=398+len(extras)
    h_spec['bindings']={n:h_old['boundary_bindings'][n]if n in h_old['boundary_bindings']else f'pins[{extras[n]}]'for n in external}
    h=graph(root,out=out,**h_spec)
    p=out/'r_front_control_proch.sv';p.write_text(p.read_text().replace('input wire [27:0] pins',
        f'input wire [{width-1}:0] pins,input wire [3:0] next_raw,input wire priority_n,freeze,rb_forward,rb_forward_n'))
    (out/'r_front_ready_procl.sv').write_bytes((out/'processor-source-audit/r_front_ready_procl.sv').read_bytes())
    for p in(out/'memory-source-audit').glob('*.sv'):
        raw=p.read_text()
        if p.name=='contb_hold_reference.sv':
            old='output wire [33:0] flags);';assert raw.count(old)==1
            raw=raw.replace(old,'output wire [33:0] flags,output wire ca_root,output wire [1:0] processor_roots);')
            old='.p4(cb_root));';assert raw.count(old)==1
            raw=raw.replace(old,'.p4(cb_root),.p2(ca_root),.p12(processor_roots[1]),.p13(processor_roots[0]));')
        (out/p.name).write_text(raw)

    raw=(out/'processor-source-audit/r_front_local_reference.sv').read_text()
    edits=[('module r_front_local_reference','module r_front_control_reference',1),('[797:0]','[843:0]',2),
           ('input wire [325:0] pins',f'input wire [{width-1}:0] pins',1),('[68:0] clocks','[99:0] clocks',1),
           ('output wire [29:0] ready_flags);','output wire [29:0] ready_flags,output wire [93:0] control_flags);',1),
           ('wire [69:0] fc','wire [129:0] fc',1),('reg [245:0] cpu_pins',f'reg [{width-1}:0] cpu_pins',1),
           ('cpu_pins=pins[245:0];','cpu_pins=ap;',1),
           ('r_front_l17_conta c(','r_front_control_conta c(',1),
           ('   pins,next_raw,priority_n,freeze,rb_forward,rb_forward_n,',
            '   ap[325:0],next_raw,priority_n,freeze,rb_forward,rb_forward_n,m.b.h03.p2,',1),
           ('r_front_ready_proch h(','r_front_control_proch h(',1),
           ('en_l,cpu_pins,','en_l,cpu_pins[245:0],',1),
           (' assign clocks={cc[24],',' assign clocks={memory_clocks,cc[24],',1)]
    for old,new,count in edits:
        assert raw.count(old)==count,(old,raw.count(old));raw=raw.replace(old,new)
    start=raw.index(' cell_MC10210 base_fanout(');end=raw.index(';',start)+1
    raw=raw[:start]+''' wire ca_root;wire [1:0] processor_roots;
 assign bases={ca_root,processor_roots};
'''+raw[end:]
    marker=' always @* begin\n  cpu_pins=';assert raw.count(marker)==1
    bridge=f''' reg [{width-1}:0] ap;reg [71:0] bp;wire [33:0] bf;wire [30:0] memory_clocks;
 always @*begin
  ap=pins;ap[68]=bf[28];ap[69]=bf[26];ap[294]=bf[0];ap[293]=bf[20];
  ap[202]=bf[1];ap[203]=bf[2];ap[204]=bf[3];
  ap[205]=bf[4];ap[206]=bf[5];ap[207]=bf[6];
  ap[11]=bf[7];ap[10]=bf[8];ap[9]=bf[9];
  bp=pins[397:326];bp[0]=base;bp[1]=fc[121];bp[2]=fc[128];bp[4]=fc[129];bp[5]=fc[125];bp[6]=fc[127];
  bp[56]=fc[123];bp[57]=fc[122];bp[58]=fc[124];
  bp[10]=pins[291];bp[11]=pins[290];bp[12]=pins[289];bp[13]=pins[292];
  bp[15]=pins[282];bp[16]=pins[283];bp[17]=pins[284];
 end
 contb_hold_reference m(fixture_load,fixture_state[843:798],bp,state_q[843:798],memory_clocks,bf,ca_root,processor_roots);
 assign control_flags={{bf,fc[129:70]}};
'''
    raw=raw.replace(marker,bridge+marker)
    (out/'r_front_control_reference.sv').write_text(raw)
    result=dict(scope=__doc__,boards=[ca,h,l_old,*memory['boards']],physical_bits=844,ram_bits=9216,
                pin_count=width,clock_channels=100,control_flags=94,extra_H_boundaries=extras,
                state_map=dict(processor='797:0',ContB='837:798',MemC='843:838'),
                CA_flag_seams=dict(zip(SEAMS,range(121,130))),
                original_clock_source=memory['clock_source'],native_integrated=False,physical_timing_qualified=False)
    assert sum(b['physical_bits']for b in result['boards'])==844
    (out/'manifest.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();r=generate(ROOT,a.output.resolve());print('R_FRONT_CONTROL_REFERENCE',r['physical_bits'],r['clock_channels'],r['extra_H_boundaries'])
