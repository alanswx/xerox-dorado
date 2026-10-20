`timescale 1ns/1ps
`default_nettype none
module tb_t_file_step;
    localparam [126:0] SEED_PINS=127'h128|(127'h1<<66)|(127'h1<<67)|(127'h7fff<<50);
    reg [403:0] state_q=0,fixture_state=0;
    reg before_clocks=0,after_clocks=0;
    reg [126:0] pins_before=SEED_PINS,pins_after=SEED_PINS;
    wire [403:0] state_next;
    wire [24:0] events,repeated;
    wire unsettled;
    wire [1:0] write_n,latch_n,ref_write_n,ref_latch_n,ref_hold_clocks,t_latch_n,bypass,ref_t_latch_n,ref_bypass;
    dorado_t_file_step dut(state_q,before_clocks,after_clocks,
        pins_before,pins_after,state_next,events,repeated,unsettled,write_n,latch_n,t_latch_n,bypass);
    reg fixture_load=0,monitor=0;
    reg reference_clocks_in=0;
    reg [126:0] reference_pins=SEED_PINS;
    wire [403:0] reference;
    wire [24:0] reference_clocks;
    wire [8:0] reference_flags;
    wire [3:0] reference_next;
    t_file_reference original(fixture_load,fixture_state,
        reference_clocks_in,reference_pins,reference,reference_clocks,reference_flags,reference_next,ref_write_n,ref_latch_n,ref_hold_clocks,ref_t_latch_n,ref_bypass);
    integer counts[0:24],totals[0:24];
    for(genvar i=0;i<25;i=i+1) begin: count_edges
        initial counts[i]=0;
        always @(posedge reference_clocks[i]) if(monitor) counts[i]=counts[i]+1;
    end
    function automatic [403:0] normalized(input [403:0] value);
        reg [403:0] q;
        begin
            q=value;
            if(q[21]) begin q[14]=1;q[15]=1;q[16]=0;q[17]=0;end
            if(q[6] || !q[7]) begin
                q[23]=0;q[24]=0;q[25]=0;q[26]=0;q[27]=0;q[28]=0;q[29]=1;q[31]=0;
            end
            q[30]=1;normalized=q;
        end
    endfunction
    function automatic [3:0] raw_ct(input [403:0] q);
        raw_ct={q[78],q[79],q[76],q[77]};
    endfunction
    function automatic [3:0] raw_next(input [403:0] q);
        raw_next=q[73] ? ~q[67:64] : raw_ct(q);
    endfunction
    function automatic [3:0] task_number(input [3:0] raw);
        task_number={raw[0],raw[1],raw[2],raw[3]};
    endfunction
    function automatic [8:0] flags(input [403:0] q,input [126:0] pins);
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
    integer transparent_updates=0,memory_updates=0,md_updates=0,pd_updates=0,closed_probes=0,hold_blocks=0,ff075_seen=0;
    reg [7:0] lc_seen=0;
    integer checks=0,held_task_checks=0,yield_checks=0,split_t_captures=0;
    integer suppressed_fetch=0,fetch_resume=0,coincident_next=0,address_changes=0;
    reg [15:0] tasks_seen=0;
    task automatic seed(input [403:0] value);
        monitor=0;fixture_load=0;reference_clocks_in=1;reference_pins=SEED_PINS | 127'h4;
        #20;fixture_state=0;fixture_load=1;
        #20;fixture_load=0;
        #20;fixture_state=normalized(value);fixture_load=1;
        #20;fixture_load=0;
        #20;fixture_load=1;
        #20;fixture_load=0;
        #20;reference_pins=SEED_PINS;
        #20;
        if(reference!==normalized(value)) $fatal(1,"T-file seed mismatch wanted=%h got=%h",normalized(value),reference);
        state_q=normalized(value);before_clocks=1;after_clocks=1;
        pins_before=SEED_PINS;pins_after=SEED_PINS;monitor=1;
    endtask
    task automatic pin_event(input next_clocks,input [126:0] next_pins);
        reg [403:0] expected,old_q;
        reg [24:0] predicted;
        reg [1:0] expected_write,expected_latch,expected_t_latch,expected_bypass;
        integer previous_counts[0:24];
        if(!before_clocks && !next_clocks && {next_pins[69],next_pins[65]}!={pins_before[69],pins_before[65]})
            $fatal(1,"Hold/StopTasks producer changed outside its clocked high window");
        after_clocks=next_clocks;pins_after=next_pins;
        #20;expected=state_next;predicted=events;old_q=state_q;expected_write=write_n;expected_latch=latch_n;expected_t_latch=t_latch_n;expected_bypass=bypass;
        if(unsettled || repeated) $fatal(1,"task solver unsettled/repeated q=%h roots=%b->%b pins=%h repeats=%h",state_q,before_clocks,next_clocks,next_pins,repeated);
        for(integer i=0;i<25;i=i+1) previous_counts[i]=counts[i];
        {reference_clocks_in,reference_pins}={next_clocks,next_pins};
        #20;
        if(reference!==expected) $fatal(1,"T-file state mismatch q=%h roots=%b->%b pins=%h candidate=%h original=%h",state_q,before_clocks,next_clocks,next_pins,expected,reference);
        if((expected_write | expected_latch)!=2'b11) $fatal(1,"T-file data/write windows overlap");
        if(ref_t_latch_n!==expected_t_latch || ref_bypass!==expected_bypass) $fatal(1,"T-file read gate mismatch");
        if(ref_write_n!==expected_write || ref_latch_n!==expected_latch)
            $fatal(1,"T-file window mismatch write original=%b candidate=%b latch original=%b candidate=%b",ref_write_n,expected_write,ref_latch_n,expected_latch);
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
        lc_seen[next_pins[11:9]]=1;
        if(next_pins[19:12]==8'o075) ff075_seen=ff075_seen+1;
        if(expected_latch==2'b11 && (next_pins[107:76]!=pins_before[107:76])) closed_probes=closed_probes+1;
        if(!expected_latch[0] && expected[107:100]!=old_q[107:100]) begin
            transparent_updates=transparent_updates+1;
            if(expected[36]) md_updates=md_updates+1;else pd_updates=pd_updates+1;
        end
        if(!expected_latch[1] && expected[115:108]!=old_q[115:108]) begin
            transparent_updates=transparent_updates+1;
            if(expected[52]) md_updates=md_updates+1;else pd_updates=pd_updates+1;
        end
        if(!after_clocks && expected[23] && ((expected[93]&&!expected[38]) || (expected[99]&&!expected[54]))) hold_blocks=hold_blocks+1;
        if(expected_latch[0] && expected[107:100]!==old_q[107:100]) $fatal(1,"T-file high latch changed while closed");
        if(expected_latch[1] && expected[115:108]!==old_q[115:108]) $fatal(1,"T-file low latch changed while closed");
        if(expected_write[0] && expected_write[1] && expected[371:116]!==old_q[371:116]) $fatal(1,"T-file changed while both write gates closed");
        for(integer slot=0;slot<16;slot=slot+1) if(expected[116+16*slot+:16]!=old_q[116+16*slot+:16]) memory_updates=memory_updates+1;
        state_q=expected;before_clocks=next_clocks;pins_before=next_pins;checks=checks+1;
    endtask
    task automatic transfer(input next_clocks,input [126:0] next_pins);
        reg [126:0] setup_pins;
        // Registered memory/tasking producer outputs change while the root
        // masks gated clocks. Next/SwitchUp are now generated by real FFs.
        setup_pins=next_pins;
        if(!before_clocks) begin setup_pins[69]=pins_before[69];setup_pins[65]=pins_before[65];end
        pin_event(before_clocks,setup_pins);pin_event(next_clocks,setup_pins);
        if(next_clocks && setup_pins!=next_pins) pin_event(1'b1,next_pins);
    endtask
    integer h_windows=0,l_windows=0,h_adr_changes=0,l_adr_changes=0;
    reg h_open=0,l_open=0;
    reg [3:0] h_address=0,l_address=0;
    wire [3:0] actual_h_address={original.h.l03.p10,original.h.l03.p9,original.h.l03.p7,original.h.l03.p6};
    wire [3:0] actual_l_address={original.l.l03.p10,original.l.l03.p9,original.l.l03.p7,original.l.l03.p6};
    always @(ref_write_n[0] or actual_h_address) begin
        if(!monitor) h_open=0;
        else if(!ref_write_n[0]) begin
            if(h_open && actual_h_address!=h_address) h_adr_changes=h_adr_changes+1;
            if(!h_open) h_windows=h_windows+1;
            h_open=1;h_address=actual_h_address;
        end else h_open=0;
    end
    always @(ref_write_n[1] or actual_l_address) begin
        if(!monitor) l_open=0;
        else if(!ref_write_n[1]) begin
            if(l_open && actual_l_address!=l_address) l_adr_changes=l_adr_changes+1;
            if(!l_open) l_windows=l_windows+1;
            l_open=1;l_address=actual_l_address;
        end else l_open=0;
    end
    reg [126:0] selected;
    reg [403:0] initial_state;
    reg [255:0] held_memory;
    integer manual_hold_checks=0;
    reg [15:0] signatures[0:15];
    integer current_task;
    task automatic cycle(input integer transitions);
        for(integer n=0;n<transitions;n=n+1) transfer(!before_clocks,selected);
    endtask
    initial begin
        for(integer i=0;i<25;i=i+1) totals[i]=0;
        initial_state=7 | (404'h1<<38) | (404'h1<<54) | (404'h1<<75) | (404'h1<<93) | (404'h1<<99);
        initial_state[43:40]=15;initial_state[47:44]=15;
        initial_state[59:56]=15;initial_state[63:60]=15;
        initial_state[83:80]=15;initial_state[87:84]=15;
        for(integer slot=0;slot<16;slot=slot+1) initial_state[116+16*slot+:16]=16'ha100 ^ 16'(slot*19);
        seed(initial_state);
        selected=SEED_PINS;selected[19:0]=20'h178;selected[126:76]=0;
        cycle(64);
        for(integer task_no=0;task_no<16;task_no=task_no+1) begin
            selected[11:9]=0;cycle(16);
            selected[34:20]=task_no ? (15'h1<<(task_no-1)) : 0;
            cycle(64);
            current_task=task_number(raw_ct(state_q));
            if(current_task!=task_no) $fatal(1,"manual T-file wakeup failed task=%0d current=%0d",task_no,current_task);
            selected[91:76]=16'h9c00 ^ 16'(task_no*16'h121);
            selected[107:92]=16'h53ff ^ 16'(task_no*16'h13b);
            selected[19:12]=(task_no%3==2) ? 8'o075 : 0;
            selected[11:9]=(task_no%3==2) ? 5 : ((task_no%3==1) ? 3 : 1);
            signatures[task_no]=(task_no%3) ? selected[107:92] : selected[91:76];
            cycle(64);selected[11:9]=0;cycle(16);
            // Rewrite the same task through the explicit TgetsMd FF075 path.
            selected[107:92]=selected[107:92]^16'h5a5a;
            selected[91:76]=selected[91:76]^16'ha5a5;
            selected[19:12]=8'o075;selected[11:9]=5;
            signatures[task_no]=selected[107:92];
            cycle(32);selected[11:9]=0;cycle(16);
            // A held task may update its transparent data latches, but its
            // delayed Hold must keep the SRAM write window closed.
            selected[68]=1;cycle(16);held_memory=state_q[371:116];
            selected[19:12]=0;selected[11:9]=1;selected[91:76]=~signatures[task_no];
            cycle(32);
            if(state_q[371:116]!==held_memory) $fatal(1,"manual held T-file changed");
            manual_hold_checks=manual_hold_checks+1;
            selected[68]=0;selected[11:9]=0;cycle(16);
            // Release/drain, then execute a fresh write of the intended word.
            selected[19:12]=8'o075;selected[11:9]=5;
            cycle(32);selected[11:9]=0;cycle(16);
            for(integer previous_task=0;previous_task<=task_no;previous_task=previous_task+1)
                if(state_q[116+16*(15-previous_task)+:16]!==signatures[previous_task])
                    $fatal(1,"manual task data isolation failed task=%0d got=%h expected=%h",previous_task,state_q[116+16*(15-previous_task)+:16],signatures[previous_task]);
            if(state_q[403:388]!==signatures[task_no]) $fatal(1,"manual visible T read failed task=%0d got=%h expected=%h",task_no,state_q[403:388],signatures[task_no]);
            tasks_seen[task_no]=1;
        end
        // Revisit every saved task with writes off, including a switch down
        // from task 15 to emulator task 0. Read T through the real two latch
        // stages, not by looking only at the SRAM image.
        held_memory=state_q[371:116];selected[117]=0;selected[11:9]=0;
        for(integer task_no=0;task_no<16;task_no=task_no+1) begin
            selected[66]=0;selected[67]=0;
            selected[34:20]=task_no ? (15'h1<<(task_no-1)) : 0;
            cycle(64);selected[66]=1;selected[67]=1;cycle(16);
            if(task_number(raw_ct(state_q))!=task_no || state_q[403:388]!==signatures[task_no])
                $fatal(1,"manual saved T read failed task=%0d got=%h expected=%h",task_no,state_q[403:388],signatures[task_no]);
        end
        if(state_q[371:116]!==held_memory) $fatal(1,"manual read-only task revisit wrote SRAM");
        selected[117]=1;
        for(integer sign_bit=0;sign_bit<2;sign_bit=sign_bit+1) begin
            selected[118]=sign_bit[0];selected[126:119]=sign_bit ? 8'h96 : 8'h69;
            cycle(16);
            if(state_q[403:388]!=={selected[126:119],{8{sign_bit[0]}}}) $fatal(1,"manual IFU T read failed");
        end
        if(state_q[371:116]!==held_memory) $fatal(1,"manual IFU read wrote SRAM");
        selected[126:117]=0;
        // Random data/control while all writes are attributed by the real
        // address pipeline. Controller runs normally; no forced task sources.
        for(integer n=0;n<4096;n=n+1) begin
            rng=random_next(rng);selected[31:0]=rng;selected[8:0]=9'h178 | (9'(rng[1])<<1);
            rng=random_next(rng);selected[63:32]=rng;
            rng=random_next(rng);selected[95:64]=rng;selected[75]=0;
            if(selected[70]) selected[74:71]=0;
            rng=random_next(rng);selected[126:96]=rng[30:0];selected[108]=0;
            rng=random_next(rng);transfer(rng[0],selected);
        end
        if(tasks_seen!=16'hffff || manual_hold_checks!=16 || lc_seen!=8'hff || ff075_seen<1 || transparent_updates<100 || md_updates<100 || pd_updates<100 || closed_probes<100 || hold_blocks<100 || memory_updates<100 || h_windows<100 || l_windows<100 || h_adr_changes || l_adr_changes)
            $fatal(1,"incomplete or invalid T-file coverage LC=%h FF075=%0d transparent=%0d md=%0d pd=%0d closed=%0d held=%0d memory=%0d windows=%0d/%0d addr=%0d/%0d",lc_seen,ff075_seen,transparent_updates,md_updates,pd_updates,closed_probes,hold_blocks,memory_updates,h_windows,l_windows,h_adr_changes,l_adr_changes);
        $display("T_FILE_STEP PASS checks=%0d tasks=%h transparent=%0d updates=%0d Hwindows=%0d Lwindows=%0d Haddr_moves=%0d Laddr_moves=%0d md=%0d pd=%0d closed=%0d held=%0d LC=%h FF075=%0d manual_holds=%0d",checks,tasks_seen,transparent_updates,memory_updates,h_windows,l_windows,h_adr_changes,l_adr_changes,md_updates,pd_updates,closed_probes,hold_blocks,lc_seen,ff075_seen,manual_hold_checks);
        $finish;
    end
endmodule
`default_nettype wire
