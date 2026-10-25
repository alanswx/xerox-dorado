#!/usr/bin/env python3
"""Explain the real128-record prefix with the native write-window protocol.

Enumerate ALL1024 initial histories through the frozen native cell and require
a unique history compatible with the observed first RAM transition. Replay the
complete contiguous prefix with an independently retained helper state. This
deduces, rather than directly samples, that initial history; no source machine,
firmware, original assertion or bound changes. Other flag clocks are unexercised.
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

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
TB='''`timescale 1ns/1ps
module tb_native_flag_prefix;
 reg clk=0;always #5 clk=~clk;
 reg [198:0] records[0:127],row;
 wire [79:0] before_flags=row[198:119],after_flags=row[79:0];
 wire [3:0] levels;wire [23:0] data;
 branch_condition_cone before_view(.fixture_load(1'b0),.fixture_state(before_flags),
  .base(row[80]),.start_a(1'b0),.en(1'b0),.pins(row[118:81]),.clocks(levels),.capture_data(data));
 reg freeze=1,request=0,guest=0;reg [4:0] index=0;reg [9:0] load_data=0;
 wire [63:0] actual_memory;wire [9:0] actual_window;
 cell_F10145A_snapshot original(.state_host_clk(clk),.state_advance(guest),
  .state_freeze(freeze),.state_guest_held(freeze),.state_request(request),.state_write(1'b1),
  .state_index(index),.state_data(load_data),.physical_memory(actual_memory),.physical_window(actual_window),.sys_clk(clk),
  .p10(data[19]),.p9(data[18]),.p7(data[17]),.p6(data[16]),
  .p12(data[23]),.p11(data[22]),.p4(data[21]),.p5(data[20]),.p3(1'b0),.p13(levels[3]),.p8(1'b0));
 reg [73:0] own_state,next_state;wire [73:0] helper_next;
 dorado_flag_file_window_step helper(own_state[73:10],own_state[9:0],1'b1,
  data[19:16],data[23:20],1'b0,levels[3],helper_next[73:10],helper_next[9:0]);
 task automatic tick;
  begin @(posedge clk);#1;@(negedge clk);end
 endtask
 task automatic load(input [63:0] memory,input [9:0] window);
  begin
   freeze=1;request=0;guest=0;tick();
   for(integer k=0;k<16;k++)begin request=1;index=k;load_data={6'b0,memory[4*k+:4]};tick();end
   request=1;index=16;load_data=window;tick();request=0;freeze=0;
  end
 endtask
 integer compatible=0,witness=-1,matched=0;
 initial begin
  $readmemh("RECORDS",records);@(negedge clk);row=records[0];#1;
  $display("NATIVE_FILE_FIRST_PINS clocks=%h data=%h",levels,data);
  for(integer w=0;w<1024;w++)begin
   load(before_flags[63:0],10'(w));guest=1;tick();guest=0;
   if(actual_memory===after_flags[63:0])begin compatible++;witness=w;end
  end
  if(compatible!=1)$fatal(1,"NATIVE_FILE_HISTORY not uniquely established count=%0d",compatible);
  $display("NATIVE_FILE_INFERRED_HISTORY possibilities=%0d window=%h",compatible,10'(witness));
  own_state={before_flags[63:0],BAD_WINDOW ? 10'b0:10'(witness)};
  load(own_state[73:10],own_state[9:0]);
  for(integer n=0;n<128;n++)begin
   row=records[n];#1;
   if(own_state[73:10]!==before_flags[63:0])$fatal(1,"NATIVE_FILE_REPLAY before mismatch index=%0d",n);
   next_state=helper_next;guest=1;tick();guest=0;
   if({actual_memory,actual_window}!==next_state)$fatal(1,"NATIVE_FILE_REPLAY helper/native mismatch index=%0d",n);
   if(actual_memory!==after_flags[63:0])
    $fatal(1,"NATIVE_FILE_REPLAY measured RAM mismatch index=%0d actual=%h observed=%h",n,actual_memory,after_flags[63:0]);
   own_state=next_state;matched++;
  end
  $display("NATIVE_FLAG_FILE_PREFIX_PASS records=%0d file_bits=64 history_bits=10 unique_inferred_history=%h final_window=%h",matched,10'(witness),own_state[9:0]);$finish;
 end
endmodule
'''


def run(analysis,proof,out,engine,bad=False):
    assert not out.exists();out.mkdir(parents=True)
    a=json.loads((analysis/'results.json').read_text())
    assert a['status']=='OBSERVED_NATIVE_ALU_PHASE_PREFIX_MISMATCH'and a['observed_records']==128
    assert len(a['mismatches'])==1 and a['mismatches'][0]['index']==0 and a['mismatches'][0]['differing_bits']==[0]
    assert not a['noncontinuous_physical_frames']and not any(a['candidate_event_counts'])
    p=json.loads((proof/'run-state.json').read_text());verify(proof,p['files'])
    assert p['status']=='PASS_ACTUAL_NATIVE_FLAG_FILE_WINDOW_TRANSFER'and all(x['passed']for x in p['actions'])
    raw=(analysis/'native-phase-records.log').read_text().splitlines();packed=[];times=[]
    for line in raw:
        r=dict(re.findall(r'(\w+)=([^ ]+)',line));before=(int(r['before'],16)>>844)&((1<<80)-1);after=(int(r['after'],16)>>844)&((1<<80)-1)
        value=(before<<119)|(int(r['before_flags'],16)<<81)|(int(r['flag_clock'][0])<<80)|after
        packed.append(f'{value:050x}');times.append((int(r['host']),int(r['guest'])))
    assert len(packed)==128 and all((h2-h,g2-g)==(1,1)for(h,g),(h2,g2)in zip(times,times[1:]))
    (out/'records.hex').write_text('\n'.join(packed)+'\n')
    for n in ['original.sv','actual.sv']:shutil.copy2(proof/'complete-storage'/n,out/n)
    source=BUILD/'native-alu-observation-stage-v3-20261025'
    m=json.loads((source/'r-front-control-native.json').read_text());verify(source,m['source_hashes'])
    cone='src/verilog/rtl/dorado_branch_condition_cone.sv';shutil.copy2(source/cone,out/'cone.sv')
    tb=TB.replace('RECORDS',str(out/'records.hex')).replace('BAD_WINDOW',"1'b1"if bad else"1'b0")
    (out/'tb.sv').write_text(tb);shutil.copy2(Path(__file__),out/Path(__file__).name)
    s=dict(status='RUNNING',scope=__doc__,started=time.time(),engine=engine,bad_missing_history=bad,
        analysis=str(analysis),analysis_state_sha256=sha(analysis/'results.json'),proof=str(proof),proof_state_sha256=sha(proof/'run-state.json'),
        all_initial_1024_histories_enumerated=True,initial_history_is_inferred_not_directly_sampled=True,
        all74_native_storage_bits_compared=True,independent_helper_history_retained=True,
        files={p.name:sha(p)for p in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(s,indent=2)+'\n')
    paths=[str(out/n)for n in ['original.sv','actual.sv','cone.sv','tb.sv']]
    commands=([['iverilog','-g2012','-s','tb_native_flag_prefix','-o',str(out/'test.vvp'),*paths],['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else
              [['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_native_flag_prefix','--Mdir',str(out/'obj'),'-o','test',*paths],[str(out/'obj/test')]])
    save();code=1
    for label,cmd in zip(['compile','test'],commands):
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log);row=dict(label=label,command=cmd,pid=child.pid,started=time.time())
            s['actions'].append(row);save();code=child.wait()
        row.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    text=(out/'test.log').read_text()if(out/'test.log').exists()else''
    passed=s['actions'][0]['returncode']==0 and ((code!=0 and'measured RAM mismatch index=0'in text)if bad else
        code==0 and'NATIVE_FLAG_FILE_PREFIX_PASS records=128 file_bits=64 history_bits=10 unique_inferred_history=220 final_window=221'in text)
    s.update(status=('PASS_NATIVE_FLAG_FILE_MISSING_HISTORY_COUNTEREXAMPLE'if bad else'PASS_NATIVE_FLAG_FILE_OBSERVED_PREFIX_COMPATIBILITY')if passed else
             'TERMINAL_NATIVE_FLAG_FILE_PREFIX_NOT_ESTABLISHED',returncode=code,finished=time.time(),native_arrival_contract_qualified=False,
             firmware_or_timing_or_speed_qualified=False,source_run_or_media_changed=False)
    save();print(s['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['analysis','proof','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True);p.add_argument('--bad-missing-history',action='store_true')
    a=p.parse_args();raise SystemExit(run(a.analysis.resolve(),a.proof.resolve(),a.output.resolve(),a.engine,a.bad_missing_history))
