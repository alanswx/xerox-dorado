// Coupled original control and processor T/task capture event experiment.
// Not integrated into the native core. No reset, power-up state or pipeline
// stage is added. All 64 Q bits belong to complete original packages.
// pins[8:0]: qualified controller inputs; [11:9]={LC.0,LC.1,LC.2};
// [19:12]={FA,FB,FC}; [23:20]={Next.3,Next.2,Next.1,Next.0};
// [24]=BNTGtCT'a (active low), [25]=PRhold(E42), [26]=Hold(E26),
// [27]=SwitchUp. The two Hold nets are deliberately separate.
// Hold/SwitchUp are registered producer outputs (HM4.9, ContA f15); update
// them in the common clock's high window. Other data meets root-edge setup.
// Analog skew and malformed/asynchronous producer timing are not qualified.
`default_nettype none
module dorado_processor_capture_step #(parameter integer WAVES=16)(
 input wire [63:0] state_q,
 input wire clock_before,clock_after, // BaseBd l01 parallel clock copies
 input wire [27:0] pins_before,pins_after,
 output reg [63:0] state_next,
 output reg [21:0] clock_events,repeated_events,
 output reg unsettled
);
// Q0..31=qualified controller; each processor has l02.A/B, six T-control
// Qs in pin order, a21(Curr') and a20(Last'). ProcH32..47, ProcL48..63.
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

    function automatic [21:0] all_clocks(input [63:0] q,input base_clock,input [27:0] pins);
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
            all_clocks=levels;
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

    reg [63:0] q,n;
    reg [21:0] previous,levels,captures;
    reg settled;
    always @* begin
        q=state_q;n=q;previous=all_clocks(q,clock_before,pins_before);
        levels=previous;captures=0;clock_events=0;repeated_events=0;settled=0;
        for(integer wave=0;wave<WAVES;wave=wave+1) begin
            levels=all_clocks(q,clock_after,pins_after);captures=levels & ~previous;
            repeated_events=repeated_events | (captures & clock_events);
            clock_events=clock_events | captures;
            n[31:0]=controller_transfer(q[31:0],captures[8:0],pins_after[8:0]);
            n[47:32]=processor_transfer(q[47:32],captures[13:9],pins_after,!q[23],1'b0);
            n[63:48]=processor_transfer(q[63:48],captures[18:14],pins_after,!q[23],1'b1);
            settled=(n==q && levels==previous);previous=levels;q=n;
        end
        state_next=q;unsettled=!settled;
    end
endmodule
`default_nettype wire
