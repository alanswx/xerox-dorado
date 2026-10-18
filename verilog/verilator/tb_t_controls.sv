// Pin-level original LC control cone with an ideal positive-edge MC10176.
// Combinational cells retain their actual pin functions; clock capture here
// is event driven, independently of the candidate's explicit enables.
`timescale 1ns/1ps
`default_nettype none
module original_t_control #(parameter bit LOW_HALF=0) (
    input wire clock_pin,
    input wire [2:0] lc,
    input wire [7:0] ff,
    input wire curr_ne_next,
    output reg [5:0] controls
);
    wire lc1,lc1_n,ff075,lc_t_md,t_md,r_term,t_term,r_write_n,t_write_n;
    // h24d: LC.1 inversion/buffer. f20d: FC=5', FB=7', FA=0'.
    cell_MC10101 h24(.p13(lc[1]),.p9(lc1),.p15(lc1_n));
    cell_MC10100 f20(.p12(ff[2:0]!=3'd5),.p13(ff[5:3]!=3'd7),
        .p9(ff[7:6]!=2'd0),.p15(ff075));
    if(LOW_HALF) begin: low_board
        // ProcL h23a/b/c, g23a/b; d17 has RbSelMd' on p2, RbSelMd on p3.
        cell_MC10102 h23(.p4(lc1_n),.p5(lc[0]),.p2(r_term),
            .p6(lc[2]),.p7(lc1_n),.p3(t_term),
            .p10(lc[2]),.p11(r_term),.p14(r_write_n));
        cell_MC10102 g23(.p4(lc[0]),.p5(t_term),.p2(t_write_n),
            .p6(lc[2]),.p7(lc1_n),.p3(lc_t_md));
    end else begin: high_board
        // ProcH h23a/b/c, f18a/b; h22 has RbSelMd on p2, RbSelMd' on p3.
        cell_MC10102 h23(.p4(lc1_n),.p5(lc[0]),.p2(r_term),
            .p6(lc[2]),.p7(lc1_n),.p3(t_term),
            .p10(lc[2]),.p11(r_term),.p14(r_write_n));
        cell_MC10102 f18(.p4(lc[0]),.p5(t_term),.p2(t_write_n),
            .p6(lc[2]),.p7(lc1_n),.p3(lc_t_md));
    end
    assign t_md=ff075 | lc_t_md; // Original tied ECL outputs, explicit OR.
    always @(posedge clock_pin)
        controls <= {curr_ne_next,t_write_n,r_write_n,t_md,lc1,lc1_n};
endmodule

module tb_t_controls;
    reg clk=0; always #5 clk=~clk;
    reg capture_h=0,capture_l=0,clock_h=0,clock_l=0;
    reg [2:0] lc=0;
    reg [7:0] ff=0;
    reg curr_h=0,curr_l=0;
    wire [5:0] h,l,ref_h,ref_l;
    dorado_t_controls dut(.clk(clk),.capture_h(capture_h),.capture_l(capture_l),
        .lc(lc),.ff(ff),.curr_ne_next_h(curr_h),.curr_ne_next_l(curr_l),
        .controls_h(h),.controls_l(l));
    original_t_control #(.LOW_HALF(0)) high_ref(clock_h,lc,ff,curr_h,ref_h);
    original_t_control #(.LOW_HALF(1)) low_ref(clock_l,lc,ff,curr_l,ref_l);
    // A universal-enable mutation. This must be detected when the board
    // captures differ; it is not a valid shortcut for the two clock cones.
    wire [5:0] bad_h,bad_l;
    dorado_t_controls universal(.clk(clk),.capture_h(capture_h|capture_l),
        .capture_l(capture_h|capture_l),.lc(lc),.ff(ff),
        .curr_ne_next_h(curr_h),.curr_ne_next_l(curr_l),
        .controls_h(bad_h),.controls_l(bad_l));
    always @(posedge clk) begin clock_h=capture_h; clock_l=capture_l; end
    always @(negedge clk) begin clock_h=0; clock_l=0; end

    // Independent manual Table10 check for the LEGAL LC/FF combinations.
    // FF075 with LC other than5 is illegal (HM3.8), but the gate-cone
    // comparison still tests all of those electrical input combinations.
    function automatic [4:0] manual(input [2:0] load,input [7:0] func);
        reg rw,tw,tm;
        begin
            case(load)
            0: begin rw=0;tw=0;tm=0;end
            1: begin rw=0;tw=1;tm=0;end
            2: begin rw=1;tw=1;tm=1;end
            3: begin rw=0;tw=1;tm=1;end
            4: begin rw=1;tw=0;tm=0;end
            5: begin rw=1;tw=1;tm=(func==8'o075);end
            6: begin rw=1;tw=0;tm=0;end
            7: begin rw=1;tw=1;tm=0;end
            endcase
            manual={!tw,!rw,tm,load[1],!load[1]};
        end
    endfunction
    integer comparisons=0,manual_checks=0,universal_failures=0,held=0;
    reg [5:0] previous_h,previous_l;
    task check;
        if(h!==ref_h || l!==ref_l)
            $fatal(1,"T cone mismatch LC=%o FF=%o h=%b/%b l=%b/%b",lc,ff,h,ref_h,l,ref_l);
        if(ff!=8'o075 || lc==3'd5) begin
            if(capture_h && h[4:0]!==manual(lc,ff)) $fatal(1,"high half differs from HM3.8");
            if(capture_l && l[4:0]!==manual(lc,ff)) $fatal(1,"low half differs from HM3.8");
            manual_checks=manual_checks+1;
        end
        if(!capture_h && h!==previous_h) $fatal(1,"high changed without its capture");
        if(!capture_l && l!==previous_l) $fatal(1,"low changed without its capture");
        if(!capture_h && !capture_l) held=held+1;
        if(bad_h!==ref_h || bad_l!==ref_l) universal_failures=universal_failures+1;
        comparisons=comparisons+1;
    endtask
    initial begin
        // Establish defined state through real captures, not added resets.
        @(negedge clk); capture_h=1;capture_l=1;
        @(posedge clk); #1;
        previous_h=h;previous_l=l;
        for(integer load=0;load<8;load=load+1)
            for(integer func=0;func<256;func=func+1)
                for(integer edges=0;edges<4;edges=edges+1) begin
                    @(negedge clk);
                    lc=3'(load);ff=8'(func);
                    capture_h=edges[1];capture_l=edges[0];
                    curr_h=func[0]^edges[0];curr_l=func[1]^edges[1];
                    previous_h=h;previous_l=l;
                    @(posedge clk);#1;check;
                    // Inputs may change AFTER the physical capture. Neither
                    // control bank may pass those changes through its FFs.
                    #1;lc=~lc;ff=~ff;curr_h=~curr_h;curr_l=~curr_l;
                    #1;
                    if(h!==ref_h || l!==ref_l ||
                       (!capture_h && h!==previous_h) || (!capture_l && l!==previous_l))
                        $fatal(1,"late input passed through capture");
                end
        if(comparisons!=8192 || held!=2048 || universal_failures<100)
            $fatal(1,"incomplete coverage: %d held=%d bad=%d",comparisons,held,universal_failures);
        $display("T-CONTROLS PASS comparisons=%0d manual_checks=%0d held=%0d universal_enable_failures=%0d",
            comparisons,manual_checks,held,universal_failures);
        $finish;
    end
    initial begin #1000000;$fatal(1,"T controls timeout");end
endmodule
`default_nettype wire
