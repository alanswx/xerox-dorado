#!/usr/bin/env python3
"""Expose original control-register availability at the downstream run clock.

The source-only root interface cannot distinguish both original arrival orders.
Add a non-architectural descriptor: RunRefresh reaches l03's direct Set pins
before its clock. Preserve all156 physical Qs and all existing functions;
only the two l03 sink samples use the descriptor. The data and l04 clear paths
include another gate and retain their old operands at the downstream clock.
Native clock generation must supply this contract and remains unqualified.
"""
import argparse
import hashlib
from pathlib import Path
from generate_control_hold_candidate import generate as baseline_generate


def generate(out):
    assert not out.exists();base=out.with_name(out.stem+'-root-baseline.sv')
    parent=baseline_generate(base);assert parent['output_sha256']=='b79903a970ad7e4bd4ec79020f3d43f542dc9a71b412b21a2f4f39365528323e'
    raw=base.read_text().replace('module dorado_control_hold_step','module dorado_control_hold_ordered_step')
    old=' output wire [93:0] flags);';assert raw.count(old)==1
    raw=raw.replace(old,''' // l06.B RunRefresh reaches l03's direct Set pins before f13's clock.
 // Describes source clock ordering, not a guest control or extra state.
 input wire refresh_before_run_capture,output wire [93:0] flags);''')
    marker=' function automatic [130:0] ca_pins';assert raw.count(marker)==1
    helper=''' function automatic [31:0] controller_transfer_ordered(
  input [31:0] q,input [8:0] captures,input [8:0] pins,input refresh_early);
  reg [31:0] n;reg refresh_at_capture,d_mem_run;
  begin
   n=controller_transfer(q,captures,pins);
   // Zero descriptor retains the existing atomic capture contract exactly.
   if(refresh_early && captures[2])begin
    // l06 captures OLD WantRunRfsh at the upstream source edge. Its physical
    // Q21 update remains in controller_transfer, independent of these views.
    refresh_at_capture=q[19];
    // g04 AND and h03 OR data routes retain the old source operands. Unlike
    // direct RunRefresh -> Set, those routes include a further gate before D.
    d_mem_run=q[5] | (q[19] & !q[21]);
    // l03's two asynchronous Set inputs sample the actual available source.
    if(refresh_at_capture)begin n[14]=1;n[15]=1;end
    else begin n[14]=d_mem_run;n[15]=d_mem_run;end
    // h03 adds a gate before l04 ClearMemStop; its original update stays.
   end
   controller_transfer_ordered=n;
  end
 endfunction
'''
    raw=raw.replace(marker,helper+marker)
    old='controller_transfer(q[31:0],ac[8:0],controls(q[109:0],ap))';assert raw.count(old)==1
    raw=raw.replace(old,'controller_transfer_ordered(q[31:0],ac[8:0],controls(q[109:0],ap),refresh_before_run_capture)')
    out.write_text(raw)
    return dict(scope=__doc__,output_sha256=hashlib.sha256(raw.encode()).hexdigest(),parents=parent['parents'],
                baseline_sha256=parent['output_sha256'],physical_bits=156,clock_channels=56,selected_flags=94,
                native_integrated=False,physical_timing_qualified=False,root_only_clock_generation_qualified=False,
                descriptor='RunRefresh at direct l03 Set before f13 capture; old data and l04 clear operands retained')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    print(generate(p.parse_args().output.resolve())['output_sha256'])
