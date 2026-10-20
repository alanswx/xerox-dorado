#!/usr/bin/env python3
"""Extract final R address/select registers and muxes with the coupled processor.

Front-end register-D nets, RSTK, address-comparison results and PrBlock remain
explicit inputs. Every selected mixed net retains all of its original drivers.
This graph introduces no candidate timing/address equations.
"""
import argparse
import json
from pathlib import Path
from generate_processor_capture_reference import graph
from generate_r_flags_reference import specs as flag_specs,generate as generate_flags
from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]


def specs(root=ROOT):
    result=flag_specs();result[0]['module']='r_address_conta'
    for low,s in zip((False,True),result[1:]):
        s['module']='r_address_procl'if low else'r_address_proch'
        s['comb']=sorted(set(s['comb']+(['k08','k09','k10','g09','l10','h17','i18','l05','h12','h13','i11']if low else['k08','k09','k10','h19'])))
        s['seq'].update({'g10':(274,2),'h10':(276,2),'i15':(278,6),'k11':(284,6),
            'g08':(290,6),'j11':(296,6),'l11':(302,2),'h15':(304,4)}if low else{'l05':(274,6)})
        b=s['bindings'];extra={f'RSTK.{i}':f'pins[{172+i}]'for i in range(4)}
        if low:
            extra.update({f'ProcL23.sil+{n}':f'pins[{156+i}]'for i,n in enumerate((3,6,7,9))})
            extra.update({f'ProcL23.sil+{11+i}':f'pins[{160+i}]'for i in range(4)})
            extra.update({f'ProcL25.sil+{n}':f'pins[{164+i}]'for i,n in enumerate((5,1,2,4,3,14,12,13))})
            extra.update({"PrBlock'":'pins[176]',"RmBypass'":'pins[177]',"StkBypass'":'pins[178]',
                'Q.14':'pins[179]',"FFeqMul'":"(pins[19:12]!=8'o072)",
                "CkMdParity'":'pins[182]',"Md.Perr'":'pins[183]',
                'ProcL24.sil+3':'pins[184]',"RBaseWriteEn'":'pins[185]',
                "StkPSaveEn'":'pins[186]',"IfuRBaseSel'":'pins[187]'})
            extra.update({f"RBase.{i}'":f'pins[{188+i}]'for i in range(4)})
            extra.update({f"CntMux.{12+i}'":f'pins[{192+i}]'for i in range(4)})
            extra.update({f"FF.{4+i}'a":f'~pins[{15-i}]'for i in range(4)})
            extra.update({"SelRBaseWadr'":'pins[196]',"RBaseBypass'":'pins[197]',
                'SubTask.0':'pins[198]','SubTask.1':'pins[199]',
                'ProcL22.sil+4':'pins[200]','BumpRBase':'pins[201]'})
            s['clock_taps']+=['@g10.ca','@g10.cb','@h10.ca','@h10.cb',
                '@i15.p9','@k11.p9','@g08.p9','@j11.p9','@l11.ca','@l11.cb','@h15.p7']
            s['flag_taps']+=[f'RbAdr.{i}'for i in range(4)]+[f"RbAdr.{i}'"for i in range(8)]
            s['flag_taps'] += [f'StkAdr.{i}a'for i in range(8)]+[f'StkAdr.{i}b'for i in range(8)]
            s['flag_taps'] += ["SelectRm'a","SelectStk'a","SelectRm'b","SelectStk'b",'RbBypass',"RbBypass'",'StkSela',"StkSel'a"]
        else:
            extra.update({f'ProcH22.sil+{3+i}':f'pins[{152+i}]'for i in range(4)})
            extra.update({"PropCnt'":'pins[180]','ProcH17.sil+3':'pins[181]',
                'RbBypass':'rb_forward',"RbBypass'":'rb_forward_n'})
            s['clock_taps']+=['@l05.p9']
            s['flag_taps'] += [f"RbAdr.{i}'"for i in range(4,8)]
        external=set(graph(root,out=None,inspect=True,**s))
        b.update(extra)
        s['bindings']={n:v for n,v in b.items()if n in external}
        assert external==set(s['bindings']),(s['bn'],external-set(s['bindings']))
    return result


def generate(root,out):
    out.mkdir(parents=True,exist_ok=False)
    for s in specs(root):
        board=load_board(str(root/'chm/sil'/(s['bn']+'.dm!1_')/(s['bn']+'.wl')))
        chosen=set(s['comb'])|set(s['seq'])
        for name,net in board.nets.items():
            drivers=[p['pkg']for p in net['pins']if p['dir']=='out']
            if any(p['pkg']in chosen and p['dir']=='in'for p in net['pins'])and any(p in chosen for p in drivers):
                assert all(p in chosen for p in drivers),(s['bn'],name,drivers)
    records=[graph(root,out=out,**s)for s in specs(root)]
    for s in specs(root):
        p=out/(s['module']+'.sv');text=p.read_text();assert text.count('input wire [27:0] pins')==1
        p.write_text(text.replace('input wire [27:0] pins',
            'input wire [201:0] pins,input wire [3:0] next_raw,input wire priority_n,freeze,rb_forward,rb_forward_n'))
    audit=generate_flags(root,out/'flags-source-audit')
    body=(out/'flags-source-audit/r_flags_reference.sv').read_text()
    edits=[('module r_flags_reference','module r_address_reference',1),('[595:0]','[635:0]',2),
        ('[151:0] pins','[201:0] pins',1),('[32:0] clocks','[44:0] clocks',1),
        ('wire [13:0] ch,cl','wire [14:0] ch;wire [24:0] cl',1),
        ('wire [12:0] fh,fl','wire [16:0] fh;wire [48:0] fl',1),
        ('wire [273:0] sh,sl,ih,il','wire [279:0] sh,ih;wire [307:0] sl,il',1),
        ('r_flags_conta c(','r_address_conta c(',1),('r_flags_proch h(','r_address_proch h(',1),
        ('r_flags_procl l(','r_address_procl l(',1),
        ('next_raw,priority_n,freeze,','next_raw,priority_n,freeze,rb_forward,rb_forward_n,',3),
        ('assign clocks={cl[13:12]','assign clocks={cl[24:14],ch[14],cl[13:12]',1),
        ('output wire [1:0] rm_flags_n);','output wire [1:0] rm_flags_n,output wire [31:0] address,output wire [3:0] select_n,output wire [1:0] forwarded_bypass);',1)]
    for before,after,count in edits:
        assert body.count(before)==count,(before,body.count(before));body=body.replace(before,after)
    body=body.replace(' wire freeze=fc[13];',' wire freeze=fc[13];\n wire rb_forward=fl[45],rb_forward_n=fl[46];')
    extra='''
 assign ih[279:274]=fixture_state[601:596];assign state_q[601:596]=sh[279:274];
 assign il[307:274]=fixture_state[635:602];assign state_q[635:602]=sl[307:274];
 assign address[7:0]={fl[13],fl[14],fl[15],fl[16],fh[13],fh[14],fh[15],fh[16]};
 assign address[15:8]={fl[25],fl[26],fl[27],fl[28],fl[29],fl[30],fl[31],fl[32]};
 assign address[23:16]={fl[17],fl[18],fl[19],fl[20],fl[21],fl[22],fl[23],fl[24]};
 assign address[31:24]={fl[33],fl[34],fl[35],fl[36],fl[37],fl[38],fl[39],fl[40]};
 assign select_n={fl[44],fl[43],fl[42],fl[41]};
 assign forwarded_bypass={fl[46],fl[45]};
'''
    body=body.replace('endmodule\n',extra+'endmodule\n');(out/'r_address_reference.sv').write_text(body)
    result=dict(boards=records,physical_bits=sum(r['physical_bits']for r in records),
        packages=sum(len(r['packages'])for r in records),original_pins=sum(r['original_pins']for r in records),
        source_audit=audit,scope=__doc__,native_integrated=False,
        boundary='Resolved front register-D inputs, RSTK, PrBlock, address comparators and Q14; RAM ports remain separate')
    (out/'manifest.json').write_text(json.dumps(result,indent=2)+'\n')
    print('R_ADDRESS_REFERENCE',result['physical_bits'],'bits',result['packages'],'packages',result['original_pins'],'pins')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,default=ROOT)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();generate(a.root.resolve(),a.output.resolve())
