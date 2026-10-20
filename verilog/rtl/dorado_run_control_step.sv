// Experimental event-boundary translation of ContA's run/refresh controller.
// Not instantiated in a native core. No new reset or state is introduced:
// state_q/state_next describe the 22 existing Q bits listed below.
//
// This evaluates a single change of the external pins to quiescence. Derived
// positive clock edges and asynchronous set/reset effects are propagated in
// order. It is not a replacement for the machine's phase/timebase scheduler.
// Buffer arrival/setup timing and the surrounding Hold/Freeze logic remain
// separate obligations before native integration. Data/control inputs must
// settle before a root clock transition; simultaneous changes need an ordered
// scheduler event, not an assumption that all new data meets every FF's setup.
`default_nettype none
module dorado_run_control_step #(
    parameter integer WAVES = 12
) (
    input wire [21:0] state_q,
    input wire clock_before,clock_after, // ContA CLK.ca'
    // {rStop,SetRun,SetSS',SetRunRfsh,Phase0,preStartCyclea,Error',StopAtT1}
    input wire [7:0] controls,
    output reg [21:0] state_next,
    output reg [6:0] clock_events,
    output reg [6:0] repeated_events,
    output reg unsettled
);
    // Q map: i03[5:0]; j04.A/B[6/7]; k01.A/B[8/9]; k02.A/B[10/11];
    // l02.A/B[12/13]; l03.A/B[14/15]; l04.A/B[16/17];
    // l05.A/B[18/19]; l06.A/B[20/21].
    function automatic [6:0] clocks(input [21:0] q,input base);
        reg run_clock,instruction_clock;
        begin
            run_clock=base | q[18];
            instruction_clock=base | q[6] | !q[7];
            // {l06.A, main-instruction, pre-instruction, main-run, pre-run,
            //  StopRunClock (l05.B), raw (l05.A)}
            clocks={instruction_clock,instruction_clock,instruction_clock,
                    run_clock,run_clock,q[18],base};
        end
    endfunction

    function automatic [21:0] transfer(input [21:0] q,input [6:0] event_mask,
                                      input [7:0] pins);
        reg [21:0] n;
        reg d_stop,d_mem_run,clear_mem_stop;
        begin
            // Original tied ECL outputs explicitly OR, rather than selecting
            // one contributor or turning the connection into a tri-state.
            d_stop=(pins[3] & pins[0]) |
                !((!pins[2] | pins[1]) & (!pins[2] | q[2]));
            d_mem_run=q[5] | (q[19] & !q[21]);
            clear_mem_stop=pins[7] | q[21];
            n=q;
            if(event_mask[3]) begin
                n[5:0]={q[4],q[3],pins[6],q[1],q[0],pins[5]};
                n[7]=q[5];
            end
            if(pins[7]) begin n[6]=0;n[10]=0;n[11]=0;n[12]=0;end
            else begin
                if(event_mask[5]) n[6]=d_stop;
                if(event_mask[4]) begin n[10]=d_stop;n[11]=d_stop;n[12]=d_stop;end
            end
            if(event_mask[2]) begin
                n[8]=q[5];n[9]=q[5];n[13]=q[5];n[21]=q[19];
            end
            if(q[21]) begin n[14]=1;n[15]=1;end
            else if(event_mask[2]) begin n[14]=d_mem_run;n[15]=d_mem_run;end
            if(clear_mem_stop) begin n[16]=0;n[17]=0;end
            else if(event_mask[4]) begin n[16]=d_stop;n[17]=d_stop;end
            if(event_mask[0]) n[18]=!q[18];
            if(event_mask[1]) n[19]=pins[4];
            if(event_mask[6]) n[20]=pins[2];
            transfer=n;
        end
    endfunction

    reg [21:0] q,n;
    reg [6:0] previous,levels,events;
    reg settled;
    always @* begin
        q=state_q;
        previous=clocks(state_q,clock_before);
        clock_events=0;repeated_events=0;settled=0;
        n=q;levels=previous;events=0;
        for(integer wave=0;wave<WAVES;wave=wave+1) begin
            if(!settled) begin
                levels=clocks(q,clock_after);
                events=levels & ~previous;
                repeated_events=repeated_events | (events & clock_events);
                clock_events=clock_events | events;
                n=transfer(q,events,controls);
                settled=(n==q && levels==previous);
                previous=levels;q=n;
            end
        end
        state_next=q;unsettled=!settled;
    end
endmodule
`default_nettype wire
