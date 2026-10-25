#!/usr/bin/env python3
"""Exercise descriptor changes against the original full processor checker.

Time-multiplex the one physical SRAM as before. Probe each of72 descriptor
bits while READ is waiting and after DONE; reject before accepting any machine
edge, abort the auxiliary read, restore the proposal and retry. The original
844-Q/RAM/clock oracle sees the final accepted proposal unchanged. This is not
native caller integration or arbitrary original gate-arrival qualification.
"""


def tagged_bench(raw,probe=False):
    original=raw;changes=[]
    def change(a,b):
        nonlocal raw
        assert raw.count(a)==1,a
        raw=raw.replace(a,b,1);changes.append((a,b))
    marker=' wire stage_active,stage_select,stage_complete,stage_ready,stage_valid;'
    change(marker,marker+'''
 wire stage_busy,stage_tag_match;
 reg [71:0] stage_descriptor_fault=0;
 integer tag_retries=0;
''')
    old=''' dorado_r_interior_read_stage interior_stage(host_clk,cp_reset,stage_start,interior_needed,stage_accept,stage_release,
  port_address,interior_address,port_select,interior_select,physical_read,
  physical_address,physical_select,stage_active,stage_select,stage_valid,stage_ready,stage_before,stage_actual_capture);'''
    new=''' dorado_r_interior_read_tagged_stage interior_stage(host_clk,cp_reset,stage_start,interior_needed,stage_accept,stage_release,
  port_address^stage_descriptor_fault[71:40],interior_address^stage_descriptor_fault[39:8],
  port_select^stage_descriptor_fault[7:4],interior_select^stage_descriptor_fault[3:0],physical_read,
  physical_address,physical_select,stage_active,stage_select,stage_valid,stage_ready,stage_before,stage_actual_capture,stage_busy,stage_tag_match);'''
    change(old,new)
    change('if(stage_active)begin\n      advance=0;stage_release=1;','if(stage_busy)begin\n      advance=0;stage_release=1;')
    if probe:
        marker='''      // Hold the physical read pending for three host clocks. No guest edge.'''
        added='''      // Deliberately change only the pending descriptor, not the committed
      // machine proposal/oracle. Prove stale buffers cannot authorize a commit.
      for(integer epoch=0;epoch<2;epoch++)for(integer bit_no=0;bit_no<72;bit_no++)begin
       if(epoch==1)begin
        stage_accept=1;host_clk=1;#100;host_clk=0;#100;stage_accept=0;
        if(!stage_valid || !stage_ready)$fatal(1,"INTERIOR_TAG fixture never reached DONE");
       end
       stage_descriptor_fault=72'b1<<bit_no;#100;
       if(stage_tag_match || stage_valid || stage_ready || stage_active || stage_select || !stage_busy)
        $fatal(1,"INTERIOR_TAG stale request accepted epoch=%0d bit=%0d",epoch,bit_no);
       // An ack and finish arriving with the mismatch must not publish it.
       stage_accept=1;stage_release=1;host_clk=1;#100;host_clk=0;#100;stage_accept=0;stage_release=0;
       if(stage_busy || stage_valid || stage_ready)$fatal(1,"INTERIOR_TAG failed to discard request");
       if(stored_q!==saved_owner || memory_image()!==saved_memory)
        $fatal(1,"INTERIOR_TAG invalid request changed owner/RAM");
       stage_descriptor_fault=0;#100;stage_start=1;host_clk=1;#100;host_clk=0;#100;stage_start=0;
       if(!stage_active || !stage_select || stage_ready || stage_valid || !stage_tag_match)
        $fatal(1,"INTERIOR_TAG retry failed epoch=%0d bit=%0d",epoch,bit_no);
       tag_retries++;
      end
'''
        change(marker,added+marker)
        marker='   $display("INTERIOR_READ_COMPONENT_PASS reads=%0d",interior_reads);'
        assert marker in raw
        change(marker,'''   if(tag_retries!=144)$fatal(1,"INTERIOR_TAG coverage=%0d",tag_retries);
   $display("INTERIOR_TAG_FULL_PROCESSOR_PASS changes=%0d READ=72 DONE=72",tag_retries);
'''+marker)
    restored=raw
    for a,b in reversed(changes):restored=restored.replace(b,a,1)
    assert restored==original
    return raw
