// Independent original pin/gate reference. Fixture loading belongs only to
// the test oracle; no such input is added to the candidate or native machine.
`timescale 1ns/1ps
`default_nettype none
module original_phase_ff (
    input wire c,d,s,r,fixture_load,fixture_data,
    output reg q
);
    // Motorola MC10231: set dominates reset, then positive-edge D capture.
    always @(posedge c or posedge s or posedge r or posedge fixture_load)
        if (fixture_load) q <= fixture_data;
        else if (s) q <= 1'b1;
        else if (r) q <= 1'b0;
        else q <= d;
endmodule

module original_control_phases (
    input wire pre_clock,main_clock,phase0_init,im_tpc,
    input wire fixture_load,
    input wire [6:0] fixture_state,
    output wire [6:0] state_q,
    output wire d_start
);
    wire p0=state_q[4],p1=state_q[3],p2=state_q[2];
    wire p3=state_q[1],p4=state_q[0],sc=state_q[5];
    wire p0_n=~p0,p4_n=~p4;
    wire j05_noninverting,phase1_data;
    // Actual combinational pin functions, independently of candidate RTL.
    cell_MC10117 j05(.p2(j05_noninverting),.p3(d_start),
        .p4(im_tpc),.p5(p0_n),.p6(1'b0),.p7(p4_n),.p9(1'b0),
        .p10(1'b0),.p11(1'b0),.p12(1'b0),.p13(1'b0));
    cell_MC10104 i04(.p3(phase1_data),.p6(j05_noninverting),.p7(p0),
        .p4(1'b0),.p5(1'b0),.p10(1'b0),.p11(1'b0),
        .p12(1'b0),.p13(1'b0),.p16(1'b0));
    original_phase_ff k03_b(pre_clock,d_start,1'b0,phase0_init,
        fixture_load,fixture_state[6],state_q[6]);
    original_phase_ff k04_b(main_clock,d_start,1'b0,phase0_init,
        fixture_load,fixture_state[5],state_q[5]);
    original_phase_ff l07_b(main_clock,sc,phase0_init,1'b0,
        fixture_load,fixture_state[4],state_q[4]);
    original_phase_ff l07_a(main_clock,phase1_data,1'b0,phase0_init,
        fixture_load,fixture_state[3],state_q[3]);
    original_phase_ff k05_b(main_clock,p1,1'b0,phase0_init,
        fixture_load,fixture_state[2],state_q[2]);
    original_phase_ff k05_a(main_clock,p2,1'b0,phase0_init,
        fixture_load,fixture_state[1],state_q[1]);
    original_phase_ff j01_b(main_clock,p3,1'b0,phase0_init,
        fixture_load,fixture_state[0],state_q[0]);
endmodule

module tb_control_phase_next;
    reg [6:0] state_q=0,fixture_state=0;
    reg capture_pre=0,capture_main=0,phase0_init=0,im_tpc=0;
    wire d_start;
    wire [6:0] state_next;
    dorado_control_phase_next dut(state_q,capture_pre,capture_main,
        phase0_init,im_tpc,d_start,state_next);
    reg pre_clock=0,main_clock=0,reference_init=0,fixture_load=0;
    wire [6:0] reference;
    wire reference_d;
    original_control_phases original(pre_clock,main_clock,reference_init,im_tpc,
        fixture_load,fixture_state,reference,reference_d);
    integer checks=0,known_cases=0,unknown_cases=0;
    integer ordinary_intervals=0,extended_intervals=0,held=0,partial=0;
    reg [31:0] random_state=32'h54d02f19;
    function automatic [31:0] random_next(input [31:0] value);
        reg [31:0] x;
        begin x=value^(value<<13);x=x^(x>>17);random_next=x^(x<<5);end
    endfunction

    task automatic seed(input [6:0] value);
        pre_clock=0;main_clock=0;reference_init=0;fixture_load=0;
        #1;fixture_state=value;fixture_load=1;
        #1;fixture_load=0;state_q=value;
        #1;
        if(reference!==value) $fatal(1,"fixture did not establish original state");
    endtask

    task automatic transfer(input bit pre,input bit main,input bit init,
                            input logic extended);
        reg [6:0] expected;
        pre_clock=0;main_clock=0;reference_init=0;
        capture_pre=pre;capture_main=main;phase0_init=init;im_tpc=extended;
        #1;
        if(d_start!==reference_d) $fatal(1,"original j05 D differs state=%b ext=%b",state_q,extended);
        expected=state_next;
        // Schedule original clock/set/reset pins, rather than copying the
        // candidate next-state equations into the oracle.
        reference_init=init;pre_clock=pre;main_clock=main;
        #1;
        if(reference!==expected)
            $fatal(1,"phase mismatch state=%b ext=%b pre=%b main=%b init=%b got=%b expected=%b",
                state_q,extended,pre,main,init,expected,reference);
        if(!pre&&!main&&!init) begin
            if(expected!==state_q) $fatal(1,"hold changed original state");
            held=held+1;
        end
        if(pre!=main&&!init) partial=partial+1;
        state_q=expected;checks=checks+1;
    endtask

    // Base-four digits exercise X and Z in each of the seven state bits and
    // the instruction decode. Capture and initialization controls stay known.
    function automatic logic four_value(input integer digit);
        case(digit) 0:four_value=0;1:four_value=1;2:four_value=1'bx;3:four_value=1'bz;endcase
    endfunction
    reg [6:0] selected;
    integer digits,last_start,intervals;
    initial begin
        for(integer old=0;old<128;old=old+1)
            for(integer ext=0;ext<2;ext=ext+1)
                for(integer enables=0;enables<4;enables=enables+1)
                    for(integer init=0;init<2;init=init+1) begin
                        seed(7'(old));
                        transfer(enables[1],enables[0],init[0],ext[0]);
                        known_cases=known_cases+1;
                    end
        if($test$plusargs("four_state")) begin
            for(integer code=0;code<65536;code=code+1) begin
                digits=code;
                for(integer b=0;b<7;b=b+1) begin
                    selected[b]=four_value(digits%4);digits=digits/4;
                end
                for(integer enables=0;enables<4;enables=enables+1)
                    for(integer init=0;init<2;init=init+1) begin
                        seed(selected);
                        transfer(enables[1],enables[0],init[0],four_value(digits));
                        unknown_cases=unknown_cases+1;
                    end
            end
        end
        // Closed-loop original and candidate sequencers: one seed, then
        // successive clock events with no fixture injection between them.
        for(integer ext=0;ext<2;ext=ext+1) begin
            seed(0);transfer(0,0,1,ext[0]);last_start=-1;intervals=0;
            for(integer edge_no=0;edge_no<200;edge_no=edge_no+1) begin
                #1; // Let D decode the state committed by the prior event.
                if(state_q[5]===1'b0 && d_start===1'b1) begin
                    if(last_start>=0) begin
                        if(edge_no-last_start!=(ext?6:2))
                            $fatal(1,"manual two/six-clock interval changed ext=%0d clocks=%0d",ext,edge_no-last_start);
                        intervals=intervals+1;
                    end
                    last_start=edge_no;
                end
                transfer(1,1,0,ext[0]);
            end
            if(intervals<30) $fatal(1,"insufficient manual cadence coverage");
            if(ext) extended_intervals=intervals;else ordinary_intervals=intervals;
        end
        seed(0);transfer(0,0,1,0);
        for(integer n=0;n<20000;n=n+1) begin
            random_state=random_next(random_state);
            transfer(random_state[0],random_state[1],random_state[7:2]==0,random_state[8]);
        end
        if(known_cases!=2048 || ordinary_intervals<90 || extended_intervals<30 || held<4000 || partial<8000)
            $fatal(1,"incomplete phase coverage");
        $display("CONTROL_PHASE_NEXT PASS checks=%0d known=%0d four_state=%0d ordinary_intervals=%0d extended_intervals=%0d held=%0d partial=%0d",
            checks,known_cases,unknown_cases,ordinary_intervals,extended_intervals,held,partial);
        $finish;
    end
endmodule
`default_nettype wire
