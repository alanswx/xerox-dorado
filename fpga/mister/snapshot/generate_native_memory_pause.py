#!/usr/bin/env python3
"""Connect late memory freeze in the frozen nineteen-board native hierarchy.

Only original raw-memory transactions drain. Original clocks, board pins,
CPU/cache/map/ECC and normal frontend readiness remain intact. Other service
acknowledgments remain explicit virtual inputs; no deployable snapshot claim.
Quarantine recovery requires a complete FPGA configuration, never PLL loss.
"""
from generate_serial_boards import once

BP='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
WRAPPER='src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
PREFIX='src/fpga/mister/snapshot_serial/'


def core(text):
    text=once(text,'assign CAPACITY_MEMORY_DRAINED=snapshot_frontends_ready;\n','')
    text=once(text,'wire board_state_guest_held,','wire [4:0] board_state_services_held;\nwire board_state_guest_held,')
    text=once(text,'.local_held(board_state_guest_held),.services_held(CAPACITY_SERVICES_HELD)',
        '.local_held(board_state_guest_held),.services_held(board_state_services_held)')
    text=once(text,'.snapshot_memory_drained(snapshot_frontends_ready),',
        '.snapshot_memory_drained(CAPACITY_MEMORY_DRAINED),\n    .snapshot_frontends_ready(snapshot_frontends_ready),\n    .snapshot_services_status(board_state_services_held),')
    text=once(text,'.snapshot_system_reset(~pll_locked),',".snapshot_system_reset(1'b0),")
    text=once(text,'.clk(clk_sys),.system_reset(~pll_locked),.transport_reset(CAPACITY_TRANSPORT_RESET),.bus(board_state_ext_bus)',
        ".clk(clk_sys),.system_reset(1'b0),.transport_reset(CAPACITY_TRANSPORT_RESET),.bus(board_state_ext_bus)")
    return once(text,'// SYSTEM reset is PLL/RBF lifecycle; menu/framework/parser resets cannot thaw state.',
        '// Snapshot control initializes on complete RBF configuration only.\n// PLL loss and menu/framework/parser resets cannot thaw partially imported state.')


def backplane(text):
    text=once(text,'    input wire snapshot_guest_held,',
        '    input wire snapshot_guest_held,\n    input wire snapshot_consumers_hold,snapshot_frontend_hold,')
    assert text.count('.state_global_hold(snapshot_hold)')==19
    text=text.replace('.state_global_hold(snapshot_hold)','.state_global_hold(snapshot_consumers_hold)')
    assert text.count('.state_freeze(snapshot_hold)')==19
    text=text.replace('.state_freeze(snapshot_hold)','.state_freeze(snapshot_consumers_hold)')
    # Only the eight MSA instances have this following named port.
    needle='    .state_memory_drained(snapshot_msa_drained['
    assert text.count(needle)==8
    return text.replace(needle,'    .state_frontend_hold(snapshot_frontend_hold),\n'+needle)


def wrapper(text):
    text=once(text,'    output wire snapshot_memory_drained,',
        '    output wire snapshot_memory_drained,\n    output wire snapshot_frontends_ready,\n    output wire [4:0] snapshot_services_status,')
    text=once(text,'  wire board_ready;',
        '  wire snapshot_owner_hold,snapshot_owner_fault;\n  wire snapshot_frontend_hold,snapshot_memory_fault;\n  wire snapshot_arbiter_idle,snapshot_raw_idle;\n  wire board_ready;')
    text=once(text,'    .clk(clk_host),.reset(reset || !storage_initialized),',
        '    .clk(clk_host),.reset(reset || !storage_initialized),.snapshot_idle(snapshot_arbiter_idle),')
    text=once(text,'    .clk(clk_host),.reset(reset),\n    .req_valid(storage_initialized ? rv : init_valid)',
        '    .clk(clk_host),.reset(reset),.snapshot_idle(snapshot_raw_idle),\n    .req_valid(storage_initialized ? rv : init_valid)')
    prefix='''  // Real memory readiness tightens external service bit0; no acknowledgment
  // is tied high. The same vector reaches the owner and public status.
  assign snapshot_services_status={snapshot_services_held[4:1],
      (snapshot_services_held[0] && snapshot_memory_drained)};
  assign snapshot_fault=snapshot_owner_fault || snapshot_memory_fault;
  dorado_memory_pause_barrier u_memory_pause(
    .clk(clk_host),.machine_reseed(snapshot_system_reset),.owner_hold(snapshot_owner_hold),
    .guest_held(snapshot_guest_held),
    .abort(snapshot_owner_fault || (snapshot_transport_reset && snapshot_hold)),
    .frontends_drained(snapshot_frontends_ready),
    .arbiter_idle(storage_initialized && snapshot_arbiter_idle),
    .raw_service_idle(storage_initialized && snapshot_raw_idle),
    .frontends_frozen(snapshot_frozen[11]),.guest_hold(snapshot_hold),
    .frontend_hold(snapshot_frontend_hold),.ready(snapshot_memory_drained),.fault(snapshot_memory_fault));
'''
    text=once(text,'  dorado_native_backplane #(.SYSPER(SYSPER)) m (\n',
        prefix+'  dorado_native_backplane #(.SYSPER(SYSPER)) m (\n'+
        '    .snapshot_consumers_hold(snapshot_hold),.snapshot_frontend_hold(snapshot_frontend_hold),\n')
    text=once(text,'.snapshot_services_held(snapshot_services_held),','.snapshot_services_held(snapshot_services_status),')
    text=once(text,'.snapshot_hold(snapshot_hold),','.snapshot_hold(snapshot_owner_hold),')
    text=once(text,'.snapshot_fault(snapshot_fault),','.snapshot_fault(snapshot_owner_fault),')
    return once(text,'.snapshot_memory_drained(snapshot_memory_drained),','.snapshot_memory_drained(snapshot_frontends_ready),')
