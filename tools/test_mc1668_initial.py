#!/usr/bin/env python3
"""Run the existing original-ROM Initial component gate in isolated frozen files.

Use Native59's original generated boards/cells with the MC1668 correction.
This enters the original ROM payload at its checksum-cleared F43 debug entry,
using the existing CPU-manifold fixture and ALUFM setup. It is NOT a BaseBoard
firmware boot or Cedar qualification. No saved C machine state is imported.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import time

ROOT=Path(__file__).resolve().parents[1]


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(candidate,out):
    r=json.loads((candidate/'mc1668-candidate.json').read_text())
    for name,h in r['source_hashes'].items():assert sha(candidate/name)==h,name
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs'
    shutil.copytree(candidate/'src',inputs)
    # Native59's original DskEth source has its receive observer's type name.
    # Bind that same type in the component top; leave every board/cell byte
    # and its hierarchy beneath b_DskEth unchanged.
    top=inputs/'verilog/generated/dorado_full.v';body=top.read_text()
    needle='DskEth_m_Rev_m_Cf #'
    assert body.count(needle)==1
    top.write_text(body.replace(needle,'DskEth_m_Rev_m_Cf_rxmonitor #'))
    for name in ['verilog/verilator/tb_exec.sv','verilog/verilator/dorado_pack.sv',
                 'tools/baseboard_rom_im.py','tools/test_mc1668_initial.py',
                 'chm/dorado/doradobaserom.mb!13',
                 'chm/dorado/expanded/bootstrap.dm!20_/Initial.mb','chm/dorado/AEmu.mb!2']:
        dest=inputs/name;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes((ROOT/name).read_bytes())
    fixture=inputs/'fixture';fixture.mkdir();converters={}
    def convert(executable,args,target):
        path=ROOT/'dorado/build'/executable;converters[executable]=sha(path)
        with target.open('w')as stream, (fixture/(executable+'.log')).open('w')as errors:
            subprocess.run([str(path),*args],cwd=inputs,stdout=stream,stderr=errors,check=True)
    convert('baseboard_rom_dump',['chm/dorado/doradobaserom.mb!13'],fixture/'rom.txt')
    convert('im_image',['chm/dorado/expanded/bootstrap.dm!20_/Initial.mb'],fixture/'alu.txt')
    vec=inputs/'verilog/verilator/obj_exec/exec.vec';vec.parent.mkdir(parents=True,exist_ok=True)
    convert('boot0_hunks',['chm/dorado/AEmu.mb!2','-1','4'],vec)
    with(fixture/'im.txt').open('w')as stream:
        subprocess.run(['python3',str(inputs/'tools/baseboard_rom_im.py'),str(fixture/'rom.txt'),
                        '--alu-fixture',str(fixture/'alu.txt')],cwd=inputs,stdout=stream,check=True)
    record=dict(scope=__doc__,candidate_deltas=r['intended_deltas'],sysper=4,cycles=16000000,
        converter_binary_hashes=converters,entry='original Initial F43 debug entry',
        fixture_type_binding='DskEth original rxmonitor type; observer is output-only',
        inputs={str(p.relative_to(inputs)):sha(p)for p in inputs.rglob('*')if p.is_file()})
    (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')
    print('MC1668_INITIAL prepared; component ROM entry only',flush=True)


def run(out):
    r=json.loads((out/'provenance.json').read_text());inputs=out/'executed-inputs'
    for name,h in r['inputs'].items():assert sha(inputs/name)==h,name
    state_path=out/'run-state.json';assert not state_path.exists(),'Observe the existing job'
    actions=[];state=dict(status='RUNNING',actions=actions)
    def execute(command,label):
        print('MC1668_INITIAL',label,flush=True)
        with(out/(label+'.log')).open('x')as log:
            p=subprocess.Popen([str(x)for x in command],cwd=inputs,stdout=log,stderr=subprocess.STDOUT)
            a=dict(stage=label,command=[str(x)for x in command],pid=p.pid,started=time.time());actions.append(a)
            state_path.write_text(json.dumps(state,indent=2)+'\n');code=p.wait();a.update(returncode=code,finished=time.time())
            state_path.write_text(json.dumps(state,indent=2)+'\n')
            if code:raise RuntimeError(label+' failed')
    boards=['ContA-Rev-Cd','ContB-Rev-Cd','ProcH-Rev-Ce','ProcL-Rev-Ci','MemC-Rev-Be',
            'MemD-Rev-Ca','MemX-Rev-Ch','msa','IFU-Rev-Ch','DispY-Rev-Cl','DskEth-Rev-Cf']
    try:
        execute(['verilator','--binary','--timing','--top-module','tb_exec','-Wno-fatal',
            '-Wno-PINMISSING','-Wno-UNOPTFLAT','-Wno-MULTIDRIVEN','-Wno-LATCH',
            '+define+SYSPER_OVERRIDE=4','+define+WORLD','+define+SCREEN','+define+FULL',
            '-Iverilog/cells','-Iverilog/generated','--Mdir',out/'obj','-o','test',
            'verilog/verilator/tb_exec.sv','verilog/generated/dorado_full.v',
            *['verilog/generated/'+b+'.v'for b in boards],
            'verilog/verilator/dorado_pack.sv','verilog/cells/dorado_trident.v',
            'fpga/mister/dorado_receive_monitor.sv'],'compile')
        execute([out/'obj/test','+preload=fixture/im.txt','+start=3907','+cycles=16000000',
            '+mb0','+nostuck','+terminal','+pendulum','+hasram','+ioreset','+fastwait',
            '+imparityon','+keepmirdebug','+cpu-manifold','+paritysources','+initial-rom-check'],'test')
        text=(out/'test.log').read_text();assert 'INITIAL ROM PASS:'in text
        state['status']='PASS_MC1668_INITIAL_COMPONENT';state_path.write_text(json.dumps(state,indent=2)+'\n')
        print(state['status'],flush=True)
    except BaseException:
        state['status']='TERMINAL_FAILED';state_path.write_text(json.dumps(state,indent=2)+'\n');raise


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);a=p.add_mutually_exclusive_group(required=True)
    a.add_argument('--prepare',type=Path);a.add_argument('--run',type=Path);p.add_argument('--candidate',type=Path);args=p.parse_args()
    if args.prepare:
        assert args.candidate;prepare(args.candidate.resolve(),args.prepare.resolve())
    else:run(args.run.resolve())
