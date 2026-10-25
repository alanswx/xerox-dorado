#!/usr/bin/env python3
"""Replay the prepared operand frontier with the complete native packet caller.

Every original reference, assertion and architectural result remains exact.
Only host preparation is added, with full owner/RAM/input no-publication checks.
Require the prepared physical844-bit view to equal the actual raw view at each
publication, including the original paired-path bypass and interior-read case.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from integrate_native_operand_buffer import control,caller,HELPER
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha
from replay_r_read_buffer import outcomes

ROOT=Path(__file__).resolve().parents[1]
PREPARE='''
 integer operand_prepare_edges=0;
 task automatic prepare_operand_view;
  integer edges;reg [9215:0] saved_memory;reg [843:0] saved_owner;
  reg [400:0] saved_pins;reg saved_root;
  begin
   edges=0;saved_memory=memory_image();saved_owner=held_q;
   saved_pins=dut.r_front_before;saved_root=dut.r_front_clock_before;
   while(dut.operand_view_ready!==1'b1 && edges<4)begin
    advance=0;#1;host_clk=1;#2;host_clk=0;#2;edges++;
    if(memory_image()!==saved_memory || held_q!==saved_owner ||
       dut.r_front_before!==saved_pins || dut.r_front_clock_before!==saved_root)
     $fatal(1,"OPERAND_BUFFER preparation published architectural state record=%0d",idx);
   end
   if(dut.operand_view_ready!==1'b1)$fatal(1,"OPERAND_BUFFER no matching view record=%0d",idx);
   if(prepared_physical_state!==first_result[859:16])$fatal(1,"OPERAND_BUFFER wrong ready view record=%0d",idx);
   operand_prepare_edges+=edges;
  end
 endtask
 always @(posedge host_clk)if(ready)begin
  if(prepared_physical_state!==first_result[859:16])$fatal(1,"OPERAND_BUFFER stale published view");
 end
 final $display("OPERAND_BUFFER_PREPARATION edges=%0d",operand_prepare_edges);
'''


def bench(raw,source,out):
    original=raw;after=raw.replace(str(source),str(out));edits=[]
    def change(a,b):
        nonlocal after
        assert after.count(a)==1,a
        after=after.replace(a,b,1);edits.append((a,b))
    a='wire [843:0] held_q;';change(a,'wire [843:0] prepared_physical_state;'+a)
    a='stage_busy,stage_tag_match,owner_commit,native_R_frame);';change(a,a[:-2]+',prepared_physical_state);')
    a=' task automatic prepare_entry;';change(a,PREPARE+a)
    a='   prepare_physical_read;';change(a,a+'prepare_operand_view;')
    a='#2;prepare_physical_read;#2;';change(a,'#2;prepare_physical_read;prepare_operand_view;#2;')
    restored=after
    for a,b in reversed(edits):restored=restored.replace(b,a,1)
    assert restored.replace(str(out),str(source))==original
    return after


def run(source,proof,out,engine):
    assert not out.exists();out.mkdir(parents=True)
    s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
    assert s['status']=='PASS_REGISTERED_READ_COMPLETE_NATIVE_CALLER'and s['original_assertions_and_reference_unchanged']
    p=json.loads((proof/'run-state.json').read_text());verify(proof,p['files'])
    assert p['status']=='PASS_ACTUAL_NATIVE_PREPARED_OPERAND_VIEW'and p['assumptions']==0
    assert p['epoch_host_clock_independence_proved']
    helper=ROOT/HELPER;assert sha(helper)==p['helper_sha256']
    paths=[];rows=[]
    for row in s['source_files']:
        n=row['path'];assert sha(source/n)==row['output_sha256']
        q=out/n;q.parent.mkdir(parents=True,exist_ok=True);raw=(source/n).read_text();text=raw
        if n=='candidate.sv':text=control(raw)
        elif n=='caller.sv':text=caller(raw)
        elif n=='tb.sv':text=bench(raw,source,out)
        q.write_text(text);paths.append(q);rows.append(dict(path=n,parent_sha256=row['output_sha256'],output_sha256=sha(q)))
    n='adapters/dorado_native_operand_buffer.sv';q=out/n;shutil.copy2(helper,q);paths.append(q)
    rows.append(dict(path=n,parent_sha256=None,output_sha256=sha(q)))
    for n in s['files']:
        if n.startswith('executed-tools/'):continue
        q=out/n
        if q.exists():continue
        q.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(source/n,q)
    for n in [Path(__file__).name,'integrate_native_operand_buffer.py']:
        q=out/'executed-tools'/n;q.parent.mkdir(exist_ok=True);shutil.copy2(Path(__file__).with_name(n),q)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),engine=engine,source=str(source),
        source_state_sha256=sha(source/'run-state.json'),operand_proof_state_sha256=sha(proof/'run-state.json'),
        original_assertions_and_reference_unchanged=True,native_cells_j19_single_SRAM_and_commit_barriers_retained=True,
        source_files=rows,actions=[],files={str(q.relative_to(out)):sha(q)for q in out.rglob('*')if q.is_file()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    cmds=([['iverilog','-g2012','-s','tb_r_front_packet','-o',str(out/'test.vvp'),*map(str,paths)],['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else
          [['verilator','--binary','--timing','--top-module','tb_r_front_packet','-Wno-fatal','-j','2','--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    save();code=1
    for label,cmd in zip(['compile','test'],cmds):
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log);a=dict(label=label,pid=child.pid,command=cmd,started=time.time())
            state['actions'].append(a);save();code=child.wait()
        a.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    passed=code==0 and outcomes(log)==outcomes((source/'test.log').read_text())
    if passed:
        m=re.search(r'^OPERAND_BUFFER_PREPARATION edges=(\d+)$',log,re.M)
        prepared_edges=int(m[1])if m else None
        state.update(outcomes=outcomes(log),operand_prepare_edges=prepared_edges,
                     original_results_match=True,known_preparation_coverage=bool(prepared_edges))
        passed=bool(prepared_edges)
    state.update(status='PASS_PREPARED_OPERAND_COMPLETE_NATIVE_CALLER'if passed else'TERMINAL_PREPARED_OPERAND_CALLER_NOT_ESTABLISHED',
        returncode=code,finished=time.time(),original60ns_schedule_or_timing_or_firmware_qualified=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','proof','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.proof.resolve(),a.output.resolve(),a.engine))
