#!/usr/bin/env python3
"""Prove packet canonical specialization under actual native publication.

All1416 cache bits and ready/fast/hit remain equal for arbitrary binary
histories, external refresh, visible-R aliases and uninterpreted next/prefix
data. Architectural owner/context power-up is unconstrained; only actual cache
initializers apply. Preserve the whole-packet final root and generic prefix key.
Required bad publication/refresh controls must change real cache interfaces.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from generate_owned_r_packet import cache,caller,publication_projection,native_publication
from generate_cached_r_packet import projection
from prove_cp_prefix_cache import module
from prove_native_cp_context import BP
from test_r_front_forward_view import sha

TOP='''module owned_packet_cache_equivalence(
 input wire storage_clk,reset,contb_advance,other_guard,use_pair,clock_after,refresh,settled,
 input wire [843:0] next_held,input wire [400:0] pins_after,
 input wire [7:0] r_front_visible_r_ProcH,r_front_visible_r_ProcL,
 input wire [268:0] prefix_live,
 output wire equivalent,interfaces_equal,original_ready,native_ready,original_fast,native_fast,
 output wire original_hit,native_hit,original_matches,native_matches);
 reg [571:0] r_front_lower_q;reg [255:0] r_front_upper_q;
 reg [400:0] r_front_before;reg r_front_clock_before;
 wire [843:0] r_front_held=HELD;
 wire [843:0] r_front_next=next_held;
 wire [400:0] r_front_live=pins_after;wire CLK_ca_p_=clock_after;
 wire control_ready=original_ready && other_guard;
 PUB
 PROJECTION
 wire [155:0] current_q=control_projection(r_front_held),next_q=control_projection(next_held);
 wire first_after=use_pair ? 1'b0:clock_after;
 wire [1415:0] original_state,native_state;
 wire [268:0] original_saved,native_saved;wire [10:0] original_cp,native_cp,original_after,native_after;
 generic_cache original(storage_clk,reset,contb_advance && control_ready,current_q,next_q,
  r_front_clock_before,first_after,r_front_before[397:0],pins_after[397:0],refresh,settled,prefix_live,
  original_ready,original_fast,original_hit,original_saved,original_cp,original_after,clock_after,original_state,original_matches);
 specialized_cache actual(storage_clk,reset,contb_advance && native_ready && other_guard,current_q,next_q,
  r_front_clock_before,first_after,r_front_before[397:0],pins_after[397:0],refresh,settled,prefix_live,
  native_ready,native_fast,native_hit,native_saved,native_cp,native_after,clock_after,native_state,native_matches);
 assign interfaces_equal=({original_ready,original_fast,original_hit,original_saved,original_cp,original_after}==
                          {native_ready,native_fast,native_hit,native_saved,native_cp,native_after});
 assign equivalent=interfaces_equal && original_state==native_state && original_matches && native_matches;
endmodule
'''
OBSERVE='''
 assign context_state={canonical_valid,cache_valid,canonical_q,canonical_pins,canonical_root,canonical_refresh,
  cache_key,prefix_saved,prefix_cp_saved,after_cp_saved};
 assign owner_matches=!canonical_valid ||
  ((current_q==canonical_q)&&(before_pins==canonical_pins)&&(before_root==canonical_root));
'''


def observed(text,name):
    old=''' output reg [268:0] prefix_saved=0,
 output reg [10:0] prefix_cp_saved=0,after_cp_saved=0,input wire committed_root);'''
    new=''' output reg [268:0] prefix_saved,
 output reg [10:0] prefix_cp_saved,after_cp_saved,input wire committed_root,
 output wire [1415:0] context_state,output wire owner_matches);
 initial begin prefix_saved=0;prefix_cp_saved=0;after_cp_saved=0;end'''
    assert text.count(old)==1
    changed=text.replace(old,new,1).replace('module dorado_packet_prefix_cache(',f'module {name}(',1).replace('\nendmodule',OBSERVE+'\nendmodule',1)
    assert changed.replace(OBSERVE,'',1).replace(f'module {name}(','module dorado_packet_prefix_cache(',1).replace(new,old,1)==text
    return changed


def make_top(raw,bp,mutation=None):
    wrapper=caller(raw,bp,mutation)
    block=re.findall(r'  always @\(posedge storage_clk\)if\(contb_advance && control_ready\)begin\n.*?\n  end',wrapper,re.S)
    assert len(block)==1;_,held=native_publication(bp)
    return TOP.replace('HELD',held).replace('PUB',block[0]).replace('PROJECTION',projection(raw))


def run(source,native,out,yosys):
    assert not out.exists();s=json.loads((source/'run-state.json').read_text())
    assert s['status']=='PASS_ENTRY_CACHED_FULL_PROCESSOR_PACKETS' and s['packet_variant']=='split-parallel-fault'
    for n,h in s['files'].items():assert sha(source/n)==h
    for row in s['source_files']:assert sha(source/row['path'])==row['output_sha256']
    manifest=native/'r-front-control-native.json';n=json.loads(manifest.read_text());assert sha(native/BP)==n['source_hashes'][BP]
    raw=(source/'candidate.sv').read_text();bp=(native/BP).read_text();native_publication(bp)
    out.mkdir(parents=True)
    files={'caller.sv':caller(raw,bp),'generic-cache.sv':cache(raw),'specialized-cache.sv':cache(raw,True),
           'native-backplane.v':bp,'packet.sv':(source/'packet.sv').read_text()}
    for name,text in files.items():(out/name).write_text(text)
    for name in [Path(__file__).name,'generate_owned_r_packet.py']:shutil.copy2(Path(__file__).with_name(name),out/name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),source_state_sha256=sha(source/'run-state.json'),
        native_stage=str(native),native_manifest_sha256=sha(manifest),native_backplane_sha256=sha(native/BP),
        cache_state_bits_compared=1416,physical_control_projection_bits=156,stored_owner_bits=828,original_external_R_bits=16,
        architectural_power_up_state='unconstrained',cache_power_up_state='exact explicit RTL initializers',
        external_refresh_histories_unconstrained=True,prefix_key_unchanged=True,whole_packet_final_root_used=True,
        owner_guard_generalization='entry_ready AND arbitrary same packet guard; actual packet guard implies entry_ready',
        files={p.name:sha(p) for p in out.iterdir() if p.is_file()},cases=[],native_integrated=False,hardware_changed=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for mode in ['projection','induction','stale-owner','stale-pins','stale-final-root','drop-refresh']:
        d=out/mode;d.mkdir()
        if mode=='projection':
            (d/'top.sv').write_text(publication_projection(raw,bp));sources=['top.sv'];entity='owned_packet_projection';signal='equal';strategy='';flag='verify'
        else:
            (d/'generic.sv').write_text(observed(cache(raw),'generic_cache'))
            (d/'actual.sv').write_text(observed(cache(raw,True,mode),'specialized_cache'))
            (d/'top.sv').write_text(make_top(raw,bp,mode))
            sources=['generic.sv','actual.sv','top.sv'];entity='owned_packet_cache_equivalence'
            signal='equivalent' if mode=='induction' else 'interfaces_equal'
            strategy='-seq 2 -tempinduct -maxsteps 4' if mode=='induction' else '-seq 4'
            flag='verify' if mode=='induction' else 'falsify'
        (d/'proof.ys').write_text('read_verilog -sv '+' '.join('/work/'+v for v in sources)+f'''
hierarchy -check -top {entity}
proc
flatten
opt
check -assert
sat {strategy} -timeout 120 -prove {signal} 1 -{flag} -show-ports -dump_json /work/counterexample.json
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys'];row=dict(mode=mode,command=cmd,files={p.name:sha(p) for p in d.iterdir()},started=time.time())
        with (d/'proof.log').open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT,env=env);row['pid']=child.pid;state['cases'].append(row);save();code=child.wait()
        text=(d/'proof.log').read_text();marker=('Induction step proven: SUCCESS!' if mode=='induction' else 'no model found: SUCCESS!') if flag=='verify' else 'model found: FAIL!'
        row.update(passed=code==0 and marker in text,returncode=code,finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        if not row['passed']:break
    passed=len(state['cases'])==6 and all(v['passed'] for v in state['cases'])
    state.update(status='PASS_SOURCE_BOUND_OWNED_PACKET_CANONICAL_CONTEXT' if passed else 'TERMINAL_OWNED_PACKET_CONTEXT_NOT_ESTABLISHED',finished=time.time());save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','native','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.native.resolve(),a.output.resolve(),a.yosys.resolve()))
