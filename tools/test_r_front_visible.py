#!/usr/bin/env python3
"""Compare explicit old read-latch sourcing against the original full graph.

Retain the MIR mask, original stack-adder cells and all original graph/state/
RAM/clock comparisons. Only the local visible-R combinational mux changes.
"""
import argparse
import json
from pathlib import Path
import sys
from unittest.mock import patch
import test_r_front_mir_mask as masked
from rewrite_r_visible import rewrite


def run(a):
    parent=masked.commands.parent;original=parent.adapters_generate
    def generated(root,out):
        record=original(root,out)
        (out/'parent-manifest.json').write_text(json.dumps(record,indent=2)+'\n')
        transform=rewrite(out/'dorado_local_r_operand_step.sv')
        for row in record['files']:
            if row['output']=='dorado_local_r_operand_step.sv':
                assert row['output_sha256']==transform['parent_sha256']
                row.update(parent_adapter_sha256=row['output_sha256'],output_sha256=transform['output_sha256'],
                           inverse_byte_identical=False,visible_rewrite=transform)
        record.update(visible_rewrite=transform,scope=__doc__)
        (out/'manifest.json').write_text(json.dumps(record,indent=2)+'\n')
        return record
    a.frozen_switch=False;a.late_no_dispatch=True
    with patch.object(parent,'adapters_generate',generated):rc=masked.run(a)
    p=a.output.resolve()/'run-state.json';r=json.loads(p.read_text())
    r.update(experiment='existing_read_latch_sources_visible_R',native_integrated=False,physical_timing_qualified=False)
    p.write_text(json.dumps(r,indent=2)+'\n')
    return rc


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--base',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],default='verilator')
    p.add_argument('--profile',choices=masked.commands.parent.PROFILES,default='balanced')
    p.add_argument('--iterations',type=int,default=1024)
    sys.exit(run(p.parse_args()))
