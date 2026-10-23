#!/usr/bin/env python3
"""Review the complete old-word/early-response fit without assembling it."""
import argparse
import json
from pathlib import Path

from review_storage_response_native import review as shared_review


def review(root,config):
    return shared_review(root,config,timing_paths=True,old_word=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['root','config','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();assert not a.output.exists()
    r=review(a.root.resolve(),a.config.resolve());a.output.write_text(json.dumps(r,indent=2)+'\n')
    print(r['status'],len(r['detailed_paths']),r['minimum_setup_ns'],r['minimum_hold_ns'])
