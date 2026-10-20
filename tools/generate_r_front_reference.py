#!/usr/bin/env python3
"""Extract RBase, stack pointer/saved address and complete bypass comparison cones.

The original final R-address/processor graph is retained. MIR/IFU/ALU/counter
and I/O producer boundaries remain explicit, with no non-rail constant stubs
in the extended processor graphs. Every consumed mixed net keeps all drivers.
"""
import argparse
import json
from pathlib import Path
import re

from generate_processor_capture_reference import graph
from generate_r_address_reference import specs as address_specs, generate as generate_address
from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]
EXTRA_SEQ={'g14':(308,4),'g15':(312,64),'h14':(376,6),
           'l14':(382,6),'l15':(388,2),'i16':(390,4),'i17':(394,4),
           'l16':(398,4),'l18':(402,4),'j19':(406,2)}
HIGH_COMB=['b23','b24','c23','d20','d22','d23','d24','e23','e24','f23','f24','g20','k05']
LOW_COMB=['b22','b23','b24','c23','d20','d23','d24','e19','e21','e23','e24','f22','f23','f24',
          'g19','g20','g21','g22','h11','h16','h18','h19','h20','i10','j13','j14','j15',
          'j16','j17','j20','k12','k13','k14','k15','k16','k17','k18','k19','k20',
          'l12','l13','l17','l19','a19']


def specs(root=ROOT):
    result=address_specs(root);result[0]['module']='r_front_conta'
    signals={};next_pin=202
    def external(name):
        nonlocal next_pin
        if name not in signals:signals[name]=next_pin;next_pin+=1
        return f'pins[{signals[name]}]'
    for low,s in zip((False,True),result[1:]):
        s['module']='r_front_procl'if low else'r_front_proch'
        old=dict(s['bindings'])
        s['comb']=sorted(set(s['comb']+(LOW_COMB if low else HIGH_COMB)))
        if low:s['seq'].update(EXTRA_SEQ)
        names=graph(root,out=None,inspect=True,**s)
        bindings={}
        for n in sorted(names):
            if re.fullmatch(r'FF\.[0-7]',n):bindings[n]=f'pins[{19-int(n[-1])}]'
            elif n=='Freeze':bindings[n]='freeze'
            elif n in ("FFok'a","FFok'b"):bindings[n]=external("FFok'common")
            elif n in old and old[n]not in ("1'b0","1'b1"):bindings[n]=old[n]
            else:bindings[n]=external(n)
        # The two outputs of the SAME unmodelled pipeline flip-flop form a
        # complementary boundary, rather than two independently driven pins.
        if low:
            md=external("_MD'Dly")
            bindings["_MD'Dly"]=md
            if 'ProcL20.sil+7'in names:bindings['ProcL20.sil+7']='~'+md
        s['bindings']=bindings
        s['flag_taps'] += ["FA=0'b","FA=1'a","FA=2'a","FA=3'"]
        if low:
            s['clock_taps']+=['@g14.p7','@g15.p13','@h14.p9','@l14.p9',
                             '@l15.ca','@l15.cb','@i16.p7','@i17.p7',
                             '@l16.clk','@l18.clk','@j19.ca','@j19.cb']
        board=load_board(str(root/'chm/sil'/(s['bn']+'.dm!1_')/(s['bn']+'.wl')))
        chosen=set(s['comb'])|set(s['seq'])
        for n,net in board.nets.items():
            drivers={p['pkg']for p in net['pins']if p['dir']=='out'}
            if any(p['pkg']in chosen and p['dir']=='in'for p in net['pins'])and drivers&chosen:
                assert drivers<=chosen,(s['bn'],n,sorted(drivers-chosen))
        for n in bindings:
            assert not n.startswith(('FA=','FB=','FC=','FFeq')),('Synthetic decoded boundary retained',s['bn'],n)
        # Publish actual physical sources for every inherited single-pin
        # boundary that became internal (or a complementary source alias).
        resolved=[]
        for n,expression in old.items():
            m=re.fullmatch(r'pins\[(\d+)\]',expression)
            if not m:continue
            drivers={p['pkg']for p in board.nets[n]['pins']if p['dir']=='out'}
            if drivers&chosen or bindings.get(n,expression)!=expression:
                resolved.append(dict(pin=int(m[1]),net=n,flag=len(s['flag_taps'])))
                s['flag_taps'].append(n)
        s['_resolved']=resolved
    return result,signals,next_pin



def audit_ffok_alias(root):
    """Verify that the two decoder boundary nets have identical wired drivers."""
    board=load_board(str(root/'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl'))
    assert board.packages['f24']['type']=='MC10211'
    expected={"FFok'a":{4,13},"FFok'b":{2,12},"FFok'c":{3,14}}
    observed={}
    for name,pins in expected.items():
        drivers={(p['pkg'],p['pin'])for p in board.nets[name]['pins']if p['dir']=='out'}
        assert drivers=={('f24',pin)for pin in pins},(name,drivers)
        observed[name]=sorted(drivers)
    # The first NOR has replicated outputs 2,3,4; the second 12,13,14.
    # Each external net ORs exactly one output of each NOR, so all are equal.
    return dict(package='ContA.f24',type='MC10211',drivers=observed,
                reason='Each wired-OR net combines one replica from each identical NOR output group.')


def generate(root,out):
    out.mkdir(parents=True,exist_ok=False)
    plans,signals,pin_count=specs(root);records=[]
    for plan in plans:
        s={k:v for k,v in plan.items()if not k.startswith('_')}
        records.append(graph(root,out=out,**s))
        p=out/(s['module']+'.sv');raw=p.read_text();assert raw.count('input wire [27:0] pins')==1
        raw=raw.replace('input wire [27:0] pins',
            f'input wire [{pin_count-1}:0] pins,input wire [3:0] next_raw,input wire priority_n,freeze,rb_forward,rb_forward_n')
        if s['bn']=='ProcL-Rev-Ci':
            for name in ('l16','l18'):
                before=' original_F10000 '+name+'(';assert raw.count(before)==1
                raw=raw.replace(before,' original_F10000_gated '+name+'(')
        p.write_text(raw)
    generate_address(root,out/'address-source-audit')
    body=(out/'address-source-audit/r_address_reference.sv').read_text()
    hflags=len(plans[1]['flag_taps']);lflags=len(plans[2]['flag_taps'])
    edits=[('module r_address_reference','module r_front_reference',1),('[635:0]','[735:0]',2),
           ('[201:0] pins',f'[{pin_count-1}:0] pins',1),('[44:0] clocks','[56:0] clocks',1),
           ('wire [24:0] cl','wire [36:0] cl',1),
           ('wire [16:0] fh;wire [48:0] fl',f'wire [{hflags-1}:0] fh;wire [{lflags-1}:0] fl',1),
           ('wire [307:0] sl,il','wire [407:0] sl,il',1),
           ('r_address_conta c(','r_front_conta c(',1),('r_address_proch h(','r_front_proch h(',1),
           ('r_address_procl l(','r_front_procl l(',1),
           ('assign clocks={cl[24:14]','assign clocks={cl[36:25],cl[24:14]',1),
           ('output wire [1:0] forwarded_bypass);',
            'output wire [1:0] forwarded_bypass,output wire [201:0] source_pins,output wire [7:0] fa_n,output wire [43:0] front_data);',1)]
    for before,after,count in edits:
        assert body.count(before)==count,(before,body.count(before));body=body.replace(before,after)
    extra=[' assign il[407:308]=fixture_state[735:636];assign state_q[735:636]=sl[407:308];',
           ' assign fa_n={fl[52:49],fh[20:17]};']
    resolved={}
    for spec,flags in zip(plans[1:],('fh','fl')):
        for row in spec['_resolved']:
            pin=row['pin'];assert pin not in resolved,(pin,resolved[pin],row)
            resolved[pin]=f"{flags}[{row['flag']}]"
    for pin in range(202):extra.append(f" assign source_pins[{pin}]={resolved.get(pin,f'pins[{pin}]')};")
    # These outputs observe physical D/address pins. The sequential reference
    # still captures them independently at its actual delayed gate edges.
    fields=[(0,4,'l.g14.p9 ? {l.g14.p11,l.g14.p13,l.g14.p4,l.g14.p6} : {l.g14.p10,l.g14.p12,l.g14.p3,l.g14.p5}'),
            (4,4,'{l.g15.p12,l.g15.p11,l.g15.p4,l.g15.p5}'),
            (8,4,'{l.g15.p10,l.g15.p9,l.g15.p7,l.g15.p6}'),
            (12,6,'{l.h14.p12,l.h14.p11,l.h14.p10,l.h14.p7,l.h14.p6,l.h14.p5}'),
            (18,6,'{l.l14.p12,l.l14.p11,l.l14.p10,l.l14.p7,l.l14.p6,l.l14.p5}'),
            (24,2,'{l.l15.p10,l.l15.p7}'),
            (26,4,'l.i16.p9 ? {l.i16.p11,l.i16.p13,l.i16.p4,l.i16.p6} : {l.i16.p10,l.i16.p12,l.i16.p3,l.i16.p5}'),
            (30,4,'l.i17.p9 ? {l.i17.p11,l.i17.p13,l.i17.p4,l.i17.p6} : {l.i17.p10,l.i17.p12,l.i17.p3,l.i17.p5}'),
            (34,4,'{l.l16.p11,l.l16.p10,l.l16.p9,l.l16.p7}'),
            (38,4,'{l.l18.p11,l.l18.p10,l.l18.p9,l.l18.p7}'),
            (42,2,'{l.j19.p10,l.j19.p7}')]
    for base,width,expression in fields:extra.append(f' assign front_data[{base}+:{width}]={expression};')
    body=body.replace('endmodule\n','\n'.join(extra)+'\nendmodule\n')
    (out/'r_front_reference.sv').write_text(body)
    low_board=load_board(str(root/'chm/sil/ProcL-Rev-Ci.dm!1_/ProcL-Rev-Ci.wl'))
    bypass_drivers={}
    for name,count in (("RmBypass'",9),("StkBypass'",8)):
        drivers=sorted((p['pkg'],p['pin'])for p in low_board.nets[name]['pins']if p['dir']=='out')
        assert len(drivers)==count,(name,drivers)
        assert all(pkg in plans[2]['comb']for pkg,pin in drivers),(name,drivers)
        bypass_drivers[name]=drivers
    result=dict(boards=records,signals=signals,pin_count=pin_count,
        ffok_alias_audit=audit_ffok_alias(root),bypass_drivers=bypass_drivers,
        resolved={s['bn']:s.get('_resolved',[])for s in plans},
        physical_bits=sum(r['physical_bits']for r in records),
        packages=sum(len(r['packages'])for r in records),
        original_pins=sum(r['original_pins']for r in records),
        scope=__doc__,native_integrated=False)
    (out/'manifest.json').write_text(json.dumps(result,indent=2)+'\n')
    print('R_FRONT_REFERENCE',result['physical_bits'],'bits',result['packages'],'packages',pin_count,'input pins')
    print('R_FRONT_SIGNALS',json.dumps(signals,sort_keys=True))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,default=ROOT)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();generate(a.root.resolve(),a.output.resolve())
