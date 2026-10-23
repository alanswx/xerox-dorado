// Experimental composition of original Control-A producers with the R front.
// Q[751:0] retains its qualified map; [763:752] CP; [785:764] MIR.
// Compare the shared control/task/Ready computations before accepting this
// composition. No native use, extra state, reset or second RAM copy.
`default_nettype none
module dorado_r_front_next_macro_step #(parameter integer POST_MIR_PINS=2,
 parameter integer STOP_CAPTURE_OLD=1,STOP_CLOCK_NEW=1,BRANCH_CAPTURE_OLD=1)(
 input wire host_clk,advance,input wire [791:0] state_q,
 input wire clock_before,clock_after,input wire [314:0] pins_before,pins_after,
 output wire [791:0] state_next,output wire [67:0] clock_events,repeated_events,
 output wire unsettled,output wire [1:0] write_n,latch_n,t_latch_n,bypass,
 output wire [3:0] parity_ok,faults,output wire [1:0] r_read_n,r_visible_n,
 output wire [3:0] r_flags,output wire [1:0] rm_flags_n,
 output wire [31:0] address,output wire [3:0] select_n,
 output wire [1:0] forwarded_bypass,r_write_n,
 output wire [31:0] port_address,output wire [3:0] port_select_n,
 output wire [15:0] file_read_data,output wire [1:0] file_read_parity,
 output wire [201:0] source_pins,output wire [7:0] fa_n,
 output wire [29:0] ready_flags,output wire [103:0] conta_next,
 output wire shared_state_mismatch
);
 function automatic [7:0] local_decode(input [103:0] q,input [129:0] p);
  reg ffok_n,fa1_n,fb4_n,fc1_n,fc2_n,fc3_n;
  reg task_off,task_on,midas_on,toff_ok,stop_tasks,rwtpc;
  begin
   ffok_n=!p[124] | !(q[85]|q[84]|q[88]|q[89]);
   fa1_n=ffok_n | p[123] | !q[78] | q[79];
   fb4_n=({q[77],q[76],q[83]}!=3'd4);
   fc1_n=({q[82],q[81],q[80]}!=3'd1);
   fc2_n=({q[82],q[81],q[80]}!=3'd2);
   fc3_n=({q[82],q[81],q[80]}!=3'd3);
   task_off=!(fa1_n|fb4_n|fc2_n);task_on=!(fa1_n|fb4_n|fc3_n);
   midas_on=!(fa1_n|fb4_n|fc1_n);toff_ok=fa1_n|fb4_n|fc2_n|!q[41];
   stop_tasks=(task_off & toff_ok) | !(q[99] | task_on);
   // d21.2 reads d23's complemented B half, not d24's JCN.2 copy.
   // e21 Return'a is the wired OR of both three-input OR groups.
   rwtpc=q[89] & !q[85] & q[84] & q[86] & q[94] & q[95];
   local_decode={!stop_tasks,toff_ok,midas_on,task_on,task_off,q[99],rwtpc,stop_tasks};
  end
 endfunction
 function automatic [129:0] conta_pins(input [314:0] p);
  reg [129:0] r;
  begin
   r=0;r[75:0]=p[75:0];r[111:76]=p[281:246];r[122:112]=p[292:282];
   r[123]=p[294];r[124]=p[205];r[125]=p[293];r[127:126]=p[244:243];r[128]=p[310];r[129]=p[297];conta_pins=r;
  end
 endfunction
 wire [103:0] cq={state_q[791:786],state_q[785:764],state_q[763:752],state_q[751:736],state_q[79:64],state_q[31:0]};
 wire [22:0] ce,cr;wire [7:0] local_flags;wire cu;wire [38:0] cf;wire [3:0] cn;
 dorado_conta_local_step conta(cq,clock_before,clock_after,conta_pins(pins_before),conta_pins(pins_after),
  conta_next,ce,cr,cu,cf,cn,local_flags);
 function automatic [245:0] child_pins(input [314:0] p,input [11:0] cp,input [21:0] mir,input [103:0] producer);
  reg [245:0] r;reg [7:0] d;
  begin
   d=local_decode(producer,conta_pins(p));r=p[245:0];r[1]=d[1];r[65]=d[0];r[212]=mir[19]&mir[18]&mir[10]&!mir[8]&mir[15]&!mir[9];r[235]=!p[311];r[0]=cp[5];r[2]=cp[1];r[7]=cp[0];r[70]=cp[3];r[242]=cp[9];
   r[19:12]={mir[3],mir[2],mir[1],mir[0],mir[7],mir[6],mir[5],mir[4]};
   r[66]=p[263];r[67]=mir[17];r[176]=mir[21];
   r[209]=!p[205] | !(mir[9]|mir[8]|mir[12]|mir[13]);
   r[239:236]={mir[4],mir[5],mir[6],mir[7]};
   // Pre-event task state is used by the qualified child's Ready capture.
   r[240]=!(p[69] | mir[17]) | state_q[75];
   r[241]=p[294] | r[209] | !mir[3] | !mir[2] | !mir[1] | !mir[0];
   child_pins=r;
  end
 endfunction
 wire [245:0] before_child=child_pins(pins_before,cq[75:64],cq[97:76],cq);
 // Original H l05/L i15 rise before MIR clock-to-Q on a closing HIGH root.
 // LOW windows remain transparent to the newly arrived MIR controls.
 // Modes 0/1 retain the rejected uniform epochs as diagnostic controls.
 wire [21:0] capture_mir=(POST_MIR_PINS==0 || (POST_MIR_PINS==2 && clock_after)) ?
  cq[97:76] : conta_next[97:76];
 wire [7:0] capture_local=local_decode(cq,conta_pins(pins_after));
 wire [245:0] after_raw=child_pins(pins_after,conta_next[75:64],capture_mir,conta_next);
 reg [245:0] after_child;
 always @* begin
  after_child=after_raw;
  after_child[1]=BRANCH_CAPTURE_OLD ? capture_local[1] : after_raw[1];
  after_child[65]=STOP_CLOCK_NEW ? after_raw[65] : capture_local[0];after_child[67]=cq[93];
  after_child[239:236]={cq[80],cq[81],cq[82],cq[83]};
  after_child[240]=!(pins_after[69]|cq[93])|cq[43];
  after_child[241]=pins_after[294] | (!pins_after[205]|!(cq[85]|cq[84]|cq[88]|cq[89])) | !cq[79]|!cq[78]|!cq[77]|!cq[76];
 end
 wire [751:0] child_next;wire child_unsettled;
 wire [201:0] child_source;wire [7:0] child_fa;wire [29:0] child_ready;
 wire [31:0] child_address;wire [3:0] child_select;wire [1:0] child_forward;
 dorado_local_r_front_ready_step child(STOP_CAPTURE_OLD ? capture_local[0] : after_raw[65],host_clk,advance,state_q[751:0],clock_before,clock_after,before_child,after_child,
  child_next,clock_events[58:0],repeated_events[58:0],child_unsettled,
  write_n,latch_n,t_latch_n,bypass,parity_ok,faults,r_read_n,r_visible_n,r_flags,rm_flags_n,
  child_address,child_select,child_forward,r_write_n,port_address,port_select_n,file_read_data,file_read_parity,
  child_source,child_fa,child_ready);
 assign shared_state_mismatch={child_next[751:736],child_next[79:64],child_next[31:0]}!=conta_next[63:0];
 assign state_next[31:0]=conta_next[31:0];assign state_next[79:64]=conta_next[47:32];
 assign state_next[751:736]=conta_next[63:48];assign state_next[785:752]=conta_next[97:64];assign state_next[791:786]=conta_next[103:98];
 assign state_next[63:32]=child_next[63:32];assign state_next[735:80]=child_next[735:80];
 // Settled observations use actual new MIR/CP/Ready Qs. This cone has no
 // storage and does not feed new data back into the closing capture epoch.
 wire [245:0] settled_pins=child_pins(pins_after,conta_next[75:64],conta_next[97:76],conta_next);
 wire [201:0] settled_source;wire [7:0] settled_fa;
 wire [31:0] settled_address;wire [3:0] settled_select;wire [1:0] settled_forward;
 dorado_r_front_cone settled_view(.state_q(state_next[735:0]),.base(clock_after),.pins(settled_pins[235:0]),
  .source_pins(settled_source),.fa_n(settled_fa),.address(settled_address),
  .select_n(settled_select),.forwarded_bypass(settled_forward));
 assign ready_flags=POST_MIR_PINS==2 ? {cf[14:0],cf[29:15]} : child_ready;
 assign source_pins=POST_MIR_PINS==2 ?
  {settled_source[201:65],ready_flags[29:15],ready_flags[14:0],settled_source[34:0]} : child_source;
 assign fa_n=POST_MIR_PINS==2 ? settled_fa : child_fa;
 assign address=POST_MIR_PINS==2 ? settled_address : child_address;
 assign select_n=POST_MIR_PINS==2 ? settled_select : child_select;
 assign forwarded_bypass=POST_MIR_PINS==2 ? settled_forward : child_forward;
 assign clock_events[67]=ce[22];assign repeated_events[67]=cr[22];
 assign clock_events[66:59]={ce[21:20],ce[17:12]};assign repeated_events[66:59]={cr[21:20],cr[17:12]};
 assign unsettled=cu|child_unsettled|shared_state_mismatch|
  (clock_events[8:0]!=ce[8:0])|(clock_events[19]!=ce[9])|
  (clock_events[21:20]!={2{ce[10]}})|(clock_events[22]!=ce[11])|
  (clock_events[58:57]!=ce[19:18]);
endmodule
`default_nettype wire
