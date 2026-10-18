#!/usr/bin/env python3
"""Build cache tests against the real pinned Main worker (Linux / MiSTer ARM)."""
import argparse
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

p = argparse.ArgumentParser(description=__doc__)
p.add_argument('source', type=Path, help='unmodified pinned Main worker sources')
p.add_argument('--emit', type=Path)
args = p.parse_args()
here = Path(__file__).resolve().parent

def build(root, execute):
    root.mkdir(parents=True, exist_ok=True)
    for name in ('offload.cpp', 'offload.h', 'profiling.h'):
        shutil.copyfile(args.source/name, root/name)
    subprocess.run(['patch','-p1','-i',str(here.parent/'main-offload-260912.patch')],
                   cwd=root, check=True, capture_output=True)
    for name in ('dorado_disk_cache.cpp', 'dorado_disk_cache.h'):
        shutil.copyfile(here.parent/name, root/name)
    shutil.copyfile(here/'test_disk_cache.cpp', root/'test.cpp')
    command = [os.environ.get('CXX','g++'), '-std=c++14', '-O2', '-g', '-Wall',
               '-Wextra', '-Werror', '-pthread', 'test.cpp','dorado_disk_cache.cpp', '-o','disk-cache-test']
    subprocess.run(command,cwd=root,check=True)
    if execute:
        subprocess.run([str(root/'disk-cache-test')],cwd=root,check=True,timeout=40)
        original = (root/'dorado_disk_cache.cpp').read_text()
        controls = [
            ('shared-buffer',
             'snapshot.data() + done, wanted, first * Sector + done',
             'image_.data() + first * Sector + done, wanted, first * Sector + done',
             'worker write buffer changed'),
            ('lost-dirty',
             'if (!dirty_[i]) { dirty_[i] = 1; ++dirty_count_; }',
             'if (false && !dirty_[i]) { dirty_[i] = 1; ++dirty_count_; }',
             'write did not start'),
        ]
        for name, marker, replacement, expected in controls:
            assert marker in original
            (root/'dorado_disk_cache.cpp').write_text(original.replace(marker,replacement,1))
            subprocess.run(command[:-1]+[name],cwd=root,check=True)
            result = subprocess.run([str(root/name)],cwd=root,capture_output=True,text=True,timeout=20)
            assert result.returncode == 23 and expected in result.stderr, result
            print('DISK-CACHE CONTROL PASS:', name, 'fails:', expected)
        (root/'dorado_disk_cache.cpp').write_text(original)

if args.emit:
    build(args.emit.resolve(),False)
else:
    with tempfile.TemporaryDirectory(prefix='dorado-disk-cache-',dir='/tmp') as d:
        build(Path(d),True)
