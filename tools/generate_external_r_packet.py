#!/usr/bin/env python3
"""Move the qualified packet's sole SRAM to its existing publication caller.

No processor invocation, forwarding, cache key, ready condition or result
layout changes. Export the exact selected FIRST-event port. The caller retains
the original828+16 ownership and uses the same commit for owners and SRAM.
This is preparation for sharing the port with the rare staged-read path; the
two paths and native visible-R cells are not integrated by this transformation.
"""
from generate_owned_r_packet import native_publication
from prove_cp_prefix_cache import module


MEMORY = ''' dorado_r_file_ports files(host_clk,advance && ready,owned_r_write_n,owned_port_address,owned_port_select_n,
  owned_file_write_data,owned_port_address,owned_port_select_n,original_read[15:0],original_read[17:16]);'''


def packet(raw):
    original = module(raw, 'dorado_r_front_packet_step')
    actual = original
    edits = []
    def change(old, new):
        nonlocal actual
        assert actual.count(old) == 1, old
        actual = actual.replace(old, new, 1)
        edits.append((old, new))
    change(' output reg ready,output wire [1:0] forwarded,input wire reset);',
           ''' output reg ready,output wire [1:0] forwarded,input wire reset,
 input wire [17:0] external_file_read,
 output wire [1:0] memory_write_n,output wire [31:0] memory_address,
 output wire [3:0] memory_select_n,output wire [15:0] memory_write_data);''')
    change(MEMORY, ''' // Same selected first-event ports; the caller owns the ONLY SRAM.
 assign original_read=external_file_read;
 assign memory_write_n=owned_r_write_n;
 assign memory_address=owned_port_address;
 assign memory_select_n=owned_port_select_n;
 assign memory_write_data=owned_file_write_data;''')
    restored = actual
    for old, new in reversed(edits):
        assert restored.count(new) == 1
        restored = restored.replace(new, old, 1)
    assert restored == original
    assert raw.count(original) == 1
    result = raw.replace(original, actual, 1)
    assert result.replace(actual, original, 1) == raw
    assert 'dorado_r_file_ports files(' not in result
    return result


def caller(raw, mutation=None):
    native_publication(raw)  # Exact owner/layout/write-site binding.
    old = '''  first_result,last_result,control_ready,forwarded,reset);'''
    new = '''  first_result,last_result,control_ready,forwarded,reset,
  physical_read,memory_write_n,memory_address,memory_select_n,memory_write_data);
 // Exactly the publication condition above, with unchanged read-before-write.
 dorado_r_file_ports files(storage_clk,owner_commit,memory_write_n,memory_address,memory_select_n,
  memory_write_data,memory_address,memory_select_n,physical_read[15:0],physical_read[17:16]);'''
    assert raw.count(old) == 1
    actual = raw.replace(old, new, 1)
    declaration = ''' wire owner_commit=contb_advance && control_ready;
 wire [17:0] physical_read;
 wire [1:0] memory_write_n;wire [31:0] memory_address;
 wire [3:0] memory_select_n;wire [15:0] memory_write_data;
'''
    marker = ' dorado_r_front_packet_step packet('
    assert actual.count(marker) == 1
    actual = actual.replace(marker, declaration+marker, 1)
    assert actual.replace(declaration, '', 1).replace(new, old, 1) == raw
    assert actual.count('dorado_r_file_ports files(') == 1
    if mutation == 'early-write':
        actual = actual.replace('files(storage_clk,owner_commit,', "files(storage_clk,1'b1,", 1)
    elif mutation == 'drop-forward':
        # Mutated in packet(), not in the caller.
        pass
    else:
        assert mutation is None
    return actual


def bench(raw):
    old = 'dut.packet.files.'
    assert raw.count(old) >= 12
    actual = raw.replace(old, 'dut.files.')
    assert actual.replace('dut.files.', old) == raw
    return actual
