#!/usr/bin/env python3
"""Qualify run/refresh event translation against an extracted original graph.

Binary stable data/control inputs precede root-clock transitions. This is a
component test, not native clock/hold/freeze or analog setup qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
from generate_run_control_reference import COMBINATIONAL,SEQUENTIAL,BOARD
from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]
CANDIDATE='verilog/rtl/dorado_run_control_step.sv'
TEST='verilog/verilator/tb_run_control_step.sv'
FF='verilog/verilator/run_control_original_ff.sv'
MUTATIONS={
    'memory_refresh_follows_cpu': [('if(q[21]) begin n[14]=1;n[15]=1;end',
                                  'if(q[21] && q[7]) begin n[14]=1;n[15]=1;end')],
    'ignore_refresh_set': [('if(q[21]) begin n[14]=1;n[15]=1;end',
                           "if(1'b0) begin n[14]=1;n[15]=1;end")],
    'ignore_memory_stop_clear': [('clear_mem_stop=pins[7] | q[21];','clear_mem_stop=pins[7];')],
    'drop_feedback_clock': [('if(event_mask[1]) n[19]=pins[4];',"if(1'b0) n[19]=pins[4];")],
    'capture_new_drun': [('n[8]=q[5];','n[8]=n[5];')],
    'repeat_clock': [('previous=levels;q=n;','previous=clocks(state_q,clock_before);q=n;')],
    'ignore_stop_reset': [('if(pins[7]) begin',"if(1'b0) begin")],
    'wrong_prestart_polarity': [('!pins[2] | pins[1]','pins[2] | pins[1]'),
                               ('!pins[2] | q[2]','pins[2] | q[2]')],
    'one_wave': [('parameter integer WAVES = 12','parameter integer WAVES = 1')],
    'drop_stop_t1_contributor': [('(pins[3] & pins[0]) |',"1'b0 |")],
}


def execute(command,cwd,log):
    with log.open('w')as f:
        code=subprocess.run([str(x)for x in command],cwd=cwd,stdout=f,stderr=subprocess.STDOUT).returncode
    return dict(command=[str(x)for x in command],cwd=str(cwd),returncode=code,log=str(log))


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs'
    b=load_board(str(ROOT/BOARD));kinds={b.packages[p]['type'] for p in set(SEQUENTIAL)|set(COMBINATIONAL)}
    files=[CANDIDATE,TEST,FF,'tools/generate_run_control_reference.py','tools/sil_netlist.py',
           'tools/test_run_control.py',BOARD,BOARD.replace('.wl','.lc')]+[
           'verilog/cells/cell_'+kind+'.v'for kind in sorted(kinds)]
    hashes={}
    for name in files:
        raw=(ROOT/name).read_bytes();p=inputs/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
        hashes[name]=hashlib.sha256(raw).hexdigest()
    actions=[]
    def checked(command,log):
        row=execute(command,inputs,log);actions.append(row)
        if row['returncode']:raise RuntimeError('Command failed: '+str(log))
    graph=out/'reference'
    checked([sys.executable,inputs/'tools/generate_run_control_reference.py','--output',graph],out/'generation.log')
    manifest=json.loads((graph/'manifest.json').read_text())
    assert manifest['physical_bits']==22 and len(manifest['packages'])==22 and manifest['original_pins']==181
    comb=sorted(set(manifest['combinational_sources']))
    support=[inputs/TEST,inputs/FF,graph/'original_run_control.sv',graph/'run_control_reference_adapter.sv',*[inputs/n for n in comb]]
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
        directory=out/name;directory.mkdir();rtl=directory/'dorado_run_control_step.sv';rtl.write_text(raw)
        binary=directory/'test.vvp'
        checked(['iverilog','-g2012','-s','tb_run_control_step','-o',binary,rtl,*support],directory/'compile.log')
        row=execute(['vvp',binary],inputs,directory/'test.log');actions.append(row)
        text=(directory/'test.log').read_text()
        if name=='original':
            assert not row['returncode'] and 'RUN_CONTROL_STEP PASS checks=66632 'in text,text
            baseline=text.split('RUN_CONTROL_STEP PASS ')[1].splitlines()[0]
        else:
            assert row['returncode'] and re.search(r'FATAL:.*(?:run state mismatch|run event solver failed|run clock count mismatch|manual .*failed|manual memory refresh stopped)',text),(name,text)
        results.append(dict(case=name,returncode=row['returncode'],expected_failure=name!='original'))
    v=out/'verilator';v.mkdir()
    checked(['verilator','--binary','--timing','--top-module','tb_run_control_step',
             '-Wno-fatal','-Wno-PINMISSING','-Wno-TIMESCALEMOD','-Wno-MULTIDRIVEN',
             '--Mdir',v/'obj','-o','test',inputs/CANDIDATE,*support],v/'compile.log')
    checked([v/'obj/test'],v/'test.log')
    vt=(v/'test.log').read_text();assert 'RUN_CONTROL_STEP PASS '+baseline in vt,vt
    for name,digest in hashes.items():
        assert hashlib.sha256((inputs/name).read_bytes()).hexdigest()==digest
    record=dict(status='PASS_ORIGINAL_RUN_REFRESH_COMPONENT',inputs=hashes,reference=manifest,
                cases=results,actions=actions,summary=baseline,physical_bits=22,
                native_integrated=False,hold_freeze_qualified=False,scope=__doc__)
    (out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    print(record['status'],'22 original Qs;66632 events in each simulator;',len(cases)-1,'bad variants rejected')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
