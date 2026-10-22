#!/usr/bin/env python3
"""Start unchanged reviewed57MHz RTL/Main after a recorded closed-volume boundary.

Use a separately repaired offline pack, preserving the old source and logs.
No runtime import, external SPI owner or repeated load on observation timeout.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import signal
import subprocess
import tempfile
import time

from deploy_mc1668_native import MAIN, SEED, sha, matches, stopped
from deploy_r_front_ready_native import no_open, rewrite_mgl
from deploy_storage_board_native import image
from restart_storage_board_native import FIXED_SERVER

EXPECTED_BYTES=[247080,247082,247098,247100,247478,247479]


def repaired_copy(source, original, report):
    r=json.loads(report.read_text())
    assert sha(original)==r['source_sha256'] and sha(source)==r['output_sha256']
    assert source.stat().st_size==original.stat().st_size==63578624
    assert r['changed_bytes']==EXPECTED_BYTES
    assert [(c['root'],c['root_page'],c['slot'],c['before'],c['after']) for c in r['changes']]==[
        ('logical',84,2,[104,0],[3,20]),('logical',84,3,[139,0],[4,27])]
    different=[];offset=0
    with original.open('rb') as a,source.open('rb') as b:
        while True:
            x,y=a.read(65536),b.read(65536)
            if not x and not y:break
            assert len(x)==len(y)
            if x!=y:different.extend(offset+i for i,(u,v) in enumerate(zip(x,y)) if u!=v)
            offset+=len(x)
    assert different==EXPECTED_BYTES,different
    return r


def start(a):
    assert '10.0.2.14/' in subprocess.check_output(['ip','-4','addr','show'],text=True)
    for n in [a.previous,a.name,a.launcher,a.tag]:assert re.fullmatch('[A-Za-z0-9_-]+',n)
    assert len(a.launcher)<=29 and a.previous!=a.name
    base=Path('/media/fat/games/DoradoRTL');old=base/a.previous;new=base/a.name
    oldcore=Path('/media/fat/_Computer/DoradoRTL_'+a.previous)
    core=Path('/media/fat/_Computer/DoradoRTL_'+a.name)
    close=json.loads((old/'germ827-close.json').read_text())
    assert close['status']=='CLOSED_PACK_AND_LOGS_PRESERVED_MAIN_EXITED_DAEMON_MAPPING_REVOKED'
    assert close['no_open_pack_or_log_descriptors'] and not close['disk_errors']
    assert not Path('/proc/%d/exe'%close['main_pid']).exists()
    assert sha(old/'cedar-work.pack')==close['pack_sha256']
    no_open(old/'cedar-work.pack')
    c=json.loads((old/'reboot-resume.json').read_text());old_server=close['server_pid']
    assert not matches(c['main']['exe'])
    assert matches(c['server']['exe'],c['server']['argv'])==[old_server]
    assert sha('/proc/%d/exe'%old_server)==FIXED_SERVER
    assert '/dev/mem' not in Path('/proc/%d/maps'%old_server).read_text()
    assert sha(c['main']['exe'])==MAIN and sha(c['server']['exe'])==FIXED_SERVER
    original=base/'System_20261017/cedar-work-native-hints-f3bce2a8.pack' if a.fresh_seed else old/'cedar-work.pack'
    if a.fresh_seed:assert sha(original)==SEED
    repair=repaired_copy(a.source,original,a.report)
    files=['qualification.json','assembly-state.json','fit-review-controls.json','assembly-guard-controls.json','storage-board-native.json']
    with tempfile.TemporaryDirectory(prefix='closed-board-review-') as tmp:
        stage=Path(tmp)
        for n in files:(stage/n).symlink_to(old/n)
        (stage/'Dorado.rbf').symlink_to(oldcore/'Dorado.rbf')
        expected=image(stage)
    assert expected=='7c4d39e414e5de8bafae5d3ff0fd0dfddae310c79ff17d87e5a1e8b97ee24b8a'
    mainlog=new/'main.log';disklog=new/'disk.jsonl'
    assert not new.exists() and not core.exists() and not mainlog.exists() and not disklog.exists(),'Inspect existing journal/processes; do not restart blindly'
    ini=Path('/media/fat/MiSTer.ini');text=ini.read_text();section='['+a.launcher+']';assert section not in text
    mgl=rewrite_mgl(Path(c['main']['argv'][2]).read_text(),a.previous,a.name,a.launcher)
    new.mkdir();core.mkdir();shutil.copyfile(a.source,new/'cedar-work.pack');assert sha(new/'cedar-work.pack')==repair['output_sha256']
    shutil.copyfile(oldcore/'Dorado.rbf',core/'Dorado.rbf');assert sha(core/'Dorado.rbf')==expected
    for n in files:shutil.copyfile(old/n,new/n)
    shutil.copyfile(old/'germ827-close.json',new/'preceding-closure.json')
    shutil.copyfile(a.report,new/'boot-link-repair.json')
    shutil.copyfile(ini,new/'MiSTer.ini.before')
    (core/'Boot.mgl').write_text(mgl)
    ini.write_text(text+'\n'+section+'\nvideo_mode=1920,88,44,148,1080,4,5,36,74250,1,1\nvsync_adjust=0\nmain='+c['main']['exe']+'\n')
    journal=dict(phase='PREPARED_REPAIRED_CLOSED_SOURCE',driver_pid=os.getpid(),started=time.time(),old_main=close['main_pid'],old_server=old_server,rbf_sha256=expected,main_sha256=MAIN,source_pack_sha256=repair['output_sha256'],repair=repair,fresh_seed_with_logical_CHS=a.fresh_seed)
    def save():
        temp=new/'preparation.json.tmp';temp.write_text(json.dumps(journal,indent=2)+'\n');os.replace(temp,new/'preparation.json')
    save();os.kill(old_server,signal.SIGTERM);stopped(old_server)
    assert not Path(c['server']['env']['DORADO_NET_CONTROL']).exists()
    with (new/'server.log').open('ab') as log:
        server=subprocess.Popen(c['server']['argv'],cwd=c['server']['cwd'],env=dict(os.environ,**c['server']['env']),stdin=subprocess.DEVNULL,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
    time.sleep(.3);assert server.poll() is None
    c['main']['argv']=[c['main']['exe'],str(core/'Dorado.rbf'),str(core/'Boot.mgl')]
    c['main']['env']['DORADO_DISK_TRACE']=str(disklog);c['main_stdout']=str(mainlog)
    with mainlog.open('ab') as log:
        main=subprocess.Popen(c['main']['argv'],cwd=c['main']['cwd'],env=dict(os.environ,**c['main']['env']),stdin=subprocess.DEVNULL,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
    (new/'reboot-resume.json').write_text(json.dumps(c,indent=2)+'\n')
    result=dict(main_pid=main.pid,server_pid=server.pid,main_sha256=MAIN,server_sha256=FIXED_SERVER,rbf_sha256=expected,source_pack_sha256=repair['output_sha256'],original_source_sha256=repair['source_sha256'],fresh_seed_with_logical_CHS=a.fresh_seed,main_log=str(mainlog),disk_trace=str(disklog),host_clock_hz=57142857,sysper=4,phase='Original memory/firmware/desktop qualification pending')
    (new/'deployment.json').write_text(json.dumps(result,indent=2)+'\n');journal.update(result);journal['phase']='NEW_MAIN_SERVER_STARTED';save()
    for _ in range(300):
        assert main.poll() is None and server.poll() is None
        t=mainlog.read_text()
        if 'Dorado disk: image cache ready,' in t and 'Dorado DDR: direct frame queues active,' in t:break
        time.sleep(.1)
    else:raise RuntimeError('Observe same Main/server; readiness pending, do not reload')
    assert sha('/proc/%d/exe'%main.pid)==MAIN and sha('/proc/%d/exe'%server.pid)==FIXED_SERVER
    assert '/dev/mem' in Path('/proc/%d/maps'%server.pid).read_text()
    # Main argv attaches to an existing FPGA. Only its normal loader programs RBF.
    journal['phase']='FIRST_ACTUAL_FPGA_LOAD_REQUESTED';save()
    Path('/dev/MiSTer_cmd').write_text('load_core '+str(core/'Boot.mgl')+'\n')
    for _ in range(500):
        found=matches(c['main']['exe'],c['main']['argv'])
        if len(found)==1 and found[0]!=main.pid:break
        time.sleep(.1)
    else:raise RuntimeError('Observe same loader; never repeat a load request')
    temporary=found[0]
    assert sha('/proc/%d/exe'%temporary)==MAIN
    journal.update(phase='ACTUAL_FPGA_LOADED_TEMPORARY_MAIN',temporary_main=temporary);save()
    os.kill(temporary,signal.SIGTERM);stopped(temporary)
    no_open(new/'cedar-work.pack');no_open(disklog)
    assert '/dev/mem' not in Path('/proc/%d/maps'%server.pid).read_text()
    assert sha(new/'cedar-work.pack')==repair['output_sha256'], 'Guest changed seed before final observer; inspect existing state'
    assert not (new/'prequalification-disk.jsonl').exists()
    disklog.rename(new/'prequalification-disk.jsonl')
    offset=mainlog.stat().st_size
    with mainlog.open('ab') as log:
        final=subprocess.Popen(c['main']['argv'],cwd=c['main']['cwd'],env=dict(os.environ,**c['main']['env']),stdin=subprocess.DEVNULL,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
    result.update(main_pid=final.pid,actual_loader_main_pid=temporary,initial_attachment_main_pid=main.pid,
                  phase='FPGA_LOADED_FINAL_MAIN_STARTED_MEMORY_FIRMWARE_PENDING')
    (new/'deployment.json').write_text(json.dumps(result,indent=2)+'\n')
    journal.update(result);save()
    for _ in range(300):
        assert final.poll() is None and server.poll() is None
        with mainlog.open() as log:log.seek(offset);t=log.read()
        assert 'trace open failed' not in t
        if 'Dorado disk: image cache ready,' in t and 'Dorado DDR: direct frame queues active,' in t:break
        time.sleep(.1)
    else:raise RuntimeError('Observe same final Main; readiness pending, do not reload')
    assert sha('/proc/%d/exe'%final.pid)==MAIN
    assert '/dev/mem' in Path('/proc/%d/maps'%server.pid).read_text()
    journal['phase']='FPGA_LOADED_CACHE_DDR_READY_MEMORY_FIRMWARE_PENDING';save()
    print(json.dumps(result,indent=2),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','report']:p.add_argument('--'+n,type=Path,required=True)
    for n in ['previous','name','launcher','tag']:p.add_argument('--'+n,required=True)
    p.add_argument('--fresh-seed',action='store_true');start(p.parse_args())
