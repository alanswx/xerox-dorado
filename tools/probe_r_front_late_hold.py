#!/usr/bin/env python3
"""Precompute closed R flags for both values of a late cache-hold input.

Use two stateless combinational forecasts, then select eight CLOSED owner
bits with the real late pin. The original evaluator remains the owner of
all other outputs and the ONLY four physical SRAM banks. Forecasts consume
that owner's real read data; they instantiate no RAM or sequential storage.
No pipeline register, guest cycle or reference change is introduced.
This is a component experiment, not native timing or speed qualification.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import time

from test_r_front_forward_view import sha
from test_r_front_task_event import replace

PIN=386  # MemC15.sil+2 -> MemC e22 D -> RefHold' -> Hold/MIR


def pure_sources(source):
    raw=(source/'candidate.sv').read_text()
    top=raw[:raw.index('\nendmodule')+len('\nendmodule')]
    top=replace(top,'module dorado_r_front_control_step #(',
                'module dorado_late_hold_forecast_step #(')
    top=replace(top,'dorado_local_r_front_ready_step child(',
                'dorado_late_hold_ready_step child(')
    parts=[top]
    names=[('dorado_local_r_front_ready_step','dorado_late_hold_ready_step'),
           ('dorado_local_r_front_step','dorado_late_hold_front_step'),
           ('dorado_local_r_memory_step','dorado_late_hold_memory_step')]
    for i,(old,new) in enumerate(names):
        body=(source/'adapters'/(old+'.sv')).read_text()
        body=replace(body,'module '+old+'(','module '+new+'(')
        if i<2:body=replace(body,names[i+1][0]+' child(',names[i+1][1]+' child(')
        else:
            call=''' dorado_r_file_ports files(host_clk,advance,r_write_n,port_address,port_select_n,
   state_q[555:540],previous_address,previous_select,file_read_data,file_read_parity);'''
            body=replace(body,call,''' // Stateless read seam: these forecasts have no RAM instance or write.
 assign {file_read_parity,file_read_data}=pins_before[144:127];''')
        parts.append(body)
    result='\n'.join(parts)
    assert 'dorado_r_file_ports files('not in result
    assert not re.search(r'\b(?:always\s*@\s*\(\s*(?:posedge|negedge)|initial\b)',result)
    return result


def candidate(raw,mutation=False,all_owner_bits=False):
    old=''' assign held_next={state_next[843:596],closed_r_flags_next,state_next[587:572],closed_read_next,state_next[539:404],closed_t_visible_next,
   closed_t_inputs_next[31:16],state_next[371:116],closed_t_inputs_next[15:0],state_next[99:0]};'''
    new=''' wire [843:0] unfactored_held_next={state_next[843:596],closed_r_flags_next,state_next[587:572],closed_read_next,state_next[539:404],closed_t_visible_next,
   closed_t_inputs_next[31:16],state_next[371:116],closed_t_inputs_next[15:0],state_next[99:0]};
 // Shannon expansion of one late input, without a state or RAM copy.
 wire [400:0] forecast_before={pins_before[400:145],file_read_parity,file_read_data,pins_before[126:0]};
 wire [400:0] forecast_after0={pins_after[400:387],1'b0,pins_after[385:0]};
 wire [400:0] forecast_after1={pins_after[400:387],1'b1,pins_after[385:0]};
 (* keep *) wire [843:0] forecast0,forecast1;
 wire [31:0] forecast_address0,forecast_address1;wire [3:0] forecast_select0,forecast_select1;
 wire [1:0] forecast_write0,forecast_write1;
 dorado_late_hold_forecast_step #(.POST_MIR_PINS(POST_MIR_PINS),.STOP_CAPTURE_OLD(STOP_CAPTURE_OLD),
  .STOP_CLOCK_NEW(STOP_CLOCK_NEW),.BRANCH_CAPTURE_OLD(BRANCH_CAPTURE_OLD)) forecast_low(
  .host_clk(host_clk),.advance(1'b0),.held_q(held_q),.clock_before(clock_before),.clock_after(clock_after),
  .pins_before(forecast_before),.pins_after(forecast_after0),.refresh_before_run_capture(refresh_before_run_capture),
  .held_next(forecast0),.port_address(forecast_address0),.port_select_n(forecast_select0),.r_write_n(forecast_write0));
 dorado_late_hold_forecast_step #(.POST_MIR_PINS(POST_MIR_PINS),.STOP_CAPTURE_OLD(STOP_CAPTURE_OLD),
  .STOP_CLOCK_NEW(STOP_CLOCK_NEW),.BRANCH_CAPTURE_OLD(BRANCH_CAPTURE_OLD)) forecast_high(
  .host_clk(host_clk),.advance(1'b0),.held_q(held_q),.clock_before(clock_before),.clock_after(clock_after),
  .pins_before(forecast_before),.pins_after(forecast_after1),.refresh_before_run_capture(refresh_before_run_capture),
  .held_next(forecast1),.port_address(forecast_address1),.port_select_n(forecast_select1),.r_write_n(forecast_write1));
 (* keep *) wire [7:0] flags_if_low=forecast0[595:588],flags_if_high=forecast1[595:588];
 wire [843:0] selected_forecast=PIN_SELECT ? forecast1 : forecast0;
 wire [7:0] selected_closed_flags=PIN_SELECT ? flags_if_high : flags_if_low;
 assign held_next={unfactored_held_next[843:596],selected_closed_flags,unfactored_held_next[587:0]};'''
    select='!pins_after[386]'if mutation and not all_owner_bits else'pins_after[386]'
    if all_owner_bits:
        selected='(pins_after[386] ? forecast0 : forecast1)'if mutation else'selected_forecast'
        new=replace(new,'assign held_next={unfactored_held_next[843:596],selected_closed_flags,unfactored_held_next[587:0]};',
                    'assign held_next='+selected+';')
    return replace(raw,old,new.replace('PIN_SELECT',select))


def bench(raw):
    # Icarus requires the diagnostic counter to be declared before a task
    # that refers to it; do not depend on Verilator's forward binding.
    marker=' reg [5:0] l17_changed=0;integer checks=0;'
    assert raw.count(marker)==1
    raw=replace(raw,marker,marker+'''
 integer late_hold_checks=0,late_hold_distinct=0;reg [1:0] late_hold_seen=0;
 reg [7:0] late_hold_distinct_bits=0;
 integer late_hold_state_distinct=0;reg [843:0] late_hold_state_bits=0;
 task automatic check_late_hold;
  begin
   if(dut.selected_forecast!==dut.unfactored_held_next)
     $fatal(1,"Late Hold selected full-state forecast mismatch check=%0d pin=%b",checks,pa[386]);
   if(dut.forecast_address0!==port_address || dut.forecast_address1!==port_address ||
      dut.forecast_select0!==port_select || dut.forecast_select1!==port_select)
     $fatal(1,"Late Hold forecast lost shared SRAM address/selection epoch check=%0d",checks);
   if((pa[386] ? dut.forecast_write1 : dut.forecast_write0)!==rwrite)
     $fatal(1,"Late Hold selected SRAM write window mismatch check=%0d",checks);
   late_hold_checks=late_hold_checks+1;late_hold_seen[pa[386]]=1;
   if(dut.flags_if_low!==dut.flags_if_high)late_hold_distinct=late_hold_distinct+1;
   late_hold_distinct_bits=late_hold_distinct_bits|(dut.flags_if_low^dut.flags_if_high);
   if(dut.forecast0!==dut.forecast1)late_hold_state_distinct=late_hold_state_distinct+1;
   late_hold_state_bits=late_hold_state_bits|(dut.forecast0^dut.forecast1);
  end
 endtask
''')
    raw=replace(raw,'expected=n;predicted=events;ea=address;',
                'check_late_hold();expected=n;predicted=events;ea=address;')
    raw=replace(raw,'if(n!==expected) $fatal(1,"R front state commit changed settled Q check=%0d",checks);',
                '''if(n!==expected) $fatal(1,"R front state commit changed settled Q check=%0d",checks);
     check_late_hold();''')
    marker=' $display("R_FRONT_LOCAL_PROBE PASS '
    assert raw.count(marker)==1
    raw=raw.replace(marker,''' $display("LATE_HOLD_COFACTOR checks=%0d distinct=%0d bits=%h seen=%b state_distinct=%0d state_bits=%h shared_RAM_banks=4 sequential_forecasts=0",
   late_hold_checks,late_hold_distinct,late_hold_distinct_bits,late_hold_seen,late_hold_state_distinct,late_hold_state_bits);
'''+marker)
    return raw


def run(source,out,mutation,all_owner_bits):
    assert not out.exists();parent=json.loads((source/'run-state.json').read_text())
    assert parent['status']=='PASS_CP_BEFORE_ROOT_SUBEVENTS' and not parent['mutation']
    assert parent['summary']=='checks=14884 writes=238 reads=6928 epochs=392'
    assert parent['actions'][-1]['returncode']==0
    out.mkdir(parents=True);paths=[];records=[]
    for row in parent['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256'];raw=p.read_text()
        if p.name=='candidate.sv':raw=candidate(raw,mutation,all_owner_bits)
        elif p.name=='tb_r_front_local.sv':raw=bench(raw)
        q=out/row['path'];q.parent.mkdir(parents=True,exist_ok=True);q.write_text(raw);paths.append(str(q))
        records.append(dict(path=row['path'],parent_sha256=sha(p),output_sha256=sha(q),changed=sha(p)!=sha(q)))
    assert sum(v['changed']for v in records)==2
    pure=out/'late_hold_forecasts.sv';pure.write_text(pure_sources(source));paths.append(str(pure))
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    engine=parent['engine'];defines=[s for s in parent['actions'][0]['command']if s.startswith('-D')]
    commands=[['iverilog','-g2012','-s','tb_r_front_local',*defines,'-o',str(out/'test.vvp'),*paths],
              ['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else[
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,
         '--Mdir',str(out/'obj'),'-o','test',*paths],[str(out/'obj/test')]]
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),scope=__doc__,engine=engine,mutation=mutation,
               parent=str(source),parent_state_sha256=sha(source/'run-state.json'),source_files=records,
               pure_forecasts_sha256=sha(pure),actions=[],late_after_input_bit=PIN,
               late_input_name='MemC15.sil+2 / MemC e22 D / RefHold',
               all_owner_bits_selected=all_owner_bits,selected_owner_bits=list(range(844))if all_owner_bits else list(range(588,596)),
               original_reference_and_checks_retained=True,forecasts_sequential_storage_bits=0,
               forecasts_RAM_instances=0,physical_SRAM_banks=4,extra_guest_cycles=0,
               full_selected_forecast_checked=True,both_SRAM_address_select_epochs_checked=True,
               selected_write_gate_checked=True,native_qualified=False,physical_timing_qualified=False,
               assembly_permitted=False,speedup_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for i,command in enumerate(commands):
        with(out/('compile.log'if i==0 else'test.log')).open('w')as log:
            p=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT)
            a=dict(command=command,pid=p.pid,started=time.time());state['actions'].append(a);save()
            rc=p.wait();a.update(returncode=rc,finished=time.time());save()
        if rc:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    match=re.search(r'R_FRONT_LOCAL_PROBE PASS ([^\n]+)',log)
    audit=re.search(r'^LATE_HOLD_COFACTOR (.+)$',log,re.M)
    passed=not rc and bool(match) and bool(audit)
    state.update(status='PASS_LATE_HOLD_COFACTOR'if passed else'LATE_HOLD_COFACTOR_FAILED',
                 summary=match[1]if match else None,cofactor_coverage=audit[1]if audit else None,finished=time.time())
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--invert-late-select',action='store_true')
    p.add_argument('--all-owner-bits',action='store_true',help='Select the complete next state, retaining the original shared SRAM')
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.invert_late_select,a.all_owner_bits))
