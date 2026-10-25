#!/usr/bin/env python3
"""Bind a field-only R-fault substitution to its proofs and unchanged owners.

The original stage reads its faults port nowhere internally. Only that output
bit changes; the unchanged ready/cache/owner/SRAM cones therefore reuse the
qualified split packet history theorem. Separately require real child binary
proof and X/Z tests for the new field, with exact actual invocation inputs.
"""
import json
from pathlib import Path
import re
from audit_split_r_packet import audit as audit_split
from generate_cached_r_packet import stage as base_stage,cache
from generate_parallel_r_fault import INPUTS,PORTS,CHILD,stage,helper,positional,validate_child_signature
from generate_split_r_packet import packet as split_packet
from prove_cp_prefix_cache import module
from test_r_front_forward_view import sha


def verify(d,files):
    for n,h in files.items():assert sha(d/n)==h,(d,n)


def evidence(proof,four_state,bad_unknown,source):
    s=json.loads((proof/'run-state.json').read_text());verify(proof,s['files'])
    assert s['status']=='PASS_PARALLEL_R_FAULT_ALL_BINARY_INPUTS' and s['arbitrary_binary_input_bits']==1517
    assert [v['mode'] for v in s['cases']]==['positive','invert-low','stale-low']
    for row in s['cases']:
        d=proof/row['mode'];verify(d,row['files']);assert row['passed'] and row['returncode']==0 and sha(d/'proof.log')==row['log_sha256']
    source_state=json.loads((source/'run-state.json').read_text())
    for row in source_state['source_files']:assert sha(source/row['path'])==row['output_sha256']==s['files']['inputs/'+row['path']]
    validate_child_signature((source/'adapters/dorado_local_r_front_ready_step.sv').read_text())
    assert (proof/'positive/helper.sv').read_text()==helper()
    for d,status in [(four_state,'PASS_PARALLEL_FAULT_FOUR_STATE'),(bad_unknown,'PASS_PARALLEL_FAULT_UNKNOWN_COUNTEREXAMPLE')]:
        t=json.loads((d/'run-state.json').read_text());verify(d,t['files'])
        assert t['status']==status and t['proof_state_sha256']==sha(proof/'run-state.json') and t['actions'][0]['returncode']==0
        assert sha(d/'test.log')==t['test_log_sha256']
        if d==four_state:
            assert t['actions'][-1]['returncode']==0
            assert 'inputs=1517 known=8 unknown=12136 selector_unknown=8 low=6068 high=6068 checks=12144' in (d/'test.log').read_text()
        else:
            assert t['actions'][-1]['returncode']!=0
            assert 'FAULT_COFACTOR mismatch trial=0 bit=473 mode=2 late=x actual=0 expected=1' in (d/'test.log').read_text()
    return dict(binary_proof_state_sha256=sha(proof/'run-state.json'),four_state_sha256=sha(four_state/'run-state.json'),bad_unknown_sha256=sha(bad_unknown/'run-state.json'))


def audit(raw,actual):
    top=module(actual,'dorado_r_front_packet_step');old_top=module(split_packet(raw),'dorado_r_front_packet_step')
    old=' dorado_r_front_prefix_step single(';new=' dorado_r_front_parallel_fault_step single('
    assert top.count(new)==1 and top.replace(new,old,1)==old_top
    base=audit_split(raw,top.replace(new,old,1))
    original=base_stage(raw,True);changed=stage(raw)
    assert len(re.findall(r'\bfaults\b',original))==2 # declaration and output assignment; no feedback
    assert module(actual,'dorado_r_front_prefix_step')==module(original,'dorado_r_front_prefix_step')
    assert module(actual,'dorado_r_front_parallel_fault_step')==module(changed,'dorado_r_front_parallel_fault_step')
    assert module(actual,'dorado_parallel_r_fault_step')==module(helper(),'dorado_parallel_r_fault_step')
    assert module(actual,'dorado_packet_prefix_cache')==module(cache(raw),'dorado_packet_prefix_cache')
    old_call=original.split(' '+CHILD+' child(',1)[1].split(');',1)[0]
    args=positional(old_call);assert len(args)==len(PORTS);reference=dict(zip(PORTS,args))
    call=changed.split(' dorado_parallel_r_fault_step early_fault(',1)[1].split(');',1)[0]
    bindings={}
    for line in call.splitlines():
        if not line.strip():continue
        m=re.fullmatch(r'\s*\.(\w+)\((.*)\),?',line);assert m,line;assert m[1] not in bindings;bindings[m[1]]=m[2]
    assert bindings=={**{n:reference[n] for n,w in INPUTS},'original_fault':'child_faults[1]','fault':'parallel_r_fault'}
    assert len(re.findall(r'\bparallel_r_fault\b',changed))==3
    return dict(status='PASS_FIELD_ONLY_PARALLEL_FAULT_AND_UNCHANGED_SPLIT_CONES',
        base_invocation_substitution=base,all11_child_inputs_bound_to_actual_invocation=True,
        cache_ready_owner_SRAM_input_cones_unchanged=True,only_single_fault_bit1_changed=True,
        new_state_or_guest_cycles=0,source_device_timestamps_or_native_timing_qualified=False)
