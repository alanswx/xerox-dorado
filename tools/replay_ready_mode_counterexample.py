#!/usr/bin/env python3
"""Replay sparse-Ready witness before/after the fix in both RTL simulators.

Require the original false comparison's exact2970-bit difference, and exact
agreement after retaining the existing sparse path. The arbitrary component
context is not asserted reachable by firmware; no assumption excludes it.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def run(failed,corrected,out):
    assert not out.exists();out.mkdir(parents=True)
    old=json.loads((failed/'run-state.json').read_text());new=json.loads((corrected/'run-state.json').read_text())
    assert old['status']=='TERMINAL_NATIVE_READY_MODE_NOT_ESTABLISHED'
    assert old['source_control_sha256']==new['source_control_sha256']and old['compared_public_outputs']==new['compared_public_outputs']==2970
    cex=failed/'complete-native-interface/counterexample.json'
    c=json.loads(cex.read_text());values={v['name']:v['data'][0]for v in c['signal']if'data'in v}
    assert len(values['stimulus'])==1982 and set(values['stimulus'])<={'0','1'}
    difference=int(values['expected'],2)^int(values['actual'],2);assert difference
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),failed_state_sha256=sha(failed/'run-state.json'),
               counterexample_sha256=sha(cex),corrected_stage_manifest_sha256=new['stage_manifest_sha256'],actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,source,fixed in [('original-witness',failed,False),('corrected',corrected,True)]:
        d=out/label;d.mkdir();action=source/'complete-native-interface'
        meta=next(v for v in json.loads((source/'run-state.json').read_text())['actions']if v['label']=='complete-native-interface')
        verify(action,meta['files'])
        for p in action.glob('*.sv'):shutil.copy2(p,d/p.name)
        test=f'''`timescale 1ns/1ps
module tb_ready_witness;
 reg [1981:0] stimulus;
 wire [2969:0] expected,actual;wire equivalent;
 native_ready_mode_contract dut(stimulus,expected,actual,equivalent);
 initial begin
 stimulus='x;#1;stimulus=1982'b{values['stimulus']};
 #10;
 $display("READY_WITNESS_OBSERVATION target=%h mode=%b original_cmp=%b candidate_cmp=%b child_data=%h raw_capture=%h",dut.original.conta_next[63:48],dut.candidate.ready_target_comparison.ready_mode,dut.original.child_ready_comparison,dut.candidate.parallel_ready_comparison,dut.candidate.child.capture_data,dut.candidate.child_next[751:736]);
 if(expected!==2970'b{values['expected']})$fatal(1,"READY_WITNESS reference changed");
 if((expected^actual)!==2970'h{0 if fixed else difference:x})$fatal(1,"READY_WITNESS wrong original/corrected difference");
 $display("READY_WITNESS_PASS fixed={int(fixed)} target=%h mode=%b original_cmp=%b candidate_cmp=%b",dut.original.conta_next[63:48],dut.candidate.ready_target_comparison.ready_mode,dut.original.child_ready_comparison,dut.candidate.parallel_ready_comparison);
 $finish;
 end
endmodule
'''
        (d/'tb.sv').write_text(test)
        for engine in ['icarus','verilator']:
            paths=[str(p)for p in sorted(d.glob('*.sv'))]
            commands=([['iverilog','-g2012','-s','tb_ready_witness','-o',str(d/'test.vvp'),*paths],['vvp',str(d/'test.vvp')]]if engine=='icarus'else
                      [['verilator','--binary','--timing','--top-module','tb_ready_witness','-Wno-fatal','--Mdir',str(d/'obj'),'-o','test','-j','2',*paths],[str(d/'obj/test')]])
            case=dict(label=label,engine=engine,actions=[]);state['actions'].append(case);save()
            for n,cmd in enumerate(commands):
                logname=engine+('-compile.log'if n==0 else'-test.log')
                with(d/logname).open('x')as log:
                    child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);a=dict(pid=child.pid,command=cmd,started=time.time());case['actions'].append(a);save();code=child.wait()
                a.update(returncode=code,finished=time.time(),log_sha256=sha(d/logname));save()
                if code:break
            passed=len(case['actions'])==2 and all(a['returncode']==0 for a in case['actions'])and'READY_WITNESS_PASS'in(d/(engine+'-test.log')).read_text()
            case['passed']=passed;save();print(label,engine,passed,flush=True)
            if not passed:state.update(status='TERMINAL_READY_WITNESS_REPLAY_FAILED',finished=time.time());save();return 1
    state.update(status='PASS_DUAL_ENGINE_READY_SPARSE_WITNESS_AND_FIX',finished=time.time());save();return 0


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['failed','corrected','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.failed.resolve(),a.corrected.resolve(),a.output.resolve()))
