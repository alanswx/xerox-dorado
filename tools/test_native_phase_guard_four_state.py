#!/usr/bin/env python3
"""Check all native shared-control outputs and every input's X/Z fallback.

Use the actual formal changed-input/real-clock witness. Compare795 bits,
including432 shared capture-frame bits. A deliberately wrong unknown default
must corrupt a functional result, not merely the sparse-path debug flag.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

TB='''`timescale 1ns/1ps
module tb_native_phase_xz;
 reg [955:0] stimulus,base;
 wire [155:0] q;wire cb,ca;wire [397:0] pb,pa;wire refresh,entry;
 assign {q,cb,ca,pb,pa,refresh,entry}=stimulus;
 wire equal,certificate,old_sparse,new_sparse,witness;
 wire [794:0] actual,expected;
 native_fall_guard_contract dut(q,cb,ca,pb,pa,refresh,entry,equal,certificate,old_sparse,new_sparse,actual,expected,witness);
 integer checks=0,known=0,unknown=0;
 task automatic compare(input integer trial,input integer bit_no,input integer mode);
  begin
   #1;
   if(actual!==expected)begin
    for(integer k=0;k<795;k++)if(actual[k]!==expected[k])
     $display("PHASE_GUARD_XZ_DIFF trial=%0d input=%0d mode=%0d output=%0d actual=%b expected=%b",trial,bit_no,mode,k,actual[k],expected[k]);
    $fatal(1,"PHASE_GUARD_XZ functional mismatch");
   end
   checks++;
  end
 endtask
 initial begin
  for(integer trial=0;trial<4;trial++)begin
   base=956'hSEED;
   if(trial==1)base[2+20+:15]^=15'h7fff;
   if(trial==2)base[2+348+:6]^=6'h3f;
   if(trial==3)base[2+119]^=1'b1;
   stimulus=base;compare(trial,-1,0);known++;
   if(certificate!==1 || old_sparse!==0 || new_sparse!==1 || witness!==1)
    $fatal(1,"PHASE_GUARD_XZ nonvacuous witness lost trial=%0d",trial);
   for(integer k=0;k<956;k++)for(integer mode=0;mode<2;mode++)begin
    stimulus=base;stimulus[k]=mode ? 1'bz:1'bx;compare(trial,k,mode+2);unknown++;
    if(CHECK_GUARD && (old_sparse!==0 || new_sparse!==0))$fatal(1,"PHASE_GUARD_XZ unknown selected shortcut");
   end
  end
  $display("NATIVE_PHASE_GUARD_XZ_PASS known=%0d unknown=%0d checks=%0d outputs=795",known,unknown,checks);$finish;
 end
endmodule
'''


def run(proof,out,bad=False):
    assert not out.exists();out.mkdir(parents=True)
    p=json.loads((proof/'run-state.json').read_text());verify(proof,p['files'])
    assert p['status']=='PASS_ACTUAL_NATIVE_PHASE_FALL_GUARD'and len(p['actions'])==6
    assert all(r['passed']for r in p['actions'])
    case=proof/'cover-clock-and-data-w5'
    row=next(r for r in p['actions']if r['mode']=='cover-clock-and-data');verify(case,row['files'])
    assert sha(case/'proof.log')==row['log_sha256']
    sig={s['name']:s for s in json.loads((case/'counterexample.json').read_text())['signal']}
    def value(n):
        item=sig[n];return int(item['data'][0],2)if'data'in item else int(item['wave'][0],2)
    assert value('witnessed_change')==value('certificate')==value('new_sparse')==1 and value('old_sparse')==0
    seed=value('q')
    for n,w in [('cb',1),('ca',1),('pb',398),('pa',398),('refresh',1),('entry',1)]:seed=(seed<<w)|value(n)
    source=proof/'positive-w5'
    for n in ['original.sv','actual.sv','top.sv']:shutil.copy2(source/n,out/n)
    if bad:
        path=out/'actual.sv';raw=path.read_text();assert raw.count('default:using_sparse=0;')==1
        path.write_text(raw.replace('default:using_sparse=0;','default:using_sparse=1;',1))
    tb=TB.replace("956'hSEED",f"956'h{seed:x}").replace('CHECK_GUARD',"1'b0"if bad else"1'b1")
    (out/'tb.sv').write_text(tb);shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,proof=str(proof),proof_state_sha256=sha(proof/'run-state.json'),
               bad_unknown_default=bad,variable_bits=956,compared_bits=795,started=time.time(),actions=[],
               files={p.name:sha(p)for p in out.iterdir()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save();code=1
    paths=[str(out/n)for n in ['original.sv','actual.sv','top.sv','tb.sv']]
    for label,cmd in [('compile',['iverilog','-g2012','-s','tb_native_phase_xz','-o',str(out/'test.vvp'),*paths]),('test',['vvp','-i',str(out/'test.vvp')])]:
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log);row=dict(label=label,command=cmd,pid=child.pid,started=time.time())
            state['actions'].append(row);save();code=child.wait()
        row.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    passed=state['actions'][0]['returncode']==0 and ((code!=0 and 'PHASE_GUARD_XZ_DIFF'in log)if bad else
        code==0 and 'NATIVE_PHASE_GUARD_XZ_PASS known=4 unknown=7648 checks=7652 outputs=795'in log)
    state.update(status=('PASS_NATIVE_PHASE_GUARD_UNKNOWN_COUNTEREXAMPLE'if bad else'PASS_NATIVE_PHASE_GUARD_XZ')if passed else'TERMINAL_NATIVE_PHASE_GUARD_XZ_NOT_ESTABLISHED',
                 returncode=code,finished=time.time());save();print(state['status']);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['proof','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--bad-unknown',action='store_true');a=p.parse_args();raise SystemExit(run(a.proof.resolve(),a.output.resolve(),a.bad_unknown))
