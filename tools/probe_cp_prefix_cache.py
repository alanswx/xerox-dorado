#!/usr/bin/env python3
"""Qualify a guarded CP-prefix cache against the complete original graph.

Only canonical unchanged prefixes bypass. Other prefixes cross a host register;
the root/operand transfer remains a single commit of the original four SRAMs.
Cache tags cover every intrinsic prefix input and the CP pins consumed by the
root. No architectural owner advances while unready. This component experiment
does not qualify native pause consumers, clocks, frequency or software speed.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import time

from fit_memory_mir_mask import gate
from test_r_front_forward_view import sha
from test_r_front_task_event import replace

CACHE=r'''
module dorado_cp_prefix_cache(
 input wire clk,reset,request,
 input wire [155:0] current_q,next_q,
 input wire before_root,after_root,
 input wire [397:0] before_pins,after_pins,
 input wire refresh,settled,
 input wire [268:0] prefix_live,
 output wire ready,fast_path,cache_hit,
 output reg [268:0] prefix_saved=0,
 output reg [10:0] prefix_cp_saved=0,after_cp_saved=0);
 reg canonical_valid=0,cache_valid=0;
 reg [155:0] canonical_q=0;
 reg [397:0] canonical_pins=0;
 reg canonical_root=0,canonical_refresh=0;
 reg [566:0] cache_key=0;
 wire split_event=(before_root!=after_root)&&(before_pins[292:282]!=after_pins[292:282]);
 wire [10:0] prefix_cp=split_event ? after_pins[292:282] : before_pins[292:282];
 wire [566:0] current_key={current_q,before_pins,before_root,prefix_cp,refresh};
 wire canonical_match=(current_q==canonical_q)&&(before_pins==canonical_pins)&&
                      (before_root==canonical_root)&&(refresh==canonical_refresh);
 assign fast_path=canonical_valid && canonical_match && !split_event;
 assign cache_hit=cache_valid && (cache_key==current_key) && (after_cp_saved==after_pins[292:282]);
 assign ready=!reset && (fast_path || cache_hit);
 always @(posedge clk)begin
  if(reset)begin canonical_valid<=0;cache_valid<=0;end
  else begin
   if(!fast_path && !cache_hit)begin
    cache_key<=current_key;prefix_saved<=prefix_live;cache_valid<=1;
    prefix_cp_saved<=prefix_cp;after_cp_saved<=after_pins[292:282];
   end
   if(request && ready)begin
    canonical_valid<=settled;canonical_q<=next_q;canonical_pins<=after_pins;
    canonical_root<=after_root;canonical_refresh<=refresh;
    // Preserve the completed transaction's identity prefix until the caller
    // publishes its new owners/pins. Real native owners update on this edge;
    // retaining the tag also makes the interface safe for a delayed caller.
    if(fast_path)begin
     cache_key<=current_key;prefix_saved<={current_q,56'b0,56'b0,1'b0};cache_valid<=1;
     prefix_cp_saved<=prefix_cp;after_cp_saved<=after_pins[292:282];
    end
   end
  end
 end
endmodule
'''


def parallel_control(body,direct_cp=False):
    """Select complete control results after two pure combinational roots.

    Keep the raw prefix producer and the single physical SRAM owner intact.
    The guard no longer feeds either root's data/clock computation.
    """
    start=body.index(' assign {prefix_q,prefix_events,prefix_repeated,prefix_unsettled}=')
    end=body.index(' dorado_control_hold_atomic_step #(.WAVES(WAVES)) cp_arrival(',start)
    body=body[:start]+''' assign {prefix_q,prefix_events,prefix_repeated,prefix_unsettled}=prefix_saved;
 wire [397:0] root_before={pins_before[397:293],prefix_cp_saved,pins_before[281:0]};
 wire [397:0] root_after={pins_after[397:293],after_cp_saved,pins_after[281:0]};
 wire [155:0] staged_q,direct_q;
 wire [55:0] direct_events,direct_repeated;
 wire staged_unsettled,direct_unsettled;
 wire [93:0] staged_flags,direct_flags;
'''+body[end:]
    body=replace(body,'  state_next,root_events,root_repeated,root_unsettled,\n  refresh_before_run_capture,flags);',
        '''  staged_q,root_events,root_repeated,root_unsettled,
  refresh_before_run_capture,staged_flags);
 dorado_control_hold_atomic_step #(.WAVES(WAVES)) direct_arrival(
  state_q,clock_before,clock_after,pins_before,pins_after,
  direct_q,direct_events,direct_repeated,direct_unsettled,
  refresh_before_run_capture,direct_flags);''')
    body=replace(body,''' assign cp_prefix_events=prefix_events;assign cp_prefix_repeated=prefix_repeated;
 assign clock_events=prefix_events|root_events;
 assign repeated_events=prefix_repeated|root_repeated|(prefix_events&root_events);
 assign unsettled=prefix_unsettled|root_unsettled;''',
        ''' assign {state_next,clock_events,repeated_events,unsettled,flags,cp_prefix_events,cp_prefix_repeated}=
  fast_path ? {direct_q,direct_events,direct_repeated,direct_unsettled,direct_flags,56'b0,56'b0} :
  {staged_q,prefix_events|root_events,prefix_repeated|root_repeated|(prefix_events&root_events),
   prefix_unsettled|root_unsettled,staged_flags,prefix_events,prefix_repeated};''')
    assert 'fast_path ?'in body and body.count('fast_path ?')==1
    if direct_cp:
        body=replace(body,' assign {state_next,clock_events,repeated_events,unsettled,flags,cp_prefix_events,cp_prefix_repeated}=',
            ''' wire [155:0] selected_q;
 // Direct CP Q is identical to ordered prefix/root CP Q for every binary
 // input, independently of the guard. Keep every other result selected.
 assign state_next={selected_q[155:76],direct_q[75:64],selected_q[63:0]};
 assign {selected_q,clock_events,repeated_events,unsettled,flags,cp_prefix_events,cp_prefix_repeated}=''' )
    return body


def candidate(raw,mutation=None,parallel_root=False,control_settled=False,settling_waves=8,direct_cp=False):
    marker='output wire shared_state_mismatch,output wire [93:0] control_flags,output wire [843:0] physical_state\n'
    raw=replace(raw,marker,marker.rstrip()+',\n input wire cp_reset,output wire cp_ready\n')
    call=''' dorado_control_hold_ordered_step control(joint_q,clock_before,clock_after,pins_before[397:0],pins_after[397:0],
  joint_next,joint_events,joint_repeated,cu,refresh_before_run_capture,control_flags,cp_prefix_events,cp_prefix_repeated);'''
    new=''' wire [268:0] cp_prefix_live,cp_prefix_saved;
 wire [10:0] cp_prefix_cp_saved,cp_after_cp_saved;
 wire cp_fast,cp_cache_hit;
 wire cp_commit=advance && cp_ready;
 dorado_cp_prefix_cache cp_guard(host_clk,cp_reset,advance,joint_q,joint_next,
  clock_before,clock_after,pins_before[397:0],pins_after[397:0],refresh_before_run_capture,
  !unsettled,cp_prefix_live,cp_ready,cp_fast,cp_cache_hit,cp_prefix_saved,cp_prefix_cp_saved,cp_after_cp_saved);
 dorado_control_hold_ordered_step control(joint_q,clock_before,clock_after,pins_before[397:0],pins_after[397:0],
  joint_next,joint_events,joint_repeated,cu,refresh_before_run_capture,control_flags,cp_prefix_events,cp_prefix_repeated,
  cp_fast,cp_prefix_saved,cp_prefix_cp_saved,cp_after_cp_saved,cp_prefix_live);'''
    raw=replace(raw,call,new)
    raw=replace(raw,'},host_clk,advance,state_q[751:0],clock_before,clock_after,before_child,after_child,',
                '},host_clk,cp_commit,state_q[751:0],clock_before,clock_after,before_child,after_child,')
    start=raw.index('module dorado_control_hold_ordered_step ')
    end=raw.index('\nendmodule',start)
    body=raw[start:end]
    body=replace(body,'output wire [55:0] cp_prefix_events,cp_prefix_repeated);',
        '''output wire [55:0] cp_prefix_events,cp_prefix_repeated,
 input wire fast_path,input wire [268:0] prefix_saved,
 input wire [10:0] prefix_cp_saved,after_cp_saved,
 output wire [268:0] prefix_live);''')
    body=replace(body,' wire prefix_unsettled,root_unsettled;',
        ''' wire prefix_unsettled,root_unsettled;
 wire [155:0] raw_prefix_q;wire [55:0] raw_prefix_events,raw_prefix_repeated;wire raw_prefix_unsettled;
 assign prefix_live={raw_prefix_q,raw_prefix_events,raw_prefix_repeated,raw_prefix_unsettled};
 assign {prefix_q,prefix_events,prefix_repeated,prefix_unsettled}=
  fast_path ? {state_q,56'b0,56'b0,1'b0} : prefix_saved;
 wire [397:0] root_before=fast_path ? prefix_pins :
  {pins_before[397:293],prefix_cp_saved,pins_before[281:0]};
 wire [397:0] root_after=fast_path ? pins_after :
  {pins_after[397:293],after_cp_saved,pins_after[281:0]};''')
    body=replace(body,'  prefix_q,prefix_events,prefix_repeated,prefix_unsettled,',
                 '  raw_prefix_q,raw_prefix_events,raw_prefix_repeated,raw_prefix_unsettled,')
    body=replace(body,'  prefix_q,clock_before,clock_after,prefix_pins,pins_after,',
                 '  prefix_q,clock_before,clock_after,root_before,root_after,')
    assert not direct_cp or parallel_root
    if parallel_root:body=parallel_control(body,direct_cp)
    raw=raw[:start]+body+raw[end:]+CACHE
    assert settling_waves in range(1,9)
    assert raw.count('parameter integer WAVES=8')==1
    assert 'module dorado_control_hold_atomic_step #(parameter integer WAVES=24)'in raw
    raw=raw.replace('parameter integer WAVES=8',f'parameter integer WAVES={settling_waves}')
    if control_settled:
        raw=replace(raw,'  !unsettled,cp_prefix_live,cp_ready,', '  !cu,cp_prefix_live,cp_ready,')
    if mutation=='stale-tag':raw=replace(raw,'assign cache_hit=cache_valid && (cache_key==current_key) && (after_cp_saved==after_pins[292:282]);','assign cache_hit=cache_valid;')
    elif mutation=='bad-prefix':raw=replace(raw,'prefix_saved<=prefix_live;','prefix_saved<=prefix_live ^ (269\'b1 << (113+69));')
    elif mutation=='bad-mir-prefix':raw=replace(raw,'prefix_saved<=prefix_live;','prefix_saved<=prefix_live ^ (269\'b1 << (113+85));')
    elif mutation=='unguarded-write':raw=replace(raw,'wire cp_commit=advance && cp_ready;','wire cp_commit=advance;')
    elif mutation is not None:raise ValueError(mutation)
    return raw


def bench(raw):
    raw=replace(raw,' reg host_clk=0,advance=1;',' reg host_clk=0,advance=1,cp_reset=0;wire cp_ready;')
    raw=replace(raw,'port_address,port_select,raw_data,raw_parity,source_pins,fa_n,ready_flags,conta_next,shared_mismatch,control_flags,q);',
                'port_address,port_select,raw_data,raw_parity,source_pins,fa_n,ready_flags,conta_next,shared_mismatch,control_flags,q,cp_reset,cp_ready);')
    marker=' reg [5:0] l17_changed=0;integer checks=0;'
    raw=replace(raw,marker,marker+'''
 integer cp_fast_transactions=0,cp_staged_transactions=0,cp_wait_edges=0,cp_retries=0,cp_reset_invalidations=0;
 task automatic cp_wait_edge;
  reg [9215:0] unchanged;
  begin
   unchanged=memory_image();advance=1;#1;
   if(cp_ready!==1'b0)$fatal(1,"CP wait fixture was already ready check=%0d",checks);
   if(dut.child.child.child.files.advance!==1'b0)
    $fatal(1,"CP unready request reached the original SRAM owner check=%0d",checks);
   host_clk=1;#100;host_clk=0;#100;
   if(memory_image()!==unchanged)$fatal(1,"CP stage changed original SRAM before ready check=%0d",checks);
   cp_wait_edges=cp_wait_edges+1;
  end
 endtask
 task automatic prepare_cp;
  integer waits;reg [400:0] saved_pa;
  begin
   waits=0;
   if(!cp_ready)begin
    cp_wait_edge;
    if(!cp_ready)$fatal(1,"CP stage failed to become ready check=%0d",checks);
    // Reset only the auxiliary pipeline while owners and reference stay put.
    if(checks==0)begin
     cp_reset=1;#100;
     if(cp_ready)$fatal(1,"CP ready survived pipeline reset");
     cp_wait_edge;cp_reset=0;#100;cp_reset_invalidations=cp_reset_invalidations+1;
     if(cp_ready)$fatal(1,"CP stale cache survived pipeline reset");
     cp_wait_edge;
    end
    // Change a prefix/root CP data input while the guest is held. An old
    // payload must not be consumed; recapture, then restore and retry.
    if(checks==0 || checks%257==0)begin
     saved_pa=pa;pa[282]=!pa[282];#100;
     if(dut.cp_cache_hit)$fatal(1,"CP stale tag accepted changed CP data check=%0d",checks);
     if(!dut.cp_fast)begin
      cp_wait_edge;
      pa=saved_pa;#100;
      if(dut.cp_cache_hit)$fatal(1,"CP retry reused wrong captured data check=%0d",checks);
      cp_wait_edge;cp_retries=cp_retries+1;
     end else begin pa=saved_pa;#100;end
    end
   end
   while(!cp_ready && waits<4)begin cp_wait_edge;waits=waits+1;end
   if(cp_ready!==1'b1)$fatal(1,"CP transaction never ready check=%0d",checks);
   advance=1;
   if(dut.cp_fast)cp_fast_transactions=cp_fast_transactions+1;
   else cp_staged_transactions=cp_staged_transactions+1;
  end
 endtask
''')
    raw=replace(raw,'     ca=next_root;pa=pins;pa[144:127]=0;\n     #100;',
                '     ca=next_root;pa=pins;pa[144:127]=0;\n     #100;prepare_cp;#100;')
    marker='   $display("MEMORY_MIR_MASK wave_checks=%0d high_distinct=%0d low_clear_epochs=%0d",mir_mask_checks,mir_mask_high_distinct,mir_mask_low_clears);'
    raw=replace(raw,marker,'''   if(cp_fast_transactions==0 || cp_staged_transactions==0 || cp_retries==0 || cp_reset_invalidations!=1)
    $fatal(1,"CP cache coverage incomplete fast=%0d staged=%0d retry=%0d reset=%0d",cp_fast_transactions,cp_staged_transactions,cp_retries,cp_reset_invalidations);
   $display("CP_PREFIX_CACHE fast=%0d staged=%0d wait_edges=%0d retries=%0d resets=%0d",cp_fast_transactions,cp_staged_transactions,cp_wait_edges,cp_retries,cp_reset_invalidations);
'''+marker)
    return raw


def run(source,out,mutation,parallel_root=False,control_settled=False,settling_waves=8,direct_cp=False):
    assert not out.exists()
    parent,g=gate(source,json.loads((source/'run-state.json').read_text())['engine'])
    assert parent['all_hold_clocks']and parent['settling_waves']==8
    generated=candidate((source/'candidate.sv').read_text(),mutation,parallel_root,control_settled,settling_waves,direct_cp)
    tb=bench((source/'inputs/tb_r_front_local.sv').read_text())
    out.mkdir(parents=True);paths=[];records=[]
    for row in parent['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256'];raw=p.read_text()
        if p.name=='candidate.sv':raw=generated
        elif p.name=='tb_r_front_local.sv':raw=tb
        q=out/row['path'];q.parent.mkdir(parents=True,exist_ok=True);q.write_text(raw);paths.append(str(q))
        records.append(dict(path=row['path'],parent_sha256=sha(p),output_sha256=sha(q),changed=sha(p)!=sha(q)))
    assert sum(r['changed']for r in records)==2
    for fatal in re.findall(r'\$fatal\([^;]+;',(source/'inputs/tb_r_front_local.sv').read_text()):assert fatal in tb,fatal
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    engine=parent['engine'];defines=[s for s in parent['actions'][0]['command']if s.startswith('-D')]
    commands=[['iverilog','-g2012','-s','tb_r_front_local',*defines,'-o',str(out/'test.vvp'),*paths],
              ['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else[
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,
         '--Mdir',str(out/'obj'),'-o','test',*paths],[str(out/'obj/test')]]
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),scope=__doc__,engine=engine,mutation=mutation,
        parent=str(source),parent_state_sha256=sha(source/'run-state.json'),source_files=records,actions=[],
        parallel_control_results=parallel_root,control_only_settled_eligibility=control_settled,
        settling_waves=settling_waves,
        direct_cp_projection=direct_cp,
        corrupted_prefix_joint_Q_bit={'bad-prefix':69,'bad-mir-prefix':85}.get(mutation),
        original_reference_and_checks_retained=True,physical_SRAM_banks=4,extra_guest_cycles=0,
        auxiliary_prefix_bits=269,tag_key_bits=567,cached_CP_pin_bits=22,
        native_pause_consumers_qualified=False,native_qualified=False,physical_timing_qualified=False,
        assembly_permitted=False,speedup_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for i,command in enumerate(commands):
        with(out/('compile.log'if i==0 else'test.log')).open('w')as log:
            process=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT)
            action=dict(command=command,pid=process.pid,started=time.time());state['actions'].append(action);save()
            code=process.wait();action.update(returncode=code,finished=time.time());save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    match=re.search(r'R_FRONT_LOCAL_PROBE PASS ([^\n]+)',log);coverage=re.search(r'^CP_PREFIX_CACHE (.+)$',log,re.M)
    passed=code==0 and bool(match)and bool(coverage)
    state.update(status='PASS_CP_PREFIX_CACHE_ORIGINAL_COMPARISON'if passed else'CP_PREFIX_CACHE_FAILED',
                 summary=match[1]if match else None,cache_coverage=coverage[1]if coverage else None,finished=time.time())
    if passed:
        clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',log,re.M)
        assert len(clocks)==100 and clocks==re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',(source/'test.log').read_text(),re.M)
        state['all100_original_clock_counts_match']=True
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['source','output']:parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--mutation',choices=['stale-tag','bad-prefix','bad-mir-prefix','unguarded-write'])
    parser.add_argument('--parallel-root',action='store_true')
    parser.add_argument('--control-settled',action='store_true')
    parser.add_argument('--settling-waves',type=int,choices=range(1,9),default=8)
    parser.add_argument('--direct-cp',action='store_true')
    args=parser.parse_args();raise SystemExit(run(args.source.resolve(),args.output.resolve(),args.mutation,args.parallel_root,args.control_settled,args.settling_waves,args.direct_cp))
