#!/usr/bin/env python3
"""Separate shared task clock resolution from parallel register capture.

The frozen consumer carries88 state bits through seven waves. Only the two
StartCycle bits feed its local clock resolution: the controller/task Qs are
supplied by the four shared frames. Resolve those clock events first, then
select each register's last capture with parallel reductions. Compare all139
outputs for all678 arbitrary binary inputs, with no assumptions. Other wave
parameters and the original complete unknown-context fallback stay unchanged.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from prove_cp_prefix_cache import module
from qualify_native_task_terminal import TASK, NAME, ORIGINAL, TOP
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

PARALLEL = '''
 // HM 2.3/4.9: preserve each shared control capture and processor epoch.
 // Clock resolution carries only StartCycle, never the88-bit data result.
 function automatic [138:0] parallel_capture(
  input [87:0] initial_q,input cb,ca,input [75:0] pb,pa,
  input [432:0] frames,input [1:0] canonical);
  reg [24:0] level[0:7],event_at[0:6];
  reg [6:0] event_column,last_flags,last_address,earlier_next;
  reg [87:0] clock_q,result_q;
  reg [107:0] frame;
  reg [27:0] source;
  reg [24:0] all_events,all_repeated;
  reg [15:0] half_q,half_result;
  reg start_h,start_l,repeat_curr,seen_next,select_next,selected_diff;
  reg any_flags,any_address,any_next,any_pp;
  reg [3:0] next_value;
  reg [15:0] flag_value;
  integer w,b,h,k,offset;
  begin
   source=capture_pins(initial_q,pa);repeat_curr=pa[68] && initial_q[75];
   start_h=initial_q[32];start_l=initial_q[48];
   level[0]=clocks(initial_q,cb,pb);
   for(w=0;w<7;w=w+1)begin
    frame=w<4 ? frames[108*w+:108]:{frames[431:384],frames[431:384],12'b0};
    clock_q=initial_q;clock_q[31:0]=frame[43:12];clock_q[79:64]=frame[59:44];
    clock_q[32]=start_h;clock_q[48]=start_l;
    level[w+1]=clocks(clock_q,ca,pa);
    event_at[w]=level[w+1] & ~level[w];
    event_at[w][8:0]=frame[8:0];event_at[w][19]=frame[9];
    event_at[w][20]=frame[10];event_at[w][21]=frame[10];event_at[w][22]=frame[11];
    if(event_at[w][9])start_h=!clock_q[23];
    if(event_at[w][14])start_l=!clock_q[23];
   end
   all_events=0;all_repeated=0;
   for(b=0;b<25;b=b+1)begin
    for(w=0;w<7;w=w+1)event_column[w]=event_at[w][b];
    all_events[b]=|event_column;
    for(w=0;w<7;w=w+1)begin
     // At least two rises, independent of processor data capture.
     for(k=0;k<w;k=k+1)all_repeated[b]=all_repeated[b] | (event_column[w]&event_column[k]);
    end
   end
   all_repeated[22]=all_repeated[22] | canonical[1];
   result_q=initial_q;result_q[31:0]=frames[415:384];result_q[79:64]=frames[431:416];
   result_q[32]=start_h;result_q[48]=start_l;
   for(h=0;h<2;h=h+1)begin
    offset=9+5*h;half_q=initial_q[32+16*h+:16];half_result=half_q;
    any_flags=0;any_address=0;any_next=0;any_pp=0;seen_next=0;
    last_flags=0;last_address=0;earlier_next=0;
    for(w=0;w<7;w=w+1)begin
     earlier_next[w]=seen_next;
     seen_next=seen_next | (event_at[w][offset+2] & !repeat_curr);
     any_next=any_next | event_at[w][offset+2];
     any_pp=any_pp | event_at[w][offset];
     last_flags[w]=event_at[w][offset+(h==0 ? 3:2)];
     last_address[w]=event_at[w][offset+4];
     for(k=w+1;k<7;k=k+1)begin
      last_flags[w]=last_flags[w] & !event_at[k][offset+(h==0 ? 3:2)];
      last_address[w]=last_address[w] & !event_at[k][offset+4];
     end
     any_flags=any_flags | last_flags[w];any_address=any_address | last_address[w];
    end
    // Data capture sees the stored next-task nibble BEFORE this event.
    selected_diff=0;select_next=0;
    for(w=0;w<7;w=w+1)begin
     selected_diff=selected_diff | (last_flags[w] & !earlier_next[w] & (|(half_q[11:8]^~source[23:20])));
     select_next=select_next | (last_address[w] & earlier_next[w]);
    end
    flag_value=processor_transfer(half_q,5'b11111,source,1'b0,h==1);
    if(any_flags)half_result[7:2]={selected_diff,flag_value[6:2]};
    if(any_next && !repeat_curr)half_result[11:8]=~source[23:20];
    if(any_address)half_result[15:12]=select_next ? ~source[23:20]:half_q[11:8];
    // This bit clears at a base-clock rise, independently of data/flags.
    event_column=0;for(w=0;w<7;w=w+1)event_column[w]=event_at[w][offset+1];
    if(|event_column)half_result[1]=0;
    half_result[0]=h==0 ? start_h:start_l;
    result_q[32+16*h+:16]=half_result;
    next_value=repeat_curr ? half_q[11:8] : (!half_q[0] ? half_q[15:12]:~source[23:20]);
    if(any_pp)result_q[80+4*h+:4]=next_value;
   end
   parallel_capture={result_q,all_events,all_repeated,1'b0};
  end
 endfunction
 wire [138:0] parallel_result=parallel_capture(state_q,clock_before,clock_after,pins_before,pins_after,shared_capture,canonical_task_clock);
'''


def transform(raw, bad=False):
    old=module(raw,NAME)
    marker=' reg [87:0] q,n;reg [24:0] previous,levels,captures,events,repeated;'
    assert old.count(marker)==1
    new=old.replace(marker,PARALLEL+'\n'+marker,1)
    token="1'b0,1'b1:if(shared_capture[432]){state_next,clock_events,repeated_events,unsettled}={q,events,repeated,((WAVES==16) ? 1'b0:!settled)};"
    assert new.count(token)==1
    replacement="1'b0,1'b1:if(shared_capture[432]){state_next,clock_events,repeated_events,unsettled}=WAVES==16 ? parallel_result:{q,events,repeated,!settled};"
    new=new.replace(token,replacement,1)
    if bad:
        token='earlier_next[w]=seen_next;'
        assert new.count(token)==1
        new=new.replace(token,'earlier_next[w]=seen_next | (event_at[w][offset+2] & !repeat_curr);',1)
    return raw.replace(old,new,1)


def run(stage,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    raw=(stage/TASK).read_text();after=transform(raw)
    (out/'native-task-before.sv').write_text(raw);(out/'native-task-after.sv').write_text(after)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),stage=str(stage),
        stage_manifest_sha256=sha(stage/'r-front-control-native.json'),source_task_sha256=sha(stage/TASK),
        arbitrary_binary_input_bits=678,compared_outputs=139,assumptions=0,
        clock_recurrence_bits=2,wide_processor_state_recurrence_removed=True,
        unknown_fallback_and_nondefault_parameters_retained=True,
        files={p.name:sha(p)for p in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,bad in [('complete-result',False),('wrong-data-epoch',True)]:
        d=out/label;d.mkdir()
        (d/'original.sv').write_text(module(raw,ORIGINAL)+'\n'+module(raw,NAME))
        (d/'actual.sv').write_text(module(transform(raw,bad),NAME).replace('module '+NAME+' #','module native_task_terminal #',1))
        (d/'top.sv').write_text(TOP)
        script=f'''read_verilog -sv -defer /work/original.sv /work/actual.sv /work/top.sv
hierarchy -check -top native_task_terminal_contract
proc
flatten
opt
check -assert
sat -timeout 120 -prove equivalent 1 -{'falsify'if bad else'verify'} -show-ports -dump_json /work/counterexample.json
'''
        (d/'proof.ys').write_text(script)
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        command=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with(d/'proof.log').open('x')as log:
            child=subprocess.Popen(command,stdout=log,stderr=log,env=env)
            row=dict(label=label,negative=bad,pid=child.pid,command=command,started=time.time(),
                files={p.name:sha(p)for p in d.iterdir()if p.name!='proof.log'})
            state['actions'].append(row);save();code=child.wait()
        marker='model found: FAIL!'if bad else'no model found: SUCCESS!'
        row.update(returncode=code,passed=code==0 and marker in(d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(label,row['passed'],flush=True)
        if not row['passed']:break
    passed=len(state['actions'])==2 and all(a['passed']for a in state['actions'])
    state.update(status='PASS_ACTUAL_NATIVE_TASK_PARALLEL_CAPTURE'if passed else'TERMINAL_NATIVE_TASK_PARALLEL_NOT_ESTABLISHED',
        finished=time.time(),native_selected=False,physical_timing_qualified=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.output.resolve(),a.yosys.resolve()))
