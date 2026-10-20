#!/usr/bin/env python3
"""Extract original ContA/ProcH/ProcL clock, hold and T/task capture graphs.

Only selected packages are included. Task-arbiter Next/SwitchUp/priority and
memory Hold/PRhold remain explicit boundary pins. No candidate equations are
used in the reference graph.
"""
import argparse
import hashlib
import json
from pathlib import Path
from generate_control_clock_reference import SEQUENTIAL as CONTROL_SEQ,COMBINATIONAL as CONTROL_COMB
from generate_run_control_reference import model_ports
from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]
PROC_SEQ={'l02':(0,2),'h22':(2,6),'a21':(8,4),'a20':(12,4)}
PROC_COMB=['l01','f12','f13','g12','g13','c18','f19','a01','a22','a24','h24','h23','f20']


def graph(root,bn,comb,seq,module,bindings,clock_taps,flag_taps,out,inspect=False):
    path='chm/sil/'+bn+'.dm!1_/'+bn+'.wl';b=load_board(str(root/path));selected=set(comb)|set(seq)
    pin_net={(p['pkg'],p['pin']):n for n,net in b.nets.items()for p in net['pins']}
    kinds={p:b.packages[p]['type']for p in sorted(selected)}
    nets=sorted({n for(p,_),n in pin_net.items()if p in selected});names={n:'n'+str(i)for i,n in enumerate(nets)}
    drivers={n:[p for p in b.nets[n]['pins']if p['pkg']in selected and p['dir']=='out']for n in nets}
    rails={n:('1'if n.startswith(('VCC','VDD','Vdd'))else'0')for n in nets if n.startswith(('VCC','VDD','Vdd','GND'))}
    external=[n for n in nets if not drivers[n]and n not in rails]
    if inspect:return external
    assert set(bindings)==set(external),(bn,set(external)-set(bindings),set(bindings)-set(external))
    ports={k:model_ports(root,k)for k in sorted(set(kinds.values()))}
    for pkg in selected:
        for n,net in b.nets.items():
            for p in net['pins']:
                if p['pkg']==pkg:assert ports[kinds[pkg]].get(p['pin'])==('input'if p['dir']=='in'else'output'),(bn,pkg,p)
    width=max(base+width for base,width in seq.values())
    lines=['`timescale 1ns/1ps','`default_nettype none',
           f'module {module}(input wire fixture_load,input wire [{width-1}:0] fixture_state,',
           ' input wire base,start_a,en,input wire [27:0] pins,',
           f' output wire [{width-1}:0] state_q,output wire [{len(clock_taps)-1}:0] clocks,',
           f' output wire [{len(flag_taps)-1}:0] flags);']
    for n in nets:lines.append(' wire '+names[n]+'; // '+n)
    for n,v in rails.items():lines.append(' assign '+names[n]+"=1'b"+v+';')
    for n in external:lines.append(' assign '+names[n]+'='+bindings[n]+';')
    for n in nets:
        if drivers[n]:
            terms=[names[n]+'_'+p['pkg']+'_'+str(p['pin'])for p in drivers[n]]
            lines+=[' wire '+t+';'for t in terms]+[' assign '+names[n]+'='+' | '.join(terms)+';']
    for pkg in sorted(selected):
        args=[]
        for pin,direction in sorted(ports[kinds[pkg]].items()):
            n=pin_net.get((pkg,pin))
            if n is None:
                if direction=='input':args.append('.p'+str(pin)+"(1'b0)")
            else:args.append('.p'+str(pin)+'('+names[n]+(''if direction=='input'else'_'+pkg+'_'+str(pin))+')')
        if pkg in seq:
            base,width=seq[pkg];args+=['.fixture_load(fixture_load)',f'.fixture_state(fixture_state[{base}+:{width}])',f'.state_q(state_q[{base}+:{width}])']
        lines.append(' '+('original_'if pkg in seq else'cell_')+kinds[pkg]+' '+pkg+'('+','.join(args)+');')
    for port,taps in [('clocks',clock_taps),('flags',flag_taps)]:
        for i,n in enumerate(taps):lines.append(f' assign {port}[{i}]='+(n[1:]if n.startswith('@')else names[n])+';')
    lines+=['endmodule','`default_nettype wire'];(out/(module+'.sv')).write_text('\n'.join(lines)+'\n')
    return dict(board=bn,packages=kinds,sequential=seq,physical_bits=max(x+y for x,y in seq.values()),
                original_pins=sum((p,n)in pin_net for p in selected for n in ports[kinds[p]]),
                boundary_bindings=bindings,clock_taps=clock_taps,flag_taps=flag_taps,
                source_sha256=hashlib.sha256((root/path).read_bytes()).hexdigest(),
                combinational_sources=sorted({'verilog/cells/cell_'+kinds[p]+'.v'for p in comb}))


def control_bindings():
    return {"CLK.ca'":'base','rStop':'pins[7]','SetRun':'pins[6]',"SetSS'":'pins[5]',
        'SetRunRfsh':'pins[4]',"Error'":'pins[3]','StopAtT1':'pins[2]',
        'RWTPCorRWIM':'pins[1]','NoDispatch':'pins[0]',"WIM'":'pins[8]',
        'Hold':'pins[26]','SwitchUp':'pins[27]',
        "CPAddr.0'":"1'b1","CPStrb'":"1'b1",'ContA28.sil+7':"1'b0",'ContA30.sil+1':"1'b0",'GetTLink':"1'b0"}


def processor_bindings(low):
    bn='ProcL'if low else'ProcH'
    result={"CLK.pl'"if low else"CLK.ph'":'base',"CLKEnable'b"if low else"CLKEnable'c":'en',
        "StartCycle'a":'start_a','PRhold':'pins[25]',"BNTGtCT'a":'pins[24]',
        'LC.0':'pins[11]','LC.1':'pins[10]','LC.2':'pins[9]',
        'Next.0':'pins[20]','Next.1':'pins[21]','Next.2':'pins[22]','Next.3':'pins[23]',
        "FA=0'b":"(pins[19:18]!=2'd0)","FC=5'":"(pins[14:12]!=3'd5)","FB=7'":"(pins[17:15]!=3'd7)"}
    # These decode pins supply unused gates in the selected physical packages.
    result.update({"FB=3'":"(pins[17:15]!=3'd3)","FC=6'":"(pins[14:12]!=3'd6)",
                   "BSEL.0'":"1'b0","BSEL.1'":"1'b0","BSEL.2'":"1'b0",'IOBout':"1'b0"})
    if low:
        result.update({"FB=2'":"(pins[17:15]!=3'd2)","FC=4/5'":"((pins[14:12]!=3'd4)&&(pins[14:12]!=3'd5))",
            'HoldDly':"1'b0","IOBin'":"1'b0",'ProcL26.sil+1':"1'b0",'ProcL27.sil+11':"1'b0",'TisIFdata':"1'b0"})
    else:
        result.update({"FA=1'a":"(pins[19:18]!=2'd1)","FA=3'":"(pins[19:18]!=2'd3)",
            "FC=7'":"(pins[14:12]!=3'd7)","FF.2'a":"~pins[17]",'FF.2a':'pins[17]',"FF.3'a":"~pins[16]",
            'ProcH26.sil+3':"1'b0",'ProcH27.sil+12':"1'b0"})
    return result


def specs():
    control_clocks=["prepreclk'd",'StopRunClock',"preRunClk'Bc","RunClk'a","preclk2'Bb","clk2'Bc",
        "@l06.ca","@k04.ca",'preCRamClock',"clk0'Ba","h*fclk0'Ba","h*fclk0'Da"]
    return [dict(bn='ContA-Rev-Cd',comb=[*CONTROL_COMB,'f19','f20','g13','j07','j18'],seq=CONTROL_SEQ,module='capture_conta',bindings=control_bindings(),
                 clock_taps=control_clocks,flag_taps=['CAHold','RepeatCur','FreezeBD',"CLKEnable'c","CLKEnable'b","StartCycle'a"]),
            *[dict(bn='ProcL-Rev-Ci'if low else'ProcH-Rev-Ce',comb=[*PROC_COMB,'g23']if low else[*PROC_COMB,'f18','i18'],
                   seq={('d17'if low and p=='h22'else p):v for p,v in PROC_SEQ.items()},module='capture_procl'if low else'capture_proch',
                   bindings=processor_bindings(low),clock_taps=['@l02.ca','@l02.cb',"SHCP'C","PreClock0'B"if low else"Clock0'Da","Clock1'Ca"],
                   flag_taps=['RepeatCurrC','Hold',"Curr=Next'"])for low in (False,True)]]


def generate(root,out,inspect=False):
    if inspect:
        for spec in specs():
            external=graph(root,out=None,inspect=True,**spec)
            print(spec['bn'],'unbound',sorted(set(external)-set(spec['bindings'])),'extra',sorted(set(spec['bindings'])-set(external)))
        return
    out.mkdir(parents=True,exist_ok=False);records=[graph(root,out=out,**s)for s in specs()]
    boards={r['board']:load_board(str(root/'chm/sil'/(r['board']+'.dm!1_')/(r['board']+'.wl')))for r in records}
    c=boards['ContA-Rev-Cd'];h=boards['ProcH-Rev-Ce'];l=boards['ProcL-Rev-Ci']
    seams=[]
    # The Dorado backplane is wired by named net, not straight-through pins.
    for consumer,net,source_pin,sink_pin in [(h,"StartCycle'a",'C24','C5'),(l,"StartCycle'a",'C24','C5'),
                                           (h,"CLKEnable'c",'C13','C8'),(l,"CLKEnable'b",'C12','C8')]:
        assert c.nets[net]['backplane']=={source_pin}and consumer.nets[net]['backplane']=={sink_pin}
        seams.append(dict(signal=net,consumer=consumer.name,source_pin=source_pin,sink_pin=sink_pin))
    assert c.nets['Hold']['backplane']=={'E26'}
    assert h.nets['PRhold']['backplane']==l.nets['PRhold']['backplane']=={'E42'}
    base_path='chm/sil/BaseBd-Rev-Am.dm!1_/BaseBd-Rev-Am.wl'
    base_board=load_board(str(root/base_path))
    base_pins={(p['pkg'],p['pin']):n for n,net in base_board.nets.items()for p in net['pins']}
    expected={('j02',5):"StartClockPulse'",('j02',6):'EndClockPulse',('j02',7):'dStartClockPulse',
              ('j02',3):'BaseBd12.sil+1',('l01',7):'BaseBd12.sil+1',('l01',11):'BaseBd12.sil+1',
              ('l01',2):"CLK.ca'",('l01',12):"CLK.ph'",('l01',13):"CLK.pl'"}
    assert base_board.packages['j02']['type']==base_board.packages['l01']['type']=='MC10210'
    assert all(base_pins.get(k)==v for k,v in expected.items())
    assert all(k not in base_pins for k in [('l01',5),('l01',6),('l01',9),('l01',10)])
    body='''`timescale 1ns/1ps
module processor_capture_reference(input wire fixture_load,input wire [63:0] fixture_state,
 input wire base,input wire [27:0] pins,output wire [63:0] state_q,
 output wire [21:0] clocks,output wire [8:0] flags);
 wire [2:0] bases;
 cell_MC10210 base_fanout(.p7(base),.p11(base),.p5(1'b0),.p6(1'b0),.p9(1'b0),.p10(1'b0),
   .p2(bases[2]),.p12(bases[1]),.p13(bases[0]));
 wire [11:0] cc;wire [4:0] ch,cl;wire [5:0] fc;wire [2:0] fh,fl;
 wire en_h=fc[3],en_l=fc[4],start_a=fc[5];
 capture_conta c(fixture_load,fixture_state[31:0],bases[2],1'b0,1'b0,pins,state_q[31:0],cc,fc);
 capture_proch h(fixture_load,fixture_state[47:32],bases[1],start_a,en_h,pins,state_q[47:32],ch,fh);
 capture_procl l(fixture_load,fixture_state[63:48],bases[0],start_a,en_l,pins,state_q[63:48],cl,fl);
 assign clocks={cc[11:9],cl,ch,cc[8:0]};assign flags={fl,fh,fc[2:0]};
endmodule
'''
    (out/'processor_capture_reference.sv').write_text(body)
    result=dict(boards=records,physical_bits=sum(x['physical_bits']for x in records),original_pins=sum(x['original_pins']for x in records),packages=sum(len(x['packages'])for x in records),seams=seams,
                clock_source=dict(board=base_path,sha256=hashlib.sha256((root/base_path).read_bytes()).hexdigest(),
                                  contract='CLK.ca/ph/pl are parallel outputs of BaseBd l01 from the same j02 pulse; analog skew is not modeled.'))
    (out/'manifest.json').write_text(json.dumps(result,indent=2)+'\n');print('PROCESSOR_CAPTURE_REFERENCE',result['physical_bits'],'Qs',result['packages'],'packages',result['original_pins'],'pins')
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,default=ROOT)
    p.add_argument('--output',type=Path);p.add_argument('--inspect',action='store_true');a=p.parse_args()
    if not a.inspect and not a.output:p.error('Specify --output or --inspect')
    generate(a.root.resolve(),a.output.resolve()if a.output else None,a.inspect)
