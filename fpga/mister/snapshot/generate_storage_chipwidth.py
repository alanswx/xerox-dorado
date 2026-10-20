#!/usr/bin/env python3
"""Experimental compact cache holding exactly the original chip-vector width.

Raw words are16bits, but only WIDTH low bits feed the original guest pipeline.
Trim unused implementation padding from cache_data; preserve raw16bit payloads,
all original guest fields and every actual remaining snapshot bit. New schema.
"""
import argparse
import hashlib
import json
from pathlib import Path
import generate_storage_compact as base

original=base.original
FIELDS=[(n,'WIDTH'if n=='cache_data'else w)for n,w in base.FIELDS]


def fields(width,addr_bits=16):
    if width not in (8,12)or addr_bits!=16:raise ValueError('unqualified chip-width cache geometry')
    return [(n,width if w=='WIDTH'else w)for n,w in FIELDS]


def generate():
    source,old=base.generate();reference=source
    edits=[('module dorado_storage_group_compact #(','module dorado_storage_group_chipwidth #('),
           ('reg [15:0] cache_data=0;','reg [WIDTH-1:0] cache_data=0;'),
           ('STATE_BITS=22+3*WIDTH','STATE_BITS=6+4*WIDTH'),
           ('cache_data<=req_data;','cache_data<=req_data[WIDTH-1:0];'),
           ('cache_data<=rsp_data;','cache_data<=rsp_data[WIDTH-1:0];')]
    for before,after in edits:
        assert before in source;source=source.replace(before,after)
    inverse=source
    for before,after in reversed(edits):inverse=inverse.replace(after,before)
    assert inverse==reference
    manifest=dict(old);manifest.update(scope=__doc__,fields=FIELDS,physical_bits=dict(width8=38,width12=54),
        generated_sha256=hashlib.sha256(source.encode()).hexdigest(),parent_generated_sha256=hashlib.sha256(reference.encode()).hexdigest(),
        actual_removed_padding_bits=dict(width8=8,width12=4),
        state_relation='cache_valid=reference.hit; cache_data=reference.read_value[WIDTH-1:0]; other eight fields equal; new snapshot schema mandatory',
        limits='Prototype only; not integrated in a complete MSA, catalog or native fit')
    return source,manifest


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    a.output.mkdir(parents=True,exist_ok=False);source,manifest=generate()
    (a.output/'storage_group_chipwidth.sv').write_text(source);(a.output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
