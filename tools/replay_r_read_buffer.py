#!/usr/bin/env python3
"""Exercise the registered read boundary in the complete actual native caller.

Keep the original fixtures, references and every architectural assertion.
Only host preparation clocks are added; their no-publication checks compare
the complete owner, RAM and published input context. Original packet counts,
data/state/clock results and all geometry checks must still match.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from integrate_r_read_buffer import caller, memory
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
PREPARE='''
 // Physical read preparation is an auxiliary host edge, not a guest edge.
 integer read_buffer_edges=0;
 task automatic prepare_physical_read;
  integer edges;reg [9215:0] saved_memory;reg [843:0] saved_owner;
  reg [400:0] saved_pins;reg saved_root;
  begin
   edges=0;saved_memory=memory_image();saved_owner=held_q;
   saved_pins=dut.r_front_before;saved_root=dut.r_front_clock_before;
   while(dut.buffer_ready!==1'b1 && edges<4)begin
    advance=0;#1;host_clk=1;#2;host_clk=0;#2;edges++;
    if(memory_image()!==saved_memory || held_q!==saved_owner ||
       dut.r_front_before!==saved_pins || dut.r_front_clock_before!==saved_root)
     $fatal(1,"R_READ_BUFFER preparation published architectural state record=%0d",idx);
   end
   if(dut.buffer_ready!==1'b1)$fatal(1,"R_READ_BUFFER no matching response record=%0d",idx);
   if(dut.physical_read!==dut.asynchronous_read)$fatal(1,"R_READ_BUFFER wrong physical data record=%0d",idx);
   read_buffer_edges+=edges;
  end
 endtask
 always @(posedge host_clk)begin
  if(owner_commit && !stage_active && !dut.buffer_ready)$fatal(1,"R_READ_BUFFER unready publication");
  if(dut.buffer_ready && dut.physical_read!==dut.asynchronous_read)$fatal(1,"R_READ_BUFFER stale matching response");
 end
 final $display("R_READ_BUFFER_PREPARATION edges=%0d",read_buffer_edges);
'''


def bench(raw,source,out):
    after=raw.replace(str(source),str(out));edits=[]
    def change(a,b):
        nonlocal after
        assert after.count(a)==1,a
        after=after.replace(a,b,1);edits.append((a,b))
    change(' task automatic prepare_entry;',PREPARE+' task automatic prepare_entry;')
    a='   if(dut.packet.entry_ready!==1\'b1)$fatal(1,"CACHED_PACKET entry did not become ready record=%0d",idx);'
    change(a,a+'\n   prepare_physical_read;')
    a='pa=geometry_pa;cb=geometry_cb;ca=geometry_ca;dut.r_front_clock_before=geometry_cb;#2;'
    change(a,a+'prepare_physical_read;#2;')
    restored=after
    for a,b in reversed(edits):restored=restored.replace(b,a,1)
    assert restored.replace(str(out),str(source))==raw
    return after


def outcomes(text):
    return re.findall(r'^(?:PACKET_CLOCK|FULL_PROCESSOR_PACKET_PASS|CP_PACKET_GEOMETRY_REJECTIONS|CP_PACKET_STAGED_READS|NATIVE_R_PACKET_COVERAGE) .*$',text,re.M)


def run(source,proof,out,engine):
    assert not out.exists();out.mkdir(parents=True)
    s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
    assert s['status']=='PASS_ACTUAL_NATIVE_PHASE_GUARD_PROCESSOR_REPLAY'
    assert s['native_cells_j19_single_SRAM_and_commit_barriers_retained']
    r=json.loads((proof/'run-state.json').read_text());verify(proof,r['files'])
    assert r['status']=='PASS_REGISTERED_R_READ_FRONTIER'and r['assumptions']==0
    helper=ROOT/'verilog/rtl/dorado_r_read_buffer.sv';assert sha(helper)==r['buffer_sha256']
    paths=[];rows=[]
    for row in s['source_files']:
        n=row['path'];assert sha(source/n)==row['output_sha256']
        q=out/n;q.parent.mkdir(parents=True,exist_ok=True);raw=(source/n).read_text();text=raw
        if n=='caller.sv':text=caller(raw)
        elif n.endswith('/dorado_r_file_ports.sv'):
            assert sha(source/n)==r['memory_sha256'];text=memory(raw)
        elif n=='tb.sv':text=bench(raw,source,out)
        q.write_text(text);paths.append(q);rows.append(dict(path=n,parent_sha256=row['output_sha256'],output_sha256=sha(q)))
    n='adapters/dorado_r_read_buffer.sv';q=out/n;shutil.copy2(helper,q);paths.append(q)
    rows.append(dict(path=n,parent_sha256=None,output_sha256=sha(q)))
    for n in s['files']:
        if n.startswith('executed-tools/'):continue
        q=out/n
        if q.exists():continue
        q.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(source/n,q)
    for n in [Path(__file__).name,'integrate_r_read_buffer.py']:
        q=out/'executed-tools'/n;q.parent.mkdir(exist_ok=True);shutil.copy2(Path(__file__).with_name(n),q)
    state=dict(status='RUNNING',started=time.time(),scope=__doc__,source=str(source),engine=engine,
        source_state_sha256=sha(source/'run-state.json'),buffer_proof_state_sha256=sha(proof/'run-state.json'),
        original_assertions_and_reference_unchanged=True,extra_host_preparation_edges_are_not_guest_edges=True,
        native_cells_j19_single_SRAM_and_commit_barriers_retained=True,source_files=rows,actions=[],
        files={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    commands=([['iverilog','-g2012','-s','tb_r_front_packet','-o',str(out/'test.vvp'),*map(str,paths)],['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else
              [['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_packet','--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    save();code=1
    for label,cmd in zip(['compile','test'],commands):
        with(out/(label+'.log')).open('x')as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=log);a=dict(label=label,command=cmd,pid=p.pid,started=time.time())
            state['actions'].append(a);save();code=p.wait()
        a.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    passed=code==0 and outcomes(log)==outcomes((source/'test.log').read_text())
    if passed:
        m=re.search(r'^R_READ_BUFFER_PREPARATION edges=(\d+)$',log,re.M);assert m and int(m[1])>0
        state.update(outcomes=outcomes(log),read_buffer_preparation_edges=int(m[1]))
    state.update(status='PASS_REGISTERED_READ_COMPLETE_NATIVE_CALLER'if passed else'TERMINAL_REGISTERED_READ_CALLER_NOT_ESTABLISHED',
        returncode=code,finished=time.time(),original60ns_schedule_qualified=False,FPGA_or_firmware_or_speed_qualified=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','proof','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.proof.resolve(),a.output.resolve(),a.engine))
