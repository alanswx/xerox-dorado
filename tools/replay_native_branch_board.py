#!/usr/bin/env python3
"""Compare the complete native ProcH board before/after common flag ownership.

Drive identical arbitrary external signals through both real boards, retaining
their actual clock buffers, ALU/data feedback and all unchanged cells. Seed
only the changed storage through the existing board administration port.
Compare every board output plus the complete104-bit flag/history frontier.
This diagnostic is not an original-firmware boot or a60ns phase qualification.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from stage_native_branch_owner import BOARD,transform
from test_external_r_packet import sha


def ports(raw):
    header=re.search(r'^module ProcH_full_snapshot_experiment #\([^\n]+\) \(\n(.*?)^\);',raw,re.M|re.S)[1]
    result=[];direction=None;msb=lsb=0
    for item in header.split(','):
        item=re.sub(r'//[^\n]*','',item).strip()
        m=re.match(r'(input|output)\s+(?:wire|reg)\s*(?:\[(\d+):(\d+)\])?\s*(.*)',item,re.S)
        if m:
            direction=m[1];msb=int(m[2]or 0);lsb=int(m[3]or 0);name=m[4]
        else:name=item
        name=name.split('=')[0].strip();assert re.fullmatch(r'\w+',name),(item,name)
        result.append(dict(name=name,direction=direction,msb=msb,lsb=lsb,bits=msb-lsb+1))
    assert len({p['name']for p in result})==len(result)
    return result


def run(stage,native,out,wrong_clock=False):
    assert not out.exists();out.mkdir(parents=True)
    s=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,s['source_hashes'])
    parent=Path(s['parent_stage']);p=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,p['source_hashes'])
    original=(parent/BOARD).read_text();changed,binding=transform(original)
    assert changed==(stage/BOARD).read_text()and json.loads(json.dumps(binding))==s['actual_native_flag_owner_binding']
    prov=json.loads(gzip.decompress((native/'provenance.json.gz').read_bytes()));verify(native/'inputs',prov['inputs'])
    used=list(prov['used_hdl'])
    for n in s['extra_RTL_sources']:
        if n not in used:used.append(n)
    # Native package PROMs use relative readmem paths below src/. Retain their
    # exact bytes too; equal zero-filled PROMs are not a complete-board gate.
    for n in [n for n in s['source_hashes']if n.startswith('src/')]:
        target=out/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(stage/n,target)
    if wrong_clock:
        # Deliberately bypass only the EXISTING native clock buffer. This is
        # a private negative witness, never a candidate or a timing exception.
        path=out/BOARD;text=path.read_text()
        line=next(line for line in text.splitlines()if 'wire [3:0] native_branch_clocks='in line)
        assert line.count('Clock1_p_Da')==2 and 'Clock1_p_Da__lead'not in line
        path.write_text(text.replace(line,line.replace('Clock1_p_Da','Clock1_p_Da__lead'),1))
    token='module ProcH_full_snapshot_experiment #';assert original.count(token)==1
    (out/'original-board.sv').write_text(original.replace(token,'module ProcH_branch_reference #',1))
    interface=ports(original);assert interface==ports(changed)
    manual={'sys_clk':'clk','state_host_clk':'clk','state_guest_advance':'advance',
        'state_freeze':'freeze','state_guest_held':'guest_held','state_request':'request',
        'state_write':'writing','state_index':'index','state_data':'admin_data'}
    declarations=[];args=[[],[]];output_names=[[],[]];input_bits=output_bits=0
    for port in interface:
        n=port['name'];width=port['bits']
        if port['direction']=='input':
            expression=manual.get(n,f'stimulus[{input_bits}+:{width}]')
            if n not in manual:input_bits+=width
            for a in args:a.append('.'+n+'('+expression+')')
        else:
            for side in range(2):
                name=('old_'if side==0 else'new_')+n
                declarations.append(f' wire [{port["msb"]}:{port["lsb"]}] {name};')
                args[side].append('.'+n+'('+name+')');output_names[side].append(name)
            output_bits+=width
    tb=f'''`timescale 1ns/1ps
module tb_branch_board;
 reg clk=0,advance=0,freeze=1,guest_held=1,request=0,writing=0;
 reg [15:0] index=0;reg [17:0] admin_data=0;
 reg [{input_bits-1}:0] stimulus=0;reg [31:0] rng=32'hd04ad061;
 {chr(10).join(declarations)}
 ProcH_branch_reference #(.SYSPER(4)) original({','.join(args[0])});
 ProcH_full_snapshot_experiment #(.SYSPER(4)) candidate({','.join(args[1])});
 wire [{output_bits-1}:0] expected={{{','.join(output_names[0])}}};
 wire [{output_bits-1}:0] actual={{{','.join(output_names[1])}}};
 wire [103:0] old_flags={{original.u_h11.ck_d,original.u_h11.d_pre,original.u_g11.ck_d,original.u_g11.d_pre,
  original.u_e13.ce_lat,original.u_e13.d_lat,original.u_e13.a_lat,original.u_e13.we_d,old_alu_physical[923:844]}};
 wire [103:0] new_flags=candidate.native_branch_physical;
 reg [79:0] previous_flags;integer checks=0,guest_checks=0;
 integer changed_file=0,changed_f17=0,changed_g11=0,changed_h11=0;
 task automatic tick(input integer count_changes);
  begin
   #2;clk=1;#2;
   if({1 if wrong_clock else 0} && new_flags[79:0]!==old_flags[79:0])begin
    $fatal(1,"BRANCH_BOARD_WRONG_CLOCK_PHYSICAL_Q check=%0d old=%h new=%h",checks,old_flags[79:0],new_flags[79:0]);
   end
   if(!{1 if wrong_clock else 0} && (actual!==expected || new_flags!==old_flags))begin
    for(integer k=0;k<{output_bits};k++)if(actual[k]!==expected[k])$display("BRANCH_BOARD_OUTPUT_DIFF check=%0d bit=%0d actual=%b expected=%b",checks,k,actual[k],expected[k]);
    $fatal(1,"BRANCH_BOARD mismatch check=%0d old_flags=%h new_flags=%h",checks,old_flags,new_flags);
   end
   if(count_changes)begin
    guest_checks++;
    if(new_flags[63:0]!=previous_flags[63:0])changed_file++;
    if(new_flags[67:64]!=previous_flags[67:64])changed_f17++;
    if(new_flags[73:68]!=previous_flags[73:68])changed_g11++;
    if(new_flags[79:74]!=previous_flags[79:74])changed_h11++;
   end
   previous_flags=new_flags[79:0];checks++;clk=0;#2;
  end
 endtask
 initial begin
  tick(0);tick(0);request=1;writing=1;
  for(integer row=0;row<=16;row++)begin
   index=16'((64<<9)|row);admin_data=row==16 ? 18'h255:18'(row*7+3);tick(0);
  end
  index=16'(26<<9);admin_data=18'hc;tick(0);
  index=16'(32<<9);admin_data=18'h1234;tick(0);
  index=16'(40<<9);admin_data=18'hac;tick(0);
  request=0;writing=0;freeze=0;guest_held=0;tick(0);advance=1;
  for(integer trial=0;trial<1024;trial++)begin
   for(integer k=0;k<{input_bits};k++)begin rng=rng^(rng<<13);rng=rng^(rng>>17);rng=rng^(rng<<5);stimulus[k]=rng[0];end
   advance=trial%17!=0;
   for(integer beat=0;beat<8;beat++)tick(1);
  end
  if(!changed_file || !changed_f17 || !changed_g11 || !changed_h11)$fatal(1,"BRANCH_BOARD uncovered storage family file=%0d f17=%0d g11=%0d h11=%0d",changed_file,changed_f17,changed_g11,changed_h11);
  $display("NATIVE_BRANCH_BOARD_PASS inputs={input_bits} outputs={output_bits} flags=104 checks=%0d guest_checks=%0d file=%0d f17=%0d g11=%0d h11=%0d",checks,guest_checks,changed_file,changed_f17,changed_g11,changed_h11);$finish;
 end
endmodule
'''
    (out/'tb.sv').write_text(tb);shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),stage=str(stage),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        original_native_provenance_sha256=sha(native/'provenance.json.gz'),input_bits=input_bits,output_bits=output_bits,additional_flag_history_comparison_bits=104,
        wrong_clock_buffer_bypass=wrong_clock,
        actual_native_clock_buffers_and_data_feedback_retained=True,all_board_outputs_compared=True,
        files={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    sources=[str(out/n)for n in used]+[str(out/'original-board.sv'),str(out/'tb.sv')]
    commands=[['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_branch_board',
        '-Wno-PINMISSING','-Wno-UNOPTFLAT','-Wno-MULTIDRIVEN','-Wno-LATCH','-Wno-TIMESCALEMOD',
        '-I'+str(out/'src/verilog/cells'),'-I'+str(out/'src/verilog/generated'),'-I'+str(out/'src/verilog/vendor/6502'),'-I'+str(out/'src/verilog/vendor/riot'),
        *['-D'+d for d in prov['defines']],'--Mdir',str(out/'obj'),'-o','test',*sources],[str(out/'obj/test')]]
    save();code=1
    for label,cmd in zip(['compile','test'],commands):
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(cmd,cwd=out/'src',stdout=log,stderr=log);a=dict(label=label,pid=child.pid,command=cmd,started=time.time())
            state['actions'].append(a);save();code=child.wait()
        a.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    match=re.search(r'^NATIVE_BRANCH_BOARD_PASS .*$',log,re.M)
    passed=code==0 and match is not None and '$readmem file not found'not in log
    if wrong_clock:passed=state['actions'][0]['returncode']==0 and code!=0 and 'BRANCH_BOARD_WRONG_CLOCK_PHYSICAL_Q'in log and '$readmem file not found'not in log
    state.update(status=('PASS_ACTUAL_NATIVE_BRANCH_BOARD_CLOCK_COUNTEREXAMPLE'if wrong_clock else'PASS_ACTUAL_NATIVE_BRANCH_BOARD_REPLAY')if passed else'TERMINAL_NATIVE_BRANCH_BOARD_NOT_ESTABLISHED',
        returncode=code,finished=time.time(),summary=match[0]if match else None,
        original_firmware_or_60ns_or_FPGA_timing_qualified=False,hardware_or_original_live_simulations_changed=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','native','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--wrong-clock',action='store_true')
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.native.resolve(),a.output.resolve(),a.wrong_clock))
