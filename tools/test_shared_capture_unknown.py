#!/usr/bin/env python3
"""Exercise the real shared processor consumer behind its full-context X/Z guard.

Freeze one active input/output record from the passing original-processor replay.
Poison every1941-bit caller-context position independently; deliberately wrong
capture data must fall back to all139 original results. An always-known bad
guard must change actual processor Qs, not merely a diagnostic field.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from generate_shared_control_capture import KNOWN
from review_baseboard_clock_consumer import verify
from test_r_front_forward_view import sha

TB='''`timescale 1ns/1ps
module tb_shared_unknown;
 KNOWN
 localparam [677:0] ACTIVE=678'hINPUT_VALUE;
 localparam [138:0] EXPECTED=139'hOUTPUT_VALUE;
 localparam [431:0] BAD_START=432'hBAD_VALUE;
 wire stop;wire [1:0] canonical;wire [87:0] q;wire cb,ca;wire [75:0] pb,pa;wire [432:0] saved;
 assign {stop,canonical,q,cb,ca,pb,pa,saved}=ACTIVE;
 reg [1940:0] context_bits=0,background;reg wrong=0;
 wire [431:0] frames=saved[431:0]^(wrong ? BAD_START:432'b0);
 wire enabled=shared_inputs_known(context_bits);
 wire [138:0] actual,original,bad;
 dorado_local_task_capture_atomic_step candidate(stop,canonical,q,cb,ca,pb,pa,
  actual[138:51],actual[50:26],actual[25:1],actual[0],{enabled,frames});
 dorado_local_task_capture_atomic_step bad_guard(stop,canonical,q,cb,ca,pb,pa,
  bad[138:51],bad[50:26],bad[25:1],bad[0],{1'b1,frames});
 dorado_local_task_capture_atomic_original_step fallback(stop,canonical,q,cb,ca,pb,pa,
  original[138:51],original[50:26],original[25:1],original[0]);
 integer checks=0,known=0,unknown=0,bad_seen=0;
 reg [31:0] rng=32'hc1407a61;
 initial begin
  for(integer style=0;style<4;style++)begin
   for(integer bit_no=0;bit_no<1941;bit_no++)begin
    rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;
    background[bit_no]=style==0 ? 0:style==1 ? 1:rng[0];
   end
   wrong=0;context_bits=background;#1;
   if(enabled!==1 || actual!==EXPECTED)$fatal(1,"SHARED_XZ known active result mismatch");known++;checks++;
   wrong=1;
   for(integer bit_no=0;bit_no<1941;bit_no++)for(integer mode=0;mode<2;mode++)begin
    context_bits=background;context_bits[bit_no]=mode ? 1'bz:1'bx;#1;
    if(enabled!==0 || actual!==original)$fatal(1,"SHARED_XZ fallback mismatch style=%0d bit=%0d mode=%0d",style,bit_no,mode);
    if(bad[114:83]===original[114:83])$fatal(1,"SHARED_XZ bad guard must change actual processor Q32..63");
    unknown++;bad_seen++;checks++;
   end
  end
  if(known!=4 || unknown!=15528 || bad_seen!=15528)$fatal(1,"SHARED_XZ incomplete coverage");
  $display("SHARED_CAPTURE_XZ_PASS known=%0d unknown=%0d checks=%0d bad_architectural_Q=%0d",known,unknown,checks,bad_seen);
  $finish;
 end
endmodule
'''


def run(source,out):
    assert not out.exists()
    s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
    assert s['status']=='PASS_FULL_PROCESSOR_SHARED_CAPTURE' and s['full_context_XZ_fallback'] and s['passive_consumer_snapshot']
    log=(source/'test.log').read_text();rows=re.findall(r'^SHARED_CAPTURE_SAMPLE input=([0-9a-f]+) result=([0-9a-f]+) fallback=([0-9a-f]+)$',log,re.M)
    assert len(rows)==1;sample,expected,fallback=rows[0]
    assert expected==fallback and int(sample,16)<(1<<678)
    packet=(source/'packet.sv').read_text();assert packet.count(KNOWN)==2
    binding='shared_inputs_known({held_q,clock_before,clock_after,pins_before,pins_after,\n    refresh_before_run_capture,cp_fast,cp_prefix_saved,cp_prefix_cp_saved,cp_after_cp_saved})'
    assert packet.count(binding)==2
    out.mkdir(parents=True)
    shutil.copy2(source/'adapters/dorado_local_task_capture_step.sv',out/'consumer.sv')
    mask=sum(1<<(108*i+35) for i in range(4))
    tb=TB.replace('KNOWN',KNOWN).replace('INPUT_VALUE',sample).replace('OUTPUT_VALUE',expected).replace('BAD_VALUE',format(mask,'x'))
    (out/'tb.sv').write_text(tb)
    for n in [Path(__file__).name,'generate_shared_control_capture.py']:shutil.copy2(Path(__file__).with_name(n),out/n)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),
               source_state_sha256=sha(source/'run-state.json'),source_test_log_sha256=sha(source/'test.log'),
               caller_context_bits=1941,original_results_compared=139,poisoned_context_is_outside_local_data_projection=True,
               files={p.name:sha(p) for p in out.iterdir()},actions=[],native_integrated=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save();code=1
    for label,cmd in [('compile',['iverilog','-g2012','-s','tb_shared_unknown','-o',str(out/'test.vvp'),str(out/'consumer.sv'),str(out/'tb.sv')]),('test',['vvp','-i',str(out/'test.vvp')])]:
        row=dict(label=label,command=cmd,started=time.time())
        with (out/(label+'.log')).open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);row['pid']=child.pid;state['actions'].append(row);save();code=child.wait()
        row.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    passed=code==0 and 'SHARED_CAPTURE_XZ_PASS known=4 unknown=15528 checks=15532 bad_architectural_Q=15528' in (out/'test.log').read_text()
    state.update(status='PASS_SHARED_CAPTURE_FULL_CONTEXT_XZ_FALLBACK' if passed else 'TERMINAL_SHARED_CAPTURE_XZ_NOT_ESTABLISHED',finished=time.time());save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve()))
