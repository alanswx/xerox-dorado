#!/usr/bin/env python3
"""Stage and qualify the pause-aware daemon/client without hardware access.

Execute from copied source/data, preserve every command and raw log, and run
the real legacy process, Main ownership, clock/MMIO and pause mutation gates.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import platform
import shutil
import subprocess
import time


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(out,root):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs'
    paths=[*sorted((root/'dorado/include').glob('*.h')),root/'dorado/src/ethernet.c',root/'dorado/src/io.c']
    for directory in ('fpga/mister/server','fpga/mister/main'):
        paths +=[p for p in sorted((root/directory).rglob('*'))if p.is_file()and(p.suffix in ('.c','.cpp','.h','.py')or p.name=='Makefile')]
    paths +=[root/p for p in ('dorado/worlds/aemu.eb','chm/bootfiles/CRTTEST.BOOT!1','chm/dorado/CedarDorado.eb!6','chm/cedar/stp-root/Cedar6.1/VersionMap/CedarSource.VersionMap')]
    for p in paths:
        q=inputs/p.relative_to(root);q.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,q)
    server=inputs/'fpga/mister/server';main=inputs/'fpga/mister/main';binary=out/'dorado-server';steps=[]
    env={k:v for k,v in os.environ.items()if not k.startswith('DORADO_')}
    def execute(label,command,timeout=120):
        start=time.monotonic()
        (out/(label+'-command.json')).write_text(json.dumps(command,indent=2)+'\n')
        with(out/(label+'.log')).open('wb')as log:
            result=subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,env=env,timeout=timeout)
        steps.append(dict(step=label,returncode=result.returncode,elapsed_seconds=round(time.monotonic()-start,3),log_sha256=sha(out/(label+'.log'))))
        print(label+': rc='+str(result.returncode),flush=True);assert not result.returncode,(label,(out/(label+'.log')).read_text())
    execute('control-clock-ring',['make','-C',str(server),'OUT='+str(binary),'ddr-control-test','service-clock-test','ddr-ring-test','ddr-map-test'])
    execute('legacy-server',['make','-C',str(server),'OUT='+str(binary),'all','test','cedar-test','bsp-retry-test','bsp-ack-race-test'])
    execute('main-client',['make','-C',str(main),'OUT='+str(out/'main'),'control-test'])
    execute('main-manager',['python3',str(main/'tests/test_ddr_transport.py'),'--emit',str(out/'manager')])
    execute('main-transport-build',['c++','-O2','-Wall','-Wextra','-Werror','-std=c++14','-I'+str(main/'tests'),'-I'+str(server),
        str(main/'dorado_net.cpp'),str(main/'dorado_ddr.cpp'),str(main/'dorado_control_client.cpp'),str(main/'tests/test_main_transport.cpp'),'-o',str(out/'main-transport')])
    execute('main-transport',['python3',str(main/'tests/run.py'),str(binary),str(out/'main-transport'),str(inputs)])
    execute('pause-mutations',['python3',str(server/'test_snapshot_pause_mutations.py'),'--output',str(out/'mutations'),'--root',str(inputs)])
    result=dict(status='PASS',scope=__doc__,platform=platform.platform(),steps=steps,
        executed_inputs={str(p.relative_to(inputs)):sha(p)for p in inputs.rglob('*')if p.is_file()},
        hardware_access=False,production_executable_sha256=sha(binary),
        limits='Host process and guarded-memory/client qualification. No deployed daemon/Main, FPGA pause/drain, serialized server state or full snapshot restore.')
    (out/'qualification.json').write_text(json.dumps(result,indent=2)+'\n');print('SNAPSHOT PAUSE QUALIFICATION PASS',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--root',type=Path,default=Path(__file__).resolve().parents[3]);a=p.parse_args();run(a.output.resolve(),a.root.resolve())
