`timescale 1ns/1ps
`default_nettype none
module tb_processor_capture_step;
    localparam [27:0] SEED_PINS=28'h128;
    reg [63:0] state_q=0,fixture_state=0;
    reg before_clocks=0,after_clocks=0;
    reg [27:0] pins_before=SEED_PINS,pins_after=SEED_PINS;
    wire [63:0] state_next;
    wire [21:0] events,repeated;
    wire unsettled;
    dorado_processor_capture_step dut(state_q,before_clocks,after_clocks,
        pins_before,pins_after,state_next,events,repeated,unsettled);
    reg fixture_load=0,monitor=0;
    reg reference_clocks_in=0;
    reg [27:0] reference_pins=SEED_PINS;
    wire [63:0] reference;
    wire [21:0] reference_clocks;
    wire [8:0] reference_flags;
    processor_capture_reference original(fixture_load,fixture_state,
        reference_clocks_in,reference_pins,reference,reference_clocks,reference_flags);
    integer counts[0:21],totals[0:21];
    for(genvar i=0;i<22;i=i+1) begin: count_edges
        initial counts[i]=0;
        always @(posedge reference_clocks[i]) if(monitor) counts[i]=counts[i]+1;
    end
    function automatic [63:0] normalized(input [63:0] value);
        reg [63:0] q;
        begin
            q=value;
            if(q[21]) begin q[14]=1;q[15]=1;q[16]=0;q[17]=0;end
            if(q[6] || !q[7]) begin
                q[23]=0;q[24]=0;q[25]=0;q[26]=0;q[27]=0;q[28]=0;q[29]=1;q[31]=0;
            end
            q[30]=1;normalized=q;
        end
    endfunction
    function automatic [8:0] flags(input [63:0] q,input [27:0] pins);
        reg [8:0] f;
        begin
            f[0]=pins[26];f[1]=pins[26]&~pins[27];f[2]=pins[0];
            f[3]=pins[25]&pins[24];f[4]=pins[25];f[5]=|(q[43:40]^~pins[23:20]);
            f[6]=pins[25]&pins[24];f[7]=pins[25];f[8]=|(q[59:56]^~pins[23:20]);flags=f;
        end
    endfunction
    reg [31:0] rng=32'h641025ad;
    function automatic [31:0] random_next(input [31:0] value);
        reg [31:0] x;
        begin x=value^(value<<13);x=x^(x>>17);random_next=x^(x<<5);end
    endfunction
    integer checks=0,held_task_checks=0,yield_checks=0,split_t_captures=0;
    integer suppressed_fetch=0,fetch_resume=0;
    reg [15:0] tasks_seen=0;
    task automatic seed(input [63:0] value);
        monitor=0;fixture_load=0;reference_clocks_in=0;reference_pins=SEED_PINS | 28'h4;
        #1;fixture_state=0;fixture_load=1;
        #1;fixture_load=0;
        #1;fixture_state=normalized(value);fixture_load=1;
        #1;fixture_load=0;
        #1;fixture_load=1;
        #1;fixture_load=0;
        #1;reference_pins=SEED_PINS;
        #1;
        if(reference!==normalized(value)) $fatal(1,"processor seed mismatch wanted=%h got=%h",normalized(value),reference);
        state_q=normalized(value);before_clocks=0;after_clocks=0;
        pins_before=SEED_PINS;pins_after=SEED_PINS;monitor=1;
    endtask
    task automatic pin_event(input next_clocks,input [27:0] next_pins);
        reg [63:0] expected,old_q;
        reg [21:0] predicted;
        integer previous_counts[0:21];
        if(!before_clocks && !next_clocks && next_pins[27:26]!=pins_before[27:26])
            $fatal(1,"input hold/priority source changed outside its clocked high window");
        after_clocks=next_clocks;pins_after=next_pins;
        #1;expected=state_next;predicted=events;old_q=state_q;
        if(unsettled || repeated) $fatal(1,"processor solver unsettled/repeated q=%h roots=%b->%b pins=%h repeats=%h",state_q,before_clocks,next_clocks,next_pins,repeated);
        for(integer i=0;i<22;i=i+1) previous_counts[i]=counts[i];
        {reference_clocks_in,reference_pins}={next_clocks,next_pins};
        #1;
        if(reference!==expected) $fatal(1,"processor state mismatch q=%h roots=%b->%b pins=%h got=%h expected=%h",state_q,before_clocks,next_clocks,next_pins,expected,reference);
        if(reference_flags!==flags(expected,next_pins)) $fatal(1,"processor flag mismatch got=%b expected=%b",reference_flags,flags(expected,next_pins));
        for(integer i=0;i<22;i=i+1) begin
            if(counts[i]-previous_counts[i]!=int'(predicted[i]))
                $fatal(1,"processor clock mismatch channel=%0d actual=%0d predicted=%0d q=%h roots=%b->%b pins=%h->%h clocks=%h",i,counts[i]-previous_counts[i],predicted[i],old_q,before_clocks,next_clocks,pins_before,next_pins,reference_clocks);
            totals[i]=totals[i]+counts[i]-previous_counts[i];
        end
        if(predicted[12]!=predicted[16]) split_t_captures=split_t_captures+1;
        if(next_pins[25]&&next_pins[24]&&(predicted[11]||predicted[16])) begin
            if(expected[43:40]!==old_q[43:40] || expected[59:56]!==old_q[59:56])
                $fatal(1,"held task changed");
            held_task_checks=held_task_checks+1;
        end
        if(next_pins[25]&&!next_pins[24]&&(predicted[11]||predicted[16])) yield_checks=yield_checks+1;
        if(next_pins[26]&&!next_pins[27]&&predicted[19]&&!predicted[20]) suppressed_fetch=suppressed_fetch+1;
        if(next_pins[26]&&next_pins[27]&&predicted[20]) fetch_resume=fetch_resume+1;
        state_q=expected;before_clocks=next_clocks;pins_before=next_pins;checks=checks+1;
    endtask
    task automatic transfer(input next_clocks,input [27:0] next_pins);
        reg [27:0] setup_pins;
        // HM4.9: Hold is clocked at t1. ContA f15 clocks SwitchUp on clk0'Cb.
        // These producer FF outputs arrive after a common positive root edge,
        // while the raw clock masks the fetch-clock OR gates. They are not
        // arbitrary asynchronous pins that may swap during the low window.
        setup_pins=next_pins;
        if(!before_clocks) setup_pins[27:26]=pins_before[27:26];
        pin_event(before_clocks,setup_pins);pin_event(next_clocks,setup_pins);
        if(next_clocks && setup_pins[27:26]!=next_pins[27:26]) pin_event(1'b1,next_pins);
    endtask
    reg [27:0] selected;
    reg [63:0] capture_seed;
    integer n0,nh,nl;
    initial begin
        for(integer i=0;i<22;i=i+1) totals[i]=0;
        // A valid pre-edge phase state can give the high clock a fresh edge
        // while the low SHCP is already high. Exercise all LC values, normal
        // and FF075 decode, and all task pin patterns. A shared capture enable
        // would overwrite the low controls in these directed cases.
        for(integer lc=0;lc<8;lc=lc+1)
            for(integer md=0;md<2;md=md+1)
                for(integer task_no=0;task_no<16;task_no=task_no+1) begin
                    capture_seed=(64'h1<<30)|(64'h1<<25)|(64'h1<<23)|(64'h1<<13)|
                        (64'h1<<9)|(64'h1<<8)|(64'h1<<7)|(64'h1<<5);
                    capture_seed[43:40]=~4'(task_no^1);capture_seed[59:56]=~4'(task_no^1);
                    seed(capture_seed);
                    selected=28'h178|(28'(lc)<<9)|(28'(md?8'o075:0)<<12)|(28'(task_no)<<20);
                    transfer(1'b1,selected);
                end
        seed(7);
        // Exercise every task pin pattern, then hold the current task, then
        // allow a priority task to load while PRhold remains asserted.
        for(integer task_no=0;task_no<16;task_no=task_no+1) begin
            selected=28'h178 | (28'(task_no)<<20) | (28'(task_no%8)<<9) | (28'(task_no*13)<<12);
            for(integer n=0;n<64;n=n+1) transfer(n[0],selected);
            if(state_q[43:40]!==~4'(task_no) || state_q[59:56]!==~4'(task_no))
                $fatal(1,"manual task load failed");
            tasks_seen[task_no]=1;
        end
        selected=28'h178 | (28'h3<<20) | (28'h1<<25) | (28'h1<<24);
        for(integer n=0;n<64;n=n+1) transfer(n[0],selected);
        if(state_q[43:40]!==4'h0 || state_q[59:56]!==4'h0) $fatal(1,"manual held task changed");
        selected[24]=0;
        for(integer n=0;n<64;n=n+1) transfer(n[0],selected);
        if(state_q[43:40]!==4'hc || state_q[59:56]!==4'hc) $fatal(1,"manual held priority yield failed");
        // ContA Hold is independent of PRhold. It suppresses repeated fetch
        // clocks while the controller continues running, until SwitchUp.
        selected[25]=0;selected[26]=1;selected[27]=0;
        for(integer n=0;n<32;n=n+1) transfer(n[0],selected);
        n0=counts[19];nh=counts[20];nl=counts[21];
        for(integer n=0;n<64;n=n+1) transfer(n[0],selected);
        if(counts[19]<=n0 || counts[20]!=nh || counts[21]!=nl) $fatal(1,"manual fetch hold failed");
        selected[27]=1;
        for(integer n=0;n<64;n=n+1) transfer(n[0],selected);
        if(counts[20]<=nh || counts[21]<=nl) $fatal(1,"manual priority fetch resume failed");
        for(integer sample=0;sample<128;sample=sample+1) begin
            rng=random_next(rng);selected=rng[27:0];rng=random_next(rng);seed({rng,selected,4'b0000});
            for(integer n=0;n<256;n=n+1) begin
                rng=random_next(rng);selected=rng[27:0];rng=random_next(rng);
                transfer(rng[0],selected);
            end
        end
        if(tasks_seen!=16'hffff || held_task_checks<100 || yield_checks<100 || split_t_captures<100 || suppressed_fetch<10 || fetch_resume<10)
            $fatal(1,"incomplete processor capture coverage tasks=%h held=%0d yield=%0d split=%0d suppressed=%0d resumed=%0d",tasks_seen,held_task_checks,yield_checks,split_t_captures,suppressed_fetch,fetch_resume);
        for(integer i=0;i<22;i=i+1) if(totals[i]<100) $fatal(1,"missing processor clock %0d",i);
        $display("PROCESSOR_CAPTURE_STEP PASS checks=%0d tasks=%h held=%0d yield=%0d split_t=%0d fetch_held=%0d fetch_resumed=%0d",checks,tasks_seen,held_task_checks,yield_checks,split_t_captures,suppressed_fetch,fetch_resume);
        $finish;
    end
endmodule
`default_nettype wire
