#!/usr/bin/env python3
"""Compare a native six-bit stack adder with the complete original graph.

Retain the qualified MIR mask and all original state/RAM/clock comparisons.
Only the three combinational MC10182 instances in the candidate cone change.
No reference cell, clock, register, capture epoch or memory bank is altered.
"""
import argparse
import json
from pathlib import Path
import sys
from unittest.mock import patch

import test_r_front_mir_mask as masked
from rewrite_stack_adder import rewrite


def run(a):
    parent = masked.commands.parent
    original_run = parent.run
    def rewritten(*args, **kwargs):
        assert 'cone_rewrite' not in kwargs
        return original_run(*args, **kwargs, cone_rewrite=rewrite)
    a.frozen_switch = False
    a.late_no_dispatch = True
    with patch.object(parent, 'run', rewritten):
        rc = masked.run(a)
    p = a.output.resolve()/'run-state.json'
    s = json.loads(p.read_text())
    s.update(experiment='native_six_bit_stack_adder_with_MIR_mask',
             physical_timing_qualified=False, native_integrated=False, speedup_qualified=False)
    assert s['cone_rewrite']['non_arithmetic_source_unchanged']
    p.write_text(json.dumps(s, indent=2)+'\n')
    return rc


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--base', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--engine', choices=['icarus', 'verilator'], default='verilator')
    p.add_argument('--profile', choices=masked.commands.parent.PROFILES, default='balanced')
    p.add_argument('--iterations', type=int, default=1024)
    sys.exit(run(p.parse_args()))
