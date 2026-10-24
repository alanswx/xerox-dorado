#!/usr/bin/env python3
"""Source-bound proof of pair-direct ready, permissions and full cache history.

The default experiment changes only first.entry_fast to use_pair OR entry_fast.
Sparse mode additionally normalizes paired inputs under the original geometry;
split mode uses an unconditional paired-first function and a separate single/
real-prefix invocation. Bind every actual invocation and the unchanged prefix
producer before allowing arbitrary result differences outside certified geometry.
Induct both actual1416-bit caches, ready and SRAM permission for arbitrary
histories. Required negatives drop the entry guard or corrupt unready prefix
capture. No new architectural input assumption or physical timing claim.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from generate_cached_r_packet import packet,stage,cache
from prove_cp_prefix_cache import module
from test_r_front_forward_view import sha

TOP='''module pair_direct_cache_equivalence(
 input wire clk,reset,request,use_pair,refresh,before_root,first_root,committed_root,
 input wire [155:0] current_q,next_common,next_invalid,
 input wire [397:0] before_pins,after_pins,
 input wire [268:0] prefix_live,prefix_invalid,
 input wire settled_common,settled_invalid,guard_common,guard_invalid,
 output wire equal,old_ready,new_ready,old_permission,new_permission);
 wire old_entry,new_entry,old_fast,new_fast,old_hit,new_hit;
 wire [1415:0] old_state,new_state;
 wire new_domain=!use_pair || new_fast;
 wire new_guard=new_domain ? guard_common:guard_invalid;
 original_cache original(clk,reset,request && old_ready,current_q,next_common,
  before_root,first_root,before_pins,after_pins,refresh,settled_common,prefix_live,
  old_entry,old_fast,old_hit,,, ,committed_root,old_state);
 changed_cache actual(clk,reset,request && new_ready,current_q,new_domain ? next_common:next_invalid,
  before_root,first_root,before_pins,after_pins,refresh,new_domain ? settled_common:settled_invalid,PREFIX,
  new_entry,new_fast,new_hit,,, ,committed_root,new_state);
 assign old_ready=old_entry && (!use_pair || (old_fast && guard_common));
 assign new_ready=new_entry && (!use_pair || (ENTRY_GUARD && new_guard));
 assign old_permission=request && old_ready;
 assign new_permission=request && new_ready;
 assign equal=(old_state==new_state) && old_ready==new_ready && old_permission==new_permission &&
  {old_entry,old_fast,old_hit}=={new_entry,new_fast,new_hit};
endmodule
'''


def exposed(raw,name):
    original=cache(raw)
    old=''' output reg [268:0] prefix_saved=0,
 output reg [10:0] prefix_cp_saved=0,after_cp_saved=0,input wire committed_root);'''
    new=''' output reg [268:0] prefix_saved,
 output reg [10:0] prefix_cp_saved,after_cp_saved,input wire committed_root,output wire [1415:0] context_state);
 initial begin prefix_saved=0;prefix_cp_saved=0;after_cp_saved=0;end'''
    assert original.count(old)==1
    observe='''
 assign context_state={canonical_valid,cache_valid,canonical_q,canonical_pins,canonical_root,canonical_refresh,
  cache_key,prefix_saved,prefix_cp_saved,after_cp_saved};
'''
    return original.replace(old,new,1).replace('module dorado_packet_prefix_cache(',f'module {name}(',1).replace('\nendmodule',observe+'\nendmodule',1)


def run(source,out,yosys,sparse_pair=False,split_pair=False):
    assert not split_pair or sparse_pair
    assert not out.exists();s=json.loads((source/'run-state.json').read_text());assert s['status']=='PASS_FULL_PROCESSOR_STATELESS_EVALUATOR'
    for row in s['source_files']:assert sha(source/row['path'])==row['output_sha256']
    raw=(source/'candidate.sv').read_text();old=packet(raw)
    if split_pair:
        from generate_split_r_packet import packet as split
        new=split(raw)
        # The split generator inverse-checks its exact edit whitelist. Bind
        # the actual invocation which produces the shared raw prefix to the
        # REAL inputs, separately from unconditional paired-first data.
        single=new.split(' dorado_r_front_prefix_step single(',1)[1].split(');',1)[0]
        first=new.split(' dorado_r_front_prefix_step first(',1)[1].split(');',1)[0]
        second=new.split(' dorado_r_front_prefix_step second(',1)[1].split(');',1)[0]
        for binding in ['.held_q(held_q)','.clock_before(clock_before)','.clock_after(first_after)',
                        '.pins_before(pins_before)','.pins_after(pins_after)',
                        '.entry_fast(entry_fast)',".pair_fall(1'b0)",'.live_prefix(first_live_prefix)']:
            assert binding in single,binding
        for binding in ['.held_q(held_q)',".clock_before(1'b1)",".clock_after(1'b0)",
                        '.pins_before(pins_after)','.pins_after(pins_after)',".entry_fast(1'b1)",
                        ".pair_fall(1'b1)",'.live_prefix()','.control_owner(first_owner)']:
            assert binding in first,binding
        assert '.held_q(first_held_next)' in second
        assert new.count('.live_prefix(first_live_prefix)')==1
        assert new.count(' dorado_r_file_ports files(')==1
        assert 'control_projection(use_pair ? second_held_next:single_held_next)' in new
    else:
        new=packet(raw,pair_direct=True,sparse_pair=sparse_pair)
        assert new.count('.entry_fast((use_pair || entry_fast))')==1
        restored=new.replace('.entry_fast((use_pair || entry_fast))','.entry_fast(entry_fast)',1)
        if sparse_pair:
            for v in ['use_pair',"1'b0"]:
                assert restored.count(',\n  .pair_fall('+v+')')==1
                restored=restored.replace(',\n  .pair_fall('+v+')','',1)
        assert restored==old
    assert 'ready=entry_ready && entry_fast && clock_before && clock_after' in new
    actual_stage=stage(raw,sparse_pair);ordered=module(raw,'dorado_control_hold_ordered_step')
    assert 'assign control_owner=joint_q;assign live_prefix=cp_prefix_live;' in actual_stage
    assert 'wire [155:0] joint_q={held_q[843:798],cq};' in actual_stage
    assert 'assign prefix_live={raw_prefix_q,raw_prefix_events,raw_prefix_repeated,raw_prefix_unsettled};' in ordered
    assert '''dorado_control_hold_atomic_step #(.WAVES(WAVES)) cp_arrival(
  state_q,clock_before,clock_before,pins_before,prefix_pins,
  raw_prefix_q,raw_prefix_events,raw_prefix_repeated,raw_prefix_unsettled,
  refresh_before_run_capture,);''' in ordered
    assert '''wire [397:0] prefix_pins=split_event ?
  {pins_before[397:293],pins_after[292:282],pins_before[281:0]} : pins_before;''' in ordered
    if sparse_pair:
        # The generated function's inverse audit checks every unmentioned byte.
        # Explicitly bind normalization and selector choice to the SAME pair
        # branch as the wrapper's unchanged full401-pin/high-root ready guard.
        assert '''wire capture_clock_before=pair_fall ? 1'b1:clock_before;
 wire [400:0] capture_pins_before=pair_fall ? pins_after:pins_before;''' in actual_stage
        assert 'control(joint_q,clock_before,clock_after,pins_before[397:0],pins_after[397:0],' in actual_stage
        assert 'pair_fall ? {falling_control,112\'b0}:ordered_control;' in actual_stage
        assert 'pair_fall ? {joint_q[118],joint_q[121],joint_q[120]}:ordinary_selector;' in actual_stage
        assert 'clock_before && clock_after && pins_before==pins_after' in new
    out.mkdir(parents=True)
    for n,text in {'old-packet.sv':old,'new-packet.sv':new,'controlled-stage.sv':actual_stage,'actual-ordered.sv':ordered}.items():(out/n).write_text(text)
    for n in [Path(__file__).name,'generate_cached_r_packet.py']:shutil.copy2(Path(__file__).with_name(n),out/n)
    if split_pair:shutil.copy2(Path(__file__).with_name('generate_split_r_packet.py'),out/'generate_split_r_packet.py')
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),
        source_state_sha256=sha(source/'run-state.json'),cache_state_bits=1416,sparse_pair=sparse_pair,split_pair=split_pair,
        source_bound_input_substitution_only=not split_pair,prefix_producer_fast_mode_independent=True,
        all_unready_pair_result_differences_allowed=True,input_histories_unconstrained=True,
        files={p.name:sha(p) for p in out.iterdir() if p.is_file()},cases=[],hardware_changed=False)

    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')

    save()
    for mode in ['induction','drop-entry-guard','prefix-taint']:
        d=out/mode;d.mkdir()
        (d/'original.sv').write_text(exposed(raw,'original_cache'));(d/'actual.sv').write_text(exposed(raw,'changed_cache'))
        top=TOP.replace('PREFIX',('new_domain ? prefix_live:prefix_invalid' if mode=='prefix-taint' else 'prefix_live')).replace(
            'ENTRY_GUARD',"1'b1" if mode=='drop-entry-guard' else 'new_fast')
        if sparse_pair:
            top=top.replace('[397:0] before_pins,after_pins','[400:0] before_pins,after_pins',1)
            top=top.replace(' wire new_domain=!use_pair || new_fast;',
                ''' wire geometry=before_root && committed_root && !first_root && before_pins==after_pins;
 wire new_domain=!use_pair || (new_fast && geometry);''',1)
            top=top.replace('before_root,first_root,before_pins,after_pins,','before_root,first_root,before_pins[397:0],after_pins[397:0],')
            top=top.replace('old_fast && guard_common','old_fast && geometry && guard_common',1)
            top=top.replace('&& new_guard','&& geometry && new_guard',1)
        (d/'top.sv').write_text(top)
        flag='verify' if mode=='induction' else 'falsify'
        strategy='-seq 2 -tempinduct -maxsteps 4 -set-init-zero' if mode=='induction' else '-seq 3 -set-init-zero'
        (d/'proof.ys').write_text(f'''read_verilog -sv /work/original.sv /work/actual.sv /work/top.sv
hierarchy -check -top pair_direct_cache_equivalence
proc
flatten
opt
check -assert
sat {strategy} -timeout 120 -prove equal 1 -{flag} -show-ports -dump_json /work/counterexample.json
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys'];files={p.name:sha(p) for p in d.iterdir() if p.is_file()}
        with (d/'proof.log').open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT,env=env);row=dict(mode=mode,pid=child.pid,command=cmd,files=files,started=time.time())
            state['cases'].append(row);save();code=child.wait()
        text=(d/'proof.log').read_text();passed=code==0 and ('Induction step proven: SUCCESS!' if mode=='induction' else 'model found: FAIL!') in text
        row.update(passed=passed,returncode=code,finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        if not passed:break
    passed=len(state['cases'])==3 and all(v['passed'] for v in state['cases'])
    state.update(status='PASS_PAIR_DIRECT_CACHE_READY_AND_PERMISSION_INDUCTION' if passed else 'TERMINAL_PAIR_DIRECT_CACHE_NOT_ESTABLISHED',finished=time.time());save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--sparse-pair',action='store_true')
    p.add_argument('--split-pair',action='store_true')
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.yosys.resolve(),a.sparse_pair,a.split_pair))
