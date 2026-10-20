#!/usr/bin/env python3
"""Extract l06+l07's full wired-OR sign/odd nets from original PARC wiring."""
import argparse
import json
from pathlib import Path
from generate_processor_capture_reference import graph
from generate_r_operand_reference import specs as operand_specs, generate as generate_operand

ROOT=Path(__file__).resolve().parents[1]


def specs():
    result=operand_specs();result[0]['module']='r_flags_conta'
    for low,s in zip((False,True),result[1:]):
        s['module']='r_flags_procl' if low else 'r_flags_proch'
        s['seq']['l06']=(272,2)
        s['clock_taps']+=['@l06.p7','@l06.p9']
        s['flag_taps']+=["RmOdd'" if low else "RmLtZero'"]
    return result


def generate(root,out,inspect=False):
    if inspect:
        for s in specs():
            external=graph(root,out=None,inspect=True,**s)
            assert not set(external)-set(s['bindings']),(s['bn'],external)
        return
    out.mkdir(parents=True,exist_ok=False)
    records=[graph(root,out=out,**s) for s in specs()]
    for s in specs():
        p=out/(s['module']+'.sv');text=p.read_text()
        assert text.count('input wire [27:0] pins')==1
        p.write_text(text.replace('input wire [27:0] pins',
            'input wire [151:0] pins,input wire [3:0] next_raw,input wire priority_n,freeze'))
    audit=generate_operand(root,out/'operand-source-audit')
    body=(out/'operand-source-audit/r_operand_reference.sv').read_text()
    edits=[('module r_operand_reference','module r_flags_reference',1),
        ('[591:0]','[595:0]',2),('[28:0]','[32:0]',1),
        ('wire [11:0] ch,cl','wire [13:0] ch,cl',1),
        ('wire [11:0] fh,fl','wire [12:0] fh,fl',1),
        ('wire [271:0] sh,sl,ih,il','wire [273:0] sh,sl,ih,il',1),
        ('r_operand_conta c(','r_flags_conta c(',1),
        ('r_operand_proch h(','r_flags_proch h(',1),
        ('r_operand_procl l(','r_flags_procl l(',1),
        ('output wire [3:0] r_flags);','output wire [3:0] r_flags,output wire [1:0] rm_flags_n);',1),
        ('assign clocks={cl[11:10]','assign clocks={cl[13:12],ch[13:12],cl[11:10]',1)]
    for before,after,count in edits:
        assert body.count(before)==count,(before,body.count(before))
        body=body.replace(before,after)
    extra=''' assign ih[273:272]=fixture_state[593:592];assign il[273:272]=fixture_state[595:594];
 assign state_q[593:592]=sh[273:272];assign state_q[595:594]=sl[273:272];
 assign rm_flags_n={fl[12],fh[12]};
'''
    body=body.replace('endmodule\n',extra+'endmodule\n')
    (out/'r_flags_reference.sv').write_text(body)
    manifest=dict(boards=records,physical_bits=sum(r['physical_bits']for r in records),
        packages=sum(len(r['packages'])for r in records),
        original_pins=sum(r['original_pins']for r in records),source_audit=audit,
        boundary='Complete l06/l07 sign/odd; functional RM/STK and inherited producers remain external')
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print('R_FLAGS_REFERENCE',manifest['physical_bits'],'bits',manifest['packages'],'packages',manifest['original_pins'],'pins')
    return manifest


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,default=ROOT)
    p.add_argument('--output',type=Path);p.add_argument('--inspect',action='store_true');a=p.parse_args()
    if not a.inspect and not a.output:p.error('Specify --output or --inspect')
    generate(a.root.resolve(),a.output.resolve()if a.output else None,a.inspect)
