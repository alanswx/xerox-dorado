#!/usr/bin/env python3
"""Prove the owned native context makes the generic wide comparison redundant.

First compile the actual closed-owner/held-Q/control projections and prove
all156 next control owners equal joint_next. Then compare the actual generic
and specialized cache RTL with source-bound owner snapshot enables. All input
histories are arbitrary, owner power-up values are unconstrained, and only
explicit cache RTL initializers constrain auxiliary power-up state. This is
compositional digital equivalence, not whole-machine timing/boot qualification.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import time

from prove_cp_prefix_cache import module
from test_r_front_forward_view import sha

RTL='src/verilog/rtl/dorado_r_front_control_step.sv'
BP='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
MATCH=''' wire canonical_match=(current_q==canonical_q)&&(before_pins==canonical_pins)&&
                      (before_root==canonical_root)&&(refresh==canonical_refresh);'''
OBSERVE='''
 assign context_state={canonical_valid,cache_valid,canonical_q,canonical_pins,canonical_root,canonical_refresh,
                       cache_key,prefix_saved,prefix_cp_saved,after_cp_saved};
 assign owner_matches=!canonical_valid ||
  ((current_q==canonical_q)&&(before_pins==canonical_pins)&&(before_root==canonical_root)&&(refresh==canonical_refresh));
'''
TOP='''module native_context_equivalence(
 input wire clk,reset,advance,settled,input wire [155:0] next_q,
 input wire after_root,input wire [397:0] after_pins,input wire [268:0] prefix_live,
 output wire equivalent,original_ready,native_ready,original_fast,native_fast,
 output wire original_matches,native_matches);
 // These abstract owners have the exact protected enable and D projection
 // proved from the actual native source. They intentionally have no init.
 reg [155:0] owner_q;
 reg [397:0] before_pins;
 reg before_root;
 wire original_hit,native_hit;
 wire [268:0] original_saved,native_saved;
 wire [10:0] original_prefix_cp,native_prefix_cp,original_after_cp,native_after_cp;
 wire [1415:0] original_state,native_state;
 generic_cache original(clk,reset,advance,owner_q,next_q,before_root,after_root,before_pins,after_pins,
  1'b0,settled,prefix_live,original_ready,original_fast,original_hit,original_saved,original_prefix_cp,original_after_cp,
  original_state,original_matches);
 specialized_cache actual(clk,reset,advance,owner_q,next_q,before_root,after_root,before_pins,after_pins,
  1'b0,settled,prefix_live,native_ready,native_fast,native_hit,native_saved,native_prefix_cp,native_after_cp,
  native_state,native_matches);
 always @(posedge clk)if(advance && original_ready)begin
  owner_q<=OWNER_NEXT;before_pins<=after_pins;before_root<=after_root;
 end
 assign equivalent=(original_state==native_state)&&original_matches&&native_matches&&
  ({original_ready,original_fast,original_hit}=={native_ready,native_fast,native_hit});
endmodule
'''


def expression(raw,name):
    hits=re.findall(r'\b(?:assign\s+|wire\s+\[\d+:0\]\s+)'+name+r'\s*=\s*([^;]+);',raw)
    assert len(hits)==1,(name,len(hits))
    return hits[0]


def project_source(raw,bp):
    top=module(raw,'dorado_r_front_control_step')
    cq=expression(top,'cq');joint=expression(top,'joint_q')
    held_next=expression(top,'held_next');native_held=expression(bp,'r_front_held')
    assert 'wire [31:0] current_read_q,closed_read_next;'in top
    assert 'wire [15:0] current_t_visible,closed_t_visible_next;'in top
    assert 'wire [7:0] current_r_flags,closed_r_flags_next;'in top
    assert 'wire [31:0] current_t_inputs,closed_t_inputs_next;'in top
    assert expression(top,'conta_next')=='joint_next[109:0]'
    assignments=re.findall(r'\bassign\s+state_next\[\d+:\d+\]\s*=\s*\w+\[\d+:\d+\];',top)
    assert len(assignments)==9,len(assignments)
    assert 'r_front_lower_q<=r_front_next[571:0];r_front_upper_q<=r_front_next[843:588];'in bp
    return '''module owned_projection(
 input wire [155:0] joint_next,input wire [843:0] child_next,
 input wire [7:0] closed_r_flags_next,input wire [31:0] closed_read_next,closed_t_inputs_next,
 input wire [15:0] closed_t_visible_next,
 input wire [7:0] r_front_visible_r_ProcL,r_front_visible_r_ProcH,
 output wire equal,output wire [155:0] different);
 wire [109:0] conta_next=joint_next[109:0];
 wire [843:0] state_next;
 ASSIGNMENTS
 wire [843:0] held_next=HELD_NEXT;
 wire [843:0] r_front_next=held_next;
 wire [571:0] r_front_lower_q=r_front_next[571:0];
 wire [255:0] r_front_upper_q=r_front_next[843:588];
 wire [843:0] r_front_held=NATIVE_HELD;
 wire [843:0] held_q=r_front_held;
 wire [109:0] cq=CQ;
 wire [155:0] joint_q=JOINT;
 assign different=joint_q^joint_next;
 assign equal=~|different;
endmodule
'''.replace('ASSIGNMENTS','\n '.join(assignments)).replace('HELD_NEXT',held_next).replace('NATIVE_HELD',native_held).replace('CQ',cq).replace('JOINT',joint)


def expose(raw,name):
    suffix=' output reg [10:0] prefix_cp_saved,after_cp_saved);'
    extended=''' output reg [10:0] prefix_cp_saved,after_cp_saved,
 output wire [1415:0] context_state,output wire owner_matches);'''
    assert raw.count(suffix)==1
    renamed=raw.replace('module dorado_cp_prefix_cache(',f'module {name}(',1)
    new=renamed.replace(suffix,extended,1).replace('\nendmodule',OBSERVE+'\nendmodule',1)
    assert new.replace(OBSERVE,'',1).replace(extended,suffix,1)==renamed
    return new


def run(generic,native,yosys,out,stale):
    assert not out.exists()
    states=[]
    for directory in [generic,native]:
        s=json.loads((directory/'r-front-control-native.json').read_text())
        for name,h in s['source_hashes'].items():assert sha(directory/name)==h,name
        states.append(s)
    assert not states[0].get('owned_context_specialization_requested',False)
    assert states[1]['owned_context_specialization_requested']and states[1]['owner_commit_also_guarded_by_control_ready']
    original=(generic/RTL).read_text();actual=(native/RTL).read_text()
    assert actual.replace(" wire canonical_match=1'b1;",MATCH,1)==original
    bp=(native/BP).read_text()
    block='''  always @(posedge storage_clk)if(contb_advance && control_ready)begin
    r_front_lower_q<=r_front_next[571:0];r_front_upper_q<=r_front_next[843:588];
    r_front_before<=r_front_live;r_front_clock_before<=CLK_ca_p_;
  end'''
    assert bp.count(block)==1
    assert len(re.findall(r'\br_front_before\s*<=',bp))==1
    assert len(re.findall(r'\br_front_clock_before\s*<=',bp))==1
    for snippet in ['.held_q(r_front_held)', '.clock_before(r_front_clock_before),.clock_after(CLK_ca_p_)',
                    '.pins_before(r_front_before),.pins_after(r_front_live),.refresh_before_run_capture(1\'b0)',
                    '.held_next(r_front_next)', '.host_clk(storage_clk),.advance(contb_advance)',
                    '.cp_reset(storage_reset),.cp_ready(control_ready)']:
        assert bp.count(snippet)==1,snippet
    cache=module(original,'dorado_cp_prefix_cache');special=module(actual,'dorado_cp_prefix_cache')
    assert 'if(request && ready)begin' in cache
    out.mkdir(parents=True)
    sources={'projection.sv':project_source(actual,bp),'generic.sv':expose(cache,'generic_cache'),
             'native.sv':expose(special,'specialized_cache'),
             'top.sv':TOP.replace('OWNER_NEXT',"next_q ^ (156'b1<<69)"if stale else'next_q')}
    for name,text in sources.items():(out/name).write_text(text)
    script='''read_verilog -sv /work/projection.sv
hierarchy -check -top owned_projection
proc
flatten
opt
check -assert
sat -timeout 120 -prove equal 1 -verify -show-ports -dump_json /work/projection-counterexample.json
design -reset
read_verilog -sv /work/generic.sv /work/native.sv /work/top.sv
hierarchy -check -top native_context_equivalence
proc
flatten
opt
check -assert
SAT_COMMAND
'''
    strategy='sat -seq 4 -timeout 120 -prove equivalent 1 -falsify -show-ports -dump_json /work/context-counterexample.json'if stale else \
             'sat -seq 2 -tempinduct -maxsteps 4 -timeout 120 -prove equivalent 1 -verify -show-ports -dump_json /work/context-counterexample.json'
    (out/'proof.ys').write_text(script.replace('SAT_COMMAND',strategy))
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    for name,path in [('native-backplane.v',native/BP),('native-control.sv',native/RTL),('generic-control.sv',generic/RTL)]:
        (out/name).write_bytes(path.read_bytes())
    env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
    (out/'version.txt').write_text(subprocess.check_output([str(yosys),'-V'],text=True,env=env))
    record=dict(status='RUNNING',scope=__doc__,driver_pid=os.getpid(),started=time.time(),
        generic_stage=str(generic),native_stage=str(native),generic_manifest_sha256=sha(generic/'r-front-control-native.json'),
        native_manifest_sha256=sha(native/'r-front-control-native.json'),owned_projection_bits=156,
        auxiliary_state_compared_bits=1416,cache_interface_bits_compared=3,
        source_inverse_byte_identical_except_wide_match=True,cache_power_up_state='exact explicit RTL initializers',
        owner_power_up_state='unconstrained',input_or_protocol_assumptions_added=False,
        prefix_and_next_data='same unconstrained inputs; source control functions are byte-identical',
        stale_owner_control=stale,unbounded_induction_proven=False,
        source_files={p.name:sha(p)for p in out.iterdir()if p.is_file()},
        native_clock_hz=57142857,sysper=4,firmware_qualified=False,physical_timing_qualified=False,speedup_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(record,indent=2)+'\n')
    save();command=[str(yosys),'-Q','-T','-s','/work/proof.ys']
    with(out/'proof.log').open('x')as log:
        process=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT,env=env)
        record.update(pid=process.pid,command=command);save();code=process.wait()
    text=(out/'proof.log').read_text()
    projection='SAT proof finished - no model found: SUCCESS!'in text
    induction='Induction step proven: SUCCESS!'in text
    counterexample='SAT proof finished - model found: FAIL!'in text
    passed=code==0 and projection and(counterexample if stale else induction)
    record.update(status=('PASS_STALE_NATIVE_OWNER_COUNTEREXAMPLE'if stale else'PASS_SOURCE_BOUND_NATIVE_CP_CONTEXT')
                  if passed else'TERMINAL_NATIVE_CONTEXT_NOT_ESTABLISHED',
                  returncode=code,finished=time.time(),owned_projection_proven=projection,
                  unbounded_induction_proven=bool(passed and not stale),counterexample_found=counterexample,
                  proof_log_sha256=sha(out/'proof.log'))
    save();print(record['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['generic','native','yosys','output']:parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--stale-owner',action='store_true')
    a=parser.parse_args();raise SystemExit(run(a.generic.resolve(),a.native.resolve(),a.yosys.resolve(),a.output.resolve(),a.stale_owner))
