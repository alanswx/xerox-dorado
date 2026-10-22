#!/usr/bin/env python3
"""Load the reviewed disk-window image once through existing Main on MiSTer14.

Preserve the preceding CLOSED checkpoint pack and barrier logs. Keep Main and
the receiver binaries unchanged, with a separate fresh seed and log paths.
No C runtime import or host completion of guest state. Inspect the journal
after any timeout; never repeat a load or signal on an observation timeout.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import signal
import subprocess
import time

MAIN = '21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
SERVER = '08452a86d9708d70cca47c82d6bc5f42294393a086c5d95a1faede533bbdfc2c'
OLD_RBF = '7c4d39e414e5de8bafae5d3ff0fd0dfddae310c79ff17d87e5a1e8b97ee24b8a'
SEED = 'f4203ce8cef17d60b88471fac1904fdd45fe160bb43a6e4e77ec95d6e9b2a8c4'
BASE = Path('/media/fat/games/DoradoRTL')


def sha(path):
    h = hashlib.sha256()
    with Path(path).open('rb') as f:
        for b in iter(lambda: f.read(65536), b''):
            h.update(b)
    return h.hexdigest()


def image(stage):
    q, c, g, a, r = [json.loads((stage / n).read_text()) for n in
                     ['qualification.json', 'fit-review-controls.json', 'assembly-guard-controls.json', 'assembly-state.json', 'pack-window-native.json']]
    assert q['status'] == 'PASS_NATIVE_PACK_WINDOW_PREASSEMBLY_REVIEW'
    assert q['frequency_hz'] == 57142857 and q['sysper'] == 4 and q['state_admin'] == 'INACTIVE'
    assert len(q['source_hashes']) == 503 and q['source_hashes'] == r['source_hashes']
    assert q['minimum_setup_ns'] > 0 and q['minimum_hold_ns'] > 0
    assert len(q['timing_checks']) == 31 and len(q['detailed_paths']) == 332
    assert q['sample_registers'] == 2048 and q['resources']['ram_blocks'] == 332
    assert q['memory']['cache']['packing'] == 'independent40'
    assert not q['firmware_qualified'] and not q['hardware_speedup_qualified']
    assert c['status'] == 'PASS_NATIVE_PACK_WINDOW_REVIEW_REJECTION_CONTROLS'
    assert len(c['cases']) == 16 and all(x['rejected'] and x['returncode'] != 0 for x in c['cases'])
    assert g['status'] == 'PASS_NATIVE_PACK_WINDOW_ASSEMBLY_GUARD_CONTROLS'
    assert len(g['cases']) == 7 and all(x['rejected'] for x in g['cases'])
    assert a['status'] == 'TERMINAL_ASSEMBLED_REVIEWED_PACK_WINDOW' and a['returncode'] == 0
    assert a['qualification_sha256'] == c['positive_qualification_sha256'] == g['qualification_sha256'] == sha(stage / 'qualification.json')
    assert a['controls_sha256'] == g['reviewer_controls_sha256'] == sha(stage / 'fit-review-controls.json')
    assert sha(stage / 'Dorado.rbf') == a['rbf_sha256']
    assert sha(stage / 'fresh-seed.pack') == SEED
    return a['rbf_sha256']


def matches(exe, argv):
    found = []
    for proc in Path('/proc').iterdir():
        if not proc.name.isdigit():
            continue
        try:
            if os.readlink(proc / 'exe') == exe and (proc / 'cmdline').read_bytes().split(b'\0')[:-1] == [s.encode() for s in argv]:
                found.append(int(proc.name))
        except (FileNotFoundError, PermissionError, ProcessLookupError):
            pass
    return found


def stopped(pid):
    for _ in range(300):
        if not Path('/proc/%d/exe' % pid).exists():
            return
        time.sleep(.1)
    raise RuntimeError('Same process has not exited; observe it, do not repeat signal')


def no_open(path):
    for proc in Path('/proc').iterdir():
        if not proc.name.isdigit():
            continue
        try:
            for fd in (proc / 'fd').iterdir():
                try:
                    assert os.readlink(fd) != str(path), ('Still open', path, proc.name, fd)
                except OSError:
                    pass
        except (FileNotFoundError, PermissionError, ProcessLookupError):
            pass


def deploy(stage):
    assert '10.0.2.14/' in subprocess.check_output(['ip', '-4', 'addr', 'show'], text=True)
    expected = image(stage)
    old, new = BASE / 'CedarStorageBoard57_Link1_20261022', BASE / 'CedarPackWindow57_20261022'
    oldcore = Path('/media/fat/_Computer/DoradoRTL_' + old.name)
    core = Path('/media/fat/_Computer/DoradoRTL_' + new.name)
    c = json.loads((old / 'reboot-resume.json').read_text())
    assert matches(c['main']['exe'], c['main']['argv']) == [31285]
    assert matches(c['server']['exe'], c['server']['argv']) == [31284]
    assert sha('/proc/31285/exe') == MAIN and sha('/proc/31284/exe') == SERVER
    assert sha(oldcore / 'Dorado.rbf') == OLD_RBF
    assert not new.exists() and not core.exists(), 'Inspect existing journal/processes; do not repeat'
    ini = Path('/media/fat/MiSTer.ini');text = ini.read_text()
    section = '[Dorado57Window1]';assert section not in text
    mgl = (oldcore / 'Boot.mgl').read_text().replace(old.name, new.name)
    mgl, count = re.subn(r'(<setname\b[^>]*>)[^<]*(</setname>)', r'\1Dorado57Window1\2', mgl)
    assert count == 1 and old.name not in mgl
    oldtrace = Path(c['main']['env']['DORADO_DISK_TRACE']);oldlog = Path(c['main_stdout'])
    offset = oldtrace.stat().st_size
    new.mkdir();core.mkdir()
    shutil.copyfile(stage / 'fresh-seed.pack', new / 'cedar-work.pack')
    shutil.copyfile(stage / 'Dorado.rbf', core / 'Dorado.rbf')
    assert sha(new / 'cedar-work.pack') == SEED and sha(core / 'Dorado.rbf') == expected
    for n in ['qualification.json', 'fit-review-controls.json', 'assembly-guard-controls.json', 'assembly-state.json', 'pack-window-native.json', 'fresh-seed.pack.boot-links.json']:
        shutil.copyfile(stage / n, new / n)
    shutil.copyfile(ini, new / 'MiSTer.ini.before')
    (core / 'Boot.mgl').write_text(mgl)
    ini.write_text(text + '\n' + section + '\nvideo_mode=1920,88,44,148,1080,4,5,36,74250,1,1\nvsync_adjust=0\nmain=' + c['main']['exe'] + '\n')
    path = new / 'preparation.json'
    journal = dict(status='PREPARED', driver_pid=os.getpid(), started=time.time(), old_main=31285,
                   old_server=31284, old_trace_offset=offset, rbf_sha256=expected, seed_sha256=SEED,
                   scope='Existing Main normal load and disk/network exit barriers; new final cache/server. No clean Cedar shutdown or clean latency claim.')

    def save():
        temp = path.with_suffix('.json.tmp');temp.write_text(json.dumps(journal, indent=2) + '\n');os.replace(temp, path)

    save()
    journal['status'] = 'NORMAL_FPGA_LOAD_REQUESTED';save()
    Path('/dev/MiSTer_cmd').write_text('load_core ' + str(core / 'Boot.mgl') + '\n')
    argv = [c['main']['exe'], str(core / 'Dorado.rbf'), str(core / 'Boot.mgl')]
    for _ in range(500):
        found = matches(c['main']['exe'], argv)
        if len(found) == 1 and not Path('/proc/31285/exe').exists():
            break
        time.sleep(.1)
    else:
        raise RuntimeError('Observe the existing loader; never request another load')
    temporary = found[0]
    journal.update(status='FPGA_LOADED_TEMPORARY_MAIN', temporary_main=temporary);save()
    assert sha('/proc/%d/exe' % temporary) == MAIN
    time.sleep(1)
    journal['status'] = 'TEMPORARY_MAIN_NORMAL_EXIT_REQUESTED';save()
    os.kill(temporary, signal.SIGTERM);stopped(temporary)
    for _ in range(100):
        if '/dev/mem' not in Path('/proc/31284/maps').read_text():
            break
        time.sleep(.1)
    else:
        raise RuntimeError('Observe same daemon; DDR mapping has not been revoked')
    for f in [old / 'cedar-work.pack', new / 'cedar-work.pack', oldtrace]:
        no_open(f)
    assert sha(new / 'cedar-work.pack') == SEED, 'Seed changed during first attachment: preserve/inspect; do not repeat load'
    return finish_closed(stage, journal)


def finish_closed(stage, journal):
    """Continue only after actual Main exits and mapping revoke; no FPGA load."""
    assert journal['status'] == 'TEMPORARY_MAIN_NORMAL_EXIT_REQUESTED'
    old, new = BASE / 'CedarStorageBoard57_Link1_20261022', BASE / 'CedarPackWindow57_20261022'
    core = Path('/media/fat/_Computer/DoradoRTL_' + new.name)
    path = new / 'preparation.json'
    c = json.loads((old / 'reboot-resume.json').read_text())
    temporary = journal['temporary_main'];expected = image(stage)
    argv = [c['main']['exe'], str(core / 'Dorado.rbf'), str(core / 'Boot.mgl')]
    assert not Path('/proc/31285/exe').exists() and not Path('/proc/%d/exe' % temporary).exists()
    assert not matches(c['main']['exe'], c['main']['argv']) and not matches(c['main']['exe'], argv)
    assert sha('/proc/31284/exe') == SERVER and '/dev/mem' not in Path('/proc/31284/maps').read_text()
    assert sha(core / 'Dorado.rbf') == expected and sha(new / 'cedar-work.pack') == SEED
    oldtrace = Path(c['main']['env']['DORADO_DISK_TRACE']);oldlog = Path(c['main_stdout'])
    for f in [old / 'cedar-work.pack', new / 'cedar-work.pack', oldtrace]:
        no_open(f)

    def save():
        temp = path.with_suffix('.json.tmp');temp.write_text(json.dumps(journal, indent=2) + '\n');os.replace(temp, path)

    rows = [json.loads(line) for line in oldtrace.read_text().splitlines()]
    errors = [r for r in rows if 'error' in r['event'] or r.get('result', 0) < 0 or
              (r['event'] in ['pread_end', 'pwrite_end'] and r['result'] != r['bytes'])]
    gaps = [r for r in rows if r['event'] == 'trace_dropped']
    # Historical trace-ring loss is retained as an observability limitation.
    # Require a complete, error/gap-free current close window and real barriers.
    with oldtrace.open('rb') as f:
        offset = journal['old_trace_offset']
        if offset:
            f.seek(offset - 1)
            if f.read(1) != b'\n':
                f.readline()
            else:
                f.seek(offset)
        window = [json.loads(line) for line in f]
    assert window and not errors and rows[-1]['event'] == 'barrier_end'
    assert not any(r['event'] == 'trace_dropped' for r in window)
    assert sum(r['event'] == 'barrier_end' for r in window) >= 2
    closed = dict(status='CLOSED_CHECKPOINT_PACK_PRESERVED', main_pid=31285, temporary_main=temporary,
                  pack=str(old / 'cedar-work.pack'), pack_sha256=sha(old / 'cedar-work.pack'),
                  pack_bytes=(old / 'cedar-work.pack').stat().st_size, disk_records=len(rows),
                  disk_errors=errors, trace_gaps=gaps, last_event=rows[-1],
                  close_window_records=len(window), close_window_trace_gaps=[],
                  no_open_pack_and_trace=True, daemon_mapping_revoked=True,
                  limits='Historical diagnostic trace overflow retained; no clean full-trace/latency/stress claim. Current close window has no errors/gaps and normal barriers. Includes unchanged fresh-seed initial attachment. Not a guest clean shutdown.')
    for source, name in [(oldtrace, 'disk-through-window-load-close.jsonl'), (oldlog, 'main-through-window-load-close.log')]:
        target = old / name;assert not target.exists();shutil.copyfile(source, target)
        closed[name + '_sha256'] = sha(target)
    (old / 'checkpoint-volume-close.json').write_text(json.dumps(closed, indent=2) + '\n')
    journal.update(status='OLD_CHECKPOINT_VOLUME_CLOSED_PRESERVED', closed=closed);save()
    os.kill(31284, signal.SIGTERM);stopped(31284)
    assert not Path(c['server']['env']['DORADO_NET_CONTROL']).exists()
    with (new / 'server.log').open('xb') as log:
        server = subprocess.Popen(c['server']['argv'], cwd=c['server']['cwd'], env=dict(os.environ, **c['server']['env']),
                                  stdout=log, stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL, start_new_session=True)
    time.sleep(.3);assert server.poll() is None
    c['main']['argv'] = argv;c['main']['env']['DORADO_DISK_TRACE'] = str(new / 'disk.jsonl')
    c['main_stdout'] = str(new / 'main.log')
    with (new / 'main.log').open('xb') as log:
        main = subprocess.Popen(argv, cwd=c['main']['cwd'], env=dict(os.environ, **c['main']['env']),
                                stdout=log, stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL, start_new_session=True)
    (new / 'reboot-resume.json').write_text(json.dumps(c, indent=2) + '\n')
    result = dict(main_pid=main.pid, server_pid=server.pid, main_sha256=MAIN, server_sha256=SERVER,
                  rbf_sha256=expected, source_pack_sha256=SEED, host_clock_hz=57142857, sysper=4,
                  phase='ORIGINAL_MEMORY_FIRMWARE_CEDAR_PENDING', actual_loader_main_pid=temporary,
                  main_log=str(new / 'main.log'), disk_trace=str(new / 'disk.jsonl'))
    (new / 'deployment.json').write_text(json.dumps(result, indent=2) + '\n')
    journal.update(status='FINAL_MAIN_SERVER_STARTED', **result);save()
    for _ in range(300):
        assert main.poll() is None and server.poll() is None
        t = (new / 'main.log').read_text()
        if 'Dorado disk: image cache ready,' in t and 'Dorado DDR: direct frame queues active,' in t:
            break
        time.sleep(.1)
    else:
        raise RuntimeError('Observe same Main/server; readiness pending, do not reload')
    assert sha('/proc/%d/exe' % main.pid) == MAIN and sha('/proc/%d/exe' % server.pid) == SERVER
    assert '/dev/mem' in Path('/proc/%d/maps' % server.pid).read_text()
    assert sha(old / 'cedar-work.pack') == closed['pack_sha256']
    # The close audit can contain130K records. Release it before waiting on
    # Main; the supervisor must not retain a second disk-cache-sized heap.
    del rows, window, errors, gaps
    journal['status'] = 'FINAL_CACHE_DDR_READY_ORIGINAL_BOOT_PENDING';save()
    print(json.dumps(result, indent=2), flush=True)
    # Keep this parent available to reap/observe children. No restart loop.
    journal['final_main_exit_code'] = main.wait();journal['final_main_exited'] = time.time();save()


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__);p.add_argument('--staging', type=Path, required=True)
    p.add_argument('--check-only', action='store_true')
    p.add_argument('--resume-closed', action='store_true', help='Continue an already loaded, actually closed deployment; never reload or signal old Main')
    a = p.parse_args()
    if a.check_only:
        print('PASS_REVIEWED_PACK_WINDOW_IMAGE', image(a.staging.resolve()))
    elif a.resume_closed:
        journal = json.loads((BASE / 'CedarPackWindow57_20261022/preparation.json').read_text())
        journal['continuation_driver_pid'] = os.getpid()
        finish_closed(a.staging.resolve(), journal)
    else:
        deploy(a.staging.resolve())
