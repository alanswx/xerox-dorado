#!/usr/bin/env python3
"""Separate unconditional paired-first data from the staged single fallback.

Three combinational function invocations, ONE architectural/RM-STK owner.
The second event consumes only the unconditional fall's intermediate owners
and forwarding. A separate ordinary invocation supplies singles and the raw
real-input prefix for every history, even while a pair is rejected. Preserve
all original memory/geometry/error guards and one final architectural commit.
This generator is an experiment; proofs/replay/full timing are still required.
"""
from generate_cached_r_packet import packet as original_packet
from trace_r_front_transactions import layout


def packet(raw,mutation=None,pair_direct=True,sparse_pair=True):
    assert pair_direct and sparse_pair
    actual=original_packet(raw,mutation,True,True);original=actual;changes=[]
    def change(old,new):
        nonlocal actual
        assert actual.count(old)==1,old
        actual=actual.replace(old,new,1);changes.append((old,new))
    ports,bits=layout(raw)
    decl='\n'.join(f' wire {w} single_{n};' for n,w in ports)
    change(' wire first_after=use_pair ?',decl+'\n wire first_after=use_pair ?')
    start=actual.index(' dorado_r_front_prefix_step first(');end=actual.index(');',start)+2
    old_call=actual[start:end];single=old_call.replace('prefix_step first(','prefix_step single(',1)
    for n,w in ports:single=single.replace('('+f'first_{n}'+')','('+f'single_{n}'+')',1)
    single=single.replace('.control_owner(first_owner)','.control_owner()',1).replace('.pair_fall(use_pair)',".pair_fall(1'b0)",1)
    single=single.replace('.entry_fast((use_pair || entry_fast))','.entry_fast(entry_fast)',1)
    fall=old_call
    for a,b in [('.clock_before(clock_before)',".clock_before(1'b1)"),('.clock_after(first_after)',".clock_after(1'b0)"),
                ('.pins_before(pins_before)','.pins_before(pins_after)'),('.entry_fast((use_pair || entry_fast))',".entry_fast(1'b1)"),
                ('.live_prefix(first_live_prefix)','.live_prefix()'),('.pair_fall(use_pair)',".pair_fall(1'b1)")]:
        assert fall.count(a)==1;fall=fall.replace(a,b,1)
    change(old_call,single+'\n'+fall)
    change('control_projection(use_pair ? second_held_next:first_held_next)',
           'control_projection(use_pair ? second_held_next:single_held_next)')
    change('!first_unsettled && (!use_pair || !second_unsettled)',
           '!(use_pair ? first_unsettled:single_unsettled) && (!use_pair || !second_unsettled)')
    first='{'+','.join('first_'+n for n,w in ports)+'}';singles='{'+','.join('single_'+n for n,w in ports)+'}'
    change(' assign first_result='+first+';',' assign first_result=use_pair ? '+first+' : '+singles+';')
    second='{'+','.join('second_'+n for n,w in ports)+'}'
    change(' assign last_result=use_pair ? '+second+' : '+first+';',
           ' assign last_result=use_pair ? '+second+' : '+singles+';')
    # The physical SRAM sees the selected FIRST event, never the second event's
    # uncommitted ports. Forwarding uses the unconditional paired-first data.
    start=actual.index(' dorado_r_file_ports files(');end=actual.index(');',start)+2
    old_files=actual[start:end];new_files=old_files
    selected=[]
    for n,w in [('r_write_n','[1:0]'),('port_address','[31:0]'),('port_select_n','[3:0]'),('file_write_data','[15:0]')]:
        name='owned_'+n
        selected.append(f' wire {w} {name}=use_pair ? first_{n}:single_{n};')
        assert 'first_'+n in new_files;new_files=new_files.replace('first_'+n,name)
    change(old_files,'\n'.join(selected)+'\n'+new_files)
    # Explicitly bind every edit; no hidden guard/state/memory transformation.
    restored=actual
    for old,new in reversed(changes):
        assert restored.count(new)==1;restored=restored.replace(new,old,1)
    assert restored==original
    return actual
