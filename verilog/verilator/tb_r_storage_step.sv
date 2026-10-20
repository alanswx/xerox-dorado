`timescale 1ns/1ps
`default_nettype none
module tb_r_storage_step;
 localparam [151:0] SEED_PINS=152'h128|(152'h1<<66)|(152'h1<<67)|(152'h7fff<<50);
 reg host_clk=0,advance=1,fixture_load=0;
 reg [595:0] state_q=0,fixture_state=0;
 reg [9215:0] fixture_memory=0;
 reg before_clock=1,after_clock=1,reference_clock=1;
 reg [151:0] pins_before=SEED_PINS,pins_after=SEED_PINS,reference_control=SEED_PINS,selected=SEED_PINS;
 reg [31:0] address=0,reference_address=0;
 reg [3:0] select_n=15,reference_select=15;
 wire [595:0] next_q,reference;
 wire [32:0] events,repeated,ref_clocks;
 wire unsettled;
 wire [1:0] twrite,tlatch,tvisible,tbypass,rread,rvisible,rm_flags_n,rwrite,raw_parity;
 wire [1:0] rtwrite,rtlatch,rtvisible,rtbypass,rrread,rrvisible,rrm_flags_n,rhold,rparity_clocks;
 wire [3:0] parity_ok,faults,r_flags,ref_parity_ok,ref_faults,ref_r_flags;
 wire [15:0] raw_data,ref_raw_data;
 wire [1:0] ref_raw_parity,generated_parity;
 wire [9215:0] ref_memory;
 wire [8:0] ref_flags;wire [3:0] ref_next;
 wire [1:0] ref_rwrite={original.l.j05.p13,original.h.j05.p13};
 wire [151:0] reference_pins={reference_control[151:145],ref_raw_parity,ref_raw_data,reference_control[126:0]};
 dorado_r_storage_step dut(host_clk,advance,state_q,before_clock,after_clock,pins_before,pins_after,
   address,select_n,next_q,events,repeated,unsettled,twrite,tlatch,tvisible,tbypass,
   parity_ok,faults,rread,rvisible,r_flags,rm_flags_n,rwrite,raw_data,raw_parity);
 r_flags_reference original(fixture_load,fixture_state,reference_clock,reference_pins,
   reference,ref_clocks,ref_flags,ref_next,rtwrite,rtlatch,rhold,rtvisible,rtbypass,
   rparity_clocks,ref_parity_ok,ref_faults,rrread,rrvisible,ref_r_flags,rrm_flags_n);
 r_file_ports_reference original_files(fixture_load,fixture_memory,1'b1,ref_rwrite,
   reference_address,reference_select,reference[555:540],ref_raw_data,ref_raw_parity,ref_memory,generated_parity);
 integer checks=0,writes=0,read_updates=0,held=0,address_updates=0,fault_observations=0,directed=0;
 integer edge_counts[0:32],totals[0:32];reg monitor=0;
 reg [255:0] bank_addresses[0:3];reg [15:0] select_masks=0;reg [7:0] lc_seen=0;
    realtime data_changed[0:7],clock_arrived[0:7];
    realtime minimum_setup=1.0e9,minimum_hold=1.0e9;
    integer setup_samples=0,hold_samples=0;
    reg [7:0] data_valid=0,clock_valid=0;
    wire [7:0] capture_data={original.l.g16.p11,original.l.g16.p10,original.l.g16.p9,original.l.g16.p7,
                             original.h.g16.p11,original.h.g16.p10,original.h.g16.p9,original.h.g16.p7};
    for(genvar input_bit=0;input_bit<8;input_bit=input_bit+1) begin: check_g16_arrivals
        always @(capture_data[input_bit]) begin
            if(monitor && clock_valid[input_bit]) begin
                if($realtime-clock_arrived[input_bit]<minimum_hold) minimum_hold=$realtime-clock_arrived[input_bit];
                hold_samples=hold_samples+1;
`ifdef PARC_PARITY_TIMING
                if($realtime-clock_arrived[input_bit]<1.1-0.0001) $fatal(1,"PARC g16 hold violation pin=%0d margin=%0.3f",input_bit,$realtime-clock_arrived[input_bit]);
`endif
            end
            data_changed[input_bit]=$realtime;data_valid[input_bit]=1;
        end
        always @(posedge rparity_clocks[input_bit/4]) begin
            if(monitor && data_valid[input_bit]) begin
                if($realtime-data_changed[input_bit]<minimum_setup) minimum_setup=$realtime-data_changed[input_bit];
                setup_samples=setup_samples+1;
`ifdef PARC_PARITY_TIMING
                if($realtime-data_changed[input_bit]<1.7-0.0001) $fatal(1,"PARC g16 setup violation pin=%0d margin=%0.3f",input_bit,$realtime-data_changed[input_bit]);
`endif
            end
            clock_arrived[input_bit]=$realtime;clock_valid[input_bit]=1;
        end
    end

 for(genvar i=0;i<33;i=i+1) begin
   initial edge_counts[i]=0;
   always @(posedge ref_clocks[i]) if(monitor) edge_counts[i]=edge_counts[i]+1;
 end
 function automatic [595:0] normalized(input [595:0] value);
   reg [595:0] q;
   begin
     q=value;
     if(q[21]) begin q[14]=1;q[15]=1;q[16]=0;q[17]=0;end
     if(q[6] || !q[7]) begin q[23]=0;q[24]=0;q[25]=0;q[26]=0;q[27]=0;q[28]=0;q[29]=1;q[31]=0;end
     q[30]=1;normalized=q;
   end
 endfunction
 task automatic seed(input [595:0] value);
   monitor=0;fixture_load=0;reference_clock=1;reference_control=SEED_PINS|152'h4;
   #40;fixture_state=0;fixture_load=1;#40;fixture_load=0;
   #40;fixture_state=normalized(value);fixture_load=1;#40;fixture_load=0;
   #40;fixture_load=1;#40;fixture_load=0;#40;reference_control=SEED_PINS;#40;
   if(reference!==normalized(value)) $fatal(1,"R storage seed mismatch");
   state_q=normalized(value);before_clock=1;after_clock=1;
   pins_before=SEED_PINS;pins_after=SEED_PINS;
   for(integer i=0;i<8;i=i+1) data_changed[i]=$realtime;
   data_valid=8'hff;clock_valid=0;monitor=1;
 endtask
 task automatic event_step(input clock_next,input [151:0] controls);
   reg [595:0] expected;
   reg [32:0] predicted;
   reg [1:0] expected_rw;
   integer previous[0:32];
   after_clock=clock_next;pins_after=controls;#40;
   expected=next_q;predicted=events;expected_rw=rwrite;
   if(unsettled || repeated) $fatal(1,"R storage solver unsettled");
   if((rwrite|rread)!=3) $fatal(1,"R storage write/read windows overlap");
   for(integer i=0;i<33;i=i+1) previous[i]=edge_counts[i];
   reference_clock=clock_next;reference_control=controls;#40;
   if(ref_rwrite!==expected_rw) $fatal(1,"R storage write gate mismatch original=%b candidate=%b",ref_rwrite,expected_rw);
   if(reference!==expected) $fatal(1,"R storage state mismatch clock=%b->%b q=%h candidate=%h original=%h",before_clock,clock_next,state_q,expected,reference);
   if({ref_parity_ok,ref_faults,rrm_flags_n}!=={parity_ok,faults,rm_flags_n}) $fatal(1,"R storage flag/parity mismatch");
   for(integer i=0;i<33;i=i+1) begin
     if(edge_counts[i]-previous[i]!=int'(predicted[i])) $fatal(1,"R storage clock mismatch channel=%0d",i);
     totals[i]=totals[i]+edge_counts[i]-previous[i];
   end
   // Commit RAM at the resolved write window. The original reference writes
   // throughout its level-sensitive window. Read latches must be closed here.
   host_clk=1;#40;host_clk=0;#40;
   if({raw_parity,raw_data}!=={ref_raw_parity,ref_raw_data}) $fatal(1,"R storage RAM output mismatch");
   if(next_q!==expected) $fatal(1,"R storage commit changed a sampled source epoch");
   for(integer h=0;h<2;h=h+1) begin
     if(!rwrite[h]) writes=writes+1;
     if(!rread[h]) read_updates=read_updates+1;
   end
   if(faults!=0) fault_observations=fault_observations+1;
   state_q=expected;before_clock=clock_next;pins_before=controls;
   checks=checks+1;lc_seen[controls[11:9]]=1;
 endtask
 task automatic cycle(input integer n);
   for(integer i=0;i<n;i=i+1) event_step(!before_clock,selected);
 endtask
 task automatic change_ports(input [31:0] a,input [3:0] s);
   if(!before_clock) event_step(1,selected);
   if(rwrite!=3 || ref_rwrite!=3) $fatal(1,"R storage address changed during write");
   address=a;select_n=s;#40;
   reference_address=a;reference_select=s;#40;
   if(reference!==state_q || next_q!==state_q) $fatal(1,"R storage address changed closed core state");
   if({raw_parity,raw_data}!=={ref_raw_parity,ref_raw_data}) $fatal(1,"R storage address read mismatch");
   for(integer b=0;b<4;b=b+1) bank_addresses[b][a[8*b+:8]]=1;
   select_masks[s]=1;address_updates=address_updates+1;
 endtask
 reg [595:0] initial_state;
 reg [8:0] word0,word1,word2,word3;
 reg [31:0] rng=32'h13f856b1;
 initial begin
   for(integer i=0;i<33;i=i+1) totals[i]=0;
   for(integer b=0;b<4;b=b+1) bank_addresses[b]=0;
   // Private simulator fixture initialization only. Production has no array
   // scan/init port; the separate port gate verifies initialization by writes.
   for(integer i=0;i<256;i=i+1) begin
     word0={1'b0,8'(i*23+11)};word0[8]=~^word0[7:0];
     word1={1'b0,8'(i*37+47)};word1[8]=~^word1[7:0];
     word2={1'b0,8'(i*17+71)};word2[8]=~^word2[7:0];
     word3={1'b0,8'(i*43+99)};word3[8]=~^word3[7:0];
     dut.files.rm_h[i]=word0;dut.files.stk_h[i]=word1;
     dut.files.rm_l[i]=word2;dut.files.stk_l[i]=word3;
     fixture_memory[4*i+:4]=word0[3:0];fixture_memory[1024+4*i+:4]=word0[7:4];
     fixture_memory[2048+4*i+:4]=word1[3:0];fixture_memory[3072+4*i+:4]=word1[7:4];
     fixture_memory[4096+i]=word0[8];fixture_memory[4352+i]=word1[8];
     fixture_memory[4608+4*i+:4]=word2[3:0];fixture_memory[5632+4*i+:4]=word2[7:4];
     fixture_memory[6656+4*i+:4]=word3[3:0];fixture_memory[7680+4*i+:4]=word3[7:4];
     fixture_memory[8704+i]=word2[8];fixture_memory[8960+i]=word3[8];
   end
   initial_state=7|(596'h1<<38)|(596'h1<<54)|(596'h1<<75)|(596'h1<<93)|(596'h1<<99);
   initial_state[43:40]=15;initial_state[47:44]=15;initial_state[59:56]=15;initial_state[63:60]=15;
   initial_state[83:80]=15;initial_state[87:84]=15;
   for(integer slot=0;slot<16;slot=slot+1) begin
     initial_state[404+4*slot]=1;initial_state[468+4*slot]=1;
   end
   initial_state[535:532]=4'hc;initial_state[539:536]=4'hc;
   seed(initial_state);selected=SEED_PINS;selected[19:0]=20'h178;cycle(64);
   for(integer i=0;i<256;i=i+1) begin
     change_ports({8'(i^8'ha5),8'(i+17),8'(255-i),8'(i)},4'(i));
     selected[34:20]=i%16 ? 15'h1<<((i%16)-1) : 0;
     selected[11:9]=3'(i);selected[91:76]=16'(i*173+7);selected[107:92]=16'(i*317+19);
     selected[151:148]=4'(i>>3);selected[126:118]=9'(i*31);
     cycle(32);
   end
   // Memory Hold and task repetition while physical port addresses stay put.
   for(integer i=0;i<256;i=i+1) begin
     if(!before_clock) event_step(1,selected);
     selected[68]=i[0];selected[69]=i[1];selected[11:9]=3'(i>>2);
     selected[91:76]=16'(i*51);selected[107:92]=16'(i*79);
     cycle(16);if(selected[68]) held=held+1;
   end
   if(!before_clock) event_step(1,selected);
   selected[68]=0;selected[69]=0;cycle(16);
   // Exercise BOTH write-data sources in EVERY physical bank independently
   // of select-mask/LC correlations in the address sweep above.
   for(integer bank=0;bank<4;bank=bank+1)
     for(integer mode=0;mode<2;mode=mode+1) begin
       change_ports({4{8'(bank*37+mode*19)}},~(4'b1<<bank));
       selected[11:9]=mode ? 4 : 2;
       selected[91:76]=16'hc35a;selected[107:92]=16'h39e7;
       selected[151:148]=0;cycle(32);
       if(raw_data!==(bank<2 ? (mode ? 16'h00e7 : 16'h005a) : (mode ? 16'h3900 : 16'hc300)))
         $fatal(1,"R storage directed write source failed bank=%0d mode=%0d got=%h",bank,mode,raw_data);
       directed=directed+1;
     end
   for(integer i=0;i<1024;i=i+1) begin
     if(!before_clock) event_step(1,selected);
     rng=rng^(rng<<13);rng=rng^(rng>>17);rng=rng^(rng<<5);
     selected[11:9]=rng[2:0];selected[91:76]=rng[15:0];selected[107:92]=rng[31:16];
     selected[151:148]=rng[7:4];cycle(8);
   end
   for(integer b=0;b<4;b=b+1) if(bank_addresses[b]!=={256{1'b1}}) $fatal(1,"R storage address coverage incomplete");
   if(writes<100 || read_updates<100 || held!=128 || directed!=8 || select_masks!=16'hffff || lc_seen!=8'hff)
     $fatal(1,"R storage coverage incomplete");
   if(setup_samples<1000 || hold_samples<100) $fatal(1,"R storage arrival coverage incomplete");
   $display("R_STORAGE_TIMING setup_min_ns=%0.3f hold_min_ns=%0.3f setup_samples=%0d hold_samples=%0d",minimum_setup,minimum_hold,setup_samples,hold_samples);
   $display("R_STORAGE_STEP PASS checks=%0d writes=%0d reads=%0d addresses=%0d held=%0d directed=%0d masks=%h LC=%h faults=%0d",
     checks,writes,read_updates,address_updates,held,directed,select_masks,lc_seen,fault_observations);
   $finish;
 end
endmodule
`default_nettype wire
