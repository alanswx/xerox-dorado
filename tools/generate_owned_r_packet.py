#!/usr/bin/env python3
"""Publish packet owners/context using the actual native caller's storage seam.

Keep828 stored bits and16 original external visible-R aliases. Pins/root and
owners publish together under the same ready/advance guard as the one SRAM.
Specialize ONLY canonical equality, retaining refresh and original X/Z behavior.
The generic prefix key/retained-old-context behavior remains byte-identical.
This caller is not yet integrated with the native source/device event stream.
"""
import re
from generate_cached_r_packet import cache as generic_cache,projection
from prove_native_cp_context import MATCH,expression,BP

GENERIC=''' wire canonical_match=(current_q==canonical_q)&&(before_pins==canonical_pins)&&
                      (before_root==canonical_root)&&(refresh==canonical_refresh);'''
SPECIAL=''' wire original_canonical_match=(current_q==canonical_q)&&(before_pins==canonical_pins)&&
                      (before_root==canonical_root)&&(refresh==canonical_refresh);
 reg canonical_match;
 always @*begin
  // Binary equality follows from the protected common publication. Preserve
  // the actual full comparison for ANY unknown owned-context/tag bit.
  case(^{current_q,canonical_q,before_pins,canonical_pins,before_root,canonical_root})
   1'b0,1'b1:canonical_match=(refresh==canonical_refresh);
   default:canonical_match=original_canonical_match;
  endcase
 end'''


def cache(raw,specialized=False,mutation=None):
    original=generic_cache(raw);actual=original
    assert GENERIC==MATCH
    if specialized:
        new=SPECIAL
        if mutation=='drop-refresh':new=new.replace('canonical_match=(refresh==canonical_refresh);',"canonical_match=1'b1;",1)
        if mutation=='drop-unknown-fallback':new=new.replace('canonical_match=original_canonical_match;',"canonical_match=(refresh==canonical_refresh);",1)
        assert actual.count(GENERIC)==1;actual=actual.replace(GENERIC,new,1)
        assert actual.replace(new,GENERIC,1)==original
    return actual


def native_publication(bp):
    block='''  always @(posedge storage_clk)if(contb_advance && control_ready)begin
    r_front_lower_q<=r_front_next[571:0];r_front_upper_q<=r_front_next[843:588];
    r_front_before<=r_front_live;r_front_clock_before<=CLK_ca_p_;
  end'''
    assert bp.count(block)==1
    for n in ['r_front_lower_q','r_front_upper_q','r_front_before','r_front_clock_before']:
        assert len(re.findall(r'\b'+n+r'\s*<=',bp))==1,n
    held=expression(bp,'r_front_held')
    assert held=='{r_front_upper_q,r_front_visible_r_ProcL,r_front_visible_r_ProcH,r_front_lower_q}'
    return block,held


def caller(raw,bp,mutation=None):
    block,held=native_publication(bp)
    if mutation=='stale-owner':block=block.replace('r_front_lower_q<=r_front_next[571:0];',"r_front_lower_q<=r_front_next[571:0] ^ (572'b1<<69);",1)
    if mutation=='stale-pins':block=block.replace('r_front_before<=r_front_live;',"r_front_before<=r_front_live ^ (401'b1<<151);",1)
    if mutation=='stale-final-root':block=block.replace('r_front_clock_before<=CLK_ca_p_;',"r_front_clock_before<=use_pair ? 1'b0:CLK_ca_p_;",1)
    return '''`default_nettype none
module dorado_r_front_owned_packet(
 input wire storage_clk,contb_advance,use_pair,clock_after,refresh,reset,
 input wire [400:0] pins_after,input wire [7:0] r_front_visible_r_ProcH,r_front_visible_r_ProcL,
 output wire [2469:0] first_result,last_result,output wire control_ready,
 output wire [1:0] forwarded,output wire [843:0] r_front_held);
 // Exact native storage seam. Original visible-R owners stay outside.
 reg [571:0] r_front_lower_q;reg [255:0] r_front_upper_q;
 reg [400:0] r_front_before;reg r_front_clock_before;
 assign r_front_held=HELD;
 wire [843:0] r_front_next=last_result[2469:1626];
 wire [400:0] r_front_live=pins_after;
 // This is the WHOLE packet endpoint, never first_after of the fall.
 wire CLK_ca_p_=clock_after;
 BLOCK
 dorado_r_front_packet_step packet(storage_clk,contb_advance,use_pair,r_front_held,
  r_front_clock_before,clock_after,r_front_before,pins_after,refresh,
  first_result,last_result,control_ready,forwarded,reset);
endmodule
`default_nettype wire
'''.replace('HELD',held).replace('BLOCK',block)


def publication_projection(raw,bp):
    _,held=native_publication(bp)
    return '''module owned_packet_projection(
 input wire [843:0] last_held,input wire [7:0] r_front_visible_r_ProcH,r_front_visible_r_ProcL,
 output wire equal,output wire [155:0] different);
 wire [571:0] r_front_lower_q=last_held[571:0];wire [255:0] r_front_upper_q=last_held[843:588];
 wire [843:0] published_held=HELD;
 PROJECTION
 assign different=control_projection(published_held)^control_projection(last_held);
 assign equal=different==156'b0;
endmodule
'''.replace('HELD',held).replace('PROJECTION',projection(raw))
