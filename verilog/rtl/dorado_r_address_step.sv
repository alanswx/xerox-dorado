// Coupled processor plus the original final R-address/select stages.
// Component only: front RBase/stack D producers, comparisons, IFU, Ready,
// MIR and Hold remain boundaries. Functional R storage is not connected here.
// Extra Qs: H l05[601:596]; L g10/h10[605:602], i15[611:606],
// k11[617:612], g08[623:618], j11[629:624], l11[631:630], h15[635:632].
// Address bytes are physical MSB-first {Hrm,Hstk,Lrm,Lstk}, low byte first.
`default_nettype none
module dorado_r_address_step(
 input wire [635:0] state_q,input wire clock_before,clock_after,
 input wire [201:0] pins_before,pins_after,
 output reg [635:0] state_next,output wire [44:0] clock_events,repeated_events,
 output wire unsettled,output wire [1:0] write_n,latch_n,t_latch_n,bypass,
 output wire [3:0] parity_ok,faults,output wire [1:0] r_read_n,r_visible_n,
 output wire [3:0] r_flags,output wire [1:0] rm_flags_n,
 output wire [31:0] address,output wire [3:0] select_n,
 output wire [1:0] forwarded_bypass);
 function automatic [3:0] next_raw(input [87:0] q);
   next_raw=q[73] ? ~q[67:64] : {q[78],q[79],q[76],q[77]};
 endfunction
 function automatic [11:0] address_clocks(input [635:0] q,input base,input [201:0] p);
   reg ph,pl,cl0;
   begin
     ph=base | !q[13] | q[12];pl=base | !q[8] | q[10];cl0=pl | !q[23];
     // DblClock'B is ppClk, without the StartA qualification of Clock0'D.
     address_clocks={cl0,cl0 | (p[19:12]!=8'o072),cl0 | (p[68]&q[75]),
                     pl,pl,pl,cl0,pl,pl,pl,pl,ph | !q[23]};
   end
 endfunction
 function automatic rb_forward(input [635:0] q,input [201:0] p);
   rb_forward=!(q[99] | q[53] | p[177] | p[178]);
 endfunction
 function automatic [3:0] low_address_data(input [635:0] q,input [201:0] p);
   reg [3:0] a,b;
   reg repeat_cur,sh;
   begin
     sh=!q[48];repeat_cur=p[68]&q[75];
     a=p[196] ? 4'b0 : ~(sh ? q[635:632] : p[159:156]);
     b=p[197] ? 4'b0 : ~(repeat_cur ? p[191:188] : p[195:192]);
     // h12+h13+i11 are wired OR; i19 adds the fourth driver on bit 3.
     low_address_data=a|b|{(p[200]&p[199]),(p[200]&p[198]),2'b0}|
       {!(|(q[59:56]^~next_raw(q[87:0])) | p[186] | sh | p[187]),3'b0};
   end
 endfunction
 wire [87:0] core_next;
 wire [24:0] core_events,core_repeated;
 wire core_unsettled;
 dorado_task_capture_step core(state_q[87:0],clock_before,clock_after,
   pins_before[75:0],pins_after[75:0],core_next,core_events,core_repeated,core_unsettled);
 wire [11:0] old_clocks=address_clocks(state_q,clock_before,pins_before);
 reg [635:0] addressed;
 reg [11:0] new_clocks;
 reg [3:0] captured_low;
 always @* begin
   addressed=state_q;addressed[87:0]=core_next;
   new_clocks=address_clocks(addressed,clock_after,pins_after);
   captured_low=low_address_data(state_q,pins_after);
   if(new_clocks[0] && !old_clocks[0])
     addressed[601:596]={state_q[600],state_q[83],pins_after[155:152]};
   for(integer bit_no=0;bit_no<4;bit_no=bit_no+1)
     if(new_clocks[1+bit_no] && !old_clocks[1+bit_no]) addressed[602+bit_no]=captured_low[bit_no];
   if(new_clocks[5] && !old_clocks[5])
     addressed[611:606]={state_q[610],!(state_q[630]|pins_after[176]),pins_after[163:160]};
   if(new_clocks[6] && !old_clocks[6])
     addressed[617:612]={2'b0,{2{pins_after[165]}},{2{pins_after[164]}}};
   if(new_clocks[7] && !old_clocks[7])
     addressed[623:618]=pins_after[171:166];
   if(new_clocks[8] && !old_clocks[8])
     addressed[629:624]=pins_after[171:166];
   if(new_clocks[9] && !old_clocks[9]) addressed[630]=|next_raw(state_q[87:0]);
   if(new_clocks[10] && !old_clocks[10]) addressed[631]=pins_after[179];
   // MC10173 is LOW-transparent. A closing event retains its prior data.
   if(!new_clocks[11]) addressed[635:632]=pins_after[201] ?
     ~{pins_after[12],pins_after[13],pins_after[14],pins_after[15]} : pins_after[191:188];
   if(core_events[17]) addressed[99]=pins_after[68];
 end
 wire [151:0] child_before,child_after;
 assign child_before={pins_before[151],rb_forward(state_q,pins_before),
     pins_before[149],rb_forward(state_q,pins_before),pins_before[147:113],
     !(pins_before[182]|pins_before[183]),pins_before[111:0]};
 assign child_after={pins_after[151],rb_forward(addressed,pins_after),
     pins_after[149],rb_forward(addressed,pins_after),pins_after[147:113],
     !(pins_after[182]|pins_after[183]),pins_after[111:0]};
 wire [595:0] child_next;
 wire child_unsettled;
 dorado_r_flags_step child(state_q[595:0],clock_before,clock_after,child_before,child_after,
   child_next,clock_events[32:0],repeated_events[32:0],child_unsettled,
   write_n,latch_n,t_latch_n,bypass,parity_ok,faults,r_read_n,r_visible_n,r_flags,rm_flags_n);
 assign unsettled=core_unsettled|child_unsettled|(child_next[87:0]!=core_next);
 assign clock_events[44:33]=new_clocks & ~old_clocks;
 assign repeated_events[44:33]=0;
 always @* begin state_next=addressed;state_next[595:0]=child_next;end
 wire [3:0] rstk={pins_after[172],pins_after[173],pins_after[174],pins_after[175]};
 wire [3:0] high_wadr={addressed[596],addressed[597],addressed[598],addressed[599]};
 wire [3:0] low_wadr={addressed[606],addressed[607],addressed[608],addressed[609]};
 wire [3:0] low_adr={addressed[602],addressed[603],addressed[604],addressed[605]};
 assign address[7:0]={low_adr,~(core_next[32] ? high_wadr : rstk)};
 assign address[23:16]={~low_adr,~(core_next[48] ? low_wadr : rstk)};
 assign address[15:8]={addressed[612],addressed[614],addressed[618],addressed[619],
                      addressed[620],addressed[621],addressed[622],addressed[623]};
 assign address[31:24]={addressed[613],addressed[615],addressed[624],addressed[625],
                       addressed[626],addressed[627],addressed[628],addressed[629]};
 wire selected_stk=(!core_next[48]|child_next[97]) &
                    (core_next[48]|addressed[630]|pins_after[176]);
 assign select_n={selected_stk,!selected_stk,selected_stk,!selected_stk};
 assign forwarded_bypass={!rb_forward(addressed,pins_after),rb_forward(addressed,pins_after)};
endmodule
`default_nettype wire
