#!/usr/bin/env python3
"""Direct-RTL CP-cache safety against the uncached ordered evaluator.

Inputs may change independently on every host step. Starting at reset, every
ready result must match all475 uncached control/event/flag bits, and an unready
request must not reach the SRAM commit interface. Other-board settled status
is arbitrary and may only restrict canonical eligibility. Default mode checks
four bounded steps. Optional induction strengthens the property with exact
cached-payload and canonical-identity invariants; only a completed induction
establishes unbounded digital safety. Neither mode qualifies native pauses,
analog arrival/pulse timing, FPGA timing or software speed.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import time

from probe_cp_prefix_cache import candidate
from prove_memory_mir_rtl import atomic
from test_r_front_forward_view import sha

TOP='''module cp_cache_equivalence(
 input wire clk,reset,request,other_settled,
 input wire [155:0] q,input wire before_root,after_root,
 input wire [397:0] before_pins,after_pins,input wire refresh,
 output wire equivalent,ready,fast_path,cache_hit,
 output wire [474:0] different,output wire ram_permission);
 wire [155:0] old_q,new_q;
 wire [55:0] old_events,new_events,old_repeated,new_repeated,old_cp_events,new_cp_events,old_cp_repeated,new_cp_repeated;
 wire old_unsettled,new_unsettled;
 wire [93:0] old_flags,new_flags;
 wire [268:0] prefix_live,prefix_saved;
 wire [10:0] prefix_cp_saved,after_cp_saved;
 original_ordered original(q,before_root,after_root,before_pins,after_pins,
  old_q,old_events,old_repeated,old_unsettled,refresh,old_flags,old_cp_events,old_cp_repeated);
 dorado_cp_prefix_cache guard(clk,reset,request,q,new_q,before_root,after_root,before_pins,after_pins,
  refresh,other_settled && !new_unsettled,prefix_live,ready,fast_path,cache_hit,
  prefix_saved,prefix_cp_saved,after_cp_saved);
 cached_ordered actual(q,before_root,after_root,before_pins,after_pins,
  new_q,new_events,new_repeated,new_unsettled,refresh,new_flags,new_cp_events,new_cp_repeated,
  fast_path,prefix_saved,prefix_cp_saved,after_cp_saved,prefix_live);
 assign different={old_q,old_events,old_repeated,old_unsettled,old_flags,old_cp_events,old_cp_repeated}^
                  {new_q,new_events,new_repeated,new_unsettled,new_flags,new_cp_events,new_cp_repeated};
 assign ram_permission=RAM_PERMISSION;
 assign equivalent=(!ready || ~|different) && (ready || !ram_permission);
endmodule
'''

INVARIANTS='''
 // Ghost functions inspect auxiliary state without changing the scheduler.
 // A valid cached payload must equal the original prefix function of its
 // complete saved key. A valid canonical context must have an identity prefix.
 wire [155:0] key_q,canonical_check_q;
 wire [55:0] key_events,key_repeated,canonical_events,canonical_repeated;
 wire key_unsettled,canonical_unsettled;
 wire [397:0] key_before=cache_key[410:13];
 wire [397:0] key_after={key_before[397:293],cache_key[11:1],key_before[281:0]};
 dorado_control_hold_atomic_step key_function(cache_key[566:411],cache_key[12],cache_key[12],
  key_before,key_after,key_q,key_events,key_repeated,key_unsettled,cache_key[0],);
 dorado_control_hold_atomic_step canonical_function(canonical_q,canonical_root,canonical_root,
  canonical_pins,canonical_pins,canonical_check_q,canonical_events,canonical_repeated,canonical_unsettled,canonical_refresh,);
 assign invariant=(!cache_valid ||
   ({key_q,key_events,key_repeated,key_unsettled}==prefix_saved && prefix_cp_saved==cache_key[11:1])) &&
  (!canonical_valid ||
   ({canonical_check_q,canonical_events,canonical_repeated,canonical_unsettled}=={canonical_q,113'b0}));
'''


def module(raw,name):
    match=re.search(r'^module '+name+r'\b.*?^endmodule',raw,re.M|re.S)
    assert match,name
    return match[0]+'\n'


def run(experiment,yosys,out,depth,induction=False):
    assert not out.exists()
    r=json.loads((experiment/'run-state.json').read_text());parent=Path(r['parent'])
    raw=(experiment/'candidate.sv').read_text();old=(parent/'candidate.sv').read_text()
    assert raw==candidate(old,r['mutation'],r.get('parallel_control_results',False),r.get('control_only_settled_eligibility',False),r.get('settling_waves',8),r.get('direct_cp_projection',False))
    assert ',host_clk,cp_commit,state_q[751:0],clock_before,clock_after,before_child,after_child,'in raw
    permission=re.findall(r'wire cp_commit=([^;]+);',raw);assert len(permission)==1
    permission=re.sub(r'\badvance\b','request',permission[0]);permission=re.sub(r'\bcp_ready\b','ready',permission)
    assert permission in ['request && ready','request']
    out.mkdir(parents=True)
    cache=module(raw,'dorado_cp_prefix_cache')
    port_init=''' output reg [268:0] prefix_saved=0,
 output reg [10:0] prefix_cp_saved=0,after_cp_saved=0);'''
    body_init=''' output reg [268:0] prefix_saved,
 output reg [10:0] prefix_cp_saved,after_cp_saved);
 initial begin prefix_saved=0;prefix_cp_saved=0;after_cp_saved=0;end'''
    # Yosys's Verilog frontend rejects ANSI output-reg initializers. Lower
    # only these three constant-zero initializers, retaining their values.
    assert cache.count(port_init)==1
    lowered=cache.replace(port_init,body_init,1)
    assert lowered.replace(body_init,port_init,1)==cache
    top=TOP.replace('RAM_PERMISSION',permission)
    if r.get('control_only_settled_eligibility',False):
        assert '  !cu,cp_prefix_live,cp_ready,'in raw
        top=top.replace('refresh,other_settled && !new_unsettled,prefix_live,',
                        'refresh,!new_unsettled,prefix_live,',1)
    if induction:
        # Expose a pure observation of existing private auxiliary state. The
        # inverse must reproduce the syntax-lowered actual module exactly.
        suffix=' output reg [10:0] prefix_cp_saved,after_cp_saved);'
        observed_suffix=' output reg [10:0] prefix_cp_saved,after_cp_saved,output wire invariant);'
        assert lowered.count(suffix)==1
        observed=lowered.replace(suffix,observed_suffix,1).replace('\nendmodule',INVARIANTS+'\nendmodule',1)
        assert observed.replace(INVARIANTS,'',1).replace(observed_suffix,suffix,1)==lowered
        lowered=observed
        top=top.replace('wire [268:0] prefix_live,prefix_saved;', 'wire [268:0] prefix_live,prefix_saved;wire invariant;')
        top=top.replace('prefix_saved,prefix_cp_saved,after_cp_saved);', 'prefix_saved,prefix_cp_saved,after_cp_saved,invariant);',1)
        top=top.replace('assign equivalent=(!ready || ~|different)', 'assign equivalent=invariant && (!ready || ~|different)',1)
    sources={
        'atomic.sv':atomic(raw,'dorado_control_hold_atomic_step')+'\n',
        'original.sv':module(old,'dorado_control_hold_ordered_step').replace('module dorado_control_hold_ordered_step ', 'module original_ordered ',1),
        'cached.sv':module(raw,'dorado_control_hold_ordered_step').replace('module dorado_control_hold_ordered_step ', 'module cached_ordered ',1),
        'cache.sv':lowered,
        'top.sv':top,
    }
    for name,text in sources.items():(out/name).write_text(text)
    flag='falsify'if r['mutation']else'verify'
    strategy=(f'-seq 2 -tempinduct -maxsteps {depth} -set-init-zero' if induction else
              f'-seq {depth} -set-init-zero -set-at 1 reset 1')
    script='''read_verilog -sv /work/atomic.sv /work/original.sv /work/cached.sv /work/cache.sv /work/top.sv
hierarchy -check -top cp_cache_equivalence
proc
flatten
opt
check -assert
sat STRATEGY -timeout 120 -prove equivalent 1 -FLAG -show-ports -dump_json /work/counterexample.json
'''.replace('STRATEGY',strategy).replace('FLAG',flag)
    (out/'proof.ys').write_text(script);(out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
    (out/'version.txt').write_text(subprocess.check_output([str(yosys),'-V'],text=True,env=env))
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,
        experiment=str(experiment),experiment_state_sha256=sha(experiment/'run-state.json'),mutation=r['mutation'],
        compared_control_output_bits=475,arbitrary_input_bits_per_step=958,bounded_host_steps=None if induction else depth,
        induction_requested=induction,maximum_induction_steps=depth if induction else None,
        only_input_constraint='none; all inputs/reset histories arbitrary' if induction else 'reset=1 on first host step; all other inputs/reset history arbitrary',
        initial_auxiliary_state='zero, matching explicit RTL power-up values',
        output_initializer_syntax_lowering=dict(constant_zero_outputs=3,inverse_byte_identical=True),
        source_bound_SRAM_permission=permission,source_files={p.name:sha(p)for p in out.iterdir()if p.is_file()},
        parallel_control_results=r.get('parallel_control_results',False),
        control_only_settled_eligibility=r.get('control_only_settled_eligibility',False),
        settling_waves=r.get('settling_waves',8),
        direct_cp_projection=r.get('direct_cp_projection',False),
        unbounded_induction_proven=False,native_qualified=False,physical_timing_qualified=False,speedup_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    command=[str(yosys),'-Q','-T','-s','/work/proof.ys']
    with(out/'proof.log').open('x')as log:
        process=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT,env=env)
        state.update(pid=process.pid,command=command);save()
        code=process.wait()
    text=(out/'proof.log').read_text();success=('Induction step proven: SUCCESS!' if induction else 'SAT proof finished - no model found: SUCCESS!')in text
    counterexample='SAT proof finished - model found: FAIL!'in text
    passed=code==0 and (counterexample if r['mutation']else success)
    status=('PASS_CP_CACHE_INDUCTIVE_READY_EQUIVALENCE' if induction else 'PASS_BOUNDED_CP_CACHE_READY_EQUIVALENCE')
    state.update(status=('PASS_BOUNDED_CP_CACHE_COUNTEREXAMPLE'if r['mutation']else status)if passed else'TERMINAL_CP_CACHE_PROOF_NOT_ESTABLISHED',
        returncode=code,finished=time.time(),proof_succeeded=success,counterexample_found=counterexample,
        unbounded_induction_proven=bool(induction and passed and not r['mutation']),
        proof_log_sha256=sha(out/'proof.log'))
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['experiment','yosys','output']:parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--depth',type=int,choices=range(2,9),default=4)
    parser.add_argument('--induction',action='store_true')
    args=parser.parse_args();raise SystemExit(run(args.experiment.resolve(),args.yosys.resolve(),args.output.resolve(),args.depth,args.induction))
