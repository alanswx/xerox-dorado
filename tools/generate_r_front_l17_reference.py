#!/usr/bin/env python3
"""Close original ContA l17 feedback with all six physical package Qs.

Add l17, j19 clock fanout, g03 feedback and e01 RBMux gates to the existing
original processor graph. B bus and unused gate-half producers remain explicit.
No candidate equations supply any original reference arrival.
"""
import argparse
import hashlib
import json
from pathlib import Path
from generate_r_front_commands_reference import generate as parent_generate
from generate_processor_capture_reference import graph,ROOT


# Nine new package-input boundaries, all kept distinct. Only BMux.04 reaches
# the l17 D2 feedback; unused gate halves still retain their original inputs.
EXTRAS={'BMux.04':317,'BMux.12':318,'BMux.13':319,'BMux.14':320,'BMux.15':321,
        'CPReg.06':322,'CPReg.07':323,'ContA17.sil+4':324,'ContA17.sil+5':325}


def generate(root,out):
    out.mkdir(parents=True,exist_ok=False);base=out/'parent-source-audit'
    manifest=parent_generate(root,base);old=manifest['boards'][0]
    spec=dict(bn=old['board'],comb=sorted((set(old['packages'])-set(old['sequential']))|{'g03','e01','j19'}),
              seq={**old['sequential'],'l17':(104,6)},module='r_front_l17_conta',bindings={},
              clock_taps=old['clock_taps']+["clk1'Da"],
              flag_taps=old['flag_taps']+['FF=UseDMD','FF=B_Dbuf','ContA28.sil+5','RBMux.04'])
    external=set(graph(root,out=None,inspect=True,**spec))
    removed={"Call13'",'ContA28.sil+2','ContA28.sil+4','ContA28.sil+7','SWm'}
    assert external==(set(old['boundary_bindings'])-removed)|set(EXTRAS)
    spec['bindings']={n:old['boundary_bindings'][n]if n in old['boundary_bindings']else f'pins[{EXTRAS[n]}]'for n in external}
    assert len(set(spec['bindings'].values()))==len(spec['bindings'])
    board=graph(root,out=out,**spec)
    p=out/'r_front_l17_conta.sv';s=p.read_text().replace('input wire [27:0] pins',
        'input wire [325:0] pins,input wire [3:0] next_raw,input wire priority_n,freeze,rb_forward,rb_forward_n')
    for n in ['a09','b09','c09','d09']:s=s.replace('original_F10000 '+n+'(','original_F10000_ready '+n+'(')
    p.write_text(s)
    for n in ['proch','procl']:(out/f'r_front_ready_{n}.sv').write_bytes((base/f'r_front_ready_{n}.sv').read_bytes())
    s=(base/'r_front_local_reference.sv').read_text()
    edits=[('[791:0]','[797:0]',2),('[316:0]','[325:0]',1),('[67:0] clocks','[68:0] clocks',1),
           ('wire [23:0] cc','wire [24:0] cc',1),('[65:0] fc','[69:0] fc',1),
           ('r_front_commands_conta c(','r_front_l17_conta c(',1),
           ('{fixture_state[791:786],','{fixture_state[797:792],fixture_state[791:786],',1),
           ('{state_q[791:786],','{state_q[797:792],state_q[791:786],',1),
           ('assign clocks={cc[23],','assign clocks={cc[24],cc[23],',1)]
    for a,b,count in edits:
        assert s.count(a)==count,(a,s.count(a));s=s.replace(a,b)
    (out/'r_front_local_reference.sv').write_text(s)
    manifest['boards'][0]=board
    manifest.update(physical_bits=798,pin_count=326,clock_channels=69,l17_internal=True,
                    extra_l17_boundaries=EXTRAS,obsolete_l17_slots=[297,299,300,308,312],
                    packages=sum(len(b['packages'])for b in manifest['boards']),
                    original_pins=sum(b['original_pins']for b in manifest['boards']),scope=__doc__,full_native_producers_closed=False)
    assert sum(b['physical_bits']for b in manifest['boards'])==798
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');return manifest


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();r=generate(ROOT,a.output.resolve());print('R_FRONT_L17_REFERENCE',r['physical_bits'],r['clock_channels'],r['extra_l17_boundaries'])
