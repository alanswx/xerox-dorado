#!/usr/bin/env python3
"""Predict the exact post-edge MSA frontend flags without changing old state.

The reset-free result is registered outside the MSA in this prototype. Live
reset/initialization guards must remain in the final CPU enable. Raw physical
memory-drain acknowledgments retain their original independent equations.
"""
import copy
import hashlib
from generate_serial_boards import ROOT,raw_receipt,once
import generate_msa_late_freeze as prior

REPORT='2026-10-20-snapshot-native-disk-pause-source'
PREFIX='staged-source/src/fpga/mister/snapshot_serial/MSA__'
BLOCK='''    // Pure prediction of the original four post-edge readiness flags.
    // Automatic locals carry no history; all original clocked writes remain.
    function automatic [3:0] ready_flags_after_edge(
        input [3:0] current_flags,scan_flags,
        input held,scan,advance,reset_now,write_start,address_change,
        input accepted,response,write_kind
    );
        reg [3:0] next_flags;
        begin
            next_flags=current_flags;
            if(held)begin
                if(scan)next_flags=scan_flags;
            end else begin
                if(advance)next_flags[0]=write_start;
                if(reset_now)next_flags[3:1]=3'b000;
                else begin
                    if(!current_flags[0])next_flags[2]=0;
                    if(advance)begin
                        if(address_change)next_flags[1]=0;
                    end
                    if(accepted)next_flags[3]=1;
                    if(response)begin
                        next_flags[3]=0;next_flags[1]=1;
                        if(write_kind)next_flags[2]=1;
                    end
                end
            end
            ready_flags_after_edge=next_flags;
        end
    endfunction
    wire [3:0] ready_flags_next=ready_flags_after_edge(
        {waiting,write_done,cache_valid,write_pending},
        {state_bits[6+3*WIDTH],state_bits[5+3*WIDTH],state_bits[4+3*WIDTH],state_bits[3]},
        state_hold,state_scan_step,state_guest_advance,reset,
        !ras_n && !cas_n && !we_n && (cas_d || we_d),addr_will_change,
        req_valid && req_ready,waiting && rsp_valid,req_write);
    // A live reset is intentionally excluded. Higher-level reset and storage
    // initialization still mask the registered aggregate before any advance.
    assign state_flags_ready_next=!ready_flags_next[3] && ready_flags_next[1] &&
        (!ready_flags_next[0] || ready_flags_next[2]);
'''


def reference(name):return raw_receipt(ROOT/'fpga/mister/reports'/REPORT,PREFIX+name).decode()


def frontend(source):
    source=once(source,'output wire state_serial_out,state_drain_ready,',
        'output wire state_serial_out,state_drain_ready,state_flags_ready_next,')
    return once(source,'    assign state_serial_out=state_bits[0];\n',
        '    assign state_serial_out=state_bits[0];\n'+BLOCK)


def inverse_frontend(source):
    source=once(source,BLOCK,'')
    return once(source,'output wire state_serial_out,state_drain_ready,state_flags_ready_next,',
        'output wire state_serial_out,state_drain_ready,')


def board(source):
    source=once(source,'output wire state_scan_hold_required,state_scan_fault,state_memory_drained,',
        'output wire state_scan_hold_required,state_scan_fault,state_memory_drained,state_memory_flags_next,')
    source=once(source,'  wire [15:0] state_storage_serial,state_storage_frozen,state_storage_drained;',
        '  wire [15:0] state_storage_serial,state_storage_frozen,state_storage_drained,state_storage_flags_next;')
    source=once(source,'  assign state_memory_drained=&state_storage_drained;',
        '  assign state_memory_drained=&state_storage_drained;\n  assign state_memory_flags_next=&state_storage_flags_next;')
    for i in range(16):source=once(source,f'.state_drain_ready(state_storage_drained[{i}]),',
        f'.state_drain_ready(state_storage_drained[{i}]),.state_flags_ready_next(state_storage_flags_next[{i}]),')
    return source


def inverse_board(source):
    source=once(source,',state_memory_flags_next,',',')
    source=once(source,',state_storage_flags_next;', ';')
    source=once(source,'\n  assign state_memory_flags_next=&state_storage_flags_next;','')
    for i in range(16):source=once(source,f'.state_flags_ready_next(state_storage_flags_next[{i}]),','')
    return source


def generate():
    old,manifest,helpers=prior.generate()
    assert old==reference('msa_board.sv'),'generated MSA differs from frozen Serial72'
    assert helpers['storage_group.sv']==reference('storage_group.sv'),'generated frontend differs from frozen Serial72'
    result=board(old);storage=frontend(helpers['storage_group.sv'])
    assert inverse_board(result)==old and inverse_frontend(storage)==helpers['storage_group.sv']
    helpers=dict(helpers);helpers['storage_group.sv']=storage;manifest=copy.deepcopy(manifest)
    manifest.update(scope=__doc__,generated_sha256=hashlib.sha256(result.encode()).hexdigest(),
        cloned_sources={n:hashlib.sha256(s.encode()).hexdigest()for n,s in helpers.items()},
        original_board_bits=1538,new_group_sequential_bits=0,new_board_sequential_bits=0,
        external_required_aggregate_bits_per_msa=1,
        limits='Prediction prototype; native integration, new aggregate-state access, capacity/timing and FPGA qualification remain open.')
    return result,manifest,helpers
