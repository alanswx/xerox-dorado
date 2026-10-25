#!/usr/bin/env python3
"""Compose corrected CP captures, ordinary pairs and a tagged single SRAM.

Experimental integration. Build ALL processor invocations from the same
pre-sharing source, then install the existing shared capture descriptors and
actual CP producer views. Preserve sparse paired fall/forwarding and the2470
original result bits. Interior descriptors are separate auxiliary ports.
Native visible-R/clock/device-event integration and timing remain open.
"""
import re
from unittest.mock import patch

import generate_cached_r_packet as cached
import generate_split_r_packet as split
from generate_cached_r_packet import stage
from generate_parallel_r_fault import packet as parallel_packet
from generate_owned_r_packet import cache
from generate_external_r_packet import packet as external_packet,caller as external_caller
from generate_cp_block_capture import rewrite as block_rewrite
from generate_cp_prblock_capture import rewrite as prblock_rewrite
from generate_cp_interior_read import rewrite as interior_rewrite,PORTS
import generate_shared_control_capture as shared
from prove_cp_prefix_cache import module
from trace_r_front_transactions import layout


def original_layout(raw):
    # Do not put the37 auxiliary descriptor bits into the original result.
    assert raw.count(PORTS)==2
    return layout(raw.replace(PORTS,''))


def source_close(raw,functions):
    for name in ['dorado_r_front_control_step','dorado_r_front_pure_step',
                 'dorado_r_front_prefix_step','dorado_r_front_parallel_fault_step',
                 'dorado_r_front_packet_step']:
        old=module(raw,name);header,body=old.split('\n);',1) if '\n);' in old else old.split(');',1)
        sep='\n);' if '\n);' in old else ');'
        for a,b in [('pins_before','source_before'),('pins_after','source_after')]:
            body=re.sub(r'(?<![.\w])'+a+r'\b',b,body)
        view=functions+''' wire [400:0] source_before=cp_source_pins(pins_before,cp_bus(pins_before));
 wire [400:0] source_after=cp_source_pins(pins_after,cp_bus(pins_after));
'''
        marker=' // These32 bits retain the CLOSED input latch data.'
        if name not in ['dorado_r_front_control_step','dorado_r_front_pure_step']:
            actual=header+sep+'\n'+view+body
        else:
            assert body.count(marker)==1
            actual=header+sep+body.replace(marker,view+marker,1)
        restored_body=body
        for a,b in [('source_before','pins_before'),('source_after','pins_after')]:
            restored_body=re.sub(r'\b'+a+r'\b',b,restored_body)
        assert header+sep+restored_body==old
        assert raw.count(old)==1;raw=raw.replace(old,actual,1)
    return raw


def processor(original,functions):
    corrected=interior_rewrite(prblock_rewrite(block_rewrite(original,True),True,True))
    with patch.object(cached,'layout',original_layout),patch.object(split,'layout',original_layout):
        packets=parallel_packet(corrected)+'\n'+stage(corrected,True)+'\n'+cache(corrected,True)
    packets=external_packet(packets)
    top=module(packets,'dorado_r_front_packet_step');actual=top;edits=[]
    def change(a,b):
        nonlocal actual
        assert actual.count(a)==1,a
        actual=actual.replace(a,b,1);edits.append((a,b))
    old=' output wire [3:0] memory_select_n,output wire [15:0] memory_write_data);'
    new=''' output wire [3:0] memory_select_n,output wire [15:0] memory_write_data,
 input wire [17:0] interior_read_data,input wire interior_read_valid,
 output wire read_prepare,interior_read_needed,
 output wire [31:0] interior_port_address,output wire [3:0] interior_port_select_n);'''
    change(old,new)
    declarations=''
    for n in ['single','first','second']:
        declarations+=f' wire {n}_interior_needed;wire [31:0] {n}_interior_address;wire [3:0] {n}_interior_select;\n'
        # Bind each entire named child instance, preserving every old port.
        start=actual.index(' '+('dorado_r_front_parallel_fault_step' if n=='single' else 'dorado_r_front_prefix_step')+' '+n+'(')
        end=actual.index(');',start)+2;old_call=actual[start:end]
        data='interior_read_data' if n=='single' else "18'b0"
        valid='interior_read_valid' if n=='single' else "1'b0"
        ports=f'''.cp_ready(),
  .interior_read_data({data}),.interior_read_valid({valid}),
  .interior_read_needed({n}_interior_needed),
  .interior_port_address({n}_interior_address),.interior_port_select_n({n}_interior_select),'''
        assert old_call.count('.cp_ready(),')==1
        change(old_call,old_call.replace('.cp_ready(),',ports,1))
    marker=' wire first_after=use_pair ?'
    change(marker,declarations+marker)
    marker=' wire entry_ready,entry_fast,entry_hit;'
    change(marker,marker+'''\n assign read_prepare=entry_ready;
 assign interior_read_needed=!use_pair && single_interior_needed;
 assign interior_port_address=single_interior_address;
 assign interior_port_select_n=single_interior_select;
''')
    # A pair requiring an interior read falls back to separate events; never
    # evaluate that capture with invented zero data or add a second RAM port.
    marker='second_r_write_n==2\'b11 && !first_unsettled && !second_unsettled &&'
    change(marker,marker+'\n     !first_interior_needed && !second_interior_needed &&')
    restored=actual
    for a,b in reversed(edits):
        assert restored.count(b)==1;restored=restored.replace(b,a,1)
    assert restored==top
    packets=packets.replace(top,actual,1)
    # Share real ordered capture frames after creating the sparse packet
    # variants. Their existing pair_fall branch keeps its original evaluator.
    result=shared.kernel(corrected+'\n'+packets,share_native=True)+'\n'+shared.control(corrected)
    return source_close(result,functions)


def caller(original,functions):
    raw=external_caller(original);actual=raw;edits=[]
    def change(a,b):
        nonlocal actual
        assert actual.count(a)==1,a
        actual=actual.replace(a,b,1);edits.append((a,b))
    change('output wire [843:0] r_front_held);','''output wire [843:0] r_front_held,
 input wire read_accept,payload_ready,
 output wire read_needed,stage_active,stage_reading,stage_valid,stage_busy,stage_tag_match,
 output wire owner_commit);''')
    change(' wire [400:0] r_front_live=pins_after;',functions+'''
 wire [400:0] r_front_live=cp_source_pins(pins_after,cp_bus(pins_after));''')
    change(' wire owner_commit=contb_advance && control_ready;',''' assign owner_commit=contb_advance && control_ready;
 wire packet_ready,read_ready,read_prepare;
 wire [31:0] interior_address,physical_address;wire [3:0] interior_select,physical_select;
 wire [17:0] before_data,capture_data,saved_before;
 assign before_data=stage_active ? saved_before:physical_read;
 // Upstream payload validity must cover the WHOLE machine. Read preparation
 // remains independent so this gate cannot prevent its own completion.
 assign control_ready=packet_ready && read_ready && payload_ready;
 dorado_r_interior_read_tagged_stage stage(storage_clk,reset,read_prepare,read_needed,read_accept,owner_commit,
  memory_address,interior_address,memory_select_n,interior_select,physical_read,
  physical_address,physical_select,stage_active,stage_reading,stage_valid,read_ready,
  saved_before,capture_data,stage_busy,stage_tag_match);''')
    # Declarations precede all uses in both simulators.
    old_start=actual.index(' assign owner_commit=');old_end=actual.index(' dorado_r_front_packet_step packet(',old_start)
    old_group=actual[old_start:old_end]
    lines=old_group.splitlines(True)
    decl=[l for l in lines if l.startswith(' wire ')]
    body=[l for l in lines if not l.startswith(' wire ')]
    change(old_group,''.join(decl+body))
    change('packet(storage_clk,contb_advance,use_pair,','packet(storage_clk,owner_commit,use_pair,')
    change('first_result,last_result,control_ready,forwarded,reset,\n  physical_read,memory_write_n,memory_address,memory_select_n,memory_write_data);',
           '''first_result,last_result,packet_ready,forwarded,reset,
  before_data,memory_write_n,memory_address,memory_select_n,memory_write_data,
  capture_data,stage_valid,read_prepare,read_needed,interior_address,interior_select);''')
    change('memory_write_data,memory_address,memory_select_n,physical_read[15:0],physical_read[17:16]);',
           'memory_write_data,physical_address,physical_select,physical_read[15:0],physical_read[17:16]);')
    restored=actual
    for a,b in reversed(edits):
        assert restored.count(b)==1;restored=restored.replace(b,a,1)
    assert restored==raw
    return actual
