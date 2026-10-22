#!/usr/bin/env python3
"""Prepare registered request lookahead without changing round-robin priority.

The qualified response-overlap arbiter selects a live128-way payload and then
looks up backing-buffer tags/words in one host cycle. Cache the next selection
at the preceding edge. Use it only while the complete pending mask still
matches; otherwise retain the original response-boundary selection and normal
registered REQUEST cycle. There is still at most one accepted raw operation.
No CPU, chip timing, architectural memory, clock or deployed source changes.
"""
import argparse
import hashlib
import json
from pathlib import Path

from prepare_storage_response_pipeline import EXPECTED, replace_once, transform_arb
from prepare_storage_response_parallel import transform_parallel

BASELINE_ARB = '65df46089a2f18bb73abc056a522a4b460e8c2bf93990dcefe5304fd293105ea'
BASELINE_BACKEND = '61f99c6a4013db1d0289882ec77157d2cb7388ffc15ab7c84202c0d5d62c4d38'


def transform(text):
    text = replace_once(text, 'parameter integer RESPONSE_OVERLAP=0',
                        'parameter integer RESPONSE_OVERLAP=0,\n'
                        '    parameter integer REQUEST_LOOKAHEAD=0')
    anchor = '    wire [2*LEAVES-1:1] present,preferred;'
    text = replace_once(text, anchor, '''    reg look_valid=0;
    reg [GROUPS-1:0] look_mask=0;
    reg [PAYLOAD-1:0] look_payload=0;
    wire [GROUPS-1:0] selection_mask;
    wire [ID_BITS-1:0] selection_origin;
    wire [2*LEAVES-1:1] present,preferred;''')
    text = replace_once(text, 'assign present[LEAVES+leaf]=req_valid[leaf];',
                        'assign present[LEAVES+leaf]=selection_mask[leaf];')
    text = replace_once(text, 'assign preferred[LEAVES+leaf]=req_valid[leaf];',
                        'assign preferred[LEAVES+leaf]=selection_mask[leaf];')
    text = replace_once(text,
                        "assign preferred[LEAVES+leaf]=req_valid[leaf] && ID_BITS'(leaf)>=next_group;",
                        "assign preferred[LEAVES+leaf]=selection_mask[leaf] && ID_BITS'(leaf)>=selection_origin;")
    start = text.index('    // The current response still belongs to owner.')
    end = text.index('    always @* begin\n        req_ready=0;rsp_valid=0;', start)
    text = text[:start] + '''    // A cached candidate is NOT accepted or reserved at its preselection
    // edge. A changed mask (including a late higher-priority arrival) must
    // use the original live selection, through REQUEST on the next cycle.
    wire cached_choice=REQUEST_LOOKAHEAD && look_valid && look_mask==req_valid;
    wire [PAYLOAD-1:0] successor=cached_choice ? look_payload : payload[1];
    wire [ID_BITS-1:0] successor_owner;
    wire [GROUP_ADDR_BITS-1:0] successor_address;
    wire [15:0] successor_data;
    wire successor_write;
    assign {successor_owner,successor_address,successor_data,successor_write}=successor;
    wire response_consumed=state==RESPONSE && raw_rsp_valid && rsp_ready[owner];
    wire overlap_request=RESPONSE_OVERLAP && PREFETCH_NEXT && response_consumed &&
        (REQUEST_LOOKAHEAD ? cached_choice : present[1]);
    wire [ID_BITS-1:0] request_owner=overlap_request ? successor_owner : owner;
    wire [GROUP_ADDR_BITS-1:0] request_address=
        overlap_request ? successor_address : address_latched;
    wire raw_accepted=raw_req_valid && raw_req_ready;
    wire select_after_accept=REQUEST_LOOKAHEAD && raw_accepted;
    wire [GROUPS-1:0] exclude_accepted=select_after_accept ?
        ({{(GROUPS-1){1'b0}},1'b1} << request_owner) : {GROUPS{1'b0}};
    assign selection_mask=req_valid & ~exclude_accepted;
    assign selection_origin=select_after_accept ?
        (request_owner==GROUPS-1 ? ID_BITS'(0) : request_owner+ID_BITS'(1)) : next_group;
    // Exclude the operation accepted on this edge; its producer drops valid
    // after the edge. While a response waits, refresh this uncommitted choice.
    always @(posedge clk)begin
        if(reset)look_valid<=0;
        else if(REQUEST_LOOKAHEAD)begin
            if(raw_accepted || (state==RESPONSE && !response_consumed))begin
                look_valid<=present[1];look_mask<=selection_mask;
                look_payload<=payload[1];
            end else if(response_consumed)look_valid<=0;
        end
    end
''' + text[end:]
    text = replace_once(text, 'assign raw_req_write=overlap_request ? chosen_write : write_latched;',
                        'assign raw_req_write=overlap_request ? successor_write : write_latched;')
    text = replace_once(text, 'assign raw_req_data=overlap_request ? chosen_data : data_latched;',
                        'assign raw_req_data=overlap_request ? successor_data : data_latched;')
    text = replace_once(text, 'if(PREFETCH_NEXT && present[1])begin',
                        'if(PREFETCH_NEXT && (cached_choice || present[1]))begin')
    text = replace_once(text,
                        'owner<=chosen_owner;address_latched<=chosen_address;\n'
                        '                data_latched<=chosen_data;write_latched<=chosen_write;',
                        'owner<=successor_owner;address_latched<=successor_address;\n'
                        '                data_latched<=successor_data;write_latched<=successor_write;')
    text = replace_once(text,
                        "next_group<=chosen_owner==GROUPS-1 ? 0 : chosen_owner+1'b1;",
                        "next_group<=successor_owner==GROUPS-1 ? 0 : successor_owner+1'b1;")
    text = replace_once(text,
                        '(RESPONSE_OVERLAP && !PREFETCH_NEXT))',
                        '(RESPONSE_OVERLAP && !PREFETCH_NEXT) ||\n'
                        '               (REQUEST_LOOKAHEAD!=0 && REQUEST_LOOKAHEAD!=1) ||\n'
                        '               (REQUEST_LOOKAHEAD && !RESPONSE_OVERLAP))')
    anchor = '    // synthesis translate_on'
    return replace_once(text, anchor, '''    // Producers must retain an unaccepted payload, as for the original
    // registered REQUEST state. The native paused-machine boundary supplies
    // this contract. Verify cached values at every actual overlapping use.
    always @(posedge clk)if(!reset && REQUEST_LOOKAHEAD && overlap_request)begin
        if(!req_valid[successor_owner] ||
           req_addr[successor_owner*GROUP_ADDR_BITS+:GROUP_ADDR_BITS]!==successor_address ||
           req_data[successor_owner*16+:16]!==successor_data ||
           req_write[successor_owner]!==successor_write)
            $fatal(1,"LOOKAHEAD cached payload differs from live request");
    end
    // synthesis translate_on''')


def split_selection(text):
    """Keep live fallback and post-accept trees independent of ready/accept.

    The first experiment fed raw acceptance back into its own selection tree.
    Although its guarded cases settled in simulation, Quartus retained a real
    combinational loop. Overlapping raw requests now use ONLY registered
    look_payload. A second tree predicts the following unaccepted request from
    registered owner/state and live producer payloads. Late acceptance chooses
    the already computed tree result at the register, never at its input.
    """
    anchor = '    // A cached candidate is NOT accepted or reserved at its preselection'
    text = replace_once(text, anchor, '''    wire [ID_BITS-1:0] look_owner=look_payload[PAYLOAD-1-:ID_BITS];
    // A raw operation accepted in RESPONSE is necessarily the registered
    // lookahead owner. REQUEST acceptance belongs to the registered owner.
    // Neither this choice nor this tree depends on raw_ready/cached_choice.
    wire [ID_BITS-1:0] post_owner=state==RESPONSE ? look_owner : owner;
    wire [ID_BITS-1:0] post_origin=post_owner==GROUPS-1 ? ID_BITS'(0) : post_owner+ID_BITS'(1);
    wire [GROUPS-1:0] post_mask=req_valid &
        ~({{(GROUPS-1){1'b0}},1'b1} << post_owner);
    wire [2*LEAVES-1:1] post_present,post_preferred;
    wire [PAYLOAD-1:0] post_payload [1:2*LEAVES-1];
    genvar post_leaf,post_node;
    generate for(post_leaf=0;post_leaf<LEAVES;post_leaf=post_leaf+1)begin:post_leaf_choice
        if(post_leaf<GROUPS)begin:populated
            assign post_present[LEAVES+post_leaf]=post_mask[post_leaf];
            if(post_leaf==LEAVES-1)begin:last_index
                assign post_preferred[LEAVES+post_leaf]=post_mask[post_leaf];
            end else begin:compare_index
                assign post_preferred[LEAVES+post_leaf]=post_mask[post_leaf] && ID_BITS'(post_leaf)>=post_origin;
            end
            assign post_payload[LEAVES+post_leaf]=payload[LEAVES+post_leaf];
        end else begin:padding
            assign post_present[LEAVES+post_leaf]=1'b0;
            assign post_preferred[LEAVES+post_leaf]=1'b0;
            assign post_payload[LEAVES+post_leaf]='0;
        end
    end
    for(post_node=1;post_node<LEAVES;post_node=post_node+1)begin:post_node_choice
        wire choose_left=post_preferred[2*post_node] ||
            (!post_preferred[2*post_node+1] && post_present[2*post_node]);
        assign post_present[post_node]=post_present[2*post_node] || post_present[2*post_node+1];
        assign post_preferred[post_node]=post_preferred[2*post_node] || post_preferred[2*post_node+1];
        assign post_payload[post_node]=choose_left ? post_payload[2*post_node] : post_payload[2*post_node+1];
    end endgenerate
    wire [PAYLOAD-1:0] overlap_payload=REQUEST_LOOKAHEAD ? look_payload : payload[1];
    wire [ID_BITS-1:0] overlap_owner;
    wire [GROUP_ADDR_BITS-1:0] overlap_address;
    wire [15:0] overlap_data;
    wire overlap_write;
    assign {overlap_owner,overlap_address,overlap_data,overlap_write}=overlap_payload;
''' + anchor)
    text = replace_once(text,
                        'wire [ID_BITS-1:0] request_owner=overlap_request ? successor_owner : owner;',
                        'wire [ID_BITS-1:0] request_owner=overlap_request ? overlap_owner : owner;')
    text = replace_once(text, 'overlap_request ? successor_address : address_latched;',
                        'overlap_request ? overlap_address : address_latched;')
    text = replace_once(text, 'assign raw_req_write=overlap_request ? successor_write : write_latched;',
                        'assign raw_req_write=overlap_request ? overlap_write : write_latched;')
    text = replace_once(text, 'assign raw_req_data=overlap_request ? successor_data : data_latched;',
                        'assign raw_req_data=overlap_request ? overlap_data : data_latched;')
    start = text.index('    wire select_after_accept=REQUEST_LOOKAHEAD && raw_accepted;')
    end = text.index('    always @* begin\n        req_ready=0;rsp_valid=0;', start)
    text = text[:start] + '''    assign selection_mask=req_valid;
    assign selection_origin=next_group;
    // Acceptance is a late choice between independent, already computed
    // candidates. It never feeds a priority tree or an outgoing raw payload.
    always @(posedge clk)begin
        if(reset)look_valid<=0;
        else if(REQUEST_LOOKAHEAD)begin
            if(raw_accepted)begin
                look_valid<=post_present[1];look_mask<=post_mask;
                look_payload<=post_payload[1];
            end else if(state==RESPONSE && !response_consumed)begin
                look_valid<=present[1];look_mask<=req_valid;
                look_payload<=payload[1];
            end else if(response_consumed)look_valid<=0;
        end
    end
''' + text[end:]
    return text


def prepare(source, out, split=False):
    assert not out.exists(), 'Inspect the existing experiment before another run'
    originals = {n: (source / n).read_bytes() for n in EXPECTED}
    assert {n: hashlib.sha256(b).hexdigest() for n, b in originals.items()} == EXPECTED
    arb = transform_arb(originals['dorado_storage_arb.sv'].decode())
    backend = transform_parallel(originals['dorado_storage_sdram_burst.sv'].decode())
    assert hashlib.sha256(arb.encode()).hexdigest() == BASELINE_ARB
    assert hashlib.sha256(backend.encode()).hexdigest() == BASELINE_BACKEND
    candidate = transform(arb)
    if split:
        candidate = split_selection(candidate)
    generated = {'dorado_storage_arb.sv': candidate,
                 'dorado_storage_sdram_burst.sv': backend}
    out.mkdir(parents=True)
    for name, body in generated.items():
        (out / name).write_text(body)
    r = dict(status='PREPARED_REGISTERED_REQUEST_LOOKAHEAD_UNQUALIFIED', scope=__doc__,
             parent_hashes=EXPECTED, qualified_overlap_arb_sha256=BASELINE_ARB,
             generated_hashes={n: hashlib.sha256(s.encode()).hexdigest() for n, s in generated.items()},
             defaults_off=True, parallel_lookup=True, request_lookahead=True,
             split_selection=split,
             hardware_changed=False, native_fit_or_firmware_qualified=False)
    (out / 'provenance.json').write_text(json.dumps(r, indent=2) + '\n')
    return r


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--split-selection', action='store_true')
    a = p.parse_args()
    print(json.dumps(prepare(a.source.resolve(), a.output.resolve(), a.split_selection), indent=2))
