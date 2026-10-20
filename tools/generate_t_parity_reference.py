#!/usr/bin/env python3
"""Extend the extracted T path with original parity RAM, capture and check pins.

R data/R-RAM parity and unused g16 inputs remain boundaries. Low g18 parity
fault inputs are now driven by original i19 logic. No candidate equations are
used in this graph. PARC's dictionary establishes g16's used parallel path.
"""
import argparse
import hashlib
import json
from pathlib import Path

from generate_processor_capture_reference import graph
from generate_t_file_reference import specs as t_specs, generate as generate_t
from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]
DICTIONARY='chm/sil/msa-Rev-Bg.dm!1_/ecldict.analyze'


def specs():
    result=t_specs();result[0]['module']='t_parity_conta'
    for low,s in zip([False,True],result[1:]):
        s['module']='t_parity_procl'if low else't_parity_proch'
        s['seq']={**s['seq'],('i13'if low else'i11'):(178,64),'g16':(242,4)}
        s['comb']=sorted(set(s['comb']+['k04','c06','d06']+(['i19']if low else[])))
        b=s['bindings']
        for name in ["T.Perr'","R.Perr'",'ProcL27.sil+8','ProcL27.sil+9']:
            b.pop(name,None)
        for bit in range(8):
            number=bit+(8 if low else 0);b[f'R.{number:02}']=f'pins[{127+number}]'
        b['ProcL27.sil+1'if low else'ProcH27.sil+1']='pins[144]'if low else'pins[143]'
        b['RbBypassDly']='pins[150]'if low else'pins[148]';b['RisIFdata']='pins[151]'if low else'pins[149]'
        if low:
            b.update({"StkPWriteEn'":'pins[146]','ProcL26.sil+3':'pins[147]',
                      "StkPSaveEn'":"1'b1","SHSelect'D":"1'b0","IfuRBaseSel'":"1'b1"})
        else:b['ProcH11.sil+14']='pins[145]'
        s['clock_taps']+=["Clock1'Da"]
        s['flag_taps']+=["T.Perr'","R.Perr'",'ProcL27.sil+9'if low else'ProcH27.sil+10',
                         'ProcL27.sil+8'if low else'ProcH27.sil+9']
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
        if s['bn'].startswith('Proc'):
            assert b.packages['g16']['type']=='F10000'
            wired={p['pin']for net in b.nets.values()for p in net['pins']if p['pkg']=='g16'}
            assert not (wired&{4,5,6,12}),('unused g16 modes are not tied low',s['bn'])
    records=[graph(root,out=out,**s)for s in specs()]
    for s in specs():
        p=out/(s['module']+'.sv');text=p.read_text();assert text.count('input wire [27:0] pins')==1
        p.write_text(text.replace('input wire [27:0] pins','input wire [151:0] pins,input wire [3:0] next_raw,input wire priority_n,freeze'))
    audit=generate_t(root,out/'t-data-source-audit')
    body=(out/'t-data-source-audit/t_file_reference.sv').read_text()
    for before,after,count in [('module t_file_reference','module t_parity_reference',1),
                               ('[403:0]','[539:0]',2),('[126:0]','[151:0]',1),
                               ('wire [8:0] ch,cl','wire [9:0] ch,cl',1),
                               ('wire [4:0] fh,fl','wire [8:0] fh,fl',1),
                               ('wire [177:0] sh,sl,ih,il','wire [245:0] sh,sl,ih,il',1),
                               ('t_file_conta c(','t_parity_conta c(',1),
                               ('t_file_proch h(','t_parity_proch h(',1),('t_file_procl l(','t_parity_procl l(',1),
                               ('hold_clocks,t_latch_n,bypass);','hold_clocks,t_latch_n,bypass,parity_clocks,output wire [3:0] parity_ok,faults);',1)]:
        assert body.count(before)==count,before;body=body.replace(before,after)
    extra=''' assign ih[241:178]=fixture_state[467:404];assign il[241:178]=fixture_state[531:468];
 assign ih[245:242]=fixture_state[535:532];assign il[245:242]=fixture_state[539:536];
 assign state_q[467:404]=sh[241:178];assign state_q[531:468]=sl[241:178];
 assign state_q[535:532]=sh[245:242];assign state_q[539:536]=sl[245:242];
 assign parity_clocks={cl[9],ch[9]};
 assign parity_ok={fl[6],fl[5],fh[6],fh[5]};assign faults={fl[8],fl[7],fh[8],fh[7]};
'''
    body=body.replace('endmodule\n',extra+'endmodule\n');(out/'t_parity_reference.sv').write_text(body)
    raw=(root/DICTIONARY).read_bytes();lines=raw.decode('latin-1').splitlines()
    excerpts=[]
    for i,line in enumerate(lines):
        if line.strip()in('F10000, F00','F10145A, F145A','MC170'):
            excerpts.append('\n'.join(lines[i:i+5]))
    manifest=dict(boards=records,physical_bits=sum(r['physical_bits']for r in records),
                  packages=sum(len(r['packages'])for r in records),
                  original_pins=sum(r['original_pins']for r in records),source_audit=audit,
                  dictionary=dict(path=DICTIONARY,sha256=hashlib.sha256(raw).hexdigest(),excerpts=excerpts),
                  boundary='R data/RAM parity, unused g16 inputs, R masks, inherited T and task producers')
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print('T_PARITY_REFERENCE',manifest['physical_bits'],'bits',manifest['packages'],'packages',manifest['original_pins'],'pins')
    return manifest


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,default=ROOT)
    p.add_argument('--output',type=Path);p.add_argument('--inspect',action='store_true');a=p.parse_args()
    if not a.inspect and not a.output:p.error('Specify --output or --inspect')
    generate(a.root.resolve(),a.output.resolve()if a.output else None,a.inspect)
