#!/usr/bin/env python3
"""Normalize physical AND logical Pilot boot links using their actual labels.

Write a separate offline copy. Native packs use pdi_to_trident's virtual/stagger
mapping; no RAM, boot payload, labels or controller behavior is synthesized.
Unlike a hi==0 heuristic, a valid CHS link at sector zero remains unchanged.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct

from pdi_install_lv_bootfiles import pilot_checksum
from pdi_to_trident import physical_location, CYLINDERS, HEADS, SECTORS, TRACK_SLOTS, SECTOR_WORDS


def repair(raw, kind):
    data = bytearray(raw)
    if kind == 'pdi':
        if raw[:8] != b'PARCDISK' or len(raw) < 512:
            raise ValueError('not a Pilot PDI')
        h = struct.unpack_from('>256H', raw)
        pages = h[8] * (h[9] or 1) * (h[10] or 1)
        if (h[4], h[5], h[12], h[13]) != (1, 2, 20, 512) or len(raw) != 512 + pages * 532:
            raise ValueError('expected version1 Pilot PDI with ten-word labels')
        endian = '>'
        def location(page): return 512 + page * 532, 20
    elif kind == 'native-pack':
        size = CYLINDERS * HEADS * TRACK_SLOTS * SECTOR_WORDS * 2
        if len(raw) != (size + 511) // 512 * 512:
            raise ValueError('wrong system80 native pack size')
        pages = CYLINDERS * HEADS * SECTORS
        endian = '<'
        def location(page):
            c, h, s = physical_location(page)
            base = ((c * HEADS + h) * TRACK_SLOTS + s) * SECTOR_WORDS * 2
            if struct.unpack_from('<2H', raw, base + 2) != divmod(page, SECTORS):
                raise ValueError('native virtual-address header mismatch at page %d' % page)
            return base + 6, 20
    else:
        raise ValueError('unknown format')
    if not 1 <= pages <= CYLINDERS * HEADS * SECTORS:
        raise ValueError('unsupported Pilot page count')

    def page_at(page):
        if not 0 <= page < pages: raise ValueError('page outside medium')
        off, lb = location(page)
        return list(struct.unpack_from(endian + '10H', raw, off)), list(struct.unpack_from(endian + '256H', raw, off + lb)), off + lb

    pv_label, pv, pv_off = page_at(0)
    if pv[0] != 0o121212: raise ValueError('missing physical root')
    count = pv[64]
    if not 0 < count <= 6: raise ValueError('invalid subvolume count')
    roots = [('physical', 0, pv, pv_off, 8, 4)]
    seen = set()
    for i in range(count):
        d = pv[75 + 13*i:75 + 13*(i+1)]
        base, length = d[9] | d[10] << 16, d[11] | d[12] << 16
        page = base + d[7] + (d[8] << 16)
        if page in seen or base + length > pages or not base <= page < base + length:
            raise ValueError('invalid/duplicate logical root')
        seen.add(page)
        _, words, off = page_at(page)
        if words[0] != 0o131313: raise ValueError('missing logical root seal')
        roots.append(('logical', page, words, off, 31, 6))

    changes = []
    inspected = []
    checksums = []
    allowed = set()
    for root_kind, root_page, words, off, first, slots in roots:
        # Cedar6.1 RootTransfer/WriteRoot do not maintain this field during
        # normal writes. Report stale values; labels/seals select valid roots.
        # Keep an unchanged root byte-exact, including its stored checksum.
        checksums.append(dict(root=root_kind, page=root_page, stored=words[255],
                              computed=pilot_checksum(words[:255])))
        modified = False
        for slot in range(slots):
            index = first + 9*slot
            entry = words[index:index+9]
            if not any(entry): continue
            lo, hi = entry[7:9]
            candidates = {}
            for name, page in [('flat', lo | hi << 16), ('chs', lo*SECTORS + hi)]:
                if name == 'chs' and hi >= SECTORS: continue
                if not 0 <= page < pages: continue
                label, _, _ = page_at(page)
                if label[:5] == entry[:5] and label[5:7] == entry[5:7]:
                    candidates[name] = page
            targets = set(candidates.values())
            if len(targets) != 1:
                raise ValueError('boot link has no unique label-matched target at %s page%d slot%d: %s' % (root_kind, root_page, slot, candidates))
            target = targets.pop()
            new = list(divmod(target, SECTORS))
            item = dict(root=root_kind, root_page=root_page, slot=slot, file_id=entry[:5], first_page=entry[5:7], before=[lo,hi], target_page=target, matching=candidates, after=new)
            inspected.append(item)
            if [lo,hi] == new: continue
            words[index+7:index+9] = new
            for j in [index+7, index+8]:
                struct.pack_into(endian+'H', data, off+2*j, words[j])
                allowed.update([off+2*j,off+2*j+1])
            modified = True
            changes.append(item)
        if modified:
            struct.pack_into(endian+'H', data, off+510, pilot_checksum(words[:255]))
            allowed.update([off+510,off+511])
    differences = [i for i,(a,b) in enumerate(zip(raw,data)) if a != b]
    if not set(differences) <= allowed: raise AssertionError('unadvertised byte change')
    result = bytes(data)
    return result, dict(format=kind, pages=pages, inspected=inspected, changes=changes,
        changed_bytes=differences, source_sha256=hashlib.sha256(raw).hexdigest(), output_sha256=hashlib.sha256(result).hexdigest(),
        root_checksums_before=checksums, labels_headers_preserved=True,
        nonroot_payloads_preserved=True, unchanged_roots_preserved=True, root_fp_entries_preserved=True)


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('source',type=Path);p.add_argument('output',type=Path)
    p.add_argument('--format',choices=['pdi','native-pack'],default='pdi')
    a=p.parse_args();report=a.output.with_suffix(a.output.suffix+'.boot-links.json')
    if a.source.resolve()==a.output.resolve() or a.output.exists() or report.exists():
        p.error('use a new separate offline output and report')
    result,r=repair(a.source.read_bytes(),a.format)
    with a.output.open('xb') as f:f.write(result)
    with report.open('x') as f:json.dump(r,f,indent=2);f.write('\n')
    print(json.dumps(r,indent=2))


if __name__=='__main__':main()
