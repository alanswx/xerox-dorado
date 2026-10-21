#!/usr/bin/env python3
"""Observe native CPU pins with preserved cell models, without driving the target.

This is an open-loop capture diagnostic at the current component-fixture clock.
Compare only after input pins and both outputs settle for the recorded number
of samples (three by default); exclude
active RAM writes. A difference identifies a capture epoch to inspect, not a
physical-timing qualification or permission to bypass original firmware checks.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from stage_r_front_ready_native import q_outputs

ROOT=Path(__file__).resolve().parents[1]


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(parent,native,inventory,out,cycles,settle_samples=3):
    assert 1<=settle_samples<=3
    record=json.loads(gzip.decompress((parent/'provenance.json.gz').read_bytes()))
    frozen=parent/'executed-inputs'
    for name,digest in record['inputs'].items():assert sha(frozen/name)==digest,name
    candidate=json.loads((native/'r-front-ready-native.json').read_text())
    assert sha(inventory/'results.json')==candidate['native_inventory_sha256']
    inv=json.loads((inventory/'results.json').read_text())
    assert len(inv['packages'])==91 and inv['physical_Q_bits']==752
    assert 'r_front_evaluator' in (frozen/'verilog/generated/dorado_full.v').read_text()
    for row in inv['packages']:
        name=row['native_source'];assert sha(frozen/name[4:])==candidate['source_hashes'][name],name
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs'
    shutil.copytree(frozen,inputs)
    lines=[]
    for row in inv['packages']:
        board,pkg,kind=row['board'],row['package'],row['kind']
        prefix='native_q_'+board+'_'+pkg;bits=row['global_Q_bits_lsb_first'];width=len(bits)
        pins={int(k):v for k,v in row['physical_pin_connections'].items()}
        ram=kind=='F10145A'
        outputs={2:0,1:1,15:2,14:3}if ram else q_outputs(kind,[str(i)for i in range(width)])
        def net(name):return 'm.b_'+board+'.'+name
        vec='{'+','.join(net(v)for p,v in sorted(pins.items())if p not in outputs)+'}'
        pin_count=sum(p not in outputs for p in pins)
        qexpr='{'+','.join('m.r_front_q['+str(b)+']'for b in reversed(bits))+'}'
        lines += [f'  wire [{width-1}:0] {prefix}_old,{prefix}_new={qexpr};',
                  f'  cell_{kind}_snapshot {row["native_parameters"]} {prefix}(',
                  "    .sys_clk(sys_clk),.state_host_clk(sys_clk),.state_advance(1'b1),"]
        if ram:
            lines += ["    .state_freeze(1'b0),.state_guest_held(1'b0),.state_request(1'b0),",
                      "    .state_write(1'b0),.state_index(5'b0),.state_data(10'b0),",
                      '    .state_frozen(),.state_done(),.state_error(),.state_result(),']
        else:
            lines += ["    .state_hold(1'b0),.state_write(1'b0),.state_data(18'b0),.state_result(),"]
        conn=[]
        for pin in sorted(set(pins)|set(outputs)):
            if pin in outputs:
                value=str(outputs[pin])
                expression='' if ram or value.startswith('~')else f'{prefix}_old[{value}]'
            else:expression=net(pins[pin])
            conn.append(f'    .p{pin}({expression})')
        lines += [',\n'.join(conn),'  );']
        if ram:
            lines += [f'  for(genvar qword=0;qword<16;qword++)begin:{prefix}_words',
                      f'    assign {prefix}_old[4*qword+:4]={prefix}.mem[qword];','  end']
        condition=' && '+net(pins[13])if ram else ''
        lines += [f'  wire [{pin_count-1}:0] {prefix}_pins={vec};',
                  f'  reg [{pin_count-1}:0] {prefix}_last_pins;',
                  f'  reg [{width-1}:0] {prefix}_last_old,{prefix}_last_new;',
                  f'  integer {prefix}_stable=0,{prefix}_checks=0;',
                  '  always @(negedge sys_clk)begin',
                  f'    if({prefix}_pins==={prefix}_last_pins && {prefix}_old==={prefix}_last_old && {prefix}_new==={prefix}_last_new)',
                  f'      {prefix}_stable={prefix}_stable<{settle_samples+1} ? {prefix}_stable+1:{settle_samples+1};',
                  f'    else {prefix}_stable=0;',
                  f'    if(paritysources_arm && parity_enables==6\'b111111 && {prefix}_stable=={settle_samples}{condition})begin',
                  f'      {prefix}_checks++;',
                  f'      if({prefix}_new!=={prefix}_old)$fatal(1,"NATIVE_Q_CAPTURE_MISMATCH {board}/{pkg} {kind} sample=%0d CIA=%h task=%h new=%h old=%h pins=%b",',
                  f'        n_cyc2,ring_cia,pct_ctask,{prefix}_new,{prefix}_old,{prefix}_pins);',
                  '    end',
                  f'    {prefix}_last_pins={prefix}_pins;{prefix}_last_old={prefix}_old;{prefix}_last_new={prefix}_new;',
                  '  end',
                  f'  final $display("NATIVE_Q_CAPTURE_CHECK {board}/{pkg} checks=%0d",{prefix}_checks);']
    bench=inputs/'verilog/verilator/tb_exec.sv';body=bench.read_text()
    assert body.count('module tb_exec;')==1
    body=body.replace(str(parent/'cycle-limit.txt'),str(out/'cycle-limit.txt'))
    bench.write_text(body.replace('module tb_exec;','module tb_exec;\n'+'\n'.join(lines)+'\n'))
    (inputs/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    (inputs/'stage_r_front_ready_native.py').write_bytes((ROOT/'tools/stage_r_front_ready_native.py').read_bytes())
    state=json.loads((parent/'run-state.json').read_text());compile_command=list(state['actions'][0]['command'])
    compile_command[compile_command.index('--Mdir')+1]=str(out/'obj')
    args=[str(out/'obj/test')]+[a for a in state['actions'][1]['command'][1:]
          if not a.startswith(('+cycles=','+cycle-limit-file=','+loaded-eb=','+inspect-state=','+inspect-at='))
          and a not in('+check-cedar-mapwrites','+fastwait')]
    assert '"pcfrom=%d"'in body and '"pclen=%d"'in body
    args+=['+cycles='+str(cycles),'+pcfrom=48495000','+pclen=600000',
           '+inspect-state='+str(out/'register-map-inspection.txt'),'+inspect-at=49900000']
    (out/'cycle-limit.txt').write_text(str(cycles)+'\n')
    result=dict(scope=__doc__,commands=[compile_command,args],parent_provenance_sha256=sha(parent/'provenance.json.gz'),
        inventory_sha256=sha(inventory/'results.json'),external_inputs=record['external_inputs'],
        physical_Q_bits=752,shadow_packages=91,settle_samples=settle_samples,
        inputs={str(p.relative_to(inputs)):sha(p)for p in inputs.rglob('*')if p.is_file()})
    (out/'provenance.json.gz').write_bytes(gzip.compress((json.dumps(result,indent=2)+'\n').encode(),mtime=0))
    print('Prepared91 preserved native capture shadows; no target drivers',flush=True)


def run(out):
    r=json.loads(gzip.decompress((out/'provenance.json.gz').read_bytes()));inputs=out/'executed-inputs'
    for n,h in r['inputs'].items():assert sha(inputs/n)==h,n
    for n,h in r['external_inputs'].items():assert sha(ROOT/n)==h,n
    path=out/'run-state.json';assert not path.exists(),'Observe existing run'
    state=dict(status='RUNNING',actions=[])
    try:
        for command,label in zip(r['commands'],['compile','test']):
            with(out/(label+'.log')).open('x')as stream:
                p=subprocess.Popen(command,cwd=inputs,stdout=stream,stderr=subprocess.STDOUT)
                action=dict(stage=label,pid=p.pid,command=command,started=time.time());state['actions'].append(action)
                path.write_text(json.dumps(state,indent=2)+'\n');code=p.wait()
                action.update(returncode=code,finished=time.time())
                assert code==0,(label,out/(label+'.log'))
        text=(out/'test.log').read_text()
        checks=re.findall(r'NATIVE_Q_CAPTURE_CHECK (\S+) checks=(\d+)',text)
        assert len(checks)==91 and all(int(n)>0 for _,n in checks),checks
        state['status']='PASS_STABLE_NATIVE_Q_CAPTURE_COMPONENT_ONLY'
    except BaseException:
        state['status']='TERMINAL_FAILED';raise
    finally:path.write_text(json.dumps(state,indent=2)+'\n')
    print(state['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);mode=p.add_mutually_exclusive_group(required=True)
    mode.add_argument('--prepare',type=Path);mode.add_argument('--run',type=Path)
    for name in['parent','native','inventory']:p.add_argument('--'+name,type=Path)
    p.add_argument('--cycles',type=int,default=50000000)
    p.add_argument('--settle-samples',type=int,default=3);a=p.parse_args()
    if a.prepare:
        assert a.parent and a.native and a.inventory
        prepare(a.parent.resolve(),a.native.resolve(),a.inventory.resolve(),a.prepare.resolve(),a.cycles,a.settle_samples)
    else:run(a.run.resolve())
