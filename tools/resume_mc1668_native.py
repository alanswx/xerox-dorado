#!/usr/bin/env python3
"""Continue the one interrupted MC1668 load; do not issue another load request.

Native59's historical trace gap is preserved and reported as incomplete evidence.
This does not qualify its guest filesystem or claim a clean guest shutdown.
The new original seed and normal Main/server binaries are independently checked.
"""
from pathlib import Path
import argparse
import json
import os
import shutil
import signal
import subprocess
import time

from deploy_mc1668_native import MAIN, SERVER, SEED, OLD_RBF, sha, matches, stopped


def resume(staging):
    base = Path('/media/fat/games/DoradoRTL')
    old = base / 'Cedar59_20261019'
    new = base / 'CedarMC1668_20261020'
    core = Path('/media/fat/_Computer/DoradoRTL_CedarMC1668_20261020')
    q = json.loads((new / 'qualification.json').read_text())
    c = json.loads((old / 'reboot-resume.json').read_text())
    assert q == json.loads((staging / 'qualification.json').read_text())
    assert q['status'] == 'PASS_NATIVE_MC1668_BUILD'
    assert q['state_admin'] == 'INACTIVE' and q['frequency_mhz'] == 50 and q['sysper'] == 4
    assert q['minimum_slack_ns'] > 0 and len(q['detailed_paths']) == 196
    assert not q['firmware_qualified'] and not q['hardware_speedup_claimed']
    assert sha(core / 'Dorado.rbf') == sha(staging / 'Dorado.rbf') == q['rbf_sha256']
    assert sha(Path('/media/fat/_Computer/DoradoRTL_Cedar59_20261019/Dorado.rbf')) == OLD_RBF
    assert not matches(c['main']['exe']), 'Inspect existing Main; never duplicate SPI ownership'
    assert matches(c['server']['exe'], c['server']['argv']) == [29510]
    assert sha(c['main']['exe']) == MAIN and sha('/proc/29510/exe') == SERVER
    assert '/dev/mem' not in Path('/proc/29510/maps').read_text()
    assert sha(new / 'cedar-work.pack') == SEED
    assert not (new / 'deployment.json').exists()
    assert not (new / 'recovery-started.json').exists(), 'Inspect prior recovery; never blindly rerun'
    main_log = Path('/tmp/dorado-mc1668-main.log')
    disk_log = Path('/tmp/dorado-mc1668-disk.jsonl')
    assert not main_log.exists() and not disk_log.exists()
    mgl = core / 'CedarMC1668.mgl'
    expected = Path('/media/fat/_Computer/DoradoRTL_Cedar59_20261019/Cedar59.mgl').read_text().replace(
        'Cedar59_20261019', 'CedarMC1668_20261020')
    assert mgl.read_text() == expected
    trace = Path(c['main']['env']['DORADO_DISK_TRACE'])
    records = [json.loads(line) for line in trace.open('rb')]
    gaps = [r for r in records if r['event'] == 'trace_dropped']
    assert len(records) == 116146
    assert gaps == [dict(ns=153679932108714, event='trace_dropped', offset=0,
                         bytes=80580, result=0, duration_ns=0)], gaps
    assert not [r for r in records if 'error' in r['event'] or r['result'] < 0]
    assert [r['event'] for r in records[-4:]] == [
        'barrier_begin', 'barrier_end', 'barrier_begin', 'barrier_end']
    assert records[-1]['ns'] == 155948302370802
    # Verify Main has closed the old pack, independently of its trace.
    for p in Path('/proc').iterdir():
        if not p.name.isdigit():
            continue
        try:
            for fd in (p / 'fd').iterdir():
                try:
                    target = os.readlink(fd)
                except OSError:
                    continue
                assert target != str(old / 'cedar-work.pack'), (p.name, fd, target)
        except (FileNotFoundError, PermissionError):
            continue
    proof = dict(path=str(old / 'cedar-work.pack'),
                 bytes=(old / 'cedar-work.pack').stat().st_size,
                 sha256=sha(old / 'cedar-work.pack'), disk_records=len(records),
                 disk_trace_sha256=sha(trace), disk_trace_complete=False,
                 historical_trace_gaps=gaps,
                 boundary='Normal Main loader and temporary Main exit; final two disk barriers complete; no pack descriptor open',
                 guest='No clean guest shutdown, filesystem-integrity proof or FPGA snapshot claimed')
    shutil.copyfile(trace, old / 'disk-through-mc1668-load.jsonl')
    shutil.copyfile(c['main_stdout'], old / 'main-through-mc1668-load.log')
    assert sha(old / 'disk-through-mc1668-load.jsonl') == proof['disk_trace_sha256']
    (old / 'preservation-before-mc1668.json').write_text(json.dumps(proof, indent=2) + '\n')
    (new / 'recovery-started.json').write_text(json.dumps(dict(
        pid=os.getpid(), time=time.time(), preserved_old_volume=proof,
        phase='Verified interrupted boundary; normal server stop and fresh Main start next'), indent=2) + '\n')
    print('Interrupted boundary verified; old volume/trace preserved with explicit historical gap', flush=True)
    os.kill(29510, signal.SIGTERM)
    stopped(29510)
    assert not Path('/tmp/dorado-net.sock.control').exists()
    with (new / 'server.log').open('ab') as f:
        sp = subprocess.Popen(c['server']['argv'], cwd=c['server']['cwd'],
            env=dict(os.environ, **c['server']['env']), stdin=subprocess.DEVNULL,
            stdout=f, stderr=subprocess.STDOUT, start_new_session=True)
    time.sleep(.3)
    assert sp.poll() is None
    argv = [c['main']['exe'], str(core / 'Dorado.rbf'), str(mgl)]
    c['main']['argv'] = argv
    c['main']['env']['DORADO_DISK_TRACE'] = str(disk_log)
    c['main_stdout'] = str(main_log)
    with main_log.open('ab') as f:
        mp = subprocess.Popen(argv, cwd=c['main']['cwd'],
            env=dict(os.environ, **c['main']['env']), stdin=subprocess.DEVNULL,
            stdout=f, stderr=subprocess.STDOUT, start_new_session=True)
    (new / 'reboot-resume.json').write_text(json.dumps(c, indent=2) + '\n')
    d = dict(old_main=29511, old_server=29510, main_pid=mp.pid, server_pid=sp.pid,
        main_sha256=MAIN, server_sha256=SERVER, rbf_sha256=q['rbf_sha256'], seed_sha256=SEED,
        preserved_old_volume=proof, main_log=str(main_log), disk_trace=str(disk_log),
        phase='Resumed interrupted normal load; original firmware boot pending')
    (new / 'deployment.json').write_text(json.dumps(d, indent=2) + '\n')
    for _ in range(300):
        assert mp.poll() is None and sp.poll() is None
        log = main_log.read_text()
        assert 'trace open failed' not in log and 'packet ownership held' not in log
        if 'Dorado disk: image cache ready,' in log and 'Dorado DDR: direct frame queues active,' in log:
            break
        time.sleep(.1)
    else:
        raise RuntimeError('Observe these same processes; fresh cache/DDR grant not yet confirmed')
    assert '/dev/mem' in Path('/proc/%d/maps' % sp.pid).read_text()
    assert sha('/proc/%d/exe' % mp.pid) == MAIN and sha('/proc/%d/exe' % sp.pid) == SERVER
    print(json.dumps(d, indent=2), flush=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--staging', type=Path, required=True)
    resume(parser.parse_args().staging.resolve())
