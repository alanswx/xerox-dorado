#!/usr/bin/env python3
"""Normally load the reviewed MC1668 candidate on MiSTer14.

Run on MiSTer14 only after build review. Retire Native59 through Main's real
network/disk loader barriers, preserve its closed volume, then start unchanged
Main/server binaries with a fresh original seed. No guest state import or SPI
owner outside Main. The current old guest is at its profile question; no clean
OS shutdown or full machine snapshot is claimed by host-boundary preservation.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import shutil
import signal
import subprocess
import time

MAIN='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
SERVER='c59962490772f12d061ec61aa64e6a567144d26d7802a28349a8d9a044eda87e'
SEED='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
OLD_RBF='db6883e495bacd51c188f0b22c459937071d4860516ae102bb84c4c7bf441cad'


def sha(path):
    h=hashlib.sha256()
    with Path(path).open('rb')as f:
        for b in iter(lambda:f.read(1048576),b''):h.update(b)
    return h.hexdigest()


def matches(exe,argv=None):
    found=[]
    for p in Path('/proc').iterdir():
        if not p.name.isdigit():continue
        try:
            if os.readlink(p/'exe')==exe and(argv is None or(p/'cmdline').read_bytes().split(b'\0')[:len(argv)]==[x.encode()for x in argv]):found.append(int(p.name))
        except OSError:pass
    return found


def stopped(pid):
    for _ in range(300):
        if not Path('/proc/%d/exe'%pid).exists():return
        time.sleep(.1)
    raise RuntimeError('Normal stop incomplete; inspect same process, do not force kill: '+str(pid))


def deploy(staging):
    base=Path('/media/fat/games/DoradoRTL');old=base/'Cedar59_20261019';new=base/'CedarMC1668_20261020'
    oldcore=Path('/media/fat/_Computer/DoradoRTL_Cedar59_20261019');core=Path('/media/fat/_Computer/DoradoRTL_CedarMC1668_20261020')
    q=json.loads((staging/'qualification.json').read_text())
    assert q['status']=='PASS_NATIVE_MC1668_BUILD'and q['state_admin']=='INACTIVE'
    assert q['frequency_mhz']==50 and q['sysper']==4 and q['minimum_slack_ns']>0
    assert len(q['detailed_paths'])==196 and len(q['timing_checks'])==31
    assert q['initial_component_enabled_samples']>=100000 and q['physical_instances']==12
    assert not q['hardware_speedup_claimed']and not q['firmware_qualified']
    assert sha(staging/'Dorado.rbf')==q['rbf_sha256']
    c=json.loads((old/'reboot-resume.json').read_text())
    assert matches(c['main']['exe'],c['main']['argv'])==[29511]
    assert matches(c['server']['exe'],c['server']['argv'])==[29510]
    assert sha('/proc/29511/exe')==MAIN and sha('/proc/29510/exe')==SERVER
    assert '/dev/mem'in Path('/proc/29510/maps').read_text()
    assert sha(oldcore/'Dorado.rbf')==OLD_RBF
    assert not new.exists()and not core.exists(),'Inspect previous preparation, never blindly rerun'
    main_log=Path('/tmp/dorado-mc1668-main.log');disk_log=Path('/tmp/dorado-mc1668-disk.jsonl')
    assert not main_log.exists()and not disk_log.exists()
    seed=base/'System_20261017/cedar-work-native-hints-f3bce2a8.pack';assert sha(seed)==SEED
    new.mkdir();core.mkdir();shutil.copy2(seed,new/'cedar-work.pack');assert sha(new/'cedar-work.pack')==SEED
    shutil.copy2(staging/'Dorado.rbf',core/'Dorado.rbf')
    mgl=(oldcore/'Cedar59.mgl').read_text().replace('Cedar59_20261019','CedarMC1668_20261020')
    assert 'Cedar59_20261019'not in mgl
    (core/'CedarMC1668.mgl').write_text(mgl)
    ini=Path('/media/fat/MiSTer.ini');text=ini.read_text();assert '[DoradoCedarMC1668_20261020]'not in text
    shutil.copy2(ini,new/'MiSTer.ini.before')
    ini.write_text(text+'\n[DoradoCedarMC1668_20261020]\nvideo_mode=1920,88,44,148,1080,4,5,36,74250,1,1\nvsync_adjust=0\nmain='+c['main']['exe']+'\n')
    (new/'qualification.json').write_text(json.dumps(q,indent=2)+'\n')
    (new/'preparation.json').write_text(json.dumps(dict(old_main=29511,old_server=29510,phase='Fresh original seed prepared; normal loader barriers next'),indent=2)+'\n')
    print('MC1668_NATIVE_DEPLOY prepared; requesting normal Main loader',flush=True)
    fd=os.open('/dev/MiSTer_cmd',os.O_WRONLY)
    try:os.write(fd,b'load_core /media/fat/_Computer/DoradoRTL_CedarMC1668_20261020/CedarMC1668.mgl\n')
    finally:os.close(fd)
    argv=[c['main']['exe'],str(core/'Dorado.rbf'),str(core/'CedarMC1668.mgl')]
    for _ in range(500):
        found=matches(c['main']['exe'],argv)
        if len(found)==1 and not matches(c['main']['exe'],c['main']['argv']):break
        time.sleep(.1)
    else:raise RuntimeError('Normal replacement not observed; inspect same Main, never force program')
    temporary=found[0];time.sleep(1);assert sha('/proc/%d/exe'%temporary)==MAIN
    os.kill(temporary,signal.SIGTERM);stopped(temporary)
    assert '/dev/mem'not in Path('/proc/29510/maps').read_text()
    assert sha(new/'cedar-work.pack')==SEED,'New seed changed before real boot'
    trace=Path(c['main']['env']['DORADO_DISK_TRACE']);records=0;last=None
    with trace.open('rb')as stream:
        for raw in stream:
            last=json.loads(raw);records+=1
            assert 'error'not in last['event']and last['event']!='trace_dropped',last
    assert records and last['event']=='barrier_end',last
    shutil.copyfile(trace,old/'disk-through-mc1668-load.jsonl')
    shutil.copyfile(c['main_stdout'],old/'main-through-mc1668-load.log')
    proof=dict(path=str(old/'cedar-work.pack'),bytes=(old/'cedar-work.pack').stat().st_size,
        sha256=sha(old/'cedar-work.pack'),disk_records=records,
        boundary='Actual Main loader disk/network barriers and normal temporary Main exit',
        guest='Observed Cedar profile question before load; no guest Boot/clean shutdown or FPGA snapshot claimed')
    (old/'preservation-before-mc1668.json').write_text(json.dumps(proof,indent=2)+'\n')
    os.kill(29510,signal.SIGTERM);stopped(29510)
    assert not Path('/tmp/dorado-net.sock.control').exists()
    with(new/'server.log').open('ab')as f:
        sp=subprocess.Popen(c['server']['argv'],cwd=c['server']['cwd'],env=dict(os.environ,**c['server']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
    time.sleep(.3);assert sp.poll()is None
    c['main']['argv']=argv;c['main']['env']['DORADO_DISK_TRACE']=str(disk_log);c['main_stdout']=str(main_log)
    with main_log.open('ab')as f:
        mp=subprocess.Popen(argv,cwd=c['main']['cwd'],env=dict(os.environ,**c['main']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
    (new/'reboot-resume.json').write_text(json.dumps(c,indent=2)+'\n')
    d=dict(old_main=29511,old_server=29510,temporary_main=temporary,main_pid=mp.pid,server_pid=sp.pid,
        main_sha256=MAIN,server_sha256=SERVER,rbf_sha256=q['rbf_sha256'],seed_sha256=SEED,preserved_old_volume=proof,
        main_log=str(main_log),disk_trace=str(disk_log),phase='Core loaded; original firmware boot pending')
    (new/'deployment.json').write_text(json.dumps(d,indent=2)+'\n')
    for _ in range(300):
        assert mp.poll()is None;text=main_log.read_text()
        assert 'trace open failed'not in text and 'packet ownership held'not in text
        if 'Dorado disk: image cache ready,'in text and 'Dorado DDR: direct frame queues active,'in text:break
        time.sleep(.1)
    else:raise RuntimeError('Cache-ready/fresh DDR grant missing; inspect same processes')
    assert '/dev/mem'in Path('/proc/%d/maps'%sp.pid).read_text()
    print(json.dumps(d,indent=2),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--staging',type=Path,required=True);a=p.parse_args();deploy(a.staging.resolve())
