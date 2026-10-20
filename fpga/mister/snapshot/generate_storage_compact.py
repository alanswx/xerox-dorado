#!/usr/bin/env python3
"""Experimental native nondeferred frontend without duplicate transaction buffers.

Required contract: advance only when every group is ready; addr_will_change
predicts the address-register update on that guest edge. Address/write_data
therefore stay stable for each accepted raw transaction. The cache commits a
completed write AFTER the guest has sampled the old word. All actual registers
are exported; a new schema is required. No native integration is implied.
"""
import argparse
import hashlib
import json
from pathlib import Path
import generate_storage_group as original

FIELDS=[('cas_d',1),('we_d',1),('write_pending',1),('write_data','WIDTH'),
        ('read_pipe','WIDTH'),('data_out','WIDTH'),('cache_valid',1),
        ('write_done',1),('waiting',1),('cache_data',16)]


def fields(width,addr_bits=16):
    if width not in (8,12) or addr_bits != 16: raise ValueError('unqualified compact geometry')
    return [(n,width if w=='WIDTH' else w) for n,w in FIELDS]


def generate():
    raw=original.original_source();assert hashlib.sha256(raw).hexdigest()==original.SHA
    old=raw.decode()
    guest=old[old.index('        cas_d<=cas_n;we_d<=we_n;'):old.index('    always @(posedge host_clk)')]
    assert guest.endswith('    end\n');guest=guest[:-len('    end\n')]
    packed='{'+','.join(n for n,_ in reversed(FIELDS))+'}'
    text='''// Experimental compact frontend. Original DRAM guest pipeline retained.
`default_nettype none
module dorado_storage_group_compact #(
    parameter integer ADDR_BITS=16, WIDTH=8
) (
    input wire state_guest_advance,state_hold,state_scan_step,state_serial_in,
    output wire state_serial_out,state_drain_ready,
    output reg state_frozen=1'b0,
    input wire host_clk,machine_clk,reset,
    input wire ras_n,cas_n,we_n,addr_will_change,
    input wire [ADDR_BITS-1:0] addr,
    input wire [WIDTH-1:0] data_in,
    output reg [WIDTH-1:0] data_out=0,
    output wire ready,req_valid,
    input wire req_ready,
    output wire req_write,
    output wire [ADDR_BITS-1:0] req_addr,
    output wire [15:0] req_data,
    input wire rsp_valid,
    output wire rsp_ready,
    input wire [15:0] rsp_data
);
    reg cas_d=0,we_d=0,write_pending=0;
    reg [WIDTH-1:0] write_data=0,read_pipe=0;
    reg cache_valid=0,write_done=0,waiting=0;
    reg [15:0] cache_data=0;
    wire normal_ready=!reset && !waiting && cache_valid && (!write_pending || write_done);
    wire normal_req_valid=!reset && !waiting && (!cache_valid || (write_pending && !write_done));
    assign ready=normal_ready && !state_hold;
    assign req_valid=normal_req_valid && !state_hold;
    assign rsp_ready=waiting && !state_hold;
    assign state_drain_ready=normal_ready && !normal_req_valid;
    assign req_write=cache_valid && write_pending && !write_done;
    assign req_addr=addr;
    assign req_data={{(16-WIDTH){1'b0}},write_data};
    wire [15:0] read_value=cache_data;
'''
    text+=f'''    localparam integer STATE_BITS=22+3*WIDTH;
    wire [STATE_BITS-1:0] state_bits={packed};
    assign state_serial_out=state_bits[0];
    always @(posedge host_clk)state_frozen<=state_hold;
    always @(posedge host_clk)begin
        if(state_hold)begin
            if(state_scan_step){packed}<={{state_serial_in,state_bits[STATE_BITS-1:1]}};
        end else begin
            if(state_guest_advance)begin
{guest}            end
            if(reset)begin
                cache_valid<=0;write_done<=0;waiting<=0;
                if(write_pending && write_done)cache_data<=req_data;
            end else begin
                if(!write_pending)write_done<=0;
                if(state_guest_advance)begin
                    if(addr_will_change)cache_valid<=0;
                    if(write_pending && write_done)cache_data<=req_data;
                end
                if(req_valid && req_ready)waiting<=1;
                if(waiting && rsp_valid)begin
                    waiting<=0;cache_valid<=1;
                    if(req_write)write_done<=1;
                    else cache_data<=rsp_data;
                end
            end
        end
    end
    generate if(ADDR_BITS!=16 || (WIDTH!=8 && WIDTH!=12))begin:unqualified_geometry
        initial $fatal(1,"Compact frontend requires address16/width8-or-12");
    end endgenerate
    // synthesis translate_off
    always @(posedge host_clk)if(!state_hold && state_guest_advance)begin
        if(!ready)$fatal(1,"Dorado clock advanced while compact storage stalled");
        if(write_pending && (cas_d || we_d))$fatal(1,"Pending storage write has inconsistent DRAM controls");
        if(!ras_n && !cas_n && !we_n && (cas_d || we_d) && req_valid)
            $fatal(1,"Storage write_data changed while request was enabled");
    end
    // synthesis translate_on
endmodule
`default_nettype wire
'''
    return text,dict(scope=__doc__,original_sha256=original.SHA,generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
        fields=FIELDS,physical_bits=dict(width8=46,width12=58),private_bits=1,
        removed_fields=['saved_write','cache_addr','saved_addr','saved_data','old_data'],
        removed_actual_register_bits_per_group=65,
        state_relation='cache_valid=reference.hit; cache_data=reference.read_value; eight other fields equal; no old snapshot-schema compatibility')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    a.output.mkdir(parents=True,exist_ok=False);source,manifest=generate()
    (a.output/'storage_group_compact.sv').write_text(source)
    (a.output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
