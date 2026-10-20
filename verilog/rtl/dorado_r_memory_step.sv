// Real address/select stages coupled to the four physical RM/STK banks.
// Component experiment, not native integration. At a g16 capture, keep the
// physical port on its pre-event address/select epoch while writes are closed.
// Other events use the resolved address. This uses one port per bank, with
// no extra RAM copies, reset clearing or flattened architectural RAM interface.
`default_nettype none
module dorado_r_memory_step(
 input wire host_clk,advance,input wire [635:0] state_q,
 input wire clock_before,clock_after,input wire [201:0] pins_before,pins_after,
 output wire [635:0] state_next,output wire [44:0] clock_events,repeated_events,
 output wire unsettled,output wire [1:0] write_n,latch_n,t_latch_n,bypass,
 output wire [3:0] parity_ok,faults,output wire [1:0] r_read_n,r_visible_n,
 output wire [3:0] r_flags,output wire [1:0] rm_flags_n,
 output wire [31:0] address,output wire [3:0] select_n,
 output wire [1:0] forwarded_bypass,r_write_n,
 output wire [31:0] port_address,output wire [3:0] port_select_n,
 output wire [15:0] file_read_data,output wire [1:0] file_read_parity);
 function automatic [31:0] old_address(input [635:0] q,input [201:0] p);
   reg [3:0] low_adr,rstk,hw,lw;
   begin
     low_adr={q[602],q[603],q[604],q[605]};
     rstk={p[172],p[173],p[174],p[175]};
     hw={q[596],q[597],q[598],q[599]};lw={q[606],q[607],q[608],q[609]};
     old_address[7:0]={low_adr,~(q[32] ? hw : rstk)};
     old_address[23:16]={~low_adr,~(q[48] ? lw : rstk)};
     old_address[15:8]={q[612],q[614],q[618],q[619],q[620],q[621],q[622],q[623]};
     old_address[31:24]={q[613],q[615],q[624],q[625],q[626],q[627],q[628],q[629]};
   end
 endfunction
 function automatic [3:0] old_select(input [635:0] q,input [201:0] p);
   reg stk_n;
   begin
     stk_n=(!q[48]|q[97]) & (q[48]|q[630]|p[176]);
     old_select={stk_n,!stk_n,stk_n,!stk_n};
   end
 endfunction
 // Preview only the clock/address cone; do not unroll the full arbiter into
 // the asynchronous RAM read path. Its outputs must match the full child.
 wire [31:0] resolved_address;
 wire [3:0] resolved_select;
 wire [1:0] parity_events;
 wire [44:0] preview_events={26'b0,parity_events[1],4'b0,parity_events[0],13'b0};
 wire preview_unsettled;
 dorado_r_port_preview preview(state_q,clock_before,clock_after,pins_before,pins_after,
   resolved_address,resolved_select,parity_events,preview_unsettled);
 wire [31:0] previous_address=old_address(state_q,pins_before);
 wire [3:0] previous_select=old_select(state_q,pins_before);
 assign port_address={preview_events[18] ? previous_address[31:16] : resolved_address[31:16],
                      preview_events[13] ? previous_address[15:0] : resolved_address[15:0]};
 assign port_select_n={preview_events[18] ? previous_select[3:2] : resolved_select[3:2],
                       preview_events[13] ? previous_select[1:0] : resolved_select[1:0]};
 wire [201:0] source_before={pins_before[201:145],file_read_parity,file_read_data,pins_before[126:0]};
 wire [201:0] source_after={pins_after[201:145],file_read_parity,file_read_data,pins_after[126:0]};
 wire child_unsettled;
 dorado_r_address_step child(state_q,clock_before,clock_after,source_before,source_after,
   state_next,clock_events,repeated_events,child_unsettled,write_n,latch_n,t_latch_n,bypass,
   parity_ok,faults,r_read_n,r_visible_n,r_flags,rm_flags_n,address,select_n,forwarded_bypass);
 assign unsettled=child_unsettled | preview_unsettled | (resolved_address!=address) | (resolved_select!=select_n) |
   (parity_events!={clock_events[18],clock_events[13]});
 assign r_write_n={clock_after|!state_next[8]|state_next[10]|!state_next[23]|state_next[99]|state_next[53],
                   clock_after|!state_next[13]|state_next[12]|!state_next[23]|state_next[93]|state_next[37]};
 dorado_r_file_ports files(host_clk,advance,r_write_n,port_address,port_select_n,
   state_next[555:540],file_read_data,file_read_parity);
endmodule
`default_nettype wire
