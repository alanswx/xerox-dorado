#!/usr/bin/env python3
"""Replay the formal wrong-Hold counterexample in both Verilog simulators.

This is a pure component fixture, not guest-state import or an FPGA snapshot.
Keep the exact363-bit mismatch, including event flags even when Qs agree.
"""
import argparse
import json
from pathlib import Path
import subprocess
import time

from test_r_front_forward_view import sha


def run(source,out):
    assert not out.exists()
    record=json.loads((source/'run-state.json').read_text())
    assert record['status']in ['PASS_RTL_COUNTEREXAMPLE_CONTROL','TERMINAL_RTL_PROOF_NOT_ESTABLISHED']and record['counterexample_found']
    cex=json.loads((source/'counterexample.json').read_text())
    signals={s['name']:(s['data'][0]if 'data'in s else s['wave'][0])for s in cex['signal']}
    widths=dict(q=156,before_root=1,after_root=1,before_pins=398,after_pins=398,refresh=1,different=363)
    for name,width in widths.items():
        assert len(signals[name])==width and set(signals[name])<={'0','1'},name
    assert signals['equal']=='0'
    different=int(signals['different'],2)
    assert different!=0
    bits={name:[i for i in range(width)if different>>(shift+i)&1]for name,shift,width in
          [('flags',0,94),('unsettled',94,1),('repeated',95,56),('events',151,56),('Q',207,156)]}
    out.mkdir(parents=True)
    for name in ['baseline.sv','candidate.sv','top.sv','counterexample.json']:
        (out/name).write_bytes((source/name).read_bytes())
    declarations='\n'.join(f" reg [{width-1}:0] {name};"for name,width in widths.items()if name!='different')
    assignments='\n'.join(f"  {name}={width}'b{signals[name]};"for name,width in widths.items()if name!='different')
    expected="363'b"+signals['different']
    tb='''`timescale 1ns/1ps
module tb_memory_mir_counterexample;
DECL
 wire equal;wire [362:0] different;
 memory_mir_equivalence dut(q,before_root,after_root,before_pins,after_pins,refresh,equal,different);
 initial begin
ASSIGN
 #10;
 if(equal!==1'b0 || different!==EXPECTED)
  $fatal(1,"Formal counterexample replay mismatch equal=%b diff=%h",equal,different);
 $display("MEMORY_MIR_COUNTEREXAMPLE PASS different=%h",different);
 $finish;
 end
endmodule
'''.replace('DECL',declarations).replace('ASSIGN',assignments).replace('EXPECTED',expected)
    (out/'tb.sv').write_text(tb)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state=dict(status='RUNNING',started=time.time(),source=str(source),
               source_state_sha256=sha(source/'run-state.json'),mismatch_bits=bits,actions=[],
               source_files={p.name:sha(p)for p in out.iterdir()if p.is_file()},
               native_state_import=False,physical_timing_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for engine in ['icarus','verilator']:
        paths=[str(out/name)for name in ['baseline.sv','candidate.sv','top.sv','tb.sv']]
        commands=[['iverilog','-g2012','-s','tb_memory_mir_counterexample','-o',str(out/'test.vvp'),*paths],
                  ['vvp',str(out/'test.vvp')]]if engine=='icarus'else[
            ['verilator','--binary','--timing','--top-module','tb_memory_mir_counterexample','-Wno-fatal',
             '--Mdir',str(out/'obj'),'-o','test','-j','2',*paths],[str(out/'obj/test')]]
        for i,command in enumerate(commands):
            name=engine+('-compile'if i==0 else'-test')
            with(out/(name+'.log')).open('x')as log:
                process=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT)
                action=dict(engine=engine,command=command,pid=process.pid,started=time.time());state['actions'].append(action);save()
                code=process.wait();action.update(returncode=code,finished=time.time());save()
            if code:
                state.update(status='TERMINAL_REPLAY_FAILED',finished=time.time());save();raise RuntimeError((name,code))
        assert 'MEMORY_MIR_COUNTEREXAMPLE PASS'in(out/(engine+'-test.log')).read_text()
    state.update(status='PASS_DUAL_SIMULATOR_FORMAL_COUNTEREXAMPLE_REPLAY',finished=time.time());save()
    print(state['status'],bits,flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['source','output']:parser.add_argument('--'+name,type=Path,required=True)
    args=parser.parse_args();run(args.source.resolve(),args.output.resolve())
