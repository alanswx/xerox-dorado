// Same four-second host-service policy at different fabric frequencies.
// Hz are scaled down to keep the test small; real elapsed simulated time
// is checked, independently of the DUT's cycle counter or timeout constant.
`timescale 1ns/1ps
module pack_wallclock_case #(parameter integer HZ=333)(output reg done=0);
    reg clk=0;always #(500000000.0/HZ)clk=~clk;
    reg reset=1,ack=0;
    wire fault,rd,wr,ready;
    wire [31:0] detail,extra;
    realtime started,elapsed;
    localparam real PERIOD=1000000000.0/HZ;
    dorado_pack_hps #(.LABEL_WORDS(10),.INITIAL_CYL(3),.HOST_CLOCK_HZ(HZ)) d(
        .clk_sys(clk),.machine_clk(1'b0),.reset(reset),.attached(1'b1),.readonly(1'b0),.remounted(1'b0),
        .tagbus_n(12'hfff),.cyltag_n(1'b1),.headtag_n(1'b1),.drivetag_n(1'b1),.conttag_n(1'b1),
        .selected(1'b1),.data_adv(1'b0),.pulse(16'd0),.write_data_p(1'b1),.write_data_m(1'b0),
        .fault(fault),.fault_debug(detail),.fault_extra(extra),.ready(ready),.sd_rd(rd),.sd_wr(wr),
        .sd_ack(ack),.sd_buff_addr(13'd0),.sd_buff_dout(16'd0),.sd_buff_wr(1'b0));
    task fresh;
        @(negedge clk);reset=1;ack=0;
        repeat(2)@(negedge clk);reset=0;
        wait(rd);started=$realtime;
    endtask
    task deadline;
        wait(fault);elapsed=$realtime-started;
        if(elapsed<4000000000.0-PERIOD || elapsed>4000000000.0+PERIOD)
            $fatal(1,"host wall-time budget changed at%0dHz: %0.3fns",HZ,elapsed);
        if(ready || rd || wr || detail[31:28]!=8 || extra!=457)
            $fatal(1,"timeout fabricated completion/lost LBA: %h %h",detail,extra);
    endtask
    initial begin
        fresh();
        // A three-second ARM response must succeed at EVERY frequency.
        #3000000000;
        if(fault || !rd || ready)$fatal(1,"host budget shrank before3s service at%0dHz",HZ);
        @(negedge clk);ack=1;
        @(negedge clk);ack=0;
        repeat(3)@(negedge clk);
        if(fault || !ready || rd || wr)$fatal(1,"real host ACK did not complete read");
        fresh();deadline();
        if(detail[5:3]!=3 || detail[2:0]!=3'b100)
            $fatal(1,"waiting-for-ACK phase not preserved: %h",detail);
        ack=1;repeat(3)@(negedge clk);
        if(!fault || ready || detail[5:3]!=3 || extra!=457)$fatal(1,"first fault changed/cleared");
        fresh();
        @(negedge clk);ack=1;
        deadline();
        if(detail[5:3]!=4 || detail[2:0]!=3'b001)
            $fatal(1,"waiting-for-ACK-release phase not preserved: %h",detail);
        $display("PACK-WALLCLOCK PASS: %0dHz;3s host service succeeds,4s assert/release watchdogs, sticky first fault/no fabricated completion",HZ);
        done=1;
    end
endmodule
module tb_pack_wallclock;
    wire [2:0] done;
    pack_wallclock_case #(.HZ(333)) a(done[0]);
    pack_wallclock_case #(.HZ(400)) b(done[1]);
    pack_wallclock_case #(.HZ(500)) c(done[2]);
    initial begin wait(&done);$finish;end
    initial begin #15000000000;$fatal(1,"wall-clock test timed out");end
endmodule
