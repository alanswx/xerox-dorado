#!/usr/bin/env python3
"""Prove IFUM pin/fanout grouping from the frozen board and original wire list.

Review only: no RTL generation, CE tie-off, state access, timing or snapshot.
Pure scalar gates may be expanded; stateful/unknown outputs remain atoms.
"""
import argparse
import ast
from collections import defaultdict
import hashlib
import json
from pathlib import Path
import re

import audit_contb_im as logic

ROOT=Path(__file__).resolve().parents[3]
SIL=ROOT/'chm/sil/IFU-Rev-Ch.dm!1_/IFU-Rev-Ch.wl'
BOARD_SHA='b7f3b3275e8428fe03d6f3f645259a67205bcc382223267578503d492667d161'


def network(text,source_root):
    driver={};sources={}
    def add(name,fn):
        if name in driver:raise ValueError('multiple driver in IFUM pin cone: '+name)
        driver[name]=fn
    for target,rhs in re.findall(r'\bassign\s+(\w+)\s*=\s*([^;]+);',text):
        node=logic.expression(rhs)
        add(target,lambda resolve,node=node:logic.evaluate(node,resolve))
    for kind,name,pins in logic.instances(text):
        path=source_root/'verilog/cells'/f'{kind}.v';source=path.read_text()
        sources[str(path.relative_to(source_root))]=hashlib.sha256(path.read_bytes()).hexdigest()
        code=re.sub(r'/\*.*?\*/|//[^\n]*','',source,flags=re.S)
        inputs=set(re.findall(r'\binput\s+(?:wire\s+)?(p\d+)\b',code));local={}
        for variable,rhs in re.findall(r'\bwire\s+(\w+)\s*=\s*([^;]+);',code):
            try:local[variable]=logic.expression(rhs)
            except (ValueError,SyntaxError):pass
        def closed(node,seen=frozenset()):
            for n in ast.walk(node):
                if not isinstance(n,ast.Name)or n.id in inputs:continue
                if n.id not in local or n.id in seen or not closed(local[n.id],seen|{n.id}):return False
            return True
        for output,rhs in re.findall(r'\bassign\s+(p\d+)\s*=\s*([^;]+);',code):
            if output not in pins:continue
            try:node=logic.expression(rhs)
            except (ValueError,SyntaxError):continue
            if not closed(node):continue
            def fn(resolve,node=node,pins=pins,local=local,name=name):
                def get(p):
                    if p in local:return logic.evaluate(local[p],get)
                    # Never invent a constant for an unconnected gate input.
                    return resolve(pins[p])if p in pins else ('unconnected',name,p)
                return logic.evaluate(node,get)
            add(pins[output],fn)
    cache={};active=set()
    def resolve(name):
        name=name.strip()
        if name in ("1'b0","1'b1"):return int(name[-1])
        if name in cache:return cache[name]
        if name in active:raise ValueError('combinational IFUM pin-cone cycle')
        active.add(name);result=driver[name](resolve)if name in driver else ('net',name)
        active.remove(name);cache[name]=result;return result
    return resolve,sources


def audit(source_root,text=None):
    source=source_root/'verilog/generated/IFU-Rev-Ch.v'
    if hashlib.sha256(source.read_bytes()).hexdigest()!=BOARD_SHA:raise ValueError('unreviewed frozen IFU board')
    original=source.read_text();text=original if text is None else text
    board=logic.load_board(str(SIL));sil_pins={(p['pkg'],p['pin']):n for n,net in board.nets.items()for p in net['pins']}
    expected={pkg for pkg,r in board.packages.items()if r['type']=='F10415A'}
    ram=[(n,p)for k,n,p in logic.instances(text)if k=='cell_F10415A']
    if len(ram)!=27 or {n[2:]for n,_ in ram}!=expected:raise ValueError('all27 IFUM packages required')
    resolve,sources=network(text,source_root);groups=defaultdict(list);literal=set()
    for name,pins in ram:
        pkg=name[2:]
        if pins.get('sys_clk')!='sys_clk':raise ValueError('original IFUM guest clock changed')
        if (pkg,14)in sil_pins or 'p14'in pins:raise ValueError('unreviewed IFUM CE connection; original CE is omitted')
        for pin in (*logic.ADDRESS,13,15,1):
            if pins.get('p'+str(pin))!=logic.vname(sil_pins[pkg,pin]):raise ValueError(f'original IFUM pin mismatch: {pkg}.{pin}')
        raw=tuple(pins['p'+str(pin)]for pin in (*logic.ADDRESS,13))
        literal.add(raw);key=tuple(resolve(n)for n in raw)
        groups[key].append(dict(package=pkg,field=sil_pins[pkg,1],data=sil_pins[pkg,15],
            we=sil_pins[pkg,13],address=[sil_pins[pkg,pin]for pin in logic.ADDRESS],ce='OMITTED in both wire list and generated instance'))
    high={"InstrAddrK.%d'"%i for i in range(10)}|{'TwoAlphaK'}
    low={'SignK','MemBK34',"TypeJumpK'","TypePauseK'","RBaseSelK'"}|{'NK.%d'%i for i in range(4)}|{'MemBK.%d'%i for i in range(2)}|{"LengthK.%d'"%i for i in range(2)}|{'RamParity.%d'%i for i in range(3)}
    # The two programming words do NOT prove their separately registered
    # address replicas equal. Preserve all eleven independently addressed
    # groups; do not collapse stateful fanout into an architectural word.
    expected_groups={frozenset(group.split())for group in (
        'g09','g10 g11 g14 g15 h14','h09','h10','i09 i10 i15 j09','i14',
        'j10 j14 j15 k14 k15','k09 k10','l09','l10 l11 l14 l15','l12 l13')}
    if {frozenset(m['package']for m in members)for members in groups.values()}!=expected_groups:
        raise ValueError('independent IFUM address/write groups changed')
    rows=[];seen=set()
    for key,members in groups.items():
        fields={m['field']for m in members}
        if fields<=high:label='high'
        elif fields<=low:label='low'
        else:raise ValueError('IFUM field/parity coverage differs')
        if fields&seen:raise ValueError('duplicate IFUM output field')
        seen.update(fields);rows.append(dict(group=members[0]['package'],program_word=label,
            width=len(members),canonical_pins=key,members=members))
    if seen!=high|low:raise ValueError('missing IFUM output/parity field')
    return dict(status='PASS: logical pin/fanout grouping only',ram_packages=27,ram_bits=27648,
        read_pipeline_bits=27,data_bits=24,parity_bits=3,literal_groups=len(literal),
        canonical_groups=len(groups),program_word_widths={'high':11,'low':16},
        groups=sorted(rows,key=lambda r:r['group']),sources=sources,board_sha256=BOARD_SHA,
        wirelist_sha256=hashlib.sha256(SIL.read_bytes()).hexdigest(),
        locations_sha256=hashlib.sha256(SIL.with_suffix('.lc').read_bytes()).hexdigest(),
        limits='Pure scalar address/write fanout equality only. Two11/16-bit programming words do not justify merging the eleven separately addressed groups; stateful/unknown outputs remain independent. No global hold, restore or timing qualification. All27 original CE ports are omitted, not rewritten to zero; any explicit tie-off requires separate qualification.')


def controls(source_root):
    text=(source_root/'verilog/generated/IFU-Rev-Ch.v').read_text();rows=[]
    match=next(m for m in re.finditer(r'\bcell_F10415A\s+(\w+)\s*\((.*?)\)\s*;',text,re.S))
    original=match[0]
    cases={
        'missing_package':'',
        'wrong_address':original.replace('.p2(InstrSet_0a)',".p2(1'b0)"),
        'wrong_write':original.replace('.p13(DecHi_u__p_)',".p13(1'b1)"),
        'wrong_data':original.replace('.p15(RcvdBMux_05)',".p15(1'b0)"),
        'wrong_output':original.replace('.p1(TwoAlphaK)','.p1(SignK)'),
        'new_ce':original.replace('.sys_clk(sys_clk)',".sys_clk(sys_clk),.p14(1'b0)"),
        'wrong_clock':original.replace('.sys_clk(sys_clk)',".sys_clk(1'b0)"),
    }
    for name,changed in cases.items():
        if changed==original:raise ValueError('missing negative anchor: '+name)
        damaged=text[:match.start()]+changed+text[match.end():]
        try:audit(source_root,damaged)
        except ValueError as error:rows.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise ValueError('damaged IFUM grouping accepted: '+name)
    return rows


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--source-root',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
    root=args.source_root.resolve();result=audit(root);result['failure_controls']=controls(root)
    args.output.write_text(json.dumps(result,indent=2)+'\n')
    print('IFUM grouping PASS:',[(r['group'],r['width'])for r in result['groups']],';7 damaged boards rejected; CE unchanged/omitted')
