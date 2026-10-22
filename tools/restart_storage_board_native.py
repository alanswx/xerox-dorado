#!/usr/bin/env python3
"""Restart the reviewed57.1MHz core through Main barriers with the fixed daemon.

Preserve the old closed volume/logs. Default to a cold remount of an exact
copy of that closed volume; --fresh-seed selects the original seed instead.
Never copy a live pack, preserve live BSP context across daemon replacement,
import guest state or access SPI outside Main. Inspect the journal after any
interruption rather than repeating a load on an observation timeout.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import signal
import subprocess
import time

from deploy_mc1668_native import MAIN, SERVER, SEED, sha, matches, stopped
from deploy_r_front_ready_native import no_open, rewrite_mgl
from deploy_storage_board_native import image

FIXED_SERVER = '08452a86d9708d70cca47c82d6bc5f42294393a086c5d95a1faede533bbdfc2c'


def restart(a):
    base = Path('/media/fat/games/DoradoRTL')
    for name in (a.previous, a.name, a.launcher, a.tag):
        assert re.fullmatch(r'[A-Za-z0-9_-]+', name), name
    assert len(a.launcher) <= 29 and a.previous != a.name
    expected = image(a.staging)
    old, new = base / a.previous, base / a.name
    old_d = json.loads((old / 'deployment.json').read_text())
    c = json.loads((old / 'reboot-resume.json').read_text())
    old_main, old_server = old_d['main_pid'], old_d['server_pid']
    assert old_d['rbf_sha256'] == expected
    assert old_d['server_sha256'] in (SERVER, FIXED_SERVER)
    assert matches(c['main']['exe'], c['main']['argv']) == [old_main]
    assert matches(c['server']['exe'], c['server']['argv']) == [old_server]
    assert sha('/proc/%d/exe' % old_main) == MAIN
    assert sha('/proc/%d/exe' % old_server) == old_d['server_sha256']
    assert sha(c['main']['argv'][1]) == expected
    assert '/dev/mem' in Path('/proc/%d/maps' % old_server).read_text()
    assert sha(a.daemon) == FIXED_SERVER
    boot = json.loads(a.guest_boot.read_text())
    assert boot['main_pid'] == old_main and boot['rbf_sha256'] == expected
    assert boot['observed_mp'] in (812, 845) and boot['normal_guest_keyboard_Boot']
    core = Path('/media/fat/_Computer/DoradoRTL_' + a.name)
    assert not new.exists() and not core.exists(), 'Inspect existing preparation, do not repeat'
    mainlog = Path('/tmp/dorado-' + a.tag + '-main.log')
    disklog = Path('/tmp/dorado-' + a.tag + '-disk.jsonl')
    assert not mainlog.exists() and not disklog.exists()
    oldtrace, oldlog = Path(c['main']['env']['DORADO_DISK_TRACE']), Path(c['main_stdout'])
    # Start on a complete-line boundary even if the producer was appending.
    size = oldtrace.stat().st_size
    with oldtrace.open('rb') as stream:
        start = max(0, size - 4096)
        stream.seek(start)
        tail = stream.read(size - start)
    assert b'\n' in tail
    offset = start + tail.rfind(b'\n') + 1
    seed = base / 'System_20261017/cedar-work-native-hints-f3bce2a8.pack'
    assert sha(seed) == SEED
    old_mgl = Path(c['main']['argv'][2])
    mgl = rewrite_mgl(old_mgl.read_text(), a.previous, a.name, a.launcher)
    ini = Path('/media/fat/MiSTer.ini')
    text = ini.read_text()
    section = '[' + a.launcher + ']'
    assert section not in text
    system = base / 'System_20261022_RxOrder'
    system.mkdir(exist_ok=True)
    daemon = system / 'dorado-server-08452a86d9708d70'
    if daemon.exists():
        assert sha(daemon) == FIXED_SERVER
    else:
        shutil.copy2(a.daemon, daemon)
        assert sha(daemon) == FIXED_SERVER
    new.mkdir()
    core.mkdir()
    shutil.copy2(seed, new / 'cedar-work.pack')
    assert sha(new / 'cedar-work.pack') == SEED
    shutil.copy2(a.staging / 'Dorado.rbf', core / 'Dorado.rbf')
    assert sha(core / 'Dorado.rbf') == expected
    mgl_path = core / 'Boot.mgl'
    mgl_path.write_text(mgl)
    shutil.copy2(ini, new / 'MiSTer.ini.before')
    ini.write_text(text + '\n' + section + '\nvideo_mode=1920,88,44,148,1080,4,5,36,74250,1,1\nvsync_adjust=0\nmain=' + c['main']['exe'] + '\n')
    for n in ['qualification.json', 'assembly-state.json', 'fit-review-controls.json',
              'assembly-guard-controls.json', 'storage-board-native.json']:
        shutil.copy2(a.staging / n, new / n)
    shutil.copy2(a.guest_boot, new / 'preceding-guest-boot.json')
    journal = dict(driver_pid=os.getpid(), started=time.time(), old_main=old_main,
                   old_server=old_server, old_trace_offset=offset, phase='PREPARED',
                   source='original seed' if a.fresh_seed else 'closed marked preceding volume')

    def save():
        f = new / 'preparation.json'
        temp = f.with_suffix('.json.tmp')
        temp.write_text(json.dumps(journal, indent=2) + '\n')
        os.replace(temp, f)

    save()
    Path('/dev/MiSTer_cmd').write_text('load_core ' + str(mgl_path) + '\n')
    journal['phase'] = 'NORMAL_LOAD_REQUESTED'
    save()
    argv = [c['main']['exe'], str(core / 'Dorado.rbf'), str(mgl_path)]
    for _ in range(500):
        found = matches(c['main']['exe'], argv)
        if len(found) == 1 and not matches(c['main']['exe'], c['main']['argv']):
            break
        time.sleep(.1)
    else:
        raise RuntimeError('Observe the existing loader, never issue a second load')
    temporary = found[0]
    journal.update(temporary_main=temporary, phase='TEMPORARY_NEW_MAIN')
    save()
    time.sleep(1)
    assert sha('/proc/%d/exe' % temporary) == MAIN
    os.kill(temporary, signal.SIGTERM)
    stopped(temporary)
    for _ in range(50):
        if '/dev/mem' not in Path('/proc/%d/maps' % old_server).read_text():
            break
        time.sleep(.1)
    else:
        raise RuntimeError('Old mapping not revoked; inspect existing server')
    for path in [old / 'cedar-work.pack', new / 'cedar-work.pack', oldtrace, oldlog]:
        no_open(path)
    assert sha(new / 'cedar-work.pack') == SEED
    with oldtrace.open('rb') as stream:
        stream.seek(offset)
        window = [json.loads(line) for line in stream]
    assert window and window[-1]['event'] == 'barrier_end'
    assert sum(x['event'] == 'barrier_end' for x in window) >= 2
    assert not any('error' in x['event'] or x.get('result', 0) < 0 for x in window)
    tracecopy, logcopy = old / ('disk-through-' + a.tag + '.jsonl'), old / ('main-through-' + a.tag + '.log')
    assert not tracecopy.exists() and not logcopy.exists()
    shutil.copyfile(oldtrace, tracecopy)
    shutil.copyfile(oldlog, logcopy)
    assert sha(oldtrace) == sha(tracecopy) and sha(oldlog) == sha(logcopy)
    records, gaps, errors = 0, [], []
    with tracecopy.open('rb') as stream:
        for line in stream:
            row = json.loads(line)
            records += 1
            if row['event'] == 'trace_dropped':
                gaps.append(row)
            if 'error' in row['event'] or row.get('result', 0) < 0:
                errors.append(row)
    assert not errors, errors
    closed_sha = sha(old / 'cedar-work.pack')
    preserved = dict(path=str(old / 'cedar-work.pack'), bytes=(old / 'cedar-work.pack').stat().st_size,
                     sha256=closed_sha, disk_trace=str(tracecopy), disk_trace_sha256=sha(tracecopy),
                     main_log=str(logcopy), main_log_sha256=sha(logcopy), disk_records=records,
                     trace_gaps=gaps, transition_trace_gaps=[x for x in window if x['event'] == 'trace_dropped'],
                     barriers=sum(x['event'] == 'barrier_end' for x in window), preceding_guest_boot=boot,
                     boundary='Normal guest Boot observed, then Main load/exit, no open pack/log descriptors',
                     limits='Cold marker readback and full filesystem integrity not yet established')
    (old / ('preservation-before-' + a.tag + '.json')).write_text(json.dumps(preserved, indent=2) + '\n')
    if not a.fresh_seed:
        shutil.copyfile(old / 'cedar-work.pack', new / 'cedar-work.pack')
    source_sha = SEED if a.fresh_seed else closed_sha
    assert sha(new / 'cedar-work.pack') == source_sha
    journal.update(phase='CLOSED_VOLUME_LOGS_PRESERVED_SOURCE_COPIED', preservation=preserved,
                   source_pack_sha256=source_sha)
    save()
    oldtrace.unlink()
    oldlog.unlink()
    os.kill(old_server, signal.SIGTERM)
    stopped(old_server)
    assert not Path('/tmp/dorado-net.sock.control').exists()
    c['server']['exe'] = str(daemon)
    c['server']['argv'][0] = str(daemon)
    with (new / 'server.log').open('ab') as stream:
        server = subprocess.Popen(c['server']['argv'], cwd=c['server']['cwd'],
                                  env=dict(os.environ, **c['server']['env']), stdin=subprocess.DEVNULL,
                                  stdout=stream, stderr=subprocess.STDOUT, start_new_session=True)
    time.sleep(.3)
    assert server.poll() is None
    c['main']['argv'] = argv
    c['main']['env']['DORADO_DISK_TRACE'] = str(disklog)
    c['main_stdout'] = str(mainlog)
    with mainlog.open('ab') as stream:
        main = subprocess.Popen(argv, cwd=c['main']['cwd'], env=dict(os.environ, **c['main']['env']),
                                stdin=subprocess.DEVNULL, stdout=stream, stderr=subprocess.STDOUT,
                                start_new_session=True)
    (new / 'reboot-resume.json').write_text(json.dumps(c, indent=2) + '\n')
    result = dict(main_pid=main.pid, server_pid=server.pid, temporary_main=temporary,
                  main_sha256=MAIN, server_sha256=FIXED_SERVER, rbf_sha256=expected,
                  source_pack_sha256=source_sha, original_seed=a.fresh_seed,
                  preserved_old_volume=preserved, main_log=str(mainlog), disk_trace=str(disklog),
                  host_clock_hz=57142857, sysper=4, phase='Loaded; memory/original firmware/cold marker pending')
    (new / 'deployment.json').write_text(json.dumps(result, indent=2) + '\n')
    journal.update(phase='NEW_MAIN_SERVER_STARTED', main_pid=main.pid, server_pid=server.pid)
    save()
    for _ in range(300):
        assert main.poll() is None and server.poll() is None
        text = mainlog.read_text()
        assert 'trace open failed' not in text and 'packet ownership held' not in text
        if 'Dorado disk: image cache ready,' in text and 'Dorado DDR: direct frame queues active,' in text:
            break
        time.sleep(.1)
    else:
        raise RuntimeError('Observe existing Main/server; cache/DDR not ready')
    assert sha('/proc/%d/exe' % server.pid) == FIXED_SERVER
    assert '/dev/mem' in Path('/proc/%d/maps' % server.pid).read_text()
    journal['phase'] = 'CACHE_AND_DDR_READY_MEMORY_FIRMWARE_PENDING'
    save()
    print(json.dumps(result, indent=2), flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['staging', 'daemon', 'guest-boot']:
        p.add_argument('--' + n, type=Path, required=True)
    for n in ['previous', 'name', 'launcher', 'tag']:
        p.add_argument('--' + n, required=True)
    p.add_argument('--fresh-seed', action='store_true')
    restart(p.parse_args())
