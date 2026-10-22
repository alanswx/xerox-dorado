#!/usr/bin/env python3
"""Load the reviewed57.1MHz board-ready candidate on MiSTer14 from a fresh seed.

Use normal Main loader/disk/network barriers. Preserve the old closed pack
and exact logs before reclaiming their tmpfs copies. Keep Main the sole SPI
owner and unchanged server binaries. This is not a clean Cedar shutdown or
filesystem-integrity claim. Inspect the journal after any interruption; never
repeat a load because an observation window expired.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import signal
import subprocess
import time

from deploy_mc1668_native import MAIN, SERVER, SEED, sha, matches, stopped
from deploy_r_front_ready_native import no_open, rewrite_mgl

OLD_RBF = '0c578915110c59c546456a2821e26a0fcfdffe671668ffb27ad3e3caedd459b2'


def image(staging):
    q = json.loads((staging / 'qualification.json').read_text())
    a = json.loads((staging / 'assembly-state.json').read_text())
    c = json.loads((staging / 'fit-review-controls.json').read_text())
    g = json.loads((staging / 'assembly-guard-controls.json').read_text())
    r = json.loads((staging / 'storage-board-native.json').read_text())
    assert q['status'] == 'PASS_NATIVE_STORAGE_BOARD_PREASSEMBLY_REVIEW'
    assert q['frequency_hz'] == 57142857 and q['sysper'] == 4 and q['state_admin'] == 'INACTIVE'
    assert len(q['source_hashes']) == 503 and q['source_hashes'] == r['source_hashes']
    assert q['minimum_setup_ns'] > 0 and q['minimum_hold_ns'] > 0
    assert len(q['timing_checks']) == 31 and len(q['detailed_paths']) == 316
    assert q['resources']['ram_blocks'] == 330 and q['ready_registers'] == 8
    assert r['CPU_cache_map_ECC_peripheral_logic_unchanged']
    assert a['status'] == 'TERMINAL_ASSEMBLED_REVIEWED_STORAGE_BOARD' and a['returncode'] == 0
    assert a['qualification_sha256'] == sha(staging / 'qualification.json')
    assert a['controls_sha256'] == sha(staging / 'fit-review-controls.json')
    assert c['status'] == 'PASS_NATIVE_STORAGE_BOARD_REVIEW_REJECTION_CONTROLS'
    assert c['positive_qualification_sha256'] == a['qualification_sha256']
    assert len(c['cases']) == 18 and all(x['rejected'] and x['returncode'] != 0 for x in c['cases'])
    assert g['status'] == 'PASS_NATIVE_STORAGE_BOARD_ASSEMBLY_GUARD_CONTROLS'
    assert len(g['cases']) == 6 and all(x['rejected'] for x in g['cases'])
    assert g['qualification_sha256'] == a['qualification_sha256']
    assert g['reviewer_controls_sha256'] == a['controls_sha256']
    assert not q['firmware_qualified'] and not q['hardware_speedup_qualified']
    assert sha(staging / 'Dorado.rbf') == a['rbf_sha256']
    return a['rbf_sha256']


def deploy(staging):
    expected = image(staging)
    base = Path('/media/fat/games/DoradoRTL')
    old = base / 'CedarStorageReadCapture1_20261021'
    new = base / 'CedarStorageBoard57_20261022'
    oldcore = Path('/media/fat/_Computer/DoradoRTL_' + old.name)
    core = Path('/media/fat/_Computer/DoradoRTL_' + new.name)
    c = json.loads((old / 'reboot-resume.json').read_text())
    assert matches(c['main']['exe'], c['main']['argv']) == [14025]
    assert matches(c['server']['exe'], c['server']['argv']) == [14024]
    assert sha('/proc/14025/exe') == MAIN and sha('/proc/14024/exe') == SERVER
    assert sha(oldcore / 'Dorado.rbf') == OLD_RBF
    assert '/dev/mem' in Path('/proc/14024/maps').read_text()
    assert not new.exists() and not core.exists(), 'Inspect existing preparation; never blindly repeat'
    mainlog = Path('/tmp/dorado-storage-board57-main.log')
    disklog = Path('/tmp/dorado-storage-board57-disk.jsonl')
    assert not mainlog.exists() and not disklog.exists()
    oldtrace = Path(c['main']['env']['DORADO_DISK_TRACE'])
    oldlog = Path(c['main_stdout'])
    offset = oldtrace.stat().st_size
    seed = base / 'System_20261017/cedar-work-native-hints-f3bce2a8.pack'
    assert sha(seed) == SEED
    mgl = rewrite_mgl((oldcore / 'CedarStorageReadCapture1.mgl').read_text(),
                      old.name, new.name, 'DoradoBoard57_20261022')
    ini = Path('/media/fat/MiSTer.ini')
    text = ini.read_text()
    section = '[DoradoBoard57_20261022]'
    assert section not in text
    new.mkdir()
    core.mkdir()
    shutil.copy2(seed, new / 'cedar-work.pack')
    assert sha(new / 'cedar-work.pack') == SEED
    shutil.copy2(staging / 'Dorado.rbf', core / 'Dorado.rbf')
    assert sha(core / 'Dorado.rbf') == expected
    mgl_path = core / 'CedarStorageBoard57.mgl'
    mgl_path.write_text(mgl)
    shutil.copy2(ini, new / 'MiSTer.ini.before')
    ini.write_text(text + '\n' + section + '\nvideo_mode=1920,88,44,148,1080,4,5,36,74250,1,1\nvsync_adjust=0\nmain=' + c['main']['exe'] + '\n')
    for n in ['qualification.json', 'assembly-state.json', 'fit-review-controls.json',
              'assembly-guard-controls.json', 'storage-board-native.json']:
        shutil.copy2(staging / n, new / n)
    journal = dict(driver_pid=os.getpid(), started=time.time(), old_main=14025,
                   old_server=14024, old_trace_offset=offset, phase='PREPARED')

    def save():
        (new / 'preparation.json').write_text(json.dumps(journal, indent=2) + '\n')

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
        raise RuntimeError('Observe the same loader; do not request another load')
    temporary = found[0]
    journal.update(temporary_main=temporary, phase='TEMPORARY_NEW_MAIN')
    save()
    time.sleep(1)
    assert sha('/proc/%d/exe' % temporary) == MAIN
    os.kill(temporary, signal.SIGTERM)
    stopped(temporary)
    for _ in range(50):
        if '/dev/mem' not in Path('/proc/14024/maps').read_text():
            break
        time.sleep(.1)
    else:
        raise RuntimeError('Old server mapping not revoked; inspect existing process')
    no_open(old / 'cedar-work.pack')
    no_open(new / 'cedar-work.pack')
    assert sha(new / 'cedar-work.pack') == SEED
    with oldtrace.open('rb') as stream:
        stream.seek(offset)
        window = [json.loads(line) for line in stream]
    assert window and window[-1]['event'] == 'barrier_end'
    assert sum(x['event'] == 'barrier_end' for x in window) >= 2
    assert not any('error' in x['event'] or x.get('result', 0) < 0 for x in window)
    no_open(oldtrace)
    no_open(oldlog)
    tracecopy = old / 'disk-through-storage-board57-load.jsonl'
    logcopy = old / 'main-through-storage-board57-load.log'
    assert not tracecopy.exists() and not logcopy.exists()
    shutil.copyfile(oldtrace, tracecopy)
    shutil.copyfile(oldlog, logcopy)
    assert sha(oldtrace) == sha(tracecopy) and sha(oldlog) == sha(logcopy)
    records = 0
    gaps = []
    with tracecopy.open('rb') as stream:
        for line in stream:
            record = json.loads(line)
            records += 1
            if record['event'] == 'trace_dropped':
                gaps.append(record)
    preserved = dict(path=str(old / 'cedar-work.pack'), sha256=sha(old / 'cedar-work.pack'),
                     bytes=(old / 'cedar-work.pack').stat().st_size,
                     disk_trace=str(tracecopy), disk_trace_sha256=sha(tracecopy),
                     main_log=str(logcopy), main_log_sha256=sha(logcopy),
                     disk_records=records, historical_trace_gaps=gaps,
                     transition_trace_gaps=[x for x in window if x['event'] == 'trace_dropped'],
                     barriers=sum(x['event'] == 'barrier_end' for x in window),
                     boundary='Normal Main load/exit, no open pack or log descriptors',
                     guest='Interactive Cedar/input/warm file passed; no clean Cedar shutdown or cold-persistence claim')
    (old / 'preservation-before-storage-board57.json').write_text(json.dumps(preserved, indent=2) + '\n')
    reclaimed = oldtrace.stat().st_size + oldlog.stat().st_size
    oldtrace.unlink()
    oldlog.unlink()
    journal.update(phase='OLD_CLOSED_PACK_AND_LOGS_PRESERVED', preservation=preserved,
                   reclaimed_tmp_bytes=reclaimed)
    save()
    os.kill(14024, signal.SIGTERM)
    stopped(14024)
    assert not Path('/tmp/dorado-net.sock.control').exists()
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
                  main_sha256=MAIN, server_sha256=SERVER, rbf_sha256=expected, seed_sha256=SEED,
                  preserved_old_volume=preserved, main_log=str(mainlog), disk_trace=str(disklog),
                  host_clock_hz=57142857, sysper=4, phase='Loaded; physical memory and original firmware pending')
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
        raise RuntimeError('Observe the same Main/server; cache/DDR not ready')
    assert '/dev/mem' in Path('/proc/%d/maps' % server.pid).read_text()
    journal['phase'] = 'CACHE_AND_DDR_READY_MEMORY_FIRMWARE_PENDING'
    save()
    print(json.dumps(result, indent=2), flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--staging', type=Path, required=True)
    a = p.parse_args()
    deploy(a.staging.resolve())
