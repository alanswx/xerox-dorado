#!/usr/bin/env python3
"""Compare every original saved-flag/TIOA bit and clock at legal boundary frames.

Exercise all16 task slots, arithmetic carry consumption after task restoration,
logical preservation, FreezeBC and write suppression. ALU status inputs are
supplied by independent arithmetic in the bench; this is not an original
microinstruction execution test or native phase/timing qualification.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from generate_branch_condition_reference import generate,ROOT
from test_external_r_packet import sha

PROFILES={'balanced':(1000,1000,1000),'early':(1750,400,1000),'aperture':(3200,1500,1000)}
TB=r'''`timescale 1ns/1ps
module tb_branch;
 localparam integer PINS=PIN_COUNT;
 reg load=0,root=1,cb=1,ca=1;reg [PINS-1:0] rp=0,p=0,pb=0,pa=0;
 reg [79:0] fixture=0,q=0;wire [79:0] reference,n;
 wire [3:0] clocks,flags,events;wire unsettled;
 branch_condition_reference original(.fixture_load(load),.fixture_state(fixture),
  .base(root),.start_a(1'b0),.en(1'b0),.pins(rp),.state_q(reference),.clocks(clocks),.flags(flags));
 dorado_branch_condition_step #(.PINS(PINS)) dut(q,cb,ca,pb,pa,n,events,unsettled);
 integer counts[0:3];integer checks=0,restores=0,operations=0,logical_ops=0,freezes=0,carry_ones=0;
 reg monitor=0;reg [15:0] written=0,restored=0;reg [79:0] changed=0;
 wire [3:0] reference_address={original.e13.p10,original.e13.p9,original.e13.p7,original.e13.p6};
 wire [3:0] reference_write_data={original.e13.p12,original.e13.p11,original.e13.p4,original.e13.p5};
 reg window_open=0;reg [3:0] window_address,window_data;integer write_windows=0;
 always @(negedge original.e13.p13)if(monitor)begin
  window_open=1;window_address=reference_address;window_data=reference_write_data;write_windows++;
 end
 always @(posedge original.e13.p13)window_open=0;
 always @(reference_address or reference_write_data)
  if(monitor && window_open && !original.e13.p13 &&
     {reference_address,reference_write_data}!=={window_address,window_data})
   $fatal(1,"BRANCH original file address/data changed inside write window");
 reg [3:0] book[0:15];reg [31:0] rng=32'h9147bc03;
 for(genvar i=0;i<4;i++)begin:clock_count
  initial counts[i]=0;
  always @(posedge clocks[i])if(monitor)counts[i]++;
 end
 task automatic event_step(input next_root,input [PINS-1:0] value);
  reg [79:0] predicted;reg [3:0] predicted_events;integer old_count[0:3];
  begin
   cb=root;ca=next_root;pb=rp;pa=value;#1;
   predicted=n;predicted_events=events;
   if(unsettled)$fatal(1,"BRANCH solver unsettled check=%0d",checks);
   for(integer i=0;i<4;i++)old_count[i]=counts[i];
   rp=value;root=next_root;#100;
   for(integer i=0;i<80;i++)if(reference[i]!==predicted[i])
    $display("BRANCH_STATE_DIFF check=%0d bit=%0d predicted=%b original=%b",checks,i,predicted[i],reference[i]);
   if(reference!==predicted)$fatal(1,"BRANCH original state mismatch check=%0d",checks);
   for(integer i=0;i<4;i++)if(predicted_events[i]!==1'(counts[i]-old_count[i]) || counts[i]-old_count[i]>1)
    $fatal(1,"BRANCH clock mismatch check=%0d channel=%0d actual=%0d predicted=%b",checks,i,counts[i]-old_count[i],predicted_events[i]);
   changed|=q^predicted;q=predicted;checks++;
  end
 endtask
 task automatic address(input integer task_no);
  begin
   for(integer b=0;b<4;b++)p[PIN{CurrLast.0'}+b]=((task_no^15)>>(3-b))&1;
  end
 endtask
 task automatic restore_task(input integer task_no);
  begin
   if(!root)event_step(1,p);
   p[PIN{BCWriteEn'}]=1;p[PIN{Last=Curr'}]=1;address(task_no);event_step(1,p);
   event_step(0,p);
   if(q[67:64]!==book[task_no])$fatal(1,"BRANCH task restore mismatch task=%0d actual=%h expected=%h",task_no,q[67:64],book[task_no]);
   restored[task_no]=1;restores++;event_step(1,p);
  end
 endtask
 task automatic operation(input integer task_no,input [15:0] a,b,
                          input logical_mode,freeze_mode);
  reg [16:0] sum;reg [15:0] result;reg carry_in,overflow;
  reg [3:0] expected,old_flags;
  begin
   restore_task(task_no);old_flags=q[67:64];carry_in=q[66];
   if(carry_in)carry_ones++;
   sum={1'b0,a}+{1'b0,b}+carry_in;result=logical_mode ? a^b:sum[15:0];
   overflow=~(a[15]^b[15])&(a[15]^sum[15]);
   expected=freeze_mode ? book[task_no]:
    {logical_mode ? book[task_no][3]:overflow,
     logical_mode ? book[task_no][2]:sum[16],!result[15],result!=0};
   p[PIN{alu.00}]=result[15];p[PIN{aluOut=0'}]=result!=0;
   p[PIN{aluCout}]=logical_mode ? ~book[task_no][2]:sum[16];
   p[PIN{SignedCarry}]=logical_mode ? ~book[task_no][3]:overflow;
   p[PIN{aluM}]=logical_mode;
   p[PIN{ProcH11.sil+13}]=old_flags[2];p[PIN{ProcH11.sil+12}]=old_flags[3];
   for(integer bit_no=0;bit_no<8;bit_no++)p[PIN{TIOA.0}+bit_no]=(operations>>(7-bit_no))&1;
   // Keep the file disabled while staging the next Clock1 data; only then
   // open its Clock0 window. Existing native producers must eventually prove
   // this ordering; these boundary frames do not insert guest cycles.
   event_step(1,p);event_step(0,p);event_step(1,p);
   p[PIN{BCWriteEn'}]=freeze_mode;p[PIN{Last=Curr'}]=0;event_step(1,p);
   event_step(0,p);
   if(q[67:64]!==expected)$fatal(1,"BRANCH visible flags mismatch op=%0d actual=%h expected=%h",operations,q[67:64],expected);
   if(!freeze_mode)begin
    if(q[4*(task_no^15)+:4]!==expected)$fatal(1,"BRANCH saved flags mismatch task=%0d",task_no);
    written[task_no]=1;book[task_no]=expected;
   end
   event_step(1,p);operations++;if(logical_mode)logical_ops++;if(freeze_mode)freezes++;
  end
 endtask
 initial begin
  p[PIN{BCWriteEn'}]=1;p[PIN{FA=0'b}]=1;p[PIN{FA=1'a}]=1;p[PIN{FF.2'a}]=1;
  for(integer i=0;i<16;i++)begin fixture[4*i+:4]=4'(i);book[i]=4'(i^15);end
  rp=p;pb=p;pa=p;#100;load=1;#100;load=0;#100;q=reference;monitor=1;
  for(integer i=0;i<16;i++)restore_task(i);
  for(integer cycle_no=0;cycle_no<32;cycle_no++)begin
   for(integer task_no=0;task_no<16;task_no++)begin
    rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;
    operation(task_no,cycle_no[1:0]==0 ? 16'hffff:cycle_no[1:0]==1 ? 16'h7fff:rng[15:0],
              cycle_no[1:0]==0 ? 16'h0001:cycle_no[1:0]==1 ? 16'h0001:rng[31:16],
              cycle_no%4==2,cycle_no%4==3);
   end
  end
  for(integer i=15;i>=0;i--)restore_task(i);
  // Stop the original distributed clock by its physical enable boundary.
  // Changing every other source and both root levels must hold ALL80 bits.
  begin:disabled_clock
   reg [79:0] held;
   p[PIN{CLKEnable'c}]=1;event_step(1,p);held=q;
   for(integer pattern=0;pattern<4;pattern++)begin
    for(integer bit_no=0;bit_no<PINS;bit_no++)
     if(bit_no!=PIN{CLKEnable'c})p[bit_no]=(pattern==0 ? 0:pattern==1 ? 1:pattern==2 ? bit_no%2:1-bit_no%2);
    event_step(1,p);event_step(0,p);event_step(1,p);
    if(q!==held)$fatal(1,"BRANCH disabled clock changed stored state");
   end
   $display("BRANCH_DISABLED_CLOCK_PASS sources=%0d patterns=4",PINS-1);
  end
  // Walk all physical control/data boundaries with input settling separated
  // from root edges. Retain the original per-event checks and complete state.
  for(integer bit_no=0;bit_no<PINS;bit_no++)begin
   p=0;p[PIN{BCWriteEn'}]=1;p[PIN{FA=0'b}]=1;p[PIN{FA=1'a}]=1;p[PIN{FF.2'a}]=1;
   event_step(1,p);p[bit_no]=~p[bit_no];event_step(1,p);
   event_step(0,p);event_step(1,p);
  end
  if(written!=16'hffff || restored!=16'hffff || operations!=512 || logical_ops!=128 || freezes!=128 || carry_ones==0)
   $fatal(1,"BRANCH incomplete task/carry coverage");
  if(write_windows!=385 || window_open)$fatal(1,"BRANCH incomplete write-window coverage");
  $display("BRANCH_WRITE_WINDOWS_PASS windows=%0d address_changes=0 data_changes=0",write_windows);
  for(integer i=0;i<4;i++)begin
   if(counts[i]==0)$fatal(1,"BRANCH missing clock %0d",i);
   $display("BRANCH_CLOCK channel=%0d edges=%0d",i,counts[i]);
  end
  $display("BRANCH_CAPTURE_PASS checks=%0d Qs=80 file_bits=64 tasks=16 operations=%0d logical=%0d freeze=%0d carry_ones=%0d changed=%h",checks,operations,logical_ops,freezes,carry_ones,changed);
  $finish;
 end
endmodule
'''


def run(out,engine,profile,mutation=None):
    assert not out.exists();out.mkdir(parents=True)
    manifest=generate(ROOT,out/'reference',*PROFILES[profile]);pins=manifest['pin_index']
    source=ROOT/'verilog/rtl/dorado_branch_condition_step.sv';candidate=source.read_text()
    if mutation=='flag-select':
        target=out/'reference/cone.sv';raw=target.read_text()
        old=manifest['visible_mux_expression']
        assert raw.count(old)==1;target.write_text(raw.replace(old,'!'+old,1))
    elif mutation=='task-address':
        old='4*data[19:16]+:4';assert candidate.count(old)==1
        candidate=candidate.replace(old,"4*(data[19:16]^4'b1)+:4",1)
    elif mutation=='write-after-close':
        old='if(!clocks[3])';assert candidate.count(old)==1
        candidate=candidate.replace(old,'if(clocks[3] && !before_clocks[3])',1)
    (out/'candidate.sv').write_text(candidate)
    tb=TB.replace('PIN_COUNT',str(len(pins)))
    tb=re.sub(r'PIN\{([^}]+)\}',lambda m:str(pins[m[1]]),tb)
    (out/'tb.sv').write_text(tb)
    for name in [Path(__file__).name,'generate_branch_condition_reference.py']:
        shutil.copy2(Path(__file__).with_name(name),out/name)
    paths=[out/'candidate.sv',out/'reference/cone.sv',out/'reference/branch_condition_reference.sv',
           *sorted((out/'reference/timed').glob('*')),out/'tb.sv']
    record=dict(status='RUNNING',scope=__doc__,engine=engine,profile=profile,mutation=mutation,
                started=time.time(),original_candidate_sha256=sha(source),reference_manifest=manifest,
                files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file()},
                actions=[],native_integrated=False,physical_timing_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(record,indent=2)+'\n')
    commands=([['iverilog','-g2012','-s','tb_branch','-o',str(out/'test.vvp'),*map(str,paths)],
               ['vvp','-i',str(out/'test.vvp')]] if engine=='icarus' else
              [['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_branch',
                '--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    save();code=1
    for label,command in zip(['compile','test'],commands):
        with (out/(label+'.log')).open('x') as log:
            child=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT)
            row=dict(label=label,pid=child.pid,command=command,started=time.time());record['actions'].append(row);save()
            code=child.wait()
        row.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    passed=(code!=0 and 'BRANCH original state mismatch' in log) if mutation else (code==0 and 'BRANCH_CAPTURE_PASS' in log)
    record.update(status=('PASS_BRANCH_CAPTURE_COUNTEREXAMPLE' if mutation else 'PASS_BRANCH_CONDITION_CAPTURE')
                  if passed else 'TERMINAL_BRANCH_CAPTURE_NOT_ESTABLISHED',finished=time.time(),returncode=code,
                  outcomes=re.findall(r'^BRANCH_(?:CLOCK|CAPTURE_PASS) .*$',log,re.M))
    save();print(record['status']);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True)
    p.add_argument('--profile',choices=PROFILES,default='early')
    p.add_argument('--mutation',choices=['flag-select','task-address','write-after-close'])
    a=p.parse_args();raise SystemExit(run(a.output.resolve(),a.engine,a.profile,a.mutation))
