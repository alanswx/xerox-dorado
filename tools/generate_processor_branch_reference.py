#!/usr/bin/env python3
"""Compose saved flags with the original 844-bit processor/control circuit.

Append80 original bits without moving old indices. ALUCarry/Overflow cease to
be independent input pins: actual f17 outputs feed g18. Compare this single
original graph with a partition of the same original circuits connected by
physical nets. This builds the integrated oracle, not an optimized core proof.
"""
import argparse
import json
from pathlib import Path
import re
import shutil

from generate_branch_condition_reference import COMB,SEQ,generate as branch_generate
from generate_processor_capture_reference import ROOT,graph
from test_external_r_packet import sha

BUILD=ROOT/'build/rtl-executive'


def generate(out):
    assert not out.exists();out.mkdir(parents=True)
    source=BUILD/'cp-original-program-balanced-verilator-v2-20261025'
    manifest=BUILD/'r-front-control-native-inventory-v1-20261023/inputs/reference/manifest.json'
    parent=json.loads(manifest.read_text());h=next(b for b in parent['boards'] if b['board'].startswith('ProcH'))
    assert h['physical_bits']==280 and h['source_sha256']==sha(ROOT/'chm/sil/ProcH-Rev-Ce.dm!1_/ProcH-Rev-Ce.wl')
    frozen=out/'timed';shutil.copytree(source/'timed',frozen)
    branch=branch_generate(ROOT,out/'branch',1000,1000,1000)
    shutil.copy2(out/'branch/branch_condition_reference.sv',frozen/'branch_condition_reference.sv')
    pins=branch['pin_index'];ordered=[name for name,index in sorted(pins.items(),key=lambda row:row[1])]
    spec=dict(bn=h['board'],module='processor_branch_proch',
              comb=sorted((set(h['packages'])-set(h['sequential']))|set(COMB)),
              seq={**h['sequential'],**{name:(280+start,width) for name,(start,width) in SEQ.items()}},
              bindings={},clock_taps=h['clock_taps']+['@g11.p9','@h11.p9','@f17.p7','@e13.p13'],
              flag_taps=h['flag_taps']+["ResEqZero'","ResLtZero'",'ALUCarry',"Overflow'"]+ordered)
    external=set(graph(ROOT,out=None,inspect=True,**spec));old=h['boundary_bindings']
    removed=set(old)-external;assert removed=={'ALUCarry',"Overflow'"}
    extras={name:401+i for i,name in enumerate(sorted(external-set(old)))}
    assert set(extras)=={'SignedCarry','aluCout','aluM',"aluOut=0'",*[f'TIOA.{i}' for i in range(8)]}
    spec['bindings']={name:old[name] if name in old else f'pins[{extras[name]}]' for name in sorted(external)}
    record=graph(ROOT,out=frozen,**spec);path=frozen/'processor_branch_proch.sv';raw=path.read_text()
    raw=raw.replace('input wire [27:0] pins',
        'input wire [412:0] pins,input wire [3:0] next_raw,input wire priority_n,freeze,rb_forward,rb_forward_n',1)
    path.write_text(raw)
    old_top=(frozen/'r_front_control_reference.sv').read_text();top=old_top
    replacements=[('module r_front_control_reference','module processor_branch_reference'),
                  ('[843:0]','[923:0]'),('[400:0]','[412:0]'),('[99:0] clocks','[103:0] clocks'),
                  ('wire [14:0] ch','wire [18:0] ch'),('wire [26:0] fh','wire [68:0] fh'),
                  ('wire [279:0] sh,ih','wire [359:0] sh,ih'),
                  ('r_front_control_proch h(','processor_branch_proch h('),
                  ('assign clocks={memory_clocks,','assign clocks={ch[18:15],memory_clocks,'),
                  ('output wire [17:0] cp_set_levels);','output wire [17:0] cp_set_levels,output wire [37:0] branch_pins,output wire [3:0] branch_flags);')]
    for before,after in replacements:
        assert before in top,before;top=top.replace(before,after)
    for bit,flag in [(110,2),(111,3)]:
        before=f'assign source_pins[{bit}]=cpu_pins[{bit}];';assert top.count(before)==1
        top=top.replace(before,f'assign source_pins[{bit}]=branch_flags[{flag}];',1)
    extra=''' assign ih[359:280]=fixture_state[923:844];
 assign state_q[923:844]=sh[359:280];
 assign branch_flags=fh[30:27];assign branch_pins=fh[68:31];
'''
    assert top.count('endmodule')==1;top=top.replace('endmodule',extra+'endmodule',1)
    (frozen/'processor_branch_reference.sv').write_text(top)
    # Independently connect the pre-existing original graph to the original
    # flag-family graph. Existing nets come from actual old package wiring;
    # only the twelve ALU/TIOA producer inputs remain external.
    old_h=(frozen/'r_front_control_proch.sv').read_text()
    nodes={name:wire for wire,name in re.findall(r'^ wire (\w+); // ([^\n]+)$',old_h,re.M)}
    connections={name:('pins['+str(extras[name])+']' if name in extras else 'old.h.'+nodes[name]) for name in ordered}
    width=len(ordered);assert width==38
    partition=old_top
    partition=partition.replace('module r_front_control_reference','module processor_branch_partition',1)
    # Keep the old wrapper's body/ports exactly intact as a child. Its carry
    # inputs are driven only by the new original flag latch, not raw pin110/111.
    header=partition[:partition.index(' wire [2:0] bases;')]
    header=header.replace('[843:0]','[923:0]').replace('[400:0]','[412:0]').replace('[99:0] clocks','[103:0] clocks')
    header=header.replace('output wire [17:0] cp_set_levels);',
        'output wire [17:0] cp_set_levels,output wire [37:0] branch_pins,output wire [3:0] branch_flags);')
    partition=header+''' wire [400:0] old_pins;
 assign old_pins={pins[400:112],branch_flags[3:2],pins[109:0]};
 r_front_control_reference old(fixture_load,fixture_state[843:0],base,old_pins,state_q[843:0],clocks[99:0],
  flags,next_raw,write_n,latch_n,hold_clocks,t_latch_n,bypass,parity_clocks,parity_ok,faults,
  r_read_n,r_visible_n,r_flags,rm_flags_n,address,select_n,forwarded_bypass,source_pins,fa_n,
  front_data,ready_flags,control_flags,cp_set_levels);
 branch_condition_reference saved(fixture_load,fixture_state[923:844],old.processor_roots[1],1'b0,1'b0,
  branch_pins,state_q[923:844],clocks[103:100],branch_flags,);
'''+''.join(f' assign branch_pins[{index}]={connections[name]};\n' for name,index in sorted(pins.items(),key=lambda row:row[1]))+'''endmodule
'''
    (frozen/'processor_branch_partition.sv').write_text(partition)
    result=dict(scope=__doc__,physical_bits=924,old_indices_retained='843:0',new_indices='923:844',
                bulk_SRAM_bits=9216,clock_channels=104,extra_producer_pins=extras,
                closed_input_pins={'ALUCarry':110,"Overflow'":111},
                branch_boundary_connections=connections,original_high_board=record,
                source_reference=str(source),source_manifest_sha256=sha(manifest),
                source_files={str(p.relative_to(source)):sha(p) for p in (source/'timed').iterdir() if p.is_file()},
                branch_reference=branch,original_ALU_datapath_integrated=False,
                optimized_924_bit_step_qualified=False,native_integrated=False,physical_timing_qualified=False)
    (out/'manifest.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();r=generate(a.output.resolve());print('PROCESSOR_BRANCH_REFERENCE',r['physical_bits'],r['extra_producer_pins'])
