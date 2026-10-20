#!/usr/bin/env python3
"""Add real disk drain/release and local device status to frozen Serial71.

Retain all original clocks, board pins and normal media/CPU behavior. External
ARM/input/video permissions and physical DDR visibility remain required.
"""
from generate_serial_boards import once

WRAPPER='src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
PREFIX='src/fpga/mister/snapshot_serial/'


def core(text):
    text=once(text,'wire board_state_owner_hold,snapshot_network_hold,snapshot_network_parser_hold;',
        'wire board_state_disk_drained;\nwire board_state_owner_hold,snapshot_network_hold,snapshot_network_parser_hold;')
    text=once(text,"dorado_board_state_uio_msa #(.SCHEMA_ID(64'haf0b647cb43a3a62),.RESPONSE_LIMIT(4096))",
        "dorado_board_state_uio_msa #(.SCHEMA_ID(64'haf0b647cb43a3a62),.LOCAL_SERVICES_MASK(5'b00111),.RESPONSE_LIMIT(4096))")
    text=once(text,'.local_held(board_state_guest_held),.services_held(board_state_services_held),',
        '.local_held(board_state_guest_held),.services_held(board_state_services_held),\n .local_services_held({2\'b00,snapshot_network_drained,board_state_disk_drained,CAPACITY_MEMORY_DRAINED}),')
    return once(text,'.snapshot_memory_drained(CAPACITY_MEMORY_DRAINED),',
        '.snapshot_memory_drained(CAPACITY_MEMORY_DRAINED),\n    .snapshot_disk_drained(board_state_disk_drained),')


def wrapper(text):
    text=once(text,'    output wire snapshot_memory_drained,',
        '    output wire snapshot_memory_drained,\n    output wire snapshot_disk_drained,')
    text=once(text,'  wire snapshot_owner_hold,snapshot_owner_fault;',
        '  wire snapshot_disk_hold,snapshot_disk_request,snapshot_disk_idle,snapshot_disk_held,snapshot_disk_fault;\n  wire snapshot_owner_hold,snapshot_owner_fault;')
    text=once(text,'dorado_pack_hps #(.SECTORS_PER_TRACK(29),.LABEL_WORDS(10))',
        'dorado_pack_hps #(.SNAPSHOT_PAUSE(1),.SECTORS_PER_TRACK(29),.LABEL_WORDS(10))')
    text=once(text,'.clk_sys(clk_host),.machine_clk(sys_clk),.reset(reset),',
        '.clk_sys(clk_host),.machine_clk(sys_clk),.reset(reset),\n    .snapshot_hold(snapshot_disk_request),.snapshot_guest_held(snapshot_guest_held),\n    .snapshot_idle(snapshot_disk_idle),.snapshot_held(snapshot_disk_held),')
    text=once(text,'  assign snapshot_services_status={snapshot_services_held[4:1],\n      (snapshot_services_held[0] && snapshot_memory_drained)};',
        '  assign snapshot_services_status={snapshot_services_held[4:2],\n      (snapshot_services_held[1] && snapshot_disk_drained),\n      (snapshot_services_held[0] && snapshot_memory_drained)};')
    text=once(text,'snapshot_fault=snapshot_owner_fault || snapshot_memory_fault || snapshot_service_fault;',
        'snapshot_fault=snapshot_owner_fault || snapshot_memory_fault || snapshot_service_fault || snapshot_disk_fault;')
    text=once(text,'.owner_hold(snapshot_owner_hold || snapshot_service_hold),',
        '.owner_hold(snapshot_owner_hold || snapshot_service_hold || snapshot_disk_hold),')
    text=once(text,'.abort(snapshot_owner_fault || snapshot_service_fault || (snapshot_transport_reset && snapshot_hold)),',
        '.abort(snapshot_owner_fault || snapshot_service_fault || snapshot_disk_fault || (snapshot_transport_reset && snapshot_hold)),')
    block='''  // Real disk HPS drain is visible before the ARM image permission. The
  // independent hold keeps all consumers frozen through host-latch release.
  dorado_disk_pause_barrier u_disk_pause(
    .clk(clk_host),.machine_reseed(snapshot_system_reset),.owner_hold(snapshot_owner_hold),
    .guest_held(snapshot_guest_held),
    .abort(snapshot_owner_fault || snapshot_memory_fault || snapshot_service_fault ||
        (snapshot_transport_reset && snapshot_hold)),
    .device_reset(reset),.device_fault(pack_fault || pack_mounted),
    .device_idle(snapshot_disk_idle),.device_held(snapshot_disk_held),
    .hold(snapshot_disk_hold),.device_hold(snapshot_disk_request),
    .ready(snapshot_disk_drained),.fault(snapshot_disk_fault));
'''
    return once(text,'  dorado_memory_pause_barrier u_memory_pause(',block+'  dorado_memory_pause_barrier u_memory_pause(')
