#!/usr/bin/env python3
"""Review the corrected combined native fit; no assembly or deployment.

The shared reviewer retains the original response-core review unchanged. This
entry requires exact six-source regeneration, acyclic lookahead component
proof, all444 timing categories, physical lookahead/crystal inventories and
the original capture, RAM, cache, clock, coverage and warning checks.
"""
import argparse
import json
from pathlib import Path

from review_storage_response_native import review as response_review


def review(root, config):
    return response_review(root, config, timing_paths=True)


if __name__ == '__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['root','config','output']:
        p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();assert not a.output.exists()
    result=review(a.root.resolve(),a.config.resolve())
    a.output.write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],len(result['detailed_paths']),result['minimum_setup_ns'],result['minimum_hold_ns'])
