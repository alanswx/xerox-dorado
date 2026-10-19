#!/usr/bin/env python3
"""Time the real base-loader transfer chain from bounded log observations.

This checks source-file transfer markers and a matching actual guest ACK of
the final file. It does not prove file byte hashes, module STARTs or a desktop.
"""
import argparse
import json
from pathlib import Path
import re


def expected_files(root):
    listing = 'Cedar6.1/Top/Basic.Loadees'
    text = (root/listing).read_bytes().decode('latin1')
    names = [name.replace('>', '/') for name in
             re.findall(r'\[Cedar\]<([^\r\n]+\.bcd)', text, re.I)]
    if len(names) != 33 or names[-1] != 'Cedar6.1/Viewers/End.bcd':
        raise ValueError('unexpected Basic.Loadees workload')
    return {name: (root/name).stat().st_size for name in [listing, *names]}


def observed_lines(records):
    pending = ''
    lower = 0
    for row in records:
        if row.get('event') != 'chunk' or row.get('source') != 'wire':
            continue
        if not pending:
            lower = row['previous_read_end_ns']
        pending += row['text']
        while '\n' in pending:
            text, pending = pending.split('\n', 1)
            yield dict(text=text, earliest_ns=lower, latest_ns=row['read_end_ns'])
            lower = row['previous_read_end_ns']


def analyse(lines, sizes):
    seen = {}
    last_queue = last_ack = None
    final_peer = None
    ranges = {}
    first_data = probe = closing_end = None
    final_active = False
    start = None
    final_path = 'Cedar6.1/Viewers/End.bcd'
    listing = 'Cedar6.1/Top/Basic.Loadees'
    for row in lines:
        text = row['text']
        m = re.match(r'STP_TRANSFER (\S+) \((\d+) bytes\)', text)
        if m and m[1] in sizes:
            path, size = m[1], int(m[2])
            if size != sizes[path]:
                raise ValueError('source size differs: '+path)
            seen.setdefault(path, dict(**row, bytes=size))
            if path == listing and start is None:
                start = row
            if path == final_path:
                final_active = True

        m = re.match(r'TX n=(\d+): ([0-7 ]+)$', text)
        if m:
            words = [int(x, 8) for x in m[2].split()]
            last_ack = None
            if len(words) == int(m[1]) and len(words) >= 13 and words[3] == 0o22:
                last_ack = dict(row=row, identifier=(words[4] << 16) | words[5],
                                peer=tuple(words[9:12]))
        if not final_active:
            continue
        m = re.match(r'FTP_QUEUE type=0o([0-7]+) id=([0-9a-f]+) bytes=(\d+) d=([0-7]+)/([0-7]+)/([0-7]+)', text)
        if m:
            last_queue = dict(kind=int(m[1], 8), identifier=int(m[2], 16),
                              size=int(m[3]), peer=tuple(int(x, 8) for x in m.groups()[3:]))
            if final_peer is None:
                final_peer = last_queue['peer']
            if (probe is None and closing_end is not None and last_queue['peer'] == final_peer
                    and last_queue['kind'] == 0o21 and last_queue['identifier'] == closing_end):
                probe = last_queue['identifier']
        m = re.match(r'FTP_WINDOW data=(\d+) .* mode=3 ', text)
        if m and last_queue and last_queue['kind'] == 0o20 and last_queue['peer'] == final_peer:
            if int(m[1]) != last_queue['size']:
                raise ValueError('file-data window does not match queued packet')
            if first_data is None:
                first_data = last_queue['identifier']
            offset = (last_queue['identifier']-first_data) & 0xffffffff
            ranges[offset] = last_queue['size']
        if (closing_end is None and first_data is not None and last_queue
                and re.match(r'FTP_WINDOW mark=0o6 .* mode=0 ', text)
                and last_queue['peer'] == final_peer and last_queue['kind'] == 0o23):
            closing_end = (last_queue['identifier']+last_queue['size']) & 0xffffffff
        m = re.match(r'FTP_ACK ack=([0-9a-f]+)', text)
        if not m or probe is None or int(m[1], 16) != probe or not last_ack:
            continue
        if last_ack['identifier'] != probe or last_ack['peer'] != final_peer:
            continue
        cursor = 0
        for offset, count in sorted(ranges.items()):
            if offset != cursor:
                break
            cursor += count
        if cursor != sizes[final_path] or ((probe-first_data) & 0xffffffff) <= cursor:
            continue
        if list(seen) != list(sizes) or start is None:
            raise ValueError('base-loader transfer sequence missing or reordered')
        end = dict(earliest_ns=last_ack['row']['earliest_ns'], latest_ns=row['latest_ns'])
        lower = (end['earliest_ns']-start['latest_ns'])/1e9
        upper = (end['latest_ns']-start['earliest_ns'])/1e9
        if lower <= 0 or upper < lower:
            raise ValueError('invalid observation timing bounds')
        return dict(status='PASS: observed base-loader transfer chain and final guest ACK',
                    files=len(seen), bcd_files=len(seen)-1, payload_bytes=sum(sizes.values()),
                    start=start, end=end, elapsed_lower_s=lower, elapsed_upper_s=upper,
                    final_peer=list(final_peer), final_probe=f'{probe:08x}',
                    final_data_ranges=ranges, transfers=seen,
                    limits='Unbuffered protocol-log visibility bounds; not direct wire timestamps, file-byte hashes, module START or desktop qualification. Requires independent repeated matching workloads for a speed claim.')
    raise ValueError('no complete source transfer sequence with exact final guest ACK')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('observer', type=Path)
    p.add_argument('--stp-root', type=Path, required=True)
    p.add_argument('--output', type=Path)
    args = p.parse_args()
    rows = [json.loads(line) for line in args.observer.read_text().splitlines()]
    result = analyse(observed_lines(rows), expected_files(args.stp_root))
    text = json.dumps(result, indent=2)+'\n'
    if args.output:
        args.output.write_text(text)
    print(text, end='')
