#!/usr/bin/env python3
"""Measure the current-read-port/closed-input-latch844-Q experiment at15ns.

Reuse the previous input staging, registered functional observations and timing
constraints byte-for-byte. The32 latch encoding represents closed storage;
its combinational physical view is not a second bank of architectural flops.
Require BOTH complete balanced original-reference comparisons before fitting.
This remains an isolated budget experiment; no native assembly/load permission.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

import fit_control_clock as fixture

ROOT=Path(__file__).resolve().parents[1]


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(parent,runs,out):
    assert not out.exists() and len(runs)==2
    gates=[];changed={}
    for engine,d in zip(['icarus','verilator'],runs):
        r=json.loads((d/'run-state.json').read_text());log=(d/'test.log').read_text()
        assert r['engine']==engine and r['status']=='PASS_R_FRONT_READ_WINDOWS'
        assert not r.get('mutation') and r['actions'][-1]['returncode']==0
        assert r['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
        assert r['full_state_comparison']=='all844 physical Qs after owner commit'
        clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',log,re.M)
        assert len(clocks)==100 and all(int(n)>0 for c,n in clocks)
        for row in r['source_files']:
            assert sha(d/row['path'])==row['output_sha256']
            if row['changed']:
                key=Path(row['path']).name
                if key in changed:assert sha(changed[key])==row['output_sha256']
                changed[key]=d/row['path']
        assert sha(d/'dorado_r_read_window_step.sv')==sha(ROOT/'verilog/rtl/dorado_r_read_window_step.sv')
        gates.append(dict(directory=str(d),engine=engine,summary=r['summary'],clocks=clocks,run_state_sha256=sha(d/'run-state.json')))
    assert gates[0]['clocks']==gates[1]['clocks'] and len(changed)==4
    source=json.loads((parent/'provenance.json').read_text())
    assert source['clock_period_ns']==15 and source['tested_candidate_sha256']=='ef0abf3eed4497350a1ada19ab8a00c0ddc5732ed81390722844eac1ddb0efea'
    for n,h in source['files'].items():assert sha(parent/n)==h,n
    assert (parent/'clock.sdc').read_text()==fixture.SDC
    out.mkdir(parents=True)
    for p in parent.iterdir():
        if p.suffix in ['.sv','.sdc','.qsf','.qpf','.tcl'] or p.name=='fit_control_clock.py':(out/p.name).write_bytes(p.read_bytes())
    names={'candidate.sv':'dorado_r_front_control_step.sv','dorado_local_r_memory_step.sv':'dorado_local_r_memory_step.sv','dorado_r_file_ports.sv':'dorado_r_file_ports.sv'}
    for old,new in names.items():(out/new).write_bytes(changed[old].read_bytes())
    helper=out/'dorado_r_read_window_step.sv';original=(ROOT/'verilog/rtl'/helper.name).read_text()
    old=' generate for(genvar byte_no=0;';new=' genvar byte_no;\n generate for(byte_no=0;'
    assert original.count(old)==1
    lowered=original.replace(old,new,1);assert lowered.replace(new,old,1)==original
    helper.write_text(lowered)
    # Quartus17's older parser requires a separately declared genvar. Prove
    # the lowering changes only that declaration, and run the exact lowered
    # source against the existing independent original-latch unit in BOTH
    # simulators, rather than silently substituting a synthesis-only helper.
    unit=ROOT/'build/rtl-executive/r-read-window-icarus-v4-20261023'
    tb=out/'unit-proof-tb.sv';tb.write_bytes((unit/'tb.sv').read_bytes())
    oracle=out/'unit-proof-original.sv';oracle.write_bytes((unit/'inputs/verilog/verilator/t_file_original_storage.sv').read_bytes())
    proof=[]
    for engine in ['icarus','verilator']:
        commands=[['iverilog','-g2012','-s','tb_r_read_window','-o',str(out/'unit-proof.vvp'),str(helper),str(tb),str(oracle)],['vvp',str(out/'unit-proof.vvp')]]if engine=='icarus'else[
            ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_read_window','--Mdir',str(out/'unit-proof-obj'),'-o','test',str(helper),str(tb),str(oracle)],[str(out/'unit-proof-obj/test')]]
        actions=[]
        for i,cmd in enumerate(commands):
            log=out/f'unit-proof-{engine}-{i}.log'
            with log.open('w')as stream:rc=subprocess.run(cmd,stdout=stream,stderr=subprocess.STDOUT).returncode
            actions.append(dict(command=cmd,returncode=rc,log=log.name));assert rc==0,(engine,log)
        assert 'R_READ_WINDOW PASS checks=2304 closing_halves=1024 unclocked_updates=768 closed_changes=768'in(out/f'unit-proof-{engine}-1.log').read_text()
        proof.append(dict(engine=engine,actions=actions))
    syntax=dict(original_sha256=hashlib.sha256(original.encode()).hexdigest(),lowered_sha256=sha(helper),
                inverse_byte_identical=True,change='Separate genvar declaration only',original_unit_run=str(unit),comparisons=proof)
    (out/'unit-proof.json').write_text(json.dumps(syntax,indent=2)+'\n')
    qsf=out/'clock_fit.qsf';qsf.write_text(qsf.read_text()+'set_global_assignment -name SYSTEMVERILOG_FILE '+helper.name+'\n')
    top=out/'top.sv';raw=top.read_text()
    old='control_flags);';assert raw.count(old)==1
    raw=raw.replace(old,'control_flags,physical_state);',1)
    marker=' wire [843:0] q;';assert raw.count(marker)==1
    raw=raw.replace(marker,marker+'\n wire [843:0] physical_state;',1);top.write_text(raw)
    for n in [Path(__file__).name,'test_r_front_read_windows.py']:(out/n).write_bytes((ROOT/'tools'/n).read_bytes())
    r=dict(scope=__doc__,component='r_front_control_closed_read_windows',clock_period_ns=15,physical_Q_bits=844,ram_bits=9216,
           input_staging_bits=1650,launch_bits=1650,result_bits=1400,observation_outputs_registered=False,
           observation_only_outputs=['source_pins','fa_n','physical_state'],functional_gates=gates,
           parent_provenance_sha256=sha(parent/'provenance.json'),tested_candidate_sha256=sha(out/'dorado_r_front_control_step.sv'),
           quartus_genvar_syntax=syntax,
           files={p.name:sha(p)for p in out.iterdir()if p.is_file()and p.suffix!='.vvp'},assembly_permitted=False,native_qualified=False,
           full_producer_arrival_contract_qualified=False,all_profiles_qualified=False)
    (out/'provenance.json').write_text(json.dumps(r,indent=2)+'\n');print('PREPARED_READ_WINDOW_NATIVE844_15NS_COMPONENT_BUDGET',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--parent',type=Path,required=True);p.add_argument('--icarus',type=Path,required=True)
    p.add_argument('--verilator',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    prepare(a.parent.resolve(),[a.icarus.resolve(),a.verilator.resolve()],a.output.resolve())
