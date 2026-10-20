#!/usr/bin/env python3
"""Actual Main worker/cache/adapter snapshot lease against temporary files.

Linux host or static ARM build; SPI boundaries are spies. A 4.2-second write
and held fsync must never block Main polling or admit premature checkpoint data.
This does not qualify FPGA/device pause or a complete snapshot bundle.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess

HERE=Path(__file__).resolve().parent


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(source,out,execute,static,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    for name in('offload.cpp','offload.h','profiling.h','input.cpp'):
        shutil.copyfile(source/name,inputs/name)
    main_inputs={p.name:sha(p)for p in inputs.iterdir()}
    subprocess.run(['patch','-p1','-i',str(HERE.parent/'main-offload-260912.patch')],cwd=inputs,check=True,capture_output=True)
    subprocess.run(['git','apply','--include=input.cpp',str(HERE.parent/'main-disk-260912.patch')],cwd=inputs,check=True,capture_output=True)
    text=(inputs/'input.cpp').read_text();a=text.index('static void INThandler(int code)');b=text.index('\n}',a)+2
    (inputs/'input_handler.inc').write_text(text[a:b]+'\n')
    for n in('dorado_disk','dorado_disk_cache','dorado_disk_trace'):
        for ext in('.cpp','.h'):shutil.copyfile(HERE.parent/(n+ext),inputs/(n+ext))
    shutil.copyfile(HERE.parent/'dorado_net.h',inputs/'dorado_net.h')
    for name in('file_io.h','spi.h','user_io.h'):shutil.copyfile(HERE/'disk'/name,inputs/name)
    legacy=(HERE/'test_disk_adapter.cpp').read_text()
    # Its main has an implicit return0. Give the renamed helper an explicit
    # return so the same strict warnings apply to this combined test.
    marker='fclose(f.filp);offload_stop();'
    assert legacy.count(marker)==1
    (inputs/'legacy_adapter.cpp').write_text(legacy.replace(marker,marker+'return 0;',1))
    shutil.copyfile(HERE/'test_disk_snapshot.cpp',inputs/'test.cpp')
    command=[os.environ.get('CXX','g++'),'-std=c++14','-D_FILE_OFFSET_BITS=64','-O2','-g','-Wall','-Wextra','-Werror','-pthread',
        '-I'+str(inputs),str(inputs/'test.cpp'),str(inputs/'dorado_disk.cpp'),str(inputs/'dorado_disk_cache.cpp'),str(inputs/'dorado_disk_trace.cpp'),
        '-Wl,--wrap=pwrite64','-Wl,--wrap=fsync',*(['-static']if static else[]),'-o',str(out/'disk-snapshot-test')]
    (out/'build-command.json').write_text(json.dumps(command,indent=2)+'\n')
    with(out/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    results=[]
    if execute:
        for label,args in(('normal',[]),('write-error',['write-error']),('sync-error',['sync-error'])):
            cmd=[str(out/'disk-snapshot-test'),*args]
            with(out/(label+'.log')).open('w')as log:r=subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,timeout=30)
            text=(out/(label+'.log')).read_text();assert not r.returncode and 'DISK SNAPSHOT'in text,text
            print(label+': '+text.splitlines()[-1],flush=True);results.append(dict(case=label,returncode=r.returncode,log_sha256=sha(out/(label+'.log'))))
        if mutations:
            original=(inputs/'dorado_disk_cache.cpp').read_text()
            controls=[
                ('unguarded_io','if (snapshot_requested_) return Result::Wait;','',2,'snapshot served normal SPI request'),
                ('premature_ready','return state_ == State::Ready && !queued_ && !dirty_count_ && !needs_sync_\n        ? Result::Done : Result::Wait;',
                 'return state_ == State::Ready ? Result::Done : Result::Wait;',1,'snapshot acknowledged unfinished write'),
                ('premature_continue','if (r == Result::Done) snapshot_requested_ = false;',
                 'snapshot_requested_ = false;',1,'unfinished snapshot lost lease'),
                ('ignored_generation','generation != generation_','generation == UINT64_MAX',5,'stale pause generation accepted')]
            for label,before,after,count,diagnostic in controls:
                case=out/label;case.mkdir();assert original.count(before)==count,(label,original.count(before))
                cpp=case/'dorado_disk_cache.cpp';cpp.write_text(original.replace(before,after))
                cmd=[str(cpp)if s==str(inputs/'dorado_disk_cache.cpp')else s for s in command]
                cmd[-1]=str(case/'test')
                (case/'build-command.json').write_text(json.dumps(cmd,indent=2)+'\n')
                with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
                r=subprocess.run([str(case/'test')],capture_output=True,text=True,timeout=20)
                text=r.stdout+r.stderr;(case/'test.log').write_text(text)
                assert r.returncode==23 and diagnostic in text,text
                print(label+': REJECTED '+diagnostic,flush=True)
                results.append(dict(case=label,returncode=r.returncode,expected_failure=diagnostic))
    (out/'qualification.json').write_text(json.dumps(dict(status='PASS'if execute else'BUILD_ONLY',scope=__doc__,cases=results,
        original_main_inputs=main_inputs,executed_inputs={p.name:sha(p)for p in inputs.iterdir()},
        limits='Actual host worker/cache/adapter and SPI ownership spies. No FPGA service barrier, disk snapshot file/COW bundle or complete machine restore.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('source',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--no-run',action='store_true');p.add_argument('--static',action='store_true');p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.source.resolve(),a.output.resolve(),not a.no_run,a.static,a.mutations)
