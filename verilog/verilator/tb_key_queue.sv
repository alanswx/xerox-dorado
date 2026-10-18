// Real matrix decoder behind a stoppable clock, fed from the host clock.
`timescale 1ns/1ps
`default_nettype none
module tb_key_queue;
    reg host=0; always #5 host=~host;
    reg enabled=1, reset=1, vsync_n=1;
    wire machine=host & enabled;
    reg [10:0] raw=0;
    wire [10:0] buffered;
    wire overflow;
    wire [3:0] queued;
    dorado_key_queue #(.ADDRESS_BITS(3)) q (
        .clk_host(host), .reset(reset), .ps2_key(raw), .vsync_n(vsync_n),
        .machine_key(buffered), .overflow(overflow), .queued(queued)
    );
    wire [15:0] w0,w1,w2,w3,old0;
    dorado_keyboard k(.sys_clk(machine),.reset(reset),.ps2_key(buffered),
        .key_word0(w0),.key_word1(w1),.key_word2(w2),.key_word3(w3));
    // Failure control: the original direct connection loses both toggles.
    dorado_keyboard old(.sys_clk(machine),.reset(reset),.ps2_key(raw),
        .key_word0(old0),.key_word1(),.key_word2(),.key_word3());
    task event_key(input [7:0] code, input bit down);
        @(negedge host); raw={~raw[10],down,1'b0,code};
        repeat(3) @(negedge host);
    endtask
    task field;
        @(negedge host); vsync_n=0;
        repeat(4) @(negedge host);
        vsync_n=1;
        repeat(4) @(negedge host);
    endtask
    task expect_matrix(input [15:0] a,b,c,d);
        if ({w0,w1,w2,w3} !== {a,b,c,d})
            $fatal(1,"matrix %h %h %h %h expected %h %h %h %h",w0,w1,w2,w3,a,b,c,d);
    endtask
    initial begin
        repeat(4) @(negedge host); reset=0;
        @(negedge host); enabled=0;
        event_key(8'h24,1); event_key(8'h24,0); // E down/up during disk wait
        repeat(50) @(negedge host);
        if(queued!=2 || buffered!=0) $fatal(1,"events not retained across stopped machine");
        @(negedge host); enabled=1;
        field; expect_matrix(16'hefff,16'hffff,16'hffff,16'hffff);
        if(old0!=16'hffff) $fatal(1,"direct failure control unexpectedly saw the E press");
        field; expect_matrix(16'hefff,16'hffff,16'hffff,16'hffff);
        field; expect_matrix(16'hffff,16'hffff,16'hffff,16'hffff);
        field;
        // Shift and G live in different matrix words; preserve chord order.
        @(negedge host); enabled=0;
        event_key(8'h12,1); event_key(8'h34,1);
        repeat(20) event_key(8'h34,1); // host autorepeat must not fill queue
        event_key(8'h34,0); event_key(8'h12,0);
        if(queued!=4 || overflow) $fatal(1,"chord/repeat queue count %d overflow %b",queued,overflow);
        @(negedge host); enabled=1;
        field; expect_matrix(16'hffff,16'hffff,16'hffbf,16'hffff);
        field; field; expect_matrix(16'hffff,16'hffff,16'hffbf,16'hdfff);
        field; field; expect_matrix(16'hffff,16'hffff,16'hffbf,16'hffff);
        field; field; expect_matrix(16'hffff,16'hffff,16'hffff,16'hffff);
        field;
        // A bounded queue must report saturation rather than wrap its count.
        @(negedge host); enabled=0;
        repeat(5) begin event_key(8'h24,1); event_key(8'h24,0); end
        if(queued!=8 || !overflow) $fatal(1,"overflow not detected count %d",queued);
        @(negedge host); enabled=1; reset=1;
        repeat(4) @(negedge host); reset=0;
        if(queued!=0 || overflow || buffered!=0) $fatal(1,"reset did not clear queue");
        $display("tb_key_queue: PASS -- stopped-clock loss control, press/release, chord ordering, field hold, repeat coalescing, overflow/reset");
        $finish;
    end
endmodule
`default_nettype wire
