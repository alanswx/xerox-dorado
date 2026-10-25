#!/usr/bin/env python3
"""Replay the entire native flag-owner interface in two/four-state simulation.

All storage seeding uses the original administration interfaces. Exercise1024
deterministic binary commands and, in Icarus, independent X/Z substitutions
in every input over three source-bound contexts. Compare all175
outputs, including file read/admin behavior and implementation history.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def run(proof,out,engine):
    assert not out.exists();out.mkdir(parents=True)
    p=json.loads((proof/'run-state.json').read_text());verify(proof,p['files'])
    assert p['status']=='PASS_ACTUAL_NATIVE_BRANCH_COMMON_OWNER'and all(a['passed']for a in p['actions'])
    bits=p['arbitrary_binary_inputs']
    for a in p['actions']:
        d=proof/a['label'];verify(d,a['files']);assert sha(d/'proof.log')==a['log_sha256']
    witness=json.loads((proof/'wrong-g11-epoch/counterexample.json').read_text())
    sig=next(s for s in witness['signal']if s['name']=='stimulus')
    # Sequential SAT traces carry an empty initial waveform placeholder.
    # The first complete input value is the stimulus sampled at phase0.
    seed=int(next(v for v in sig['data']if len(v)==bits and set(v)<={'0','1'}),2)
    source=proof/'complete-owner'
    for f in source.glob('*.sv'):shutil.copy2(f,out/f.name)
    for f in source.glob('*.v'):shutil.copy2(f,out/f.name)
    tb=f'''`timescale 1ns/1ps
module tb_native_branch_owner;
 reg clk=0;reg [{bits-1}:0] stimulus=0,base;reg [31:0] rng=32'hd04ad061;
 wire [174:0] actual,expected;wire equivalent,guest_q_equal;
 native_branch_owner_contract dut(clk,stimulus,actual,expected,equivalent,guest_q_equal);
 integer checks=0,unknown=0;
 task automatic compare;
  begin
   // Only the test's phase counter restarts; guest state is seeded through
   // the actual interfaces, never by assigning any cell/owner storage.
   clk=0;dut.phase=0;
   for(integer edge_no=0;edge_no<21;edge_no++)begin #1;clk=1;#1;clk=0;end
   #1;
   if(actual!==expected)begin
    for(integer k=0;k<175;k++)if(actual[k]!==expected[k])
     $display("NATIVE_BRANCH_OWNER_DIFF check=%0d output=%0d actual=%b expected=%b stimulus=%h",checks,k,actual[k],expected[k],stimulus);
    $fatal(1,"NATIVE_BRANCH_OWNER complete interface mismatch");
   end
   checks++;
  end
 endtask
 initial begin
  for(integer trial=0;trial<1024;trial++)begin
   for(integer k=0;k<{bits};k++)begin rng=rng^(rng<<13);rng=rng^(rng>>17);rng=rng^(rng<<5);stimulus[k]=rng[0];end
   compare;
  end
  if({1 if engine=='icarus'else 0})begin
   for(integer trial=0;trial<3;trial++)begin
    case(trial)0:base={bits}'h{seed:x};1:base=0;2:base='1;endcase
    stimulus=base;compare;
    for(integer k=0;k<{bits};k++)for(integer mode=0;mode<2;mode++)begin
     stimulus=base;stimulus[k]=mode ? 1'bz:1'bx;compare;unknown++;
    end
   end
  end
  $display("NATIVE_BRANCH_OWNER_PASS binary=1024 unknown=%0d checks=%0d outputs=175",unknown,checks);$finish;
 end
endmodule
'''
    (out/'tb.sv').write_text(tb);shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),engine=engine,proof=str(proof),
        proof_state_sha256=sha(proof/'run-state.json'),input_bits=bits,compared_outputs=175,
        guest_storage_seeded_only_through_existing_interfaces=True,
        files={p.name:sha(p)for p in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    paths=sorted(str(p)for p in out.iterdir()if p.suffix in ['.v','.sv'])
    commands=([['iverilog','-g2012','-DDORADO_F10145_LOGIC','-s','tb_native_branch_owner','-o',str(out/'test.vvp'),*paths],
               ['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else
              [['verilator','--binary','--timing','-j','2','-Wno-fatal','-DDORADO_F10145_LOGIC','--top-module','tb_native_branch_owner',
                '--Mdir',str(out/'obj'),'-o','test',*paths],[str(out/'obj/test')]])
    save();code=1
    for label,cmd in zip(['compile','test'],commands):
        with(out/(label+'.log')).open('x')as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=log);a=dict(label=label,pid=p.pid,command=cmd,started=time.time())
            state['actions'].append(a);save();code=p.wait()
        a.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    expected=f'binary=1024 unknown={bits*6} checks={1024+3+bits*6} outputs=175'if engine=='icarus'else'binary=1024 unknown=0 checks=1024 outputs=175'
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    passed=code==0 and expected in log
    state.update(status='PASS_ACTUAL_NATIVE_BRANCH_OWNER_INTERFACE'if passed else'TERMINAL_NATIVE_BRANCH_OWNER_INTERFACE_NOT_ESTABLISHED',
        returncode=code,finished=time.time(),native_selected=False,source_arrivals_or_FPGA_timing_qualified=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['proof','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True)
    a=p.parse_args();raise SystemExit(run(a.proof.resolve(),a.output.resolve(),a.engine))
