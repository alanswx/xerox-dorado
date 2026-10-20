#!/usr/bin/env python3
"""Extract the coupled phase/run graph from original ContA pins and packages.

Includes whole phase-chain packages, not only the seven previously tested Qs.
Uses independent event-driven FF references and original combinational cells.
"""
import argparse
import hashlib
import json
from pathlib import Path
from generate_run_control_reference import BOARD,SEQUENTIAL as RUN,COMBINATIONAL as COMB,model_ports
from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]
SEQUENTIAL={**RUN,'k03':(22,2),'k04':(24,2),'k05':(26,2),'l07':(28,2),'j01':(30,2)}
COMBINATIONAL=[*COMB,'i07']


def generate(root,out):
    board=load_board(str(root/BOARD));selected=set(SEQUENTIAL)|set(COMBINATIONAL)
    pin_net={(p['pkg'],p['pin']):name for name,net in board.nets.items()for p in net['pins']}
    kinds={p:board.packages[p]['type']for p in sorted(selected)}
    assert kinds['i03']=='MC10176'and all(kinds[p]in('MC10231','SE10231')for p in SEQUENTIAL if p!='i03')
    nets=sorted({name for(pkg,_),name in pin_net.items()if pkg in selected});names={n:'n'+str(i)for i,n in enumerate(nets)}
    drivers={n:[p for p in board.nets[n]['pins']if p['pkg']in selected and p['dir']=='out']for n in nets}
    rails={n:('1'if n.startswith(('VCC','VDD','Vdd'))else'0')for n in nets if n.startswith(('VCC','VDD','Vdd','GND'))}
    external=[n for n in nets if not drivers[n]and n not in rails]
    ports={k:model_ports(root,k)for k in sorted(set(kinds.values()))}
    for pkg in selected:
        for name,net in board.nets.items():
            for p in net['pins']:
                if p['pkg']==pkg:assert ports[kinds[pkg]].get(p['pin'])==('input'if p['dir']=='in'else'output'),(pkg,p)
    lines=['`timescale 1ns/1ps','`default_nettype none',
           'module control_clock_reference(input wire fixture_load,input wire [31:0] fixture_state,',
           ' input wire base,input wire [8:0] controls,output wire [31:0] state_q,',
           ' output wire [8:0] clocks,output wire [4:0] enables,output wire [14:0] signals);']
    for name in nets:lines.append(' wire '+names[name]+'; // '+name)
    for name,value in rails.items():lines.append(' assign '+names[name]+"=1'b"+value+';')
    bindings={"CLK.ca'":'base','rStop':'controls[7]','SetRun':'controls[6]',"SetSS'":'controls[5]',
              'SetRunRfsh':'controls[4]',"Error'":'controls[3]','StopAtT1':'controls[2]',
              'RWTPCorRWIM':'controls[1]','NoDispatch':'controls[0]',"WIM'":'controls[8]',
              "CPAddr.0'":"1'b1","CPStrb'":"1'b1",'ContA28.sil+7':"1'b0",'ContA30.sil+1':"1'b0",'GetTLink':"1'b0"}
    assert set(bindings)==set(external),(set(external)-set(bindings),set(bindings)-set(external))
    for name in external:lines.append(' assign '+names[name]+'='+bindings[name]+';')
    for name in nets:
        if drivers[name]:
            terms=[names[name]+'_'+p['pkg']+'_'+str(p['pin'])for p in drivers[name]]
            lines += [' wire '+t+';'for t in terms]+[' assign '+names[name]+'='+' | '.join(terms)+';']
    for pkg in sorted(selected):
        args=[]
        for pin,direction in sorted(ports[kinds[pkg]].items()):
            name=pin_net.get((pkg,pin))
            if name is None:
                if direction=='input':args.append('.p'+str(pin)+"(1'b0)")
            else:args.append('.p'+str(pin)+'('+names[name]+(''if direction=='input'else'_'+pkg+'_'+str(pin))+')')
        if pkg in SEQUENTIAL:
            base,width=SEQUENTIAL[pkg];args+=['.fixture_load(fixture_load)',f'.fixture_state(fixture_state[{base}+:{width}])',f'.state_q(state_q[{base}+:{width}])']
        lines.append(' '+('original_'if pkg in SEQUENTIAL else'cell_')+kinds[pkg]+' '+pkg+'('+','.join(args)+');')
    for i,name in enumerate(["prepreclk'd",'StopRunClock',"preRunClk'Bc","RunClk'a","preclk2'Bb","clk2'Bc"]):lines.append(f' assign clocks[{i}]='+names[name]+';')
    lines+=[' assign clocks[6]='+names["prepreclk'd"]+' | '+names["bCLKEnable'd"]+';',
            ' assign clocks[7]='+names["clk2'Be"]+' | '+names['FreezeBD']+';',
            ' assign clocks[8]='+names['preCRamClock']+';']
    for i,name in enumerate(["CLKEnable'a","CLKEnable'b","CLKEnable'c","MemClkEnable'a","MemClkEnable'b"]):lines.append(f' assign enables[{i}]='+names[name]+';')
    for i,name in enumerate(['dStop','dMemRun','ClearMemStop','dStartCycle','sPhase0','StartCycle','preStartCyclea','Phase0','Phase1','Phase2','Phase3','Phase4','MemSH','CRamClock','SC&NoFreeze']):lines.append(f' assign signals[{i}]='+names[name]+';')
    lines+=['endmodule','`default_nettype wire'];out.mkdir(parents=True,exist_ok=False)
    (out/'control_clock_reference.sv').write_text('\n'.join(lines)+'\n')
    manifest=dict(packages=kinds,sequential=SEQUENTIAL,physical_bits=32,original_pins=sum((p,n)in pin_net for p in selected for n in ports[kinds[p]]),
        boundary_bindings=bindings,source_sha256=hashlib.sha256((root/BOARD).read_bytes()).hexdigest(),
        combinational_sources=sorted({'verilog/cells/cell_'+kinds[p]+'.v'for p in COMBINATIONAL}))
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print('CONTROL_CLOCK_REFERENCE packages='+str(len(kinds))+' pins='+str(manifest['original_pins'])+' Qs=32')
    return manifest


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,default=ROOT)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();generate(a.root.resolve(),a.output.resolve())
