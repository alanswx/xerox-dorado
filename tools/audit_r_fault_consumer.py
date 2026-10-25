#!/usr/bin/env python3
"""Bind H R-fault bit1 to the original Clock0 error-capture wiring.

This documents existing architectural stages for the remaining event-kernel
integration. It grants no multicycle exception, setup waiver or new timing
qualification, and does not equate raw g21 fault with registered RmPerr.
"""
import argparse
import json
from pathlib import Path
import shutil
from sil_netlist import load_board
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
FILES=['chm/sil/ProcH-Rev-Ce.dm!1_/ProcH-Rev-Ce.wl','verilog/generated/ProcH-Rev-Ce.v',
       'verilog/cells/cell_MC10106.v','verilog/cells/cell_MC10176.v','verilog/cells/cell_SE10210.v']


def run(out):
    assert not out.exists();b=load_board(str(ROOT/FILES[0]))
    pins={(p['pkg'],p['pin']):n for n,v in b.nets.items() for p in v['pins']}
    expected={('g21',2):'ProcH27.sil+9',('g21',9):'RbBypassDly',('g21',10):'RisIFdata',('g21',11):"R.Perr'",
              ('g21',15):'ProcH27.sil+10',('k21',7):'ProcH27.sil+9',('k21',4):'ProcH27.sil+3',
              ('k21',10):'ProcH27.sil+3',('k21',13):'RmPerr',('k21',9):"Clock0'Dd",
              ('k21',11):'ProcH27.sil+10',('k21',14):'ProcH27.sil+4',('k21',12):'ProcH27.sil+4',('k21',15):'TmPerr',
              ('i18',4):"Clock0'Dd",('i18',3):"Clock0'Da",('i18',5):"PreClock0'D",('h22',9):"Clock0'Da",
              ('l23',6):'ProcH27.sil+3',('l23',7):'ProcH27.sil+4'}
    for key,value in expected.items():assert pins[key]==value,(key,pins[key],value)
    for pkg,part in [('g21','MC10106'),('k21','MC10176'),('i18','SE10210'),('h22','MC10176')]:assert b.packages[pkg]['type']==part
    assert ('i18',6) not in pins and ('i18',7) not in pins
    assert 'assign p2 = ~(p9 | p10 | p11);' in (ROOT/FILES[2]).read_text()
    assert 'assign {p15, p14, p13, p4, p3, p2} = q;' in (ROOT/FILES[3]).read_text()
    assert 'assign p3 = (p5 | p6 | p7);' in (ROOT/FILES[4]).read_text() and 'assign p4 = (p5 | p6 | p7);' in (ROOT/FILES[4]).read_text()
    generated=(ROOT/FILES[1]).read_text()
    for a in ['.p7(ProcH27_sil_pl_9)','.p9(Clock0_p_Dd)','.p10(ProcH27_sil_pl_3)','.p13(RmPerr)']:assert a in generated
    out.mkdir(parents=True)
    for n in FILES:
        d=out/'inputs'/n;d.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(ROOT/n,d)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    result=dict(status='PASS_ORIGINAL_H_R_FAULT_CONSUMER_WIRING',pins_checked=len(expected),
        raw_fault="g21.2 = !(RbBypassDly | RisIFdata | R.Perr')",first_capture='k21.D2(p7) -> Q2(p4)',
        diagnostic_capture='k21.D3(p10)=old Q2 -> RmPerr(p13)',first_stage_error_consumers=['l23.p6: Q2','l23.p7: Q4'],
        capture_clock="Clock0'Dd, i18.p4; common source with h22 Clock0'Da at i18.p3",
        existing_architectural_k21_bits=6,new_state_added=0,error_event_migration_qualified=False,timing_exception_or_native_speed_qualified=False,
        files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file()})
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.output.resolve())
