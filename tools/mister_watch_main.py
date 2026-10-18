#!/usr/bin/env python3
"""Timestamp Dorado Main heartbeats and sample blocked host I/O, without SPI/ptrace.

Run on the allocated MiSTer as root. Write observations to /tmp (RAM), so
measurement adds no SD-card writes. Stop on the first pack fault or deadline.
This does not acknowledge I/O, pause/reset Main or modify the guest.
"""
import argparse
import datetime
import hashlib
import json
import os
from pathlib import Path
import re
import time


def snapshot(pid):
    proc = Path('/proc') / str(pid)
    result = {}
    for name in ('wchan', 'syscall', 'stack', 'io'):
        try:
            result[name] = (proc / name).read_text()
        except OSError as error:
            result[name] = {'error': str(error)}
    pack_fds = {}
    try:
        for fd in (proc / 'fd').iterdir():
            try:
                target = os.readlink(fd)
                if target.endswith('.pack'):
                    pack_fds[fd.name] = {'path': target, 'fdinfo': (proc / 'fdinfo' / fd.name).read_text()}
            except OSError:
                pass
    except OSError as error:
        result['fd_error'] = str(error)
    result['pack_fds'] = pack_fds
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--pid', type=int, required=True)
    parser.add_argument('--log', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--seconds', type=float, default=3600)
    parser.add_argument('--fault-format', type=int, choices=(1, 2), required=True)
    args = parser.parse_args()
    base = Path('/media/fat/games/DoradoRTL').resolve()
    if not args.log.resolve().is_relative_to(base):
        parser.error('log must belong to the allocated Dorado test directory')
    if args.out.parent.resolve() != Path('/tmp').resolve() or not args.out.name.startswith('dorado-'):
        parser.error('write to a new /tmp/dorado-*.jsonl file in RAM')
    if not 0 < args.seconds <= 7200:
        parser.error('observation duration must be0..7200 seconds')
    proc = Path('/proc') / str(args.pid)
    exe = Path(os.readlink(proc / 'exe'))
    argv = (proc / 'cmdline').read_bytes().split(b'\0')
    if not exe.name.startswith('MiSTer') or not any(b'DoradoRTL_' in value for value in argv):
        parser.error('PID must be Main for the allocated Dorado test launcher')
    pattern = re.compile(r'Dorado net: .*machine=([0-9a-f]{8}) state=([0-9a-f]{4}) cpu=([0-9a-f]{8})')
    started = previous = time.monotonic()
    last_probe = started
    with args.out.open('x') as out, args.log.open() as source:
        def emit(kind, **fields):
            value = dict(kind=kind, utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),
                         monotonic=time.monotonic(), **fields)
            out.write(json.dumps(value) + '\n')
            out.flush()
        emit('start', pid=args.pid, main_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),
             log=str(args.log), fault_format=args.fault_format)
        source.seek(0, 2)
        pending = ''
        while time.monotonic() - started < args.seconds:
            chunk = source.readline()
            now = time.monotonic()
            if chunk:
                pending += chunk
                if not pending.endswith('\n'):
                    continue
                line, pending = pending.rstrip('\n'), ''
                match = pattern.search(line)
                if match:
                    word, status, extra = (int(value, 16) for value in match.groups())
                    emit('heartbeat', gap_seconds=now-previous, line=line)
                    previous = now
                    if status & 0x10:
                        fault = dict(word=f'{word:08x}', extra=f'{extra:08x}', reason=word>>28,
                                     cylinder=(word>>16)&0xfff, head=(word>>10)&63)
                        if word>>28 >= 5:
                            fault['lba'] = extra
                            if args.fault_format == 2:
                                fault.update(host_state=(word>>3)&7, sd_rd=(word>>2)&1,
                                             sd_wr=(word>>1)&1, sd_ack=word&1)
                        emit('pack_fault', decoded=fault, host=snapshot(args.pid))
                        print('Observed pack fault; saved', args.out, flush=True)
                        return
            else:
                if now-previous > 1.25 and now-last_probe >= .25:
                    emit('heartbeat_gap', gap_seconds=now-previous, host=snapshot(args.pid))
                    last_probe = now
                time.sleep(.01)
        emit('complete', reason='time_limit')
    print('Observation complete:', args.out, flush=True)


if __name__ == '__main__':
    main()
