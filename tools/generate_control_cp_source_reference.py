#!/usr/bin/env python3
"""Close original CP-to-MIR set decoders in the full156-Q control graph.

The three original MC10172 packages share ContA h03's actual OR enable.
CPOut8 comes from the same physical bus bit already supplied to ContB.
Keep memory MIR data, unrelated producers, all state and all clocks unchanged.
"""
import argparse
import json
from pathlib import Path

from generate_control_hold_reference import generate as parent,SEAMS
from generate_conta_mir_reference import FLAGS as MIR_FLAGS
from generate_conta_local_reference import LOCAL_FLAGS
from generate_processor_capture_reference import graph,ROOT
from generate_native_cp_source import SET_NAMES


def generate(root,out):
    r=parent(root,out);old=r['boards'][0]
    spec=dict(bn=old['board'],comb=sorted((set(old['packages'])-set(old['sequential']))|{'g01','h01','i01'}),
              seq=old['sequential'],module='control_hold_conta',bindings={},
              clock_taps=old['clock_taps'],flag_taps=MIR_FLAGS+LOCAL_FLAGS+SEAMS+SET_NAMES)
    external=set(graph(root,out=None,inspect=True,**spec))
    removed=set(old['boundary_bindings'])-external;added=external-set(old['boundary_bindings'])
    assert removed==set(SET_NAMES) and added=={'CPOut.8'},(removed,added)
    spec['bindings']={n:('cp_out8' if n=='CPOut.8' else old['boundary_bindings'][n]) for n in external}
    # graph() writes the same original state and pin order, adding only the
    # physical producer packages. Keep the parent's shared CB reset binding.
    ca=graph(root,out=out,**spec)
    p=out/'control_hold_conta.sv';raw=p.read_text()
    assert raw.count('input wire [27:0] pins')==1
    raw=raw.replace('input wire [27:0] pins','input wire [325:0] pins,input wire cp_out8,input wire cb_reset',1)
    reset=next(line.split()[1].removesuffix(';') for line in raw.splitlines() if line.endswith('; // rMIRa'))
    old_reset=' assign '+reset+'='+reset+'_j02_14;';assert raw.count(old_reset)==1
    raw=raw.replace(old_reset,old_reset[:-1]+' | cb_reset;',1);p.write_text(raw)
    p=out/'control_hold_reference.sv';raw=p.read_text()
    old_call="1'b0,1'b0,ap,m.b.h03.p2,"
    assert raw.count(old_call)==1
    raw=raw.replace(old_call,"1'b0,1'b0,ap,pins[344],m.b.h03.p2,",1)
    assert raw.count('output wire [93:0] flags);')==1 and raw.count('wire [59:0] af;')==1
    assert raw.count('assign flags={bf,af};')==1
    raw=raw.replace('output wire [93:0] flags);','output wire [93:0] flags,output wire [17:0] cp_set_levels);',1)
    raw=raw.replace('wire [59:0] af;','wire [77:0] af;',1)
    raw=raw.replace('assign flags={bf,af};','assign flags={bf,af[59:0]};\n assign cp_set_levels=af[77:60];',1)
    p.write_text(raw)
    r['boards'][0]=ca
    r.update(scope=__doc__,physical_bits=156,clock_channels=56,flag_count=94,
             closed_CP_set_boundaries=sorted(removed),shared_CPOut8_slot=344,
             CP_set_source_packages=['g01','h01','i01','h03'],CP_source_arrival_qualified=False,
             normal_memory_MIR_data_stays_external=True,native_integrated=False)
    (out/'manifest.json').write_text(json.dumps(r,indent=2)+'\n')
    return r


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();r=generate(ROOT,a.output.resolve());print('GENERATED_ACTUAL_CP_SOURCE_CONTROL_REFERENCE',r['physical_bits'])
