`timescale 1ns/1ps
`default_nettype none
module tb_r_address_step;
 localparam [201:0] SEED=(202'h1<<143)|(202'h1<<144)|202'h128|
   (202'h1<<66)|(202'h1<<67)|(202'h7fff<<50)|(202'h1<<177)|(202'h1<<178)|
   (202'h1<<186)|(202'h1<<187)|(202'h1<<196)|(202'h1<<197);
 reg [635:0] q=0,fixture=0;
 reg cb=1,ca=1,load=0,root=1,monitor=0;
 reg [201:0] pb=SEED,pa=SEED,rp=SEED;
 wire [635:0] n,reference;
 wire [44:0] events,repeated,clocks;
 wire unsettled;
 wire [1:0] wn,ln,tln,bp,rr,rv,rf_n,forwarded;
 wire [3:0] ok,fault,rf,select_n;
 wire [31:0] address;
 wire [1:0] rwn,rln,rtln,rbp,rrr,rrv,rrf_n,rforwarded,hold_clocks,parity_clocks;
 wire [3:0] rok,rfault,rrf,rselect,next_raw;
 wire [31:0] raddress;
 wire [8:0] flags;
 dorado_r_address_step dut(q,cb,ca,pb,pa,n,events,repeated,unsettled,
   wn,ln,tln,bp,ok,fault,rr,rv,rf,rf_n,address,select_n,forwarded);
 r_address_reference original(load,fixture,root,rp,reference,clocks,flags,next_raw,
   rwn,rln,hold_clocks,rtln,rbp,parity_clocks,rok,rfault,rrr,rrv,rrf,rrf_n,
   raddress,rselect,rforwarded);
 integer counts[0:44],totals[0:44];
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
 for(genvar i=0;i<45;i=i+1) begin: count_edges
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
 task automatic event_step(input next_root,input [201:0] pins);
   reg [635:0] expected;
   reg [44:0] predicted;
   reg [31:0] ea;
   reg [3:0] es;
   reg [1:0] ef;
   reg [19:0] eo;
   integer previous[0:44];
   begin
     ca=next_root;pa=pins;
     #100;
     expected=n;predicted=events;ea=address;es=select_n;ef=forwarded;
     eo={wn,ln,tln,bp,ok,fault,rr,rv};
     if(unsettled||repeated) $fatal(1,"R address solver unsettled/repeated");
     for(integer i=0;i<45;i=i+1) previous[i]=counts[i];
     {root,rp}={next_root,pins};
     #100;
     if(reference!==expected) begin
       for(integer i=0;i<636;i=i+1) if(reference[i]!==expected[i])
         $display("R_ADDRESS_DIFF bit=%0d old=%b expected=%b original=%b",i,q[i],expected[i],reference[i]);
       $fatal(1,"R address state mismatch check=%0d roots=%b->%b pins=%h q=%h expected=%h original=%h",checks,cb,ca,pins,q,expected,reference);
     end
     if(raddress!==ea||rselect!==es||rforwarded!==ef)
       $fatal(1,"R address output mismatch check=%0d address=%h/%h selects=%h/%h bypass=%b/%b",checks,ea,raddress,es,rselect,ef,rforwarded);
     if({rwn,rln,rtln,rbp,rok,rfault,rrr,rrv}!==eo || rrf!==rf || rrf_n!==rf_n)
       $fatal(1,"R address operand output mismatch check=%0d",checks);
     for(integer i=0;i<45;i=i+1) begin
       if(counts[i]-previous[i]!=int'(predicted[i]))
         $fatal(1,"R address clock mismatch check=%0d channel=%0d actual=%0d expected=%0d",checks,i,counts[i]-previous[i],predicted[i]);
       totals[i]=totals[i]+counts[i]-previous[i];
     end
     if(q[635:632]!=expected[635:632]) transparent=transparent+1;
     if(ea!=raddress) $fatal(1,"R address unstable settled output");
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
     q=expected;cb=next_root;pb=pins;checks=checks+1;
   end
 endtask
 reg [31:0] rng=32'hd04ad042;
 function automatic [31:0] random_next(input [31:0] v);
   reg [31:0] x;
   begin x=v^(v<<13);x=x^(x>>17);random_next=x^(x<<5);end
 endfunction
 reg [201:0] p;
 initial begin
   #100;load=1;#100;load=0;#100;
   fixture=7|(636'h1<<38)|(636'h1<<54)|(636'h1<<75)|(636'h1<<93)|(636'h1<<99);
   fixture[43:40]=15;fixture[47:44]=15;fixture[59:56]=15;fixture[63:60]=15;
   fixture[83:80]=15;fixture[87:84]=15;fixture[30]=1;fixture[29]=1;
   load=1;#100;load=0;#100;load=1;#100;load=0;#100;
   if(reference!==fixture) $fatal(1,"R address fixture mismatch");
   q=fixture;monitor=1;p=SEED;p[19:0]=20'h178;
   for(integer bit_no=0;bit_no<8;bit_no=bit_no+1) data_changed[bit_no]=$realtime;
   data_valid=8'hff;clock_valid=0;
   for(integer i=0;i<64;i=i+1) event_step(!cb,p);
   for(integer i=0;i<4096;i=i+1) begin
     // External producer inputs settle separately from the root event.
     // Hold and StopTasks may change only with the root masking their gates.
     if(!cb) event_step(1,p);
     for(integer word_no=0;word_no<7;word_no=word_no+1) begin
       rng=random_next(rng);
       for(integer bit_no=0;bit_no<32;bit_no=bit_no+1)
         if(word_no*32+bit_no<202) p[word_no*32+bit_no]=rng[bit_no];
     end
     p[8:0]=9'h178|(9'(rng[1])<<1);p[75]=0;p[108]=0;
     if(p[70]) p[74:71]=0;
     event_step(1,p);event_step(0,p);
     // Probe transparent address/data stages with the root held low.
     rng=random_next(rng);p[175:152]=rng[23:0];
     rng=random_next(rng);p[201:188]=rng[13:0];
     event_step(0,p);event_step(1,p);
   end
   if(adr_moves<1000||select_moves<100||forward_moves<100||transparent<100||
      lc_seen!=8'hff||drivers_seen!=4'hf) $fatal(1,"R address incomplete coverage");
   if(seen_h!={256{1'b1}}||seen_hs!={256{1'b1}}||seen_l!={256{1'b1}}||seen_ls!={256{1'b1}})
     $fatal(1,"R address incomplete physical address coverage");
   for(integer channel=0;channel<45;channel=channel+1) begin
     if(!totals[channel]) $fatal(1,"R address unexercised clock channel=%0d",channel);
     $display("R_ADDRESS_CLOCK channel=%0d edges=%0d",channel,totals[channel]);
   end
   if(setup_samples<1000||hold_samples<100) $fatal(1,"R address incomplete g16 timing coverage");
   $display("R_ADDRESS_TIMING setup_min_ns=%0.3f hold_min_ns=%0.3f setup_samples=%0d hold_samples=%0d",minimum_setup,minimum_hold,setup_samples,hold_samples);
   $display("R_ADDRESS_COVERAGE h=%0d hs=%0d l=%0d ls=%0d selects=%h drivers=%h LC=%h",$countones(seen_h),$countones(seen_hs),$countones(seen_l),$countones(seen_ls),select_seen,drivers_seen,lc_seen);
   $display("R_ADDRESS_STEP PASS checks=%0d address_changes=%0d select_changes=%0d bypass_changes=%0d transparent=%0d",checks,adr_moves,select_moves,forward_moves,transparent);
   $finish;
 end
endmodule
`default_nettype wire
