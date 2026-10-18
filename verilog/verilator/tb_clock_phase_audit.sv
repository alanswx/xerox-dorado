// Diagnostic comparison, not a synthesis replacement.
// Reference wiring: BaseBd11.sil g04.D=h04.Q', h04.D=g04.Q,
// g03.D=g04.Q, h03.D=g03.Q; g03/h03 clock on the opposite VCO phase.
// The physical VCO is idealized; the four flip-flop connections are original.
`timescale 1ps/1ps
`default_nettype none
module clock_phase_case #(parameter integer N=16)(input wire host);
    wire scp,ecp;
    cell_CLOCKGEN #(.N(N)) actual(.sys_clk(host),
        .StartClockPulse_n(scp),.EndClockPulse(ecp));
    reg vco=0;
    always #(N*1250) vco=~vco; // Johnson output period = N * 10000 ps
    reg g04=0,h04=0,g03=0,h03=0;
    always @(posedge vco) begin g04<=~h04; h04<=g04; end
    always @(negedge vco) begin g03<=g04; h03<=g03; end
    wire ref_scp=~g04,ref_ecp=h03;
    // BaseBd j02 ORs SCP', ECP and the inactive debug-clock input. Measure
    // the low window sent onward to the per-slot clock fanout.
    wire pulse=~(scp|ecp),ref_pulse=~(ref_scp|ref_ecp);
    time pulse_start=0,reference_pulse_start=0,pulse_width=0,reference_pulse_width=0;
    always @(posedge pulse) pulse_start=$time;
    always @(negedge pulse) if($time>320000) pulse_width=$time-pulse_start;
    always @(posedge ref_pulse) reference_pulse_start=$time;
    always @(negedge ref_pulse) if($time>320000) reference_pulse_width=$time-reference_pulse_start;
    time actual_ecp=0,actual_scp=0,reference_ecp=0,reference_scp=0;
    time actual_period=0,actual_lead=0,reference_period=0,reference_lead=0;
    integer actual_edges=0,reference_edges=0;
    always @(posedge ecp) actual_ecp=$time;
    always @(posedge ref_ecp) reference_ecp=$time;
    always @(posedge scp) begin
        // Let simultaneous edges settle; an equal-phase pair must report0.
        #1;
        if($time>320001) begin
            actual_period=$time-1-actual_scp;
            actual_lead=$time-1-actual_ecp;
            actual_edges=actual_edges+1;
        end
        actual_scp=$time-1;
    end
    always @(posedge ref_scp) begin
        #1;
        if($time>320001) begin
            reference_period=$time-1-reference_scp;
            reference_lead=$time-1-reference_ecp;
            reference_edges=reference_edges+1;
        end
        reference_scp=$time-1;
    end
    final begin
        if(reference_edges<2 || reference_period!=N*10000 || reference_lead!=N*1250)
            $fatal(1,"original divider reference invalid for N=%0d",N);
        $display("CLOCK_AUDIT N=%0d actual_edges=%0d actual_period_ps=%0d actual_ecp_lead_ps=%0d reference_period_ps=%0d reference_ecp_lead_ps=%0d",
            N,actual_edges,actual_period,actual_lead,reference_period,reference_lead);
        $display("CLOCK_PULSE N=%0d actual_low_ps=%0d reference_low_ps=%0d",N,pulse_width,reference_pulse_width);
    end
endmodule
module tb_clock_phase_audit;
    reg host=0; always #5000 host=~host;
    clock_phase_case #(.N(16)) n16(host);
    clock_phase_case #(.N(8)) n8(host);
    clock_phase_case #(.N(4)) n4(host);
    clock_phase_case #(.N(2)) n2(host);
    clock_phase_case #(.N(1)) n1(host);
    initial begin #1000000; $finish; end
endmodule
`default_nettype wire
