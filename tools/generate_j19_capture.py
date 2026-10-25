#!/usr/bin/env python3
"""Experimental ordered input/root epochs for ProcL j19's two clock inputs.

Retain before, changed-input/before-root and after-root clocks. Endpoint-only
comparison loses a LOW interval between two HIGH endpoints. Capture from the
data preceding the selected edge. No guest owner, stage, reset or RAM port is
added. Other packages retain their qualified capture logic. Full source-arrival
and unknown-input qualification remain required before native selection.
"""


def rewrite(raw):
    original=raw;changes=[]
    def change(before,after):
        nonlocal raw
        assert raw.count(before)==1,before
        raw=raw.replace(before,after,1);changes.append((before,after))
    before='wire [56:0] before_clocks,after_clocks;'
    change(before,before+'\n wire [56:0] pre_front_clocks;')
    before='.source_pins(pre_source),.fa_n(pre_fa),.front_data(pre_data));'
    change(before,before.replace('.front_data(pre_data)', '.front_data(pre_data),.clocks(pre_front_clocks)'))
    before='wire [11:0] captures=after_clocks[56:45] & ~before_clocks[56:45];'
    after='''wire [1:0] j19_input_edges=pre_front_clocks[56:55] & ~before_clocks[56:55];
 wire [1:0] j19_root_edges=after_clocks[56:55] & ~pre_front_clocks[56:55];
 wire [11:0] captures={j19_input_edges|j19_root_edges,
                      after_clocks[54:45] & ~before_clocks[54:45]};'''
    change(before,after)
    for bit in range(2):
        before=f'if(captures[{10+bit}]) front_next[{98+bit}]=pre_data[{42+bit}];'
        after=f'if(captures[{10+bit}]) front_next[{98+bit}]=j19_root_edges[{bit}] ? pre_data[{42+bit}]:before_front_data[{42+bit}];'
        change(before,after)
    restored=raw
    for before,after in reversed(changes):restored=restored.replace(after,before,1)
    assert restored==original
    return raw
