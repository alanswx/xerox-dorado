#!/usr/bin/env python3
"""Close actual CP decoders in the original844-Q processor/control reference.

Keep every original processor, Ready, MIR, Hold and memory package. Add the
three actual MC10172 packages sharing h03's OR enable. The existing CPOut8 bus
wire at344 supplies both boards. All original flags and clock taps retain
their indices;18 extra passive outputs observe the now-internal MIR sets.
"""
import argparse
import json
from pathlib import Path

from generate_native_cp_source import SET_NAMES
from generate_processor_capture_reference import ROOT,graph
from generate_r_front_control_reference import generate as parent


def generate(root,out):
    r=parent(root,out);old=r['boards'][0]
    spec=dict(bn=old['board'],comb=sorted((set(old['packages'])-set(old['sequential']))|{'g01','h01','i01'}),
              seq=old['sequential'],module='r_front_control_conta',bindings={},
              clock_taps=old['clock_taps'],flag_taps=old['flag_taps']+SET_NAMES)
    external=set(graph(root,out=None,inspect=True,**spec))
    removed=set(old['boundary_bindings'])-external;added=external-set(old['boundary_bindings'])
    assert removed==set(SET_NAMES) and added=={'CPOut.8'},(removed,added)
    spec['bindings']={n:'cp_out8' if n=='CPOut.8' else old['boundary_bindings'][n] for n in external}
    ca=graph(root,out=out,**spec)
    p=out/'r_front_control_conta.sv';raw=p.read_text()
    assert raw.count('input wire [27:0] pins')==1
    raw=raw.replace('input wire [27:0] pins',
        'input wire [325:0] pins,input wire [3:0] next_raw,input wire priority_n,freeze,rb_forward,rb_forward_n,input wire cb_reset,cp_out8',1)
    for pkg in ['a09','b09','c09','d09']:
        assert raw.count('original_F10000 '+pkg+'(')==1
        raw=raw.replace('original_F10000 '+pkg+'(','original_F10000_ready '+pkg+'(',1)
    reset=next(line.split()[1].removesuffix(';') for line in raw.splitlines() if line.endswith('; // rMIRa'))
    before=' assign '+reset+'='+reset+'_j02_14;';assert raw.count(before)==1
    raw=raw.replace(before,before[:-1]+' | cb_reset;',1);p.write_text(raw)
    p=out/'r_front_control_reference.sv';raw=p.read_text()
    before='rb_forward,rb_forward_n,m.b.h03.p2,';assert raw.count(before)==1
    raw=raw.replace(before,'rb_forward,rb_forward_n,m.b.h03.p2,pins[344],',1)
    assert raw.count('wire [129:0] fc;')==1 and raw.count('output wire [93:0] control_flags);')==1
    raw=raw.replace('wire [129:0] fc;','wire [147:0] fc;',1)
    raw=raw.replace('output wire [93:0] control_flags);','output wire [93:0] control_flags,output wire [17:0] cp_set_levels);',1)
    raw=raw.replace(' assign control_flags={bf,fc[129:70]};',' assign control_flags={bf,fc[129:70]};\n assign cp_set_levels=fc[147:130];',1)
    p.write_text(raw);r['boards'][0]=ca
    r.update(scope=__doc__,closed_CP_sets=sorted(removed),CPOut8_slot=344,
             original_flag_indices_preserved=True,normal_memory_MIR_data_independent=True,
             native_integrated=False,physical_timing_qualified=False)
    assert r['physical_bits']==844 and r['clock_channels']==100 and r['ram_bits']==9216
    (out/'manifest.json').write_text(json.dumps(r,indent=2)+'\n');return r


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();generate(ROOT,a.output.resolve());print('GENERATED_ORIGINAL_844Q_CP_SOURCE_REFERENCE')
