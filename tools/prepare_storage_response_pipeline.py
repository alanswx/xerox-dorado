#!/usr/bin/env python3
"""Prepare an opt-in raw-memory response/request overlap experiment.

Use exact cache16 native sources. No CPU, clock, architectural cache, Map,
ECC, MSA group timing, Main, or deployed core is modified. Both new parameters
default off. A response may retire and its successor be accepted on the same
host edge; at most one response remains outstanding after that edge.
"""
import argparse
import hashlib
import json
from pathlib import Path

EXPECTED = {
    'dorado_storage_arb.sv': '750e623b4a2bf59fee9718fa98d1ac77ddde55e19b4e90df22d3e047a6fdd743',
    'dorado_storage_sdram_burst.sv': 'd1b443868a876e66a19f156d7114f518575db0f104a3e6d0dddfb230e6831575',
}


def replace_once(text, old, new):
    assert text.count(old) == 1, old
    return text.replace(old, new)


def transform_arb(text):
    text = replace_once(text, 'parameter integer INTERLEAVE_GROUPS=0',
                        'parameter integer INTERLEAVE_GROUPS=0,\n'
                        '    parameter integer RESPONSE_OVERLAP=0')
    anchor = '    always @* begin\n        req_ready=0;rsp_valid=0;'
    text = replace_once(text, anchor, '''    // The current response still belongs to owner. Only a simultaneously
    // accepted successor uses chosen_owner and its live, unacknowledged payload.
    // No extra queue or architectural completion is introduced.
    wire response_consumed=state==RESPONSE && raw_rsp_valid && rsp_ready[owner];
    wire overlap_request=RESPONSE_OVERLAP && PREFETCH_NEXT &&
        response_consumed && present[1];
    wire [ID_BITS-1:0] request_owner=overlap_request ? chosen_owner : owner;
    wire [GROUP_ADDR_BITS-1:0] request_address=
        overlap_request ? chosen_address : address_latched;
    always @* begin
        req_ready=0;rsp_valid=0;''')
    text = replace_once(text,
        'if(!reset && state==REQUEST)req_ready[owner]=raw_req_ready;',
        'if(!reset && (state==REQUEST || overlap_request))\n'
        '            req_ready[request_owner]=raw_req_ready;')
    text = replace_once(text, 'assign raw_req_valid=!reset && state==REQUEST;',
                        'assign raw_req_valid=!reset && (state==REQUEST || overlap_request);')
    text = replace_once(text, 'assign raw_req_write=write_latched;',
                        'assign raw_req_write=overlap_request ? chosen_write : write_latched;')
    text = replace_once(text,
        "(RAW_ADDR_BITS'(address_latched)<<GROUP_BITS) | RAW_ADDR_BITS'(owner) :\n"
        "        (RAW_ADDR_BITS'(owner)<<GROUP_ADDR_BITS) | address_latched;",
        "(RAW_ADDR_BITS'(request_address)<<GROUP_BITS) | RAW_ADDR_BITS'(request_owner) :\n"
        "        (RAW_ADDR_BITS'(request_owner)<<GROUP_ADDR_BITS) | request_address;")
    text = replace_once(text, 'assign raw_req_data=data_latched;',
                        'assign raw_req_data=overlap_request ? chosen_data : data_latched;')
    text = replace_once(text,
        'data_latched<=chosen_data;write_latched<=chosen_write;\n'
        '                state<=REQUEST;',
        '''data_latched<=chosen_data;write_latched<=chosen_write;
                // If the backend accepted the successor on this SAME edge,
                // its response will belong to the newly latched owner.
                if(overlap_request && raw_req_ready)begin
                    state<=RESPONSE;
                    next_group<=chosen_owner==GROUPS-1 ? 0 : chosen_owner+1'b1;
                end else state<=REQUEST;''')
    text = replace_once(text,
        '(INTERLEAVE_GROUPS!=0 && INTERLEAVE_GROUPS!=1))',
        '(INTERLEAVE_GROUPS!=0 && INTERLEAVE_GROUPS!=1) ||\n'
        '               (RESPONSE_OVERLAP!=0 && RESPONSE_OVERLAP!=1) ||\n'
        '               (RESPONSE_OVERLAP && !PREFETCH_NEXT))')
    return text


def transform_backend(text):
    text = replace_once(text, 'parameter integer CACHE_LINES=4',
                        'parameter integer CACHE_LINES=4,\n'
                        '    parameter integer RESPONSE_OVERLAP=0')
    text = replace_once(text,
        'assign req_ready=initialized && state==IDLE && !response_pending &&',
        '''// The old response remains stable until its normal handshake.
    // A new request may use that edge only when the old response is consumed.
    // The existing clear-before-case priority retains a new hit response.
    assign req_ready=initialized && state==IDLE &&
        (!response_pending || (RESPONSE_OVERLAP && rsp_ready)) &&''')
    text = replace_once(text,
        'CACHE_LINES<1 || CACHE_LINES>16 || (CACHE_LINES & (CACHE_LINES-1))!=0)',
        'CACHE_LINES<1 || CACHE_LINES>16 || (CACHE_LINES & (CACHE_LINES-1))!=0 ||\n'
        '           (RESPONSE_OVERLAP!=0 && RESPONSE_OVERLAP!=1))')
    return text


def prepare(source, out):
    assert not out.exists(), 'Inspect the existing experiment; do not overwrite it'
    originals = {}
    for name, expected in EXPECTED.items():
        raw = (source / name).read_bytes()
        assert hashlib.sha256(raw).hexdigest() == expected, name
        originals[name] = raw
    generated = {
        'dorado_storage_arb.sv': transform_arb(originals['dorado_storage_arb.sv'].decode()),
        'dorado_storage_sdram_burst.sv': transform_backend(
            originals['dorado_storage_sdram_burst.sv'].decode()),
    }
    out.mkdir(parents=True)
    for name, text in generated.items():
        (out / name).write_text(text)
    record = dict(scope=__doc__, status='PREPARED_COMPONENT_UNQUALIFIED',
                  parent_hashes=EXPECTED,
                  generated_hashes={n: hashlib.sha256(t.encode()).hexdigest()
                                    for n, t in generated.items()},
                  defaults_off=True, hardware_changed=False,
                  native_fit_or_firmware_qualified=False)
    (out / 'provenance.json').write_text(json.dumps(record, indent=2) + '\n')
    return record


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    print(json.dumps(prepare(args.source.resolve(), args.output.resolve()), indent=2))
