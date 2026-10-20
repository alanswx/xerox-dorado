#!/usr/bin/env python3
"""Actual Main poll/manager/client snapshot lease against the real C service.

Anonymous guarded DDR and SPI spies only; no /dev/mem, FPGA programming or
production daemon/Main replacement. The caller's FPGA/disk/input/video barrier
and complete save/restore remain separate work.
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


def run(out,execute,static,mutations,main_source):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    for name in('dorado_ddr','dorado_control_client','dorado_net'):
        for ext in('.cpp','.h'):shutil.copyfile(HERE.parent/(name+ext),inputs/(name+ext))
    server=HERE.parents[1]/'server'
    for name in('dorado_ddr_ring','dorado_ddr_control'):
        for ext in('.c','.h'):shutil.copyfile(server/(name+ext),inputs/(name+ext))
    # dorado_net.cpp includes the production disk declarations, but this gate
    # substitutes only prepare_exit; the cache/worker is qualified separately.
    for name in('dorado_disk.h','dorado_disk_cache.h'):shutil.copyfile(HERE.parent/name,inputs/name)
    for name in('spi.h','user_io.h','test_ddr_snapshot.cpp','test_ddr_snapshot.py'):
        shutil.copyfile(HERE/name,inputs/name)
    original=(HERE/'test_ddr_transport.cpp').read_text();(inputs/'legacy-original.cpp').write_text(original)
    assert original.count('#include "../dorado_ddr.h"')==1
    legacy=original.replace('#include "../dorado_ddr.h"','#include "dorado_ddr.h"',1)
    anchor='    check(!command,"nested SPI");'
    assert legacy.count(anchor)==1
    legacy=legacy.replace(anchor,'''    check(std::this_thread::get_id()==spi_owner,"worker touched SPI");
    check(!forbid_spi,"snapshot touched SPI");++snapshot_spi_calls;
'''+anchor,1)
    anchor='starts,stops);\n}';assert legacy.count(anchor)==1
    legacy=legacy.replace(anchor,'starts,stops);\nreturn 0;\n}',1)
    extra=[]
    if main_source:
        hooks=[]
        for filename,start,end,unused,tail in(
            ('fpga_io.cpp','int fpga_load_rbf(', '\tOsdDisable();','(void)name;(void)cfg;(void)xml;','return 0;'),
            ('fpga_io.cpp','void reboot(', '\tsync();','(void)cold;',''),
            ('fpga_io.cpp','void app_restart(', '\tsync();','(void)path;(void)xml;(void)exe;',''),
            ('user_io.cpp','void user_io_init(', '\tchar *name;','(void)path;(void)xml;','')):
            source=(main_source/filename).read_text();a=source.index(start);b=source.index(end,a);prefix=source[a:b]
            assert prefix.index('dorado_net_barrier(')<prefix.index('dorado_disk_')
            hooks.append(prefix+unused+'++lifecycle_mutations;'+tail+'}\n')
            shutil.copyfile(main_source/filename,inputs/('pinned-'+filename))
        (inputs/'lifecycle_hooks.inc').write_text(''.join(hooks))
        for before in('bool dorado_net_barrier(const char *reason){return dorado_ddr_barrier(reason,10);}\n',
                      'bool dorado_net_reset(){return dorado_ddr_reset();}\n'):
            assert legacy.count(before)==1;legacy=legacy.replace(before,'',1)
        extra=['-DDORADO_TEST_LIFECYCLE']
    (inputs/'legacy_helpers.cpp').write_text(legacy)
    objects=[];commands=[]
    for name in('dorado_ddr_ring','dorado_ddr_control'):
        obj=out/(name+'.o');objects.append(str(obj))
        command=[os.environ.get('CC','cc'),'-std=c11','-O2','-g','-Wall','-Wextra','-Werror','-I'+str(inputs),'-c',str(inputs/(name+'.c')),'-o',str(obj)]
        commands.append(command)
        with(out/(name+'-build.log')).open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    command=[os.environ.get('CXX','c++'),'-std=c++14','-O2','-g','-Wall','-Wextra','-Werror','-pthread','-I'+str(inputs),
        *[str(inputs/(name+'.cpp'))for name in('dorado_ddr','dorado_control_client','dorado_net','test_ddr_snapshot')],
        *objects,*extra,*( ['-static']if static else []),'-o',str(out/'test')]
    commands.append(command);(out/'build-commands.json').write_text(json.dumps(commands,indent=2)+'\n')
    with(out/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    results=[]
    if execute:
        for label in('normal','stale-ring','eof','core-change','lost-pause','timeout','unexpected-pause'):
            cmd=[str(out/'test'),*([]if label=='normal'else[label])]
            with(out/(label+'.log')).open('w')as log:result=subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,timeout=20)
            text=(out/(label+'.log')).read_text();assert not result.returncode and'MAIN SNAPSHOT'in text,(label,text)
            print(label+': '+next(line for line in text.splitlines()if line.startswith('MAIN SNAPSHOT')),flush=True)
            results.append(dict(case=label,returncode=0,log_sha256=sha(out/(label+'.log'))))
        if mutations:
            sources={n:(inputs/(n+'.cpp')).read_text()for n in('dorado_ddr','dorado_net')}
            controls=[
                ('frozen_probe','dorado_net','if(dorado_ddr_snapshot_pending()){dorado_ddr_snapshot_poll();return;}','',1,'normal','snapshot touched SPI'),
                ('lifecycle_bypass','dorado_ddr','''    if(snapshot_requested){
        fprintf(stderr,"Dorado DDR: refusing %s: snapshot retains the transport lease\\n",reason);return false;
    }
''','',1,'normal','snapshot touched SPI'),
                ('early_continue','dorado_ddr','state=State::SnapshotContinueAck;deadline=now+5000;',
                 'snapshot_requested=false;state=State::SnapshotContinueAck;deadline=now+5000;',1,'normal','snapshot lease released before CONTINUE ACK'),
                ('stale_epoch','dorado_ddr','expected!=epoch','false',2,'normal','stale pause accepted'),
                ('lost_pause','dorado_ddr','client.reply().paused!=want_paused','(client.reply().paused!=want_paused && client.reply().epoch!=epoch)',1,'lost-pause','lost daemon pause/session was accepted'),
                ('unsolicited_pause','dorado_ddr','client.reply().paused || client.reply().epoch!=epoch','client.reply().epoch!=epoch',1,'unexpected-pause','normal manager accepted unexpected pause')]
            for label,name,before,after,count,mode,diagnostic in controls:
                raw=sources[name];assert raw.count(before)==count,(label,raw.count(before))
                case=out/label;case.mkdir();cpp=case/(name+'.cpp');cpp.write_text(raw.replace(before,after))
                cmd=[str(cpp)if p==str(inputs/(name+'.cpp'))else p for p in command];cmd[-1]=str(case/'test')
                (case/'build-command.json').write_text(json.dumps(cmd,indent=2)+'\n')
                with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
                cmd=[str(case/'test'),*([]if mode=='normal'else[mode])]
                with(case/'test.log').open('w')as log:r=subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,timeout=15)
                text=(case/'test.log').read_text();assert r.returncode==23 and diagnostic in text,(label,r.returncode,text)
                print(label+': REJECTED '+diagnostic,flush=True)
                results.append(dict(case=label,returncode=r.returncode,expected_failure=diagnostic,log_sha256=sha(case/'test.log')))
    (out/'qualification.json').write_text(json.dumps(dict(status='PASS'if execute else'BUILD_ONLY',scope=__doc__,cases=results,
        inputs={p.name:sha(p)for p in inputs.iterdir()},
        limits='Actual Main/daemon control lease and polling only. No FPGA service acknowledgement, disk lease coordination, full native snapshot or restore; no deployment.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--main-source',type=Path);p.add_argument('--no-run',action='store_true');p.add_argument('--static',action='store_true');p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),not a.no_run,a.static,a.mutations,a.main_source.resolve()if a.main_source else None)
