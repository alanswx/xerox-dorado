#!/usr/bin/env python3
"""Extract the run/refresh cone directly from the original ContA wire list.

Combinational packages use the real pin-function models. Sequential packages
use independent event-driven MC10176/MC10231 models, without fabric clock
sampling. No candidate next-state equations are used to generate this oracle.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
from sil_netlist import load_board

ROOT = Path(__file__).resolve().parents[1]
BOARD = 'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl'
SEQUENTIAL = {'i03': (0,6), 'j04': (6,2), 'k01': (8,2), 'k02': (10,2),
              'l02': (12,2), 'l03': (14,2), 'l04': (16,2),
              'l05': (18,2), 'l06': (20,2)}
COMBINATIONAL = ['g04','h03','i04','j05','i05','l01','f12','g12','f13','i06','j06','g05','l08']


def model_ports(root,kind):
    raw=(root/'verilog/cells'/('cell_'+kind+'.v')).read_text()
    return {int(pin):direction for direction,pin in re.findall(
        r'\b(input|output)\s+(?:wire\s+|reg\s+)?p(\d+)\b',raw)}


def generate(root,out):
    board=load_board(str(root/BOARD))
    selected=set(SEQUENTIAL)|set(COMBINATIONAL)
    pin_net={(p['pkg'],p['pin']):name for name,net in board.nets.items() for p in net['pins']}
    kinds={p:board.packages[p]['type'] for p in selected}
    assert kinds['i03']=='MC10176'
    assert all(kinds[p] in ('MC10231','SE10231') for p in SEQUENTIAL if p!='i03')
    nets=sorted({name for (pkg,_),name in pin_net.items() if pkg in selected})
    names={name:'n'+str(index) for index,name in enumerate(nets)}
    drivers={name:[p for p in board.nets[name]['pins'] if p['pkg'] in selected and p['dir']=='out'] for name in nets}
    rails={name:('1' if name.startswith(('VCC','Vdd','VDD')) else '0') for name in nets
           if name.startswith(('GND','VCC','Vdd','VDD'))}
    external=[name for name in nets if not drivers[name] and name not in rails]
    # Pin-function models and the original graph must agree on every direction.
    ports={kind:model_ports(root,kind) for kind in set(kinds.values())}
    for pkg in selected:
        for name,net in board.nets.items():
            for pin in net['pins']:
                if pin['pkg']==pkg:
                    expected='input' if pin['dir']=='in' else 'output'
                    assert ports[kinds[pkg]].get(pin['pin'])==expected,(pkg,pin)
    text=['`timescale 1ns/1ps','`default_nettype none',
          'module original_run_control(input wire fixture_load,',
          ' input wire [21:0] fixture_state,',
          ' input wire ['+str(len(external)-1)+':0] pins,',
          ' output wire [21:0] state_q, output wire [6:0] clocks,',
          ' output wire [4:0] enables,output wire [2:0] decoded);']
    for name in nets:text.append(' wire '+names[name]+'; // '+name)
    for name,value in rails.items():text.append(' assign '+names[name]+"=1'b"+value+';')
    for i,name in enumerate(external):text.append(' assign '+names[name]+'=pins['+str(i)+'];')
    for name in nets:
        if drivers[name]:
            terms=[names[name]+'_'+p['pkg']+'_'+str(p['pin']) for p in drivers[name]]
            text.extend(' wire '+t+';' for t in terms)
            text.append(' assign '+names[name]+'='+' | '.join(terms)+';')
    for pkg in sorted(selected):
        kind=kinds[pkg];arguments=[]
        for pin,direction in sorted(ports[kind].items()):
            net=pin_net.get((pkg,pin))
            if net is None:
                if direction=='input':arguments.append(".p"+str(pin)+"(1'b0)")
            else:
                value=names[net] if direction=='input' else names[net]+'_'+pkg+'_'+str(pin)
                arguments.append('.p'+str(pin)+'('+value+')')
        if pkg in SEQUENTIAL:
            base,width=SEQUENTIAL[pkg]
            arguments+=['.fixture_load(fixture_load)',f'.fixture_state(fixture_state[{base}+:{width}])',
                        f'.state_q(state_q[{base}+:{width}])']
        text.append(' '+('original_'+kind if pkg in SEQUENTIAL else 'cell_'+kind)+' '+pkg+' ('+','.join(arguments)+');')
    taps=["prepreclk'd",'StopRunClock',"preRunClk'Bc","RunClk'a", "preclk2'Bb","clk2'Bc"]
    for i,name in enumerate(taps):text.append(f' assign clocks[{i}]='+names[name]+';')
    text.append(' assign clocks[6]='+names["prepreclk'd"]+' | '+names["bCLKEnable'd"]+';')
    for i,name in enumerate(["CLKEnable'a","CLKEnable'b","CLKEnable'c","MemClkEnable'a","MemClkEnable'b"]):
        text.append(f' assign enables[{i}]='+names[name]+';')
    for i,name in enumerate(['dStop','dMemRun','ClearMemStop']):text.append(f' assign decoded[{i}]='+names[name]+';')
    text+=['endmodule','`default_nettype wire']
    out.mkdir(parents=True,exist_ok=False)
    (out/'original_run_control.sv').write_text('\n'.join(text)+'\n')
    bindings={"CLK.ca'":'base', 'rStop':'controls[7]', 'SetRun':'controls[6]',
              "SetSS'":'controls[5]', 'SetRunRfsh':'controls[4]',
              'Phase0':'controls[3]', "Phase0'":'~controls[3]',
              'preStartCyclea':'controls[2]', "Error'":'controls[1]',
              'StopAtT1':'controls[0]', "Phase4'":"1'b1",
              "CPAddr.0'":"1'b1", "CPStrb'":"1'b1",
              'ContA28.sil+7':"1'b0", 'ContA30.sil+1':"1'b0",
              'FreezeBD':"1'b0", 'GetTLink':"1'b0", 'RWTPCorRWIM':"1'b0"}
    assert set(bindings)==set(external),(set(external)-set(bindings),set(bindings)-set(external))
    adapter=['`timescale 1ns/1ps','`default_nettype none',
             'module run_control_reference_adapter(input wire base, input wire [7:0] controls,',
             ' input wire fixture_load,input wire [21:0] fixture_state,',
             ' output wire [21:0] state_q,output wire [6:0] clocks,',
             ' output wire [4:0] enables,output wire [2:0] decoded);',
             ' wire ['+str(len(external)-1)+':0] pins;']
    adapter += [f' assign pins[{i}]={bindings[name]}; // '+name for i,name in enumerate(external)]
    adapter += [' original_run_control graph(fixture_load,fixture_state,pins,state_q,clocks,enables,decoded);',
                'endmodule','`default_nettype wire']
    (out/'run_control_reference_adapter.sv').write_text('\n'.join(adapter)+'\n')
    manifest=dict(packages=kinds,external_inputs=external,sequential=SEQUENTIAL,
                  physical_bits=22,original_pins=sum((pkg,pin) in pin_net for pkg in selected for pin in ports[kinds[pkg]]),
                  boundary_bindings=bindings,
                  source_sha256=hashlib.sha256((root/BOARD).read_bytes()).hexdigest(),
                  combinational_sources=['verilog/cells/cell_'+kinds[p]+'.v' for p in COMBINATIONAL])
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print('RUN_CONTROL_REFERENCE packages='+str(len(selected))+' pins='+str(manifest['original_pins'])+' state_bits=22')
    return manifest


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--root',type=Path,default=ROOT)
    parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
    generate(args.root.resolve(),args.output.resolve())
