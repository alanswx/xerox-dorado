#!/usr/bin/env python3
"""Extract all twelve RM/STK SRAMs and both byte-parity generators from PARC wiring.

RAM state exists only inside the reference fixture. Production ports use four
physical addresses and chip selects; no array state travels through the core.
"""
import argparse
import json
from pathlib import Path
from generate_processor_capture_reference import graph
from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]


def specs(root=ROOT):
    result=[]
    for low,bn in [(False,'ProcH-Rev-Ce'),(True,'ProcL-Rev-Ci')]:
        board=load_board(str(root/'chm/sil'/(bn+'.dm!1_')/(bn+'.wl')))
        pin_net={(p['pkg'],p['pin']):n for n,net in board.nets.items()for p in net['pins']}
        bindings={'TrueA':"1'b1"}
        def bind(pkg,pin,expression):
            net=pin_net[(pkg,pin)]
            assert net not in bindings or bindings[net]==expression,(bn,net)
            bindings[net]=expression
        for pkg,stack in [('h06',False),('j06',False),('i06',True),('k06',True)]:
            for bit in range(8):bind(pkg,7+bit,f'pins[{(15 if stack else 7)-bit}]')
            for pin in (3,4,5,6):bind(pkg,pin,f'pins[{17 if stack else 16}]')
            bind(pkg,2,'pins[26]');bind(pkg,15,"1'b0")
            for bit,pin in enumerate((19,18,17,16)):
                bind(pkg,pin,f'pins[{18+bit+(4 if pkg in ("j06","k06") else 0)}]')
        # The byte parity checkers consume these data pins plus TrueA. The
        # SRAM parity address/select nets must match the byte's physical port.
        for pkg,data_pkg,stack in [('j08','h06',False),('i08','i06',True)]:
            for bit,pin in enumerate((1,2,3,4,9,10,11,12)):
                assert pin_net[(pkg,pin)]==pin_net[(data_pkg,7+bit)]
            assert pin_net[(pkg,7)]==pin_net[(data_pkg,3)]
            bind(pkg,14,'pins[26]')
        names=[pin_net[('h06',pin)]for pin in (23,22,21,20)]
        names += [pin_net[('j06',pin)]for pin in (23,22,21,20)]
        names += [pin_net[('j08',15)],pin_net[('k03',2)]]
        seq={'h06':(0,1024),'j06':(1024,1024),'i06':(2048,1024),
             'k06':(3072,1024),'j08':(4096,256),'i08':(4352,256)}
        result.append(dict(bn=bn,comb=['k03'],seq=seq,
            module='r_file_ports_procl' if low else 'r_file_ports_proch',bindings=bindings,
            clock_taps=['@h06.p2','@i06.p2'],flag_taps=names))
    return result


def generate(root,out):
    out.mkdir(parents=True,exist_ok=False)
    records=[graph(root,out=out,**s)for s in specs(root)]
    body='''`timescale 1ns/1ps
module r_file_ports_reference(input wire fixture_load,input wire [9215:0] fixture_state,
 input wire advance,input wire [1:0] write_n,input wire [31:0] address,
 input wire [3:0] select_n,input wire [15:0] write_data,
 output wire [15:0] read_data,output wire [1:0] read_parity,
 output wire [9215:0] state_q,output wire [1:0] generated_parity);
 wire [27:0] hp={1'b0,(write_n[0]|!advance),write_data[7:0],select_n[1:0],address[15:0]};
 wire [27:0] lp={1'b0,(write_n[1]|!advance),write_data[15:8],select_n[3:2],address[31:16]};
 wire [9:0] hf,lf;wire [1:0] hc,lc;
 r_file_ports_proch h(fixture_load,fixture_state[4607:0],1'b0,1'b0,1'b0,hp,state_q[4607:0],hc,hf);
 r_file_ports_procl l(fixture_load,fixture_state[9215:4608],1'b0,1'b0,1'b0,lp,state_q[9215:4608],lc,lf);
 assign read_data={lf[7:0],hf[7:0]};assign read_parity={lf[8],hf[8]};
 assign generated_parity={lf[9],hf[9]};
endmodule
'''
    (out/'r_file_ports_reference.sv').write_text(body)
    manifest=dict(boards=records,physical_bits=9216,
        packages=sum(len(r['packages'])for r in records),
        original_pins=sum(r['original_pins']for r in records),
        scope=__doc__,native_integrated=False,
        boundary='Physical SRAM address/select and RbWrite window/data are external; address pipeline and coupled control timing not qualified')
    assert sum(r['physical_bits']for r in records)==9216
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print('R_FILE_REFERENCE',manifest['physical_bits'],'bits',manifest['packages'],'packages',manifest['original_pins'],'pins')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,default=ROOT)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();generate(a.root.resolve(),a.output.resolve())
