`timescale 1ns/1ps
`default_nettype none
module tb_r_front_step;
 localparam [235:0] SEED=(236'h1<<143)|(236'h1<<144)|236'h128|
   (236'h1<<66)|(236'h1<<67)|(236'h7fff<<50)|(236'h1<<177)|(236'h1<<178)|
   (236'h1<<186)|(236'h1<<187)|(236'h1<<196)|(236'h1<<197)|(236'h7<<202)|(236'h1<<205)|(236'h1<<209);
 reg [735:0] q=0,fixture=0;
 reg cb=1,ca=1,load=0,root=1,monitor=0;
 reg [235:0] pb=SEED,pa=SEED,rp=SEED;
 reg host_clk=0,advance=1;
 reg [9215:0] fixture_memory=0;
 wire [9215:0] reference_memory;
 wire [15:0] raw_data,ref_raw_data;
 wire [1:0] raw_parity,ref_raw_parity,generated_parity,rwrite;
 wire [31:0] port_address;
 wire [3:0] port_select;
 wire [1:0] ref_rwrite={original.l.j05.p13,original.h.j05.p13};
 wire [235:0] reference_pins={rp[235:145],ref_raw_parity,ref_raw_data,rp[126:0]};
 wire [735:0] n,reference;
 wire [56:0] events,repeated,clocks;
 wire unsettled;
 wire [1:0] wn,ln,tln,bp,rr,rv,rf_n,forwarded;
 wire [3:0] ok,fault,rf,select_n;
 wire [31:0] address;
 wire [1:0] rwn,rln,rtln,rbp,rrr,rrv,rrf_n,rforwarded,hold_clocks,parity_clocks;
 wire [3:0] rok,rfault,rrf,rselect,next_raw;
 wire [31:0] raddress;
 wire [8:0] flags;
 wire [201:0] source_pins,ref_source_pins;wire [7:0] fa_n,ref_fa_n;
 dorado_r_front_step dut(host_clk,advance,q,cb,ca,pb,pa,n,events,repeated,unsettled,
   wn,ln,tln,bp,ok,fault,rr,rv,rf,rf_n,address,select_n,forwarded,rwrite,
   port_address,port_select,raw_data,raw_parity,source_pins,fa_n);
 r_front_reference original(load,fixture,root,reference_pins,reference,clocks,flags,next_raw,
   rwn,rln,hold_clocks,rtln,rbp,parity_clocks,rok,rfault,rrr,rrv,rrf,rrf_n,
   raddress,rselect,rforwarded,ref_source_pins,ref_fa_n,);
 r_file_ports_reference original_files(load,fixture_memory,1'b1,ref_rwrite,
   raddress,rselect,reference[555:540],ref_raw_data,ref_raw_parity,reference_memory,generated_parity);
 function automatic [9215:0] memory_image();
   reg [9215:0] image;
   begin
     for(integer i=0;i<256;i=i+1) begin
       image[4*i+:4]=dut.child.files.rm_h[i][3:0];image[1024+4*i+:4]=dut.child.files.rm_h[i][7:4];
       image[2048+4*i+:4]=dut.child.files.stk_h[i][3:0];image[3072+4*i+:4]=dut.child.files.stk_h[i][7:4];
       image[4096+i]=dut.child.files.rm_h[i][8];image[4352+i]=dut.child.files.stk_h[i][8];
       image[4608+4*i+:4]=dut.child.files.rm_l[i][3:0];image[5632+4*i+:4]=dut.child.files.rm_l[i][7:4];
       image[6656+4*i+:4]=dut.child.files.stk_l[i][3:0];image[7680+4*i+:4]=dut.child.files.stk_l[i][7:4];
       image[8704+i]=dut.child.files.rm_l[i][8];image[8960+i]=dut.child.files.stk_l[i][8];
     end
     memory_image=image;
   end
 endfunction
 integer writes=0,reads=0,capture_epoch_changes=0;reg [99:0] front_changed=0;
 integer write_windows[0:1],write_address_changes[0:1],write_select_changes[0:1];
 reg [1:0] write_open=0;
 reg [15:0] write_address[0:1];
 reg [1:0] write_select[0:1];
 for(genvar half=0;half<2;half=half+1) begin: monitor_write_arrival
   initial begin write_windows[half]=0;write_address_changes[half]=0;write_select_changes[half]=0;end
   always @(ref_rwrite[half] or raddress[16*half+:16] or rselect[2*half+:2]) begin
     if(!monitor) write_open[half]=0;
     else if(!ref_rwrite[half]) begin
       if(write_open[half] && write_address[half]!=raddress[16*half+:16]) write_address_changes[half]=write_address_changes[half]+1;
       if(write_open[half] && write_select[half]!=rselect[2*half+:2]) write_select_changes[half]=write_select_changes[half]+1;
       if(!write_open[half]) write_windows[half]=write_windows[half]+1;
       write_open[half]=1;write_address[half]=raddress[16*half+:16];write_select[half]=rselect[2*half+:2];
     end else write_open[half]=0;
   end
 end
 wire [3:0] rb_address={original.l.g15.p10,original.l.g15.p9,original.l.g15.p7,original.l.g15.p6};
 reg rb_open=0;reg [3:0] rb_previous=0;
 reg [15:0] rb_seen=0;integer rb_windows=0,rb_address_moves=0;
 always @(original.l.g15.p13 or rb_address) begin
   if(!monitor) rb_open=0;
   else if(!original.l.g15.p13) begin
     if(rb_open && rb_previous!=rb_address) rb_address_moves=rb_address_moves+1;
     if(!rb_open) rb_windows=rb_windows+1;
     rb_open=1;rb_previous=rb_address;rb_seen[rb_address]=1;
   end else rb_open=0;
 end
 integer counts[0:56],totals[0:56];
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
   always @(posedge parity_clocks[input_bit/4]) begin
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
 for(genvar i=0;i<57;i=i+1) begin: count_edges
   initial begin counts[i]=0;totals[i]=0;end
   always @(posedge clocks[i]) if(monitor) counts[i]=counts[i]+1;
 end
 integer checks=0,adr_moves=0,select_moves=0,forward_moves=0,transparent=0;
 reg [255:0] seen_h=0,seen_l=0,seen_hs=0,seen_ls=0;
 reg [15:0] select_seen=0;
 reg [7:0] lc_seen=0;
 reg [3:0] drivers_seen=0;
 reg [31:0] dut_address_before=0;
 reg [3:0] dut_select_before=0;
 reg [1:0] dut_forward_before=0;
 task automatic event_step(input next_root,input [235:0] pins);
   reg [735:0] expected;
   reg [56:0] predicted;
   reg [31:0] ea;
   reg [3:0] es;
   reg [1:0] ef;
   reg [201:0] expected_source;reg [7:0] expected_fa;
   reg [19:0] eo;reg [1:0] erw;reg [9215:0] old_memory,actual_memory;
   integer previous[0:56];
   begin
     ca=next_root;pa=pins;
     #100;
     expected=n;predicted=events;ea=address;es=select_n;ef=forwarded;
     eo={wn,ln,tln,bp,ok,fault,rr,rv};erw=rwrite;
     expected_source=source_pins;expected_fa=fa_n;
     if((rwrite|rr)!=2'b11) $fatal(1,"R front write/read windows overlap");
     if((predicted[13]&&!rwrite[0])||(predicted[18]&&!rwrite[1]))
       $fatal(1,"R front capture epoch overlaps write");
     if((predicted[13]&&port_address[15:0]!=address[15:0])||
        (predicted[18]&&port_address[31:16]!=address[31:16])) capture_epoch_changes=capture_epoch_changes+1;
     if((predicted[13]&&raw_parity[0]!==ref_raw_parity[0])||
        (predicted[18]&&raw_parity[1]!==ref_raw_parity[1]))
       $fatal(1,"R front capture port lost old parity epoch");
     if(unsettled||repeated) $fatal(1,"R front solver unsettled/repeated");
     for(integer i=0;i<57;i=i+1) previous[i]=counts[i];
     {root,rp}={next_root,pins};
     #100;
     if(ref_rwrite!==erw) $fatal(1,"R front write gate mismatch");
     if(reference!==expected) begin
       for(integer i=0;i<736;i=i+1) if(reference[i]!==expected[i])
         $display("R_FRONT_DIFF bit=%0d old=%b expected=%b original=%b",i,q[i],expected[i],reference[i]);
       $fatal(1,"R front state mismatch check=%0d roots=%b->%b pins=%h q=%h expected=%h original=%h",checks,cb,ca,pins,q,expected,reference);
     end
     // Raw R memory data/parity are supplied internally by the functional
     // ports, so their external seam bits are excluded here. All newly
     // resolved processor sources must agree with the physical graph.
     if({expected_source[201:145],expected_source[126:0],expected_fa} !==
        {ref_source_pins[201:145],ref_source_pins[126:0],ref_fa_n})
       $fatal(1,"R front resolved source mismatch check=%0d",checks);
     if(raddress!==ea||rselect!==es||rforwarded!==ef)
       $fatal(1,"R front output mismatch check=%0d address=%h/%h selects=%h/%h bypass=%b/%b",checks,ea,raddress,es,rselect,ef,rforwarded);
     if({rwn,rln,rtln,rbp,rok,rfault,rrr,rrv}!==eo || rrf!==rf || rrf_n!==rf_n)
       $fatal(1,"R front operand output mismatch check=%0d",checks);
     for(integer i=0;i<57;i=i+1) begin
       if(counts[i]-previous[i]!=int'(predicted[i]))
         $fatal(1,"R front clock mismatch check=%0d channel=%0d actual=%0d expected=%0d",checks,i,counts[i]-previous[i],predicted[i]);
       totals[i]=totals[i]+counts[i]-previous[i];
     end
     if(q[635:632]!=expected[635:632]) transparent=transparent+1;
     if(ea!=raddress) $fatal(1,"R front unstable settled output");
     if(ea!=dut_address_before) adr_moves=adr_moves+1;
     if(es!=dut_select_before) select_moves=select_moves+1;
     if(ef!=dut_forward_before) forward_moves=forward_moves+1;
     dut_address_before=ea;dut_select_before=es;dut_forward_before=ef;
     seen_h[ea[7:0]]=1;seen_hs[ea[15:8]]=1;
     seen_l[ea[23:16]]=1;seen_ls[ea[31:24]]=1;select_seen[es]=1;
     lc_seen[pins[11:9]]=1;
     drivers_seen=drivers_seen|{original.l.i19.p3,original.l.i11.p14|original.l.i11.p15,
                                |{original.l.h13.p1,original.l.h13.p2,original.l.h13.p14,original.l.h13.p15},
                                |{original.l.h12.p1,original.l.h12.p2,original.l.h12.p14,original.l.h12.p15}};
     // The reference writes throughout the original level-sensitive window.
     // A closed read window makes the candidate's end-of-window RAM commit
     // unobservable to the already captured processor state.
     old_memory=memory_image();advance=0;host_clk=1;#100;host_clk=0;#100;
     if(memory_image()!==old_memory) $fatal(1,"R front advance hold changed RAM");
     advance=1;host_clk=1;#100;host_clk=0;#100;
     actual_memory=memory_image();
     if(actual_memory!==reference_memory) begin
       for(integer bit_no=0;bit_no<9216;bit_no=bit_no+1) if(actual_memory[bit_no]!==reference_memory[bit_no])
         $display("R_FRONT_RAM_DIFF bit=%0d actual=%b original=%b",bit_no,actual_memory[bit_no],reference_memory[bit_no]);
       $fatal(1,"R front storage mismatch check=%0d root=%b->%b address=%h port=%h select=%h port_select=%h write_n=%b",checks,cb,ca,address,port_address,select_n,port_select,rwrite);
     end
     if(n!==expected) $fatal(1,"R front commit changed sampled source epoch");
     for(integer half=0;half<2;half=half+1) begin
       if(!rwrite[half]) writes=writes+1;
       if(!rr[half]) reads=reads+1;
     end
     front_changed=front_changed|(q[735:636]^expected[735:636]);q=expected;cb=next_root;pb=pins;#100;
     if(n!==expected) $fatal(1,"R front state commit changed settled Q check=%0d",checks);
     if({raw_parity,raw_data}!=={ref_raw_parity,ref_raw_data}) $fatal(1,"R front settled port mismatch");
     checks=checks+1;
     if(checks%2048==0) $display("R_FRONT_PROGRESS checks=%0d",checks);
   end
 endtask
 reg [31:0] rng=32'hd04ad042;
 function automatic [31:0] random_next(input [31:0] v);
   reg [31:0] x;
   begin x=v^(v<<13);x=x^(x>>17);random_next=x^(x<<5);end
 endfunction
 reg [235:0] p;
 reg [8:0] word0,word1,word2,word3;
 integer iterations=2048;
 initial begin
   if($value$plusargs("iterations=%d",iterations)) $display("R_FRONT_ITERATIONS %0d",iterations);
   for(integer i=0;i<256;i=i+1) begin
     word0={1'b0,8'(i*23+11)};word0[8]=~^word0[7:0];
     word1={1'b0,8'(i*37+47)};word1[8]=~^word1[7:0];
     word2={1'b0,8'(i*17+71)};word2[8]=~^word2[7:0];
     word3={1'b0,8'(i*43+99)};word3[8]=~^word3[7:0];
     dut.child.files.rm_h[i]=word0;dut.child.files.stk_h[i]=word1;
     dut.child.files.rm_l[i]=word2;dut.child.files.stk_l[i]=word3;
   end
   fixture_memory=memory_image();
   #100;load=1;#100;load=0;#100;
   fixture=7|(636'h1<<38)|(636'h1<<54)|(636'h1<<75)|(636'h1<<93)|(636'h1<<99);
   fixture[43:40]=15;fixture[47:44]=15;fixture[59:56]=15;fixture[63:60]=15;
   fixture[83:80]=15;fixture[87:84]=15;fixture[30]=1;fixture[29]=1;
   load=1;#100;load=0;#100;load=1;#100;load=0;#100;
   if(reference!==fixture) $fatal(1,"R front fixture mismatch");
   q=fixture;monitor=1;p=SEED;p[19:0]=20'h178;
   for(integer bit_no=0;bit_no<8;bit_no=bit_no+1) data_changed[bit_no]=$realtime;
   data_valid=8'hff;clock_valid=0;
   for(integer i=0;i<64;i=i+1) event_step(!cb,p);
   // Drive original maintenance inputs to select every task, then execute
   // real immediate RBase writes at both nibble extremes. No Q injection.
   for(integer pass_no=0;pass_no<2;pass_no=pass_no+1)
     for(integer task_no=0;task_no<16;task_no=task_no+1) begin
       if(!cb) event_step(1,p);
       p=SEED;p[8:0]=9'h178;p[0]=1;p[65]=1;
       p[70]=1;p[74:71]=0;event_step(1,p);
       p[70]=0;p[74:71]=4'(task_no);event_step(1,p);
       p[74:71]=0;event_step(1,p);p[0]=0;event_step(1,p);
       p[204:202]=3'b110; // ASEL=4, full FA decode (negative MSB-first).
       p[209]=0;p[19:12]=8'h80|(pass_no ? 8'h0f : 8'h00);
       event_step(1,p);
       for(integer edge_no=0;edge_no<16;edge_no= edge_no+1) event_step(!cb,p);
       if(task_no==15 && q[703:640] !== (pass_no ? 64'b0 : ~64'b0))
         $fatal(1,"R front directed RBase fill mismatch pass=%0d actual=%h",pass_no,q[703:640]);
     end
   // Select emulator task zero through real maintenance pins and load the
   // complete pointer from ALU through the documented StkP<- FF (0144B).
   p=SEED;p[8:0]=9'h178;p[0]=1;p[65]=1;p[70]=1;p[74:71]=0;event_step(1,p);
   p[70]=0;event_step(1,p);p[0]=0;event_step(1,p);
   p[204:202]=3'b110;p[209]=0;p[176]=1;p[19:12]=8'o144;
   for(integer value=0;value<256;value=value+1) begin
     for(integer bit_no=0;bit_no<8;bit_no=bit_no+1) p[227+bit_no]=(value>>(7-bit_no))&1;
     event_step(1,p);
     for(integer edge_no=0;edge_no<8;edge_no=edge_no+1) event_step(!cb,p);
     if({q[710],q[711],q[712],q[713],q[714],q[715],q[717],q[716]} !== 8'(value)) begin
       $display("STKP_DEBUG fa=%h task=%h emu=%b src=%h h18=%b j13ctl=%b alu=%b h20=%b",original.fa_n,q[67:64],q[630],original.source_pins,
         {original.l.h18.p4,original.l.h18.p5,original.l.h18.p6,original.l.h18.p7,original.l.h18.p10,original.l.h18.p12,original.l.h18.p13},
         {original.l.j13.p9,original.l.j13.p7,original.l.j13.p14},
         {original.l.j16.p13,original.l.j16.p3},
         {original.l.h20.p2,original.l.h20.p3,original.l.h20.p9,original.l.h20.p10});
       $fatal(1,"R front directed StkP load mismatch value=%h actual=%b",value,{q[710],q[711],q[712],q[713],q[714],q[715],q[717],q[716]});
     end
   end
   p=SEED;event_step(1,p);
   for(integer i=0;i<iterations;i=i+1) begin
     // External producer inputs settle separately from the root event.
     // Hold and StopTasks may change only with the root masking their gates.
     if(!cb) event_step(1,p);
     for(integer word_no=0;word_no<8;word_no=word_no+1) begin
       rng=random_next(rng);
       for(integer bit_no=0;bit_no<32;bit_no=bit_no+1)
         if(word_no*32+bit_no<236) p[word_no*32+bit_no]=rng[bit_no];
     end
     p[8:0]=9'h178|(9'(rng[1])<<1);p[75]=0;p[108]=0;
     if(p[70]) p[74:71]=0;
     event_step(1,p);event_step(0,p);
     // Probe transparent address/data stages with the root held low.
     rng=random_next(rng);p[175:152]=rng[23:0];
     rng=random_next(rng);p[201:188]=rng[13:0];
     event_step(0,p);event_step(1,p);
   end
   if(iterations<512) begin
     $display("R_FRONT_SMOKE PASS checks=%0d writes=%0d reads=%0d old_epoch_changes=%0d",checks,writes,reads,capture_epoch_changes);
     $finish;
   end else begin
   if(writes<10||reads<100||capture_epoch_changes<10||$countones(front_changed)!=100)
     $fatal(1,"R front incomplete coverage writes=%0d reads=%0d epochs=%0d changed=%0d",writes,reads,capture_epoch_changes,$countones(front_changed));
   if(rb_seen!=16'hffff || rb_address_moves) $fatal(1,"R front RBase write-window coverage/stability failure addresses=%h moves=%0d",rb_seen,rb_address_moves);
   $display("R_FRONT_COVERAGE front_changed=%0d lc=%h select=%h drivers=%h RBase_addresses=%h RBase_windows=%0d RBase_address_changes=%0d",$countones(front_changed),lc_seen,select_seen,drivers_seen,rb_seen,rb_windows,rb_address_moves);
   for(integer ch=0;ch<57;ch=ch+1) begin
     if(totals[ch]==0) $fatal(1,"R front unexercised clock channel=%0d",ch);
     $display("R_FRONT_CLOCK channel=%0d edges=%0d",ch,totals[ch]);
   end
   for(integer half=0;half<2;half=half+1) begin
     if(write_address_changes[half]||write_select_changes[half]) $fatal(1,"R front write-window address/select movement");
     $display("R_FRONT_WRITE_WINDOW half=%0d windows=%0d address_changes=%0d select_changes=%0d",half,write_windows[half],write_address_changes[half],write_select_changes[half]);
   end
   $display("R_FRONT_STORAGE writes=%0d reads=%0d old_epoch_changes=%0d changed_front_bits=%0d",writes,reads,capture_epoch_changes,$countones(front_changed));
   $display("R_FRONT_TIMING setup_min_ns=%0.3f hold_min_ns=%0.3f setup_samples=%0d hold_samples=%0d",minimum_setup,minimum_hold,setup_samples,hold_samples);
   $display("R_FRONT_STEP PASS checks=%0d address_changes=%0d select_changes=%0d bypass_changes=%0d transparent=%0d",checks,adr_moves,select_moves,forward_moves,transparent);
   $finish;
   end
 end
endmodule
`default_nettype wire
