#!/usr/bin/env python3
"""Real MSA address prediction, original guest state/pins/raw operations and restore.

The original reference retains all 2690 bits. Candidate actual physical fields
are 1650 bits and are all compared to the explicit reference abstraction.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import generate_msa_compact as gen
import test_msa_board as base

HERE=Path(__file__).resolve().parent


def physical_fields():
    removed=set(gen.compact.generate()[1]['removed_fields'])
    return [r for r in base.physical_fields() if not (r['path'].startswith('u_storage_') and r['path'].split('.')[-1] in removed)]


def fixture(manifest):
    _,old,_=gen.base.generate();top,_,metadata=base.fixture(old)
    top=top.replace('[2689:0] dut_state,reference_state','[1649:0] dut_state,reference_state')
    top,count=re.subn(r'^assign (?:dut|reference)_state\[\d+\+:\d+\]=[^;]+;\n','',top,flags=re.M)
    assert count==916
    physical=physical_fields();canonical={};offset=0
    for row in physical:
        path=row['path'];width=row['width'];canonical[path]=(offset,width)
        ref=path
        if path.startswith('u_storage_'):
            if path.endswith('.cache_valid'):ref=path.removesuffix('.cache_valid')+'.hit'
            if path.endswith('.cache_data'):ref=path.removesuffix('.cache_data')+'.read_value'
        code=f'assign dut_state[{offset}+:{width}]=dut.{path};\nassign reference_state[{offset}+:{width}]=reference.{ref};\n'
        top=gen.base.once(top,'endmodule',code+'endmodule');offset+=width
    assert offset==1650
    mapping=[None]*1650
    for row in manifest['layout']:
        start,width=canonical[row['path']];assert width==row['width']
        for b in range(width):assert mapping[row['lsb']+b] is None;mapping[row['lsb']+b]=start+b
    assert set(mapping)==set(range(1650))
    header=f'constexpr unsigned STIM_BITS={metadata["inputs"]},STATE_BITS=1650,STATE_WORDS=52,SCAN_WORDS=92;\n'
    header+='constexpr unsigned canonical_bit[1650]={'+','.join(map(str,mapping))+'};\n'
    metadata.update(physical=physical,canonical_bit=mapping)
    return top,header,metadata


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    board,manifest,helpers=gen.generate();top,header,metadata=fixture(manifest)
    for p in (Path(__file__),Path(gen.__file__),Path(gen.compact.__file__),Path(base.__file__),Path(gen.base.__file__),HERE/'test_msa_board.cpp'):
        (inputs/p.name).write_bytes(p.read_bytes())
    (inputs/'msa_ddram.v').write_bytes(gen.base.original(gen.base.SOURCE))
    (inputs/'dorado_storage_group.sv').write_bytes(gen.compact.original.original_source())
    originals=gen.base.original_cells()
    for name,raw in originals.items():(inputs/name).write_bytes(raw)
    cpp=(HERE/'test_msa_board.cpp').read_text().replace('d.state_index=149;','d.state_index=91;')
    cpp=cpp.replace('{150u,255u,65535u,0xffffffu}','{92u,149u,150u,255u,65535u,0xffffffu}')
    cpp=cpp.replace('physical=2690 fields=458','physical=1650 fields=378').replace('words=150 last=8','words=92 last=12')
    cpp=cpp.replace('MSA BOARD PASS','COMPACT MSA PASS')
    (inputs/'test_msa_compact.cpp').write_text(cpp)
    cases=[('candidate',None,None,None,None)]
    if mutations:cases += [
        ('lost_prediction','cell_MK4096P_addr.sv','assign addr_will_change=((~p4 & ras_d) && (row != a)) || ((~p15 & cas_d) && (col != a));',"assign addr_will_change=1'b0;",'ordinary physical state differs'),
        ('inverted_group_prediction','msa_board.sv','.addr(dram_addr_0),.addr_will_change(dram_change_0),','.addr(dram_addr_0),.addr_will_change(~dram_change_1),','ordinary physical state differs'),
        ('strobe_instead_of_change','cell_MK4096P_addr.sv','((~p4 & ras_d) && (row != a)) || ((~p15 & cas_d) && (col != a))','(~p4 & ras_d) || (~p15 & cas_d)','ordinary physical state differs'),
        ('early_write_commit','storage_group.sv','if(req_write)write_done<=1;','if(req_write)begin write_done<=1;cache_data<=req_data;end','ordinary physical state differs'),
        ('lost_global_hold','msa_board.sv','wire state_hold=state_global_hold ||','wire state_hold=','held raw request/response leak'),
        ('broken_frontend_link','msa_board.sv','.state_serial_in(state_storage_serial[1])',".state_serial_in(1'b0)",'read rotation changed physical state'),
        ('lost_last_word','ff_scan_controller.sv','offset < 18','offset < 11','read word differs'),
    ]
    results=[]
    for label,target,before,after,why in cases:
        case=out/label;case.mkdir();assets={'msa_board.sv':board,**helpers}
        if target:assert assets[target].count(before)==1,(label,before);assets[target]=assets[target].replace(before,after,1)
        for name,source in assets.items():(case/name).write_text(source)
        (case/'fixture.sv').write_text(top);(case/'parameters.h').write_text(header)
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','msa_board_fixture','-Wno-fatal','--assert','--Mdir',str(case/'obj'),
                 '-CFLAGS','-I'+str(case),str(case/'fixture.sv'),*[str(case/n)for n in assets],str(inputs/'msa_ddram.v'),
                 str(inputs/'dorado_storage_group.sv'),*[str(inputs/n)for n in originals],str(inputs/'test_msa_compact.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vmsa_board_fixture')],capture_output=True,text=True,timeout=180)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text.strip(),flush=True)
        if why:assert result.returncode and why in text,(label,text)
        else:assert result.returncode==0 and 'COMPACT MSA PASS' in text,text
        results.append(dict(case=label,returncode=result.returncode,expected_failure=why))
    for name,value in [('manifest',manifest),('fixture-metadata',metadata),('results',dict(status='PASS',scope=__doc__,cases=results,
        executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},
        limits='One complete MSA; new native catalog/transport, full-core fit, real service barriers and FPGA speed remain unqualified.'))]:
        (out/(name+'.json')).write_text(json.dumps(value,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true')
    a=p.parse_args();run(a.output.resolve(),a.mutations)
