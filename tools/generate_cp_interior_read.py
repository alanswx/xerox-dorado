#!/usr/bin/env python3
"""Expose a distinct PrBlock interior-port read and closing data snapshot.

Component experiment: the caller must time-multiplex its ONE physical SRAM,
retain the before data, and supply interior data before publishing any owner.
This does not implement that caller or qualify other CP/arrival histories.
Original current physical Q/data observations remain the BEFORE snapshot.
"""
from prove_cp_prefix_cache import module

PORTS='''input wire [17:0] interior_read_data,input wire interior_read_valid,
 output wire [31:0] interior_port_address,output wire [3:0] interior_port_select_n,
 output wire interior_read_needed,
 '''

VIEW=''' // Interior selection precedes ordinary closing task/MIR clock-to-Q.
 // Same four SRAM banks and address wires; no additional physical read port.
 wire interior_stk_n=(!held_q[48]|held_q[97]) &
   (held_q[48]|held_q[630]|cp_prblock_capture);
 assign interior_port_address=port_address;
 assign interior_port_select_n={interior_stk_n,!interior_stk_n,interior_stk_n,!interior_stk_n};
 assign interior_read_needed=(!clock_before || !clock_after) &&
   (cp_prblock_capture!=cq[97]) && (interior_port_select_n!=port_select_n);
 wire interior_closing=interior_read_needed && interior_read_valid;
 wire [15:0] closing_r_data={
   pins_before[151] ? pins_before[126:119]:interior_read_data[15:8],
   pins_before[149] ? {8{pins_before[118]}}:interior_read_data[7:0]};
 wire [843:0] capture_state_q;
 assign capture_state_q[843:591]=state_q[843:591];
 assign capture_state_q[590]=interior_closing && current_r_flag_clocks[2] && !r_flag_clocks_after[2] ? interior_read_data[15]:state_q[590];
 assign capture_state_q[589]=state_q[589];
 assign capture_state_q[588]=interior_closing && current_r_flag_clocks[0] && !r_flag_clocks_after[0] ? interior_read_data[0]:state_q[588];
 assign capture_state_q[587:572]=state_q[587:572];
 assign capture_state_q[571:564]=interior_closing && !current_read_n[1] && r_read_n[1] ? closing_r_data[15:8]:state_q[571:564];
 assign capture_state_q[563:556]=interior_closing && !current_read_n[0] && r_read_n[0] ? closing_r_data[7:0]:state_q[563:556];
 assign capture_state_q[555:0]=state_q[555:0];
 assign captured_read_next={
   interior_closing && !current_read_n[1] && r_read_n[1] ? closing_r_data[15:8]:closed_read_next[31:24],
   interior_closing && !current_read_n[0] && r_read_n[0] ? closing_r_data[7:0]:closed_read_next[23:16],closed_read_next[15:0]};
 assign captured_r_flags_next={closed_r_flags_next[7:3],
   interior_closing && current_r_flag_clocks[2] && !r_flag_clocks_after[2] && !r_flag_sets_after[2] ? interior_read_data[15]:closed_r_flags_next[2],
   closed_r_flags_next[1],
   interior_closing && current_r_flag_clocks[0] && !r_flag_clocks_after[0] && !r_flag_sets_after[0] ? interior_read_data[0]:closed_r_flags_next[0]};
'''


def rewrite(raw):
    for name in ['dorado_r_front_control_step','dorado_r_front_pure_step']:
        old=module(raw,name);actual=old;changes=[]
        def change(a,b):
            nonlocal actual
            assert actual.count(a)==1,(name,a)
            actual=actual.replace(a,b,1);changes.append((a,b))
        marker='input wire [17:0] external_file_read,output wire [15:0] file_write_data'
        change(marker,PORTS+marker)
        marker=' wire [843:0] state_q,state_next;'
        change(marker,marker+'\n wire [31:0] captured_read_next;wire [7:0] captured_r_flags_next;')
        marker=' wire [3:0] child_parity_ok,child_faults;'
        change(marker,VIEW+" wire [15:0] child_file_read_data;wire [1:0] child_file_read_parity;\n"+
               " assign file_read_data=external_file_read[15:0];assign file_read_parity=external_file_read[17:16];\n"+marker)
        marker='port_address,port_select_n,file_read_data,file_read_parity,'
        change(marker,marker.replace('file_read_data,file_read_parity','child_file_read_data,child_file_read_parity'))
        marker='front_capture_pins,external_file_read,file_write_data);'
        change(marker,marker.replace('external_file_read','(interior_closing ? interior_read_data:external_file_read)'))
        marker='host_clk,cp_commit,state_q[751:0],clock_before,clock_after,before_child,after_child,'
        change(marker,marker.replace('state_q[751:0]','capture_state_q[751:0]'))
        marker='assign held_next={state_next[843:596],closed_r_flags_next,state_next[587:572],closed_read_next,'
        change(marker,marker.replace('closed_r_flags_next','captured_r_flags_next').replace('closed_read_next','captured_read_next'))
        restored=actual
        for a,b in reversed(changes):restored=restored.replace(b,a,1)
        assert restored==old
        raw=raw.replace(old,actual,1)
    return raw


def stage_bench(raw,mutation=None):
    """Time-multiplex the existing testbench SRAM, without an extra read port.

    A synthesizable host-clock sequencer remains a separate integration gate.
    Preserve the original before/after/clock/owner/RAM checker verbatim.
    """
    original=raw;changes=[]
    def change(a,b):
        nonlocal raw
        assert raw.count(a)==1,a
        raw=raw.replace(a,b,1);changes.append((a,b))
    marker=' wire [17:0] external_file_read;wire [15:0] file_write_data;'
    change(marker,marker+'''
 reg stage_active=0,stage_select=0,stage_complete=0;
 reg [17:0] stage_before=0,stage_capture=0;
 wire [17:0] physical_read;
 wire [31:0] interior_address,pure_interior_address;
 wire [3:0] interior_select,pure_interior_select;
 wire interior_needed,pure_interior_needed;
 wire [31:0] physical_address=stage_select ? interior_address:port_address;
 wire [3:0] physical_select=stage_select ? interior_select:port_select;
 assign external_file_read=stage_active ? stage_before:physical_read;
 integer interior_reads=0;
''')
    marker=' .external_file_read(external_file_read),'
    # Preserve the parent's spacing around named ports.
    assert raw.count(marker)==1
    change(marker,''' .interior_read_data(stage_capture),.interior_read_valid(stage_complete),
  .interior_port_address(pure_interior_address),.interior_port_select_n(pure_interior_select),
  .interior_read_needed(pure_interior_needed),
 '''+marker)
    marker='cp_reset,cp_ready,external_file_read,file_write_data);'
    change(marker,'cp_reset,cp_ready,stage_capture,stage_complete,interior_address,interior_select,interior_needed,external_file_read,file_write_data);')
    marker=''' dorado_r_file_ports files(host_clk,dut.cp_commit,rwrite,port_address,port_select,
  file_write_data,port_address,port_select,external_file_read[15:0],external_file_read[17:16]);'''
    change(marker,''' // Exactly ONE physical read port per existing bank, selected in time.
 dorado_r_file_ports files(host_clk,dut.cp_commit && !stage_select,rwrite,port_address,port_select,
  file_write_data,physical_address,physical_select,physical_read[15:0],physical_read[17:16]);''')
    marker='     #100;prepare_cp;#100;'
    stage='''
     if(interior_needed!==pure_interior_needed || interior_address!==pure_interior_address || interior_select!==pure_interior_select)
      $fatal(1,"INTERIOR_READ pure request mismatch check=%0d",checks);
     if(interior_needed)begin: service_interior
      reg [843:0] saved_owner;reg [9215:0] saved_memory;
      saved_owner=stored_q;saved_memory=memory_image();
      stage_before=physical_read;stage_active=1;stage_select=1;#100;
      if(stored_q!==saved_owner || memory_image()!==saved_memory)$fatal(1,"INTERIOR_READ changed architectural owner/RAM during read");
      stage_capture=physical_read;stage_select=0;stage_complete=1;#100;
      if(stored_q!==saved_owner || memory_image()!==saved_memory)$fatal(1,"INTERIOR_READ changed architectural owner/RAM before commit");
      $display("INTERIOR_READ check=%0d address=%h select=%h before=%h capture=%h",checks,interior_address,interior_select,stage_before,stage_capture);
      interior_reads++;
     end
'''
    if mutation=='stale-data':stage=stage.replace('stage_capture=physical_read;','stage_capture=stage_before;',1)
    if mutation=='missing-valid':stage=stage.replace('stage_complete=1;','stage_complete=0;',1)
    change(marker,marker+stage)
    marker='stored_q=expected;cb=next_root;pb=pins;pb[144:127]=0;#100;'
    change(marker,marker.replace(';#100;',';stage_active=0;stage_complete=0;#100;'))
    marker='   $display("FULL_PROCESSOR_CP_BLOCK_PASS checks=%0d Qs=844 RAM=9216 clocks=100",checks);'
    if marker in raw:
        change(marker,'''   if(interior_reads!=1)$fatal(1,"INTERIOR_READ expected one serviced request actual=%0d",interior_reads);
   $display("INTERIOR_READ_COMPONENT_PASS reads=%0d",interior_reads);
'''+marker)
    restored=raw
    for a,b in reversed(changes):restored=restored.replace(b,a,1)
    assert restored==original
    return raw


def sequential_stage_bench(raw,mutation=None):
    """Use the synthesizable host-clock stage with the original checker/SRAM."""
    raw=stage_bench(raw);original=raw;changes=[]
    def change(a,b):
        nonlocal raw
        assert raw.count(a)==1,a
        raw=raw.replace(a,b,1);changes.append((a,b))
    change(''' reg stage_active=0,stage_select=0,stage_complete=0;
 reg [17:0] stage_before=0,stage_capture=0;''',''' wire stage_active,stage_select,stage_complete,stage_ready,stage_valid;
 reg stage_start=0,stage_accept=0,stage_release=0;
 wire [17:0] stage_before,stage_capture,stage_actual_capture;''')
    change(''' wire [31:0] physical_address=stage_select ? interior_address:port_address;
 wire [3:0] physical_select=stage_select ? interior_select:port_select;''',''' wire [31:0] physical_address;wire [3:0] physical_select;
 dorado_r_interior_read_stage interior_stage(host_clk,cp_reset,stage_start,interior_needed,stage_accept,stage_release,
  port_address,interior_address,port_select,interior_select,physical_read,
  physical_address,physical_select,stage_active,stage_select,stage_valid,stage_ready,stage_before,stage_actual_capture);
 assign stage_capture=CAPTURE_DATA;
 assign stage_complete=CAPTURE_VALID;'''.replace('CAPTURE_DATA','stage_before' if mutation=='stale-data' else 'stage_actual_capture').replace('CAPTURE_VALID',"1'b0" if mutation=='missing-valid' else 'stage_valid'))
    old='''      stage_before=physical_read;stage_active=1;stage_select=1;#100;
      if(stored_q!==saved_owner || memory_image()!==saved_memory)$fatal(1,"INTERIOR_READ changed architectural owner/RAM during read");
      stage_capture=physical_read;stage_select=0;stage_complete=1;#100;
      if(stored_q!==saved_owner || memory_image()!==saved_memory)$fatal(1,"INTERIOR_READ changed architectural owner/RAM before commit");'''
    new='''      advance=0;stage_start=1;host_clk=1;#100;host_clk=0;#100;stage_start=0;
      if(!stage_active || !stage_select || stage_ready || stage_valid)$fatal(1,"INTERIOR_READ early stage ready");
      // Hold the physical read pending for three host clocks. No guest edge.
      repeat(3)begin
       host_clk=1;#100;host_clk=0;#100;
       if(stage_ready || stage_valid || !stage_select)$fatal(1,"INTERIOR_READ accepted an unacknowledged read");
       if(stored_q!==saved_owner || memory_image()!==saved_memory)$fatal(1,"INTERIOR_READ changed architectural owner/RAM during read");
      end
      stage_accept=1;host_clk=1;#100;host_clk=0;#100;stage_accept=0;
      if(!stage_ready || !stage_valid || stage_select)$fatal(1,"INTERIOR_READ not ready after acknowledged read");
      advance=1;
      if(stored_q!==saved_owner || memory_image()!==saved_memory)$fatal(1,"INTERIOR_READ changed architectural owner/RAM before commit");'''
    change(old,new)
    change('stage_active=0;stage_complete=0;#100;','''if(stage_active)begin
      advance=0;stage_release=1;host_clk=1;#100;host_clk=0;#100;stage_release=0;advance=1;
      if(stage_active || stage_valid)$fatal(1,"INTERIOR_READ release retained a stale result");
     end #100;''')
    marker='     if(interior_needed)begin: service_interior'
    change(marker,'''     if(!interior_needed && !stage_ready)$fatal(1,"INTERIOR_READ delayed an ordinary transaction");
'''+marker)
    restored=raw
    for a,b in reversed(changes):restored=restored.replace(b,a,1)
    assert restored==original
    return raw


def ram_sweep_bench(raw,cases):
    """Fresh full-processor fixtures with independent, parity-correct RAM data."""
    assert 1<=cases<=64
    helper='''
 task automatic fixture_word(input integer bank,addr,input [7:0] data);
  integer base,parity_base;
  begin
   base=(bank%2 ? 2048:0)+(bank>=2 ? 4608:0);
   parity_base=(bank%2 ? 4352:4096)+(bank>=2 ? 4608:0);
   fixture_memory[base+4*addr+:4]=data[3:0];
   fixture_memory[base+1024+4*addr+:4]=data[7:4];
   fixture_memory[parity_base+addr]=~^data;
  end
 endtask
 function automatic [7:0] edge_byte(input integer n);
  case(n%8)
   0:edge_byte=0;1:edge_byte=255;2:edge_byte=1;3:edge_byte=128;
   4:edge_byte=85;5:edge_byte=170;6:edge_byte=127;7:edge_byte=254;
  endcase
 endfunction
 task automatic seed_fixture_ram(input integer n);
  reg [31:0] memory_random;
  begin
   fixture_memory=0;memory_random=32'h73646f72 ^ (32'(n)*32'h9e3779b9);
   for(integer bank=0;bank<4;bank++)for(integer addr=0;addr<256;addr++)begin
    memory_random=random_next(memory_random);fixture_word(bank,addr,memory_random[7:0]);
   end
   // Cover all8x8 high-byte BEFORE/interior pairs, with independent low-byte
   // patterns. All other cells use deterministic independent random values.
   fixture_word(0,255,edge_byte(n));fixture_word(1,0,edge_byte(n/8));
   fixture_word(2,15,edge_byte((n/2)^5));fixture_word(3,0,edge_byte((n/4)^3));
  end
 endtask
'''
    marker=' reg [843:0] seed_fixture;integer fixture_index=0;';assert raw.count(marker)==1
    raw=raw.replace(marker,helper+marker,1)
    start=raw.index('   for(integer direction=0;direction<2;direction++)for(integer bank=4;bank<8;bank++)for(integer pattern=0;pattern<16;pattern++)begin')
    end=raw.index('   for(integer channel=0;channel<100;channel++)',start)
    program='''   for(integer memory_case=0;memory_case<CASES;memory_case++)begin
    monitor=0;seed_fixture_ram(memory_case);reload_fixture();
    for(integer i=0;i<64;i++)event_step(!cb,p);
    p[6]=1;event_step(1,p);repeat(64)begin event_step(1,p);event_step(0,p);end
    p[292]=1;p[291:289]=~3'(4);p[288:282]=7'(16>>1);p[340]=0;p[344]=0;
    $display("INTERIOR_RAM_CASE case=%0d fixture=%0d record=%0d",memory_case,fixture_index,checks);
    event_step(0,p);p[292]=0;event_step(1,p);p[292]=1;event_step(1,p);
   end
   if(fixture_index!=CASES || interior_reads!=CASES || !observed_cp_sets[17])
    $fatal(1,"INTERIOR_RAM incomplete fixtures=%0d reads=%0d",fixture_index,interior_reads);
   $display("FULL_PROCESSOR_CP_COLLISION_PASS checks=%0d RAM_cases=CASES Qs=844 RAM=9216 clocks=100 sets=%h",checks,observed_cp_sets);
'''.replace('CASES',str(cases))
    return raw[:start]+program+raw[end:]
