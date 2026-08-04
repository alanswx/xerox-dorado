#!/usr/bin/env python3
"""Check baked checkpoints against a build's snapshot ABI.

dorado_machine_restore refuses any snapshot whose header struct sizes
disagree with the running build, so a struct that grows silently kills
every checkpoint baked before it.  On 2026-08-04 the Leaf ethernet growth
(e5ecba1, +66,496 bytes in dorado_ethernet) was found to have left BOTH
saved-login checkpoints dead for a week -- native and wasm32 -- because no
gate ever restores a login checkpoint.  This reads the sizes straight out
of each header instead of booting anything.

The sizes are PER-ABI: native and wasm32 legitimately differ (sz_eth
86,392 vs 86,288), so each set of checkpoints must be checked against the
build that bakes it -- `./build/dorado --print-abi` for native,
`node build/dorado-node.js --print-abi` for the browser assets.

usage: check_snapshot_abi.py --abi ABIFILE PATH [PATH ...]
       (PATH may be a .snap, a .snap.gz, or a directory to scan)
"""
import argparse
import gzip
import struct
import sys
from pathlib import Path

MAGIC = b'DORADOSNAPSHOT\x01\x00'   # 15 chars + NUL, as the C macro stores it
# Header: char magic[16]; uint32 version; uint32 pad; then ten uint64s.
FIELDS = ['sz_mc', 'sz_cpu', 'sz_mem', 'sz_disp', 'sz_bb', 'sz_eth',
          'sz_disk', 'sz_fastio', 'sz_machine', 'storage_words']
HEADER_BYTES = 16 + 4 + 4 + 8 * len(FIELDS)


def read_header(path):
    """Return (version, {field: value}) or None if not a snapshot."""
    opener = gzip.open if path.suffix == '.gz' else open
    try:
        with opener(path, 'rb') as fh:
            raw = fh.read(HEADER_BYTES)
    except OSError as exc:
        return ('unreadable', str(exc))
    if len(raw) < HEADER_BYTES or raw[:16] != MAGIC:
        return None
    version = struct.unpack('<I', raw[16:20])[0]
    values = dict(zip(FIELDS, struct.unpack('<10Q', raw[24:HEADER_BYTES])))
    return (version, values)


def parse_abi(path):
    abi = {}
    for line in Path(path).read_text().splitlines():
        parts = line.split()
        if len(parts) == 2:
            abi[parts[0]] = int(parts[1])
    if 'sz_machine' not in abi:
        sys.exit(f'{path}: not a --print-abi report')
    return abi


def collect(paths):
    out = []
    for p in paths:
        p = Path(p)
        if p.is_dir():
            out.extend(sorted(q for q in p.iterdir()
                              if q.suffix in ('.snap', '.gz')))
        else:
            out.append(p)
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--abi', required=True,
                    help='output of `dorado --print-abi` for the matching build')
    ap.add_argument('paths', nargs='+')
    args = ap.parse_args()

    abi = parse_abi(args.abi)
    checked = stale = 0
    for path in collect(args.paths):
        header = read_header(path)
        if header is None:
            continue                      # a pack/PDI/other asset, not a snapshot
        version, values = header
        if version == 'unreadable':
            print(f'  {path}: UNREADABLE ({values})')
            stale += 1
            continue
        checked += 1
        bad = [(f, values[f], abi[f]) for f in FIELDS
               if f in abi and values[f] != abi[f]]
        if version != abi.get('version', version):
            bad.insert(0, ('version', version, abi['version']))
        if bad:
            stale += 1
            print(f'  {path}: STALE -- restore will refuse it')
            for field, got, want in bad:
                print(f'      {field}: checkpoint {got}, this build {want}')
        else:
            print(f'  {path}: ok')

    if not checked:
        sys.exit('no snapshots found -- wrong paths?')
    print(f'{checked} checkpoint(s) checked, {stale} stale')
    if stale:
        print('Rebake them; see docs/CONTINUE-HERE.md (2026-08-04).')
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
