#!/usr/bin/env python3
"""Connect qualified network pause to the immutable Serial69 memory boundary.

The physical DDR fence and Main/disk/input/video permissions remain external.
Keep every original ordinary clock, enable, memory transaction and board pin.
"""
from generate_serial_boards import once

WRAPPER='src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
PREFIX='src/fpga/mister/snapshot_serial/'


def core(text):
    text=once(text,'dorado_ethernet_ddr u_network_ddr (',
        'dorado_ethernet_ddr #(.SNAPSHOT_PAUSE(1)) u_network_ddr (')
    text=once(text,'.enable(net_ddr_enable),.epoch(net_ddr_epoch),',
        '.enable(net_ddr_enable),.epoch(net_ddr_epoch),\n    .snapshot_hold(board_state_hold),.snapshot_idle(snapshot_network_idle),')
    text=once(text,'dorado_ethernet_uio #(.MAX_WORDS(1024)',
        'dorado_ethernet_uio #(.MAX_WORDS(1024),.SNAPSHOT_PAUSE(1)')
    text=once(text,'.bus(ethernet_ext_bus),',
        '.bus(ethernet_ext_bus),\n    .snapshot_hold(snapshot_network_parser_hold),.snapshot_idle(snapshot_network_parser_idle),\n    .snapshot_held(snapshot_network_parser_held),')
    block='''// Network permission includes the actual engine/parser/settling boundary.
// The external permit still owns physical DDR visibility and the ARM lease.
wire board_state_owner_hold,snapshot_network_hold,snapshot_network_parser_hold;
wire snapshot_network_idle,snapshot_network_parser_idle,snapshot_network_parser_held;
wire snapshot_network_drained,snapshot_network_fault;
wire [4:0] snapshot_service_permits={CAPACITY_SERVICES_HELD[4:3],
    (CAPACITY_SERVICES_HELD[2] && snapshot_network_drained),CAPACITY_SERVICES_HELD[1:0]};
dorado_network_pause_barrier u_network_pause(
    .clk(clk_sys),.machine_reseed(1'b0),.owner_hold(board_state_owner_hold),
    .guest_held(board_state_guest_held),
    .abort(board_state_fault || (CAPACITY_TRANSPORT_RESET && board_state_hold)),
    .device_reset(~pll_locked),.device_fault((|net_ddr_fault) || (|net_ddr_control_errors)),
    .engine_idle(snapshot_network_idle),.parser_idle(snapshot_network_parser_idle),
    .parser_held(snapshot_network_parser_held),.hold(snapshot_network_hold),
    .parser_hold(snapshot_network_parser_hold),.ready(snapshot_network_drained),.fault(snapshot_network_fault));
'''
    text=once(text,'assign board_state_guest_held=board_state_hold && !network_step;',
        block+'assign board_state_guest_held=board_state_hold && !network_step;')
    return once(text,'.snapshot_services_held(CAPACITY_SERVICES_HELD),',
        '.snapshot_services_held(snapshot_service_permits),\n    .snapshot_service_hold(snapshot_network_hold),.snapshot_service_fault(snapshot_network_fault),\n    .snapshot_owner_holding(board_state_owner_hold),')


def wrapper(text):
    text=once(text,'    input wire snapshot_guest_held,',
        '    input wire snapshot_guest_held,\n    input wire snapshot_service_hold,snapshot_service_fault,\n    output wire snapshot_owner_holding,')
    text=once(text,'  assign snapshot_fault=snapshot_owner_fault || snapshot_memory_fault;',
        '  assign snapshot_owner_holding=snapshot_owner_hold;\n  assign snapshot_fault=snapshot_owner_fault || snapshot_memory_fault || snapshot_service_fault;')
    text=once(text,'.owner_hold(snapshot_owner_hold),',
        '.owner_hold(snapshot_owner_hold || snapshot_service_hold),')
    return once(text,'.abort(snapshot_owner_fault || (snapshot_transport_reset && snapshot_hold)),',
        '.abort(snapshot_owner_fault || snapshot_service_fault || (snapshot_transport_reset && snapshot_hold)),')
