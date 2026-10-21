`timescale 1ns/1ps
module tb_boot_fault_monitor;
    reg clk=0,error_n=0;
    reg [15:0] sources=0;
    reg [31:0] processor=0;
    wire [127:0] observed;
    dorado_boot_fault_monitor dut(.clk(clk),.error_n(error_n),
        .sources(sources),.processor(processor),.observation(observed));
    task tick;
        begin #5;clk=1;#5;clk=0;end
    endtask
    task check(input bit condition,input string description);
        if(!condition)$fatal(1,"BOOT-FAULT: %s",description);
    endtask
    initial begin
        tick();
        check(observed[127:112]==16'hd0e2 && observed[31:16]==16'hd0e1,"diagnostic markers");
        repeat(3)tick();
        check(observed[97:96]==0,"startup error must not arm");
        sources=16'hfc00;repeat(3)tick();
        check(observed[97:96]==0,"all enables without a good Error' sample must not arm");
        error_n=1;sources=16'hfc04;processor=32'h30000001;tick();
        check(observed[97:96]==2,"good enabled sample arms capture");
        sources=16'hfc08;processor=32'h30000002;tick();
        error_n=0;sources=16'hfc21;processor=32'h30cd2d44;tick();
        check(observed[97:96]==3,"first error retained");
        check(observed[63:32]==32'h30cd2d44,"same-edge CPU address");
        check(observed[95:64]==32'hfc08fc21,"first and immediately preceding sources");
        for(integer n=0;n<1000;n=n+1)begin
            error_n=n[0];sources=16'(n)^16'hffff;processor=32'(n);tick();
            check(observed[63:32]==32'h30cd2d44 && observed[95:64]==32'hfc08fc21,"later activity cannot overwrite first fault");
            check(observed[15:0]==sources,"live page remains current");
        end
        $display("BOOT-FAULT PASS: startup gating, enable/good arming, same-edge first fault, previous pins and1000 retained/live observations");
        $finish;
    end
endmodule
