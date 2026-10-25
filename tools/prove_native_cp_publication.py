#!/usr/bin/env python3
"""Prove source-closed CP history under the actual native publication guard.

All architectural owners and before-context power up unconstrained. Only the
real cache initializers apply. Its actual canonical_valid implies that the
published full401-pin snapshot has the original CP source relationships.
Compare all1416 cache bits and real interfaces; do not change the retained
generic prefix key or assume its older contents match current publication.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from generate_owned_r_packet import cache
from prove_owned_r_packet import make_top,observed
from review_baseboard_clock_consumer import verify
from test_r_front_forward_view import sha


def top(raw,bp,fn,mutation=None):
    original=make_top(raw,bp)
    actual=original.replace('input wire [843:0] next_held,input wire [400:0] pins_after,',
                            'input wire [843:0] next_held,input wire [400:0] raw_after,',1)
    marker=' reg [571:0] r_front_lower_q;'
    assert actual.count(marker)==1
    actual=actual.replace(marker,fn+' wire [400:0] pins_after=cp_source_pins(raw_after,cp_bus(raw_after));\n'+marker,1)
    check=' assign equivalent=interfaces_equal && original_state==native_state && original_matches && native_matches;'
    assert actual.count(check)==1
    actual=actual.replace(check,''' wire source_closed=(r_front_before==cp_source_pins(r_front_before,cp_bus(r_front_before)));
 assign equivalent=interfaces_equal && original_state==native_state && original_matches && native_matches &&
  (!original_state[1415] || source_closed);''',1)
    pub='r_front_before<=r_front_live;'
    assert actual.count(pub)==1
    if mutation=='raw-decoder-snapshot':
        actual=actual.replace(pub,'r_front_before<={r_front_live[400:282],raw_after[281:264],r_front_live[263:0]};',1)
    elif mutation=='wrong-CPOut8-snapshot':
        actual=actual.replace(pub,"r_front_before<=cp_source_pins(r_front_live,{cp_bus(r_front_live)[12:9],r_front_live[285],cp_bus(r_front_live)[7:0]});",1)
        # Avoid indexing a function-call expression on older Verilog parsers.
        actual=actual.replace(' wire control_ready=', ' wire [12:0] live_cp=cp_bus(r_front_live);\n wire control_ready=',1)
        actual=actual.replace('cp_bus(r_front_live)[12:9]','live_cp[12:9]').replace('cp_bus(r_front_live)[7:0]','live_cp[7:0]')
    return actual


def run(source,owner_proof,cp_source,cp_proof,out,yosys):
    assert not out.exists()
    s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
    assert s['status']=='PASS_OWNED_FULL_PROCESSOR_PACKETS' and s['mutation'] is None
    owner=json.loads((owner_proof/'run-state.json').read_text());verify(owner_proof,owner['files'])
    assert owner['status']=='PASS_SOURCE_BOUND_OWNED_PACKET_CANONICAL_CONTEXT'
    c=json.loads((cp_source/'manifest.json').read_text());verify(cp_source,c['files'])
    p=json.loads((cp_proof/'run-state.json').read_text());verify(cp_proof,p['files'])
    assert p['status']=='PASS_ORIGINAL_NATIVE_CP_SOURCE_PROJECTION' and p['source_manifest_sha256']==sha(cp_source/'manifest.json')
    for row in p['actions']:
        assert row['returncode']==0 and sha(cp_proof/(row['label']+'.log'))==row['log_sha256']
    raw=(source/'candidate.sv').read_text();bp=(owner_proof/'native-backplane.v').read_text();fn=(cp_source/'functions.sv').read_text()
    out.mkdir(parents=True)
    for name in [Path(__file__).name,'prove_owned_r_packet.py','generate_owned_r_packet.py']:
        shutil.copy2(Path(__file__).with_name(name),out/name)
    (out/'functions.sv').write_text(fn)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),
               source_state_sha256=sha(source/'run-state.json'),owner_proof_state_sha256=sha(owner_proof/'run-state.json'),
               CP_source_manifest_sha256=sha(cp_source/'manifest.json'),CP_proof_state_sha256=sha(cp_proof/'run-state.json'),
               architectural_owner_and_context_powerup_unconstrained=True,cache_state_bits_compared=1416,
               before_pin_bits_compared=401,canonical_valid_is_actual_cache_bit=1415,
               retained_prefix_key_unchanged=True,files={p.name:sha(p) for p in out.iterdir()},cases=[],
               native_integrated=False,arrival_order_qualified=False,hardware_changed=False)
    def save():
        (out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for mode in ['induction','raw-decoder-snapshot','wrong-CPOut8-snapshot']:
        d=out/mode;d.mkdir()
        (d/'generic.sv').write_text(observed(cache(raw),'generic_cache'))
        (d/'actual.sv').write_text(observed(cache(raw,True),'specialized_cache'))
        (d/'top.sv').write_text(top(raw,bp,fn,None if mode=='induction' else mode))
        signal='equivalent' if mode=='induction' else 'interfaces_equal'
        strategy='-seq 2 -tempinduct -maxsteps 4' if mode=='induction' else '-seq 4'
        flag='verify' if mode=='induction' else 'falsify'
        (d/'proof.ys').write_text(f'''read_verilog -sv /work/generic.sv /work/actual.sv /work/top.sv
hierarchy -check -top owned_packet_cache_equivalence
proc
flatten
opt
check -assert
sat {strategy} -timeout 120 -prove {signal} 1 -{flag} -show-ports -dump_json /work/counterexample.json
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        row=dict(mode=mode,command=cmd,started=time.time(),files={p.name:sha(p) for p in d.iterdir()})
        with (d/'proof.log').open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT,env=env)
            row['pid']=child.pid;state['cases'].append(row);save();code=child.wait()
        log=(d/'proof.log').read_text();marker='Induction step proven: SUCCESS!' if mode=='induction' else 'model found: FAIL!'
        row.update(returncode=code,passed=code==0 and marker in log,finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        if not row['passed']:break
    passed=len(state['cases'])==3 and all(r['passed'] for r in state['cases'])
    state.update(status='PASS_NATIVE_CP_SOURCE_PUBLICATION_INDUCTION' if passed else 'TERMINAL_CP_SOURCE_PUBLICATION_NOT_ESTABLISHED',finished=time.time())
    save();print(state['status'],flush=True)
    return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','owner-proof','cp-source','cp-proof','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.owner_proof.resolve(),a.cp_source.resolve(),a.cp_proof.resolve(),a.output.resolve(),a.yosys.resolve()))
