#!/usr/bin/env python3
"""Experimental strict-semantics MLAB placement for the shared-write F10145.

Only the physical RAM hint changes. No no_rw_check attribute, added latency,
clock change, state exclusion or relaxed timing constraint is introduced.
Actual inferred memory/collision behavior and routed timing require review.
"""
import hashlib
import generate_f10145_shared_write as shared
from generate_f10145 import once


def generate():
    text, manifest = shared.generate()
    old_hash = manifest['generated_sha256']
    old = '''`ifdef DORADO_F10145_LOGIC
  (* ramstyle = "logic" *) reg [3:0] mem [0:15];
`else
  (* ramstyle = "MLAB, no_rw_check" *) reg [3:0] mem [0:15];
`endif'''
    text = once(text, old, '  (* ramstyle = "MLAB" *) reg [3:0] mem [0:15];')
    manifest = dict(manifest, generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
                    shared_write_predecessor_sha256=old_hash,
                    memory_implementation='MLAB requested; strict RTL collisions retained',
                    placement_experiment=__doc__)
    return text, manifest
