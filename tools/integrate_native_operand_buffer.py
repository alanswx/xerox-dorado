#!/usr/bin/env python3
"""Prepare the proved native88-bit operand/flag projection before publication.

Keep every original transfer/result/guard. Export whether every actual single
function input is known, for exact original X/Z fallback. Paired mode stays on
its original path. The prepared view replaces only the native physical view;
the caller's closed owners, SRAM, visible-R cells and clock enables stay intact.
"""
from prove_cp_prefix_cache import module
from qualify_native_ready_compare import SINGLE,ports

HELPER='verilog/rtl/dorado_native_operand_buffer.sv'


def control(raw):
    original=raw;edits=[]
    before=module(raw,SINGLE)
    a='output wire [35:0] native_R_frame\n);'
    b='output wire [35:0] native_R_frame,output reg operand_context_known\n);'
    assert before.count(a)==1
    input_names=[n for d,w,n in ports(before)if d=='input']
    # host_clk/advance are constant0 at this actual packet boundary; including
    # them in the four-state detector still preserves diagnostic callers.
    extra=''' always @*begin
  case(^{CONTEXT})
   1'b0,1'b1:operand_context_known=1'b1;
   default:operand_context_known=1'b0;
  endcase
 end
'''.replace('CONTEXT',','.join(input_names))
    after=before.replace(a,b,1).replace('endmodule',extra+'endmodule',1)
    assert after.replace(b,a,1).replace(extra,'',1)==before
    raw=raw.replace(before,after,1);edits.append((before,after))
    before=module(raw,'dorado_r_front_packet_step');after=before
    a='output wire [35:0] native_R_frame);'
    b='output wire [35:0] native_R_frame,output wire native_operand_known);'
    assert after.count(a)==1;after=after.replace(a,b,1)
    a='.native_R_frame(single_native_R));';b='.native_R_frame(single_native_R),.operand_context_known(native_operand_known));'
    assert after.count(a)==1;after=after.replace(a,b,1)
    assert after.replace(b,a,1).replace('output wire [35:0] native_R_frame,output wire native_operand_known);','output wire [35:0] native_R_frame);',1)==before
    raw=raw.replace(before,after,1);edits.append((before,after))
    restored=raw
    for a,b in reversed(edits):restored=restored.replace(b,a,1)
    assert restored==original
    return raw


def caller(raw):
    original=raw;edits=[]
    def change(a,b):
        nonlocal raw
        assert raw.count(a)==1,a
        raw=raw.replace(a,b,1);edits.append((a,b))
    # The staged native caller has two additional read-only before exports.
    a='output wire [35:0] native_R_frame'
    change(a,a+',output wire [843:0] prepared_physical_state')
    a=' wire packet_ready,read_ready,read_prepare;'
    change(a,a+'''
 wire operand_context_known,operand_view_ready,operand_view_valid;
 wire [1263:0] operand_view_tag;wire [87:0] operand_view_fields;''')
    a=' assign owner_commit=contb_advance && control_ready;'
    change(a,''' dorado_native_operand_buffer operand_frontier(storage_clk,reset,
  (stage_active || buffer_ready),use_pair,operand_context_known,
  r_front_held,r_front_clock_before,r_front_before,before_data,first_result[859:16],
  operand_view_ready,prepared_physical_state,operand_view_valid,operand_view_tag,operand_view_fields);
'''+a)
    a='assign control_ready=packet_ready && read_ready && payload_ready && (stage_active || buffer_ready);'
    change(a,a[:-1]+' && operand_view_ready;')
    a='capture_data,stage_valid,read_prepare,read_needed,interior_address,interior_select,native_R_frame);'
    change(a,a[:-2]+',operand_context_known);')
    restored=raw
    for a,b in reversed(edits):restored=restored.replace(b,a,1)
    assert restored==original
    return raw
