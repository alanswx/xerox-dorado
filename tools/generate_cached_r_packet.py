#!/usr/bin/env python3
"""Build an entry-cached packet with a separately derived second-root identity.

Keep the qualified original generic cache comparison at packet ENTRY only.
Its prefix lookup uses the first event, while its canonical tag records the
whole packet's final control owner/root. Pair mode requires the entry identity,
unchanged pins and original geometry/error guards. The second uncommitted event
uses a direct root transfer, justified by the actual fall/endpoint theorem and
full closed-owner projection, never by native canonical_match specialization.
"""
import re
from prove_cp_prefix_cache import module
from prove_native_cp_context import expression
from trace_r_front_transactions import layout


def projection(raw):
    top=module(raw,'dorado_r_front_pure_step');cq=expression(top,'cq');joint=expression(top,'joint_q')
    return ''' function automatic [155:0] control_projection(input [843:0] q);
  reg [109:0] cq;
  begin cq=CQ;control_projection=JOINT;end
 endfunction
'''.replace('CQ',cq.replace('held_q','q')).replace('JOINT',joint.replace('held_q','q'))


def stage(raw):
    original=module(raw,'dorado_r_front_pure_step');actual=original
    old='output wire [15:0] file_write_data\n);'
    new='''output wire [15:0] file_write_data,
 input wire entry_fast,input wire [268:0] entry_prefix,
 input wire [10:0] entry_prefix_cp,entry_after_cp,
 output wire [155:0] control_owner,output wire [268:0] live_prefix
);'''
    assert actual.count(old)==1;actual=actual.replace(old,new,1)
    first=actual.index(' wire fresh_split=');last=actual.index(' dorado_control_hold_ordered_step control(',first)
    old_guard=actual[first:last]
    guard=''' assign cp_ready=1'b1;assign cp_cache_hit=1'b0;
 assign cp_fast=entry_fast;assign cp_prefix_saved=entry_prefix;
 assign cp_prefix_cp_saved=entry_prefix_cp;assign cp_after_cp_saved=entry_after_cp;
 assign control_owner=joint_q;assign live_prefix=cp_prefix_live;
'''
    actual=actual[:first]+guard+actual[last:]
    actual=actual.replace('module dorado_r_front_pure_step #','module dorado_r_front_prefix_step #',1)
    assert actual.replace('module dorado_r_front_prefix_step #','module dorado_r_front_pure_step #',1).replace(guard,old_guard,1).replace(new,old,1)==original
    return actual


def cache(raw,stale_root=False):
    original=module(raw,'dorado_cp_prefix_cache');actual=original
    old=' output reg [10:0] prefix_cp_saved=0,after_cp_saved=0);'
    new=' output reg [10:0] prefix_cp_saved=0,after_cp_saved=0,input wire committed_root);'
    assert actual.count(old)==1;actual=actual.replace(old,new,1)
    actual=actual.replace('module dorado_cp_prefix_cache(','module dorado_packet_prefix_cache(',1)
    if not stale_root:
        assert actual.count('canonical_root<=after_root;')==1
        actual=actual.replace('canonical_root<=after_root;','canonical_root<=committed_root;',1)
    restored=actual.replace('module dorado_packet_prefix_cache(','module dorado_cp_prefix_cache(',1).replace(new,old,1)
    if not stale_root:restored=restored.replace('canonical_root<=committed_root;','canonical_root<=after_root;',1)
    assert restored==original
    return actual


def packet(raw,mutation=None):
    ports,bits=layout(raw);decl='\n'.join(f' wire {w} {prefix}_{n};' for prefix in ['first','second'] for n,w in ports)
    calls=[]
    for prefix in ['first','second']:
        first=prefix=='first'
        bindings=dict(host_clk='host_clk',advance="1'b0",held_q=('held_q' if first or mutation=='stale-intermediate' else 'first_held_next'),
            clock_before=('clock_before' if first else "1'b0"),clock_after=('first_after' if first else "1'b1"),
            pins_before=('pins_before' if first else 'pins_after'),pins_after='pins_after',refresh_before_run_capture='refresh',cp_reset="1'b0",
            external_file_read=('original_read' if first or mutation=='drop-forward' else 'later_read'),cp_ready='',
            entry_fast=('entry_fast' if first else "1'b1"),entry_prefix=('entry_prefix' if first else "{second_owner,113'b0}"),
            entry_prefix_cp=('entry_prefix_cp' if first else 'pins_after[292:282]'),entry_after_cp=('entry_after_cp' if first else 'pins_after[292:282]'),
            control_owner=prefix+'_owner',live_prefix=('first_live_prefix' if first else ''))
        bindings.update({n:prefix+'_'+n for n,w in ports})
        calls.append(' dorado_r_front_prefix_step '+prefix+'(\n  '+',\n  '.join(f'.{n}({v})' for n,v in bindings.items())+');')
    pack=lambda p:'{'+','.join(p+'_'+n for n,w in ports)+'}'
    commit="1'b1" if mutation=='early-write' else 'advance && ready'
    return '''`default_nettype none
module dorado_r_front_packet_step(
 input wire host_clk,advance,use_pair,input wire [843:0] held_q,
 input wire clock_before,clock_after,input wire [400:0] pins_before,pins_after,
 input wire refresh,output wire [2469:0] first_result,last_result,
 output reg ready,output wire [1:0] forwarded,input wire reset);
 DECLARATIONS
 PROJECTION
 wire first_after=use_pair ? 1'b0:clock_after;
 wire [155:0] first_owner,second_owner;
 wire [155:0] final_owner=control_projection(use_pair ? second_held_next:first_held_next);
 wire [268:0] first_live_prefix,entry_prefix;
 wire [10:0] entry_prefix_cp,entry_after_cp;
 wire entry_ready,entry_fast,entry_hit;
 wire [17:0] original_read,later_read;
 CALLS
 dorado_packet_prefix_cache entry_cache(host_clk,reset,advance && ready,
  first_owner,final_owner,clock_before,first_after,pins_before[397:0],pins_after[397:0],refresh,
  !first_unsettled && (!use_pair || !second_unsettled),first_live_prefix,
  entry_ready,entry_fast,entry_hit,entry_prefix,entry_prefix_cp,entry_after_cp,clock_after);
 dorado_r_pair_read_forward forwarding(original_read,first_file_write_data,
  first_r_write_n,first_port_select_n,later_read,forwarded);
 always @*begin
  ready=entry_ready && !use_pair;
  if(use_pair)begin
   ready=0;
   case(^{held_q,clock_before,clock_after,pins_before,pins_after,refresh,original_read})
    1'b0,1'b1:ready=entry_ready && entry_fast && clock_before && clock_after && pins_before==pins_after &&
     first_port_address==second_port_address && first_port_select_n==second_port_select_n &&
     second_r_write_n==2'b11 && !first_unsettled && !second_unsettled &&
     !(|first_repeated_events) && !(|second_repeated_events);
    default:ready=0;
   endcase
  end
 end
 dorado_r_file_ports files(host_clk,COMMIT,first_r_write_n,first_port_address,first_port_select_n,
  first_file_write_data,first_port_address,first_port_select_n,original_read[15:0],original_read[17:16]);
 assign first_result=FIRST;
 assign last_result=use_pair ? SECOND : FIRST;
endmodule
`default_nettype wire
'''.replace('DECLARATIONS',decl).replace('PROJECTION',projection(raw)).replace('CALLS','\n'.join(calls)).replace('COMMIT',commit).replace('FIRST',pack('first')).replace('SECOND',pack('second'))
