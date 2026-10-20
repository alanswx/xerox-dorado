#!/usr/bin/env python3
"""Compact frontend against pinned original RTL and untouched DRAM-cell arrays.

Compare every new physical register with its explicit reference abstraction,
all guest outputs and raw handshakes, plus full scan/save/invert/restore/resume.
The fixture predicts actual address updates; native address logic is separate.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import generate_storage_compact as gen
import test_storage_group_snapshot as base

HERE=Path(__file__).resolve().parent


def fixture():
    top=base.fixture()
    top=top.replace('dorado_storage_group_snapshot','dorado_storage_group_compact')
    for i,width in enumerate((8,12)):
        before=f'.ras_n(ras_n[{i}]),.cas_n(cas_n[{i}]),.we_n(we_n[{i}]),.addr(addr),'
        assert top.count(before)==1
        top=top.replace(before,f'.addr_will_change(next_addr[{i}]!=addr),'+before,1)
        for output,prefix in [('physical','dut'),('original_state','reference')]:
            oldbits=sum(w for _,w in gen.original.fields(width));bits=sum(w for _,w in gen.fields(width))
            before=f" assign {output}[{i}]={{{128-oldbits}'b0,"+'{'+','.join(prefix+'.'+n for n,_ in reversed(gen.original.FIELDS))+'}};\n'
            terms=[prefix+'.'+n for n,_ in reversed(gen.FIELDS)]
            if prefix=='reference':
                terms=[t.replace('reference.cache_data','reference.read_value').replace('reference.cache_valid','reference.hit') for t in terms]
            after=f" assign {output}[{i}]={{{128-bits}'b0,"+'{'+','.join(terms)+'}};\n'
            assert top.count(before)==1;top=top.replace(before,after,1)
    return top


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    source,manifest=gen.generate();top=fixture()
    for p in (Path(__file__),Path(gen.__file__),Path(base.__file__),Path(gen.original.__file__),HERE/'test_storage_group_snapshot.cpp',
              gen.original.ROOT/'verilog/cells/cell_MK4096P_6.v'):
        (inputs/p.name).write_bytes(p.read_bytes())
    (inputs/'dorado_storage_group.sv').write_bytes(gen.original.original_source())
    cpp=(HERE/'test_storage_group_snapshot.cpp').read_text()
    assert cpp.count('bits{111,123}')==1;cpp=cpp.replace('bits{111,123}','bits{46,58}')
    cpp=cpp.replace('physical=111/123','physical=46/58').replace('STORAGE SNAPSHOT PASS','COMPACT STORAGE PASS')
    (inputs/'test_storage_compact.cpp').write_text(cpp)
    cases=[('candidate',None,None,None)]
    if mutations:cases += [
        ('lost_invalidation','if(addr_will_change)cache_valid<=0;',"if(1'b0)cache_valid<=0;",'physical state differs'),
        ('early_write_commit','if(req_write)write_done<=1;','if(req_write)begin write_done<=1;cache_data<=req_data;end','physical state differs'),
        ('lost_reset_commit','if(write_pending && write_done)cache_data<=req_data;',"if(1'b0)cache_data<=req_data;",'physical state differs'),
        ('lost_guest_commit','if(addr_will_change)cache_valid<=0;\n                    if(write_pending && write_done)cache_data<=req_data;',
         'if(addr_will_change)cache_valid<=0;', 'physical state differs'),
        ('wrong_response_kind','if(req_write)write_done<=1;','if(write_pending)write_done<=1;','physical state differs'),
        ('held_request_leak','assign req_valid=normal_req_valid && !state_hold;','assign req_valid=normal_req_valid;','held raw request/response leak'),
        ('scan_when_running','if(state_hold)begin','if(state_hold || state_scan_step)begin','physical state differs'),
        ('lost_guest_enable','if(state_guest_advance)begin',"if(1'b1)begin",'physical state differs'),
    ]
    results=[]
    for label,before,after,why in cases:
        case=out/label;case.mkdir();candidate=source
        if before:
            assert before in candidate;candidate=candidate.replace(before,after,1)
        (case/'storage_group_compact.sv').write_text(candidate);(case/'fixture.sv').write_text(top)
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','storage_snapshot_fixture','-Wno-fatal','--assert',
                 '--Mdir',str(case/'obj'),str(case/'fixture.sv'),str(case/'storage_group_compact.sv'),
                 str(inputs/'dorado_storage_group.sv'),str(inputs/'cell_MK4096P_6.v'),str(inputs/'test_storage_compact.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vstorage_snapshot_fixture')],capture_output=True,text=True,timeout=120)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text.strip(),flush=True)
        if why:assert result.returncode and why in text,(label,text)
        else:assert result.returncode==0 and 'COMPACT STORAGE PASS' in text,text
        results.append(dict(case=label,returncode=result.returncode,expected_failure=why))
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,cases=results,
        executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs.iterdir()},
        limits='Component address predictor in fixture; no actual MSA/native barrier, new complete schema, area/timing or hardware qualification.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true')
    a=p.parse_args();run(a.output.resolve(),a.mutations)
