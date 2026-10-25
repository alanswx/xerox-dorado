#!/usr/bin/env python3
"""Insert a registered physical RM/STK read in the actual owned caller.

The single SRAM owner and its write edge stay unchanged. Preparation runs on
host clocks while publication waits for the matching buffered read. Interior
reads must accept a matching response at BOTH of their read epochs. This is a
capture-boundary integration, not qualification of the original60ns schedule.
"""


def memory(raw):
    old='output wire [15:0] read_data,output wire [1:0] read_parity);'
    new=old.replace(');',',output wire [35:0] physical_banks);')
    assert raw.count(old)==1
    extra=''' // Raw banks feed the one registered frontier; no additional RAM owner.
 assign physical_banks={stk_l[read_address[31:24]],rm_l[read_address[23:16]],
                       stk_h[read_address[15:8]],rm_h[read_address[7:0]]};
'''
    after=raw.replace(old,new,1).replace('endmodule',extra+'endmodule',1)
    assert after.replace(extra,'',1).replace(new,old,1)==raw
    return after


def caller(raw):
    original=raw;edits=[]
    def change(a,b):
        nonlocal raw
        assert raw.count(a)==1,a
        raw=raw.replace(a,b,1);edits.append((a,b))
    change(' wire [17:0] physical_read;', ''' wire [17:0] physical_read,asynchronous_read;
 wire [35:0] physical_banks,buffer_tag,buffer_banks;
 wire buffer_valid,buffer_ready;''')
    change(' assign owner_commit=contb_advance && control_ready;', ''' // Prepare independently of packet/payload readiness: no completion feedback.
 dorado_r_read_buffer read_frontier(storage_clk,reset,!reset,
  physical_address,physical_select,physical_banks,
  owner_commit,memory_write_n,memory_address,memory_select_n,memory_write_data,
  buffer_valid,buffer_ready,physical_read,buffer_tag,buffer_banks);
 assign owner_commit=contb_advance && control_ready;''')
    change('assign control_ready=packet_ready && read_ready && payload_ready;',
           'assign control_ready=packet_ready && read_ready && payload_ready && (stage_active || buffer_ready);')
    change('stage(storage_clk,reset,read_prepare,read_needed,read_accept,owner_commit,',
           'stage(storage_clk,reset,read_prepare && buffer_ready,read_needed,read_accept && buffer_ready,owner_commit,')
    change('memory_write_data,physical_address,physical_select,physical_read[15:0],physical_read[17:16]);',
           'memory_write_data,physical_address,physical_select,asynchronous_read[15:0],asynchronous_read[17:16],physical_banks);')
    restored=raw
    for a,b in reversed(edits):restored=restored.replace(b,a,1)
    assert restored==original
    return raw
