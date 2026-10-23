// Experimental composition of original Control-A producers with the R front.
// Q[751:0] retains its qualified map; [763:752] CP; [785:764] MIR.
// Compare the shared control/task/Ready computations before accepting this
// composition. No native use, extra state, reset or second RAM copy.
`default_nettype none
module dorado_r_front_l17_step #(parameter integer POST_MIR_PINS=2,
 parameter integer STOP_CAPTURE_OLD=1,STOP_CLOCK_NEW=1,BRANCH_CAPTURE_OLD=1)(
 input wire host_clk,advance,input wire [797:0] state_q,
 input wire clock_before,clock_after,input wire [325:0] pins_before,pins_after,
 output wire [797:0] state_next,output wire [68:0] clock_events,repeated_events,
 output wire unsettled,output wire [1:0] write_n,latch_n,t_latch_n,bypass,
 output wire [3:0] parity_ok,faults,output wire [1:0] r_read_n,r_visible_n,
 output wire [3:0] r_flags,output wire [1:0] rm_flags_n,
 output wire [31:0] address,output wire [3:0] select_n,
 output wire [1:0] forwarded_bypass,r_write_n,
 output wire [31:0] port_address,output wire [3:0] port_select_n,
 output wire [15:0] file_read_data,output wire [1:0] file_read_parity,
 output wire [201:0] source_pins,output wire [7:0] fa_n,
 output wire [29:0] ready_flags,output wire [109:0] conta_next,
 output wire shared_state_mismatch
);
 function automatic [2:0] memory_commands(input [21:0] mir);
  reg return_group,read_im_tpc,wim_n,wtpc_n;
  begin
   // e21 Return'a plus d20's actual JCN.2'a/3'a/4' decoder inputs.
   // k11.9 NANDs RIM' and RTPC'. Use d24's copy of JCN.2 here.
   return_group=!mir[9]&mir[8]&mir[10]&mir[19]&mir[18];
   read_im_tpc=return_group&mir[15]&!mir[11];
   wim_n=!(return_group&mir[15]&mir[14]&mir[11]);
   wtpc_n=!(return_group&mir[15]&!mir[14]&mir[11]);
   memory_commands={wtpc_n,wim_n,read_im_tpc};
  end
 endfunction
 function automatic count_branch(input [21:0] mir,input [325:0] p);
  reg ffok_n,fa0_n,fb6_n,ff_branch,cond_branch_n,j_branch;
  begin
   // f24/a21 wired-OR FA=0', a19 FB=6', b15 and d19 Q3,
   // f22/f23 wired-OR CondBr'a, a14.15 and d21's inverter.
   ffok_n=!p[205]|!(mir[9]|mir[8]|mir[12]|mir[13]);
   fa0_n=ffok_n|(!p[294]&(mir[2]|mir[3]));
   fb6_n=({mir[1],mir[0],mir[7]}!=3'd6);
   ff_branch=!fa0_n&!fb6_n&({mir[6],mir[5],mir[4]}==3'd3);
   cond_branch_n=!(mir[15]|mir[14]|mir[8])|(mir[10]&mir[18]&mir[19])|mir[9];
   j_branch=!cond_branch_n&({mir[10],mir[19],mir[18]}==3'd3);
   count_branch=ff_branch|j_branch;
  end
 endfunction
 function automatic [7:0] local_decode(input [109:0] q,input [130:0] p);
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
 function automatic [130:0] conta_pins(input [325:0] p);
  reg [130:0] r;
  begin
   r=0;r[75:0]=p[75:0];r[111:76]=p[281:246];r[122:112]=p[292:282];
   r[123]=p[294];r[124]=p[205];r[125]=p[293];r[127:126]=p[244:243];r[128]=p[310];r[129]=p[297];r[130]=p[317];conta_pins=r;
  end
 endfunction
 wire [109:0] cq={state_q[797:792],state_q[791:786],state_q[785:764],state_q[763:752],state_q[751:736],state_q[79:64],state_q[31:0]};
 wire [23:0] ce,cr;wire [7:0] local_flags;wire cu;wire [38:0] cf;wire [3:0] cn;
 dorado_conta_l17_step conta(cq,clock_before,clock_after,conta_pins(pins_before),conta_pins(pins_after),
  conta_next,ce,cr,cu,cf,cn,local_flags);
 function automatic [245:0] child_pins(input [325:0] p,input [11:0] cp,input [21:0] mir,input [109:0] producer);
  reg [245:0] r;reg [7:0] d;reg [2:0] m;
  begin
   d=local_decode(producer,conta_pins(p));m=memory_commands(mir);r=p[245:0];r[8]=m[1];r[1]=d[1];r[65]=d[0];r[212]=mir[19]&mir[18]&mir[10]&!mir[8]&mir[15]&!mir[9];r[235]=count_branch(mir,p);r[0]=cp[5];r[2]=cp[1];r[7]=cp[0];r[70]=cp[3];r[242]=cp[9];
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
 wire [2:0] settled_commands=memory_commands(conta_next[97:76]);
 reg [245:0] after_child;
 always @* begin
  after_child=after_raw;after_child[8]=settled_commands[1];
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
 assign state_next[797:792]=conta_next[109:104];
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
 assign clock_events[68]=ce[23];assign repeated_events[68]=cr[23];
 assign clock_events[67]=ce[22];assign repeated_events[67]=cr[22];
 assign clock_events[66:59]={ce[21:20],ce[17:12]};assign repeated_events[66:59]={cr[21:20],cr[17:12]};
 assign unsettled=cu|child_unsettled|shared_state_mismatch|
  (clock_events[8:0]!=ce[8:0])|(clock_events[19]!=ce[9])|
  (clock_events[21:20]!={2{ce[10]}})|(clock_events[22]!=ce[11])|
  (clock_events[58:57]!=ce[19:18]);
endmodule
`default_nettype wire

// Experimental original Control-A evaluator with local tasking/branch producers.
// Existing physical Qs only: [31:0] control, [47:32] tasks, [63:48] Ready,
// [75:64] CP, [97:76] MIR, [103:98] d17 tasking. No native instantiation or extra pipeline stage.
// Task and Ready data use their pre-event epoch through the clock cascade.
// HM section 2.3 full cadence/IM/IFU/memory-Hold arrival closure remains open.
`default_nettype none
module dorado_conta_l17_step #(parameter integer WAVES=16)(
 input wire [109:0] state_q,input wire clock_before,clock_after,
 // [75:0] prior task boundary layout; CP/Ready/Block/FF placeholder pins unused.
 // [111:76] MIR raw data/maintenance (same order as MIR component bits35:0);
 // [122:112] CP bus; [123] ASEL.0'a,[124] BSEL.0',[125] StopMIRClk;
 // [127:126] unrelated Ready d08 gate inputs.
 input wire [130:0] pins_before,pins_after,
 output reg [109:0] state_next,
 output reg [23:0] clock_events,repeated_events,
 output reg unsettled,output wire [38:0] ready_flags,output wire [3:0] next_task,output wire [7:0] local_flags
);
    function automatic [8:0] controller_clocks(input [31:0] q,input base,input no_dispatch);
        reg run_clock,instruction_clock;
        begin
            run_clock=base | q[18];instruction_clock=base | q[6] | !q[7];
            controller_clocks={!instruction_clock,instruction_clock | no_dispatch,
                    instruction_clock,instruction_clock,instruction_clock,
                    run_clock,run_clock,q[18],base};
        end
    endfunction
    function automatic [31:0] controller_transfer(input [31:0] q,input [8:0] captures,
                                      input [8:0] pins);
        reg [31:0] n;
        reg d_stop,d_mem_run,clear_mem_stop,phase_init,phase_term,d_start;
        begin
            d_stop=(q[29] & pins[2]) | !((!q[23] | pins[3]) & (!q[23] | q[2]));
            d_mem_run=q[5] | (q[19] & !q[21]);clear_mem_stop=pins[7] | q[21];
            phase_init=!pins[2] & (q[6] | !q[7]);
            phase_term=(pins[1] | !q[29]) & !q[31];d_start=!phase_term;
            n=q;
            if(captures[3]) begin
                n[5:0]={q[4],q[3],pins[6],q[1],q[0],pins[5]};n[7]=q[5];
            end
            if(pins[7]) begin n[6]=0;n[10]=0;n[11]=0;n[12]=0;end
            else begin
                if(captures[5]) n[6]=d_stop;
                if(captures[4]) begin n[10]=d_stop;n[11]=d_stop;n[12]=d_stop;end
            end
            if(captures[2]) begin n[8]=q[5];n[9]=q[5];n[13]=q[5];n[21]=q[19];end
            if(q[21]) begin n[14]=1;n[15]=1;end
            else if(captures[2]) begin n[14]=d_mem_run;n[15]=d_mem_run;end
            if(clear_mem_stop) begin n[16]=0;n[17]=0;end
            else if(captures[4]) begin n[16]=d_stop;n[17]=d_stop;end
            if(captures[0]) begin n[18]=!q[18];n[22]=!q[18];end
            if(captures[1]) n[19]=pins[4];
            if(captures[6]) n[20]=q[23];
            if(phase_init) begin
                n[23]=0;n[24]=0;n[25]=0;n[26]=0;n[27]=0;n[28]=0;n[29]=1;n[31]=0;
            end else begin
                if(captures[4]) n[23]=d_start;
                if(captures[7]) n[24]=d_start;
                if(captures[5]) begin
                    n[25]=d_start;n[26]=q[27];n[27]=q[28];
                    n[28]=q[29] & phase_term;n[29]=q[25];n[31]=q[26];
                end
            end
            if(pins[8]) n[30]=1;
            else if(captures[8]) n[30]=!q[27];
            controller_transfer=n;
        end
    endfunction

 // Original ContA a13/a16/a19/a20/b18/b21 and e21/d21 functions.
 // [98:103] is d17's physical six-Q order, not added architectural state.
 function automatic [2:0] memory_commands(input [21:0] mir);
  reg return_group,read_im_tpc,wim_n,wtpc_n;
  begin
   // e21 Return'a plus d20's actual JCN.2'a/3'a/4' decoder inputs.
   // k11.9 NANDs RIM' and RTPC'. Use d24's copy of JCN.2 here.
   return_group=!mir[9]&mir[8]&mir[10]&mir[19]&mir[18];
   read_im_tpc=return_group&mir[15]&!mir[11];
   wim_n=!(return_group&mir[15]&mir[14]&mir[11]);
   wtpc_n=!(return_group&mir[15]&!mir[14]&mir[11]);
   memory_commands={wtpc_n,wim_n,read_im_tpc};
  end
 endfunction
 function automatic [7:0] local_decode(input [109:0] q,input [130:0] p);
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
 function automatic [130:0] task_data_pins(input [109:0] q,input [130:0] p);
  reg [130:0] r;reg [7:0] d;
  begin d=local_decode(q,p);r=p;r[65]=d[0];r[1]=d[1];task_data_pins=r;end
 endfunction
 function automatic [3:0] reverse_task(input [3:0] n);reverse_task={n[0],n[1],n[2],n[3]};endfunction
 function automatic [3:0] current_task(input [109:0] q);current_task={q[46],q[47],q[44],q[45]};endfunction
 function automatic [3:0] next_raw(input [109:0] q);next_raw=q[41] ? ~q[35:32] : current_task(q);endfunction
 function automatic [8:0] controls(input [109:0] q,input [130:0] p);
  reg [8:0] c;reg [7:0] d;reg [2:0] m;
  begin d=local_decode(q,p);m=memory_commands(q[97:76]);c=p[8:0];c[8]=m[1];c[0]=q[69];c[1]=d[1];c[2]=q[65];c[7]=q[64];controls=c;end
 endfunction
 function automatic [23:0] clock_levels(input [109:0] q,input base,input [130:0] p,input mir_switch_q);
  reg [23:0] c;reg normal0,repeat_cur,mir_clock;reg [7:0] d;
  begin
   d=local_decode(q,p);c=0;c[8:0]=controller_clocks(q[31:0],base,q[69]);
   normal0=base | q[6] | !q[7] | !q[25];repeat_cur=p[69] & q[43];
   c[9]=normal0;c[10]=normal0 | q[69] | repeat_cur;c[11]=normal0 | d[0];
   mir_clock=normal0 | (p[69]&mir_switch_q) | p[125];c[17:12]={6{mir_clock}};
   c[19:18]={2{c[10]}};
   c[20]=p[122] | (p[121:119]!=3'b111);
   c[21]=p[122] | (p[121:119]!=3'b110);
   c[22]=normal0;c[23]=base | q[6] | !q[7] | !q[20];clock_levels=c;
  end
 endfunction
 function automatic [8:0] mir_decode(input [21:0] q,input [130:0] p,input switch_up_n);
  reg ffok_n,notify_n,preempt_n;
  begin
   ffok_n=!p[124] | !(q[9] | q[8] | q[12] | q[13]);
   notify_n=p[123] | ffok_n | !q[3] | !q[2] | !q[1] | !q[0];
   preempt_n=!(p[69] | q[17]) | switch_up_n;
   mir_decode={q[21],q[17],q[4],q[5],q[6],q[7],ffok_n,preempt_n,notify_n};
  end
 endfunction
 function automatic [14:0] is_next(input [109:0] q);
  reg [14:0] result;reg [3:0] raw;
  begin
   result=0;
   for(integer i=1;i<16;i=i+1)begin
    raw=i<8 ? q[39:36] : ~q[35:32];
    result[i-1]=!q[40] || reverse_task(raw)!=4'(i);
   end
   is_next=result;
  end
 endfunction
 function automatic [3:0] priority_raw(input [109:0] source,input [130:0] p);
  reg [14:0] mask;reg [3:0] task_no;
  begin
   mask=is_next(source);task_no=0;
   for(integer i=1;i<16;i=i+1)
    if(p[19+i] || (source[48+4*((i-1)/4)+3-((i-1)%4)] && mask[i-1]))task_no=4'(i);
   priority_raw=reverse_task(task_no);
  end
 endfunction
 function automatic [15:0] task_transfer(input [109:0] q,input [109:0] source,input [21:0] capture,input [130:0] p);
  reg [15:0] n;reg [3:0] penc,ct,nxt,chosen;reg repeat_cur,greater,less,bnext_enable_n,d_switch;
  begin
   n=q[47:32];penc=priority_raw(source,p);ct=current_task(source);nxt=next_raw(source);
   repeat_cur=p[69] && source[43];chosen=repeat_cur ? ct : nxt;
   greater=reverse_task(penc)>reverse_task(chosen);less=reverse_task(penc)<reverse_task(chosen);
   bnext_enable_n=repeat_cur && reverse_task(source[39:36])>reverse_task(penc);
   d_switch=(!p[65] && greater) || (less && !repeat_cur && !p[93] && nxt!=0) ||
    (less && repeat_cur && !source[93] && ct!=0);
   if(capture[9] && !bnext_enable_n)begin n[3:0]=~penc;n[7:4]=penc;end
   if(q[69])begin n[8]=0;n[9]=0;n[11]=1;end
   else begin
    if(capture[11])begin n[8]=d_switch;n[9]=d_switch;end
    if(capture[9])n[11]=p[65] || !greater;
   end
   if(capture[9])n[10]=(penc==ct) && !source[93];
   for(integer i=0;i<4;i=i+1)begin
    if(p[71+i])case(i)0:n[13]=1;1:n[12]=1;2:n[15]=1;3:n[14]=1;endcase
    else if(q[67])case(i)0:n[13]=0;1:n[12]=0;2:n[15]=0;3:n[14]=0;endcase
    else if(capture[10])case(i)0:n[13]=nxt[0];1:n[12]=nxt[1];2:n[15]=nxt[2];3:n[14]=nxt[3];endcase
   end
   task_transfer=n;
  end
 endfunction
 function automatic [15:0] ready_transfer(input [109:0] q,input [109:0] source,input [21:0] capture,input [130:0] p);
  reg [15:0] n,data;reg [14:0] mask;reg [8:0] decoded;reg [3:0] ct,notify_task;integer bit_no;
  begin
   decoded=mir_decode(source[97:76],p,source[43]);ct=reverse_task(current_task(source));
   notify_task=reverse_task(decoded[6:3]);mask=is_next(source);data=0;n=q[63:48];
   for(integer i=1;i<16;i=i+1)begin
    bit_no=4*((i-1)/4)+3-((i-1)%4);
    data[bit_no]=(source[48+bit_no] && mask[i-1]) || (!decoded[0] && notify_task==4'(i)) || (!decoded[1] && ct==4'(i));
   end
   if(q[73])n=0;else if(capture[18])n=data;
   ready_transfer=n;
  end
 endfunction
 function automatic [11:0] cp_transfer(input [11:0] q,input [21:0] capture,input [130:0] p);
  reg [11:0] n;
  begin
   n=q;
   if(capture[20])n[5:0]={p[115],p[116],p[117],p[114],p[113],p[112]};
   if(capture[21])n[11:6]={2'b00,p[118],p[113],p[115],p[114]};
   cp_transfer=n;
  end
 endfunction
 function automatic [21:0] mir_transfer(input [21:0] q,input [21:0] capture,input [130:0] p,input rmir);
  reg [21:0] n,data,set_q,reset_q,enable;reg [35:0] m;
  begin
   m=p[111:76];
   data[7:0]={m[4],m[5],m[6],m[7],m[0],m[1],m[2],m[3]};
   data[15:8]={m[10],m[11],m[10],m[11],m[12],m[13],m[8],m[9]};
   data[19:16]={m[14],m[15],m[17],m[16]};data[20]=0;data[21]=m[17];
   set_q[7:0]={m[22],m[23],m[24],m[25],m[18],m[19],m[20],m[21]};
   set_q[15:8]={m[28],m[29],m[28],m[29],m[30],m[31],m[26],m[27]};
   set_q[19:16]={m[32],m[33],rmir,m[34]};set_q[20]=0;set_q[21]=rmir;
   reset_q[7:0]={8{rmir}};reset_q[15:8]={8{rmir}};
   reset_q[19:16]={rmir,rmir,m[35],rmir};reset_q[20]=0;reset_q[21]=m[35];
   enable={capture[17],1'b0,{4{capture[16]}},{4{capture[15]}},{4{capture[14]}},{4{capture[13]}},{4{capture[12]}}};n=q;
   for(integer i=0;i<22;i=i+1)
    if(set_q[i])n[i]=1;else if(reset_q[i])n[i]=0;else if(enable[i])n[i]=data[i];
   mir_transfer=n;
  end
 endfunction
 function automatic [1:0] l17_ff(input [109:0] s,input [130:0] p);
  reg ffok_n,fa1_n;reg [2:0] fb,fc;
  begin
   ffok_n=!p[124] | !(s[85]|s[84]|s[88]|s[89]);
   fa1_n=ffok_n | p[123] | !s[78] | s[79];
   fb={s[77],s[76],s[83]};fc={s[82],s[81],s[80]};
   l17_ff={!fa1_n && fb==3'd7 && fc==3'd5,!fa1_n && fb==3'd4 && fc==3'd0};
  end
 endfunction
 reg [109:0] q,n;reg [23:0] previous,levels,capture;reg settled;reg [7:0] old_local;reg [2:0] old_memory;reg [1:0] old_l17_ff;reg mir_switch_view,mir_switch_before;
 always @* begin
  old_l17_ff=l17_ff(state_q,pins_after);old_local=local_decode(state_q,pins_after);old_memory=memory_commands(state_q[97:76]);mir_switch_view=state_q[43];q=state_q;n=q;previous=clock_levels(q,clock_before,pins_before,q[43]);levels=previous;capture=0;
  clock_events=0;repeated_events=0;settled=0;
  for(integer wave=0;wave<WAVES;wave=wave+1)begin
   levels=clock_levels(q,clock_after,pins_after,mir_switch_view);capture=levels & ~previous;
   repeated_events|=capture & clock_events;clock_events|=capture;
   n[31:0]=controller_transfer(q[31:0],capture[8:0],controls(q,pins_after));
   n[47:32]=task_transfer(q,state_q,capture[21:0],task_data_pins(state_q,pins_after));
   n[63:48]=ready_transfer(q,state_q,capture[21:0],pins_after);
   n[75:64]=cp_transfer(q[75:64],capture[21:0],pins_after);
   n[97:76]=mir_transfer(q[97:76],capture[21:0],pins_after,q[68]);
   if(capture[22])n[103:98]={2'b00,state_q[106],old_local[5],!old_local[0],old_memory[0]};
   if(capture[23])n[109:104]={1'b0,old_l17_ff[1],state_q[101],state_q[100]&pins_after[130],state_q[100],old_l17_ff[0]};
   mir_switch_before=mir_switch_view;if(q[69])mir_switch_view=1;settled=(n==q && levels==previous && mir_switch_view==mir_switch_before);previous=levels;q=n;
  end
  state_next=q;unsettled=!settled;
 end
 wire [14:0] settled_mask=is_next(state_next);reg [14:0] settled_ready;
 always @*for(integer i=1;i<16;i=i+1)settled_ready[i-1]=state_next[48+4*((i-1)/4)+3-((i-1)%4)];
 assign ready_flags={mir_decode(state_next[97:76],pins_after,state_next[43]),settled_ready,settled_mask};
 assign next_task=next_raw(state_next);
 assign local_flags=local_decode(state_next,pins_after);
endmodule
`default_nettype wire
