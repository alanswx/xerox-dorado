#!/usr/bin/env python3
"""Select static row writes in the unchanged original/independent RIOT tests."""
import argparse
import hashlib
import json
from pathlib import Path
import generate_riot_row_decode as candidate
import test_riot_shared_port as qualified


def run(output):
    side = output.with_name(output.name+'-driver-inputs')
    side.mkdir(parents=True, exist_ok=False)
    paths = [Path(__file__), Path(candidate.__file__), Path(qualified.__file__)]
    for p in paths:
        (side/p.name).write_bytes(p.read_bytes())
    hashes = {str(p.resolve().relative_to(candidate.ROOT)):
              hashlib.sha256(p.read_bytes()).hexdigest() for p in paths}
    (side/'provenance.json').write_text(json.dumps(dict(scope=__doc__, source_hashes=hashes), indent=2)+'\n')
    qualified.gen = candidate
    qualified.run(output)
    for p in paths:
        assert hashlib.sha256(p.read_bytes()).hexdigest() == hashes[str(p.resolve().relative_to(candidate.ROOT))]


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    run(a.output.resolve())
