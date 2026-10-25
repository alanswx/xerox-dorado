#!/usr/bin/env python3
"""Use the synthesized publication/read/SRAM caller with the original oracle.

The caller owns828 registers plus the original401-pin/root history and one
SRAM. The original16 visible-R owners are modeled externally on the same commit.
Their actual native cell/clock hookup and the ordinary paired path remain open.
"""


def bench(raw,probe=False):
    original=raw;changes=[]
    def change(a,b):
        nonlocal raw
        assert raw.count(a)==1,a
        raw=raw.replace(a,b,1);changes.append((a,b))
    change(' reg [843:0] stored_q=0,fixture=0;wire [843:0] q;',' wire [843:0] stored_q;reg [843:0] fixture=0;wire [843:0] q;')
    change(' reg cb=1,ca=1,load=0,root=1,monitor=0;',' wire cb;reg ca=1,load=0,root=1,monitor=0;')
    change(' reg [400:0] pb=SEED,pa=SEED,rp=SEED;',' wire [400:0] pb;reg [400:0] pa=SEED,rp=SEED;')
    marker=' wire [17:0] external_file_read;wire [15:0] file_write_data;'
    change(marker,marker+'''
 wire owner_ready,owner_commit,stage_active,stage_select,stage_busy,stage_tag_match,stage_valid;
 reg read_accept=0,untrusted_write=0;
 reg [7:0] visible_h=0,visible_l=0;
 wire [17:0] stage_capture;
 wire [31:0] interior_address,pure_interior_address,physical_address;
 wire [3:0] interior_select,pure_interior_select,physical_select;
 wire interior_needed,pure_interior_needed;
 integer interior_reads=0;
''')
    marker=' .external_file_read(external_file_read),'
    change(marker,''' .interior_read_data(stage_capture),.interior_read_valid(stage_valid),
  .interior_port_address(pure_interior_address),.interior_port_select_n(pure_interior_select),
  .interior_read_needed(pure_interior_needed),
 '''+marker)
    change('dorado_r_front_control_step dut(host_clk,advance,stored_q,','dorado_r_front_control_step dut(host_clk,owner_commit,stored_q,')
    change('cp_reset,cp_ready,external_file_read,file_write_data);','cp_reset,cp_ready,stage_capture,stage_valid,interior_address,interior_select,interior_needed,external_file_read,file_write_data);')
    old=''' dorado_r_file_ports files(host_clk,dut.cp_commit,rwrite,port_address,port_select,
  file_write_data,port_address,port_select,external_file_read[15:0],external_file_read[17:16]);'''
    new=''' dorado_cp_read_caller owner(host_clk,cp_reset,advance,cp_ready,ca,pa,n,visible_h,visible_l,
  interior_needed,read_accept,port_address,interior_address,port_select,interior_select,
  untrusted_write ? 2'b00:rwrite,file_write_data,
  stored_q,pb,cb,owner_ready,owner_commit,external_file_read,stage_capture,stage_valid,
  stage_active,stage_select,stage_busy,stage_tag_match,physical_address,physical_select);
 // Original16 visible-R ownership stays external to the new828-bit owner.
 // The actual native transparent-cell hookup remains a separate gate.
 always @(posedge host_clk)if(owner_commit)begin
  visible_h<=n[579:572];visible_l<=n[587:580];
 end
 task automatic load_caller_fixture;
  begin
   owner.r_front_lower_q=fixture[571:0];owner.r_front_upper_q=fixture[843:588];
   owner.r_front_before=SEED;owner.r_front_clock_before=1;
   visible_h=fixture[579:572];visible_l=fixture[587:580];
   #1; // Settle the held-wire view before recording the initial fixture.
  end
 endtask'''
    change(old,new)
    # Keep the original fixture RAM bytes/physical storage implementation.
    import re
    before_paths=raw
    raw,count=re.subn(r'(?<![\w.])files\.', 'owner.files.',raw)
    assert count>=8
    # Record this single token-level path move for inverse verification below.
    changes.append((before_paths,raw))
    marker='stored_q=fixture;monitor=1;p=SEED;'
    change(marker,'load_caller_fixture;monitor=1;p=SEED;')
    if 'stored_q=fixture;cp_reset=0;'in raw:
        change('stored_q=fixture;cp_reset=0;','load_caller_fixture;cp_reset=0;')
        change('root=1;cb=1;ca=1;rp=SEED;pb=SEED;pa=SEED;','root=1;ca=1;rp=SEED;pa=SEED;')
    marker='     #100;prepare_cp;#100;'
    stage='''
     if(interior_needed!==pure_interior_needed || interior_address!==pure_interior_address || interior_select!==pure_interior_select)
      $fatal(1,"READ_OWNER pure request mismatch");
     if(interior_needed)begin: service_owned_read
      reg [843:0] saved_owner;reg [400:0] saved_before;reg saved_root;
      reg [9215:0] saved_memory;
      saved_owner=stored_q;saved_before=pb;saved_root=cb;saved_memory=memory_image();
      // Requested advance remains HIGH. The real RTL barrier, rather than
      // a testbench advance=0 convention, must prevent premature publication.
      advance=1;
      repeat(3)begin
       if(owner_ready || owner_commit || stage_valid)$fatal(1,"READ_OWNER prematurely ready");
       host_clk=1;#100;host_clk=0;#100;
       if(stored_q!==saved_owner || pb!==saved_before || cb!==saved_root || memory_image()!==saved_memory)begin
        for(integer bit_no=0;bit_no<844;bit_no++)if(stored_q[bit_no]!==saved_owner[bit_no])
         $display("READ_OWNER_EARLY_Q bit=%0d",bit_no);
        $fatal(1,"READ_OWNER pending read changed owner/context/RAM");
       end
      end
      read_accept=1;host_clk=1;#100;host_clk=0;#100;read_accept=0;
      if(!owner_ready || !stage_valid || stage_select)$fatal(1,"READ_OWNER ack did not release barrier");
      if(stored_q!==saved_owner || pb!==saved_before || cb!==saved_root || memory_image()!==saved_memory)
       $fatal(1,"READ_OWNER ack changed state before publication");
      $display("INTERIOR_READ check=%0d address=%h select=%h capture=%h",checks,interior_address,interior_select,stage_capture);
      interior_reads++;
     end else if(owner_ready!==cp_ready)$fatal(1,"READ_OWNER delayed an ordinary event");
'''
    if probe:
        stage=stage.replace('      advance=1;','      advance=1;untrusted_write=1;',1)
        stage=stage.replace('      read_accept=1;','      untrusted_write=0;read_accept=1;',1)
    change(marker,marker+stage)
    marker='stored_q=expected;cb=next_root;pb=pins;pb[144:127]=0;#100;'
    change(marker,'''if(stored_q!==expected || cb!==next_root || pb!==dut.source_after)
      $fatal(1,"READ_OWNER common publication mismatch");#100;''')
    marker='   $display("FULL_PROCESSOR_CP_BLOCK_PASS checks=%0d Qs=844 RAM=9216 clocks=100",checks);'
    if marker in raw:
        change(marker,'''   if(interior_reads!=1)$fatal(1,"READ_OWNER staged-read coverage=%0d",interior_reads);
   $display("READ_OWNER_FULL_PROCESSOR_PASS reads=%0d",interior_reads);
'''+marker)
    # Every original event assertion stays. Only the manual owner assignment
    # becomes a check of the real common hardware publication.
    restored=raw
    for a,b in reversed(changes):restored=restored.replace(b,a,1)
    assert restored==original
    return raw
