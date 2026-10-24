#!/usr/bin/env python3
"""Bind split packet calls to the previously qualified sparse-first function.

Under the ORIGINAL accepted pair geometry, every input of the dedicated fall
is identical to the old first invocation. For singles every input of the new
single invocation is identical to the old first invocation. The second call
is unchanged and consumes the paired fall's complete intermediate result.
The raw prefix inputs remain identical even outside that geometry; the source
prefix producer is independently audited to ignore fast/cached selectors.
This is a source-level input-substitution certificate, not a new CPU theorem,
physical timing result or proof that actual device pins are stable in a packet.
"""
import re
from generate_cached_r_packet import packet as old_packet,stage
from trace_r_front_transactions import layout

INPUTS=['host_clk','advance','held_q','clock_before','clock_after','pins_before','pins_after',
        'refresh_before_run_capture','cp_reset','external_file_read','entry_fast','entry_prefix',
        'entry_prefix_cp','entry_after_cp','pair_fall']


def bindings(text,name):
    body=text.split(' dorado_r_front_prefix_step '+name+'(',1)[1].split(');',1)[0]
    result={}
    for line in body.splitlines():
        if not line.strip():continue
        m=re.fullmatch(r'\s*\.(\w+)\((.*)\),?',line);assert m,line
        assert m[1] not in result;result[m[1]]=m[2]
    return result


def normalize(value,pair):
    # These identities follow directly from use_pair and the unchanged ready
    # clause: clock_before && clock_after && full401-pin before==after.
    return {'(use_pair || entry_fast)':"1'b1" if pair else 'entry_fast',
            'use_pair':"1'b1" if pair else "1'b0",
            'first_after':"1'b0" if pair else 'clock_after',
            'clock_before':"1'b1" if pair else 'clock_before',
            'pins_before':'pins_after' if pair else 'pins_before'}.get(value,value)


def audit(raw,actual):
    old=old_packet(raw,pair_direct=True,sparse_pair=True)
    reference=bindings(old,'first');single=bindings(actual,'single');fall=bindings(actual,'first')
    for pair,call in [(False,single),(True,fall)]:
        for n in INPUTS:assert normalize(reference[n],pair)==normalize(call[n],pair),(pair,n,reference[n],call[n])
    assert bindings(actual,'second')==bindings(old,'second')
    guard=r' always @\*begin\n  ready=.*?\n end\n'
    assert re.findall(guard,actual,re.S)==re.findall(guard,old,re.S) and len(re.findall(guard,old,re.S))==1
    assert 'clock_before && clock_after && pins_before==pins_after' in actual
    for n in ['held_q','clock_before','clock_after','pins_before','pins_after','refresh_before_run_capture']:
        assert single[n]==reference[n],n
    assert single['live_prefix']=='first_live_prefix' and fall['live_prefix']==''
    assert fall['control_owner']=='first_owner'
    fn=stage(raw,True)
    assert 'assign control_owner=joint_q;assign live_prefix=cp_prefix_live;' in fn
    assert 'control(joint_q,clock_before,clock_after,pins_before[397:0],pins_after[397:0],' in fn
    assert actual.count(' dorado_r_file_ports files(')==1
    for n,w in [('r_write_n','[1:0]'),('port_address','[31:0]'),('port_select_n','[3:0]'),('file_write_data','[15:0]')]:
        assert f' wire {w} owned_{n}=use_pair ? first_{n}:single_{n};' in actual
    old_memory=old.split(' dorado_r_file_ports files(',1)[1].split(');',1)[0]
    new_memory=actual.split(' dorado_r_file_ports files(',1)[1].split(');',1)[0]
    assert new_memory.replace('owned_','first_')==old_memory
    ports,_=layout(raw)
    pack=lambda p:'{'+','.join(p+'_'+n for n,w in ports)+'}'
    assert ' assign first_result=use_pair ? '+pack('first')+' : '+pack('single')+';' in actual
    assert ' assign last_result=use_pair ? '+pack('second')+' : '+pack('single')+';' in actual
    assert 'control_projection(use_pair ? second_held_next:single_held_next)' in actual
    assert '!(use_pair ? first_unsettled:single_unsettled) && (!use_pair || !second_unsettled)' in actual
    return dict(status='PASS_SOURCE_BOUND_SPLIT_INVOCATION_INPUT_SUBSTITUTION',inputs_checked_per_first_invocation=len(INPUTS),
        all_second_bindings_unchanged=True,raw_prefix_inputs_unchanged_for_all_histories=True,
        ready_guard_unchanged=True,one_physical_SRAM_owner_and_same_commit_permission=True,
        source_or_device_interior_pin_stability_qualified=False,native_timing_or_speed_qualified=False)
