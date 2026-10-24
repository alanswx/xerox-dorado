#!/usr/bin/env python3
"""Replay the full original capture oracle with one SRAM/owner commit per packet.

Use two stateless844-Q transfers for eligible adjacent FALL/RISE events. Retain
both complete2470-bit results, full9216-bit RAM and100 clocks per subevent.
The one original physical SRAM owner writes only once at packet close; read
forwarding supplies the intermediate write. A checked port/pin certificate
rejects unsupported pairs; single-event evaluation remains exact. This tests
composition, not actual source/device timestamps or native15ns timing.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from trace_r_front_transactions import layout
from fit_r_front import quartus_cone
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]


def packet(raw,mutation=None):
    ports,bits=layout(raw)
    declarations='\n'.join(f' wire {w} {prefix}_{n};' for prefix in ['first','second'] for n,w in ports)
    declarations+='\n wire first_cp_ready,second_cp_ready;'
    calls=[]
    for prefix in ['first','second']:
        first=prefix=='first'
        bindings=dict(host_clk='host_clk',advance="1'b0",held_q=('held_q' if first or mutation=='stale-intermediate' else 'first_held_next'),
            clock_before=('clock_before' if first else "1'b0"),clock_after=("use_pair ? 1'b0 : clock_after" if first else "1'b1"),
            pins_before=('pins_before' if first else 'pins_after'),pins_after='pins_after',refresh_before_run_capture='refresh',
            cp_reset="1'b0",external_file_read=('original_read' if first or mutation=='drop-forward' else 'later_read'),cp_ready=prefix+'_cp_ready')
        bindings.update({n:prefix+'_'+n for n,w in ports})
        calls.append(' dorado_r_front_pure_step '+prefix+'(\n  '+',\n  '.join(f'.{n}({v})' for n,v in bindings.items())+');')
    pack=lambda p:'{'+','.join(p+'_'+n for n,w in ports)+'}'
    commit="1'b1" if mutation=='early-write' else 'advance && ready'
    return '''`default_nettype none
module dorado_r_front_packet_step(
 input wire host_clk,advance,use_pair,input wire [843:0] held_q,
 input wire clock_before,clock_after,input wire [400:0] pins_before,pins_after,
 input wire refresh,output wire [2469:0] first_result,last_result,
 output reg ready,output wire [1:0] forwarded);
 DECLARATIONS
 wire [17:0] original_read,later_read;
 CALLS
 dorado_r_pair_read_forward forwarding(original_read,first_file_write_data,
  first_r_write_n,first_port_select_n,later_read,forwarded);
 always @*begin
  ready=!use_pair;
  if(use_pair)begin
   ready=0;
   case(^{held_q,clock_before,clock_after,pins_before,pins_after,refresh,original_read})
    1'b0,1'b1:ready=clock_before && clock_after && pins_before==pins_after &&
     first_port_address==second_port_address && first_port_select_n==second_port_select_n &&
     second_r_write_n==2'b11 && !first_unsettled && !second_unsettled &&
     !(|first_repeated_events) && !(|second_repeated_events);
    default:ready=0;
   endcase
  end
 end
 dorado_r_file_ports files(host_clk,COMMIT,first_r_write_n,first_port_address,first_port_select_n,
  first_file_write_data,first_port_address,first_port_select_n,original_read[15:0],original_read[17:16]);
 assign first_result=FIRST;
 assign last_result=use_pair ? SECOND : FIRST;
endmodule
`default_nettype wire
'''.replace('DECLARATIONS',declarations).replace('CALLS','\n'.join(calls)).replace('COMMIT',commit).replace('FIRST',pack('first')).replace('SECOND',pack('second'))

TB='''`timescale 1ns/1ps
module tb_r_front_packet;
 localparam integer COUNT=14884;
 reg host_clk=0,advance=0,use_pair=0;
 reg [843:0] held_q=0;reg cb=1,ca=1,refresh=0;
 reg [400:0] pb=0,pa=0;
 wire [2469:0] first_result,last_result;wire ready;wire [1:0] forwarded;
 dorado_r_front_packet_step dut(host_clk,advance,use_pair,held_q,cb,ca,pb,pa,refresh,first_result,last_result,ready,forwarded);
 reg [13333:0] records[0:COUNT-1];reg [10059:0] initial_image[0:0];
 reg [843:0] old_q,next_old_q;
 reg rb,ra,next_rb,next_ra;
 reg [400:0] before_pins,after_pins,next_pb,next_pa;
 reg [2469:0] expected_first,expected_last,unused_result;
 reg [9215:0] expected_memory,first_memory,unused_memory,old_memory;
 reg [843:0] expected_q;
 integer packets=0,pairs=0,fall_candidates=0,port_fallbacks=0,paired_writes=0,forwarded_bytes=0;
 integer idx=0,take=1;reg eligible;
 integer clock_totals[0:99];integer expected_clock_totals[0:99];
 function automatic [9215:0] memory_image();
  reg [9215:0] image;
  begin
   for(integer i=0;i<256;i++)begin
    image[4*i+:4]=dut.files.rm_h[i][3:0];image[1024+4*i+:4]=dut.files.rm_h[i][7:4];
    image[2048+4*i+:4]=dut.files.stk_h[i][3:0];image[3072+4*i+:4]=dut.files.stk_h[i][7:4];
    image[4096+i]=dut.files.rm_h[i][8];image[4352+i]=dut.files.stk_h[i][8];
    image[4608+4*i+:4]=dut.files.rm_l[i][3:0];image[5632+4*i+:4]=dut.files.rm_l[i][7:4];
    image[6656+4*i+:4]=dut.files.stk_l[i][3:0];image[7680+4*i+:4]=dut.files.stk_l[i][7:4];
    image[8704+i]=dut.files.rm_l[i][8];image[8960+i]=dut.files.stk_l[i][8];
   end
   memory_image=image;
  end
 endfunction
 always @(posedge host_clk)if(advance && ready)held_q<=last_result[2469:1626];
 initial begin
  $readmemh("TRACE_FILE",records);$readmemh("INITIAL_FILE",initial_image);
  {held_q,old_memory}=initial_image[0];
  for(integer i=0;i<256;i++)begin
   dut.files.rm_h[i]={old_memory[4096+i],old_memory[1024+4*i+:4],old_memory[4*i+:4]};
   dut.files.stk_h[i]={old_memory[4352+i],old_memory[3072+4*i+:4],old_memory[2048+4*i+:4]};
   dut.files.rm_l[i]={old_memory[8704+i],old_memory[5632+4*i+:4],old_memory[4608+4*i+:4]};
   dut.files.stk_l[i]={old_memory[8960+i],old_memory[7680+4*i+:4],old_memory[6656+4*i+:4]};
  end
  for(integer i=0;i<100;i++)begin clock_totals[i]=0;expected_clock_totals[i]=0;end
  #2;
  while(idx<COUNT)begin
   {old_q,rb,ra,before_pins,after_pins,expected_first,first_memory}=records[idx];
   if(held_q!==old_q)$fatal(1,"PACKET old closed owner mismatch record=%0d",idx);
   eligible=0;
   if(idx+1<COUNT)begin
    {next_old_q,next_rb,next_ra,next_pb,next_pa,expected_last,expected_memory}=records[idx+1];
    eligible=rb && !ra && !next_rb && next_ra && before_pins==after_pins && before_pins==next_pb && before_pins==next_pa;
   end
   cb=rb;ca=eligible ? 1'b1:ra;pb=before_pins;pa=after_pins;use_pair=eligible;take=1;#2;
   if(eligible)begin
    fall_candidates++;
    if(!ready)begin
     port_fallbacks++;use_pair=0;ca=ra;#2;
    end else begin take=2;pairs++;end
   end
   if(!ready)$fatal(1,"PACKET unready single record=%0d",idx);
   if(first_result!==expected_first)begin
    for(integer i=0;i<2470;i++)if(first_result[i]!==expected_first[i])$display("PACKET_DIFF first bit=%0d actual=%b expected=%b",i,first_result[i],expected_first[i]);
    $fatal(1,"PACKET first result mismatch record=%0d",idx);
   end
   if(take==2)begin
    if(last_result!==expected_last)begin
     for(integer i=0;i<2470;i++)if(last_result[i]!==expected_last[i])$display("PACKET_DIFF last bit=%0d actual=%b expected=%b",i,last_result[i],expected_last[i]);
     $fatal(1,"PACKET second result mismatch record=%0d",idx);
    end
    if(dut.first_r_write_n!=2'b11)paired_writes++;
    forwarded_bytes+=int'(forwarded[0])+int'(forwarded[1]);
   end else begin expected_last=expected_first;expected_memory=first_memory;end
   expected_q=expected_last[2469:1626];
   old_memory=memory_image();advance=0;host_clk=1;#2;host_clk=0;#2;
   if(memory_image()!==old_memory || held_q!==old_q)$fatal(1,"PACKET hold changed memory/owner record=%0d",idx);
   if(first_result!==expected_first || last_result!==expected_last)$fatal(1,"PACKET hold changed result record=%0d",idx);
   for(integer i=0;i<100;i++)begin
    clock_totals[i]+=int'(first_result[1526+i])+(take==2 ? int'(last_result[1526+i]):0);
    expected_clock_totals[i]+=int'(expected_first[1526+i])+(take==2 ? int'(expected_last[1526+i]):0);
   end
   advance=1;host_clk=1;#2;advance=0;host_clk=0;#2;
   if(held_q!==expected_q)$fatal(1,"PACKET committed owner mismatch record=%0d",idx);
   if(memory_image()!==expected_memory)$fatal(1,"PACKET committed RAM mismatch record=%0d",idx);
   idx+=take;packets++;
  end
  if(pairs<1000 || paired_writes==0 || forwarded_bytes==0)$fatal(1,"PACKET coverage incomplete pairs=%0d writes=%0d forward=%0d",pairs,paired_writes,forwarded_bytes);
  for(integer i=0;i<100;i++)begin
   if(clock_totals[i]!=expected_clock_totals[i])$fatal(1,"PACKET clock count mismatch channel=%0d",i);
   $display("PACKET_CLOCK channel=%0d edges=%0d",i,clock_totals[i]);
  end
  $display("FULL_PROCESSOR_PACKET_PASS transactions=%0d commits=%0d pairs=%0d candidates=%0d fallbacks=%0d paired_writes=%0d forwarded_bytes=%0d Qs=844 RAM=9216 results=2470 clocks=100",idx,packets,pairs,fall_candidates,port_fallbacks,paired_writes,forwarded_bytes);
  $finish;
 end
endmodule
'''


def run(source,trace,out,engine,mutation,quartus_syntax=False,geometry_checks=False):
    assert not out.exists();s=json.loads((source/'run-state.json').read_text());t=json.loads((trace/'run-state.json').read_text())
    assert s['status']=='PASS_FULL_PROCESSOR_STATELESS_EVALUATOR'
    assert t['status']=='PASS_FULL_ORIGINAL_PROCESSOR_TRANSACTION_TRACE' and t['records']==14884
    assert sha(trace/'transactions.hex')==t['trace_sha256'] and sha(trace/'initial.hex')==t['initial_sha256']
    out.mkdir(parents=True);paths=[];rows=[]
    for row in s['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256']
        if p.name=='tb_r_front_local.sv' or row['path'].startswith('timed/'):continue
        q=out/row['path'];q.parent.mkdir(parents=True,exist_ok=True);text=p.read_text()
        if quartus_syntax and p.name in ['dorado_r_front_cone.sv','dorado_r_read_window_step.sv']:
            if p.name=='dorado_r_front_cone.sv':text=quartus_cone(text)
            else:
                old=' generate for(genvar byte_no=0;';new=' genvar byte_no;\n generate for(byte_no=0;'
                assert text.count(old)==1;text=text.replace(old,new,1);assert text.replace(new,old,1)==p.read_text()
            native=ROOT/'build/rtl-executive/selector-native-stage-v2-20261024/src/verilog/rtl'/p.name
            assert text==native.read_text(),p.name
        q.write_text(text);paths.append(q)
        rows.append(dict(path=row['path'],source_sha256=sha(p),output_sha256=sha(q)))
    raw=(source/'candidate.sv').read_text();(out/'packet.sv').write_text(packet(raw,mutation));paths.append(out/'packet.sv')
    (out/'forward.sv').write_bytes((ROOT/'verilog/rtl/dorado_r_pair_read_forward.sv').read_bytes());paths.append(out/'forward.sv')
    bench=TB
    if geometry_checks:
        bench=bench.replace(' integer idx=0,take=1;reg eligible;',' integer idx=0,take=1,geometry_rejections=0;reg eligible;reg [400:0] geometry_pa;reg geometry_cb,geometry_ca;')
        marker='   expected_q=expected_last[2469:1626];'
        stress='''   if(take==2 && dut.first_r_write_n!=2'b11 && geometry_rejections==0)begin
    geometry_pa=pa;geometry_cb=cb;geometry_ca=ca;old_memory=memory_image();
    for(integer k=0;k<403;k++)begin
     pa=geometry_pa;cb=geometry_cb;ca=geometry_ca;
     if(k<401)pa[k]=!pa[k];else if(k==401)cb=0;else ca=0;
     #2;if(ready!==1'b0)$fatal(1,"PACKET invalid geometry accepted bit=%0d",k);
     advance=1;host_clk=1;#2;advance=0;host_clk=0;#2;
     if(memory_image()!==old_memory || held_q!==old_q)$fatal(1,"PACKET invalid geometry committed bit=%0d",k);
     geometry_rejections++;
    end
    pa=geometry_pa;cb=geometry_cb;ca=geometry_ca;#2;
   end
'''
        assert bench.count(marker)==1;bench=bench.replace(marker,stress+marker,1)
        bench=bench.replace('  for(integer i=0;i<100;i++)begin\n   if(clock_totals',
            '  if(geometry_rejections!=403)$fatal(1,"PACKET geometry coverage incomplete");\n  $display("PACKET_GEOMETRY_REJECTIONS cases=%0d",geometry_rejections);\n  for(integer i=0;i<100;i++)begin\n   if(clock_totals',1)
    (out/'tb.sv').write_text(bench.replace('TRACE_FILE',str(trace/'transactions.hex')).replace('INITIAL_FILE',str(trace/'initial.hex')));paths.append(out/'tb.sv')
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,engine=engine,mutation=mutation,
        source=str(source),trace=str(trace),trace_state_sha256=sha(trace/'run-state.json'),source_files=rows,
        files={p.name:sha(p) for p in [out/'packet.sv',out/'forward.sv',out/'tb.sv',out/Path(__file__).name]},
        actions=[],quartus_syntax=quartus_syntax,geometry_checks=geometry_checks,one_physical_SRAM_owner=True,commits_only_at_packet_end=True,all2470_bits_per_subevent_checked=True,
        source_and_device_timestamp_qualified=False,native_integrated=False,timing_qualified=False,hardware_changed=False)

    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')

    commands=([['iverilog','-g2012','-s','tb_r_front_packet','-o',str(out/'test.vvp'),*map(str,paths)],['vvp','-i',str(out/'test.vvp')]] if engine=='icarus' else
        [['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_packet','--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    save()
    for label,cmd in zip(['compile','test'],commands):
        with (out/(label+'.log')).open('x') as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);row=dict(label=label,command=cmd,pid=p.pid,started=time.time())
            state['actions'].append(row);save();code=p.wait();row.update(returncode=code,finished=time.time());save()
        if code:break
    log=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    if mutation:passed=state['actions'][0]['returncode']==0 and code!=0 and 'PACKET ' in log
    else:
        m=re.search(r'FULL_PROCESSOR_PACKET_PASS (.+)',log);passed=code==0 and m is not None
        if passed:
            state['summary']=m[1]
            if geometry_checks:assert 'PACKET_GEOMETRY_REJECTIONS cases=403' in log
    state.update(status=('PASS_FULL_PACKET_COUNTEREXAMPLE' if mutation else 'PASS_FULL_PROCESSOR_PACKET_SINGLE_COMMIT') if passed else
        'TERMINAL_PROCESSOR_PACKET_NOT_ESTABLISHED',finished=time.time(),test_log_sha256=sha(out/'test.log') if (out/'test.log').exists() else None);save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','trace','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['verilator','icarus'],default='verilator');p.add_argument('--mutation',choices=['drop-forward','stale-intermediate','early-write'])
    p.add_argument('--quartus-syntax',action='store_true');p.add_argument('--geometry-checks',action='store_true')
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.trace.resolve(),a.output.resolve(),a.engine,a.mutation,a.quartus_syntax,a.geometry_checks))
