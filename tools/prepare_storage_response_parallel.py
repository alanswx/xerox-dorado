#!/usr/bin/env python3
"""Shorten the measured arbiter-to-buffer read-data path without another cycle.

The rejected overlap fit serializes tag comparison, binary line selection and
word selection. Select the requested word within each line in parallel with
the tags, then retain the original last-matching-line priority. Response timing,
read-before-write, fill, invalidation and refresh remain unchanged.
"""
import argparse
import hashlib
import json
from pathlib import Path

from prepare_storage_response_pipeline import EXPECTED, replace_once, transform_arb, transform_backend


def transform_parallel(text):
    text = transform_backend(text)
    text = replace_once(text, 'reg [INDEX_BITS-1:0] lookup_line;',
                        'reg [15:0] lookup_data;')
    text = replace_once(text, 'lookup_hit=0;lookup_line=0;',
                        'lookup_hit=0;lookup_data=0;')
    text = replace_once(text, "lookup_hit=1;lookup_line=INDEX_BITS'(i);",
                        'lookup_hit=1;\n'
                        '                // Constant line index: select words alongside the tag\n'
                        '                // comparisons, then preserve last-match priority.\n'
                        '                lookup_data=line_words[i][16*req_addr[2:0]+:16];')
    return replace_once(text, 'rsp_data<=line_words[lookup_line][16*req_addr[2:0]+:16];',
                        'rsp_data<=lookup_data;')


def prepare(source, out):
    assert not out.exists(), 'Inspect the existing experiment before another run'
    originals = {name: (source / name).read_bytes() for name in EXPECTED}
    assert {n: hashlib.sha256(b).hexdigest() for n, b in originals.items()} == EXPECTED
    generated = {
        'dorado_storage_arb.sv': transform_arb(originals['dorado_storage_arb.sv'].decode()),
        'dorado_storage_sdram_burst.sv': transform_parallel(originals['dorado_storage_sdram_burst.sv'].decode()),
    }
    out.mkdir(parents=True)
    for name, text in generated.items():
        (out / name).write_text(text)
    record = dict(scope=__doc__, status='PREPARED_PARALLEL_LOOKUP_COMPONENT_UNQUALIFIED',
                  parent_hashes=EXPECTED,
                  generated_hashes={n: hashlib.sha256(t.encode()).hexdigest() for n, t in generated.items()},
                  defaults_off=True, hardware_changed=False,
                  native_fit_or_firmware_qualified=False, parallel_lookup=True)
    (out / 'provenance.json').write_text(json.dumps(record, indent=2) + '\n')
    return record


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    print(json.dumps(prepare(args.source.resolve(), args.output.resolve()), indent=2))
