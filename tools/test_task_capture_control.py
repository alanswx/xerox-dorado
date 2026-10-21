#!/usr/bin/env python3
"""Check reset/maintenance/stop/hold transitions of the task capture epoch slice.

Reuse the independent circuit and full 88-bit state comparator from the frozen
primary qualification. All producer changes settle with the common root HIGH;
simultaneous asynchronous CurrentTask set/reset remains excluded. This adds
control-state coverage, not native firmware or 60ns board qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import sys


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


CONTROL = '''
   // Exercise the original reset, task maintenance, masked StopTasks and
   // separate processor/memory Hold seams. Each producer settles HIGH.
   for(integer round_no=0;round_no<8;round_no=round_no+1) begin
     if(!before_clocks) pin_event(1,selected);
     selected[8:0]=9'h178;selected[0]=1;selected[7]=1;selected[70]=1;selected[74:71]=0;
     selected[68]=0;selected[69]=0;selected[65]=0;
     pin_event(1,selected);
     for(integer edge_no=0;edge_no<16;edge_no=edge_no+1) pin_event(!before_clocks,selected);
     if(!before_clocks) pin_event(1,selected);
     selected[7]=0;pin_event(1,selected);
     for(integer edge_no=0;edge_no<16;edge_no=edge_no+1) pin_event(!before_clocks,selected);
     if(!before_clocks) pin_event(1,selected);
     selected[70]=0;selected[74:71]=4'(round_no);pin_event(1,selected);
     selected[74:71]=0;pin_event(1,selected);
     selected[0]=0;pin_event(1,selected);
     for(integer edge_no=0;edge_no<64;edge_no=edge_no+1) pin_event(!before_clocks,selected);
     if(!before_clocks) pin_event(1,selected);
     selected[65]=1;pin_event(1,selected);
     for(integer edge_no=0;edge_no<16;edge_no=edge_no+1) pin_event(!before_clocks,selected);
     if(!before_clocks) pin_event(1,selected);
     selected[65]=0;pin_event(1,selected);
     selected[68]=1;selected[69]=1;selected[34:20]=15'h1<<round_no;pin_event(1,selected);
     for(integer edge_no=0;edge_no<32;edge_no=edge_no+1) pin_event(!before_clocks,selected);
     if(!before_clocks) pin_event(1,selected);
     selected[69]=0;pin_event(1,selected);
     for(integer edge_no=0;edge_no<16;edge_no=edge_no+1) pin_event(!before_clocks,selected);
     if(!before_clocks) pin_event(1,selected);
     selected[68]=0;pin_event(1,selected);
     for(integer edge_no=0;edge_no<32;edge_no=edge_no+1) pin_event(!before_clocks,selected);
   end

'''


def run(qualified, output):
    primary=json.loads((qualified/'results.json').read_text())
    assert primary['status']=='PASS_TASK_CAPTURE_ARRIVAL_CONTRACT'
    output.mkdir(parents=True,exist_ok=False)
    inputs=output/'executed-inputs'
    shutil.copytree(qualified/'executed-inputs',inputs)
    for name,digest in primary['inputs'].items():assert sha(inputs/name)==digest,name
    own=inputs/'tools/test_task_capture_control.py';own.write_bytes(Path(__file__).read_bytes())
    hashes={**primary['inputs'],'tools/test_task_capture_control.py':sha(own)}
    name='verilog/verilator/tb_task_capture_control.sv'
    bench=(inputs/'verilog/verilator/tb_task_capture_step.sv').read_text()
    bench=bench.replace('module tb_task_capture_step;','module tb_task_capture_control;')
    # Keep the original comparator and its clock-edge monitors. End the
    # generated initial block immediately after this directed workload.
    bench=bench[:bench.index('    reg [75:0] selected;')]+"""
    reg [75:0] selected;
    initial begin
      for(integer i=0;i<25;i=i+1) totals[i]=0;
      seed(7);selected=SEED_PINS;selected[19:0]=20'h178;
      pin_event(1,selected);
"""+CONTROL+"""
      $display("TASK_CAPTURE_CONTROL PASS checks=%0d held=%0d yield=%0d coincident_next=%0d address_changes=%0d",checks,held_task_checks,yield_checks,coincident_next,address_changes);
      for(integer i=0;i<25;i=i+1) $display("TASK_CAPTURE_CONTROL_CLOCK channel=%0d edges=%0d",i,totals[i]);
      $finish;
    end
endmodule
`default_nettype wire
"""
    (inputs/name).write_text(bench);hashes[name]=sha(inputs/name)
    rtl=[inputs/'verilog/rtl/dorado_task_capture_step.sv']
    actions=[];profiles=[];baseline=None
    def execute(cmd,log):
        with log.open('w') as stream:
            code=subprocess.run(list(map(str,cmd)),cwd=inputs,stdout=stream,stderr=subprocess.STDOUT).returncode
        actions.append(dict(command=list(map(str,cmd)),returncode=code,log=str(log)))
        assert code==0,log
    for row in primary['profiles']:
        profile=row[0];directory=output/profile;directory.mkdir()
        timed=directory/'timed-reference';shutil.copytree(qualified/profile/'timed-reference',timed)
        timed_hashes={p.name:sha(p)for p in timed.iterdir()if p.suffix in ('.sv','.v')}
        defines=[]
        support=[*rtl,inputs/name,*sorted(timed.glob('*.sv')),*sorted(timed.glob('*.v'))]
        binary=directory/'test.vvp'
        execute(['iverilog','-g2012','-s','tb_task_capture_control',*defines,'-o',binary,*support],directory/'compile.log')
        execute(['vvp',binary],directory/'test.log')
        text=(directory/'test.log').read_text()
        summary=re.search(r'^TASK_CAPTURE_CONTROL PASS ([^\n]+)$',text,re.M)[1]
        if baseline is None:baseline=summary
        assert summary==baseline,(profile,summary,baseline)
        print('TASK_CAPTURE_CONTROL',profile,'Icarus',summary,flush=True)
        verilator=directory/'verilator';verilator.mkdir()
        execute(['verilator','--binary','--timing','--top-module','tb_task_capture_control',*defines,
                 '-Wno-fatal','-Wno-PINMISSING','-Wno-TIMESCALEMOD','-Wno-MULTIDRIVEN',
                 '--Mdir',verilator/'obj','-o','test',*support],verilator/'compile.log')
        execute([verilator/'obj/test'],verilator/'test.log')
        assert re.search(r'^TASK_CAPTURE_CONTROL PASS ([^\n]+)$',(verilator/'test.log').read_text(),re.M)[1]==summary
        clocks=re.findall(r'^TASK_CAPTURE_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',text,re.M)
        assert len(clocks)==25 and clocks==re.findall(r'^TASK_CAPTURE_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',(verilator/'test.log').read_text(),re.M)
        for p,digest in timed_hashes.items():assert sha(timed/p)==digest
        profiles.append(dict(profile=profile,summary=summary,clock_counts=clocks,timed_sources=timed_hashes))
        print('TASK_CAPTURE_CONTROL',profile,'Verilator agrees',flush=True)
    for name,digest in hashes.items():assert sha(inputs/name)==digest,name
    record=dict(status='PASS_TASK_CAPTURE_CONTROL_TRANSITIONS',scope=__doc__,inputs=hashes,
                profiles=profiles,actions=actions,summary=baseline,rounds=8,
                root_high_producer_setup=True,all_processor_bits=88,clock_channels=25,
                native_qualified=False,independent_skew_qualified=False)
    (output/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    print(record['status'],flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--qualified',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();run(args.qualified.resolve(),args.output.resolve())
