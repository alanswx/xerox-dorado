#!/usr/bin/env python3
"""Prove full closed-owner projection and entry-only cached pair control safety.

The real844-Q projection must preserve every one of the156 control owners.
Induct the actual generic cache, with separate first-event and committed root,
against fresh ordered evaluation of BOTH475-bit subevents. Ghost invariants
check saved payloads and canonical identities; no input/cache history is assumed.
No physical timing, source timestamps, native selection or hardware follows.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from generate_cached_r_packet import cache,stage,projection
from prove_cp_prefix_cache import module,INVARIANTS
from prove_native_cp_context import expression
from test_r_front_forward_view import sha

CONTROL='''module packet_control_equivalence(
 input wire clk,reset,request,use_pair,other_settled,
 input wire [155:0] q,input wire before_root,after_root,
 input wire [397:0] before_pins,after_pins,input wire refresh,
 output wire equivalent,ready,entry_ready,fast,hit,output wire [949:0] different);
 wire first_after=use_pair ? 1'b0:after_root;
 wire [474:0] first,last,old_first,old_last;
 wire [268:0] live,saved,reference_first_prefix,reference_second_prefix;
 wire [10:0] saved_cp,saved_after;
 wire split=(before_root!=first_after)&&(before_pins[292:282]!=after_pins[292:282]);
 wire [10:0] prefix_cp=split ? after_pins[292:282]:before_pins[292:282];
 wire invariant;
 dorado_packet_prefix_cache guard(clk,reset,request && ready,q,use_pair ? last[474:319]:first[474:319],
  before_root,first_after,before_pins,after_pins,refresh,
  other_settled && !first[206] && (!use_pair || !last[206]),live,
  entry_ready,fast,hit,saved,saved_cp,saved_after,after_root,invariant);
 dorado_control_hold_ordered_step first_step(q,before_root,first_after,before_pins,after_pins,
  first[474:319],first[318:263],first[262:207],first[206],refresh,first[205:112],first[111:56],first[55:0],
  fast,saved,saved_cp,saved_after,live);
 dorado_control_hold_ordered_step second_step(SECOND_Q,1'b0,1'b1,after_pins,after_pins,
  last[474:319],last[318:263],last[262:207],last[206],refresh,last[205:112],last[111:56],last[55:0],
  1'b1,{first[474:319],113'b0},after_pins[292:282],after_pins[292:282],);
 dorado_control_hold_ordered_step reference_first(q,before_root,first_after,before_pins,after_pins,
  old_first[474:319],old_first[318:263],old_first[262:207],old_first[206],refresh,old_first[205:112],old_first[111:56],old_first[55:0],
  1'b0,reference_first_prefix,prefix_cp,after_pins[292:282],reference_first_prefix);
 dorado_control_hold_ordered_step reference_second(old_first[474:319],1'b0,1'b1,after_pins,after_pins,
  old_last[474:319],old_last[318:263],old_last[262:207],old_last[206],refresh,old_last[205:112],old_last[111:56],old_last[55:0],
  1'b0,reference_second_prefix,after_pins[292:282],after_pins[292:282],reference_second_prefix);
 assign ready=entry_ready && (!use_pair || (fast && before_root && after_root && before_pins==after_pins &&
  !first[206] && !last[206] && !(|first[262:207]) && !(|last[262:207])));
 assign different={first^old_first,(use_pair ? last^old_last:475'b0)};
 assign equivalent=invariant && (!ready || different==950'b0);
endmodule
'''


def projected(raw):
    top=stage(raw)
    assignments=re.findall(r'\bassign\s+state_next\[\d+:\d+\]\s*=\s*\w+\[\d+:\d+\];',top)
    assert len(assignments)==9
    return '''module projection_equivalence(
 input wire [155:0] joint_next,input wire [843:0] child_next,
 input wire [7:0] closed_r_flags_next,input wire [31:0] closed_read_next,closed_t_inputs_next,
 input wire [15:0] closed_t_visible_next,output wire equal,output wire [155:0] different);
 wire [109:0] conta_next=joint_next[109:0];wire [843:0] state_next;
 ASSIGNMENTS
 wire [843:0] held_next=HELD;
 PROJECTION
 assign different=control_projection(held_next)^joint_next;
 assign equal=different==156'b0;
endmodule
'''.replace('ASSIGNMENTS','\n '.join(assignments)).replace('HELD',expression(top,'held_next')).replace('PROJECTION',projection(raw))


def observed_cache(raw,stale):
    text=cache(raw,stale)
    old=''' output reg [268:0] prefix_saved=0,
 output reg [10:0] prefix_cp_saved=0,after_cp_saved=0,input wire committed_root);'''
    new=''' output reg [268:0] prefix_saved,
 output reg [10:0] prefix_cp_saved,after_cp_saved,input wire committed_root,output wire invariant);
 initial begin prefix_saved=0;prefix_cp_saved=0;after_cp_saved=0;end'''
    assert text.count(old)==1;text=text.replace(old,new,1)
    invariants=INVARIANTS.replace('dorado_control_hold_atomic_step key_function','dorado_control_hold_atomic_step #(.WAVES(5)) key_function').replace(
        'dorado_control_hold_atomic_step canonical_function','dorado_control_hold_atomic_step #(.WAVES(5)) canonical_function')
    invariants=invariants.replace('assign invariant=(', 'wire prefix_invariant;\n assign prefix_invariant=(',1)
    invariants+='''
 // Observe the committed context independently of the prefix identity. A
 // wrong root tag can preserve results while needlessly missing the fast path.
 reg owner_history_valid=0,committed_root_seen=0;
 always @(posedge clk)begin
  if(reset)owner_history_valid<=0;
  else if(request && ready)begin owner_history_valid<=1;committed_root_seen<=committed_root;end
 end
 assign invariant=prefix_invariant && (!owner_history_valid || canonical_root==committed_root_seen);
'''
    return text.replace('\nendmodule',invariants+'\nendmodule',1)


def sparse_control(top,mutation=None):
    old='''  first[474:319],first[318:263],first[262:207],first[206],refresh,first[205:112],first[111:56],first[55:0],'''
    new=old.replace('first[','first_raw[')
    assert top.count(old)==1
    extra=''' wire [474:0] first_raw;wire [362:0] falling_control;
 dorado_control_fall_step first_fall(q,after_pins,refresh,
  falling_control[362:207],falling_control[206:151],falling_control[150:95],falling_control[94],falling_control[93:0]);
 assign first=use_pair ? {falling_control,112'b0}:first_raw;
'''
    if mutation=='wrong-fall-cram':extra=extra.replace("{falling_control,112'b0}","{falling_control ^ (363'b1<<237),112'b0}",1)
    if mutation=='drop-fall-clock':extra=extra.replace("{falling_control,112'b0}","{falling_control & ~(363'b1<<159),112'b0}",1)
    return top.replace(old,new,1).replace(' wire first_after=',extra+' wire first_after=',1)


def run(source,out,yosys,pair_direct=False,sparse_pair=False):
    assert not sparse_pair or pair_direct
    assert not out.exists();s=json.loads((source/'run-state.json').read_text());assert s['status']=='PASS_FULL_PROCESSOR_STATELESS_EVALUATOR'
    for row in s['source_files']:assert sha(source/row['path'])==row['output_sha256']
    raw=(source/'candidate.sv').read_text()
    atoms='\n'.join(module(raw,n) for n in ['dorado_control_hold_atomic_step','dorado_control_terminal_body','dorado_control_hold_ordered_step',
        'dorado_control_certified_fall_step','dorado_control_fall_step'])
    out.mkdir(parents=True);(out/'controlled-processor.sv').write_text(stage(raw,sparse_pair));(out/'cache.sv').write_text(cache(raw))
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    shutil.copy2(Path(__file__).with_name('generate_cached_r_packet.py'),out/'generate_cached_r_packet.py')
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),source_state_sha256=sha(source/'run-state.json'),
        compared_control_bits_per_subevent=475,physical_projection_bits=156,processor_owner_bits=844,
        input_histories_unconstrained=True,committed_root_history_checked=True,pair_direct=pair_direct,sparse_pair=sparse_pair,initial_state='explicit auxiliary initializers; no architectural state assumed',
        native_canonical_match_specialization_used=False,files={p.name:sha(p) for p in out.iterdir() if p.is_file()},cases=[],hardware_changed=False)

    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')

    save()
    modes=['projection','bad-projection','induction','stale-root','stale-second']
    if sparse_pair:modes+=['wrong-fall-cram','drop-fall-clock']
    for mode in modes:
        d=out/mode;d.mkdir()
        if mode.endswith('projection'):
            top=projected(raw)
            if mode=='bad-projection':top=top.replace('q[31:0]','{q[31:1],q[1]}',1)
            files={'top.sv':top};entity='projection_equivalence';signal='equal';strategy=''
        else:
            top=CONTROL.replace('SECOND_Q','q' if mode=='stale-second' else 'first[474:319]')
            if pair_direct:
                assert top.count('  fast,saved,saved_cp,saved_after,live);')==1
                top=top.replace('  fast,saved,saved_cp,saved_after,live);','  (use_pair || fast),saved,saved_cp,saved_after,live);',1)
            if sparse_pair:
                top=sparse_control(top,mode)
                # Prove every result throughout the certified geometry, even
                # if result-dependent error guards would refuse acceptance.
                # Otherwise a corrupt result could hide by suppressing ready.
                old=" assign equivalent=invariant && (!ready || different==950'b0);"
                new=""" wire control_domain=entry_ready && (!use_pair ||
  (fast && before_root && after_root && before_pins==after_pins));
 assign equivalent=invariant && (!control_domain || different==950'b0);"""
                assert top.count(old)==1;top=top.replace(old,new,1)
            files={'control.sv':atoms,'cache.sv':observed_cache(raw,mode=='stale-root'),'top.sv':top}
            entity='packet_control_equivalence';signal='equivalent'
            strategy='-seq 2 -tempinduct -maxsteps 4 -set-init-zero' if mode=='induction' else '-seq 5 -set-init-zero'
        for name,text in files.items():(d/name).write_text(text)
        flag='verify' if mode in ['projection','induction'] else 'falsify'
        sources=' '.join('/work/'+name for name in files)
        (d/'proof.ys').write_text(f'''read_verilog -sv -defer {sources}
hierarchy -check -top {entity}
proc
flatten
opt
check -assert
sat {strategy} -timeout 120 -prove {signal} 1 -{flag} -show-ports -dump_json /work/counterexample.json
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        command=[str(yosys),'-Q','-T','-s','/work/proof.ys'];inputs={p.name:sha(p) for p in d.iterdir() if p.is_file()}
        with (d/'proof.log').open('x') as log:
            p=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT,env=env);row=dict(mode=mode,pid=p.pid,command=command,started=time.time(),files=inputs)
            state['cases'].append(row);save();code=p.wait()
        text=(d/'proof.log').read_text();marker=('Induction step proven: SUCCESS!' if mode=='induction' else 'no model found: SUCCESS!') if flag=='verify' else 'model found: FAIL!'
        passed=code==0 and marker in text;row.update(passed=passed,returncode=code,finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        if not passed:break
    passed=len(state['cases'])==len(modes) and all(v['passed'] for v in state['cases'])
    state.update(status='PASS_ENTRY_CACHED_PACKET_INDUCTION_AND_OWNER_PROJECTION' if passed else 'TERMINAL_CACHED_PACKET_PROOF_NOT_ESTABLISHED',finished=time.time());save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--pair-direct',action='store_true')
    p.add_argument('--sparse-pair',action='store_true')
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.yosys.resolve(),a.pair_direct,a.sparse_pair))
