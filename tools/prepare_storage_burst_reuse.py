#!/usr/bin/env python3
"""Prepare default-off received/future-beat and completed-line reuse during BL8.

Keep the original physical address as the stable fill tag. A12-bit extension
tracks eight received beats and one logical reply waiting for a future beat.
Only reads can enter while draining, and only when the preceding owner is no
longer waiting for data and the held reply can be consumed. Physical command,
write/old-word, full-drain, refresh and architectural commit rules are retained.
"""
import argparse
import hashlib
import json
from pathlib import Path

from prepare_storage_response_pipeline import replace_once

BACKEND='dorado_storage_sdram_burst.sv'
PARENT_SHA256='8f92b6fb9105acae978875cd79d245b4d5c2d7a6edcff18cf2960b40b55995ac'


def transform(raw):
    assert hashlib.sha256(raw).hexdigest()==PARENT_SHA256
    s=raw.decode()
    s=replace_once(s,'parameter integer BURST_READ_SUCCESSOR=0',
        'parameter integer BURST_READ_SUCCESSOR=0,\n    parameter integer BURST_READ_REUSE=0')
    s=replace_once(s,'    reg [2:0] fill_beat=0;', '''    reg [2:0] fill_beat=0;
    reg [7:0] fill_valid=0;
    reg fill_waiting=0;
    reg [2:0] fill_reply_beat=0;
    // address remains the physical request throughout READ_WAIT. Reused
    // reads have separate reply state and never replace this fill identity.
    wire [22:0] fill_tag=address[25:3];
    reg [15:0] fill_lookup_data;
    always @* begin
        fill_lookup_data=0;
        for(integer reuse_line=0;reuse_line<CACHE_LINES;reuse_line=reuse_line+1)
            if(fill_line==reuse_line)
                fill_lookup_data=line_words[reuse_line][16*req_addr[2:0]+:16];
    end''')
    s=replace_once(s,'    assign req_ready=initialized && !refresh_due && !reset &&', '''    wire fill_read_available=BURST_READ_REUSE && state==READ_WAIT &&
        delay_count==0 && !aborted && !req_write && !fill_waiting &&
        (!response_pending || (RESPONSE_OVERLAP && rsp_ready)) &&
        (lookup_hit || offered_address[25:3]==fill_tag);
    assign req_ready=initialized && !refresh_due && !reset &&''')
    s=replace_once(s,'         burst_successor_ready);','         burst_successor_ready || fill_read_available);')
    s=replace_once(s,'                fill_beat<=0;selected_data<=0;state<=SELECT;', '''                fill_beat<=0;selected_data<=0;state<=SELECT;
                if(BURST_READ_REUSE)begin
                    fill_valid<=0;fill_waiting<=1;fill_reply_beat<=req_addr[2:0];
                end''')
    s=replace_once(s,'''            if(req_valid && req_ready)begin
                // Clear-before-case priority replaces the consumed reply.
                // Keep address/data/writing as the original physical request.
                rsp_data<=dq_capture;
                response_pending<=1;
            end
            if(fill_beat==address[2:0])begin''', '''            if(BURST_READ_REUSE)begin
                if(!aborted && !reset)begin
                    fill_valid[fill_beat]<=1;
                    if(fill_waiting && fill_beat==fill_reply_beat)begin
                        rsp_data<=dq_capture;response_pending<=1;fill_waiting<=0;
                    end
                end
                if(req_valid && req_ready)begin
                    // The old response is consumed before this replacement.
                    // No raw/physical ownership changes for a reused read.
                    if(lookup_hit)begin
                        rsp_data<=lookup_data;response_pending<=1;cache_hit<=1;
                    end else if(fill_valid[req_addr[2:0]])begin
                        rsp_data<=fill_lookup_data;response_pending<=1;
                    end else if(fill_beat==req_addr[2:0])begin
                        rsp_data<=dq_capture;response_pending<=1;
                    end else begin
                        fill_reply_beat<=req_addr[2:0];fill_waiting<=1;
                    end
                end
            end else if(req_valid && req_ready)begin
                // Clear-before-case priority replaces the consumed reply.
                // Keep address/data/writing as the original physical request.
                rsp_data<=dq_capture;
                response_pending<=1;
            end
            if(!BURST_READ_REUSE && fill_beat==address[2:0])begin''')
    s=replace_once(s,'        if(reset)line_valid<=0;', '''        if(reset)begin
            line_valid<=0;
            if(BURST_READ_REUSE)begin fill_valid<=0;fill_waiting<=0;end
        end''')
    s=replace_once(s,'''           (BURST_READ_SUCCESSOR && (!EARLY_READ_RESPONSE || !RESPONSE_OVERLAP)))''', '''           (BURST_READ_SUCCESSOR && (!EARLY_READ_RESPONSE || !RESPONSE_OVERLAP)) ||
           (BURST_READ_REUSE!=0 && BURST_READ_REUSE!=1) ||
           (BURST_READ_REUSE && (!EARLY_READ_RESPONSE || !RESPONSE_OVERLAP || BURST_READ_SUCCESSOR)))''')
    return s.encode()


def prepare(parent,output):
    assert not output.exists(),'Inspect existing work rather than overwriting it'
    proof=json.loads((parent/'provenance.json').read_text())
    assert proof['burst_read_successor']
    files={name:(parent/name).read_bytes()for name in proof['generated_hashes']}
    for name,raw in files.items():assert hashlib.sha256(raw).hexdigest()==proof['generated_hashes'][name]
    files[BACKEND]=transform(files[BACKEND])
    output.mkdir(parents=True)
    for name,raw in files.items():(output/name).write_bytes(raw)
    proof.update(status='PREPARED_BURST_READ_REUSE_UNQUALIFIED',scope=__doc__,
        immediate_parent_provenance_sha256=hashlib.sha256((parent/'provenance.json').read_bytes()).hexdigest(),
        parent_hashes=proof['generated_hashes'],
        generated_hashes={name:hashlib.sha256(raw).hexdigest()for name,raw in files.items()},
        burst_read_reuse=True,new_state_bits=12,defaults_off=True,
        native_fit_or_firmware_qualified=False)
    (output/'provenance.json').write_text(json.dumps(proof,indent=2)+'\n')
    return proof


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['parent','output']:parser.add_argument('--'+name,type=Path,required=True)
    args=parser.parse_args()
    print(prepare(args.parent.resolve(),args.output.resolve())['status'])
