#!/usr/bin/env python3
"""Compare every projected CP-source pin with original decoder packages.

Prove all401 pins plus packed-bus recovery/idempotence with417 arbitrary
binary inputs. Wrong selector/enable/stale decoded-data epochs must change
real MIR set bits. Replay all65536 CP/unused-gate inputs and3336 independent
X/Z cases, including compiled counterexample controls. No capture-order or
whole-machine timing result follows from this combinational source check.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from generate_native_cp_source import functions
from review_baseboard_clock_consumer import verify
from test_r_front_forward_view import sha


def wrapper(name,fn,stale=False):
    text='''module MODULE(input wire [400:0] pins,input wire [12:0] cp,old_cp,
 output wire [400:0] result,output wire [12:0] recovered,output wire idempotent);
 FUNCTIONS
 wire [400:0] full=cp_source_pins(pins,cp);
 RESULT
 assign recovered=cp_bus(result);
 assign idempotent=(cp_source_pins(result,cp_bus(result))==result);
endmodule
'''.replace('MODULE',name).replace('FUNCTIONS',fn)
    result='assign result=full;'
    if stale:
        result='assign result={full[400:282],cp_set_decode(old_cp),full[263:0]};'
    return text.replace('RESULT',result)


MITER='''module cp_source_equivalence(input wire [400:0] pins,
 input wire [12:0] cp,old_cp,input wire [2:0] aux,
 output wire equivalent,output wire [400:0] actual,expected,
 output wire [12:0] recovered,output wire idempotent);
 original_cp_source original(pins,cp,aux,expected);
 CANDIDATE changed(pins,cp,old_cp,actual,recovered,idempotent);
 assign equivalent=(actual==expected) && (recovered==cp) && idempotent;
endmodule
'''

TB='''`timescale 1ns/1ps
module tb_native_cp_source;
 reg [416:0] stimulus,base;
 wire [400:0] pins;wire [12:0] cp;wire [2:0] aux;
 assign {pins,cp,aux}=stimulus;
 wire [400:0] actual,expected,bad_selector,bad_enable,stale;
 wire [12:0] recovered;wire idempotent;
 reg [12:0] old_cp=0;
 original_cp_source original(pins,cp,aux,expected);
 cp_candidate actual_source(pins,cp,old_cp,actual,recovered,idempotent);
 cp_bad_selector bad1(pins,cp,old_cp,bad_selector,,);
 cp_bad_enable bad2(pins,cp,old_cp,bad_enable,,);
 cp_stale_data bad3(pins,cp,old_cp,stale,,);
 integer known=0,unknown=0,checks=0,selector_seen=0,enable_seen=0,stale_seen=0;
 reg [31:0] rng=32'hc5013b17;
 task automatic compare;
  begin
   #1;
   if(actual!==expected || recovered!==cp || actual_source.cp_source_pins(actual,recovered)!==actual)
    $fatal(1,"NATIVE_CP_SOURCE mismatch check=%0d cp=%h actual=%h expected=%h",checks,cp,actual,expected);
   if(bad_selector!==expected)selector_seen++;
   if(bad_enable!==expected)enable_seen++;
   if(stale!==expected)stale_seen++;
   checks++;
  end
 endtask
 initial begin
  for(integer word=0;word<65536;word++)begin
   stimulus=0;stimulus[15:0]=16'(word);old_cp=13'(word^13'h1fff);compare;known++;
  end
  for(integer background=0;background<4;background++)begin
   for(integer b=0;b<417;b++)begin
    rng=rng^(rng<<13);rng=rng^(rng>>17);rng=rng^(rng<<5);
    base[b]=background==0 ? 0:background==1 ? 1:rng[0];
   end
   // Active decoder, both selector values, and inactive source backgrounds.
   if(background==2)begin base[15]=0;base[12]=0;end
   old_cp=13'(background*17);
   for(integer b=0;b<417;b++)for(integer mode=0;mode<2;mode++)begin
    stimulus=base;stimulus[b]=mode ? 1'bz:1'bx;compare;unknown++;
   end
  end
  if(known!=65536 || unknown!=3336 || selector_seen<100 || enable_seen<100 || stale_seen<100)
   $fatal(1,"NATIVE_CP_SOURCE coverage known=%0d unknown=%0d bad=%0d/%0d/%0d",known,unknown,selector_seen,enable_seen,stale_seen);
  $display("NATIVE_CP_SOURCE_PASS known=%0d unknown=%0d checks=%0d selector=%0d enable=%0d stale=%0d",known,unknown,checks,selector_seen,enable_seen,stale_seen);
  $finish;
 end
endmodule
'''


def run(source,out,yosys):
    assert not out.exists()
    m=json.loads((source/'manifest.json').read_text());verify(source,m['files'])
    assert m['status']=='GENERATED_SOURCE_BOUND_NATIVE_CP_INPUTS'
    good=functions(m['direct_aliases'],m['decoded_MIR_sets'],m['decoder_terms'])
    assert good==(source/'functions.sv').read_text()
    out.mkdir(parents=True)
    for name in ['original.sv','cell_MC10103.v','cell_MC10172.v']:
        shutil.copy2(source/name,out/name)
    candidate='\n'.join(wrapper(name,functions(m['direct_aliases'],m['decoded_MIR_sets'],m['decoder_terms'],mutation),stale)
                         for name,mutation,stale in [('cp_candidate',None,False),('cp_bad_selector','swap-selector',False),
                                                     ('cp_bad_enable','wrong-enable',False),('cp_stale_data',None,True)])
    (out/'candidate.sv').write_text(candidate);(out/'tb.sv').write_text(TB)
    for tool in [Path(__file__),Path(__file__).with_name('generate_native_cp_source.py')]:
        shutil.copy2(tool,out/tool.name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),
               source_manifest_sha256=sha(source/'manifest.json'),compared_pin_bits=401,
               bus_recovery_bits=13,arbitrary_binary_input_bits=417,
               files={p.name:sha(p) for p in out.iterdir()},actions=[],clock_or_owner_changes=False,
               physical_arrival_or_native_timing_qualified=False,hardware_changed=False)
    def save():
        (out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    def execute(label,cmd,env=None):
        row=dict(label=label,command=cmd,started=time.time())
        with (out/(label+'.log')).open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT,env=env)
            row['pid']=child.pid;state['actions'].append(row);save();code=child.wait()
        row.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        assert code==0,label
        return (out/(label+'.log')).read_text()
    save()
    try:
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
        for label,entity in [('positive','cp_candidate'),('wrong-selector','cp_bad_selector'),('wrong-enable','cp_bad_enable'),('stale-data','cp_stale_data')]:
            (out/(label+'-top.sv')).write_text(MITER.replace('CANDIDATE',entity))
            flag='verify' if label=='positive' else 'falsify'
            (out/(label+'.ys')).write_text(f'''read_verilog -sv /work/original.sv /work/candidate.sv /work/cell_MC10103.v /work/cell_MC10172.v /work/{label}-top.sv
hierarchy -check -top cp_source_equivalence
proc
flatten
opt
check -assert
sat -timeout 120 -prove equivalent 1 -{flag} -show-ports -dump_json /work/{label}-counterexample.json
''')
            text=execute(label,[str(yosys),'-Q','-T','-s','/work/'+label+'.ys'],env)
            assert ('no model found: SUCCESS!' if label=='positive' else 'model found: FAIL!') in text
        execute('compile',['iverilog','-g2012','-s','tb_native_cp_source','-o',str(out/'test.vvp'),
                           *[str(out/n) for n in ['original.sv','candidate.sv','cell_MC10103.v','cell_MC10172.v','tb.sv']]])
        text=execute('test',['vvp','-i',str(out/'test.vvp')])
        assert 'NATIVE_CP_SOURCE_PASS known=65536 unknown=3336 checks=68872' in text
        state.update(status='PASS_ORIGINAL_NATIVE_CP_SOURCE_PROJECTION',finished=time.time(),
                     known_cases=65536,XZ_cases=3336,compiled_bad_selector_enable_and_stale_data_detected=True,
                     arbitrary_binary_before_after_epoch_order_not_assumed=True)
    except BaseException:
        state.update(status='TERMINAL_NATIVE_CP_SOURCE_NOT_ESTABLISHED',finished=time.time());save();raise
    save();print(state['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();run(a.source.resolve(),a.output.resolve(),a.yosys.resolve())
