#!/usr/bin/env python3
"""Replay actual native storage/address clocks in both simulators.

Keep the existing native group/address fixture's state and output comparisons,
random traffic, delayed host replies, reset and coverage assertions. Compare
the staged original group against its CE variant and the original address
cell against the CE preview. Exercise both widths and half-cycle address modes.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]


def run(stage,out):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    group=stage/'src/fpga/mister/dorado_storage_group.sv'
    address=stage/'src/verilog/cells/cell_MK4096P_addr.v'
    preview=stage/'src/fpga/mister/dorado_storage_addr_preview.sv'
    assert address.exists()and preview.exists()
    original=(ROOT/'verilog/verilator/tb_storage_ready_prediction.sv').read_text();candidate=original
    changes=[
      ('parameter integer WIDTH=12','parameter integer WIDTH=12, HOST_ADDR_SAMPLE=0'),
      ('reference_storage_group #(.ADDR_BITS(16),.WIDTH(WIDTH)) gold(',
       'dorado_storage_group #(.ADDR_BITS(16),.WIDTH(WIDTH),.HOST_ADDR_SAMPLE(HOST_ADDR_SAMPLE)) gold('),
      ('.host_clk(clk),.machine_clk(machine_clk),.reset(reset),.ras_n(ras_n)',
       '.host_clk(clk),.machine_clk(machine_clk),.reset(reset),.machine_step(run),.addr_after_edge(after_addr),.ras_n(ras_n)'),
      ('dorado_storage_group #(.ADDR_BITS(16),.WIDTH(WIDTH)) dut(',
       'dorado_storage_group_ce #(.ADDR_BITS(16),.WIDTH(WIDTH),.HOST_ADDR_SAMPLE(HOST_ADDR_SAMPLE)) dut('),
      ('.host_clk(clk),.machine_clk(machine_clk),.reset(reset),.machine_step(run),.addr_after_edge(after_addr),.ras_n(ras_n),.cas_n(cas_n),.we_n(we_n),.addr(dut_addr)',
       '.guest_advance(run),.host_clk(clk),.machine_clk(clk),.reset(reset),.machine_step(run),.addr_after_edge(after_addr),.ras_n(ras_n),.cas_n(cas_n),.we_n(we_n),.addr(dut_addr)'),
      ('cell_MK4096P_addr_preview #(.ROW_BITS(8)) address_dut(.sys_clk(machine_clk)',
       'cell_MK4096P_addr_preview_ce #(.ROW_BITS(8)) address_dut(.guest_advance(run),.sys_clk(clk)')]
    for a,b in changes:
        assert candidate.count(a)==1,a
        candidate=candidate.replace(a,b,1)
    extra='''
  if({gold.ready_q,gold.addr_before_host,gold.preserve_old_word}!==
     {dut.ready_q,dut.addr_before_host,dut.preserve_old_word})
   $fatal(1,"CLOCK_ENABLE_STORAGE host preparation state differs");
'''
    marker='  #1;checks=checks+1;';assert candidate.count(marker)==1
    candidate=candidate.replace(marker,marker+extra,1)
    (out/'original-fixture.sv').write_text(original);(out/'candidate.sv').write_text(candidate)
    for p in [group,address,preview,Path(__file__)]:shutil.copy2(p,out/p.name)
    state=dict(status='RUNNING',scope=__doc__,stage=str(stage),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        original_fixture_sha256=sha(ROOT/'verilog/verilator/tb_storage_ready_prediction.sv'),original_state_output_and_coverage_assertions_retained=True,
        started=time.time(),actions=[],cases=[],hardware_changed=False,full_machine_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    def execute(label,cmd):
        with(out/(label+'.log')).open('x')as log:
            p=subprocess.Popen(cmd,cwd=stage/'src',stdout=log,stderr=log)
            a=dict(label=label,pid=p.pid,command=cmd,started=time.time());state['actions'].append(a);save();code=p.wait()
        a.update(returncode=code,finished=time.time());save();return code
    good=True
    for engine in ['icarus','verilator']:
        for width in [8,12]:
            for sample in [0,1]:
                label=f'{engine}-w{width}-sample{sample}';tb=out/'candidate.sv'
                sources=list(map(str,[group,address,preview,tb]))
                if engine=='icarus':
                    target=out/(label+'.vvp')
                    compile=['iverilog','-g2012','-s','tb_storage_ready_prediction','-Ptb_storage_ready_prediction.WIDTH='+str(width),'-Ptb_storage_ready_prediction.HOST_ADDR_SAMPLE='+str(sample),'-o',str(target),*sources]
                    test=['vvp',str(target)]
                else:
                    obj=out/('obj-'+label)
                    compile=['verilator','--binary','--timing','--top-module','tb_storage_ready_prediction','-GWIDTH='+str(width),'-GHOST_ADDR_SAMPLE='+str(sample),'-j','2','-Wno-fatal','--Mdir',str(obj),'-o','test',*sources]
                    test=[str(obj/'test')]
                code=execute(label+'-compile',compile)
                if not code:code=execute(label+'-test',test)
                log=(out/(label+'-test.log')).read_text()if(out/(label+'-test.log')).exists()else''
                matches=re.findall(r'^STORAGE_READY_GROUP PASS.*$',log,re.M)
                row=dict(label=label,engine=engine,width=width,host_address_sample=sample,returncode=code,passed=code==0 and len(matches)==1,metrics=matches)
                state['cases'].append(row);save();good=good and row['passed']
    state.update(status='PASS_ACTUAL_NATIVE_COMMON_CLOCK_STORAGE_AND_ADDRESS'if good else'TERMINAL_NATIVE_CLOCK_ENABLE_STORAGE_NOT_ESTABLISHED',
        finished=time.time(),files={p.name:sha(p)for p in out.iterdir()if p.is_file()and p.name!='run-state.json'and p.suffix!='.vvp'})
    save();print(state['status'],flush=True);return 0 if good else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.output.resolve()))
