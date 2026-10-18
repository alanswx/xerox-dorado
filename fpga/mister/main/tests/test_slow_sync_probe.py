#!/usr/bin/env python3
"""Qualify the hot-load flush probe against the actual patched Main worker."""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

p=argparse.ArgumentParser(description=__doc__)
p.add_argument('source',type=Path)
p.add_argument('--emit',type=Path)
args=p.parse_args()
here=Path(__file__).resolve().parent

def build(root,execute):
    root.mkdir(parents=True,exist_ok=True)
    for name in ('offload.cpp','offload.h','profiling.h'):shutil.copyfile(args.source/name,root/name)
    subprocess.run(['patch','-p1','-i',str(here.parent/'main-offload-260912.patch')],cwd=root,check=True,capture_output=True)
    shutil.copyfile(here/'slow_sync_probe.cpp',root/'probe.cpp')
    shutil.copyfile(here/'test_slow_sync_probe.cpp',root/'test.cpp')
    cxx=os.environ.get('CXX','g++')
    common=[cxx,'-std=c++14','-O2','-g','-Wall','-Wextra','-Werror','-pthread']
    subprocess.run([*common,'-shared','-fPIC','probe.cpp','-o','probe.so'],cwd=root,check=True)
    subprocess.run([*common,'test.cpp','-ldl','-o','probe-test'],cwd=root,check=True)
    if execute:
        for name,flags in [('normal',[]),('timeout',['timeout'])]:
            base=root/name
            subprocess.run([str(root/'probe-test'),str(root/'probe.so'),str(base),*flags],cwd=root,check=True,timeout=12)
            rows=[json.loads(x)for x in Path(str(base)+'.jsonl').read_text().splitlines()]
            assert rows[0]['phase']=='waiting'
            if flags:assert rows[-1]['phase']=='timeout' and rows[-1]['error']==110
            else:assert rows[-1]['phase']=='end' and rows[-1]['result']==0 and rows[-1]['duration_ns']>=4_100_000_000
        print('SLOW-SYNC TRACE PASS: actual worker/DSO bridge and bounded timeout')
if args.emit:build(args.emit.resolve(),False)
else:
    with tempfile.TemporaryDirectory(prefix='dorado-sync-probe-',dir='/tmp')as d:build(Path(d),True)
