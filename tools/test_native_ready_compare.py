#!/usr/bin/env python3
"""Compare every public prefix output under independent X/Z substitutions.

Use the real wrong-preempt formal witness plus zero/one contexts. Preserve
unknown values exactly on all2969 results; this is a four-state comparison
gate, not a source timing or native firmware qualification.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def run(proof,out,family='prefix'):
    assert not out.exists();out.mkdir(parents=True)
    r=json.loads((proof/'run-state.json').read_text());verify(proof,r['files'])
    assert r['status']=='PASS_ACTUAL_NATIVE_READY_COMPARISON'and all(a['passed']for a in r['actions'])
    for a in r['actions']:
        d=proof/a['label'];verify(d,a['files']);assert sha(d/'proof.log')==a['log_sha256']
    bits=r['arbitrary_binary_input_bits'];outputs=r['compared_outputs'];assert bits==1982 and outputs==2969
    signals=json.loads((proof/'wrong-preempt-selector/counterexample.json').read_text())['signal']
    row=next(s for s in signals if s['name']=='stimulus');seed=int(row['data'][0],2)
    case=r.get('proof_families',{}).get(family,{}).get('case','complete-interface')
    if family=='single':assert r['actual_native_single_path_qualified']
    for p in(proof/case).glob('*.sv'):shutil.copy2(p,out/p.name)
    tb=f'''`timescale 1ns/1ps
module tb_ready_compare_xz;
 reg [{bits-1}:0] stimulus,base;wire [{outputs-1}:0] actual,expected;wire equivalent;
 ready_compare_equivalence dut(stimulus,actual,expected,equivalent);
 integer checks=0,unknown=0;
 task automatic compare(input integer trial,input integer bit_no,input integer mode);
  begin
   #1;
   if(actual!==expected)begin
    for(integer k=0;k<{outputs};k++)if(actual[k]!==expected[k])
     $display("READY_COMPARE_XZ_DIFF trial=%0d input=%0d mode=%0d output=%0d actual=%b expected=%b",trial,bit_no,mode,k,actual[k],expected[k]);
    $fatal(1,"READY_COMPARE_XZ complete prefix mismatch");
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
  $display("NATIVE_READY_COMPARE_XZ_PASS known=3 unknown=%0d checks=%0d outputs={outputs}",unknown,checks);$finish;
 end
endmodule
'''
    (out/'tb.sv').write_text(tb);shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),proof=str(proof),proof_state_sha256=sha(proof/'run-state.json'),
        input_bits=bits,compared_outputs=outputs,family=family,files={p.name:sha(p)for p in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    paths=sorted(str(p)for p in out.glob('*.sv'));save();code=1
    for label,cmd in [('compile',['iverilog','-g2012','-s','tb_ready_compare_xz','-o',str(out/'test.vvp'),*paths]),
                      ('test',['vvp','-i',str(out/'test.vvp')])]:
        with(out/(label+'.log')).open('x')as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=log);a=dict(label=label,pid=p.pid,command=cmd,started=time.time());state['actions'].append(a);save();code=p.wait()
        a.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    passed=code==0 and f'known=3 unknown={bits*6} checks={bits*6+3} outputs={outputs}'in log
    state.update(status='PASS_ACTUAL_NATIVE_READY_COMPARE_XZ'if passed else'TERMINAL_NATIVE_READY_COMPARE_XZ_NOT_ESTABLISHED',returncode=code,finished=time.time())
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['proof','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--family',choices=['single','prefix'],default='prefix')
    a=p.parse_args();raise SystemExit(run(a.proof.resolve(),a.output.resolve(),a.family))
