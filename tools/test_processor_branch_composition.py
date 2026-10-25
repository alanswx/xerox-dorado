#!/usr/bin/env python3
"""Replay the complete producer trace through two integrated original circuits.

Compare the monolithic924-bit graph against an original844-bit graph wired to
the original80-bit flag family. Revalidate the two newly closed g18 producer
fields through that comparison, retaining all842 unchanged physical state fields,
all9216 SRAM bits and all100 old clock counts. Physical state is recorded from
the original circuit after each event; held_next is the retained owner state
and differs from the live physical state while transparent windows are open. No
optimized core, native ALU execution, physical timing or speed pass is claimed.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from generate_processor_branch_reference import generate,ROOT,BUILD
from trace_r_front_transactions import layout
from test_external_r_packet import sha

PORTS=[('flags',9),('next_raw',4),('write_n',2),('latch_n',2),('hold_clocks',2),
       ('t_latch_n',2),('bypass',2),('parity_clocks',2),('parity_ok',4),('faults',4),
       ('r_read_n',2),('r_visible_n',2),('r_flags',4),('rm_flags_n',2),('address',32),
       ('select_n',4),('forwarded_bypass',2),('source_pins',202),('fa_n',8),('front_data',44),
       ('ready_flags',30),('control_flags',94),('cp_set_levels',18),('branch_pins',38),('branch_flags',4)]


def wrapper(partition, baseline=False):
    name='baseline_machine' if baseline else ('partition_machine' if partition else'monolithic_machine')
    module='r_front_control_reference' if baseline else ('processor_branch_partition' if partition else'processor_branch_reference')
    ports=PORTS[:-2] if baseline else PORTS
    width=sum(w for n,w in ports)+22
    bits=844 if baseline else 924
    clocks=100 if baseline else 104
    text=f'''module {name}(input wire load,input wire [923:0] fixture,
 input wire [9215:0] fixture_memory,input wire root,input wire [412:0] pins,
 output wire [{bits-1}:0] state_q,output wire [9215:0] memory_q,
 output wire [{clocks-1}:0] clocks,output wire [{width-1}:0] observations);
 wire [15:0] raw_data;wire [1:0] raw_parity,generated_parity,rwrite;
 wire [412:0] effective_pins={{pins[412:145],raw_parity,raw_data,pins[126:0]}};
'''
    for n,w in ports:text+=f' wire [{w-1}:0] {n};\n'
    pin_arg='effective_pins[400:0]' if baseline else 'effective_pins'
    text+=f' {module} original(.fixture_load(load),.fixture_state(fixture[{bits-1}:0]),.base(root),.pins({pin_arg}),.state_q(state_q),.clocks(clocks),\n'
    text+=','.join('.'+n+'('+n+')' for n,w in ports)+');\n'
    scope='original.old' if partition else'original'
    text+=f''' assign rwrite={{{scope}.l.j05.p13,{scope}.h.j05.p13}};
 r_file_ports_reference files(load,fixture_memory,1'b1,rwrite,address,select_n,
  state_q[555:540],raw_data,raw_parity,memory_q,generated_parity);
 assign observations={{'''+','.join(n for n,w in ports)+''',raw_data,raw_parity,generated_parity,rwrite};
endmodule
'''
    return text,width


def bench(count,trace,initial,raw_reference,clock_field,transfer=False):
    lo,hi=clock_field['low'],clock_field['high'];assert hi-lo+1==100
    _,width=wrapper(False)
    _,baseline_width=wrapper(False,True)
    text=f'''`timescale 1ns/1ps
module tb_composition;
 localparam integer COUNT={count};
 reg [13333:0] records[0:COUNT-1];reg [10059:0] initial_record[0:0];
 reg [1647:0] raw_records[0:COUNT-1];
 reg [923:0] fixture;reg [9215:0] fixture_memory;
 reg root=1,load=0,monitor=0;reg [412:0] pins=0;
 wire [923:0] a,b;wire [9215:0] am,bm;wire [103:0] ac,bc;
 wire [{width-1}:0] ao,bo;
 wire [843:0] old_q;wire [9215:0] old_memory;wire [99:0] old_clocks;
 wire [{baseline_width-1}:0] old_observations;
 baseline_machine baseline(load,fixture,fixture_memory,root,pins,old_q,old_memory,old_clocks,old_observations);
 monolithic_machine full(load,fixture,fixture_memory,root,pins,a,am,ac,ao);
 partition_machine divided(load,fixture,fixture_memory,root,pins,b,bm,bc,bo);
 integer counts_a[0:103],counts_b[0:103],previous_a[0:103],previous_b[0:103];
 reg [923:0] changed=0,held;reg [13333:0] record;reg [843:0] legacy;
 reg [99:0] expected_edges;integer checks=0;
 localparam [843:0] CLOSED_PRODUCERS=(844'b1<<91)|(844'b1<<92);
 for(genvar i=0;i<104;i++)begin:clock_counts
  initial begin counts_a[i]=0;counts_b[i]=0;end
  always @(posedge ac[i])if(monitor)counts_a[i]++;
  always @(posedge bc[i])if(monitor)counts_b[i]++;
 end
 initial begin
  $readmemh("{trace}",records);$readmemh("{initial}",initial_record);
  $readmemh("{raw_reference}",raw_records);
  fixture={{80'b0,initial_record[0][10059:9216]}};fixture_memory=initial_record[0][9215:0];
  for(integer i=0;i<16;i++)fixture[844+4*i+:4]=4'(i);
  pins[400:0]=raw_records[0][1246+:401];root=raw_records[0][1647];
  #100;load=1;#100;load=0;#100;load=1;#100;load=0;#100;
  if(a!==fixture || b!==fixture || am!==fixture_memory || bm!==fixture_memory || old_q!==fixture[843:0] || old_memory!==fixture_memory)
   $fatal(1,"COMPOSITION initial fixture mismatch");
  monitor=1;
  for(integer i=0;i<COUNT;i++)begin
   record=records[i];if(root!==record[12489])$fatal(1,"COMPOSITION trace root history broken");
   if(raw_records[i][1647]!==record[12489] || raw_records[i][1245]!==record[12488])
    $fatal(1,"COMPOSITION raw/reference root mismatch event=%0d",i);
   if(pins[400:0]!==raw_records[i][1246+:401])$fatal(1,"COMPOSITION raw pin history broken event=%0d",i);
   held=a;
   for(integer c=0;c<104;c++)begin previous_a[c]=counts_a[c];previous_b[c]=counts_b[c];end
   pins[400:0]=raw_records[i][844+:401];
   // Explicit ALU/TIOA boundaries, common to both circuits. The real ALU
   // datapath and its source-arrival contract remain the next integration.
   pins[412:401]=12'((i*37)^(i>>3));root=record[12488];#100;
   if(a!==b)begin
    for(integer bit_no=0;bit_no<924;bit_no++)if(a[bit_no]!==b[bit_no])
     $display("COMPOSITION_STATE_DIFF event=%0d bit=%0d full=%b divided=%b",i,bit_no,a[bit_no],b[bit_no]);
    $fatal(1,"COMPOSITION original circuits disagree event=%0d",i);
   end
   if(am!==bm || ao!==bo)$fatal(1,"COMPOSITION memory/observable mismatch event=%0d",i);
   // Original MC10173 latches follow D throughout their open window. Their
   // live physical outputs are not the retained held_next owner. This new
   // recording comes from the unchanged original full-program assertions.
   legacy=raw_records[i][843:0];
   if(old_q!==legacy)begin
    for(integer bit_no=0;bit_no<844;bit_no++)if(old_q[bit_no]!==legacy[bit_no])
     $display("COMPOSITION_BASELINE_DIFF event=%0d bit=%0d replay=%b legacy=%b",i,bit_no,old_q[bit_no],legacy[bit_no]);
    $fatal(1,"COMPOSITION unchanged original replay mismatch event=%0d",i);
   end
   if(old_memory!==record[9215:0])$fatal(1,"COMPOSITION original replay RAM mismatch event=%0d",i);
   if(|((a[843:0]^legacy)&~CLOSED_PRODUCERS))begin
    for(integer bit_no=0;bit_no<844;bit_no++)if(!CLOSED_PRODUCERS[bit_no] && a[bit_no]!==legacy[bit_no])
     $display("COMPOSITION_LEGACY_DIFF event=%0d bit=%0d full=%b legacy=%b",i,bit_no,a[bit_no],legacy[bit_no]);
    $fatal(1,"COMPOSITION unchanged legacy owner mismatch event=%0d",i);
   end
   if(am!==record[9215:0])$fatal(1,"COMPOSITION legacy RAM mismatch event=%0d",i);
   expected_edges=record[9216+{lo}+:100];
   for(integer c=0;c<104;c++)begin
    if(counts_a[c]-previous_a[c]!=counts_b[c]-previous_b[c])$fatal(1,"COMPOSITION original clock disagreement event=%0d channel=%0d",i,c);
    if(c<100 && counts_a[c]-previous_a[c]!=int'(expected_edges[c]))
     $fatal(1,"COMPOSITION legacy clock mismatch event=%0d channel=%0d",i,c);
   end
   changed|=a^held;checks++;
  end
  for(integer c=0;c<104;c++)$display("COMPOSITION_CLOCK channel=%0d edges=%0d",c,counts_a[c]);
  $display("PROCESSOR_BRANCH_COMPOSITION_PASS events=%0d states=924 RAM=9216 clocks=104 observations={width} legacy_fields=842 closed_producer_fields=2 new_changed=%h",checks,changed[923:844]);
  $finish;
 end
endmodule
'''
    if transfer:
        declarations='''
 reg [79:0] transfer_q;reg [37:0] transfer_before_pins;reg transfer_before_root;
 wire [79:0] transfer_next;wire [3:0] transfer_events;wire transfer_unsettled;
 dorado_branch_condition_step transfer_candidate(transfer_q,transfer_before_root,
  full.original.processor_roots[1],transfer_before_pins,full.branch_pins,
  transfer_next,transfer_events,transfer_unsettled);
'''
        text=text.replace(' integer counts_a[0:103]',declarations+' integer counts_a[0:103]',1)
        text=text.replace('  monitor=1;', '  transfer_q=fixture[923:844];\n  monitor=1;',1)
        text=text.replace('   held=a;', '''   transfer_before_pins=full.branch_pins;transfer_before_root=full.original.processor_roots[1];
   held=a;''',1)
        marker='   changed|=a^held;checks++;'
        assert text.count(marker)==1
        text=text.replace(marker,'''   if(transfer_unsettled)$fatal(1,"COMPOSITION branch transfer unsettled event=%0d",i);
   if(transfer_next!==a[923:844])begin
    for(integer bit_no=0;bit_no<80;bit_no++)if(transfer_next[bit_no]!==a[844+bit_no])
     $display("COMPOSITION_TRANSFER_DIFF event=%0d bit=%0d predicted=%b original=%b",i,bit_no,transfer_next[bit_no],a[844+bit_no]);
    $fatal(1,"COMPOSITION optimized branch transfer mismatch event=%0d",i);
   end
   for(integer c=0;c<4;c++)if(int'(transfer_events[c])!=counts_a[100+c]-previous_a[100+c])
    $fatal(1,"COMPOSITION branch transfer clock mismatch event=%0d channel=%0d",i,c);
   transfer_q=transfer_next;
'''+marker,1)
    return text


def run(out,engine,raw_reference,mutation=None,transfer=False,preclose_latch=False):
    assert not out.exists();out.mkdir(parents=True)
    manifest=generate(out/'reference');source=BUILD/'cp-original-program-balanced-verilator-v2-20261025'
    if mutation=='carry-feedback':
        timed=out/'reference/timed'
        raw=(timed/'r_front_control_proch.sv').read_text()
        raw=raw.replace('module r_front_control_proch','module bad_carry_proch',1)
        instance=re.findall(r' original_MC10176 g18\([^\n]+',raw)
        assert len(instance)==1 and instance[0].count('.p10(n0)')==1
        raw=raw.replace(instance[0],instance[0].replace('.p10(n0)','.p10(~n0)',1),1)
        (timed/'bad_carry_proch.sv').write_text(raw)
        raw=(timed/'r_front_control_reference.sv').read_text()
        raw=raw.replace('module r_front_control_reference','module bad_carry_reference',1)
        assert raw.count('r_front_control_proch h(')==1
        raw=raw.replace('r_front_control_proch h(','bad_carry_proch h(',1)
        (timed/'bad_carry_reference.sv').write_text(raw)
        path=timed/'processor_branch_partition.sv';raw=path.read_text()
        assert raw.count('r_front_control_reference old(')==1
        path.write_text(raw.replace('r_front_control_reference old(','bad_carry_reference old(',1))
    recording=json.loads((raw_reference/'run-state.json').read_text())
    assert recording['status']=='PASS_PASSIVE_RAW_INPUT_RECORDING'
    assert recording['source']==str(source) and recording['source_state_sha256']==sha(source/'run-state.json')
    assert recording['actions'][-1]['returncode']==0
    for name in ['transactions.hex','initial.hex']:
        assert sha(raw_reference/name)==sha(source/name),name
    for name,h in recording['files'].items():assert sha(raw_reference/name)==h,name
    for name in ['transactions.hex','initial.hex']:shutil.copy2(source/name,out/name)
    shutil.copy2(raw_reference/'raw-inputs.hex',out/'raw-inputs.hex')
    lines=(out/'transactions.hex').read_text().splitlines();assert len(lines)==14884
    _,fields=layout((source/'candidate.sv').read_text())
    a,width=wrapper(False);b,_=wrapper(True);c,_=wrapper(False,True);(out/'machines.sv').write_text(a+b+c)
    assert len((out/'raw-inputs.hex').read_text().splitlines())==len(lines)
    (out/'tb.sv').write_text(bench(len(lines),out/'transactions.hex',out/'initial.hex',out/'raw-inputs.hex',fields['clock_events'],transfer))
    for name in [Path(__file__).name,'generate_processor_branch_reference.py']:
        shutil.copy2(Path(__file__).with_name(name),out/name)
    paths=[*sorted((out/'reference/timed').glob('*')),out/'machines.sv',out/'tb.sv']
    if transfer:
        shutil.copy2(ROOT/'verilog/rtl/dorado_branch_condition_step.sv',out/'branch-step.sv')
        if preclose_latch:
            p=out/'branch-step.sv';raw=p.read_text();marker='   if(wave==0)begin\n'
            assert raw.count(marker)==1
            p.write_text(raw.replace(marker,marker+'''    // Capture new D during an already-open latch window before root closure.
    if(!before_clocks[2])n[67:64]=pre_data[15:12];
''',1))
        paths+=[out/'reference/branch/cone.sv',out/'branch-step.sv']
    paths=[p for p in paths if p.suffix in ['.sv','.v']]
    state=dict(status='RUNNING',scope=__doc__,engine=engine,mutation=mutation,optimized_branch_transfer_checked=transfer,
               experimental_preclose_latch_capture=preclose_latch,started=time.time(),reference=manifest,
               original_trace_sha256=sha(source/'transactions.hex'),original_initial_sha256=sha(source/'initial.hex'),
               raw_reference=str(raw_reference),raw_reference_state_sha256=sha(raw_reference/'run-state.json'),
               physical_state_oracle='Original circuit post-event outputs, including transparent latches; not held_next retained owner',
               original_input_events=14884,actions=[],files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    commands=([['iverilog','-g2012','-s','tb_composition','-o',str(out/'test.vvp'),*map(str,paths)],['vvp','-i',str(out/'test.vvp')]]
      if engine=='icarus' else [['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_composition','--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    save();code=1
    for label,command in zip(['compile','test'],commands):
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT)
            row=dict(label=label,pid=child.pid,command=command,started=time.time());state['actions'].append(row);save();code=child.wait()
        row.update(returncode=code,log_sha256=sha(out/(label+'.log')),finished=time.time());save()
        if code:break
    log=(out/'test.log').read_text() if (out/'test.log').exists() else''
    passed=code==0 and 'PROCESSOR_BRANCH_COMPOSITION_PASS' in log
    state.update(status='PASS_ORIGINAL_PROCESSOR_BRANCH_COMPOSITION' if passed else'TERMINAL_PROCESSOR_BRANCH_COMPOSITION_NOT_ESTABLISHED',
                 returncode=code,finished=time.time(),outcomes=re.findall(r'^(?:COMPOSITION_CLOCK|PROCESSOR_BRANCH_COMPOSITION_PASS) .*$',log,re.M))
    save();print(state['status']);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True)
    p.add_argument('--raw-reference',type=Path,required=True)
    p.add_argument('--mutation',choices=['carry-feedback'])
    p.add_argument('--transfer',action='store_true',help='Also compare the optimized80-bit transfer using actual original processor/control producers')
    p.add_argument('--preclose-latch',action='store_true',help='Diagnostic open-window D capture before root closure; does not change production RTL')
    a=p.parse_args();assert not a.preclose_latch or a.transfer
    raise SystemExit(run(a.output.resolve(),a.engine,a.raw_reference.resolve(),a.mutation,a.transfer,a.preclose_latch))
