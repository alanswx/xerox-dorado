#!/usr/bin/env python3
"""Make g15's closing write use the pre-transition address/data explicitly.

Transform the existing local processor adapter without adding storage. The
new helper still exposes the same original64 g15 Qs; its closing transaction
is captured before LastNext switches. All other logic is inversely checked.
"""
import hashlib
from pathlib import Path


def rewrite(path):
    original=path.read_text();raw=original
    before=' wire [56:0] before_clocks,after_clocks;'
    after=before+'\n wire [43:0] before_front_data;'
    assert raw.count(before)==1;raw=raw.replace(before,after)
    old='.source_pins(before_source),.fa_n(before_fa),.clocks(before_clocks));'
    new='.source_pins(before_source),.fa_n(before_fa),.clocks(before_clocks),.front_data(before_front_data));'
    assert raw.count(old)==1;raw=raw.replace(old,new)
    marker=' reg [99:0] front_next;'
    block=''' wire [63:0] rbase_next;
 dorado_rbase_window_step rbase_file(state_q[703:640],before_clocks[46],after_clocks[46],
  before_front_data[11:8],pre_data[11:8],state_q[639:636],
  !after_clocks[45] ? pre_data[3:0] : state_q[639:636],rbase_next,,);
'''
    assert raw.count(marker)==1;raw=raw.replace(marker,block+marker)
    write='   if(!after_clocks[46]) front_next[4+4*pre_data[11:8]+:4]=front_next[3:0];'
    update='   front_next[67:4]=rbase_next;'
    assert raw.count(write)==1;raw=raw.replace(write,update)
    inverse=raw.replace(after,before,1).replace(new,old,1).replace(block,'',1).replace(update,write,1)
    assert inverse==original
    path.write_text(raw)
    return dict(parent_sha256=hashlib.sha256(original.encode()).hexdigest(),output_sha256=hashlib.sha256(raw.encode()).hexdigest(),
                unchanged_logic_inverse_byte_identical=True,physical_g15_bits=64,additional_state_bits=0,native_integrated=False)
