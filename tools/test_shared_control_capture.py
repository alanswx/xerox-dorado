#!/usr/bin/env python3
"""Replay the unchanged full owned processor fixture with shared capture frames.

Retain every2470-bit result,844-Q,9216-bit RAM,100-clock, geometry and cache
check. This first integration shares controller/task frames in ordinary/root
consumers; sparse paired-first and unknown transfers retain their original
evaluators. Actual CP source/arrival and native timing remain separate gates.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

import generate_shared_control_capture as generate
from review_baseboard_clock_consumer import verify
from test_r_front_forward_view import sha


def run(source,out,engine,enabled=True,diagnostic=False,mutation=None,snapshot=False):
    assert not out.exists()
    s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
    assert s['status']=='PASS_OWNED_FULL_PROCESSOR_PACKETS' and s['mutation'] is None
    out.mkdir(parents=True)
    paths=[];rows=[]
    for row in s['source_files']:
        name=row['path'];p=source/name;assert sha(p)==row['output_sha256']
        raw=p.read_text();new=raw
        if name=='candidate.sv':new=generate.kernel(raw,enabled)
        elif p.stem in generate.ADAPTERS and name.startswith('adapters/'):new=generate.adapter(raw)
        elif name=='adapters/dorado_local_task_capture_step.sv':new=generate.task_adapter(raw)
        dest=out/name;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_text(new)
        paths.append(dest);rows.append(dict(path=name,parent_sha256=sha(p),output_sha256=sha(dest),changed=new!=raw))
    for n in ['caller.sv','tb.sv','forward.sv']:
        shutil.copy2(source/n,out/n);paths.append(out/n)
    if diagnostic:
        p=out/'tb.sv';tb=p.read_text();marker='    $fatal(1,"PACKET first result mismatch record=%0d",idx);'
        assert tb.count(marker)==1
        extra=''
        for instance in ['single','first','second']:
            root='dut.packet.'+instance
            extra+=f'''    $display("SHARED_CAPTURE_DIAG {instance} cb=%b ca=%b flag=%b child=%h canonical=%h root_frame_end=%h",{root}.clock_before,{root}.clock_after,{root}.shared_capture[864],{{{root}.child_next[751:736],{root}.child_next[79:64],{root}.child_next[31:0]}},{root}.conta_next[63:0],{root}.shared_frames[863:816]);
'''
        p.write_text(tb.replace(marker,extra+marker,1))
    if snapshot:
        p=out/'tb.sv';tb=p.read_text();marker='   if(first_result!==expected_first)begin'
        assert tb.count(marker)==1
        root='dut.packet.second.child.child.child.child.core.root_arrival'
        args=f'{{{root}.stop_tasks_capture,{root}.canonical_task_clock,{root}.state_q,{root}.clock_before,{root}.clock_after,{root}.pins_before,{root}.pins_after,{root}.shared_capture}}'
        result=f'{{{root}.state_next,{root}.clock_events,{root}.repeated_events,{root}.unsettled}}'
        fallback=f'{{{root}.original_q,{root}.original_events,{root}.original_repeated,{root}.original_unsettled}}'
        extra=f'   if(idx==14)$display("SHARED_CAPTURE_SAMPLE input=%h result=%h fallback=%h",{args},{result},{fallback});\n'
        p.write_text(tb.replace(marker,extra+marker,1))
    (out/'packet.sv').write_text(generate.kernel((source/'packet.sv').read_text(),enabled));paths.append(out/'packet.sv')
    (out/'capture.sv').write_text(generate.control((source/'candidate.sv').read_text(),mutation));paths.append(out/'capture.sv')
    for n in [Path(__file__).name,'generate_shared_control_capture.py']:
        shutil.copy2(Path(__file__).with_name(n),out/n)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),
               source_state_sha256=sha(source/'run-state.json'),engine=engine,enabled=enabled,
               source_files=rows,files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file()},
               actions=[],all_original_checks_retained=True,stored_owner_bits=828,external_R_bits=16,
               SRAM_bits=9216,added_architectural_state=0,native_integrated=False,
               physical_timing_qualified=False,actual_CP_source_integrated=False,passive_diagnostic=diagnostic,mutation=mutation,
               passive_consumer_snapshot=snapshot,full_context_XZ_fallback=True)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    commands=([['iverilog','-g2012','-s','tb_r_front_packet','-o',str(out/'test.vvp'),*map(str,paths)],['vvp','-i',str(out/'test.vvp')]] if engine=='icarus' else
              [['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_packet','--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    save();code=1
    for label,cmd in zip(['compile','test'],commands):
        row=dict(label=label,command=cmd,started=time.time())
        with (out/(label+'.log')).open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);row['pid']=child.pid;state['actions'].append(row);save();code=child.wait()
        row.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    m=re.search(r'^FULL_PROCESSOR_PACKET_PASS (.+)$',log,re.M)
    passed=code==0 and m is not None and m[1]==s['summary']
    if passed:
        clocks=re.findall(r'^PACKET_CLOCK channel=(\d+) edges=(\d+)$',log,re.M)
        assert len(clocks)==100 and clocks==re.findall(r'^PACKET_CLOCK channel=(\d+) edges=(\d+)$',(source/'test.log').read_text(),re.M)
        assert 'PACKET_GEOMETRY_REJECTIONS cases=403' in log
        coverage=re.search(r'^CACHED_PACKET_COVERAGE (.+)$',log,re.M)[1];assert coverage==s['cache_coverage']
        state.update(summary=m[1],cache_coverage=coverage,all100_clock_counts_match=True)
    state.update(status='PASS_FULL_PROCESSOR_SHARED_CAPTURE' if passed else 'TERMINAL_SHARED_CAPTURE_NOT_ESTABLISHED',finished=time.time())
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],default='verilator')
    p.add_argument('--disabled',action='store_true')
    p.add_argument('--diagnostic',action='store_true')
    p.add_argument('--mutation',choices=['wrong-start'])
    p.add_argument('--snapshot',action='store_true')
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.engine,not a.disabled,a.diagnostic,a.mutation,a.snapshot))
