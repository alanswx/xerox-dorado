#!/usr/bin/env python3
"""Check Ready-choice comparison and its original X/Z fallback in both engines."""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from prove_ready_mode_compare import COMPARE
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

TB='''`timescale 1ns/1ps
module tb_ready_mode;
 reg [1308:0] stimulus=0;
 wire [155:0] q;wire cb,ca,refresh,fast,enable,event_in;
 wire [397:0] pb,pa;wire [268:0] prefix;wire [10:0] pre_cp,post_cp;
 wire [15:0] current_ready,original_capture;wire [27:0] data;
 assign {q,cb,ca,pb,pa,refresh,fast,enable,prefix,pre_cp,post_cp,current_ready,original_capture,data,event_in}=stimulus;
 wire equivalent,expected,actual;
 ready_compare_contract dut(q,cb,ca,pb,pa,refresh,fast,enable,prefix,pre_cp,post_cp,
  current_ready,original_capture,data,event_in,equivalent,expected,actual);
 integer i,j,checks=0,mode0=0,mode1=0,mode2=0,unknown_checks=0;
 reg [31:0] rng=32'hd0fad022;
 task random_inputs;begin
  for(j=0;j<1309;j=j+1)begin
   rng=rng^(rng<<13);rng=rng^(rng>>17);rng=rng^(rng<<5);stimulus[j]=rng[0];
  end
 end endtask
 task check;begin
  #1;checks++;
  if(actual!==expected)$fatal(1,"READY_MODE_COMPARE wrong mismatch actual=%b expected=%b",actual,expected);
 end endtask
 initial begin
  for(i=0;i<1024;i=i+1)begin
   random_inputs;check;
   case(dut.candidate.mode.ready_mode)
    0:mode0++;1:mode1++;2:mode2++;
   endcase
  end
`ifndef VERILATOR
  for(i=0;i<1309;i=i+1)begin
   random_inputs;stimulus[i]=1'bx;check;unknown_checks++;
   stimulus[i]=1'bz;check;unknown_checks++;
  end
`endif
  if(mode0==0 || mode1==0 || mode2==0)$fatal(1,"READY_MODE_COMPARE incomplete write-choice coverage");
  $display("READY_MODE_COMPARE_PASS checks=%0d mode0=%0d mode1=%0d mode2=%0d unknown_checks=%0d",checks,mode0,mode1,mode2,unknown_checks);
  $finish;
 end
endmodule
'''


def run(source,out):
    assert not out.exists();out.mkdir(parents=True)
    r=json.loads((source/'results.json').read_text());verify(source,r['files'])
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',started=time.time(),source_receipt_sha256=sha(source/'results.json'),actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for index,(engine,bad)in enumerate([('icarus',False),('verilator',False),('verilator',True)]):
        d=out/(engine+'-'+str(index));d.mkdir()
        for p in source.glob('*.sv'):shutil.copy2(p,d/p.name)
        if bad:
            raw=(d/'compare.sv').read_text();a="2'd1:mismatch=comparisons[1];";assert raw.count(a)==1
            (d/'compare.sv').write_text(raw.replace(a,"2'd1:mismatch=comparisons[2];",1))
        (d/'top.sv').write_text(COMPARE.replace('SPARSE_SUPPORT',r['fall_stable_pin_support']));(d/'tb.sv').write_text(TB)
        paths=[str(p)for p in sorted(d.glob('*.sv'))]
        commands=([['iverilog','-g2012','-s','tb_ready_mode','-o',str(d/'test.vvp'),*paths],['vvp',str(d/'test.vvp')]]if engine=='icarus'else
                  [['verilator','--binary','--timing','--top-module','tb_ready_mode','-Wno-fatal','--Mdir',str(d/'obj'),'-o','test','-j','2',*paths],[str(d/'obj/test')]])
        case=dict(engine=engine,negative=bad,actions=[],files={p.name:sha(p)for p in d.iterdir()if p.is_file()});state['actions'].append(case);save()
        for n,cmd in enumerate(commands):
            with(d/('compile.log'if n==0 else'test.log')).open('x')as log:
                child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);a=dict(pid=child.pid,command=cmd,started=time.time());case['actions'].append(a);save();code=child.wait()
            a.update(returncode=code,finished=time.time());save()
            if n==0 and code:break
        log=(d/'test.log').read_text()if(d/'test.log').exists()else''
        case['passed']=(len(case['actions'])==2 and case['actions'][0]['returncode']==0 and
                        ((case['actions'][1]['returncode']!=0 and'READY_MODE_COMPARE wrong mismatch'in log)if bad else
                         (case['actions'][1]['returncode']==0 and'READY_MODE_COMPARE_PASS'in log)))
        save();print(d.name,case['passed'],flush=True)
        if not case['passed']:break
    passed=len(state['actions'])==3 and all(c['passed']for c in state['actions'])
    state.update(status='PASS_DUAL_ENGINE_READY_MODE_COMPARISON_AND_XZ_FALLBACK'if passed else'TERMINAL_READY_MODE_TEST_NOT_ESTABLISHED',finished=time.time())
    save();return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve()))
