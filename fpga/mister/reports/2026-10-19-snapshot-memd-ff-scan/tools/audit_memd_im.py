#!/usr/bin/env python3
"""Review frozen MemD cache-RAM pin groups against the original wire list.

Logical grouping only: no RTL/state access, clock change, tie-off or timing
qualification. Unknown/stateful expressions stay independent opaque atoms.
"""
import argparse
from collections import defaultdict
import hashlib
import json
from pathlib import Path
import re

import audit_contb_im as logic
import audit_ifu_im as fanout

ROOT=Path(__file__).resolve().parents[3]
SIL=ROOT/'chm/sil/MemD-Rev-Ca.dm!1_/MemD-Rev-Ca.wl'
BOARD_SHA='679660eb90be88232d01f6f9626c1260330090a7882a8765f0a371f2d78acf9e'
ADDRESS=(2,3,4,5,6,7,8,10,11,12,13,14)
INSTANCES=re.compile(r'\b(cell_\w+)(\s*(?:#\(.*?\))?\s+)(u_\w+)\s*\((.*?)\)\s*;',re.S)
GROUPS=[
 '#-1c9 #-1c11 #-1c13 #-1c15 #-1c17 #-1c19', '#-1c3 #-1c5 #-1c7',
 '#-1f9 #-1f11 #-1f13 #-1f15 #-1f17 #-1f19', '#-1f3 #-1f5 #-1f7',
 '#-1i9 #-1i11 #-1i13 #-1i15 #-1i17 #-1i19', '#-1i3 #-1i5 #-1i7',
 '#-1l3 #-1l5 #-1l7 #-1l9 #-1l11 #-1l13 #-1l15 #-1l17 #-1l19',
 'a03 a05 a07 a09 a11 a13 a15 a17 a19','d03 d05 d07',
 'd09 d11 d13 d15 d17 d19','g03 g05 g07','g09 g11 g13 g15 g17 g19',
 'j03 j05 j07','j09 j11 j13 j15 j17 j19',
]


def review_network(text,root):
    # A generated storage-delay branch assigns an indexed vector. Do not
    # assume its value or choose one generate branch in a scalar pin proof.
    # If any assignment to a net is unsupported, keep ALL its assignments
    # opaque, including otherwise parseable aliases in another branch.
    opaque={}
    for target,rhs in re.findall(r'\bassign\s+(\w+)\s*=\s*([^;]+);',text):
        try:logic.expression(rhs)
        except (ValueError,SyntaxError):opaque[target]=rhs
    reduced=re.sub(r'\bassign\s+(\w+)\s*=\s*([^;]+);',
        lambda m:''if m[1]in opaque else m[0],text)
    resolve,sources=fanout.network(reduced,root)
    # Include parameterized instances even when they are opaque in the
    # scalar network helper; they remain part of the frozen dependency set.
    for kind in {m[1]for m in INSTANCES.finditer(text)}:
        p=root/'verilog/cells'/f'{kind}.v'
        sources[str(p.relative_to(root))]=hashlib.sha256(p.read_bytes()).hexdigest()
    return resolve,sources,opaque


def audit(root,text=None):
    path=root/'verilog/generated/MemD-Rev-Ca.v'
    if hashlib.sha256(path.read_bytes()).hexdigest()!=BOARD_SHA:raise ValueError('unreviewed frozen MemD')
    text=path.read_text()if text is None else text
    board=logic.load_board(str(SIL))
    sil={(p['pkg'],p['pin']):name for name,net in board.nets.items()for p in net['pins']}
    expected={pkg for pkg,p in board.packages.items()if p['type']=='F10470'}
    packages={'u_'+logic.vname(pkg):pkg for pkg in expected}
    if len(packages)!=72 or len(expected)!=72:raise ValueError('cache package naming collision/count')
    ram=[(n,p)for k,n,p in logic.instances(text)if k=='cell_F10470']
    if len(ram)!=72 or {n for n,_ in ram}!=set(packages):raise ValueError('all72 cache RAM packages required')
    resolve,sources,opaque=review_network(text,root);groups=defaultdict(list);literal=set()
    for name,pins in ram:
        pkg=packages[name]
        if pins.get('sys_clk')!='sys_clk':raise ValueError('cache guest clock changed')
        for pin in (*ADDRESS,15,16,17):
            if pins.get('p'+str(pin))!=logic.vname(sil[pkg,pin]):raise ValueError(f'cache pin differs:{pkg}.{pin}')
        field=sil[pkg,1]
        if pins.get('p1')!=logic.vname(field)+'__'+logic.vname(pkg)+'_1':raise ValueError('cache tied output differs:'+pkg)
        raw=tuple(pins['p'+str(pin)]for pin in (*ADDRESS,15,16));literal.add(raw)
        key=tuple(resolve(net)for net in raw)
        groups[key].append(dict(package=pkg,instance=name,field=field,data=sil[pkg,17],
            address=[sil[pkg,p]for p in ADDRESS],we=sil[pkg,15],ce=sil[pkg,16]))
    if {frozenset(x['package']for x in g)for g in groups.values()}!={frozenset(g.split())for g in GROUPS}:
        raise ValueError('independent cache address/write/select groups changed')
    if len(literal)!=19:raise ValueError('literal cache pin groups differ')
    rows=[dict(group=g[0]['package'],width=len(g),canonical_pins=key,members=g)for key,g in groups.items()]
    return dict(status='PASS: logical MemD cache pin grouping only',ram_packages=72,
        ram_bits=72*4096,raw_read_pipeline_bits=72,literal_groups=19,canonical_groups=14,
        groups=rows,unexpanded_board_assignments=opaque,sources=sources,
        board_sha256=BOARD_SHA,wirelist_sha256=hashlib.sha256(SIL.read_bytes()).hexdigest(),
        locations_sha256=hashlib.sha256(SIL.with_suffix('.lc').read_bytes()).hexdigest(),
        limits='Scalar Boolean identity only; stateful/unknown address replicas remain atoms. No RAM adapter, board-state classification, CE rewrite, global barrier, restore or FPGA timing proof. Indexed storage-delay branch stays opaque; no alternate parameter configuration is qualified.')


def controls(root):
    text=(root/'verilog/generated/MemD-Rev-Ca.v').read_text()
    m=next(m for m in INSTANCES.finditer(text)if m[1]=='cell_F10470');original=m[0]
    cases={
      'missing_package':'',
      'wrong_address':original.replace('.p2(Dad_00b)',".p2(1'b0)"),
      'wrong_write':original.replace('.p15(WriteD1_p_a)',".p15(1'b1)"),
      'wrong_select':original.replace('.p16(D1ACE_p_b)',".p16(1'b0)"),
      'wrong_data':original.replace('.p17(D1in_09)',".p17(1'b0)"),
      'wrong_output':original.replace('.p1(D_09___h__m_1c11_1)','.p1(D_08)'),
      'omitted_select':original.replace('    .p16(D1ACE_p_b),\n',''),
      'wrong_clock':original.replace('.sys_clk(sys_clk)',".sys_clk(1'b0)"),
    }
    result=[]
    for name,changed in cases.items():
        if changed==original:raise ValueError('missing failure anchor:'+name)
        try:audit(root,text[:m.start()]+changed+text[m.end():])
        except ValueError as e:result.append(dict(case=name,rejected=True,diagnostic=str(e)))
        else:raise AssertionError('damaged cache grouping accepted:'+name)
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source-root',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();q=audit(a.source_root.resolve())
    q['failure_controls']=controls(a.source_root.resolve());a.output.write_text(json.dumps(q,indent=2)+'\n')
    print('MemD logical grouping PASS:72x4096 RAM bits/72pipeline bits,19literal/14canonical groups;8 damaged boards reject')
