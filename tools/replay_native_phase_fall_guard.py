#!/usr/bin/env python3
"""Replay the complete corrected native caller with only its fall guard changed.

Preserve the actual16 visible-R cells, j19 epochs, original2470-bit subevents,
all844 owner bits/9216 SRAM bits/100 clocks and the original hold/read/write/
geometry checks. The bench changes only paths and passive guard-use counters.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from qualify_native_phase_fall_guard import transform,OLD_MASK,MASK
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def run(source,proof,out,engine,wrong_frame=False,task_proof=None,processor_proof=None,address_proof=None,parallel_proof=None,ready_proof=None):
    assert not out.exists();out.mkdir(parents=True)
    s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
    assert s['status']=='PASS_COMBINED_CP_COHERENT_PACKETS'and s['experimental_j19_ordered_epochs']
    assert s['actual_native_visible_R_cells']and not s.get('mutation')and not s.get('native_cell_mutation')
    p=json.loads((proof/'run-state.json').read_text());verify(proof,p['files'])
    assert p['status']=='PASS_ACTUAL_NATIVE_PHASE_FALL_GUARD'and len(p['actions'])==6
    assert all(r['passed']for r in p['actions'])
    assert sha(source/'candidate.sv')==p['source_control_sha256']
    after=(proof/'native-control-after.sv').read_text()
    assert after==transform((source/'candidate.sv').read_text(),sparse_frames=p.get('sparse_shared_capture_frames',False))
    task_after=None
    if task_proof:
        from qualify_native_task_terminal import transform as task_transform
        t=json.loads((task_proof/'run-state.json').read_text());verify(task_proof,t['files'])
        assert t['status']=='PASS_ACTUAL_NATIVE_TASK_TERMINAL_CLOSURE'and all(a['passed']for a in t['actions'])
        task_before=(source/'adapters/dorado_local_task_capture_step.sv').read_text()
        assert sha(source/'adapters/dorado_local_task_capture_step.sv')==t['source_task_sha256']
        task_after=task_transform(task_before)
        assert task_after==(task_proof/'native-task-after.sv').read_text()
    if parallel_proof:
        from qualify_native_task_parallel import transform as parallel_transform
        assert task_proof and task_after is not None
        t=json.loads((parallel_proof/'run-state.json').read_text());verify(parallel_proof,t['files'])
        assert t['status']=='PASS_ACTUAL_NATIVE_TASK_PARALLEL_CAPTURE'and all(a['passed']for a in t['actions'])
        assert sha(task_proof/'native-task-after.sv')==t['source_task_sha256']
        task_after=parallel_transform(task_after)
        assert task_after==(parallel_proof/'native-task-after.sv').read_text()
    processor_after=None
    if processor_proof:
        from qualify_native_processor_terminal import transform as processor_transform
        t=json.loads((processor_proof/'run-state.json').read_text());verify(processor_proof,t['files'])
        assert t['status']=='PASS_ACTUAL_NATIVE_PROCESSOR_TERMINAL_CLOSURE'and all(a['passed']for a in t['actions'])
        path=source/'inputs/verilog/rtl/dorado_processor_capture_step.sv'
        assert sha(path)==t['source_sha256']
        processor_after=processor_transform(path.read_text())
        assert processor_after==(processor_proof/'native-processor-after.sv').read_text()
    address_after=None
    if address_proof:
        from qualify_native_address_task_owner import transform as address_transform
        t=json.loads((address_proof/'run-state.json').read_text());verify(address_proof,t['files'])
        assert t['status']=='PASS_ACTUAL_NATIVE_SHARED_ADDRESS_TASK_OWNER'and all(a['passed']for a in t['actions'])
        path=source/'adapters/dorado_local_r_address_step.sv'
        assert sha(path)==t['source_address_sha256']
        address_after=address_transform(path.read_text())
        assert address_after==(address_proof/'native-address-after.sv').read_text()
    ready_after=None;ready_helper=None
    if ready_proof:
        from qualify_native_ready_compare import control_transform,child_transform,HELPER
        assert parallel_proof and not wrong_frame
        t=json.loads((ready_proof/'run-state.json').read_text());verify(ready_proof,t['files'])
        assert t['status']=='PASS_ACTUAL_NATIVE_READY_COMPARISON'and all(a['passed']for a in t['actions'])
        assert sha(proof/'native-control-after.sv')==t['source_control_sha256']
        path=source/'adapters/dorado_local_r_front_ready_step.sv';assert sha(path)==t['source_ready_sha256']
        after=control_transform(after);ready_after=child_transform(path.read_text())
        assert after==(ready_proof/'native-control-after.sv').read_text()
        assert ready_after==(ready_proof/'native-ready-after.sv').read_text()
        ready_helper=Path(__file__).resolve().parents[1]/HELPER
        assert sha(ready_helper)==t['helper_sha256']
    if wrong_frame:
        assert p['sparse_shared_capture_frames']
        token='fall_before=state_q[47:0],fall_after='
        assert after.count(token)==1
        after=after.replace(token,"fall_before=state_q[47:0]^(48'b1<<23),fall_after=",1)
    paths=[];rows=[]
    for row in s['source_files']:
        n=row['path'];assert sha(source/n)==row['output_sha256']
        q=out/n;q.parent.mkdir(parents=True,exist_ok=True);text=(source/n).read_text()
        if n=='candidate.sv':text=after
        elif n=='adapters/dorado_local_task_capture_step.sv'and task_after is not None:text=task_after
        elif n=='inputs/verilog/rtl/dorado_processor_capture_step.sv'and processor_after is not None:text=processor_after
        elif n=='adapters/dorado_local_r_address_step.sv'and address_after is not None:text=address_after
        elif n=='adapters/dorado_local_r_front_ready_step.sv'and ready_after is not None:text=ready_after
        elif n=='tb.sv':
            original=text;text=text.replace(str(source),str(out))
            scope='dut.packet.single.control.direct_arrival'
            monitor=f'''
 integer native_single_sparse=0,native_single_expanded=0;
 always @(posedge host_clk)if(owner_commit && !use_pair && {scope}.using_sparse)begin
  native_single_sparse++;
  if(|(({scope}.pins_before ^ {scope}.pins_after)&398'h{OLD_MASK^MASK:x}))native_single_expanded++;
 end
 final $display("NATIVE_PHASE_FALL_COVERAGE singles=%0d expanded=%0d",native_single_sparse,native_single_expanded);
'''
            assert text.count('endmodule')==1;text=text.replace('endmodule',monitor+'endmodule',1)
            assert text.replace(monitor,'',1).replace(str(out),str(source))==original
        q.write_text(text);paths.append(q);rows.append(dict(path=n,parent_sha256=row['output_sha256'],output_sha256=sha(q)))
    if ready_helper:
        n='adapters/dorado_ready_compare_step.sv';q=out/n;shutil.copy2(ready_helper,q);paths.append(q)
        rows.append(dict(path=n,parent_sha256=None,output_sha256=sha(q)))
    # Freeze non-compiled fixture files, including the recovered mixed-case
    # initial owner. Other trace paths remain the original immutable inputs.
    for n in s['files']:
        target=out/n
        if target.exists():continue
        target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(source/n,target)
    helpers=out/'executed-tools';helpers.mkdir()
    for n in [Path(__file__).name,'qualify_native_phase_fall_guard.py',*(['qualify_native_task_terminal.py']if task_proof else[]),
              *(['qualify_native_processor_terminal.py']if processor_proof else[]),
              *(['qualify_native_address_task_owner.py','prove_native_address_task_owner.py']if address_proof else[]),
              *(['qualify_native_task_parallel.py']if parallel_proof else[]),
              *(['qualify_native_ready_compare.py']if ready_proof else[])]:
        shutil.copy2(Path(__file__).with_name(n),helpers/n)
    state=dict(status='RUNNING',scope=__doc__,source=str(source),engine=engine,started=time.time(),
        proof=str(proof),proof_state_sha256=sha(proof/'run-state.json'),source_state_sha256=sha(source/'run-state.json'),
        original_trace_sha256=s['source_trace_sha256'],original_initial_sha256=s['source_initial_sha256'],
        original_assertions_and_reference_unchanged=True,
        only_functional_change=('native shared fall-stability support102->85; direct sparse frames'if p.get('sparse_shared_capture_frames')else'native shared fall-stability support102->85')+
                              ('; shared task consumer16->7 waves and proved terminal closure'if task_proof else'')+
                              ('; separate processor phase16->5 waves and proved terminal closure'if processor_proof else'')+
                              ('; one shared child task result supplies address/operand consumers'if address_proof else'')+
                              ('; separate task clock resolution and parallel last-capture selection'if parallel_proof else'')+
                              ('; parallel Ready capture comparisons with every original mismatch bit retained'if ready_proof else''),
        task_terminal_proof=str(task_proof)if task_proof else None,
        task_terminal_proof_state_sha256=sha(task_proof/'run-state.json')if task_proof else None,
        processor_terminal_proof=str(processor_proof)if processor_proof else None,
        processor_terminal_proof_state_sha256=sha(processor_proof/'run-state.json')if processor_proof else None,
        address_task_owner_proof=str(address_proof)if address_proof else None,
        address_task_owner_proof_state_sha256=sha(address_proof/'run-state.json')if address_proof else None,
        task_parallel_proof=str(parallel_proof)if parallel_proof else None,
        task_parallel_proof_state_sha256=sha(parallel_proof/'run-state.json')if parallel_proof else None,
        ready_compare_proof=str(ready_proof)if ready_proof else None,
        ready_compare_proof_state_sha256=sha(ready_proof/'run-state.json')if ready_proof else None,
        sparse_shared_capture_frames=p.get('sparse_shared_capture_frames',False),wrong_shared_start_frame=wrong_frame,
        native_cells_j19_single_SRAM_and_commit_barriers_retained=True,source_files=rows,actions=[],
        files={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    commands=([['iverilog','-g2012','-s','tb_r_front_packet','-o',str(out/'test.vvp'),*map(str,paths)],['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else
              [['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_packet','--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    save();code=1
    for label,cmd in zip(['compile','test'],commands):
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log);a=dict(label=label,command=cmd,pid=child.pid,started=time.time())
            state['actions'].append(a);save();code=child.wait()
        a.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    def outcomes(text):
        return re.findall(r'^(?:PACKET_CLOCK|FULL_PROCESSOR_PACKET_PASS|CP_PACKET_GEOMETRY_REJECTIONS|CP_PACKET_STAGED_READS|CACHED_PACKET_COVERAGE|NATIVE_R_PACKET_COVERAGE) .*$',text,re.M)
    passed=code==0 and outcomes(log)==outcomes((source/'test.log').read_text())
    if wrong_frame:
        passed=state['actions'][0]['returncode']==0 and code!=0 and 'PACKET_DIFF'in log
    if passed and not wrong_frame:
        assert len(re.findall(r'^PACKET_CLOCK channel=',log,re.M))==100
        state['outcomes']=outcomes(log)
        match=re.search(r'^NATIVE_PHASE_FALL_COVERAGE singles=(\d+) expanded=(\d+)$',log,re.M);assert match
        state['native_single_sparse_commits']=int(match[1]);state['expanded_single_commits']=int(match[2])
    state.update(status=('PASS_NATIVE_PHASE_FRAME_COUNTEREXAMPLE'if wrong_frame else'PASS_ACTUAL_NATIVE_PHASE_GUARD_PROCESSOR_REPLAY')if passed else'TERMINAL_NATIVE_PHASE_GUARD_REPLAY_NOT_ESTABLISHED',returncode=code,finished=time.time())
    save();print(state['status']);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','proof','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True)
    p.add_argument('--wrong-frame',action='store_true')
    p.add_argument('--task-proof',type=Path)
    p.add_argument('--processor-proof',type=Path)
    p.add_argument('--address-proof',type=Path)
    p.add_argument('--parallel-proof',type=Path)
    p.add_argument('--ready-proof',type=Path)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.proof.resolve(),a.output.resolve(),a.engine,a.wrong_frame,
                                        a.task_proof.resolve()if a.task_proof else None,
                                        a.processor_proof.resolve()if a.processor_proof else None,
                                        a.address_proof.resolve()if a.address_proof else None,
                                        a.parallel_proof.resolve()if a.parallel_proof else None,
                                        a.ready_proof.resolve()if a.ready_proof else None))
