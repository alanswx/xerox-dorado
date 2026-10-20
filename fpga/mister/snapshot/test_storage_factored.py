#!/usr/bin/env python3
"""Factored native8/12 frontend versus untouched RTL/DRAM oracle and full scans."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import generate_storage_factored as gen
import test_storage_group_snapshot as base


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    source,manifest=gen.generate()
    for p in (Path(__file__).resolve(),Path(gen.__file__),Path(gen.base.__file__),Path(base.__file__),
              base.HERE/'test_storage_group_snapshot.cpp',gen.base.ROOT/'verilog/cells/cell_MK4096P_6.v'):
        (inputs/p.name).write_bytes(p.read_bytes())
    (inputs/'dorado_storage_group.sv').write_bytes(gen.base.original_source())
    cases=[('candidate',None,None,None)]
    if mutations:cases+=[
        ('lost_hold','if(state_hold)begin',"if(1'b0)begin",'physical state differs'),
        ('lost_guest_enable','if(state_guest_advance) begin',"if(1'b1) begin",'physical state differs'),
        ('held_request_leak','assign req_valid=normal_req_valid && !state_hold;','assign req_valid=normal_req_valid;','held raw request/response leak'),
        ('held_response_leak','assign rsp_ready=waiting && !state_hold;','assign rsp_ready=waiting;','held raw request/response leak'),
        ('false_drain','assign state_drain_ready=normal_ready && !normal_req_valid;','assign state_drain_ready=!waiting;','drain boundary differs'),
        ('lost_old_word','read_pipe_next=read_value[WIDTH-1:0]','read_pipe_next=cache_data[WIDTH-1:0]','physical state differs'),
        ('lost_saved_address','saved_addr_next=addr','saved_addr_next=0','physical state differs'),
        ('serial_history_loss','state_bits={old_data,saved_data,cache_data,saved_addr,cache_addr,saved_write,waiting,write_done,cache_valid,data_out,read_pipe,write_data,write_pending,we_d,cas_d}',
         "state_bits={old_data,saved_data,cache_data,saved_addr,cache_addr,saved_write,waiting,write_done,cache_valid,data_out,read_pipe,write_data,write_pending,2'b0}",'serial read order differs'),
        ('scan_when_running','if(state_hold)begin','if(state_hold || state_scan_step)begin','physical state differs'),
        ('lost_done_enable','if(update_enable[7]) write_done<=write_done_next;',"if(1'b0) write_done<=write_done_next;",'physical state differs'),
        ('lost_scan_cache_enable','if(update_enable[12]) cache_data<=cache_data_next;',"if(update_enable[12] && !state_hold) cache_data<=cache_data_next;",'serial physical oracle differs'),
    ]
    results=[]
    for label,before,after,why in cases:
        case=out/label;case.mkdir();candidate=source
        if before:
            assert candidate.count(before)==1,(label,before);candidate=candidate.replace(before,after,1)
        (case/'storage_group_snapshot.sv').write_text(candidate);(case/'fixture.sv').write_text(base.fixture())
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','storage_snapshot_fixture','-Wno-fatal','--assert',
                 '--Mdir',str(case/'obj'),str(case/'fixture.sv'),str(case/'storage_group_snapshot.sv'),
                 str(inputs/'dorado_storage_group.sv'),str(inputs/'cell_MK4096P_6.v'),str(inputs/'test_storage_group_snapshot.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vstorage_snapshot_fixture')],capture_output=True,text=True,timeout=120)
        log=result.stdout+result.stderr;(case/'test.txt').write_text(log);print(label+': '+log.strip(),flush=True)
        if why:assert result.returncode and why in log,(label,log)
        else:assert result.returncode==0 and 'STORAGE SNAPSHOT PASS'in log,log
        results.append(dict(case=label,returncode=result.returncode,expected_failure=why))
    result=dict(status='PASS',scope=__doc__,manifest=manifest,cases=results,
                executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},
                limits='Frontend component only; no full MSA/native barrier, legal whole-core fit, hardware or speed claim.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
