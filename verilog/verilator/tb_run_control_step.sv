`timescale 1ns/1ps
`default_nettype none
module tb_run_control_step;
    reg [21:0] state_q=0,fixture_state=0;
    reg before_clock=0,after_clock=0;
    reg [7:0] controls=8'b00100010;
    wire [21:0] state_next;
    wire [6:0] events,repeated;
    wire unsettled;
    dorado_run_control_step dut(.state_q(state_q),.clock_before(before_clock),
        .clock_after(after_clock),.controls(controls),.state_next(state_next),
        .clock_events(events),.repeated_events(repeated),.unsettled(unsettled));
    reg reference_clock=0,fixture_load=0;
    reg [7:0] reference_controls=8'b00100010;
    wire [21:0] reference;
    wire [6:0] reference_clocks;
    wire [4:0] reference_enables;
    wire [2:0] reference_decoded;
    run_control_reference_adapter original(reference_clock,reference_controls,
        fixture_load,fixture_state,reference,reference_clocks,
        reference_enables,reference_decoded);
    integer counters[0:6];
    reg monitor=0;
    for(genvar b=0;b<7;b=b+1) begin: edge_counts
        initial counters[b]=0;
        always @(posedge reference_clocks[b]) if(monitor) counters[b]=counters[b]+1;
    end
    function automatic [21:0] normalized(input [21:0] value);
        reg [21:0] q;
        begin
            q=value;
            if(q[21]) begin q[14]=1;q[15]=1;q[16]=0;q[17]=0;end
            normalized=q;
        end
    endfunction
    function automatic [4:0] enable_values(input [21:0] q);
        enable_values={~q[14]|q[16],~q[15]|q[17],~q[13]|q[12],~q[8]|q[10],~q[9]|q[11]};
    endfunction
    integer checks=0,refresh_halted=0,run_checks=0,async_resets=0;
    integer event_totals[0:6];
    reg [31:0] random_state=32'hd04a5129;
    function automatic [31:0] random_next(input [31:0] value);
        reg [31:0] x;
        begin x=value^(value<<13);x=x^(x>>17);random_next=x^(x<<5);end
    endfunction

    task automatic seed(input [21:0] value);
        monitor=0;fixture_load=0;reference_clock=0;
        reference_controls=8'b00100010;
        #1;fixture_state=0;fixture_load=1;
        #1;fixture_load=0;
        #1;fixture_state=normalized(value);fixture_load=1;
        #1;fixture_load=0;
        #1;
        if(reference!==normalized(value))
            $fatal(1,"run oracle seed mismatch wanted=%h got=%h",normalized(value),reference);
        state_q=normalized(value);before_clock=0;after_clock=0;
        monitor=1;
    endtask

    task automatic pin_event(input bit next_clock,input [7:0] next_controls);
        reg [21:0] expected;
        reg [6:0] expected_events;
        reg [2:0] decoded;
        integer previous_counts[0:6];
        after_clock=next_clock;controls=next_controls;
        #1;expected=state_next;expected_events=events;
        if(unsettled || repeated) $fatal(1,"run event solver failed to settle q=%h before=%b after=%b pins=%h repeated=%b",state_q,before_clock,next_clock,next_controls,repeated);
        for(integer i=0;i<7;i=i+1) previous_counts[i]=counters[i];
        {reference_clock,reference_controls}={next_clock,next_controls};
        #1;
        if(reference!==expected) begin
            $display("DEBUG prior_ref_controls=%h clocks=%b predicted=%b refDecoded=%b",reference_controls,reference_clocks,expected_events,reference_decoded);
            $fatal(1,"run state mismatch q=%h clk=%b->%b pins=%h got=%h expected=%h",state_q,before_clock,next_clock,next_controls,expected,reference);
        end
        if(reference_enables!==enable_values(expected))
            $fatal(1,"run enable output mismatch");
        decoded[0]=(next_controls[3]&next_controls[0]) |
            ~((~next_controls[2]|next_controls[1])&(~next_controls[2]|expected[2]));
        decoded[1]=expected[5] | (expected[19]&~expected[21]);
        decoded[2]=next_controls[7]|expected[21];
        if(reference_decoded!==decoded) $fatal(1,"run wired output mismatch");
        for(integer i=0;i<7;i=i+1) begin
            if(counters[i]-previous_counts[i]!=int'(expected_events[i]))
                $fatal(1,"run clock count mismatch channel=%0d count=%0d predicted=%0d",i,counters[i]-previous_counts[i],expected_events[i]);
            event_totals[i]=event_totals[i]+counters[i]-previous_counts[i];
        end
        if(next_controls[7]) async_resets=async_resets+1;
        if(reference_enables[2:0]==0) run_checks=run_checks+1;
        if(expected[21] && reference_enables[2:0]==7 && reference_enables[4:3]==0)
            refresh_halted=refresh_halted+1;
        state_q=expected;before_clock=next_clock;checks=checks+1;
    endtask

    // Control/data pins settle before changing the root clock. Simultaneous
    // changes can violate setup on the original pin-level FFs. The future
    // phase scheduler must likewise preserve pre-edge control state.
    task automatic transfer(input bit next_clock,input [7:0] next_controls);
        pin_event(before_clock,next_controls);
        pin_event(next_clock,next_controls);
    endtask

    initial begin
        for(integer i=0;i<7;i=i+1) event_totals[i]=0;
        // Explicit start, then processor halt while refresh remains enabled.
        seed(22'b0000000000000000000111);
        for(integer n=0;n<160;n=n+1) transfer(n[0],8'b01110010);
        if(state_q[21]!==1 || enable_values(state_q)!==0)
            $fatal(1,"manual run/refresh start failed");
        for(integer n=0;n<160;n=n+1) transfer(n[0],8'b00110010);
        if(enable_values(state_q)!==5'b00111 || state_q[21]!==1)
            $fatal(1,"manual memory refresh stopped with CPU halt");
        // Pulse the original single-step shift pipeline. Stop must latch
        // while the run-control clock continues, then clear via rStop.
        seed(22'b0000000000000000000111);
        for(integer n=0;n<160;n=n+1) transfer(n[0],8'b01110110);
        for(integer n=0;n<4;n=n+1) transfer(n[0],8'b01010110);
        for(integer n=0;n<32;n=n+1) transfer(n[0],8'b01110110);
        if(!state_q[6] || !state_q[7] || enable_values(state_q)!==5'b00111)
            $fatal(1,"manual single-step stop failed");
        for(integer n=0;n<16;n=n+1) transfer(n[0],8'b01110110);
        if(!state_q[6] || state_q[21]!==1 || enable_values(state_q)!==5'b00111)
            $fatal(1,"manual single-step stop retention failed");
        for(integer n=0;n<16;n=n+1) transfer(n[0],8'b11110110);
        if(state_q[6] || enable_values(state_q)!==0)
            $fatal(1,"manual single-step rStop recovery failed");
        // Deliberately broad initial states and legal binary pin changes,
        // followed without re-seeding between successive external events.
        for(integer sample=0;sample<128;sample=sample+1) begin
            random_state=random_next(random_state);seed(random_state[21:0]);
            for(integer n=0;n<256;n=n+1) begin
                random_state=random_next(random_state);
                transfer(random_state[0],random_state[8:1]);
            end
        end
        if(checks!=66632 || refresh_halted<100 || run_checks<100 || async_resets<1000)
            $fatal(1,"incomplete run/refresh coverage checks=%0d halt_refresh=%0d running=%0d reset=%0d",checks,refresh_halted,run_checks,async_resets);
        for(integer i=0;i<7;i=i+1) if(event_totals[i]<100)
            $fatal(1,"missing clock channel %0d",i);
        $display("RUN_CONTROL_STEP PASS checks=%0d refresh_halted=%0d running=%0d async_resets=%0d events=%0d,%0d,%0d,%0d,%0d,%0d,%0d",
            checks,refresh_halted,run_checks,async_resets,event_totals[0],event_totals[1],event_totals[2],event_totals[3],event_totals[4],event_totals[5],event_totals[6]);
        $finish;
    end
endmodule
`default_nettype wire
