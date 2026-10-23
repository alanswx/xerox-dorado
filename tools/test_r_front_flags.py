#!/usr/bin/env python3
"""Compare factored sign/odd outputs with the unchanged complete original graph.

Retain the proven visible-R stored-latch view and original stack arithmetic.
Only flag output expressions change; all physical Qs, clocks and RAM compare.
"""
import argparse
import json
from pathlib import Path
import sys
from unittest.mock import patch
import test_r_front_visible as visible
from rewrite_r_flags import rewrite


def run(a):
    parent=visible.masked.commands.parent;original=parent.adapters_generate
    def generated(root,out):
        record=original(root,out)
        (out/'flags-parent-manifest.json').write_text(json.dumps(record,indent=2)+'\n')
        transform=rewrite(out/'dorado_local_r_flags_step.sv')
        for row in record['files']:
            if row['output']=='dorado_local_r_flags_step.sv':
                assert row['output_sha256']==transform['parent_sha256']
                row.update(parent_adapter_sha256=row['output_sha256'],output_sha256=transform['output_sha256'],
                           inverse_byte_identical=False,flags_rewrite=transform)
        record['flags_rewrite']=transform
        (out/'manifest.json').write_text(json.dumps(record,indent=2)+'\n')
        return record
    with patch.object(parent,'adapters_generate',generated):rc=visible.run(a)
    p=a.output.resolve()/'run-state.json';r=json.loads(p.read_text())
    r.update(experiment='factored_original_R_sign_odd_output',native_integrated=False,physical_timing_qualified=False)
    p.write_text(json.dumps(r,indent=2)+'\n')
    return rc


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--base',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--engine',choices=['icarus','verilator'],default='verilator')
    p.add_argument('--profile',choices=visible.masked.commands.parent.PROFILES,default='balanced')
    p.add_argument('--iterations',type=int,default=1024)
    sys.exit(run(p.parse_args()))
