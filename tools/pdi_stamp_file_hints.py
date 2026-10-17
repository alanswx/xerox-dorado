#!/usr/bin/env python3
"""Write Cedar File.FP DA hints into synthetic PDI file labels.

Cedar FilePagesImpl.HeaderLabel/DataLabel includes the entire FP (id +
logical header-page address). Synthetic volumes with zero label DA hints
fail the real controller's label check. Repair an offline COPY, preserving
all data, file-page/attribute fields and boot-chain links. Boot-only files
without a file header retain their original labels.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct


def stamp(raw):
    result = bytearray(raw)
    word = lambda off: struct.unpack_from('>H', raw, off)[0]
    long = lambda off: word(off) | (word(off+2) << 16)
    if len(raw) < 512 or raw[:8] != b'PARCDISK' or word(8) != 1 or word(10) != 2 or word(24) != 20 or word(26) != 512:
        raise ValueError('expected Pilot PDI with 10-word labels and 256-word data')
    if (len(raw)-512) % 532:
        raise ValueError('truncated PDI')
    pages = (len(raw)-512)//532
    if pages == 0:
        raise ValueError('PDI has no pages')
    data = lambda p: 512+p*532+20
    if word(data(0)) != 0o121212:
        raise ValueError('missing physical root')
    if word(data(0)+64*2) > 6:
        raise ValueError('too many subvolumes')
    files = []
    changed = []
    for i in range(word(data(0)+64*2)):
        desc = data(0)+(75+13*i)*2
        base, count = long(desc+9*2), long(desc+11*2)
        root_page = base+long(desc+7*2)
        if base+count > pages or root_page >= pages:
            raise ValueError('subvolume outside PDI')
        root = data(root_page)
        if word(root) != 0o131313:
            raise ValueError('missing logical root')
        headers = {}
        for page in range(base, base+count):
            off = 512+page*532
            label = struct.unpack_from('>10H', raw, off)
            if label[7] != 9729 or label[5:7] != (0, 0):
                continue
            key = label[:2]
            if key in headers:
                raise ValueError('duplicate first file header')
            headers[key] = page-base
        # Root-file FPs independently anchor the header addresses.
        for slot in range(16):
            fp = root+(85+6*slot)*2
            key = (word(fp), word(fp+2))
            if key in headers and long(fp+4) != headers[key]:
                raise ValueError('root FP disagrees with file header address')
        for page in range(base, base+count):
            off = 512+page*532
            label = struct.unpack_from('>10H', raw, off)
            if label[7] not in (9729, 9730) or label[:2] not in headers:
                continue
            hint = headers[label[:2]]
            old = label[2] | (label[3]<<16)
            if old not in (0, hint):
                raise ValueError('existing nonzero hint disagrees with header')
            if old != hint:
                struct.pack_into('>HH', result, off+4, hint&65535, hint>>16)
                changed.append(page)
        files.extend({'volume_base': base, 'file_id': list(key),
                      'logical_header_page': page}
                     for key, page in headers.items())
    return bytes(result), {'files': files, 'changed_labels': len(changed),
                           'changed_pages': changed,
                           'source_sha256': hashlib.sha256(raw).hexdigest(),
                           'output_sha256': hashlib.sha256(result).hexdigest()}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('source', type=Path)
    parser.add_argument('output', type=Path)
    args = parser.parse_args()
    if args.source.resolve() == args.output.resolve():
        parser.error('output must be a separate disposable copy')
    raw, report = stamp(args.source.read_bytes())
    args.output.write_bytes(raw)
    args.output.with_suffix(args.output.suffix+'.hints.json').write_text(
        json.dumps(report, indent=2)+'\n')
    print(json.dumps(report, indent=2))


if __name__ == '__main__':
    main()
