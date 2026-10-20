#!/usr/bin/env python3
"""Extract original R read/write-data/visible latches and delayed bypass flags.

RM/STK data and parity stay functional read-port boundaries, not flattened
state. All selected physical packages and mixed-net drivers are extracted
independently from the original wire lists, without candidate equations.
"""
import argparse
import json
from pathlib import Path

from generate_processor_capture_reference import graph
from generate_t_parity_reference import specs as parity_specs, generate as generate_parity
from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]


def specs():
    result=parity_specs();result[0]['module']='r_operand_conta'
    for low,s in zip((False,True),result[1:]):
        s['module']='r_operand_procl' if low else 'r_operand_proch'
        s['seq'].update(j01=(246,4),j02=(250,4),g05=(254,4),h05=(258,4),
                        g06=(262,4),g07=(266,4),l07=(270,2))
        s['comb']=sorted(set(s['comb']+['l08','l09']))
        b=s['bindings']
        b.pop('RbBypassDly')
        for bit in range(8):b.pop(f'R.{bit+(8 if low else 0):02}',None)
        bypass='pins[150]'if low else'pins[148]'
        b['RbBypass']=bypass;b["RbBypass'"]='~'+bypass
        for bit in range(8):
            num=bit+(8 if low else 0)
            net=('ddR.15'if bit==7 else f'ProcL{bit+2:02}.sil+1')if low else('ddR.00'if bit==0 else f'ProcH{bit+2:02}.sil+1')
            b[net]=f'pins[{127+num}]'
        s['clock_taps']+=['@l07.p7','@l07.p9']
        s['flag_taps']+=["Clock1'Bb",'@l07.state_q[0]','RbBypassDly']
    return result


def generate(root,out,inspect=False):
    if inspect:
        for s in specs():
            external=graph(root,out=None,inspect=True,**s)
            print(s['bn'],'unbound',sorted(set(external)-set(s['bindings'])),
                  'extra',sorted(set(s['bindings'])-set(external)))
        return
    out.mkdir(parents=True,exist_ok=False)
    for s in specs():
        b=load_board(str(root/'chm/sil'/(s['bn']+'.dm!1_')/(s['bn']+'.wl')))
        chosen=set(s['comb'])|set(s['seq'])
        for name,net in b.nets.items():
            drivers=[p['pkg']for p in net['pins']if p['dir']=='out']
            if any(p['pkg']in chosen and p['dir']=='in'for p in net['pins'])and any(p in chosen for p in drivers):
                assert all(p in chosen for p in drivers),(s['bn'],name,drivers)
    records=[graph(root,out=out,**s)for s in specs()]
    for s in specs():
        p=out/(s['module']+'.sv');text=p.read_text();assert text.count('input wire [27:0] pins')==1
        p.write_text(text.replace('input wire [27:0] pins','input wire [151:0] pins,input wire [3:0] next_raw,input wire priority_n,freeze'))
    audit=generate_parity(root,out/'parity-source-audit')
    body=(out/'parity-source-audit/t_parity_reference.sv').read_text()
    for before,after,count in [('module t_parity_reference','module r_operand_reference',1),
        ('[539:0]','[591:0]',2),('[24:0]','[28:0]',1),
        ('wire [9:0] ch,cl','wire [11:0] ch,cl',1),('wire [8:0] fh,fl','wire [11:0] fh,fl',1),
        ('wire [245:0] sh,sl,ih,il','wire [271:0] sh,sl,ih,il',1),
        ('t_parity_conta c(','r_operand_conta c(',1),('t_parity_proch h(','r_operand_proch h(',1),('t_parity_procl l(','r_operand_procl l(',1),
        ('parity_ok,faults);','parity_ok,faults,output wire [1:0] r_read_n,r_visible_n,output wire [3:0] r_flags);',1),
        ('assign clocks={cl[5]','assign clocks={cl[11:10],ch[11:10],cl[5]',1)]:
        assert body.count(before)==count,before;body=body.replace(before,after)
    extra=''' assign ih[253:246]=fixture_state[547:540];assign il[253:246]=fixture_state[555:548];
 assign ih[261:254]=fixture_state[563:556];assign il[261:254]=fixture_state[571:564];
 assign ih[269:262]=fixture_state[579:572];assign il[269:262]=fixture_state[587:580];
 assign ih[271:270]=fixture_state[589:588];assign il[271:270]=fixture_state[591:590];
 assign state_q[547:540]=sh[253:246];assign state_q[555:548]=sl[253:246];
 assign state_q[563:556]=sh[261:254];assign state_q[571:564]=sl[261:254];
 assign state_q[579:572]=sh[269:262];assign state_q[587:580]=sl[269:262];
 assign state_q[589:588]=sh[271:270];assign state_q[591:590]=sl[271:270];
 assign r_read_n={fl[9],fh[9]};assign r_visible_n={fl[3],fh[3]};
 assign r_flags={fl[11:10],fh[11:10]};
'''
    body=body.replace('endmodule\n',extra+'endmodule\n');(out/'r_operand_reference.sv').write_text(body)
    manifest=dict(boards=records,physical_bits=sum(r['physical_bits']for r in records),
        packages=sum(len(r['packages'])for r in records),original_pins=sum(r['original_pins']for r in records),
        source_audit=audit,boundary='Functional RM/STK read data/parity, RbBypass and RisIFdata plus inherited producers; storage/address/write ports unqualified')
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print('R_OPERAND_REFERENCE',manifest['physical_bits'],'bits',manifest['packages'],'packages',manifest['original_pins'],'pins')
    return manifest


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,default=ROOT)
    p.add_argument('--output',type=Path);p.add_argument('--inspect',action='store_true');a=p.parse_args()
    if not a.inspect and not a.output:p.error('Specify --output or --inspect')
    generate(a.root.resolve(),a.output.resolve()if a.output else None,a.inspect)
