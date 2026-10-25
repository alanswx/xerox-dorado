#!/usr/bin/env python3
"""Compare the whole shared-address interface on independent X/Z inputs.

Use the real wrong-StartCycle formal witness, all-zero and all-one contexts.
Every original output is compared exactly, including unknown diagnostic bits;
no self-comparison or barrier field is waived.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def run(proof,out):
    assert not out.exists();out.mkdir(parents=True)
    r=json.loads((proof/'run-state.json').read_text());verify(proof,r['files'])
    assert r['status']=='PASS_ACTUAL_NATIVE_SHARED_ADDRESS_TASK_OWNER'and all(a['passed']for a in r['actions'])
    bits=r['actual_arbitrary_binary_input_bits'];outputs=r['compared_outputs'];assert bits==2114
    source=proof/'complete-interface'
    for p in source.glob('*.sv'):shutil.copy2(p,out/p.name)
    for a in r['actions']:
        verify(proof/a['label'],a['files']);assert sha(proof/a['label']/'proof.log')==a['log_sha256']
    witness=json.loads((proof/'wrong-StartCycle-alias/counterexample.json').read_text())
    sig=next(s for s in witness['signal']if s['name']=='stimulus')
    seed=int(sig['data'][0],2)if'data'in sig else int(sig['wave'][0],2)
    tb=f'''`timescale 1ns/1ps
module tb_address_owner_xz;
 reg [{bits-1}:0] stimulus,base;
 wire [{outputs-1}:0] actual,expected;wire equivalent;
 address_owner_equivalence dut(stimulus,actual,expected,equivalent);
 integer checks=0,unknown=0;
 task automatic compare(input integer trial,input integer bit_no,input integer mode);
  begin
   #1;
   if(actual!==expected)begin
    for(integer k=0;k<{outputs};k++)if(actual[k]!==expected[k])
     $display("ADDRESS_TASK_OWNER_XZ_DIFF trial=%0d input=%0d mode=%0d output=%0d actual=%b expected=%b",trial,bit_no,mode,k,actual[k],expected[k]);
    $fatal(1,"ADDRESS_TASK_OWNER_XZ full interface mismatch");
   end
   checks++;
  end
 endtask
 initial begin
  for(integer trial=0;trial<3;trial++)begin
   case(trial)0:base={bits}'h{seed:x};1:base=0;2:base='1;endcase
   stimulus=base;compare(trial,-1,0);
   for(integer k=0;k<{bits};k++)for(integer mode=0;mode<2;mode++)begin
    stimulus=base;stimulus[k]=mode ? 1'bz:1'bx;compare(trial,k,mode+2);unknown++;
   end
  end
  $display("NATIVE_ADDRESS_TASK_OWNER_XZ_PASS known=3 unknown=%0d checks=%0d outputs={outputs}",unknown,checks);$finish;
 end
endmodule
'''
    (out/'tb.sv').write_text(tb);shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),proof=str(proof),proof_state_sha256=sha(proof/'run-state.json'),
        input_bits=bits,compared_outputs=outputs,files={p.name:sha(p)for p in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    paths=sorted(str(p)for p in out.glob('*.sv'));save();code=1
    for label,cmd in [('compile',['iverilog','-g2012','-s','tb_address_owner_xz','-o',str(out/'test.vvp'),*paths]),
                      ('test',['vvp','-i',str(out/'test.vvp')])]:
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log);row=dict(label=label,command=cmd,pid=child.pid,started=time.time())
            state['actions'].append(row);save();code=child.wait()
        row.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    text=(out/'test.log').read_text()if(out/'test.log').exists()else''
    passed=code==0 and f'known=3 unknown={bits*6} checks={bits*6+3} outputs={outputs}'in text
    state.update(status='PASS_ACTUAL_NATIVE_ADDRESS_TASK_OWNER_XZ'if passed else'TERMINAL_NATIVE_ADDRESS_TASK_OWNER_XZ_NOT_ESTABLISHED',
        returncode=code,finished=time.time());save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['proof','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.proof.resolve(),a.output.resolve()))
