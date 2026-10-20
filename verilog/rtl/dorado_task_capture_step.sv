// Original task-selection pipeline coupled to processor captures/addressing.
// Experiment only; not instantiated in the native core. Original controller
// and processor functions are reused; actual Next/SwitchUp feedback is live.
// Q[63:0]: qualified processor-capture Qs; [67:64]/[71:68]: h08/h09 BNT;
// [73:72]: f14 Switchb/Switcha; [75:74]: f15 TPCBypass/SwitchUp';
// [77:76]: j08 CTask1/CTask0, [79:78]: j09 CTask3/CTask2;
// [83:80]/[87:84]: ProcH i12/ProcL i14 CurrLast' address registers.
// pins[19:0]: prior controller+LC+FF interface; [34:20]: TWReq1..15;
// [49:35]: Ready1..15; [64:50]: IsNext1'..15'; [65]: StopTasks;
// [66]: dBlock'; [67]: Block'; [68]: PRhold; [69]: Hold;
// [70]: rCT; [74:71]: sCTask{3,2,1,0}; [75] unused/reserved zero.
// Processor captures use task data from before the root event: the original
// SHCP path leads the ContA Switch/Next update. Equal-delta scheduling cannot
// establish this ordering; compare the explicit-delay original graph.
// Ready/IsNext, MIR tasking/block, reset/set and memory Hold producers are
// explicit boundaries. Data meets gated-clock setup; analog skew excluded.
`default_nettype none
module dorado_task_capture_step #(parameter integer WAVES=16)(
 input wire [87:0] state_q,input wire clock_before,clock_after,
 input wire [75:0] pins_before,pins_after,
 output reg [87:0] state_next,
 output reg [24:0] clock_events,repeated_events,output reg unsettled
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

    function automatic [21:0] capture_clocks(input [63:0] q,input base_clock,input [27:0] pins);
        reg [21:0] levels;
        reg [2:0] base;
        reg en_h,en_l,pp_h,pp_l,normal0,repeat_a;
        begin
            base={3{base_clock}};levels=0;levels[8:0]=controller_clocks(q[31:0],base[2],pins[0]);
            en_h=!q[13] | q[12];en_l=!q[8] | q[10];
            pp_h=base[1] | en_h;pp_l=base[0] | en_l;
            levels[13:9]={pp_h | q[32],pp_h | !q[23],!q[32],base[1],pp_h};
            levels[18:14]={pp_l | q[48],pp_l | !q[23],!q[48],base[0],pp_l};
            normal0=base[2] | q[6] | !q[7] | !q[25];
            repeat_a=pins[26] & !pins[27];
            levels[19]=normal0;
            levels[20]=normal0 | pins[0] | repeat_a;
            levels[21]=normal0 | pins[0] | repeat_a;
            capture_clocks=levels;
        end
    endfunction

    function automatic [15:0] processor_transfer(input [15:0] q,input [4:0] captures,
        input [27:0] pins,input start_a,input low_half);
        reg [15:0] n;
        reg lc0,lc1,lc2,tm,rw_n,tw_n,diff,repeat_curr;
        begin
            lc0=pins[11];lc1=pins[10];lc2=pins[9];
            tm=(!lc0 & lc1) | (pins[19:12]==8'o075);
            rw_n=!(lc0 | (lc1 & !lc2));tw_n=!(lc2 | (lc1 & !lc0));
            diff=|(q[11:8] ^ ~pins[23:20]);repeat_curr=pins[25] & pins[24];
            n=q;
            if(captures[0]) n[0]=start_a;
            if(captures[1]) n[1]=0;
            if(captures[2] && !repeat_curr) n[11:8]=~pins[23:20];
            if(captures[4]) n[15:12]=q[11:8];
            if(low_half ? captures[2] : captures[3])
                n[7:2]={diff,tw_n,rw_n,tm,low_half ? !lc1 : lc1,low_half ? lc1 : !lc1};
            processor_transfer=n;
        end
    endfunction
    function automatic [3:0] reverse_task(input [3:0] value);
        reverse_task={value[0],value[1],value[2],value[3]};
    endfunction
    function automatic [3:0] current_raw(input [87:0] q);
        current_raw={q[78],q[79],q[76],q[77]};
    endfunction
    function automatic [3:0] next_raw(input [87:0] q);
        next_raw=q[73] ? ~q[67:64] : current_raw(q);
    endfunction
    function automatic [3:0] priority_raw(input [75:0] pins);
        reg [3:0] highest;
        begin
            highest=0;
            for(integer task_no=1;task_no<16;task_no=task_no+1)
                if(pins[19+task_no] || (pins[34+task_no] && pins[49+task_no])) highest=4'(task_no);
            priority_raw=reverse_task(highest);
        end
    endfunction
    function automatic [27:0] capture_pins(input [87:0] q,input [75:0] pins);
        reg [27:0] p;
        begin
            p=0;p[19:0]=pins[19:0];p[23:20]=next_raw(q);
            p[24]=q[75];p[25]=pins[68];p[26]=pins[69];p[27]=!q[75];
            capture_pins=p;
        end
    endfunction
    function automatic [24:0] clocks(input [87:0] q,input base,input [75:0] pins);
        reg [24:0] levels;
        reg [21:0] old;
        reg pp_h,pp_l;
        begin
            old=capture_clocks(q[63:0],base,capture_pins(q,pins));levels=0;levels[21:0]=old;
            levels[22]=old[19] | pins[65];
            pp_h=base | !q[13] | q[12];pp_l=base | !q[8] | q[10];
            levels[23]=pp_h;levels[24]=pp_l;
            clocks=levels;
        end
    endfunction
    function automatic [15:0] task_transfer(input [87:0] q,input [24:0] captures,input [75:0] pins);
        reg [15:0] n;
        reg [3:0] penc,ct,nxt,chosen;
        reg repeat_cur,greater,less,d_switch,bnext_enable_n;
        begin
            n=q[79:64];penc=priority_raw(pins);ct=current_raw(q);nxt=next_raw(q);
            repeat_cur=pins[69] && q[75];chosen=repeat_cur ? ct : nxt;
            greater=reverse_task(penc)>reverse_task(chosen);
            less=reverse_task(penc)<reverse_task(chosen);
            bnext_enable_n=repeat_cur && (reverse_task(q[71:68])>reverse_task(penc));
            d_switch=(!pins[65] && greater) ||
                (less && !repeat_cur && !pins[66] && (nxt!=0)) ||
                (less && repeat_cur && !pins[67] && (ct!=0));
            if(captures[19] && !bnext_enable_n) begin n[3:0]=~penc;n[7:4]=penc;end
            if(pins[0]) begin n[8]=0;n[9]=0;n[11]=1;end
            else begin
                if(captures[22]) begin n[8]=d_switch;n[9]=d_switch;end
                if(captures[19]) n[11]=pins[65] || !greater;
            end
            if(captures[19]) n[10]=(penc==ct) && !pins[67];
            // Legal maintenance input excludes simultaneous asynchronous set/reset.
            for(integer i=0;i<4;i=i+1) begin
                if(pins[71+i]) begin
                    case(i) 0:n[13]=1;1:n[12]=1;2:n[15]=1;3:n[14]=1;endcase
                end else if(pins[70]) begin
                    case(i) 0:n[13]=0;1:n[12]=0;2:n[15]=0;3:n[14]=0;endcase
                end else if(captures[20]) begin
                    case(i) 0:n[13]=nxt[0];1:n[12]=nxt[1];2:n[15]=nxt[2];3:n[14]=nxt[3];endcase
                end
            end
            task_transfer=n;
        end
    endfunction
    reg [87:0] q,n;
    reg [24:0] previous,levels,captures;
    reg [27:0] p,source_p;
    reg settled,repeat_curr;
    always @* begin
        q=state_q;n=q;source_p=capture_pins(state_q,pins_after);previous=clocks(q,clock_before,pins_before);
        levels=previous;captures=0;clock_events=0;repeated_events=0;settled=0;p=0;repeat_curr=0;
        for(integer wave=0;wave<WAVES;wave=wave+1) begin
            p=capture_pins(q,pins_after);levels=clocks(q,clock_after,pins_after);captures=levels & ~previous;
            repeated_events=repeated_events | (captures & clock_events);clock_events=clock_events | captures;
            n[31:0]=controller_transfer(q[31:0],captures[8:0],p[8:0]);
            // Task-source setup snapshot survives the phase-Q/SHCP cascade.
            n[47:32]=processor_transfer(q[47:32],captures[13:9],source_p,!q[23],1'b0);
            n[63:48]=processor_transfer(q[63:48],captures[18:14],source_p,!q[23],1'b1);
            n[79:64]=task_transfer(q,captures,pins_after);
            repeat_curr=pins_after[68] && q[75];
            if(captures[23]) n[83:80]=repeat_curr ? q[43:40] : (!q[32] ? q[47:44] : ~p[23:20]);
            if(captures[24]) n[87:84]=repeat_curr ? q[59:56] : (!q[48] ? q[63:60] : ~p[23:20]);
            settled=(n==q && levels==previous);previous=levels;q=n;
        end
        state_next=q;unsettled=!settled;
    end
endmodule
`default_nettype wire
