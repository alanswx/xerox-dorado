#!/usr/bin/env python3
"""Compute the complete R-fault bit in two parallel late-FF decode cofactors.

Only the SINGLE processor invocation changes. Existing state, RAM, guards,
clocks and every other result remain byte-identical. Both cofactors invoke the
actual pure child with identical inputs except pins_after209 fixed0/1. A known
late bit selects the exact matching invocation; X/Z returns the original fault.
No assumption about independence of that bit, no added guest cycle or storage.
"""
import re
from generate_cached_r_packet import stage as original_stage
from generate_split_r_packet import packet as split_packet

INPUTS=[('stop_tasks_capture',1),('canonical_task_clock',4),('host_clk',1),('advance',1),('state_q',752),
        ('clock_before',1),('clock_after',1),('pins_before',246),('pins_after',246),
        ('front_capture_pins',246),('external_file_read',18)]
PORTS='''stop_tasks_capture canonical_task_clock host_clk advance state_q clock_before clock_after pins_before pins_after
state_next clock_events repeated_events unsettled write_n latch_n t_latch_n bypass parity_ok faults r_read_n r_visible_n
r_flags rm_flags_n address select_n forwarded_bypass r_write_n port_address port_select_n file_read_data file_read_parity
source_pins fa_n ready_flags current_forwarded_bypass r_flag_clocks_after r_flag_sets_after front_capture_pins external_file_read file_write_data'''.split()
CHILD='dorado_local_r_front_ready_step'


def input_declarations():
    return ',\n '.join('input wire '+(f'[{w-1}:0] ' if w>1 else '')+n for n,w in INPUTS)


def child_call(instance,after,faults):
    inputs={n:n for n,w in INPUTS};inputs['pins_after']=after
    return ' '+CHILD+' '+instance+'(\n  '+',\n  '.join('.'+n+'('+v+')' for n,v in [*inputs.items(),('faults',faults)])+');'


def helper(mutation=None):
    zero="{pins_after[245:210],1'b0,pins_after[208:0]}"
    one="{pins_after[245:210],1'b1,pins_after[208:0]}"
    low='low_faults[1]';high='high_faults[1]';fallback='original_fault'
    if mutation=='invert-low':low='!low_faults[1]'
    if mutation=='stale-low':zero="{pins_before[245:210],1'b0,pins_before[208:0]}"
    if mutation=='bad-unknown':fallback='!original_fault'
    return '''module dorado_parallel_r_fault_step(
 DECLARATIONS,
 input wire original_fault,output reg fault);
 wire [3:0] low_faults,high_faults;
 LOW
 HIGH
 always @*begin
  case(pins_after[209])
   1'b0:fault=LOW_VALUE;
   1'b1:fault=HIGH_VALUE;
   default:fault=FALLBACK;
  endcase
 end
endmodule
'''.replace('DECLARATIONS',input_declarations()).replace('LOW\n',child_call('early_zero',zero,'low_faults')+'\n').replace(
        'HIGH\n',child_call('early_one',one,'high_faults')+'\n').replace('LOW_VALUE',low).replace('HIGH_VALUE',high).replace('FALLBACK',fallback)


def positional(text):
    args=[];start=0;depth=0
    for i,c in enumerate(text):
        if c in '({[':depth+=1
        elif c in ')}]':depth-=1;assert depth>=0
        elif c==',' and depth==0:args.append(text[start:i].strip());start=i+1
    assert depth==0;args.append(text[start:].strip());return args


def validate_child_signature(raw):
    header=raw.split('module '+CHILD+'(',1)[1].split(');',1)[0]
    names=[];inputs=[];direction=None;width=1
    for token in header.split(','):
        token=token.strip();m=re.match(r'(input|output)\s+(?:wire|reg)\s+(?:\[(\d+):0\]\s*)?(\w+)$',token)
        if m:direction=m[1];width=int(m[2])+1 if m[2] else 1;name=m[3]
        else:assert re.fullmatch(r'\w+',token),token;name=token
        names.append(name)
        if direction=='input':inputs.append((name,width))
    assert names==PORTS and inputs==INPUTS,(names,inputs)


def stage(raw,mutation=None):
    original=original_stage(raw,True);actual=original
    old='module dorado_r_front_prefix_step #';new='module dorado_r_front_parallel_fault_step #'
    assert actual.count(old)==1;actual=actual.replace(old,new,1)
    old_decl=' wire [3:0] child_parity_ok,child_faults;'
    new_decl=old_decl+'\n wire parallel_r_fault;'
    assert actual.count(old_decl)==1;actual=actual.replace(old_decl,new_decl,1)
    # One actual output bit is replaced. Its original calculation remains for
    # the exact unknown-selector fallback and all existing child results.
    old_fault='child_faults[1],!(current_t_bypass[0]|capture_pins_before[117]|physical_t_ok[0])'
    new_fault='parallel_r_fault,!(current_t_bypass[0]|capture_pins_before[117]|physical_t_ok[0])'
    assert actual.count(old_fault)==1;actual=actual.replace(old_fault,new_fault,1)
    start=actual.index(' '+CHILD+' child(');end=actual.index(');',start)+2
    call=actual[start:end];args=positional(call.split(' child(',1)[1][:-2]);assert len(args)==len(PORTS)
    bindings=dict(zip(PORTS,args))
    assert bindings['faults']=='child_faults'
    additions=' dorado_parallel_r_fault_step early_fault(\n  '+',\n  '.join('.'+n+'('+bindings[n]+')' for n,w in INPUTS)+',\n  .original_fault(child_faults[1]),\n  .fault(parallel_r_fault));\n'
    actual=actual[:start]+additions+actual[start:]
    assert actual.replace(additions,'',1).replace(new_fault,old_fault,1).replace(new_decl,old_decl,1).replace(new,old,1)==original
    return actual


def packet(raw,mutation=None,pair_direct=True,sparse_pair=True):
    top=split_packet(raw,mutation,pair_direct,sparse_pair)
    old=' dorado_r_front_prefix_step single(';new=' dorado_r_front_parallel_fault_step single('
    assert top.count(old)==1;changed=top.replace(old,new,1);assert changed.replace(new,old,1)==top
    return changed+'\n'+stage(raw,mutation)+'\n'+helper(mutation)
