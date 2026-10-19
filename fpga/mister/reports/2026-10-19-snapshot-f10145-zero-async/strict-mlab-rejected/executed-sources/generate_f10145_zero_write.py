#!/usr/bin/env python3
"""Experimental zero private write reply; public fabric already returns zero.

Every original state update, ordinary asynchronous output and snapshot read
stays unchanged. Only the unreleased private administrative write-result value
changes. Strict MLAB is an isolated placement experiment, never selected here
by the full-core builder before physical inference/timing qualification.
"""
import hashlib
import generate_f10145_shared_write as shared
from generate_f10145 import once

ROOT, ORIGINAL = shared.ROOT, shared.ORIGINAL


def generate(*, mlab=False):
    text, manifest = shared.generate()
    predecessor = manifest['generated_sha256']
    text = once(text, '      state_result <= state_index == 5\'d16 ? state_window :',
                '      state_result <= state_write ? 10\'b0 : state_index == 5\'d16 ? state_window :')
    if mlab:
        old = '''`ifdef DORADO_F10145_LOGIC
  (* ramstyle = "logic" *) reg [3:0] mem [0:15];
`else
  (* ramstyle = "MLAB, no_rw_check" *) reg [3:0] mem [0:15];
`endif'''
        text = once(text, old, '  (* ramstyle = "MLAB" *) reg [3:0] mem [0:15];')
    return text, dict(manifest, generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
                      shared_write_predecessor_sha256=predecessor,
                      administrative_write_result='zero; public fabric behavior unchanged',
                      memory_implementation='strict MLAB requested' if mlab else 'qualified logic selection',
                      zero_write_experiment=__doc__)
