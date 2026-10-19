#!/usr/bin/env python3
"""Run the unchanged full-board oracle using the shared-port RIOT generator."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import sys
import generate_baseboard_shared_riot as generator
import test_baseboard_full as qualified


def run(root, output):
    # Select the generator only in this fixture process; no source files or
    # original RTL change. The full physical-state and independent models stay
    # exactly the qualified ones.
    qualified.gen = generator
    names = ('test_baseboard_shared_riot.py', 'generate_baseboard_shared_riot.py',
             'generate_riot_shared_port.py', 'generate_riot.py')
    sources = [Path(__file__).parent/n for n in names]
    hashes = {p.name: hashlib.sha256(p.read_bytes()).hexdigest() for p in sources}
    qualified.run(root, output, False)
    inputs = output/'executed-inputs'
    for p in sources:
        assert hashlib.sha256(p.read_bytes()).hexdigest() == hashes[p.name]
        (inputs/p.name).write_bytes(p.read_bytes())
    result_path = output/'results.json'
    result = json.loads(result_path.read_text())
    result['executed_inputs'].update(hashes)
    result['shared_riot_scope'] = __doc__
    result_path.write_text(json.dumps(result, indent=2)+'\n')


if __name__ == '__main__':
    if resource.getrlimit(resource.RLIMIT_STACK)[0] < 32*1024*1024:
        os.execv('/bin/zsh', ['/bin/zsh', '-c', 'ulimit -s 32768 || exit; exec "$@"',
                            'baseboard-shared-riot-test', sys.executable, str(Path(__file__).resolve()), *sys.argv[1:]])
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--frozen-root', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    run(a.frozen_root.resolve(), a.output.resolve())
