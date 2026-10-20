// Experimental coupled ContA run/refresh/instruction-phase event translation.
// Not in the native core. State contains all 32 Qs of the fourteen original
// sequential packages, including the other halves of the phase-chain chips.
// No power-up values, resets or pipeline stages are invented.
// External control data must settle before changing the root clock. Internal
// phase feedback is part of this evaluation rather than supplied by a caller.
`default_nettype none
module dorado_control_clock_step #(
    parameter integer WAVES=16
) (
    input wire [31:0] state_q,
    input wire clock_before,clock_after,
    // {WIM',rStop,SetRun,SetSS',SetRunRfsh,Error',StopAtT1,RWTPCorRWIM,NoDispatch}
    input wire [8:0] controls_before,controls_after,
    output reg [31:0] state_next,
    output reg [8:0] clock_events,repeated_events,
    output reg unsettled
);
    // Bits0..21: qualified run/refresh Q map. Added whole packages:
    // k03.A/B22/23, k04.A/B24/25, k05.A/B26/27, l07.A/B28/29,
    // j01.A/B30/31. In particular Q22=MemSH', Q30=~CRamClock.
    function automatic [8:0] clocks(input [31:0] q,input base,input no_dispatch);
        reg run_clock,instruction_clock;
        begin
            run_clock=base | q[18];instruction_clock=base | q[6] | !q[7];
            clocks={!instruction_clock,instruction_clock | no_dispatch,
                    instruction_clock,instruction_clock,instruction_clock,
                    run_clock,run_clock,q[18],base};
        end
    endfunction

    function automatic [31:0] transfer(input [31:0] q,input [8:0] captures,
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
            transfer=n;
        end
    endfunction

    reg [31:0] q,n;
    reg [8:0] previous,levels,captures;
    reg settled;
    always @* begin
        q=state_q;n=q;previous=clocks(q,clock_before,controls_before[0]);
        levels=previous;captures=0;clock_events=0;repeated_events=0;settled=0;
        for(integer wave=0;wave<WAVES;wave=wave+1) begin
            // Once q and previous reach a fixed point, subsequent evaluations
            // leave them unchanged. Do not gate the next iteration with a
            // wide equality tree; only the final stability check is needed.
            levels=clocks(q,clock_after,controls_after[0]);captures=levels & ~previous;
            repeated_events=repeated_events | (captures & clock_events);
            clock_events=clock_events | captures;
            n=transfer(q,captures,controls_after);
            settled=(n==q && levels==previous);previous=levels;q=n;
        end
        state_next=q;unsettled=!settled;
    end
endmodule
`default_nettype wire
