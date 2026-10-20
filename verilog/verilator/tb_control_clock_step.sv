`timescale 1ns/1ps
`default_nettype none
module tb_control_clock_step;
    localparam [8:0] SEED_PINS=9'h128;
    reg [31:0] state_q=0,fixture_state=0;
    reg before_clock=0,after_clock=0;
    reg [8:0] controls_before=SEED_PINS,controls_after=SEED_PINS;
    wire [31:0] state_next;
    wire [8:0] events,repeated;
    wire unsettled;
    dorado_control_clock_step dut(state_q,before_clock,after_clock,
        controls_before,controls_after,state_next,events,repeated,unsettled);
    reg reference_clock=0,fixture_load=0,monitor=0;
    reg [8:0] reference_controls=SEED_PINS;
    wire [31:0] reference;
    wire [8:0] reference_clocks;
    wire [4:0] reference_enables;
    wire [14:0] reference_signals;
    control_clock_reference original(fixture_load,fixture_state,reference_clock,
        reference_controls,reference,reference_clocks,reference_enables,reference_signals);
    integer counts[0:8],event_totals[0:8];
    for(genvar b=0;b<9;b=b+1) begin: count_edges
        initial counts[b]=0;
        always @(posedge reference_clocks[b]) if(monitor) counts[b]=counts[b]+1;
    end
    function automatic [31:0] normalized(input [31:0] value);
        reg [31:0] q;
        begin
            q=value;
            if(q[21]) begin q[14]=1;q[15]=1;q[16]=0;q[17]=0;end
            if(q[6] || !q[7]) begin
                q[23]=0;q[24]=0;q[25]=0;q[26]=0;q[27]=0;q[28]=0;q[29]=1;q[31]=0;
            end
            q[30]=1;normalized=q;
        end
    endfunction
    function automatic [4:0] enable_values(input [31:0] q);
        enable_values={~q[14]|q[16],~q[15]|q[17],~q[13]|q[12],~q[8]|q[10],~q[9]|q[11]};
    endfunction
    function automatic [14:0] signals(input [31:0] q,input [8:0] pins);
        reg [14:0] d;
        begin
            d[0]=(q[29]&pins[2]) | ~((~q[23]|pins[3])&(~q[23]|q[2]));
            d[1]=q[5] | (q[19]&~q[21]);d[2]=pins[7]|q[21];
            d[3]=~((pins[1]|~q[29])&~q[31]);d[4]=~pins[2]&(q[6]|~q[7]);
            d[5]=q[25];d[6]=q[23];d[7]=q[29];d[8]=q[28];d[9]=q[27];
            d[10]=q[26];d[11]=q[31];d[12]=~q[22];d[13]=~q[30];d[14]=q[24];
            signals=d;
        end
    endfunction
    integer checks=0,halted_refresh=0,mem_sh_changes=0;
    integer ordinary_intervals=0,extended_intervals=0;
    reg [31:0] random_state=32'hc0327f15;
    function automatic [31:0] random_next(input [31:0] value);
        reg [31:0] x;
        begin x=value^(value<<13);x=x^(x>>17);random_next=x^(x<<5);end
    endfunction
    task automatic seed(input [31:0] value);
        monitor=0;fixture_load=0;reference_clock=0;reference_controls=SEED_PINS;
        #1;fixture_state=0;fixture_load=1;
        #1;fixture_load=0;
        #1;fixture_state=normalized(value);fixture_load=1;
        #1;fixture_load=0;
        // Let the newly seeded Run/Stop outputs establish sPhase0 before
        // loading phase Qs that are no longer held by asynchronous reset.
        #1;fixture_load=1;
        #1;fixture_load=0;
        #1;
        if(reference!==normalized(value)) $fatal(1,"coupled seed mismatch wanted=%h got=%h",normalized(value),reference);
        state_q=normalized(value);before_clock=0;after_clock=0;
        controls_before=SEED_PINS;controls_after=SEED_PINS;monitor=1;
    endtask
    task automatic pin_event(input bit next_clock,input [8:0] next_controls);
        reg [31:0] expected;
        reg [8:0] predicted;
        integer previous_counts[0:8];
        after_clock=next_clock;controls_after=next_controls;
        #1;expected=state_next;predicted=events;
        if(unsettled || repeated) $fatal(1,"coupled solver unsettled/repeated q=%h clk=%b->%b pins=%h repeats=%b",state_q,before_clock,next_clock,next_controls,repeated);
        for(integer i=0;i<9;i=i+1) previous_counts[i]=counts[i];
        {reference_clock,reference_controls}={next_clock,next_controls};
        #1;
        if(reference!==expected)
            $fatal(1,"coupled state mismatch q=%h clk=%b->%b pins=%h got=%h expected=%h",state_q,before_clock,next_clock,next_controls,expected,reference);
        if(reference_enables!==enable_values(expected) || reference_signals!==signals(expected,next_controls))
            $fatal(1,"coupled output mismatch enables=%b/%b signals=%h/%h",reference_enables,enable_values(expected),reference_signals,signals(expected,next_controls));
        for(integer i=0;i<9;i=i+1) begin
            if(counts[i]-previous_counts[i]!=int'(predicted[i]))
                $fatal(1,"coupled clock count mismatch channel=%0d count=%0d predicted=%0d",i,counts[i]-previous_counts[i],predicted[i]);
            event_totals[i]=event_totals[i]+counts[i]-previous_counts[i];
        end
        if(expected[21] && reference_enables==5'b00111) begin
            halted_refresh=halted_refresh+1;
            if(state_q[22]!=expected[22]) mem_sh_changes=mem_sh_changes+1;
        end
        state_q=expected;before_clock=next_clock;controls_before=next_controls;checks=checks+1;
    endtask
    task automatic transfer(input bit next_clock,input [8:0] next_controls);
        pin_event(before_clock,next_controls);pin_event(next_clock,next_controls);
    endtask
    integer last_start,intervals;
    reg old_start;
    initial begin
        for(integer i=0;i<9;i=i+1) event_totals[i]=0;
        for(integer ext=0;ext<2;ext=ext+1) begin
            seed(7);last_start=-1;intervals=0;
            for(integer n=0;n<800;n=n+1) begin
                old_start=state_q[25];transfer(n[0],9'h178 | (9'(ext)<<1));
                if(!old_start && state_q[25]) begin
                    if(last_start>=0) begin
                        if(counts[0]-last_start!=(ext?6:2))
                            $fatal(1,"coupled manual cadence mismatch ext=%0d clocks=%0d",ext,counts[0]-last_start);
                        intervals=intervals+1;
                    end
                    last_start=counts[0];
                end
            end
            if(intervals<(ext?50:150)) $fatal(1,"insufficient coupled cadence coverage");
            if(ext) extended_intervals=intervals;else ordinary_intervals=intervals;
            for(integer n=0;n<160;n=n+1) transfer(n[0],9'h138 | (9'(ext)<<1));
            if(enable_values(state_q)!==5'b00111 || !state_q[21])
                $fatal(1,"coupled manual halt/refresh failed");
        end
        seed(7);
        for(integer n=0;n<160;n=n+1) transfer(n[0],9'h178);
        for(integer n=0;n<4;n=n+1) transfer(n[0],9'h158);
        for(integer n=0;n<64;n=n+1) transfer(n[0],9'h178);
        if(!state_q[6] || !state_q[7] || enable_values(state_q)!==5'b00111)
            $fatal(1,"coupled manual single-step stop failed");
        for(integer n=0;n<16;n=n+1) transfer(n[0],9'h1f8);
        if(state_q[6] || enable_values(state_q)!==0) $fatal(1,"coupled manual single-step restart failed");
        for(integer sample=0;sample<128;sample=sample+1) begin
            random_state=random_next(random_state);seed(random_state);
            for(integer n=0;n<256;n=n+1) begin
                random_state=random_next(random_state);transfer(random_state[0],random_state[9:1]);
            end
        end
        if(checks!=69864 || halted_refresh<100 || mem_sh_changes<50)
            $fatal(1,"incomplete coupled coverage checks=%0d halted=%0d mem_sh=%0d",checks,halted_refresh,mem_sh_changes);
        for(integer i=0;i<9;i=i+1) if(event_totals[i]<100) $fatal(1,"missing coupled clock %0d",i);
        $display("CONTROL_CLOCK_STEP PASS checks=%0d ordinary=%0d extended=%0d halted_refresh=%0d mem_sh_changes=%0d events=%0d,%0d,%0d,%0d,%0d,%0d,%0d,%0d,%0d",checks,ordinary_intervals,extended_intervals,halted_refresh,mem_sh_changes,event_totals[0],event_totals[1],event_totals[2],event_totals[3],event_totals[4],event_totals[5],event_totals[6],event_totals[7],event_totals[8]);
        $finish;
    end
endmodule
`default_nettype wire
