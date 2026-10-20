#!/usr/bin/env python3
"""Prove the binary fixed-point optimization without re-encoding the circuit.

The clock and transfer functions are arbitrary deterministic functions in SMT.
If an iteration preserves both Q and clock history, it is an absorbing fixed
point, including accumulated/repeated edge flags. Continuing fixed iterations
therefore preserves the guarded evaluator's result. This proves this rewrite,
not analog timing or correctness of the original circuit functions.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

ROOT=Path(__file__).resolve().parents[1]
STEP='''levels=clocks(q,clock_after,controls_after[0]);captures=levels&~previous;
repeated_events=repeated_events|(captures&clock_events);
clock_events=clock_events|captures;n=transfer(q,captures,controls_after);
settled=(n==q&&levels==previous);previous=levels;q=n;'''
SMT='''(set-logic QF_UFBV)
(declare-fun C ((_ BitVec 32)) (_ BitVec 9))
(declare-fun T ((_ BitVec 32) (_ BitVec 9)) (_ BitVec 32))
(declare-const q0 (_ BitVec 32))
(declare-const p0 (_ BitVec 9))
(declare-const a0 (_ BitVec 9))
(declare-const r0 (_ BitVec 9))
(define-fun l0 () (_ BitVec 9) (C q0))
(define-fun e0 () (_ BitVec 9) (bvand l0 (bvnot p0)))
(define-fun q1 () (_ BitVec 32) (T q0 e0))
(define-fun a1 () (_ BitVec 9) (bvor a0 e0))
(define-fun r1 () (_ BitVec 9) (bvor r0 (bvand e0 a0)))
(define-fun l1 () (_ BitVec 9) (C q1))
(define-fun e1 () (_ BitVec 9) (bvand l1 (bvnot l0)))
(define-fun q2 () (_ BitVec 32) (T q1 e1))
(define-fun a2 () (_ BitVec 9) (bvor a1 e1))
(define-fun r2 () (_ BitVec 9) (bvor r1 (bvand e1 a1)))
(assert STABLE)
(assert (not (and (= q2 q1) (= l1 l0) (= a2 a1) (= r2 r1))))
(check-sat)
'''


def compact(raw):
    return re.sub(r'\s+','',re.sub(r'//[^\n]*|/\*.*?\*/','',raw,flags=re.S))


def prove(baseline,out):
    current=ROOT/'verilog/rtl/dorado_control_clock_step.sv'
    old=compact(baseline.read_text());new=compact(current.read_text());step=compact(STEP)
    prefix='for(integerwave=0;wave<WAVES;wave=wave+1)begin'
    before=prefix+'if(!settled)begin'+step+'endend'
    after=prefix+step+'end'
    assert old.count(before)==1 and old.replace(before,after)==new,'Unreviewed RTL rewrite'
    out.mkdir(parents=True,exist_ok=False);cases=[]
    for name,condition,expected in [
        ('fixed_point','(and (= q1 q0) (= l0 p0))','unsat'),
        ('bad_q_only','(= q1 q0)','sat'),
        ('bad_no_edges_only','(= e0 (_ bv0 9))','sat')]:
        smt=SMT.replace('STABLE',condition);path=out/(name+'.smt2');path.write_text(smt)
        result=subprocess.run(['z3',str(path)],capture_output=True,text=True,check=True)
        (out/(name+'.log')).write_text(result.stdout+result.stderr)
        assert result.stdout.strip()==expected,(name,result.stdout,result.stderr)
        cases.append(dict(case=name,result=expected))
    record=dict(status='PASS_BINARY_FIXED_POINT_REWRITE',baseline_sha256=hashlib.sha256(baseline.read_bytes()).hexdigest(),
                current_sha256=hashlib.sha256(current.read_bytes()).hexdigest(),cases=cases,
                scope=__doc__,rtl_delta='Only early-exit guard removed; functions and remaining RTL identical after removing comments/whitespace.')
    (out/'proof.json').write_text(json.dumps(record,indent=2)+'\n');print(record['status'],'arbitrary deterministic functions;two invalid criteria admit counterexamples')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--baseline',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();prove(a.baseline.resolve(),a.output.resolve())
