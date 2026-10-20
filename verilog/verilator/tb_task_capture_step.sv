`timescale 1ns/1ps
`default_nettype none
module tb_task_capture_step;
    localparam [75:0] SEED_PINS=76'h128|(76'h1<<66)|(76'h1<<67)|(76'h7fff<<50);
    reg [87:0] state_q=0,fixture_state=0;
    reg before_clocks=0,after_clocks=0;
    reg [75:0] pins_before=SEED_PINS,pins_after=SEED_PINS;
    wire [87:0] state_next;
    wire [24:0] events,repeated;
    wire unsettled;
    dorado_task_capture_step dut(state_q,before_clocks,after_clocks,
        pins_before,pins_after,state_next,events,repeated,unsettled);
    reg fixture_load=0,monitor=0;
    reg reference_clocks_in=0;
    reg [75:0] reference_pins=SEED_PINS;
    wire [87:0] reference;
    wire [24:0] reference_clocks;
    wire [8:0] reference_flags;
    wire [3:0] reference_next;
    task_capture_reference original(fixture_load,fixture_state,
        reference_clocks_in,reference_pins,reference,reference_clocks,reference_flags,reference_next);
    integer counts[0:24],totals[0:24];
    for(genvar i=0;i<25;i=i+1) begin: count_edges
        initial counts[i]=0;
        always @(posedge reference_clocks[i]) if(monitor) counts[i]=counts[i]+1;
    end
    function automatic [87:0] normalized(input [87:0] value);
        reg [87:0] q;
        begin
            q=value;
            if(q[21]) begin q[14]=1;q[15]=1;q[16]=0;q[17]=0;end
            if(q[6] || !q[7]) begin
                q[23]=0;q[24]=0;q[25]=0;q[26]=0;q[27]=0;q[28]=0;q[29]=1;q[31]=0;
            end
            q[30]=1;normalized=q;
        end
    endfunction
    function automatic [3:0] raw_ct(input [87:0] q);
        raw_ct={q[78],q[79],q[76],q[77]};
    endfunction
    function automatic [3:0] raw_next(input [87:0] q);
        raw_next=q[73] ? ~q[67:64] : raw_ct(q);
    endfunction
    function automatic [3:0] task_number(input [3:0] raw);
        task_number={raw[0],raw[1],raw[2],raw[3]};
    endfunction
    function automatic [8:0] flags(input [87:0] q,input [75:0] pins);
        reg [8:0] f;
        begin
            f[0]=pins[69];f[1]=pins[69]&q[75];f[2]=pins[0];
            f[3]=pins[68]&q[75];f[4]=pins[68];f[5]=|(q[43:40]^~raw_next(q));
            f[6]=pins[68]&q[75];f[7]=pins[68];f[8]=|(q[59:56]^~raw_next(q));flags=f;
        end
    endfunction
    reg [31:0] rng=32'h641025ad;
    function automatic [31:0] random_next(input [31:0] value);
        reg [31:0] x;
        begin x=value^(value<<13);x=x^(x>>17);random_next=x^(x<<5);end
    endfunction
    integer checks=0,held_task_checks=0,yield_checks=0,split_t_captures=0;
    integer suppressed_fetch=0,fetch_resume=0,coincident_next=0,address_changes=0;
    reg [15:0] tasks_seen=0;
    task automatic seed(input [87:0] value);
        monitor=0;fixture_load=0;reference_clocks_in=0;reference_pins=SEED_PINS | 76'h4;
        #20;fixture_state=0;fixture_load=1;
        #20;fixture_load=0;
        #20;fixture_state=normalized(value);fixture_load=1;
        #20;fixture_load=0;
        #20;fixture_load=1;
        #20;fixture_load=0;
        #20;reference_pins=SEED_PINS;
        #20;
        if(reference!==normalized(value)) $fatal(1,"task seed mismatch wanted=%h got=%h",normalized(value),reference);
        state_q=normalized(value);before_clocks=0;after_clocks=0;
        pins_before=SEED_PINS;pins_after=SEED_PINS;monitor=1;
    endtask
    task automatic pin_event(input next_clocks,input [75:0] next_pins);
        reg [87:0] expected,old_q;
        reg [24:0] predicted;
        integer previous_counts[0:24];
        if(!before_clocks && !next_clocks && {next_pins[69],next_pins[65]}!={pins_before[69],pins_before[65]})
            $fatal(1,"Hold/StopTasks producer changed outside its clocked high window");
        after_clocks=next_clocks;pins_after=next_pins;
        #20;expected=state_next;predicted=events;old_q=state_q;
        if(unsettled || repeated) $fatal(1,"task solver unsettled/repeated q=%h roots=%b->%b pins=%h repeats=%h",state_q,before_clocks,next_clocks,next_pins,repeated);
        for(integer i=0;i<25;i=i+1) previous_counts[i]=counts[i];
        {reference_clocks_in,reference_pins}={next_clocks,next_pins};
        #20;
        if(reference!==expected) $fatal(1,"task state mismatch q=%h roots=%b->%b pins=%h candidate=%h original=%h",state_q,before_clocks,next_clocks,next_pins,expected,reference);
        if(reference_next!==raw_next(expected)) $fatal(1,"task Next mismatch");
        if(reference_flags!==flags(expected,next_pins)) $fatal(1,"task flag mismatch got=%b expected=%b",reference_flags,flags(expected,next_pins));
        for(integer i=0;i<25;i=i+1) begin
            if(counts[i]-previous_counts[i]!=int'(predicted[i]))
                $fatal(1,"task clock mismatch channel=%0d actual=%0d predicted=%0d q=%h roots=%b->%b pins=%h->%h clocks=%h",i,counts[i]-previous_counts[i],predicted[i],old_q,before_clocks,next_clocks,pins_before,next_pins,reference_clocks);
            totals[i]=totals[i]+counts[i]-previous_counts[i];
        end
        if(predicted[12]!=predicted[16]) split_t_captures=split_t_captures+1;
        if(next_pins[68]&&old_q[75]&&(predicted[11]||predicted[16])) held_task_checks=held_task_checks+1;
        if(next_pins[68]&&!old_q[75]&&(predicted[11]||predicted[16])) yield_checks=yield_checks+1;
        if(next_pins[69]&&old_q[75]&&predicted[19]&&!predicted[20]) suppressed_fetch=suppressed_fetch+1;
        if(next_pins[69]&&!old_q[75]&&predicted[20]) fetch_resume=fetch_resume+1;
        if(raw_next(expected)!=raw_next(old_q)&&(predicted[11]||predicted[16])) coincident_next=coincident_next+1;
        if(expected[83:80]!=old_q[83:80]) address_changes=address_changes+1;
        if(expected[87:84]!=old_q[87:84]) address_changes=address_changes+1;
        state_q=expected;before_clocks=next_clocks;pins_before=next_pins;checks=checks+1;
    endtask
    task automatic transfer(input next_clocks,input [75:0] next_pins);
        reg [75:0] setup_pins;
        // Registered memory/tasking producer outputs change while the root
        // masks gated clocks. Next/SwitchUp are now generated by real FFs.
        setup_pins=next_pins;
        if(!before_clocks) begin setup_pins[69]=pins_before[69];setup_pins[65]=pins_before[65];end
        pin_event(before_clocks,setup_pins);pin_event(next_clocks,setup_pins);
        if(next_clocks && setup_pins!=next_pins) pin_event(1'b1,next_pins);
    endtask
    reg [75:0] selected;
    reg [87:0] random_state;
    reg [31:0] running_control;
    integer current_task;
    initial begin
        for(integer i=0;i<25;i=i+1) totals[i]=0;
        seed(7);
        selected=SEED_PINS;selected[19:0]=20'h178;
        // Drive the actual wakeup wires. No fixture drives Next or SwitchUp.
        for(integer task_no=0;task_no<16;task_no=task_no+1) begin
            selected[34:20]=task_no ? (15'h1<<(task_no-1)) : 0;
            selected[11:9]=3'(task_no);selected[19:12]=8'(task_no*13);
            for(integer n=0;n<64;n=n+1) transfer(n[0],selected);
            current_task=task_number(raw_ct(state_q));
            if(current_task!=task_no || task_number(~state_q[43:40])!=task_no || task_number(~state_q[59:56])!=task_no)
                $fatal(1,"manual hardware wakeup failed task=%0d current=%0d H=%0d L=%0d",task_no,current_task,task_number(~state_q[43:40]),task_number(~state_q[59:56]));
            tasks_seen[task_no]=1;
        end
        transfer(1'b0,selected);running_control=state_q[31:0];
        // Run/refresh is already qualified separately. Exercise the coupled
        // task path during ordinary/extended running, with producer controls
        // respecting their original registered timing.
        for(integer sample=0;sample<128;sample=sample+1) begin
            random_state[31:0]=running_control;
            rng=random_next(rng);random_state[63:32]=rng;
            rng=random_next(rng);random_state[87:64]=rng[23:0];seed(random_state);
            for(integer n=0;n<256;n=n+1) begin
                rng=random_next(rng);selected[31:0]=rng;
                selected[8:0]=9'h178 | (9'(rng[1])<<1);
                rng=random_next(rng);selected[63:32]=rng;
                rng=random_next(rng);selected[75:64]={1'b0,rng[10:0]};
                if(selected[70]) selected[74:71]=0;
                rng=random_next(rng);transfer(rng[0],selected);
            end
        end
        if(tasks_seen!=16'hffff || held_task_checks<100 || yield_checks<100 || split_t_captures<25 || suppressed_fetch<100 || fetch_resume<100 || coincident_next<100 || address_changes<1000)
            $fatal(1,"incomplete task/processor pipeline coverage");
        for(integer i=0;i<25;i=i+1) if(totals[i]<100) $fatal(1,"missing task clock %0d",i);
        $display("TASK_CAPTURE_STEP PASS checks=%0d tasks=%h held=%0d yield=%0d split_t=%0d fetch_held=%0d fetch_resumed=%0d coincident_next=%0d address_changes=%0d",checks,tasks_seen,held_task_checks,yield_checks,split_t_captures,suppressed_fetch,fetch_resume,coincident_next,address_changes);
        $finish;
    end
endmodule
`default_nettype wire
