#!/usr/bin/env python3
"""Test coupled instruction-phase/run/refresh feedback against original pins.

All 32 Qs in fourteen original sequential packages are compared. Binary
external controls settle before root-clock events; internal feedback remains
live. This is not native FPGA or analog setup/hold qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
from generate_control_clock_reference import BOARD,SEQUENTIAL,COMBINATIONAL
from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]
CANDIDATE='verilog/rtl/dorado_control_clock_step.sv'
TEST='verilog/verilator/tb_control_clock_step.sv'
FF='verilog/verilator/run_control_original_ff.sv'
MUTATIONS={
 'ignore_extension':[('(pins[1] | !q[29])',"(1'b0 | !q[29])")],
 'capture_new_phase':[('n[29]=q[25];','n[29]=n[25];')],
 'ignore_phase_initialization':[('if(phase_init) begin',"if(1'b0) begin")],
 'hold_memsh_with_cpu':[('n[18]=!q[18];n[22]=!q[18];','n[18]=!q[18];if(q[7] && !q[6]) n[22]=!q[18];')],
 'ignore_nodispatch_clock':[('instruction_clock | no_dispatch','instruction_clock')],
 'ignore_cram_set':[('if(pins[8]) n[30]=1;',"if(1'b0) n[30]=1;")],
 'wrong_prestart_polarity':[('!q[23] | pins[3]','q[23] | pins[3]'),('!q[23] | q[2]','q[23] | q[2]')],
 'one_wave':[('parameter integer WAVES=16','parameter integer WAVES=1')],
 'memory_refresh_follows_cpu':[('if(q[21]) begin n[14]=1;n[15]=1;end','if(q[21] && q[7]) begin n[14]=1;n[15]=1;end')],
 'lost_nodispatch_edge':[('previous=clocks(q,clock_before,controls_before[0]);','previous=clocks(q,clock_before,controls_after[0]);')],
 'gate_phase_reset_with_clock':[('if(phase_init) begin','if(phase_init && captures[5]) begin')],
 'skip_run_stage':[('n[7]=q[5];','n[7]=n[5];')],
}


def execute(command,cwd,log):
    with log.open('w')as f:
        code=subprocess.run([str(x)for x in command],cwd=cwd,stdout=f,stderr=subprocess.STDOUT).returncode
    return dict(command=[str(x)for x in command],cwd=str(cwd),returncode=code,log=str(log))


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs'
    b=load_board(str(ROOT/BOARD));kinds={b.packages[p]['type']for p in set(SEQUENTIAL)|set(COMBINATIONAL)}
    files=[CANDIDATE,TEST,FF,'tools/generate_control_clock_reference.py',
           'tools/generate_run_control_reference.py','tools/sil_netlist.py','tools/test_control_clock.py',
           BOARD,BOARD.replace('.wl','.lc')]+['verilog/cells/cell_'+k+'.v'for k in sorted(kinds)]
    hashes={}
    for name in files:
        raw=(ROOT/name).read_bytes();p=inputs/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
        hashes[name]=hashlib.sha256(raw).hexdigest()
    actions=[]
    def checked(command,log):
        row=execute(command,inputs,log);actions.append(row)
        if row['returncode']:raise RuntimeError('Command failed: '+str(log))
    graph=out/'reference'
    checked([sys.executable,inputs/'tools/generate_control_clock_reference.py','--output',graph],out/'generation.log')
    manifest=json.loads((graph/'manifest.json').read_text())
    assert manifest['physical_bits']==32 and len(manifest['packages'])==28 and manifest['original_pins']==235
    support=[inputs/TEST,inputs/FF,graph/'control_clock_reference.sv',*[inputs/n for n in manifest['combinational_sources']]]
    source=(inputs/CANDIDATE).read_text();cases=[('original',source)]
    if mutations:
        for name,edits in MUTATIONS.items():
            raw=source
            for before,after in edits:
                assert raw.count(before)==1,(name,before)
                raw=raw.replace(before,after)
            cases.append((name,raw))
    results=[]
    for name,raw in cases:
        directory=out/name;directory.mkdir();rtl=directory/'dorado_control_clock_step.sv';rtl.write_text(raw)
        binary=directory/'test.vvp'
        checked(['iverilog','-g2012','-s','tb_control_clock_step','-o',binary,rtl,*support],directory/'compile.log')
        row=execute(['vvp',binary],inputs,directory/'test.log');actions.append(row)
        text=(directory/'test.log').read_text()
        if name=='original':
            assert not row['returncode'] and 'CONTROL_CLOCK_STEP PASS checks=69864 'in text,text
            baseline=text.split('CONTROL_CLOCK_STEP PASS ')[1].splitlines()[0]
        else:
            assert row['returncode'] and re.search(r'FATAL:.*(?:coupled state mismatch|coupled solver unsettled|coupled clock count mismatch|coupled manual .*failed|coupled manual cadence mismatch)',text),(name,text)
        results.append(dict(case=name,returncode=row['returncode'],expected_failure=name!='original'))
    v=out/'verilator';v.mkdir()
    checked(['verilator','--binary','--timing','--top-module','tb_control_clock_step',
             '-Wno-fatal','-Wno-PINMISSING','-Wno-TIMESCALEMOD','-Wno-MULTIDRIVEN',
             '--Mdir',v/'obj','-o','test',inputs/CANDIDATE,*support],v/'compile.log')
    checked([v/'obj/test'],v/'test.log');text=(v/'test.log').read_text()
    assert 'CONTROL_CLOCK_STEP PASS '+baseline in text,text
    for name,digest in hashes.items():assert hashlib.sha256((inputs/name).read_bytes()).hexdigest()==digest
    record=dict(status='PASS_COUPLED_ORIGINAL_CONTROL_CLOCK_COMPONENT',inputs=hashes,
                reference=manifest,cases=results,actions=actions,summary=baseline,
                physical_bits=32,native_integrated=False,hold_repeat_qualified=False,
                processor_captures_qualified=False,scope=__doc__)
    (out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    print(record['status'],'32 Qs/9 clock channels;69864 events in each simulator;',len(cases)-1,'bad variants rejected')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
