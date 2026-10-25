#!/usr/bin/env python3
"""Check the actual single-function known detector and prepared-view X/Z bypass.

Compare all original2969 outputs to the unchanged native function. Substitute
X and Z independently into every1982 input over three contexts. Every such
input must select the original raw physical view, even with an invalid cache.
A missing-bypass negative must fail that actual view comparison.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from integrate_native_operand_buffer import control,HELPER
from prove_cp_prefix_cache import module
from qualify_native_ready_compare import SINGLE,CONTROL,ports
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]


def run(epoch,ready_proof,out,bad=False):
    assert not out.exists();out.mkdir(parents=True)
    e=json.loads((epoch/'run-state.json').read_text());verify(epoch,e['files'])
    assert e['status']=='PASS_ACTUAL_NATIVE_PRE_EVENT_OPERAND_VIEW'and e['host_clock_varies_independently']
    stage=Path(e['stage']);verify(stage,e['source_hashes'])
    original=module((stage/CONTROL).read_text(),SINGLE);changed=module(control((stage/CONTROL).read_text()),SINGLE)
    p=json.loads((ready_proof/'run-state.json').read_text());verify(ready_proof,p['files'])
    assert p['status']=='PASS_ACTUAL_NATIVE_READY_COMPARISON'
    signal=next(s for s in json.loads((ready_proof/'wrong-preempt-selector/counterexample.json').read_text())['signal']if s['name']=='stimulus')
    seed=int(signal['data'][0],2)
    for f in(epoch/'pre-event-view').glob('*.sv'):
        if f.name not in ['top.sv',SINGLE+'.sv']:shutil.copy2(f,out/f.name)
    (out/(SINGLE+'.sv')).write_text(changed)
    (out/'reference.sv').write_text(original.replace('module '+SINGLE+' #','module original_operand_single #',1))
    raw=(ROOT/HELPER).read_text()
    if bad:
        a='assign physical_view=(use_pair || !context_known) ? raw_view:prepared_view;'
        assert raw.count(a)==1;raw=raw.replace(a,'assign physical_view=use_pair ? raw_view:prepared_view;',1)
    (out/'buffer.sv').write_text(raw)
    decl=[];args=[[],[]];ino=outo=0;phys=None
    for direction,width,name in ports(original):
        if direction=='input':
            decl.append(f' wire [{width-1}:0] {name}=stimulus[{ino}+:{width}];');ino+=width
            for a in args:a.append(f'.{name}({name})')
        else:
            if name=='physical_state':phys=outo
            for i,prefix in enumerate(['expected','actual']):args[i].append(f'.{name}({prefix}[{outo}+:{width}])')
            outo+=width
    assert ino==1982 and outo==2969 and phys is not None
    args[1].append('.operand_context_known(known)')
    top=f'''module operand_xz(input wire [{ino-1}:0] stimulus,
 output wire [{outo-1}:0] actual,expected,output wire known,ready,output wire [843:0] prepared);
{chr(10).join(decl)}
 original_operand_single original({','.join(args[0])});
 {SINGLE} changed({','.join(args[1])});
 dorado_native_operand_buffer buffer(.clk(1'b0),.reset(1'b0),.request(1'b0),.use_pair(1'b0),.context_known(known),
  .held_q(held_q),.before_root(clock_before),.before_pins(pins_before),.before_data(external_file_read),
  .raw_view(actual[{phys}+:844]),.ready(ready),.physical_view(prepared),.valid(),.saved_tag(),.saved_fields());
endmodule
'''
    tb=f'''`timescale 1ns/1ps
module tb;
 reg [1981:0] stimulus,base;wire [2968:0] actual,expected;wire known,ready;wire [843:0] prepared;
 operand_xz dut(stimulus,actual,expected,known,ready,prepared);
 integer checks=0,unknown_count=0;
 task automatic compare(input integer unknown_case);
  begin
   #1;
   if(actual!==expected)$fatal(1,"OPERAND_XZ original raw result changed");
   if(unknown_case)begin
    if(known!==1'b0 || ready!==1'b1)$fatal(1,"OPERAND_XZ unknown input not bypassed");
    if(prepared!==expected[{phys}+:844])$fatal(1,"OPERAND_XZ prepared view differs from original unknown view");
    unknown_count++;
   end else if(known!==1'b1)$fatal(1,"OPERAND_XZ known input rejected");
   checks++;
  end
 endtask
 initial begin
  for(integer trial=0;trial<3;trial++)begin
   case(trial)0:base=1982'h{seed:x};1:base=0;2:base='1;endcase
   stimulus=base;compare(0);
   for(integer bit_no=0;bit_no<1982;bit_no++)for(integer mode=0;mode<2;mode++)begin
    stimulus=base;stimulus[bit_no]=mode ? 1'bz:1'bx;compare(1);
   end
  end
  $display("OPERAND_XZ_PASS checks=%0d unknown=%0d outputs=2969 physical=844",checks,unknown_count);$finish;
 end
endmodule
'''
    (out/'top.sv').write_text(top);(out/'tb.sv').write_text(tb);shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',started=time.time(),scope=__doc__,epoch_state_sha256=sha(epoch/'run-state.json'),
        ready_seed_proof_sha256=sha(ready_proof/'run-state.json'),helper_sha256=sha(ROOT/HELPER),broken_unknown_fallback=bad,
        files={q.name:sha(q)for q in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    paths=sorted(str(q)for q in out.glob('*.sv'));save();code=1
    for label,cmd in [('compile',['iverilog','-g2012','-s','tb','-o',str(out/'test.vvp'),*paths]),('test',['vvp','-i',str(out/'test.vvp')])]:
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log);a=dict(label=label,pid=child.pid,command=cmd,started=time.time())
            state['actions'].append(a);save();code=child.wait()
        a.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    passed=(len(state['actions'])==2 and state['actions'][0]['returncode']==0 and code!=0 and 'OPERAND_XZ prepared view differs'in log)if bad else(code==0 and 'checks=11895 unknown=11892 outputs=2969 physical=844'in log)
    state.update(status=('PASS_NATIVE_OPERAND_UNKNOWN_BYPASS_COUNTEREXAMPLE'if bad else'PASS_NATIVE_OPERAND_UNKNOWN_FALLBACK')if passed else'TERMINAL_NATIVE_OPERAND_UNKNOWN_NOT_ESTABLISHED',
        returncode=code,finished=time.time(),native_or_firmware_or_speed_qualified=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['epoch','ready-proof','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--break-unknown',action='store_true')
    a=p.parse_args();raise SystemExit(run(a.epoch.resolve(),a.ready_proof.resolve(),a.output.resolve(),a.break_unknown))
