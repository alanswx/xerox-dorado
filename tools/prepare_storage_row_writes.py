#!/usr/bin/env python3
"""Prepare an UNQUALIFIED default-off same-row single-WRITE stream experiment.

Alliance AS4C32M16SB Rev1.4 section6/Figures10-13 and section8/Table11:
MRS A9=1 selects single-location writes; data is sampled with WRITE. A next
WRITE may use the following chip clock. Keep recovery before row changes,
read turnaround and refresh; never reinterpret tWR as a per-WRITE issue gap.
ACK can be consumed only on the host edge after the chip captured the write.
Physical-pin/commit/reset/old-word and native timing qualification remain open.
"""
import argparse
import hashlib
import json
from pathlib import Path

from prepare_storage_response_pipeline import replace_once

BACKEND='dorado_storage_sdram_burst.sv'
PARENT_SHA256='8d04391c9c5e8fbb5dbf5b543840f89d4bd09c89e97d2cfa42f9b47b1ab8340f'


def transform(raw):
    assert hashlib.sha256(raw).hexdigest()==PARENT_SHA256
    s=raw.decode()
    s=replace_once(s,'parameter integer BURST_READ_REUSE=0',
        'parameter integer BURST_READ_REUSE=0,\n    parameter integer PIPELINED_ROW_WRITES=0')
    s=replace_once(s,'    assign req_ready=initialized && !refresh_due && !reset &&', '''    // The preceding WRITE has reached the intervening chip edge before
    // this next host handshake. Only another single WRITE to that same open
    // rank/bank/row bypasses recovery/SELECT; all other work takes the old path.
    wire row_write_available=PIPELINED_ROW_WRITES && state==WRITE_WAIT &&
        !aborted && req_write && response_pending && rsp_ready &&
        offered_address[25:10]==address[25:10];
    assign req_ready=initialized && !refresh_due && !reset &&''')
    s=replace_once(s,'         burst_successor_ready || fill_read_available);',
        '         burst_successor_ready || fill_read_available || row_write_available);')
    s=replace_once(s,'                delay_count<=32\'(T_WR-1);state<=WRITE_WAIT;', '''                delay_count<=32'(T_WR-1);state<=WRITE_WAIT;
                // response_pending becomes visible after this host edge.
                // Its handshake is on the NEXT host edge, after the chip's
                // intervening capture. Reset still suppresses an old ACK.
                if(PIPELINED_ROW_WRITES && !aborted && !reset)response_pending<=1;''')
    old='''        WRITE_WAIT:if(delay_count!=0)delay_count<=delay_count-1'b1;
        else begin
            if(!aborted && !reset)response_pending<=1;
            state<=IDLE;
        end'''
    new='''        WRITE_WAIT:if(PIPELINED_ROW_WRITES && req_valid && req_ready)begin
            // Consume the old ACK before replacing it. No ACK is generated
            // by queue admission: this edge emits the actual WRITE command.
            address<=offered_address;data<=req_data;writing<=1;
            sdram_ncs<=offered_address[25];sdram_ba<=offered_address[24:23];
            sdram_a<={3'b000,offered_address[9:0]};
            command<=WRITE;dq_out<=req_data;dq_oe<=1;
            delay_count<=32'(T_WR-1);response_pending<=1;
            for(integer i=0;i<CACHE_LINES;i=i+1)
                if(line_valid[i] && line_tag[i]==offered_address[25:3])line_valid[i]<=0;
        end else if(delay_count!=0)delay_count<=delay_count-1'b1;
        else begin
            // Early ACKs must not be rearmed when recovery ends.
            if(!PIPELINED_ROW_WRITES && !aborted && !reset)response_pending<=1;
            state<=IDLE;
        end'''
    s=replace_once(s,old,new)
    s=replace_once(s,'(BURST_READ_REUSE && (!EARLY_READ_RESPONSE || !RESPONSE_OVERLAP || BURST_READ_SUCCESSOR)))',
        '''(BURST_READ_REUSE && (!EARLY_READ_RESPONSE || !RESPONSE_OVERLAP || BURST_READ_SUCCESSOR)) ||
           (PIPELINED_ROW_WRITES!=0 && PIPELINED_ROW_WRITES!=1) ||
           (PIPELINED_ROW_WRITES && !RESPONSE_OVERLAP))''')
    return s.encode()


def prepare(parent,output):
    assert not output.exists(),'Inspect existing prepared work'
    proof=json.loads((parent/'provenance.json').read_text());assert proof['burst_read_reuse']
    files={name:(parent/name).read_bytes()for name in proof['generated_hashes']}
    for name,raw in files.items():assert hashlib.sha256(raw).hexdigest()==proof['generated_hashes'][name]
    files[BACKEND]=transform(files[BACKEND]);output.mkdir(parents=True)
    for name,raw in files.items():(output/name).write_bytes(raw)
    proof.update(status='PREPARED_PIPELINED_ROW_WRITES_UNQUALIFIED',scope=__doc__,
        immediate_parent_provenance_sha256=hashlib.sha256((parent/'provenance.json').read_bytes()).hexdigest(),
        parent_hashes=proof['generated_hashes'],
        generated_hashes={name:hashlib.sha256(raw).hexdigest()for name,raw in files.items()},
        pipelined_row_writes=True,new_state_bits=0,defaults_off=True,
        component_or_native_qualified=False)
    (output/'provenance.json').write_text(json.dumps(proof,indent=2)+'\n')
    return proof


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['parent','output']:parser.add_argument('--'+name,type=Path,required=True)
    args=parser.parse_args();print(prepare(args.parent.resolve(),args.output.resolve())['status'])
