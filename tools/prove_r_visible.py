#!/usr/bin/env python3
"""Prove the source-bound visible-R rewrite for all binary data and controls.

This is a combinational model proof, not analog skew, full-machine or native
qualification. Both gates belong to the original MC10173 latch chain.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
from rewrite_r_visible import rewrite
from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]
BASE=ROOT/'build/rtl-executive/r-front-mir-mask-timing-input-v1-20261023'


def clock_wiring():
    records=[]
    for bn,clock,enable in [('ProcH-Rev-Ce',"CLK.ph'","CLKEnable'c"),
                             ('ProcL-Rev-Ci',"CLK.pl'","CLKEnable'b")]:
        path=ROOT/'chm/sil'/(bn+'.dm!1_')/(bn+'.wl')
        b=load_board(str(path))
        pins={(p['pkg'],p['pin']):n for n,net in b.nets.items()for p in net['pins']}
        phase="pClock1En'"if bn.startswith('ProcH')else"pClk1En'"
        expected={('l01',11):clock,('l01',9):enable,('l01',13):"ppClk'a",
                  ('l02',9):clock,('l02',6):enable,('l02',2):phase,
                  ('f12',5):phase,('f12',11):"ppClk'a",('f12',13):'PreClock1B',
                  ('i05',7):'PreClock1B',('i05',3):"Clock1'Ba",('i05',4):"Clock1'Bb",
                  ('g12',6):phase,('g12',2):"PreSHCP'B"}
        assert pins['f12',9]==pins['f12',2]
        for key,value in expected.items():assert pins[key]==value,(bn,key)
        for key in [('f12',6),('f12',7),('f12',10),('i05',5),('i05',6),('g12',5),('g12',7)]:
            assert key not in pins,(bn,key,'Expected unwired LOW input')
        for pkg in ['g05','h05']:
            assert pins[pkg,7]=="Clock1'Bb"
        for pkg in ['j01','j02']:
            assert pins[pkg,7]==("Clock1'Ba"if pkg=='j01'else"Clock1'Bb")
        for pkg in ['g06','g07']:
            assert pins[pkg,7]=="PreSHCP'B"
        for pkg,kind in [('l01','SE10210'),('l02','SE10231'),('f12','SE10212'),
                         ('i05','SE10212'),('g12','SE10211')]+[(p,'MC10173')for p in ['g05','h05','g06','g07','j01','j02']]:
            assert b.packages[pkg]['type']==kind,(bn,pkg)
        for key in [('l01',13),('l02',2),('f12',2),('f12',13),('i05',3),('i05',4),('g12',2)]:
            assert {(p['pkg'],p['pin'])for p in b.nets[pins[key]]['pins']if p['dir']=='out'}=={key}
        records.append(dict(board=bn,wire_list=str(path.relative_to(ROOT)),
            sha256=hashlib.sha256(path.read_bytes()).hexdigest(),
            read_clock="Clock1'Bb = ppClk'a | "+phase,
            visible_clock="PreSHCP'B = ~"+phase,
            source_pins_checked=len(expected),unwired_low_inputs_checked=7))
    return records


def prove(out):
    out.mkdir(parents=True,exist_ok=False)
    wiring=clock_wiring()
    operand=BASE/'dorado_local_r_operand_step.sv'
    tfile=BASE/'dorado_local_t_file_step.sv'
    raw=operand.read_text();t=tfile.read_text()
    assert hashlib.sha256(t.encode()).hexdigest()=='d08dd5ea8df7b1f50978cb1d63e9593f8d04e11164b85f8cf92fa599c50976d6'
    for s in ['assign latch_n={pp_l | core_next[48],pp_h | core_next[32]};',
              'assign t_latch_n={~core_next[48],~core_next[32]};']:
        assert s in t
    assert 'assign r_read_n=latch_n;assign r_visible_n=t_latch_n;'in raw
    for half,flag,a,b in [(0,589,'547:540','563:556'),(1,591,'555:548','571:564')]:
        assert f'if(!r_read_n[{half}]) begin'in raw
        assert f'if(new_r_clocks[{1+2*half}]) state_next[{flag}]='in raw
    candidate=out/'candidate.sv';candidate.write_bytes(operand.read_bytes())
    transform=rewrite(candidate)
    declaration='''(set-logic QF_BV)
(declare-const pp Bool)(declare-const phase Bool)
(declare-const old_a (_ BitVec 8))(declare-const old_b (_ BitVec 8))
(declare-const incoming_a (_ BitVec 8))(declare-const incoming_b (_ BitVec 8))
(declare-const old_visible (_ BitVec 8))
(declare-const old_select Bool)(declare-const incoming_select Bool)
'''
    cases=[]
    for label,read,visible in [('complementary','(not (or pp phase))','phase'),
                              ('overlap_read','(not pp)','phase'),
                              ('overlap_visible','(not (and pp phase))','(not phase)')]:
        smt=declaration+f'''
(define-fun read_open () Bool {read})
(define-fun visible_open () Bool {visible})
(define-fun next_a () (_ BitVec 8) (ite read_open incoming_a old_a))
(define-fun next_b () (_ BitVec 8) (ite read_open incoming_b old_b))
(define-fun next_select () Bool (ite read_open incoming_select old_select))
(define-fun original () (_ BitVec 8) (ite visible_open (ite next_select next_a next_b) old_visible))
(define-fun rewritten () (_ BitVec 8) (ite visible_open (ite old_select old_a old_b) old_visible))
(assert (not (= original rewritten)))
(check-sat)
'''
        p=out/(label+'.smt2');p.write_text(smt)
        r=subprocess.run(['z3','-smt2',str(p)],capture_output=True,text=True)
        (out/(label+'.log')).write_text(r.stdout+r.stderr)
        assert r.returncode==0
        result=r.stdout.strip();assert result==('unsat'if label=='complementary'else'sat'),(label,result)
        cases.append(dict(case=label,result=result,returncode=r.returncode))
    record=dict(status='PASS_BINARY_R_VISIBLE_NONOVERLAP',cases=cases,transform=transform,original_clock_wiring=wiring,
                inputs={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in [operand,tfile]},
                full_next_state_equivalence_proven=False,analog_skew_qualified=False,native_integrated=False)
    (out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    print(record['status'],transform['output_sha256'])


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    prove(p.parse_args().output.resolve())
