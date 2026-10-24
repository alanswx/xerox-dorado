#!/usr/bin/env python3
"""Expose the full844-Q evaluator's single physical RM/STK owner.

Move only the existing four-bank instance to the caller. Preserve the exact
read address/select, old-state write data, parity generation and commit enable.
Keep all original reference assertions and cache exercises; only the instance
wiring and observational array path in the bench change. This is a prerequisite
for ordered, uncommitted packet evaluation, not packet/cache/timing qualification.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
PORTS=',input wire [17:0] external_file_read,output wire [15:0] file_write_data'
OWNER=''' dorado_r_file_ports files(host_clk,advance,r_write_n,port_address,port_select_n,
   state_q[555:540],previous_address,previous_select,file_read_data,file_read_parity);'''
SEAM=''' // Read data belongs to the caller's one physical SRAM owner. The address,
 // selection and write datum retain their original pre-event epochs.
 assign {file_read_parity,file_read_data}=external_file_read;
 assign file_write_data=state_q[555:540];'''
EXTERNAL_OWNER='''
 wire [17:0] external_file_read;wire [15:0] file_write_data;
 dorado_r_file_ports files(host_clk,dut.cp_commit,rwrite,port_address,port_select,
  file_write_data,port_address,port_select,external_file_read[15:0],external_file_read[17:16]);
'''


def once(raw,old,new):
    assert raw.count(old)==1,(old,raw.count(old))
    return raw.replace(old,new,1)


def transforms(raw,name,mutation):
    original=raw
    if name=='candidate.sv':
        old=' input wire cp_reset,output wire cp_ready\n);'
        new=' input wire cp_reset,output wire cp_ready'+PORTS+'\n);'
        raw=once(raw,old,new)
        end='child_source,child_fa,child_ready,physical_forward,r_flag_clocks_after,r_flag_sets_after,front_capture_pins);'
        extended=end[:-2]+',external_file_read,file_write_data);'
        raw=once(raw,end,extended)
        assert raw.replace(extended,end,1).replace(new,old,1)==original
    elif name in ['dorado_local_r_front_ready_step.sv','dorado_local_r_front_step.sv','dorado_local_r_memory_step.sv']:
        suffix={'dorado_local_r_front_ready_step.sv':'front_capture_pins',
                'dorado_local_r_front_step.sv':'front_capture_pins',
                'dorado_local_r_memory_step.sv':'capture_pins'}[name]
        width={'dorado_local_r_front_ready_step.sv':245,'dorado_local_r_front_step.sv':235,'dorado_local_r_memory_step.sv':201}[name]
        old=f'input wire [{width}:0] '+suffix+');';new=old[:-2]+PORTS+');'
        raw=once(raw,old,new)
        if name=='dorado_local_r_memory_step.sv':
            raw=once(raw,OWNER,SEAM)
            assert raw.replace(SEAM,OWNER,1).replace(new,old,1)==original
        else:
            tail=('front_capture_pins[235:0]);' if name=='dorado_local_r_front_ready_step.sv' else
                  'effective_pins(captured_source,captured_fa));')
            extended=tail[:-2]+',external_file_read,file_write_data);'
            raw=once(raw,tail,extended)
            assert raw.replace(extended,tail,1).replace(new,old,1)==original
    elif name=='tb_r_front_local.sv':
        old='control_flags,q,cp_reset,cp_ready);';new=old[:-2]+',external_file_read,file_write_data);'
        raw=once(raw,old,new)
        marker=' dorado_r_front_control_step dut('
        raw=once(raw,marker,EXTERNAL_OWNER+marker)
        count=raw.count('dut.child.child.child.files.');assert count==17,count
        raw=raw.replace('dut.child.child.child.files.','files.')
        assert raw.replace('files.','dut.child.child.child.files.').replace(EXTERNAL_OWNER,'',1).replace(new,old,1)==original
        if mutation=='drop-write':
            raw=once(raw,'files(host_clk,dut.cp_commit,rwrite,','files(host_clk,1\'b0,rwrite,')
        elif mutation=='invert-write-data':
            raw=once(raw,'  file_write_data,port_address,port_select,',"  file_write_data^16'h0001,port_address,port_select,")
    return raw


def run(source,out,mutation):
    assert not out.exists()
    parent=json.loads((source/'run-state.json').read_text())
    assert parent['status']=='PASS_CONTROL_FALL_COMPLETE_ORIGINAL_CAPTURE'
    assert parent['all100_clock_counts_match'] and parent['original_checks_and_references_retained']
    engine=parent['engine'];out.mkdir(parents=True)
    rows=[];paths=[]
    for row in parent['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256']
        dest=out/row['path'];dest.parent.mkdir(parents=True,exist_ok=True)
        raw=p.read_text();result=transforms(raw,p.name,mutation);dest.write_text(result)
        paths.append(dest);rows.append(dict(path=row['path'],parent_sha256=sha(p),output_sha256=sha(dest),changed=raw!=result))
    assert sum(r['changed'] for r in rows)==5
    # Original sources also compile an unused legacy dorado_r_memory_step.
    # Only the top's actual adapter hierarchy and bench are reachable here.
    reachable=[p for p in paths if 'adapters' in p.parts or p.name in ['candidate.sv','tb_r_front_local.sv']]
    assert sum(p.read_text().count('dorado_r_file_ports files(') for p in reachable)==1
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,engine=engine,mutation=mutation,
        source=str(source),source_state_sha256=sha(source/'run-state.json'),source_files=rows,actions=[],
        all_original_assertions_and_reference_models_retained=True,architectural_state_bits=844,RAM_bits=9216,clock_channels=100,
        physical_memory_instances=1,extra_architectural_or_memory_state_bits=0,
        uncommitted_packet_cache_qualified=False,native_integrated=False,timing_qualified=False,hardware_changed=False)

    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')

    defines=[a for a in parent['actions'][0]['command'] if a.startswith('-D')]
    commands=([
        ['iverilog','-g2012','-s','tb_r_front_local',*defines,'-o',str(out/'test.vvp'),*map(str,paths)],
        ['vvp','-i',str(out/'test.vvp')]] if engine=='icarus' else [
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,
         '--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    save()
    for label,command in zip(['compile','test'],commands):
        with (out/(label+'.log')).open('x') as log:
            p=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT)
            row=dict(label=label,command=command,pid=p.pid,started=time.time());state['actions'].append(row);save()
            code=p.wait();row.update(returncode=code,finished=time.time());save()
        if code:break
    text=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    compiled=state['actions'][0]['returncode']==0
    if mutation:
        passed=compiled and code!=0 and re.search(r'(R front storage mismatch|R front state mismatch|R front settled port mismatch)',text) is not None
    else:
        passed=compiled and code==0 and 'R_FRONT_LOCAL_PROBE PASS checks=14884 writes=238 reads=6928 epochs=392' in text
        if passed:
            pattern=r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$'
            assert len(re.findall(pattern,text,re.M))==100 and re.findall(pattern,text,re.M)==re.findall(pattern,(source/'test.log').read_text(),re.M)
            coverage=re.search(r'^CP_PREFIX_CACHE (.+)$',text,re.M)[1]
            assert coverage==parent['cache_coverage']
            assert 'CONTROL_FALL_COVERAGE captures=4975' in text
            state.update(cache_coverage=coverage,all100_clock_counts_match=True,sparse_captures=4975)
    state.update(status=('PASS_SINGLE_OWNER_MEMORY_SEAM_COUNTEREXAMPLE' if mutation else 'PASS_FULL_PROCESSOR_SINGLE_OWNER_MEMORY_SEAM') if passed else
        'TERMINAL_PROCESSOR_MEMORY_SEAM_NOT_ESTABLISHED',finished=time.time(),test_log_sha256=sha(out/'test.log') if (out/'test.log').exists() else None)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutation',choices=['drop-write','invert-write-data']);a=p.parse_args()
    raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.mutation))
