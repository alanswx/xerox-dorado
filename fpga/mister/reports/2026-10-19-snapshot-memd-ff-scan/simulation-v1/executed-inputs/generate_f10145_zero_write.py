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


def generate(*, mlab=False, async_mlab=False):
    assert not async_mlab or mlab
    text, manifest = shared.generate()
    predecessor = manifest['generated_sha256']
    text = once(text, '      state_result <= state_index == 5\'d16 ? state_window :\n'
                     '                      (state_index < 5\'d16 ? {6\'b0,mem[state_index[3:0]]} : 10\'b0);',
                     '      state_result <= 10\'b0;\n'
                     '      if (ram_admin_read) state_result <= {6\'b0,mem[state_index[3:0]]};\n'
                     '      else if (!state_write && state_index == 5\'d16) state_result <= state_window;')
    text = once(text, '  wire ram_restore =',
                '  wire ram_admin_read = state_accept && !state_write && state_index < 5\'d16;\n  wire ram_restore =')
    text = once(text, '\nendmodule', '''
  // synthesis translate_off
  always @(posedge state_host_clk)
    if (ram_admin_read && (ram_restore || ram_guest_write))
      $fatal(1,"administrative RAM read overlaps write");
  // synthesis translate_on
endmodule''')
    if mlab:
        old = '''`ifdef DORADO_F10145_LOGIC
  (* ramstyle = "logic" *) reg [3:0] mem [0:15];
`else
  (* ramstyle = "MLAB, no_rw_check" *) reg [3:0] mem [0:15];
`endif'''
        hint = 'MLAB, no_rw_check' if async_mlab else 'MLAB'
        text = once(text, old, '  (* ramstyle = "'+hint+'" *) reg [3:0] mem [0:15];')
    return text, dict(manifest, generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
                      shared_write_predecessor_sha256=predecessor,
                      administrative_write_result='zero; public fabric behavior unchanged',
                      memory_implementation=('original asynchronous MLAB style; registered admin reads exclude writes'
                                             if async_mlab else 'strict MLAB requested' if mlab else 'qualified logic selection'),
                      zero_write_experiment=__doc__)
