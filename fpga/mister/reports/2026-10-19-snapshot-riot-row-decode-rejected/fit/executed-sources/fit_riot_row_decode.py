#!/usr/bin/env python3
"""Compare qualified shared RIOT writes with explicit row write enables."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import fit_riot
import fit_riot_shared_port as qualified
import generate_riot_shared_port as previous
import generate_riot_row_decode as candidate


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(output):
    qualified.previous = previous
    qualified.candidate = candidate
    qualified.prepare(output)
    path = output/'provenance.json'
    record = json.loads(path.read_text())
    for source in (Path(__file__).resolve(), Path(candidate.__file__)):
        shutil.copyfile(source, output/'executed-sources'/source.name)
        record['source_hashes'][str(source.relative_to(candidate.ROOT))] = digest(source)
    record.update(scope=__doc__+' Both expose complete state and identical50MHz/opposite-edge constraints. '
                  'No core qualification or deployment.', row_builder_sha256=digest(Path(__file__)))
    path.write_text(json.dumps(record, indent=2)+'\n')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--prepare', type=Path)
    p.add_argument('--run', type=Path)
    p.add_argument('--quartus', type=Path, default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a = p.parse_args()
    if a.prepare:
        prepare(a.prepare.resolve())
    elif a.run:
        record = json.loads((a.run/'provenance.json').read_text())
        assert digest(Path(__file__)) == record['row_builder_sha256']
        fit_riot.run(a.run.resolve(), a.quartus)
    else:
        p.error('Specify --prepare or --run')
