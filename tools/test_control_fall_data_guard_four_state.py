#!/usr/bin/env python3
"""Replay the proved changed-data witness and every X/Z control input bit.

The fixed HIGH->LOW domain has565 variable input bits. Preserve complete363-bit
case equality and the original unknown-input generic fallback. The bad default
must fail actual control results; a debug guard difference alone does not pass
the negative calibration. No native integration or timing qualification.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from test_r_front_forward_view import sha

TB='''`timescale 1ns/1ps
module tb_fall_data_four_state;
 reg [564:0] stimulus,base;
 wire [155:0] q;wire [397:0] before_pins;wire [8:0] changes;wire refresh,entry;
 assign {q,before_pins,changes,refresh,entry}=stimulus;
 wire equivalent,certificate,old_sparse,new_sparse,witnessed;wire [362:0] actual,expected;
 fall_guard_equivalence dut(q,before_pins,changes,refresh,entry,equivalent,certificate,
  old_sparse,new_sparse,actual,expected,witnessed);
 integer checks=0,known=0,unknown=0;
 task automatic compare(input integer trial,input integer bit_no,input integer mode);
  begin
   #1;
   if(actual!==expected)$fatal(1,"FALL_DATA_XZ result mismatch trial=%0d bit=%0d mode=%0d actual=%h expected=%h",trial,bit_no,mode,actual,expected);
   checks++;
  end
 endtask
 initial begin
  for(integer trial=0;trial<4;trial++)begin
   base={156'hQ,398'hP,9'hC,1'bR,1'b1};
   case(trial)1:base[10:2]^=9'h1f8;2:base[10:2]=9'h1f8;3:base[10:2]=9'h008;endcase
   stimulus=base;compare(trial,-1,0);known++;
   if(certificate!==1 || old_sparse!==0 || new_sparse!==1 || witnessed!==1)$fatal(1,"FALL_DATA_XZ changed-data witness lost");
   for(integer b=0;b<565;b++)begin
    for(integer mode=0;mode<2;mode++)begin
     stimulus=base;stimulus[b]=mode ? 1'bz:1'bx;compare(trial,b,mode+2);unknown++;
     if(CHECK_GUARD && (old_sparse!==0 || new_sparse!==0))$fatal(1,"FALL_DATA_XZ unknown input selected sparse path");
    end
   end
  end
  if(known!=4 || unknown!=4520)$fatal(1,"FALL_DATA_XZ coverage");
  $display("FALL_DATA_FOUR_STATE_PASS known=%0d unknown=%0d checks=%0d",known,unknown,checks);$finish;
 end
endmodule
'''


def run(proof,out,mutation):
    assert not out.exists();p=json.loads((proof/'run-state.json').read_text())
    assert p['status']=='PASS_SOURCE_BOUND_FALL_DATA_GUARD' and len(p['cases'])==5 and all(r['passed'] for r in p['cases'])
    for n,h in p['files'].items():assert sha(proof/n)==h
    row=next(r for r in p['cases'] if r['mode']=='cover-data-clock');cover=proof/'cover-data-clock-w5'
    assert sha(cover/'proof.log')==row['log_sha256']
    signals={s['name']:s for s in json.loads((cover/'counterexample.json').read_text())['signal']}
    def value(name):
        s=signals[name];return int(s['data'][0],2) if 'data' in s else int(s['wave'][0],2)
    assert value('witnessed_data_change')==value('certificate')==value('new_sparse')==1 and value('old_sparse')==0
    source=proof/'positive-w5';r=next(v for v in p['cases'] if v['mode']=='positive' and v['waves']==5)
    for n,h in r['files'].items():assert sha(source/n)==h
    out.mkdir(parents=True)
    for n in ['atomic.sv','fall.sv','original.sv','top.sv']:shutil.copy2(source/n,out/n)
    actual=(source/'actual.sv').read_text()
    if mutation:
        assert actual.count('default:using_sparse=0;')==1
        actual=actual.replace('default:using_sparse=0;','default:using_sparse=1;',1)
    (out/'actual.sv').write_text(actual)
    tb=TB.replace("156'hQ",f"156'h{value('q'):x}").replace("398'hP",f"398'h{value('before_pins'):x}").replace("9'hC",f"9'h{value('changes'):x}").replace("1'bR",f"1'b{value('refresh')}").replace('CHECK_GUARD',"1'b0" if mutation else "1'b1")
    (out/'tb.sv').write_text(tb);shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,proof=str(proof),proof_state_sha256=sha(proof/'run-state.json'),mutation=mutation,
        variable_input_bits=565,compared_control_bits=363,files={p.name:sha(p) for p in out.iterdir()},actions=[],native_integrated=False,hardware_changed=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,cmd in [('compile',['iverilog','-g2012','-s','tb_fall_data_four_state','-o',str(out/'test.vvp'),*[str(out/n) for n in ['atomic.sv','fall.sv','original.sv','actual.sv','top.sv','tb.sv']]]),
                      ('test',['vvp','-i',str(out/'test.vvp')])]:
        with (out/(label+'.log')).open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);row=dict(label=label,command=cmd,pid=child.pid,started=time.time());state['actions'].append(row);save();code=child.wait()
        row.update(returncode=code,finished=time.time());save()
        if code:break
    log=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    passed=state['actions'][0]['returncode']==0 and ((code!=0 and 'FALL_DATA_XZ result mismatch' in log) if mutation else
        (code==0 and 'FALL_DATA_FOUR_STATE_PASS known=4 unknown=4520 checks=4524' in log))
    state.update(status=('PASS_FALL_DATA_UNKNOWN_COUNTEREXAMPLE' if mutation else 'PASS_FALL_DATA_FOUR_STATE') if passed else 'TERMINAL_FALL_DATA_FOUR_STATE_NOT_ESTABLISHED',
        finished=time.time(),test_log_sha256=sha(out/'test.log') if (out/'test.log').exists() else None);save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['proof','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--mutation',choices=['bad-unknown'])
    a=p.parse_args();raise SystemExit(run(a.proof.resolve(),a.output.resolve(),a.mutation))
