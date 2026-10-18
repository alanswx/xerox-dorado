// Independent event-driven original-divider reference versus explicit enables.
`timescale 1ns/1ps
`default_nettype none
module clock_phases_case #(parameter integer TICKS=1) (
    input wire host, reset, advance,
    output reg done=0
);
    wire [2:0] phase;
    wire scp,ecp,slot_clock,sr,sf,er,ef,cr,cf;
    dorado_clock_phases #(.TICKS_PER_SLOT(TICKS)) dut (
        .clk(host),.reset(reset),.advance(advance),.phase(phase),
        .scp_n(scp),.ecp(ecp),.slot_clock(slot_clock),
        .scp_rise(sr),.scp_fall(sf),.ecp_rise(er),.ecp_fall(ef),
        .slot_rise(cr),.slot_fall(cf)
    );

    // Actual four-flop connections from BaseBd11, driven by opposite VCO
    // edges. No phase lookup or oversampled chip-clock detectors here.
    reg vco=1, g04=0,h04=0,g03=0,h03=1;
    integer divider=0;
    always @(posedge host) begin
        if(reset) begin vco=1; divider=0; end
        else if(advance) begin
            if(divider==TICKS-1) begin vco=~vco; divider=0; end
            else divider=divider+1;
        end
    end
    always @(posedge vco or posedge reset)
        if(reset) begin g04<=0; h04<=0; end
        else begin g04<=~h04; h04<=g04; end
    always @(negedge vco or posedge reset)
        if(reset) begin g03<=0; h03<=1; end
        else begin g03<=g04; h03<=g03; end
    wire ref_scp=~g04, ref_ecp=h03;
    wire ref_clock=ref_scp | ref_ecp;

    // A two-register pipeline demonstrates simultaneous capture of OLD
    // upstream state. Input is changed between host edges, before setup.
    reg [7:0] data=8'h01;
    always @(negedge host) if(!reset && advance)
        data<={data[6:0],data[7]^data[5]^data[4]^data[3]};
    reg [7:0] first=0,second=0, ref_first=0,ref_second=0;
    always @(posedge host) begin
        if(reset) begin first<=0; second<=0; end
        else if(cr) begin first<=data; second<=first; end
    end
    always @(posedge ref_clock or posedge reset) begin
        if(reset) begin ref_first<=0; ref_second<=0; end
        else begin ref_first<=data; ref_second<=ref_first; end
    end

    // Actual legacy cell is the negative control: even with PRECAP it
    // transfers on the host edge AFTER slot_clock rises.
    wire legacy_bit;
    cell_MC10176 legacy(.sys_clk(host),.p9(slot_clock),.p5(data[0]),
        .p2(legacy_bit));
    integer legacy_late=0, captures=0, ticks=0, low_ticks=0, checks=0;
    integer rises_s=0,falls_s=0,rises_e=0,falls_e=0,rises_c=0,falls_c=0;
    reg [7:0] paused_phases=0;
    reg old_s,old_e,old_c, saved_sr,saved_sf,saved_er,saved_ef,saved_cr,saved_cf;
    reg [2:0] old_phase;
    reg was_advancing;
    always @(posedge host) begin
        old_s=scp; old_e=ecp; old_c=slot_clock; old_phase=phase;
        saved_sr=sr; saved_sf=sf; saved_er=er; saved_ef=ef;
        saved_cr=cr; saved_cf=cf; was_advancing=advance;
        #1; // Settle both original divider stages and the direct captures.
        if(!reset) begin
            if({scp,ecp,slot_clock} !== {ref_scp,ref_ecp,ref_clock})
                $fatal(1,"TICKS=%0d phase %d differs from original divider",TICKS,phase);
            if($isunknown({sr,sf,er,ef,cr,cf}))
                $fatal(1,"unknown enables");
            if({saved_sr,saved_sf,saved_er,saved_ef,saved_cr,saved_cf} !==
               {!old_s&&scp,old_s&&!scp,!old_e&&ecp,old_e&&!ecp,
                !old_c&&slot_clock,old_c&&!slot_clock})
                $fatal(1,"TICKS=%0d enables do not predict actual edges",TICKS);
            if({first,second} !== {ref_first,ref_second})
                $fatal(1,"TICKS=%0d direct capture violates pre-edge pipeline data",TICKS);
            if(!was_advancing && (phase!=old_phase || saved_cr || saved_cf ||
                                   saved_sr || saved_sf || saved_er || saved_ef))
                $fatal(1,"freeze advanced phase or transferred data");
            if(!was_advancing) paused_phases[phase]=1'b1;
            if(saved_cr) begin
                captures=captures+1;
                if(legacy_bit !== first[0]) legacy_late=legacy_late+1;
            end
            // Count enabled master ticks; pauses must stretch neither the
            // active phase fractions nor the number of captures per period.
            if(was_advancing) begin
                ticks=ticks+1;
                if(!slot_clock) low_ticks=low_ticks+1;
                rises_s=rises_s+int'(saved_sr); falls_s=falls_s+int'(saved_sf);
                rises_e=rises_e+int'(saved_er); falls_e=falls_e+int'(saved_ef);
                rises_c=rises_c+int'(saved_cr); falls_c=falls_c+int'(saved_cf);
                if(ticks==8*TICKS) begin
                    if(low_ticks!=3*TICKS || rises_s!=1 || falls_s!=1 ||
                       rises_e!=1 || falls_e!=1 || rises_c!=1 || falls_c!=1)
                        $fatal(1,"TICKS=%0d pulse width/edge count changed",TICKS);
                    ticks=0; low_ticks=0;
                    rises_s=0; falls_s=0; rises_e=0; falls_e=0; rises_c=0; falls_c=0;
                    checks=checks+1;
                end
            end
            if(checks>=24 && captures>=24 && legacy_late>=4 && paused_phases==8'hff) done=1;
        end
    end
    final begin
        if(!done) $fatal(1,"incomplete phase test TICKS=%0d checks=%0d captures=%0d legacy_late=%0d paused=%h",
            TICKS,checks,captures,legacy_late,paused_phases);
        $display("CLOCK_PHASES PASS ticks_per_slot=%0d periods=%0d captures=%0d legacy_late=%0d",
            TICKS,checks,captures,legacy_late);
    end
endmodule

module tb_clock_phases;
    reg host=0; always #5 host=~host;
    reg reset=1,advance=0;
    wire d1,d2,d3;
    clock_phases_case #(.TICKS(1)) one(host,reset,advance,d1);
    clock_phases_case #(.TICKS(2)) two(host,reset,advance,d2);
    clock_phases_case #(.TICKS(3)) three(host,reset,advance,d3);
    initial begin
        repeat(3) @(negedge host);
        reset=0; advance=1;
        // Stop at every possible subphase in all three instances. Pausing
        // while a clock is high must not create a fresh capture on resume.
        for(integer n=1;n<=48;n=n+1) begin
            repeat(n%7+1) @(negedge host);
            advance=0;
            repeat(n%5+1) @(negedge host);
            advance=1;
        end
        wait(d1&&d2&&d3);
        @(negedge host); advance=0;
        repeat(5) @(negedge host);
        $finish;
    end
    initial begin #100000; $fatal(1,"phase prototype timeout"); end
endmodule
`default_nettype wire
