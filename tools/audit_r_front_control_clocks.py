#!/usr/bin/env python3
"""Map full-reference clock channels to original board taps and missing cases.

In particular, unused MemC l02.B still has a live common clock. An unwired
per-half enable does not make this channel constant or waive its coverage.
This is a source audit, not an arrival, native boot or FPGA timing proof.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def audit(reference,out):
    assert not out.exists();meta=json.loads((reference/'manifest.json').read_text())
    raw=(reference/'r_front_control_reference.sv').read_text()
    expression=re.search(r'assign clocks=\{([^}]+)\}',raw)[1];bits=[]
    for token in expression.split(',')[::-1]:
        m=re.fullmatch(r'(cc|ch|cl|memory_clocks)(?:\[(\d+)(?::(\d+))?\])?',token);assert m,token
        name,hi,lo=m.groups();hi=int(hi)if hi else 30;lo=int(lo)if lo else hi if name!='memory_clocks'else 0
        bits.extend((name,i)for i in range(lo,hi+1))
    assert len(bits)==100
    boards={r['board'].split('-Rev-')[0]:r for r in meta['boards']}
    names={'cc':'ContA','ch':'ProcH','cl':'ProcL'};rows=[]
    for channel,(name,index)in enumerate(bits):
        board=names.get(name)
        if board is None:board='ContB'if index<25 else'MemC';index=index if index<25 else index-25
        rows.append(dict(channel=channel,board=board,tap=boards[board]['clock_taps'][index]))
    expected={43:('ProcL','@l11.cb'),91:('ContB',"ManClk.0'"),92:('ContB',"ManClk.7'"),93:('ContB',"ManClk.1'"),95:('MemC','@l02.cb')}
    for i,p in expected.items():assert(rows[i]['board'],rows[i]['tap'])==p
    inputs={};bindings={}
    for name,pkg in [('MemC','l02'),('ProcL','l11')]:
        bn=boards[name]['board'];path=ROOT/'chm/sil'/(bn+'.dm!1_')/(bn+'.wl');inputs[str(path.relative_to(ROOT))]=sha(path)
        b=load_board(str(path));assert b.packages[pkg]['type']=='MC10231'
        pins={p['pin']:n for n,r in b.nets.items()for p in r['pins']if p['pkg']==pkg};bindings[name+'.'+pkg]=pins
    mc=bindings['MemC.l02'];pl=bindings['ProcL.l11']
    assert mc[9]=="MemClkEnable'a"and mc[13]=='Hia'and mc[14]==mc[15]=='Hia'
    assert all(i not in mc for i in [10,11,12])
    assert pl[9]=="Clock0'Bd"and pl[11]=="FFeqMul'"and pl[10]=='Q.14'
    # Both independent reference and current native primitive explicitly
    # include common pin9 in the B-half clock. Freeze both source identities.
    cells=['verilog/verilator/run_control_original_ff.sv','verilog/cells/cell_MC10231.v']
    for n in cells:
        p=ROOT/n;s=p.read_text();inputs[n]=sha(p)
        assert re.search(r'\b(?:cb|clkb)\s*=\s*p9\s*\|\s*p11\b',s),n
    inputs['reference/manifest.json']=sha(reference/'manifest.json')
    inputs['reference/r_front_control_reference.sv']=sha(reference/'r_front_control_reference.sv')
    result=dict(status='PASS_ORIGINAL_CLOCK_CHANNEL_SOURCE_AUDIT',scope=__doc__,channels=rows,previously_missing=list(expected),
        original_pin_bindings=bindings,inputs=inputs,unused_MemC_half_clock_is_constant=False,
        MemC_half_clock="p9 | p11 = MemClkEnable'a | 0",MemC_B_data_grounded=True,MemC_B_reset='Hia = QB OR complemented QB',
        required='Exercise multiply Q.14 capture, all three actual maintenance release edges and the original MemClkEnable common-clock edge; compare all100 clock channels.',
        arrival_qualified=False,physical_timing_qualified=False)
    out.mkdir(parents=True);(out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status']);return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--reference',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();audit(a.reference.resolve(),a.output.resolve())
