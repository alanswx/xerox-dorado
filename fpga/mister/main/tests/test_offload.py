#!/usr/bin/env python3
"""Patch the pinned Main worker and exercise actual pthread execution on Linux.

Pass an unmodified Main source directory, or --emit DIR to keep the generated
sources/binaries for cross-compiling and running on the MiSTer ARM. This is a
worker prerequisite test, not a disk-coherence or integrated-Main test.
"""
import argparse
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('source', type=Path)
parser.add_argument('--emit', type=Path)
args = parser.parse_args()
here = Path(__file__).resolve().parent

def run(root, execute):
    root.mkdir(parents=True, exist_ok=True)
    for name in ('offload.cpp', 'offload.h', 'profiling.h'):
        shutil.copyfile(args.source/name, root/name)
    subprocess.run(['patch', '-p1', '-i', str(here.parent/'main-offload-260912.patch')],
                   cwd=root, check=True, capture_output=True, text=True)
    shutil.copyfile(here/'test_offload.cpp', root/'test.cpp')
    for name, flags in [('normal', []), ('blocking-control', ['-DCONTROL_BLOCKING_ENQUEUE'])]:
        subprocess.run([os.environ.get('CXX', 'g++'), '-std=c++14', '-O2', '-Wall',
                        '-Wextra', '-pthread', *flags, 'test.cpp', '-o', name],
                       cwd=root, check=True)
        if execute:
            result = subprocess.run([str(root/name)], capture_output=True, text=True, timeout=8)
            print(result.stdout, end='')
            if name == 'normal':
                assert result.returncode == 0, result.stderr
            else:
                assert result.returncode == 23 and 'enqueue blocked' in result.stderr, result
                print('OFFLOAD-TRY CONTROL PASS: blocking enqueue fails the 100ms bound')
    # Keep the exact candidate except for its empty-wake predicate.
    source = (root/'offload.cpp').read_text()
    marker = 'while (s_queue_head == s_queue_tail && !s_quit)'
    assert source.count(marker) == 1
    (root/'offload.cpp').write_text(source.replace(marker, marker.replace('while', 'if', 1)))
    subprocess.run([os.environ.get('CXX', 'g++'), '-std=c++14', '-O2', '-pthread',
                    'test.cpp', '-o', 'wake-control'], cwd=root, check=True)
    (root/'offload.cpp').write_text(source)
    if execute:
        result = subprocess.run([str(root/'wake-control')], capture_output=True, text=True, timeout=8)
        assert result.returncode != 0 and 'bad_function_call' in result.stderr, result
        print('OFFLOAD-WAKE CONTROL PASS: single-check predicate invokes an empty handler')

if args.emit:
    run(args.emit.resolve(), False)
else:
    with tempfile.TemporaryDirectory(prefix='dorado-offload-', dir='/tmp') as tmp:
        run(Path(tmp), True)
