#!/usr/bin/env python3
"""Serial access to the selected native storage frontend's physical registers.

The host work continues during guest stalls. Only a final external hold stops
both domains; the caller must first drain accepted raw transactions and hold
all consumers. SDRAM refresh and global service barriers are outside this cell.
The guest advance must be the actual clock gate's pre-edge enable. This pins
the exact frozen native nondeferred source; changing that policy needs a new schema/gate.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import generate_snapshot_ff as ff
from generate_serial_boards import raw_receipt

ROOT=Path(__file__).resolve().parents[3]
SHA='542b7f2b27a27fca6d7263e8d2a9631ce24de13cab952c791687aef4ee30fcfa'
FIELDS=[('cas_d',1),('we_d',1),('write_pending',1),('write_data','WIDTH'),
        ('read_pipe','WIDTH'),('data_out','WIDTH'),('cache_valid',1),('write_done',1),
        ('waiting',1),('saved_write',1),('cache_addr','ADDR_BITS'),('saved_addr','ADDR_BITS'),
        ('cache_data',16),('saved_data',16),('old_data',16)]


def fields(width,addr_bits=16):
    if width not in (8,12)or addr_bits!=16:raise ValueError('unqualified native storage geometry')
    return [(n,width if w=='WIDTH'else addr_bits if w=='ADDR_BITS'else w)for n,w in FIELDS]


def original_source():
    return raw_receipt(ROOT/'fpga/mister/reports/2026-10-19-snapshot-msa-ff',
                       'native-source/src/fpga/mister/dorado_storage_group.sv')


def generate(source=None):
    if source is None:source=original_source().decode()
    if hashlib.sha256(source.encode()).hexdigest()!=SHA:raise ValueError('unreviewed original storage frontend')
    # Exact pinned boundaries retain both normal process bodies byte-for-byte.
    machine_start=source.index('    always @(posedge machine_clk) begin')
    host_start=source.index('    always @(posedge host_clk) begin',machine_start)
    assertion_start=source.index('    // synthesis translate_off',host_start)
    machine=source[machine_start:host_start]
    host=source[host_start:assertion_start]
    text=source[:machine_start]+source[assertion_start:]
    text=ff.contb.replace_once(text,'module dorado_storage_group #(','module dorado_storage_group_snapshot #(')
    text=ff.contb.replace_once(text,'    input wire host_clk, machine_clk, reset,','''    input wire state_guest_advance,state_hold,state_scan_step,state_serial_in,
    output wire state_serial_out,state_drain_ready,
    output reg state_frozen=1'b0,
    input wire host_clk, machine_clk, reset,''')
    text=ff.contb.replace_once(text,'assign ready=!reset && !waiting &&','assign normal_ready=!reset && !waiting &&')
    text=ff.contb.replace_once(text,'assign req_valid=!reset && !waiting &&','assign normal_req_valid=!reset && !waiting &&')
    text=ff.contb.replace_once(text,'assign rsp_ready=waiting;','assign rsp_ready=waiting && !state_hold;')
    text=ff.contb.replace_once(text,'    wire hit=cache_valid && cache_addr==addr;',
         '    wire normal_ready,normal_req_valid;\n    wire hit=cache_valid && cache_addr==addr;')
    packed='{'+','.join(n for n,_ in reversed(FIELDS))+'}'
    machine_body=re.sub(r'^\s*always @\(posedge machine_clk\)\s*','',machine,count=1)
    host_body=re.sub(r'^\s*always @\(posedge host_clk\)\s*','',host,count=1)
    access=f'''    localparam integer STATE_BITS=55+3*WIDTH+2*ADDR_BITS;
    wire [STATE_BITS-1:0] state_bits={packed};
    assign state_serial_out=state_bits[0];
    assign ready=normal_ready && !state_hold;
    assign req_valid=normal_req_valid && !state_hold;
    // Use only BEFORE final hold, not as a live acknowledgment during rotation.
    assign state_drain_ready=normal_ready && !normal_req_valid;
    always @(posedge host_clk) state_frozen<=state_hold;
    always @(posedge host_clk)begin
        if(state_hold)begin
            if(state_scan_step) {packed} <= {{state_serial_in,state_bits[STATE_BITS-1:1]}};
        end else begin
            if(state_guest_advance) {machine_body}
            {host_body}
        end
    end
    generate if(ADDR_BITS!=16 || (WIDTH!=8 && WIDTH!=12))begin:unqualified_native_geometry
        initial $fatal(1,"Storage snapshot requires native address16/width8-or-12");
    end endgenerate
'''
    text=ff.contb.replace_once(text,'    // synthesis translate_off',access+'    // synthesis translate_off')
    text=ff.contb.replace_once(text,'    always @(posedge machine_clk) begin',
        '    always @(posedge host_clk) if(!state_hold && state_guest_advance)begin')
    # All original initial values remain. No deferred-policy fields are added.
    manifest=dict(scope=__doc__,original_sha256=SHA,generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
                  module='dorado_storage_group_snapshot',parameters=dict(ADDR_BITS=16,WIDTH=[8,12]),
                  fields=FIELDS,physical_bits=dict(width8=111,width12=123),private_bits=1,
                  policy='Exact archived native nondeferred source, not the working-tree deferred-capable variant')
    return text,manifest


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    a.output.mkdir(parents=True,exist_ok=False);source,manifest=generate()
    (a.output/'storage_group_snapshot.sv').write_text(source)
    (a.output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
