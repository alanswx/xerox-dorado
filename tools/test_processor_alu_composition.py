#!/usr/bin/env python3
"""Compare complete original ALU/processor storage against a split flag owner.

The original401-input producer program remains byte-exact. Added native
operand/control boundaries receive reproducible independent values. Every1064
physical state bit,9216 SRAM bits,108 clock counts and964 observations compares.
Optional optimized flag capture is an additional check, never a replacement
for those comparisons. Operand registers outside this cone remain boundaries.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from generate_processor_alu_reference import generate,ROOT,BUILD
from test_processor_branch_composition import PORTS
from test_external_r_packet import sha
from generate_processor_alu_projection import generate as projection_generate

ALL_PORTS=PORTS+[('alu_word',16),('alu_function',6),('legacy_pins',401)]


def wrapper(split,pins):
    name='split_machine'if split else'full_machine'
    core='processor_alu_partition'if split else'processor_alu_reference'
    width=sum(w for n,w in ALL_PORTS)+22
    raw=f'''module {name}(input wire load,input wire [1063:0] fixture,
 input wire [9215:0] fixture_memory,input wire root,input wire [{pins-1}:0] pins,
 output wire [1063:0] q,output wire [9215:0] memory_q,
 output wire [107:0] clocks,output wire [{width-1}:0] observations);
 wire [15:0] raw_data;wire [1:0] raw_parity,generated_parity,rwrite;
 wire [{pins-1}:0] effective_pins={{pins[{pins-1}:145],raw_parity,raw_data,pins[126:0]}};
'''
    for n,w in ALL_PORTS:raw+=f' wire [{w-1}:0] {n};\n'
    raw+=f' {core} original(.fixture_load(load),.fixture_state(fixture),.base(root),.pins(effective_pins),.state_q(q),.clocks(clocks),\n'
    raw+=','.join('.'+n+'('+n+')'for n,w in ALL_PORTS)+');\n'
    raw+=''' assign rwrite={original.l.j05.p13,original.h.j05.p13};
 r_file_ports_reference files(load,fixture_memory,1'b1,rwrite,address,select_n,
  q[555:540],raw_data,raw_parity,memory_q,generated_parity);
 assign observations={'''+','.join(n for n,w in ALL_PORTS)+''',raw_data,raw_parity,generated_parity,rwrite};
endmodule
'''
    return raw,width


def bench(count,pins,width,out,branch_pins,transfer,trace=False,projection=False,write_suffix=False,alu_pins=None,trace_range=(14,17)):
    raw=f'''`timescale 1ns/1ps
module tb_alu_composition;
 reg [1647:0] records[0:{count-1}];reg [10059:0] initial_record[0:0];
 reg [3:0] selectors[0:{count-1}];
 reg [1063:0] fixture;reg [9215:0] fixture_memory;reg load=0,root=1,monitor=0;
 reg [{pins-1}:0] pins=0;wire [1063:0] a,b;wire [9215:0] am,bm;
 wire [107:0] ac,bc;wire [{width-1}:0] ao,bo;
 full_machine full(load,fixture,fixture_memory,root,pins,a,am,ac,ao);
 split_machine divided(load,fixture,fixture_memory,root,pins,b,bm,bc,bo);
 integer counts_a[0:107],counts_b[0:107],old_a[0:107],old_b[0:107];
 reg [31:0] rng=32'hd04a1810;reg [1063:0] changed=0,held;
 reg [63:0] functions_seen=0;reg [65535:0] results_seen=0;integer checks=0;
 reg [15:0] written_addresses=0;reg function_write_open=0;
 reg [3:0] function_write_address;reg [7:0] function_write_data;
 integer function_address_changes=0,function_data_changes=0;
 wire [3:0] function_address={{full.original.l.e13.p10,full.original.l.e13.p9,full.original.l.e13.p7,full.original.l.e13.p6}};
 wire [7:0] function_data={{full.original.l.e14.p12,full.original.l.e14.p11,full.original.l.e14.p4,full.original.l.e14.p5,
                         full.original.l.e13.p12,full.original.l.e13.p11,full.original.l.e13.p4,full.original.l.e13.p5}};
 always @(negedge full.original.l.e13.p13)if(monitor)begin
  written_addresses[function_address]=1;function_write_open=1;
  function_write_address=function_address;function_write_data=function_data;
 end
 always @(posedge full.original.l.e13.p13)function_write_open=0;
 always @(function_address or function_data)if(monitor && function_write_open && !full.original.l.e13.p13)begin
  if(function_address!==function_write_address)function_address_changes++;
  if(function_data!==function_write_data)function_data_changes++;
 end
 for(genvar c=0;c<108;c++)begin:count
  initial begin counts_a[c]=0;counts_b[c]=0;end
  always @(posedge ac[c])if(monitor)counts_a[c]++;
  always @(posedge bc[c])if(monitor)counts_b[c]++;
 end
 initial begin
  $readmemh("{out/'raw-inputs.hex'}",records);$readmemh("{out/'initial.hex'}",initial_record);
  $readmemh("{out/'selectors.hex'}",selectors);
  fixture={{220'b0,initial_record[0][10059:9216]}};fixture_memory=initial_record[0][9215:0];
  for(integer i=0;i<16;i++)begin
   fixture[844+4*i+:4]=4'(i);
   fixture[930+4*i+:4]=4'(i*3+1);fixture[994+4*i+:4]=4'(i*5+2);
  end
  pins[400:0]=records[0][1246+:401];root=records[0][1647];
  #100;load=1;#100;load=0;#100;load=1;#100;load=0;#100;
  if(a!==fixture || b!==fixture || am!==fixture_memory || bm!==fixture_memory)
   $fatal(1,"ALU_COMPOSITION initial fixture mismatch full=%h expected=%h",a,fixture);
  monitor=1;
  for(integer i=0;i<{count};i++)begin
   if(root!==records[i][1647] || pins[400:0]!==records[i][1246+:401])
    $fatal(1,"ALU_COMPOSITION broken original input history event=%0d",i);
   held=a;
   for(integer c=0;c<108;c++)begin old_a[c]=counts_a[c];old_b[c]=counts_b[c];end
   pins[400:0]=records[i][844+:401];
   for(integer k=401;k<{pins};k++)begin
    rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;pins[k]=rng[0];
   end
   root=records[i][1245];#100;
   if(a!==b)begin
    for(integer k=0;k<1064;k++)if(a[k]!==b[k])
     $display("ALU_COMPOSITION_STATE_DIFF event=%0d bit=%0d full=%b split=%b",i,k,a[k],b[k]);
    $fatal(1,"ALU_COMPOSITION original states disagree event=%0d",i);
   end
   if(am!==bm)$fatal(1,"ALU_COMPOSITION original SRAM mismatch event=%0d",i);
   if(ao!==bo)$fatal(1,"ALU_COMPOSITION observable mismatch event=%0d",i);
   for(integer c=0;c<108;c++)if(counts_a[c]-old_a[c]!=counts_b[c]-old_b[c])
    $fatal(1,"ALU_COMPOSITION clock mismatch event=%0d channel=%0d",i,c);
   changed|=a^held;functions_seen[full.alu_function]=1;results_seen[full.alu_word]=1;checks++;
   if(checks%2048==0)$display("ALU_COMPOSITION_PROGRESS events=%0d",checks);
  end
  for(integer c=0;c<108;c++)$display("ALU_COMPOSITION_CLOCK channel=%0d edges=%0d",c,counts_a[c]);
  $display("ALU_FUNCTION_WRITE_COVERAGE addresses=%h address_changes=%0d data_changes=%0d",written_addresses,function_address_changes,function_data_changes);
  $display("ORIGINAL_ALU_COMPOSITION_PASS events=%0d states=1064 RAM=9216 clocks=108 observations={width} functions=%h results=%0d new_changed=%h",checks,functions_seen,$countones(results_seen),changed[1063:924]);
  $finish;
 end
endmodule
'''
    if transfer:
        mask=sum(1<<branch_pins[f'TIOA.{i}']for i in range(8))
        declarations=f''' reg [79:0] tq;reg [37:0] before_pins;reg before_root;
 wire [79:0] next_q;wire [3:0] events;wire unsettled;
 wire [37:0] early_pins=(before_pins & ~38'h{mask:x}) | (full.branch_pins & 38'h{mask:x});
 dorado_branch_condition_frame_step candidate(tq,before_root,full.original.processor_roots[1],
  before_pins,early_pins,full.branch_pins,next_q,events,unsettled);
'''
        raw=raw.replace(' integer counts_a[0:107]',declarations+' integer counts_a[0:107]',1)
        raw=raw.replace('  monitor=1;','  tq=fixture[923:844];monitor=1;',1)
        raw=raw.replace('   held=a;','   before_pins=full.branch_pins;before_root=full.original.processor_roots[1];held=a;',1)
        marker='   changed|=a^held;'
        raw=raw.replace(marker,'''   if(next_q!==a[923:844])begin
    for(integer k=0;k<80;k++)if(next_q[k]!==a[844+k])
     $display("ALU_TRANSFER_DIFF event=%0d bit=%0d predicted=%b original=%b",i,k,next_q[k],a[844+k]);
    $fatal(1,"ALU_COMPOSITION optimized flag transfer mismatch event=%0d",i);
   end
   if(unsettled)$fatal(1,"ALU_COMPOSITION flag transfer unsettled event=%0d",i);
   for(integer c=0;c<4;c++)if(int'(events[c])!=counts_a[100+c]-old_a[100+c])
    $fatal(1,"ALU_COMPOSITION flag event mismatch event=%0d channel=%0d",i,c);
   tq=next_q;
'''+marker,1)
        if projection:
            declaration=f''' reg [1063:0] projection_q;reg projection_root;
 wire [37:0] projected_pins;wire [15:0] projected_word;wire [5:0] projected_function;
 alu_projection_processor_alu_reference early_view(.fixture_load(1'b0),.fixture_state(projection_q),
  .base(projection_root),.pins(pins),.branch_pins(projected_pins),.alu_word(projected_word),.alu_function(projected_function));
'''
            raw=raw.replace(' reg [79:0] tq;',declaration+' reg [79:0] tq;',1)
            marker='before_pins,early_pins,full.branch_pins,next_q,events,unsettled);'
            assert raw.count(marker)==1;raw=raw.replace(marker,'before_pins,projected_pins,full.branch_pins,next_q,events,unsettled);',1)
            marker='   before_pins=full.branch_pins;'
            raw=raw.replace(marker,'   projection_q=a;projection_root=root;\n'+marker,1)
    if trace:
        extra='''
 always @(full.original.h.h11.p9 or full.original.h.h11.p12 or
          full.original.h.g11.p9 or full.original.h.g11.p11 or
          full.original.h.f17.p7 or full.original.l.e12.p9 or full.alu_function or
          full.alu_word or full.branch_pins or a[923:908])
  if(monitor && checks>=14 && checks<=17)
   $display("ALU_FLAG_ARRIVAL t=%0.3f event=%0d root=%b f17_C=%b h11_C=%b h11_D5=%b g11_C=%b g11_D4=%b function_C=%b function=%h ALU=%h flags=%h h11_Q=%h boundary=%h",$realtime,checks,root,
    full.original.h.f17.p7,full.original.h.h11.p9,full.original.h.h11.p12,
    full.original.h.g11.p9,full.original.h.g11.p11,full.original.l.e12.p9,
    full.alu_function,full.alu_word,a[911:908],a[923:918],full.branch_pins);
'''
        extra=extra.replace('checks>=14 && checks<=17',f'checks>={trace_range[0]} && checks<={trace_range[1]}')
        raw=raw.replace('endmodule',extra+'endmodule',1)
    if write_suffix:
        marker='   root=records[i][1245];#100;'
        overrides='''   if(i>=14884)begin
    for(integer k=401;k<PIN_COUNT;k++)pins[k]=0;
'''.replace('PIN_COUNT',str(pins))
        for bit,pin in enumerate(alu_pins):overrides+=f'    pins[{pin}]=selectors[i][{3-bit}];\n'
        overrides+='   end\n'
        raw=raw.replace(marker,overrides+marker,1)
        marker='  for(integer c=0;c<108;c++)$display'
        coverage='''  if(counts_a[105]==0 || counts_a[106]==0 || written_addresses!=16'hffff)$fatal(1,"ALU_COMPOSITION incomplete function RAM write windows addresses=%h",written_addresses);
'''
        raw=raw.replace(marker,coverage+marker,1)
    return raw


def with_write_suffix(lines):
    values=[int(s,16)for s in lines];selectors=[0]*len(lines)
    root=(values[-1]>>1245)&1;value=(values[-1]>>844)&((1<<401)-1);selector=0
    def event(next_root,new_value):
        nonlocal root,value
        for bit,pin in enumerate([363,364,365,366]):
            new_value=(new_value&~(1<<pin))|(((selector>>(3-bit))&1)<<pin)
        values.append((root<<1647)|(value<<1246)|(next_root<<1245)|(new_value<<844))
        selectors.append(selector);root=next_root;value=new_value
    def field(v,lo,width,data):return(v&~(((1<<width)-1)<<lo))|((data&((1<<width)-1))<<lo)
    original=BUILD/'cp-original-program-balanced-verilator-v2-20261025/inputs/tb_r_front_local.sv'
    expression=re.search(r'localparam \[400:0\] SEED=(.*?);',original.read_text(),re.S)[1]
    expression=re.sub(r"\d+'([hodb])([0-9a-fA-F]+)",lambda m:str(int(m[2],dict(h=16,o=8,d=10,b=2)[m[1]])),expression)
    assert re.fullmatch(r'[0-9\s()<>|&^+*]+',expression)
    seed=eval(''.join(expression.split()),{'__builtins__':{}},{})
    assert seed==(values[0]>>1246)&((1<<401)-1)
    p=field(seed,354,6,0b110001);p=field(p,360,3,0);p=field(p,0,9,0x178)
    for bit,data in [(293,0),(294,0),(205,1),(254,1)]:p=field(p,bit,1,data)
    p=field(p,202,3,0b110);event(1,p)
    def cp(bank,data):
        nonlocal p
        if not root:event(1,p)
        p=field(field(field(p,292,1,1),289,3,~bank),282,7,data);event(1,p)
        p=field(p,292,1,0);event(1,p);p=field(p,292,1,1);event(1,p)
    for bank,data in [(0,40),(1,64),(0,8),(1,0),(0,0)]:cp(bank,data)
    for unused in range(32):event(1-root,p)
    def ff(code):
        nonlocal p
        reversed_bits=int(f'{code:08b}'[::-1],2);p=field(p,246,8,reversed_bits);event(1,p)
    ff(0x62)  # Same normal TaskingOff setup as the original full program.
    for unused in range(32):event(1-root,p)
    for address in range(16):
        ff(0)
        for unused in range(8):event(1-root,p)
        selector=address;event(1,p)
        for unused in range(4):event(1-root,p)
        ff(0o262)  # Original ALUFMRW decode: FA=2,FB=6,FC=2.
        for unused in range(16):event(1-root,p)
    ff(0)
    for unused in range(8):event(1-root,p)
    return [f'{v:0412x}'for v in values],selectors


def run(out,engine,transfer,trace=False,projection=False,write_suffix=False,trace_range=(14,17),mutation=None):
    assert not out.exists();out.mkdir(parents=True);m=generate(out/'reference')
    if mutation=='mode-data':
        timed=out/'reference/timed';path=timed/'processor_alu_procl.sv';raw=path.read_text()
        instance=re.findall(r' original_MC10176 e12\([^\n]+',raw);assert len(instance)==1
        pin=re.search(r'\.p12\((\w+)\)',instance[0])[1]
        bad=instance[0].replace('.p12('+pin+')','.p12(~'+pin+')',1)
        raw=raw.replace(instance[0],bad,1).replace('module processor_alu_procl','module processor_alu_procl_badmode',1)
        (timed/'processor_alu_procl_badmode.sv').write_text(raw)
        path=timed/'processor_alu_partition.sv';raw=path.read_text()
        assert raw.count('processor_alu_procl l(')==1
        path.write_text(raw.replace('processor_alu_procl l(','processor_alu_procl_badmode l(',1))
    source=BUILD/'processor-raw-input-recording-v1-20261025';s=json.loads((source/'run-state.json').read_text())
    assert s['status']=='PASS_PASSIVE_RAW_INPUT_RECORDING'
    for n in ['raw-inputs.hex','initial.hex']:shutil.copy2(source/n,out/n)
    records=(out/'raw-inputs.hex').read_text().splitlines();assert len(records)==14884
    selectors=[0]*len(records)
    if write_suffix:
        records,selectors=with_write_suffix(records);(out/'raw-inputs.hex').write_text('\n'.join(records)+'\n')
    (out/'selectors.hex').write_text('\n'.join(f'{v:x}'for v in selectors)+'\n')
    a,width=wrapper(False,m['boundary_pins']);b,_=wrapper(True,m['boundary_pins'])
    (out/'machines.sv').write_text(a+b)
    branch_pins=json.loads((out/'reference/base/branch/manifest.json').read_text())['pin_index']
    alu_pins=m.get('actual_instruction_ALUF_pins')or[m['extra_boundary_pins'][f'ALUF.{i}']for i in range(4)]
    (out/'tb.sv').write_text(bench(len(records),m['boundary_pins'],width,out,branch_pins,transfer,trace,projection,write_suffix,alu_pins,trace_range))
    paths=sorted((out/'reference/timed').iterdir())+[out/'machines.sv',out/'tb.sv']
    if transfer:
        shutil.copy2(ROOT/'verilog/rtl/dorado_branch_condition_frame_step.sv',out/'branch-step.sv')
        paths += [out/'reference/base/branch/cone.sv',out/'branch-step.sv']
        paths=[p for p in paths if p.name!='processor_alu_flag_views.sv']
    if projection:
        projection_generate(out/'reference',out/'projection');paths.append(out/'projection/projection.sv')
    paths=[p for p in paths if p.suffix in ['.sv','.v']]
    for n in [Path(__file__).name,'generate_processor_alu_reference.py','generate_processor_alu_projection.py']:shutil.copy2(ROOT/'tools'/n,out/n)
    state=dict(status='RUNNING',scope=__doc__,engine=engine,optimized_flag_transfer_checked=transfer,started=time.time(),
        mutation=mutation,
        passive_ALU_arrival_trace=trace,
        trace_event_range=trace_range,
        explicit_pre_root_ALU_projection=projection,
        normal_MIR_ALUF_write_suffix=write_suffix,original_prefix_records=14884,records=len(records),
        reference=m,original_stimulus_sha256=sha(source/'raw-inputs.hex'),actions=[],
        files={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    commands=([['iverilog','-g2012','-s','tb_alu_composition','-o',str(out/'test.vvp'),*map(str,paths)],['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else
        [['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_alu_composition','--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    save();code=1
    for label,cmd in zip(['compile','test'],commands):
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log);row=dict(label=label,command=cmd,pid=child.pid,started=time.time());state['actions'].append(row);save();code=child.wait()
        row.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    passed=code==0 and 'ORIGINAL_ALU_COMPOSITION_PASS'in log
    state.update(status='PASS_ORIGINAL_ALU_PROCESSOR_COMPOSITION'if passed else'TERMINAL_ALU_PROCESSOR_COMPOSITION_NOT_ESTABLISHED',
        returncode=code,finished=time.time(),outcomes=re.findall(r'^(?:ALU_COMPOSITION_CLOCK|ORIGINAL_ALU_COMPOSITION_PASS) .*$',log,re.M))
    save();print(state['status']);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['verilator','icarus'],required=True);p.add_argument('--transfer',action='store_true')
    p.add_argument('--trace-arrivals',action='store_true')
    p.add_argument('--projection',action='store_true')
    p.add_argument('--write-suffix',action='store_true')
    p.add_argument('--trace-events',default='14:17')
    p.add_argument('--mutation',choices=['mode-data'])
    a=p.parse_args();assert not a.projection or a.transfer
    trace_range=tuple(map(int,a.trace_events.split(':')));assert len(trace_range)==2 and 0<=trace_range[0]<=trace_range[1]
    raise SystemExit(run(a.output.resolve(),a.engine,a.transfer,a.trace_arrivals,a.projection,a.write_suffix,trace_range,a.mutation))
