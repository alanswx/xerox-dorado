#!/usr/bin/env python3
"""Prove the original three-latch flag output equals the factored source mux.

This source-bound binary proof preserves every physical latch update, including
HIGH-transparent data priority and closed set. It is not an analog-skew proof.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
from rewrite_r_flags import rewrite

ROOT=Path(__file__).resolve().parents[1]
BASE=ROOT/'build/rtl-executive/r-front-visible-timing-input-v1-20261023'


def prove(out):
    out.mkdir(parents=True,exist_ok=False)
    flags=BASE/'dorado_local_r_flags_step.sv';operand=BASE/'dorado_local_r_operand_step.sv'
    raw=flags.read_text();op=operand.read_text()
    assert hashlib.sha256(op.encode()).hexdigest()=='2e760e63e1a4332b4f0af567b22044fff5f12e8e6ee04e672030a15a2733724b'
    for snippet in ['wire [1:0] new_md={child_next[51],child_next[34]};',
                    'wire [1:0] new_bypass={pins_after[150],pins_after[148]};',
                    'if(new_clocks[2*half]) state_next[592+2*half]=pins_after[half ? 107 : 92];',
                    'else if(!r_read_n[half] && (!new_bypass[half] || !new_md[half]))',
                    'if(new_clocks[2*half+1]) state_next[593+2*half]=pins_after[half ? 91 : 76];',
                    'else if(!r_read_n[half] && (!new_bypass[half] || new_md[half]))']:
        assert snippet in raw,snippet
    for snippet in ['if(new_r_clocks[0]) state_next[588]=pins_after[127];',
                    'if(new_r_clocks[2]) state_next[590]=pins_after[142];',
                    'if(!r_read_n[0] && pins_after[148]) state_next[588]=1;',
                    'if(!r_read_n[1] && pins_after[150]) state_next[590]=1;']:
        assert snippet in op,snippet
    candidate=out/'candidate.sv';candidate.write_bytes(flags.read_bytes());transform=rewrite(candidate)
    declaration='''(set-logic QF_UF)
(declare-const read_open Bool)(declare-const bypass Bool)(declare-const md Bool)
(declare-const md_bit Bool)(declare-const pd_bit Bool)(declare-const ram_bit Bool)
(declare-const old_md Bool)(declare-const old_pd Bool)(declare-const old_ram Bool)
(define-fun md_clock () Bool (and read_open bypass md))
(define-fun pd_clock () Bool (and read_open bypass (not md)))
(define-fun next_md () Bool (ite md_clock md_bit (ite (and read_open (or (not bypass) (not md))) true old_md)))
(define-fun next_pd () Bool (ite pd_clock pd_bit (ite (and read_open (or (not bypass) md)) true old_pd)))
(define-fun next_ram () Bool (ite (and read_open bypass) true (ite (and read_open (not bypass)) ram_bit old_ram)))
(define-fun original () Bool (not (and next_md next_pd next_ram)))
'''
    cases=[]
    for label,ram,choose_md in [('original','ram_bit','md'),('stale_ram','old_ram','md'),('swap_md','ram_bit','(not md)')]:
        smt=declaration+f'''
(define-fun selected () Bool (ite bypass (ite {choose_md} md_bit pd_bit) {ram}))
(define-fun rewritten () Bool (ite read_open (not selected) (not (and old_md old_pd old_ram))))
(assert (not (= original rewritten)))
(check-sat)
'''
        p=out/(label+'.smt2');p.write_text(smt)
        r=subprocess.run(['z3','-smt2',str(p)],capture_output=True,text=True)
        (out/(label+'.log')).write_text(r.stdout+r.stderr)
        assert r.returncode==0 and r.stdout.strip()==('unsat'if label=='original'else'sat')
        cases.append(dict(case=label,result=r.stdout.strip(),returncode=r.returncode,
                          smt_sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
    record=dict(status='PASS_BINARY_R_FLAG_FACTORIZATION',cases=cases,transform=transform,
                inputs={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in [flags,operand]},
                full_next_state_equivalence_proven=False,analog_skew_qualified=False,native_integrated=False)
    (out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    print(record['status'],transform['output_sha256'])


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    prove(p.parse_args().output.resolve())
