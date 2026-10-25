#!/usr/bin/env python3
"""Check every task-consumer input's X/Z fallback against the frozen source.

Use the actual four-wave counterexample plus zero/one contexts. Keep all139
outputs exact, including unknowns. The bad default must change state/events,
rather than only changing the unsettled diagnostic.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

TB='''`timescale 1ns/1ps
module tb_native_task_xz;
 reg [677:0] stimulus,base;
 wire stop_tasks,cb,ca;wire [1:0] canonical;wire [87:0] q;wire [75:0] pb,pa;wire [432:0] frames;
 assign {stop_tasks,canonical,q,cb,ca,pb,pa,frames}=stimulus;
 wire [138:0] actual,expected;wire equivalent,terminal_closed;
 native_task_terminal_contract dut(stop_tasks,canonical,q,cb,ca,pb,pa,frames,actual,expected,equivalent,terminal_closed);
 integer checks=0,unknown=0;
 task automatic compare(input integer trial,input integer bit_no,input integer mode);
  begin
   #1;
   if(actual!==expected)begin
    for(integer k=0;k<139;k++)if(actual[k]!==expected[k])
     $display("TASK_TERMINAL_XZ_DIFF trial=%0d input=%0d mode=%0d output=%0d actual=%b expected=%b",trial,bit_no,mode,k,actual[k],expected[k]);
    $fatal(1,"TASK_TERMINAL_XZ functional mismatch");
   end
   checks++;
  end
 endtask
 initial begin
  for(integer trial=0;trial<3;trial++)begin
   case(trial)0:base=678'hSEED;1:base=0;2:base='1;endcase
   stimulus=base;compare(trial,-1,0);
   if(terminal_closed!==1)$fatal(1,"TASK_TERMINAL_XZ binary closure lost");
   for(integer k=0;k<678;k++)for(integer mode=0;mode<2;mode++)begin
    stimulus=base;stimulus[k]=mode ? 1'bz:1'bx;compare(trial,k,mode+2);unknown++;
   end
  end
  $display("NATIVE_TASK_TERMINAL_XZ_PASS known=3 unknown=%0d checks=%0d outputs=139",unknown,checks);$finish;
 end
endmodule
'''


def run(proof,out,bad=False):
    assert not out.exists();out.mkdir(parents=True)
    s=json.loads((proof/'run-state.json').read_text());verify(proof,s['files'])
    assert s['status']=='PASS_ACTUAL_NATIVE_TASK_TERMINAL_CLOSURE'and all(a['passed']for a in s['actions'])
    witness=proof/'four-wave-counterexample'
    sig={x['name']:x for x in json.loads((witness/'counterexample.json').read_text())['signal']}
    def value(n):
        row=sig[n];return int(row['data'][0],2)if'data'in row else int(row['wave'][0],2)
    seed=0
    for n,width in [('stop_tasks',1),('canonical',2),('q',88),('cb',1),('ca',1),('pb',76),('pa',76),('frames',433)]:
        seed=(seed<<width)|value(n)
    for n in ['original.sv','actual.sv','top.sv']:shutil.copy2(proof/'seven-wave-complete-result'/n,out/n)
    if bad:
        p=out/'actual.sv';text=p.read_text();token='default:begin end'
        assert text.count(token)==1
        p.write_text(text.replace(token,"default:{state_next,clock_events,repeated_events,unsettled}={q,events,repeated,1'b0};",1))
    (out/'tb.sv').write_text(TB.replace("678'hSEED",f"678'h{seed:x}"));shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,proof=str(proof),proof_state_sha256=sha(proof/'run-state.json'),
        bad_unknown_default=bad,variable_bits=678,compared_bits=139,started=time.time(),actions=[],
        files={p.name:sha(p)for p in out.iterdir()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save();code=1
    paths=[str(out/n)for n in ['original.sv','actual.sv','top.sv','tb.sv']]
    for label,cmd in [('compile',['iverilog','-g2012','-s','tb_native_task_xz','-o',str(out/'test.vvp'),*paths]),
                      ('test',['vvp','-i',str(out/'test.vvp')])]:
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log);row=dict(label=label,command=cmd,pid=child.pid,started=time.time())
            state['actions'].append(row);save();code=child.wait()
        row.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    passed=state['actions'][0]['returncode']==0 and (code!=0 and any(
        f'output={k} 'in log for k in range(1,139))if bad else
        code==0 and 'NATIVE_TASK_TERMINAL_XZ_PASS known=3 unknown=4068 checks=4071 outputs=139'in log)
    state.update(status=('PASS_NATIVE_TASK_TERMINAL_UNKNOWN_COUNTEREXAMPLE'if bad else'PASS_NATIVE_TASK_TERMINAL_XZ')if passed else
                 'TERMINAL_NATIVE_TASK_TERMINAL_XZ_NOT_ESTABLISHED',returncode=code,finished=time.time())
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['proof','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--bad-unknown',action='store_true');a=p.parse_args()
    raise SystemExit(run(a.proof.resolve(),a.output.resolve(),a.bad_unknown))
