#!/usr/bin/env python3
"""Compile the actual disk adapter/cache with real Main worker and SPI spies.

Only SPI/fileTYPE boundaries are stubbed. Linker-wrapped pwrite64 injects a
4.2-second file operation in the worker. This does not qualify an FPGA boot.
"""
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
    for name in ('offload.cpp','offload.h','profiling.h'):
        shutil.copyfile(args.source/name,root/name)
    subprocess.run(['patch','-p1','-i',str(here.parent/'main-offload-260912.patch')],
                   cwd=root,check=True,capture_output=True)
    for name in ('dorado_disk','dorado_disk_cache','dorado_disk_trace'):
        for ext in ('.cpp','.h'):shutil.copyfile(here.parent/(name+ext),root/(name+ext))
    for name in ('file_io.h','spi.h','user_io.h'):shutil.copyfile(here/'disk'/name,root/name)
    shutil.copyfile(here/'test_disk_adapter.cpp',root/'test.cpp')
    shutil.copyfile(args.source/'input.cpp',root/'input.cpp')
    subprocess.run(['git','apply','--include=input.cpp',str(here.parent/'main-disk-260912.patch')],
                   cwd=root,check=True,capture_output=True)
    source=(root/'input.cpp').read_text()
    start=source.index('static void INThandler(int code)')
    end=source.index('\n}',start)+2
    (root/'input_handler.inc').write_text(source[start:end]+'\n')
    cmd=[os.environ.get('CXX','g++'),'-std=c++14','-D_FILE_OFFSET_BITS=64',
         '-O2','-g','-Wall','-Wextra','-Werror','-pthread','test.cpp','dorado_disk.cpp',
         'dorado_disk_cache.cpp','dorado_disk_trace.cpp','-Wl,--wrap=pwrite64','-o','disk-adapter-test']
    subprocess.run(cmd,cwd=root,check=True)
    if execute:
        def trace_environment(name):
            return dict(os.environ,DORADO_DISK_TRACE=str(root/(name+'.jsonl')))
        for name,flags in (('normal',[]),('error',['error'])):
            subprocess.run([str(root/'disk-adapter-test'),*flags],cwd=root,check=True,timeout=30,
                           env=trace_environment(name))
        for mode,expected_exit,name in (('term',143,'termination'),('int',130,'interrupt')):
            disk=root/(name+'.pack')
            result=subprocess.run([str(root/'disk-adapter-test'),mode,str(disk)],cwd=root,timeout=30,
                                  env=trace_environment(name))
            assert result.returncode==expected_exit,result
            assert disk.read_bytes()==Path(str(disk)+'.expected').read_bytes()
        print('DISK-ADAPTER TERMINATION PASS: SIGTERM and exact patched input.cpp SIGINT handler drain before exit')
        for name in ('normal','error','termination','interrupt'):
            rows=[json.loads(line) for line in (root/(name+'.jsonl')).read_text().splitlines()]
            assert not any(x['event']=='trace_dropped' for x in rows),name
            start=next(x for x in rows if x['event']=='pwrite_begin')
            end=next(x for x in rows if x['event']=='pwrite_end')
            assert end['duration_ns']>=4_100_000_000,(name,end)
            active=[x for x in rows if start['ns']<x['ns']<end['ns']]
            assert sum(x['event']=='read_ack' for x in active)>100,name
            assert sum(x['event']=='write_ack' for x in active)>100,name
            assert sum(x['event']=='poll' for x in active)>=2,name
            if name=='error':
                assert any(x['event']=='pwrite_error' and x['result']==28 for x in rows)
                assert any(x['event']=='barrier_error' for x in rows)
            else:assert any(x['event']=='barrier_end' for x in rows)
        print('DISK-ADAPTER TRACE PASS: monotonic IO durations and concurrent request/ACK/poll records, explicit errors/barriers, no dropped records')

if args.emit:build(args.emit.resolve(),False)
else:
    with tempfile.TemporaryDirectory(prefix='dorado-disk-adapter-',dir='/tmp') as tmp:
        build(Path(tmp),True)
