#!/usr/bin/env python3
"""Pure source transform for the qualified MIR clock-mask experiment.

The caller supplies the maintained command component. No state or pipeline
stage is introduced; the per-wave combinational view preserves NoDispatch.
"""


def candidate(original, frozen_switch=False):
    raw = original
    edits = [
        ('clock_levels(input [103:0] q,input base,input [129:0] p)',
         'clock_levels(input [103:0] q,input base,input [129:0] p,input mir_switch_q)'),
        ('mir_clock=normal0 | repeat_cur | p[125]',
         'mir_clock=normal0 | (p[69]&mir_switch_q) | p[125]'),
        ('previous=clock_levels(q,clock_before,pins_before)',
         'previous=clock_levels(q,clock_before,pins_before,q[43])'),
        ('levels=clock_levels(q,clock_after,pins_after)',
         'levels=clock_levels(q,clock_after,pins_after,mir_switch_view)'),
        ('reg [2:0] old_memory;', 'reg [2:0] old_memory;reg mir_switch_view,mir_switch_before;'),
        ('q=state_q;n=q;previous=clock_levels', 'mir_switch_view=state_q[43];q=state_q;n=q;previous=clock_levels'),
        ('settled=(n==q && levels==previous);previous=levels;q=n;',
         'mir_switch_before=mir_switch_view;'+('' if frozen_switch else 'if(q[69])mir_switch_view=1;')+
         'settled=(n==q && levels==previous && mir_switch_view==mir_switch_before);previous=levels;q=n;'),
    ]
    for before, after in edits:
        assert raw.count(before) == 1, before
        raw = raw.replace(before, after)
    inverse = raw
    for before, after in reversed(edits):
        inverse = inverse.replace(after, before)
    assert inverse == original
    return raw, edits
