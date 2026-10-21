#!/usr/bin/env python3
"""Original-ROM Initial component test with the actual changed native boards.

Reuse the proven ROM/checksum-cleared F43 fixture and its six parity-source
checks. Replace its three generic boards with byte-exact staged native boards
and the native central storage wiring. Other boards retain the original
component fixture. This is not a complete native BaseBoard or Cedar boot.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from stage_r_front_ready_native import BACKPLANE


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(candidate, initial, inventory, out):
    initial_state=json.loads((initial/'run-state.json').read_text())
    assert initial_state['status']=='PASS_MC1668_INITIAL_COMPONENT'
    prior=json.loads((initial/'provenance.json').read_text())
    for n,h in prior['inputs'].items():assert sha(initial/'executed-inputs'/n)==h,n
    native=json.loads((candidate/'r-front-ready-native.json').read_text())
    for n,h in native['source_hashes'].items():assert sha(candidate/n)==h,n
    inv=json.loads((inventory/'results.json').read_text())
    assert sha(inventory/'results.json')==native['native_inventory_sha256']
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs'
    shutil.copytree(initial/'executed-inputs',inputs)
    # Keep one frozen compilation tree, with exact native source identities.
    additions=[]
    for n in native['source_hashes']:
        if n.startswith('src/fpga/mister/snapshot_native') or n.startswith('src/verilog/rtl/'):
            name=n[4:];p=inputs/name;p.parent.mkdir(parents=True,exist_ok=True)
            p.write_bytes((candidate/n).read_bytes());additions.append(name)
    top=inputs/'verilog/generated/dorado_full.v';body=top.read_text()
    paths={r['board']:r['native_source'][4:]for r in inv['packages']}
    for board in paths:
        pattern=r'\b'+board+r'_m_Rev_m_\w+ b_'+board+r' \('
        matches=list(re.finditer(pattern,body));assert len(matches)==1
        needle=matches[0][0]
        body=body.replace(needle,board+'_full_snapshot_experiment b_'+board+''' (
    .state_host_clk(sys_clk),.state_guest_advance(1'b1),
    .state_freeze(1'b0),.state_guest_held(1'b0),.state_request(1'b0),
    .state_write(1'b0),.state_index('0),.state_data(18'b0),
    .state_frozen(),.state_done(),.state_error(),.state_result(),
    .r_front_q(r_front_q),.r_front_data(r_front_data),.r_front_parity(r_front_parity),
    .r_front_pins(r_front_pins_'''+board+'),.r_front_alias_ok(r_front_alias_'+board+'),')
    bp=(candidate/BACKPLANE).read_text()
    logic=bp.split('  // Physical state only;',1)[1].split('endmodule',1)[0]
    logic='  // Physical state only;'+logic
    # Original component fabric clock never pauses. The actual native board
    # storage equations/wiring are otherwise unchanged from the candidate.
    logic='  wire storage_clk=sys_clk;wire contb_advance=1\'b1;\n'+logic
    assert body.count('endmodule')==2
    top.write_text(body.replace('endmodule',logic+'\nendmodule',1))
    tb=inputs/'verilog/verilator/tb_exec.sv';s=tb.read_text();edits={}
    for row in inv['packages']:
        prefix='m.b_'+row['board']+'.'+row['native_instance'];bits=row['global_Q_bits_lsb_first']
        if row['kind']=='F10145A':
            pattern=re.escape(prefix)+r'\.mem\[([^\]]+)\]'
            if re.search(pattern,s):
                stride=bits[4]-bits[0]
                assert bits==[bits[0]+stride*w+b for w in range(16)for b in range(4)]
                s,n=re.subn(pattern,lambda m:f'm.r_front_q[{bits[0]}+{stride}*({m[1]})+:4]',s)
                edits[prefix+'.mem']=n
        else:
            for name,subset in [('q',bits),('qa',bits[:1]),('qb',bits[1:2])]:
                pattern=re.escape(prefix+'.'+name)+r'\b'
                if not re.search(pattern,s):continue
                assert subset==list(range(subset[0],subset[0]+len(subset)))
                s,n=re.subn(pattern,f'm.r_front_q[{subset[0]}+:{len(subset)}]',s)
                edits[prefix+'.'+name]=n
    for row in inv['RAM_packages']:
        prefix='m.b_'+row['board']+'.'+row['native_instance']+'.mem'
        pattern=re.escape(prefix)+r'\[([^\]]+)\]'
        if not re.search(pattern,s):continue
        word_bits=row['component_word_bits'];assert len(word_bits)==4
        s,n=re.subn(pattern,lambda m:'m.r_front_evaluator.child.child.files.'+row['component_array']+'['+m[1]+f'][{word_bits[0]}+:4]',s)
        edits[prefix]=n
    old='m.b_ProcH.u_i11.a';new='m.b_ProcH.r_front_i11_a'
    assert old in s;s=s.replace(old,new);edits[old]=new
    row=next(r for r in inv['RAM_packages']if r['board']=='ProcL'and r['package']=='j08')
    old='m.b_ProcL.u_j08.a'
    new='{'+','.join('m.b_ProcL.'+row['physical_pin_connections'][str(p)]for p in [1,2,3,4,9,10,11,12])+'}'
    assert old in s;s=s.replace(old,new);edits[old]=new
    # Three removed oversampling histories occur only in optional diagnostic
    # $display payloads. Mark them unavailable; no test/capture input changes.
    for old in ['m.b_ProcH.u_g16.ck_en','m.b_ContA.u_j04.d_pre_a','m.b_ContA.u_j04.cka_en']:
        assert s.count(old)==1
        s=s.replace(old,"1'b0 /* removed history; diagnostic unavailable */");edits[old]='diagnostic unavailable'
    s=s.replace('dpre=%b en=%b FF=', 'removed-dpre=%b removed-en=%b FF=')
    monitor='''
  integer r_front_native_samples=0;
  always @(negedge sys_clk) if($test$plusargs("initial-rom-check"))begin
    r_front_native_samples=r_front_native_samples+1;
    if(r_front_native_samples>1024)begin
      if(!m.r_front_alias_ok)
        $fatal(1,"Actual native Initial live producer aliases disagree sample=%0d",r_front_native_samples);
      if(m.r_front_unsettled || (|m.r_front_repeated))
        $fatal(1,"Actual native Initial slice has unsettled/repeated event sample=%0d unsettled=%b repeated=%h q=%h pins=%h clocks=%b%b",
          r_front_native_samples,m.r_front_unsettled,m.r_front_repeated,m.r_front_q,m.r_front_live,m.r_front_clock_before,m.CLK_ca_p_);
    end
  end
'''
    assert s.count('module tb_exec;')==1
    s=s.replace('module tb_exec;','module tb_exec;'+monitor)
    tb.write_text(s)
    (inputs/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    # Compile exact original fixture support plus native cell definitions and
    # the three actual board wrappers; do not compile their generic originals.
    compile_old=initial_state['actions'][0]['command'];assert compile_old[0]=='verilator'
    command=[];i=0
    while i<len(compile_old):
        arg=compile_old[i]
        if arg=='--Mdir':command += [arg,str(out/'obj')];i+=2;continue
        if arg in ['verilog/generated/'+b+'-Rev-'+rev+'.v'for b,rev in [('ContA','Cd'),('ProcH','Ce'),('ProcL','Ci')]]:
            i+=1;continue
        command.append(arg);i+=1
    command += ['-j','4',*sorted(paths.values())]
    command += sorted(n for n in additions if '/cell_'in n or n.startswith('verilog/rtl/'))
    commands=[command,[str(out/'obj/test'),*initial_state['actions'][1]['command'][1:]]]
    result=dict(scope=__doc__,candidate_sha256=sha(candidate/'r-front-ready-native.json'),
        native_board_hashes={n:sha(inputs/n)for n in paths.values()},fixture_edits=edits,
        original_fixture_sha256=sha(initial/'provenance.json'),commands=commands,
        inputs={str(p.relative_to(inputs)):sha(p)for p in inputs.rglob('*')if p.is_file()})
    (out/'provenance.json').write_text(json.dumps(result,indent=2)+'\n')
    print('Prepared actual native-board original-ROM Initial component',flush=True)


def run(out):
    r=json.loads((out/'provenance.json').read_text());inputs=out/'executed-inputs'
    for n,h in r['inputs'].items():assert sha(inputs/n)==h,n
    path=out/'run-state.json';assert not path.exists(),'Observe existing process'
    state=dict(status='RUNNING',actions=[])
    try:
        for command,label in zip(r['commands'],['compile','test']):
            with(out/(label+'.log')).open('x')as log:
                p=subprocess.Popen(command,cwd=inputs,stdout=log,stderr=subprocess.STDOUT)
                row=dict(stage=label,pid=p.pid,command=command,started=time.time());state['actions'].append(row)
                path.write_text(json.dumps(state,indent=2)+'\n')
                code=p.wait();row.update(returncode=code,finished=time.time());assert code==0,(label,out/(label+'.log'))
        text=(out/'test.log').read_text()
        assert 'INITIAL ROM PASS:'in text and '960 microinstructions written, 960 read back, 0 wrong'in text
        state['status']='PASS_ACTUAL_NATIVE_BOARDS_INITIAL_COMPONENT'
    except BaseException:
        state['status']='TERMINAL_FAILED';raise
    finally:path.write_text(json.dumps(state,indent=2)+'\n')
    print(state['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);g=p.add_mutually_exclusive_group(required=True)
    g.add_argument('--prepare',type=Path);g.add_argument('--run',type=Path)
    for n in ['candidate','initial','inventory']:p.add_argument('--'+n,type=Path)
    a=p.parse_args()
    if a.prepare:
        assert a.candidate and a.initial and a.inventory
        prepare(a.candidate.resolve(),a.initial.resolve(),a.inventory.resolve(),a.prepare.resolve())
    else:run(a.run.resolve())
