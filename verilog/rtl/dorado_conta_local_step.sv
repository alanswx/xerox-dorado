// Experimental original Control-A evaluator with local tasking/branch producers.
// Existing physical Qs only: [31:0] control, [47:32] tasks, [63:48] Ready,
// [75:64] CP, [97:76] MIR, [103:98] d17 tasking. No native instantiation or extra pipeline stage.
// Task and Ready data use their pre-event epoch through the clock cascade.
// HM section 2.3 full cadence/IM/IFU/memory-Hold arrival closure remains open.
`default_nettype none
module dorado_conta_local_step #(parameter integer WAVES=16)(
 input wire [103:0] state_q,input wire clock_before,clock_after,
 // [75:0] prior task boundary layout; CP/Ready/Block/FF placeholder pins unused.
 // [111:76] MIR raw data/maintenance (same order as MIR component bits35:0);
 // [122:112] CP bus; [123] ASEL.0'a,[124] BSEL.0',[125] StopMIRClk;
 // [127:126] unrelated Ready d08 gate inputs.
 input wire [129:0] pins_before,pins_after,
 output reg [103:0] state_next,
 output reg [22:0] clock_events,repeated_events,
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
 function automatic [129:0] task_data_pins(input [103:0] q,input [129:0] p);
  reg [129:0] r;reg [7:0] d;
  begin d=local_decode(q,p);r=p;r[65]=d[0];r[1]=d[1];task_data_pins=r;end
 endfunction
 function automatic [3:0] reverse_task(input [3:0] n);reverse_task={n[0],n[1],n[2],n[3]};endfunction
 function automatic [3:0] current_task(input [103:0] q);current_task={q[46],q[47],q[44],q[45]};endfunction
 function automatic [3:0] next_raw(input [103:0] q);next_raw=q[41] ? ~q[35:32] : current_task(q);endfunction
 function automatic [8:0] controls(input [103:0] q,input [129:0] p);
  reg [8:0] c;reg [7:0] d;
  begin d=local_decode(q,p);c=p[8:0];c[0]=q[69];c[1]=d[1];c[2]=q[65];c[7]=q[64];controls=c;end
 endfunction
 function automatic [22:0] clock_levels(input [103:0] q,input base,input [129:0] p);
  reg [22:0] c;reg normal0,repeat_cur,mir_clock;reg [7:0] d;
  begin
   d=local_decode(q,p);c=0;c[8:0]=controller_clocks(q[31:0],base,q[69]);
   normal0=base | q[6] | !q[7] | !q[25];repeat_cur=p[69] & q[43];
   c[9]=normal0;c[10]=normal0 | q[69] | repeat_cur;c[11]=normal0 | d[0];
   mir_clock=normal0 | repeat_cur | p[125];c[17:12]={6{mir_clock}};
   c[19:18]={2{c[10]}};
   c[20]=p[122] | (p[121:119]!=3'b111);
   c[21]=p[122] | (p[121:119]!=3'b110);
   c[22]=normal0;clock_levels=c;
  end
 endfunction
 function automatic [8:0] mir_decode(input [21:0] q,input [129:0] p,input switch_up_n);
  reg ffok_n,notify_n,preempt_n;
  begin
   ffok_n=!p[124] | !(q[9] | q[8] | q[12] | q[13]);
   notify_n=p[123] | ffok_n | !q[3] | !q[2] | !q[1] | !q[0];
   preempt_n=!(p[69] | q[17]) | switch_up_n;
   mir_decode={q[21],q[17],q[4],q[5],q[6],q[7],ffok_n,preempt_n,notify_n};
  end
 endfunction
 function automatic [14:0] is_next(input [103:0] q);
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
 function automatic [3:0] priority_raw(input [103:0] source,input [129:0] p);
  reg [14:0] mask;reg [3:0] task_no;
  begin
   mask=is_next(source);task_no=0;
   for(integer i=1;i<16;i=i+1)
    if(p[19+i] || (source[48+4*((i-1)/4)+3-((i-1)%4)] && mask[i-1]))task_no=4'(i);
   priority_raw=reverse_task(task_no);
  end
 endfunction
 function automatic [15:0] task_transfer(input [103:0] q,input [103:0] source,input [21:0] capture,input [129:0] p);
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
 function automatic [15:0] ready_transfer(input [103:0] q,input [103:0] source,input [21:0] capture,input [129:0] p);
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
 function automatic [11:0] cp_transfer(input [11:0] q,input [21:0] capture,input [129:0] p);
  reg [11:0] n;
  begin
   n=q;
   if(capture[20])n[5:0]={p[115],p[116],p[117],p[114],p[113],p[112]};
   if(capture[21])n[11:6]={2'b00,p[118],p[113],p[115],p[114]};
   cp_transfer=n;
  end
 endfunction
 function automatic [21:0] mir_transfer(input [21:0] q,input [21:0] capture,input [129:0] p,input rmir);
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
 reg [103:0] q,n;reg [22:0] previous,levels,capture;reg settled;reg [7:0] old_local;
 always @* begin
  old_local=local_decode(state_q,pins_after);q=state_q;n=q;previous=clock_levels(q,clock_before,pins_before);levels=previous;capture=0;
  clock_events=0;repeated_events=0;settled=0;
  for(integer wave=0;wave<WAVES;wave=wave+1)begin
   levels=clock_levels(q,clock_after,pins_after);capture=levels & ~previous;
   repeated_events|=capture & clock_events;clock_events|=capture;
   n[31:0]=controller_transfer(q[31:0],capture[8:0],controls(q,pins_after));
   n[47:32]=task_transfer(q,state_q,capture[21:0],task_data_pins(state_q,pins_after));
   n[63:48]=ready_transfer(q,state_q,capture[21:0],pins_after);
   n[75:64]=cp_transfer(q[75:64],capture[21:0],pins_after);
   n[97:76]=mir_transfer(q[97:76],capture[21:0],pins_after,q[68]);
   if(capture[22])n[103:98]={2'b00,pins_after[129],old_local[5],!old_local[0],pins_after[128]};
   settled=(n==q && levels==previous);previous=levels;q=n;
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
