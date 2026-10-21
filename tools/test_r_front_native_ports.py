#!/usr/bin/env python3
"""Compare the integrated R files against the preserved native RAM packages.

Simulation-only shadows observe the actual native address/data/write pins.
They drive no target net. Compare all 9216 bits after both write windows have
closed for two fabric samples, with original error enables active. This checks
the native integration's write epochs, not a Cedar boot or full-speed timing.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import time

ROOT=Path(__file__).resolve().parents[1]


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(parent,native,inventory,out,cycles):
    record=json.loads(gzip.decompress((parent/'provenance.json.gz').read_bytes()))
    frozen=parent/'executed-inputs'
    for n,h in record['inputs'].items():assert sha(frozen/n)==h,n
    candidate=json.loads((native/'r-front-ready-native.json').read_text())
    inv=json.loads((inventory/'results.json').read_text())
    assert sha(inventory/'results.json')==candidate['native_inventory_sha256']
    assert len(inv['RAM_packages'])==12 and inv['RAM_bits']==9216
    for row in inv['RAM_packages']:
        n=row['native_source'];assert sha(frozen/n[4:])==candidate['source_hashes'][n],n
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs'
    shutil.copytree(frozen,inputs)
    lines=["  reg native_port_shadow_fault=0;",
           '  initial native_port_shadow_fault=$test$plusargs("native-port-shadow-fault");']
    names=[]
    for row in inv['RAM_packages']:
        board,pkg,kind=row['board'],row['package'],row['kind']
        name='native_shadow_'+board+'_'+pkg;names.append(name)
        pins={int(k):v for k,v in row['physical_pin_connections'].items()}
        width=len(row['component_word_bits']);assert width in (1,4)
        outputs=[15] if width==1 else [20,21,22,23]
        def net(v):return 'm.b_'+board+'.'+v
        lines += [f'  wire [{width-1}:0] {name}_out;',
                  f'  cell_{kind}_snapshot {name}(',
                  "    .sys_clk(sys_clk),.state_host_clk(sys_clk),.state_advance(1'b1),",
                  "    .state_freeze(1'b0),.state_guest_held(1'b0),.state_request(1'b0),",
                  f"    .state_write(1'b0),.state_index(9'b0),.state_data({width}'b0),",
                  '    .state_frozen(),.state_done(),.state_error(),.state_result(),']
        conn=[]
        for pin,v in sorted(pins.items()):
            if pin in outputs:
                expression=f'{name}_out[{outputs.index(pin)}]'
            else:
                expression=net(v)
                if board=='ProcH' and pkg=='h06' and pin==19:
                    expression='('+expression+' ^ native_port_shadow_fault)'
            conn.append(f'    .p{pin}({expression})')
        lines += [',\n'.join(conn),'  );']
        adr=[1,2,3,4,9,10,11,12] if width==1 else list(range(7,15))
        address='{'+','.join(net(pins[p]) for p in adr)+'}'
        old_write='!'+net(pins[14])+' && !'+net(pins[7]) if width==1 else (
            '!'+net(pins[2])+' && !('+ '|'.join(net(pins[p])for p in (3,4,5,6))+')')
        array=row['component_array'];bank={'rm_h':0,'stk_h':1,'rm_l':2,'stk_l':3}[array]
        half=0 if board=='ProcH' else 1
        files='m.r_front_evaluator.child.child.files'
        bit=row['component_word_bits'][0]
        actual=f'{files}.{array}[word_no][{bit}+:{width}]'
        lines += [f'  wire {name}_old_write=({old_write});',
                  f'  wire {name}_new_write=!{files}.write_n[{half}] && !{files}.select_n[{bank}];',
                  f'  reg {name}_sample=0,{name}_previous=0,{name}_dirty=1;',
                  f'  integer {name}_checks=0,{name}_windows=0;',
                  f'  reg [7:0] {name}_last_old=0,{name}_last_new=0;',
                  '  always @(posedge sys_clk)begin',
                  f'    {name}_previous<={name}_sample;',
                  f'    {name}_sample<={name}_old_write|{name}_new_write;',
                  f'    if({name}_old_write){name}_last_old<={address};',
                  f'    if({name}_new_write){name}_last_new<={files}.address[{8*bank}+:8];',
                  '  end',
                  '  always @(negedge sys_clk)begin',
                  f'    if({name}_sample){name}_dirty=1;',
                  f'    if(paritysources_arm && parity_enables==6\'b111111 && {name}_dirty && !{name}_sample && !{name}_previous)begin',
                  f'      {name}_windows++;',
                  '      for(integer word_no=0;word_no<256;word_no++)begin',
                  f'        {name}_checks++;',
                  f'        if({actual} !== {name}.mem[word_no])',
                  f'          $fatal(1,"NATIVE_R_PORT_MISMATCH {board}/{pkg} bank={array} bits={bit}+{width} sample=%0d CIA=%h word=%02h actual=%h original=%h last_old=%h last_new=%h",',
                  f'            n_cyc2,ring_cia,word_no[7:0],{actual},{name}.mem[word_no],{name}_last_old,{name}_last_new);',
                  '      end',
                  f'      {name}_dirty=0;',
                  '    end',
                  '  end',
                  '  final begin',
                  f'    if({name}_checks<256)$error("Native port coverage missing: {board}/{pkg}");',
                  f'    $display("NATIVE_R_PORT_CHECK {board}/{pkg} checks=%0d windows=%0d",{name}_checks,{name}_windows);',
                  '  end']
    tb=inputs/'verilog/verilator/tb_exec.sv';body=tb.read_text()
    assert body.count('module tb_exec;')==1
    body=body.replace(str(parent/'cycle-limit.txt'),str(out/'cycle-limit.txt'))
    body=body.replace('module tb_exec;','module tb_exec;\n'+'\n'.join(lines)+'\n')
    tb.write_text(body);(inputs/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state=json.loads((parent/'run-state.json').read_text())
    compile_command=list(state['actions'][0]['command'])
    compile_command[compile_command.index('--Mdir')+1]=str(out/'obj')
    args=[str(out/'obj/test')]+[arg for arg in state['actions'][1]['command'][1:]
          if not arg.startswith(('+cycles=','+cycle-limit-file=','+loaded-eb=','+inspect-state=','+inspect-at='))
          and arg not in ('+check-cedar-mapwrites','+fastwait')]
    args += ['+cycles='+str(cycles)]
    (out/'cycle-limit.txt').write_text(str(cycles)+'\n')
    r=dict(scope=__doc__,parent_provenance_sha256=sha(parent/'provenance.json.gz'),
           inventory_sha256=sha(inventory/'results.json'),commands=[compile_command,args],
           shadow_packages=names,physical_RAM_bits=9216,external_inputs=record['external_inputs'],
           inputs={str(p.relative_to(inputs)):sha(p)for p in inputs.rglob('*')if p.is_file()})
    (out/'provenance.json.gz').write_bytes(gzip.compress((json.dumps(r,indent=2)+'\n').encode(),mtime=0))
    print('Prepared12 simulation-only physical RAM shadows',flush=True)


def run(out):
    r=json.loads(gzip.decompress((out/'provenance.json.gz').read_bytes()));inputs=out/'executed-inputs'
    for n,h in r['inputs'].items():assert sha(inputs/n)==h,n
    for n,h in r['external_inputs'].items():assert sha(ROOT/n)==h,n
    path=out/'run-state.json';assert not path.exists(),'Observe existing run'
    state=dict(status='RUNNING',actions=[])
    try:
        for cmd,label in zip(r['commands'],['compile','positive']):
            with(out/(label+'.log')).open('x')as stream:
                proc=subprocess.Popen(cmd,cwd=inputs,stdout=stream,stderr=subprocess.STDOUT)
                a=dict(stage=label,pid=proc.pid,command=cmd,started=time.time());state['actions'].append(a)
                path.write_text(json.dumps(state,indent=2)+'\n');code=proc.wait()
                a.update(returncode=code,finished=time.time());assert code==0,(label,out/(label+'.log'))
        text=(out/'positive.log').read_text()
        assert text.count('NATIVE_R_PORT_CHECK ')==12
        state['status']='PASS_STABLE_NATIVE_R_PORT_EPOCHS_COMPONENT_ONLY'
    except BaseException:
        state['status']='TERMINAL_FAILED';raise
    finally:path.write_text(json.dumps(state,indent=2)+'\n')
    print(state['status'],flush=True)


def negative(out):
    """Flip a shadow-only data pin and require the expected monitor failure."""
    r=json.loads(gzip.decompress((out/'provenance.json.gz').read_bytes()))
    inputs=out/'executed-inputs'
    for n,h in r['inputs'].items():assert sha(inputs/n)==h,n
    for n,h in r['external_inputs'].items():assert sha(ROOT/n)==h,n
    positive=json.loads((out/'run-state.json').read_text())
    assert positive['status']=='PASS_STABLE_NATIVE_R_PORT_EPOCHS_COMPONENT_ONLY'
    path=out/'negative-state.json';assert not path.exists(),'Observe existing negative run'
    cmd=r['commands'][1]+['+native-port-shadow-fault']
    state=dict(status='RUNNING',command=cmd,binary_sha256=sha(Path(cmd[0])))
    try:
        with(out/'negative.log').open('x')as stream:
            proc=subprocess.Popen(cmd,cwd=inputs,stdout=stream,stderr=subprocess.STDOUT)
            state.update(pid=proc.pid,started=time.time())
            path.write_text(json.dumps(state,indent=2)+'\n')
            state.update(returncode=proc.wait(),finished=time.time())
        log=(out/'negative.log').read_text()
        assert state['returncode']!=0,'Shadow-only fault escaped'
        assert 'NATIVE_R_PORT_MISMATCH ProcH/h06 bank=rm_h' in log,log[-1500:]
        state['status']='PASS_REJECTED_SHADOW_ONLY_DATA_PIN_FAULT'
    except BaseException:
        state['status']='TERMINAL_FAILED';raise
    finally:path.write_text(json.dumps(state,indent=2)+'\n')
    print(state['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);g=p.add_mutually_exclusive_group(required=True)
    g.add_argument('--prepare',type=Path);g.add_argument('--run',type=Path)
    g.add_argument('--negative',type=Path)
    for n in ['parent','native','inventory']:p.add_argument('--'+n,type=Path)
    p.add_argument('--cycles',type=int,default=16000000);a=p.parse_args()
    if a.prepare:
        assert a.parent and a.native and a.inventory
        prepare(a.parent.resolve(),a.native.resolve(),a.inventory.resolve(),a.prepare.resolve(),a.cycles)
    elif a.negative:negative(a.negative.resolve())
    else:run(a.run.resolve())
