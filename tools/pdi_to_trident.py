#!/usr/bin/env python3
"""Convert a Pilot PDI to raw sectors for the RTL serial Trident medium.

PilotDiskDefs.mc!1 specifies drive 0's virtual geometry and PilotDisk.mc!1
specifies its sector stagger. Preserve labels/data byte-exact; manufacture
only the omitted disk-address headers and container dummy word. Boot links
must already be real DiskAddress values (pdi_boot_links_to_chs.py).
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import struct

CYLINDERS, HEADS, SECTORS, TRACK_SLOTS = 815, 5, 28, 29
SECTOR_WORDS = 269  # dummy + two header + ten label + 256 data


def physical_location(vda):
    virtual_cylinder, sector = divmod(vda, SECTORS)
    head, cylinder = divmod(virtual_cylinder, CYLINDERS)
    if not 0 <= head < HEADS:
        raise ValueError('Pilot page is outside a system80 drive')
    staggered = (sector + 8*(cylinder % 4)) % SECTORS
    return cylinder, head, staggered


def convert(raw):
    if raw[:8] != b'PARCDISK' or len(raw) < 512:
        raise ValueError('not a PDI container')
    word = lambda index: struct.unpack_from('>H', raw, index*2)[0]
    pages = word(8)*(word(9) or 1)*(word(10) or 1)
    if word(4)!=1 or word(5)!=2 or word(12)!=20 or word(13)!=512:
        raise ValueError('expected version-1 Pilot PDI with 10-word label and 256-word data')
    if len(raw)!=512+pages*532 or not 1<=pages<=CYLINDERS*HEADS*SECTORS:
        raise ValueError('invalid PDI length or page count')
    packed = bytearray(CYLINDERS*HEADS*TRACK_SLOTS*SECTOR_WORDS*2)
    for vda in range(CYLINDERS*HEADS*SECTORS):
        cylinder, head, sector = physical_location(vda)
        target = ((cylinder*HEADS+head)*TRACK_SLOTS+sector)*SECTOR_WORDS*2
        # Header check compares the ORIGINAL virtual DiskAddress, before
        # PilotDisk converts it to real cylinder/head/staggered sector.
        vc, vs = divmod(vda, SECTORS)
        struct.pack_into('<HHH', packed, target, 0, vc, vs)
        if vda<pages:
            payload = bytearray(raw[512+vda*532:512+(vda+1)*532])
            payload[0::2], payload[1::2] = payload[1::2], payload[0::2]
            packed[target+6:target+538] = payload
    packed.extend(b'\0' * (-len(packed)%512))
    return packed, pages


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('pdi', type=Path)
    p.add_argument('pack', type=Path)
    a = p.parse_args()
    raw = gzip.decompress(a.pdi.read_bytes()) if a.pdi.suffix=='.gz' else a.pdi.read_bytes()
    packed, pages = convert(raw)
    a.pack.parent.mkdir(parents=True, exist_ok=True)
    a.pack.write_bytes(packed)
    a.pack.with_suffix(a.pack.suffix+'.json').write_text(json.dumps({
        'source': str(a.pdi), 'source_pdi_sha256': hashlib.sha256(raw).hexdigest(),
        'pack_sha256': hashlib.sha256(packed).hexdigest(), 'pdi_pages': pages,
        'physical_cylinders': CYLINDERS, 'physical_heads': HEADS,
        'sectors_per_track': TRACK_SLOTS, 'usable_sectors_per_track': SECTORS, 'header_words': 2, 'label_words': 10,
        'data_words': 256, 'words_per_stored_sector': SECTOR_WORDS,
        'drive': 0, 'stagger': '(virtualSector + 8*(realCylinder mod 4)) mod 28',
        'virtual_mapping': 'realCylinder=virtualCylinder mod 815; realHead=virtualCylinder/815',
        'limitations': 'Raw medium conversion only; real controller/germ boot not yet qualified',
    }, indent=2)+'\n')
    print(f'{pages} Pilot pages -> {len(packed)} raw/padded bytes: {a.pack}')


if __name__=='__main__':
    main()
