#!/usr/bin/env python3
"""Load the reviewed Ready/front image on MiSTer14 using normal Main barriers.

Preserve the old closed pack and complete trace before reclaiming its closed
temporary logs. Start unchanged Main/server binaries with a fresh original
seed. No C state, IOCB completion, raw SPI owner or guest clean-shutdown claim.
Inspect the written preparation journal after any interruption; never rerun a
load because an observation expired.
"""
from pathlib import Path
import argparse
import json
import os
import shutil
import signal
import subprocess
import time

from deploy_mc1668_native import MAIN,SERVER,SEED,sha,matches,stopped

OLD_RBF='15730f449bae786a3cee9ffa93d4bbc7de2e90083483f1a59cfd607c02af49a8'
RBF='57a552aff6fd0131f307a25ee75316c56b4c4da9118a437d2c9c17b0172dfd21'


def no_open(path):
    for proc in Path('/proc').iterdir():
        if not proc.name.isdigit():continue
        try:
            for fd in(proc/'fd').iterdir():
                try:target=os.readlink(fd)
                except OSError:continue
                assert target!=str(path),('File still open',path,proc.name,fd)
        except(FileNotFoundError,PermissionError):continue


def deploy(staging, repeat_boot2=False):
    base=Path('/media/fat/games/DoradoRTL')
    old=base/('CedarRFrontReady_20261021' if repeat_boot2 else 'CedarMC1668_20261020')
    new=base/('CedarRFrontReady_Boot2_20261021' if repeat_boot2 else 'CedarRFrontReady_20261021')
    oldcore=Path('/media/fat/_Computer')/('DoradoRTL_'+old.name)
    core=Path('/media/fat/_Computer')/('DoradoRTL_'+new.name)
    old_main,old_server=(15708,15703) if repeat_boot2 else (3486,3485)
    old_rbf=RBF if repeat_boot2 else OLD_RBF
    q=json.loads((staging/'qualification.json').read_text());a=json.loads((staging/'assembly-state.json').read_text());controls=json.loads((staging/'fit-review-controls.json').read_text())
    assert q['status']=='PASS_NATIVE_R_FRONT_READY_PREASSEMBLY_REVIEW'and q['state_admin']=='INACTIVE'
    assert q['frequency_mhz']==50 and q['sysper']==4 and q['minimum_slack_ns']>0
    assert len(q['timing_checks'])==31 and len(q['detailed_paths'])==196 and len(q['central_paths'])==136
    assert q['initial_component_enabled_samples']>=100000 and not q['firmware_qualified']and not q['hardware_speedup_claimed']
    assert a['status']=='TERMINAL_ASSEMBLED_REVIEWED_NATIVE_CANDIDATE'and a['returncode']==0
    assert sha(staging/'qualification.json')==a['qualification_sha256']
    assert sha(staging/'Dorado.rbf')==a['rbf_sha256']==RBF
    assert controls['status']=='PASS_NATIVE_READY_FIT_READER_CONTROLS'and len(controls['cases'])==11 and all(r['rejected']for r in controls['cases'])
    c=json.loads((old/'reboot-resume.json').read_text())
    assert matches(c['main']['exe'],c['main']['argv'])==[old_main]and matches(c['server']['exe'],c['server']['argv'])==[old_server]
    assert sha('/proc/%d/exe'%old_main)==MAIN and sha('/proc/%d/exe'%old_server)==SERVER
    assert '/dev/mem'in Path('/proc/%d/maps'%old_server).read_text()and sha(oldcore/'Dorado.rbf')==old_rbf
    assert not new.exists()and not core.exists(),'Inspect prior preparation/load; never blindly rerun'
    stem='dorado-rfront-ready-boot2' if repeat_boot2 else 'dorado-rfront-ready'
    main_log=Path('/tmp')/(stem+'-main.log');disk_log=Path('/tmp')/(stem+'-disk.jsonl')
    assert not main_log.exists()and not disk_log.exists()
    oldtrace=Path(c['main']['env']['DORADO_DISK_TRACE']);oldlog=Path(c['main_stdout'])
    oldtrace_offset=oldtrace.stat().st_size
    seed=base/'System_20261017/cedar-work-native-hints-f3bce2a8.pack';assert sha(seed)==SEED
    new.mkdir();core.mkdir();shutil.copy2(seed,new/'cedar-work.pack');assert sha(new/'cedar-work.pack')==SEED
    shutil.copy2(staging/'Dorado.rbf',core/'Dorado.rbf')
    old_mgl=oldcore/('CedarRFrontReady.mgl' if repeat_boot2 else 'CedarMC1668.mgl')
    mgl=old_mgl.read_text().replace(old.name,new.name)
    assert old.name not in mgl
    mgl_path=core/('CedarRFrontReadyBoot2.mgl' if repeat_boot2 else 'CedarRFrontReady.mgl')
    mgl_path.write_text(mgl)
    launcher='DoradoRFrontB2_20261021' if repeat_boot2 else 'Dorado'+new.name
    assert len(launcher)<=29,launcher
    mgl_path.write_text(mgl.replace('Dorado'+new.name,launcher))
    section='['+launcher+']'
    ini=Path('/media/fat/MiSTer.ini');text=ini.read_text();assert section not in text
    shutil.copy2(ini,new/'MiSTer.ini.before')
    ini.write_text(text+'\n'+section+'\nvideo_mode=1920,88,44,148,1080,4,5,36,74250,1,1\nvsync_adjust=0\nmain='+c['main']['exe']+'\n')
    for name in ['qualification.json','assembly-state.json','fit-review-controls.json']:shutil.copy2(staging/name,new/name)
    journal=dict(driver_pid=os.getpid(),started=time.time(),old_main=old_main,old_server=old_server,old_trace_offset=oldtrace_offset,phase='PREPARED')
    def save(): (new/'preparation.json').write_text(json.dumps(journal,indent=2)+'\n')
    save();print('READY_NATIVE prepared; requesting NORMAL Main loader',flush=True)
    fd=os.open('/dev/MiSTer_cmd',os.O_WRONLY)
    try:os.write(fd,('load_core '+str(mgl_path)+'\n').encode())
    finally:os.close(fd)
    journal['phase']='NORMAL_LOAD_REQUESTED';save()
    argv=[c['main']['exe'],str(core/'Dorado.rbf'),str(mgl_path)]
    for _ in range(500):
        found=matches(c['main']['exe'],argv)
        if len(found)==1 and not matches(c['main']['exe'],c['main']['argv']):break
        time.sleep(.1)
    else:raise RuntimeError('Observe same loader/process; do not issue another load')
    temporary=found[0];journal.update(temporary_main=temporary,phase='NEW_TEMPORARY_MAIN');save()
    time.sleep(1);assert sha('/proc/%d/exe'%temporary)==MAIN
    os.kill(temporary,signal.SIGTERM);stopped(temporary)
    for _ in range(50):
        if '/dev/mem'not in Path('/proc/%d/maps'%old_server).read_text():break
        time.sleep(.1)
    else:raise RuntimeError('Old server mapping was not revoked; inspect same process')
    no_open(old/'cedar-work.pack');no_open(new/'cedar-work.pack')
    assert sha(new/'cedar-work.pack')==SEED,'Seed changed before explicit firmware boot'
    with oldtrace.open('rb')as f:
        f.seek(oldtrace_offset);window=[json.loads(line)for line in f]
    assert window and window[-1]['event']=='barrier_end'
    assert sum(r['event']=='barrier_end'for r in window)>=2,window[-5:]
    recent_errors=[r for r in window if 'error'in r['event']or r.get('result',0)<0]
    assert not recent_errors,recent_errors
    prefix='through-rfront-ready-boot2-load' if repeat_boot2 else 'through-rfront-ready-load'
    trace_copy=old/('disk-'+prefix+'.jsonl');log_copy=old/('main-'+prefix+'.log')
    no_open(oldtrace);no_open(oldlog)
    shutil.copyfile(oldtrace,trace_copy);shutil.copyfile(oldlog,log_copy)
    assert sha(oldtrace)==sha(trace_copy)and sha(oldlog)==sha(log_copy)
    gaps=[];records=0
    with trace_copy.open('rb')as f:
        for line in f:
            r=json.loads(line);records+=1
            if r['event']=='trace_dropped':gaps.append(r)
    proof=dict(path=str(old/'cedar-work.pack'),bytes=(old/'cedar-work.pack').stat().st_size,sha256=sha(old/'cedar-work.pack'),
        disk_trace=str(trace_copy),disk_trace_sha256=sha(trace_copy),disk_records=records,historical_trace_gaps=gaps,
        transition_trace_gaps=[r for r in window if r['event']=='trace_dropped'],barriers=sum(r['event']=='barrier_end'for r in window),
        boundary='Normal Main loader and temporary Main exit; no pack descriptors open',
        guest=('First Ready/front boot reached Cedar microcode/Mesa trap loop without desktop; no clean shutdown or snapshot claim' if repeat_boot2 else 'Earlier desktop/marker observed; no clean guest shutdown, filesystem-integrity or FPGA snapshot claim'))
    (old/('preservation-before-rfront-ready-boot2.json' if repeat_boot2 else 'preservation-before-rfront-ready.json')).write_text(json.dumps(proof,indent=2)+'\n')
    # Only verified CLOSED logs are removed from tmpfs. Their original bytes
    # remain in the old volume directory. Other temporary artifacts stay put.
    reclaimed=oldtrace.stat().st_size+oldlog.stat().st_size
    oldtrace.unlink();oldlog.unlink()
    journal.update(phase='OLD_CLOSED_VOLUME_AND_LOGS_PRESERVED',preservation=proof,reclaimed_tmp_bytes=reclaimed);save()
    os.kill(old_server,signal.SIGTERM);stopped(old_server)
    assert not Path('/tmp/dorado-net.sock.control').exists()
    with(new/'server.log').open('ab')as f:
        sp=subprocess.Popen(c['server']['argv'],cwd=c['server']['cwd'],env=dict(os.environ,**c['server']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
    time.sleep(.3);assert sp.poll()is None
    c['main']['argv']=argv;c['main']['env']['DORADO_DISK_TRACE']=str(disk_log);c['main_stdout']=str(main_log)
    with main_log.open('ab')as f:
        mp=subprocess.Popen(argv,cwd=c['main']['cwd'],env=dict(os.environ,**c['main']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
    (new/'reboot-resume.json').write_text(json.dumps(c,indent=2)+'\n')
    d=dict(old_main=old_main,old_server=old_server,temporary_main=temporary,main_pid=mp.pid,server_pid=sp.pid,main_sha256=MAIN,server_sha256=SERVER,
        rbf_sha256=RBF,seed_sha256=SEED,preserved_old_volume=proof,main_log=str(main_log),disk_trace=str(disk_log),
        phase='Core loaded; original firmware boot pending; unchanged50MHz/SYSPER4')
    (new/'deployment.json').write_text(json.dumps(d,indent=2)+'\n')
    journal.update(phase='NEW_MAIN_SERVER_STARTED',main_pid=mp.pid,server_pid=sp.pid);save()
    for _ in range(300):
        assert mp.poll()is None and sp.poll()is None
        text=main_log.read_text();assert 'trace open failed'not in text and 'packet ownership held'not in text
        if 'Dorado disk: image cache ready,'in text and 'Dorado DDR: direct frame queues active,'in text:break
        time.sleep(.1)
    else:raise RuntimeError('Observe existing Main/server; cache/grant not ready')
    assert '/dev/mem'in Path('/proc/%d/maps'%sp.pid).read_text()
    journal['phase']='CACHE_AND_DDR_READY_FIRMWARE_PENDING';save();print(json.dumps(d,indent=2),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--staging',type=Path,required=True)
    p.add_argument('--repeat-boot2',action='store_true',help='Preserve the diagnosed first boot and repeat the same image from the original seed')
    a=p.parse_args();deploy(a.staging.resolve(),a.repeat_boot2)
