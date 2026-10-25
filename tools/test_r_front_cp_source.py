#!/usr/bin/env python3
"""Test source-closed CP arrivals against the original full processor graph.

Retain the frozen original844-Q/9216-RAM/100-clock checker, all physical R
window/capture tests, pure/cached comparisons and commit/hold assertions.
Replace only its stimulus program; normalize the real13-wire CP producer at
the DUT boundaries and close the original decoders in the reference. Ordinary
memory MIR data stays independent. This is not native timing or boot proof.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from generate_cp_interior_read import rewrite as interior_rewrite,stage_bench,sequential_stage_bench,ram_sweep_bench
from generate_cp_block_capture import rewrite,route
from generate_cp_prblock_capture import rewrite as prblock_rewrite,route as prblock_route
from generate_r_front_cp_source_reference import generate
import generate_shared_control_capture as shared
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
PROFILES={'balanced':(1000,1000,1000),'later_control':(1750,400,1250)}


def inherited_timing(trace):
    gates=set()
    for p in (trace/'timed').glob('cell_*.v'):
        gates.update(map(int,re.findall(r'assign\s+#\((\d+)\)',p.read_text())))
    raw=(trace/'timed/run_control_original_ff.sv').read_text()
    phase=module(raw,'original_SE10231');ordinary=raw.replace(phase,'')
    ff=set(map(int,re.findall(r'<=#\((\d+)\)',ordinary)))
    ph=set(map(int,re.findall(r'<=#\((\d+)\)',phase)))
    assert gates==ff==ph=={1000},(gates,ff,ph)
    return (1000,1000,1000)


def timed_source(name,text,profile):
    gate,ff,phase=PROFILES[profile]
    if name.startswith('timed/cell_'):
        assert set(map(int,re.findall(r'assign\s+#\((\d+)\)',text)))=={1000}
        return re.sub(r'(assign\s+#\()1000(\))',r'\g<1>'+str(gate)+r'\2',text)
    if name=='timed/run_control_original_ff.sv':
        part=module(text,'original_SE10231');rest=text.replace(part,'PHASE_FF_SENTINEL',1)
        rest=rest.replace('<=#(1000)','<=#('+str(ff)+')')
        part=part.replace('<=#(1000)','<=#('+str(phase)+')')
        return rest.replace('PHASE_FF_SENTINEL',part,1)
    if name=='timed/processor_capture_original_ff.sv':return text.replace('<=#(1000)','<=#('+str(ff)+')')
    if name.startswith('timed/') and 'parameter integer DELAY_PS=1000' in text:
        return text.replace('parameter integer DELAY_PS=1000','parameter integer DELAY_PS='+str(ff))
    return text


def source_top(raw,fn):
    for name in ['dorado_r_front_control_step','dorado_r_front_pure_step']:
        old=module(raw,name);header,body=old.split('\n);',1)
        # Retain the public port names and every named child-port label.
        for before,after in [('pins_before','source_before'),('pins_after','source_after')]:
            body=re.sub(r'(?<![.\w])'+before+r'\b',after,body)
        new=header+'\n);'+body
        marker=' // These32 bits retain the CLOSED input latch data.';assert new.count(marker)==1
        view=fn+''' wire [400:0] source_before=cp_source_pins(pins_before,cp_bus(pins_before));
 wire [400:0] source_after=cp_source_pins(pins_after,cp_bus(pins_after));
'''
        new=new.replace(marker,view+marker,1)
        restored=new.replace(view,'',1)
        header,body=restored.split('\n);',1)
        for before,after in [('source_before','pins_before'),('source_after','pins_after')]:
            body=re.sub(r'\b'+before+r'\b',after,body)
        assert header+'\n);'+body==old
        raw=raw.replace(old,new,1)
    return raw


def bench(raw,out,repro,collisions=False,ready_zero=False,trace_arrivals=False,no_cp_pulse=False,arrival=0,equal_bank_data=False,matched_interior_data=False):
    # The frozen bench was compiled with Verilator. Keep the same nets and
    # connections, declaring them before their first use for Icarus as well.
    declaration=' wire [17:0] external_file_read;wire [15:0] file_write_data;'
    assert raw.count(declaration)==1
    raw=raw.replace(declaration+'\n','',1)
    marker=' wire [843:0] pure_held_next;';assert raw.count(marker)==1
    raw=raw.replace(marker,declaration+'\n'+marker,1)
    for old,new in [("pb,pa,1'b0,n,events",f"pb,pa,1'b{arrival},n,events"),
                    (".refresh_before_run_capture(1'b0)",f".refresh_before_run_capture(1'b{arrival})")]:
        assert raw.count(old)==1;raw=raw.replace(old,new,1)
    if ready_zero:
        marker="fixture[751:736]=16'hffff;";assert raw.count(marker)==1
        raw=raw.replace(marker,"fixture[751:736]=16'h0000;",1)
    if equal_bank_data:
        marker="     files.rm_h[i]=word0;files.stk_h[i]=word1;"
        assert raw.count(marker)==1
        raw=raw.replace(marker,"     files.rm_h[i]=word0;files.stk_h[i]=word0;",1)
        marker="     files.rm_l[i]=word2;files.stk_l[i]=word3;"
        assert raw.count(marker)==1
        raw=raw.replace(marker,"     files.rm_l[i]=word2;files.stk_l[i]=word2;",1)
    if matched_interior_data:
        marker="   fixture_memory=memory_image();"
        assert raw.count(marker)==1
        raw=raw.replace(marker,"   files.stk_h[0]=files.rm_h[255];files.stk_l[0]=files.rm_l[15];\n"+marker,1)
    marker=' wire unsettled,shared_mismatch;';assert raw.count(marker)==1
    raw=raw.replace(marker,' wire [17:0] ref_cp_sets;reg [17:0] observed_cp_sets=0;\n'+marker,1)
    marker='ref_fa_n,,ref_ready_flags,ref_control_flags);';assert raw.count(marker)==1
    raw=raw.replace(marker,'ref_fa_n,,ref_ready_flags,ref_control_flags,ref_cp_sets);',1)
    marker='     if(ref_rwrite!==erw)';assert raw.count(marker)==1
    raw=raw.replace(marker,'''     observed_cp_sets|=ref_cp_sets;
     if(ref_cp_sets!==dut.source_after[281:264])$fatal(1,"FULL_CP decoder level mismatch check=%0d",checks);
'''+marker,1)
    marker='trace_owner=stored_q;trace_before=cb;trace_after=ca;trace_pb=pb;trace_pa=pa;'
    assert raw.count(marker)==1
    raw=raw.replace(marker,'trace_owner=stored_q;trace_before=cb;trace_after=ca;trace_pb=dut.source_before;trace_pa=dut.source_after;',1)
    # Retain passive tracing, redirecting both files away from the protected
    # qualified original trace. No historical artifact is reopened for writing.
    paths=re.findall(r'\$fopen\("([^\"]+)"',raw);assert len(paths)==2
    for name in paths:
        raw=raw.replace('$fopen("'+name+'"','$fopen("'+str(out/Path(name).name)+'"',1)
    if trace_arrivals:
        marker='     if({rwn,rln,rtln,rbp,rrr,rrv}!=={eo[19:12],eo[3:0]} || rrf!==rf || rrf_n!==rf_n)'
        assert raw.count(marker)==1
        raw=raw.replace(marker,'''     if(checks>=193 && checks<=195)$display("FULL_CP_OPERAND expected=%h original=%h R_flags=%h/%h RM_flags=%h/%h",eo,{rwn,rln,rtln,rbp,rok,rfault,rrr,rrv},rf,rrf,rf_n,rrf_n);
'''+marker,1)
        marker=' task automatic event_step(';assert raw.count(marker)==1
        raw=raw.replace(marker,''' always @(ref_raw_data or raddress or rselect or original.c.g16.state_q or original.l.l10.p14 or original.l.i15.state_q or original.l.h14.state_q)
  if(monitor && checks==194)$display("FULL_CP_SRAM_ARRIVAL t=%0.3f address=%h select=%h data=%h PrBlock=%b StkSela=%b i15=%h h14=%h candidate_address=%h select=%h data=%h",$realtime,raddress,rselect,ref_raw_data,original.c.g16.state_q[1],original.l.l10.p14,original.l.i15.state_q,original.l.h14.state_q,port_address,port_select,raw_data);
 always @(original.h.l07.p7 or original.h.l07.p5 or original.h.l07.p6 or original.h.l07.p9 or original.h.l07.p12 or original.h.l07.p11 or original.h.l07.state_q or original.h.l06.p7 or original.h.l06.p9 or original.h.l06.p5 or original.h.l06.p12 or original.h.l06.state_q or rforwarded or rrr)
  if(monitor && checks==194)$display("FULL_CP_RFLAG_ARRIVAL t=%0.3f read_n=%b forward=%b l07_C=%b%b S=%b%b D=%b%b Q=%b l06_C=%b%b S=%b%b D=%b%b Q=%b",$realtime,rrr[0],rforwarded[0],original.h.l07.p9,original.h.l07.p7,original.h.l07.p12,original.h.l07.p5,original.h.l07.p11,original.h.l07.p6,original.h.l07.state_q,original.h.l06.p9,original.h.l06.p7,original.h.l06.p12,original.h.l06.p5,original.h.l06.p11,original.h.l06.p6,original.h.l06.state_q);
 always @(original.l.g16.clk or original.l.g16.p9 or original.l.g16.state_q or original.l.h14.state_q or original.c.f15.state_q)
  if(monitor && checks>=193 && checks<=195)
   $display("FULL_CP_G16_ARRIVAL t=%0.3f check=%0d root=%b sBLOCK=%b C=%b D9=%b Q2=%b h14=%h predicted_aux=%b pre_aux=%b low_aux=%b Block=%b TPC=%b",$realtime,checks,root,ref_cp_sets[17],original.l.g16.clk,original.l.g16.p9,original.l.g16.state_q[1],original.l.h14.state_q,n[537],dut.child.child.core_pre[146],dut.child.child.low_source[146],reference[781],reference[74]);
'''+marker,1)
    start=raw.index(' initial begin\n   if($value$plusargs("iterations=%d",iterations))')
    end=raw.index('\n end\n',start)+len('\n end')
    initial=raw[start:end]
    stop=initial.index('   for(integer i=0;i<64;i=i+1) event_step(!cb,p);')
    setup=initial[:stop]+'''   for(integer i=0;i<64;i=i+1)event_step(!cb,p);
   p[6]=1;event_step(1,p);repeat(64)begin event_step(1,p);event_step(0,p);end
'''
    if collisions:
        helper=''' reg [843:0] seed_fixture;integer fixture_index=0;
 task automatic reload_fixture;
  begin
   monitor=0;advance=0;cp_reset=1;#100;host_clk=1;#100;host_clk=0;
   root=1;cb=1;ca=1;rp=SEED;pb=SEED;pa=SEED;
   for(integer i=0;i<256;i++)begin
    files.rm_h[i]={fixture_memory[4096+i],fixture_memory[1024+4*i+:4],fixture_memory[4*i+:4]};
    files.stk_h[i]={fixture_memory[4352+i],fixture_memory[3072+4*i+:4],fixture_memory[2048+4*i+:4]};
    files.rm_l[i]={fixture_memory[8704+i],fixture_memory[5632+4*i+:4],fixture_memory[4608+4*i+:4]};
    files.stk_l[i]={fixture_memory[8960+i],fixture_memory[7680+4*i+:4],fixture_memory[6656+4*i+:4]};
   end
   fixture=0;#100;load=1;#100;load=0;#100;
   fixture=seed_fixture;#100;load=1;#100;load=0;#100;load=1;#100;load=0;#100;
   if(reference!==fixture)$fatal(1,"FULL_CP reload fixture mismatch index=%0d",fixture_index);
   stored_q=fixture;cp_reset=0;advance=1;monitor=1;p=SEED;p[19:0]=20'h178;fixture_index++;
   if(memory_image()!==fixture_memory)$fatal(1,"FULL_CP reload RAM mismatch");
   $display("FULL_CP_FIXTURE_BOUNDARY fixture=%0d record=%0d owner=%h memory=%h%h",fixture_index,checks,stored_q,fixture_memory[9215:8192],fixture_memory[8191:0]);
   #100;
  end
 endtask
'''
        raw=raw[:start]+helper+raw[start:];start+=len(helper);end+=len(helper)
        program='''   seed_fixture=fixture;
   for(integer direction=0;direction<2;direction++)for(integer bank=4;bank<8;bank++)for(integer pattern=0;pattern<16;pattern++)begin
    reload_fixture();event_step(1,p);repeat(64)begin event_step(1,p);event_step(0,p);end
    p[292]=1;p[291:289]=~3'(bank);p[288:282]=7'((pattern==15 ? 511:1<<(pattern%9))>>1);
    p[340]=(pattern==15 || pattern%9==0);p[344]=(pattern==15 || pattern%9==8);
    $display("FULL_CP_COLLISION_BEGIN direction=%0d bank=%0d pattern=%0d record=%0d",direction,bank,pattern,checks);
    event_step(!direction[0],p);p[292]=0;event_step(direction[0],p);p[292]=1;event_step(direction[0],p);
   end
   if(fixture_index!=128 || observed_cp_sets!==18'h3ffff)$fatal(1,"FULL_CP collision coverage fixtures=%0d sets=%h",fixture_index,observed_cp_sets);
   $display("FULL_PROCESSOR_CP_COLLISION_PASS checks=%0d collisions=128 Qs=844 RAM=9216 clocks=100 sets=%h",checks,observed_cp_sets);
'''
    elif repro:
        program='''   p[292]=1;p[291:289]=~3'(4);p[288:282]=7'(16>>1);p[340]=0;p[344]=0;
   event_step(0,p);p[292]=0;event_step(1,p);p[292]=1;event_step(1,p);
   if(!observed_cp_sets[17])$fatal(1,"FULL_CP sBLOCK not observed");
   $display("FULL_PROCESSOR_CP_BLOCK_PASS checks=%0d Qs=844 RAM=9216 clocks=100",checks);
'''
        if no_cp_pulse:
            program=program.replace('p[292]=0;event_step(1,p);','p[292]=1;event_step(1,p);',1)
            program=program.replace('if(!observed_cp_sets[17])$fatal(1,"FULL_CP sBLOCK not observed");',
                                    'if(observed_cp_sets!==0)$fatal(1,"FULL_CP inactive strobe decoded a set");',1)
    else:
        # Keep a continuous physical history: each word runs through the same
        # real maintenance decoder rather than replacing owner bits in flight.
        program='''   for(integer level=0;level<2;level++)for(integer bank=4;bank<8;bank++)for(integer word=0;word<512;word++)begin
    p[292]=1;p[291:289]=~3'(bank);p[288:282]=7'(word>>1);p[340]=word[0];p[344]=word[8];
    event_step(level[0],p);p[292]=0;event_step(level[0],p);p[292]=1;event_step(level[0],p);
   end
   if(observed_cp_sets!==18'h3ffff)$fatal(1,"FULL_CP decoder coverage=%h",observed_cp_sets);
   $display("FULL_PROCESSOR_CP_SOURCE_PASS checks=%0d words=4096 Qs=844 RAM=9216 clocks=100 sets=%h",checks,observed_cp_sets);
'''
    program+='''   for(integer channel=0;channel<100;channel++)$display("FULL_CP_CLOCK channel=%0d edges=%0d",channel,totals[channel]);
   $finish;
 end'''
    raw=raw[:start]+setup+program+raw[end:]
    return raw


def run(source,original_trace,cp_source,out,engine,block_capture=True,repro=True,collisions=False,ready_zero=False,
        profile='balanced',trace_arrivals=False,no_cp_pulse=False,prblock_capture=False,equal_bank_data=False,matched_interior_data=False,interior_read=False,interior_mutation=None,interior_sequencer=False,ram_cases=0):
    assert not out.exists()
    assert not interior_read or (prblock_capture and profile=='later_control')
    assert interior_read or interior_mutation is None
    assert not interior_sequencer or interior_read
    assert not ram_cases or (collisions and interior_sequencer and 1<=ram_cases<=64)
    s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
    assert s['status']=='PASS_OWNED_FULL_PROCESSOR_PACKETS' and s['mutation'] is None
    t=json.loads((original_trace/'run-state.json').read_text());assert t['status']=='PASS_FULL_ORIGINAL_PROCESSOR_TRANSACTION_TRACE'
    for row in t['source_files']:assert sha(original_trace/row['path'])==row['output_sha256']
    cp=json.loads((cp_source/'manifest.json').read_text());verify(cp_source,cp['files'])
    assert cp['status']=='GENERATED_SOURCE_BOUND_NATIVE_CP_INPUTS'
    out.mkdir(parents=True)
    (out/'run-state.json').write_text(json.dumps(dict(status='PREPARING',driver_pid=os.getpid(),started=time.time(),
        scope=__doc__,source=str(source),original_trace=str(original_trace),engine=engine),indent=2)+'\n')
    for name in [Path(__file__).name,'generate_r_front_cp_source_reference.py','generate_shared_control_capture.py','generate_cp_block_capture.py','generate_cp_prblock_capture.py','generate_cp_interior_read.py']:
        shutil.copy2(Path(__file__).with_name(name),out/name)
    reference=out/'reference';r=generate(ROOT,reference)
    actual_inherited=inherited_timing(original_trace)
    fn=(cp_source/'functions.sv').read_text();gate,ff,phase=PROFILES[profile];descriptor=route(ROOT,gate,ff)
    raw=rewrite((source/'candidate.sv').read_text(),block_capture and bool(descriptor['early']))
    pr_descriptor=prblock_route(ROOT,gate,ff)
    if prblock_capture:raw=prblock_rewrite(raw,bool(pr_descriptor['g16_early']),bool(pr_descriptor['address_early']))
    if interior_read:raw=interior_rewrite(raw)
    candidate=source_top(shared.kernel(raw,share_native=True),fn)+'\n'+shared.control(raw)
    paths=[];rows=[]
    for row in t['source_files']:
        name=row['path'];old=original_trace/name;dest=out/name;dest.parent.mkdir(parents=True,exist_ok=True)
        text=old.read_text()
        if name=='candidate.sv':text=candidate
        elif name=='inputs/tb_r_front_local.sv':
            text=bench(text,out,repro,collisions,ready_zero,trace_arrivals,no_cp_pulse,int(phase<gate),equal_bank_data,matched_interior_data)
            if ram_cases:text=ram_sweep_bench(text,ram_cases)
            if interior_read:text=(sequential_stage_bench if interior_sequencer else stage_bench)(text,interior_mutation)
        elif name.startswith('adapters/'):
            if old.stem in shared.ADAPTERS:text=shared.adapter(text)
            elif old.stem=='dorado_local_task_capture_step':text=shared.task_adapter(text)
        elif name in ['timed/r_front_control_conta.sv','timed/r_front_control_reference.sv']:
            text=(reference/old.name).read_text()
        text=timed_source(name,text,profile)
        dest.write_text(text);paths.append(dest)
        rows.append(dict(path=name,parent_sha256=sha(old),output_sha256=sha(dest),changed=old.read_text()!=text))
    if interior_sequencer:
        name='verilog/rtl/dorado_r_interior_read_stage.sv';old=ROOT/name;dest=out/name;dest.parent.mkdir(parents=True,exist_ok=True)
        dest.write_bytes(old.read_bytes());paths.append(dest)
        rows.append(dict(path=name,parent_sha256=sha(old),output_sha256=sha(dest),changed=False))
    for board in ['BaseBd-Rev-Am',*[b['board']for b in r['boards']]]:
        for ext in ['wl','lc']:
            name='chm/sil/'+board+'.dm!1_/'+board+'.'+ext;p=out/'source-audit'/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/name).read_bytes())
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,engine=engine,
               source=str(source),source_state_sha256=sha(source/'run-state.json'),original_trace=str(original_trace),
               original_trace_state_sha256=sha(original_trace/'run-state.json'),CP_manifest_sha256=sha(cp_source/'manifest.json'),
               block_capture_enabled=block_capture,block_capture_descriptor=descriptor,reproduction_only=repro,
               root_CP_collision_program=collisions,trace_context_discontinuities=ram_cases or (128 if collisions else 0),ram_sweep_cases=ram_cases,
               initial_ready_fixture='0000' if ready_zero else 'ffff',equal_RM_STK_fixture_data=equal_bank_data,matched_intermediate_port_data=matched_interior_data,
               profile=profile,inherited_profile_ps=actual_inherited,passive_g16_arrival_trace=trace_arrivals,CP_strobe_pulsed=not no_cp_pulse,
               PrBlock_capture_enabled=prblock_capture,PrBlock_capture_descriptor=pr_descriptor,
               staged_interior_read=interior_read,interior_mutation=interior_mutation,
               interior_read_caller_implemented_in_testbench_only=interior_read,
               synthesizable_interior_sequencer=interior_sequencer,native_caller_integration_pending=interior_read,
               trace_inputs_normalized_to_actual_CP_source=True,
               shared_cached_and_pure_consumers=True,all_original_event_assertions_retained=True,
               physical_Q_bits=844,physical_RAM_bits=9216,clock_channels=100,
               declared_gate_delay_ps=gate,declared_ff_delay_ps=ff,declared_phase_delay_ps=phase,
               source_files=rows,files={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()and p.name!='run-state.json'},
               actions=[],native_integrated=False,physical_timing_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    commands=([['iverilog','-g2012','-s','tb_r_front_local','-o',str(out/'test.vvp'),*map(str,paths)],['vvp','-i',str(out/'test.vvp')]] if engine=='icarus' else
              [['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local','--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    save();code=1
    for label,cmd in zip(['compile','test'],commands):
        a=dict(label=label,command=cmd,started=time.time())
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);a['pid']=child.pid;state['actions'].append(a);save();code=child.wait()
        a.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text() if(out/'test.log').exists()else''
    marker='FULL_PROCESSOR_CP_COLLISION_PASS' if collisions else 'FULL_PROCESSOR_CP_BLOCK_PASS' if repro else 'FULL_PROCESSOR_CP_SOURCE_PASS'
    passed=code==0 and marker in log
    if passed:
        assert len(re.findall(r'^FULL_CP_CLOCK channel=\d+ edges=\d+$',log,re.M))==100
    state.update(status='PASS_ORIGINAL_FULL_PROCESSOR_CP_SOURCE' if passed else 'TERMINAL_FULL_PROCESSOR_CP_SOURCE_NOT_ESTABLISHED',finished=time.time())
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['source','original-trace','cp-source','output']:p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],default='verilator')
    p.add_argument('--without-block-capture',action='store_true')
    p.add_argument('--ready-zero',action='store_true',help='Independent physical Ready fixture0000 instead of the inheritedffff fixture')
    p.add_argument('--profile',choices=PROFILES,default='balanced')
    p.add_argument('--trace-arrivals',action='store_true');p.add_argument('--no-cp-pulse',action='store_true')
    p.add_argument('--prblock-capture',action='store_true')
    p.add_argument('--interior-read',action='store_true')
    p.add_argument('--interior-sequencer',action='store_true')
    p.add_argument('--interior-mutation',choices=['stale-data','missing-valid'])
    p.add_argument('--equal-bank-data',action='store_true',help='Diagnostic memory control: seed RM and STK with identical data; does not qualify independent bank data')
    p.add_argument('--matched-interior-data',action='store_true',help='Diagnostic control: match the two transient STK cells to the RM cells observed at check194; not general qualification')
    group=p.add_mutually_exclusive_group();group.add_argument('--all-words',action='store_true');group.add_argument('--collisions',action='store_true');group.add_argument('--ram-cases',type=int,default=0)
    a=p.parse_args()
    try:code=run(a.source.resolve(),a.original_trace.resolve(),a.cp_source.resolve(),a.output.resolve(),a.engine,not a.without_block_capture,not (a.all_words or a.collisions or a.ram_cases),bool(a.collisions or a.ram_cases),a.ready_zero,a.profile,a.trace_arrivals,a.no_cp_pulse,a.prblock_capture,a.equal_bank_data,a.matched_interior_data,a.interior_read,a.interior_mutation,a.interior_sequencer,a.ram_cases)
    except Exception as error:
        state_file=a.output/'run-state.json'
        if state_file.exists():
            state=json.loads(state_file.read_text());state.update(status='TERMINAL_PREPARATION_FAILED' if state['status']=='PREPARING' else 'TERMINAL_FULL_PROCESSOR_CP_SOURCE_EXCEPTION',
                exception=repr(error),finished=time.time());state_file.write_text(json.dumps(state,indent=2)+'\n')
        raise
    raise SystemExit(code)
