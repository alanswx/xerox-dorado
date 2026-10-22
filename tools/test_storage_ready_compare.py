#!/usr/bin/env python3
"""Prove parallel ready-tag selection, including the old four-state if policy.

SMT covers every binary16-bit tag/control combination at both ready outputs.
Icarus compares the actual old/new modules with forced boundary state and
X/Z control/tag cases; no clocks or states of a live machine are changed.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

from prepare_storage_ready_compare import GROUP_SHA, transform

SMT = '''(set-logic QF_BV)
(declare-const cache (_ BitVec 16))
(declare-const saved (_ BitVec 16))
(declare-const addr (_ BitVec 16))
(declare-const after (_ BitVec 16))
(declare-const waiting Bool)
(declare-const response Bool)
(declare-const valid Bool)
(declare-const pending Bool)
(declare-const done Bool)
(declare-const saved_write Bool)
(declare-const accept Bool)
(declare-const advance_pending Bool)
(define-fun arrived () Bool (and waiting response))
(define-fun pred_waiting () Bool (ite arrived false (or waiting accept)))
(define-fun pred_valid () Bool (or valid arrived))
(define-fun pred_done () Bool (ite (and arrived saved_write) true (and pending done)))
(define-fun oldtag () (_ BitVec 16) (ite arrived saved cache))
(define-fun oldhold () Bool (and (not pred_waiting) pred_valid (= oldtag addr) (or (not pending) pred_done)))
(define-fun oldadvance () Bool (and (not pred_waiting) pred_valid (= oldtag after) (or (not advance_pending) pred_done)))
(define-fun newhold () Bool (and (not pred_waiting) pred_valid (ite SELECT (= saved addr) (= cache addr)) (or (not pending) pred_done)))
(define-fun newadvance () Bool (and (not pred_waiting) pred_valid (ite SELECT (= saved ADVANCE) (= cache ADVANCE)) (or (not advance_pending) pred_done)))
(assert (or (not (= oldhold newhold)) (not (= oldadvance newadvance))))
(check-sat)
'''

BENCH = r'''`timescale 1ns/1ps
module tb_ready_compare;
 reg [7:0] a=0,next_a=0,cache_a=0,saved_a=0;
 reg wait_q=0,valid_q=0,done_q=0,pending_q=0,saved_write_q=0,req_ready=0,rsp=0,cd=0,wd=0;
 wire old_hold,old_advance,new_hold,new_advance;
 original_group #(.ADDR_BITS(8)) original(PORTS_OLD);
 dorado_storage_group #(.ADDR_BITS(8)) candidate(PORTS_NEW);
 function automatic four(input integer n);
  case(n)0:four=0;1:four=1;2:four=1'bx;3:four=1'bz;endcase
 endfunction
 integer checks=0;
 task automatic tags(input integer n);
  cache_a=8'(n*37);saved_a=8'(n*97);a=8'(n*19);next_a=8'(n*17)^8'h55;
  case(n%4)0:a=cache_a;1:a=saved_a;2:next_a=cache_a;3:next_a=saved_a;endcase
  if(n%15==0)begin a[3]=1'bx;next_a[2]=1'bz;end
  if(n%17==0)begin cache_a[5]=1'bz;saved_a[7]=1'bx;end
  #1;
  if({old_hold,old_advance}!=={new_hold,new_advance})$fatal(1,"READY_COMPARE old/new four-state mismatch");
  checks++;
 endtask
 initial begin
  force original.waiting=wait_q;force candidate.waiting=wait_q;
  force original.cache_valid=valid_q;force candidate.cache_valid=valid_q;
  force original.write_done=done_q;force candidate.write_done=done_q;
  force original.write_pending=pending_q;force candidate.write_pending=pending_q;
  force original.saved_write=saved_write_q;force candidate.saved_write=saved_write_q;
  force original.cache_addr=cache_a;force candidate.cache_addr=cache_a;
  force original.saved_addr=saved_a;force candidate.saved_addr=saved_a;
  force original.cas_d=cd;force candidate.cas_d=cd;
  force original.we_d=wd;force candidate.we_d=wd;
  for(integer f=0;f<512;f++)begin
   {wait_q,valid_q,done_q,pending_q,saved_write_q,req_ready,rsp,cd,wd}=9'(f);
   for(integer n=0;n<64;n++)tags(n);
  end
  for(integer w=0;w<4;w++)for(integer r=0;r<4;r++)for(integer f=0;f<128;f++)begin
   {valid_q,done_q,pending_q,saved_write_q,req_ready,cd,wd}=7'(f);
   wait_q=four(w);rsp=four(r);
   for(integer n=0;n<64;n++)tags(n);
  end
  $display("READY_COMPARE PASS actual_modules checks=%0d four_state=1",checks);$finish;
 end
endmodule
'''
PORTS = '''.host_clk(1'b0),.machine_clk(1'b0),.reset(1'b0),.machine_step(1'b0),
 .addr_after_edge(next_a),.ras_n(1'b0),.cas_n(1'b0),.we_n(1'b0),.addr(a),.data_in(16'd0),
 .req_ready(req_ready),.rsp_valid(rsp),.rsp_data(16'd0),
 .ready_hold_next(HOLD),.ready_advance_next(ADVANCE)'''


def run(original, generated, out):
    assert not out.exists()
    raw = original.read_bytes()
    assert hashlib.sha256(raw).hexdigest() == GROUP_SHA
    candidate = (generated / 'dorado_storage_group.sv').read_bytes()
    assert candidate == transform(raw)
    out.mkdir(parents=True)
    inp = out / 'inputs'
    inp.mkdir()
    (inp / 'reference.sv').write_bytes(raw.replace(b'module dorado_storage_group #(', b'module original_group #('))
    (inp / 'candidate.sv').write_bytes(candidate)
    (inp / 'tb.sv').write_text(BENCH.replace('PORTS_OLD', PORTS.replace('HOLD', 'old_hold').replace('ADVANCE', 'old_advance'))
                            .replace('PORTS_NEW', PORTS.replace('HOLD', 'new_hold').replace('ADVANCE', 'new_advance')))
    (inp / Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    cases = []
    for name, select, advance, expected in [('positive', 'arrived', 'after', 'unsat'),
                                           ('bad-response-selector', 'response', 'after', 'sat'),
                                           ('bad-next-address', 'arrived', 'addr', 'sat')]:
        f = out / (name + '.smt2')
        f.write_text(SMT.replace('SELECT', select).replace('ADVANCE', advance))
        r = subprocess.run(['z3', str(f)], capture_output=True, text=True, check=True)
        (out / (name + '.log')).write_text(r.stdout + r.stderr)
        assert r.stdout.strip() == expected, (name, r.stdout)
        cases.append(dict(name=name, result=expected))
    with (out / 'compile.log').open('x') as log:
        subprocess.run(['iverilog', '-g2012', '-s', 'tb_ready_compare', '-o', str(out / 'test.vvp'),
                        str(inp / 'reference.sv'), str(inp / 'candidate.sv'), str(inp / 'tb.sv')],
                       stdout=log, stderr=subprocess.STDOUT, check=True)
    with (out / 'test.log').open('x') as log:
        subprocess.run(['vvp', str(out / 'test.vvp')], stdout=log, stderr=subprocess.STDOUT, check=True)
    assert 'READY_COMPARE PASS actual_modules checks=163840 four_state=1' in (out / 'test.log').read_text()
    r = dict(status='PASS_READY_COMPARE_ALL_BINARY_AND_ACTUAL_FOUR_STATE', cases=cases,
             scope=__doc__, inputs={f.name: hashlib.sha256(f.read_bytes()).hexdigest() for f in inp.iterdir()},
             captures_changed=False, native_qualified=False)
    (out / 'proof.json').write_text(json.dumps(r, indent=2) + '\n')
    print(r['status'])


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['original', 'generated', 'output']:
        p.add_argument('--' + n, type=Path, required=True)
    a = p.parse_args()
    run(a.original.resolve(), a.generated.resolve(), a.output.resolve())
