#!/usr/bin/env python3
"""Check reset/maintenance/stop/hold transitions of the qualified R-memory slice.

Reuse the independent circuit and full-state/RAM comparator from the frozen
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
     if(!cb) event_step(1,p);
     p[8:0]=9'h178;p[0]=1;p[7]=1;p[70]=1;p[74:71]=0;
     p[68]=0;p[69]=0;p[65]=0;
     event_step(1,p);
     for(integer edge_no=0;edge_no<16;edge_no=edge_no+1) event_step(!cb,p);
     if(!cb) event_step(1,p);
     p[7]=0;event_step(1,p);
     for(integer edge_no=0;edge_no<16;edge_no=edge_no+1) event_step(!cb,p);
     if(!cb) event_step(1,p);
     p[70]=0;p[74:71]=4'(round_no);event_step(1,p);
     p[74:71]=0;event_step(1,p);
     p[0]=0;event_step(1,p);
     for(integer edge_no=0;edge_no<64;edge_no=edge_no+1) event_step(!cb,p);
     if(!cb) event_step(1,p);
     p[65]=1;event_step(1,p);
     for(integer edge_no=0;edge_no<16;edge_no=edge_no+1) event_step(!cb,p);
     if(!cb) event_step(1,p);
     p[65]=0;event_step(1,p);
     p[68]=1;p[69]=1;p[34:20]=15'h1<<round_no;event_step(1,p);
     for(integer edge_no=0;edge_no<32;edge_no=edge_no+1) event_step(!cb,p);
     if(!cb) event_step(1,p);
     p[69]=0;event_step(1,p);
     for(integer edge_no=0;edge_no<16;edge_no=edge_no+1) event_step(!cb,p);
     if(!cb) event_step(1,p);
     p[68]=0;event_step(1,p);
     for(integer edge_no=0;edge_no<32;edge_no=edge_no+1) event_step(!cb,p);
   end
   iterations=0;
'''


def run(qualified, output):
    primary=json.loads((qualified/'results.json').read_text())
    assert primary['status']=='PASS_R_MEMORY_ARRIVAL_CONTRACT'
    output.mkdir(parents=True,exist_ok=False)
    inputs=output/'executed-inputs'
    shutil.copytree(qualified/'executed-inputs',inputs)
    for name,digest in primary['inputs'].items():assert sha(inputs/name)==digest,name
    own=inputs/'tools/test_r_memory_control.py';own.write_bytes(Path(__file__).read_bytes())
    hashes={**primary['inputs'],'tools/test_r_memory_control.py':sha(own)}
    name='verilog/verilator/tb_r_memory_control.sv'
    bench=(inputs/'verilog/verilator/tb_r_memory_step.sv').read_text()
    assert bench.count('for(integer i=0;i<iterations;i=i+1)')==1
    bench=bench.replace('module tb_r_memory_step;','module tb_r_memory_control;')
    # End this initial block after its directed checks. Verilator can execute
    # following statements in the same coroutine after $finish, so retaining
    # the primary random-workload coverage tail would wrongly fail this run.
    bench=bench[:bench.index('   for(integer i=0;i<iterations;i=i+1)')]+CONTROL+'''
   $display("R_MEMORY_CONTROL PASS checks=%0d writes=%0d reads=%0d old_epoch_changes=%0d",checks,writes,reads,capture_epoch_changes);
   $finish;
 end
endmodule
`default_nettype wire
'''
    (inputs/name).write_text(bench);hashes[name]=sha(inputs/name)
    rtl=[inputs/'verilog/rtl'/x for x in ['dorado_r_memory_step.sv','dorado_r_port_preview.sv',
         'dorado_processor_capture_step.sv','dorado_r_address_step.sv','dorado_r_flags_step.sv',
         'dorado_r_file_ports.sv','dorado_r_operand_step.sv','dorado_t_parity_step.sv',
         'dorado_t_file_step.sv','dorado_task_capture_step.sv']]
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
        defines=['-DPARC_PARITY_TIMING']if row[4]else[]
        support=[*rtl,inputs/name,*sorted(timed.glob('*.sv')),*sorted(timed.glob('*.v'))]
        binary=directory/'test.vvp'
        execute(['iverilog','-g2012','-s','tb_r_memory_control',*defines,'-o',binary,*support],directory/'compile.log')
        execute(['vvp',binary],directory/'test.log')
        text=(directory/'test.log').read_text()
        summary=re.search(r'^R_MEMORY_CONTROL PASS ([^\n]+)$',text,re.M)[1]
        if baseline is None:baseline=summary
        assert summary==baseline,(profile,summary,baseline)
        print('R_MEMORY_CONTROL',profile,'Icarus',summary,flush=True)
        verilator=directory/'verilator';verilator.mkdir()
        execute(['verilator','--binary','--timing','--top-module','tb_r_memory_control',*defines,
                 '-Wno-fatal','-Wno-PINMISSING','-Wno-TIMESCALEMOD','-Wno-MULTIDRIVEN',
                 '--Mdir',verilator/'obj','-o','test',*support],verilator/'compile.log')
        execute([verilator/'obj/test'],verilator/'test.log')
        assert re.search(r'^R_MEMORY_CONTROL PASS ([^\n]+)$',(verilator/'test.log').read_text(),re.M)[1]==summary
        for p,digest in timed_hashes.items():assert sha(timed/p)==digest
        profiles.append(dict(profile=profile,summary=summary,timed_sources=timed_hashes))
        print('R_MEMORY_CONTROL',profile,'Verilator agrees',flush=True)
    for name,digest in hashes.items():assert sha(inputs/name)==digest,name
    record=dict(status='PASS_R_MEMORY_CONTROL_TRANSITIONS',scope=__doc__,inputs=hashes,
                profiles=profiles,actions=actions,summary=baseline,rounds=8,
                root_high_producer_setup=True,all_processor_bits=636,all_R_memory_bits=9216,
                native_qualified=False,independent_skew_qualified=False)
    (output/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    print(record['status'],flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--qualified',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();run(args.qualified.resolve(),args.output.resolve())
