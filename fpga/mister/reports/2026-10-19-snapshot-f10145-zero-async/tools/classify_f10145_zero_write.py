#!/usr/bin/env python3
"""Classify the zero-private-write-reply F10145 with the qualified field rules."""
import argparse
import hashlib
import json
from pathlib import Path
import classify_f10145_shared_write as classifier
import generate_f10145_zero_write as candidate


def run(out):
    classifier.candidate = candidate
    classifier.run(out)
    sources = [Path(__file__), Path(classifier.__file__), Path(candidate.__file__)]
    (out/'driver-provenance.json').write_text(json.dumps(dict(
        scope=__doc__, candidate_module=candidate.__name__,
        source_hashes={str(p.resolve().relative_to(candidate.ROOT)):
                       hashlib.sha256(p.read_bytes()).hexdigest() for p in sources}),
        indent=2)+'\n')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    run(a.output.resolve())
